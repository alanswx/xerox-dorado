#!/usr/bin/env python3
"""Stage the exact three-selector constant projection after complete gates.

Require the482-bit binary theorem at5/24 waves, required-selector corruption,
unchanged complete references in both engines and every-input X/Z coverage.
Only three next-state bits and their corresponding producer pins use the
equivalent constant-Hold projection; any unknown function input uses the
original result. Preserve all533 compiled inputs except this one RTL file
after path relocation, including clocks/SDCs/SRAM edges/global barriers.
Native owner-context induction and physical timing are separate later gates.
"""
import argparse
import json
from pathlib import Path
import re

from preview_hold_selectors import transform, NAME
from prove_cp_prefix_cache import module
from prove_native_cp_context import RTL
from test_r_front_forward_view import sha


def read(p):
    return json.loads(p.read_text())


def verify(directory, files):
    for name, digest in files.items():
        assert sha(directory/name) == digest, (directory, name)


def stage(parent, evidence, out, project):
    assert not out.exists() and project.is_absolute()
    m = read(parent/'r-front-control-native.json')
    assert m['frequency_hz'] == 57142857 and m['sysper'] == 4 and not m['assembly_permitted']
    assert len(m['source_hashes']) == 533 and m['cp_cache_key_projection']['compared_control_outputs'] == 475
    verify(parent, m['source_hashes'])
    proof = evidence/'selector-preview-positive-v2-20261024'
    p = read(proof/'run-state.json')
    assert p['status'] == 'PASS_SELECTOR_PREVIEW_ALL_PRODUCER_BITS'
    assert p['compared_output_bits'] == 482 and p['compared_next_state_bits'] == 156
    assert p['compared_producer_output_bits'] == 326 and p['arbitrary_binary_input_bits'] == 1253
    assert not p['protocol_or_state_assumptions'] and not p['extra_saved_state_or_guest_cycles']
    assert [(r['waves'], r['passed']) for r in p['results']] == [(5, True), (24, True)]
    verify(proof, p['files'])
    negative = evidence/'selector-preview-negative-v2-20261024'
    n = read(negative/'run-state.json')
    assert n['status'] == 'PASS_SELECTOR_PREVIEW_COUNTEREXAMPLES'
    assert [(r['waves'], r['passed']) for r in n['results']] == [(5, True), (24, True)]
    verify(negative, n['files'])
    original = (parent/RTL).read_text()
    proved = (proof/'original.sv').read_text()
    for name in ['dorado_control_hold_atomic_step', 'dorado_control_terminal_body', 'dorado_control_hold_ordered_step']:
        assert module(original, name) == module(proved, name)
    function_pattern = r' function automatic \[325:0\] producer_pins\(.*? endfunction'
    assert re.search(function_pattern, original, re.S)[0] == re.search(function_pattern, proved, re.S)[0]
    actual = transform(original)
    assert module(actual, NAME) == module((proof/'candidate.sv').read_text(), NAME)
    tests = []
    for engine in ['icarus', 'verilator']:
        d = evidence/f'selector-{engine}-v2-20261024'
        s = read(d/'run-state.json')
        assert s['status'] == 'PASS_SELECTOR_COMPLETE_ORIGINAL_COMPARISON'
        verify(d, s['files'])
        assert s['all100_clock_counts_match'] and s['original_testbench_and_references_unchanged']
        assert s['proof_state_sha256'] == sha(proof/'run-state.json')
        assert module((d/'candidate.sv').read_text(), NAME) == module(actual, NAME)
        tests.append(dict(directory=str(d), state_sha256=sha(d/'run-state.json'), cache_coverage=s['cache_coverage']))
    for variant, status in [('four-state', 'PASS_SELECTOR_PREVIEW_FOUR_STATE'),
                            ('bad-unknown', 'PASS_SELECTOR_UNKNOWN_FALLBACK_COUNTEREXAMPLE')]:
        d = evidence/f'selector-{variant}-v2-20261024'
        s = read(d/'run-state.json')
        assert s['status'] == status
        verify(d, s['files'])
        assert s['proof_state_sha256'] == sha(proof/'run-state.json')
    out.mkdir(parents=True)
    for name in m['source_hashes']:
        data = actual.encode() if name == RTL else (parent/name).read_bytes()
        if Path(name).suffix in ['.qsf', '.qpf', '.qip', '.tcl', '.sdc']:
            data = data.replace(m['project'].encode(), str(project).encode())
        target = out/name
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(data)
    changes = [name for name in m['source_hashes'] if (out/name).read_bytes().replace(
        str(project).encode(), m['project'].encode()) != (parent/name).read_bytes()]
    assert changes == [RTL]
    record = {**m, 'project': str(project), 'scope': __doc__, 'parent_stage': str(parent),
        'parent_manifest_sha256': sha(parent/'r-front-control-native.json'),
        'hold_selector_projection': dict(late_input_removed=386, selector_Q_bits=[118,121,120],
            producer_pins=[11,10,9], compared_output_bits=482, arbitrary_binary_inputs=1253,
            wave_modes_proven=[5,24], proof=str(proof), proof_state_sha256=sha(proof/'run-state.json'),
            required_selector_counterexample=str(negative), full_original_comparisons=tests,
            four_state_producer_checks=5140, unknown_checks=5012, fallback_contract_checks=32,
            unknown_function_input_fallback='exact original selector for any of1247 unknown function inputs',
            all_other_state_events_flags_byte_unchanged=True, extra_saved_state_or_guest_cycles=0,
            SRAM_ports_clocks_and_capture_unchanged=True),
        'source_hashes': {name: sha(out/name) for name in m['source_hashes']},
        'physical_timing_qualified': False, 'firmware_qualified': False, 'speedup_qualified': False,
        'assembly_permitted': False, 'hardware_changed': False}
    (out/'r-front-control-native.json').write_text(json.dumps(record, indent=2)+'\n')
    for name in [Path(__file__).name, 'preview_hold_selectors.py']:
        (out/name).write_bytes((Path(__file__).parent/name).read_bytes())
    print('STAGED_PROVED_HOLD_SELECTOR_PROJECTION', len(m['source_hashes']), 'sources', flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'evidence', 'output', 'project']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    stage(a.parent.resolve(), a.evidence.resolve(), a.output.resolve(), a.project)
