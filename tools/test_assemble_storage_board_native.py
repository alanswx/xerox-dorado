#!/usr/bin/env python3
"""Test final assembly source/report/dependency binding without running Quartus."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import tempfile

from assemble_storage_board_native import validate

ROOT = Path(__file__).resolve().parents[1]
CASES = ('changed-source', 'changed-report', 'changed-qualification',
         'changed-reviewer', 'nonrejecting-control', 'missing-report')


def sha(b):
    return hashlib.sha256(b).hexdigest()


def run(root, qualified, controls, output):
    assert not output.exists()
    output.mkdir(parents=True)
    fixture = output / 'fixture'
    shutil.copytree(root, fixture, copy_function=os.link)
    shutil.copy2(qualified, fixture / 'preassembly-qualification.json')
    shutil.copy2(controls, fixture / 'board-fit-review-controls.json')
    c = json.loads(controls.read_text())
    for n, h in c['reviewer_dependencies'].items():
        b = (ROOT / n).read_bytes()
        assert sha(b) == h
        p = fixture / 'review-dependencies' / n
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(b)
    validate(fixture)
    before = {str(f.relative_to(root)): sha(f.read_bytes()) for f in root.rglob('*') if f.is_file()}

    def write(p, b):
        temp = p.with_name(p.name + '.mutation-tmp')
        temp.write_bytes(b)
        os.replace(temp, p)

    def changed(p, name):
        f = p / name
        write(f, f.read_bytes() + b'\n')

    cases = []
    mutations = [
        lambda p: changed(p, 'src/fpga/mister/dorado_storage_sdram_burst.sv'),
        lambda p: changed(p, 'core/native-ready2-gate-setup-7_slow_1100mv_100c.rpt'),
        lambda p: changed(p, 'preassembly-qualification.json'),
        lambda p: changed(p, 'review-dependencies/tools/review_storage_board_native.py'),
        None,
        lambda p: (p / 'core/burst-first-fault-hold-7_slow_1100mv_100c.rpt').unlink(),
    ]

    def nonrejecting(p):
        f = p / 'board-fit-review-controls.json'
        r = json.loads(f.read_text())
        r['cases'][0]['returncode'] = 0
        write(f, json.dumps(r).encode())

    mutations[4] = nonrejecting
    for name, mutate in zip(CASES, mutations):
        with tempfile.TemporaryDirectory(prefix='dorado-assembly-bad-') as temp:
            p = Path(temp) / 'receipt'
            shutil.copytree(fixture, p, copy_function=os.link)
            mutate(p)
            try:
                validate(p)
            except (AssertionError, FileNotFoundError) as error:
                cases.append(dict(name=name, rejected=True, error=type(error).__name__, detail=str(error)))
            else:
                raise AssertionError('Assembly guard accepted ' + name)
            assert not (p / 'board-assembly-state.json').exists()
            print(name, 'rejected', flush=True)
    validate(fixture)
    assert before == {str(f.relative_to(root)): sha(f.read_bytes()) for f in root.rglob('*') if f.is_file()}
    result = dict(status='PASS_NATIVE_STORAGE_BOARD_ASSEMBLY_GUARD_CONTROLS', cases=cases,
                  assembler_sha256=sha((ROOT / 'tools/assemble_storage_board_native.py').read_bytes()),
                  qualification_sha256=sha(qualified.read_bytes()),
                  reviewer_controls_sha256=sha(controls.read_bytes()),
                  original_evidence_unchanged=True, Quartus_executed=False)
    (output / 'guard-controls.json').write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(cases))


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['root', 'qualified', 'controls', 'output']:
        p.add_argument('--' + n, type=Path, required=True)
    a = p.parse_args()
    run(a.root.resolve(), a.qualified.resolve(), a.controls.resolve(), a.output.resolve())
