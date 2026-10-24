#!/usr/bin/env python3
"""Collect completed native-fit and read-only reports after exact readback.

No images or databases. Preserve failed reports as evidence, without treating
successful compilation/reporting as timing, firmware or speed qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import tarfile


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def collect(project, output, reports_dir='physical-native-reports-v1', additional_reports=()):
    assert project not in output.parents and not output.exists()
    build = json.loads((project / 'run-state.json').read_text())
    assert Path(reports_dir).name == reports_dir and reports_dir not in ['.', '..']
    follow = project / reports_dir
    extra = json.loads((follow / 'run-state.json').read_text())
    assert build['status'] in ['TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED', 'TERMINAL_FAILED']
    assert extra['status'] in ['TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED', 'TERMINAL_FAILED']
    pids = [build['builder_pid'], extra['pid']] + [a['pid'] for a in build['stages'] + extra['actions']]
    assert not [pid for pid in pids if (Path('/proc') / str(pid)).exists()], 'Observe existing processes until exit'
    manifest = project / 'r-front-control-native.json'
    r = json.loads(manifest.read_text())
    assert sha(manifest) == extra['manifest_sha256'] and not r['assembly_permitted']
    assert sha(follow / 'audit_r_front_physical_native.tcl') == extra['script_sha256']
    actual = {n: sha(project / n) for n in r['source_hashes']}
    assert actual == r['source_hashes'], 'A compiled source/configuration changed'
    additional = []
    for name in additional_reports:
        assert Path(name).name == name and name not in ['.', '..', reports_dir]
        folder = project / name
        state = json.loads((folder / 'run-state.json').read_text())
        assert state['status'].startswith('TERMINAL')
        assert state['manifest_sha256'] == sha(manifest) and not state['assembly_permitted']
        assert not state['constraints_changed']
        assert all('returncode' in a for a in state['actions'])
        extra_pids = [state['pid'], *[a['pid'] for a in state['actions']]]
        assert not [pid for pid in extra_pids if (Path('/proc') / str(pid)).exists()]
        pids.extend(extra_pids)
        additional.append(folder)
    assert not list((project / 'core/output_files').glob('*.sof'))
    assert not list((project / 'core/output_files').glob('*.rbf'))
    readback = project / 'physical-native-input-readback.json'
    assert not readback.exists()
    readback.write_text(json.dumps(dict(status='TERMINAL_PHYSICAL_NATIVE_INPUT_READBACK',
        absent_pids=pids, actual=actual, build_status=build['status'], report_status=extra['status'],
        timing_qualified=False, firmware_qualified=False, assembly_collected=False), indent=2) + '\n')
    files = set(actual)
    for directory in [project, project / 'core', project / 'core/output_files', follow, *additional]:
        files.update(str(p.relative_to(project)) for p in directory.iterdir()
                     if p.is_file() and p.suffix in ['.rpt', '.summary', '.log', '.json', '.py', '.tcl'])
    rows = []
    with tarfile.open(output, 'w:gz') as archive:
        for name in sorted(files):
            p = project / name
            assert not Path(name).is_absolute() and '..' not in Path(name).parts
            assert p.suffix not in ['.sof', '.rbf', '.o', '.a', '.pack']
            assert not p.read_bytes().startswith(b'\x7fELF')
            archive.add(p, arcname=name)
            rows.append(dict(path=name, sha256=sha(p), bytes=p.stat().st_size))
    receipt = dict(status='TERMINAL_PHYSICAL_NATIVE_RAW_COLLECTION', archive=str(output),
                   archive_sha256=sha(output), members=rows, timing_qualified=False,
                   firmware_qualified=False, speedup_qualified=False)
    Path(str(output) + '.json').write_text(json.dumps(receipt, indent=2) + '\n')
    print(json.dumps({k: receipt[k] for k in ['status', 'archive', 'archive_sha256']}), flush=True)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--project', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--reports-dir', default='physical-native-reports-v1')
    p.add_argument('--additional-report', action='append', default=[])
    a = p.parse_args()
    collect(a.project.resolve(), a.output.resolve(), a.reports_dir, a.additional_report)
