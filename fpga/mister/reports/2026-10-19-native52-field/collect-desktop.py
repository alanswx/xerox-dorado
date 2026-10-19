"""Read-only terminal desktop/Boot evidence; never copy the live disk pack."""
from pathlib import Path
import datetime,gzip,hashlib,io,json,tarfile

base=Path('/media/fat/games/DoradoRTL/Cedar52_20261019')
sha=lambda data:hashlib.sha256(data).hexdigest()
assert sha(Path('/proc/16101/exe').read_bytes())=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert sha(Path('/proc/16100/exe').read_bytes())=='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
assert not Path('/proc/16342/exe').exists(),'completed loader observer reappeared'
files={name:path.read_bytes()for name,path in {
    'main-through-desktop.log':Path('/tmp/dorado52-main.log'),
    'disk-through-desktop.jsonl':Path('/tmp/dorado52-disk.jsonl'),
    'server-through-desktop.log':base/'server.log',
    'desktop-qualification.json':base/'desktop-qualification.json',
    'normal-boot-qualification.json':base/'normal-boot-qualification.json',
}.items()}
raw=files['disk-through-desktop.jsonl'];lines=raw.splitlines()
if not raw.endswith(b'\n'):lines=lines[:-1]
rows=[json.loads(x)for x in lines]
assert rows and not any('error'in r['event']or r['event']=='trace_dropped'for r in rows)
metadata=dict(utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
    main_pid=16101,server_pid=16100,completed_disk_trace_records=len(rows),disk_errors_or_trace_loss=False,
    scope='Read-only live logs after observed normal guest Boot; no host disk barrier, live pack copy, full snapshot or cold persistence claim',
    files={name:dict(bytes=len(data),sha256=sha(data))for name,data in files.items()})
files['desktop-collection.json']=(json.dumps(metadata,indent=2)+'\n').encode()
with tarfile.open('/tmp/dorado52-desktop-collection.tar.gz','w:gz')as tar:
    for name,data in files.items():
        if name.endswith(('.log','.jsonl')):data=gzip.compress(data,mtime=0);name+='.gz'
        info=tarfile.TarInfo(name);info.size=len(data);tar.addfile(info,io.BytesIO(data))
print(json.dumps(metadata,indent=2),flush=True)
