#!/usr/bin/env python3
"""Preserve native key-pruning timing and all-primary clock source qualification.

The key reduction is real in the fitted netlist but full native timing still
fails. The complete CPU/pixel/BaseBoard/Ethernet producer is source-qualified
and UNCONNECTED. Preserve the full objective and the user's FPGA reservation.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from prove_primary_clock_events import extract
from review_late_hold_and_input_reset import retain, first_path
from review_memc_lcell_buffers import envelope
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def read(p):
    return json.loads(p.read_text())


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    fitted = BUILD/'cp-key-fit-terminal-v1-20261024'
    receipt = BUILD/'cp-key-native-v1-20261024-terminal-v1.tgz.json'
    collected = envelope(fitted, receipt)
    assert len(collected['members']) == 963
    build = read(fitted/'run-state.json')
    physical = read(fitted/'physical-report/run-state.json')
    keys = read(fitted/'cp-key-report/run-state.json')
    assert build['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert physical['status'] == 'TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
    assert keys['status'] == 'TERMINAL_CP_KEY_TOPOLOGY_REVIEW_REQUIRED'
    assert all(a['returncode'] == 0 for a in [*build['stages'], *physical['actions'], *keys['actions']])
    m = read(fitted/'r-front-control-native.json')
    readback = read(fitted/'physical-native-input-readback.json')
    assert readback['actual'] == m['source_hashes'] and len(readback['actual']) == 533
    for name, digest in m['source_hashes'].items():
        assert sha(fitted/name) == digest
    assert sha(fitted/'cp-key-report/audit.log') == keys['log_sha256']
    assert sha(fitted/'cp-key-report/audit_cp_cache_key_native.tcl') == keys['script_sha256']
    topology = read(fitted/'cp-key-report/results.json')
    assert topology['physical_key_keeper_count'] == 241 and len(topology['distinct_logical_key_bits']) == 239
    assert not topology['critical_unused_key138_present'] and topology['out_of_support_bits'] == []
    assert set(topology['distinct_logical_key_bits']) <= set(m['cp_cache_key_projection']['cared_key_bits'])
    log = (fitted/'physical-report/audit.log').read_text()
    coverage = re.findall(r'PHYSICAL_NATIVE_COVERAGE (\S+) keepers=(\d+) central=(\d+) primary=(\d+) duplicates=(\d+) history=(\d+)', log)
    assert len(coverage) == 4 and all(r[1:] == ('84713', '804', '695', '109', '163') for r in coverage)
    corners = []
    for corner in ['7_slow_1100mv_100c', '7_slow_1100mv_-40c', 'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c']:
        row = dict(condition=corner)
        for label, prefix in [('setup', 'native59-setup'), ('hold', 'native59-hold'),
                              ('input', 'physical-native-input-setup'), ('internal', 'physical-native-internal-setup')]:
            row[label] = first_path(fitted/'core'/(prefix+'-'+corner+'.rpt'))
        visible = re.findall(r'^PHYSICAL_NATIVE_VISIBLE_R '+re.escape(corner)+r' (.+)$', log, re.M)
        assert len({re.sub(r'~DUPLICATE.*$', '', name) for name in visible}) == 16
        assert len(re.findall(r'^PHYSICAL_NATIVE_RAM '+re.escape(corner)+r' (\w+) keepers=', log, re.M)) == 4
        corners.append(row)
    assert [r['setup']['slack_ns'] for r in corners] == [-0.445, 0.054, 3.957, 4.772]
    assert [r['hold']['slack_ns'] for r in corners] == [0.247, 0.219, 0.131, 0.116]
    assert [r['input']['slack_ns'] for r in corners] == [-0.059, 0.054, 4.322, 5.103]
    assert [r['internal']['slack_ns'] for r in corners] == [0.234, 0.351, 8.250, 9.566]
    assert 'u_d15|qa' in corners[0]['setup']['source'] and 'rm_l_rtl_0' in corners[0]['setup']['sink']
    path = (fitted/'core/native59-setup-7_slow_1100mv_100c.rpt').read_text().split('Path #1:', 1)[1].split('Path #2:', 1)[0]
    for value in ['16.809', '-0.976', 'state_next[121]~90', 'after_child[10]~1', 'root_arrival|state_next[53]~52']:
        assert value in path
    summary = (fitted/'core/output_files/Dorado.fit.summary').read_text()
    assert '36,263 / 41,910 ( 87 % )' in summary and 'Total registers : 55244' in summary
    assert 'analyzing 1 combinational loops as latches' in (fitted/'sta.log').read_text()
    retain(fitted, out/fitted.name)
    shutil.copy2(receipt, out/receipt.name)

    audit = BUILD/'native-primary-clock-audit-v2-20261024'
    a = read(audit/'results.json')
    assert a['status'] == 'PASS_SOURCE_BOUND_ALL_PRIMARY_CLOCK_SCHEDULE_AUDIT'
    assert a['compared_intervals'] == 160000 and a['required_record_capacity'] == 5
    assert a['exact_units_per_ns'] == 9412 and a['joint_period_ns'] == 300000
    assert a['stage_manifest_sha256'] == sha(fitted/'r-front-control-native.json')
    for name, digest in a['source_hashes'].items():
        assert sha(audit/name) == digest
    assert any(r['first_four_record_overflow'] is not None for r in a['rotations'])
    assert any(r['first_four_record_overflow'] is None for r in a['rotations'])
    retain(audit, out/audit.name)
    failed = BUILD/'native-primary-clock-audit-v1-20261024'
    assert read(failed/'failure.json')['status'] == 'REJECTED_AUDIT_REQUIRED_OVERFLOW_IN_EVERY_CPU_ROTATION'
    retain(failed, out/('rejected-'+failed.name))
    tests = BUILD/'primary-clock-original-reference-v1-20261024'
    t = read(tests/'run-state.json')
    assert t['status'] == 'PASS_ALL_PRIMARY_CLOCK_ORIGINAL_REFERENCES' and len(t['cases']) == 12
    for name, digest in t['inputs'].items():
        assert sha(tests/'inputs'/name) == digest
    for case in t['cases']:
        assert case['passed'] and case['actions'][0]['returncode'] == 0
        assert (case['actions'][-1]['returncode'] != 0) == case['expected_rejection']
        d = tests/(case['engine']+'-'+case['variant'])
        assert sha(d/'test.log') == case['log_sha256']
        if case['variant'] == 'positive':
            assert 'samples=160576 joint_period_ns=300000 maximum=5 resets=4' in (d/'test.log').read_text()
    for name in ['dorado_primary_clock_events.sv', 'dorado_clock_event_batch.sv', 'dorado_oscillator_events.sv']:
        assert sha(ROOT/'verilog/rtl'/name) == t['inputs'][name]
    retain(tests, out/tests.name)
    proofs = BUILD/'primary-clock-formal-v1-20261024'
    p = read(proofs/'run-state.json')
    assert p['status'] == 'PASS_PRIMARY_CLOCK_MERGE_AND_PHASE_PROOFS' and len(p['cases']) == 7
    assert p['tests_state_sha256'] == sha(tests/'run-state.json') and p['crystal_phase_unbounded_induction']
    for case in p['cases']:
        d = proofs/(case['mode']+'-'+case['variant'])
        assert case['passed'] and case['returncode'] == 0
        for name, digest in case['files'].items():
            assert sha(d/name) == digest
        assert sha(d/'proof.log') == case['log_sha256']
        raw = (d/'source.sv').read_text()
        source_name = 'dorado_primary_clock_events.sv' if case['variant'] == 'positive' else case['variant']+'.sv'
        assert sha(d/'source.sv') == t['inputs'][source_name]
        assert "localparam [17:0] INTERVAL=18'd141180;" in raw and "localparam [17:0] UNUSED=18'h3ffff;" in raw
        actual, reference = extract(raw, case['mode'])
        assert (d/'actual.sv').read_text() == actual
        if reference:
            assert (d/'reference.sv').read_text() == reference
    retain(proofs, out/proofs.name)
    original = BUILD/'clock-batch-capacity-positive-proof-v2-20261024'
    old = read(original/'run-state.json')
    assert old['status'] == 'PASS_COMMON_CLOCK_BATCH_INDUCTION' and old['unbounded_induction_proven']
    assert sha(original/'original.sv') == t['inputs']['dorado_clock_event_batch.sv']
    for name, digest in old['files'].items():
        assert sha(original/name) == digest
    retain(original, out/original.name)
    result = dict(status='REVIEWED_NATIVE_KEY_REMOVAL_AND_ALL_PRIMARY_CLOCK_SOURCES',
        native_fit=dict(raw_members=963, source_hashes_verified=533, corners=corners, keeper_coverage=coverage,
            ALMs=36263, registers=55244, physical_key_keepers_before=389, physical_key_keepers_after=241,
            logical_key_bits_before=385, logical_key_bits_after=239, unused_key138_removed=True,
            retained_keys_outside_support=[], timing_accepted=False,
            remaining_path='MemC d15.qa -> cache tags/Hold -> control state121 -> after_child10 -> T path -> low RM write',
            data_delay_ns=16.809, clock_skew_ns=-0.976),
        primary_source_audit=a, simulator_cases=t['cases'], formal_cases=p['cases'],
        native_event_consumers_integrated=False, original60ns_qualified=False,
        user_using_FPGA=True, hardware_test_requires_user_release=True,
        physical_timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
        assembly_permitted=False, hardware_changed=False, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    result = review(a.output.resolve())
    append_archive(a.archive.resolve(), a.output.resolve(), [], label='primary-clocks-followup', extra_tools=[
        'audit_native_primary_clocks.py', 'test_primary_clock_events.py', 'prove_primary_clock_events.py',
        'report_cp_cache_key_native.py', Path(__file__).name])
    print(result['status'], flush=True)
