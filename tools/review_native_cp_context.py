#!/usr/bin/env python3
"""Retain native owner-context proof, bounded startup and honest boot windows.

The context proof is compositional binary equivalence. The completed cold
prefix checks real native consumers but does not reach Cedar. The two actual
Cedar113 windows have different raw traffic and cannot establish a speedup.
A caller-slice timing result, when supplied, is separate from full-core timing.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import shutil

from prove_native_cp_context import BP, RTL, MATCH
from report_native_storage_windows import parse_windows
from report_native_storage_traffic import compare
from review_late_hold_and_input_reset import first_path, retain
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/rtl-executive'


def read(path):
    return json.loads(path.read_text())


def inputs(run):
    if (run / 'provenance.json').exists():
        p = read(run / 'provenance.json')
    elif (run / 'provenance.json.gz').exists():
        p = json.loads(gzip.decompress((run / 'provenance.json.gz').read_bytes()))
    else:
        p = read(run / 'run-state.json')
    for name, digest in p['inputs'].items():
        assert sha(run / 'inputs' / name) == digest, (run.name, name)
    return p


def review_fit(fit, out):
    state = read(fit / 'run-state.json')
    assert state['status'] == 'TERMINAL_REVIEW_REQUIRED'
    assert [(s['stage'], s['returncode']) for s in state['stages']] == [
        ('map', 0), ('fit', 0), ('sta', 0), ('corners', 0)]
    p = read(fit / 'provenance.json')
    assert p['component'] == 'native_owned_caller'
    assert (p['launch_bits'], p['result_bits'], p['physical_Q_bits'], p['ram_bits']) == (420, 1401, 844, 9216)
    assert p['owner_commit_block_byte_identical'] and p['original_result_assignment_byte_identical']
    assert p['SDC_byte_identical'] and not p['added_timing_exceptions'] and not p['assembly_permitted']
    for name, digest in p['files'].items():
        raw = (fit / name).read_bytes()
        if name == 'clock_fit.qsf':
            raw = re.sub(rb'\nset_global_assignment -name LAST_QUARTUS_VERSION "17\.0\.2 Lite Edition"\n?$', b'', raw)
        assert hashlib.sha256(raw).hexdigest() == digest, name
    reports = fit / 'native-owned-caller-reports'
    corners = []
    for condition in ['7_slow_1100mv_100c', '7_slow_1100mv_-40c', 'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c']:
        row = dict(condition=condition)
        for key, prefix in [('global', 'clock-setup'), ('owners', 'clock-owner-results'),
                            ('late_owners', 'clock-late-hold-owners'), ('hold', 'clock-hold')]:
            row[key] = first_path(reports / (prefix + '-' + condition + '.rpt'))
        ucp = (reports / ('clock-ucp-' + condition + '.rpt')).read_text()
        counts = re.findall(r'; Unconstrained (?:Clocks|Input Ports|Input Port Paths|Output Ports|Output Port Paths)\s*; (\d+)\s*; (\d+)', ucp)
        assert len(counts) == 5 and all(v == ('0', '0') for v in counts)
        pulse = (reports / ('clock-mpw-' + condition + '.rpt')).read_text()
        row['pulse_slack_ns'] = float(re.search(r'Worst case slack is ([+-]?[\d.]+)', pulse)[1])
        for mode in ['recovery', 'removal']:
            report = reports / ('clock-' + mode + '-' + condition + '.rpt')
            row[mode] = None if 'Nothing to report.' in report.read_text() else first_path(report)
        corners.append(row)
    area_raw = (fit / 'clock_fit.fit.summary').read_text()
    area = dict(ALMs=int(re.search(r'Logic utilization \(in ALMs\) : ([\d,]+)', area_raw)[1].replace(',', '')),
                registers=int(re.search(r'Total registers : (\d+)', area_raw)[1]))
    passed = all(r['global']['slack_ns'] >= 0 and r['hold']['slack_ns'] >= 0 and r['pulse_slack_ns'] >= 0 and
                 all(r[m] is None or r[m]['slack_ns'] >= 0 for m in ['recovery', 'removal']) for r in corners)
    retain(fit, out / fit.name)
    return dict(directory=fit.name, scope='Actual native owner/control slice; not full core or the earlier generic fixture',
                clock_period_ns=15, all1401_result_indices_retained=True, exceptions_added=False,
                corners=corners, area=area, caller_slice_timing_passed=passed, full_native_timing_qualified=False)


def review(out, fit=None):
    assert not out.exists()
    out.mkdir(parents=True)
    generic = BUILD / 'cp-prefix-native-stage-v2-20261024'
    native = BUILD / 'cp-prefix-native-owned-stage-v1-20261024'
    a = read(generic / 'r-front-control-native.json')
    b = read(native / 'r-front-control-native.json')
    assert set(a['source_hashes']) == set(b['source_hashes']) and len(b['source_hashes']) == 533
    changed = []
    for name in a['source_hashes']:
        assert sha(generic / name) == a['source_hashes'][name]
        assert sha(native / name) == b['source_hashes'][name]
        raw = (native / name).read_bytes().replace(b['project'].encode(), a['project'].encode())
        if raw != (generic / name).read_bytes():
            changed.append(name)
    assert set(changed) == {BP, RTL}
    assert (native / RTL).read_text().replace(" wire canonical_match=1'b1;", MATCH, 1) == (generic / RTL).read_text()
    assert (native / BP).read_text().replace('if(contb_advance && control_ready)begin\n    r_front_lower_q',
            'if(contb_advance)begin\n    r_front_lower_q', 1) == (generic / BP).read_text()
    retain(native, out / native.name)
    proofs = []
    for name, expected in [('native-cp-context-proof-v1-20261024', 'PASS_SOURCE_BOUND_NATIVE_CP_CONTEXT'),
                           ('native-cp-context-stale-v1-20261024', 'PASS_STALE_NATIVE_OWNER_COUNTEREXAMPLE')]:
        source = BUILD / name
        p = read(source / 'run-state.json')
        assert p['status'] == expected and p['returncode'] == 0
        assert p['native_manifest_sha256'] == sha(native / 'r-front-control-native.json')
        assert p['owned_projection_bits'] == 156 and p['auxiliary_state_compared_bits'] == 1416
        assert p['owner_power_up_state'] == 'unconstrained' and not p['input_or_protocol_assumptions_added']
        for path, digest in p['source_files'].items():
            assert sha(source / path) == digest, path
        assert sha(source / 'proof.log') == p['proof_log_sha256']
        assert p['owned_projection_proven'] and (p['counterexample_found'] if 'stale' in name else p['unbounded_induction_proven'])
        retain(source, out / name)
        proofs.append(dict(directory=name, status=expected, state_sha256=sha(source / 'run-state.json')))

    cold = BUILD / 'cp-prefix-native-cold-v3-20261024'
    c = read(cold / 'run-state.json')
    assert c['status'] == 'TERMINAL_NATIVE_MEMORY_DIAGNOSTIC' and c['reason'] == 'HOST_LIMIT'
    assert c['actions'][-1]['returncode'] == 0 and int((cold / 'host-cycle-limit.txt').read_text()) == 300000000
    inputs(cold)
    log = (cold / 'test.log').read_text()
    assert 'NATIVE_CP_PAUSE host=300000000 guest=68288387 ready=1 commits=68288388 pauses=289 snapshots=289' in log
    assert 'NATIVE_C_RELEASED ' not in log and 'NATIVE_WINDOW_BEGIN' not in log
    retain(cold, out / cold.name)
    ledger = []
    for name, extended in [('native-window-legacy-regression-v1-20261024', False),
                           ('native-window-control-pause-v1-20261024', True)]:
        source = BUILD / name
        p = read(source / 'run-state.json')
        assert p['status'] == 'PASS_PASSIVE_NATIVE_WINDOW_OBSERVER_TWO_SIMULATORS'
        assert p['control_pause_schema_tested'] == extended
        inputs(source)
        assert set(p['results']) == {'icarus', 'verilator'}
        if extended:
            for engine in p['results']:
                w = p['results'][engine]['windows']['1']
                assert len(w['gate']) == 8 and int(w['gate']['7']['ticks']) == 3
        retain(source, out / source.name)
        ledger.append(dict(directory=name, gate_classes=8 if extended else 7, engines=['icarus', 'verilator']))

    live = []
    for name in ['cp-prefix-native-owned-cold-v1-20261024',
                 'native-cedar-5c65-input-reset-v3-20261023', 'native-cedar-queue-input-reset-v3-20261023']:
        source = BUILD / name
        inputs(source)
        dest = out / ('observed-' + name)
        live.append(observation(source, dest))
        retain(source / 'inputs', dest / 'inputs')
        for path in ['provenance.json.gz', 'host-cycle-limit.txt', 'window-control.txt']:
            shutil.copy2(source / path, dest / path)
    windows = {}
    for kind in ['5c65', 'queue']:
        source = BUILD / f'native-cedar-{kind}-input-reset-v3-20261023'
        raw = (source / 'test.log').read_bytes()
        selected = []
        for line in raw.splitlines(keepends=True):
            if line.endswith(b'\n') and (line.startswith(b'NATIVE_WINDOW_') and b'id=1 ' in line or
                                        line.startswith((b'NATIVE_C_RELEASED ', b'NATIVE_INPUT_RESET_PASS ', b'NATIVE_BOOT_REQUEST '))):
                selected.append(line)
        path = out / (kind + '-cedar113-window.log')
        path.write_bytes(b''.join(selected))
        parsed = parse_windows(path.read_bytes())
        w = parsed['windows'][1]
        assert w['complete'] and w['firmware_trigger_observed'] and len(w['trace']['completed']) == 8192
        windows[kind] = w
    comparison = compare(windows['5c65']['trace'], windows['queue']['trace'])
    assert comparison['first_disagreement']['index'] == 1536
    assert comparison['prefix_tick_reduction_fraction'] is None
    (out / 'cedar113-windows.json').write_text(json.dumps(dict(windows=windows, comparison=comparison), indent=2) + '\n')
    result = dict(status='REVIEWED_NATIVE_CP_CONTEXT_AND_BOUNDED_STARTUP', scope=__doc__,
        source_changes_after_project_relocation=changed, native_source_count=533,
        native_stage_manifest_sha256=sha(native / 'r-front-control-native.json'), context_proofs=proofs,
        cold_prefix=dict(host_bound=300000000, enabled_machine_edges=68288387, checked_control_pauses=289,
            all_pause_snapshots_equal=True, original_ROM_CP_writes=1, Cedar_boot_reached=False,
            limitation='Frozen cold-v3 has the old out-of-bounds CP window class; no window opens in this prefix. Do not use it for Cedar-window qualification.'),
        window_ledger_tests=ledger, corrected_constructor_uses8_classes=True,
        original_live_observations=live, Cedar113_raw_window_comparison=comparison,
        caller_timing=review_fit(fit, out) if fit else None,
        hardware_changed=False, assembly_permitted=False, firmware_qualified=False,
        native_timing_qualified=False, speedup_qualified=False, full_goal_active=True)
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        p.add_argument('--' + name, type=Path, required=True)
    p.add_argument('--fit', type=Path)
    a = p.parse_args()
    result = review(a.output.resolve(), a.fit.resolve() if a.fit else None)
    append_archive(a.archive.resolve(), a.output.resolve(), [], label='native-cp-context', extra_tools=[
        'prove_native_cp_context.py', 'fit_native_cp_caller.py', 'prepare_cp_native_pause.py',
        'stage_cp_prefix_native.py', 'report_native_storage_windows.py', 'test_native_storage_windows.py', Path(__file__).name])
    print(result['status'], flush=True)
