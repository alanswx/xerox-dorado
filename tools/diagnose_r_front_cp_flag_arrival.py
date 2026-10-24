#!/usr/bin/env python3
"""Passively trace original ProcH g18 data/clock at the CP-clear boundary.

Keep the candidate, independent timed graph and every fatal unchanged. Print
pre-event state/RAM for bounded replay and original pin/Q arrival times. Added
observation can perturb a same-delta race; this is never a qualifying gate.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from test_r_front_forward_view import sha
from test_r_front_task_event import replace


def run(source,out):
    r=json.loads((source/'run-state.json').read_text());engine=r['engine']
    assert r['status'] in ['PASS_CP_BEFORE_ROOT_SUBEVENTS','CP_BEFORE_ROOT_SUBEVENTS_FAILED']
    assert r['task_CP_subevent'] and r['processor_CP_subevent']
    assert r['actions'][0]['returncode']==0 and 'returncode' in r['actions'][-1]
    out.mkdir(parents=True,exist_ok=False);records=[];paths=[]
    for row in r['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];raw=p.read_text()
        if p.name=='tb_r_front_local.sv':
            raw=replace(raw,' task automatic event_step(input next_root,input [400:0] pins);','''
 always @(original.h.g18.p5 or original.h.g18.p9 or original.h.n107 or original.h.g18.state_q[0])
  if(monitor && checks>=12837 && checks<=12840)
   $display("CP_FLAG_ARRIVAL t=%0.3f check=%0d root=%b CP_NoDispatch=%b freeze=%b D=%b C=%b Q=%b",
    $realtime,checks,root,reference[757],original.h.n107,original.h.g18.p5,original.h.g18.p9,original.h.g18.state_q[0]);
 always @(posedge original.h.g18.p9)
  if(monitor && checks>=12837 && checks<=12840)
   $display("CP_FLAG_CLOCK t=%0.3f check=%0d D=%b Q=%b",$realtime,checks,original.h.g18.p5,original.h.g18.state_q[0]);
 task automatic event_step(input next_root,input [400:0] pins);''')
            raw=replace(raw,'     ca=next_root;pa=pins;pa[144:127]=0;','''
     if(checks==12839)begin
      old_memory=memory_image();
      $display("CP_FLAG_PRE root=%b next_root=%b stored=%h original=%h pins_before=%h pins_after=%h",
       cb,next_root,stored_q,reference,pb,pins);
      $display("CP_FLAG_RAM_LO %h",old_memory[4607:0]);
      $display("CP_FLAG_RAM_HI %h",old_memory[9215:4608]);
     end
     ca=next_root;pa=pins;pa[144:127]=0;''')
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(raw);paths.append(str(q))
        records.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q),changed=sha(p)!=sha(q)))
    assert sum(x['changed']for x in records)==1
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    defines=[x for x in r['actions'][0]['command']if x.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*paths],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,parent=str(source),
               parent_state_sha256=sha(source/'run-state.json'),parent_status=r['status'],
               parent_test_returncode=r['actions'][-1]['returncode'],source_files=records,actions=[],
               qualifying_gate=False,original_reference_unchanged=True,all_original_fatals_retained=True,
               passive_observer_may_change_same_delta_order=True)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for i,command in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            a=dict(command=command,pid=p.pid,started=time.time());state['actions'].append(a);save()
            rc=p.wait();a.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    observed='CP_FLAG_PRE 'in log and 'CP_FLAG_CLOCK 'in log
    state.update(status='DIAGNOSTIC_CP_FLAG_ARRIVAL_OBSERVED'if observed else'DIAGNOSTIC_CP_FLAG_ARRIVAL_FAILED',
                 observed_test_returncode=rc,outcome_changed_by_observation=(rc!=r['actions'][-1]['returncode'])if observed else None,
                 arrival_lines=re.findall(r'^CP_FLAG_(?:ARRIVAL|CLOCK)[^\n]*',log,re.M),finished=time.time());save()
    print(state['status'],flush=True);return 0 if observed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve()))
