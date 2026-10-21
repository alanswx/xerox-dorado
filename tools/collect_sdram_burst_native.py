#!/usr/bin/env python3
"""Collect terminal native BL8 fit evidence, excluding FPGA images/databases."""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import tarfile


def sha(data):
    return hashlib.sha256(data).hexdigest()


def collect(project):
    build = json.loads((project / 'run-state.json').read_text())
    extra = json.loads((project / 'burst-review-v2-state.json').read_text())
    assert build['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert extra['status'] == 'TERMINAL_BURST_REPORTS_REVIEW_REQUIRED'
    pids = [build['builder_pid'], extra['driver_pid']] + [r['pid'] for r in build['stages'] + extra['actions']]
    assert all(r['returncode'] == 0 for r in build['stages'] + extra['actions'])
    for pid in pids:
        try:
            os.kill(pid, 0)
        except ProcessLookupError:
            continue
        raise RuntimeError('Recorded process still exists; observe ' + str(pid))
    record = json.loads((project / 'sdram-burst-native.json').read_text())
    actual = {n: sha((project / n).read_bytes()) for n in record['source_hashes']}
    assert actual == record['source_hashes'] and len(actual) == 499 + int(record.get('passive_boot_fault_observer', False)) + int(record.get('raw_storage_selftest', False))
    check = dict(status='TERMINAL_NATIVE_BURST_INPUT_READBACK', actual=actual,
                 absent_pids=pids, assembly_collected=False)
    (project / 'collected-input-check.json').write_text(json.dumps(check, indent=2) + '\n')
    files = set(actual)
    for directory in [project, project / 'core', project / 'core/output_files', project / 'rejected-burst-review-v1']:
        if not directory.exists():
            continue
        files.update(str(p.relative_to(project)) for p in directory.iterdir()
                     if p.is_file() and p.suffix in ['.log', '.json', '.py', '.tcl', '.rpt', '.summary'])
    out = project / 'burst-terminal-receipt-v1'
    out.mkdir(exist_ok=False)
    rows = []
    for name in sorted(files):
        assert not Path(name).is_absolute() and '..' not in Path(name).parts
        assert Path(name).suffix not in ['.rbf', '.sof', '.qdb', '.o', '.a', '.pack']
        raw = (project / name).read_bytes()
        stored = name + '.gz'
        target = out / stored
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(gzip.compress(raw, mtime=0))
        assert gzip.decompress(target.read_bytes()) == raw
        rows.append(dict(raw_name=name, stored=stored, raw_bytes=len(raw), raw_sha256=sha(raw)))
    (out / 'archive-manifest.json.gz').write_bytes(gzip.compress((json.dumps(rows, indent=2) + '\n').encode(), mtime=0))
    archive = project / 'burst-terminal-receipt-v1.tar.gz'
    with tarfile.open(archive, 'w:gz') as stream:
        stream.add(out, arcname='receipt')
    print(json.dumps(dict(status='TERMINAL_NATIVE_BURST_RECEIPT_READBACK', members=len(rows),
                          archive=str(archive), archive_sha256=sha(archive.read_bytes()),
                          timing_qualified=False, firmware_qualified=False)))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--project', type=Path, required=True)
    args = parser.parse_args()
    collect(args.project.resolve())
