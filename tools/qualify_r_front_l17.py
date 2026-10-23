#!/usr/bin/env python3
"""Qualify actual ContA l17 feedback across the inherited six arrival profiles.

New 798-Q comparisons use original package/pin circuitry for every profile.
Wrong external feedback and wrong Clock0 variants must fail after compilation.
This does not qualify remaining producers, native boot, timing or 60 ns speed.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor,as_completed
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import threading
import time
from test_r_front_l17 import visible

ROOT=Path(__file__).resolve().parents[1]


def run(base,out):
    out.mkdir(parents=True,exist_ok=False);state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),cases=[]);lock=threading.Lock()
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    cases=[(p,e,None)for p in visible.masked.commands.parent.PROFILES for e in ['icarus','verilator']]
    cases += [('balanced',e,m)for m in ['external','clock0']for e in ['icarus','verilator']]
    pilots={('balanced','icarus',None):'r-front-l17-icarus-v1-20261023',
            ('balanced','verilator',None):'r-front-l17-verilator-v1-20261023',
            ('balanced','verilator','external'):'r-front-l17-external-verilator-v1-20261023'}
    def execute(case):
        profile,engine,mutation=case;name=profile+'-'+engine+'-'+(mutation or 'original');d=out/name
        cmd=[sys.executable,str(ROOT/'tools/test_r_front_l17.py'),'--base',str(base),'--output',str(d),'--engine',engine,'--profile',profile]
        if mutation:cmd+=['--mutation',mutation]
        if case in pilots:
            d=out.parent/pilots[case]
            previous=json.loads((d/'run-state.json').read_text())
            assert previous['status']!='RUNNING' and len(previous['actions'])==2
            assert previous['profile']==profile and previous['engine']==engine
            expected_sha={None:'665188a29d07ba22981061233a825f496a479d577f5a6794a6b81cfac8c418d2',
                          'external':'1f305a950c8cff5c7c7e8b384a4f025966ff87b5a740928cebb2d3301d351691',
                          'clock0':'267f75e2f719e559ad97d90038c200fde3220c7b97d748b8e6b5ae4e332941e7'}[mutation]
            assert previous['executed_candidate_sha256']==expected_sha
            assert hashlib.sha256((d/'candidate.sv').read_bytes()).hexdigest()==expected_sha
            rc=previous['actions'][-1]['returncode']
            row=dict(profile=profile,engine=engine,mutation=mutation,directory='../'+d.name,
                     reused_terminal_run=True,original_test_commands=[r['command']for r in previous['actions']],
                     pid=previous['pid'],started=previous['started'],finished=previous['finished'],returncode=rc)
            with lock:state['cases'].append(row);save()
        else:
            with (out/(name+'.log')).open('w')as log:
                p=subprocess.Popen(cmd,cwd=ROOT,stdout=log,stderr=subprocess.STDOUT)
                row=dict(profile=profile,engine=engine,mutation=mutation,directory=name,command=cmd,pid=p.pid,started=time.time())
                with lock:state['cases'].append(row);save()
                rc=p.wait()
            with lock:row.update(returncode=rc,finished=time.time());save()
        r=json.loads((d/'run-state.json').read_text());assert r['status']!='RUNNING' and len(r['actions'])==2 and r['actions'][0]['returncode']==0,d
        for n,h in r['inputs'].items():assert hashlib.sha256((d/'inputs'/n).read_bytes()).hexdigest()==h,(d,n)
        text=(d/'test.log').read_text()
        if mutation:
            assert rc!=0 and r['status']=='R_FRONT_LOCAL_PROBE_FAILED',d
            assert re.search(r'R front (?:state|clock) mismatch check=',text),d
            row['qualification']='REJECTED_L17_'+mutation.upper()
        else:
            assert rc==0 and r['status']=='PASS_R_FRONT_LOCAL_PROBE',d
            assert r['summary']=='checks=13932 writes=328 reads=5800 epochs=797',d
            assert r['executed_candidate_sha256']=='665188a29d07ba22981061233a825f496a479d577f5a6794a6b81cfac8c418d2'
            assert 'R_FRONT_L17_DIRECTED changed=3f 'in text
            clocks=re.findall(r'^R_FRONT_LOCAL_CLOCK channel=(\d+) edges=(\d+)$',text,re.M)
            assert len(clocks)==69 and {int(c)for c,n in clocks}==set(range(69)) and all(int(n)>0 for c,n in clocks)
            assert 'MIR_MASK_LATE_NODISPATCH root=0 MIRcaptures=1 SwitchUp_n=1'in text
            assert 'R_FRONT_COMMANDS_DIRECTED commands=8 FF=12 JCN=4 intervals=120 WriteIM=16'in text
            row.update(qualification='PASS_BOUNDED_L17_ORDERING',summary=r['summary'],clocks=clocks)
        row['run_state_sha256']=hashlib.sha256((d/'run-state.json').read_bytes()).hexdigest()
        with lock:save()
        print(row['qualification'],name,flush=True)
    failures=[]
    with ThreadPoolExecutor(max_workers=3)as pool:
        futures={pool.submit(execute,c):c for c in cases if c not in pilots}
        for f in as_completed(futures):
            try:f.result()
            except Exception as e:failures.append(dict(case=futures[f],error=repr(e)));print('QUALIFICATION_FAILURE',futures[f],repr(e),flush=True)
    for case in pilots:
        try:execute(case)
        except Exception as e:failures.append(dict(case=case,error=repr(e)));print('PILOT_QUALIFICATION_FAILURE',case,repr(e),flush=True)
    primary=[r for r in state['cases']if r.get('qualification')=='PASS_BOUNDED_L17_ORDERING']
    for profile in visible.masked.commands.parent.PROFILES:
        pair=[r for r in primary if r['profile']==profile]
        if len(pair)!=2 or pair[0]['clocks']!=pair[1]['clocks']:failures.append(dict(profile=profile,error='Missing pass or engine event counts differ'))
    state.update(status='L17_ORDERING_FAILED'if failures else'PASS_BOUNDED_R_FRONT_L17_ORDERING',finished=time.time(),failures=failures,primary=primary,
                 negative_cases=[r for r in state['cases']if r.get('qualification','').startswith('REJECTED_L17_')],physical_bits=798,ram_bits=9216,clock_channels=69,
                 full_producer_arrival_contract_qualified=False,physical_timing_qualified=False,native_integrated=False,hardware_speedup_claimed=False)
    save();(out/'results.json').write_text(json.dumps(state,indent=2)+'\n');print(state['status'],flush=True);return bool(failures)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--base',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();sys.exit(run(a.base.resolve(),a.output.resolve()))
