#!/usr/bin/env python3
"""Compare the shared f14 event in both engines/six original timing profiles.

Reuse existing balanced runs, observing their actual driver PIDs while live.
Keep the already running read-window matrix intact; each terminal source is
frozen and checked before the next derived run. No clock/state oracle exclusions.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import threading
import time

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
PROFILES=['balanced','fast_gates','later_control','parc_parity_min','parc_parity_max','later_r_memory']


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def terminal(d):
    while True:
        r=json.loads((d/'run-state.json').read_text())
        if r['status']!='RUNNING':return r
        os.kill(r['pid'],0)
        time.sleep(1)


def check(d):
    r=json.loads((d/'run-state.json').read_text());assert r['status']=='PASS_R_FRONT_TASK_EVENT'
    assert r['actions'][-1]['returncode']==0 and not r['mutation'] and r['independent_reference_and_bench_unchanged']
    assert r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
    text=(d/'test.log').read_text();clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',text,re.M)
    assert len(clocks)==100 and all(int(n)>0 for c,n in clocks)
    for row in r['source_files']:assert sha(d/row['path'])==row['output_sha256']
    return dict(directory=str(d.relative_to(ROOT)),engine=r['engine'],summary=r['summary'],clocks=clocks,
                candidate_sha256=sha(d/'candidate.sv'),run_state_sha256=sha(d/'run-state.json'))


def run(out):
    names=['tools/test_r_front_task_event.py','tools/test_r_front_task_event_matrix.py']
    hashes={n:sha(ROOT/n)for n in names};out.mkdir(parents=True,exist_ok=False)
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),source_hashes=hashes,cases=[],actions=[],errors=[],
               native_integrated=False,physical_timing_qualified=False);lock=threading.Lock()
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def unchanged():
        for n,h in hashes.items():assert sha(ROOT/n)==h,('Frozen source changed',n)
    save()
    def worker(engine):
        d=BUILD/f'r-front-task-event-{engine}-v2-20261023';terminal(d);unchanged()
        row=check(d);row.update(profile='balanced',reused_terminal_pass=True)
        with lock:state['cases'].append(row);save()
        for profile in PROFILES[1:]:
            unchanged();source=BUILD/'r-front-read-windows-matrix-v2-20261023'/f'{profile}-{engine}'
            r=terminal(source);assert r['status']=='PASS_R_FRONT_READ_WINDOWS'
            d=out/f'{profile}-{engine}';cmd=[sys.executable,str(ROOT/'tools/test_r_front_task_event.py'),'--source',str(source),'--output',str(d),'--engine',engine]
            with(out/(d.name+'.driver.log')).open('w')as log:
                p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);action=dict(command=cmd,pid=p.pid,started=time.time(),profile=profile,engine=engine)
                with lock:state['actions'].append(action);save()
                rc=p.wait()
            with lock:action.update(returncode=rc,finished=time.time());save()
            assert rc==0,(engine,profile,rc)
            row=check(d);row['profile']=profile
            with lock:state['cases'].append(row);save()
        return True
    def guarded(engine):
        try:return worker(engine)
        except Exception as e:
            with lock:state['errors'].append(dict(engine=engine,error=repr(e)));save()
            return False
    with ThreadPoolExecutor(max_workers=2)as pool:passed=list(pool.map(guarded,['icarus','verilator']))
    unchanged()
    if all(passed):
        assert len(state['cases'])==12
        for profile in PROFILES:
            pair=[r for r in state['cases']if r['profile']==profile];assert len(pair)==2 and pair[0]['clocks']==pair[1]['clocks']
        assert len({r['candidate_sha256']for r in state['cases']})==1
    state.update(status='PASS_R_FRONT_TASK_EVENT_PROFILE_MATRIX'if all(passed)else'R_FRONT_TASK_EVENT_PROFILE_MATRIX_FAILED',finished=time.time());save()
    print(state['status'],flush=True);return 0 if all(passed)else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();sys.exit(run(a.output.resolve()))
