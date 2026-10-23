#!/usr/bin/env python3
"""Review current forwarding, actual capture inputs, and full timing failures.

Retain the complete failed 15 ns fixture and the separate all-keeper query.
No full native boot, setup/hold, cadence or speed qualification is implied.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re

from review_r_front_windows import timing, matrix
from review_r_front_control_native import archive
from test_r_front_forward_matrix import check
from test_r_front_current_memory_matrix import check as memory_check
from test_r_front_forward_view import candidate, front, ready, bench, sha
from test_r_forward_consumer import bench as consumer_bench
from audit_r_front_fit_endpoints import review as endpoint_review
from test_r_front_memory_inputs import memory as project_memory

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/rtl-executive'


def verify_run(d, consumer=False):
    r = json.loads((d / 'run-state.json').read_text())
    parent = Path(r['parent'])
    assert sha(parent / 'run-state.json') == r['parent_run_state_sha256']
    changed = 0
    for row in r['source_files']:
        p, q = d / row['path'], parent / row['path']
        assert sha(p) == row['output_sha256'] and sha(q) == row['parent_sha256']
        raw = q.read_text()
        expected = raw
        if consumer:
            if p.name == 'tb_r_front_local.sv':
                expected = consumer_bench(raw)
            if r['mutation'] and p.name == 'dorado_local_r_address_step.sv':
                old = 'pins_before[149],rb_forward(state_q,pins_before),pins_before[147:113],'
                assert raw.count(old) == 1
                expected = raw.replace(old, 'pins_before[149],!rb_forward(state_q,pins_before),pins_before[147:113],')
        else:
            transforms = {'candidate.sv': lambda s: candidate(s, r['mutation']),
                          'dorado_local_r_front_step.sv': front,
                          'dorado_local_r_front_ready_step.sv': ready, 'tb_r_front_local.sv': bench}
            if p.name in transforms:
                expected = transforms[p.name](raw)
        assert p.read_text() == expected, p
        assert row['changed'] == (expected != raw)
        changed += row['changed']
    assert changed == ((2 if r['mutation'] else 1) if consumer else 4)
    log = (d / 'test.log').read_text()
    if r['mutation']:
        assert r['actions'][-1]['returncode'] != 0
        message = 'Physical RbBypass consumer mismatch BEFORE commit check=0' if consumer else 'Physical RbBypass mismatch BEFORE commit check=0'
        assert message in log
    else:
        assert r['actions'][-1]['returncode'] == 0
        assert r['summary'] == 'checks=11300 writes=238 reads=4368 epochs=392'
        clocks = re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$', log, re.M)
        assert len(clocks) == 100 and all(int(n) > 0 for c, n in clocks)
    return dict(directory=d.name, status=r['status'], mutation=r['mutation'],
                source_transform_exact=True, run_state_sha256=sha(d / 'run-state.json'))


def review(out):
    assert not out.exists()
    dirs, runs = [], []
    current = BUILD / 'r-front-current-memory-matrix-v1-20261023'
    m = matrix(current, memory_check, 'PASS_R_FRONT_CURRENT_MEMORY_PROFILE_MATRIX')
    dirs += [current] + [ROOT / row['directory'] for row in m['cases']]
    d = BUILD / 'r-front-forward-view-matrix-v1-20261023'
    m = matrix(d, check, 'PASS_R_FRONT_PHYSICAL_FORWARD_PROFILE_MATRIX')
    dirs.append(d)
    for row in m['cases']:
        case = ROOT / row['directory']
        runs.append(verify_run(case))
        dirs.append(case)
    for engine in ['icarus', 'verilator']:
        for name, consumer in [(f'r-front-forward-view-invert-{engine}-v1-20261023', False),
                               (f'r-forward-consumer-{engine}-v1-20261023', True),
                               (f'r-forward-consumer-invert-{engine}-v1-20261023', True)]:
            d = BUILD / name
            runs.append(verify_run(d, consumer))
            dirs.append(d)
    d = BUILD / 'r-forwarding-source-audit-v2-20261023'
    a = json.loads((d / 'results.json').read_text())
    assert a['status'] == 'PASS_ORIGINAL_RBYPASS_PRODUCER_CONSUMER_AUDIT'
    for n, h in a['inputs'].items():
        assert sha(d / 'inputs' / n) == h
    dirs += [d, BUILD / 'r-forwarding-source-audit-v1-20261023']
    fit = BUILD / 'r-front-forward-view-fit-terminal-v1-20261023'
    t = timing(fit, 'r_front_control_physical_forward')
    dirs.append(fit)
    endpoints = BUILD / 'r-front-forward-endpoints-v1-20261023'
    endpoint_review(endpoints)
    e = json.loads((endpoints / 'results.json').read_text())
    dirs.append(endpoints)
    projections = []
    for engine in ['icarus', 'verilator']:
        d = BUILD / f'r-front-memory-inputs-{engine}-v1-20261023'
        r = json.loads((d / 'run-state.json').read_text())
        assert r['status'] == 'PASS_R_FRONT_MEMORY_INPUT_PROJECTION' and r['engine'] == engine
        assert r['actions'][-1]['returncode'] == 0 and r['summary'] == 'checks=11300 writes=238 reads=4368 epochs=392'
        parent = Path(r['parent'])
        assert sha(parent / 'run-state.json') == r['parent_run_state_sha256']
        for row in r['source_files']:
            p, q = d / row['path'], parent / row['path']
            assert sha(p) == row['output_sha256'] and sha(q) == row['parent_sha256']
            expected = project_memory(q.read_text())[0] if p.name == 'dorado_local_r_memory_step.sv' else q.read_text()
            assert p.read_text() == expected
        projections.append(dict(directory=d.name, status=r['status'], run_state_sha256=sha(d / 'run-state.json')))
        dirs.append(d)
    native = []
    for version in [1, 2, 3]:
        d = BUILD / f'r-front-physical-native-stage-v{version}-20261023'
        if not d.exists():
            continue
        r = json.loads((d / 'r-front-control-native.json').read_text())
        for n, h in r['source_hashes'].items():
            assert sha(d / n) == h, n
        assert r['physical_Q_export_separate_from_closed_storage'] and r['canonical_visible_R_owners'] == 16
        lint = BUILD / f'r-front-physical-native-lint-v{version}-20261023'
        l = json.loads((lint / 'run-state.json').read_text())
        assert l['status'] == 'PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION'
        assert l['structural_warnings_visible'] and not l['simulation_started']
        assert sha(d / 'r-front-control-native.json') == l['stage_manifest_sha256']
        warnings = dict(Counter(re.findall(r'^%Warning-([A-Z0-9_]+):', (lint / 'lint.log').read_text(), re.M)))
        native.append(dict(stage=d.name, lint=lint.name, warnings=warnings,
                           manifest_sha256=sha(d / 'r-front-control-native.json')))
        dirs += [d, lint]
    out.mkdir(parents=True)
    result = dict(status='PASS_CURRENT_FORWARDING_REVIEW_TIMING_STILL_FAILS', scope=__doc__,
                  current_memory_matrix_cases=12, current_forward_matrix_cases=12,
                  comparisons=runs, original_source_audit=a['status'], full_fit=t,
                  endpoint_audit=e, memory_input_projection=projections,
                  native_stages=native, native_timing_qualified=False,
                  ordinary60ns_qualified=False, speedup_qualified=False,
                  next='Resolve real source/phase/dependency seams and measure complete native core; retain full A-F goal.')
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    return result, dirs


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--archive', type=Path)
    a = p.parse_args()
    result, dirs = review(a.output.resolve())
    print(result['status'])
    if a.archive:
        archive(a.archive.resolve(), a.output.resolve(), dirs, additional_tools=[
            'test_r_front_forward_view.py', 'test_r_front_forward_matrix.py', 'test_r_forward_consumer.py',
            'fit_r_front_forward_view.py', 'audit_r_forwarding.py', 'audit_r_front_fit_endpoints.py',
            'stage_r_front_physical_native.py', 'test_r_front_memory_inputs.py', 'review_r_front_forward_view.py',
            'build_r_front_ready_native.py'])
