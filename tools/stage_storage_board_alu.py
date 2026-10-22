#!/usr/bin/env python3
"""Stage a 66.7 MHz timing experiment with equivalent MC10181 carry logic.

Change only the combinational cell from the frozen board-ready native stage.
Keep every CPU register, capture epoch, clock, timing constraint, cache, Map,
ECC and peripheral source. Binary-pin equivalence is not timing or firmware
qualification; this experiment must pass a fresh full-device timing review.
"""
import argparse
import hashlib
import json
from pathlib import Path
from test_mc10181_lookahead import CELL, lookahead

ROOT = Path(__file__).resolve().parents[1]


def sha(b):
    return hashlib.sha256(b).hexdigest()


def stage(parent, proof, output, project):
    assert not output.exists() and project.is_absolute()
    r = json.loads((parent / 'storage-board-native.json').read_text())
    assert r['frequency_hz'] == 66666667 and r['sysper'] == 4
    assert r['state_admin'] == 'INACTIVE'
    p = json.loads((proof / 'provenance.json').read_text())
    s = json.loads((proof / 'run-state.json').read_text())
    assert s['status'] == 'PASS_MC10181_LOOKAHEAD_ALL_BINARY_PINS_TWO_SIMULATORS'
    assert len(s['actions']) == 12 and all('returncode' in a for a in s['actions'])
    for n, h in p['inputs'].items():
        assert sha((proof / 'inputs' / n).read_bytes()) == h, n
    files = {}
    for n, h in r['source_hashes'].items():
        b = (parent / n).read_bytes()
        assert sha(b) == h, n
        if Path(n).suffix in ('.qsf', '.qip', '.qpf', '.sdc', '.tcl'):
            b = b.replace(r['project'].encode(), str(project).encode())
        files[n] = b
    assert sha(files[CELL]) == p['original_cell_sha256']
    candidate = lookahead(files[CELL])
    assert candidate == (proof / 'inputs/candidate.v').read_bytes()
    files[CELL] = candidate
    assert len(files) == 503
    output.mkdir(parents=True)
    for n, b in files.items():
        f = output / n
        f.parent.mkdir(parents=True, exist_ok=True)
        f.write_bytes(b)
    r.update(status='STAGED_BOARD_READY_ALU_LOOKAHEAD_NATIVE_REVIEW_REQUIRED',
             scope=__doc__, project=str(project),
             source_hashes={n: sha(b) for n, b in files.items()},
             immediate_parent_manifest_sha256=sha((parent / 'storage-board-native.json').read_bytes()),
             ALU_pin_proof_sha256=sha((proof / 'provenance.json').read_bytes()),
             CPU_cache_map_ECC_peripheral_logic_unchanged=False,
             CPU_registers_and_capture_epochs_unchanged=True,
             cache_map_ECC_peripheral_logic_unchanged=True,
             combinational_ALU_binary_pin_equivalence=True,
             functional_source_delta=[CELL],
             assembly_permitted=False, firmware_qualified=False,
             hardware_speedup_qualified=False)
    (output / 'storage-board-native.json').write_text(json.dumps(r, indent=2) + '\n')
    for n in [Path(__file__).name, 'build_r_front_ready_native.py']:
        (output / n).write_bytes((ROOT / 'tools' / n).read_bytes())
    print(r['status'], len(files), 'sources')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['parent', 'proof', 'output', 'project']:
        p.add_argument('--' + n, type=Path, required=True)
    a = p.parse_args()
    stage(a.parent.resolve(), a.proof.resolve(), a.output.resolve(), a.project)
