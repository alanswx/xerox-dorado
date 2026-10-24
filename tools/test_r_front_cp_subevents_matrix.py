#!/usr/bin/env python3
"""Compare CP-before-root subevents against six independent timing profiles.

Reuse the actual existing balanced/later-control handles. New cases copy exact
timed cells from already qualified profile sources; the expanded coincidence
bench and every original assertion remain intact. Freeze the probe while live.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import threading
import time

from test_r_front_forward_view import sha
from test_r_front_task_event_matrix import PROFILES

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def terminal(d):
    while True:
        r=json.loads((d/'run-state.json').read_text())
        if r['status']!='RUNNING':return r
        a=r['actions'][-1]
        ps=subprocess.run(['ps','-p',str(a['pid']),'-o','pid=,command='],capture_output=True,text=True)
        if ps.returncode or str(d) not in ps.stdout:
            again=json.loads((d/'run-state.json').read_text())
            if again['status']!='RUNNING':return again
            if again['actions'][-1]!=a:continue
            raise RuntimeError(('Recorded live action missing; do not restart',d,a,ps.stdout,ps.stderr))
        time.sleep(1)


def check(d):
    r=terminal(d)
    assert r['status']=='PASS_CP_BEFORE_ROOT_SUBEVENTS' and not r['mutation']
    assert r['processor_CP_subevent'] and r['task_CP_subevent'] and r['canonical_task_clock_stage_ownership']
    assert r['actions'][-1]['returncode']==0
    assert r.get('original_reference_unchanged_within_selected_profile',r.get('original_reference_and_checks_unchanged'))
    assert r['summary']=='checks=14884 writes=238 reads=6928 epochs=392'
    for row in r['source_files']:assert sha(d/row['path'])==row['output_sha256']
    log=(d/'test.log').read_text()
    clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
    captures=re.findall(r'^ALL_ADDRESS_PIN_AUDIT group=(\d+) package=(\S+) edges=(\d+) distinct=(\d+)$',log,re.M)
    assert len(clocks)==100 and all(int(n)>0 for i,n in clocks)
    assert len(captures)==8 and sum(int(e)for i,p,e,n in captures)==28700
    return dict(directory=str(d.relative_to(ROOT)),engine=r['engine'],summary=r['summary'],clocks=clocks,captures=captures,
                candidate_sha256=sha(d/'candidate.sv'),state_sha256=sha(d/'run-state.json'),
                processor_sha256=sha(d/'inputs/verilog/rtl/dorado_processor_capture_step.sv'),
                adapters={p['path']:p['output_sha256']for p in r['source_files'] if p['path'].startswith('adapters/')})


def run(out):
    assert not out.exists();out.mkdir(parents=True)
    names=['tools/probe_r_front_cp_subevents.py','tools/test_r_front_cp_subevents_matrix.py']
    hashes={n:sha(ROOT/n)for n in names}
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),source_hashes=hashes,
               cases=[],actions=[],errors=[],native_timing_qualified=False,assembly_permitted=False)
    lock=threading.Lock()
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    def unchanged():
        for n,h in hashes.items():assert sha(ROOT/n)==h,('Frozen driver changed',n)
    save()
    def worker(engine):
        try:
            for profile in PROFILES:
                unchanged()
                reused=profile=='balanced' or (profile=='later_control' and engine=='verilator')
                if reused:
                    d=BUILD/(f'r-front-cp-subevents-{engine}-v3-20261023' if profile=='balanced' else
                             'r-front-cp-subevents-later-control-verilator-v3-20261023')
                else:
                    d=out/f'{profile}-{engine}'
                    source=BUILD/f'r-front-all-address-coincident-cp-{engine}-v1-20261023'
                    timing=BUILD/'r-front-all-address-matrix-v1-20261023'/f'{profile}-{engine}'
                    command=[sys.executable,str(ROOT/'tools/probe_r_front_cp_subevents.py'),
                             '--source',str(source),'--output',str(d),'--engine',engine,
                             '--processor','--task','--timing-profile-source',str(timing)]
                    with(out/(d.name+'.driver.log')).open('w')as log:
                        p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
                        a=dict(command=command,pid=p.pid,started=time.time(),engine=engine,profile=profile)
                        with lock:state['actions'].append(a);save()
                        rc=p.wait()
                    with lock:a.update(returncode=rc,finished=time.time());save()
                    assert rc==0,(engine,profile,rc)
                row=check(d);row.update(profile=profile,reused_existing_handle=reused)
                with lock:state['cases'].append(row);save()
            return True
        except Exception as error:
            with lock:state['errors'].append(dict(engine=engine,error=repr(error)));save()
            return False
    with ThreadPoolExecutor(max_workers=2)as pool:passed=list(pool.map(worker,['icarus','verilator']))
    unchanged()
    if all(passed):
        assert len(state['cases'])==12
        for profile in PROFILES:
            pair=[r for r in state['cases']if r['profile']==profile]
            assert len(pair)==2 and pair[0]['clocks']==pair[1]['clocks'] and pair[0]['captures']==pair[1]['captures']
        assert len({r['candidate_sha256']for r in state['cases']})==1
        assert len({r['processor_sha256']for r in state['cases']})==1
        assert all(r['adapters']==state['cases'][0]['adapters']for r in state['cases'])
    state.update(status='PASS_CP_SUBEVENT_PROFILE_MATRIX'if all(passed)else'CP_SUBEVENT_PROFILE_MATRIX_FAILED',finished=time.time())
    save();print(state['status'],flush=True);return 0 if all(passed)else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.output.resolve()))
