from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time
base=Path('/media/fat/games/DoradoRTL');old=base/'Cedar41d_20261018';new=base/'Cedar42_20261018'
oldcore=Path('/media/fat/_Computer/DoradoRTL_Cedar41d_20261018');core=Path('/media/fat/_Computer/DoradoRTL_Cedar42_20261018')
main_hash='40e14b6b97d62435c6e1ecd5c78bf9959c74b816a8e9452b1e64c28179ca8188'
server_hash='e237dffc8dfcb446b51d417a8cbb63bcf52812a861b75f16c4454bdaffda6f94'
rbf_hash='45af2e303267c4583480b29464720b0892a06cbf82881fa00ba1634eab0b8c55'
seed_hash='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
def matches(exe,argv=None):
 found=[]
 for p in Path('/proc').iterdir():
  if p.name.isdigit():
   try:
    if os.readlink(p/'exe')==exe and (argv is None or (p/'cmdline').read_bytes().split(b'\0')[:len(argv)]==[x.encode()for x in argv]):found.append(int(p.name))
   except OSError:pass
 return found
def wait_stopped(pid):
 for _ in range(300):
  try:os.readlink('/proc/'+str(pid)+'/exe')
  except OSError:return
  time.sleep(.1)
 raise RuntimeError('Normal stop refused/timed out; no force kill: '+str(pid))
assert not new.exists() and not core.exists(),'Inspect prior preparation before retry'
assert not Path('/tmp/dorado42-disk.jsonl').exists() and not Path('/tmp/dorado42-main.log').exists()
c=json.loads((old/'reboot-resume.json').read_text());pids={}
for role,expected in [('main',main_hash),('server',server_hash)]:
 assert sha(c[role]['exe'])==expected
 found=matches(c[role]['exe'],c[role]['argv']);assert len(found)==1,(role,found);pids[role]=found[0]
