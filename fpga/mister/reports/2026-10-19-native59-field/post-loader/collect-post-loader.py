"""Read-only Native59 completed-loader source/log evidence; bounded SD spooling."""
from pathlib import Path
import gzip,hashlib,io,json,tarfile,tempfile,time
base=Path('/media/fat/games/DoradoRTL');run=base/'Cedar59_20261019';old=base/'Cedar58_20261019'
def sha(p):
 h=hashlib.sha256()
 with Path(p).open('rb')as f:
  for b in iter(lambda:f.read(1048576),b''):h.update(b)
 return h.hexdigest()
assert sha('/proc/29511/exe')=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert sha('/proc/29510/exe')=='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
assert sha('/media/fat/_Computer/DoradoRTL_Cedar59_20261019/Dorado.rbf')=='db6883e495bacd51c188f0b22c459937071d4860516ae102bb84c4c7bf441cad'
assert not any(Path('/proc/%d/exe'%p).exists()for p in (27199,27195,29487,29777))
maps={n:[s for s in Path('/proc/%d/maps'%p).read_text().splitlines()if '/dev/mem'in s or 'input-probe'in s]for n,p in [('main',29511),('server',29510)]}
assert not any('1ff00000'in s or 'input-probe'in s for s in maps['main'])
assert len(maps['server'])==1 and '1ff00000'in maps['server'][0]
proof=json.loads((old/'preservation.json').read_text());assert sha(proof['path'])==proof['sha256']
paths={'deployment.json':run/'deployment.json','reboot-resume.json':run/'reboot-resume.json','qualification.json':run/'qualification.json','retired58-preservation.json':old/'preservation.json','retired58-desktop-qualification.json':old/'desktop-qualification.json','retired58-normal-boot-qualification.json':old/'normal-boot-qualification.json','retired58-disk-through-load.jsonl':old/'disk-through-native59-load.jsonl','retired58-main-through-load.log':old/'main-through-native59-loader.log','main-through-deployment.log':Path('/tmp/dorado59-main.log'),'disk-through-deployment.jsonl':Path('/tmp/dorado59-disk.jsonl'),'server-through-deployment.log':run/'server.log'}
paths['loader-observer.jsonl']=Path('/tmp/dorado59-loader-observer.jsonl')
files={};records=0;last=None
out=run/'post-loader-evidence.tar.gz';assert not out.exists(),'Inspect existing collection; never overwrite'
with tempfile.TemporaryDirectory(prefix='evidence-spool-',dir=str(run))as tmp:
 for name,p in paths.items():
  size=p.stat().st_size;left=size;h=hashlib.sha256();copy=Path(tmp)/name
  with p.open('rb')as inp,copy.open('wb')as dest:
   while left:
    b=inp.read(min(left,1048576));assert b,'Source shrank';dest.write(b);h.update(b);left-=len(b)
  files[name]=dict(source=str(p),bytes=size,sha256=h.hexdigest())
  if name=='retired58-disk-through-load.jsonl':
   with copy.open('rb')as inp:
    for raw in inp:
     last=json.loads(raw);records+=1
     assert 'error'not in last['event']and last['event']!='trace_dropped'
   assert last['event']=='barrier_end'
 reference=json.loads(Path('/tmp/dorado59-source-manifest-reference.json').read_text());sources={}
 for name,expected in reference.items():
  p=base/'System_20261017/chm/cedar/stp-root'/name
  entry=dict(bytes=p.stat().st_size,sha256=sha(p));assert entry==expected,(name,entry,expected);sources[name]=entry
 (Path(tmp)/'remote-source-hashes.json').write_text(json.dumps(dict(status='PASS',files=len(sources),manifest=sources,scope='Served source files verified after terminal timed transfer; not physical received payload hashes'),indent=2)+'\n')
 terminal=None
 with (Path(tmp)/'loader-observer.jsonl').open('rb')as inp:
  for raw in inp:terminal=json.loads(raw)
 assert terminal['event']=='end'and terminal['reason']=='stop file'
 meta=dict(utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),main_pid=29511,server_pid=29510,maps=maps,retired_processes_absent=True,retired_disk_records=records,old_pack_hash_reverified=proof['sha256'],scope='Read-only startup prefixes and retired58 barrier evidence; no live pack copy, SPI, guest write or atomic snapshot. Actual base34/finalACK/idle measurement proved separately; Viewers/input remain pending.',files=files)
 (Path(tmp)/'post-loader-collection.json').write_text(json.dumps(meta,indent=2)+'\n')
 with tarfile.open(out,'w:gz')as tar:
  for p in sorted(Path(tmp).iterdir()):
   if p.suffix in('.jsonl','.log'):
    z=p.with_name(p.name+'.gz')
    with p.open('rb')as src,gzip.open(z,'wb')as dest:
     for b in iter(lambda:src.read(1048576),b''):dest.write(b)
    tar.add(z,arcname=z.name)
   else:tar.add(p,arcname=p.name)
print(json.dumps(dict(archive=str(out),**meta),indent=2),flush=True)
