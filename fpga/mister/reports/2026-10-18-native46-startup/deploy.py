from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time
base=Path('/media/fat/games/DoradoRTL');old=base/'Cedar42_20261018';new=base/'Cedar46_20261018'
oldcore=Path('/media/fat/_Computer/DoradoRTL_Cedar42_20261018');core=Path('/media/fat/_Computer/DoradoRTL_Cedar46_20261018')
system=base/'System_20261018_DDRNetwork';candidate=Path('/tmp/dorado46-candidate')
main_hash='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
elf_hash='118d90bd19a63f92efd27a20d890b964254d279fd73cc8796e8f40eaad6e0435'
server_hash='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
rbf_hash='c7e6f373dd10ff55cfaaccda3c95ad6aa9d27a2a5e27dee83cb2f1728856ebbb'
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
def stopped(pid):
 for _ in range(300):
  try:os.readlink('/proc/'+str(pid)+'/exe')
  except OSError:return
  time.sleep(.1)
 raise RuntimeError('Normal stop refused/timed out; no force kill: '+str(pid))
assert not new.exists() and not core.exists(),'Inspect existing preparation before any retry'
assert not system.exists(),'Inspect immutable binary staging before retry'
assert not Path('/tmp/dorado46-disk.jsonl').exists() and not Path('/tmp/dorado46-main.log').exists()
c=json.loads((old/'reboot-resume.json').read_text());pids={}
for role,expected in [('main','40e14b6b97d62435c6e1ecd5c78bf9959c74b816a8e9452b1e64c28179ca8188'),('server','e237dffc8dfcb446b51d417a8cbb63bcf52812a861b75f16c4454bdaffda6f94')]:
 assert sha(c[role]['exe'])==expected
 found=matches(c[role]['exe'],c[role]['argv']);assert len(found)==1,(role,found);pids[role]=found[0]
assert pids==dict(main=20496,server=20495),pids
main_log=Path(os.readlink('/proc/20496/fd/1'));assert main_log==Path('/tmp/dorado42-main.log')
last=main_log.read_text().splitlines()[-1];assert not(int(last.split('state=')[1].split()[0],16)&16),last
q=json.loads((candidate/'qualification.json').read_text())
assert q['frequency_mhz']==50 and q['sysper']==4 and q['raw_line_cache_entries']==0 and q['network_ddr']
assert q['timing_checks']==31 and q['path_categories_checked']==36 and q['operating_conditions_checked']==4 and q['minimum_slack_ns']>0
assert q['illegal_clocks']==q['unconstrained_clocks']==0
for filename,expected in [('dorado-main-21e6',main_hash),('dorado-main-21e6.elf',elf_hash),('dorado-server-c599',server_hash),('Dorado.rbf',rbf_hash)]:assert sha(candidate/filename)==expected,filename
# Current physical map is recorded before the daemon's production parser
# checks it again at the actual grant. No host physical writes occur here.
iomem=Path('/proc/iomem').read_text();assert '00000000-1fefffff : System RAM' in iomem
seed=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack';assert sha(seed)==seed_hash
new.mkdir();core.mkdir();system.mkdir()
for src,dst in [('dorado-main-21e6','MiSTer-21e67a3f5588a875'),('dorado-main-21e6.elf','MiSTer-21e67a3f5588a875.elf'),('dorado-server-c599','dorado-server-c59962490772f12d')]:
 shutil.copy2(candidate/src,system/dst);(system/dst).chmod(0o755)
