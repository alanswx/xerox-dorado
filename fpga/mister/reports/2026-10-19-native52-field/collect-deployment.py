"""Read-only actual Native52 load/ownership and preserved49c evidence."""
from pathlib import Path
import gzip,hashlib,io,json,tarfile,time
base=Path('/media/fat/games/DoradoRTL');run=base/'Cedar52_20261019';old=base/'Cedar49c_20261019'
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
assert sha('/proc/16101/exe')=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert sha('/proc/16100/exe')=='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
assert sha('/media/fat/_Computer/DoradoRTL_Cedar52_20261019/Dorado.rbf')=='be80e03f26e5b9e5e7984a0d9b5df15b84d264a1f526e95149e085d25a08768c'
assert not any(Path('/proc/%d/exe'%pid).exists()for pid in [14717,14716,16088])
maps={n:[x for x in Path('/proc/%d/maps'%p).read_text().splitlines()if '/dev/mem'in x or 'input-probe'in x]for n,p in [('main',16101),('server',16100)]}
assert not any('1ff00000'in x or 'input-probe'in x for x in maps['main'])
assert len(maps['server'])==1 and '1ff00000'in maps['server'][0]
paths={'deployment.json':run/'deployment.json','reboot-resume.json':run/'reboot-resume.json','qualification.json':run/'qualification.json',
'main-through-deployment.log':Path('/tmp/dorado52-main.log'),'disk-through-deployment.jsonl':Path('/tmp/dorado52-disk.jsonl'),'server-through-deployment.log':run/'server.log',
'retired49c-preservation.json':old/'preservation.json','retired49c-desktop-qualification.json':old/'desktop-qualification.json','retired49c-normal-boot-qualification.json':old/'normal-boot-qualification.json',
# Executed recipe retained historical lowercase native49c output labels. These
# exact files capture the actual49c->52 loader; do not rename or edit raw bytes.
'retired49c-disk-through-load.jsonl':old/'disk-through-native49c-load.jsonl',
'retired49c-main-through-load.log':old/'main-through-native49c-loader.log'}
files={name:p.read_bytes()for name,p in paths.items()}
rows=[json.loads(x)for x in files['retired49c-disk-through-load.jsonl'].splitlines()]
assert rows[-1]['event']=='barrier_end'
assert not any('error'in x['event']or x['event']=='trace_dropped'for x in rows)
oldproof=json.loads(files['retired49c-preservation.json']);assert sha(oldproof['path'])==oldproof['sha256']
meta=dict(utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),main_pid=16101,server_pid=16100,maps=maps,old_processes_absent=True,
old_pack_hash_reverified=oldproof['sha256'],scope='Read-only startup evidence; no live-pack copy, SPI, guest write or atomic snapshot. Cedar qualification pending.',
remote_files={name:str(p)for name,p in paths.items()},files={n:dict(bytes=len(d),sha256=hashlib.sha256(d).hexdigest())for n,d in files.items()})
files['deployment-collection.json']=(json.dumps(meta,indent=2)+'\n').encode()
out=Path('/tmp/dorado52-deployment-evidence.tar.gz');assert not out.exists()
with tarfile.open(out,'w:gz')as tar:
 for name,data in files.items():
  if name.endswith(('.log','.jsonl')):data=gzip.compress(data,mtime=0);name+='.gz'
  info=tarfile.TarInfo(name);info.size=len(data);tar.addfile(info,io.BytesIO(data))
print(json.dumps(dict(archive=str(out),**meta),indent=2))
