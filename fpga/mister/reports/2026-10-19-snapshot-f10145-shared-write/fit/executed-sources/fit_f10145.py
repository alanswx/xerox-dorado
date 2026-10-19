#!/usr/bin/env python3
"""Prepare/run matched routed physical-file access experiments; no programming."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

import generate_f10145 as gen
from fit_f10415 import AUDIT
from fit_f10415_enable import SDC

TOP='''module snapshot_fit(
 input wire host_clk,advance,input wire [3:0] address,data,
 input wire ce_n,we_n,freeze,held,request,write,
 input wire [4:0] index,input wire [9:0] state_data,
 output reg frozen,done,error,output reg [9:0] result,output reg [3:0] q);
 // Matched opposite-edge shell. Embedded cell paths have10ns budgets;
 // virtual I/O has2ns maximum/0ns minimum delays. Counts include the shell.
 reg [29:0] launch;
 always @(negedge host_clk)
  launch <= {advance,address,data,ce_n,we_n,freeze,held,request,write,index,state_data};
 wire step_r,ce_r,we_r,freeze_r,held_r,request_r,write_r;
 wire [3:0] address_r,data_r;wire [4:0] index_r;wire [9:0] state_r;
 assign {step_r,address_r,data_r,ce_r,we_r,freeze_r,held_r,request_r,write_r,index_r,state_r}=launch;
 wire frozen_w,done_w,error_w;wire [9:0] result_w;wire [3:0] q_w;
 always @(negedge host_clk){frozen,done,error,result,q}<={frozen_w,done_w,error_w,result_w,q_w};
`ifdef SNAPSHOT_BANK_EXPERIMENT
 cell_F10145A_snapshot bank(
 .state_host_clk(host_clk),.state_advance(step_r),.state_freeze(freeze_r),
 .state_guest_held(held_r),.state_request(request_r),.state_write(write_r),
 .state_index(index_r),.state_data(state_r),.state_frozen(frozen_w),
 .state_done(done_w),.state_error(error_w),.state_result(result_w),
`else
 assign {frozen_w,done_w,error_w,result_w}=13'b0;
 cell_F10145A bank(
`endif
 .sys_clk(host_clk),.p10(address_r[3]),.p9(address_r[2]),.p7(address_r[1]),.p6(address_r[0]),
 .p5(data_r[0]),.p4(data_r[1]),.p11(data_r[2]),.p12(data_r[3]),
 .p3(ce_r),.p13(we_r),.p2(q_w[0]),.p1(q_w[1]),.p15(q_w[2]),.p14(q_w[3]),.p8(1'b0));
endmodule
'''


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(output):
    output.mkdir(parents=True,exist_ok=False)
    generated,manifest=gen.generate()
    for variant in ('baseline','snapshot'):
        case=output/variant;case.mkdir()
        (case/'cell_F10145A_snapshot.sv').write_text(generated)
        shutil.copyfile(gen.ORIGINAL,case/gen.ORIGINAL.name)
        (case/'top.sv').write_text(TOP);(case/'bank.sdc').write_text(SDC)
        (case/'audit.tcl').write_text(AUDIT)
        (case/'snapshot_fit.qpf').write_text('PROJECT_REVISION = "snapshot_fit"\n')
        qsf='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS OFF
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE cell_F10145A_snapshot.sv
set_global_assignment -name VERILOG_FILE cell_F10145A.v
set_global_assignment -name VERILOG_MACRO "DORADO_F10145_LOGIC=1"
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
        for port in ('advance','address[*]','data[*]','ce_n','we_n','freeze','held','request',
                     'write','index[*]','state_data[*]','frozen','done','error','result[*]','q[*]'):
            qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
        if variant=='snapshot':qsf+='set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        (case/'snapshot_fit.qsf').write_text(qsf)
    paths=[Path(__file__).resolve(),gen.HERE/'generate_f10145.py',gen.ORIGINAL,
           gen.HERE/'fit_f10415.py',gen.HERE/'fit_f10415_enable.py']
    record=dict(scope='Isolated one F10145 file, original full-advance baseline vs explicit actual-advance snapshot clone;50MHz/opposite-edge registered shell/2ns max0ns min virtual I/O. Counts include shell; no board/global barrier/FPGA programming.',
        manifest=manifest,source_hashes={str(p.relative_to(gen.ROOT)):digest(p)for p in paths},
        compiled_files={str(p.relative_to(output)):digest(p)for p in output.glob('*/*')if p.is_file()})
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(output,quartus):
    record=json.loads((output/'provenance.json').read_text())
    for name,sha in record['compiled_files'].items():
        if digest(output/name)!=sha:raise ValueError('prepared input differs: '+name)
    for variant in ('baseline','snapshot'):
        case=output/variant
        if (case/'map.log').exists():raise ValueError('existing job/evidence; inspect rather than restart')
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),
            ('quartus_fit',['snapshot_fit'],'fit'),('quartus_sta',['snapshot_fit'],'sta'),
            ('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(variant,label,flush=True)
            with (case/(label+'.log')).open('w')as log:
                subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('F10145 fits terminal; audit both variants/four conditions before qualification.',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepare',type=Path)
    parser.add_argument('--run',type=Path)
    parser.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=parser.parse_args()
    if args.prepare:prepare(args.prepare.resolve())
    elif args.run:run(args.run.resolve(),args.quartus)
    else:parser.error('Specify --prepare or --run')
