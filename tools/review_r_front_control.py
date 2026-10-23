#!/usr/bin/env python3
"""Preserve the unpromoted 844-Q composition and its RBase closing-window gap.

Balanced processor writes/reads compare in both engines. Required early-phase
stress fails in both; a phase-delay-only diagnostic removes the mismatch.
No native, full directed-test, physical memory timing or speed qualification.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import tempfile

from archive_r_front_ready_followup import verify
from generate_r_front_control_candidate import generate
from sil_netlist import load_board

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'
NAMES=['r-front-control-'+e+'-v'+v+'-20261023'for v in ['1','2']for e in ['icarus','verilator']]
NAMES+=['r-front-control-'+kind+'-'+e+'-v3-20261023'for kind in ['early_refresh','new_fields']for e in ['icarus','verilator']]
NAMES+=['r-front-control-early-trace-verilator-v4-20261023','r-front-control-late-phase-verilator-v4-20261023']


def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()


def read_run(d):
    r=json.loads((d/'run-state.json').read_text());assert r['status']!='RUNNING'
    assert len(r['actions'])==2 and r['actions'][0]['returncode']==0
    assert all('returncode'in a for a in r['actions'])
    assert sha(d/'candidate.sv')==r['executed_candidate_sha256']
    assert r['reference']['physical_bits']==844 and r['reference']['clock_channels']==100
    for n,h in r['inputs'].items():assert sha(d/'inputs'/n)==h,(d,n)
    for n,h in r['timed_sources'].items():assert sha(d/'timed'/n)==h,(d,n)
    for row in r['adapters']['files']:assert sha(d/'adapters'/row['output'])==row['output_sha256'],(d,row['output'])
    assert sha(d/'dorado_r_front_cone.sv')==r['executed_cone_sha256']
    return r,(d/'test.log').read_text()


def review(out):
    assert not out.exists();rows=[];primary=[]
    with tempfile.TemporaryDirectory(prefix='dorado-full-control-review-')as name:
        p=Path(name)/'candidate.sv';meta=generate(p);candidate_sha=sha(p)
    for name in NAMES:
        d=BUILD/name;r,log=read_run(d)
        row=dict(run=name,engine=r['engine'],profile=r['profile'],summary=r['summary'],
                 candidate_sha256=r['executed_candidate_sha256'],run_state_sha256=sha(d/'run-state.json'))
        if r.get('joint_mutation')=='new_fields':
            assert r['status']=='R_FRONT_LOCAL_PROBE_FAILED'and r['actions'][-1]['returncode']!=0
            assert 'R front state mismatch check=131'in log
            assert re.findall(r'R_FRONT_DIFF bit=(\d+) ',log)==['38','54']
            row.update(result='REJECTED_NEW_INSTRUCTION_CAPTURE_FIELDS',check=131,bits=[38,54])
        elif r['profile']=='early_refresh':
            assert r['status']=='R_FRONT_LOCAL_PROBE_FAILED'and r['actions'][-1]['returncode']!=0
            assert r['executed_candidate_sha256']==candidate_sha
            assert 'R front state mismatch check=1456'in log
            assert re.findall(r'R_FRONT_DIFF bit=(\d+) ',log)==['644','645','646','647']
            row.update(result='FAIL_REQUIRED_RBASE_WRITE_WINDOW',check=1456,bits=[644,645,646,647])
        else:
            assert r['status']=='PASS_R_FRONT_LOCAL_PROBE'and r['actions'][-1]['returncode']==0
            assert r['executed_candidate_sha256']==candidate_sha
            if '-v1-'in name:
                assert r['summary']=='checks=2112 writes=0 reads=538 epochs=120'
                row['result']='PASS_READ_ONLY_PILOT_INSUFFICIENT_WRITE_COVERAGE'
            else:
                assert r['summary']=='checks=4168 writes=242 reads=1542 epochs=74'
                assert 'R_FRONT_CONTROL_LC actual_instruction_inputs=8 writes=156 reads=282'in log
                clocks=re.findall(r'^R_FRONT_CONTROL_CLOCK channel=(\d+) edges=(\d+)$',log,re.M)
                assert len(clocks)==100 and [int(c)for c,n in clocks]==list(range(100))
                row.update(result='PASS_BOUNDED_WRITE_READ_PILOT',clocks=clocks,
                           unexercised_clocks=[int(c)for c,n in clocks if int(n)==0])
                if r['profile']=='balanced':primary.append(row)
        rows.append(row)
    assert len(primary)==2 and primary[0]['clocks']==primary[1]['clocks']
    assert primary[0]['unexercised_clocks']==[43,91,92,93,95]
    trace=(BUILD/NAMES[-2]/'test.log').read_text();late=(BUILD/NAMES[-1]/'test.log').read_text()
    assert 't=1020108.250 check=1456 root=1 we_n=0 addr=1'in trace
    assert 't=1020108.750 check=1456 root=1 we_n=1 addr=1'in trace
    assert 't=1020108.750 check=1456 root=1 we_n=1 addr=0'in late
    assert 't=1020109.000 check=1456 root=1 we_n=1 addr=1'in late
    wire_checks=[]
    for bn in ['ProcH-Rev-Ce','ProcL-Rev-Ci']:
        b=load_board(str(ROOT/'chm/sil'/(bn+'.dm!1_')/(bn+'.wl')))
        pins={(p['pkg'],p['pin']):n for n,r in b.nets.items()for p in r['pins']}
        expected={('f19',13):'PRhold',('f19',14):"Hold'",('e19',10):"Hold'",('e19',13):'Holda'}
        assert all(pins[k]==n for k,n in expected.items())and('e19',9)not in pins
        assert b.packages['f19']['type']=='MC1660'and b.packages['e19']['type']=='MC10195'
        wire_checks.append(dict(board=bn,pins={p+'.'+str(i):n for(p,i),n in expected.items()},mode_pin9_unwired=True))
    out.mkdir(parents=True)
    r=dict(status='INCOMPLETE_PROCESSOR_CONTROL_HOLD_COMPOSITION',scope=__doc__,candidate_sha256=candidate_sha,
           source_recipe_byte_identity=True,physical_bits=844,ram_bits=9216,clock_channels=100,selected_control_flags=94,
           cases=rows,primary=primary,Hold_source_checks=wire_checks,
           arrival_gap=dict(check=1456,storage='ProcL g15 F10145A word1',bits=[644,645,646,647],
               source='LastNext address from a19 MC10158 selected by PreSHCP C',
               write_gate='RBaseWrite from i18 SE10212',address_change_before_write_close_ns=0.5,
               reference_model='Zero-delay level-written original_F10145A writes the new address during overlap.',
               causal_test='Change phase FF1250->2000ps only; write closes0.25ns before address changes and full digital comparison passes.',
               physical_outcome_qualified=False),
           primary_reference=dict(title='1979 Fairchild Bipolar Memory',
               url='https://ftpmirror.your.org/pub/misc/bitsavers/pdf/fairchild/_dataBooks/1979_Fairchild_Bipolar_Memory.pdf',
               pages='F10145A printed7-20 through7-24; PDF87-91, AC table89 and write diagram90',
               condition='AC table at VEE=-5.2V, TA=25C; specified limits, not typical values.',
               write_pulse_min_ns=4.0,address_setup_before_write_assertion_ns=3.5,address_hold_after_write_deassertion_ns=1.0,
               caveat='The illustrative early overlap and the late0.25ns margin do not establish physical RAM correctness; no oracle filtering or timing waiver.'),
           promoted_to_maintained_RTL=False,native_integrated=False,physical_timing_qualified=False,
           original_raw_boundary_directed_cases_qualified=False,hardware_speedup_claimed=False,
           next='Derive native write-close/address-switch phases with RAM setup/hold margin, respecting HM2.3/Table4 and ordinary60ns cadence; expand actual-source directed and missing-clock coverage, then remaining producers and full native timing.')
    pdf=Path('/tmp/dorado-fairchild-bipolar-memory-1979-mirror.pdf')
    if pdf.exists():r['primary_reference'].update(pdf_sha256=sha(pdf),pdf_bytes=pdf.stat().st_size)
    (out/'results.json').write_text(json.dumps(r,indent=2)+'\n');print(r['status'],'2 write/read passes;2 required early failures;2 wrong-field rejects;causal trace retained')
    return r


def archive(out,review_dir):
    assert not out.exists();r=json.loads((review_dir/'results.json').read_text());paths={}
    for name in NAMES:
        d=BUILD/name;read_run(d)
        for p in d.rglob('*'):
            if p.is_file()and 'obj'not in p.relative_to(d).parts and p.suffix in {'.py','.sv','.v','.json','.log','.wl','.lc','.txt'}:
                paths['runs/'+name+'/'+str(p.relative_to(d))]=p
    for p in review_dir.rglob('*'):
        if p.is_file():paths['review/'+str(p.relative_to(review_dir))]=p
    for n in ['generate_r_front_control_candidate.py','generate_r_front_control_reference.py','test_r_front_control.py',Path(__file__).name]:
        paths['current/tools/'+n]=ROOT/'tools'/n
    out.mkdir(parents=True);receipt=dict(status=r['status'],scope=__doc__,members=[],blobs={},candidate_sha256=r['candidate_sha256'])
    for name,p in sorted(paths.items()):
        raw=p.read_bytes();h=hashlib.sha256(raw).hexdigest()
        if h not in receipt['blobs']:
            stored='blobs/'+h+'.gz';target=out/stored;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(gzip.compress(raw,mtime=0))
            receipt['blobs'][h]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=target.stat().st_size,stored_sha256=sha(target))
        receipt['members'].append(dict(name=name,blob=h))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(receipt,indent=2)+'\n').encode(),mtime=0))
    (out/'summary.json').write_text(json.dumps(r,indent=2)+'\n');verify(out)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path)
    a=p.parse_args();review(a.output.resolve())
    if a.archive:archive(a.archive.resolve(),a.output.resolve())
