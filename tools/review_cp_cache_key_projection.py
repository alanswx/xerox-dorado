#!/usr/bin/env python3
"""Preserve projected-key safety, complete comparisons and native fit inputs.

The smaller key is digitally qualified and integrated into exact native
owners. Physical cache-register removal and whole-device timing remain live
gates; original60ns, firmware, Cedar/Rollback/persistence and stress remain
required. Preserve every input, negative control and actual live handle.
"""
import argparse
import json
from pathlib import Path
import shutil

from prove_cp_cache_key_projection import projected
from prove_cp_prefix_cache import module
from prove_native_cp_context import RTL, MATCH
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def read(p):
    return json.loads(p.read_text())


def verify(directory, files):
    for name, digest in files.items():
        assert sha(directory/name) == digest, (directory, name)


def review(out):
    assert not out.exists()
    out.mkdir(parents=True)
    formal = []
    for variant, status, marker in [
            ('positive', 'PASS_CP_CACHE_KEY_INDUCTIVE_SAFETY', 'Induction step proven: SUCCESS!'),
            ('negative', 'PASS_CP_CACHE_KEY_NEGATIVE_CONTROL', 'SAT proof finished - model found: FAIL!')]:
        d = BUILD/f'cp-cache-key-projection-{variant}-v1-20261024'
        p = read(d/'run-state.json')
        assert p['status'] == status and p['returncode'] == 0
        assert p['compared_control_output_bits'] == 475 and p['compared_prefix_bits'] == 269
        assert p['unbounded_safety_proven'] == (variant == 'positive')
        verify(d, p['source_hashes'])
        assert sha(d/'proof.log') == p['log_sha256'] and marker in (d/'proof.log').read_text()
        retain(d, out/d.name)
        formal.append(dict(directory=d.name, status=status, state_sha256=sha(d/'run-state.json')))
    proof = BUILD/'cp-cache-key-projection-positive-v1-20261024'
    care = set(read(proof/'run-state.json')['cared_key_bits'])
    assert len(care) == 269 and 138 not in care and all(i in care for i in range(1, 12))
    tests = []
    for variant, status in [('icarus', 'PASS_CP_KEY_COMPLETE_ORIGINAL_COMPARISON'),
                            ('verilator', 'PASS_CP_KEY_COMPLETE_ORIGINAL_COMPARISON'),
                            ('four-state', 'PASS_CP_KEY_FOUR_STATE_COMPARISON'),
                            ('bad-unknown', 'PASS_CP_KEY_UNKNOWN_FALLBACK_COUNTEREXAMPLE')]:
        d = BUILD/f'cp-key-{variant}-v1-20261024'
        p = read(d/'run-state.json')
        assert p['status'] == status and p['proof_state_sha256'] == sha(proof/'run-state.json')
        verify(d, p['files'])
        if variant in ['icarus', 'verilator']:
            assert p['all100_clock_counts_match'] and p['original_testbench_and_references_unchanged']
            assert 'R_FRONT_LOCAL_PROBE PASS checks=14884 writes=238 reads=6928 epochs=392' in (d/'test.log').read_text()
        elif variant == 'four-state':
            assert 'checks=10208 binary_checks=1136 unknown_checks=9072 key_bits=567 outputs=3' in (d/'test.log').read_text()
        else:
            assert p['actions'][0]['returncode'] == 0 and p['actions'][-1]['returncode'] != 0
            assert 'CP_KEY_FOUR_STATE mismatch' in (d/'test.log').read_text()
        retain(d, out/d.name)
        tests.append(dict(directory=d.name, status=status, state_sha256=sha(d/'run-state.json'),
            cache_coverage=p.get('cache_coverage')))
    stages = []
    for variant in ['generic-', '']:
        d = BUILD/f'cp-key-native-{variant}stage-v1-20261024'
        m = read(d/'r-front-control-native.json')
        parent = Path(m['parent_stage'])
        old = read(parent/'r-front-control-native.json')
        assert sha(parent/'r-front-control-native.json') == m['parent_manifest_sha256']
        verify(d, m['source_hashes'])
        assert len(m['source_hashes']) == 533 and m['frequency_hz'] == 57142857 and m['sysper'] == 4
        assert not m['assembly_permitted'] and 'cache_tag_lcell_buffers' not in m
        assert set(m['cp_cache_key_projection']['cared_key_bits']) == care
        assert m['cp_cache_key_projection']['proof_state_sha256'] == sha(proof/'run-state.json')
        before = (parent/RTL).read_text()
        old_cache = module(before, 'dorado_cp_prefix_cache')
        expected = before.replace(old_cache, projected(old_cache, care), 1)
        assert (d/RTL).read_text() == expected
        changes = [name for name in old['source_hashes'] if (d/name).read_bytes().replace(
            m['project'].encode(), old['project'].encode()) != (parent/name).read_bytes()]
        assert changes == [RTL]
        retain(d, out/d.name)
        stages.append(dict(directory=d.name, manifest_sha256=sha(d/'r-front-control-native.json')))
    generic = BUILD/'cp-key-native-generic-stage-v1-20261024'
    native = BUILD/'cp-key-native-stage-v1-20261024'
    assert (native/RTL).read_text().replace(" wire canonical_match=1'b1;", MATCH, 1) == (generic/RTL).read_text()
    contexts = []
    for variant, status in [('proof', 'PASS_SOURCE_BOUND_NATIVE_CP_CONTEXT'),
                            ('stale', 'PASS_STALE_NATIVE_OWNER_COUNTEREXAMPLE')]:
        d = BUILD/f'cp-key-native-context-{variant}-v1-20261024'
        p = read(d/'run-state.json')
        assert p['status'] == status and p['returncode'] == 0
        verify(d, p['source_files'])
        assert sha(d/'proof.log') == p['proof_log_sha256']
        assert p['native_manifest_sha256'] == sha(native/'r-front-control-native.json')
        assert p['generic_manifest_sha256'] == sha(generic/'r-front-control-native.json')
        assert p['owned_projection_bits'] == 156 and p['auxiliary_state_compared_bits'] == 1416
        assert not p['input_or_protocol_assumptions_added']
        assert p['unbounded_induction_proven'] == (variant == 'proof')
        retain(d, out/d.name)
        contexts.append(dict(directory=d.name, status=status, state_sha256=sha(d/'run-state.json')))
    lint = BUILD/'cp-key-native-lint-v1-20261024'
    p = read(lint/'run-state.json')
    assert p['status'] == 'PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION' and p['structural_warnings_visible']
    assert p['stage_manifest_sha256'] == sha(native/'r-front-control-native.json')
    log = (lint/'lint.log').read_text()
    assert log.count('%Warning-UNOPTFLAT:') == 3 and log.count('%Warning-MULTIDRIVEN:') == 1
    retain(lint, out/lint.name)
    calibration = BUILD/'cp-key-query-calibration-v1-20261024'
    c = read(calibration/'verified-readback.json')
    verify(calibration, c['files'])
    assert c['all_query_process_handles_absent'] and c['critical_unused_key138_present']
    q = read(calibration/'results.json')
    assert q['physical_key_keeper_count'] == 389 and len(q['distinct_logical_key_bits']) == 385
    assert q['critical_unused_key138_present']
    assert sha(calibration/'audit_cp_cache_key_native.tcl') == read(calibration/'run-state.json')['script_sha256']
    retain(calibration, out/calibration.name)
    live_path = BUILD/'cp-key-native-live-v1-20261024.json'
    live = read(live_path)
    assert live['run_state']['status'] == 'RUNNING' and live['source_hashes_verified'] == 533
    assert live['manifest_sha256'] == sha(native/'r-front-control-native.json')
    assert len(live['processes']) == len(live['reporters']) == 2
    assert all(not r['state']['actions'] for r in live['reporters'])
    shutil.copy2(live_path, out/live_path.name)
    result = dict(status='REVIEWED_PROJECTED_KEY_SAFETY_AND_NATIVE_FIT_STAGING',
        formal=formal, complete_reference_and_four_state_tests=tests, stages=stages, contexts=contexts,
        whole_control_results=475, complete_cached_prefix_outputs=269, logical_key_bits=567,
        cared_key_bits=sorted(care), comparison_checks=10208, unknown_checks=9072,
        original_global_barrier_SRAM_ports_and_capture_phases_preserved=True,
        parent_query_calibration=dict(physical_keepers=389, logical_key_bits=385, critical_key138_present=True),
        frozen_live_fit=live, all_original_firmware_test_jobs_and_bounds_unchanged=True,
        physical_key_removal_qualified=False, physical_timing_qualified=False,
        original60ns_qualified=False, faster_SDRAM_qualified=False, firmware_qualified=False,
        speedup_qualified=False, assembly_permitted=False, hardware_changed=False, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    result = review(a.output.resolve())
    append_archive(a.archive.resolve(), a.output.resolve(), [], label='cp-key-native-staging', extra_tools=[
        'prove_cp_cache_key_projection.py', 'test_cp_cache_key_projection.py', 'stage_cp_cache_key_projection.py',
        'report_cp_cache_key_native.py', 'prove_native_cp_context.py', Path(__file__).name])
    print(result['status'], flush=True)
