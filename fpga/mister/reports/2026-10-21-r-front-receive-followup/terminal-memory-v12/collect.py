#!/usr/bin/env python3
"""Close the archived V12 prefix without replacing its historical evidence."""
import gzip
import hashlib
import json
from pathlib import Path

ROOT=Path(__file__).resolve().parents[5]
OUT=Path(__file__).resolve().parent
RUN='r-front-ready-memory-control-v12-20261021'


def sha(data):return hashlib.sha256(data).hexdigest()


if __name__=='__main__':
    parent=OUT.parent/'manifest.json.gz'
    archive=json.loads(gzip.decompress(parent.read_bytes()))
    members={m['name']:m['blob']for m in archive['members']}
    run=ROOT/'build/rtl-executive'/RUN
    state=json.loads((run/'run-state.json').read_text())
    assert state['status']=='TERMINAL_RECEIVE_FILTER_PROBE'
    assert all(a['returncode']==0 for a in state['actions'])
    provenance=json.loads(gzip.decompress((run/'provenance.json.gz').read_bytes()))
    for name,digest in provenance['inputs'].items():
        assert sha((run/'executed-inputs'/name).read_bytes())==digest
        assert members['simulation/'+RUN+'/inputs/'+name]==digest,name
    log=(run/'test.log').read_text()
    assert 'BOOT_DIAG_WORLD sample=49570338 CIA=238 task=0' in log
    assert 'INITIAL ROM PASS:' in log
    receipt=dict(scope=__doc__,parent_manifest_sha256=sha(parent.read_bytes()),
        status=state['status'],original_Cedar_entry_observed=True,
        full_IM_IFUM_readback_qualified=False,desktop_qualified=False,speedup_qualified=False,
        collector_sha256=sha(Path(__file__).read_bytes()),members=[])
    for path in sorted(run.iterdir()):
        if path.is_file()and path.suffix in('.json','.txt','.log','.gz'):
            raw=path.read_bytes();target=OUT/(path.name+'.gz')
            with target.open('xb')as stream:stream.write(gzip.compress(raw,mtime=0))
            assert gzip.decompress(target.read_bytes())==raw
            receipt['members'].append(dict(name=path.name,stored=target.name,
                raw_sha256=sha(raw),raw_bytes=len(raw),stored_sha256=sha(target.read_bytes())))
    with(OUT/'receipt.json').open('x')as stream:json.dump(receipt,stream,indent=2);stream.write('\n')
    print(receipt['status'],len(receipt['members']),'terminal members verified losslessly')
