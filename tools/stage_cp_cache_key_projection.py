#!/usr/bin/env python3
"""Stage the proved prefix-key projection without changing native timing phases.

Require full475-output temporal safety, required-bit and unknown-fallback
counterexamples, complete original references in both engines and10,208
four-state/binary comparison checks. Change only the cache comparison after
path relocation; preserve owners, SRAM windows, clocks, SDCs and barrier.
Exact generic/owned context proof is required separately before native boot.
No assembly or deployment is permitted by this staging step.
"""
import argparse
import json
from pathlib import Path

from prove_cp_cache_key_projection import projected
from prove_cp_prefix_cache import module
from prove_native_cp_context import RTL, MATCH
from test_cp_cache_key_projection import lower_initializers
from test_r_front_forward_view import sha


def read(p):
    return json.loads(p.read_text())


def verify(directory, mapping):
    for name, digest in mapping.items():
        assert sha(directory/name) == digest, (directory, name)


def stage(parent, evidence, out, project):
    assert not out.exists() and project.is_absolute()
    m = read(parent/'r-front-control-native.json')
    assert m['frequency_hz'] == 57142857 and m['sysper'] == 4
    assert not m['assembly_permitted'] and 'cache_tag_lcell_buffers' not in m
    verify(parent, m['source_hashes'])
    raw = (parent/RTL).read_text()
    proof = evidence/'cp-cache-key-projection-positive-v1-20261024'
    p = read(proof/'run-state.json')
    assert p['status'] == 'PASS_CP_CACHE_KEY_INDUCTIVE_SAFETY' and p['unbounded_safety_proven']
    assert p['compared_control_output_bits'] == 475 and p['compared_prefix_bits'] == 269
    assert p['input_constraints'] == 'none; arbitrary inputs and reset histories'
    verify(proof, p['source_hashes'])
    assert sha(proof/'proof.log') == p['log_sha256']
    n = evidence/'cp-cache-key-projection-negative-v1-20261024'
    negative = read(n/'run-state.json')
    assert negative['status'] == 'PASS_CP_CACHE_KEY_NEGATIVE_CONTROL'
    verify(n, negative['source_hashes'])
    assert sha(n/'proof.log') == negative['log_sha256']
    proved = (proof/'actual-source.sv').read_text()
    for name in ['dorado_control_hold_atomic_step', 'dorado_control_terminal_body', 'dorado_control_hold_ordered_step']:
        assert module(raw, name) == module(proved, name), name
    generic_cache = module(proved, 'dorado_cp_prefix_cache')
    old_cache = module(raw, 'dorado_cp_prefix_cache')
    owned = m.get('owned_context_specialization_requested', False)
    expected = generic_cache.replace(MATCH, " wire canonical_match=1'b1;", 1) if owned else generic_cache
    assert old_cache == expected
    care = set(p['cared_key_bits'])
    assert len(care) == 269 and 138 not in care and all(i in care for i in range(1, 12))
    new_cache = projected(old_cache, care)
    actual = raw.replace(old_cache, new_cache, 1)
    assert actual.replace(new_cache, old_cache, 1) == raw
    comparisons = []
    for engine in ['icarus', 'verilator']:
        d = evidence/f'cp-key-{engine}-v1-20261024'
        s = read(d/'run-state.json')
        assert s['status'] == 'PASS_CP_KEY_COMPLETE_ORIGINAL_COMPARISON'
        verify(d, s['files'])
        assert s['all100_clock_counts_match'] and s['original_testbench_and_references_unchanged']
        assert lower_initializers(module((d/'candidate.sv').read_text(), 'dorado_cp_prefix_cache')) == (proof/'candidate-cache.sv').read_text()
        assert s['proof_state_sha256'] == sha(proof/'run-state.json')
        comparisons.append(dict(directory=str(d), state_sha256=sha(d/'run-state.json'), cache_coverage=s['cache_coverage']))
    for variant, status in [('four-state', 'PASS_CP_KEY_FOUR_STATE_COMPARISON'),
                            ('bad-unknown', 'PASS_CP_KEY_UNKNOWN_FALLBACK_COUNTEREXAMPLE')]:
        d = evidence/f'cp-key-{variant}-v1-20261024'
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
        'cp_cache_key_projection': dict(compared_control_outputs=475, complete_prefix_outputs=269,
            logical_key_bits=567, cared_key_bits=sorted(care), binary_key_assumptions='none',
            proof=str(proof), proof_state_sha256=sha(proof/'run-state.json'),
            required_bit_counterexample=str(n), full_reference_comparisons=comparisons,
            comparison_checks=10208, unknown_checks=9072, all_saved_CP_bits_compared=True,
            unknown_key_behavior='exact original full-key equality',
            extra_saved_state_or_guest_cycles=0, SRAM_ports_clocks_and_capture_unchanged=True),
        'source_hashes': {name: sha(out/name) for name in m['source_hashes']},
        'physical_timing_qualified': False, 'firmware_qualified': False, 'speedup_qualified': False,
        'assembly_permitted': False, 'hardware_changed': False}
    (out/'r-front-control-native.json').write_text(json.dumps(record, indent=2)+'\n')
    for name in [Path(__file__).name, 'prove_cp_cache_key_projection.py']:
        (out/name).write_bytes((Path(__file__).parent/name).read_bytes())
    print('STAGED_PROVED_CP_KEY_PROJECTION', len(m['source_hashes']), 'sources', flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'evidence', 'output', 'project']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    stage(a.parent.resolve(), a.evidence.resolve(), a.output.resolve(), a.project)
