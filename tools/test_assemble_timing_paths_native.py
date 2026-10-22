#!/usr/bin/env python3
"""Reject changed bound native evidence before any Quartus assembler call."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import tempfile

from assemble_timing_paths_native import validate, GUARD_CASES

ROOT = Path(__file__).resolve().parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(root, qualified, controls, out):
    assert not out.exists()
    out.mkdir(parents=True)
    fixture = out / 'fixture'
    shutil.copytree(root, fixture, copy_function=os.link)
    shutil.copyfile(qualified, fixture / 'timing-preassembly-qualification.json')
    shutil.copyfile(controls, fixture / 'timing-fit-review-controls.json')
    c = json.loads(controls.read_text())
    for name, digest in c['reviewer_dependencies'].items():
        source = ROOT / name
        assert sha(source) == digest
        target = fixture / 'review-dependencies' / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)
    validate(fixture)

    def hashes(directory):
        return {str(p.relative_to(directory)): sha(p) for p in directory.rglob('*') if p.is_file()}

    before = hashes(root)

    def write(path, raw):
        temporary = path.with_name(path.name + '.mutation')
        temporary.write_bytes(raw)
        os.replace(temporary, path)

    def changed(directory, name):
        path = directory / name
        write(path, path.read_bytes() + b'\n')

    def nonrejecting(directory):
        path = directory / 'timing-fit-review-controls.json'
        value = json.loads(path.read_text())
        value['cases'][0]['returncode'] = 0
        write(path, json.dumps(value).encode())

    corner = '7_slow_1100mv_100c'
    mutations = {
        'changed-source': lambda p: changed(p, 'src/fpga/mister/dorado_storage_arb.sv'),
        'changed-report': lambda p: changed(p, f'core/storage-sample-input-setup-{corner}.rpt'),
        'changed-qualification': lambda p: changed(p, 'timing-preassembly-qualification.json'),
        'changed-reviewer': lambda p: changed(p, 'review-dependencies/tools/review_timing_paths_native.py'),
        'nonrejecting-control': nonrejecting,
        'missing-response-report': lambda p: (p / f'storage-ready-reports-v1/response-backend-setup-{corner}.rpt').unlink(),
        'changed-manifest': lambda p: changed(p, 'timing-paths-native.json'),
        'changed-counter-report': lambda p: changed(p, f'storage-ready-reports-v1/cache-hit-event-setup-{corner}.rpt'),
        'changed-lookahead-inventory': lambda p: changed(p, 'storage-ready-reports-v1/request-lookahead.log'),
        'changed-crystal-report': lambda p: changed(p, f'storage-ready-reports-v1/crystal-baseboard-hold-{corner}.rpt'),
    }
    assert set(mutations) == GUARD_CASES
    cases = []
    for name, mutate in mutations.items():
        with tempfile.TemporaryDirectory(prefix='dorado-response-assembly-bad-') as temporary:
            directory = Path(temporary) / 'fixture'
            shutil.copytree(fixture, directory, copy_function=os.link)
            mutate(directory)
            try:
                validate(directory)
            except (AssertionError, FileNotFoundError) as error:
                cases.append(dict(name=name, rejected=True, error=type(error).__name__, detail=str(error)))
            else:
                raise AssertionError('Corrupt assembly evidence accepted: ' + name)
            assert not (directory / 'timing-assembly-state.json').exists()
            print(name, 'rejected', flush=True)
    validate(fixture)
    assert hashes(root) == before
    result = dict(status='PASS_NATIVE_TIMING_PATHS_ASSEMBLY_GUARD_CONTROLS', cases=cases,
                  assembler_sha256=sha(ROOT / 'tools/assemble_timing_paths_native.py'),
                  qualification_sha256=sha(qualified), reviewer_controls_sha256=sha(controls),
                  original_evidence_unchanged=True, Quartus_executed=False)
    (out / 'guard-controls.json').write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], len(cases))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['root', 'qualified', 'controls', 'output']:
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    run(args.root.resolve(), args.qualified.resolve(), args.controls.resolve(), args.output.resolve())
