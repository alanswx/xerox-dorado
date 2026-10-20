#!/usr/bin/env python3
"""Collect terminal native evidence, excluding Quartus databases and RBF bytes.

Run on misterubuntu after the builder has exited. Raw sources/reports/logs
are losslessly gzip-compressed with an explicit manifest and readback hashes.
The separately copied RBF is identified by SHA256/size in artifact.json.
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
    s=json.loads((project/'run-state.json').read_text())
    assert s['status']=='TERMINAL_REVIEW_REQUIRED'
    pids=[s['builder_pid']]+[r['pid']for r in s['stages']]
    for pid in pids:
        try:os.kill(pid,0)
        except ProcessLookupError:continue
        raise RuntimeError('Recorded process still exists: '+str(pid))
    record=json.loads((project/'mc1668-candidate.json').read_text())
    actual={name:sha((project/name).read_bytes())for name in record['source_hashes']}
    assert actual==record['source_hashes'],'Fitted source/configuration changed'
    readback=dict(status='TERMINAL_INPUT_READBACK',job_pids_absent=pids,actual=actual)
    (project/'collected-input-check.json').write_text(json.dumps(readback,indent=2)+'\n')
    rbf=project/'core/output_files/Dorado.rbf'
    artifact=dict(path=str(rbf),sha256=sha(rbf.read_bytes()),bytes=rbf.stat().st_size,
        scope='Assembled candidate only; independent memory/timing and physical firmware review still required')
    (project/'artifact.json').write_text(json.dumps(artifact,indent=2)+'\n')
    files=set(record['source_hashes'])
    files.update(str(p.relative_to(project))for p in project.glob('*.log'))
    files.update(str(p.relative_to(project))for p in project.glob('*.json'))
    files.update(str(p.relative_to(project))for p in project.glob('*.py'))
    for directory in(project/'core',project/'core/output_files'):
        files.update(str(p.relative_to(project))for p in directory.iterdir()if p.is_file()and p.suffix in('.rpt','.summary','.log'))
    out=project/'receipt';out.mkdir(exist_ok=False)
    rows=[]
    for name in sorted(files):
        assert not Path(name).is_absolute()and '..'not in Path(name).parts
        raw=(project/name).read_bytes();stored=name if name.endswith('.json')and len(raw)<=102400 else name+'.gz'
        p=out/stored;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw if stored==name else gzip.compress(raw,mtime=0))
        rows.append(dict(raw_name=name,stored=stored,raw_bytes=len(raw),raw_sha256=sha(raw)))
    (out/'archive-manifest.json.gz').write_bytes(gzip.compress((json.dumps(rows,indent=2)+'\n').encode(),mtime=0))
    for row in rows:
        raw=(out/row['stored']).read_bytes()
        if row['stored']!=row['raw_name']:raw=gzip.decompress(raw)
        assert len(raw)==row['raw_bytes']and sha(raw)==row['raw_sha256']
    path=project/'receipt.tar.gz'
    with tarfile.open(path,'w:gz')as tar:tar.add(out,arcname='receipt')
    print(json.dumps(dict(status='TERMINAL_NATIVE_RECEIPT_READBACK',members=len(rows),archive=str(path),artifact=artifact)))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--project',type=Path,required=True);a=p.parse_args();collect(a.project.resolve())
