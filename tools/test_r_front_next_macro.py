#!/usr/bin/env python3
"""Probe original MIR-driven NextMacro in the combined 792-Q graph.

Reuse the full qualified comparison bench. Obsolete external IFUNext'a is
driven independently; candidate and original must use actual MIR producers.
This is a component experiment, not native/physical timing qualification.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import sys
from unittest.mock import patch

import test_r_front_local as parent
from generate_r_front_next_macro_reference import CANDIDATE, generate
from archive_r_front_ready_followup import verify


def archive(out):
    assert not out.exists()
    build = parent.ROOT/'build/rtl-executive'
    candidate_sha = hashlib.sha256((parent.ROOT/CANDIDATE).read_bytes()).hexdigest()
    paths = {}
    rows = []
    for engine in ['icarus', 'verilator']:
        for mutation in [False, True]:
            name = 'r-front-next-macro-'+('external-' if mutation else '')+engine+'-v1-20261023'
            d = build/name
            state = json.loads((d/'run-state.json').read_text())
            assert state['status'] != 'RUNNING' and state['external_IFUNext_mutation'] == mutation
            assert len(state['actions']) == 2 and state['actions'][0]['returncode'] == 0
            assert all('returncode' in a for a in state['actions'])
            for n, digest in state['inputs'].items():
                assert hashlib.sha256((d/'inputs'/n).read_bytes()).hexdigest() == digest, (name, n)
            log = (d/'test.log').read_text()
            if mutation:
                assert state['status'] == 'R_FRONT_LOCAL_PROBE_FAILED'
                assert state['actions'][1]['returncode'] != 0
                assert 'R front resolved source mismatch check=6221' in log
            else:
                assert state['status'] == 'PASS_R_FRONT_LOCAL_PROBE'
                assert state['summary'] == 'checks=11854 writes=318 reads=5012 epochs=801'
                assert state['executed_candidate_sha256'] == candidate_sha
                assert state['reference']['NextMacro_internal']
                assert "IFUNext'a" not in state['reference']['boards'][0]['boundary_bindings']
                assert 'e22' in state['reference']['boards'][0]['packages']
                assert 'R_FRONT_LOCAL_CHANGED front=fffffffffffffffffffffffff Ready=ffff' in log
            rows.append(dict(run=name, engine=engine, mutation=mutation, status=state['status'], summary=state.get('summary')))
            for p in d.rglob('*'):
                if p.is_file() and 'obj' not in p.relative_to(d).parts and p.suffix in ['.py', '.sv', '.v', '.json', '.log', '.wl', '.lc', '.txt']:
                    paths['runs/'+name+'/'+str(p.relative_to(d))] = p
    for n in ['test_r_front_next_macro.py', 'generate_r_front_next_macro_reference.py']:
        paths['current/tools/'+n] = parent.ROOT/'tools'/n
    paths['current/'+CANDIDATE] = parent.ROOT/CANDIDATE
    paths['source-delta.json'] = build/'r-front-next-macro-source-check-v1-20261023.json'
    out.mkdir(parents=True)
    receipt = dict(scope=__doc__, candidate_sha256=candidate_sha, runs=rows, members=[], blobs={},
                   profile='balanced', full_arrival_contract_qualified=False,
                   physical_timing_qualified=False, native_integrated=False, hardware_speedup_claimed=False)
    for name, p in sorted(paths.items()):
        raw = p.read_bytes()
        digest = hashlib.sha256(raw).hexdigest()
        if digest not in receipt['blobs']:
            stored = 'blobs/'+digest+'.gz'
            target = out/stored
            target.parent.mkdir(parents=True, exist_ok=True)
            encoded = gzip.compress(raw, mtime=0)
            target.write_bytes(encoded)
            receipt['blobs'][digest] = dict(stored=stored, raw_bytes=len(raw), stored_bytes=len(encoded),
                                          stored_sha256=hashlib.sha256(encoded).hexdigest())
        receipt['members'].append(dict(name=name, blob=digest))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(receipt, indent=2)+'\n').encode(), mtime=0))
    verify(out)


def run(a):
    candidate = CANDIDATE
    if a.mutation:
        source = (parent.ROOT/CANDIDATE).read_text()
        before = 'r[212]=mir[19]&mir[18]&mir[10]&!mir[8]&mir[15]&!mir[9]'
        assert source.count(before) == 1
        path = a.output.resolve().parent/(a.output.name+'-external-candidate.sv')
        assert not path.exists()
        path.write_text(source.replace(before, 'r[212]=!p[313]'))
        candidate = str(path.relative_to(parent.ROOT))
    original_bench = parent.bench
    def bench(*args, **kwargs):
        raw = original_bench(*args, **kwargs)
        assert raw.count('dorado_r_front_local_step dut(') == 1
        return raw.replace('dorado_r_front_local_step dut(', 'dorado_r_front_next_macro_step dut(')
    with patch.object(parent, 'CANDIDATE', candidate), patch.object(parent, 'generate', generate), patch.object(parent, 'bench', bench):
        rc = parent.run(a.base.resolve(), a.output.resolve(), a.iterations, a.engine, a.profile,
                        True, True, True, True, None, True)
    path = a.output.resolve()/'run-state.json'
    record = json.loads(path.read_text())
    record.update(experiment='actual_ContA_e22_NextMacro', external_IFUNext_mutation=a.mutation,
                  full_arrival_contract_qualified=False, physical_timing_qualified=False)
    path.write_text(json.dumps(record, indent=2)+'\n')
    return rc


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--base', type=Path)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--engine', choices=['icarus', 'verilator'], default='verilator')
    p.add_argument('--profile', choices=parent.PROFILES, default='balanced')
    p.add_argument('--iterations', type=int, default=1024)
    p.add_argument('--mutation', action='store_true', help='Use obsolete external IFUNext instead of actual MIR decode')
    p.add_argument('--archive', action='store_true', help='Preserve the four terminal v1 experiments')
    a = p.parse_args()
    if a.archive:
        archive(a.output.resolve())
    else:
        assert a.base
        sys.exit(run(a))
