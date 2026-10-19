"""Read-only live desktop evidence; no SPI, guest writes or disk barrier."""
from pathlib import Path
import datetime,gzip,hashlib,io,json,tarfile

main=Path('/proc/12945');server=Path('/proc/12944')
assert hashlib.sha256((main/'exe').read_bytes()).hexdigest()=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert hashlib.sha256((server/'exe').read_bytes()).hexdigest()=='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
files={name:path.read_bytes()for name,path in {
 'main-through-desktop.log':Path('/tmp/dorado51b-main.log'),
 'disk-through-desktop.jsonl':Path('/tmp/dorado51b-disk.jsonl'),
 'server-through-desktop.log':Path('/media/fat/games/DoradoRTL/Cedar51b_20261019/server.log')}.items()}
metadata=dict(utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),main_pid=12945,server_pid=12944,
 maps={name:[line for line in (path/'maps').read_text().splitlines()if '/dev/mem'in line or 'input-probe'in line]
       for name,path in [('main',main),('server',server)]},
 files={name:dict(bytes=len(data),sha256=hashlib.sha256(data).hexdigest())for name,data in files.items()},
 scope='Live file prefix capture after visually verified original-firmware Viewers, mouse, exact keyboard and guest marker readback. No live pack copy; no atomic disk/machine snapshot or barrier.')
files['desktop-collection.json']=(json.dumps(metadata,indent=2)+'\n').encode()
output=Path('/tmp/dorado51b-desktop-evidence.tar.gz');assert not output.exists()
with tarfile.open(output,'w:gz')as tar:
 for name,data in files.items():
  if name.endswith(('.log','.jsonl')):data=gzip.compress(data,mtime=0);name+='.gz'
  entry=tarfile.TarInfo(name);entry.size=len(data);tar.addfile(entry,io.BytesIO(data))
print(json.dumps(dict(output=str(output),**metadata),indent=2),flush=True)
