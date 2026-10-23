#!/usr/bin/env python3
"""Use the actual ContA f14 capture event throughout the processor component.

The joint original controller already owns f14's physical clock (channel12).
Thread its capture/repeat events to the child's duplicate task calculation,
instead of decoding the new Switchb/StopTasks back into that same clock. Keep
old task DATA and every original reference,844-Q/9216-RAM/100-clock/port check.
This is a component experiment, not native/60ns/physical timing qualification.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import time

ROOT=Path(__file__).resolve().parents[1]
NAMES=['task_capture','t_file','t_parity','r_operand','r_flags','r_address','r_memory','r_front','r_front_ready']
MODULES={n:'dorado_local_'+n+'_step'for n in NAMES}


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def replace(raw,a,b):
    assert raw.count(a)==1,(a,raw.count(a));return raw.replace(a,b,1)


def candidate(raw,mutation):
    old='dorado_local_r_front_ready_step child(STOP_CAPTURE_OLD ? capture_local[0] : after_raw[65],'
    event="2'b00"if mutation=='omit_task_clock'else'{joint_repeated[12],joint_events[12]}'
    return replace(raw,old,old+event+',')


def adapter(raw,name):
    original=raw
    raw=replace(raw,'input wire stop_tasks_capture,','input wire stop_tasks_capture,input wire [1:0] canonical_task_clock,')
    calls=0
    for module in MODULES.values():
        pattern=r'(?m)^(\s*'+module+r'\s+\w+\()stop_tasks_capture,'
        raw,count=re.subn(pattern,lambda m:m[1]+'stop_tasks_capture,canonical_task_clock,',raw);calls+=count
    if name=='task_capture':
        # HM2.3: one physical f14 clock, not another clock built from its
        # newly captured output. Channel22 remains the original f14 event;
        # its independent original clock counter is still checked in full.
        raw=replace(raw,'levels[22]=old[19] | pins[65];','levels[22]=0; // f14 capture comes from its canonical joint owner below.')
        old='captures=levels & ~previous;'
        raw=replace(raw,old,old+'\n            captures[22]=canonical_task_clock[0] && (wave==0);')
        raw=replace(raw,'state_next=q;unsettled=!settled;','repeated_events[22]=repeated_events[22] | canonical_task_clock[1];\n        state_next=q;unsettled=!settled;')
    # Inverse every plumbing and core change; nothing else may drift.
    inverse=raw.replace('input wire stop_tasks_capture,input wire [1:0] canonical_task_clock,','input wire stop_tasks_capture,',1)
    for module in MODULES.values():
        inverse=re.sub(r'(?m)^(\s*'+module+r'\s+\w+\()stop_tasks_capture,canonical_task_clock,',r'\1stop_tasks_capture,',inverse)
    if name=='task_capture':
        inverse=inverse.replace('levels[22]=0; // f14 capture comes from its canonical joint owner below.','levels[22]=old[19] | pins[65];',1)
        inverse=inverse.replace('\n            captures[22]=canonical_task_clock[0] && (wave==0);','',1)
        inverse=inverse.replace('repeated_events[22]=repeated_events[22] | canonical_task_clock[1];\n        state_next=q;unsettled=!settled;','state_next=q;unsettled=!settled;',1)
    assert inverse==original,('Unexpected adapter change',name)
    return raw,calls


def run(source,out,engine,mutation):
    parent=json.loads((source/'run-state.json').read_text())
    assert parent['status']=='PASS_R_FRONT_READ_WINDOWS' and parent['actions'][-1]['returncode']==0
    assert parent['summary']=='checks=11300 writes=238 reads=4368 epochs=392' and not parent.get('mutation')
    assert sha(source/'candidate.sv')=='2226d6baf560757f20c4102c035506c79f91fd4cadfa8804b1c087b80c3649c4'
    for row in parent['source_files']:assert sha(source/row['path'])==row['output_sha256']
    assert sha(source/'dorado_r_read_window_step.sv')==parent['helper_sha256']
    build=next(a for a in parent['actions']if a['command'][0]in ['iverilog','verilator'])
    paths=[Path(n)for n in build['command']if n.endswith(('.sv','.v'))]
    out.mkdir(parents=True,exist_ok=False);records=[];copied=[];plumbed=0
    for p in paths:
        rel=p.relative_to(source);q=out/rel;q.parent.mkdir(parents=True,exist_ok=True)
        original=p.read_text();raw=original
        if p.name=='candidate.sv':raw=candidate(raw,mutation)
        for name,module in MODULES.items():
            if p.name==module+'.sv':raw,count=adapter(raw,name);plumbed+=count
        q.write_text(raw);copied.append(q)
        records.append(dict(path=str(rel),parent_sha256=sha(p),output_sha256=sha(q),changed=original!=raw))
    assert sum(r['changed']for r in records)==10 and plumbed==9
    assert sha(out/'inputs/tb_r_front_local.sv')==sha(source/'inputs/tb_r_front_local.sv')
    (out/'source-run-state.json').write_bytes((source/'run-state.json').read_bytes())
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,mutation=mutation,parent=str(source),
               parent_run_state_sha256=sha(source/'run-state.json'),source_files=records,actions=[],
               full_state_comparison='all844 physical Qs after owner commit',RAM_bits=9216,clock_channels=100,
               canonical_clock=dict(board='ContA',package='f14',joint_channel=12,child_channel=22,event_and_repeat_forwarded=True),
               task_data_epoch_unchanged=True,independent_reference_and_bench_unchanged=True,
               native_integrated=False,physical_timing_qualified=False,assembly_permitted=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    defines=[x for x in build['command']if x.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*map(str,copied)],['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,'--Mdir',str(out/'obj'),'-o','test',*map(str,copied)],[str(out/'obj/test')]]
    save();rc=0
    for i,cmd in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT);row=dict(command=cmd,pid=p.pid,started=time.time());state['actions'].append(row);save()
            rc=p.wait();row.update(returncode=rc,finished=time.time());save()
        if rc:break
    text=(out/'test.log').read_text()if(out/'test.log').exists()else'';m=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',text)
    state.update(status='PASS_R_FRONT_TASK_EVENT'if not rc and m else'R_FRONT_TASK_EVENT_FAILED',summary=m[1]if m else None,finished=time.time());save()
    print(state['status'],flush=True);return rc if rc else(0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],default='icarus');p.add_argument('--mutation',choices=['omit_task_clock']);a=p.parse_args()
    sys.exit(run(a.source.resolve(),a.output.resolve(),a.engine,a.mutation))
