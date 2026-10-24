#!/usr/bin/env python3
"""Retain first clock-consumer correctness and complete native hold recovery.

Keep every failed oracle/fixture attempt and the original corner filename
collision. Consumer correctness is digital package behavior, not analog
timing or whole BaseBoard/6502 qualification. The consumer misses15ns; the
unchanged native design passes its existing17.5ns timing constraints only.
"""
import argparse
import json
from pathlib import Path
import re

from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import first_path
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'
CORNERS = ['7_slow_1100mv_100c', '7_slow_1100mv_-40c', 'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c']


def read(path):
    return json.loads(path.read_text())


def verify(directory, files):
    for name, digest in files.items():
        assert sha(directory/name) == digest, (directory, name)


def collection(name):
    d = BUILD/name
    receipt_path = BUILD/(name+'.tgz.json')
    r = read(receipt_path)
    assert sha(BUILD/(name+'.tgz')) == r['archive_sha256']
    for row in r['members']:
        assert sha(d/row['path']) == row['sha256'] and (d/row['path']).stat().st_size == row['bytes']
    return d, receipt_path, r


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    dirs = []
    for version in [1, 2, 3, 4]:
        d = BUILD/f'baseboard-clock-consumer-v{version}-20261024'
        s = read(d/'run-state.json')
        verify(d/'inputs', s['inputs'])
        if version < 4:
            assert s['status'] == 'TERMINAL_BASEBOARD_CONSUMER_NOT_ESTABLISHED'
            c = s['cases'][0]
            assert c['actions'][0]['returncode'] == 0 and c['actions'][-1]['returncode'] != 0
            assert 'edge mismatch phase=5 trial=0 tick=1 step=4 edge=3' in (d/'icarus-positive/test.log').read_text()
        else:
            assert s['status'] == 'PASS_BASEBOARD_CLOCK_EVENT_CONSUMER_REFERENCES' and len(s['cases']) == 12
            assert s['physical_state_bits'] == 6 and s['initial_states'] == 64 and s['CPU_phases'] == 8
            for c in s['cases']:
                assert c['passed'] and c['actions'][0]['returncode'] == 0
                assert (c['actions'][-1]['returncode'] != 0) == c['expected_rejection']
                p = d/(c['engine']+'-'+c['variant'])/'test.log'
                assert sha(p) == c['test_log_sha256']
                if c['variant'] == 'positive':
                    assert 'samples=176384 steps=873792 holds=12648 restores=520' in p.read_text()
            for name in ['dorado_baseboard_clock_step.sv', 'dorado_primary_clock_events.sv', 'dorado_oscillator_events.sv']:
                assert sha(ROOT/'verilog/rtl'/name) == s['inputs'][name]
        dirs.append(d)
    for version in [1, 3]:
        d = BUILD/f'baseboard-clock-consumer-delta-trace-v{version}-20261024'
        s = read(d/'results.json')
        verify(d, s['files'])
        assert s['original_checks_and_reference_packages_unchanged'] and s['test_returncode'] != 0
        dirs.append(d)
    for version in [1, 2]:
        d = BUILD/f'baseboard-clock-consumer-component-stage-v{version}-20261024'
        verify(d, read(d/'provenance.json')['files'])
        dirs.append(d)
    failed = BUILD/'baseboard-clock-consumer-component-v1-20261024'
    f = read(failed/'run-state.json')
    assert f['status'] == 'TERMINAL_FAILED' and f['stages'][0]['returncode'] == 3
    assert 'near text: "for"' in (failed/'map.log').read_text()
    dirs.append(failed)
    fit, receipt_path, r = collection('baseboard-clock-consumer-component-terminal-v2-20261024')
    assert len(r['members']) == 59
    s = read(fit/'run-state.json')
    assert s['status'] == 'TERMINAL_REVIEW_REQUIRED' and all(x['returncode'] == 0 for x in s['stages'])
    p = read(fit/'provenance.json')
    assert p['result_bits'] == 2568 and p['producer_bits_per_rotation'] == 195 and p['consumer_bits_per_rotation'] == 126
    consumer_corners = []
    for corner in CORNERS:
        row = dict(condition=corner, setup=first_path(fit/f'clock-setup-{corner}.rpt'), hold=first_path(fit/f'clock-hold-{corner}.rpt'))
        consumer_corners.append(row)
    assert [r['setup']['slack_ns'] for r in consumer_corners] == [-2.473, -2.542, 5.614, 5.725]
    assert min(r['hold']['slack_ns'] for r in consumer_corners) == 0.165
    assert 'ether_phase[4]' in consumer_corners[1]['setup']['source'] and consumer_corners[1]['setup']['sink'] == 'result[968]~reg0'
    assert re.findall(r'BB_CONSUMER_OUTPUT_BITS (\d+)', (fit/'corners.log').read_text()) == ['2568']*4
    dirs.append(fit)
    (out/receipt_path.name).write_bytes(receipt_path.read_bytes())
    proof = BUILD/'primary-bb-edge-contract-v1-20261024'
    s = read(proof/'run-state.json')
    assert s['status'] == 'PASS_PRIMARY_CLOCK_MERGE_AND_PHASE_PROOFS'
    assert s['at_most_one_BB_transition_per_interval_proven'] and s['independent_BB_and_Ether_endpoints_proven']
    assert not s['reset_advance_protocol_assumptions'] and len(s['cases']) == 7
    for c in s['cases']:
        assert c['passed'] and c['returncode'] == 0
        d = proof/(c['mode']+'-'+c['variant'])
        verify(d, c['files'])
        assert sha(d/'proof.log') == c['log_sha256']
    dirs.append(proof)
    native, receipt_path, raw = collection('pack-hold-native-corrected-terminal-v2-20261024')
    assert len(raw['members']) == 1084
    m = read(native/'r-front-control-native.json')
    verify(native, m['source_hashes'])
    assert len(m['source_hashes']) == 534
    assert read(native/'physical-native-input-readback-v2.json')['actual'] == m['source_hashes']
    assert read(native/'corner-report-collision.json')['expected_corner_files'] == 4
    states = ['run-state.json', 'physical-report/run-state.json', 'cp-key-report/run-state.json',
              'corrected-corners-report/run-state.json', 'corrected-crossings-report/run-state.json']
    for file in states:
        s = read(native/file)
        assert s['status'].startswith('TERMINAL')
        assert all(x['returncode'] == 0 for x in s.get('stages', s.get('actions', [])))
    native_corners = []
    for corner in CORNERS:
        row = dict(condition=corner)
        for name, prefix in [('setup', 'native59-setup'), ('hold', 'native59-hold'),
                             ('input', 'physical-native-input-setup'), ('internal', 'physical-native-internal-setup')]:
            row[name] = first_path(native/'core'/(prefix+'-'+corner+'.rpt'))
            assert row[name]['slack_ns'] > 0
        row['pack_hold'] = first_path(native/'corrected-corners-report'/('pack-hold-'+corner+'.rpt'))
        assert row['pack_hold']['slack_ns'] > 0
        native_corners.append(row)
    assert min(r['setup']['slack_ns'] for r in native_corners) == 0.201
    assert min(r['hold']['slack_ns'] for r in native_corners) == 0.116
    corrected = read(native/'corrected-crossings-report/run-state.json')
    assert len(corrected['actions']) == 5 and all(x['inverse_byte_identical'] for x in corrected['bindings'])
    for bind in corrected['bindings']:
        assert sha(native/'core'/bind['name']) == bind['original_sha256']
        assert sha(native/'corrected-crossings-report'/bind['name']) == bind['changed_sha256']
    dirs.append(native)
    (out/receipt_path.name).write_bytes(receipt_path.read_bytes())
    calibration = BUILD/'pack-hold-calibration-v3-20261024'
    c = read(calibration/'run-state.json')
    assert c['status'] == 'PASS_STRONGER_PACK_HOLD_CONTRACT_CALIBRATION' and c['constraint_namespace_isolated']
    verify(calibration, c['files'])
    dirs.append(calibration)
    stage = BUILD/'pack-hold-native-stage-v2-20261024'
    verify(stage, read(stage/'r-front-control-native.json')['source_hashes'])
    dirs.append(stage)
    for d in dirs:
        retain_text(d, out/d.name)
    result = dict(status='REVIEWED_FIRST_BOARD_CONSUMER_AND_RECOVERED_NATIVE_HOLD_TIMING',
        consumer_samples_per_engine=176384, consumer_steps_per_engine=873792, physical_state_bits=6,
        consumer_initial_states=64, CPU_phases=8, negative_controls_per_engine=5,
        reference_delta_failures_preserved=True, original_three_input_gate_truth_preserved=True,
        analog_runt_or_setup_hold_qualified=False, native_6502_RIOT_consumers_integrated=False,
        consumer_component_corners=consumer_corners, consumer_15ns_timing_qualified=False,
        single_BB_transition_and_independent_endpoint_contract_proven=True,
        native_existing_17_5ns_constraint_corners=native_corners,
        original_report_collision_preserved=True, all_original_report_queries_recovered=True,
        namespace_isolated_future_stage_prepared=True, namespace_isolated_native_refit_started=False,
        native_original_60ns_or_15ns_qualified=False, firmware_qualified=False, speedup_qualified=False,
        hardware_changed=False, assembly_permitted=False, hardware_requires_user_release=True, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    r = review(a.output.resolve())
    append_archive(a.archive.resolve(), a.output.resolve(), [], label='baseboard-consumer-followup', extra_tools=[
        'test_baseboard_clock_consumer.py', 'fit_baseboard_clock_consumer.py', 'report_pack_hold_corners.py',
        'report_pack_hold_crossings.py', 'qualify_pack_hold_margin.py', 'stage_pack_hold_margin.py',
        'collect_r_front_physical_native.py', 'prove_primary_clock_events.py', Path(__file__).name])
    print(r['status'], flush=True)
