from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time
base=Path('/media/fat/games/DoradoRTL');old=base/'Cedar41c_20261018';new=base/'Cedar41d_20261018'
oldcore=Path('/media/fat/_Computer/DoradoRTL_Cedar41c_20261018');core=Path('/media/fat/_Computer/DoradoRTL_Cedar41d_20261018')
main_hash='40e14b6b97d62435c6e1ecd5c78bf9959c74b816a8e9452b1e64c28179ca8188'
server_hash='e237dffc8dfcb446b51d417a8cbb63bcf52812a861b75f16c4454bdaffda6f94'
rbf_hash='a97af93667bf20b2f9bebb0959dc67a4710895f6416e91987242e7486a1f9297'
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
def wait_stop(pid):
 for _ in range(300):
  try:os.readlink('/proc/'+str(pid)+'/exe')
  except OSError:return
  time.sleep(.1)
 raise RuntimeError('Normal termination refused/timed out; no force kill: '+str(pid))
c=json.loads((old/'reboot-resume.json').read_text())
assert not new.exists() and not core.exists()
assert not Path('/tmp/dorado41d-disk.jsonl').exists()
assert sha('/proc/16875/exe')==main_hash and sha('/proc/16872/exe')==server_hash
assert sha(oldcore/'Dorado.rbf')==rbf_hash
assert os.readlink('/proc/16875/fd/1')=='/tmp/dorado41c-main.log'
assert not int((Path('/tmp/dorado41c-main.log').read_text().splitlines()[-1].split('state=')[1].split()[0]),16)&16
probe=[json.loads(x)for x in Path('/tmp/dorado41c-slowflush.jsonl').read_text().splitlines()]
assert probe[-1]['phase']=='end' and probe[-1]['result']==0
new.mkdir();core.mkdir()
# Normal SIGTERM requests Main's actual cache drain/fsync from its poll loop.
os.kill(16875,signal.SIGTERM);wait_stop(16875)
trace=[json.loads(x)for x in Path('/tmp/dorado41c-disk.jsonl').read_text().splitlines()]
assert trace[-1]['event']=='barrier_end',trace[-8:]
assert not any('error'in x['event']or x['event']=='trace_dropped'for x in trace)
os.kill(16872,signal.SIGTERM);wait_stop(16872)
shutil.copyfile('/tmp/dorado41c-main.log',old/'main-ramlog-through-normal-stop.log')
shutil.copyfile('/tmp/dorado41c-disk.jsonl',old/'disk-through-normal-stop.jsonl')
# Immutable physical-file baseline after guest Boot and normal host drain.
preserved=old/'cedar-work-clean-boot-preserved.pack'
assert not preserved.exists()
shutil.copy2(old/'cedar-work.pack',preserved)
with preserved.open('rb')as f:os.fsync(f.fileno())
pack_sha=sha(preserved);assert sha(old/'cedar-work.pack')==pack_sha
shutil.copy2(preserved,new/'cedar-work.pack')
with (new/'cedar-work.pack').open('rb')as f:os.fsync(f.fileno())
assert sha(new/'cedar-work.pack')==pack_sha
shutil.copy2(oldcore/'Dorado.rbf',core/'Dorado.rbf')
(core/'Cedar41d.mgl').write_text((oldcore/'Cedar41c.mgl').read_text().replace('Cedar41c','Cedar41d'))
ini=Path('/media/fat/MiSTer.ini');s=ini.read_text();section='[DoradoCedar41d_20261018]';assert section not in s
shutil.copy2(ini,new/'MiSTer.ini.before-native41d')
ini.write_text(s+'\n'+section+'\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+c['main']['exe']+'\n')
q=json.loads((old/'qualification.json').read_text());q.update(hardware='Native41d cold ARM cache/full FPGA reload persistence test, NOT a fresh seed performance boot',assistance='None',source_run='Cedar41c_20261018',cold_image_sha256=pack_sha,seed_sha256=None,disk_trace='/tmp/dorado41d-disk.jsonl',main_sha256=main_hash,server_sha256=server_hash,rbf_sha256=rbf_hash,persistence='PENDING guest marker reads after original firmware boot')
(new/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
with (new/'server.log').open('ab')as f:
 server=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=dict(os.environ,**c['server']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
time.sleep(.3);assert server.poll()is None
c['main']['argv']=[c['main']['exe'],str(core/'Dorado.rbf'),str(core/'Cedar41d.mgl')]
c['main']['env'].update(DORADO_DISK_TRACE='/tmp/dorado41d-disk.jsonl')
c['main_stdout']='/tmp/dorado41d-main.log'
with Path(c['main_stdout']).open('ab')as f:
 main=subprocess.Popen(c['main']['argv'],cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
(new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
d=dict(time_utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),main_pid=main.pid,server_pid=server.pid,cold_image_sha256=pack_sha,rbf_sha256=rbf_hash,main_sha256=main_hash,server_sha256=server_hash,preserved_source_pack=str(preserved),phase='New Main cache cold preload, then normal full FPGA load requested')
(new/'deployment.json').write_text(json.dumps(d,indent=2)+'\n')
for _ in range(300):
 assert main.poll()is None
 if 'Dorado disk: image cache ready,'in Path(c['main_stdout']).read_text():break
 time.sleep(.1)
else:raise RuntimeError('No new Main cache ready; inspect')
fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
try:os.write(fd,b'load_core /media/fat/_Computer/DoradoRTL_Cedar41d_20261018/Cedar41d.mgl\n')
finally:os.close(fd)
print(json.dumps(d,indent=2),flush=True)
print('Full reload requested; verify new image-cache-ready before C watcher/CoolBoot',flush=True)