assert pids==dict(main=19438,server=19352),pids
main_log=Path(os.readlink('/proc/19438/fd/1'));assert main_log==Path('/tmp/dorado41d-main-final.log')
last=main_log.read_text().splitlines()[-1];assert not(int(last.split('state=')[1].split()[0],16)&16),last
assert sha(base/'Cedar41c_20261018/cedar-work-clean-boot-preserved.pack')=='81c4195570989a9d3ebf216f5600348af0c57997c738c4e98bf3ea6dd351ca04'
candidate=Path('/tmp/dorado42-candidate');q=json.loads((candidate/'qualification.json').read_text())
assert q['frequency_mhz']==50 and q['sysper']==4 and q['raw_line_cache_entries']==0
assert q['timing_checks']==29 and q['path_categories_checked']==20 and q['operating_conditions_checked']==4 and q['minimum_slack_ns']>0
assert q['illegal_clocks']==q['unconstrained_clocks']==0 and sha(candidate/'Dorado.rbf')==rbf_hash
seed=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack';assert sha(seed)==seed_hash
new.mkdir();core.mkdir();print('Copying original untouched seed; existing41d remains live until normal loader',flush=True)
shutil.copy2(seed,new/'cedar-work.pack');assert sha(new/'cedar-work.pack')==seed_hash
shutil.copy2(candidate/'Dorado.rbf',core/'Dorado.rbf')
(core/'Cedar42.mgl').write_text((oldcore/'Cedar41d.mgl').read_text().replace('Cedar41d','Cedar42'))
ini=Path('/media/fat/MiSTer.ini');s=ini.read_text();section='[DoradoCedar42_20261018]';assert section not in s
shutil.copy2(ini,new/'MiSTer.ini.before-native42')
ini.write_text(s+'\n'+section+'\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+c['main']['exe']+'\n')
q.update(hardware='Fresh matched Native42 DDR control; requested, not yet qualified',main_sha256=main_hash,server_sha256=server_hash,seed_sha256=seed_hash,rbf_sha256=rbf_hash,assistance='None; original firmware CoolBoot after final Main cache-ready',source_run='Cedar41d_20261018',disk_cache='1',disk_trace='/tmp/dorado42-disk.jsonl')
(new/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
shutil.copy2(main_log,old/'main-before-native42.log');log_offset=main_log.stat().st_size
# Actual Main loader invokes the disk barrier BEFORE FPGA programming, so no
# old guest/track can write into the new seed before full native reset.
fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
try:os.write(fd,b'load_core /media/fat/_Computer/DoradoRTL_Cedar42_20261018/Cedar42.mgl\n')
finally:os.close(fd)
new_argv=[c['main']['exe'],str(core/'Dorado.rbf'),str(core/'Cedar42.mgl')]
for _ in range(400):
 found=matches(c['main']['exe'],new_argv)
 if len(found)==1 and not matches(c['main']['exe'],c['main']['argv']):break
 time.sleep(.1)
else:raise RuntimeError('Normal loader/reexec replacement not observed; inspect')
staging=found[0]
# The loader inherits the old O_EXCL diagnostic pathname. The staging process
# may report trace-open failure; use a fresh final process/trace before boot.
time.sleep(1);os.kill(staging,signal.SIGTERM);wait_stopped(staging)
assert sha(new/'cedar-work.pack')==seed_hash,'New image changed before firmware boot'
trace=Path('/tmp/dorado41d-disk-final.jsonl')
rows=[json.loads(x)for x in trace.read_text().splitlines()]
assert rows[-1]['event']=='barrier_end',rows[-8:]
assert not any('error'in x['event']or x['event']=='trace_dropped'for x in rows)
shutil.copyfile(trace,old/'disk-through-native42-load.jsonl')
shutil.copyfile(main_log,old/'main-through-native42-loader.log')
preserve=dict(path=str(old/'cedar-work.pack'),bytes=(old/'cedar-work.pack').stat().st_size,sha256=sha(old/'cedar-work.pack'),guest_boot='Accepted after exact cold marker readbacks; germ812 display',host_boundary='Real Main FPGA-programming/restart barriers and normal loader; last observer barrier_end',fpga_snapshot=False)
(old/'preservation.json').write_text(json.dumps(preserve,indent=2)+'\n')
os.kill(pids['server'],signal.SIGTERM);wait_stopped(pids['server'])
with (new/'server.log').open('ab')as f:
 server=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=dict(os.environ,**c['server']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
time.sleep(.3);assert server.poll()is None
c['main']['argv']=new_argv;c['main']['env'].update(DORADO_DISK_TRACE='/tmp/dorado42-disk.jsonl');c['main_stdout']='/tmp/dorado42-main.log'
with Path(c['main_stdout']).open('ab')as f:
 main=subprocess.Popen(new_argv,cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
(new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
d=dict(time_utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),old_pids=pids,staging_main_pid=staging,new_main_pid=main.pid,new_server_pid=server.pid,main_sha256=main_hash,server_sha256=server_hash,rbf_sha256=rbf_hash,seed_sha256=seed_hash,main_log=c['main_stdout'],disk_trace=c['main']['env']['DORADO_DISK_TRACE'],preserved_old_volume=preserve,phase='Full42 FPGA load complete; waiting for final cache-ready before original CoolBoot')
(new/'deployment.json').write_text(json.dumps(d,indent=2)+'\n')
for _ in range(300):
 assert main.poll()is None
 s=Path(c['main_stdout']).read_text();assert 'trace open failed'not in s
 if 'Dorado disk: image cache ready,'in s:break
 time.sleep(.1)
else:raise RuntimeError('Final Main cache-ready not observed; inspect')
assert matches(c['main']['exe'],new_argv)==[main.pid]
print(json.dumps(d,indent=2),flush=True)
print('Final Main cache ready. Arm NEW42 C watcher, then real CoolBoot pulses.',flush=True)
