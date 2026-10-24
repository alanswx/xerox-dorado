#!/usr/bin/env python3
"""Collect a terminal failed native fit without opening its unfinished database.

Require failure of the actual fit stage, all recorded builder/report processes
absent, source hashes unchanged and no report queries executed. Preserve all
frozen inputs, scripts, text reports and logs; omit databases and FPGA images.
These reports do not provide a valid completed physical timing result.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path
import tarfile


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def run(project, out):
    assert not out.exists() and not Path(str(out)+'.json').exists()
    state = json.loads((project/'run-state.json').read_text())
    assert state['status'] == 'TERMINAL_FAILED'
    assert state['stages'][-1]['stage'] == 'fit' and state['stages'][-1]['returncode'] != 0
    reporters = []
    for name in ['physical-report', 'tag-buffer-report']:
        p = project/name/'run-state.json'
        if p.exists():
            r = json.loads(p.read_text())
            assert r['status'].startswith('TERMINAL') and not r['actions']
            reporters.append(r)
    for pid in [state['builder_pid'], *[r['pid'] for r in reporters], *[s['pid'] for s in state['stages']]]:
        assert not (Path('/proc')/str(pid)).exists(), pid
    manifest = project/'r-front-control-native.json'
    m = json.loads(manifest.read_text())
    actual = {name: sha(project/name) for name in m['source_hashes']}
    assert actual == m['source_hashes'] and not m['assembly_permitted']
    files = {name: project/name for name in m['source_hashes']}
    for p in project.iterdir():
        if p.is_file() and p.suffix in ['.py', '.json', '.log', '.tcl', '.wl', '.lc']:
            files[p.name] = p
    for directory in ['physical-report', 'tag-buffer-report', 'address-proof', 'core/output_files']:
        for p in (project/directory).rglob('*'):
            if p.is_file() and p.suffix in ['.rpt', '.summary', '.log', '.json', '.tcl', '.py', '.sv', '.v', '.ys']:
                files[str(p.relative_to(project))] = p
    frozen = dict(status='FAILED_FIT_SOURCE_READBACK_VERIFIED', actual=actual, manifest_sha256=sha(manifest),
        all_recorded_processes_absent=True, report_queries_executed=False,
        completed_physical_timing_available=False, assembly_permitted=False)
    readback = (json.dumps(frozen, indent=2)+'\n').encode()
    rows = []
    with tarfile.open(out, 'x:gz') as archive:
        for name, p in sorted(files.items()):
            raw = p.read_bytes()
            assert not raw.startswith((b'\x7fELF', b'\xcf\xfa\xed\xfe')), name
            entry = tarfile.TarInfo(name)
            entry.size = len(raw)
            archive.addfile(entry, io.BytesIO(raw))
            rows.append(dict(path=name, bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest()))
        name = 'failed-fit-input-readback.json'
        assert name not in files
        entry = tarfile.TarInfo(name)
        entry.size = len(readback)
        archive.addfile(entry, io.BytesIO(readback))
        rows.append(dict(path=name, bytes=len(readback), sha256=hashlib.sha256(readback).hexdigest()))
    record = dict(status='TERMINAL_FAILED_NATIVE_FIT_RAW_COLLECTION', archive_sha256=sha(out), members=rows,
                  completed_physical_timing_available=False, assembly_permitted=False)
    Path(str(out)+'.json').write_text(json.dumps(record, indent=2)+'\n')
    print(json.dumps(dict(status=record['status'], archive=str(out), archive_sha256=record['archive_sha256'])), flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['project', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    run(a.project.resolve(), a.output.resolve())
