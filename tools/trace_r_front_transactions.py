#!/usr/bin/env python3
"""Record full transactions only after the unchanged original references pass.

The existing cached/pure comparison, all844-Q/9216-RAM checks, all clock and
capture assertions, retries and reset tests stay intact. Passive snapshots
record every input epoch, all30 result ports and the independently checked
post-commit SRAM image. Replay can then combine adjacent events while retaining
an exact original-graph oracle, rather than comparing only two new functions.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha
from test_r_front_pure_evaluator import outputs


def port_map(raw,tb):
    top=module(raw,'dorado_r_front_control_step')
    header=top[top.index(' input wire host_clk'):top.index('\n);')]
    names=[re.search(r'(\w+)\s*$',v)[1] for v in header.replace('\n',' ').split(',')]
    args=[v.strip() for v in tb.split(' dorado_r_front_control_step dut(',1)[1].split(');',1)[0].replace('\n','').split(',')]
    assert len(names)==len(args);return dict(zip(names,args))


def layout(raw):
    ports=[(n,w) for n,w in outputs(module(raw,'dorado_r_front_pure_step')) if n!='cp_ready']
    pos=0;result={}
    for n,w in reversed(ports):
        size=1 if not w else int(w[1:].split(':')[0])+1
        result[n]=dict(low=pos,high=pos+size-1,bits=size);pos+=size
    assert pos==2470;return ports,result


def run(source,out):
    assert not out.exists();s=json.loads((source/'run-state.json').read_text())
    assert s['status']=='PASS_FULL_PROCESSOR_STATELESS_EVALUATOR'
    raw=(source/'candidate.sv').read_text();tb=(source/'inputs/tb_r_front_local.sv').read_text()
    ports,bits=layout(raw);names=port_map(raw,tb);pack='{'+','.join(names[n] for n,w in ports)+'}'
    declarations='''
 integer trace_file,trace_initial;reg [2469:0] trace_result;
 reg [13333:0] trace_record;reg [10059:0] trace_init_record;
 reg [843:0] trace_owner;reg trace_before,trace_after;
 reg [400:0] trace_pb,trace_pa;
 initial begin
  trace_file=$fopen("TRACE_FILE","w");trace_initial=$fopen("INITIAL_FILE","w");
  if(!trace_file || !trace_initial)$fatal(1,"Could not open passive transaction trace");
 end
'''.replace('TRACE_FILE',str(out/'transactions.hex')).replace('INITIAL_FILE',str(out/'initial.hex'))
    sample='     expected=n;predicted=events;'
    capture=f'     trace_result={pack};trace_owner=stored_q;trace_before=cb;trace_after=ca;trace_pb=pb;trace_pa=pa;\n'
    initial='   stored_q=fixture;monitor=1;p=SEED;p[19:0]=20\'h178;'
    initial_trace='\n   trace_init_record={stored_q,memory_image()};$fdisplay(trace_initial,"%h%h",trace_init_record[10059:8192],trace_init_record[8191:0]);$fclose(trace_initial);'
    commit='     checks=checks+1;'
    emit='     trace_record={trace_owner,trace_before,trace_after,trace_pb,trace_pa,trace_result,actual_memory};$fdisplay(trace_file,"%h%h",trace_record[13333:8192],trace_record[8191:0]);\n'
    marker=' dorado_r_front_control_step dut('
    for token in [sample,initial,commit,marker]:assert tb.count(token)==1
    new=tb.replace(marker,declarations+marker,1).replace(sample,capture+sample,1).replace(initial,initial+initial_trace,1).replace(commit,emit+commit,1)
    assert new.replace(declarations,'',1).replace(capture,'',1).replace(initial_trace,'',1).replace(emit,'',1)==tb
    out.mkdir(parents=True);paths=[];rows=[]
    for row in s['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256']
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(new if p.name=='tb_r_front_local.sv' else p.read_text())
        paths.append(q);rows.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q)))
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,source=str(source),
        source_state_sha256=sha(source/'run-state.json'),source_files=rows,result_layout=bits,
        record_bits=13334,result_bits=2470,original_assertions_and_references_retained=True,actions=[],hardware_changed=False)

    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')

    defines=[v for v in s['actions'][0]['command'] if v.startswith('-D')]
    commands=[['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,'--Mdir',str(out/'obj'),'-o','test',*map(str,paths)],
              [str(out/'obj/test')]]
    save()
    for label,cmd in zip(['compile','test'],commands):
        with (out/(label+'.log')).open('x') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(label=label,command=cmd,pid=p.pid,started=time.time())
            state['actions'].append(row);save();code=p.wait();row.update(returncode=code,finished=time.time());save()
        if code:break
    text=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    passed=code==0 and 'checks=14884 writes=238 reads=6928 epochs=392' in text
    if passed:
        lines=(out/'transactions.hex').read_text().splitlines();assert len(lines)==14884
        assert all(len(v)==(13334+3)//4 and re.fullmatch('[0-9a-fA-F]+',v) for v in lines)
        assert len((out/'initial.hex').read_text().splitlines())==1
        for token in ['R_FRONT_CONTROL_CLOCK','CP_PREFIX_CACHE','CONTROL_FALL_COVERAGE']:
            pattern='^'+token+' .*';assert re.findall(pattern,text,re.M)==re.findall(pattern,(source/'test.log').read_text(),re.M)
        state.update(records=len(lines),trace_sha256=sha(out/'transactions.hex'),initial_sha256=sha(out/'initial.hex'))
    state.update(status='PASS_FULL_ORIGINAL_PROCESSOR_TRANSACTION_TRACE' if passed else 'TERMINAL_PROCESSOR_TRACE_NOT_ESTABLISHED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve()))
