"""Read-only collection of this live51b run and its normally retired51 parent.

This observes files/processes only. A live disk trace is not a disk barrier,
an OS shutdown or a complete machine snapshot. Never copy a live pack here.
"""
from pathlib import Path
import datetime,gzip,hashlib,io,json,os,tarfile

base=Path('/media/fat/games/DoradoRTL')
run=base/'Cedar51b_20261019';old=base/'Cedar51_20261019'
main_hash='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
server_hash='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
rbf_hash='3d253187b42615e53fde1dc1bed688eb0328265d007bffdc3c161196294ccac3'
sha=lambda b:hashlib.sha256(b).hexdigest()
assert sha(Path('/proc/12945/exe').read_bytes())==main_hash
assert sha(Path('/proc/12944/exe').read_bytes())==server_hash
assert sha(Path('/media/fat/_Computer/DoradoRTL_Cedar51b_20261019/Dorado.rbf').read_bytes())==rbf_hash
assert not Path('/proc/13211/exe').exists(),'observer still active; do not restart it'
assert not any(Path('/proc/%d/exe'%pid).exists()for pid in (11259,11258,12929))
maps={name:Path('/proc/%d/maps'%pid).read_text().splitlines()
      for name,pid in [('main',12945),('server',12944)]}
assert not any('1ff00000'in line for line in maps['main'])
assert any('1ff00000'in line and '/dev/mem'in line for line in maps['server'])
assert not any('input-probe'in line for line in maps['main'])
source=base/'System_20261017/chm/cedar/stp-root'
reference=json.loads(Path('/tmp/dorado51b-source-manifest-reference.json').read_text())
manifest={}
for name,expected in reference.items():
    data=(source/name).read_bytes()
    actual=dict(bytes=len(data),sha256=sha(data))
    assert actual==expected,(name,actual,expected)
    manifest[name]=actual
files={}
for name,path in {
    'deployment.json':run/'deployment.json','reboot-resume.json':run/'reboot-resume.json',
    'main-through-collection.log':Path('/tmp/dorado51b-main.log'),
    'disk-through-collection.jsonl':Path('/tmp/dorado51b-disk.jsonl'),
    'server-through-collection.log':run/'server.log',
    'loader-observer.jsonl':Path('/tmp/dorado51b-loader-observer.jsonl'),
    'retired51-preservation.json':old/'preservation.json',
    'retired51-normal-boot-qualification.json':old/'normal-boot-qualification.json',
    'retired51-desktop-qualification.json':old/'desktop-qualification.json',
    'retired51-disk-through-load.jsonl':old/'disk-through-native51b-load.jsonl',
    'retired51-main-through-load.log':old/'main-through-native51b-loader.log',
}.items():
    # Growing logs may end with an incomplete final line. Preserve exact
    # bytes; any analyser must ignore that incomplete trailing record.
    files[name]=path.read_bytes()
oldtrace=[json.loads(row)for row in files['retired51-disk-through-load.jsonl'].splitlines()]
assert oldtrace[-1]['event']=='barrier_end'
assert not any('error'in row['event']or row['event']=='trace_dropped'for row in oldtrace)
observer=[json.loads(row)for row in files['loader-observer.jsonl'].splitlines()]
assert observer[-1]['event']=='end'and observer[-1]['reason']=='stop file'
metadata=dict(utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
    main_pid=12945,server_pid=12944,main_sha256=main_hash,server_sha256=server_hash,
    rbf_sha256=rbf_hash,observer_terminal=observer[-1],
    prior_main_server_staging_absent=True,
    maps={k:[line for line in v if '/dev/mem'in line or 'input-probe'in line]for k,v in maps.items()},
    scope='Read-only live process/log collection; no pause, SPI, memory map, guest write or live pack copy. Live logs are not a disk barrier or machine snapshot.',
    files={name:dict(bytes=len(data),sha256=sha(data))for name,data in files.items()})
files['field-collection.json']=(json.dumps(metadata,indent=2)+'\n').encode()
files['remote-source-hashes.json']=(json.dumps(dict(status='PASS',files=len(manifest),
    check_timing='After Native51 timed transfer and retired observer; same34 source files as49b',
    source_root=str(source),manifest=manifest,
    limits='Served source file hashes, not hashes of physically received payloads'),indent=2)+'\n').encode()
output=Path('/tmp/dorado51b-field-collection.tar.gz')
with tarfile.open(output,'w:gz')as tar:
    for name,data in files.items():
        # Archive large raw logs compressed independently for direct git storage.
        if name.endswith(('.log','.jsonl')):
            data=gzip.compress(data,mtime=0);name+='.gz'
        info=tarfile.TarInfo(name);info.size=len(data);tar.addfile(info,io.BytesIO(data))
print(json.dumps(dict(archive=str(output),bytes=output.stat().st_size,**metadata),indent=2),flush=True)
