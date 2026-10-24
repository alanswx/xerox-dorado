#!/usr/bin/env python3
"""Retain the rejected cofactor fit and source-bound shorter control evidence.

Timing failure remains failure even when transfer proofs pass. Preserve initial
syntax/parser failures, all363 output checks, original full graph references,
all955 single-input X/Z cases and actual live native job handles. No deployment
or original60ns/firmware/speed qualification follows from this review.
"""
import argparse
import json
from pathlib import Path
import re
import shutil

from shorten_control_terminal_wave import transform, BODY
from stage_control_terminal_wave import verify_files, CONTROL
from prove_cp_prefix_cache import module
from review_late_hold_and_input_reset import retain, first_path
from review_r_front_capture_followup import append_archive
from review_r_front_cp_subevents import observation
from test_r_front_forward_view import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def read(p):
    return json.loads(p.read_text())


def review(out, live_observation):
    assert not out.exists()
    out.mkdir(parents=True)
    failed = BUILD/'memory-hold-cofactor-fit-terminal-v1-20261024'
    envelope = BUILD/'memory-hold-cofactor-native-v1-20261024-terminal.tgz.json'
    collection = read(envelope)
    assert sha(Path(str(envelope)[:-5])) == collection['archive_sha256']
    for row in collection['members']:
        p = failed/row['path']
        assert sha(p) == row['sha256'] and p.stat().st_size == row['bytes']
    state = read(failed/'run-state.json')
    assert state['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert all(s['returncode'] == 0 for s in state['stages'])
    assert read(failed/'physical-report/run-state.json')['status'] == 'TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
    manifest = read(failed/'r-front-control-native.json')
    for name, digest in manifest['source_hashes'].items():
        assert sha(failed/name) == digest, name
    assert read(failed/'physical-native-input-readback.json')['actual'] == manifest['source_hashes']
    corners = []
    for c in ['7_slow_1100mv_100c', '7_slow_1100mv_-40c', 'MIN_fast_1100mv_100c', 'MIN_fast_1100mv_-40c']:
        row = dict(condition=c)
        for label, prefix in [('setup', 'native59-setup'), ('hold', 'native59-hold'),
                              ('primary_input', 'physical-native-input-setup'),
                              ('internal', 'physical-native-internal-setup'), ('MemC_output', 'native59-memc-out-setup')]:
            row[label] = first_path(failed/'core'/(prefix+'-'+c+'.rpt'))
        corners.append(row)
    assert [r['setup']['slack_ns'] for r in corners] == [-3.618, -2.835, 2.590, 3.846]
    assert min(r['hold']['slack_ns'] for r in corners) == -0.020
    assert corners[0]['setup']['source'].endswith('r_front_upper_q[234]')
    assert corners[0]['setup']['sink'].endswith('r_front_lower_q[260]')
    retain(failed, out/failed.name)
    shutil.copy2(envelope, out/envelope.name)
    diagnostic = []
    for kind in ['four-transfer', 'settled']:
        d = BUILD/f'control-terminal-{kind}-v1-20261024'
        s = read(d/'run-state.json')
        assert s['status'] == 'PROVED_TERMINAL_WAVE_PROPERTY' and s['proved'] and not s['assumptions_added']
        assert s['arbitrary_binary_state_input_bits'] == 955 and not s['authorizes_wave_count_change']
        assert sha(d/'proof.log') == s['proof_log_sha256']
        for name, digest in s['files'].items():
            assert sha(d/name) == digest
        retain(d, out/d.name)
        diagnostic.append(dict(directory=d.name, outputs_compared=s['outputs_compared'], state_sha256=sha(d/'run-state.json')))
    preparation = BUILD/'control-terminal-preparation-failed-v1-20261024'
    retain(preparation, out/preparation.name)
    proof_results = []
    for version in [1, 2]:
        for variant in ['positive', 'negative']:
            d = BUILD/f'control-short-{variant}-proof-v{version}-20261024'
            s = read(d/'run-state.json')
            verify_files(d, s)
            if version == 1:
                assert s['status'] == 'TERMINAL_SHORT_CONTROL_PROOF_NOT_ESTABLISHED'
                assert s['actions'][0]['returncode'] != 0 and 'syntax error' in (d/'proof-5.log').read_text()
            else:
                assert s['status'] == ('PASS_SHORT_CONTROL_ALL_OUTPUTS' if variant == 'positive' else 'PASS_SHORT_CONTROL_COUNTEREXAMPLES')
                assert s['compared_output_bits'] == 363 and s['arbitrary_binary_input_and_state_bits'] == 955
                assert not s['assumptions_added'] and [(r['waves'], r['passed']) for r in s['results']] == [(5, True), (24, True)]
                assert (d/'candidate.sv').read_text() == transform((d/'original.sv').read_text(), variant == 'negative')
            retain(d, out/d.name)
            proof_results.append(dict(directory=d.name, status=s['status'], state_sha256=sha(d/'run-state.json')))
    comparisons = []
    for version in [1, 2]:
        kinds = ['four-state', 'icarus', 'verilator', 'bad-unknown', 'bad-state-icarus']
        if version == 2:
            kinds.append('bad-state-verilator')
        for kind in kinds:
            d = BUILD/f'control-short-{kind}-v{version}-20261024'
            s = read(d/'run-state.json')
            verify_files(d, s)
            if version == 1:
                assert s['status'] == 'TERMINAL_SHORT_CONTROL_TEST_NOT_ESTABLISHED'
                assert s['actions'][0]['returncode'] != 0 and 'syntax error' in (d/'compile.log').read_text()
                meaning = 'Initial positional-port syntax error; no test executed; retained without overwrite.'
            elif kind.startswith('bad-state'):
                assert s['actions'][0]['returncode'] == 0 and s['actions'][-1]['returncode'] != 0
                assert 'R front state commit changed settled Q check=0' in (d/'test.log').read_text()
                assert s['status'] == ('TERMINAL_SHORT_CONTROL_TEST_NOT_ESTABLISHED' if kind.endswith('icarus') else 'PASS_SHORT_CONTROL_SIMULATOR_COUNTEREXAMPLE')
                meaning = 'Original reference rejected state-bit corruption at check0; Icarus diagnostic parser initially missed this message; no rerun.'
            else:
                expected = {'four-state': 'PASS_SHORT_CONTROL_FOUR_STATE', 'bad-unknown': 'PASS_SHORT_CONTROL_SIMULATOR_COUNTEREXAMPLE'}.get(
                    kind, 'PASS_SHORT_CONTROL_COMPLETE_ORIGINAL_COMPARISON')
                assert s['status'] == expected
                if kind in ['icarus', 'verilator']:
                    assert s['all100_clock_counts_match'] and s['original_testbench_and_references_unchanged']
                    assert s['cache_coverage'] == 'fast=13347 staged=1537 wait_edges=1553 retries=7 resets=1'
                    assert 'checks=14884 writes=238 reads=6928 epochs=392' in (d/'test.log').read_text()
                meaning = 'Complete original reference comparison' if kind in ['icarus', 'verilator'] else 'All-input four-state comparison / independent unknown-fallback rejection'
            retain(d, out/d.name)
            comparisons.append(dict(directory=d.name, status=s['status'], interpretation=meaning,
                                    state_sha256=sha(d/'run-state.json')))
    stage = BUILD/'control-short-native-stage-v1-20261024'
    n = read(stage/'r-front-control-native.json')
    parent = Path(n['parent_stage'])
    old = read(parent/'r-front-control-native.json')
    for name, digest in n['source_hashes'].items():
        assert sha(stage/name) == digest
    changed = [name for name in old['source_hashes'] if (stage/name).read_bytes().replace(
        n['project'].encode(), old['project'].encode()) != (parent/name).read_bytes()]
    assert changed == [CONTROL] and (stage/CONTROL).read_text() == transform((parent/CONTROL).read_text())
    assert n['frequency_hz'] == 57142857 and n['sysper'] == 4 and not n['assembly_permitted']
    retain(stage, out/stage.name)
    lint = BUILD/'control-short-native-lint-v1-20261024'
    l = read(lint/'run-state.json')
    assert l['status'] == 'PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION' and l['structural_warnings_visible']
    assert l['stage_manifest_sha256'] == sha(stage/'r-front-control-native.json')
    assert len(re.findall(r'^%Warning-UNOPTFLAT:', (lint/'lint.log').read_text(), re.M)) == 3
    retain(lint, out/lint.name)
    live = read(live_observation)
    assert live['run_state']['status'] == 'RUNNING' and 'quartus' in live['stage_command']
    assert live['manifest_sha256'] == sha(stage/'r-front-control-native.json')
    shutil.copy2(live_observation, out/live_observation.name)
    rom = BUILD/'native-initial-rom-wait-v4-20261024'
    r = read(rom/'results.json')
    assert r['status'] == 'PASS_ORIGINAL_ROM_WAIT_BINDING'
    assert r['global_Q'] == r['sequence_number'] == 0
    assert r['RTC430_value'] == r['emulator_task_T_threshold'] == 13 and r['timeout_subtraction_is_negative']
    for name, digest in r['input_hashes'].items():
        assert sha(rom/'inputs'/name) == digest
    debug = BUILD/'native-5c65-network-read-v6-20261024'
    assert sha(debug/'result.json') == r['readback_sha256']
    for d in [rom, debug]:
        retain(d, out/d.name)
    native = []
    for name in ['cp-prefix-balanced-native-boot-v1-20261024', 'cp-prefix-native-owned-cold-v1-20261024',
                 'native-cedar-5c65-input-reset-v3-20261023', 'native-cedar-queue-input-reset-v3-20261023']:
        d = BUILD/name
        dest = out/('observed-'+name)
        native.append(observation(d, dest))
        for file in ['provenance.json.gz', 'host-cycle-limit.txt', 'window-control.txt']:
            shutil.copy2(d/file, dest/file)
    result = dict(status='REVIEWED_COFACTOR_REJECTION_AND_SHORT_CONTROL', scope=__doc__,
        rejected_cofactor_corners=corners, cofactor_accepted=False, collected_native_members=len(collection['members']),
        diagnostic_properties=diagnostic, full_output_proofs=proof_results, comparisons=comparisons,
        all_original_physical_Q_bits=844, all_original_RAM_bits=9216, all_original_clock_counts=100,
        arbitrary_binary_input_bits=955, compared_atomic_output_bits=363,
        explicit_four_state_checks=1974, unknown_input_checks=1910, independent_unknown_inputs=955,
        native_shortened_control=n['shortened_control_terminal_wave'], native_live=live, native_boot_observations=native,
        baseline_original_ROM_wait=r, packet0_acceptance_or_clock_calibration_qualified=False,
        native_timing_qualified=False, firmware_qualified=False, speedup_qualified=False,
        assembly_permitted=False, hardware_changed=False, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive', 'live-observation']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    r = review(a.output.resolve(), a.live_observation.resolve())
    append_archive(a.archive.resolve(), a.output.resolve(), [], label='control-terminal-wave', extra_tools=[
        'prove_control_terminal_wave.py', 'shorten_control_terminal_wave.py', 'test_control_terminal_wave.py',
        'stage_control_terminal_wave.py', Path(__file__).name])
    print(r['status'], flush=True)
