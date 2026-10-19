#!/usr/bin/env python3
"""Matched actual netlist6502 routed experiment, no production changes."""
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
    import generate_6502 as gen
    from fit_f10415 import AUDIT
    from fit_f10415_enable import SDC
    output.mkdir(parents=True,exist_ok=False);adapter,manifest=gen.generate()
    top='''module snapshot_fit(
 input wire host_clk,advance,freeze,held,request,write,
 input wire [6:0] index,input wire [17:0] data,input wire [13:0] pins,
 output reg [17:0] result,output reg [25:0] outputs,output reg frozen,done,error);
 reg [43:0] input_stage,launch;
 always @(negedge host_clk)input_stage<={advance,freeze,held,request,write,index,data,pins};
 always @(posedge host_clk)launch<=input_stage;
 wire step_r,freeze_r,held_r,request_r,write_r;wire [6:0] index_r;wire [17:0] data_r;wire [13:0] pins_r;
 assign {step_r,freeze_r,held_r,request_r,write_r,index_r,data_r,pins_r}=launch;
 wire [17:0] result_w;wire [25:0] outputs_w;wire frozen_w,done_w,error_w;
 always @(posedge host_clk){result,outputs,frozen,done,error}<={result_w,outputs_w,frozen_w,done_w,error_w};
`ifdef SNAPSHOT_BANK_EXPERIMENT
 chip_6502_snapshot cell_inst(
 .state_host_clk(host_clk),.state_advance(step_r),.state_freeze(freeze_r),.state_guest_held(held_r),
 .state_request(request_r),.state_write(write_r),.state_index(index_r),.state_data(data_r),
 .state_frozen(frozen_w),.state_done(done_w),.state_error(error_w),.state_result(result_w),
`else
 assign {frozen_w,done_w,error_w,result_w}=21'b0;
 chip_6502 cell_inst(
`endif
 .clk(host_clk),.phi(pins_r[0]),.res(pins_r[1]),.so(pins_r[2]),.rdy(pins_r[3]),
 .nmi(pins_r[4]),.irq(pins_r[5]),.dbi(pins_r[13:6]),.dbo(outputs_w[7:0]),
 .rw(outputs_w[8]),.sync(outputs_w[9]),.ab(outputs_w[25:10]));
endmodule
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
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE chip_6502_snapshot.sv
set_global_assignment -name VERILOG_FILE chip_6502.v
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
    for port in ('advance','freeze','held','request','write','index[*]','data[*]','pins[*]','result[*]','outputs[*]','frozen[*]','done[*]','error[*]'):
        qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
    for variant in ('baseline','snapshot'):
        case=output/variant;case.mkdir();shutil.copyfile(gen.ROOT/'verilog/vendor/6502/chip_6502.v',case/'chip_6502.v')
        for name,text in {'top.sv':top,'chip_6502_snapshot.sv':adapter.decode(),'bank.sdc':SDC,'audit.tcl':AUDIT,
            'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n',
            'snapshot_fit.qsf':qsf+('set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'if variant=='snapshot'else'')}.items():(case/name).write_text(text)
    sources=[Path(__file__).resolve(),gen.ROOT/'fpga/mister/snapshot/generate_6502.py',gen.ROOT/'fpga/mister/snapshot/fit_f10415.py',gen.ROOT/'fpga/mister/snapshot/fit_f10415_enable.py',gen.ROOT/'verilog/vendor/6502/chip_6502.v']
    executed=output/'executed-sources';executed.mkdir()
    for p in sources:shutil.copyfile(p,executed/p.name)
    record=dict(scope='Isolated actual vendor netlist6502, all1725node vector with96words. Original CRLF/combinational netlist/relaxation preserved. Matched50MHz negative-edge external staging/positive-edge launch/capture,20ns embedded paths,2ns max0ns min virtual I/O. Counts include shell/admin/pruning; no full BaseBoard/global snapshot or speed claim.',
        manifest=manifest,builder_sha256=digest(Path(__file__)),source_hashes={str(p.relative_to(gen.ROOT)):digest(p)for p in sources},
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
    print('NETLIST6502 matched fits terminal; qualify exact sources/all conditions before use.',flush=True)


def normalized(text):
    return '\n'.join(line for line in text.splitlines()if line.strip()and line!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def audit(directory):
    record=json.loads((directory/'provenance.json').read_text());results={}
    for name,sha in record['compiled_files'].items():
        if not name.endswith('.qsf'):assert digest(directory/name)==sha,'compiled input differs: '+name
    corners={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
    for variant in ('baseline','snapshot'):
        case=directory/variant
        assert normalized((case/'snapshot_fit.qsf').read_text())==normalized(record['expected_qsf'][variant]),'project source/macro/constraint differs'
        summary=(case/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
        resources={}
        for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_blocks','Total RAM Blocks')]:
            m=re.search(label+r' : ([\d,]+)',summary);assert m,'missing '+key
            resources[key]=int(m[1].replace(',',''))
        assert resources['ram_blocks']==0,'unexpected RAM inference'
        assert {p.name[len('bank-setup-'):-4]for p in case.glob('bank-setup-*.rpt')}==corners
        timing={}
        for corner in sorted(corners):
            timing[corner]={}
            for mode in ('setup','hold','mpw'):
                text=(case/f'bank-{mode}-{corner}.rpt').read_text()
                m=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',text)
                assert m and int(m[1])==0 and float(m[2])>=0,'timing violation/missing path '+mode+'/'+corner
                timing[corner][mode]=float(m[2])
            for mode in ('recovery','removal'):
                assert (case/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.'),'unexpected async path'
            text=(case/f'bank-ucp-{corner}.rpt').read_text()
            for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                assert re.search('; '+label+r'\s*; 0\s*; 0\s*;',text),'unconstrained '+label
        warnings={}
        for phase in ('map','fit'):
            text=(case/(phase+'.log')).read_text();assert 'Critical Warning'not in text,'critical warning'
            warnings[phase]=dict(Counter(re.findall(r'Warning \((\d+)\)',text)))
        results[variant]=dict(resources=resources,timing=timing,warnings=warnings)
    for phase in ('map','fit'):
        new=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase])
        assert not(set(new)-{'10036','13024','13410'}),('unreviewed warnings',phase,new)
    return dict(status='PASS',scope=record['scope'],results=results,
        delta={k:results['snapshot']['resources'][k]-results['baseline']['resources'][k]for k in ('alms','registers','ram_blocks')},
        minimum_snapshot_slack_ns=min(v for row in results['snapshot']['timing'].values()for v in row.values()))


def controls(directory):
    rows=[]
    for name,path,before,after in [
        ('source','snapshot/chip_6502_snapshot.sv','state_hold && state_frozen && state_request','state_request'),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('warning','snapshot/map.log',None,None),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-netlist6502-control-',dir='/tmp')as temp:
            out=Path(temp)/'evidence';shutil.copytree(directory,out,ignore=shutil.ignore_patterns('db','incremental_db'));p=out/path;text=p.read_text()
            if before:assert before in text; text=text.replace(before,after)
            else:text+='\nWarning (10999): unreviewed inference\n'
            p.write_text(text)
            try:audit(out)
            except AssertionError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged NETLIST6502 fit evidence accepted: '+name)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--prepare',type=Path);parser.add_argument('--run',type=Path);parser.add_argument('--audit',type=Path)
    parser.add_argument('--output',type=Path);parser.add_argument('--negative-controls',action='store_true')
    parser.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=parser.parse_args()
    if args.prepare:prepare(args.prepare.resolve())
    elif args.run:run(args.run.resolve(),args.quartus)
    elif args.audit and args.output:
        result=audit(args.audit.resolve())
        if args.negative_controls:result['failure_controls']=controls(args.audit.resolve())
        args.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
    else:parser.error('Specify --prepare, --run or --audit/--output')
