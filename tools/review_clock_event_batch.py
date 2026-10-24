#!/usr/bin/env python3
"""Retain shorter-control timing rejection and the common-clock source proof.

Full native timing remains rejected despite improvement. The new chronological
CPU/pixel producer is unconnected to board consumers. Preserve source bindings,
complete reports, both simulator references, all formal controls and the first
incorrect coincidence-coverage test. No hardware/firmware/speed pass follows.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil

from review_late_hold_and_input_reset import retain, first_path
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from sil_netlist import load_board
from test_r_front_forward_view import sha
from prove_native_cp_context import BP, RTL, MATCH

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def read(p):
    return json.loads(p.read_text())


def retain_test(source, dest):
    for p in source.rglob('*'):
        rel = p.relative_to(source)
        if not p.is_file() or any(part.startswith('obj') for part in rel.parts) or p.suffix in ['.vvp', '.o', '.a']:
            continue
        q = dest/rel
        q.parent.mkdir(parents=True, exist_ok=True)
        q.write_bytes(p.read_bytes())


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    failed = BUILD/'control-short-fit-terminal-v1-20261024'
    envelope = BUILD/'control-short-native-v1-20261024-terminal.tgz.json'
    e = read(envelope)
    assert sha(Path(str(envelope)[:-5])) == e['archive_sha256']
    for row in e['members']:
        p = failed/row['path']
        assert sha(p) == row['sha256'] and p.stat().st_size == row['bytes']
    s = read(failed/'run-state.json')
    assert s['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED' and all(a['returncode'] == 0 for a in s['stages'])
    assert read(failed/'physical-report/run-state.json')['status'] == 'TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
    m = read(failed/'r-front-control-native.json')
    assert m['frequency_hz'] == 57142857 and m['sysper'] == 4 and not m['assembly_permitted']
    assert read(failed/'physical-native-input-readback.json')['actual'] == m['source_hashes']
    for name, digest in m['source_hashes'].items():
        assert sha(failed/name) == digest
    corners = []
    log = (failed/'physical-report/audit.log').read_text()
    coverage = re.findall(r'PHYSICAL_NATIVE_COVERAGE (\S+) keepers=(\d+) central=(\d+) primary=(\d+) duplicates=(\d+) history=(\d+)', log)
    assert len(coverage) == 4 and all(row[1:] == ('85151', '804', '695', '109', '274') for row in coverage)
    for c in ['7_slow_1100mv_100c', '7_slow_1100mv_-40c', 'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c']:
        row = dict(condition=c)
        for label, prefix in [('setup', 'native59-setup'), ('hold', 'native59-hold'),
                              ('input', 'physical-native-input-setup'), ('internal', 'physical-native-internal-setup')]:
            row[label] = first_path(failed/'core'/(prefix+'-'+c+'.rpt'))
        visible = re.findall(r'^PHYSICAL_NATIVE_VISIBLE_R '+re.escape(c)+r' (.+)$', log, re.M)
        assert len({re.sub(r'~DUPLICATE.*$', '', name) for name in visible}) == 16
        assert len(re.findall(r'^PHYSICAL_NATIVE_RAM '+re.escape(c)+r' (\w+) keepers=', log, re.M)) == 4
        corners.append(row)
    assert [r['setup']['slack_ns'] for r in corners] == [-0.475, -0.145, 3.708, 4.549]
    assert min(r['hold']['slack_ns'] for r in corners) == 0.112
    assert min(r['internal']['slack_ns'] for r in corners) == 0.195
    path = (failed/'core/native59-setup-7_slow_1100mv_100c.rpt').read_text()
    path = path[path.index('Path #1:'):path.index('Path #2:')]
    assert re.search(r';\s*321\s*;[^\n]*u_d15\|qb\|q\s*;', path)
    assert 'rm_l_rtl_0' in corners[0]['setup']['sink']
    retain(failed, out/failed.name)
    shutil.copy2(envelope, out/envelope.name)
    tests = BUILD/'clock-batch-original-reference-v1-20261024'
    t = read(tests/'run-state.json')
    assert t['status'] == 'PASS_COMMON_CPU_PIXEL_EVENT_BATCH' and len(t['cases']) == 12
    for name, digest in t['inputs'].items():
        assert sha(tests/'inputs'/name) == digest
    for row in t['cases']:
        assert row['passed'] and (row['returncode'] != 0) == row['expected_rejection']
        assert sha(tests/(row['engine']+'-'+row['variant']+'-test.log')) == row['log_sha256']
    for name in ['dorado_clock_event_batch.sv', 'dorado_oscillator_events.sv']:
        assert sha(ROOT/'verilog/rtl'/name) == t['inputs'][name]
    assert sha(ROOT/'verilog/verilator/tb_clock_event_batch.sv') == t['inputs']['tb_clock_event_batch.sv']
    board = load_board(str(tests/'inputs/BaseBd-Rev-Am.wl'))
    pins = {p['pin']: name for name, net in board.nets.items() for p in net['pins'] if p['pkg'] == 'j02'}
    assert board.packages['j02']['type'] == 'MC10210'
    assert {pin: pins[pin] for pin in [5, 6, 7, 9, 10, 11]} == {
        5: "StartClockPulse'", 6: 'EndClockPulse', 7: 'dStartClockPulse',
        9: "StartClockPulse'", 10: 'EndClockPulse', 11: 'dStartClockPulse'}
    retain_test(tests, out/tests.name)
    proofs = []
    for variant in ['positive', 'drop-second', 'late-cpu', 'lose-coincident', 'advance-held', 'reverse-order', 'capacity-positive']:
        version = 2 if variant == 'capacity-positive' else 1
        d = BUILD/f'clock-batch-{variant}-proof-v{version}-20261024'
        p = read(d/'run-state.json')
        positive = variant.endswith('positive')
        assert p['status'] == ('PASS_COMMON_CLOCK_BATCH_INDUCTION' if positive else 'PASS_CLOCK_BATCH_COUNTEREXAMPLE')
        assert p['cpu_rotations'] == 8 and not p['assumptions_added'] and p['exact_observational_alias_inverse']
        assert p['unbounded_induction_proven'] == positive
        assert p['tests_state_sha256'] == sha(tests/'run-state.json')
        for name, digest in p['files'].items():
            assert sha(d/name) == digest
        assert sha(d/'proof.log') == p['proof_log_sha256']
        assert (d/'original.sv').read_bytes() == (tests/'inputs'/(p['variant']+'.sv')).read_bytes()
        if version == 2:
            assert '(count<=4)' in (d/'reference.sv').read_text()
        retain(d, out/d.name)
        proofs.append(dict(directory=d.name, status=p['status'], state_sha256=sha(d/'run-state.json')))
    smoke = BUILD/'clock-batch-positive-smoke-v1-20261024'
    old = read(smoke/'results.json')
    assert old['status'] == 'TERMINAL_FAILED'
    for name, digest in old['inputs'].items():
        assert sha(ROOT/name) == digest
    text = (smoke/'test.log').read_text()
    assert 'CLOCK_BATCH incomplete coverage phase=1 coincident=743 full=0' in text
    assert 'timeline mismatch' not in text and 'committed phase mismatch' not in text
    retain_test(smoke, out/smoke.name)
    native = []
    for name in ['cp-prefix-balanced-native-boot-v1-20261024', 'cp-prefix-native-owned-cold-v1-20261024',
                 'native-cedar-5c65-input-reset-v3-20261023', 'native-cedar-queue-input-reset-v3-20261023']:
        source = BUILD/name
        dest = out/('observed-'+name)
        native.append(observation(source, dest))
        for name in ['provenance.json.gz', 'host-cycle-limit.txt', 'window-control.txt']:
            shutil.copy2(source/name, dest/name)
    result = dict(status='REVIEWED_SHORT_CONTROL_REJECTION_AND_COMMON_CLOCK_BATCH', scope=__doc__,
        rejected_native_corners=corners, collected_members=len(e['members']), native_accepted=False,
        critical_source_loads=321, physical_keeper_coverage=coverage, central_physical_primary=695,
        central_physical_copies=109, canonical_visible_R_bits=16, physical_SRAM_banks=4,
        fitted_ALMs=36289, fitted_registers=55684, minimum_internal_setup_ns=0.195,
        clock_batch_test_cases=t['cases'], clock_batch_proofs=proofs, cpu_rotations=8,
        simulator_intervals_per_rotation=2048, records_per_interval_max=4, time_quantum_ps=1250,
        simultaneous_events_merged=True, whole_pre_event_state_required=True,
        root_debug_pulse_must_be_evaluated_by_board=True, board_consumers_connected=False,
        initial_smoke_failure='Reference timeline matched; incorrect demand for four records in coincidence rotations corrected in new full test; original failure retained.',
        native_observations=native, native_timing_qualified=False, firmware_qualified=False,
        speedup_qualified=False, hardware_changed=False, assembly_permitted=False, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


def review_boot(out):
    """Bind the shorter control to a new original-ROM run, without clock changes."""
    assert not out.exists()
    out.mkdir(parents=True)
    stages = []
    for name in ['control-short-native-generic-stage-v1-20261024', 'control-short-native-stage-v1-20261024']:
        d = BUILD/name
        s = read(d/'r-front-control-native.json')
        assert s['shortened_control_terminal_wave']['compared_outputs'] == 363
        assert s['frequency_hz'] == 57142857 and s['sysper'] == 4 and not s['assembly_permitted']
        for path, digest in s['source_hashes'].items():
            assert sha(d/path) == digest
        retain(d, out/d.name)
        stages.append((d, s))
    generic, g = stages[0]
    native, n = stages[1]
    assert (native/RTL).read_text().replace(" wire canonical_match=1'b1;", MATCH, 1) == (generic/RTL).read_text()
    changed = [path for path in g['source_hashes'] if (native/path).read_bytes().replace(
        n['project'].encode(), g['project'].encode()) != (generic/path).read_bytes()]
    assert set(changed) == {BP, RTL}
    proofs = []
    for kind, expected in [('proof', 'PASS_SOURCE_BOUND_NATIVE_CP_CONTEXT'), ('stale', 'PASS_STALE_NATIVE_OWNER_COUNTEREXAMPLE')]:
        d = BUILD/f'control-short-native-context-{kind}-v1-20261024'
        s = read(d/'run-state.json')
        assert s['status'] == expected and s['returncode'] == 0
        assert s['native_manifest_sha256'] == sha(native/'r-front-control-native.json')
        assert s['generic_manifest_sha256'] == sha(generic/'r-front-control-native.json')
        assert not s['input_or_protocol_assumptions_added'] and s['owner_power_up_state'] == 'unconstrained'
        for name, digest in s['source_files'].items():
            assert sha(d/name) == digest
        assert sha(d/'proof.log') == s['proof_log_sha256']
        retain(d, out/d.name)
        proofs.append(dict(directory=d.name, status=expected, state_sha256=sha(d/'run-state.json')))
    run = BUILD/'control-short-native-boot-v1-20261024'
    p = json.loads(gzip.decompress((run/'provenance.json.gz').read_bytes()))
    assert p['stage_manifest_sha256'] == sha(native/'r-front-control-native.json')
    assert p['bounded_original_firmware_run'] and p['host_limit'] == 5000000000
    assert int((run/'host-cycle-limit.txt').read_text()) == 5000000000
    assert p['window_gate_classes'] == 8 and p['owned_context_proof_sha256'] == proofs[0]['state_sha256']
    assert p['passive_commit_and_all_ten_board_pause_checks'] and p['passive_closed_owner_and_SRAM_pause_checks']
    assert p['passive_held128_request_response_checks'] and not p['medium_runtime_writes_persisted']
    for name, digest in p['inputs'].items():
        assert sha(run/'inputs'/name) == digest
    state = read(run/'run-state.json')
    assert state['actions'][-1]['stage'] == 'test' and all(a['returncode'] == 0 for a in state['actions'][:-1])
    dest = out/('observed-'+run.name)
    live = observation(run, dest)
    retain(run/'inputs', dest/'inputs')
    shutil.copy2(run/'compile.log', dest/'compile.log')
    for name in ['provenance.json.gz', 'host-cycle-limit.txt', 'window-control.txt']:
        shutil.copy2(run/name, dest/name)
    result = dict(status='REVIEWED_SHORT_CONTROL_SOURCE_BOUND_LIVE_BOOT', source_count=533,
        generic_to_owned_changes=changed, context_proofs=proofs, live_boot=live, fixed_host_bound=5000000000,
        original_firmware_and_framework_reset_unchanged=True, original_older_bounds_unchanged=True,
        all_eight_window_classes=True, original_medium_readonly_guest_writes_volatile=True,
        common_clock_batch_connected=False, native_clock_hz=57142857, sysper=4,
        native_timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
        hardware_changed=False, assembly_permitted=False, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--native-boot', action='store_true')
    a = p.parse_args()
    r = review_boot(a.output.resolve()) if a.native_boot else review(a.output.resolve())
    append_archive(a.archive.resolve(), a.output.resolve(), [], label='short-control-native-boot' if a.native_boot else 'common-clock-batch', extra_tools=[
        'test_clock_event_batch.py', 'prove_clock_event_batch.py', 'stage_control_terminal_wave.py', Path(__file__).name])
    print(r['status'], flush=True)
