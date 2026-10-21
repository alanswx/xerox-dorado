#!/usr/bin/env python3
"""Preserve a closed native receipt, sharing unchanged parent source blobs."""
import argparse
import gzip
import hashlib
import json
from pathlib import Path


def digest(data):return hashlib.sha256(data).hexdigest()


def preserve(source,output,status):
    parent=output.parent
    common=json.loads(gzip.decompress((parent/'manifest.json.gz').read_bytes()))
    entries=json.loads(gzip.decompress((source/'archive-manifest.json.gz').read_bytes()))
    output.mkdir(exist_ok=False)
    record=dict(scope=__doc__,status=status,members=[],blobs={},
                timing_qualified=False,hardware_qualified=False)
    reused=set()
    for entry in entries:
        name=entry['raw_name'];assert not Path(name).is_absolute()and '..'not in Path(name).parts
        raw=(source/entry['stored']).read_bytes()
        if entry['stored']!=name:raw=gzip.decompress(raw)
        key=digest(raw)
        assert key==entry['raw_sha256']and len(raw)==entry['raw_bytes']
        if key not in record['blobs']:
            if key in common['blobs']:
                blob=dict(common['blobs'][key]);blob['stored']='../'+blob['stored'];reused.add(key)
            else:
                stored='blobs/'+key+'.gz';path=output/stored;path.parent.mkdir(exist_ok=True)
                data=gzip.compress(raw,mtime=0);path.write_bytes(data)
                blob=dict(stored=stored,raw_bytes=len(raw),stored_bytes=len(data),stored_sha256=digest(data))
            record['blobs'][key]=blob
        record['members'].append(dict(name=name,blob=key))
    record['parent_blobs_reused']=len(reused)
    (output/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(record,indent=2)+'\n').encode(),mtime=0))
    print(json.dumps(dict(members=len(entries),unique_blobs=len(record['blobs']),parent_blobs_reused=len(reused))))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--status',choices=['REJECTED_NATIVE_STORAGE_HOLD','CLASSIFIED_R_REPORTS_ONLY'],required=True)
    a=p.parse_args();preserve(a.source,a.output,a.status)
