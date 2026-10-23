#!/usr/bin/env python3
"""Preserve the visible-R latch rewrite and terminal isolated timing.

Two new complete graph runs and a source-bound binary nonoverlap proof bridge
inherit twelve prior capture runs. No new architecture, native assembly,
analog skew or hardware throughput is qualified by this receipt.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
from archive_r_front_ready_followup import verify
from audit_control_clock_fit import audit, visible_bridge

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def archive(out):
    assert not out.exists()
    qualified=BUILD/'r-front-visible-budget-gate-v1-20261023'
    gate=json.loads((qualified/'results.json').read_text())
    visible_bridge(qualified,gate)
    for n,digest in gate['inputs'].items():
        assert sha(qualified/'executed-inputs'/n)==digest,n
    for row in gate['arrivals']:
        d=BUILD/row['run'];s=json.loads((d/'run-state.json').read_text())
        assert sha(d/'run-state.json')==row['run_state_sha256']
        assert s['status']=='PASS_R_FRONT_LOCAL_PROBE' and len(s['actions'])==2
        assert all(a['returncode']==0 for a in s['actions'])
        assert s['executed_cone_sha256']=='60d63614c927570d9fbed383e837955510fe10242a3ef74574ff485e0e6647c5'
        assert sha(d/'adapters/dorado_local_r_operand_step.sv')==gate['visible_operand_sha256']
        for n,digest in s['inputs'].items():assert sha(d/'inputs'/n)==digest,n
    terminal=BUILD/'r-front-visible-timing-terminal-v1-20261023'
    reviewed=audit(terminal,qualified,False)
    review=BUILD/'r-front-visible-timing-review-v1-20261023.json'
    assert reviewed==json.loads(review.read_text())
    for name,count in [('r-visible-bridge-reader-v1-20261023',6),
                       ('r-visible-old-stack-reader-v1-20261023',20),
                       ('r-front-visible-reader-v1-20261023',20 if reviewed['status']=='REJECTED_COMPONENT_TIMING' else 22)]:
        r=json.loads((BUILD/name/'results.json').read_text())
        assert len(r['cases'])==count and all(c['rejected']for c in r['cases'])
    comparison=BUILD/'r-visible-fit-source-comparison-v1-20261023.json'
    assert json.loads(comparison.read_text())['status']=='PASS_ONLY_VISIBLE_R_MUX_DIFFERS'
    paths={}
    directories=sorted(d for pattern in ['r-front-visible-*-20261023','r-visible-*-20261023']for d in BUILD.glob(pattern)if d.is_dir())
    suffixes={'.py','.sv','.v','.json','.log','.wl','.lc','.txt','.smt2','.rpt','.summary','.sdc','.qsf','.qpf','.tcl'}
    for d in directories:
        for p in d.rglob('run-state.json'):
            s=json.loads(p.read_text())
            assert s['status']!='RUNNING',('Observe existing job',p)
            assert all('returncode'in a for a in s.get('actions',[])),p
        for p in d.rglob('*'):
            if p.is_file() and not {'obj','db','incremental_db'}.intersection(p.relative_to(d).parts) and p.suffix in suffixes:
                paths['runs/'+d.name+'/'+str(p.relative_to(d))]=p
    for n in ['rewrite_r_visible.py','prove_r_visible.py','test_r_front_visible.py',
              'test_r_front_local.py','fit_r_front_visible.py','audit_control_clock_fit.py',
              'test_control_clock_fit_reader.py','test_r_visible_bridge.py','archive_r_front_visible.py',
              'collect_control_clock_fit.py']:
        paths['current/tools/'+n]=ROOT/'tools'/n
    for p in [review,comparison,BUILD/'r-visible-attempt-notes-v1-20261023.json',
              BUILD/'r-front-visible-path-review-v1-20261023.json']:
        paths['review/'+p.name]=p
    out.mkdir(parents=True)
    receipt=dict(scope=__doc__,timing_status=reviewed['status'],members=[],blobs={},
                 directories=[d.name for d in directories],native_integrated=False,
                 physical_timing_qualified=False,full_producer_arrival_contract_qualified=False,
                 hardware_speedup_claimed=False)
    for name,p in sorted(paths.items()):
        raw=p.read_bytes();digest=hashlib.sha256(raw).hexdigest()
        if digest not in receipt['blobs']:
            stored='blobs/'+digest+'.gz';target=out/stored
            target.parent.mkdir(parents=True,exist_ok=True)
            target.write_bytes(gzip.compress(raw,mtime=0))
            receipt['blobs'][digest]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=target.stat().st_size,stored_sha256=sha(target))
        receipt['members'].append(dict(name=name,blob=digest))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(receipt,indent=2)+'\n').encode(),mtime=0))
    verify(out)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();archive(a.output.resolve())
