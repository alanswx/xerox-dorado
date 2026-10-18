from pathlib import Path
import os,signal,subprocess,json,time,hashlib
p=Path('/media/fat/games/DoradoRTL/Cedar41d_20261018');d=json.loads((p/'deployment.json').read_text());c=json.loads((p/'reboot-resume.json').read_text())
matches=[]
for proc in Path('/proc').iterdir():
 if proc.name.isdigit():
  try:
   if os.readlink(proc/'exe')==c['main']['exe'] and (proc/'cmdline').read_bytes().split(b'\0')[:3]==[x.encode()for x in c['main']['argv']]:matches.append(int(proc.name))
  except OSError:pass
assert len(matches)==1,matches
pid=matches[0];assert hashlib.sha256(Path('/proc',str(pid),'exe').read_bytes()).hexdigest()==d['main_sha256']
assert not Path('/tmp/dorado41d-disk-final.jsonl').exists()
os.kill(pid,signal.SIGTERM)
for _ in range(300):
 try:os.readlink('/proc/'+str(pid)+'/exe')
 except OSError:break
 time.sleep(.1)
else:raise RuntimeError('Normal stop refused; inspect')
c['main']['env']['DORADO_DISK_TRACE']='/tmp/dorado41d-disk-final.jsonl'
c['main_stdout']='/tmp/dorado41d-main-final.log'
with Path(c['main_stdout']).open('ab')as f:
 main=subprocess.Popen(c['main']['argv'],cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
(p/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
d.update(staging_main_pid=pid,main_pid=main.pid,phase='Full FPGA reload complete; final new Main cache ready pending original CoolBoot',main_log=c['main_stdout'],disk_trace=c['main']['env']['DORADO_DISK_TRACE'])
(p/'deployment.json').write_text(json.dumps(d,indent=2)+'\n')
for _ in range(300):
 assert main.poll()is None
 s=Path(c['main_stdout']).read_text()
 assert 'trace open failed'not in s
 if 'Dorado disk: image cache ready,'in s:break
 time.sleep(.1)
else:raise RuntimeError('No cache ready')
print(json.dumps(d,indent=2),flush=True)
