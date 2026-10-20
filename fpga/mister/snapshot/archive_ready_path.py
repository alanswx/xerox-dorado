#!/usr/bin/env python3
"""Lossless receipts for the readiness experiment and separate disk prototype.

Compress every raw file except JSON up to100KiB; never store build executables,
object directories or Quartus databases. Check every stored member on readback.
"""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
from generate_serial_boards import ROOT,raw_receipt

SKIP={'obj','obj_xml','db','incremental_db','__pycache__'}


def sha(raw):return hashlib.sha256(raw).hexdigest()


def archive(out,terminal=None):
    out.mkdir(parents=True,exist_ok=False);rows=[]
    def add(name,raw):
        stored=name if name.endswith('.json')and len(raw)<=102400 else name+'.gz'
        p=out/stored;p.parent.mkdir(parents=True,exist_ok=True)
        p.write_bytes(raw if stored==name else gzip.compress(raw,mtime=0))
        rows.append(dict(raw_name=name,stored=stored,raw_bytes=len(raw),raw_sha256=sha(raw)))
    def tree(label,root):
        for p in sorted(root.rglob('*')):
            relative=p.relative_to(root)
            if not p.is_file()or any(x in SKIP for x in relative.parts)or p.name.startswith('._')or p.suffix in ('.vvp','.o','.a','.so','.dylib','.pyc'):continue
            add(label+'/'+str(relative),p.read_bytes())
    build=ROOT/'build/rtl-executive'
    for label,name in [('msa','ready-path-msa-v1-20261019'),('owner','ready-path-owner-v3-20261019'),
                       ('fourstate','ready-path-fourstate-v1-20261019'),('project','serial65-ready-source-v1-20261019'),
                       ('classification','serial65-ready-inventory-v1-20261019'),('disk-capture','disk-cache-capture-v2-20261019'),
                       ('disk-phase','disk-cache-phase-v1-20261019'),('disk-payload','disk-cache-payload-v1-20261019')]:
        root=build/name;assert root.is_dir(),name
        if label not in ('project','classification'):assert json.loads((root/'results.json').read_text())['status']=='PASS',name
        tree(label,root)
    add('source-audit.json',(build/'serial65-ready-source-audit-v1-20261019.json').read_bytes())
    for root in (ROOT/'fpga/mister/snapshot',ROOT/'tools'):
        for p in sorted(root.iterdir()):
            if p.is_file()and p.suffix in ('.py','.sv','.cpp','.h'):add('tools/'+str(p.relative_to(ROOT)),p.read_bytes())
    for name in ('cell_MC10176.v','cell_MC10231.v'):
        raw=raw_receipt(ROOT/'fpga/mister/reports/2026-10-19-snapshot-serial-chipwidth','tools/verilog/cells/'+name)
        assert raw==(ROOT/'verilog/cells'/name).read_bytes(),name
        add('tools/verilog/cells/'+name,raw)
    payload=build/'disk-cache-payload-v1-20261019/executed-inputs'
    for name in ('tb_hps_pack.sv','hps_pack_test.cpp','hps_disk_test.h','dorado_pack.sv'):
        raw=(ROOT/'verilog/verilator'/name).read_bytes();expected=raw
        if name=='hps_pack_test.cpp':expected=raw.replace(b'i<1000000',b'i<3000000').replace(b'bool run=t.pack_ready;',b'bool run=t.pack_ready && (i%7!=0);')
        assert expected==(payload/name).read_bytes(),name
        add('tools/verilog/verilator/'+name,raw)
    add('disk-payload/input.pack',(ROOT/'chm/diskpacks/games-trident.pack').read_bytes())
    if terminal:tree('terminal-fit',terminal)
    names=[r['raw_name']for r in rows];assert len(names)==len(set(names))
    manifest=json.dumps(rows,indent=2).encode()+b'\n';(out/'archive-manifest.json.gz').write_bytes(gzip.compress(manifest,mtime=0))
    readback(out)


def readback(out):
    rows=json.loads(gzip.decompress((out/'archive-manifest.json.gz').read_bytes()))
    total=stored=0
    for row in rows:
        p=out/row['stored'];encoded=p.read_bytes();raw=encoded if row['stored']==row['raw_name']else gzip.decompress(encoded)
        assert len(raw)==row['raw_bytes']and sha(raw)==row['raw_sha256'],row['raw_name']
        total+=len(raw);stored+=len(encoded)
    result=dict(status='PASS',members=len(rows),raw_bytes=total,stored_bytes=stored)
    (out/'archive-readback.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--terminal',type=Path);p.add_argument('--readback',action='store_true');a=p.parse_args()
    if a.readback:readback(a.output.resolve())
    else:archive(a.output.resolve(),a.terminal.resolve()if a.terminal else None)
