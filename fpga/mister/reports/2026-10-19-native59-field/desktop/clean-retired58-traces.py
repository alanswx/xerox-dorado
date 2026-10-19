from pathlib import Path
import hashlib,json,os,time
base=Path('/media/fat/games/DoradoRTL/Cedar58_20261019');rows=[]
def sha(p):
 h=hashlib.sha256()
 with p.open('rb')as f:
  for b in iter(lambda:f.read(1048576),b''):h.update(b)
 return h.hexdigest()
for source,name in [('/tmp/dorado58-disk.jsonl','disk-through-native59-load.jsonl'),('/tmp/dorado58-main.log','main-through-native59-loader.log')]:
 src=Path(source);dst=base/name;assert src.exists()and dst.exists();a,b=src.stat(),dst.stat();assert a.st_size==b.st_size
 digest=sha(src);assert digest==sha(dst),'retained bytes differ'
 for proc in Path('/proc').iterdir():
  if not proc.name.isdigit():continue
  try:fds=list((proc/'fd').iterdir())
  except OSError:continue
  for fd in fds:
   try:
    stat=fd.stat();assert(stat.st_dev,stat.st_ino)!=(a.st_dev,a.st_ino),'retired source still open'
   except FileNotFoundError:pass
 assert(src.stat().st_size,src.stat().st_mtime_ns)==(a.st_size,a.st_mtime_ns),'retired source changed'
 rows.append(dict(removed=str(src),retained=str(dst),bytes=a.st_size,sha256=digest,open_descriptors=False));src.unlink()
out=Path('/media/fat/games/DoradoRTL/Cedar59_20261019/tmpfs-retired58-trace-cleanup.json')
assert not out.exists();meta=dict(utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),scope='Closed Native58 duplicates only; exact retained SD hashes and no open descriptors. Current Native59 files untouched.',files=rows,freed_bytes=sum(r['bytes']for r in rows));out.write_text(json.dumps(meta,indent=2)+'\n');print(json.dumps(meta),flush=True)
