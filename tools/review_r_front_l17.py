#!/usr/bin/env python3
"""Review terminal l17 runs using exact compiled bytes, preserving failed aggregation.

The initial aggregation checked a mutation name cleared by the parent driver's
namespace. Actual mutant bytes and original-circuit failures are authoritative.
No simulation is restarted and no earlier record is overwritten by this review.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
from test_r_front_local import PROFILES

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
SHA={None:'665188a29d07ba22981061233a825f496a479d577f5a6794a6b81cfac8c418d2',
     'external':'1f305a950c8cff5c7c7e8b384a4f025966ff87b5a740928cebb2d3301d351691',
     'clock0':'267f75e2f719e559ad97d90038c200fde3220c7b97d748b8e6b5ae4e332941e7'}


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def review(out):
    assert not out.exists()
    suite=BUILD/'r-front-l17-full-v1-20261023'
    old=json.loads((suite/'results.json').read_text())
    assert old['status']=='L17_ORDERING_FAILED' and len(old['failures'])==1
    assert old['failures'][0]['case']==['balanced','verilator','external']
    pilots={('balanced','icarus',None):BUILD/'r-front-l17-icarus-v1-20261023',
            ('balanced','verilator',None):BUILD/'r-front-l17-verilator-v1-20261023',
            ('balanced','verilator','external'):BUILD/'r-front-l17-external-verilator-v1-20261023'}
    cases=[(p,e,None)for p in PROFILES for e in ['icarus','verilator']]
    cases += [('balanced',e,m)for m in ['external','clock0']for e in ['icarus','verilator']]
    rows=[]
    for profile,engine,mutation in cases:
        d=pilots.get((profile,engine,mutation),suite/(profile+'-'+engine+'-'+(mutation or 'original')))
        r=json.loads((d/'run-state.json').read_text())
        assert r['status']!='RUNNING' and r['profile']==profile and r['engine']==engine
        assert len(r['actions'])==2 and r['actions'][0]['returncode']==0 and all('returncode'in a and 'finished'in a for a in r['actions'])
        assert r['executed_candidate_sha256']==SHA[mutation]==sha(d/'candidate.sv')
        for n,h in r['inputs'].items():assert sha(d/'inputs'/n)==h,(d,n)
        for n,h in r['timed_sources'].items():assert sha(d/'timed'/n)==h,(d,n)
        assert r['reference']['physical_bits']==798 and r['reference']['clock_channels']==69
        assert sha(d/'dorado_r_front_cone.sv')=='60d63614c927570d9fbed383e837955510fe10242a3ef74574ff485e0e6647c5'
        text=(d/'test.log').read_text();tb=(d/'inputs/tb_r_front_local.sv').read_text()
        for snippet in ['if(reference!==expected)','i<798','i<69','actual_memory!==reference_memory',
                        'if(n!==expected)','R front resolved source mismatch','R front clock mismatch']:
            assert snippet in tb,(d,snippet)
        row=dict(profile=profile,engine=engine,mutation=mutation,run=str(d.relative_to(BUILD)),
                 run_state_sha256=sha(d/'run-state.json'),candidate_sha256=SHA[mutation],
                 actions=r['actions'],reported_mutation_field=r.get('l17_mutation'))
        if mutation:
            assert r['status']=='R_FRONT_LOCAL_PROBE_FAILED' and r['actions'][-1]['returncode']!=0
            match=re.search(r'R front (?:state|clock) mismatch check=(\d+)',text);assert match,d
            assert 'R_FRONT_DIFF bit=789 'in text if mutation=='external'else'R_FRONT_DIFF bit=792 'in text
            row.update(result='REJECTED_'+mutation.upper(),first_failure_check=int(match[1]),
                       metadata_note='Old namespace field is cleared; qualified mutation is determined by exact executed bytes.')
        else:
            assert r['status']=='PASS_R_FRONT_LOCAL_PROBE' and r['actions'][-1]['returncode']==0
            assert r['summary']=='checks=13932 writes=328 reads=5800 epochs=797'
            assert 'R_FRONT_L17_DIRECTED changed=3f 'in text
            assert 'MIR_MASK_LATE_NODISPATCH root=0 MIRcaptures=1 SwitchUp_n=1'in text
            assert 'R_FRONT_COMMANDS_DIRECTED commands=8 FF=12 JCN=4 intervals=120 WriteIM=16'in text
            clocks=re.findall(r'^R_FRONT_LOCAL_CLOCK channel=(\d+) edges=(\d+)$',text,re.M)
            assert len(clocks)==69 and all(int(n)>0 for c,n in clocks)
            row.update(result='PASS',summary=r['summary'],clocks=clocks)
        rows.append(row)
    for p in PROFILES:
        pair=[r for r in rows if r['profile']==p and r['mutation']is None]
        assert len(pair)==2 and pair[0]['clocks']==pair[1]['clocks'],p
    result=dict(status='PASS_REVIEWED_BOUNDED_R_FRONT_L17_ORDERING',scope=__doc__,cases=rows,
                primary=[r for r in rows if r['result']=='PASS'],negative_cases=[r for r in rows if r['mutation']],
                retained_failed_aggregation_sha256=sha(suite/'results.json'),physical_bits=798,ram_bits=9216,clock_channels=69,
                full_producer_arrival_contract_qualified=False,native_integrated=False,physical_timing_qualified=False,hardware_speedup_claimed=False)
    out.mkdir(parents=True);(out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(result['status'],len(result['primary']),len(result['negative_cases']))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    review(p.parse_args().output.resolve())
