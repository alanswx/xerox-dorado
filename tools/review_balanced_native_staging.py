#!/usr/bin/env python3
"""Retain the timing-passing slice's real native staging and live boot test.

Keep this distinct from full-machine timing, completed boot and performance.
The earlier owned-context synthesis maps successfully, with all988 warnings
retained; absence of a loop diagnostic is not proof of fitted feedback safety.
The new5B boot test is a separate experiment, never an extended older bound.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import shutil

from prove_native_cp_context import BP, RTL, MATCH
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/rtl-executive'


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    stages = []
    for kind in ['generic', 'owned']:
        source = BUILD / f'cp-prefix-balanced-native-{kind}-stage-v1-20261024'
        p = json.loads((source / 'r-front-control-native.json').read_text())
        assert p['balanced_task_priority_proven'] and p['owned_caller_slice15ns_passed']
        assert p['frequency_hz'] == 57142857 and p['sysper'] == 4 and p['ordinary_host_ticks'] == 8
        assert not p['assembly_permitted'] and not p['native_qualified']
        assert p['priority_review_sha256'] == sha(Path(p['priority_review']) / 'results.json')
        for name, digest in p['source_hashes'].items():
            assert sha(source / name) == digest, name
        retain(source, out / source.name)
        stages.append((source, p))
    g, a = stages[0]
    n, b = stages[1]
    assert set(a['source_hashes']) == set(b['source_hashes']) and len(b['source_hashes']) == 533
    changed = [name for name in a['source_hashes'] if
               (n / name).read_bytes().replace(b['project'].encode(), a['project'].encode()) != (g / name).read_bytes()]
    assert set(changed) == {BP, RTL}
    assert (n / RTL).read_text().replace(" wire canonical_match=1'b1;", MATCH, 1) == (g / RTL).read_text()
    proofs = []
    for kind, expected in [('proof', 'PASS_SOURCE_BOUND_NATIVE_CP_CONTEXT'),
                           ('stale', 'PASS_STALE_NATIVE_OWNER_COUNTEREXAMPLE')]:
        source = BUILD / f'native-balanced-cp-context-{kind}-v1-20261024'
        p = json.loads((source / 'run-state.json').read_text())
        assert p['status'] == expected and p['returncode'] == 0
        assert p['native_manifest_sha256'] == sha(n / 'r-front-control-native.json')
        assert p['generic_manifest_sha256'] == sha(g / 'r-front-control-native.json')
        assert not p['input_or_protocol_assumptions_added'] and p['owner_power_up_state'] == 'unconstrained'
        for name, digest in p['source_files'].items():
            assert sha(source / name) == digest, name
        assert sha(source / 'proof.log') == p['proof_log_sha256']
        retain(source, out / source.name)
        proofs.append(dict(directory=source.name, status=expected, state_sha256=sha(source / 'run-state.json')))
    mapped = BUILD / 'native-cp-context-map-terminal-v1-20261024'
    m = json.loads((mapped / 'native-map-state.json').read_text())
    assert m['status'] == 'TERMINAL_NATIVE_SYNTHESIS_REVIEW_REQUIRED' and m['returncode'] == 0
    assert not m['source_changes'] and sha(mapped / 'native-map.log') == m['log_sha256']
    assert m['source_manifest_sha256'] == sha(mapped / 'r-front-control-native.json')
    older = BUILD / 'cp-prefix-native-owned-stage-v1-20261024'
    assert sha(older / 'r-front-control-native.json') == m['source_manifest_sha256']
    for name, digest in json.loads((older / 'r-front-control-native.json').read_text())['source_hashes'].items():
        assert sha(older / name) == digest, name
    text = (mapped / 'native-map.log').read_text()
    assert 'successful. 0 errors, 988 warnings' in text
    structural = [line for line in text.splitlines() if re.search(r'loop|combinational path|multiple driver|inferred latch', line, re.I)]
    retain(mapped, out / mapped.name)
    source = BUILD / 'cp-prefix-balanced-native-boot-v1-20261024'
    p = json.loads(gzip.decompress((source / 'provenance.json.gz').read_bytes()))
    assert p['stage_manifest_sha256'] == sha(n / 'r-front-control-native.json')
    assert p['bounded_original_firmware_run'] and p['host_limit'] == 5000000000
    assert int((source / 'host-cycle-limit.txt').read_text()) == 5000000000
    assert p['window_gate_classes'] == 8 and p['owned_context_proof_sha256'] == proofs[0]['state_sha256']
    for name, digest in p['inputs'].items():
        assert sha(source / 'inputs' / name) == digest, name
    dest = out / ('observed-' + source.name)
    live = observation(source, dest)
    retain(source / 'inputs', dest / 'inputs')
    for name in ['provenance.json.gz', 'host-cycle-limit.txt', 'window-control.txt']:
        shutil.copy2(source / name, dest / name)
    result = dict(status='REVIEWED_BALANCED_NATIVE_STAGING_AND_LIVE_BOOT', scope=__doc__,
        native_source_count=533, generic_to_owned_changes=changed, context_proofs=proofs,
        native_clock_hz=57142857, sysper=4, ordinary_host_ticks=8,
        earlier_owned_native_synthesis=dict(directory=mapped.name, returncode=0, warnings=988,
            source_changes=[], selected_structural_diagnostics=structural, fitted_feedback_safety_qualified=False),
        new_boot=live, new_boot_fixed_bound=5000000000, older_jobs_or_bounds_changed=False,
        native_timing_qualified=False, original60ns_qualified=False, firmware_qualified=False,
        hardware_changed=False, speedup_qualified=False, assembly_permitted=False, full_goal_active=True)
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        p.add_argument('--' + name, type=Path, required=True)
    a = p.parse_args()
    r = review(a.output.resolve())
    append_archive(a.archive.resolve(), a.output.resolve(), [], label='balanced-native-staging', extra_tools=[
        'stage_cp_prefix_native.py', 'prepare_cp_native_pause.py', 'run_r_front_control_native.py',
        'prove_native_cp_context.py', 'map_native_cp_context.py', Path(__file__).name])
    print(r['status'], flush=True)
