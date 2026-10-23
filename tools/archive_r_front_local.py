#!/usr/bin/env python3
"""Preserve bounded 792-Q Control-A/processor ordering evidence losslessly.

No native, FPGA timing, ROM-boot or speed qualification is implied. Exclude
compiler databases, executables, disk images and snapshots. Preserve earlier
failed fixtures and the incomplete jcnt-only state coverage attempt explicitly.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path

from archive_r_front_ready_followup import verify
from test_r_front_local import CANDIDATE, ROOT, PROFILES

BUILD = ROOT/'build/rtl-executive'
PRIMARY = BUILD/'r-front-local-full-v8-20261023'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def archive(out):
    assert not out.exists()
    record = json.loads((PRIMARY/'results.json').read_text())
    assert record['status'] == 'PASS_BOUNDED_R_FRONT_LOCAL_ORDERING'
    assert not record['failures'] and len(record['cases']) == 22
    assert len(record['primary']) == 12 and len(record['negative_cases']) == 10
    assert {(r['profile'], r['engine']) for r in record['primary']} == {
        (p, e) for p in PROFILES for e in ['icarus', 'verilator']}
    candidate = sha(ROOT/CANDIDATE)
    assert candidate == record['candidate_sha256']
    for row in record['cases']:
        d = PRIMARY/row['directory']
        state = json.loads((d/'run-state.json').read_text())
        assert sha(d/'run-state.json') == row['run_state_sha256']
        assert state['status'] != 'RUNNING' and all('returncode' in a for a in state['actions'])
        for n, digest in state['inputs'].items():
            assert sha(d/'inputs'/n) == digest, (d, n)
        if row['mutation'] is None:
            assert state['executed_candidate_sha256'] == candidate
            assert row['summary'] == 'checks=11854 writes=318 reads=5012 epochs=807', row
            assert 'R_FRONT_LOCAL_CHANGED front=fffffffffffffffffffffffff Ready=ffff' in (d/'test.log').read_text()
            assert len(state['adapters']['files']) == 10
            assert all(f['inverse_byte_identical'] for f in state['adapters']['files'])
            for n, digest in state['inputs'].items():
                if (ROOT/n).exists():
                    assert sha(ROOT/n) == digest, ('primary source changed', d, n)
    boundaries = BUILD/'r-front-local-boundaries-v3-20261023'
    audited = json.loads((boundaries/'results.json').read_text())
    assert audited['status'] == 'PASS_UNUSED_R_FRONT_LOCAL_GATE_BOUNDARIES'
    assert len(audited['boundaries']) == 11 and len(audited['rejected_controls']) == 5
    assert len(audited['live_crossings']) == 2
    directories = sorted(p for p in BUILD.glob('r-front-local-*-20261023') if p.is_dir())
    assert PRIMARY in directories
    paths = {}
    suffixes = {'.py', '.sv', '.v', '.json', '.log', '.wl', '.lc', '.txt', '.analyze'}
    for directory in directories:
        for p in directory.rglob('*'):
            if p.is_file() and 'obj' not in p.relative_to(directory).parts and p.suffix in suffixes:
                paths['runs/'+directory.name+'/'+str(p.relative_to(directory))] = p
        for p in directory.rglob('run-state.json'):
            s = json.loads(p.read_text())
            assert s['status'] != 'RUNNING', ('live attempt', p)
            assert all('returncode' in a for a in s.get('actions', [])), p
    notes = BUILD/'r-front-local-attempt-notes-v1-20261023.json'
    paths['attempt-notes.json'] = notes
    for n in ['r-front-local-decode-source-check-v1-20261023.json',
              'r-front-local-producer-frontier-v1-20261023.json']:
        paths['followup/'+n] = BUILD/n
    frontier = json.loads((BUILD/'r-front-local-producer-frontier-v1-20261023.json').read_text())
    for n, digest in frontier['sources'].items():
        assert sha(ROOT/n) == digest, n
        paths['original/'+n] = ROOT/n
    paths['followup/native-watch-running-prefix.json'] = BUILD/'native-firmware-window-watch-v1-20261023/state.json'
    for n in ['generate_r_front_local_reference.py', 'generate_r_front_local_adapters.py',
              'test_r_front_local.py', 'qualify_r_front_local.py',
              'audit_r_front_local_boundaries.py', 'archive_r_front_local.py']:
        paths['current/tools/'+n] = ROOT/'tools'/n
    paths['current/'+CANDIDATE] = ROOT/CANDIDATE
    out.mkdir(parents=True)
    receipt = dict(scope=__doc__, candidate_sha256=candidate, primary=PRIMARY.name,
                   directories=[p.name for p in directories], members=[], blobs={},
                   native_integrated=False, physical_timing_qualified=False,
                   full_producer_arrival_contract_qualified=False, hardware_speedup_claimed=False)
    for name, p in sorted(paths.items()):
        raw = p.read_bytes()
        digest = hashlib.sha256(raw).hexdigest()
        if digest not in receipt['blobs']:
            stored = 'blobs/'+digest+'.gz'
            target = out/stored
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(gzip.compress(raw, mtime=0))
            receipt['blobs'][digest] = dict(stored=stored, raw_bytes=len(raw),
                                          stored_bytes=target.stat().st_size, stored_sha256=sha(target))
        receipt['members'].append(dict(name=name, blob=digest))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(receipt, indent=2)+'\n').encode(), mtime=0))
    verify(out)


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    archive(p.parse_args().output.resolve())
