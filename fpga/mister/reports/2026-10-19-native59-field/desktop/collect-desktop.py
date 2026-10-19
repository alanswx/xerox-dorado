"""Bounded read-only Native59 desktop collection; never copy the live pack."""
from pathlib import Path
import datetime,gzip,hashlib,io,json,tarfile,tempfile
base=Path('/media/fat/games/DoradoRTL/Cedar59_20261019')
def sha(p):
 h=hashlib.sha256()
 with Path(p).open('rb')as f:
  for block in iter(lambda:f.read(1024*1024),b''):h.update(block)
 return h.hexdigest()
assert sha('/proc/29511/exe')=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert sha('/proc/29510/exe')=='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
assert sha('/media/fat/_Computer/DoradoRTL_Cedar59_20261019/Dorado.rbf')=='db6883e495bacd51c188f0b22c459937071d4860516ae102bb84c4c7bf441cad'
assert not Path('/proc/29777/exe').exists(),'Completed loader observer must remain stopped'
proof=Path('/tmp/dorado59-desktop-qualification.json').read_bytes()
assert json.loads(proof)['main_pid']==29511
assert not(base/'desktop-qualification.json').exists(),'Inspect existing qualification; do not overwrite'
assert not (base/'desktop-collection.tar.gz').exists(),'Inspect existing collection; do not overwrite'
files={};records=0;incomplete=False
# Spool each fixed-size prefix to disk. Neither parsed records nor full logs
# are retained in memory; physical RAM is scarce with the ARM disk cache.
with tempfile.TemporaryDirectory(prefix='dorado59-desktop-',dir=str(base))as tmp:
 for name,p in [('main-through-desktop.log',Path('/tmp/dorado59-main.log')),('disk-through-desktop.jsonl',Path('/tmp/dorado59-disk.jsonl')),('server-through-desktop.log',base/'server.log')]:
  size=p.stat().st_size;left=size;digest=hashlib.sha256();dest=Path(tmp)/name
  with p.open('rb')as src,dest.open('wb')as out:
   while left:
    block=src.read(min(left,1024*1024));assert block,'Log shrank during prefix collection'
    out.write(block);digest.update(block);left-=len(block)
  if name.startswith('disk-'):
   with dest.open('rb')as inp:
    for raw in inp:
     if not raw.endswith(b'\n'):incomplete=True;break
     r=json.loads(raw);records+=1
     assert 'error'not in r['event']and r['event']!='trace_dropped',r
  files[name]=dict(source=str(p),bytes=size,sha256=digest.hexdigest())
 (base/'desktop-qualification.json').write_bytes(proof)
 meta=dict(utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),main_pid=29511,server_pid=29510,completed_disk_trace_records=records,partial_final_record=incomplete,disk_errors_or_trace_loss=False,files=files,collector_memory='Streaming fixed-size prefixes, disk spooling and one parsed record at a time',scope='Read-only live log prefixes after observed desktop/input; no live-pack copy, guest Boot, host disk barrier, clean shutdown, cold persistence or full machine snapshot')
 (Path(tmp)/'desktop-qualification.json').write_bytes(proof)
 (Path(tmp)/'desktop-collection.json').write_text(json.dumps(meta,indent=2)+'\n')
 with tarfile.open(str(base/'desktop-collection.tar.gz'),'w:gz')as tar:
  for p in sorted(Path(tmp).iterdir()):
   if p.suffix in('.log','.jsonl'):
    z=p.with_name(p.name+'.gz')
    with p.open('rb')as src,gzip.open(z,'wb')as out:
     for block in iter(lambda:src.read(1024*1024),b''):out.write(block)
    tar.add(z,arcname=z.name)
   else:tar.add(p,arcname=p.name)
 print(json.dumps(meta,indent=2),flush=True)
