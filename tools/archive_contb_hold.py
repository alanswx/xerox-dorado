#!/usr/bin/env python3
"""Preserve coupled memory-produced Hold ordering and exact failed controls.

Six full profile/engine comparisons pass all46 Qs/31 clocks/34 flags and96
held-data releases each;14 wrong variants fail. No full producer closure,
native integration, physical timing or hardware speed claim is made.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import tempfile
from archive_r_front_ready_followup import verify
from generate_contb_hold_candidate import generate

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
SHA='bd23230a24789c1544e119aaa828bf8c39d12113c8517c2faeede686fb418c90'
CB_SHA='a793d9bb15bb12bdab9b0048b94ef69462ac668498b020b890a952ebed102fea'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def archive(out):
    assert not out.exists();review_dir=BUILD/'contb-hold-review-v1-20261023'
    r=json.loads((review_dir/'results.json').read_text())
    assert r['status']=='PASS_BOUNDED_MEMORY_PRODUCED_HOLD'and not r['failures']
    assert (len(r['primary']),len(r['negative_cases']))==(6,14)
    for row in r['cases']:
        d=(review_dir/row['directory']).resolve();assert sha(d/'run-state.json')==row['run_state_sha256']
        run=json.loads((d/'run-state.json').read_text());assert len(run['actions'])==2 and run['actions'][0]['returncode']==0
        assert all('returncode'in a for a in run['actions'])and run['status']!='RUNNING'
        for n,h in run['inputs'].items():assert sha(d/'inputs'/n)==h,(d,n)
        for n,h in run['compiled_sources'].items():assert sha(d/n)==h,(d,n)
        for n,h in run['timing']['files'].items():assert sha(d/'timed'/n)==h,(d,n)
        assert sha(d/'candidate-cb.sv')==CB_SHA
        if row in r['primary']:assert sha(d/'candidate.sv')==SHA
    boundary=json.loads((BUILD/'contb-hold-boundaries-v1-20261023/results.json').read_text())
    assert boundary['status']=='PASS_SELECTED_MEMC_PHASE_HOLD_BOUNDARIES'
    assert len(boundary['unused_boundaries'])==8 and len(boundary['rejected_controls'])==5
    for name,h in [('dorado_contb_hold_step.sv',SHA),('dorado_contb_mir_step.sv',CB_SHA)]:assert sha(ROOT/'verilog/rtl'/name)==h
    with tempfile.TemporaryDirectory(prefix='dorado-contb-hold-recipe-')as name:
        p=Path(name)/'candidate.sv';generate(p,72);assert sha(p)==SHA and sha(p.with_name('candidate-cb.sv'))==CB_SHA
    paths={};directories=sorted(d for d in BUILD.glob('contb-hold-*-20261023')if d.is_dir())
    for d in directories:
        for p in d.rglob('run-state.json'):
            run=json.loads(p.read_text());assert not run['status'].startswith('RUNNING'),('Observe existing job',p)
            assert all('returncode'in a for a in run.get('actions',[])),p
        for p in d.rglob('*'):
            if p.is_file()and 'obj'not in p.relative_to(d).parts and p.suffix in {'.py','.sv','.v','.json','.log','.wl','.lc','.txt'}:
                paths['runs/'+d.name+'/'+str(p.relative_to(d))]=p
    for n in ['generate_contb_hold_reference.py','generate_contb_hold_candidate.py','test_contb_hold.py','qualify_contb_hold.py','audit_contb_hold_boundaries.py','archive_contb_hold.py']:
        paths['current/tools/'+n]=ROOT/'tools'/n
    for n in ['dorado_contb_hold_step.sv','dorado_contb_mir_step.sv']:paths['current/verilog/rtl/'+n]=ROOT/'verilog/rtl'/n
    out.mkdir(parents=True);receipt=dict(scope=__doc__,candidate_sha256=SHA,ContB_helper_sha256=CB_SHA,members=[],blobs={},
        full_producer_arrival_contract_qualified=False,native_integrated=False,physical_timing_qualified=False,hardware_speedup_claimed=False)
    for name,p in sorted(paths.items()):
        raw=p.read_bytes();h=hashlib.sha256(raw).hexdigest()
        if h not in receipt['blobs']:
            stored='blobs/'+h+'.gz';target=out/stored;target.parent.mkdir(parents=True,exist_ok=True)
            target.write_bytes(gzip.compress(raw,mtime=0));receipt['blobs'][h]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=target.stat().st_size,stored_sha256=sha(target))
        receipt['members'].append(dict(name=name,blob=h))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(receipt,indent=2)+'\n').encode(),mtime=0));verify(out)
    qualification=dict(status='PASS_BOUNDED_MEMORY_PRODUCED_HOLD',candidate_sha256=SHA,ContB_helper_sha256=CB_SHA,
        summary='7260 events per pass;46 original Qs;31 clocks;34 flags;96 held-data releases through three real Hold sources',
        primary=[dict(profile=x['profile'],engine=x['engine'],summary=x['summary'])for x in r['primary']],
        negative_cases=[dict(mutation=x['mutation'],engine=x['engine'],check=x['first_failure_check'])for x in r['negative_cases']],
        boundary_audit=dict(unused=8,rejected_controls=5,pin_checks=boundary['physical_pin_checks']),
        original_baseboard_fanout_pin_checks=9,recipe_byte_identity=True,
        observation_contract='CA enables and MemSH change at a HIGH root; isolated LOW IgnoreProc and DisHold transitions are also tested. Upstream producer arrival closure remains open.',
        unchanged_standalone_counterexamples='65affeb0a original raw-CBHold handoff failures remain valid; the coupled comparisons use real memory-produced Hold.',
        remaining_producers=boundary['remaining_producers'],
        native_integrated=False,physical_timing_qualified=False,full_producer_arrival_contract_qualified=False,hardware_speedup_claimed=False)
    (out/'qualification.json').write_text(json.dumps(qualification,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    archive(p.parse_args().output.resolve())
