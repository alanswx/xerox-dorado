#!/usr/bin/env python3
"""Retain exact pixel-state reduction and all independent clock source gates.

Keep the first generic-branch elaboration failure and its successful fixed-ratio
checks. Only the corrected candidate has the full twelve-configuration gate.
No timing or native consumer acceptance is inferred from these source proofs.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from review_late_hold_and_input_reset import first_path
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def read(path):
    return json.loads(path.read_text())


def retain_text(source, destination):
    for path in source.rglob('*'):
        rel = path.relative_to(source)
        if not path.is_file() or any(p.startswith('obj') or p in ['db', 'incremental_db'] for p in rel.parts):
            continue
        if path.suffix in ['.vvp', '.o', '.a', '.rbf', '.sof', '.pack']:
            continue
        data = path.read_bytes()
        assert not data.startswith((b'\x7fELF', b'\xcf\xfa\xed\xfe')), path
        target = destination/rel
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(data)


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    records = []

    def checked(name, status, stage=False):
        d = BUILD/name
        state_name = 'results.json' if stage else 'run-state.json'
        s = read(d/state_name)
        assert s['status'] == status, name
        for file, digest in s.get('files', {}).items():
            assert sha(d/file) == digest, (name, file)
        for file, digest in s.get('inputs', {}).items():
            assert sha(d/'inputs'/file) == digest, (name, file)
        if 'proof_log_sha256' in s:
            assert sha(d/'proof.log') == s['proof_log_sha256']
        for c in s.get('cases', []):
            assert c.get('passed', True)
            if 'mode' in c:
                directory = d/(c['mode']+'-'+c['variant'])
                assert c['returncode'] == 0
                assert sha(directory/'proof.log') == c['log_sha256']
                for file, digest in c['files'].items():
                    assert sha(directory/file) == digest
            elif 'variant' in c:
                assert (c.get('returncode', c.get('actions', [{}])[-1].get('returncode')) != 0) == c['expected_rejection']
                if 'actions' in c:
                    assert c['actions'][0]['returncode'] == 0
        retain_text(d, out/name)
        records.append(dict(directory=name, status=status, state_sha256=sha(d/state_name)))
        return d, s

    for version in [1, 2]:
        suffix = f'v{version}-20261024'
        stage, s = checked('exact-pixel-phase-stage-'+suffix, 'PREPARED_EXACT_PIXEL_PHASE', True)
        assert s['specialized_samples'] == 4 and s['specialized_frequency_khz'] == 50000
        assert s['original_output_equations_and_level_update_preserved'] and not s['extra_virtual_cycles']
        assert sha(stage/'original.sv') == sha(BUILD/'primary-clock-component-stage-v1-20261024/dorado_oscillator_events.sv')
        _, p = checked('exact-pixel-phase-proof-'+suffix, 'PASS_EXACT_PIXEL_PHASE_INDUCTION')
        assert p['compared_bits'] == 136 and p['unbounded_induction_proven']
        assert not p['protocol_or_state_assumptions']
        _, n = checked('exact-pixel-phase-negative-'+suffix, 'PASS_PIXEL_PHASE_COUNTEREXAMPLE')
        assert n['stage_state_sha256'] == p['stage_state_sha256']
        batch, b = checked('exact-pixel-clock-batch-'+suffix, 'PASS_COMMON_CPU_PIXEL_EVENT_BATCH')
        assert len(b['cases']) == 12
        assert sha(batch/'inputs/dorado_oscillator_events.sv') == sha(stage/'dorado_oscillator_events.sv')
        proof, p = checked('exact-pixel-clock-batch-proof-'+suffix, 'PASS_COMMON_CLOCK_BATCH_INDUCTION')
        assert p['unbounded_induction_proven'] and not p['assumptions_added'] and p['cpu_rotations'] == 8
        assert p['tests_state_sha256'] == sha(batch/'run-state.json')
        assert sha(proof/'pixel-original.sv') == sha(stage/'dorado_oscillator_events.sv')
        _, n = checked('exact-pixel-clock-batch-negative-'+suffix, 'PASS_CLOCK_BATCH_COUNTEREXAMPLE')
        assert n['tests_state_sha256'] == p['tests_state_sha256']
        tests, t = checked('exact-pixel-primary-reference-'+suffix, 'PASS_ALL_PRIMARY_CLOCK_ORIGINAL_REFERENCES')
        assert len(t['cases']) == 12
        assert t['oscillator_source_sha256'] == sha(stage/'dorado_oscillator_events.sv')
        baseline = BUILD/'primary-clock-original-reference-v1-20261024/inputs'
        for name in ['tb.sv', 'expected.hex', 'dorado_clock_event_batch.sv']:
            assert sha(tests/'inputs'/name) == sha(baseline/name)
    failed, s = checked('exact-pixel-oscillator-v1-20261024', 'TERMINAL_OSCILLATOR_EVENT_TEST_FAILED')
    assert s['actions'][0]['returncode'] != 0
    assert read(failed/'classification.json')['status'] == 'REJECTED_GENERIC_BRANCH_ELABORATION'
    assert 'declaration after use' in (failed/'icarus-positive-compile.log').read_text()
    osc, s = checked('exact-pixel-oscillator-v2-20261024', 'PASS_ORDERED_OSCILLATOR_EVENTS_AND_NATIVE_RATE_COUNTEREXAMPLE')
    assert len(s['cases']) == 14
    assert all((c['returncode'] != 0) == c['expected_rejection'] for c in s['cases'])
    for engine in ['icarus', 'verilator']:
        assert 'OSC_EVENTS_ALL_PASS cases=12' in (osc/(engine+'-positive-test.log')).read_text()
    _, p = checked('exact-pixel-primary-formal-v2-20261024', 'PASS_PRIMARY_CLOCK_MERGE_AND_PHASE_PROOFS')
    assert len(p['cases']) == 7 and p['merged_output_bits'] == 195
    assert p['crystal_phase_unbounded_induction'] and not p['reset_advance_protocol_assumptions']
    fixture = BUILD/'exact-pixel-primary-component-stage-v2-20261024'
    f = read(fixture/'provenance.json')
    assert f['clock_period_ns'] == 15 and f['result_bits'] == 1560
    assert f['pixel_phase_qualification']['unbounded_CPU_pixel_and_full_oscillator_equivalence']
    for name, digest in f['files'].items():
        assert sha(fixture/name) == digest
    baseline = BUILD/'primary-clock-parallel-component-stage-v1-20261024'
    for name in ['top.sv', 'clock.sdc', 'clock_fit.qsf', 'audit.tcl', 'fit_control_clock.py', 'dorado_primary_clock_events.sv']:
        assert sha(fixture/name) == sha(baseline/name)
    retain_text(fixture, out/fixture.name)
    result = dict(status='REVIEWED_EXACT_PIXEL_PHASE_AND_ALL_SOURCE_GATES', records=records,
                  generic_elaboration_failure_preserved=True, oscillator_parameter_cases=12,
                  full_oscillator_induction_bits=136, CPU_pixel_rotations=8,
                  primary_merged_bits=195, primary_full_samples_per_positive=160576,
                  extra_virtual_cycles=0, frequencies_changed=False,
                  canonical_RTL_changed=False, native_consumers_integrated=False,
                  physical_timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
                  hardware_changed=False, hardware_requires_user_release=True, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


def review_terminal(out):
    assert not out.exists()
    out.mkdir(parents=True)
    name = 'exact-pixel-primary-component-terminal-v2-20261024'
    d = BUILD/name
    receipt = read(BUILD/(name+'.tgz.json'))
    assert sha(BUILD/(name+'.tgz')) == receipt['archive_sha256'] and len(receipt['members']) == 58
    for row in receipt['members']:
        assert sha(d/row['path']) == row['sha256'] and (d/row['path']).stat().st_size == row['bytes']
    state = read(d/'run-state.json')
    assert state['status'] == 'TERMINAL_REVIEW_REQUIRED' and len(state['stages']) == 4
    assert all(a['returncode'] == 0 for a in state['stages'])
    readback = read(d/'collected-input-check.json')
    assert readback['job_pids_absent'] == [state['builder_pid']]+[a['pid'] for a in state['stages']]
    assert readback['changed_inputs'] == ['clock_fit.qsf']
    p = read(d/'provenance.json')
    for file, digest in p['files'].items():
        raw = (d/file).read_bytes()
        assert sha(d/file) == readback['actual'][file]
        if file == 'clock_fit.qsf':
            annotation = b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
            assert raw.endswith(annotation) and raw.count(annotation) == 1
            raw = raw[:-len(annotation)]
        assert hashlib.sha256(raw).hexdigest() == digest
    corners = []
    for corner in ['7_slow_1100mv_100c', '7_slow_1100mv_-40c', 'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c']:
        row = dict(condition=corner)
        for kind in ['setup', 'hold', 'evaluator']:
            row[kind] = first_path(d/f'clock-{kind}-{corner}.rpt')
        text = (d/f'clock-ucp-{corner}.rpt').read_text()
        counts = re.findall(r'; Unconstrained (?:Clocks|Input Ports|Input Port Paths|Output Ports|Output Port Paths)\s*; (\d+)\s*; (\d+)', text)
        assert len(counts) == 5 and all(pair == ('0', '0') for pair in counts)
        corners.append(row)
    assert [r['setup']['slack_ns'] for r in corners] == [-0.273, -0.194, 5.168, 5.316]
    assert min(r['hold']['slack_ns'] for r in corners) == 0.170
    assert 'bb_phase[4]' in corners[0]['setup']['source']
    assert corners[0]['setup']['sink'] == 'result[34]~reg0'
    fit = (d/'clock_fit.fit.summary').read_text()
    assert '3,210 / 41,910' in fit and 'Total registers : 1407' in fit and 'Total DSP Blocks : 0 /' in fit
    assert re.findall(r'PRIMARY_CLOCK_OUTPUT_BITS (\d+)', (d/'corners.log').read_text()) == ['1560']*4
    retain_text(d, out/name)
    (out/(name+'.tgz.json')).write_bytes((BUILD/(name+'.tgz.json')).read_bytes())
    result = dict(status='REVIEWED_EXACT_PIXEL_COMPONENT_TIMING_REJECTION', corners=corners,
                  ALMs=3210, registers=1407, DSPs=0, period_ns=15, data_delay_ns=14.627,
                  clock_skew_ns=-0.476, logic_levels=13,
                  remaining_path='BB phase -> due/time -> rank -> Ether insertion -> joined mask -> level cursor',
                  physical_timing_qualified=False, canonical_RTL_changed=False,
                  native_consumers_integrated=False, firmware_qualified=False, speedup_qualified=False,
                  hardware_changed=False, hardware_requires_user_release=True, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


def review_native(out):
    assert not out.exists()
    out.mkdir(parents=True)
    d = BUILD/'selector-native-terminal-v2-20261024'
    receipt_path = BUILD/(d.name+'.tgz.json')
    receipt = read(receipt_path)
    assert sha(BUILD/(d.name+'.tgz')) == receipt['archive_sha256'] and len(receipt['members']) == 961
    for row in receipt['members']:
        assert sha(d/row['path']) == row['sha256'] and (d/row['path']).stat().st_size == row['bytes']
    states = [read(d/'run-state.json'), read(d/'physical-report/run-state.json'), read(d/'cp-key-report/run-state.json')]
    assert [s['status'] for s in states] == ['TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED',
        'TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED', 'TERMINAL_CP_KEY_TOPOLOGY_REVIEW_REQUIRED']
    assert all(a['returncode'] == 0 for s in states for a in s.get('stages', s.get('actions', [])))
    m = read(d/'r-front-control-native.json')
    r = read(d/'physical-native-input-readback.json')
    assert r['actual'] == m['source_hashes'] and len(r['actual']) == 533
    for name, digest in m['source_hashes'].items():
        assert sha(d/name) == digest
    assert m['frequency_hz'] == 57142857 and m['sysper'] == 4 and not m['assembly_permitted']
    keys = read(d/'cp-key-report/results.json')
    assert keys['physical_key_keeper_count'] == 243 and len(keys['distinct_logical_key_bits']) == 239
    assert not keys['critical_unused_key138_present'] and keys['out_of_support_bits'] == []
    log = (d/'physical-report/audit.log').read_text()
    coverage = re.findall(r'PHYSICAL_NATIVE_COVERAGE (\S+) keepers=(\d+) central=(\d+) primary=(\d+) duplicates=(\d+) history=(\d+)', log)
    assert len(coverage) == 4 and all(r[1:] == ('84837', '801', '695', '106', '153') for r in coverage)
    corners = []
    for corner in ['7_slow_1100mv_100c', '7_slow_1100mv_-40c', 'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c']:
        row = dict(condition=corner)
        for kind, prefix in [('setup', 'native59-setup'), ('hold', 'native59-hold'),
                             ('input', 'physical-native-input-setup'), ('internal', 'physical-native-internal-setup')]:
            row[kind] = first_path(d/'core'/(prefix+'-'+corner+'.rpt'))
        visible = re.findall(r'^PHYSICAL_NATIVE_VISIBLE_R '+re.escape(corner)+r' (.+)$', log, re.M)
        assert len({re.sub(r'~DUPLICATE.*$', '', name) for name in visible}) == 16
        assert len(re.findall(r'^PHYSICAL_NATIVE_RAM '+re.escape(corner)+r' (\w+) keepers=', log, re.M)) == 4
        corners.append(row)
    assert [r['setup']['slack_ns'] for r in corners] == [0.067, 0.197, 4.066, 4.781]
    assert [r['hold']['slack_ns'] for r in corners] == [-0.064, -0.041, 0.034, 0.011]
    assert [r['input']['slack_ns'] for r in corners] == [0.368, 0.337, 4.602, 5.329]
    assert [r['internal']['slack_ns'] for r in corners] == [0.067, 0.299, 7.759, 9.345]
    assert 'generation[19]' in corners[0]['hold']['source'] and 'flushed_generation[19]' in corners[0]['hold']['sink']
    fit = (d/'core/output_files/Dorado.fit.summary').read_text()
    assert '36,213 / 41,910' in fit and 'Total registers : 55368' in fit
    retain_text(d, out/d.name)
    (out/receipt_path.name).write_bytes(receipt_path.read_bytes())
    result = dict(status='REVIEWED_NATIVE_SELECTOR_SETUP_GAIN_AND_HOLD_REJECTION',
                  raw_members=961, compiled_source_hashes_verified=533, ALMs=36213, registers=55368,
                  corners=corners, coverage=coverage, clock_period_ns=17.5,
                  physical_key_keepers=243, logical_key_bits=239, unused_key138_removed=True,
                  failing_hold='disk generation[19] -> flushed_generation[19]',
                  hold_data_delay_ns=0.629, hold_clock_skew_ns=0.693,
                  physical_timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
                  assembly_permitted=False, hardware_changed=False, hardware_requires_user_release=True,
                  original60ns_qualified=False, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--terminal-fit', action='store_true')
    parser.add_argument('--native-fit', action='store_true')
    args = parser.parse_args()
    assert not (args.terminal_fit and args.native_fit)
    result = review_native(args.output.resolve()) if args.native_fit else review_terminal(args.output.resolve()) if args.terminal_fit else review(args.output.resolve())
    label = 'native-selector-terminal' if args.native_fit else 'exact-pixel-phase-terminal' if args.terminal_fit else 'exact-pixel-phase-followup'
    append_archive(args.archive.resolve(), args.output.resolve(), [], label=label,
                   extra_tools=['optimize_pixel_phase.py', 'test_oscillator_events.py', 'test_clock_event_batch.py',
                                'test_primary_clock_events.py', 'fit_primary_clock_events.py', Path(__file__).name])
    print(result['status'], flush=True)
