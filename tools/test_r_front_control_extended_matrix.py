#!/usr/bin/env python3
"""Gate consistent old MIR/ContB task data across the established profiles.

Independent original reference, all844Qs/9216RAM/100clocks/functional ports.
Keep baseline stimulus and add real late-task/branch/multiply/maintenance cases.
No native boot, analog arrivals, FPGA timing or speed qualification.
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

from review_r_front_control import read_run

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
PROFILES=['balanced','fast_gates','later_control','parc_parity_min','parc_parity_max','later_r_memory']
CANDIDATE='d8c8f7443887d57739e9abc0896ba283ac07b3d86bfea9bf5d849de35024d7d5'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def check(d,extended):
    r,log=read_run(d)
    assert r['status']=='PASS_R_FRONT_LOCAL_PROBE'and r['actions'][-1]['returncode']==0
    assert r['pre_event_control_capture']and r['rbase_close']and r['actual_RBase_StkP_directed']and r['owned_r_inputs_zeroed']
    assert bool(r['extended_actual_directed'])==extended and not r['solver_rejection_deferred_for_diagnostic']
    assert r['executed_candidate_sha256']==CANDIDATE
    if extended:
        assert r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
        for text in ['R_FRONT_LOCAL_LATE_TASKING old_data=1 root=0 switch=1 tasking_n=0',
                     'R_FRONT_LOCAL_LATE_BRANCH old_stretch=1 root=0 StartCycle_c=0',
                     'R_FRONT_CONTROL_EXTENDED multiply=32 maintenance=65,65,65 unused_memc=1']:
            assert text in log,(d,text)
    else:assert r['summary']=='checks=10121 writes=242 reads=4166 epochs=391'
    clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
    assert len(clocks)==100
    if extended:assert all(int(n)>0 for c,n in clocks)
    else:assert [int(c)for c,n in clocks if int(n)==0]==[43,91,92,93,95]
    return dict(directory=str(d.relative_to(ROOT)),engine=r['engine'],profile=r['profile'],extended=extended,
                summary=r['summary'],clocks=clocks,run_state_sha256=sha(d/'run-state.json'))


def run(out):
    assert not out.exists()
    baseline=[check(BUILD/f'r-front-control-extended-{engine}-v4-20261023',True)for engine in ['icarus','verilator']]
    out.mkdir(parents=True)
    sources=['test_r_front_control.py','generate_r_front_control_candidate.py','generate_r_front_local_adapters.py','rewrite_rbase_window.py',Path(__file__).name]
    hashes={n:sha(ROOT/'tools'/n)for n in sources}
    lock=threading.Lock();state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),scope=__doc__,tool_hashes=hashes,cases=[],actions=[],errors=[])
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    # Reuse the exact completed balanced extended tests, never restart them.
    for row in baseline:
        row['reused_terminal_pass']=True;state['cases'].append(row)
    save()
    def worker(engine):
        cases=[('balanced',False)]+[(p,True)for p in PROFILES if p!='balanced']
        for profile,extended in cases:
            for n,h in hashes.items():assert sha(ROOT/'tools'/n)==h,('Tool changed during matrix',n)
            d=out/(profile+'-'+engine+('-extended'if extended else'-baseline'))
            cmd=[sys.executable,str(ROOT/'tools/test_r_front_control.py'),'--base',str(BUILD/'r-front-ready-qualified-v1-20261020'),
                 '--reference',str(BUILD/'r-front-control-reference-v2-20261023'),'--output',str(d),
                 '--engine',engine,'--profile',profile,'--rbase-close','--actual-directed','--zero-owned-r-inputs','--old-control-capture']
            if extended:cmd+=['--extended-directed']
            with(out/(d.name+'.driver.log')).open('w')as log:
                p=subprocess.Popen(cmd,cwd=ROOT,stdout=log,stderr=subprocess.STDOUT)
                row=dict(directory=d.name,engine=engine,profile=profile,extended=extended,command=cmd,pid=p.pid,started=time.time())
                with lock:state['actions'].append(row);save()
                rc=p.wait()
            with lock:row.update(returncode=rc,finished=time.time());save()
            if rc:return False
            checked=check(d,extended)
            with lock:state['cases'].append(checked);save()
        return True
    def guarded(engine):
        try:return worker(engine)
        except Exception as e:
            with lock:state['errors'].append(dict(engine=engine,error=repr(e)));save()
            return False
    with ThreadPoolExecutor(max_workers=2)as pool:passed=list(pool.map(guarded,['icarus','verilator']))
    state.update(status='PASS_OLD_CONTROL_CAPTURE_PROFILE_MATRIX'if all(passed)else'OLD_CONTROL_CAPTURE_PROFILE_MATRIX_FAILED',finished=time.time(),
                 physical_bits=844,RAM_bits=9216,clock_channels=100,full_arrival_contract_qualified=False,physical_timing_qualified=False,native_firmware_qualified=False)
    if all(passed):
        assert len(state['cases'])==14
        for profile in PROFILES:
            pair=[r for r in state['cases']if r['profile']==profile and r['extended']];assert len(pair)==2 and pair[0]['clocks']==pair[1]['clocks']
    save();print(state['status'],flush=True);return 0 if all(passed)else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();sys.exit(run(a.output.resolve()))
