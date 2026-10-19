#!/usr/bin/env python3
"""Matched isolated full ContA fits; actual FPGA remains unchanged."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess

from fit_f10415 import AUDIT
from fit_f10415_enable import SDC

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def top(ports,module):
    inputs=[p for p in ports if p['direction']=='input'];outputs=[p for p in ports if p['direction']=='output']
    ni=sum(p['width']for p in inputs);no=sum(p['width']for p in outputs)
    code=[f'''module snapshot_fit(
 input wire host_clk,advance,input wire [{ni-1}:0] stimulus,
 input wire freeze,held,request,write,input wire [11:0] index,
 input wire [17:0] state_data,output reg [{no-1}:0] q,
 output reg frozen,done,error,output reg [17:0] result);
 // Same opposite-edge launch/capture shell and budgets for both variants.
 reg [{ni+34}:0] launch;
 always @(negedge host_clk)
  launch<={{advance,stimulus,freeze,held,request,write,index,state_data}};
 wire step_r,freeze_r,held_r,request_r,write_r;
 wire [{ni-1}:0] stimulus_r;wire [11:0] index_r;wire [17:0] state_r;
 assign {{step_r,stimulus_r,freeze_r,held_r,request_r,write_r,index_r,state_r}}=launch;
 wire bank_frozen,bank_done,bank_error;wire [17:0] bank_result;
 wire [{no-1}:0] bank_q;
 always @(negedge host_clk){{q,frozen,done,error,result}}<={{bank_q,bank_frozen,bank_done,bank_error,bank_result}};
''']
    for port in outputs:
        shape=''if port['width']==1 else f"[{port['width']-1}:0] "
        code.append(f"wire {shape}bank_{port['name']};")
    code.append('assign bank_q={'+','.join('bank_'+p['name']for p in reversed(outputs))+'};')
    code.append(f'''`ifdef SNAPSHOT_BANK_EXPERIMENT
 {module} #(.SYSPER(4)) bank(
 .state_host_clk(host_clk),.state_guest_advance(step_r),.state_freeze(freeze_r),
 .state_guest_held(held_r),.state_request(request_r),.state_write(write_r),
 .state_index(index_r),.state_data(state_r),.state_frozen(bank_frozen),
 .state_done(bank_done),.state_error(bank_error),.state_result(bank_result),
`else
 assign {{bank_frozen,bank_done,bank_error,bank_result}}=21'b0;
 ContA_m_Rev_m_Cd_monitor #(.SYSPER(4)) bank(
`endif''')
    connections=['.sys_clk(host_clk)'];output_names={p['name']for p in outputs};offset=0
    for p in inputs:
        name,width=p['name'],p['width']
        expression=f'stimulus_r[{offset}]'if width==1 else f'stimulus_r[{offset}+:{width}]'
        if name+'__drv'in output_names:expression+=' | bank_'+name+'__drv'
        connections.append(f'.{name}({expression})');offset+=width
    connections += [f".{p['name']}(bank_{p['name']})"for p in outputs]
    code+= [',\n'.join(connections),');','endmodule']
    return '\n'.join(code)+'\n'


def prepare(source,output):
    import generate_conta_full as gen
    from test_conta_full import fixture
    output.mkdir(parents=True,exist_ok=False)
    board,manifest,clones=gen.generate(source)
    ports=fixture(source,manifest)[2]['ports'];wrapper=top(ports,manifest['module'])
    originals=[ROOT/'verilog/cells'/f'{name}.v'for name in manifest['original_sources']]
    for variant in ('baseline','snapshot'):
        case=output/variant;case.mkdir()
        shutil.copyfile(source,case/'ContA_original.v')
        (case/'ContA_snapshot.sv').write_text(board)
        for name,text in clones.items():(case/name).write_text(text)
        for path in originals:shutil.copyfile(path,case/path.name)
        (case/'top.sv').write_text(wrapper);(case/'bank.sdc').write_text(SDC);(case/'audit.tcl').write_text(AUDIT)
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
set_global_assignment -name VERILOG_MACRO "DORADO_F10145_LOGIC=1"
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
        for name in ['top.sv','ContA_snapshot.sv',*clones]:qsf+='set_global_assignment -name SYSTEMVERILOG_FILE '+name+'\n'
        for name in ['ContA_original.v',*[p.name for p in originals]]:qsf+='set_global_assignment -name VERILOG_FILE '+name+'\n'
        for port in ['advance','stimulus[*]','freeze','held','request','write','index[*]','state_data[*]','q[*]','frozen','done','error','result[*]']:
            qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
        if variant=='snapshot':qsf+='set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        (case/'snapshot_fit.qsf').write_text(qsf)
    paths=[Path(__file__).resolve(),HERE/'generate_conta_full.py',HERE/'generate_snapshot_ff.py',
           HERE/'generate_contb_full.py',HERE/'generate_contb.py',HERE/'generate_f10145.py',
           HERE/'test_conta_full.py',HERE/'audit_contb_im.py',HERE/'fit_f10415.py',HERE/'fit_f10415_enable.py',*originals]
    provenance=dict(scope='Isolated full physical ContA, original continuous baseline vs exact-advance adapter;50MHz/opposite-edge registered shell/2ns max0ns min virtual I/O. Counts include shell; no whole-core barrier/FPGA programming.',
        manifest=manifest,driver_sha256=digest(Path(__file__)),source_board_sha256=digest(source),
        source_hashes={str(p.relative_to(ROOT)):digest(p)for p in paths},
        compiled_files={str(p.relative_to(output)):digest(p)for p in output.glob('*/*')if p.is_file()})
    (output/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')


def run(output,quartus):
    record=json.loads((output/'provenance.json').read_text())
    if digest(Path(__file__))!=record['driver_sha256']:raise ValueError('prepared driver differs')
    for name,sha in record['compiled_files'].items():
        if digest(output/name)!=sha:raise ValueError('prepared input differs: '+name)
    for variant in ('baseline','snapshot'):
        case=output/variant
        if (case/'map.log').exists():raise ValueError('existing job/evidence: inspect rather than restart')
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                               ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(variant,label,flush=True)
            with (case/(label+'.log')).open('w')as log:
                subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('ContA fits terminal; audit all sources/paths/resources before qualification.',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepare',type=Path);parser.add_argument('--source',type=Path);parser.add_argument('--run',type=Path)
    parser.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=parser.parse_args()
    if args.prepare and args.source:prepare(args.source.resolve(),args.prepare.resolve())
    elif args.run:run(args.run.resolve(),args.quartus)
    else:parser.error('Specify --prepare/--source or --run')
