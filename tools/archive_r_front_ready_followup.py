#!/usr/bin/env python3
"""Preserve terminal receive/port diagnostics with deduplicated frozen inputs.

This receipt is component evidence, never firmware, Cedar or speed qualification.
Immutable previous running-prefix archives remain untouched.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import subprocess
import time

ROOT=Path(__file__).resolve().parents[1]
RUNS=[
    'r-front-ready-pilot-v3-20261021',
    'r-front-ready-pilot-control-v3-20261021',
    'r-front-ready-pilot-control-v5-20261021',
    'r-front-ready-filter-control-v6-20261021',
    'r-front-ready-eof-control-v7-20261021',
    'r-front-ready-checksum-phase-control-v8-20261021',
    'r-front-ready-checksum-line-control-v9-20261021',
    'r-front-ready-memory-control-v10-20261021',
    'r-front-ready-memory-control-v11-20261021',
    'r-front-native-port-shadows-v1-20261021',
    'r-front-native-port-shadows-v2-20261021',
]


def sha(data):
    return hashlib.sha256(data).hexdigest()


def verify(out):
    record=json.loads(gzip.decompress((out/'manifest.json.gz').read_bytes()))
    for digest,entry in record['blobs'].items():
        data=(out/entry['stored']).read_bytes()
        assert sha(data)==entry['stored_sha256'],digest
        raw=gzip.decompress(data)
        assert sha(raw)==digest and len(raw)==entry['raw_bytes'],digest
    assert all(member['blob'] in record['blobs'] for member in record['members'])
    print(json.dumps(dict(lossless_readback=True,members=len(record['members']),
                          unique_blobs=len(record['blobs']),
                          raw_bytes=sum(e['raw_bytes']for e in record['blobs'].values()),
                          stored_bytes=sum(e['stored_bytes']for e in record['blobs'].values()))))


def archive(out,running_prefixes=()):
    paths={};runs={};observations=[]
    for name in RUNS+list(running_prefixes):
        directory=ROOT/'build/rtl-executive'/name
        state=json.loads((directory/'run-state.json').read_text())
        if state['status']=='RUNNING':
            assert name in running_prefixes,('Observe existing live job',name)
            action=state['actions'][-1];pid=action['pid']
            observed=subprocess.run(['ps','-p',str(pid),'-o','pid=,etime=,command='],
                                    capture_output=True,text=True)
            assert observed.returncode==0 and str(directory) in observed.stdout,(name,observed)
            observations.append(dict(run=name,pid=pid,observed=time.time(),
                ps=observed.stdout.strip(),scope='Running log prefix only; no terminal result'))
        else:
            assert all('returncode' in a for a in state['actions']),name
        runs[name]=state['status']
        provenance=json.loads(gzip.decompress((directory/'provenance.json.gz').read_bytes()))
        for relative,digest in provenance['inputs'].items():
            assert sha((directory/'executed-inputs'/relative).read_bytes())==digest,(name,relative)
        for path in directory.iterdir():
            if path.is_file() and path.suffix in ('.log','.json','.gz','.txt'):
                if path.name=='negative-state.json':
                    negative=json.loads(path.read_text())
                    assert negative['status']!='RUNNING' and 'returncode' in negative,name
                paths['simulation/'+name+'/'+path.name]=path
        for path in (directory/'executed-inputs').rglob('*'):
            if path.is_file():paths['simulation/'+name+'/inputs/'+str(path.relative_to(directory/'executed-inputs'))]=path
    for name in ['test_r_front_ready_boot.py','test_r_front_native_ports.py',
                 'check_microcode_reply_stream.py','inspect_r_front_checksum.py',
                 'decode_xmt.py','eb_replies.py',Path(__file__).name]:
        paths['tools/'+name]=ROOT/'tools'/name
    for name in ['build/rtl-executive/cedar113-bol-replies.txt','chm/dorado/CedarDorado.eb!6',
                 'build/rtl-executive/r-front-ready-native-inventory-v2-20261020/results.json',
                 'build/rtl-executive/r-front-ready-native-v1-20261021/r-front-ready-native.json']:
        paths['external/'+name]=ROOT/name
    out.mkdir(parents=True,exist_ok=False)
    record=dict(scope=__doc__,runs=runs,running_prefix_observations=observations,
                members=[],blobs={},desktop_qualified=False,speedup_qualified=False)
    for name,path in sorted(paths.items()):
        with path.open('rb')as stream:raw=stream.read(path.stat().st_size)
        if path.suffix=='.log' and raw and not raw.endswith(b'\n'):
            raw=raw[:raw.rfind(b'\n')+1]
        assert path.suffix not in ('.rbf','.sof','.qdb','.o','.a','.pack'),path
        assert not raw.startswith(b'\x7fELF'),path
        digest=sha(raw)
        if digest not in record['blobs']:
            stored='blobs/'+digest+'.gz';target=out/stored
            target.parent.mkdir(parents=True,exist_ok=True)
            target.write_bytes(gzip.compress(raw,mtime=0))
            record['blobs'][digest]=dict(stored=stored,raw_bytes=len(raw),
                stored_bytes=target.stat().st_size,stored_sha256=sha(target.read_bytes()))
        record['members'].append(dict(name=name,blob=digest))
    (out/'manifest.json.gz').write_bytes(gzip.compress((json.dumps(record,indent=2)+'\n').encode(),mtime=0))
    verify(out)


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    mode=parser.add_mutually_exclusive_group(required=True)
    mode.add_argument('--archive',type=Path);mode.add_argument('--verify',type=Path)
    parser.add_argument('--running-prefix',action='append',default=[])
    args=parser.parse_args()
    if args.archive:archive(args.archive.resolve(),args.running_prefix)
    else:verify(args.verify.resolve())
