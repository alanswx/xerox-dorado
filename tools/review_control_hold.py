#!/usr/bin/env python3
"""Review exact joint-control evidence without promoting a failed arrival gate.

Six complete bounded comparisons pass; the required early-phase profile fails
in both engines. Keep the original failed matrix, fourteen wrong-source
rejects, two raw-parity gaps and five causal refresh probes unchanged.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
from test_control_refresh import CAPTURED_STATE

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
SHA='b79903a970ad7e4bd4ec79020f3d43f542dc9a71b412b21a2f4f39365528323e'


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def read_run(d):
    r=json.loads((d/'run-state.json').read_text())
    assert r['status']!='RUNNING'and len(r['actions'])==2 and r['actions'][0]['returncode']==0,d
    assert all('returncode'in a for a in r['actions']),d
    for n,h in r['inputs'].items():assert sha(d/'inputs'/n)==h,(d,n)
    for n,h in r['compiled_sources'].items():assert sha(d/n)==h,(d,n)
    for n,h in r['timing']['files'].items():assert sha(d/'timed'/n)==h,(d,n)
    assert sha(d/'candidate.sv')==r['executed_candidate_sha256']and sha(d/'tb.sv')==r['testbench_sha256']
    return r,(d/'test.log').read_text()


def run(out):
    assert not out.exists();matrix_dir=BUILD/'control-hold-review-v1-20261023';matrix=json.loads((matrix_dir/'results.json').read_text())
    assert matrix['status']=='JOINT_CONTROL_HOLD_REVIEW_FAILED'
    assert len(matrix['primary'])==4 and len(matrix['negative_cases'])==14 and len(matrix['parity_gap_cases'])==2
    rows=[]
    for row in matrix['cases']:
        d=(matrix_dir/row['directory']).resolve();r,text=read_run(d)
        reviewed=dict(run=d.name if d.parent==BUILD else str(d.relative_to(BUILD)),engine=r['engine'],profile=r['profile'],
                      candidate_sha256=r['executed_candidate_sha256'],run_state_sha256=sha(d/'run-state.json'))
        if r['profile']=='later_control'and not r.get('mutation')and not r.get('parity_handoff'):
            assert r['status']=='CONTROL_HOLD_PROBE_FAILED'and r['actions'][-1]['returncode']!=0
            assert r['executed_candidate_sha256']==SHA
            f=re.search(r'Control Hold state mismatch check=403 root=0->1 old=([0-9a-f]+) expected=([0-9a-f]+) original=([0-9a-f]+)',text);assert f,text
            old,expected,actual=map(lambda n:int(n,16),f.groups())
            assert old==CAPTURED_STATE and expected^actual==(1<<14)|(1<<15)
            assert expected&(3<<14)==3<<14 and actual&(3<<14)==0
            clocks=re.findall(r'CONTROL_HOLD_CLOCK_DIFF check=403 channel=(\d+) actual=1 predicted=0',text)
            assert clocks==['51'],clocks
            reviewed.update(result='FAIL_REQUIRED_EARLY_PHASE_PROFILE',check=403,differing_physical_bits=[14,15],extra_clock=51)
        else:
            assert sha(d/'run-state.json')==row['run_state_sha256'],d
            reviewed['result']=row['qualification']
            if row['qualification']=='PASS_BOUNDED_JOINT_CONTROL_HOLD':reviewed.update(summary=r['summary'],clocks=row['clocks'])
            if row['qualification']=='EXPOSED_RAW_PARITY_HANDOFF_GAP':reviewed['check']=row['first_failure_check']
        rows.append(reviewed)
    for engine in ['icarus','verilator']:
        d=BUILD/('control-hold-ca-later-'+engine+'-v1-20261023');r,text=read_run(d)
        assert r['status']=='PASS_CONTROL_HOLD_PROBE'and r['executed_candidate_sha256']==SHA
        assert r['summary'].startswith('checks=5678 physical=156 clocks=56 changed=')
        clocks=re.findall(r'^CONTROL_HOLD_CLOCK channel=(\d+) edges=(\d+)$',text,re.M);assert len(clocks)==56 and all(int(n)>=8 for c,n in clocks)
        rows.append(dict(run=d.name,engine=engine,profile=r['profile'],result='PASS_BOUNDED_JOINT_CONTROL_HOLD',
                         candidate_sha256=SHA,run_state_sha256=sha(d/'run-state.json'),summary=r['summary'],clocks=clocks))
    primary=[r for r in rows if r['result']=='PASS_BOUNDED_JOINT_CONTROL_HOLD'];assert len(primary)==6
    for profile in ['balanced','fast_gates','ca_later_control']:
        pair=[r for r in primary if r['profile']==profile];assert len(pair)==2 and pair[0]['clocks']==pair[1]['clocks']
    causal=[]
    for name,success in [('early-release-icarus',False),('early-held-icarus',True),('delayed-release-icarus',True),
                         ('bounded-release-icarus',False),('early-release-verilator',False)]:
        d=BUILD/('control-refresh-'+name+'-v1-20261023');r,text=read_run(d);assert r['executed_candidate_sha256']==SHA
        assert 'CONTROL_HOLD_REFRESH source_only_edge=1'in text
        if success:
            assert r['status']=='PASS_CONTROL_HOLD_PROBE'and r['actions'][-1]['returncode']==0
        else:
            assert r['status']=='CONTROL_HOLD_PROBE_FAILED'and r['actions'][-1]['returncode']!=0
            assert 'Control Hold state mismatch check=1 root=0->1'in text
            assert 'CONTROL_HOLD_DIFF bit=14 old=1 expected=1 original=0'in text and 'CONTROL_HOLD_DIFF bit=15 old=1 expected=1 original=0'in text
        causal.append(dict(run=d.name,result='MATCH'if success else'REFRESH_RELEASE_MISMATCH',timing=r['timing'],
                           run_state_sha256=sha(d/'run-state.json'),summary=r['summary']))
    wiring=json.loads((BUILD/'control-hold-wiring-v1-20261023/results.json').read_text())
    assert wiring['status']=='PASS_JOINT_CONTROL_SOURCE_WIRING'and len(wiring['inherited_functions'])==24
    out.mkdir(parents=True)
    r=dict(status='INCOMPLETE_JOINT_CONTROL_HOLD',evidence_review_passed=True,candidate_sha256=SHA,scope=__doc__,cases=rows,
           primary=primary,required_arrival_failures=[x for x in rows if x['result']=='FAIL_REQUIRED_EARLY_PHASE_PROFILE'],
           wrong_variants=14,raw_parity_handoff_gaps=2,causal_probes=causal,wirelist_pin_checks=34,source_functions_unchanged=24,
           original_matrix_status_unchanged=matrix['status'],
           next='Establish the original RunRefresh Set-release/capture contract and correct the shared control epoch before promoting this joint component; close real IM/parity and MCR/hold data, then full processor/native phases.',
           native_integrated=False,physical_timing_qualified=False,full_producer_arrival_contract_qualified=False,hardware_speedup_claimed=False)
    (out/'results.json').write_text(json.dumps(r,indent=2)+'\n');print(r['status'],'6 bounded passes;2 required profile failures;14 wrong variants;2 parity gaps;5 causal probes')


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    run(p.parse_args().output.resolve())
