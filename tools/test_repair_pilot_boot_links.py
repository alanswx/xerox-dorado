#!/usr/bin/env python3
"""Exercise label-adjudicated boot metadata repair, including native byte oracles."""
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

from repair_pilot_boot_links import repair
from pdi_to_trident import convert

HERE=Path(__file__).resolve().parent

def fixture():
    raw=bytearray(512+200*532);raw[:8]=b'PARCDISK'
    for index,value in [(4,1),(5,2),(8,200),(9,1),(10,1),(12,20),(13,512)]:struct.pack_into('>H',raw,index*2,value)
    pv=[0]*256;pv[0:2]=[0o121212,1];pv[64]=1;pv[82]=3;pv[86]=200;pv[255]=0xa004
    lr=[0]*256;lr[0:2]=[0o131313,5];lr[255]=0x8f99
    for slot,fid,page in [(2,2,104),(3,3,139)]:
        p=[fid,0,0,0,0,0,0,*divmod(page,28)];pv[8+9*slot:8+9*(slot+1)]=p
        lr[31+9*slot:31+9*(slot+1)]=p[:7]+[page,0]
        lr[85+6*slot:85+6*(slot+1)]=[fid,0,0,0,0,0]
        struct.pack_into('>10H',raw,512+page*532,fid,0,0,0,0,0,0,0,0,0)
    # Valid CHS with sector0, whose low word looks like an unrelated flat page.
    lr[31:40]=[9,0,0,0,0,0,0,4,0]
    struct.pack_into('>10H',raw,512+112*532,9,0,0,0,0,0,0,0,0,0)
    for page,words in [(0,pv),(3,lr)]:struct.pack_into('>256H',raw,512+page*532+20,*words)
    return bytes(raw)

def oracle(before,after,report,root_off,endian):
    assert len(before)==len(after)
    expected={56:3,57:20,65:4,66:27}
    for i,v in expected.items():assert struct.unpack_from(endian+'H',after,root_off+2*i)[0]==v
    assert struct.unpack_from(endian+'2H',after,root_off+2*38)==(4,0)
    allowed={root_off+2*i+b for i in [56,57,65,66,255] for b in [0,1]}
    diff={i for i,(a,b) in enumerate(zip(before,after)) if a!=b}
    assert diff and diff<=allowed and diff==set(report['changed_bytes'])
    assert len(report['changes'])==2 and all(x['root']=='logical' for x in report['changes'])
    # Every header, label, other page/root byte and root FP is an exact oracle.
    for i in allowed:assert i>=root_off
    assert report['nonroot_payloads_preserved'] and report['unchanged_roots_preserved']

def rejects(raw,reason):
    try:repair(raw,'pdi')
    except ValueError as e:assert reason in str(e),str(e)
    else:raise AssertionError('bad metadata accepted')

raw=fixture();fixed,report=repair(raw,'pdi');oracle(raw,fixed,report,512+3*532+20,'>')
again,r=repair(fixed,'pdi');assert again==fixed and not r['changes']
assert struct.unpack_from('>H',fixed,512+20+510)[0]==0xa004,'unchanged PV checksum changed'
missing=bytearray(raw);struct.pack_into('>H',missing,512+104*532,99);rejects(bytes(missing),'no unique label-matched target')
ambiguous=bytearray(raw);struct.pack_into('>10H',ambiguous,512+4*532,9,0,0,0,0,0,0,0,0,0);rejects(bytes(ambiguous),'no unique label-matched target')
bad=bytearray(raw);struct.pack_into('>H',bad,512+3*532+20,0);rejects(bytes(bad),'missing logical root seal')
pack,_=convert(raw);native,r=repair(bytes(pack),'native-pack');oracle(bytes(pack),native,r,3*538+26,'<')
again,r2=repair(native,'native-pack');assert again==native and not r2['changes']
with tempfile.TemporaryDirectory(prefix='pilot-boot-links-') as temp:
    d=Path(temp);old=d/'old.pdi';old.write_bytes(raw)
    subprocess.run([sys.executable,str(HERE/'pdi_boot_links_to_chs.py'),str(old)],check=True,capture_output=True)
    assert old.read_bytes()==raw,'physical-only control unexpectedly repairs logical links'
    out=d/'new.pdi';subprocess.run([sys.executable,str(HERE/'repair_pilot_boot_links.py'),str(old),str(out)],check=True,capture_output=True)
    assert out.read_bytes()==fixed and old.read_bytes()==raw
    for source,target in [(old,old),(old,out)]:
        p=subprocess.run([sys.executable,str(HERE/'repair_pilot_boot_links.py'),str(source),str(target)],capture_output=True)
        assert p.returncode==2 and old.read_bytes()==raw and out.read_bytes()==fixed
print('BOOT-LINK REPAIR PASS: PDI/native full-byte oracle, unchanged roots/FPs/payloads, sector0 CHS idempotence, missing/ambiguous/seal rejection, source/output guards, original physical-only failure control')
