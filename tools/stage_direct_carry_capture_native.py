#!/usr/bin/env python3
"""Stage direct ALU carry and read-beat capture on exact reviewed690f sources.

Keep CPU registers/result adders, capture epochs, cache/Map/ECC, clocks and all
other devices. Both component proofs are required; fit/report only, no assembly.
"""
import argparse
import hashlib
import json
from pathlib import Path

from stage_storage_read_word_native import stage as capture_stage, BACKEND, WRAPPER
from test_mc10181_direct_carry import direct_carry, QUALIFIED_CELL, CELL

ROOT = Path(__file__).resolve().parents[1]


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def stage(parent, qualification, generated, miter, pins, board, direct_alu, output, project):
    assert not output.exists()
    provenance = json.loads((direct_alu / 'provenance.json').read_text())
    state = json.loads((direct_alu / 'run-state.json').read_text())
    assert state['status'] == 'PASS_MC10181_DIRECT_CARRY_ALL_BINARY_PINS_TWO_SIMULATORS'
    assert state['inputs_unchanged'] and provenance['original_cell_sha256'] == QUALIFIED_CELL
    assert provenance['binary_cases'] == 16384 and provenance['outputs'] == 7
    assert provenance['function_table_and_result_path_unchanged'] and not provenance['registers_changed']
    assert not provenance['four_state_equivalent'] and not provenance['native_qualified']
    raw = (parent / CELL).read_bytes()
    assert sha(raw) == QUALIFIED_CELL
    for n, h in provenance['inputs'].items():
        assert sha((direct_alu / 'inputs' / n).read_bytes()) == h, n
    assert (direct_alu / 'inputs/reference.v').read_bytes() == raw.replace(
        b'module cell_MC10181 (', b'module original_MC10181 (')
    assert (direct_alu / 'inputs/candidate.v').read_bytes() == direct_carry(raw)
    for engine in ['icarus', 'verilator']:
        label = engine + '-positive'
        assert state['results'][label]['passed']
        assert 'MC10181_LOOKAHEAD PASS all=16384' in (direct_alu / (label + '-test.log')).read_text()
        for name in ['wrong-select-group', 'lost-Cn', 'wrong-GG']:
            assert state['results'][engine + '-' + name]['rejected']
    action = [a for a in state['actions'] if a['label'] == 'verilator-positive-compile']
    assert len(action) == 1 and action[0]['returncode'] == 0
    assert '-Werror-UNOPTFLAT' in action[0]['command']
    assert all('returncode' in a for a in state['actions'])
    assert len(state['results']) == 8
    for f in direct_alu.glob('*compile.log'):
        assert 'UNOPTFLAT' not in f.read_text(), f

    record = capture_stage(parent, qualification, generated, miter, pins, board, output, project)
    assert set(record['modified_RTL_sources']) == {BACKEND, WRAPPER}
    (output / CELL).write_bytes(direct_carry(raw))
    record.update(status='STAGED_DIRECT_CARRY_CAPTURE_NATIVE_FIT_REVIEW_REQUIRED', scope=__doc__,
        modified_RTL_sources=sorted([BACKEND, WRAPPER, CELL]),
        source_hashes={n: sha((output / n).read_bytes()) for n in record['source_hashes']},
        ALU_direct_carry=True, ALU_function_table_result_adder_unchanged=True,
        direct_alu_provenance_sha256=sha((direct_alu / 'provenance.json').read_bytes()),
        direct_alu_state_sha256=sha((direct_alu / 'run-state.json').read_bytes()),
        four_state_ALU_equivalence_qualified=False)
    for f in direct_alu.iterdir():
        if f.is_file() and f.suffix in ['.json', '.log', '.v']:
            target = output / 'evidence/direct-carry' / f.name
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(f.read_bytes())
            record['evidence_hashes']['direct-carry/' + f.name] = sha(f.read_bytes())
    (output / 'timing-paths-native.json').write_text(json.dumps(record, indent=2) + '\n')
    for name in [Path(__file__).name, 'test_mc10181_direct_carry.py']:
        (output / name).write_bytes((ROOT / 'tools' / name).read_bytes())
    return record


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['parent', 'qualification', 'generated', 'miter', 'pins', 'board',
                 'direct-alu', 'output', 'project']:
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    result = stage(**{n: p if n == 'project' else p.resolve() for n, p in vars(args).items()})
    print(result['status'], len(result['source_hashes']), 'sources; three RTL changes; no assembly/load')
