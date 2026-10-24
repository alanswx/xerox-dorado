#!/usr/bin/env python3
"""Test CP-pin arrival before a coincident distributed CPU-root transition.

Compose two existing pure control evaluators without adding state, RAM, clocks
or guest cycles. The first sees only CP pins at the old root; the second sees
the distributed root and remaining inputs. Keep every original reference,
physical-state, clock, RAM and fixed-point assertion unchanged.
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


WRAPPER='''
// CP inputs reach their local register before the distributed CPU root.
// Both evaluators are combinational; their intermediate Q is a wire, not an
// additional physical storage bank or an architectural instruction cycle.
module dorado_control_hold_ordered_step #(parameter integer WAVES=24)(
 input wire [155:0] state_q,input wire clock_before,clock_after,
 input wire [397:0] pins_before,pins_after,output wire [155:0] state_next,
 output wire [55:0] clock_events,repeated_events,output wire unsettled,
 input wire refresh_before_run_capture,output wire [93:0] flags);
 wire split_event=SPLIT_EXPRESSION;
 wire [397:0] prefix_pins=split_event ?
  {pins_before[397:293],pins_after[292:282],pins_before[281:0]} : pins_before;
 wire [155:0] prefix_q;
 wire [55:0] prefix_events,prefix_repeated,root_events,root_repeated;
 wire prefix_unsettled,root_unsettled;
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) cp_arrival(
  state_q,clock_before,clock_before,pins_before,prefix_pins,
  prefix_q,prefix_events,prefix_repeated,prefix_unsettled,
  refresh_before_run_capture,);
 dorado_control_hold_atomic_step #(.WAVES(WAVES)) root_arrival(
  prefix_q,clock_before,clock_after,prefix_pins,pins_after,
  state_next,root_events,root_repeated,root_unsettled,
  refresh_before_run_capture,flags);
 assign clock_events=prefix_events|root_events;
 assign repeated_events=prefix_repeated|root_repeated|(prefix_events&root_events);
 assign unsettled=prefix_unsettled|root_unsettled;
endmodule
'''

PROCESSOR_WRAPPER='''
// The same CP-before-root order reaches the redundant processor/control
// evaluator through its direct NoDispatch/StopAtT1/ClrStop pins. Preserve a
// LOW interval before a rising root when NoDispatch is cleared concurrently.
module dorado_processor_capture_step #(parameter integer WAVES=16)(
 input wire [63:0] state_q,input wire clock_before,clock_after,
 input wire [27:0] pins_before,pins_after,output wire [63:0] state_next,
 output wire [21:0] clock_events,repeated_events,output wire unsettled);
 wire split_event=SPLIT_EXPRESSION;
 wire [27:0] prefix_pins=split_event ?
  {pins_before[27:8],pins_after[7],pins_before[6:3],pins_after[2],pins_before[1],pins_after[0]} : pins_before;
 wire [63:0] prefix_q;
 wire [21:0] prefix_events,prefix_repeated,root_events,root_repeated;
 wire prefix_unsettled,root_unsettled;
 dorado_processor_capture_atomic_step #(.WAVES(WAVES)) cp_arrival(
  state_q,clock_before,clock_before,pins_before,prefix_pins,
  prefix_q,prefix_events,prefix_repeated,prefix_unsettled);
 dorado_processor_capture_atomic_step #(.WAVES(WAVES)) root_arrival(
  prefix_q,clock_before,clock_after,prefix_pins,pins_after,
  state_next,root_events,root_repeated,root_unsettled);
 assign clock_events=prefix_events|root_events;
 assign repeated_events=prefix_repeated|root_repeated|(prefix_events&root_events);
 assign unsettled=prefix_unsettled|root_unsettled;
endmodule
'''

TASK_WRAPPER='''
// Preserve CP-local arrival in the task/controller which actually supplies
// the physical clock-event predictions. Canonical f14 events retain their
// prefix/root ownership; no clock or redundant-Q check is masked.
module dorado_local_task_capture_step #(parameter integer WAVES=16)(
 input wire stop_tasks_capture,input wire [3:0] canonical_task_clock,
 input wire [87:0] state_q,input wire clock_before,clock_after,
 input wire [75:0] pins_before,pins_after,output wire [87:0] state_next,
 output wire [24:0] clock_events,repeated_events,output wire unsettled);
 wire split_event=SPLIT_EXPRESSION;
 wire [75:0] prefix_pins=split_event ?
  {pins_before[75:71],pins_after[70],pins_before[69:8],pins_after[7],pins_before[6:3],pins_after[2],pins_before[1],pins_after[0]} : pins_before;
 wire [87:0] prefix_q;
 wire [24:0] prefix_events,prefix_repeated,root_events,root_repeated;
 wire prefix_unsettled,root_unsettled;
 dorado_local_task_capture_atomic_step #(.WAVES(WAVES)) cp_arrival(
  stop_tasks_capture,canonical_task_clock[3:2],state_q,clock_before,clock_before,pins_before,prefix_pins,
  prefix_q,prefix_events,prefix_repeated,prefix_unsettled);
 dorado_local_task_capture_atomic_step #(.WAVES(WAVES)) root_arrival(
  stop_tasks_capture,canonical_task_clock[1:0],prefix_q,clock_before,clock_after,prefix_pins,pins_after,
  state_next,root_events,root_repeated,root_unsettled);
 assign clock_events=prefix_events|root_events;
 assign repeated_events=prefix_repeated|root_repeated|(prefix_events&root_events);
 assign unsettled=prefix_unsettled|root_unsettled;
endmodule
'''


def candidate(raw, mutation, include_task):
    old='module dorado_control_hold_ordered_step #(parameter integer WAVES=24)('
    raw=replace(raw,old,'module dorado_control_hold_atomic_step #(parameter integer WAVES=24)(')
    expression="1'b0" if mutation=='root_first' else '(clock_before!=clock_after) && (pins_before[292:282]!=pins_after[292:282])'
    wrapper=WRAPPER.replace('SPLIT_EXPRESSION',expression)
    if include_task:
        raw=replace(raw,' wire [155:0] joint_next;wire [55:0] joint_events,joint_repeated;',
                    ' wire [55:0] cp_prefix_events,cp_prefix_repeated;\n wire [155:0] joint_next;wire [55:0] joint_events,joint_repeated;')
        raw=replace(raw,'joint_next,joint_events,joint_repeated,cu,refresh_before_run_capture,control_flags);',
                    'joint_next,joint_events,joint_repeated,cu,refresh_before_run_capture,control_flags,cp_prefix_events,cp_prefix_repeated);')
        raw=replace(raw,'{joint_repeated[12],joint_events[12]}',
                    '{cp_prefix_repeated[12],cp_prefix_events[12],joint_repeated[12],(joint_events[12]&~cp_prefix_events[12])}')
        wrapper=replace(wrapper,'input wire refresh_before_run_capture,output wire [93:0] flags);',
                        'input wire refresh_before_run_capture,output wire [93:0] flags,\n output wire [55:0] cp_prefix_events,cp_prefix_repeated);')
        wrapper=replace(wrapper,' assign clock_events=prefix_events|root_events;',
                        ' assign cp_prefix_events=prefix_events;assign cp_prefix_repeated=prefix_repeated;\n assign clock_events=prefix_events|root_events;')
    return raw+wrapper


def processor(raw,mutation):
    raw=replace(raw,'module dorado_processor_capture_step #(parameter integer WAVES=16)(',
                'module dorado_processor_capture_atomic_step #(parameter integer WAVES=16)(')
    expression="1'b0" if mutation=='processor_root_first' else '''(clock_before!=clock_after) &&
  ((pins_before[0]!=pins_after[0]) || (pins_before[2]!=pins_after[2]) || (pins_before[7]!=pins_after[7]))'''
    return raw+PROCESSOR_WRAPPER.replace('SPLIT_EXPRESSION',expression)


def task(raw,mutation):
    raw=replace(raw,'module dorado_local_task_capture_step #(parameter integer WAVES=16)(',
                'module dorado_local_task_capture_atomic_step #(parameter integer WAVES=16)(')
    expression="1'b0" if mutation in ['processor_root_first','task_root_first'] else '''(clock_before!=clock_after) &&
  ((pins_before[0]!=pins_after[0]) || (pins_before[2]!=pins_after[2]) ||
   (pins_before[7]!=pins_after[7]) || (pins_before[70]!=pins_after[70]))'''
    return raw+TASK_WRAPPER.replace('SPLIT_EXPRESSION',expression)


def run(source,out,engine,mutation,include_processor,include_task,timing_source=None):
    r=json.loads((source/'run-state.json').read_text())
    assert r['engine']==engine and r['status']=='POINTER_PACKAGE_D_AT_CLOCK_AUDIT_FAILED'
    assert r['extra_coincident_CP_transactions'] and r['actions'][-1]['returncode']!=0
    assert 'R front solver unsettled/repeated' in (source/'test.log').read_text()
    timing=None;timing_rows={}
    if timing_source:
        timing=json.loads((timing_source/'run-state.json').read_text())
        assert timing['status']=='PASS_R_FRONT_ALL_ADDRESS_CAPTURE' and timing['engine']==engine
        assert timing['actions'][-1]['returncode']==0 and not timing.get('mutation')
        timing_rows={row['path']:row for row in timing['source_files']}
        assert set(timing_rows)=={row['path']for row in r['source_files']}
        for row in r['source_files']:
            p=timing_source/row['path'];assert sha(p)==timing_rows[row['path']]['output_sha256']
            if not row['path'].startswith('timed/') and p.name!='tb_r_front_local.sv':
                assert sha(p)==row['output_sha256'],('Unexpected functional profile change',row['path'])
    out.mkdir(parents=True,exist_ok=False);paths=[];records=[];task_adapters=[]
    for row in r['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];raw=p.read_text()
        if timing_source and row['path'].startswith('timed/'):
            raw=(timing_source/row['path']).read_text()
        if p.name=='candidate.sv':raw=candidate(raw,mutation,include_task)
        if include_processor and p.name=='dorado_processor_capture_step.sv':raw=processor(raw,mutation)
        if include_task and row['path'].startswith('adapters/'):
            if p.name=='dorado_local_task_capture_step.sv':
                raw=task(raw,mutation);task_adapters.append(row['path'])
            elif 'input wire [1:0] canonical_task_clock' in raw:
                raw=replace(raw,'input wire [1:0] canonical_task_clock','input wire [3:0] canonical_task_clock')
                task_adapters.append(row['path'])
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(raw);paths.append(str(q))
        records.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q),changed=sha(p)!=sha(q),
                            profile_source_sha256=timing_rows[row['path']]['output_sha256'] if timing_source and row['path'].startswith('timed/') else None))
    assert sum(x['changed'] for x in records if not x['path'].startswith('timed/'))==(2 if include_processor else 1)+len(task_adapters)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    defines=[x for x in (timing or r)['actions'][0]['command'] if x.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*paths],
              ['vvp','-i',str(out/'test.vvp')]] if engine=='icarus' else [
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),engine=engine,mutation=mutation,
               parent=str(source),parent_run_state_sha256=sha(source/'run-state.json'),source_files=records,actions=[],
               original_reference_and_checks_unchanged=timing_source is None,
               original_reference_unchanged_within_selected_profile=True,control_atomic_transfer_unchanged=True,
               timing_profile_source=str(timing_source) if timing_source else None,
               timing_profile_state_sha256=sha(timing_source/'run-state.json') if timing_source else None,
               processor_atomic_transfer_unchanged=True,processor_CP_subevent=include_processor,
               task_atomic_transfer_unchanged=True,task_CP_subevent=include_task,
               canonical_task_clock_stage_ownership=include_task,task_clock_adapters=task_adapters,
               CP_before_root_combinational_subevent=True,extra_architectural_state=False,
               extra_RAM_or_guest_cycle=False,qualifying_native_timing=False,assembly_permitted=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for i,cmd in enumerate(commands):
        with(out/('compile.log' if i==0 else 'test.log')).open('w') as log:
            p=subprocess.Popen(cmd,stdout=log,stderr=subprocess.STDOUT)
            a=dict(command=cmd,pid=p.pid,started=time.time());state['actions'].append(a);save()
            rc=p.wait();a.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text() if (out/'test.log').exists() else ''
    m=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',log)
    state.update(status='PASS_CP_BEFORE_ROOT_SUBEVENTS' if not rc and m else 'CP_BEFORE_ROOT_SUBEVENTS_FAILED',
                 summary=m[1] if m else None,finished=time.time());save();print(state['status'],flush=True)
    return rc if rc else (0 if m else 1)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--engine',choices=['icarus','verilator'],required=True)
    p.add_argument('--mutation',choices=['root_first','processor_root_first','task_root_first'])
    p.add_argument('--processor',action='store_true',help='Order direct CP inputs in the processor/control evaluator too')
    p.add_argument('--task',action='store_true',help='Order the actual task/controller evaluator and retain canonical clock-stage ownership')
    p.add_argument('--timing-profile-source',type=Path,help='Use exact independent timed cells from this qualified address-profile run')
    a=p.parse_args()
    assert a.mutation!='processor_root_first' or a.processor
    assert not a.task or a.processor
    assert a.mutation!='task_root_first' or a.task
    raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.engine,a.mutation,a.processor,a.task,
                        a.timing_profile_source.resolve() if a.timing_profile_source else None))
