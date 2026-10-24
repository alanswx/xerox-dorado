#!/usr/bin/env python3
"""Retain guarded-prefix correctness, negative controls and full timing results.

This receipt qualifies a component experiment only. Preserve timing failures,
all original comparisons and each proof's explicit scope. Native all-board
pause integration, firmware, timers, physical SDRAM and measured speed remain
open; the complete delivery goal is not narrowed by this review.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil

from fit_cp_prefix_cache import gate
from review_late_hold_and_input_reset import first_path, retain
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/rtl-executive'


def read(path):
    return json.loads(path.read_text())


def review(out, fit, parallel=False):
    assert not out.exists()
    out.mkdir(parents=True)
    positive, negative, proofs = [], [], []
    family = 'cp-prefix-parallel' if parallel else 'cp-prefix-cache'
    version = 1 if parallel else 2
    for engine in ['icarus', 'verilator']:
        source = BUILD / f'{family}-{engine}-v{version}-20261023'
        state, checked = gate(source, engine)
        assert checked['coverage'] == dict(fast=13347, staged=1537, wait_edges=1553, retries=7, resets=1)
        positive.append(checked)
        retain(source, out / source.name)
        # First passing prototype lacked the explicit downstream write-enable
        # assertion; retain it as history, not the final protocol evidence.
        if not parallel:
            source = BUILD / f'cp-prefix-cache-{engine}-v1-20261023'
            state = read(source / 'run-state.json')
            assert state['status'] == 'PASS_CP_PREFIX_CACHE_ORIGINAL_COMPARISON'
            for row in state['source_files']:
                assert sha(source / row['path']) == row['output_sha256']
            retain(source, out / source.name)
        for mutation, fatal in [
                ('stale-tag', 'CP stale tag accepted changed CP data check=0'),
                ('bad-prefix', 'R front resolved source mismatch check=0'),
                ('unguarded-write', 'CP unready request reached the original SRAM owner check=0')]:
            source = BUILD / f'{family}-{mutation}-{engine}-v{version}-20261023'
            state = read(source / 'run-state.json')
            assert state['status'] == 'CP_PREFIX_CACHE_FAILED' and state['mutation'] == mutation
            assert state['actions'][0]['returncode'] == 0 and state['actions'][-1]['returncode'] != 0
            assert fatal in (source / 'test.log').read_text()
            for row in state['source_files']:
                assert sha(source / row['path']) == row['output_sha256']
            negative.append(dict(directory=source.name, engine=engine, mutation=mutation,
                                 rejection=fatal, state_sha256=sha(source / 'run-state.json')))
            retain(source, out / source.name)
    assert positive[0]['candidate_sha256'] == positive[1]['candidate_sha256']
    assert positive[0]['clocks'] == positive[1]['clocks']

    cases = [
            ('cp-prefix-cache-ready-proof-v1-20261023', 'TERMINAL_CP_CACHE_PROOF_NOT_ESTABLISHED'),
            ('cp-prefix-cache-ready-proof-v2-20261023', 'PASS_BOUNDED_CP_CACHE_READY_EQUIVALENCE'),
            *[(f'cp-prefix-cache-{m}-proof-v1-20261023', 'PASS_BOUNDED_CP_CACHE_COUNTEREXAMPLE')
              for m in ['stale-tag', 'bad-prefix', 'unguarded-write']],
            ('cp-prefix-cache-induction-v1-20261023', None)]
    if parallel:
        cases = [(f'{family}-induction-v1-20261023', 'PASS_CP_CACHE_INDUCTIVE_READY_EQUIVALENCE'),
                 *[(f'{family}-{m}-proof-v1-20261023', 'PASS_BOUNDED_CP_CACHE_COUNTEREXAMPLE')
                   for m in ['stale-tag', 'bad-prefix', 'unguarded-write']]]
    for name, expected in cases:
        source = BUILD / name
        state = read(source / 'run-state.json')
        assert state['status'] != 'RUNNING', name
        if expected:
            assert state['status'] == expected, (name, state['status'])
        for path, h in state['source_files'].items():
            assert sha(source / path) == h, path
        assert sha(source / 'proof.log') == state['proof_log_sha256']
        if 'ready-proof-v1' in name:
            assert 'interface in module port list cannot have a default value' in (source / 'proof.log').read_text()
        elif 'induction' not in name:
            assert state['returncode'] == 0
        proofs.append(dict(directory=name, status=state['status'], state_sha256=sha(source / 'run-state.json'),
                           bounded_host_steps=state['bounded_host_steps'],
                           unbounded_induction_proven=state['unbounded_induction_proven'],
                           proof_succeeded=state['proof_succeeded'], counterexample_found=state['counterexample_found']))
        retain(source, out / source.name)

    fit_state = read(fit / 'run-state.json')
    assert fit_state['status'] == 'TERMINAL_REVIEW_REQUIRED'
    assert [(s['stage'], s['returncode']) for s in fit_state['stages']] == [('map', 0), ('fit', 0), ('sta', 0), ('corners', 0)]
    provenance = read(fit / 'provenance.json')
    assert provenance['result_bits'] == 1401 and provenance['launch_bits'] == 1651
    assert provenance['original_shell_inverse_byte_identical']
    assert provenance['physical_SRAM_banks'] == 4 and not provenance['assembly_permitted']
    if parallel:
        assert provenance['parallel_control_results'] and provenance['control_only_settled_eligibility']
        assert provenance['unbounded_induction_proven']
    for name, h in provenance['files'].items():
        raw = (fit / name).read_bytes()
        if name == 'clock_fit.qsf':
            raw = re.sub(rb'\nset_global_assignment -name LAST_QUARTUS_VERSION "17\.0\.2 Lite Edition"\n?$', b'', raw)
        assert hashlib.sha256(raw).hexdigest() == h, name
    reports = fit / 'cp-prefix-cache-reports'
    corners = []
    for condition in ['7_slow_1100mv_100c', '7_slow_1100mv_-40c', 'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c']:
        row = dict(condition=condition)
        for key, prefix in [('global', 'clock-setup'), ('owners', 'clock-owner-results'),
                            ('late_owners', 'clock-late-hold-owners'), ('hold', 'clock-hold')]:
            row[key] = first_path(reports / (prefix + '-' + condition + '.rpt'))
        counts = re.findall(r'; Unconstrained (?:Clocks|Input Ports|Input Port Paths|Output Ports|Output Port Paths)\s*; (\d+)\s*; (\d+)',
                            (reports / ('clock-ucp-' + condition + '.rpt')).read_text())
        assert len(counts) == 5 and all(v == ('0', '0') for v in counts)
        pulse = reports / ('clock-mpw-' + condition + '.rpt')
        row['minimum_pulse_width_slack_ns'] = float(re.search(r'Worst case slack is ([+-]?[\d.]+)', pulse.read_text())[1])
        for mode in ['recovery', 'removal']:
            report = reports / ('clock-' + mode + '-' + condition + '.rpt')
            row[mode] = None if 'Nothing to report.' in report.read_text() else first_path(report)
        corners.append(row)
    fit_passed = all(row['global']['slack_ns'] >= 0 and row['hold']['slack_ns'] >= 0 and
                     row['minimum_pulse_width_slack_ns'] >= 0 and
                     all(row[mode] is None or row[mode]['slack_ns'] >= 0 for mode in ['recovery', 'removal'])
                     for row in corners)
    fit_summary = (fit / 'clock_fit.fit.summary').read_text()
    area = dict(ALMs=int(re.search(r'Logic utilization \(in ALMs\) : ([\d,]+)', fit_summary)[1].replace(',', '')),
                registers=int(re.search(r'Total registers : (\d+)', fit_summary)[1]))
    retain(fit, out / fit.name)
    native = []
    for kind in ['5c65', 'queue']:
        for suffix in ['skip-wait-v1', 'input-reset-v3']:
            source = BUILD / f'native-cedar-{kind}-{suffix}-20261023'
            dest = out / ('observed-' + source.name)
            row = observation(source, dest)
            assert int((source / 'host-cycle-limit.txt').read_text()) == 5000000000
            for name in ['provenance.json.gz', 'host-cycle-limit.txt', 'window-control.txt']:
                shutil.copy2(source / name, dest / name)
            native.append(row)
    result = dict(status='REVIEWED_PARALLEL_GUARDED_CP_PREFIX_COMPONENT' if parallel else 'REVIEWED_GUARDED_CP_PREFIX_COMPONENT', scope=__doc__,
                  parallel_control_results=parallel,
                  functional_gates=positive, negative_controls=negative, formal_proofs=proofs,
                  component_fit_passed=fit_passed, fit_corners=corners, component_area=area,
                  original_results_preserved=True, timing_exceptions_added=False, physical_SRAM_banks=4,
                  fixture_fast_fraction_is_not_a_native_speed_measurement=True,
                  native_observations=native, native_all_board_pause_integration_qualified=False,
                  original_CP_arrival_profile_still_unqualified=True, hardware_changed=False,
                  physical_timing_qualified=False, speedup_qualified=False,
                  full_goal_active=True, full_FPGA_snapshots_deferred=True)
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'fit', 'archive']:
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--parallel', action='store_true')
    a = parser.parse_args()
    result = review(a.output.resolve(), a.fit.resolve(), a.parallel)
    append_archive(a.archive.resolve(), a.output.resolve(), [], label='cp-prefix-parallel' if a.parallel else 'cp-prefix-cache', extra_tools=[
        'probe_cp_prefix_cache.py', 'prove_cp_prefix_cache.py', 'fit_cp_prefix_cache.py', Path(__file__).name])
    print(result['status'], flush=True)
