#!/usr/bin/env python3
"""Matched native-label10 disk fits with the actual falling-edge ALTCLKCTRL.

Both variants expose every original I/O through identical registered launch/
capture shells, use50MHz,2ns/0ns virtual I/O and identical fitter policy.
All setup/hold paths stay timed; no false paths.
Only the18-bit cache-tag phase staging differs. No assembly or deployment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import xml.etree.ElementTree as ET


def sha(raw):return hashlib.sha256(raw).hexdigest()


def ports(xml):
    root=ET.fromstring(xml);module=root.find('.//module');types={t.get('id'):t for t in root.findall('.//typetable/*')}
    result=[]
    for var in module.findall('./var'):
        if not var.get('dir'):continue
        t=types[var.get('dtype_id')];assert t.tag=='basicdtype'and t.get('name')=='logic'
        width=abs(int(t.get('left','0'))-int(t.get('right','0')))+1
        result.append(dict(name=var.get('name'),direction=var.get('dir'),width=width))
    assert len(result)==36
    return result


def top(portlist):
    declarations=[' input wire extra_ready']
    for p in portlist:
        if p['name']=='machine_clk':continue
        width=f" [{p['width']-1}:0]"if p['width']>1 else''
        declarations.append(f" {p['direction']} wire{width} {p['name']}")
    source='module snapshot_fit(\n'+',\n'.join(declarations)+');\n'
    for p in [dict(name='extra_ready',direction='input',width=1),*portlist]:
        if p['name']in ('clk_sys','machine_clk'):continue
        name=p['name'];width=f" [{p['width']-1}:0]"if p['width']>1 else''
        if p['direction']=='input':
            source+=f''' reg{width} {name}_stage,{name}_launch;
 always @(negedge clk_sys){name}_stage<={name};
 always @(posedge clk_sys){name}_launch<={name}_stage;
'''
        else:
            source+=f''' wire{width} {name}_raw;
 reg{width} {name}_capture;
 always @(posedge clk_sys){name}_capture<={name}_raw;
 assign {name}={name}_capture;
'''
    source+=''' wire guest_clock;
 altclkctrl #(.clock_type("Global Clock"),.ena_register_mode("falling edge"),.intended_device_family("Cyclone V")) machine_clock(
 .inclk({3'b000,clk_sys}),.clkselect(2'b00),.ena(!reset_launch && ready_raw && extra_ready_launch),.outclk(guest_clock));
 dorado_pack_hps #(.LABEL_WORDS(10),.HOST_CLOCK_HZ(50000000)) u_medium(
'''
    connection=lambda p:'guest_clock'if p['name']=='machine_clk'else'clk_sys'if p['name']=='clk_sys'else p['name']+('_launch'if p['direction']=='input'else'_raw')
    source+=',\n'.join('.'+p['name']+'('+connection(p)+')'for p in portlist)
    return source+');\nendmodule\n'


def prepare(out,inventory):
    import generate_disk_cache_phase as gen
    import audit_disk_cache_phase as source_audit
    from fit_f10415 import AUDIT
    original=gen.reference().encode();candidate=gen.transform(original.decode()).encode()
    source_audit.audit(candidate,gen.ROOT/'fpga/mister/reports')
    xml=(inventory/'original/disk.xml').read_bytes();portlist=ports(xml);shell=top(portlist)
    sdc='''create_clock -name host_clock -period 20.000 [get_ports clk_sys]
derive_clock_uncertainty
set_input_delay -clock host_clock -max 2.000 [remove_from_collection [all_inputs] [get_ports clk_sys]]
set_input_delay -clock host_clock -min 0.000 [remove_from_collection [all_inputs] [get_ports clk_sys]]
set_output_delay -clock host_clock -max 2.000 [all_outputs]
set_output_delay -clock host_clock -min 0.000 [all_outputs]
'''
    qsf='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS OFF
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name GENERATE_RBF_FILE OFF
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE medium.sv
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to clk_sys
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to clk_sys
set_instance_assignment -name VIRTUAL_PIN ON -to extra_ready
'''
    for p in portlist:
        if p['name']in ('clk_sys','machine_clk'):continue
        name=p['name']+('[*]'if p['width']>1 else'');qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+name+'\n'
    out.mkdir(parents=True,exist_ok=False)
    for label,raw in [('original',original),('candidate',candidate)]:
        case=out/label;case.mkdir();(case/'medium.sv').write_bytes(raw)
        for name,text in {'top.sv':shell,'bank.sdc':sdc,'snapshot_fit.qsf':qsf,'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n','audit.tcl':AUDIT}.items():(case/name).write_text(text)
    # Syntax/binding preflight only. Quartus uses the real dedicated primitive;
    # this clock stub is never listed in either QSF or used as timing evidence.
    lint=out/'lint';lint.mkdir();stub=lint/'altclkctrl.sv'
    stub.write_text('''module altclkctrl #(
 parameter clock_type="Global Clock",ena_register_mode="falling edge",intended_device_family="Cyclone V")(
 input wire [3:0] inclk,input wire [1:0] clkselect,input wire ena,output wire outclk);
 assign outclk=inclk[0];endmodule
''')
    for label in ('original','candidate'):
        command=['verilator','--lint-only','--timing','--assert','--top-module','snapshot_fit','-Wno-fatal',
                 str(out/label/'top.sv'),str(out/label/'medium.sv'),str(stub)]
        (lint/(label+'-command.json')).write_text(json.dumps(command,indent=2)+'\n')
        with(lint/(label+'.log')).open('w')as log:subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,check=True)
    inputs=out/'executed-sources';inputs.mkdir()
    for p in (Path(__file__),Path(gen.__file__),Path(source_audit.__file__)):(inputs/p.name).write_bytes(p.read_bytes())
    (inputs/'original.xml').write_bytes(xml)
    record=dict(scope=__doc__,builder_sha256=sha(Path(__file__).read_bytes()),ports=portlist,virtual_pin_bits=sum(p['width']for p in portlist)-1,
                registered_embedded_io=True,
                compiled_files={str(p.relative_to(out)):sha(p.read_bytes())for p in out.rglob('*')if p.is_file()},
                expected_top=shell,expected_sdc=sdc,expected_qsf=qsf,expected_audit=AUDIT,
                original_sha256=sha(original),candidate_sha256=sha(candidate),assembly_permitted=False,
                limits='Matched embedded disk component only; physical board IO, whole-core clocks/timing and full snapshots remain separate.')
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n');print('Prepared matched disk phase experiment;',record['virtual_pin_bits'],'virtual pins',flush=True)


def run(out,quartus):
    record=json.loads((out/'provenance.json').read_text());assert not record['assembly_permitted']
    assert record['builder_sha256']==sha(Path(__file__).read_bytes())
    for name,digest in record['compiled_files'].items():assert sha((out/name).read_bytes())==digest,name
    for label in ('original','candidate'):
        case=out/label;assert not(case/'map.log').exists(),'observe the existing run; do not restart'
        for tool,args,stage in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(label,stage,flush=True)
            with(case/(stage+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Matched disk phase terminal; independent source/capacity/timing review required; no assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--prepare',type=Path);p.add_argument('--inventory',type=Path);p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.prepare:prepare(a.prepare.resolve(),a.inventory.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --prepare or --run')
