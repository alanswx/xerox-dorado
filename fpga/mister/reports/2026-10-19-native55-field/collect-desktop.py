"""Read-only qualified desktop evidence; never copy the live disk pack."""
from pathlib import Path
import datetime,gzip,hashlib,io,json,tarfile

base=Path('/media/fat/games/DoradoRTL/Cedar55_20261019')
sha=lambda data:hashlib.sha256(data).hexdigest()
assert sha(Path('/proc/21577/exe').read_bytes())=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert sha(Path('/proc/21576/exe').read_bytes())=='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
assert sha(Path('/media/fat/_Computer/DoradoRTL_Cedar55_20261019/Dorado.rbf').read_bytes())=='3eee3109a46eb029e0253a1d776cfafd652870d99e5a2cd5f3ffd0c650b7b107'
assert not Path('/proc/21775/exe').exists(),'completed loader observer reappeared'
files={name:path.read_bytes()for name,path in {
    'main-through-desktop.log':Path('/tmp/dorado55-main.log'),
    'disk-through-desktop.jsonl':Path('/tmp/dorado55-disk.jsonl'),
    'server-through-desktop.log':base/'server.log',
    'desktop-qualification.json':base/'desktop-qualification.json',
}.items()}
raw=files['disk-through-desktop.jsonl'];lines=raw.splitlines()
if not raw.endswith(b'\n'):lines=lines[:-1]
rows=[json.loads(x)for x in lines]
assert rows and not any('error'in r['event']or r['event']=='trace_dropped'for r in rows)
metadata=dict(utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
    main_pid=21577,server_pid=21576,completed_disk_trace_records=len(rows),disk_errors_or_trace_loss=False,
    scope='Read-only live logs after observed desktop/input; no guest Boot boundary claimed; no host disk barrier, live pack copy, full snapshot or cold persistence claim',
    files={name:dict(bytes=len(data),sha256=sha(data))for name,data in files.items()})
files['desktop-collection.json']=(json.dumps(metadata,indent=2)+'\n').encode()
assert not Path('/tmp/dorado55-desktop-collection.tar.gz').exists(),'inspect existing collection; do not overwrite'
with tarfile.open('/tmp/dorado55-desktop-collection.tar.gz','w:gz')as tar:
    for name,data in files.items():
        if name.endswith(('.log','.jsonl')):data=gzip.compress(data,mtime=0);name+='.gz'
        info=tarfile.TarInfo(name);info.size=len(data);tar.addfile(info,io.BytesIO(data))
print(json.dumps(metadata,indent=2),flush=True)
