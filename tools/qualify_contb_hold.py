#!/usr/bin/env python3
"""Review coupled MemC-produced Hold/ContB capture ordering in both engines.

Three illustrative delay profiles, 96 real held-data release cases per pass,
and seven wrong-source/epoch/clock variants. Upstream CA/MCR and hold-data
remain boundaries; neither full-machine arrivals nor physical timing is proven.
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
from test_contb_hold import PROFILES,MUTATIONS,bench

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out):
    out.mkdir(parents=True,exist_ok=False);own=out/'qualify_contb_hold.py';own.write_bytes(Path(__file__).read_bytes())
    result=dict(status='RUNNING',pid=os.getpid(),started=time.time(),cases=[],review_source_sha256=sha(own));lock=threading.Lock()
    def save():(out/'run-state.json').write_text(json.dumps(result,indent=2)+'\n')
    save();cases=[(p,e,None)for p in PROFILES for e in ['icarus','verilator']]
    cases += [('balanced',e,m)for m in MUTATIONS for e in ['icarus','verilator']]
    pilots={('balanced','icarus',None):'contb-hold-icarus-v2-20261023',
            ('balanced','verilator',None):'contb-hold-verilator-v1-20261023'}
    # A fresh checkout can run the complete matrix without generated parents.
    # If a named pilot exists, audit that exact handle/result rather than
    # silently replacing an incomplete or failed observation with another run.
    pilots={case:name for case,name in pilots.items()if(out.parent/name/'run-state.json').exists()}
    expected_tb=hashlib.sha256(bench().encode()).hexdigest()
    def execute(case):
        profile,engine,mutation=case;name=profile+'-'+engine+'-'+(mutation or 'original');d=out/name
        if case in pilots:
            d=out.parent/pilots[case];previous=json.loads((d/'run-state.json').read_text())
            assert previous['status']!='RUNNING'and len(previous['actions'])==2
            rc=previous['actions'][-1]['returncode']
            row=dict(profile=profile,engine=engine,mutation=mutation,directory='../'+d.name,
                     reused_terminal_run=True,pid=previous['pid'],started=previous['started'],finished=previous['finished'],returncode=rc,
                     commands=[r['command']for r in previous['actions']])
            with lock:result['cases'].append(row);save()
        else:
            cmd=[sys.executable,str(ROOT/'tools/test_contb_hold.py'),'--output',str(d),'--engine',engine,'--profile',profile]
            if mutation:cmd+=['--mutation',mutation]
            with(out/(name+'.log')).open('w')as log:
                p=subprocess.Popen(cmd,cwd=ROOT,stdout=log,stderr=subprocess.STDOUT)
                row=dict(profile=profile,engine=engine,mutation=mutation,directory=name,pid=p.pid,command=cmd,started=time.time())
                with lock:result['cases'].append(row);save()
                rc=p.wait()
            with lock:row.update(returncode=rc,finished=time.time());save()
        r=json.loads((d/'run-state.json').read_text())
        assert r['status']!='RUNNING'and len(r['actions'])==2 and r['actions'][0]['returncode']==0,(name,r['status'])
        assert r['engine']==engine and r['profile']==profile and r.get('mutation')==mutation,name
        assert r['actions'][-1]['returncode']==rc,name
        for n,h in r['inputs'].items():assert sha(d/'inputs'/n)==h,(name,n)
        for n,h in r['compiled_sources'].items():assert sha(d/n)==h,(name,n)
        for n,h in r['timing']['files'].items():assert sha(d/'timed'/n)==h,(name,n)
        assert r['testbench_sha256']==expected_tb and sha(d/'tb.sv')==expected_tb,name
        assert sha(d/'candidate.sv')==r['executed_candidate_sha256'],name
        assert sha(d/'candidate-cb.sv')=='a793d9bb15bb12bdab9b0048b94ef69462ac668498b020b890a952ebed102fea'
        text=(d/'test.log').read_text()
        if mutation:
            assert rc!=0 and r['status']=='CONTB_HOLD_PROBE_FAILED',name
            failure=re.search(r'ContB hold (state|flags|clock) mismatch check=(\d+)',text);assert failure,(name,text)
            assert r['executed_candidate_sha256']!=r['candidate']['output_sha256'],name
            row.update(qualification='REJECTED_'+mutation.upper(),first_failure_kind=failure[1],first_failure_check=int(failure[2]))
        else:
            assert rc==0 and r['status']=='PASS_CONTB_HOLD_PROBE',name
            assert r['summary']=='checks=7260 physical=46 clocks=31 releases=96',(name,r['summary'])
            assert r['executed_candidate_sha256']==r['candidate']['output_sha256'],name
            assert 'CONTB_HOLD_RELEASE actual_sources=3 stable_data_cases=96 no_early_MIR_edge=1'in text,name
            clocks=re.findall(r'^CONTB_HOLD_CLOCK channel=(\d+) edges=(\d+)$',text,re.M)
            assert len(clocks)==31 and {int(c)for c,n in clocks}==set(range(31))and all(int(n)>=16 for c,n in clocks),name
            row.update(qualification='PASS_BOUNDED_MEMORY_PRODUCED_HOLD',summary=r['summary'],clocks=clocks)
        row.update(run_state_sha256=sha(d/'run-state.json'),candidate_sha256=r['executed_candidate_sha256'])
        with lock:save()
        print(row['qualification'],name,flush=True)
    failures=[]
    with ThreadPoolExecutor(max_workers=2)as pool:
        futures={pool.submit(execute,c):c for c in cases if c not in pilots}
        for f in as_completed(futures):
            try:f.result()
            except Exception as error:failures.append(dict(case=futures[f],error=repr(error)));print('REVIEW_FAILURE',futures[f],repr(error),flush=True)
    for case in pilots:
        try:execute(case)
        except Exception as error:failures.append(dict(case=case,error=repr(error)));print('PILOT_REVIEW_FAILURE',case,repr(error),flush=True)
    passes=[r for r in result['cases']if r.get('qualification')=='PASS_BOUNDED_MEMORY_PRODUCED_HOLD']
    for profile in PROFILES:
        pair=[r for r in passes if r['profile']==profile]
        if len(pair)!=2 or pair[0]['clocks']!=pair[1]['clocks']:failures.append(dict(profile=profile,error='Missing pass or clock counts differ'))
    result.update(status='CONTB_HOLD_REVIEW_FAILED'if failures else'PASS_BOUNDED_MEMORY_PRODUCED_HOLD',finished=time.time(),failures=failures,
                  primary=passes,negative_cases=[r for r in result['cases']if r.get('qualification','').startswith('REJECTED_')],
                  full_producer_arrival_contract_qualified=False,native_integrated=False,physical_timing_qualified=False,hardware_speedup_claimed=False,
                  remaining_boundaries='Original CA enables/MemSH/BNT/UseDMD/reset; MCR DisHold and actual hold-data; IM/parity/DMD; analog skew')
    save();(out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True);return bool(failures)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    sys.exit(run(p.parse_args().output.resolve()))
