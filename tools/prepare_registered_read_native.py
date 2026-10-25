#!/usr/bin/env python3
"""Prepare original-ROM native startup with the actual registered read caller.

Reuse the existing complete physical-SDRAM/HPS/Pup harness and every original
publication/pause assertion. Replace only the source-bound staged machine,
clock macro and passive buffer checks. Guest writes remain in the host's
private vector; no medium, baseline run, input or existing limit is changed.
This bounded startup diagnostic is not Cedar/60ns/timing qualification.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

MONITOR='''
  // Passive checks of the actual native registered physical read boundary.
  longint unsigned registered_read_matches=0,registered_read_waits=0;
  always @(posedge host_clk)if(!reset)begin
    if(dut.m.r_front_dispatch.buffer_ready)begin
      if(dut.m.r_front_dispatch.physical_read!==dut.m.r_front_dispatch.asynchronous_read)
        $fatal(1,"NATIVE_REGISTERED_READ stale matching response host=%0d",host_ticks);
      registered_read_matches++;
    end else if(storage_initialized)registered_read_waits++;
    if(dut.m.r_capture_commit && !dut.m.r_front_dispatch.stage_active &&
       !dut.m.r_front_dispatch.buffer_ready)
      $fatal(1,"NATIVE_REGISTERED_READ unready publication host=%0d",host_ticks);
  end
  always @(negedge host_clk)if(!reset && host_ticks+2>=limit)
    $display("NATIVE_REGISTERED_READ matches=%0d waits=%0d guest=%0d",
      registered_read_matches,registered_read_waits,guest_ticks);
'''


def memc_monitor(manifest):
    if not manifest.get('registered_MemC_tag_reads_connected'):return ''
    packages=manifest['MemC_tag_read_packages'];assert len(packages)==21
    text='''
  always @(posedge host_clk)if(!reset)begin
    if(step && !dut.m.memc_reads_ready)
      $fatal(1,"NATIVE_MEMC_READ unready machine publication host=%0d",host_ticks);
'''
    for pkg in packages:
        cell='dut.m.b_MemC.u_'+pkg
        text+=f'''    if({cell}.read_ready && {{{cell}.p20,{cell}.p21,{cell}.p22,{cell}.p23}} !==
        ({cell}.sel ? {cell}.state_read:4'b0))
      $fatal(1,"NATIVE_MEMC_READ stale ready SRAM {pkg} host=%0d",host_ticks);
'''
    return text+'  end\n'


def prepare(stage,baseline,out,limit):
    assert not out.exists()and 200000000<=limit<=5000000000
    s=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,s['source_hashes'])
    assert s['registered_R_read_buffer_connected']and s['single_SRAM_owner_and_write_edge_unchanged']
    assert s['actual_native_single_comparison_connected']and s['frequency_hz']==66666667 and s['sysper']==4
    assert not s['assembly_permitted']
    p=json.loads(gzip.decompress((baseline/'provenance.json.gz').read_bytes()));verify(baseline/'inputs',p['inputs'])
    assert p['normal_baseboard_boot_presses']==3 and p['skip_disk_wait_jumper_fitted']
    medium=Path(p['artifacts']['medium']['path'])
    assert sha(medium)=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    out.mkdir(parents=True);inputs=out/'inputs';shutil.copytree(baseline/'inputs',inputs)
    for n in s['source_hashes']:
        q=inputs/n;q.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(stage/n,q)
    for n,h in p['inputs'].items():
        if '/proms/eprom/'in n:assert sha(inputs/n)==h,n
    tb=inputs/'harness/tb_native_cedar.sv';raw=tb.read_text()
    assert 'NATIVE_CP late readiness change after falling-edge gate'in raw
    assert 'NATIVE_CP held request changed'in raw and 'NATIVE_CP held response changed'in raw
    assert 'NATIVE_CP board advanced while control paused'in raw
    assert '.SYSPER(4)'in raw and '.SYSPER(2)'not in raw
    assert 'dut.m.r_front_dispatch.files.advance'in raw and raw.count('endmodule')==1
    extra=MONITOR+memc_monitor(s)
    actual=raw.replace('endmodule',extra+'\nendmodule',1)
    assert actual.replace(extra+'\n','',1)==raw
    tb.write_text(actual)
    host=(inputs/'harness/host.cpp').read_text()
    assert 'fopen(medium,"rb")'in host and 'pack[word]=uint16_t(value);'in host
    assert 'fwrite('not in host and 'ofstream'not in host
    macros=re.findall(r'VERILOG_MACRO\s+"(DORADO_HOST_CLOCK_HZ=\d+)"',(stage/'core/Dorado.qsf').read_text())
    assert macros==['DORADO_HOST_CLOCK_HZ=66666667']
    definitions=[d for d in p['defines']if not d.startswith('DORADO_HOST_CLOCK_HZ=')]+macros
    assert len(definitions)==len(p['defines'])
    used=list(p['used_hdl'])
    for n in s['extra_RTL_sources']:
        if n not in used:used.append(n)
    shutil.copy2(Path(__file__),inputs/Path(__file__).name)
    (out/'host-cycle-limit.txt').write_text(str(limit)+'\n');(out/'window-control.txt').write_text('0 2\n')
    record={**p,'status':'PREPARED_REGISTERED_READ_ORIGINAL_ROM_STARTUP','scope':__doc__,
        'stage':str(stage),'stage_manifest_name':'r-front-control-native.json',
        'stage_manifest_sha256':sha(stage/'r-front-control-native.json'),
        'baseline_run':str(baseline),'inputs':{str(q.relative_to(inputs)):sha(q)for q in inputs.rglob('*')if q.is_file()},
        'used_hdl':used,'defines':definitions,'host_clock_hz':66666667,'host_limit':limit,
        'window_control_path':str(out/'window-control.txt'),'window_initial_control':'0 2\n',
        'every_original_harness_assertion_and_boot_input_retained':True,
        'single_SRAM_owner_and_unchanged_write_edge':True,'registered_read_passive_monitor':True,
        'registered_MemC_tag_read_passive_monitor':bool(s.get('registered_MemC_tag_reads_connected')),
        'baseline_inputs_and_limits_unchanged':True,'full60ns_event_batch_connected':False,
        'firmware_qualified':False,'hardware_changed':False,'full_goal_active':True}
    (out/'provenance.json.gz').write_bytes(gzip.compress((json.dumps(record,indent=2)+'\n').encode(),mtime=0))
    print(record['status'],len(used),'HDL sources; fixed host limit',limit,flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','baseline','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--limit',type=int,default=300000000)
    a=p.parse_args();prepare(a.stage.resolve(),a.baseline.resolve(),a.output.resolve(),a.limit)
