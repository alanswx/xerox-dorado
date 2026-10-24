#!/usr/bin/env python3
"""Retain parallel primary-event qualification and exact live native work.

All primary sources and source phase state remain unchanged. Check the full
absolute-time reference, independent merger theorem, phase induction and all
negative controls. Preserve the initial unsupported-ROM proof, source-bound
15ns fixture, actual synthesis logs and timestamped verified live jobs.
No component or native timing result is inferred from area or live processes.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re

from prove_primary_clock_events import extract
from review_late_hold_and_input_reset import retain, first_path
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def read(path):
    return json.loads(path.read_text())


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    directories = []
    stage = BUILD/'primary-parallel-merge-stage-v1-20261024'
    s = read(stage/'results.json')
    for name, digest in s['files'].items():
        assert sha(stage/name) == digest
    assert s['state_and_source_clocks_byte_unchanged'] and not s['extra_saved_state_or_virtual_cycles']
    source = stage/'dorado_primary_clock_events.sv'
    original = (stage/'original.sv').read_text()
    actual = source.read_text()
    assert sha(stage/'original.sv') == sha(ROOT/'verilog/rtl/dorado_primary_clock_events.sv')
    assert extract(original, 'phase') == extract(actual, 'phase')
    directories.append(stage)
    tests = BUILD/'primary-parallel-original-reference-v1-20261024'
    t = read(tests/'run-state.json')
    assert t['status'] == 'PASS_ALL_PRIMARY_CLOCK_ORIGINAL_REFERENCES' and len(t['cases']) == 12
    assert t['candidate_source_sha256'] == sha(source)
    for name, digest in t['inputs'].items():
        assert sha(tests/'inputs'/name) == digest
    parent = BUILD/'primary-clock-original-reference-v1-20261024'
    for name in ['tb.sv', 'expected.hex', 'dorado_oscillator_events.sv', 'dorado_clock_event_batch.sv']:
        assert sha(tests/'inputs'/name) == sha(parent/'inputs'/name)
    for c in t['cases']:
        assert c['passed'] and c['actions'][0]['returncode'] == 0
        assert (c['actions'][-1]['returncode'] != 0) == c['expected_rejection']
        d = tests/(c['engine']+'-'+c['variant'])
        assert sha(d/'test.log') == c['log_sha256']
        if c['variant'] == 'positive':
            assert 'samples=160576 joint_period_ns=300000 maximum=5 resets=4' in (d/'test.log').read_text()
    directories.append(tests)
    failed = BUILD/'primary-parallel-formal-v1-20261024'
    f = read(failed/'run-state.json')
    assert f['status'] == 'TERMINAL_PRIMARY_CLOCK_PROOF_NOT_ESTABLISHED'
    assert len(f['cases']) == 1 and f['cases'][0]['returncode'] != 0
    assert 'No SAT model available for cell' in (failed/'merge-positive/proof.log').read_text()
    assert read(failed/'classification.json')['no_counterexample_or_equivalence_result']
    directories.append(failed)
    proofs = BUILD/'primary-parallel-formal-v2-20261024'
    p = read(proofs/'run-state.json')
    assert p['status'] == 'PASS_PRIMARY_CLOCK_MERGE_AND_PHASE_PROOFS' and len(p['cases']) == 7
    assert p['tests_state_sha256'] == sha(tests/'run-state.json')
    assert p['merged_output_bits'] == 195 and p['crystal_phase_unbounded_induction']
    assert not p['reset_advance_protocol_assumptions']
    for c in p['cases']:
        assert c['passed'] and c['returncode'] == 0
        d = proofs/(c['mode']+'-'+c['variant'])
        for name, digest in c['files'].items():
            assert sha(d/name) == digest
        assert sha(d/'proof.log') == c['log_sha256']
        raw = (d/'source.sv').read_text()
        expected, reference = extract(raw, c['mode'])
        assert (d/'actual.sv').read_text() == expected
        if reference:
            assert (d/'reference.sv').read_text() == reference
        assert '\nmemory_map\n' in (d/'proof.ys').read_text()
    directories.append(proofs)
    fixture = BUILD/'primary-clock-parallel-component-stage-v1-20261024'
    f = read(fixture/'provenance.json')
    assert f['tests_state_sha256'] == sha(tests/'run-state.json')
    assert f['proofs_state_sha256'] == sha(proofs/'run-state.json')
    assert f['candidate_source_sha256'] == sha(source)
    assert f['result_bits'] == 1560 and f['clock_period_ns'] == 15 and f['CPU_rotations'] == 8
    for name, digest in f['files'].items():
        assert sha(fixture/name) == digest
    baseline = BUILD/'primary-clock-component-stage-v1-20261024'
    for name in ['top.sv', 'clock.sdc', 'clock_fit.qsf', 'audit.tcl', 'fit_control_clock.py']:
        assert sha(fixture/name) == sha(baseline/name)
    directories.append(fixture)
    observed = BUILD/'selector-fit-and-boot-observation-v1-20261024'
    jobs = read(observed/'jobs.json')
    assert len(jobs['jobs']) == 3
    assert [j['source_files_verified'] for j in jobs['jobs']] == [10, 533, 10]
    for j in jobs['jobs']:
        assert j['state']['status'] == 'RUNNING'
        assert j['handles'][0]['live'] and j['handles'][-1]['live']
        assert j['project'] in j['handles'][0]['command']
    sims = read(observed/'simulations.json')
    assert len(sims['runs']) == 4 and sims['no_restart_or_limit_changes']
    areas = []
    for name, expected in [('original', 28390), ('parallel', 6829)]:
        log = (observed/'map-logs'/(name+'.log')).read_text()
        assert 'Analysis & Synthesis was successful.' in log
        count = int(re.search(r'Implemented (\d+) logic cells', log)[1])
        assert count == expected
        warnings = Counter(re.findall(r'Warning \((\d+)\)', log))
        assert set(warnings) == {'12473', '13024', '13410'}
        areas.append(dict(variant=name, synthesized_logic_cells=count, warning_codes=dict(warnings),
                          log_sha256=sha(observed/'map-logs'/(name+'.log'))))
    directories.append(observed)
    for d in directories:
        retain(d, out/d.name)
    result = dict(status='REVIEWED_PARALLEL_PRIMARY_CLOCK_MERGER_AND_LIVE_NATIVE_FIT',
                  source_phase_state_unchanged=True, independent_full_reference_cases=12,
                  full_vector_samples_per_positive=160576, formal_cases=7,
                  unsupported_ROM_proof_retained=True, memory_lowering_adds_no_assumptions=True,
                  synthesis_observations=areas, jobs=jobs,
                  native_consumers_integrated=False, physical_timing_qualified=False,
                  firmware_qualified=False, speedup_qualified=False, assembly_permitted=False,
                  hardware_changed=False, hardware_requires_user_release=True, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


def review_terminal(out):
    assert not out.exists()
    out.mkdir(parents=True)
    results = []
    for variant, name, setup, alms, result_registers in [
        ('original', 'primary-clock-component-terminal-v1-20261024',
         [-136.409, -136.557, -66.261, -53.767], 18260, 1537),
        ('parallel', 'primary-clock-parallel-component-terminal-v1-20261024',
         [-0.960, -0.825, 4.908, 5.146], 3856, 1489)]:
        d = BUILD/name
        receipt = read(BUILD/(name+'.tgz.json'))
        assert sha(BUILD/(name+'.tgz')) == receipt['archive_sha256']
        assert len(receipt['members']) == 58
        for row in receipt['members']:
            assert sha(d/row['path']) == row['sha256']
            assert (d/row['path']).stat().st_size == row['bytes']
        s = read(d/'run-state.json')
        assert s['status'] == 'TERMINAL_REVIEW_REQUIRED'
        assert [r['stage'] for r in s['stages']] == ['map', 'fit', 'sta', 'corners']
        assert all(r['returncode'] == 0 for r in s['stages'])
        p = read(d/'provenance.json')
        readback = read(d/'collected-input-check.json')
        assert readback['job_pids_absent'] == [s['builder_pid']]+[r['pid'] for r in s['stages']]
        assert readback['changed_inputs'] == ['clock_fit.qsf']
        assert p['result_bits'] == 1560 and p['clock_period_ns'] == 15
        for name_in, digest in p['files'].items():
            raw = (d/name_in).read_bytes()
            assert sha(d/name_in) == readback['actual'][name_in]
            if name_in == 'clock_fit.qsf':
                annotation = b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
                assert raw.endswith(annotation) and raw.count(annotation) == 1
                raw = raw[:-len(annotation)]
            import hashlib
            assert hashlib.sha256(raw).hexdigest() == digest
        log = (d/'corners.log').read_text()
        assert re.findall(r'PRIMARY_CLOCK_OUTPUT_BITS (\d+)', log) == ['1560']*4
        assert re.findall(r'PRIMARY_CLOCK_REGISTER_COVERAGE launch=(\d+) result=(\d+)', log) == [('3', str(result_registers))]*4
        corners = []
        for corner in ['7_slow_1100mv_100c', '7_slow_1100mv_-40c', 'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c']:
            row = dict(condition=corner)
            for kind in ['setup', 'hold', 'evaluator']:
                row[kind] = first_path(d/f'clock-{kind}-{corner}.rpt')
            ucp = (d/f'clock-ucp-{corner}.rpt').read_text()
            counts = re.findall(r'; Unconstrained (?:Clocks|Input Ports|Input Port Paths|Output Ports|Output Port Paths)\s*; (\d+)\s*; (\d+)', ucp)
            assert len(counts) == 5 and all(pair == ('0', '0') for pair in counts)
            assert row['hold']['slack_ns'] > 0
            corners.append(row)
        assert [r['setup']['slack_ns'] for r in corners] == setup
        fit = (d/'clock_fit.fit.summary').read_text()
        assert f'{alms:,} / 41,910' in fit
        if variant == 'parallel':
            assert 'Total DSP Blocks : 0 /' in fit
            assert 'pixel|remainder[12]' in corners[0]['setup']['source']
            assert corners[0]['setup']['sink'] == 'result[1205]~reg0'
        retain(d, out/d.name)
        (out/(d.name+'.tgz.json')).write_bytes((BUILD/(d.name+'.tgz.json')).read_bytes())
        results.append(dict(variant=variant, ALMs=alms, corners=corners, timing_accepted=False))
    result = dict(status='REVIEWED_PRIMARY_COMPONENT_TIMING_REJECTIONS', results=results,
                  remaining_path='pixel remainder -> inner record count -> BB coalescing -> rank -> joined mask -> level cursor -> result',
                  physical_timing_qualified=False, native_consumers_integrated=False,
                  firmware_qualified=False, speedup_qualified=False, hardware_changed=False,
                  assembly_permitted=False, hardware_requires_user_release=True, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--terminal-fits', action='store_true')
    args = parser.parse_args()
    result = review_terminal(args.output.resolve()) if args.terminal_fits else review(args.output.resolve())
    label = 'parallel-primary-terminal' if args.terminal_fits else 'parallel-primary-followup'
    append_archive(args.archive.resolve(), args.output.resolve(), [], label=label,
                   extra_tools=['optimize_primary_clock_merge.py', 'test_primary_clock_events.py',
                                'prove_primary_clock_events.py', 'fit_primary_clock_events.py', Path(__file__).name])
    print(result['status'], flush=True)
