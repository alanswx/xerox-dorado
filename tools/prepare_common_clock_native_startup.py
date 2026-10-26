#!/usr/bin/env python3
"""Freeze original-ROM native startup on the experimental common host clock.

Keep the physical SDRAM, HPS/Pup services, boot-button input and all original
state/data assertions. Retarget observers to the accepted rising-edge step;
the physical clock level no longer represents guest permission. Additionally
check every converted scalar/vector state while paused. Arrays retain the
original RAM checks and separate BaseBoard/storage comparison coverage.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil
import xml.etree.ElementTree as ET

from prepare_registered_read_native import MONITOR,memc_monitor,operand_monitor
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def observer(inventory,xml_source):
    inv=json.loads((inventory/'results.json').read_text());xml=ET.parse(xml_source/'native.xml').getroot()
    assert inv['native_XML_sha256']==sha(xml_source/'native.xml')
    modules={m.get('name'):m for m in xml.findall('.//module')}
    types={t.get('id'):t for t in xml.findall('.//typetable/*')}
    root=inv['native_root_path'];seen=set();signals=[];arrays=[];total_bits=0
    def width(dtype):
        if dtype.tag=='basicdtype':
            if dtype.get('left')is not None:return abs(int(dtype.get('left'))-int(dtype.get('right')))+1
            return {'byte':8,'shortint':16,'integer':32,'int':32,'longint':64,'time':64}.get(dtype.get('name'),1)
        assert dtype.tag=='packarraydtype',dtype.attrib
        dims=[int(c.get('name').split('h')[1],16)for c in dtype.findall('range/const')]
        assert len(dims)==2
        return (abs(dims[0]-dims[1])+1)*width(types[dtype.get('sub_dtype_id')])
    for process in inv['processes']:
        if process['classification']!='VIRTUAL_CLOCK_ENABLE_REQUIRED':continue
        module=modules[process['module']];parents={c:p for p in module.iter()for c in p}
        for name in process['written_variables']:
            key=process['path'],name
            if key in seen:continue
            seen.add(key)
            variables=[v for v in module.findall('.//var')if v.get('name')==name];assert len(variables)==1,key
            var=variables[0];scope=[];cursor=var
            while cursor in parents:
                cursor=parents[cursor]
                if cursor.tag in ['begin','genblock']and cursor.get('name'):scope.insert(0,cursor.get('name'))
            path='dut'+process['path'][len(root):]+'.'+'.'.join(scope+[name])
            dtype=types[var.get('dtype_id')]
            if dtype.tag=='unpackarraydtype':arrays.append(path)
            else:
                assert dtype.tag in ['basicdtype','packarraydtype'],(path,dtype.tag)
                signals.append(path);total_bits+=width(dtype)
    assert len(signals)==2785 and len(arrays)==38
    image='{'+',\n'.join(signals)+'}'
    extra=f'''
// Passive snapshot of EVERY converted scalar/vector state. The HPS buffer
// is dual-clock RAM; it must keep accepting host writes while guest-paused.
localparam integer CE_PAUSED_STATE_BITS={total_bits};
wire [CE_PAUSED_STATE_BITS-1:0] ce_guest_image={image};
initial if($bits({image})!=CE_PAUSED_STATE_BITS)
 $fatal(1,"NATIVE_CE inventory/state-image width mismatch");
reg [CE_PAUSED_STATE_BITS-1:0] ce_guest_before;
longint unsigned ce_frozen_checks=0,ce_accepted_edges=0;
always @(posedge host_clk)begin
 if(step !== (!reset && pack_ready && storage_ready && control_ready))
  $fatal(1,"NATIVE_CE rising-edge grant differs from whole-machine readiness");
 if(step)ce_accepted_edges++;
 if(!reset && !step)begin
  ce_guest_before=ce_guest_image;
  #1;
  if(ce_guest_image!==ce_guest_before)
   $fatal(1,"NATIVE_CE converted guest state changed while paused host=%0d",host_ticks);
  ce_frozen_checks++;
 end
end
final begin
 if(guest_ticks!=ce_accepted_edges || cp_native_commits!=ce_accepted_edges)
  $fatal(1,"NATIVE_CE accepted edge/counter mismatch guest=%0d accepted=%0d commits=%0d",guest_ticks,ce_accepted_edges,cp_native_commits);
 $display("NATIVE_CE_PAUSE_CHECKS vectors={len(signals)} bits=%0d paused_checks=%0d accepted_edges=%0d",CE_PAUSED_STATE_BITS,ce_frozen_checks,ce_accepted_edges);
end
'''
    return extra,signals,arrays


def retime(raw):
    changes=[
      ('dorado_native_machine #','dorado_native_machine_ce #'),
      ('reg step=0;always @(negedge host_clk)step<=source_enable;',
       'wire step=source_enable;\n  reg accepted_step=0;always @(posedge host_clk)accepted_step<=step;'),
      ('wire machine_clk=host_clk && step;','wire machine_clk=host_clk;'),
      ('always @(posedge machine_clk)if(boot_samples<boot_end_samples)',
       'always @(posedge machine_clk)if(step && boot_samples<boot_end_samples)'),
      ('host_ticks<=host_ticks+1;\n    if(step)begin','host_ticks<=host_ticks+1;\n    if(accepted_step)begin'),
      ('always @(posedge machine_clk) begin\n if(cp_observe_clock',
       'always @(posedge machine_clk) if(step) begin\n if(cp_observe_clock'),
      ('always @(negedge host_clk)if(!reset && step && control_ready)',
       'always @(negedge host_clk)if(!reset && accepted_step && control_ready)'),
      ('always @(negedge host_clk)if(step && !reset)',
       'always @(negedge host_clk)if(accepted_step && !reset)'),
      ('always @(negedge host_clk)if(!reset)begin\n    if(step)begin',
       'always @(negedge host_clk)if(!reset)begin\n    if(accepted_step)begin'),
      ('NATIVE_CP late readiness change after falling-edge gate',
       'NATIVE_CP unready rising-edge publication'),
      ('if(step || machine_clk || {dut.m.b_ProcH.state_guest_advance',
       'if(step || {dut.m.b_ProcH.state_guest_advance')]
    original=raw
    for a,b in changes:
        assert raw.count(a)==1,a
        raw=raw.replace(a,b,1)
    reversed_text=raw
    for a,b in reversed(changes):reversed_text=reversed_text.replace(b,a,1)
    assert reversed_text==original
    assert raw.count('$fatal(')==original.count('$fatal(')
    return raw,changes


def prepare(stage,baseline,inventory,xml_source,audit,out,limit):
    assert not out.exists()and 300000000<=limit<=5000000000
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    assert m['clock_enable_conversion_staged']and m['frequency_hz']==66666667 and m['sysper']==4
    assert not m['assembly_permitted']
    a=json.loads((audit/'results.json').read_text());assert a['status']=='PASS_NATIVE_COMMON_CLOCK_ENABLE_STRUCTURAL_AUDIT'
    assert a['stage_manifest_sha256']==sha(stage/'r-front-control-native.json')
    assert sha(inventory/'results.json')==m['clock_enable_inventory_sha256']
    p=json.loads(gzip.decompress((baseline/'provenance.json.gz').read_bytes()));verify(baseline/'inputs',p['inputs'])
    assert p['normal_baseboard_boot_presses']==3 and p['skip_disk_wait_jumper_fitted']
    medium=Path(p['artifacts']['medium']['path'])
    assert sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    out.mkdir(parents=True);inputs=out/'inputs';shutil.copytree(baseline/'inputs',inputs)
    for name in m['source_hashes']:
        dest=inputs/name;dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(stage/name,dest)
    tb=inputs/'harness/tb_native_cedar.sv';original=tb.read_text();raw,changes=retime(original)
    extra,signals,arrays=observer(inventory,xml_source)
    checks=MONITOR+memc_monitor(m)+operand_monitor(m)+extra
    assert raw.count('endmodule')==1;raw=raw.replace('endmodule',checks+'\nendmodule',1);tb.write_text(raw)
    # Originals, memory model and all host services remain exact.
    for name,h in p['inputs'].items():
        if '/proms/eprom/'in name or name in ['harness/host.cpp','harness/xsds_model.sv','host/ethernet.c','host/io.c']:
            assert sha(inputs/name)==h,name
    host=(inputs/'harness/host.cpp').read_text();assert 'fopen(medium,"rb")'in host and 'fwrite('not in host
    macros=re.findall(r'VERILOG_MACRO\s+"(DORADO_HOST_CLOCK_HZ=\d+)"',(stage/'core/Dorado.qsf').read_text())
    assert macros==['DORADO_HOST_CLOCK_HZ=66666667']
    defines=[d for d in p['defines']if not d.startswith('DORADO_HOST_CLOCK_HZ=')]+macros
    used=list(p['used_hdl'])
    for name in m['extra_RTL_sources']:
        if name not in used:used.append(name)
    shutil.copy2(Path(__file__),inputs/Path(__file__).name)
    (out/'host-cycle-limit.txt').write_text(str(limit)+'\n');(out/'window-control.txt').write_text('0 2\n')
    record={**p,'status':'PREPARED_COMMON_CLOCK_ORIGINAL_ROM_NATIVE_STARTUP','scope':__doc__,
        'stage':str(stage),'stage_manifest_name':'r-front-control-native.json','stage_manifest_sha256':sha(stage/'r-front-control-native.json'),
        'baseline_run':str(baseline),'clock_enable_audit_sha256':sha(audit/'results.json'),
        'original_harness_sha256':p['inputs']['harness/tb_native_cedar.sv'],
        'accepted_rising_edge_observers':True,'harness_reversible_retiming_edits':changes,
        'old_harness_fatal_assertions_retained':original.count('$fatal('),
        'pause_checked_scalar_vectors':signals,'array_scope_not_in_scalar_image':arrays,
        'inputs':{str(q.relative_to(inputs)):sha(q)for q in inputs.rglob('*')if q.is_file()},
        'used_hdl':used,'defines':defines,'host_clock_hz':66666667,'host_limit':limit,
        'window_control_path':str(out/'window-control.txt'),'window_initial_control':'0 2\n',
        'original_ROM_host_services_physical_SDRAM_and_boot_input_unchanged':True,
        'every_old_data_state_and_device_assertion_retained':True,
        'original120ns_phase_schedule_unchanged':True,'full60ns_event_batch_connected':False,
        'firmware_qualified':False,'hardware_changed':False,'full_goal_active':True}
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(record,indent=2)+'\n').encode(),mtime=0))
    print(record['status'],len(used),'HDL',len(signals),'paused vectors; fixed host bound',limit,flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','baseline','inventory','xml-source','audit','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--limit',type=int,default=1500000000)
    a=p.parse_args();prepare(a.stage.resolve(),a.baseline.resolve(),a.inventory.resolve(),a.xml_source.resolve(),a.audit.resolve(),a.output.resolve(),a.limit)
