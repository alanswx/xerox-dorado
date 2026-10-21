#!/usr/bin/env python3
"""Preserve full stopped observer logs locally, with verified gzip readback."""
import gzip
import hashlib
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parents[5]
OUT=Path(__file__).resolve().parent


def chunks(stream):
    while data:=stream.read(1024*1024):yield data


if __name__=='__main__':
    receipts=[]
    for name in['r-front-ready-memory-line-control-v16-20261021',
                'r-front-ready-memory-line-candidate-v16-20261021']:
        directory=ROOT/'build/rtl-executive'/name
        state=json.loads((directory/'run-state.json').read_text())
        assert state['status']=='TERMINAL_FAILED'and state['actions'][-1]['returncode']==-15
        raw=directory/'test.log';packed=directory/'test.log.gz';before=raw.stat()
        digest=hashlib.sha256();size=0
        with raw.open('rb')as source,packed.open('xb')as target:
            with gzip.GzipFile(fileobj=target,mode='wb',compresslevel=1,mtime=0)as archive:
                for data in chunks(source):digest.update(data);size+=len(data);archive.write(data)
        readback=hashlib.sha256();recovered=0
        with gzip.open(packed,'rb')as source:
            for data in chunks(source):readback.update(data);recovered+=len(data)
        assert digest.digest()==readback.digest()and size==recovered==before.st_size
        assert raw.stat().st_mtime_ns==before.st_mtime_ns
        packed_digest=hashlib.sha256()
        with packed.open('rb')as source:
            for data in chunks(source):packed_digest.update(data)
        receipt=dict(run=name,status=state['status'],test_returncode=-15,
            reason=json.loads((directory/'diagnostic-stop.json').read_text()),
            raw_bytes=size,raw_sha256=digest.hexdigest(),local_gzip=str(packed),
            gzip_bytes=packed.stat().st_size,gzip_sha256=packed_digest.hexdigest(),
            full_log_preserved_locally=True,lossless_readback=True,raw_file_removed=False)
        path=OUT/(name+'.json')
        with path.open('x')as stream:json.dump(receipt,stream,indent=2);stream.write('\n')
        raw.unlink();receipt['raw_file_removed']=True
        path.write_text(json.dumps(receipt,indent=2)+'\n')
        receipts.append(dict(run=name,raw_bytes=size,gzip_bytes=packed.stat().st_size))
    print(json.dumps(receipts))
