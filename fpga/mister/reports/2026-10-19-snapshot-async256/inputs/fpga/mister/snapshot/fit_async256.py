#!/usr/bin/env python3
"""Matched routed experiment for one MB7071H and one F10414; no programming."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

import generate_async256 as gen
from fit_f10415 import AUDIT
from fit_f10415_enable import SDC
from test_async256 import MBPORTS, FPORTS

TOP = '''module snapshot_fit(
 input wire host_clk,advance,input wire [7:0] address,
 input wire [3:0] data,select_mb,input wire ce_f,we_mb_n,we_f_n,
 input wire freeze,held,request,write,input wire [8:0] index,
 input wire [3:0] state_data,state_data_f,
 output reg [1:0] frozen,done,error,
 output reg [3:0] result_mb,result_f,q_mb,output reg q_f);
 // Matched50MHz/opposite-edge shell:10ns embedded data paths.
 // Both variants include the same2ns max/0ns min virtual I/O contract.
 reg [40:0] launch;
 always @(negedge host_clk)
  launch<={advance,address,data,select_mb,ce_f,we_mb_n,we_f_n,
            freeze,held,request,write,index,state_data,state_data_f};
 wire step_r,ce_r,wem_r,wef_r,freeze_r,held_r,request_r,write_r;
 wire [7:0] address_r;wire [3:0] data_r,select_r,state_r,statef_r;
 wire [8:0] index_r;
 assign {step_r,address_r,data_r,select_r,ce_r,wem_r,wef_r,
         freeze_r,held_r,request_r,write_r,index_r,state_r,statef_r}=launch;
 wire [1:0] frozen_w,done_w,error_w;
 wire [3:0] resultmb_w,resultf_w,qmb_w;wire qf_w;
 always @(negedge host_clk)
  {frozen,done,error,result_mb,result_f,q_mb,q_f}<=
  {frozen_w,done_w,error_w,resultmb_w,resultf_w,qmb_w,qf_w};
`ifdef SNAPSHOT_BANK_EXPERIMENT
 cell_MB7071H_snapshot mb(
 .state_host_clk(host_clk),.state_advance(step_r),.state_freeze(freeze_r),
 .state_guest_held(held_r),.state_request(request_r),.state_write(write_r),
 .state_index(index_r),.state_data(state_r),.state_frozen(frozen_w[0]),
 .state_done(done_w[0]),.state_error(error_w[0]),.state_result(resultmb_w),
`else
 assign {frozen_w,done_w,error_w,resultmb_w,resultf_w}=14'b0;
 cell_MB7071H mb(
`endif
 .sys_clk(host_clk),
 MBPORTS
`ifdef SNAPSHOT_BANK_EXPERIMENT
 cell_F10414_snapshot f(
 .state_host_clk(host_clk),.state_advance(step_r),.state_freeze(freeze_r),
 .state_guest_held(held_r),.state_request(request_r),.state_write(write_r),
 .state_index(index_r),.state_data(statef_r),.state_frozen(frozen_w[1]),
 .state_done(done_w[1]),.state_error(error_w[1]),.state_result(resultf_w),
`else
 cell_F10414 f(
`endif
 .sys_clk(host_clk),
 FPORTS
endmodule
'''
for label, source in [('MBPORTS', MBPORTS), ('FPORTS', FPORTS)]:
    for before, after in [('we_mb_n','wem_r'),('we_f_n','wef_r'),('select_mb','select_r'),
                          ('address','address_r'),('data','data_r'),('ce_f','ce_r'),
                          ('q_mb','qmb_w'),('q_f','qf_w')]:
        source = source.replace(before, after)
    TOP = TOP.replace(label, source)


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(output):
    output.mkdir(parents=True, exist_ok=False)
    manifests = {k: gen.generate(k)[1] for k in gen.SPECS}
    for variant in ('baseline','snapshot'):
        case = output/variant
        case.mkdir()
        for kind in gen.SPECS:
            (case/('cell_'+kind+'_snapshot.sv')).write_text(gen.generate(kind)[0])
            shutil.copyfile(gen.ROOT/'verilog/cells'/('cell_'+kind+'.v'),case/('cell_'+kind+'.v'))
        (case/'top.sv').write_text(TOP)
        (case/'bank.sdc').write_text(SDC)
        (case/'audit.tcl').write_text(AUDIT)
        (case/'snapshot_fit.qpf').write_text('PROJECT_REVISION = "snapshot_fit"\n')
        qsf = '''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS OFF
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
        for kind in gen.SPECS:
            qsf+='set_global_assignment -name SYSTEMVERILOG_FILE cell_'+kind+'_snapshot.sv\n'
            qsf+='set_global_assignment -name VERILOG_FILE cell_'+kind+'.v\n'
        for port in ('advance','address[*]','data[*]','select_mb[*]','ce_f','we_mb_n','we_f_n',
                     'freeze','held','request','write','index[*]','state_data[*]','state_data_f[*]',
                     'frozen[*]','done[*]','error[*]','result_mb[*]','result_f[*]','q_mb[*]','q_f'):
            qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
        if variant=='snapshot':
            qsf+='set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        (case/'snapshot_fit.qsf').write_text(qsf)
    paths=[Path(__file__).resolve(),gen.HERE/'generate_async256.py',gen.HERE/'test_async256.py',
           gen.HERE/'fit_f10415.py',gen.HERE/'fit_f10415_enable.py']
    paths += [gen.ROOT/'verilog/cells'/('cell_'+k+'.v') for k in gen.SPECS]
    record=dict(scope='Isolated1280-bit async RAM pair, original full-advance vs actual-advance adapters;50MHz/opposite-edge registered shell/2ns max0ns min virtual I/O. Counts include shell/pruned baseline admin. Frozen scanning may change combinational outputs: all consumers must be held externally. No board/global barrier or physical snapshot.',
        manifests=manifests,source_hashes={str(p.relative_to(gen.ROOT)):digest(p) for p in paths},
        compiled_files={str(p.relative_to(output)):digest(p) for p in output.glob('*/*') if p.is_file()})
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(output, quartus):
    record=json.loads((output/'provenance.json').read_text())
    for name, expected in record['compiled_files'].items():
        assert digest(output/name)==expected, 'prepared input differs: '+name
    for variant in ('baseline','snapshot'):
        case=output/variant
        assert not (case/'map.log').exists(), 'Inspect existing job; never blindly rerun'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),
            ('quartus_fit',['snapshot_fit'],'fit'),('quartus_sta',['snapshot_fit'],'sta'),
            ('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(variant,label,flush=True)
            with (case/(label+'.log')).open('w') as log:
                subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Async RAM fits terminal; audit sources/inference/all conditions before qualification.',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepare',type=Path)
    parser.add_argument('--run',type=Path)
    parser.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=parser.parse_args()
    if args.prepare:
        prepare(args.prepare.resolve())
    elif args.run:
        run(args.run.resolve(),args.quartus)
    else:
        parser.error('Specify --prepare or --run')
