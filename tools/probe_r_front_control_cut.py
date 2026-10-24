#!/usr/bin/env python3
"""Test a registered control/operand cut against the original timed graph.

This is a two-host-edge COMPONENT transaction, not a native phase scheduler.
The first edge saves only control evaluation; the second commits the existing
physical SRAM ports. Old owner state and pins remain stable across the cut.
Every parent reference, physical-state, clock, RAM and fixed-point check stays.
No native fit, guest instruction cadence or CPU speed is qualified here.
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


CONTROL = '''module dorado_r_front_control_launch(
 input wire [843:0] held_q,input wire clock_before,clock_after,
 input wire [400:0] pins_before,pins_after,input wire refresh_before_run_capture,
 output wire [474:0] result);
 wire [109:0] cq={held_q[797:792],held_q[791:786],held_q[785:764],held_q[763:752],held_q[751:736],held_q[79:64],held_q[31:0]};
 wire [155:0] joint_q={held_q[843:798],cq};
 wire [155:0] joint_next;wire [55:0] joint_events,joint_repeated;
 wire cu;wire [93:0] control_flags;wire [55:0] cp_prefix_events,cp_prefix_repeated;
 dorado_control_hold_ordered_step control(joint_q,clock_before,clock_after,pins_before[397:0],pins_after[397:0],
  joint_next,joint_events,joint_repeated,cu,refresh_before_run_capture,control_flags,cp_prefix_events,cp_prefix_repeated);
 assign result={joint_next,joint_events,joint_repeated,cu,control_flags,cp_prefix_events,cp_prefix_repeated};
endmodule
'''


def candidate(raw):
    raw=replace(raw,'parameter integer STOP_CAPTURE_OLD=1,STOP_CLOCK_NEW=1,BRANCH_CAPTURE_OLD=1)(',
                'parameter integer STOP_CAPTURE_OLD=1,STOP_CLOCK_NEW=1,BRANCH_CAPTURE_OLD=1,CONTROL_CUT=0)(')
    raw=replace(raw,'output wire shared_state_mismatch,output wire [93:0] control_flags,output wire [843:0] physical_state\n',
                'output wire shared_state_mismatch,output wire [93:0] control_flags,output wire [843:0] physical_state,\n input wire [474:0] control_result\n')
    call=''' dorado_control_hold_ordered_step control(joint_q,clock_before,clock_after,pins_before[397:0],pins_after[397:0],
  joint_next,joint_events,joint_repeated,cu,refresh_before_run_capture,control_flags,cp_prefix_events,cp_prefix_repeated);'''
    assert raw.count(call)==1
    raw=replace(raw,call,''' wire [474:0] local_control_result;
 dorado_r_front_control_launch control_launch(held_q,clock_before,clock_after,pins_before,pins_after,
  refresh_before_run_capture,local_control_result);
 assign {joint_next,joint_events,joint_repeated,cu,control_flags,cp_prefix_events,cp_prefix_repeated}=
  CONTROL_CUT ? control_result : local_control_result;''')
    return raw+'\n'+CONTROL


def bench(raw):
    raw=replace(raw,' dorado_r_front_control_step dut(host_clk,advance,stored_q,cb,ca,pb,pa,1\'b0,n,events,repeated,unsettled,',
                ''' wire [474:0] cut_live;reg [474:0] cut_saved;
 reg cut_use_saved=0;
 dorado_r_front_control_launch cut_producer(stored_q,cb,ca,pb,pa,1'b0,cut_live);
 // The control stage never advances SRAM. The original second host edge
 // commits its single set of four physical banks once, with the old epoch.
 always @(posedge host_clk)if(!advance)cut_saved<=cut_live;
 dorado_r_front_control_step #(.CONTROL_CUT(1)) dut(host_clk,advance,stored_q,cb,ca,pb,pa,1'b0,n,events,repeated,unsettled,''')
    raw=replace(raw,'port_address,port_select,raw_data,raw_parity,source_pins,fa_n,ready_flags,conta_next,shared_mismatch,control_flags,q);',
                'port_address,port_select,raw_data,raw_parity,source_pins,fa_n,ready_flags,conta_next,shared_mismatch,control_flags,q,cut_use_saved ? cut_saved : cut_live);')
    raw=replace(raw,'if(memory_image()!==old_memory) $fatal(1,"R front advance hold changed RAM");',
                '''if(memory_image()!==old_memory) $fatal(1,"R front advance hold changed RAM");
     cut_use_saved=1;#100;
     if(n!==expected || events!==predicted || control_flags!==expected_control_flags)
       $fatal(1,"Registered control cut lost its original capture epoch check=%0d",checks);
     if(unsettled || repeated || shared_mismatch)
       $fatal(1,"Registered control cut rejected its saved control result check=%0d",checks);''')
    raw=replace(raw,'stored_q=expected;cb=next_root;pb=pins;pb[144:127]=0;#100;',
                '''stored_q=expected;cb=next_root;pb=pins;pb[144:127]=0;
     // Post-commit physical views are evaluated from the new owners, rather
     // than reusing a transaction's saved clock-event descriptor.
     cut_use_saved=0;#100;''')
    return raw


def run(source,out):
    assert not out.exists()
    parent=json.loads((source/'run-state.json').read_text());engine=parent['engine']
    assert parent['status']=='PASS_CP_BEFORE_ROOT_SUBEVENTS'
    assert parent['summary']=='checks=14884 writes=238 reads=6928 epochs=392'
    assert parent['actions'][-1]['returncode']==0 and parent['task_CP_subevent']
    out.mkdir(parents=True);paths=[];records=[]
    for row in parent['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];raw=p.read_text()
        if p.name=='candidate.sv':raw=candidate(raw)
        elif p.name=='tb_r_front_local.sv':raw=bench(raw)
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(raw);paths.append(str(q))
        records.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q),changed=sha(p)!=sha(q)))
    assert sum(v['changed']for v in records)==2
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    defines=[s for s in parent['actions'][0]['command']if s.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*paths],
              ['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),scope=__doc__,engine=engine,
               parent=str(source),parent_state_sha256=sha(source/'run-state.json'),source_files=records,actions=[],
               registered_control_bits=475,physical_owner_bits=844,RAM_bits=9216,
               original_reference_and_checks_unchanged=True,transaction_old_state_and_pins_held=True,
               original_four_SRAM_banks_retained=True,control_stage_RAM_advance=False,
               post_commit_views_use_new_owners=True,extra_architectural_state=False,
               native_phase_schedule_qualified=False,extra_guest_cycle_excluded=False,
               native_qualified=False,assembly_permitted=False,speedup_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    for i,command in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            a=dict(command=command,pid=p.pid,started=time.time());state['actions'].append(a);save()
            rc=p.wait();a.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    match=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',log)
    passed=not rc and bool(match)
    state.update(status='PASS_REGISTERED_CONTROL_COMPONENT_CUT'if passed else'REGISTERED_CONTROL_COMPONENT_CUT_FAILED',
                 summary=match[1]if match else None,finished=time.time());save();print(state['status'],flush=True)
    return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve()))
