#!/usr/bin/env python3
"""Observe one existing native builder, then run central-Q reports serially.

Never restart the builder or assemble/deploy. Wait for its terminal state AND
absence of all recorded processes before opening the same Quartus database.
Run once on misterubuntu. Polling a live PID is a verified build wait; missing
builder with nonterminal state is an error requiring inspection.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time


def live(pid):
    try:os.kill(pid,0);return True
    except ProcessLookupError:return False


def run(project,quartus):
    path=project/'central-review-state.json';assert not path.exists(),'Observe existing review process'
    script=project/'audit-r-front-ready.tcl';assert script.is_file()
    state=dict(status='WAITING_FOR_RECORDED_BUILDER',review_pid=os.getpid(),started=time.time(),
        tcl_sha256=hashlib.sha256(script.read_bytes()).hexdigest(),actions=[],assembly_permitted=False)
    def write():path.write_text(json.dumps(state,indent=2)+'\n')
    write()
    try:
        while True:
            build=json.loads((project/'run-state.json').read_text())
            state['builder_pid']=build['builder_pid'];state['observed_builder_status']=build['status']
            if build['status'].startswith('TERMINAL_'):
                pids=[build['builder_pid']]+[r['pid']for r in build['stages']]
                if any(live(p)for p in pids):
                    write();time.sleep(2);continue
                assert build['status']=='TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED',('Inspect failed native builder before supplemental STA',build['status'])
                assert len(build['stages'])==8 and all(r['returncode']==0 for r in build['stages'])
                state['absent_builder_pids']=pids
                break
            assert build['status']=='RUNNING' and live(build['builder_pid']),'Builder missing with nonterminal state; inspect, do not restart'
            cmd=(Path('/proc')/str(build['builder_pid'])/'cmdline').read_bytes()
            assert b'build_r_front_ready_native.py'in cmd and str(project).encode()in cmd,'Recorded builder identity differs'
            state['last_live_observation']=time.time();write();time.sleep(10)
        state['status']='RUNNING_CENTRAL_Q_REPORTS';write()
        command=[str(quartus/'quartus_sta'),'-t',str(script)]
        with(project/'central-crossings.log').open('x')as log:
            p=subprocess.Popen(command,cwd=project/'core',stdout=log,stderr=subprocess.STDOUT)
            action=dict(command=command,pid=p.pid,started=time.time());state['actions'].append(action);write()
            code=p.wait();action.update(returncode=code,finished=time.time());write()
            assert code==0,('Supplemental central-Q STA failed; inspect existing log',code)
        state.update(status='TERMINAL_CENTRAL_REPORTS_REVIEW_REQUIRED',finished=time.time());write()
        print(state['status'],flush=True)
    except BaseException:
        state.update(status='TERMINAL_FAILED',finished=time.time());write();raise


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True)
    p.add_argument('--quartus',type=Path,default=Path('/home/alans/intelFPGA_lite/17.0/quartus/bin'))
    a=p.parse_args();run(a.project.resolve(),a.quartus.resolve())
