#!/usr/bin/env python3
"""Matched four-family/five-mode DskEth FF routed experiment, no production changes."""
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
    import generate_dsketh_ff as gen
    from fit_f10415 import AUDIT
    from fit_f10415_enable import SDC
    from test_dsketh_ff import KINDS,PORTS
    output.mkdir(parents=True,exist_ok=False);generated=gen.generate()
    top='''module snapshot_fit(input wire host_clk,advance,hold,write,
 input wire [17:0] data,input wire [11:0] pins,
 output reg [89:0] result,output reg [29:0] outputs);
 reg [32:0] input_stage,launch;
 always @(negedge host_clk)input_stage<={advance,hold,write,data,pins};
 always @(posedge host_clk)launch<=input_stage;
 wire advance_r,hold_r,write_r;wire [17:0] data_r;wire [11:0] pins_r;
 assign {advance_r,hold_r,write_r,data_r,pins_r}=launch;
 wire [17:0] result_w[5];wire [5:0] outputs_w[5];
'''
    for i in range(5):
        kind=KINDS[min(i,3)];param=' #(.PRECAP('+str(i-3)+'))'if i>=3 else''
        ports=PORTS[min(i,3)].replace('[I]','['+str(i)+']').replace('pins[','pins_r[').replace('outputs[','outputs_w[')
        top+='generate begin:case'+str(i)+'\n`ifdef SNAPSHOT_BANK_EXPERIMENT\n'
        top+=kind+'_snapshot'+param+' cell_inst(.state_host_clk(host_clk),.state_advance(advance_r),.state_hold(hold_r),.state_write(write_r),.state_data(data_r),.state_result(result_w['+str(i)+']),.sys_clk(host_clk),'+ports+');\n'
        top+='`else\nassign result_w['+str(i)+']=0;\n'+kind+param+' cell_inst(.sys_clk(host_clk),'+ports+');\n`endif\n'
        driven={0:2,1:5,2:1,3:6,4:6}[i]
        if driven<6:top+='assign outputs_w['+str(i)+'][5:'+str(driven)+']=0;\n'
        top+='always @(posedge host_clk)begin result['+str(i*18)+'+:18]<=result_w['+str(i)+'];outputs['+str(i*6)+'+:6]<=outputs_w['+str(i)+'];end\nend endgenerate\n'
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
    for kind in KINDS:
        qsf+='set_global_assignment -name SYSTEMVERILOG_FILE '+kind+'_snapshot.sv\nset_global_assignment -name VERILOG_FILE '+kind+'.v\n'
    for port in ('advance','hold','write','data[*]','pins[*]','result[*]','outputs[*]'):
        qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'\n'
    for variant in ('baseline','snapshot'):
        case=output/variant;case.mkdir()
        for kind,(adapter,_)in generated.items():
            shutil.copyfile(gen.ROOT/'verilog/cells'/(kind+'.v'),case/(kind+'.v'))
            (case/(kind+'_snapshot.sv')).write_text(adapter)
        for name,text in {'top.sv':top,'bank.sdc':SDC,'audit.tcl':AUDIT,
            'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n',
            'snapshot_fit.qsf':qsf+('set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'if variant=='snapshot'else'')}.items():(case/name).write_text(text)
    sources=[Path(__file__).resolve(),gen.ROOT/'fpga/mister/snapshot/generate_dsketh_ff.py',gen.ROOT/'fpga/mister/snapshot/fit_f10415.py',gen.ROOT/'fpga/mister/snapshot/fit_f10415_enable.py',*[gen.ROOT/'verilog/cells'/(k+'.v')for k in KINDS],gen.ROOT/'fpga/mister/snapshot/test_dsketh_ff.py']
    executed=output/'executed-sources';executed.mkdir()
    for p in sources:shutil.copyfile(p,executed/p.name)
    record=dict(scope='Isolated four DskEth cell families/five modes including SN174 PRECAP0/1. Original comparator/counter/setup/clear semantics retained verbatim. Matched50MHz negative-edge external staging/positive-edge launch/capture,20ns embedded paths,2ns max0ns min virtual I/O. Counts include shell/admin/pruning; no behavior change/full board/global snapshot or speed claim.',
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
    print('DskEth FF matched fits terminal; qualify exact sources/all conditions before use.',flush=True)


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
        ('source','snapshot/cell_MC1650_snapshot.sv','if(state_hold)','if(1\'b0)'),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('warning','snapshot/map.log',None,None),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-dsketh-ff-control-',dir='/tmp')as temp:
            out=Path(temp)/'evidence';shutil.copytree(directory,out,ignore=shutil.ignore_patterns('db','incremental_db'));p=out/path;text=p.read_text()
            if before:assert before in text; text=text.replace(before,after)
            else:text+='\nWarning (10999): unreviewed inference\n'
            p.write_text(text)
            try:audit(out)
            except AssertionError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged DskEth FF fit evidence accepted: '+name)
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
