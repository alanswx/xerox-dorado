#!/usr/bin/env python3
"""Review bounded ContB capture comparisons and the unresolved clock handoff.

Three illustrative arrival profiles compare both event simulators. Six wrong
logic variants must fail after compiling, and the simultaneous root-rise /
RepeatCur-release gap must reproduce in both engines. Passing this review is
not full producer-arrival, native, physical timing, or speed qualification.
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
from test_contb_mir import PROFILES,MUTATIONS

ROOT=Path(__file__).resolve().parents[1]


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out):
    out.mkdir(parents=True,exist_ok=False)
    own=out/'qualify_contb_mir.py';own.write_bytes(Path(__file__).read_bytes())
    result=dict(status='RUNNING',pid=os.getpid(),started=time.time(),cases=[],review_source_sha256=sha(own))
    lock=threading.Lock()
    def save():(out/'run-state.json').write_text(json.dumps(result,indent=2)+'\n')
    save()
    cases=[(p,e,None,False)for p in PROFILES for e in ['icarus','verilator']]
    cases += [('balanced',e,m,False)for m in MUTATIONS for e in ['icarus','verilator']]
    cases += [('balanced',e,None,True)for e in ['icarus','verilator']]
    def execute(case):
        profile,engine,mutation,hazard=case
        name=profile+'-'+engine+'-'+('handoff-gap'if hazard else mutation or 'original')
        d=out/name;cmd=[sys.executable,str(ROOT/'tools/test_contb_mir.py'),'--output',str(d),'--engine',engine,'--profile',profile]
        if mutation:cmd+=['--mutation',mutation]
        if hazard:cmd+=['--hazard-case']
        with (out/(name+'.log')).open('w')as log:
            p=subprocess.Popen(cmd,cwd=ROOT,stdout=log,stderr=subprocess.STDOUT)
            row=dict(profile=profile,engine=engine,mutation=mutation,hazard_case=hazard,directory=name,pid=p.pid,command=cmd,started=time.time())
            with lock:result['cases'].append(row);save()
            rc=p.wait()
        with lock:row.update(returncode=rc,finished=time.time());save()
        r=json.loads((d/'run-state.json').read_text())
        assert r['status']!='RUNNING' and len(r['actions'])==2 and r['actions'][0]['returncode']==0,(name,r)
        assert r['actions'][1]['returncode']==rc,(name,rc,r['actions'])
        assert r['engine']==engine and r['profile']==profile and r['mutation']==mutation and r['hazard_case']==hazard,name
        for n,h in r['inputs'].items():assert sha(d/'inputs'/n)==h,(name,n)
        for n,h in r['compiled_sources'].items():assert sha(d/n)==h,(name,n)
        for n,h in r['timing']['files'].items():assert sha(d/'timed'/n)==h,(name,n)
        assert sha(d/'candidate.sv')==r['executed_candidate_sha256'] and sha(d/'tb.sv')==r['testbench_sha256']
        text=(d/'test.log').read_text()
        if mutation or hazard:
            assert rc!=0 and r['status']=='CONTB_MIR_PROBE_FAILED',name
            failure=re.search(r'ContB MIR (state|flags|clock) mismatch check=(\d+)',text)
            assert failure,(name,text)
            row.update(qualification='EXPOSED_CLOCK_HANDOFF_GAP'if hazard else'REJECTED_WRONG_'+mutation.upper(),
                       first_failure_kind=failure[1],first_failure_check=int(failure[2]))
            if hazard:
                assert failure[2]=='34' and 'CONTB_CLOCK_DIFF check=34 channel=1 actual=1 expected=0'in text,(name,text)
                assert r['executed_candidate_sha256']==r['candidate']['output_sha256'],name
            else:assert r['executed_candidate_sha256']!=r['candidate']['output_sha256'],name
        else:
            assert rc==0 and r['status']=='PASS_CONTB_MIR_PROBE',name
            assert r['summary']=='checks=4961 physical=40 clocks=25',(name,r['summary'])
            assert 'CONTB_MIR_CP seen_one=3dfffe seen_zero=3fffff'in text,name
            assert r['executed_candidate_sha256']==r['candidate']['output_sha256'],name
            clocks=re.findall(r'^CONTB_MIR_CLOCK channel=(\d+) edges=(\d+)$',text,re.M)
            assert len(clocks)==25 and {int(c)for c,n in clocks}==set(range(25)) and all(int(n)>16 for c,n in clocks),name
            row.update(qualification='PASS_BOUNDED_CONTB_CAPTURES',summary=r['summary'],clocks=clocks)
        row['run_state_sha256']=sha(d/'run-state.json')
        row['candidate_sha256']=r['executed_candidate_sha256']
        with lock:save()
        print(row['qualification'],name,flush=True)
    failures=[]
    with ThreadPoolExecutor(max_workers=2)as pool:
        futures={pool.submit(execute,c):c for c in cases}
        for f in as_completed(futures):
            try:f.result()
            except Exception as error:failures.append(dict(case=futures[f],error=repr(error)));print('REVIEW_FAILURE',futures[f],repr(error),flush=True)
    passes=[r for r in result['cases']if r.get('qualification')=='PASS_BOUNDED_CONTB_CAPTURES']
    for profile in PROFILES:
        pair=[r for r in passes if r['profile']==profile]
        if len(pair)!=2 or pair[0]['clocks']!=pair[1]['clocks']:failures.append(dict(profile=profile,error='Missing pass or engines disagree on clock edges'))
    result.update(status='CONTB_MIR_REVIEW_FAILED'if failures else'PASS_BOUNDED_CONTB_WITH_HANDOFF_GAP',finished=time.time(),failures=failures,
                  primary=passes,negative_cases=[r for r in result['cases']if r.get('qualification','').startswith('REJECTED_WRONG_')],
                  handoff_gap=[r for r in result['cases']if r.get('qualification')=='EXPOSED_CLOCK_HANDOFF_GAP'],
                  full_producer_arrival_contract_qualified=False,physical_timing_qualified=False,native_integrated=False,hardware_speedup_claimed=False,
                  observation_contract='Root rise and RepeatCur release separated; simultaneous independent handoff demonstrably unsupported. Upstream producers remain explicit.')
    save();(out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True);return bool(failures)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    sys.exit(run(p.parse_args().output.resolve()))
