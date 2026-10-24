#!/usr/bin/env python3
"""Prove the source-bound LOW-phase memory-clock recurrence for MIR Hold.

This is a binary controller/memory clock proof, not complete next-state
equivalence, analog source timing, physical timing or native qualification.
Original timed-graph comparisons are also required for the proposed rewrite.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess

from prove_mir_clock_mask import function
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]

DECL='''(set-logic QF_UF)
(declare-const before_root Bool)
(declare-const stop Bool)(declare-const run Bool)
(declare-const cp_phase Bool)(declare-const run_phase Bool)
(declare-const cpu_run Bool)(declare-const cpu_stop Bool)
(declare-const mem_run Bool)(declare-const mem_stop Bool)
(declare-const refresh Bool)(declare-const mem_sh Bool)
(declare-const rstop Bool)(declare-const stop_at_t1 Bool)
(declare-const d_stop Bool)(declare-const d_run Bool)(declare-const d_start Bool)
(declare-const d_mem_sh Bool)(declare-const want_refresh Bool)
(declare-const dis_hold Bool)
(declare-const h2 Bool)(declare-const h3 Bool)(declare-const h4 Bool)
(define-fun run_clock () Bool run_phase)
(define-fun instruction () Bool (or stop (not run)))
(define-fun normal0 () Bool (or instruction (not cp_phase)))
(define-fun mem_enable () Bool (or (not mem_run) mem_stop))
(define-fun cpu_enable () Bool (or (not cpu_run) cpu_stop))
(define-fun mem_phase () Bool (or mem_enable (not mem_sh)))
(define-fun hold_clock () Bool (or mem_phase cpu_enable))
(define-fun old_hold () Bool (or h4 (not h2) (not h3)))
'''
STEP='''
(declare-const prev_run Bool)(declare-const prev_instruction Bool)
(declare-const prev_normal0 Bool)(declare-const prev_mem Bool)
(declare-const prev_mem_phase Bool)(declare-const prev_hold Bool)
(assert (=> run_clock prev_run))
(assert (=> instruction prev_instruction))
(assert (=> normal0 prev_normal0))
(assert (=> mem_enable prev_mem))
(assert (=> mem_phase prev_mem_phase))
(assert (=> hold_clock prev_hold))
(define-fun cap_run () Bool (and run_clock (not prev_run)))
(define-fun cap_instruction () Bool (and instruction (not prev_instruction)))
(define-fun cap_mem () Bool (and mem_enable (not prev_mem)))
(define-fun cap_hold () Bool (and hold_clock (not prev_hold)))
(define-fun next_stop () Bool (ite rstop false (ite cap_instruction d_stop stop)))
(define-fun next_run () Bool (ite cap_run d_run run))
(define-fun next_cpu_run () Bool (ite cap_run d_run cpu_run))
(define-fun next_cpu_stop () Bool (ite rstop false (ite cap_instruction d_stop cpu_stop)))
(define-fun next_refresh () Bool (ite cap_run want_refresh refresh))
(define-fun next_mem_run () Bool (ite refresh true (ite cap_run d_run mem_run)))
(define-fun next_mem_stop () Bool (ite (or rstop refresh) false (ite cap_instruction d_stop mem_stop)))
(define-fun phase_init () Bool (and (not stop_at_t1) instruction))
(define-fun next_cp_phase () Bool (ite phase_init false (ite cap_instruction d_start cp_phase)))
(define-fun next_mem_sh () Bool (ite cap_mem d_mem_sh mem_sh))
(define-fun next_instruction () Bool (or next_stop (not next_run)))
(define-fun next_normal0 () Bool (or next_instruction (not next_cp_phase)))
(define-fun next_mem_enable () Bool (or (not next_mem_run) next_mem_stop))
(define-fun next_cpu_enable () Bool (or (not next_cpu_run) next_cpu_stop))
(define-fun next_mem_phase () Bool (or next_mem_enable (not next_mem_sh)))
(define-fun next_hold_clock () Bool (or next_mem_phase next_cpu_enable))
'''


def prove(source,out):
    assert not out.exists()
    state=json.loads((source/'run-state.json').read_text())
    assert state['status']=='PASS_CP_BEFORE_ROOT_SUBEVENTS'
    raw=(source/'candidate.sv').read_text()
    atomic=raw.split('module dorado_control_hold_atomic_step ',1)[1].split('\nendmodule',1)[0]
    control=ROOT/'verilog/rtl/dorado_conta_local_step.sv'
    assert sha(control)=='e109ac28840e67e0415a04e1f121384e980ad0e8aa4bbbc9de875e6a5212f934'
    for name in ['controller_clocks','controller_transfer']:
        assert function(atomic,name).lstrip()==function(control.read_text(),name).lstrip()
    required=[
        'hold_at=q[4] | !q[2] | !q[3];',
        'phase1=p[0]|p[56]|!q[0];',
        'memory_clocks={phase1,{3{phase1|p[57]}},p[56],p[0]|p[56]};',
        "n=q;if(capture[0])n[0]=p[58];",
        "if(p[59])begin n[3:2]=2'b11;n[4]=0;n[5]=1;end",
        'if(capture[2])n[2]=p[60];if(capture[3])n[3]=p[61];',
        'if(capture[4])n[4]=p[62];if(capture[5])n[5]=p[63];',
        'b[56]=!s[15]|s[17];b[57]=!s[8]|s[10];b[58]=!s[22];',
        'b=p[397:326];b[0]=base;',
        'normal0=base | q[6] | !q[7] | !q[25];',
        'mir_clock=normal0 | (p[69]&mir_switch_q) | p[125];',
        'a[69]=hold_at(s[155:150]);',
        'previous=joint_clocks(q,clock_before,pins_before,mir_switch_view,tasking_view);',
    ]
    for text in required:assert text in atomic,text
    initial=DECL+'''
(assert (not (and
 (=> run_clock (or before_root run_clock))
 (=> instruction (or before_root instruction))
 (=> normal0 (or before_root normal0))
 (=> mem_enable (or before_root mem_enable))
 (=> mem_phase (or before_root mem_phase))
 (=> hold_clock (or before_root hold_clock)))))
(check-sat)
'''
    invariant='''(and (not cap_run) (not cap_instruction) (not cap_mem) (not cap_hold)
 (= next_refresh refresh) (=> next_instruction instruction) (=> next_normal0 normal0)
 (=> next_mem_enable mem_enable) (=> next_mem_phase mem_phase)
 (=> next_hold_clock hold_clock))'''
    induction=DECL+STEP+'(assert (not '+invariant+'))\n(check-sat)\n'
    hold=DECL+STEP+'''
(declare-const d2 Bool)(declare-const d3 Bool)(declare-const d4 Bool)
(define-fun next_h2 () Bool (ite dis_hold true (ite cap_hold d2 h2)))
(define-fun next_h3 () Bool (ite dis_hold true (ite cap_hold d3 h3)))
(define-fun next_h4 () Bool (ite dis_hold false (ite cap_hold d4 h4)))
(define-fun next_hold () Bool (or next_h4 (not next_h2) (not next_h3)))
(assert (not (= next_hold (ite dis_hold false old_hold))))
(check-sat)
'''
    mask='''(set-logic QF_UF)
(declare-const hold Bool)(declare-const substitute Bool)
(declare-const normal0 Bool)(declare-const stop_mir Bool)(declare-const switch Bool)
(assert (not (= (or true normal0 (and hold switch) stop_mir)
                (or true normal0 (and substitute switch) stop_mir))))
(check-sat)
'''
    wrong='''(set-logic QF_UF)
(declare-const hold Bool)(declare-const normal0 Bool)
(declare-const stop_mir Bool)(declare-const switch Bool)
(assert (not (= (or normal0 (and hold switch) stop_mir)
                (or normal0 (and false switch) stop_mir))))
(check-sat)
'''
    early=wrong.replace('(and false switch)','(and (ite true false hold) switch)')
    cases=[('LOW_initial_clock_covers',initial,'unsat'),
           ('LOW_inductive_no_memory_capture',induction,'unsat'),
           ('LOW_Hold_retained_or_DisHold_cleared',hold,'unsat'),
           ('HIGH_masks_arbitrary_Hold',mask,'unsat'),
           ('bad_unclocked_Run_update',induction.replace('(ite cap_run d_run run)','d_run'),'sat'),
           ('bad_phase_init_without_instruction_mask',induction.replace('(and (not stop_at_t1) instruction)','(not stop_at_t1)'),'sat'),
           ('bad_constant_zero_Hold',wrong,'sat'),
           ('bad_DisHold_applied_before_first_transfer',early,'sat')]
    out.mkdir(parents=True);rows=[]
    for name,text,expected in cases:
        path=out/(name+'.smt2');path.write_text(text)
        result=subprocess.run(['z3',str(path)],capture_output=True,text=True)
        (out/(name+'.log')).write_text(result.stdout+result.stderr)
        assert result.returncode==0 and result.stdout.strip()==expected,(name,result.stdout,result.stderr)
        rows.append(dict(case=name,result=expected,sha256=sha(path)))
    for name,path in [('candidate.sv',source/'candidate.sv'),('original-control.sv',control),('proof.py',Path(__file__))]:
        (out/name).write_bytes(path.read_bytes())
    result=dict(status='PASS_SOURCE_BOUND_MEMORY_MIR_MASK',scope=__doc__,cases=rows,
                parent=str(source),parent_state_sha256=sha(source/'run-state.json'),
                candidate_sha256=sha(source/'candidate.sv'),original_control_sha256=sha(control),
                pin_binding=dict(DisHold=385,Hold_state_bits=[152,153,154]),
                arbitrary_state_and_binary_inputs=True,complete_next_state_equivalence_proven=False,
                original_cells_changed=False,physical_timing_qualified=False,native_qualified=False)
    (out/'proof.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],'4 recurrence/mask proofs;4 invalid-rewrite counterexamples',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output']:parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args();prove(args.source.resolve(),args.output.resolve())
