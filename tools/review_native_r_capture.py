#!/usr/bin/env python3
"""Review original visible-R owners, their capture contract and native hookup.

Separate primitive/interface proofs, bounded processor replays and a private
full-board startup observation. None establishes native paired-phase timing,
firmware boot, FPGA timing closure or a measured hardware speed improvement.
"""
import argparse
import gzip
import json
from pathlib import Path
import re
import subprocess
import time

from generate_cp_paired_read import processor, caller
from generate_native_r_capture import generate, expose, expose_caller, rewire_board
from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from review_cp_paired_read import observed
from review_late_hold_and_input_reset import retain
from review_r_front_capture_followup import append_archive
from test_external_r_packet import sha

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/rtl-executive'
TOOLS = ['generate_native_r_capture.py', 'prove_native_r_capture.py',
         'test_native_r_capture_four_state.py', 'stage_native_cp_read.py',
         'test_cp_paired_read.py', 'replay_cp_paired_read.py',
         'inspect_native_baseboard_process.py', 'review_native_r_capture.py']


def read(path):
    return json.loads(path.read_text())


def clock_outcomes(directory):
    return re.findall(r'^(?:FULL_PROCESSOR_PACKET_PASS|PACKET_CLOCK|CACHED_PACKET_COVERAGE|'
                      r'CP_PACKET_GEOMETRY_REJECTIONS|CP_PACKET_STAGED_READS|NATIVE_R_PACKET_COVERAGE) .*$',
                      (directory / 'test.log').read_text(), re.M)


