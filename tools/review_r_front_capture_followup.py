#!/usr/bin/env python3
"""Append terminal v6 timing and bounded pointer probes to the flag receipt.

Live experiments are represented only by current process observations. Archive
terminal sources/reports, retain all original checks and explicit coverage gaps.
No native selection or timing/firmware/speed qualification is implied.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time

from archive_r_front_ready_followup import verify
from test_r_front_forward_view import sha

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/'build/rtl-executive'


def read(p):return json.loads(p.read_text())


def sources(d,r):
    for row in r['source_files']:assert sha(d/row['path'])==row['output_sha256']


def review(out):
    assert not out.exists();dirs=[];pointer=[]
    native=BUILD/'r-front-physical-native-fit-terminal-v6-20261023'
    envelope=BUILD/'r-front-physical-native-v6-terminal-20261023.tgz.json';collection=read(envelope)
    assert sha(Path(str(envelope)[:-5]))==collection['archive_sha256']
    for row in collection['members']:
        p=native/row['path'];assert sha(p)==row['sha256'] and p.stat().st_size==row['bytes']
    manifest=read(native/'r-front-control-native.json');readback=read(native/'physical-native-input-readback.json')
    assert readback['actual']==manifest['source_hashes'] and readback['report_status']=='TERMINAL_PHYSICAL_NATIVE_REPORTS_REVIEW_REQUIRED'
    assert all(s['returncode']==0 for s in read(native/'run-state.json')['stages'])
    fitdir=BUILD/'r-front-physical-native-review-v6-20261023';fit=read(fitdir/'results.json')
    assert fit['status']=='REVIEWED_NATIVE_PHYSICAL_FIT_REJECTED'
    assert [r['setup_ns'] for r in fit['corners']]==[-3.878,-3.817,3.675,4.477]
    assert fit['hold_min_ns']==0.114 and fit['mpw_min_ns']==1.122
    dirs.extend([native,fitdir])
    for engine in ['icarus','verilator']:
        for mutation in [None,'zero_pointer','invert_pointer']:
            suffix=(mutation+'-') if mutation else ''
            d=BUILD/f'r-front-pointer-capture-{suffix}{engine}-v1-20261023';r=read(d/'run-state.json');sources(d,r)
            assert r['original_reference_unchanged'] and r['all_checks_unchanged']
            assert r['actions'][0]['returncode']==0
            if mutation:
                assert r['status']=='R_FRONT_POINTER_CAPTURE_PROBE_FAILED' and r['actions'][-1]['returncode']!=0
                assert 'R front resolved source mismatch check=15' in (d/'test.log').read_text()
            else:
                assert r['status']=='PASS_R_FRONT_POINTER_CAPTURE_PROBE' and r['actions'][-1]['returncode']==0
                assert r['summary']=='checks=11300 writes=238 reads=4368 epochs=392'
            pointer.append(dict(directory=d.name,status=r['status'],run_state_sha256=sha(d/'run-state.json')));dirs.append(d)
        d=BUILD/f'r-front-pointer-epoch-audit-{engine}-v1-20261023';r=read(d/'run-state.json');sources(d,r)
        assert r['status']=='PASS_POINTER_PACKAGE_D_AT_CLOCK_AUDIT' and r['coverage']==[2181,2180,0,0]
        assert r['all_original_checks_retained'] and r['actions'][-1]['returncode']==0
        pointer.append(dict(directory=d.name,status=r['status'],capture_coverage=r['coverage'],run_state_sha256=sha(d/'run-state.json')));dirs.append(d)
    d=BUILD/'r-front-pointer-epoch-low-cp-verilator-v1-20261023';r=read(d/'run-state.json');sources(d,r)
    assert r['status']=='PASS_POINTER_PACKAGE_D_AT_CLOCK_AUDIT' and r['coverage']==[2181,2180,0,0]
    assert r['extra_low_root_CP_transactions'] and r['summary']=='checks=12840 writes=238 reads=4368 epochs=392'
    pointer.append(dict(directory=d.name,status=r['status'],summary=r['summary'],capture_coverage=r['coverage']));dirs.append(d)
    stage=BUILD/'r-front-physical-native-stage-v7-20261023';s=read(stage/'r-front-control-native.json')
    for n,h in s['source_hashes'].items():assert sha(stage/n)==h,n
    assert not s['assembly_permitted'] and not s['exhaustive_epoch_invariant_qualified']
    lint=BUILD/'r-front-physical-native-lint-v7-20261023';l=read(lint/'run-state.json')
    assert l['status']=='PASS_NATIVE_PROCESSOR_CONTROL_ELABORATION' and l['structural_warnings_visible']
    assert len(re.findall(r'^%Warning-UNOPTFLAT:',(lint/'lint.log').read_text(),re.M))==3
    dirs.extend([stage,lint])
    matrix=BUILD/'r-front-pointer-capture-matrix-v1-20261023';m=read(matrix/'run-state.json')
    if m['status']=='RUNNING':
        ps=subprocess.run(['ps','-p',str(m['pid']),'-o','pid=,etime=,stat=,command='],capture_output=True,text=True,check=True)
        assert str(matrix.relative_to(ROOT)) in ps.stdout
        matrix_observation=dict(status='VERIFIED_LIVE_MATRIX',pid=m['pid'],terminal_cases_at_observation=len(m['cases']),
                                ps=ps.stdout.strip(),observed=time.time(),run_state_sha256=sha(matrix/'run-state.json'))
    else:
        assert m['status']=='PASS_R_FRONT_POINTER_CAPTURE_PROFILE_MATRIX' and len(m['cases'])==12 and not m['errors']
        for row in m['cases']:
            d=ROOT/row['directory'];r=read(d/'run-state.json');sources(d,r);assert r['actions'][-1]['returncode']==0
        matrix_observation=dict(status=m['status'],cases=12,run_state_sha256=sha(matrix/'run-state.json'));dirs.append(matrix)
    out.mkdir(parents=True)
    (out/'collection.json').write_bytes(envelope.read_bytes())
    result=dict(status='REVIEWED_V6_REJECTION_AND_BOUNDED_POINTER_CAPTURES',scope=__doc__,native_v6=fit,
                native_v6_exact_collected_members=len(collection['members']),pointer_probes=pointer,
                pointer_matrix=matrix_observation,native_v7_manifest_sha256=sha(stage/'r-front-control-native.json'),
                distinct_capture_epoch_cases=0,exhaustive_epoch_invariant_qualified=False,
                native_timing_qualified=False,firmware_qualified=False,speedup_qualified=False,assembly_permitted=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');return result,dirs


def append_archive(archive,out,dirs):
    verify(archive)
    r=json.loads(gzip.decompress((archive/'manifest.json.gz').read_bytes()));names={m['name'] for m in r['members']}
    paths={}
    for d in dirs:
        for p in d.rglob('*'):
            rel=p.relative_to(d)
            if not p.is_file() or ('obj' in rel.parts and p.suffix!='.dot'):continue
            if p.suffix in {'.vvp','.o','.a','.rbf','.sof','.pack'}:continue
            paths['runs/'+d.name+'/'+str(rel)]=p
    for p in out.iterdir():paths['capture-followup/'+p.name]=p
    for name in ['probe_r_front_pointer_capture.py','test_r_front_pointer_matrix.py','inspect_r_front_pointer_epoch.py',
                 'stage_r_front_pointer_native.py','review_r_front_capture_followup.py']:
        paths['capture-followup/tools/'+name]=ROOT/'tools'/name
    for name,p in sorted(paths.items()):
        assert name not in names,name
        raw=p.read_bytes()
        if p.suffix=='.log' and raw and not raw.endswith(b'\n'):raw=raw[:raw.rfind(b'\n')+1]
        assert not raw.startswith(b'\x7fELF'),p
        h=hashlib.sha256(raw).hexdigest()
        if h not in r['blobs']:
            stored='blobs/'+h+'.gz';q=archive/stored;q.write_bytes(gzip.compress(raw,mtime=0))
            r['blobs'][h]=dict(stored=stored,raw_bytes=len(raw),stored_bytes=q.stat().st_size,stored_sha256=sha(q))
        r['members'].append(dict(name=name,blob=h));names.add(name)
    (archive/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(r,indent=2)+'\n').encode(),mtime=0))
    (archive/'capture-followup-summary.json').write_bytes((out/'results.json').read_bytes());verify(archive)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--archive',type=Path,required=True)
    a=p.parse_args();r,dirs=review(a.output.resolve());append_archive(a.archive.resolve(),a.output.resolve(),dirs);print(r['status'])
