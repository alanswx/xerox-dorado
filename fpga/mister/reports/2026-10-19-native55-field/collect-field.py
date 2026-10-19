"""Read-only Native55 completed base loader and preserved52 evidence."""
from pathlib import Path
import gzip,hashlib,io,json,tarfile,time
base=Path('/media/fat/games/DoradoRTL');run=base/'Cedar55_20261019';old=base/'Cedar54_20261019'
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
assert sha('/proc/21577/exe')=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert sha('/proc/21576/exe')=='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
assert sha('/media/fat/_Computer/DoradoRTL_Cedar55_20261019/Dorado.rbf')=='3eee3109a46eb029e0253a1d776cfafd652870d99e5a2cd5f3ffd0c650b7b107'
assert not any(Path('/proc/%d/exe'%pid).exists()for pid in [20155,20154,21562])
maps={n:[x for x in Path('/proc/%d/maps'%p).read_text().splitlines()if '/dev/mem'in x or 'input-probe'in x]for n,p in [('main',21577),('server',21576)]}
assert not any('1ff00000'in x or 'input-probe'in x for x in maps['main'])
assert len(maps['server'])==1 and '1ff00000'in maps['server'][0]
paths={'deployment.json':run/'deployment.json','reboot-resume.json':run/'reboot-resume.json','qualification.json':run/'qualification.json',
'main-through-deployment.log':Path('/tmp/dorado55-main.log'),'disk-through-deployment.jsonl':Path('/tmp/dorado55-disk.jsonl'),'server-through-deployment.log':run/'server.log',
'retired54-preservation.json':old/'preservation.json','retired54-desktop-qualification.json':old/'desktop-qualification.json','retired54-normal-boot-qualification.json':old/'normal-boot-qualification.json',
# Exact retired54 files record the actual52->53 normal loader boundary.
'retired54-disk-through-load.jsonl':old/'disk-through-native55-load.jsonl',
'retired54-main-through-load.log':old/'main-through-native55-loader.log'}
paths['loader-observer.jsonl']=Path('/tmp/dorado55-loader-observer.jsonl')
assert not Path('/proc/21775/exe').exists(),'Completed observer remains live; do not restart'
files={name:p.read_bytes()for name,p in paths.items()}
observer=[json.loads(x)for x in files['loader-observer.jsonl'].splitlines()]
assert observer[-1]['event']=='end'and observer[-1]['reason']=='stop file'
reference=json.loads(Path('/tmp/dorado55-source-manifest-reference.json').read_text())
source=base/'System_20261017/chm/cedar/stp-root';actual={}
for name,expected in reference.items():
 data=(source/name).read_bytes();entry=dict(bytes=len(data),sha256=hashlib.sha256(data).hexdigest())
 assert entry==expected,(name,entry,expected)
 actual[name]=entry
files['remote-source-hashes.json']=(json.dumps(dict(status='PASS',files=len(actual),manifest=actual,
 scope='Served source files after terminal timed observer; not physical received-payload hashes'),indent=2)+'\n').encode()
rows=[json.loads(x)for x in files['retired54-disk-through-load.jsonl'].splitlines()]
assert rows[-1]['event']=='barrier_end'
assert not any('error'in x['event']or x['event']=='trace_dropped'for x in rows)
oldproof=json.loads(files['retired54-preservation.json']);assert sha(oldproof['path'])==oldproof['sha256']
meta=dict(utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),main_pid=21577,server_pid=21576,maps=maps,old_processes_absent=True,
old_pack_hash_reverified=oldproof['sha256'],scope='Read-only startup evidence; no live-pack copy, SPI, guest write or atomic snapshot. Actual34/final ACK proved separately; full desktop/input pending.',
remote_files={name:str(p)for name,p in paths.items()},files={n:dict(bytes=len(d),sha256=hashlib.sha256(d).hexdigest())for n,d in files.items()})
files['field-collection.json']=(json.dumps(meta,indent=2)+'\n').encode()
out=Path('/tmp/dorado55-field-evidence.tar.gz');assert not out.exists()
with tarfile.open(out,'w:gz')as tar:
 for name,data in files.items():
  if name.endswith(('.log','.jsonl')):data=gzip.compress(data,mtime=0);name+='.gz'
  info=tarfile.TarInfo(name);info.size=len(data);tar.addfile(info,io.BytesIO(data))
print(json.dumps(dict(archive=str(out),**meta),indent=2))
