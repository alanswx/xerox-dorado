#!/usr/bin/env python3
"""Freeze same-edge board readiness, capture and rejected faster-native evidence.

Do not copy running databases, FPGA images or disk volumes. A terminal Quartus
process is not a timing pass. Preserve failed helpers and source-only stages
with their actual scope, and deduplicate against immutable earlier receipts.
"""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/rtl-executive'
REPORTS = ROOT / 'fpga/mister/reports'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def archive(output):
    assert not output.exists()
    prior = {}
    for name in [
        '2026-10-21-sdram-read-capture/component',
        '2026-10-21-sdram-read-capture/native-fit',
        '2026-10-21-sdram-read-capture/desktop-field',
        '2026-10-21-storage-burst-observer/component',
        '2026-10-21-storage-burst-observer/native-fit',
        '2026-10-22-canonical-sdram',
        '2026-10-22-storage-ready-prediction',
        '2026-10-22-storage-ready-native/integration',
        '2026-10-22-storage-ready-native/native-fit',
    ]:
        p = REPORTS / name
        manifest = json.loads(gzip.decompress((p / 'manifest.json.gz').read_bytes()))
        for h, e in manifest['blobs'].items():
            prior[h] = (p, e)
    paths = {}
    runs = {}

    def tree(p, prefix):
        for f in p.rglob('*'):
            if not f.is_file():
                continue
            relative = f.relative_to(p)
            if any(n.startswith('obj') or n in ('__pycache__', 'db', 'incremental_db')
                   for n in relative.parts):
                continue
            if f.suffix in ('.vvp', '.pyc', '.o', '.a', '.rbf', '.sof', '.pack'):
                continue
            if f.name.endswith('.tar.gz'):
                continue
            paths[prefix + '/' + str(relative)] = f.read_bytes()

    for name in [
        'storage-board-ready-v1-20261022',
        'storage-board-ready-groups-v1-20261022',
        'storage-board-capture-combined-v1-20261022',
        'storage-capture-decode-pins-v1-20261022',
        'storage-capture-decode-pins-v2-20261022',
        'storage-capture-decode-pins-v3-20261022',
        'storage-capture-decode-trace-v2-20261022',
        'storage-capture-decode-trace-v3-20261022',
        'storage-capture-decode-full66-v1-20261022',
        'storage-capture-decode-full66-v2-20261022',
        'storage-capture-decode-full57-v1-20261022',
        'storage-board-native66-startup-v1-20261022',
        'storage-board-native66-wrong-step-v1-20261022',
        'storage-board-native57-startup-v1-20261022',
        'mc10181-lookahead-v1-20261022',
    ]:
        p = BUILD / name
        state = json.loads((p / 'run-state.json').read_text())
        assert state['status'] != 'RUNNING'
        assert all('returncode' in a for a in state['actions']), name
        runs[name] = state['status']
        tree(p, 'simulation/' + name)
    for name in ['storage-board-ready-fit-v2-20261022/terminal',
                 'storage-board-ready-review-controls-v1-20261022']:
        tree(BUILD / name, 'component-fit/' + name)
    for name in ['storage-capture-decode-trace-v1-20261022.log',
                 'storage-board-ready-fit-v1-20261022.log']:
        p = BUILD / name
        if p.exists():
            paths['failed-pre-run/' + name] = p.read_bytes()

    p = BUILD / 'storage-board-native66-v1-20261022'
    raw = p / 'terminal-v1/raw'
    terminal = json.loads((raw / 'run-state.json').read_text())
    observation = json.loads((raw / 'timing-observation.json').read_text())
    assert terminal['status'] == 'TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED'
    assert not observation['all_summary_slacks_nonnegative']
    assert not observation['assembly_permitted']
    rows = json.loads(gzip.decompress(
        (p / 'terminal-v1/receipt/archive-manifest.json.gz').read_bytes()))
    assert len(rows) == 953
    for row in rows:
        b = (raw / row['raw_name']).read_bytes()
        assert len(b) == row['raw_bytes'] and sha(b) == row['raw_sha256']
    tree(raw, 'native66-rejected/terminal')
    for name in ['storage-board-native66-v1-20261022',
                 'storage-board-native57-v1-20261022']:
        p = BUILD / name
        record = json.loads((p / 'storage-board-native.json').read_text())
        for n, h in record['source_hashes'].items():
            b = (p / n).read_bytes()
            assert sha(b) == h, (name, n)
            paths['source-only/' + name + '/' + n] = b
        for f in p.iterdir():
            if f.is_file() and f.suffix in ('.py', '.json'):
                paths['source-only/' + name + '/' + f.name] = f.read_bytes()
        for d in p.glob('storage-ready-reports-v*'):
            tree(d, 'source-only/' + name + '/' + d.name)
    for name in [
        'prepare_storage_board_ready.py', 'test_storage_board_ready.py',
        'fit_storage_board_ready.py', 'review_storage_board_ready.py',
        'test_review_storage_board_ready.py', 'storage_capture_decode.py',
        'test_storage_capture_decode.py', 'test_storage_capture_trace.py',
        'stage_storage_board_native.py', 'test_mc10181_lookahead.py',
        'build_r_front_ready_native.py', 'collect_storage_ready_native.py',
        'report_storage_ready_native.py', 'test_storage_ready_native.py',
        Path(__file__).name,
    ]:
        paths['current-tools/' + name] = (ROOT / 'tools' / name).read_bytes()
    p = BUILD / 'mister14-live-status-20261022.png'
    assert p.read_bytes().endswith(bytes.fromhex('0000000049454e44ae426082'))
    paths['hardware-unchanged/live-status-20261022.png'] = p.read_bytes()
    output.mkdir(parents=True)
    m = dict(scope=__doc__, runs=runs, members=[], blobs={},
             native66_timing_pass=False, firmware_qualified=False,
             hardware_speedup_qualified=False, assembly_permitted=False)
    for name, b in sorted(paths.items()):
        assert not b.startswith(b'\x7fELF'), name
        h = sha(b)
        if h not in m['blobs']:
            if h in prior:
                base, e = prior[h]
                entry = dict(e, stored=os.path.relpath(base / e['stored'], output))
            else:
                stored = 'blobs/' + h + '.gz'
                f = output / stored
                f.parent.mkdir(exist_ok=True)
                z = gzip.compress(b, mtime=0)
                f.write_bytes(z)
                entry = dict(stored=stored, raw_bytes=len(b), stored_bytes=len(z),
                             stored_sha256=sha(z))
            assert sha(gzip.decompress((output / entry['stored']).read_bytes())) == h
            m['blobs'][h] = entry
        m['members'].append(dict(name=name, blob=h))
    (output / 'manifest.json.gz').write_bytes(
        gzip.compress((json.dumps(m, indent=2) + '\n').encode(), mtime=0))
    print(len(paths), 'members', len(m['blobs']), 'blobs')


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    archive(a.output.resolve())
