#!/usr/bin/env python3
"""Observe original control Q after a previously rejected CP coincidence.

Diagnostic only: at the existing solver failure, stop audit monitors, advance
only the independent original reference once, print its shared control state,
then execute the original fatal assertion. No rejection becomes a passing gate.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

from test_r_front_forward_view import sha
from test_r_front_task_event import replace


def run(source,out):
    r=json.loads((source/'run-state.json').read_text())
    assert r['status']=='POINTER_PACKAGE_D_AT_CLOCK_AUDIT_FAILED' and r['actions'][-1]['returncode']!=0
    assert r['engine']=='verilator' and r['extra_coincident_CP_transactions']
    assert 'R front solver unsettled/repeated' in (source/'test.log').read_text()
    out.mkdir(parents=True,exist_ok=False);paths=[];records=[]
    for row in r['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];raw=p.read_text()
        if p.name=='tb_r_front_local.sv':
            marker='       $fatal(1,"R front solver unsettled/repeated");'
            raw=replace(raw,marker,'''       $display("CP_EPOCH_DIAGNOSTIC check=%0d root=%b->%b bank=%0d data=%0d old_cp=%h next_cp=%h old_shared=%h child_shared=%h joint_shared=%h",
         checks,cb,ca,~pins[291:289],pins[288:282],q[763:752],n[763:752],
         {q[751:736],q[79:64],q[31:0]},
         {dut.child_next[751:736],dut.child_next[79:64],dut.child_next[31:0]},dut.conta_next[63:0]);
       // This branch is already rejected. Observe reference data without
       // committing the candidate or converting the fatal into a pass.
       monitor=0;{root,rp}={next_root,pins};#100;
       $display("CP_EPOCH_ORIGINAL shared=%h full_q=%h",
         {reference[751:736],reference[79:64],reference[31:0]},reference);
'''+marker)
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(raw);paths.append(str(q))
        records.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q),changed=sha(p)!=sha(q)))
    assert sum(x['changed'] for x in records)==1
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    defines=[x for x in r['actions'][0]['command'] if x.startswith('-D')]
    commands=[['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
               '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state=dict(status='RUNNING_DIAGNOSTIC',pid=os.getpid(),started=time.time(),source_files=records,actions=[],
               parent=str(source),parent_state_sha256=sha(source/'run-state.json'),qualifying_gate=False,
               original_solver_rejection_retained=True,reference_only_observation_after_existing_failure=True)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for i,cmd in enumerate(commands):
        with(out/('compile.log' if i==0 else 'test.log')).open('w') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
            a=dict(command=cmd,pid=p.pid,started=time.time());state['actions'].append(a);save()
            rc=p.wait();a.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    observed='CP_EPOCH_ORIGINAL shared=' in log and 'R front solver unsettled/repeated' in log and rc!=0
    state.update(status='DIAGNOSTIC_OBSERVED_ORIGINAL_AND_RETAINED_REJECTION' if observed else 'DIAGNOSTIC_FAILED',
                 diagnostic_lines=re.findall(r'^CP_EPOCH_[^\n]+',log,re.M),finished=time.time());save()
    print(state['status'],flush=True);return 0 if observed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['source','output']:p.add_argument('--'+n,type=Path,required=True)
    a=p.parse_args();sys.exit(run(a.source.resolve(),a.output.resolve()))
