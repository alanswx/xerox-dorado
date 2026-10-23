#!/usr/bin/env python3
"""Review and preserve conditional control transfers, not native clock generation.

All four existing illustrative delay profiles compare in both engines when
the direct RunRefresh-Set arrival order is supplied independently. Preserve
the failed root-only matrix and reject opposite arrival and new-data epochs.
This does not qualify the FPGA phase producer or physical timing.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import tempfile

from archive_r_front_ready_followup import verify
from audit_control_hold_wiring import functions,run as wiring_audit
from generate_control_hold_ordered import generate
from review_control_hold import read_run
from sil_netlist import load_board
from test_control_hold_ordered import descriptor

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
SHA='23f60f6def48b1a9d6a9a84542afaafce82c038d41cfda7008a5f0961f5504fe'
BASE_SHA='b79903a970ad7e4bd4ec79020f3d43f542dc9a71b412b21a2f4f39365528323e'
PROFILES=['balanced','fast_gates','later_control','ca_later_control']


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def review(out):
    assert not out.exists()
    original=ROOT/'fpga/mister/reports/2026-10-23-control-hold/experiment'
    original_summary=json.loads((original/'summary.json').read_text())
    assert original_summary['status']=='INCOMPLETE_JOINT_CONTROL_HOLD'
    assert original_summary['required_profile_failures']==2
    rows=[];dirs=[]
    for profile in PROFILES:
        for engine in ['icarus','verilator']:
            name=f'control-hold-ordered-{profile}-{engine}-v3-20261023'
            if profile=='later_control'and engine=='icarus':
                name='control-hold-ordered-early-icarus-v2-20261023'
            d=BUILD/name;r,log=read_run(d);dirs.append(d)
            assert r['status']=='PASS_CONTROL_HOLD_PROBE'and r['actions'][-1]['returncode']==0
            assert r['engine']==engine and r['profile']==profile and r['forced_arrival']is None
            assert r['executed_candidate_sha256']==SHA and not r['root_only_clock_generation_qualified']
            assert r['executed_control_arrival']==descriptor(profile)==r['expected_control_arrival']
            assert r['summary'].startswith('checks=5678 physical=156 clocks=56 changed=')
            clocks=re.findall(r'^CONTROL_HOLD_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
            assert len(clocks)==56 and [int(c)for c,n in clocks]==list(range(56))
            assert all(int(n)>=8 for c,n in clocks)
            rows.append(dict(run=name,result='PASS_CONDITIONAL_CONTROL_TRANSFER',engine=engine,profile=profile,
                             arrival=descriptor(profile),summary=r['summary'],clocks=clocks,run_state_sha256=sha(d/'run-state.json')))
        pair=rows[-2:];assert pair[0]['clocks']==pair[1]['clocks']and pair[0]['summary']==pair[1]['summary']
    negatives=[]
    for kind,profile,check in [('wrong','later_control',403),('wrong','balanced',403),('new-Want','later_control',331)]:
        for engine in ['icarus','verilator']:
            name=f'control-hold-ordered-{kind}-'+(f'{profile}-'if kind=='wrong'else'')+f'{engine}-v3-20261023'
            d=BUILD/name;r,log=read_run(d);dirs.append(d)
            assert r['status']=='CONTROL_HOLD_PROBE_FAILED'and r['actions'][-1]['returncode']!=0
            assert r['engine']==engine and r['profile']==profile
            assert re.search(r'Control Hold state mismatch check='+str(check)+r' root=0->1',log)
            bits=re.findall(r'CONTROL_HOLD_DIFF bit=(\d+) ',log);assert bits==['14','15'],bits
            if kind=='wrong':
                assert r['forced_arrival']==1-descriptor(profile)
                assert r['executed_candidate_sha256']==SHA
                assert r['executed_control_arrival']==r['forced_arrival']
            else:
                assert r['mutation']=='new_Want_data'and r['executed_candidate_sha256']!=SHA
                assert r['forced_arrival']is None and r['executed_control_arrival']==descriptor(profile)
            negatives.append(dict(run=name,result='REJECTED_WRONG_ARRIVAL'if kind=='wrong'else'REJECTED_NEW_DATA_EPOCH',
                                  engine=engine,profile=profile,check=check,bits=[14,15],run_state_sha256=sha(d/'run-state.json')))
    out.mkdir(parents=True)
    with tempfile.TemporaryDirectory(prefix='dorado-control-order-review-')as name:
        p=Path(name)/'candidate.sv';generate(p);assert sha(p)==SHA
        base=p.with_name('candidate-root-baseline.sv');assert sha(base)==BASE_SHA
        bf,cf=functions(base.read_text()),functions(p.read_text())
        assert set(cf)==set(bf)|{'controller_transfer_ordered'}
        assert all(bf[n]==cf[n]for n in bf)
        h=cf['controller_transfer_ordered']
        assert set(re.findall(r'n\[(\d+)\]\s*=',h))=={'14','15'}
        assert 'n=controller_transfer(q,captures,pins);'in h and 'if(refresh_early && captures[2])'in h
        assert 'refresh_at_capture=q[19];'in h and 'd_mem_run=q[5] | (q[19] & !q[21]);'in h
        wiring_audit(dirs[0]/'reference',p,out/'inherited-wiring')
    b=load_board(str(ROOT/'chm/sil/ContA-Rev-Cd.dm!1_/ContA-Rev-Cd.wl'))
    pins={(p['pkg'],p['pin']):n for n,r in b.nets.items()for p in r['pins']}
    checks={('g04',12):'WantRunRfsh',('g04',13):"RunRefresh'",('g04',15):'dMemRun',
            ('h03',2):'dMemRun',('h03',4):'dRun',('h03',3):'ClearMemStop',('h03',6):'rStop',('h03',7):'RunRefresh',
            ('l03',7):'dMemRun',('l03',10):'dMemRun',('l03',5):'RunRefresh',('l03',12):'RunRefresh',
            ('l03',9):"preRunClk'Bc",('f13',14):"preRunClk'Bc",('l04',4):'ClearMemStop',('l04',13):'ClearMemStop'}
    assert all(pins[k]==n for k,n in checks.items())
    assert b.packages['g04']['type']=='MC10104'and b.packages['h03']['type']=='MC10103'
    r=dict(status='PASS_CONDITIONAL_CONTROL_TRANSFER',scope=__doc__,candidate_sha256=SHA,baseline_sha256=BASE_SHA,
           primary=rows,negative_cases=negatives,physical_bits=156,clock_channels=56,selected_flags=94,
           original_root_only_status=original_summary['status'],original_root_only_required_failures=2,
           unchanged_baseline_functions=len(bf),additional_route_pin_checks=len(checks),
           descriptor_source='Independently declared phase FF delay < f13 gate delay; not read from reference outputs.',
           data_epoch='g04/h03 data and h03 Clear paths retain old operands under these profiles.',
           promoted_to_maintained_RTL=False,native_integrated=False,physical_timing_qualified=False,
           root_only_clock_generation_qualified=False,hardware_speedup_claimed=False,
           next='Supply the arrival contract from the actual FPGA clock partition; integrate the full processor and close real IM/parity/MCR/hold-data/DMD/B-bus producers.')
    (out/'results.json').write_text(json.dumps(r,indent=2)+'\n')
    print(r['status'],'8 complete comparisons;6 wrong arrival/data rejects;native phase generation remains open')
    return r,dirs


def archive(out,review_dir):
    assert not out.exists();result=json.loads((review_dir/'results.json').read_text())
    assert result['status']=='PASS_CONDITIONAL_CONTROL_TRANSFER'
    names=[r['run']for r in result['primary']+result['negative_cases']]
    # Preserve the failed first two-descriptor prototype, including exact bytes.
    names+=['control-hold-ordered-early-icarus-v1-20261023']
    paths={}
    for name in names:
        d=BUILD/name;read_run(d)
        for p in d.rglob('*'):
            if p.is_file()and 'obj'not in p.relative_to(d).parts and p.suffix in {'.py','.sv','.v','.json','.log','.wl','.lc','.txt'}:
                paths['runs/'+name+'/'+str(p.relative_to(d))]=p
    for p in review_dir.rglob('*'):
        if p.is_file():paths['review/'+str(p.relative_to(review_dir))]=p
    for name in ['generate_control_hold_ordered.py','test_control_hold_ordered.py',Path(__file__).name]:
        paths['current/tools/'+name]=ROOT/'tools'/name
    out.mkdir(parents=True);receipt=dict(status=result['status'],scope=__doc__,members=[],blobs={},
        candidate_sha256=SHA,native_integrated=False,physical_timing_qualified=False,root_only_clock_generation_qualified=False)
    for name,p in sorted(paths.items()):
        raw=p.read_bytes();h=hashlib.sha256(raw).hexdigest()
        if h not in receipt['blobs']:
            stored='blobs/'+h+'.gz';target=out/stored;target.parent.mkdir(parents=True,exist_ok=True)
            target.write_bytes(gzip.compress(raw,mtime=0))
            receipt['blobs'][h]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=target.stat().st_size,stored_sha256=sha(target))
        receipt['members'].append(dict(name=name,blob=h))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(receipt,indent=2)+'\n').encode(),mtime=0))
    (out/'summary.json').write_text(json.dumps(result,indent=2)+'\n');verify(out)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--archive',type=Path)
    a=p.parse_args();review(a.output.resolve())
    if a.archive:archive(a.archive.resolve(),a.output.resolve())
