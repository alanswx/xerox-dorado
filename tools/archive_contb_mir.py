#!/usr/bin/env python3
"""Preserve bounded ContB comparisons, original-source seams and known gaps.

Keep all failed earlier comparisons and their exact inputs. The final bounded
matrix passes six cases and rejects twelve wrong variants, while two combined
handoff cases and two stable-data diagnostics expose the unqualified arrival
gap. No native circuit or hardware speed qualification is implied.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import tempfile
from archive_r_front_ready_followup import verify
from generate_contb_mir_candidate import generate

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
CANDIDATE_SHA='a793d9bb15bb12bdab9b0048b94ef69462ac668498b020b890a952ebed102fea'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def archive(out):
    assert not out.exists()
    review=json.loads((BUILD/'contb-mir-review-v3-20261023/results.json').read_text())
    assert review['status']=='PASS_BOUNDED_CONTB_WITH_HANDOFF_GAP'and not review['failures']
    assert (len(review['primary']),len(review['negative_cases']),len(review['handoff_gap']))==(6,12,2)
    for row in review['primary']+review['handoff_gap']:assert row['candidate_sha256']==CANDIDATE_SHA
    stable=[]
    for engine in ['icarus','verilator']:
        d=BUILD/('contb-mir-handoff-stable-'+engine+'-v1-20261023');r=json.loads((d/'run-state.json').read_text())
        assert r['status']=='CONTB_MIR_PROBE_FAILED'and len(r['actions'])==2
        assert r['actions'][0]['returncode']==0 and r['actions'][1]['returncode']!=0
        assert r['executed_candidate_sha256']==CANDIDATE_SHA
        text=(d/'test.log').read_text();assert 'CONTB_STABLE_HANDOFF raw_IM_stable=1'in text
        failure=re.search(r'ContB MIR state mismatch check=35 before=([0-9a-f]+) after=([0-9a-f]+)',text)
        assert failure,text
        before,after=[int(x,16)for x in failure.groups()]
        assert before^after==9 and before&(1<<3)and not before&1
        clocks=re.findall(r'^CONTB_CLOCK_DIFF check=35 channel=(\d+) actual=1 expected=0$',text,re.M)
        assert {int(x)for x in clocks}==set(range(22))-{0,17}
        stable.append(dict(engine=engine,run=d.name,run_state_sha256=sha(d/'run-state.json'),failure_check=35,
                           raw_instruction_stable=True,only_changed_inputs=['base LOW -> HIGH','CBHold HIGH -> LOW'],
                           unexpected_clock_channels=list(map(int,clocks))))
    b=json.loads((BUILD/'contb-mir-boundaries-v3-20261023/results.json').read_text())
    assert b['status']=='PASS_SELECTED_CONTB_CLOCK_RESET_BOUNDARIES'
    assert len(b['unused_boundaries'])==10 and len(b['rejected_controls'])==5
    with tempfile.TemporaryDirectory(prefix='dorado-contb-recipe-')as name:
        generated=Path(name)/'candidate.sv';generate(generated);assert sha(generated)==CANDIDATE_SHA
    paths={};directories=sorted(d for d in BUILD.glob('contb-mir-*-20261023')if d.is_dir())
    suffixes={'.py','.sv','.v','.json','.log','.wl','.lc','.txt'}
    for d in directories:
        for p in d.rglob('run-state.json'):
            r=json.loads(p.read_text());assert not r['status'].startswith('RUNNING'),('Observe existing job',p)
            assert all('returncode'in a for a in r.get('actions',[])),p
            for n,h in r.get('inputs',{}).items():assert sha(p.parent/'inputs'/n)==h,(p,n)
            for n,h in r.get('compiled_sources',{}).items():assert sha(p.parent/n)==h,(p,n)
        for p in d.rglob('*'):
            if p.is_file()and 'obj'not in p.relative_to(d).parts and p.suffix in suffixes:
                paths['runs/'+d.name+'/'+str(p.relative_to(d))]=p
    for n in ['generate_contb_mir_candidate.py','generate_contb_mir_reference.py','test_contb_mir.py',
              'qualify_contb_mir.py','audit_contb_mir_boundaries.py','test_contb_mir_handoff.py','archive_contb_mir.py']:
        paths['current/tools/'+n]=ROOT/'tools'/n
    out.mkdir(parents=True)
    receipt=dict(scope=__doc__,candidate_sha256=CANDIDATE_SHA,members=[],blobs={},directories=[d.name for d in directories],
                 full_producer_arrival_contract_qualified=False,native_integrated=False,physical_timing_qualified=False,hardware_speedup_claimed=False)
    for name,p in sorted(paths.items()):
        raw=p.read_bytes();h=hashlib.sha256(raw).hexdigest()
        if h not in receipt['blobs']:
            stored='blobs/'+h+'.gz';target=out/stored;target.parent.mkdir(parents=True,exist_ok=True)
            target.write_bytes(gzip.compress(raw,mtime=0));receipt['blobs'][h]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=target.stat().st_size,stored_sha256=sha(target))
        receipt['members'].append(dict(name=name,blob=h))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(receipt,indent=2)+'\n').encode(),mtime=0));verify(out)
    qualification=dict(status='BOUNDED_CONTB_COMPARISON_WITH_KNOWN_HANDOFF_GAP',candidate_sha256=CANDIDATE_SHA,
                       summary='4961 events per positive case;40 original Qs;25 clocks;26 flags;all CP-wired MIR bits set/cleared',
                       positive_cases=6,rejected_logic_cases=12,combined_handoff_failures=2,stable_data_handoff_failures=stable,
                       selected_boundary_audit=dict(unused=10,rejected_live_controls=5,ContB_pin_checks=b['physical_pin_checks'],MemC_pin_checks=b['memory_producer']['physical_pin_checks']),
                       failed_predecessors=['v1 review missed CP-selector mutation; MIR already set','v2 review exposed wrong BSEL.2 CP selection',
                                            'v4 Icarus pilot exposed wrong ALUF.2 CP selection'],
                       full_producer_arrival_contract_qualified=False,native_integrated=False,physical_timing_qualified=False,hardware_speedup_claimed=False)
    (out/'qualification.json').write_text(json.dumps(qualification,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    archive(p.parse_args().output.resolve())
