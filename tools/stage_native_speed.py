#!/usr/bin/env python3
"""Restore the qualified Native59 inputs for normal-operation clock work.

Read immutable receipts, not today's generated HDL or the Serial74 snapshot
experiment. Verify both the original 413 compiled inputs and all 498 archived
baseline files before writing. Only project-root paths in build configuration
change. This stages source; it never builds or programs an FPGA.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REPORTS = ROOT / 'fpga/mister/reports'
ORIGINAL = '/home/alans/mister/Dorado_Native59_20261019'
CONFIG = {'.qsf', '.qpf', '.qip', '.sdc', '.tcl'}


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def read_raw(path):
    if path.is_file():
        return path.read_bytes()
    return gzip.decompress(Path(str(path) + '.gz').read_bytes())


def baseline(reports):
    original = reports / '2026-10-19-native59-dsketh'
    archive = reports / '2026-10-19-snapshot-serial-area'
    compiled = json.loads((original / 'compiled-inputs.json').read_text())
    if len(compiled) != 413:
        raise ValueError('Native59 compiled-input coverage changed')
    for name, digest in compiled.items():
        if sha(read_raw(original / name)) != digest:
            raise ValueError('Original compiled input differs: ' + name)
    rows = json.loads(read_raw(archive / 'archive-manifest.json'))
    files = {}
    for row in rows:
        prefix = 'baseline-native59/'
        if not row['raw_name'].startswith(prefix):
            continue
        name = row['raw_name'][len(prefix):]
        if Path(name).is_absolute() or '..' in Path(name).parts or name in files:
            raise ValueError('Unsafe/duplicate baseline member: ' + name)
        stored = Path(row['stored'])
        if stored.is_absolute() or '..' in stored.parts:
            raise ValueError('Unsafe receipt storage path')
        raw = (archive / stored).read_bytes()
        if stored.suffix == '.gz':
            raw = gzip.decompress(raw)
        if len(raw) != row['raw_bytes'] or sha(raw) != row['raw_sha256']:
            raise ValueError('Archived baseline differs: ' + name)
        files[name] = raw
    if len(files) != 498:
        raise ValueError('Native59 archived baseline coverage changed')
    for name, digest in compiled.items():
        if name not in files or sha(files[name]) != digest:
            raise ValueError('Baseline does not match compiled input: ' + name)
    return files, compiled


def relocated(name, raw, project):
    if Path(name).suffix in CONFIG:
        return raw.replace(ORIGINAL.encode(), str(project).encode())
    return raw


def verify(directory, reports):
    record = json.loads((directory / 'speed-baseline.json').read_text())
    files, compiled = baseline(reports)
    if record['compiled_inputs'] != compiled:
        raise ValueError('Recorded compiled-input identity differs')
    project = Path(record['project'])
    expected = set(files) | {'speed-baseline.json'}
    actual = {str(p.relative_to(directory)) for p in directory.rglob('*') if p.is_file()}
    if actual != expected:
        raise ValueError('Unexpected/missing staged files')
    for name, raw in files.items():
        output = relocated(name, raw, project)
        if (directory / name).read_bytes() != output:
            raise ValueError('Staged input differs: ' + name)
        if record['source_hashes'].get(name) != sha(output):
            raise ValueError('Recorded staged identity differs: ' + name)
    if set(record['source_hashes']) != set(files):
        raise ValueError('Staged hash coverage differs')
    if record['snapshot_admin'] != 'INACTIVE' or record['hardware_speedup_claimed']:
        raise ValueError('Invalid baseline scope')
    return record


def stage(directory, reports, project):
    if directory.exists():
        raise ValueError('Output exists; verify it rather than restarting')
    if not project.is_absolute():
        raise ValueError('Project destination must be absolute')
    files, compiled = baseline(reports)
    outputs = {name: relocated(name, raw, project) for name, raw in files.items()}
    for name, raw in outputs.items():
        path = directory / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(raw)
    record = dict(
        status='PASS_NATIVE59_SPEED_BASELINE_SOURCE', project=str(project),
        source_hashes={name: sha(raw) for name, raw in outputs.items()},
        compiled_inputs=compiled, archived_files=len(files),
        changes='Project-root relocation in configuration only; HDL unchanged',
        snapshot_admin='INACTIVE', hardware_speedup_claimed=False,
        frequency_mhz=50, sysper=4,
        limits='Source restoration only; no new fit, firmware run or speed measurement',
    )
    (directory / 'speed-baseline.json').write_text(json.dumps(record, indent=2) + '\n')
    return verify(directory, reports)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    action = parser.add_mutually_exclusive_group(required=True)
    action.add_argument('--stage', type=Path)
    action.add_argument('--verify', type=Path)
    parser.add_argument('--reports', type=Path, default=REPORTS)
    parser.add_argument('--project', type=Path,
                        help='Optional absolute destination when staging for another host')
    args = parser.parse_args()
    if args.verify:
        result = verify(args.verify.resolve(), args.reports.resolve())
    else:
        directory = args.stage.resolve()
        result = stage(directory, args.reports.resolve(), args.project or directory)
    print(result['status'], '413 compiled inputs verified; 498 baseline files restored')