main_exe=str(system/'MiSTer-21e67a3f5588a875');server_exe=str(system/'dorado-server-c59962490772f12d')
shutil.copy2(candidate/'dorado-main-21e6.elf','/tmp/dorado-main-21e6.elf')
shutil.copy2(seed,new/'cedar-work.pack');assert sha(new/'cedar-work.pack')==seed_hash
shutil.copy2(candidate/'Dorado.rbf',core/'Dorado.rbf')
(core/'Cedar46.mgl').write_text((oldcore/'Cedar42.mgl').read_text().replace('Cedar42','Cedar46'))
ini=Path('/media/fat/MiSTer.ini');s=ini.read_text();section='[DoradoCedar46_20261018]';assert section not in s
shutil.copy2(ini,new/'MiSTer.ini.before-native46')
ini.write_text(s+'\n'+section+'\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+main_exe+'\n')
(new/'iomem-before-grant.txt').write_text(iomem)
q.update(hardware='Native46 direct-DDR network candidate, staged; hardware unqualified',main_sha256=main_hash,server_sha256=server_hash,seed_sha256=seed_hash,rbf_sha256=rbf_hash,assistance='None; original firmware only; caller visually verified normal42 guest Boot/germ812 before loader',disk_cache='1',network_ddr='1',disk_trace='/tmp/dorado46-disk.jsonl')
(new/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
(new/'preparation.json').write_text(json.dumps(dict(old_pids=pids,new_main=main_exe,new_server=server_exe,phase='Staged artifacts/INI; normal loader next'),indent=2)+'\n')
shutil.copy2(main_log,old/'main-before-native46.log')
print('Native42 guest Boot/germ812 verified. Normal loader now; old Main owns its durability barrier.',flush=True)
fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
try:os.write(fd,b'load_core /media/fat/_Computer/DoradoRTL_Cedar46_20261018/Cedar46.mgl\n')
finally:os.close(fd)
new_argv=[main_exe,str(core/'Dorado.rbf'),str(core/'Cedar46.mgl')]
for _ in range(500):
 found=matches(main_exe,new_argv)
 if len(found)==1 and not matches(c['main']['exe'],c['main']['argv']):break
 time.sleep(.1)
else:raise RuntimeError('Normal loader/reexec not observed; inspect, do not restart blindly')
staging=found[0];time.sleep(1);os.kill(staging,signal.SIGTERM);stopped(staging)
assert sha(new/'cedar-work.pack')==seed_hash,'Image changed before original firmware boot'
trace=Path('/tmp/dorado42-disk.jsonl');rows=[json.loads(x)for x in trace.read_text().splitlines()]
assert rows[-1]['event']=='barrier_end',rows[-8:]
assert not any('error'in x['event']or x['event']=='trace_dropped'for x in rows)
shutil.copyfile(trace,old/'disk-through-native46-load.jsonl');shutil.copyfile(main_log,old/'main-through-native46-loader.log')
pack=old/'cedar-work.pack';preserve=dict(path=str(pack),bytes=pack.stat().st_size,sha256=sha(pack),guest_boot='Normal Boot accepted, germ812 captured',host_boundary='Actual Main loader drain/fsync before FPGA load/reexec; final barrier_end',fpga_snapshot=False)
(old/'preservation.json').write_text(json.dumps(preserve,indent=2)+'\n')
os.kill(pids['server'],signal.SIGTERM);stopped(pids['server'])
c['server']['exe']=server_exe;c['server']['argv'][0]=server_exe;c['server']['env'].update(DORADO_NET_DDR='1')
with (new/'server.log').open('ab')as f:
 server=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=dict(os.environ,**c['server']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
time.sleep(.3);assert server.poll()is None
c['main']['exe']=main_exe;c['main']['argv']=new_argv;c['main']['env'].update(DORADO_NET_DDR='1',DORADO_DISK_TRACE='/tmp/dorado46-disk.jsonl');c['main_stdout']='/tmp/dorado46-main.log'
with Path(c['main_stdout']).open('ab')as f:
 main=subprocess.Popen(new_argv,cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
(new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
d=dict(time_utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),old_pids=pids,staging_main_pid=staging,new_main_pid=main.pid,new_server_pid=server.pid,main_sha256=main_hash,server_sha256=server_hash,rbf_sha256=rbf_hash,seed_sha256=seed_hash,main_log=c['main_stdout'],disk_trace=c['main']['env']['DORADO_DISK_TRACE'],preserved_old_volume=preserve,phase='Full Native46 load; waiting for cache-ready/direct-DDR grant before firmware boot')
(new/'deployment.json').write_text(json.dumps(d,indent=2)+'\n')
for _ in range(300):
 assert main.poll()is None
 s=Path(c['main_stdout']).read_text();assert 'trace open failed'not in s and 'packet ownership held'not in s,s[-2000:]
 if 'Dorado disk: image cache ready,'in s and 'Dorado DDR: direct frame queues active,'in s:break
 time.sleep(.1)
else:raise RuntimeError('Final Main cache-ready/DDR grant not observed; inspect existing processes')
assert matches(main_exe,new_argv)==[main.pid]
print(json.dumps(d,indent=2),flush=True)
print('Cache ready and physical DDR active. Arm fresh46 C watcher, then real CoolBoot pulses.',flush=True)
