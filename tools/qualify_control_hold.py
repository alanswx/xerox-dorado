#!/usr/bin/env python3
"""Review joint original ContA/ContB/MemC capture ordering and open parity gap.

Compare all156 Qs/56 clocks/94 flags in both engines and three illustrative
delay profiles. Reject seven wrong crossings/epochs; preserve the simultaneous
raw-parity/source-rise counterexample. Not native or physical qualification.
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
from test_control_hold import PROFILES,MUTATIONS,TB

ROOT=Path(__file__).resolve().parents[1]
SHA='b79903a970ad7e4bd4ec79020f3d43f542dc9a71b412b21a2f4f39365528323e'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def run(out):
    out.mkdir(parents=True,exist_ok=False);own=out/'qualify_control_hold.py';own.write_bytes(Path(__file__).read_bytes())
    result=dict(status='RUNNING',pid=os.getpid(),started=time.time(),cases=[],review_source_sha256=sha(own));lock=threading.Lock()
    def save():(out/'run-state.json').write_text(json.dumps(result,indent=2)+'\n')
    save();cases=[(p,e,None,False)for p in PROFILES for e in ['icarus','verilator']]
    cases += [('balanced',e,m,False)for m in MUTATIONS for e in ['icarus','verilator']]
    cases += [('balanced',e,None,True)for e in ['icarus','verilator']]
    pilots={('balanced','icarus',None,False):'control-hold-icarus-v7-20261023',
            ('balanced','verilator',None,False):'control-hold-verilator-v2-20261023'}
    pilots={case:name for case,name in pilots.items()if(out.parent/name/'run-state.json').exists()}
    expected_tb=hashlib.sha256(TB.encode()).hexdigest()
    def execute(case):
        profile,engine,mutation,gap=case;name=profile+'-'+engine+'-'+('parity-gap'if gap else mutation or 'original');d=out/name
        if case in pilots:
            d=out.parent/pilots[case];previous=json.loads((d/'run-state.json').read_text())
            assert previous['status']!='RUNNING'and len(previous['actions'])==2,('Observe existing pilot',d)
            rc=previous['actions'][-1]['returncode']
            row=dict(profile=profile,engine=engine,mutation=mutation,parity_handoff=gap,directory='../'+d.name,
                     reused_terminal_run=True,pid=previous['pid'],started=previous['started'],finished=previous['finished'],returncode=rc,
                     commands=[r['command']for r in previous['actions']])
            with lock:result['cases'].append(row);save()
        else:
            cmd=[sys.executable,str(ROOT/'tools/test_control_hold.py'),'--output',str(d),'--engine',engine,'--profile',profile]
            if mutation:cmd+=['--mutation',mutation]
            if gap:cmd+=['--parity-handoff']
            with(out/(name+'.log')).open('w')as log:
                p=subprocess.Popen(cmd,cwd=ROOT,stdout=log,stderr=subprocess.STDOUT)
                row=dict(profile=profile,engine=engine,mutation=mutation,parity_handoff=gap,directory=name,pid=p.pid,command=cmd,started=time.time())
                with lock:result['cases'].append(row);save()
                rc=p.wait()
            with lock:row.update(returncode=rc,finished=time.time());save()
        r=json.loads((d/'run-state.json').read_text())
        assert r['status']!='RUNNING'and len(r['actions'])==2 and r['actions'][0]['returncode']==0,(name,r['status'])
        assert r['engine']==engine and r['profile']==profile and r.get('mutation')==mutation and r.get('parity_handoff',False)==gap,name
        assert r['actions'][-1]['returncode']==rc,name
        for n,h in r['inputs'].items():assert sha(d/'inputs'/n)==h,(name,n)
        for n,h in r['compiled_sources'].items():assert sha(d/n)==h,(name,n)
        for n,h in r['timing']['files'].items():assert sha(d/'timed'/n)==h,(name,n)
        assert sha(d/'tb.sv')==r['testbench_sha256'] and sha(d/'candidate.sv')==r['executed_candidate_sha256'],name
        if not gap:assert r['testbench_sha256']==expected_tb,name
        text=(d/'test.log').read_text()
        if mutation or gap:
            assert rc!=0 and r['status']=='CONTROL_HOLD_PROBE_FAILED',name
            failure=re.search(r'Control Hold (state|flags|clock) mismatch check=(\d+)',text);assert failure,(name,text)
            row.update(qualification='EXPOSED_RAW_PARITY_HANDOFF_GAP'if gap else'REJECTED_'+mutation.upper(),
                       first_failure_kind=failure[1],first_failure_check=int(failure[2]))
            if gap:
                assert r['executed_candidate_sha256']==SHA and 'CONTROL_HOLD_RAW_PARITY_HANDOFF old_root=0 new_root=1 stable_IM=1'in text,name
                differences=re.findall(r'^CONTROL_HOLD_CLOCK_DIFF check=(\d+) channel=(\d+) actual=1 predicted=0$',text,re.M)
                wanted=set(range(15,21))|({25+i for i in range(22)}-{25,42})
                assert {int(ch)for check,ch in differences}==wanted and all(check==failure[2]for check,ch in differences),(name,differences)
            else:assert r['executed_candidate_sha256']!=SHA,name
        else:
            assert rc==0 and r['status']=='PASS_CONTROL_HOLD_PROBE',name
            assert r['summary'].startswith('checks=5678 physical=156 clocks=56 changed='),(name,r['summary'])
            assert r['executed_candidate_sha256']==SHA,name
            assert 'CONTROL_HOLD_DIRECTED actual_UseDMD=1 maintenance_codes=3 CP_selectors=4 LOW_NoDispatch=1'in text,name
            clocks=re.findall(r'^CONTROL_HOLD_CLOCK channel=(\d+) edges=(\d+)$',text,re.M)
            assert len(clocks)==56 and {int(c)for c,n in clocks}==set(range(56))and all(int(n)>=8 for c,n in clocks),name
            row.update(qualification='PASS_BOUNDED_JOINT_CONTROL_HOLD',summary=r['summary'],clocks=clocks)
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
    passes=[r for r in result['cases']if r.get('qualification')=='PASS_BOUNDED_JOINT_CONTROL_HOLD']
    for profile in PROFILES:
        pair=[r for r in passes if r['profile']==profile]
        if len(pair)!=2 or pair[0]['clocks']!=pair[1]['clocks']:failures.append(dict(profile=profile,error='Missing pass or clock counts differ'))
    result.update(status='JOINT_CONTROL_HOLD_REVIEW_FAILED'if failures else'PASS_BOUNDED_JOINT_CONTROL_HOLD_WITH_PARITY_GAP',finished=time.time(),failures=failures,
                  primary=passes,negative_cases=[r for r in result['cases']if r.get('qualification','').startswith('REJECTED_')],
                  parity_gap_cases=[r for r in result['cases']if r.get('qualification')=='EXPOSED_RAW_PARITY_HANDOFF_GAP'],
                  full_producer_arrival_contract_qualified=False,native_integrated=False,physical_timing_qualified=False,hardware_speedup_claimed=False,
                  remaining_boundaries='MCR DisHold and actual hold-data; IM/parity/DMD/B-bus; full processor/capture-stage native integration; analog skew')
    save();(out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True);return bool(failures)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    sys.exit(run(p.parse_args().output.resolve()))
