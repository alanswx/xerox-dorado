#!/usr/bin/env python3
"""Retain passing15ns primary-source timing and calibrated native hold work.

The selected source modules remain unconnected to native board consumers.
Retain the first uncertainty-calibration failure and all32-bit corrected
checks. The stricter native fit is live, not a timing/firmware/speed pass.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import first_path
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def read(path):
    return json.loads(path.read_text())


def verify(directory, files):
    for name, digest in files.items():
        assert sha(directory/name) == digest, (directory, name)


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    dirs = []
    stage = BUILD/'exact-bb-phase-stage-v1-20261024'
    s = read(stage/'results.json')
    verify(stage, s['files'])
    assert s['merge_body_byte_unchanged'] and s['phase_states'] == 10 and not s['extra_virtual_cycles']
    dirs.append(stage)
    tests = BUILD/'exact-bb-primary-reference-v1-20261024'
    t = read(tests/'run-state.json')
    assert t['status'] == 'PASS_ALL_PRIMARY_CLOCK_ORIGINAL_REFERENCES' and len(t['cases']) == 12
    verify(tests/'inputs', t['inputs'])
    assert all(c['passed'] and c['actions'][0]['returncode'] == 0 for c in t['cases'])
    for name in ['tb.sv', 'expected.hex', 'dorado_clock_event_batch.sv']:
        assert sha(tests/'inputs'/name) == sha(BUILD/'primary-clock-original-reference-v1-20261024/inputs'/name)
    dirs.append(tests)
    proofs = BUILD/'exact-bb-primary-formal-v1-20261024'
    p = read(proofs/'run-state.json')
    assert p['status'] == 'PASS_PRIMARY_CLOCK_MERGE_AND_PHASE_PROOFS' and len(p['cases']) == 7
    assert p['tests_state_sha256'] == sha(tests/'run-state.json')
    assert p['crystal_phase_unbounded_induction'] and not p['reset_advance_protocol_assumptions']
    for c in p['cases']:
        d = proofs/(c['mode']+'-'+c['variant'])
        assert c['passed'] and c['returncode'] == 0
        verify(d, c['files'])
        assert sha(d/'proof.log') == c['log_sha256']
    assert 'assign equivalent=bb_phase_tenths<10 &&' in (proofs/'phase-positive/actual.sv').read_text()
    dirs.append(proofs)
    fixture = BUILD/'exact-bb-primary-component-stage-v1-20261024'
    f = read(fixture/'provenance.json')
    verify(fixture, f['files'])
    assert f['tests_state_sha256'] == sha(tests/'run-state.json') and f['proofs_state_sha256'] == sha(proofs/'run-state.json')
    dirs.append(fixture)
    fitted = BUILD/'exact-bb-primary-component-terminal-v1-20261024'
    receipt = read(BUILD/(fitted.name+'.tgz.json'))
    assert sha(BUILD/(fitted.name+'.tgz')) == receipt['archive_sha256'] and len(receipt['members']) == 58
    verify(fitted, {r['path']: r['sha256'] for r in receipt['members']})
    s = read(fitted/'run-state.json')
    assert s['status'] == 'TERMINAL_REVIEW_REQUIRED' and all(a['returncode'] == 0 for a in s['stages'])
    r = read(fitted/'collected-input-check.json')
    assert r['job_pids_absent'] == [s['builder_pid']]+[a['pid'] for a in s['stages']]
    for name, digest in f['files'].items():
        raw = (fitted/name).read_bytes()
        assert sha(fitted/name) == r['actual'][name]
        if name == 'clock_fit.qsf':
            annotation = b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
            assert raw.endswith(annotation) and raw.count(annotation) == 1
            raw = raw[:-len(annotation)]
        assert hashlib.sha256(raw).hexdigest() == digest
    corners = []
    for corner in ['7_slow_1100mv_100c', '7_slow_1100mv_-40c', 'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c']:
        row = dict(condition=corner)
        for kind in ['setup', 'hold', 'evaluator']:
            row[kind] = first_path(fitted/f'clock-{kind}-{corner}.rpt')
        assert row['setup']['slack_ns'] > 0 and row['hold']['slack_ns'] > 0
        ucp = (fitted/f'clock-ucp-{corner}.rpt').read_text()
        assert re.findall(r'; Unconstrained (?:Clocks|Input Ports|Input Port Paths|Output Ports|Output Port Paths)\s*; (\d+)\s*; (\d+)', ucp) == [('0', '0')]*5
        corners.append(row)
    assert min(c['setup']['slack_ns'] for c in corners) == 1.064 and min(c['hold']['slack_ns'] for c in corners) == 0.189
    assert re.findall(r'PRIMARY_CLOCK_OUTPUT_BITS (\d+)', (fitted/'corners.log').read_text()) == ['1560']*4
    dirs.append(fitted)
    for name in ['dorado_primary_clock_events.sv', 'dorado_oscillator_events.sv']:
        assert sha(ROOT/'verilog/rtl'/name) == sha(fitted/name) == t['inputs'][name]
    for version in [1, 2]:
        d = BUILD/f'pack-hold-calibration-v{version}-20261024'
        c = read(d/'run-state.json')
        if version == 1:
            assert c['status'] == 'TERMINAL_HOLD_CALIBRATION_NOT_ESTABLISHED' and c['recorded_handles_absent']
            assert (d/'run-state-before-classification.json').exists()
        else:
            assert c['status'] == 'PASS_STRONGER_PACK_HOLD_CONTRACT_CALIBRATION' and c['returncode'] == 0
            verify(d, c['files'])
            assert len(c['corners']) == 4
            assert all(r['setup_all32_paths_verified'] and r['hold_all32_paths_verified'] and r['derived_uncertainty_increment_ns'] == 0.120 for r in c['corners'])
        dirs.append(d)
    native = BUILD/'pack-hold-native-stage-v1-20261024'
    n = read(native/'r-front-control-native.json')
    assert len(n['source_hashes']) == 534 and n['pack_hold_margin']['RTL_byte_unchanged']
    verify(native, n['source_hashes'])
    dirs.append(native)
    selection = BUILD/'clock-source-selection-v1-20261024'
    job = read(selection/'native-job.json')
    assert job['state']['status'] == 'RUNNING' and all(h['live'] for h in job['handles'])
    assert job['compiled_input_files_verified'] == 534 and job['manifest_sha256'] == sha(native/'r-front-control-native.json')
    assert all(r['live'] for r in job['reporters'])
    dirs.append(selection)
    for d in dirs:
        retain_text(d, out/d.name)
    (out/(fitted.name+'.tgz.json')).write_bytes((BUILD/(fitted.name+'.tgz.json')).read_bytes())
    result = dict(status='REVIEWED_SELECTED_15NS_PRIMARY_SOURCES_AND_STRICTER_HOLD_WORK',
                  component_corners=corners, component_timing_qualified=True, source_modules_selected=True,
                  ALMs=2808, registers=1391, DSPs=0, all_primary_output_bits=1560,
                  calibration_verified_bits=32, calibration_verified_corners=4, stricter_hold_ns=0.370,
                  native_job=job, native_consumers_integrated=False, native_timing_qualified=False,
                  firmware_qualified=False, speedup_qualified=False, hardware_changed=False,
                  hardware_requires_user_release=True, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        p.add_argument('--'+name, type=Path, required=True)
    args = p.parse_args()
    result = review(args.output.resolve())
    append_archive(args.archive.resolve(), args.output.resolve(), [], label='bb-phase-hold-followup',
                   extra_tools=['optimize_baseboard_phase.py', 'prove_primary_clock_events.py',
                                'qualify_pack_hold_margin.py', 'stage_pack_hold_margin.py', Path(__file__).name])
    print(result['status'], flush=True)
