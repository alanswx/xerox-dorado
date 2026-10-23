#!/usr/bin/env python3
"""Prove the original LOW-root clock mask used by the MIR experiment.

This is a source-bound Boolean proof of the controller subset and masked task
data, not a full next-state equivalence proof, analog/skew model or FPGA timing
qualification. Complete original-graph comparisons remain necessary.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[1]
BASELINE = 'verilog/rtl/dorado_r_front_commands_step.sv'
CONTROL = 'verilog/rtl/dorado_conta_local_step.sv'
DECL = '''(set-logic QF_UF)
(declare-const before_root Bool)
(declare-const q18 Bool)
(declare-const q6 Bool)
(declare-const q7 Bool)
(declare-const q25 Bool)
(declare-const rstop Bool)
(declare-const stop_at_t1 Bool)
(declare-const d_run Bool)
(declare-const d_stop Bool)
(declare-const d_start Bool)
(define-fun instruction () Bool (or q6 (not q7)))
(define-fun normal0 () Bool (or instruction (not q25)))
'''
STEP = '''
(declare-const previous_run Bool)
(declare-const previous_instruction Bool)
(assert (=> q18 previous_run))
(assert (=> instruction previous_instruction))
(define-fun capture_run () Bool (and q18 (not previous_run)))
(define-fun capture_instruction () Bool (and instruction (not previous_instruction)))
(define-fun phase_init () Bool PHASE_INIT)
(define-fun next_run () Bool NEXT_RUN)
(define-fun next_stop () Bool (ite rstop false (ite capture_instruction d_stop q6)))
(define-fun next_phase0 () Bool (ite phase_init false (ite capture_instruction d_start q25)))
(define-fun next_instruction () Bool (or next_stop (not next_run)))
(define-fun next_normal0 () Bool (or next_instruction (not next_phase0)))
(assert (not (and (=> next_instruction instruction) (=> next_normal0 normal0))))
(check-sat)
'''


def function(raw, name):
    found = re.findall(r'(?m)^[ \t]*function automatic[^\n]*\b'+name+r'\(.*?endfunction', raw, re.S)
    assert len(found) == 1, (name, len(found))
    return found[0]


def prove(candidate, out):
    assert not out.exists()
    baseline = (ROOT/BASELINE).read_text()
    control = (ROOT/CONTROL).read_text()
    assert hashlib.sha256(baseline.encode()).hexdigest() == '3110f49e6dc9e3eef6b3dcdd341d1d1ee05b3f5d14b1a3613663dfd419eee350'
    assert hashlib.sha256(control.encode()).hexdigest() == 'e109ac28840e67e0415a04e1f121384e980ad0e8aa4bbbc9de875e6a5212f934'
    for name in ['controller_clocks', 'controller_transfer', 'task_transfer']:
        assert function(baseline, name) == function(control, name), name
        assert function(candidate.read_text(), name) == function(control, name), name
    for snippet in ["normal0=base | q[6] | !q[7] | !q[25]", 'mir_switch_view=state_q[43]',
                    'if(q[69])mir_switch_view=1;', 'levels=clock_levels(q,clock_after,pins_after,mir_switch_view)',
                    'mir_clock=normal0 | (p[69]&mir_switch_q) | p[125]']:
        assert snippet in candidate.read_text(), snippet
    initial = DECL+'''
(define-fun before_run () Bool (or before_root q18))
(define-fun before_instruction () Bool (or before_root instruction))
(define-fun before_normal0 () Bool (or before_root normal0))
(assert (or (and q18 (not before_run))
            (and instruction (not before_instruction))
            (and normal0 (not before_normal0))))
(check-sat)
'''
    induction = DECL+STEP
    induction = induction.replace('PHASE_INIT', '(and (not stop_at_t1) instruction)')
    induction = induction.replace('NEXT_RUN', '(ite capture_run d_run q7)')
    high = '''(set-logic QF_UF)
(declare-const actual_switch Bool)(declare-const temporary_switch Bool)
(declare-const hold Bool)(declare-const stop_mir Bool)
(assert (not (= (or true (and hold actual_switch) stop_mir)
                (or true (and hold temporary_switch) stop_mir))))
(check-sat)
'''
    low = '''(set-logic QF_UF)
(declare-const actual_switch Bool)(declare-const temporary_switch Bool)
(declare-const no_dispatch Bool)
(assert (= actual_switch temporary_switch))
(define-fun next_actual () Bool (ite no_dispatch true actual_switch))
(define-fun next_temporary () Bool TEMPORARY_UPDATE)
(assert (not (= next_actual next_temporary)))
(check-sat)
'''
    cases = [
        ('LOW_initial_no_clock_rise', initial, 'unsat'),
        ('LOW_inductive_no_normal0_rise', induction, 'unsat'),
        ('HIGH_masks_arbitrary_task_data', high, 'unsat'),
        ('LOW_preserves_async_SwitchUp_epoch', low.replace('TEMPORARY_UPDATE', '(ite no_dispatch true temporary_switch)'), 'unsat'),
        ('bad_phase_init_without_instruction_mask', induction.replace('(and (not stop_at_t1) instruction)', '(not stop_at_t1)'), 'sat'),
        ('bad_run_capture_without_clock', induction.replace('(ite capture_run d_run q7)', 'd_run'), 'sat'),
        ('bad_frozen_switch_loses_NoDispatch', low.replace('TEMPORARY_UPDATE', 'temporary_switch'), 'sat'),
    ]
    out.mkdir(parents=True)
    rows = []
    for name, smt, expected in cases:
        p = out/(name+'.smt2')
        p.write_text(smt)
        result = subprocess.run(['z3', str(p)], capture_output=True, text=True, check=True)
        (out/(name+'.log')).write_text(result.stdout+result.stderr)
        assert result.stdout.strip() == expected, (name, result.stdout, result.stderr)
        rows.append(dict(case=name, result=expected))
    paths = {'baseline.sv': ROOT/BASELINE, 'control.sv': ROOT/CONTROL,
             'candidate.sv': candidate, 'proof.py': Path(__file__)}
    hashes = {}
    for name, p in paths.items():
        (out/name).write_bytes(p.read_bytes())
        hashes[name] = hashlib.sha256(p.read_bytes()).hexdigest()
    record = dict(status='PASS_SOURCE_BOUND_MIR_CLOCK_MASK_PROPERTIES', scope=__doc__,
                  cases=rows, inputs=hashes, clock_qualification='Boolean original-controller subset only',
                  full_next_state_equivalence_proven=False, physical_timing_qualified=False,
                  native_integrated=False)
    (out/'proof.json').write_text(json.dumps(record, indent=2)+'\n')
    print(record['status'], '4 proofs; 3 invalid-mask counterexamples')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--candidate', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    prove(a.candidate.resolve(), a.output.resolve())
