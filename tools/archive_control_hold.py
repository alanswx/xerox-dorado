#!/usr/bin/env python3
"""Preserve the unpromoted joint-control integration and arrival counterexamples.

The required matrix remains failed. Archive six bounded passes, two required
profile failures, fourteen wrong variants, two parity gaps and five causal
probes, with exact compiled inputs and failed fixture/test predecessors.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import tempfile
from archive_r_front_ready_followup import verify
from generate_control_hold_candidate import generate

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
SHA='b79903a970ad7e4bd4ec79020f3d43f542dc9a71b412b21a2f4f39365528323e'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def archive(out):
    assert not out.exists();review=json.loads((BUILD/'control-hold-reviewed-v1-20261023/results.json').read_text())
    assert review['status']=='INCOMPLETE_JOINT_CONTROL_HOLD'and review['evidence_review_passed']
    assert len(review['primary'])==6 and len(review['required_arrival_failures'])==2 and len(review['causal_probes'])==5
    assert review['wrong_variants']==14 and review['raw_parity_handoff_gaps']==2
    for row in review['cases']+review['causal_probes']:
        d=BUILD/row['run'];assert sha(d/'run-state.json')==row['run_state_sha256']
        r=json.loads((d/'run-state.json').read_text());assert r['status']!='RUNNING'and all('returncode'in a for a in r['actions'])
        for n,h in r['inputs'].items():assert sha(d/'inputs'/n)==h,(d,n)
        for n,h in r['compiled_sources'].items():assert sha(d/n)==h,(d,n)
        for n,h in r['timing']['files'].items():assert sha(d/'timed'/n)==h,(d,n)
    with tempfile.TemporaryDirectory(prefix='dorado-control-hold-recipe-')as name:
        p=Path(name)/'candidate.sv';generate(p);assert sha(p)==SHA
    paths={};directories=sorted({d for prefix in ['control-hold-','control-refresh-']for d in BUILD.glob(prefix+'*-20261023')if d.is_dir()})
    for d in directories:
        for p in d.rglob('run-state.json'):
            r=json.loads(p.read_text());assert not r['status'].startswith('RUNNING'),('Observe existing job',p)
            assert all('returncode'in a for a in r.get('actions',[])),p
        for p in d.rglob('*'):
            if p.is_file()and 'obj'not in p.relative_to(d).parts and p.suffix in {'.py','.sv','.v','.json','.log','.wl','.lc','.txt'}:
                paths['runs/'+d.name+'/'+str(p.relative_to(d))]=p
    for n in ['generate_control_hold_reference.py','generate_control_hold_candidate.py','test_control_hold.py','qualify_control_hold.py',
              'test_control_refresh.py','review_control_hold.py','audit_control_hold_wiring.py','archive_control_hold.py']:
        paths['current/tools/'+n]=ROOT/'tools'/n
    # Keep the bounded observer's ended state and immutable new observer recipe.
    # Native test logs/inputs remain live and are never copied or changed here.
    for name in ['native-firmware-window-watch-v2-20261023/state.json','watch-native-firmware-windows-v2-20261023.py','watch-native-firmware-windows-v3-20261023.py']:
        p=BUILD/name
        if p.exists():paths['observer/'+name]=p
    out.mkdir(parents=True);receipt=dict(scope=__doc__,candidate_sha256=SHA,members=[],blobs={},status='INCOMPLETE_JOINT_CONTROL_HOLD',
        promoted_to_maintained_RTL=False,native_integrated=False,physical_timing_qualified=False,hardware_speedup_claimed=False)
    for name,p in sorted(paths.items()):
        raw=p.read_bytes();h=hashlib.sha256(raw).hexdigest()
        if h not in receipt['blobs']:
            stored='blobs/'+h+'.gz';target=out/stored;target.parent.mkdir(parents=True,exist_ok=True)
            target.write_bytes(gzip.compress(raw,mtime=0));receipt['blobs'][h]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=target.stat().st_size,stored_sha256=sha(target))
        receipt['members'].append(dict(name=name,blob=h))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(receipt,indent=2)+'\n').encode(),mtime=0));verify(out)
    summary=dict(status='INCOMPLETE_JOINT_CONTROL_HOLD',candidate_sha256=SHA,
        bounded_passes=6,required_profile_failures=2,wrong_variants_rejected=14,raw_parity_handoff_failures=2,causal_probes=5,
        physical_bits=156,clock_channels=56,selected_flags=94,events_per_full_pass=5678,
        inherited_functions_unchanged=24,physical_pin_checks=34,source_recipe_byte_identity=True,
        required_failure=dict(profile='later_control',gate_ps=1750,FF_ps=400,phase_FF_ps=1250,check=403,bits=[14,15],extra_clock=51),
        causal_result='Source-only replay fails; holding WantRunRfsh or delaying only its phase FF removes the mismatch. Additional Motorola clock-part-range example also fails.',
        original_failed_matrix_preserved=True,promoted_to_maintained_RTL=False,
        native_integrated=False,physical_timing_qualified=False,full_producer_arrival_contract_qualified=False,hardware_speedup_claimed=False)
    (out/'summary.json').write_text(json.dumps(summary,indent=2)+'\n')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    archive(p.parse_args().output.resolve())
