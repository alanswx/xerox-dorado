#!/usr/bin/env python3
"""Matched isolated MosRam14/16 fits and read-only all-corner/RAM audit."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

CORNERS={'7_slow_1100mv_-40c','7_slow_1100mv_100c','MIN_fast_1100mv_-40c','MIN_fast_1100mv_100c'}
HERE=Path(__file__).resolve().parent
TOP='''module snapshot_fit(
 input wire host_clk,advance,input wire [7:0] address,
 input wire ras_n,cas_n,we_n,data,freeze,held,request,write,
 input wire [16:0] index,input wire [17:0] state_data,
 output reg [1:0] q,frozen,done,error,output reg [35:0] result);
 // Matched negative-edge external staging; positive-edge embedded neighbors.
 reg [51:0] input_stage,launch;
 always @(negedge host_clk)input_stage<={advance,address,ras_n,cas_n,we_n,data,freeze,held,request,write,index,state_data};
 always @(posedge host_clk)launch<=input_stage;
 wire step_r,ras_r,cas_r,we_r,data_r,freeze_r,held_r,request_r,write_r;
 wire [7:0] address_r;wire [16:0] index_r;wire [17:0] state_r;
 assign {step_r,address_r,ras_r,cas_r,we_r,data_r,freeze_r,held_r,request_r,write_r,index_r,state_r}=launch;
 wire [1:0] bank_q,bank_frozen,bank_done,bank_error;wire [35:0] bank_result;
 always @(posedge host_clk){q,frozen,done,error,result}<={bank_q,bank_frozen,bank_done,bank_error,bank_result};
@BANKS@
endmodule
'''


def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def top():
    code=[]
    for k,a in enumerate((14,16)):
        code.append(f'''`ifdef SNAPSHOT_BANK_EXPERIMENT
 cell_MosRam_snapshot #(.ADDR_BITS({a})) bank_{a}(
 .state_host_clk(host_clk),.state_advance(step_r),.state_freeze(freeze_r),.state_guest_held(held_r),
 .state_request(request_r),.state_write(write_r),.state_index(index_r[{a}:0]),.state_data(state_r),
 .state_frozen(bank_frozen[{k}]),.state_done(bank_done[{k}]),.state_error(bank_error[{k}]),.state_result(bank_result[{k*18}+:18]),
`else
 assign {{bank_frozen[{k}],bank_done[{k}],bank_error[{k}],bank_result[{k*18}+:18]}}=21'b0;
 cell_MosRam #(.ADDR_BITS({a})) bank_{a}(
`endif
 .sys_clk(host_clk),.p2(data_r),.p3(we_r),.p4(ras_r),.p5(address_r[1]),.p6(address_r[3]),.p7(address_r[2]),
 .p9(address_r[7]),.p10(address_r[6]),.p11(address_r[5]),.p12(address_r[4]),.p13(address_r[0]),
 .p15(cas_r),.p14(bank_q[{k}]),.p1(1'b0),.p8(1'b0),.p16(1'b0));''')
    return TOP.replace('@BANKS@','\n'.join(code))


def prepare(output):
    import generate_mosram as gen
    from fit_f10415 import AUDIT
    from fit_f10415_enable import SDC
    output.mkdir(parents=True,exist_ok=False);candidate=gen.generate()
    original=gen.ROOT/'verilog/cells/cell_MosRam.v'
    for variant in ('baseline','snapshot'):
        case=output/variant;case.mkdir();shutil.copyfile(original,case/'cell_MosRam.v')
        files={'top.sv':top(),'cell_MosRam_snapshot.sv':candidate,'bank.sdc':SDC,'audit.tcl':AUDIT,
               'snapshot_fit.qpf':'PROJECT_REVISION = "snapshot_fit"\n'}
        for n,s in files.items():(case/n).write_text(s)
        qsf='''set_global_assignment -name FAMILY "Cyclone V"
set_global_assignment -name DEVICE 5CSEBA6U23I7
set_global_assignment -name TOP_LEVEL_ENTITY snapshot_fit
set_global_assignment -name MIN_CORE_JUNCTION_TEMP -40
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 100
set_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS OFF
set_global_assignment -name OPTIMIZATION_MODE "HIGH PERFORMANCE EFFORT"
set_global_assignment -name OPTIMIZE_HOLD_TIMING ALL_PATHS
set_global_assignment -name NUM_PARALLEL_PROCESSORS 4
set_global_assignment -name SDC_FILE bank.sdc
set_global_assignment -name SYSTEMVERILOG_FILE top.sv
set_global_assignment -name SYSTEMVERILOG_FILE cell_MosRam_snapshot.sv
set_global_assignment -name VERILOG_FILE cell_MosRam.v
set_location_assignment PIN_V11 -to host_clk
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to host_clk
'''
        for p in ['advance','address[*]','ras_n','cas_n','we_n','data','freeze','held','request','write','index[*]','state_data[*]',
                  'q[*]','frozen[*]','done[*]','error[*]','result[*]']:qsf+='set_instance_assignment -name VIRTUAL_PIN ON -to '+p+'\n'
        if variant=='snapshot':qsf+='set_global_assignment -name VERILOG_MACRO "SNAPSHOT_BANK_EXPERIMENT=1"\n'
        (case/'snapshot_fit.qsf').write_text(qsf)
    paths=[Path(__file__).resolve(),HERE/'generate_mosram.py',HERE/'generate_contb_full.py',HERE/'generate_contb.py',
        HERE/'audit_contb_im.py',HERE/'generate_snapshot_ff.py',HERE/'fit_f10415.py',HERE/'fit_f10415_enable.py',original]
    for p in paths:
        dest=output/'executed-sources'/p.name;dest.parent.mkdir(exist_ok=True);shutil.copyfile(p,dest)
    p=dict(scope='Two isolated original MosRam sockets14/16 vs independent exact-advance/state adapters; matched50MHz staged-input/positive-edge neighbors,20ns embedded paths. Counts include shell/admin/pruning; no full MemX/global barrier/physical FPGA save/load or speed qualification.',
        driver_sha256=digest(Path(__file__)),schemas={str(a):gen.manifest(a)for a in (14,16)},
        generated_sha256=hashlib.sha256(candidate.encode()).hexdigest(),original_sha256=gen.ORIGINAL_SHA,
        source_hashes={str(p):digest(p)for p in paths},
        compiled_files={str(p.relative_to(output)):digest(p)for p in output.rglob('*')if p.is_file()},
        expected_qsf={v:(output/v/'snapshot_fit.qsf').read_text()for v in ('baseline','snapshot')})
    (output/'provenance.json').write_text(json.dumps(p,indent=2)+'\n')


def run(directory,quartus):
    p=json.loads((directory/'provenance.json').read_text());assert digest(Path(__file__))==p['driver_sha256'],'prepared driver changed'
    for name,h in p['compiled_files'].items():assert digest(directory/name)==h,'prepared input changed:'+name
    for v in ('baseline','snapshot'):
        c=directory/v;assert not(c/'map.log').exists(),'inspect existing job/evidence, do not restart'
        for tool,args,label in [('quartus_map',['snapshot_fit'],'map'),('quartus_fit',['snapshot_fit'],'fit'),
            ('quartus_sta',['snapshot_fit'],'sta'),('quartus_sta',['-t','audit.tcl'],'corners')]:
            print(v,label,flush=True)
            with(c/(label+'.log')).open('w')as log:subprocess.run([str(quartus/tool),*args],cwd=c,stdout=log,stderr=subprocess.STDOUT,check=True)
    print('MosRam fits terminal; audit sources/RAM/all-corner timing before qualification.',flush=True)


def macro_rows(text):
    headers=list(re.finditer(r'(?m)^;\s*Parameter Settings for (?:User|Inferred) Entity Instance:\s*([^\n;]+)',text));rows=[]
    for i,h in enumerate(headers):
        m=re.search(r'\bbank_(14|16)\|',h[1])
        if not m or 'altsyncram:'not in h[1]:continue
        a=int(m[1]);section=text[h.end():headers[i+1].start()if i+1<len(headers)else len(text)];values={}
        # The exact original has no RAM-style attribute; both templates use
        # AUTO inference. The physical table below MUST still prove M10K.
        for key,wanted in [('OPERATION_MODE','DUAL_PORT'),('RAM_BLOCK_TYPE','AUTO'),('WIDTH_A','1'),('WIDTH_B','1'),
            ('NUMWORDS_A',str(1<<a)),('NUMWORDS_B',str(1<<a)),('READ_DURING_WRITE_MODE_MIXED_PORTS','OLD_DATA')]:
            actual=re.findall(r'; '+key+r'\s*; (\w+)\s*;',section);assert actual==[wanted],('MosRam macro differs',a,key,actual);values[key]=wanted
        rows.append(dict(address_bits=a,instance=h[1].strip(),parameters=values))
    assert len(rows)==2 and {r['address_bits']for r in rows}=={14,16},'missing/extra MosRam macro'
    return rows


def physical_rows(text):
    i=text.find('; Fitter RAM Summary');assert i>=0,'missing physical RAM table'
    table=text[i:].split('\n\n',1)[0];header=None;rows=[]
    for l in table.splitlines():
        if not l.startswith('; '):continue
        f=[x.strip()for x in l.split(';')[1:-1]]
        if f and f[0]=='Name':header=f
        elif f and re.search(r'\bbank_(14|16)\|',f[0]):
            assert header and len(header)==len(f),'bad physical row';rows.append(dict(zip(header,f)))
    assert len(rows)==2,'physical bank count differs';seen=set()
    for r in rows:
        a=int(re.search(r'\bbank_(14|16)\|',r['Name'])[1]);assert a not in seen,'duplicate physical bank';seen.add(a)
        wanted={'Type':'AUTO','Mode':'Simple Dual Port','Clock Mode':'Single Clock','Size':str(1<<a),'Implementation Bits':str(1<<a),
            'Port A Depth':str(1<<a),'Port B Depth':str(1<<a),'Port A Width':'1','Port B Width':'1','M10K blocks':str((1<<a)//8192),'MLAB cells':'0',
            'Implementation Port A Depth':str(1<<a),'Implementation Port B Depth':str(1<<a),'Implementation Port A Width':'1','Implementation Port B Width':'1',
            'Port A Input Registers':'yes','Port B Input Registers':'yes','Port A Output Registers':'no','Port B Output Registers':'no','Mixed Width RDW Mode':'Old data'}
        for k,v in wanted.items():assert r.get(k)==v,('physical MosRam differs',a,k,r.get(k))
        locations=[x.strip()for x in r['Location'].split(',')]
        assert len(locations)==(1<<a)//8192 and len(set(locations))==len(locations),'physical M10K location count differs'
        assert all(re.fullmatch(r'M10K_X\d+_Y\d+_N\d+',x)for x in locations),'non-M10K physical placement'
    return rows


def normalized(text):return '\n'.join(l for l in text.splitlines()if l.strip()and l!='set_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"')


def audit(directory):
    p=json.loads((directory/'provenance.json').read_text());result={}
    assert {k:(v['address_bits'],v['modeled_state_bits'])for k,v in p['schemas'].items()}=={'14':(14,16405),'16':(16,65559)}
    for n,h in p['compiled_files'].items():
        if not n.endswith('.qsf'):assert digest(directory/n)==h,'compiled source/SDC differs:'+n
    for v in ('baseline','snapshot'):
        c=directory/v;assert normalized((c/'snapshot_fit.qsf').read_text())==normalized(p['expected_qsf'][v]),'QSF/macro changed'
        summary=(c/'snapshot_fit.fit.summary').read_text();assert 'Fitter Status : Successful'in summary
        resources={}
        for key,label in [('alms',r'Logic utilization \(in ALMs\)'),('registers','Total registers'),('ram_bits','Total block memory bits'),('ram_blocks','Total RAM Blocks')]:
            m=re.search(label+r' : ([\d,]+)',summary);assert m,'missing resource'+key;resources[key]=int(m[1].replace(',',''))
        assert resources['ram_bits']==81920 and resources['ram_blocks']==10,'RAM geometry/packing differs'
        assert {f.name[len('bank-setup-'):-4]for f in c.glob('bank-setup-*.rpt')}==CORNERS,'missing corner'
        timing={}
        for corner in sorted(CORNERS):
            timing[corner]={}
            for mode in ('setup','hold','mpw'):
                s=(c/f'bank-{mode}-{corner}.rpt').read_text();m=re.search(r'\((\d+) violated\)\.\s+Worst case slack is (-?[\d.]+)',s)
                assert m and int(m[1])==0 and float(m[2])>=0,'timing violation:'+v+'/'+mode+'/'+corner
                timing[corner][mode]=float(m[2])
            for mode in ('recovery','removal'):assert(c/f'bank-{mode}-{corner}.rpt').read_text().strip().endswith('Nothing to report.'),'unexpected async path'
            s=(c/f'bank-ucp-{corner}.rpt').read_text()
            for label in ('Illegal Clocks','Unconstrained Clocks','Unconstrained Input Ports','Unconstrained Input Port Paths','Unconstrained Output Ports','Unconstrained Output Port Paths'):
                assert re.search('; '+label+r'\s*; 0\s*; 0\s*;',s),'unconstrained:'+label
        warnings={}
        for label in ('map','fit'):
            s=(c/(label+'.log')).read_text();assert 'Critical Warning'not in s,'critical warning';warnings[label]=dict(Counter(re.findall(r'Warning \((\d+)\)',s)))
        result[v]=dict(resources=resources,timing=timing,warnings=warnings,macros=macro_rows((c/'snapshot_fit.map.rpt').read_text()),physical=physical_rows((c/'snapshot_fit.fit.rpt').read_text(errors='replace')))
    for label in ('map','fit'):
        new=Counter(result['snapshot']['warnings'][label])-Counter(result['baseline']['warnings'][label]);assert not(set(new)-{'10036','13024','13410'}),('unreviewed warning increase',label,new)
    return dict(status='PASS',scope=p['scope'],results=result,
        delta={k:result['snapshot']['resources'][k]-result['baseline']['resources'][k]for k in ('alms','registers','ram_bits','ram_blocks')},
        minimum_snapshot_slack_ns=min(x for row in result['snapshot']['timing'].values()for x in row.values()))


def controls(directory):
    rows=[]
    for name,path,before,after in [
        ('source','snapshot/cell_MosRam_snapshot.sv','restore_pending<=1\'b0;',"restore_pending<=1'b1;"),
        ('constraint','snapshot/bank.sdc','20.000','18.000'),
        ('macro','snapshot/snapshot_fit.qsf','SNAPSHOT_BANK_EXPERIMENT=1','SNAPSHOT_BANK_EXPERIMENT=0'),
        ('old_data','snapshot/snapshot_fit.map.rpt','OLD_DATA','NEW_DATA'),
        ('physical_old_data','snapshot/snapshot_fit.fit.rpt','; Old data','; New data'),
        ('ram_count','snapshot/snapshot_fit.fit.summary','Total RAM Blocks : 10','Total RAM Blocks : 11'),
        ('hold','snapshot/bank-hold-7_slow_1100mv_-40c.rpt','(0 violated).','(1 violated).'),
        ('warning','snapshot/map.log',None,None),
    ]:
        with tempfile.TemporaryDirectory(prefix='dorado-mosram-control-',dir='/tmp')as tmp:
            d=Path(tmp)/'evidence';shutil.copytree(directory,d,ignore=shutil.ignore_patterns('db','incremental_db'));f=d/path;s=f.read_text(errors='replace')
            if before:assert before in s,'missing fault anchor:'+name;s=s.replace(before,after)
            else:s+='\nWarning (10999): unreviewed inference\n'
            f.write_text(s)
            try:audit(d)
            except AssertionError as e:rows.append(dict(case=name,rejected=True,diagnostic=str(e)))
            else:raise AssertionError('damaged MosRam evidence accepted:'+name)
    return rows


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--prepare',type=Path);p.add_argument('--run',type=Path)
    p.add_argument('--audit',type=Path);p.add_argument('--output',type=Path);p.add_argument('--negative-controls',action='store_true')
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'));a=p.parse_args()
    if a.prepare:prepare(a.prepare.resolve())
    elif a.run:run(a.run.resolve(),a.quartus)
    elif a.audit and a.output:
        q=audit(a.audit.resolve())
        if a.negative_controls:q['failure_controls']=controls(a.audit.resolve())
        a.output.write_text(json.dumps(q,indent=2)+'\n');print(json.dumps(q,indent=2))
    else:p.error('Specify --prepare, --run, or --audit/--output')
