#!/usr/bin/env python3
"""Damage actual bound inputs and require rejection before Quartus execution."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import tempfile

from assemble_pack_window_native import validate, GUARD_CASES

ROOT = Path(__file__).resolve().parents[1]


def sha(b):
    return hashlib.sha256(b).hexdigest()


def run(root, qualified, controls, output):
    output.mkdir(parents=True, exist_ok=False)
    fixture = output / 'fixture'
    shutil.copytree(root, fixture, copy_function=os.link)
    shutil.copy2(qualified, fixture / 'pack-preassembly-qualification.json')
    shutil.copy2(controls, fixture / 'pack-fit-review-controls.json')
    c = json.loads(controls.read_text())
    for n, h in c['reviewer_dependencies'].items():
        b = (ROOT / n).read_bytes();assert sha(b) == h
        f = fixture / 'review-dependencies' / n;f.parent.mkdir(parents=True, exist_ok=True);f.write_bytes(b)
    validate(fixture)
    before = {str(f.relative_to(root)): sha(f.read_bytes()) for f in root.rglob('*') if f.is_file()}

    def write(p, b):
        temp = p.with_name(p.name + '.mutation-tmp');temp.write_bytes(b);os.replace(temp, p)

    def changed(p, n):
        f = p / n;write(f, f.read_bytes() + b'\n')

    def nonrejecting(p):
        f = p / 'pack-fit-review-controls.json';r = json.loads(f.read_bytes())
        r['cases'][0]['returncode'] = 0;write(f, json.dumps(r).encode())

    mutations = {
        'changed-source': lambda p: changed(p, 'src/fpga/mister/dorado_storage_group.sv'),
        'changed-report': lambda p: changed(p, 'core/storage-sample-input-setup-7_slow_1100mv_100c.rpt'),
        'changed-qualification': lambda p: changed(p, 'pack-preassembly-qualification.json'),
        'changed-reviewer': lambda p: changed(p, 'review-dependencies/tools/review_pack_window_native.py'),
        'nonrejecting-control': nonrejecting,
        'missing-phase-report': lambda p: (p / 'core/storage-sample-output-hold-7_slow_1100mv_100c.rpt').unlink(),
        'changed-window-manifest': lambda p: changed(p, 'pack-window-native.json'),
    }
    assert set(mutations) == GUARD_CASES
    cases = []
    for name, mutate in mutations.items():
        with tempfile.TemporaryDirectory(prefix='dorado-pack-assembly-bad-') as d:
            p = Path(d) / 'receipt';shutil.copytree(fixture, p, copy_function=os.link);mutate(p)
            try:
                validate(p)
            except (AssertionError, FileNotFoundError) as e:
                cases.append(dict(name=name, rejected=True, error=type(e).__name__, detail=str(e)))
            else:
                raise AssertionError('Assembly guard accepted ' + name)
            assert not (p / 'pack-window-assembly-state.json').exists()
            print(name, 'rejected', flush=True)
    validate(fixture)
    assert before == {str(f.relative_to(root)): sha(f.read_bytes()) for f in root.rglob('*') if f.is_file()}
    r = dict(status='PASS_NATIVE_PACK_WINDOW_ASSEMBLY_GUARD_CONTROLS', cases=cases,
             assembler_sha256=sha((ROOT / 'tools/assemble_pack_window_native.py').read_bytes()),
             qualification_sha256=sha(qualified.read_bytes()), reviewer_controls_sha256=sha(controls.read_bytes()),
             original_evidence_unchanged=True, Quartus_executed=False)
    (output / 'guard-controls.json').write_text(json.dumps(r, indent=2) + '\n')
    print(r['status'], len(cases))


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for n in ['root', 'qualified', 'controls', 'output']:
        p.add_argument('--' + n, type=Path, required=True)
    a = p.parse_args()
    run(a.root.resolve(), a.qualified.resolve(), a.controls.resolve(), a.output.resolve())
