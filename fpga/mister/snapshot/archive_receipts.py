#!/usr/bin/env python3
"""Write/readback raw source/report receipts; gzip everything except small JSON.

Exclude compiler databases, object directories and executables. Preserve every
selected byte, including already-compressed inputs, under an explicit raw name.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path

SKIP={'db','incremental_db','obj','obj_xml','__pycache__'}
BINARY_MAGIC=(b'\x7fELF',b'\xcf\xfa\xed\xfe',b'\xce\xfa\xed\xfe',
              b'\xfe\xed\xfa\xcf',b'\xfe\xed\xfa\xce',b'\xca\xfe\xba\xbe')


def sha(raw):return hashlib.sha256(raw).hexdigest()


def readback(root):
    rows=json.loads(gzip.decompress((root/'archive-manifest.json.gz').read_bytes()));total=stored=0
    for row in rows:
        encoded=(root/row['stored']).read_bytes();raw=encoded if row['stored']==row['raw_name']else gzip.decompress(encoded)
        assert len(raw)==row['raw_bytes']and sha(raw)==row['raw_sha256'],row['raw_name']
        total+=len(raw);stored+=len(encoded)
    result=dict(status='PASS',members=len(rows),raw_bytes=total,stored_bytes=stored)
    (root/'archive-readback.json').write_text(json.dumps(result,indent=2)+'\n');return result


def archive(root,trees,files):
    root.mkdir(parents=True,exist_ok=False);rows=[];names=set()
    def add(name,raw):
        path=Path(name);assert not path.is_absolute()and '..'not in path.parts and name not in names,name
        names.add(name);stored=name if path.suffix=='.json'and len(raw)<=102400 else name+'.gz'
        p=root/stored;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw if name==stored else gzip.compress(raw,mtime=0))
        rows.append(dict(raw_name=name,stored=stored,raw_bytes=len(raw),raw_sha256=sha(raw)))
    for label,directory in trees:
        directory=Path(directory)
        for p in sorted(directory.rglob('*')):
            rel=p.relative_to(directory)
            if not p.is_file()or p.is_symlink()or any(x in SKIP for x in rel.parts)or p.name.startswith('._')or p.suffix in ('.pyc','.vvp','.o','.a','.so','.dylib'):continue
            assert p.suffix not in ('.rbf','.sof'),'unexpected FPGA image in source/report bundle'
            raw=p.read_bytes()
            if raw.startswith(BINARY_MAGIC):continue
            add(label+'/'+str(rel),raw)
    for name,path in files:add(name,Path(path).read_bytes())
    (root/'archive-manifest.json.gz').write_bytes(gzip.compress((json.dumps(rows,indent=2)+'\n').encode(),mtime=0))
    return readback(root)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--tree',nargs=2,action='append',default=[]);p.add_argument('--file',nargs=2,action='append',default=[]);p.add_argument('--readback',action='store_true');a=p.parse_args()
    print(json.dumps(readback(a.output.resolve())if a.readback else archive(a.output.resolve(),a.tree,a.file)))
