#!/usr/bin/env python3
"""Matched width1/2/4/5/18 M10K/state-access experiment; no FPGA programming."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

WIDTHS=(1,2,4,5,18)


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def prepare(output):
    from fit_f10415 import AUDIT
    from fit_f10415_enable import SDC
    here=Path(__file__).resolve().parent;root=here.parents[2]
    output.mkdir(parents=True,exist_ok=False)
    top='''module snapshot_fit(
 input wire host_clk,input wire [4:0] advance,ce_n,we_n,freeze,held,request,write,
 input wire [49:0] address,input wire [89:0] data,state_data,
 input wire [54:0] word_index,output reg [89:0] q,result,
 output reg [4:0] frozen,done,error);
 // Matched negative-edge launch/capture,50MHz/10ns internal paths in both
 // variants. Independent bank pins prevent artificial cross-width sharing.
'''
    for i,width in enumerate(WIDTHS):
        top+=f'''
 reg [63:0] launch_{i};
 always @(negedge host_clk)launch_{i}<=
  {{advance[{i}],address[{i*10}+:10],data[{i*18}+:18],ce_n[{i}],we_n[{i}],
    freeze[{i}],held[{i}],request[{i}],write[{i}],word_index[{i*11}+:11],state_data[{i*18}+:18]}};
 wire step_{i},ce_{i},we_{i},freeze_{i},held_{i},request_{i},write_{i};
 wire [9:0] address_{i};wire [17:0] data_{i},state_{i};wire [10:0] word_{i};
 assign {{step_{i},address_{i},data_{i},ce_{i},we_{i},freeze_{i},held_{i},request_{i},write_{i},word_{i},state_{i}}}=launch_{i};
 wire [{width-1}:0] q_{i};wire [17:0] result_{i};wire frozen_{i},done_{i},error_{i};
 always @(negedge host_clk)
  {{q[{i*18}+:18],frozen[{i}],done[{i}],error[{i}],result[{i*18}+:18]}}<=
  {{{{{18-width}{{1'b0}}}},q_{i},frozen_{i},done_{i},error_{i},result_{i}}};
`ifdef SNAPSHOT_BANK_EXPERIMENT
 dorado_snapshot_f10415_word_bank #(.WIDTH({width})) bank_{i}(
  .host_clk(host_clk),.guest_advance(step_{i}),.address(address_{i}),.data(data_{i}[{width-1}:0]),
  .ce_n(ce_{i}),.we_n(we_{i}),.q(q_{i}),.freeze(freeze_{i}),.guest_held(held_{i}),.frozen(frozen_{i}),
  .request(request_{i}),.write(write_{i}),.word_index(word_{i}),.state_data(state_{i}),
  .done(done_{i}),.error(error_{i}),.result(result_{i}));
`else
 assign frozen_{i}=0;assign done_{i}=0;assign error_{i}=0;assign result_{i}=0;
 genvar b_{i};
 generate for(b_{i}=0;b_{i}<{width};b_{i}=b_{i}+1)begin:original_{i}
  cell_F10415A bit_cell(host_clk,address_{i}[9],address_{i}[8],address_{i}[7],address_{i}[6],
   address_{i}[5],address_{i}[4],address_{i}[3],address_{i}[2],address_{i}[1],address_{i}[0],
   data_{i}[b_{i}],ce_{i},we_{i},q_{i}[b_{i}]);
 end endgenerate
`endif
'''
    top+='endmodule\n'
    qsf='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS ON
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE f10415_word_bank.sv
set_global_assignment -name VERILOG_FILE cell_F10415A.v
set_global_assignment -name VERILOG_MACRO "DORADO_F10415_M10K=1"
set_global_assignment -name SDC_FILE bank.sdc
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
    for port in ('advance','ce_n','we_n','freeze','held','request','write','address','data','state_data','word_index','q','result','frozen','done','error'):
        qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+port+'[*]\n'
    for variant in ('baseline','snapshot'):
        case=output/variant;case.mkdir()
        shutil.copyfile(here/'f10415_word_bank.sv',case/'f10415_word_bank.sv')
        shutil.copyfile(root/'verilog/cells/cell_F10415A.v',case/'cell_F10415A.v')
        for name,text in {'top.sv':top,'bank.sdc':SDC,'audit.tcl':AUDIT,
            'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n',
            'snapshot_fit.qsf':qsf+('set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'if variant=='snapshot'else'')}.items():
            (case/name).write_text(text)
    sources=[Path(__file__).resolve(),here/'f10415_word_bank.sv',root/'verilog/cells/cell_F10415A.v',
             here/'fit_f10415.py',here/'fit_f10415_enable.py']
    record=dict(scope='Five independent width1/2/4/5/18 banks; original full-advance vs exact-advance/state-access cells, same50MHz negative-edge launch/capture shell and10ns embedded paths,2ns max0ns min virtual I/O, no cuts. Counts include shell/admin/pruning; no IFU/global barrier/physical snapshot or omitted-CE synthesis qualification.',
        widths=list(WIDTHS),modeled_state_bits=sum(WIDTHS)*1025,builder_sha256=digest(Path(__file__)),
        source_hashes={str(p.relative_to(root)):digest(p)for p in sources},
        compiled_files={str(p.relative_to(output)):digest(p)for p in output.glob('*/*')if p.is_file()},
        expected_qsf={v:(output/v/'snapshot_fit.qsf').read_text()for v in ('baseline','snapshot')})
    (output/'provenance.json').write_text(json.dumps(record,indent=2)+'\n')


def run(directory,quartus):
    record=json.loads((directory/'provenance.json').read_text())
    assert digest(Path(__file__))==record['builder_sha256'],'builder differs from preparation'
    for name,expected in record['compiled_files'].items():assert digest(directory/name)==expected,'prepared input differs: '+name
    for variant in ('baseline','snapshot'):
        case=directory/variant;assert not(case/'map.log').exists(),'inspect existing job; do not restart'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
                                ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(variant,label,flush=True)
            with (case/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=case,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('Width-correct RAM fits terminal; audit inference/all corners before qualification.',flush=True)


def normalized(text):
    return '\n'.join(line for line in text.splitlines()if line.strip()and line!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def inferred_memories(text):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',text));rows=[]
    for i,header in enumerate(headers):
        if 'altsyncram:'not in header[1]or not re.search(r'\bbank_[0-4]\|',header[1]):continue
        section=text[header.end():headers[i+1].start()if i+1<len(headers)else len(text)]
        number=int(re.search(r'\bbank_([0-4])\|',header[1])[1]);values={}
        for key,wanted in [('OPERATION_MODE','DUAL_PORT'),('RAM_BLOCK_TYPE','M10K'),('WIDTH_A',str(WIDTHS[number])),('WIDTH_B',str(WIDTHS[number])),
                           ('NUMWORDS_A','1024'),('NUMWORDS_B','1024'),('READ_DURING_WRITE_MODE_MIXED_PORTS','OLD_DATA')]:
            actual=re.findall(r'; '+key+r'\s*; (\w+)\s*;',section)
            assert actual==[wanted],('RAM inference differs',number,key,actual)
            values[key]=wanted
        rows.append(dict(bank=number,instance=header[1].strip(),parameters=values))
    assert len(rows)==5 and {r['bank']for r in rows}==set(range(5)),'missing/extra width-correct RAM macro'
    return rows


def audit(directory):
    record=json.loads((directory/'provenance.json').read_text());results={}
    assert record['widths']==list(WIDTHS)and record['modeled_state_bits']==30750
    for name,expected in record['compiled_files'].items():
        if not name.endswith('.qsf'):assert digest(directory/name)==expected,'compiled input differs: '+name
    corners={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
    for variant in ('baseline','snapshot'):
        case=directory/variant
        assert normalized((case/'snapshot_fit.qsf').read_text())==normalized(record['expected_qsf'][variant]),'project source/macro/SDC changed'
        summary=(case/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
        resources={}
        for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
            m=re.search(label+r' : ([\d,]+)',summary);assert m,'missing resource '+key
            resources[key]=int(m[1].replace(',',''))
        assert resources['ram_bits']==30720 and resources['ram_blocks']==6,'RAM geometry/packing differs'
        assert {p.name[len('bank-setup-'):-4]for p in case.glob('bank-setup-*.rpt')}==corners,'missing corner'
        timing={}
        for corner in sorted(corners):
            timing[corner]={}
            for mode in ('setup','hold','mpw'):
                text=(case/f'bank-{mode}-{corner}.rpt').read_text();m=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',text)
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
        row=dict(resources=resources,timing=timing,warnings=warnings)
        if variant=='snapshot':row['instruction_memories']=inferred_memories((case/'snapshot_fit.map.rpt').read_text())
        results[variant]=row
    for phase in ('map','fit'):
        new=Counter(results['snapshot']['warnings'][phase])-Counter(results['baseline']['warnings'][phase])
        assert not(set(new)-{'10036','13024','13410'}),('unreviewed warning increase',phase,new)
    return dict(status='PASS',scope=record['scope'],results=results,
        delta={key:results['snapshot']['resources'][key]-results['baseline']['resources'][key]for key in ('alms','registers','ram_bits','ram_blocks')},
        minimum_snapshot_slack_ns=min(v for row in results['snapshot']['timing'].values()for v in row.values()))


def controls(directory):
    rows=[]
    for name,path,before,after in [
        ('source','snapshot/f10415_word_bank.sv','saved_q<=state_data[WIDTH-1:0];',"saved_q<='0;"),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('old_data','snapshot/snapshot_fit.map.rpt','OLD_DATA','NEW_DATA'),
        ('ram_count','snapshot/snapshot_fit.fit.summary','Total RAM Blocks : 6','Total RAM Blocks : 7'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('warning','snapshot/map.log',None,None),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-word-control-',dir='/tmp')as temp:
            out=Path(temp)/'evidence';shutil.copytree(directory,out);p=out/path;text=p.read_text()
            if before:assert before in text; text=text.replace(before,after)
            else:text+='\nWarning (10999): unreviewed inference\n'
            p.write_text(text)
            try:audit(out)
            except AssertionError as error:rows.append(dict(case=name,rejected=True,diagnostic=str(error)))
            else:raise AssertionError('damaged RAM fit evidence accepted: '+name)
    return rows


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--prepare',type=Path)
    parser.add_argument('--run',type=Path);parser.add_argument('--audit',type=Path);parser.add_argument('--output',type=Path)
    parser.add_argument('--negative-controls',action='store_true')
    parser.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    args=parser.parse_args()
    if args.prepare:prepare(args.prepare.resolve())
    elif args.run:run(args.run.resolve(),args.quartus)
    elif args.audit and args.output:
        result=audit(args.audit.resolve())
        if args.negative_controls:result['failure_controls']=controls(args.audit.resolve())
        args.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
    else:parser.error('Specify --prepare, --run or --audit/--output')
