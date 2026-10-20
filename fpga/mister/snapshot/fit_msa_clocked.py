#!/usr/bin/env python3
"""Matched complete-MSA area fits with actual ALTCLKCTRL falling-edge gating.

Both variants retain all 2690 physical bits and the same registered50MHz shell.
Raw guest enable is available before the falling edge; actual guest advance is
latched at that edge. Controllers/frontends stay on host_clk. No fabric AND
clock, clock-period relaxation, timing exception, assembly or deployment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(out):
    import generate_msa_clocked as gen
    import test_msa_board as test
    from fit_f10415 import AUDIT
    from fit_f10415_enable import SDC
    clocked,manifest,helpers=gen.generate()
    combinational={name:raw for name,raw in gen.base.original_cells().items()if name[:-2]not in gen.base.native.FIELDS}
    baseline=clocked.replace(".state_host_clk(state_ff_clk),.state_advance(1'b1),.state_hold(state_hold)",
                             '.state_host_clk(storage_clk),.state_advance(state_guest_advance),.state_hold(state_hold)')
    assert baseline.count('.state_host_clk(storage_clk),.state_advance(state_guest_advance)')==78
    regular=[p for p in test.ports()if p['name']!='sys_clk'and not p['name'].startswith('storage_')]
    inputs=[p for p in regular if p['direction']=='input'];outputs=[p for p in regular if p['direction']=='output']
    ni=sum(p['width']for p in inputs);no=sum(p['width']for p in outputs);launch_bits=339+ni;output_bits=586+no
    top=f'''module snapshot_fit(
 input wire host_clk,advance,freeze,held,global_hold,global_abort,system_reset,request,write,reset,
 input wire [23:0] index,input wire [17:0] data,input wire [{ni-1}:0] pins,
 input wire [15:0] req_ready,rsp_valid,input wire [255:0] rsp_data,
 output reg [{output_bits-1}:0] outputs);
 reg [{launch_bits-1}:0] input_stage,launch;
 always @(negedge host_clk)input_stage<={{advance,freeze,held,global_hold,global_abort,system_reset,request,write,reset,index,data,pins,req_ready,rsp_valid,rsp_data}};
 always @(posedge host_clk)launch<=input_stage;
 wire advance_r,freeze_r,held_r,global_hold_r,global_abort_r,system_reset_r,request_r,write_r,reset_r;
 wire [23:0] index_r;wire [17:0] data_r;wire [{ni-1}:0] pins_r;
 wire [15:0] req_ready_r,rsp_valid_r;wire [255:0] rsp_data_r;
 assign {{advance_r,freeze_r,held_r,global_hold_r,global_abort_r,system_reset_r,request_r,write_r,reset_r,index_r,data_r,pins_r,req_ready_r,rsp_valid_r,rsp_data_r}}=launch;
 reg actual_advance=0;
 always @(negedge host_clk)actual_advance<=advance_r;
 wire ff_clock,ff_clock_request;
 altclkctrl #(.clock_type("Global Clock"),.ena_register_mode("falling edge"),.intended_device_family("Cyclone V")) ff_clock_control(
 .inclk({{3'b000,host_clk}}),.clkselect(2'b00),.ena(advance_r || ff_clock_request),.outclk(ff_clock));
 wire storage_ready,ready,frozen,done,error,hold_required,fault,drained;wire [17:0] result;
 wire [15:0] req_valid,req_write,rsp_ready;wire [255:0] req_addr,req_data;
'''
    for p in outputs:
        assert p['width']==1;top+=' wire normal_'+p['name']+';\n'
    con=['.sys_clk(host_clk)','.storage_clk(host_clk)','.storage_reset(reset_r)','.storage_ready(storage_ready)',
         '.storage_req_ready(req_ready_r)','.storage_rsp_valid(rsp_valid_r)','.storage_rsp_data(rsp_data_r)',
         '.storage_req_valid(req_valid)','.storage_req_write(req_write)','.storage_rsp_ready(rsp_ready)',
         '.storage_req_addr(req_addr)','.storage_req_data(req_data)',
         '.state_ff_clk(ff_clock)','.state_ff_clock_request(ff_clock_request)',
         '.state_guest_advance(actual_advance)','.state_freeze(freeze_r)','.state_guest_held(held_r)',
         '.state_global_hold(global_hold_r)','.state_global_abort(global_abort_r)','.state_system_reset(system_reset_r)',
         '.state_request(request_r)','.state_write(write_r)','.state_index(index_r)','.state_data(data_r)',
         '.state_ready(ready)','.state_frozen(frozen)','.state_done(done)','.state_error(error)',
         '.state_scan_hold_required(hold_required)','.state_scan_fault(fault)','.state_memory_drained(drained)',
         '.state_result(result)']
    offset=0;out_names={p['name']for p in outputs}
    for p in inputs:
        val=f'pins_r[{offset}]'if p['width']==1 else f'pins_r[{offset}+:{p["width"]}]'
        if p['name']+'__drv'in out_names:val+=' | normal_'+p['name']+'__drv'
        con.append('.'+p['name']+'('+val+')');offset+=p['width']
    con+=['.'+p['name']+'(normal_'+p['name']+')'for p in outputs]
    top+=' dorado_serial_MSA #(.SYSPER(4)) board(\n'+',\n'.join(con)+'\n);\n'
    normal='{'+','.join('normal_'+p['name']for p in reversed(outputs))+'}'
    top+=' always @(posedge host_clk)outputs<={'+normal+',storage_ready,ready,frozen,done,error,hold_required,fault,drained,result,req_valid,req_write,rsp_ready,req_addr,req_data};\nendmodule\n'
    qsf='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS OFF
set_global_assignment -name OPTIMIZATION_MODE "Aggressive Area"
set_global_assignment -name OPTIMIZATION_TECHNIQUE AREA
set_global_assignment -name QII_AUTO_PACKED_REGISTERS "Minimize Area with Chains"
set_global_assignment -name PHYSICAL_SYNTHESIS_COMBO_LOGIC OFF
set_global_assignment -name PHYSICAL_SYNTHESIS_REGISTER_DUPLICATION OFF
set_global_assignment -name PHYSICAL_SYNTHESIS_REGISTER_RETIMING OFF
set_global_assignment -name PHYSICAL_SYNTHESIS_COMBO_LOGIC_FOR_AREA ON
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name GENERATE_RBF_FILE OFF
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE msa_board.sv
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
    for name in helpers:qsf+='set_global_assignment -name SYSTEMVERILOG_FILE '+name+'\n'
    for name in combinational:qsf+='set_global_assignment -name VERILOG_FILE original_'+name+'\n'
    for port in ('advance','freeze','held','global_hold','global_abort','system_reset','request','write','reset','index[*]','data[*]','pins[*]','req_ready[*]','rsp_valid[*]','rsp_data[*]','outputs[*]'):
        qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
    out.mkdir(parents=True,exist_ok=False)
    for label,source in [('baseline',baseline),('clocked',clocked)]:
        case=out/label;case.mkdir()
        for name,body in {'msa_board.sv':source,**helpers,'top.sv':top,'bank.sdc':SDC,'audit.tcl':AUDIT,
                          'snapshot_fit.qsf':qsf,'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n'}.items():(case/name).write_text(body)
        for name,raw in combinational.items():(case/('original_'+name)).write_bytes(raw)
    executed=out/'executed-sources';executed.mkdir()
    for p in (Path(__file__).resolve(),Path(gen.__file__),Path(test.__file__),gen.base.ROOT/'fpga/mister/snapshot/fit_f10415.py',gen.base.ROOT/'fpga/mister/snapshot/fit_f10415_enable.py'):shutil.copyfile(p,executed/p.name)
    record=dict(scope=__doc__,builder_sha256=sha(Path(__file__)),manifest=manifest,
                compiled_files={str(p.relative_to(out)):sha(p)for p in out.rglob('*')if p.is_file()},
                expected_qsf=qsf,expected_top=top,expected_sdc=SDC,expected_audit=AUDIT,
                input_bits=launch_bits,output_bits=output_bits,normal_input_bits=ni,normal_output_bits=no,
                combinational_source_hashes={name:hashlib.sha256(raw).hexdigest()for name,raw in combinational.items()},
                limits='Matched component only. Gate phase is hardware state requiring future barrier handling; no native/full snapshot or speed claim.')
    (out/'provenance.json').write_text(json.dumps(record,indent=2)+'\n');print('Prepared complete MSA matched clock-control experiment',launch_bits,output_bits,flush=True)


def run(out,quartus):
    record=json.loads((out/'provenance.json').read_text());assert sha(Path(__file__))==record['builder_sha256']
    for name,digest in record['compiled_files'].items():assert sha(out/name)==digest,'prepared source differs: '+name
    for label in ('baseline','clocked'):
        case=out/label;assert not(case/'map.log').exists(),'observe existing build; do not restart'
        for tool,args,stage in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(label,stage,flush=True)
            with(case/(stage+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Matched MSA clock-control run terminal; independent timing/area audit required; no assembly.',flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--prepare',type=Path);p.add_argument('--run',type=Path);p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.prepare:prepare(a.prepare.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --prepare or --run')
