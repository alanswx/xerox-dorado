#!/usr/bin/env python3
"""Retain selector independence, complete comparisons and native context gates.

The failed swapped-cofactor negative is evidence against the original premise,
not a passing negative control. V2 proves the constant projection separately.
No native fit, timing, firmware or speed acceptance follows from these gates.
The user's FPGA remains reserved; this review performs no hardware operation.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re

from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def read(path):
    return json.loads(path.read_text())


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    records = []

    def checked(name, status):
        directory = BUILD/name
        state = read(directory/'run-state.json')
        assert state['status'] == status, name
        for file, digest in state.get('files', {}).items():
            assert sha(directory/file) == digest, (name, file)
        retain(directory, out/name)
        records.append(dict(directory=name, status=status,
                            state_sha256=sha(directory/'run-state.json')))
        return directory, state

    checked('selector-preview-positive-v1-20261024', 'PASS_SELECTOR_PREVIEW_ALL_PRODUCER_BITS')
    d, failed = checked('selector-preview-negative-v1-20261024', 'TERMINAL_SELECTOR_PREVIEW_NOT_ESTABLISHED')
    assert len(failed['results']) == 1 and failed['results'][0]['returncode'] == 1
    log = (d/'proof-5.log').read_text()
    assert 'Called with -falsify and proof did succeed!' in log
    assert 'no model found: SUCCESS!' in log
    assert sha(d/'proof-5.log') == failed['results'][0]['log_sha256']
    for variant, status in [('positive', 'PASS_SELECTOR_PREVIEW_ALL_PRODUCER_BITS'),
                            ('negative', 'PASS_SELECTOR_PREVIEW_COUNTEREXAMPLES')]:
        d, s = checked(f'selector-preview-{variant}-v2-20261024', status)
        assert s['compared_output_bits'] == 482 and s['arbitrary_binary_input_bits'] == 1253
        assert not s['protocol_or_state_assumptions'] and not s['extra_saved_state_or_guest_cycles']
        assert [(r['waves'], r['passed'], r['returncode']) for r in s['results']] == [(5, True, 0), (24, True, 0)]
        for row in s['results']:
            assert sha(d/f"proof-{row['waves']}.log") == row['log_sha256']
    for engine in ['icarus', 'verilator']:
        d, s = checked(f'selector-{engine}-v2-20261024', 'PASS_SELECTOR_COMPLETE_ORIGINAL_COMPARISON')
        assert s['all100_clock_counts_match'] and s['original_testbench_and_references_unchanged']
        assert all(a['returncode'] == 0 for a in s['actions'])
        assert 'PASS checks=14884 writes=238 reads=6928 epochs=392' in (d/'test.log').read_text()
    d, _ = checked('selector-four-state-v2-20261024', 'PASS_SELECTOR_PREVIEW_FOUR_STATE')
    assert 'producer_checks=5140 unknown_checks=5012 fallback_checks=32 inputs=1253 outputs=326 waves=5/24' in (d/'test.log').read_text()
    d, s = checked('selector-bad-unknown-v2-20261024', 'PASS_SELECTOR_UNKNOWN_FALLBACK_COUNTEREXAMPLE')
    assert [a['returncode'] for a in s['actions']] == [0, 1]
    assert 'mismatch' in (d/'test.log').read_text()
    for name in ['selector-native-generic-stage-v2-20261024', 'selector-native-stage-v2-20261024']:
        d = BUILD/name
        s = read(d/'r-front-control-native.json')
        assert len(s['source_hashes']) == 533 and not s['assembly_permitted']
        for file, digest in s['source_hashes'].items():
            assert sha(d/file) == digest, (name, file)
        retain(d, out/name)
    _, s = checked('selector-native-context-proof-v2-20261024', 'PASS_SOURCE_BOUND_NATIVE_CP_CONTEXT')
    assert s['unbounded_induction_proven'] and s['owned_projection_bits'] == 156
    assert s['auxiliary_state_compared_bits'] == 1416 and not s['input_or_protocol_assumptions_added']
    assert s['owner_power_up_state'] == 'unconstrained'
    _, s = checked('selector-native-context-stale-v2-20261024', 'PASS_STALE_NATIVE_OWNER_COUNTEREXAMPLE')
    assert s['counterexample_found'] and not s['unbounded_induction_proven']
    d, s = checked('selector-native-lint-v2-20261024', 'PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION')
    assert s['structural_warnings_visible']

    def warnings(path):
        return Counter(re.sub(r'^%Warning-([^:]+): .*?:\d+:\d+: ', r'\1: ', line)
                       for line in path.read_text().splitlines() if line.startswith('%Warning-'))

    actual = warnings(d/'lint.log')
    assert actual == warnings(BUILD/'cp-key-native-lint-v1-20261024/lint.log')
    counts = Counter(line.split(':', 1)[0] for line in actual.elements())
    assert counts['UNOPTFLAT'] == 3 and counts['MULTIDRIVEN'] == 1
    component = BUILD/'primary-clock-component-stage-v1-20261024'
    p = read(component/'provenance.json')
    assert p['clock_period_ns'] == 15 and p['result_bits'] == 1560 and p['CPU_rotations'] == 8
    for file, digest in p['files'].items():
        assert sha(component/file) == digest
    retain(component, out/component.name)
    result = dict(status='REVIEWED_SELECTOR_INDEPENDENCE_AND_NATIVE_CONTEXT', records=records,
                  original_swapped_cofactor_negative_rejected=True,
                  binary_constant_projection_proven=True, compared_bits=482,
                  four_state_checks=5140, unknown_checks=5012, fallback_contract_checks=32,
                  full_original_comparisons_per_engine=14884, warning_counts=dict(counts),
                  native_fit_not_yet_started=True, primary_component_fit_requires_terminal_collection=True,
                  physical_timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
                  assembly_permitted=False, hardware_changed=False, hardware_requires_user_release=True,
                  full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    result = review(args.output.resolve())
    append_archive(args.archive.resolve(), args.output.resolve(), [], label='selector-projection-followup',
                   extra_tools=['preview_hold_selectors.py', 'test_hold_selector_preview.py',
                                'stage_hold_selector_preview.py', 'fit_primary_clock_events.py', Path(__file__).name])
    print(result['status'], flush=True)
