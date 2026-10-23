#!/usr/bin/env python3
"""Run the full current-read-window integration across six timing profiles.

Reuse the existing balanced runs; wait only while their recorded drivers are
actually alive. Preserve the six independent original-reference graphs and
every844-Q/9216-RAM/100-clock/functional-port assertion. Timing and native
firmware acceptance remain separate, and all sources stay frozen while active.
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


def check(d):
    r=json.loads((d/'run-state.json').read_text());assert r['status']=='PASS_R_FRONT_READ_WINDOWS'
    assert r['actions'][-1]['returncode']==0 and not r.get('mutation')
    assert r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
    assert r['full_state_comparison']=='all844 physical Qs after owner commit'
    text=(d/'test.log').read_text();clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',text,re.M)
    assert len(clocks)==100 and all(int(n)>0 for c,n in clocks)
    for f in r['source_files']:assert sha(d/f['path'])==f['output_sha256']
    assert sha(d/'dorado_r_read_window_step.sv')==r['helper_sha256']==sha(ROOT/'verilog/rtl/dorado_r_read_window_step.sv')
    return dict(directory=str(d.relative_to(ROOT)),engine=r['engine'],summary=r['summary'],clocks=clocks,
                candidate_sha256=sha(d/'candidate.sv'),run_state_sha256=sha(d/'run-state.json'))


def run(out):
    parent=BUILD/'r-front-control-tasking-mask-matrix-v1-20261023'
    assert json.loads((parent/'run-state.json').read_text())['status']=='PASS_TASKING_CLOCK_MASK_PROFILE_MATRIX'
    frozen=['tools/test_r_front_read_windows.py','tools/test_r_front_read_window_matrix.py','verilog/rtl/dorado_r_read_window_step.sv']
    hashes={n:sha(ROOT/n)for n in frozen};out.mkdir(parents=True,exist_ok=False)
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),source_hashes=hashes,cases=[],actions=[],errors=[],
               native_integrated=False,physical_timing_qualified=False);lock=threading.Lock()
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def unchanged():
        for n,h in hashes.items():assert sha(ROOT/n)==h,('Frozen source changed',n)
    save()
    def worker(engine):
        balanced=BUILD/f'r-front-read-windows-{engine}-v4-20261023'
        while True:
            r=json.loads((balanced/'run-state.json').read_text())
            if r['status']!='RUNNING':break
            os.kill(r['pid'],0)  # A state file alone is not a live handle.
            unchanged();time.sleep(1)
        row=check(balanced);row.update(profile='balanced',reused_terminal_pass=True)
        with lock:state['cases'].append(row);save()
        for profile in PROFILES[1:]:
            unchanged();source=parent/f'{profile}-{engine}-extended';d=out/f'{profile}-{engine}'
            cmd=[sys.executable,str(ROOT/'tools/test_r_front_read_windows.py'),'--source',str(source),'--output',str(d),'--engine',engine]
            with(out/(d.name+'.driver.log')).open('w')as log:
                p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
                action=dict(command=cmd,pid=p.pid,started=time.time(),profile=profile,engine=engine)
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
    state.update(status='PASS_R_FRONT_READ_WINDOW_PROFILE_MATRIX'if all(passed)else'R_FRONT_READ_WINDOW_PROFILE_MATRIX_FAILED',finished=time.time())
    save();print(state['status'],flush=True);return 0 if all(passed)else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();sys.exit(run(a.output.resolve()))
