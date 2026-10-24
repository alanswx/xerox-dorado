#!/usr/bin/env python3
"""Remove late memory Hold from MIR clock evaluation using original phases.

LOW memory capture clocks cannot rise: retain the initial Hold, with DisHold
clearing it only after the first transfer. HIGH masks this operand entirely.
Keep every other clock/data function, owner, SRAM, event and original check.
This is a component experiment, not native timing or speed qualification.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from prove_mir_clock_mask import function
from test_r_front_forward_view import sha
from test_r_front_task_event import replace


def candidate(raw,mutation=None,settling_waves=24,all_hold_clocks=False):
    start=raw.index('module dorado_control_hold_atomic_step ')
    end=raw.index('\nendmodule',start)
    body=raw[start:end]
    clocks=function(body,'clock_levels')
    shorter=re.sub(r'\bclock_levels\b','clock_levels_mir_epoch',clocks)
    shorter=replace(shorter,'input mir_switch_q);','input mir_switch_q,input mir_hold);')
    shorter=replace(shorter,'(p[69]&mir_switch_q)','(mir_hold&mir_switch_q)')
    if all_hold_clocks:
        shorter=replace(shorter,'repeat_cur=p[69] & q[43];','repeat_cur=mir_hold & q[43];')
    joint=function(body,'joint_clocks')
    new_joint=re.sub(r'\bjoint_clocks\b','joint_clocks_mir_epoch',joint)
    new_joint=replace(new_joint,'input tasking_view);','input tasking_view,input mir_hold);')
    new_joint=replace(new_joint,'clock_levels(clock_s,base,ca_pins(s,p),switch_view)',
                      'clock_levels_mir_epoch(clock_s,base,ca_pins(s,p),switch_view,mir_hold)')
    body=replace(body,joint,joint+'\n'+shorter+'\n'+new_joint)
    declaration=' reg [155:0] q,n;reg [55:0] previous,levels,capture;reg [23:0] ac;'
    body=replace(body,declaration,''' // Pure diagnostic wave vectors, not saved owners or pipeline state.
 reg [WAVES-1:0] mir_mask_diff,mir_mask_high_changed,mir_mask_low_clear;
 reg mir_epoch_hold;
'''+declaration)
    marker='  levels=previous;capture=0;ac=0;ap=0;mp=0;bp=0;clock_events=0;repeated_events=0;done=0;old_view=mir_switch_view;'
    body=replace(body,marker,marker+'''
  mir_mask_diff=0;mir_mask_high_changed=0;mir_mask_low_clear=0;mir_epoch_hold=0;''')
    original='   levels=joint_clocks(q,clock_after,pins_after,mir_switch_view,tasking_view);capture=levels&~previous;'
    epoch="(wave==0 || !pins_after[385]) ? hold_at(state_q[155:150]) : 1'b0"
    if mutation=='zero':epoch="1'b0"
    elif mutation=='invert':epoch='!('+epoch+')'
    elif mutation=='early-clear':epoch="pins_after[385] ? 1'b0 : hold_at(state_q[155:150])"
    elif mutation is not None:raise ValueError(mutation)
    body=replace(body,original,'''
   // Use the real first-wave input, then the documented asynchronous clear.
   // All other clocks still see actual current Hold through ca_pins/cb_pins.
   mir_epoch_hold=EPOCH;
   levels=joint_clocks_mir_epoch(q,clock_after,pins_after,mir_switch_view,tasking_view,mir_epoch_hold);
   mir_mask_diff[wave]=(levels!==joint_clocks(q,clock_after,pins_after,mir_switch_view,tasking_view));
   mir_mask_high_changed[wave]=clock_after && (mir_epoch_hold!=hold_at(q[155:150]));
   mir_mask_low_clear[wave]=!clock_after && pins_after[385] && wave!=0;
   capture=levels&~previous;'''.replace('EPOCH',epoch))
    result=raw[:start]+body+raw[end:]
    assert settling_waves in range(1,25)
    if settling_waves!=24:
        result=replace(result,'module dorado_control_hold_ordered_step #(parameter integer WAVES=24)(',
                       f'module dorado_control_hold_ordered_step #(parameter integer WAVES={settling_waves})(')
    return result


def bench(raw,original_only=False):
    if original_only:return raw
    marker=' reg [5:0] l17_changed=0;integer checks=0;'
    extra='''
 integer mir_mask_checks=0,mir_mask_high_distinct=0,mir_mask_low_clears=0;
 task automatic check_mir_mask;
  begin
   if(dut.control.cp_arrival.mir_mask_diff || dut.control.root_arrival.mir_mask_diff)
    $fatal(1,"MIR Hold clock substitution mismatch check=%0d prefix=%h root=%h",checks,
      dut.control.cp_arrival.mir_mask_diff,dut.control.root_arrival.mir_mask_diff);
   mir_mask_checks=mir_mask_checks+$bits(dut.control.cp_arrival.mir_mask_diff)+$bits(dut.control.root_arrival.mir_mask_diff);
   if(dut.control.cp_arrival.mir_mask_high_changed || dut.control.root_arrival.mir_mask_high_changed)
     mir_mask_high_distinct=mir_mask_high_distinct+1;
   if(dut.control.cp_arrival.mir_mask_low_clear || dut.control.root_arrival.mir_mask_low_clear)
     mir_mask_low_clears=mir_mask_low_clears+1;
  end
 endtask
'''
    raw=replace(raw,marker,marker+extra)
    for when in ['BEFORE','AFTER']:
        marker=f'     if(dut.current_r_flag_clocks!==clocks[32:25])$fatal(1,"R flag C mismatch {when} commit check=%0d",checks);'
        raw=replace(raw,marker,marker+'\n     check_mir_mask;')
    marker='   $display("R_FRONT_LOCAL_PROBE PASS checks=%0d writes=%0d reads=%0d epochs=%0d",checks,writes,reads,capture_epoch_changes);'
    raw=replace(raw,marker,'''   $display("MEMORY_MIR_MASK wave_checks=%0d high_distinct=%0d low_clear_epochs=%0d",mir_mask_checks,mir_mask_high_distinct,mir_mask_low_clears);
'''+marker)
    return raw


def run(source,proof,out,mutation,original_only=False,settling_waves=24,rtl_proof=None,all_hold_clocks=False,prepare_only=False):
    assert not out.exists()
    parent=json.loads((source/'run-state.json').read_text())
    assert parent['status']=='PASS_CP_BEFORE_ROOT_SUBEVENTS'and not parent['mutation']
    assert parent['summary']=='checks=14884 writes=238 reads=6928 epochs=392'
    formal=json.loads((proof/'proof.json').read_text())
    assert formal['status']=='PASS_SOURCE_BOUND_MEMORY_MIR_MASK'
    assert formal['candidate_sha256']==sha(source/'candidate.sv')
    assert [v['result']for v in formal['cases']]==['unsat']*4+['sat']*4
    assert not original_only or mutation is not None
    if settling_waves!=24 and not prepare_only:
        assert rtl_proof is not None
        quant=json.loads((rtl_proof/'run-state.json').read_text())
        assert quant['status']=='PASS_ALL_BINARY_CONTROL_MIR_EQUIVALENCE'and quant['proof_succeeded']
        assert quant['candidate_atomic_waves']==settling_waves and quant['reference_atomic_waves']==24
        assert quant['original_candidate_sha256']==sha(source/'candidate.sv')
        assert 'SAT proof finished - no model found: SUCCESS!'in(rtl_proof/'proof.log').read_text()
        actual=candidate((source/'candidate.sv').read_text(),mutation,settling_waves,all_hold_clocks)
        start=actual.index('module dorado_control_hold_atomic_step ')
        end=actual.index('\nendmodule',start)+len('\nendmodule')
        formal_body=actual[start:end].replace('module dorado_control_hold_atomic_step ','module candidate_atomic ',1)+'\n'
        assert formal_body==(rtl_proof/'candidate.sv').read_text(),'Proof applies to a different Hold substitution'
    # Validate both source substitutions before creating an experiment dir.
    generated_candidate=candidate((source/'candidate.sv').read_text(),mutation,settling_waves,all_hold_clocks)
    generated_bench=bench((source/'inputs/tb_r_front_local.sv').read_text(),original_only)
    out.mkdir(parents=True);paths=[];records=[]
    for row in parent['source_files']:
        p=source/row['path'];assert sha(p)==row['output_sha256'];raw=p.read_text()
        if p.name=='candidate.sv':raw=generated_candidate
        elif p.name=='tb_r_front_local.sv':raw=generated_bench
        q=out/row['path'];q.parent.mkdir(parents=True,exist_ok=True);q.write_text(raw);paths.append(str(q))
        records.append(dict(path=row['path'],parent_sha256=sha(p),output_sha256=sha(q),changed=sha(p)!=sha(q)))
    assert sum(v['changed']for v in records)==(1 if original_only else 2)
    old=(source/'inputs/tb_r_front_local.sv').read_text();new=(out/'inputs/tb_r_front_local.sv').read_text()
    for statement in re.findall(r'\$fatal\([^;]+;',old):assert statement in new,statement
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    engine=parent['engine'];defines=[s for s in parent['actions'][0]['command']if s.startswith('-D')]
    commands=[['iverilog','-g2012','-s','tb_r_front_local',*defines,'-o',str(out/'test.vvp'),*paths],
              ['vvp','-i',str(out/'test.vvp')]]if engine=='icarus'else[
        ['verilator','--binary','--timing','-j','2','-Wno-fatal','--top-module','tb_r_front_local',*defines,
         '--Mdir',str(out/'obj'),'-o','test',*paths],[str(out/'obj/test')]]
    state=dict(status='RUNNING',pid=os.getpid(),started=time.time(),scope=__doc__,engine=engine,mutation=mutation,
               parent=str(source),parent_state_sha256=sha(source/'run-state.json'),source_files=records,
               new_clock_substitution_assertion_enabled=not original_only,
               settling_waves=settling_waves,
               all_hold_clocks=all_hold_clocks,
               rtl_proof_sha256=sha(rtl_proof/'run-state.json')if rtl_proof else None,
               formal_proof=str(proof),formal_proof_sha256=sha(proof/'proof.json'),actions=[],
               original_reference_and_checks_retained=True,extra_guest_cycles=0,extra_pipeline_registers=0,
               physical_SRAM_banks=4,native_qualified=False,physical_timing_qualified=False,
               assembly_permitted=False,speedup_qualified=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    save()
    if prepare_only:
        state['status']='PREPARED_MEMORY_MIR_MASK_COMPONENT';save();print(state['status'],flush=True);return 0
    for i,command in enumerate(commands):
        with(out/('compile.log'if i==0 else'test.log')).open('w')as log:
            p=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            action=dict(command=command,pid=p.pid,started=time.time());state['actions'].append(action);save()
            rc=p.wait();action.update(returncode=rc,finished=time.time());save()
        if rc:break
    log=(out/'test.log').read_text()if(out/'test.log').exists()else''
    match=re.search(r'R_FRONT_LOCAL_PROBE PASS ([^\n]+)',log)
    coverage=re.search(r'^MEMORY_MIR_MASK (.+)$',log,re.M)
    passed=not rc and bool(match)and bool(coverage)
    state.update(status='PASS_MEMORY_MIR_MASK_ORIGINAL_COMPARISON'if passed else'MEMORY_MIR_MASK_FAILED',
                 summary=match[1]if match else None,mask_coverage=coverage[1]if coverage else None,finished=time.time())
    if original_only and rc==0:state['status']='UNEXPECTED_ORIGINAL_ONLY_MUTATION_PASS'
    if passed:
        clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
        assert len(clocks)==100 and clocks==re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',(source/'test.log').read_text(),re.M)
        state['all100_original_clock_counts_match']=True
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['source','proof','output']:parser.add_argument('--'+name,type=Path,required=True)
    parser.add_argument('--mutation',choices=['zero','invert','early-clear'])
    parser.add_argument('--original-checks-only',action='store_true',help='Negative control: use byte-identical original bench with no new diagnostic assertions')
    parser.add_argument('--settling-waves',type=int,default=24,choices=range(1,25))
    parser.add_argument('--rtl-proof',type=Path)
    parser.add_argument('--all-hold-clocks',action='store_true',help='Use the same phase mask for RepeatCurrent/task clocks as well as MIR clocks')
    parser.add_argument('--prepare-only',action='store_true',help='Freeze a source variant for formal proof; no simulator or qualification')
    args=parser.parse_args();raise SystemExit(run(args.source.resolve(),args.proof.resolve(),args.output.resolve(),args.mutation,args.original_checks_only,args.settling_waves,args.rtl_proof.resolve()if args.rtl_proof else None,args.all_hold_clocks,args.prepare_only))
