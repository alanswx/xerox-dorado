#!/usr/bin/env python3
"""Preserve actual l17 producer ordering, failures and exact executed sources.

Twelve complete profile/engine passes and two additional LOW-B passes compare
798 original Qs,9216 RAM bits,69 clocks. Four bad producers fail. No new native
fit, original-ROM boot, 15 ns or hardware speed qualification is claimed.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
from archive_r_front_ready_followup import verify

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
SHA='665188a29d07ba22981061233a825f496a479d577f5a6794a6b81cfac8c418d2'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def archive(out):
    assert not out.exists()
    reviewed=BUILD/'r-front-l17-reviewed-v1-20261023/results.json';review=json.loads(reviewed.read_text())
    assert review['status']=='PASS_REVIEWED_BOUNDED_R_FRONT_L17_ORDERING'
    assert len(review['primary'])==12 and len(review['negative_cases'])==4
    assert all(r['first_failure_check']==17 for r in review['negative_cases'])
    late=BUILD/'r-front-l17-late-review-v1-20261023.json';l=json.loads(late.read_text())
    assert l['status']=='PASS_L17_LOW_B_CAPTURE_AND_FEEDBACK' and len(l['cases'])==2
    for row in review['cases']+l['cases']:
        d=BUILD/row['run'];assert sha(d/'run-state.json')==row['run_state_sha256']
        r=json.loads((d/'run-state.json').read_text());assert r['status']!='RUNNING' and all('returncode'in a for a in r['actions'])
        for n,h in r['inputs'].items():assert sha(d/'inputs'/n)==h,(d,n)
        for n,h in r['timed_sources'].items():assert sha(d/'timed'/n)==h,(d,n)
    boundary=json.loads((BUILD/'r-front-l17-boundaries-v3-20261023/results.json').read_text())
    assert len(boundary['unused_boundaries'])==16 and len(boundary['rejected_controls'])==5
    old=json.loads((BUILD/'r-front-l17-old-command-audit-v1-20261023/results.json').read_text())
    assert len(old['boundaries'])==12 and len(old['rejected_controls'])==5
    assert sha(ROOT/'verilog/rtl/dorado_r_front_l17_step.sv')==SHA
    identity=json.loads((BUILD/'r-front-l17-recipe-identity-v1-20261023.json').read_text())
    assert identity['status']=='PASS_GENERATION_BYTE_IDENTITY' and identity['l17_sha256']==SHA
    paths={};directories=sorted(d for d in BUILD.glob('r-front-l17-*-20261023')if d.is_dir())
    suffixes={'.py','.sv','.v','.json','.log','.wl','.lc','.txt','.smt2','.rpt','.summary','.sdc','.qsf','.qpf','.tcl'}
    for d in directories:
        for p in d.rglob('run-state.json'):
            r=json.loads(p.read_text());assert r['status']!='RUNNING',('Observe existing job',p)
            assert all('returncode'in a for a in r.get('actions',[])),p
        for p in d.rglob('*'):
            if p.is_file() and not {'obj','db','incremental_db'}.intersection(p.relative_to(d).parts) and p.suffix in suffixes:
                paths['runs/'+d.name+'/'+str(p.relative_to(d))]=p
    for p in BUILD.glob('r-front-l17-*-20261023.json'):paths['followup/'+p.name]=p
    for p in BUILD.glob('r-front-l17-*-20261023-*.sv'):paths['generated/'+p.name]=p
    for n in ['generate_r_front_l17_candidate.py','generate_r_front_l17_reference.py','generate_r_front_mir_mask_candidate.py',
              'test_r_front_l17.py','test_r_front_l17_late.py','qualify_r_front_l17.py','review_r_front_l17.py',
              'audit_r_front_l17_boundaries.py','audit_r_front_command_boundaries.py','test_r_front_local.py',
              'test_r_front_mir_mask.py','archive_r_front_l17.py']:
        paths['current/tools/'+n]=ROOT/'tools'/n
    paths['current/verilog/rtl/dorado_r_front_l17_step.sv']=ROOT/'verilog/rtl/dorado_r_front_l17_step.sv'
    out.mkdir(parents=True)
    receipt=dict(scope=__doc__,candidate_sha256=SHA,members=[],blobs={},directories=[d.name for d in directories],
                 full_producer_arrival_contract_qualified=False,native_integrated=False,physical_timing_qualified=False,hardware_speedup_claimed=False)
    for name,p in sorted(paths.items()):
        raw=p.read_bytes();h=hashlib.sha256(raw).hexdigest()
        if h not in receipt['blobs']:
            stored='blobs/'+h+'.gz';target=out/stored;target.parent.mkdir(parents=True,exist_ok=True)
            target.write_bytes(gzip.compress(raw,mtime=0));receipt['blobs'][h]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=target.stat().st_size,stored_sha256=sha(target))
        receipt['members'].append(dict(name=name,blob=h))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(receipt,indent=2)+'\n').encode(),mtime=0));verify(out)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    archive(p.parse_args().output.resolve())
