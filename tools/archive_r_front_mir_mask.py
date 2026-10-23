#!/usr/bin/env python3
"""Preserve the MIR mask's bounded ordering/properties and prepared budget.

The timing build observation is a live-process prefix only. This archive does
not turn it into terminal timing, native boot, hardware speed or deployment
qualification. Exclude compiler databases, executables, disk and snapshot bytes.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path

from archive_r_front_ready_followup import verify
from fit_r_front_mir_mask import ROOT, BUILD, CANDIDATE_SHA, top_source
from audit_control_clock_fit import audit


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def archive(out):
    assert not out.exists()
    suite = BUILD/'r-front-mir-mask-full-v4-20261023'
    result = json.loads((suite/'results.json').read_text())
    assert result['status'] == 'PASS_BOUNDED_R_FRONT_MIR_MASK_ORDERING' and not result['failures']
    assert len(result['primary']) == 12 and len(result['negative_cases']) == 2
    for row in result['cases']:
        d = suite/row['directory']
        assert sha(d/'run-state.json') == row['run_state_sha256']
        s = json.loads((d/'run-state.json').read_text())
        assert s['status'] != 'RUNNING' and all('returncode' in r for r in s['actions'])
        assert s['actions'][0]['returncode'] == 0
        for n, digest in s['inputs'].items():
            assert sha(d/'inputs'/n) == digest, (d, n)
        if not row['frozen']:
            assert sha(d/'candidate.sv') == CANDIDATE_SHA
            assert s['summary'] == 'checks=13515 writes=328 reads=5608 epochs=797'
    gate = BUILD/'r-front-mir-mask-budget-gate-v1-20261023'
    g = json.loads((gate/'results.json').read_text())
    assert g['status'] == 'PASS_BOUNDED_R_FRONT_MIR_MASK_BUDGET_GATE'
    assert g['candidate_sha256'] == CANDIDATE_SHA
    for n, digest in g['inputs'].items():
        assert sha(gate/'executed-inputs'/n) == digest, n
    p = json.loads((gate/'mask-properties/proof.json').read_text())
    assert p['status'] == 'PASS_SOURCE_BOUND_MIR_CLOCK_MASK_PROPERTIES'
    assert len(p['cases']) == 7 and [r['result'] for r in p['cases']] == ['unsat']*4+['sat']*3
    prepared = BUILD/'r-front-mir-mask-timing-input-v1-20261023'
    assert (prepared/'top.sv').read_text() == top_source()
    assert sha(prepared/'dorado_r_front_commands_step.sv') == CANDIDATE_SHA
    for name, count in [('r-front-mir-mask-old-rejected-reader-v2-20261023', 19),
                        ('r-front-mir-mask-old-ready-reader-v1-20261023', 20)]:
        r = json.loads((BUILD/name/'results.json').read_text())
        assert len(r['cases']) == count and all(c['rejected'] for c in r['cases'])
    observation = BUILD/'r-front-mir-mask-build-observation-v1-20261023.json'
    seen = json.loads(observation.read_text())
    assert seen['state']['status'] == 'RUNNING'
    assert seen['processes'][str(seen['state']['builder_pid'])]['exists']
    assert seen['processes'][str(seen['state']['stages'][-1]['pid'])]['exists']
    assert seen['candidate_sha256'] == CANDIDATE_SHA
    paths = {}
    directories = sorted(p for p in BUILD.glob('r-front-mir-mask-*-20261023') if p.is_dir())
    suffixes = {'.py', '.sv', '.v', '.json', '.log', '.wl', '.lc', '.txt', '.smt2',
                '.rpt', '.summary', '.sdc', '.qsf', '.qpf', '.tcl'}
    for d in directories:
        for p in d.rglob('run-state.json'):
            s = json.loads(p.read_text())
            assert s['status'] != 'RUNNING', ('Observe existing local job', p)
            assert all('returncode' in r for r in s.get('actions', [])), p
        for p in d.rglob('*'):
            if p.is_file() and not {'obj', 'db', 'incremental_db'}.intersection(p.relative_to(d).parts) and p.suffix in suffixes:
                paths['runs/'+d.name+'/'+str(p.relative_to(d))] = p
    for n in ['test_r_front_mir_mask.py', 'prove_mir_clock_mask.py', 'fit_r_front_mir_mask.py',
              'audit_control_clock_fit.py', 'test_control_clock_fit_reader.py', 'archive_r_front_mir_mask.py']:
        paths['current/tools/'+n] = ROOT/'tools'/n
    for n in ['r-front-mir-mask-attempt-notes-v1-20261023.json', observation.name]:
        paths['followup/'+n] = BUILD/n
    out.mkdir(parents=True)
    receipt = dict(scope=__doc__, candidate_sha256=CANDIDATE_SHA, members=[], blobs={},
                   directories=[d.name for d in directories], timing_build_scope='RUNNING_PREFIX_ONLY',
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


def archive_timing(out):
    assert not out.exists()
    directory = BUILD/'r-front-mir-mask-timing-terminal-v1-20261023'
    qualified = BUILD/'r-front-mir-mask-budget-gate-v1-20261023'
    reviewed = audit(directory, qualified, False)
    review_path = BUILD/'r-front-mir-mask-timing-review-v1-20261023.json'
    assert reviewed == json.loads(review_path.read_text())
    assert reviewed['status'] == 'REJECTED_COMPONENT_TIMING'
    assert reviewed['minimum_slack_ns'] == dict(setup=-1.996, hold=0.137, mpw=6.116)
    assert reviewed['resources']['mlab_memory_bits'] == 9216
    assert reviewed['resources']['alms'] == 3391 and reviewed['resources']['registers'] == 3001
    controls = BUILD/'r-front-mir-mask-rejected-reader-v1-20261023'
    r = json.loads((controls/'results.json').read_text())
    assert r['status'] == 'PASS_REJECTED_FIT_READER_CONTROLS'
    assert len(r['cases']) == 20 and all(c['rejected'] for c in r['cases'])
    path_review = BUILD/'r-front-mir-mask-path-review-v1-20261023.json'
    p = json.loads(path_review.read_text())
    assert p['launch']['bit'] == 753 and p['result']['field'] == 'faults' and p['result']['bit'] == 3
    assert p['logic_levels'] == 19 and p['data_delay_ns'] == 16.683
    paths = {'timing-review.json': review_path, 'path-review.json': path_review}
    suffixes = {'.json', '.log', '.sv', '.v', '.py', '.txt', '.sdc', '.qsf', '.qpf', '.tcl', '.rpt', '.summary', '.smt2'}
    for prefix, source in [('terminal', directory), ('reader-controls', controls), ('qualified', qualified)]:
        for f in source.rglob('*'):
            if f.is_file() and not {'obj', 'db', 'incremental_db'}.intersection(f.relative_to(source).parts) and f.suffix in suffixes:
                paths[prefix+'/'+str(f.relative_to(source))] = f
    for n in ['audit_control_clock_fit.py', 'test_control_clock_fit_reader.py', 'fit_r_front_mir_mask.py',
              'collect_control_clock_fit.py', 'archive_r_front_mir_mask.py']:
        paths['current/tools/'+n] = ROOT/'tools'/n
    out.mkdir(parents=True)
    receipt = dict(scope='Terminal rejected15ns masked-MIR component fit; no native/firmware/speed qualification.',
                   candidate_sha256=CANDIDATE_SHA, timing_status=reviewed['status'], members=[], blobs={},
                   native_integrated=False, physical_timing_qualified=False,
                   full_producer_arrival_contract_qualified=False, hardware_speedup_claimed=False)
    for name, f in sorted(paths.items()):
        raw = f.read_bytes()
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
    p.add_argument('--timing', action='store_true')
    a = p.parse_args()
    (archive_timing if a.timing else archive)(a.output.resolve())
