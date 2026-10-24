#!/usr/bin/env python3
"""Retain the balanced task encoder's complete comparisons and timing result.

Pure function substitution is proved at both five and24 atomic waves, with
all363 outputs retained and corrupted-output counterexamples. Whole original
component comparisons and475-bit cache induction remain separate gates.
This receipt cannot qualify full-native clocks, Cedar or hardware performance.
"""
import argparse
import json
from pathlib import Path

from balance_control_priority import transform
from fit_cp_prefix_cache import gate
from review_late_hold_and_input_reset import retain
from review_native_cp_context import review_fit
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/rtl-executive'


def review(out, fit):
    assert not out.exists()
    out.mkdir(parents=True)
    proofs = []
    for name, broken in [('native-balanced-priority-proof-v1-20261024', False),
                         ('native-balanced-priority-bad-v1-20261024', True)]:
        source = BUILD / name
        p = json.loads((source / 'run-state.json').read_text())
        assert p['status'] == ('PASS_BALANCED_PRIORITY_COUNTEREXAMPLES' if broken else 'PASS_BALANCED_PRIORITY_ALL_ATOMIC_OUTPUTS')
        assert p['broken_low_bit_control'] == broken and not p['assumptions_added']
        assert p['state_and_input_bits'] == 955 and p['atomic_result_bits'] == 363
        assert [(r['waves'], r['passed']) for r in p['results']] == [(5, True), (24, True)]
        for name, digest in p['files'].items():
            assert sha(source / name) == digest, name
        assert (source / 'candidate.sv').read_text() == transform((source / 'original.sv').read_text(), broken)
        retain(source, out / source.name)
        proofs.append(dict(directory=source.name, status=p['status'], state_sha256=sha(source / 'run-state.json')))
    gates = []
    for engine in ['icarus', 'verilator']:
        source = BUILD / f'cp-prefix-balanced-{engine}-v1-20261024'
        state, checked = gate(source, engine)
        assert state['balanced_task_priority'] and state['settling_waves'] == 5 and state['direct_cp_projection']
        assert checked['coverage'] == dict(fast=13347, staged=1537, wait_edges=1553, retries=7, resets=1)
        retain(source, out / source.name)
        gates.append(checked)
    assert gates[0]['candidate_sha256'] == gates[1]['candidate_sha256'] and gates[0]['clocks'] == gates[1]['clocks']
    source = BUILD / 'cp-prefix-balanced-induction-v1-20261024'
    induction = json.loads((source / 'run-state.json').read_text())
    assert induction['status'] == 'PASS_CP_CACHE_INDUCTIVE_READY_EQUIVALENCE'
    assert induction['unbounded_induction_proven'] and induction['balanced_task_priority']
    assert induction['compared_control_output_bits'] == 475
    assert induction['source_bound_SRAM_permission'] == 'request && ready'
    for name, digest in induction['source_files'].items():
        assert sha(source / name) == digest, name
    assert sha(source / 'proof.log') == induction['proof_log_sha256']
    retain(source, out / source.name)
    fp = json.loads((fit / 'provenance.json').read_text())
    assert fp['balanced_priority_proof']['state_sha256'] == proofs[0]['state_sha256']
    assert fp['balanced_priority_proof']['complete_original_comparisons'] == json.loads(json.dumps(gates))
    positive = BUILD / 'native-balanced-priority-proof-v1-20261024'
    assert (fit / 'dorado_r_front_control_step.sv').read_bytes() == (positive / 'candidate.sv').read_bytes()
    failure = BUILD / 'balanced-control-priority-review-v1-20261024'
    if (failure / 'failure.json').exists():
        retain(failure, out / ('superseded-' + failure.name))
    result = dict(status='REVIEWED_BALANCED_CONTROL_PRIORITY_COMPONENT', scope=__doc__,
        atomic_proofs=proofs, original_graph_comparisons=gates,
        all_original_physical_Q_bits=844, all_original_RAM_bits=9216, all_original_clock_counts=100,
        unbounded475_bit_ready_equivalence=True, SRAM_permission='request && ready',
        induction_state_sha256=sha(source / 'run-state.json'), caller_timing=review_fit(fit, out),
        source_change='Only two task-priority encoder functions; no owner/capture/SRAM/clock change',
        native_timing_qualified=False, original60ns_qualified=False, firmware_qualified=False,
        hardware_changed=False, assembly_permitted=False, speedup_qualified=False, full_goal_active=True)
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'fit', 'archive']:
        p.add_argument('--' + name, type=Path, required=True)
    a = p.parse_args()
    r = review(a.output.resolve(), a.fit.resolve())
    append_archive(a.archive.resolve(), a.output.resolve(), [], label='balanced-control-priority', extra_tools=[
        'balance_control_priority.py', 'fit_native_cp_caller.py', 'probe_cp_prefix_cache.py',
        'prove_cp_prefix_cache.py', 'fit_cp_prefix_cache.py', 'review_native_cp_context.py', Path(__file__).name])
    print(r['status'], flush=True)
