#!/usr/bin/env python3
"""Run unchanged full-board restore fixtures with only the F10145 write fold.

Eight affected boards retain byte-identical generated wiring and other helpers.
Original cells come from the frozen Native59 source receipt. This is positive
board integration qualification, not another run of each older broken-board
suite, global snapshot coherence, whole-core timing or a hardware boot.
"""
import argparse
import copy
import hashlib
import importlib
import json
from pathlib import Path
import generate_f10145_shared_write as shared

HERE = Path(__file__).resolve().parent
CELL = 'cell_F10145A_snapshot.sv'
BOARDS = ('ContA', 'ProcH', 'ProcL', 'MemC', 'MemD', 'MemX', 'DskEth', 'DispY')


def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


class Generator:
    def __init__(self, module, root, board):
        self.module, self.root, self.board = module, root, board

    def __getattr__(self, name):
        return self.root if name == 'ROOT' else getattr(self.module, name)

    def generate(self, *args, **kwargs):
        board, manifest, clones = self.module.generate(*args, **kwargs)
        original = list((self.root/'fpga/mister').glob('snapshot_native*/'+self.board+'_full_snapshot.sv'))
        assert len(original) == 1 and board.encode() == original[0].read_bytes(), 'generated board differs: '+self.board
        text, cell = shared.generate()
        assert hashlib.sha256(clones[CELL].encode()).hexdigest() == cell['qualified_predecessor_sha256']
        for name, expected in manifest['original_sources'].items():
            path = name if '/' in name else 'verilog/cells/'+name+'.v'
            assert digest(self.root/path) == expected, 'actual original cell differs: '+path
        manifest = copy.deepcopy(manifest)
        clones[CELL] = text
        manifest['cloned_sources'][CELL] = cell['generated_sha256']
        manifest['shared_f10145_experiment'] = dict(reference_cell_sha256=cell['qualified_predecessor_sha256'],
            replacement_cell_sha256=cell['generated_sha256'], board_source_unchanged=True, scope=__doc__)
        return board, manifest, clones


def run(root, out, boards):
    out.mkdir(parents=True, exist_ok=False)
    inputs = out/'executed-driver'
    inputs.mkdir()
    paths = [Path(__file__).resolve(), HERE/'generate_f10145_shared_write.py', HERE/'generate_f10145.py']
    for name in ('conta', 'processor', 'memc', 'memd', 'memx', 'dsketh', 'dispy'):
        paths += [HERE/('test_'+name+'_full.py'), HERE/('test_'+name+'_full.cpp'), HERE/('generate_'+name+'_full.py')]
    hashes = {p.name: digest(p) for p in paths}
    for path in paths:
        (inputs/path.name).write_bytes(path.read_bytes())
    (out/'driver-provenance.json').write_text(json.dumps(dict(scope=__doc__, frozen_root=str(root),
        boards=list(boards), source_hashes=hashes), indent=2)+'\n')
    for board in boards:
        group = ('processor' if board in ('ProcH', 'ProcL') else board.lower())
        module = importlib.import_module('test_'+group+'_full')
        module.gen = Generator(module.gen, root, board)
        # The two processor boards share the same test module; unwrap an older
        # fixture-local selector rather than composing transformations.
        if isinstance(module.gen.module, Generator):
            module.gen.module = module.gen.module.module
        target = out/board
        print('Starting complete '+board+' with the shared F10145 writer', flush=True)
        if board == 'ContA':
            module.run(root/'build/rtl-executive/native16-tx-cable/ContA_monitor.v', target, False)
        elif board in ('ProcH', 'ProcL', 'MemC'):
            filename = {'ProcH': 'ProcH-Rev-Ce.v', 'ProcL': 'ProcL-Rev-Ci.v', 'MemC': 'MemC-Rev-Be.v'}[board]
            module.run(board, root/'verilog/generated'/filename, target, False)
        else:
            module.run(root, target, False)
        for p in paths:
            assert digest(p) == hashes[p.name], 'fixture source changed: '+p.name
    (out/'results.json').write_text(json.dumps(dict(status='PASS', boards=list(boards),
        scope=__doc__, source_hashes=hashes), indent=2)+'\n')
    print('SHARED F10145 COMPLETE BOARD GATES PASS: '+', '.join(boards), flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--frozen-root', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--board', action='append', choices=BOARDS)
    a = p.parse_args()
    run(a.frozen_root.resolve(), a.output.resolve(), a.board or BOARDS)
