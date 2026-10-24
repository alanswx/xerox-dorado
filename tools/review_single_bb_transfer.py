#!/usr/bin/env python3
"""Retain full single-transfer consumer equivalence, references and timing.

Every intermediate state/edge and all2568 source/consumer output bits remain
in scope. Source geometry is separately proved; no new guest cycles or owner
state are added. Component timing never proves native firmware or speed.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

from fit_baseboard_clock_consumer import TOP
from prove_primary_clock_events import extract
from review_baseboard_clock_consumer import CORNERS
from review_exact_pixel_phase import retain_text
from review_late_hold_and_input_reset import first_path
from review_r_front_capture_followup import append_archive
from test_r_front_forward_view import sha
from test_baseboard_clock_consumer import MUTATIONS

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT/'build/rtl-executive'


def read(path):
    return json.loads(path.read_text())


def verify(directory, files):
    for name, digest in files.items():
        assert sha(directory/name) == digest, (directory, name)


def proof(directory, status, count, primary=False):
    state = read(directory/'run-state.json')
    assert state['status'] == status and len(state['cases']) == count
    for c in state['cases']:
        assert c['passed'] and c['returncode'] == 0
        d = directory/(c['mode']+'-'+c['variant'] if primary else c['variant'])
        verify(d, c['files'])
        assert sha(d/'proof.log') == c['log_sha256']
        if primary:
            actual, reference = extract((d/'source.sv').read_text(), c['mode'])
            assert (d/'actual.sv').read_text() == actual
            if reference:
                assert (d/'reference.sv').read_text() == reference
    return state


def references(directory, status, marker):
    state = read(directory/'run-state.json')
    assert state['status'] == status and len(state['cases']) == 12
    verify(directory/'inputs', state['inputs'])
    for c in state['cases']:
        assert c['passed'] and c['actions'][0]['returncode'] == 0
        assert (c['actions'][-1]['returncode'] != 0) == c['expected_rejection']
        p = directory/(c['engine']+'-'+c['variant'])/'test.log'
        assert sha(p) == c.get('log_sha256', c.get('test_log_sha256'))
        if c['variant'] == 'positive':
            assert marker in p.read_text()
    return state


def review(out, fit):
    assert not out.exists()
    stage = BUILD/'single-bb-transfer-stage-v1-20261024'
    s = read(stage/'results.json')
    assert s['status'] == 'PREPARED_SINGLE_BB_TRANSFER_CANDIDATE'
    verify(stage, s['files'])
    assert s['extra_guest_cycles'] == 0 and not s['native_consumers_integrated']
    original = BUILD/'baseboard-clock-consumer-v4-20261024/inputs'
    for name in ['dorado_baseboard_clock_step.sv', 'dorado_primary_clock_events.sv']:
        assert sha(stage/('original-'+name)) == sha(original/name)
    source_tests = BUILD/'single-bb-primary-reference-v1-20261024'
    t = references(source_tests, 'PASS_ALL_PRIMARY_CLOCK_ORIGINAL_REFERENCES',
                   'samples=160576 joint_period_ns=300000 maximum=5 resets=4')
    for name in ['tb.sv', 'expected.hex', 'dorado_clock_event_batch.sv', 'dorado_oscillator_events.sv']:
        assert sha(source_tests/'inputs'/name) == sha(BUILD/'exact-bb-primary-reference-v1-20261024/inputs'/name)
    assert t['inputs']['dorado_primary_clock_events.sv'] == sha(stage/'dorado_primary_clock_events.sv')
    source_proof = BUILD/'single-bb-primary-proof-v1-20261024'
    p = proof(source_proof, 'PASS_PRIMARY_CLOCK_MERGE_AND_PHASE_PROOFS', 7, True)
    assert p['tests_state_sha256'] == sha(source_tests/'run-state.json')
    assert p['merged_output_bits'] == 195 and p['at_most_one_BB_transition_per_interval_proven']
    assert p['independent_BB_and_Ether_endpoints_proven'] and not p['reset_advance_protocol_assumptions']
    assert sha(source_proof/'merge-positive/source.sv') == t['inputs']['dorado_primary_clock_events.sv']
    consumer_proof = BUILD/'single-bb-transfer-proof-v1-20261024'
    c = proof(consumer_proof, 'PASS_SINGLE_BB_TRANSFER_FULL_CONSUMER_EQUIVALENCE', 3)
    assert c['compared_bits'] == 126 and c['physical_Q_bits'] == 6 and c['debug_inputs_unconstrained']
    assert c['stage_state_sha256'] == sha(stage/'results.json')
    assert sha(consumer_proof/'positive/actual.sv') == sha(stage/'dorado_baseboard_clock_step.sv')
    reference_original = (consumer_proof/'positive/original.sv').read_text().replace(
        'module original_baseboard_clock_step(', 'module dorado_baseboard_clock_step(', 1)
    assert reference_original == (original/'dorado_baseboard_clock_step.sv').read_text()
    consumer_tests = BUILD/'single-bb-consumer-reference-v1-20261024'
    r = references(consumer_tests, 'PASS_BASEBOARD_CLOCK_EVENT_CONSUMER_REFERENCES',
                   'samples=176384 steps=873792 holds=12648 restores=520')
    assert r['schedule_state_sha256'] == sha(source_tests/'run-state.json')
    for name in ['dorado_baseboard_clock_step.sv', 'dorado_primary_clock_events.sv']:
        assert r['inputs'][name] == sha(stage/name)
    for name in ['baseboard_clock_original.sv', 'cell_MC10210.v', 'cell_SN74LS04.v', 'expected.hex', 'BaseBd.wl', 'BaseBd_nominal.v']:
        assert sha(consumer_tests/'inputs'/name) == sha(original/name)
    bench = (consumer_tests/'inputs/tb_baseboard_clock_events.sv').read_text().replace(
        'consumer(owners[p],before_sources[p],after_sources,counts[p]',
        'consumer(owners[p],before_sources[p],counts[p]', 1)
    assert bench == (original/'tb_baseboard_clock_events.sv').read_text()
    fixture = BUILD/'single-bb-consumer-component-stage-v1-20261024'
    f = read(fixture/'provenance.json')
    verify(fixture, f['files'])
    assert f['clock_period_ns'] == 15 and f['result_bits'] == 2568 and f['CPU_rotations'] == 8
    assert f['tests_state_sha256'] == sha(consumer_tests/'run-state.json')
    assert f['full_output_equivalence']['consumer_state_sha256'] == sha(consumer_proof/'run-state.json')
    assert f['full_output_equivalence']['source_state_sha256'] == sha(source_proof/'run-state.json')
    for name in ['dorado_baseboard_clock_step.sv', 'dorado_primary_clock_events.sv',
                 'dorado_clock_event_batch.sv', 'dorado_oscillator_events.sv']:
        assert sha(fixture/name) == r['inputs'][name]
    assert (fixture/'top.sv').read_text() == TOP.replace('consumer(launch[7:2],before_levels,count',
        'consumer(launch[7:2],before_levels,after_levels,count', 1)
    parent = BUILD/'baseboard-clock-consumer-component-stage-v2-20261024'
    for name in ['clock.sdc', 'clock_fit.qsf', 'audit.tcl', 'fit_control_clock.py']:
        assert sha(fixture/name) == sha(parent/name)
    envelope = Path(str(fit)+'.tgz.json')
    receipt = read(envelope)
    assert sha(Path(str(fit)+'.tgz')) == receipt['archive_sha256']
    for row in receipt['members']:
        assert sha(fit/row['path']) == row['sha256'] and (fit/row['path']).stat().st_size == row['bytes']
    fit_state = read(fit/'run-state.json')
    assert fit_state['status'] == 'TERMINAL_REVIEW_REQUIRED'
    assert all(row['returncode'] == 0 for row in fit_state['stages'])
    readback = read(fit/'collected-input-check.json')
    assert readback['job_pids_absent'] == [fit_state['builder_pid']]+[row['pid'] for row in fit_state['stages']]
    assert readback['changed_inputs'] == ['clock_fit.qsf']
    assert read(fit/'provenance.json') == f
    for name, digest in f['files'].items():
        raw = (fit/name).read_bytes()
        assert sha(fit/name) == readback['actual'][name]
        if name == 'clock_fit.qsf':
            annotation = b'\nset_global_assignment -name LAST_QUARTUS_VERSION "17.0.2 Lite Edition"'
            assert raw.endswith(annotation) and raw.count(annotation) == 1
            raw = raw[:-len(annotation)]
        assert hashlib.sha256(raw).hexdigest() == digest
    log = (fit/'corners.log').read_text()
    assert re.findall(r'BB_CONSUMER_OUTPUT_BITS (\d+)', log) == ['2568']*4
    corners = []
    for corner in CORNERS:
        row = dict(condition=corner)
        for mode in ['setup', 'hold']:
            row[mode] = first_path(fit/('clock-'+mode+'-'+corner+'.rpt'))
        ucp = (fit/('clock-ucp-'+corner+'.rpt')).read_text()
        counts = re.findall(r'; (?:Illegal Clocks|Unconstrained [^;]+)\s*;\s*(\d+)\s*;\s*(\d*)\s*;', ucp)
        assert len(counts) == 6 and all(a == '0' and b in ['', '0'] for a, b in counts)
        corners.append(row)
    timing_pass = all(row[mode]['slack_ns'] >= 0 for row in corners for mode in ['setup', 'hold'])
    out.mkdir(parents=True)
    for d in [stage, source_tests, source_proof, consumer_proof, consumer_tests, fixture, fit]:
        retain_text(d, out/d.name)
    (out/envelope.name).write_bytes(envelope.read_bytes())
    result = dict(status='REVIEWED_SINGLE_BB_TRANSFER_CORRECTNESS_AND_COMPONENT_TIMING',
        consumer_compared_bits=126, source_compared_bits=195, physical_Q_bits=6,
        consumer_intervals_per_engine=176384, consumer_steps_per_engine=873792,
        original_package_reference_unchanged=True, all_original_checks_retained=True,
        single_BB_geometry_independently_proven=True, extra_guest_cycles=0,
        component_output_bits=2568, CPU_rotations=8, corners=corners,
        component_15ns_setup_and_hold_pass=timing_pass, complete_collection_members=len(receipt['members']),
        native_consumers_integrated=False, native_timing_qualified=False,
        analog_capture_qualified=False, firmware_qualified=False, speedup_qualified=False,
        hardware_changed=False, hardware_requires_user_release=True, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


def selection(out):
    """Bind the selected canonical files/default fixture to the qualified run."""
    assert not out.exists()
    reviewed = BUILD/'single-bb-transfer-review-v1-20261024'
    result = read(reviewed/'results.json')
    assert result['component_15ns_setup_and_hold_pass'] and result['all_original_checks_retained']
    tested = BUILD/'single-bb-consumer-reference-v1-20261024/inputs'
    fitted = BUILD/'single-bb-consumer-component-stage-v1-20261024'
    staged = BUILD/'single-bb-canonical-component-stage-v1-20261024'
    verify(staged, read(staged/'provenance.json')['files'])
    bindings = {}
    for name in ['dorado_primary_clock_events.sv', 'dorado_baseboard_clock_step.sv',
                 'dorado_oscillator_events.sv', 'dorado_clock_event_batch.sv']:
        path = ROOT/'verilog/rtl'/name
        assert path.read_bytes() == (tested/name).read_bytes() == (fitted/name).read_bytes() == (staged/name).read_bytes()
        bindings[str(path.relative_to(ROOT))] = sha(path)
    path = ROOT/'verilog/verilator/tb_baseboard_clock_events.sv'
    assert path.read_bytes() == (tested/path.name).read_bytes()
    bindings[str(path.relative_to(ROOT))] = sha(path)
    for name in ['top.sv', 'clock.sdc', 'clock_fit.qsf', 'clock_fit.qpf', 'audit.tcl', 'fit_control_clock.py']:
        assert (staged/name).read_bytes() == (fitted/name).read_bytes()
    assert TOP == (fitted/'top.sv').read_text()
    mutations = dict(MUTATIONS)
    mutations['drop-boundary'] = [('events_at_steps[7*index+:7]=selected_events;',
        "events_at_steps[7*index+:7]=(index==0) ? 7'b0 : selected_events;")]
    raw = (ROOT/'verilog/rtl/dorado_baseboard_clock_step.sv').read_text()
    for name, replacements in mutations.items():
        changed = raw
        for before, after in replacements:
            assert changed.count(before) == 1
            changed = changed.replace(before, after, 1)
        assert changed == (tested/(name+'.sv')).read_text()
    out.mkdir(parents=True)
    for name in bindings:
        target = out/name
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes((ROOT/name).read_bytes())
    retain_text(staged, out/staged.name)
    result = dict(status='SELECTED_QUALIFIED_SINGLE_BB_TRANSFER_SOURCES', canonical_files=bindings,
        review_state_sha256=sha(reviewed/'results.json'),
        canonical_RTL_and_bench_byte_identical_to_qualified_inputs=True,
        canonical_fixture_RTL_top_and_constraints_byte_identical_to_qualified_fit=True,
        all_five_regenerated_corruptions_byte_identical_to_qualified_tests=True,
        native_consumers_integrated=False, hardware_changed=False, new_speed_qualified=False,
        hardware_requires_user_release=True, full_goal_active=True)
    (out/'results.json').write_text(json.dumps(result, indent=2)+'\n')
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['output', 'archive']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--fit', type=Path)
    p.add_argument('--selection', action='store_true')
    a = p.parse_args()
    assert a.selection != (a.fit is not None), 'Choose either a collected fit or canonical selection'
    result = selection(a.output.resolve()) if a.selection else review(a.output.resolve(), a.fit.resolve())
    append_archive(a.archive.resolve(), a.output.resolve(), [], label='single-bb-selection' if a.selection else 'single-bb-transfer', extra_tools=[
        'prepare_baseboard_clock_parallel.py', 'prove_baseboard_clock_parallel.py',
        'test_baseboard_clock_consumer.py', 'fit_baseboard_clock_consumer.py',
        'test_primary_clock_events.py', 'prove_primary_clock_events.py',
        'review_baseboard_clock_consumer.py', Path(__file__).name])
    print(json.dumps(result), flush=True)