def review(out, require_full=False):
    assert not out.exists()
    out.mkdir(parents=True)
    runs, pending, directories = [], [], []
    for directory in sorted(BUILD.glob('native-R-*-20261025')):
        if 'review' in directory.name:
            continue
        state_path = directory / 'run-state.json'
        if not state_path.exists():
            continue
        state, row = observed(directory)
        runs.append(row)
        if row.get('verified_live'):
            pending.append(row)
        retain(directory, out / directory.name)
        directories.append(directory)

    base = BUILD / 'supported-fall-guard-owned-verilator-v1-20261024'
    functions = (BUILD / 'native-cp-source-v1-20261024/functions.sv').read_text()
    original = processor((base / 'candidate.sv').read_text(), functions)
    candidate = expose(original)
    owned = expose_caller(caller((base / 'caller.sv').read_text(), functions))
    bridge, cell, pins = generate(ROOT)
    for name in ['dorado_r_front_control_step', 'dorado_r_front_pure_step']:
        assert module(candidate, name) == module(original, name)
    focused = []
    for engine in ['icarus', 'verilator']:
        directory = BUILD / f'native-R-single-{engine}-v1-20261025'
        state = read(directory / 'run-state.json')
        assert state['status'] == 'PASS_COMBINED_CP_PACKET_SINGLE_FALLBACK'
        assert state['actual_native_visible_R_cells']
        assert state['full_trace_byte_identical_to_original_checked_fixture']
        assert any(row[1] == '1' for row in state['payload_probes'])
        assert (directory / 'candidate.sv').read_text() == candidate
        assert (directory / 'caller.sv').read_text() == owned
        assert (directory / 'native_R.sv').read_text() == bridge
        assert (directory / 'native_MC10173.v').read_text() == cell
        focused.append(directory)
    for name in ['transactions.hex', 'initial.hex']:
        assert sha(focused[0] / name) == sha(focused[1] / name)
    bad = BUILD / 'native-R-single-bad-select-icarus-v1-20261025'
    assert read(bad / 'run-state.json')['status'] == 'PASS_COMBINED_CP_PACKET_COUNTEREXAMPLE'
    assert 'READ_OWNER common publication mismatch' in (bad / 'test.log').read_text()

    complete, replays = [], {}
    for mode, version in [('full', 2), ('words', 1)]:
        peers = [BUILD / f'native-R-{mode}-{engine}-v{version}-20261025'
                 for engine in ['icarus', 'verilator']]
        states = [read(d / 'run-state.json') for d in peers]
        assert states[1]['status'] == 'PASS_COMBINED_CP_COHERENT_PACKETS'
        for directory, state in zip(peers, states):
            assert state['actual_native_visible_R_cells']
            assert not state['external_visible_R_predicted_result_register']
            assert state['original_per_event_result_owner_RAM_and_clock_comparisons_retained']
            assert (directory / 'candidate.sv').read_text() == candidate
            assert (directory / 'caller.sv').read_text() == owned
            assert (directory / 'native_R.sv').read_text() == bridge
            assert (directory / 'native_MC10173.v').read_text() == cell
        if all(s['status'] == 'PASS_COMBINED_CP_COHERENT_PACKETS' for s in states):
            assert clock_outcomes(peers[0]) == clock_outcomes(peers[1])
            complete.append(mode)
        replays[mode] = dict(summary=states[1]['summary'],
                            coverage=states[1]['native_R_coverage'],
                            staged_reads=states[1]['staged_reads'])
        assert states[1]['geometry_cases'] == 378
        assert len(re.findall(r'^PACKET_CLOCK channel=\d+ edges=\d+$',
                              (peers[1] / 'test.log').read_text(), re.M)) == 100
    assert 'paired_writes=119 forwarded_bytes=238' in replays['full']['summary']
    assert replays['words']['staged_reads'] == 1
    if require_full:
        assert not pending and len(complete) == 2, (pending, complete)

    # This particular legal replay does NOT distinguish the final-clock-only
    # mutation: every first-only write repeats an already held visible byte.
    # Preserve the failed attempt at a negative, without calling it a DUT fail.
    vacuous = BUILD / 'native-R-full-final-clock-icarus-v2-20261025'
    state = read(vacuous / 'run-state.json')
    assert state['status'] == 'TERMINAL_COMBINED_CP_PACKET_REPLAY_NOT_ESTABLISHED'
    log = (vacuous / 'test.log').read_text()
    assert 'FULL_PROCESSOR_PACKET_PASS transactions=14884' in log
    assert 'NATIVE_R_PACKET_COVERAGE first_only=4358 first_only_changed=0' in log

    proof = BUILD / 'native-R-capture-proof-v2-20261025'
    state = read(proof / 'run-state.json')
    verify(proof, state['files'])
    assert state['status'] == 'PASS_ORIGINAL_NATIVE_R_CAPTURE_AND_NEGATIVES'
    assert state['no_protocol_assumptions'] and state['initial_native_Q_bits_unconstrained'] == 16
    for case in state['cases']:
        directory = proof / (case['mutation'] or 'contracts')
        verify(directory, case['files'])
        assert case['passed'] and case['returncode'] == 0
        assert sha(directory / 'proof.log') == case['log_sha256']
        script = (directory / 'proof.ys').read_text()
        assert '-prove native_match 1' in script
        if case['mutation']:
            witness = read(directory / 'counterexample.json')
            assert {s['name'] for s in witness['signal']} >= {'visible', 'expected', 'native_match'}
            assert 'model found: FAIL!' in (directory / 'proof.log').read_text()
        else:
            assert (directory / 'bridge.sv').read_text() == bridge
            assert (directory / 'cell.v').read_text() == cell
            assert 'Induction step proven: SUCCESS!' in (directory / 'proof.log').read_text()
    four = BUILD / 'native-R-four-state-icarus-v1-20261025'
    state = read(four / 'run-state.json')
    assert state['status'] == 'PASS_NATIVE_R_FOUR_STATE_CAPTURE'
    assert state['engine'] == 'icarus' and state['actual_native_cell_bits'] == 16
    assert (four / 'bridge.sv').read_text() == bridge
    assert (four / 'cell.v').read_text() == cell
    assert 'NATIVE_R_FOUR_STATE_PASS checks=16384' in (four / 'test.log').read_text()

    native = BUILD / 'native-cp-read-cells-prefix-v1-20261025'
    provenance = read_gzip(native / 'provenance.json.gz')
    verify(native / 'inputs', provenance['inputs'])
    baseline = Path(provenance['baseline_run'])
    for board in ['ProcH', 'ProcL']:
        name = f'src/fpga/mister/snapshot_native_processor/{board}_full_snapshot.sv'
        assert (native / 'inputs' / name).read_text() == rewire_board(
            (baseline / 'inputs' / name).read_text(), board)
    assert not provenance['paired_mode_enabled'] and not provenance['assembly_permitted']
    assert provenance['host_limit'] == 250000000 and provenance['original_ROM_bytes_unchanged']
    assert len(provenance['used_hdl']) == 194
    for name, digest in provenance['inputs'].items():
        if '/proms/eprom/' in name:
            assert sha(baseline / 'inputs' / name) == digest
    lint = read(native / 'lint-state.json')
    assert lint['status'] == 'PASS_PRIVATE_NATIVE_CP_READ_ELABORATION'
    warnings = re.findall(r'^%Warning-UNOPTFLAT: (.+)$', (native / 'lint.log').read_text(), re.M)
    assert len(warnings) == 8
    state = read(native / 'run-state.json')
    current = state['actions'][-1]
    native_observation = dict(status=state['status'], pid=current['pid'], observed=time.time(),
                              state_sha256=sha(native / 'run-state.json'))
    if state['status'] == 'RUNNING':
        ps = subprocess.run(['ps', '-p', str(current['pid']), '-o', 'pid=,etime=,time=,stat=,command='],
                            capture_output=True, text=True, check=True)
        assert str(native) in ps.stdout
        native_observation.update(verified_live=True, ps=ps.stdout.strip())
    native_observation['progress_lines'] = (native / 'test.log').read_text().splitlines()[-12:]
    retain(native, out / native.name)
    inspections = []
    for directory in sorted(BUILD.glob('native-cp-read-startup-inspection-*-20261025')):
        inspection = read(directory / 'result.json')
        assert inspection['read_only_guest_inspection'] and inspection['pid'] == current['pid']
        assert inspection['binary_sha256'] == sha(native / 'obj/native-cedar')
        assert inspection['header_sha256'] == sha(native / 'obj/Vtb_native_cedar___024root.h')
        assert f"Process {current['pid']} detached" in (directory / 'lldb.log').read_text()
        assert 'front_control_source_binding' in inspection
        retain(directory, out / directory.name)
        inspections.append(dict(directory=directory.name, result_sha256=sha(directory / 'result.json'),
                                counters=inspection['counters'], read_only_detached=True))
    # This startup bound reaches the first native advances, not the normal
    # CoolBoot input sequence. Retain all six memory passes and normal inputs.
    baseline_log = (baseline / 'test.log').read_text().splitlines()
    first_release = next(line for line in baseline_log
                         if line.startswith('NATIVE_PROGRESS') and 'initialized=1' in line)
    baseline_first_release = int(re.search(r'host=(\d+)', first_release)[1])
    selftest = native / 'inputs/src/fpga/mister/dorado_storage_selftest.sv'
    assert 'if(phase==5)begin ready<=1;phase<=6;end' in selftest.read_text()
    medium = BUILD / 'cedar-work-native-hints.pack'
    assert sha(medium) == 'f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
    result = dict(status='REVIEWED_ORIGINAL_NATIVE_R_CELLS_AND_PRIVATE_HOOKUP' +
                  ('_WITH_PENDING_PEERS' if pending else ''), scope=__doc__,
                  runs=runs, pending_peers=pending, complete_both_engine_replays=complete,
                  capture_source_pins=pins, focused_payload_gate_both_engines=True,
                  predicted_result_R_register_removed=True, unchanged_original_cell_state_bits=16,
                  unchanged_processor_result_bits=2470, checked_all_owner_bits=844,
                  checked_all_SRAM_bits=9216, checked_clock_counts=100, replays=replays,
                  arbitrary_binary_capture_interface_induction=True, interface_protocol_assumptions=0,
                  primitive_four_state_cases=16384, full_processor_four_state_proof=False,
                  actual_mux_mutation_corrupts_native_R=True,
                  formal_native_R_corruption_mutations=['final-clock', 'last-data', 'invert-select'],
                  final_clock_full_replay_counterexample=False,
                  final_clock_full_replay_first_only_changed=0,
                  full_replay_mutation_coverage_limitation_preserved=True,
                  private_native_source_count=194, actual_original_R_board_cells_rewired=True,
                  private_native_paired_mode_enabled=False, native_lint_UNOPTFLAT=warnings,
                  native_startup_observation=native_observation,
                  native_read_only_inspections=inspections, original_memory_selftest_passes=6,
                  baseline_first_logged_memory_release_host_tick=baseline_first_release,
                  bounded_startup_qualifies_CoolBoot_or_Cedar=False,
                  original_firmware_bytes_unchanged=True, protected_medium_unchanged=True,
                  native_all_board_event_retention_qualified=False,
                  native_paired_phase_composition_qualified=False,
                  physical15ns_or60ns_qualified=False, firmware_boot_qualified=False,
                  new_RBF_ready=False, hardware_changed=False, measured_hardware_speed_gain=False,
                  full_goal_active=True)
    (out / 'results.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


def read_gzip(path):
    return json.loads(gzip.decompress(path.read_bytes()))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--archive', type=Path, required=True)
    parser.add_argument('--require-full', action='store_true')
    parser.add_argument('--label', default='native-R-capture')
    args = parser.parse_args()
    result = review(args.output.resolve(), args.require_full)
    append_archive(args.archive.resolve(), args.output.resolve(), [], label=args.label, extra_tools=TOOLS)
    print(result['status'])
