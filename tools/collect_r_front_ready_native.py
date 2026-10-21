#!/usr/bin/env python3
"""Collect terminal native clock-integration evidence without FPGA images.

Run on misterubuntu only after the recorded builder/stages are absent. Keep
failed and successful reports; collection never implies timing qualification.
No Quartus databases, binaries, assembly or hardware access.
"""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import tarfile


def sha(raw):return hashlib.sha256(raw).hexdigest()


def collect(project):
    state=json.loads((project/'run-state.json').read_text())
    assert state['status'] in ['TERMINAL_TIMING_OBSERVED_REVIEW_REQUIRED','TERMINAL_FAILED']
    pids=[state['builder_pid']]+[r['pid']for r in state['stages']]
    central=json.loads((project/'central-review-state.json').read_text())
    assert central['status'] in ['TERMINAL_CENTRAL_REPORTS_REVIEW_REQUIRED','TERMINAL_FAILED']
    central_pids=[central['review_pid']]+[r['pid']for r in central['actions']]
    video_pids=[]
    if(project/'video-review-state.json').exists():
        video=json.loads((project/'video-review-state.json').read_text())
        assert video['status']=='TERMINAL_CENTRAL_REPORTS_REVIEW_REQUIRED'
        assert len(video['actions'])==1 and video['actions'][0]['returncode']==0
        video_pids=[video['review_pid'],video['actions'][0]['pid']]
    for pid in pids+central_pids+video_pids:
        try:os.kill(pid,0)
        except ProcessLookupError:continue
        raise RuntimeError('Recorded process still exists; observe it: '+str(pid))
    record=json.loads((project/'r-front-ready-native.json').read_text())
    actual={n:sha((project/n).read_bytes())for n in record['source_hashes']}
    assert actual==record['source_hashes'],'Compiled source/configuration changed'
    readback=dict(status='TERMINAL_NATIVE_INPUT_READBACK',run_status=state['status'],
                  absent_pids=pids,absent_central_pids=central_pids,
                  central_run_status=central['status'],actual=actual,assembly_collected=False)
    if video_pids:readback['absent_video_pids']=video_pids
    (project/'collected-input-check.json').write_text(json.dumps(readback,indent=2)+'\n')
    files=set(record['source_hashes'])
    for suffix in ['*.log','*.json','*.py','*.tcl']:
        files.update(str(p.relative_to(project))for p in project.glob(suffix))
    for directory in [project/'core',project/'core/output_files']:
        if directory.exists():
            files.update(str(p.relative_to(project))for p in directory.iterdir()
                         if p.is_file()and p.suffix in ['.rpt','.summary','.log'])
    out=project/'receipt';out.mkdir(exist_ok=False);rows=[]
    for name in sorted(files):
        assert not Path(name).is_absolute()and '..'not in Path(name).parts
        assert Path(name).suffix not in ['.rbf','.sof']
        raw=(project/name).read_bytes()
        stored=name if name.endswith('.json')and len(raw)<=102400 else name+'.gz'
        p=out/stored;p.parent.mkdir(parents=True,exist_ok=True)
        p.write_bytes(raw if stored==name else gzip.compress(raw,mtime=0))
        rows.append(dict(raw_name=name,stored=stored,raw_bytes=len(raw),raw_sha256=sha(raw)))
    (out/'archive-manifest.json.gz').write_bytes(gzip.compress((json.dumps(rows,indent=2)+'\n').encode(),mtime=0))
    for r in rows:
        raw=(out/r['stored']).read_bytes()
        if r['stored']!=r['raw_name']:raw=gzip.decompress(raw)
        assert len(raw)==r['raw_bytes']and sha(raw)==r['raw_sha256']
    path=project/'receipt.tar.gz'
    with tarfile.open(path,'w:gz')as tar:tar.add(out,arcname='receipt')
    print(json.dumps(dict(status='TERMINAL_RAW_NATIVE_RECEIPT_READBACK',members=len(rows),
                          run_status=state['status'],archive=str(path),timing_qualified=False)))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True)
    a=p.parse_args();collect(a.project.resolve())
