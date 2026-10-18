from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time
base=Path('/media/fat/games/DoradoRTL');old=base/'Cedar41b_20261018';new=base/'Cedar41c_20261018'
oldcore=Path('/media/fat/_Computer/DoradoRTL_Cedar41b_20261018');core=Path('/media/fat/_Computer/DoradoRTL_Cedar41c_20261018')
stage=Path('/tmp/dorado-disk-stage');system=base/'System_20261018_DiskCache'
main_hash='40e14b6b97d62435c6e1ecd5c78bf9959c74b816a8e9452b1e64c28179ca8188'
rbf_hash='a97af93667bf20b2f9bebb0959dc67a4710895f6416e91987242e7486a1f9297'
seed_hash='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
old_pack_hash='0046452c45d333b30020a47546e5fdfa65b4ecb9edd46d14cbb9ba048bf02b7b'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def matches(exe):
 out=[]
 for p in Path('/proc').iterdir():
  if p.name.isdigit():
   try:
    if os.readlink(p/'exe')==str(exe):out.append(int(p.name))
   except OSError:pass
 return out
def wait_stopped(pid):
 for _ in range(300):
  try:os.readlink('/proc/'+str(pid)+'/exe')
  except OSError:return
  time.sleep(.1)
 raise RuntimeError('Process did not stop normally; no force kill or further mutation: '+str(pid))
assert not new.exists() and not core.exists() and not system.exists(),'Inspect any existing preparation before retry'
assert not Path('/tmp/dorado41c-disk.jsonl').exists()
c=json.loads((old/'reboot-resume.json').read_text());pids={}
expected={'main':'1fb2e098e6f0dba64b2ad8f13964e8a6ecef553bab72039ee24e53cb9619e8d2','server':'e237dffc8dfcb446b51d417a8cbb63bcf52812a861b75f16c4454bdaffda6f94'}
for role in ('main','server'):
 assert sha(c[role]['exe'])==expected[role]
 found=matches(c[role]['exe']);assert len(found)==1,(role,found);pids[role]=found[0]
assert sha(old/'cedar-work.pack')==old_pack_hash
assert sha(old/'cedar-work-fault-preserved.pack')==old_pack_hash
assert 'machine=802a020a state=47fc cpu=000018ff' in (old/'main.log').read_text()[-4096:]
q=json.loads((old/'qualification.json').read_text())
assert q['timing_checks']>=31 and q['path_categories_checked']==36 and q['operating_conditions_checked']==4 and q['minimum_slack_ns']>0
assert q['illegal_clocks']==0 and q['unconstrained_clocks']==0
assert sha(oldcore/'Dorado.rbf')==rbf_hash
seed=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack';assert sha(seed)==seed_hash
assert sha(stage/'MiSTer')==main_hash
build_q=json.loads((stage/'qualification.json').read_text());assert build_q['main_sha256']==main_hash and sha(stage/'MiSTer.elf')==build_q['main_elf_sha256']
# Every reviewable input is checked before touching the allocated system.
system.mkdir();exe=system/'MiSTer-40e14b6b97d62435';elf=system/'MiSTer-40e14b6b97d62435.elf'
shutil.copy2(stage/'MiSTer',exe);exe.chmod(0o755);shutil.copy2(stage/'MiSTer.elf',elf)
shutil.copy2(stage/'qualification.json',system/'qualification.json')
assert sha(exe)==main_hash
new.mkdir();core.mkdir();shutil.copy2(seed,new/'cedar-work.pack');shutil.copy2(oldcore/'Dorado.rbf',core/'Dorado.rbf')
assert sha(new/'cedar-work.pack')==seed_hash
mgl=(oldcore/'Cedar41b.mgl').read_text().replace('Cedar41b','Cedar41c')
assert 'Cedar41b' not in mgl;(core/'Cedar41c.mgl').write_text(mgl)
ini=Path('/media/fat/MiSTer.ini');before=ini.read_text();section='[DoradoCedar41c_20261018]';assert section not in before
shutil.copy2(ini,new/'MiSTer.ini.before-native41c')
ini.write_text(before+'\n'+section+'\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+str(exe)+'\n')
q.update(hardware='Fresh same-Native41 SDRAM bitstream, new ARM disk-cache Main; requested, not yet qualified',main_sha256=main_hash,server_sha256=expected['server'],rbf_sha256=rbf_hash,seed_sha256=seed_hash,assistance='None; original firmware boot is requested after final Main initialization',original_run='Cedar41b_20261018',disk_cache='1',disk_trace='/tmp/dorado41c-disk.jsonl')
(new/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
shutil.copy2(old/'main.log',old/'main-fault-before-native41c.log')
# Let the old Main program the RBF using its normal core loader. The temporary
# auto-reexec Main inherits the old (cache-disabled) environment. Restart it
# normally with the explicitly recorded cache/trace environment before CoolBoot.
fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
try:os.write(fd,b'load_core /media/fat/_Computer/DoradoRTL_Cedar41c_20261018/Cedar41c.mgl\n')
finally:os.close(fd)
for _ in range(300):
 found=matches(exe)
 if len(found)==1 and not matches(c['main']['exe']):break
 time.sleep(.1)
else:raise RuntimeError('New Main auto-reexec was not observed; inspect before retry')
staging_main=found[0];time.sleep(1);os.kill(staging_main,signal.SIGTERM);wait_stopped(staging_main)
shutil.copy2(old/'server.log',old/'server-before-native41c.log')
os.kill(pids['server'],signal.SIGTERM);wait_stopped(pids['server'])
assert sha(old/'cedar-work.pack')==old_pack_hash
server_env=dict(os.environ,**c['server']['env'])
with (new/'server.log').open('ab') as f:
 server=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=server_env,stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
time.sleep(.3);assert server.poll() is None
c['main']['exe']=str(exe);c['main']['argv']=[str(exe),str(core/'Dorado.rbf'),str(core/'Cedar41c.mgl')]
c['main']['env'].update(DORADO_DISK_CACHE='1',DORADO_DISK_TRACE='/tmp/dorado41c-disk.jsonl')
main_env=dict(os.environ,**c['main']['env'])
with (new/'main.log').open('ab') as f:
 main=subprocess.Popen(c['main']['argv'],cwd=c['main']['cwd'],env=main_env,stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
(new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
d=dict(time_utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),old_pids=pids,staging_main_pid=staging_main,new_main_pid=main.pid,new_server_pid=server.pid,main_sha256=main_hash,rbf_sha256=rbf_hash,server_sha256=expected['server'],seed_sha256=seed_hash,preserved_old_pack_sha256=old_pack_hash,phase='Waiting for final Main image-cache-ready before original firmware CoolBoot')
(new/'deployment.json').write_text(json.dumps(d,indent=2)+'\n')
for _ in range(300):
 assert main.poll() is None,('Final Main exited',main.returncode)
 if 'Dorado disk: image cache ready,' in (new/'main.log').read_text():break
 time.sleep(.1)
else:raise RuntimeError('No cache-ready log within30s; inspect before boot or retry')
assert matches(exe)==[main.pid]
assert b'DORADO_DISK_CACHE=1\x00' in Path('/proc',str(main.pid),'environ').read_bytes()
print(json.dumps(d,indent=2),flush=True)
print('Final Main cache ready. Start new C watcher, then three real CoolBoot pulses.',flush=True)
