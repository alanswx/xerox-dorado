#!/usr/bin/env python3
"""Matched nine-family BaseBoard clock/reset primitive fits, no production changes."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import tempfile


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(output):
    import generate_baseboard_ff as gen
    from fit_f10415 import AUDIT
    from fit_f10415_enable import SDC
    from test_baseboard_ff import KINDS,ports
    output.mkdir(parents=True,exist_ok=False);generated=gen.generate()
    top='''module snapshot_fit(input wire host_clk,advance,hold,write,
 input wire [3:0] selected,input wire [1:0] index,input wire [17:0] data,input wire [143:0] pins,
 output reg [161:0] result,output reg [143:0] outputs);
 reg [170:0] input_stage,launch;
 always @(negedge host_clk)input_stage<={advance,hold,write,selected,index,data,pins};
 always @(posedge host_clk)launch<=input_stage;
 wire advance_r,hold_r,write_r;wire [3:0] selected_r;wire [1:0] index_r;wire [17:0] data_r;wire [143:0] pins_flat;
 assign {advance_r,hold_r,write_r,selected_r,index_r,data_r,pins_flat}=launch;
 wire [15:0] pins_r[9];wire [17:0] result_w[9];wire [15:0] outputs_w[9];
'''
    for i,kind in enumerate(KINDS):
        param=' #(.N(4))'if kind=='cell_CLOCKGEN'else''
        pp,count=ports(kind,i,'pins_r','outputs_w');top+=f'assign pins_r[{i}]=pins_flat[{i*16}+:16];\n'
        top+='generate begin:case'+str(i)+'\n`ifdef SNAPSHOT_BANK_EXPERIMENT\n'
        top+=kind+'_snapshot'+param+f' cell_inst(.state_host_clk(host_clk),.state_advance(advance_r),.state_hold(hold_r),.state_write(write_r && selected_r==4\'d{i}),.state_index(index_r),.state_data(data_r),.state_result(result_w[{i}]),.sys_clk(host_clk),'+pp+');\n'
        top+='`else\nassign result_w['+str(i)+']=0;\n'+kind+param+' cell_inst(.sys_clk(host_clk),'+pp+');\n`endif\n'
        top+=f'assign outputs_w[{i}][15:{count}]=0;\n'
        top+=f'always @(posedge host_clk)begin result[{i*18}+:18]<=result_w[{i}];outputs[{i*16}+:16]<=outputs_w[{i}];end\nend endgenerate\n'
    top+='endmodule\n'
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
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
    for kind in KINDS:qsf+='set_global_assignment -name SYSTEMVERILOG_FILE '+kind+'_snapshot.sv\nset_global_assignment -name VERILOG_FILE '+kind+'.v\n'
    for port in ('advance','hold','write','selected[*]','index[*]','data[*]','pins[*]','result[*]','outputs[*]'):qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
    for variant in ('baseline','snapshot'):
        case=output/variant;case.mkdir()
        for kind,(adapter,_)in generated.items():
            (case/(kind+'.v')).write_bytes(gen.original_source(kind));(case/(kind+'_snapshot.sv')).write_text(adapter)
        for name,text in {'top.sv':top,'bank.sdc':SDC,'audit.tcl':AUDIT,
            'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n',
            'snapshot_fit.qsf':qsf+('set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'if variant=='snapshot'else'')}.items():(case/name).write_text(text)
    sources=[Path(__file__).resolve(),gen.ROOT/'fpga/mister/snapshot/generate_baseboard_ff.py',gen.ROOT/'fpga/mister/snapshot/generate_snapshot_ff.py',gen.ROOT/'fpga/mister/snapshot/test_baseboard_ff.py',gen.ROOT/'fpga/mister/snapshot/fit_f10415.py',gen.ROOT/'fpga/mister/snapshot/fit_f10415_enable.py']
    executed=output/'executed-sources';executed.mkdir()
    for p in sources:shutil.copyfile(p,executed/p.name)
    for k in KINDS:(executed/(k+'.v')).write_bytes(gen.original_source(k))
    record=dict(scope='Nine actual BaseBoard clock/reset/TTL families including original CLOCKGEN N4:70representative statebits/87actual boardbits. Untouched original processes with local word access. Matched50MHz negative-edge input-stage/positive-edge launch/capture,20ns embedded paths,2nsmax0nsmin virtualIO,no cuts. Includes shell/admin/pruning;not full BaseBoard/global budget or timer calibration.',
        manifest={k:m for k,(_,m)in generated.items()},builder_sha256=digest(Path(__file__)),source_hashes={str(p.relative_to(gen.ROOT)):digest(p)for p in sources},
        compiled_files={str(p.relative_to(output)):digest(p)for p in output.glob('*/*')if p.is_file()},expected_qsf={v:(output/v/'snapshot_fit.qsf').read_text()for v in ('baseline','snapshot')})
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(output,quartus):
    record=json.loads((output/'provenance.json').read_text())
    assert digest(Path(__file__))==record['builder_sha256'],'prepared builder differs'
    for name,sha in record['compiled_files'].items():assert digest(output/name)==sha,'prepared input differs: '+name
    for variant in ('baseline','snapshot'):
        case=output/variant;assert not(case/'map.log').exists(),'inspect existing job; do not restart'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                                ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(variant,label,flush=True)
            with (case/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('BaseBoard FF matched fits terminal; qualify exact sources/all conditions before use.',flush=True)



if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--prepare',type=Path);p.add_argument('--run',type=Path)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.prepare:prepare(a.prepare.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    else:p.error('Specify --prepare or --run')
