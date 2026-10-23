#!/usr/bin/env python3
"""Preserve qualified command-producer ordering and all earlier attempts.

This is bounded component evidence. No native timing, firmware, speed, disk,
snapshot, compiler database or executable is included or implied.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import tempfile

from archive_r_front_ready_followup import verify
from generate_r_front_commands_reference import CANDIDATE, ROOT, candidate
from qualify_r_front_commands import MUTATIONS, PROFILES

BUILD = ROOT/'build/rtl-executive'
PRIMARY = BUILD/'r-front-commands-full-v4-20261023'


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def archive(out):
    assert not out.exists()
    result = json.loads((PRIMARY/'results.json').read_text())
    assert result['status'] == 'PASS_BOUNDED_R_FRONT_COMMAND_ORDERING' and not result['failures']
    assert len(result['primary']) == 12 and len(result['negative_cases']) == 8 and len(result['cases']) == 20
    assert {(r['profile'], r['engine']) for r in result['primary']} == {
        (p, e) for p in PROFILES for e in ['icarus', 'verilator']}
    assert {(r['engine'], r['mutation']) for r in result['negative_cases']} == {
        (e, m) for e in ['icarus', 'verilator'] for m in MUTATIONS}
    current = sha(ROOT/CANDIDATE)
    assert result['candidate_sha256'] == current
    with tempfile.TemporaryDirectory(prefix='dorado-command-source-') as d:
        p = Path(d)/'candidate.sv'
        candidate(ROOT, p)
        assert sha(p) == current, 'Unaccounted candidate source change'
    for row in result['cases']:
        d = PRIMARY/row['directory']
        assert sha(d/'run-state.json') == row['run_state_sha256']
        s = json.loads((d/'run-state.json').read_text())
        assert s['status'] != 'RUNNING' and all('returncode' in a for a in s['actions'])
        for n, digest in s['inputs'].items():
            assert sha(d/'inputs'/n) == digest, (d, n)
        if row['mutation'] is None:
            assert s['executed_candidate_sha256'] == current
            assert s['reference']['WIM_internal'] and s['reference']['RIM_RTPC_d17_input_internal']
            assert all(x['inverse_byte_identical'] for x in s['adapters']['files'])
    boundaries = BUILD/'r-front-commands-boundaries-v2-20261023'
    b = json.loads((boundaries/'results.json').read_text())
    assert b['status'] == 'PASS_UNUSED_R_FRONT_COMMAND_GATE_BOUNDARIES'
    assert len(b['boundaries']) == 12 and len(b['rejected_controls']) == 5
    assert any(len(p['route']) >= 3 for r in b['boundaries'] for p in r['paths'])
    paths = {}
    directories = sorted(p for p in BUILD.glob('r-front-commands-*-20261023') if p.is_dir())
    for d in directories:
        for p in d.rglob('run-state.json'):
            s = json.loads(p.read_text())
            assert s['status'] != 'RUNNING', p
            assert all('returncode' in a for a in s.get('actions', [])), p
        for p in d.rglob('*'):
            if p.is_file() and 'obj' not in p.relative_to(d).parts and p.suffix in ['.py', '.v', '.sv', '.json', '.log', '.wl', '.lc', '.txt']:
                paths['runs/'+d.name+'/'+str(p.relative_to(d))] = p
    for n in ['generate_r_front_commands_reference.py', 'test_r_front_commands.py',
              'qualify_r_front_commands.py', 'audit_r_front_command_boundaries.py', 'archive_r_front_commands.py']:
        paths['current/tools/'+n] = ROOT/'tools'/n
    paths['current/'+CANDIDATE] = ROOT/CANDIDATE
    for n in ['r-front-commands-attempt-notes-v1-20261023.json',
              'r-front-commands-d17-frontier-v1-20261023.json']:
        paths['followup/'+n] = BUILD/n
    for version in [1, 2]:
        paths[f'followup/native-watch-v{version}.json'] = BUILD/f'native-firmware-window-watch-v{version}-20261023/state.json'
        paths[f'followup/native-watch-v{version}.py'] = BUILD/f'watch-native-firmware-windows-v{version}-20261023.py'
    out.mkdir(parents=True)
    receipt = dict(scope=__doc__, candidate_sha256=current, primary=PRIMARY.name,
                   directories=[d.name for d in directories], members=[], blobs={},
                   full_arrival_contract_qualified=False, physical_timing_qualified=False,
                   native_integrated=False, hardware_speedup_claimed=False)
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
