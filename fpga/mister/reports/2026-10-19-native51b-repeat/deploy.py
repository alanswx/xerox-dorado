from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time

base=Path('/media/fat/games/DoradoRTL')
old=base/'Cedar51_20261019';new=base/'Cedar51b_20261019'
oldcore=Path('/media/fat/_Computer/DoradoRTL_Cedar51_20261019')
core=Path('/media/fat/_Computer/DoradoRTL_Cedar51b_20261019')
main_hash='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
server_hash='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
candidate=Path('/tmp/dorado51-candidate')
rbf_hash=json.loads((candidate/'qualification.json').read_text())['rbf_sha256']
seed_hash='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
def matches(exe,argv=None):
 found=[]
 for p in Path('/proc').iterdir():
  if not p.name.isdigit():continue
  try:
   if os.readlink(p/'exe')==exe and (argv is None or (p/'cmdline').read_bytes().split(b'\0')[:len(argv)]==[s.encode()for s in argv]):found.append(int(p.name))
  except OSError:pass
 return found
def wait_stopped(pid):
 for _ in range(300):
  if not Path('/proc/%d/exe'%pid).exists():return
  time.sleep(.1)
 raise RuntimeError('Normal stop refused/timed out; preserve same process, no force kill: '+str(pid))
assert not new.exists() and not core.exists(),'Inspect existing preparation; never blindly rerun'
assert not Path('/tmp/dorado51b-main.log').exists() and not Path('/tmp/dorado51b-disk.jsonl').exists()
c=json.loads((old/'reboot-resume.json').read_text())
assert matches(c['main']['exe'],c['main']['argv'])==[11259]
assert matches(c['server']['exe'],c['server']['argv'])==[11258]
assert sha('/proc/11259/exe')==main_hash and sha('/proc/11258/exe')==server_hash
assert '/dev/mem'in Path('/proc/11258/maps').read_text(),'Expected active old DDR grant; normal loader must stop it'
assert sha(oldcore/'Dorado.rbf')=='3d253187b42615e53fde1dc1bed688eb0328265d007bffdc3c161196294ccac3'
assert sha(candidate/'Dorado.rbf')==rbf_hash
q=json.loads((candidate/'qualification.json').read_text())
proof=json.loads((old/'desktop-qualification.json').read_text())
assert proof['main_pid']==11259 and proof['server_pid']==11258
assert proof['rbf_sha256']=='3d253187b42615e53fde1dc1bed688eb0328265d007bffdc3c161196294ccac3'
assert proof['status'].startswith('PASS: original-firmware fresh Viewers')
assert proof['warm_marker']['expected']=='rtl51persist' and proof['warm_marker']['status'].startswith('PASS')
boundary=json.loads((old/'normal-boot-qualification.json').read_text())
assert boundary['main_pid']==11259 and boundary['observed']=='germ812'
assert boundary['rbf_sha256']==proof['rbf_sha256']
assert len(q['instruction_memories'])==8 and q['snapshot_admin'].startswith('INACTIVE')
assert q['storage_prefetch'] and q['storage_interleave'] and not q['storage_defer_reads']
assert q['frequency_mhz']==50 and q['sysper']==4 and q['raw_line_cache_entries']==0
assert q['network_ddr'] and q['raw_backend'].startswith('SDRAM') and q['operating_conditions_checked']==4
assert q['timing_checks']==31 and q['path_categories_checked']==52 and q['minimum_slack_ns']>0
assert q['illegal_clocks']==q['unconstrained_clocks']==0 and q['extra_timing_exceptions']=='None'
seed=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack';assert sha(seed)==seed_hash
new.mkdir();core.mkdir();shutil.copy2(seed,new/'cedar-work.pack');assert sha(new/'cedar-work.pack')==seed_hash
shutil.copy2(candidate/'Dorado.rbf',core/'Dorado.rbf')
launcher=(oldcore/'Cedar51.mgl').read_text().replace('Cedar51_20261019','Cedar51b_20261019')
assert 'Cedar51_20261019'not in launcher
(core/'Cedar51b.mgl').write_text(launcher)
ini=Path('/media/fat/MiSTer.ini');s=ini.read_text();assert '[DoradoCedar51b_20261019]'not in s
shutil.copy2(ini,new/'MiSTer.ini.before-native51b')
ini.write_text(s+'\n[DoradoCedar51b_20261019]\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+c['main']['exe']+'\n')
q.update(hardware='Native51b full-core ContB adapters; admin inactive; fresh original seed and original firmware boot pending',assistance='None; untouched seed before final cache-ready',source_run='Cedar51_20261019',disk_trace='/tmp/dorado51b-disk.jsonl')
q.pop('input_commands',None);q.pop('latest_counters',None)
(new/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
(new/'preparation.json').write_text(json.dumps(dict(old_main=11259,old_server=11258,initial_network='Active old grant; actual normal loader barriers required',phase='Prepared new seed; normal loader next'),indent=2)+'\n')
print('Prepared Native51b. Normal loader will check actual network/disk barriers before programming.',flush=True)
fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
try:os.write(fd,b'load_core /media/fat/_Computer/DoradoRTL_Cedar51b_20261019/Cedar51b.mgl\n')
finally:os.close(fd)
argv=[c['main']['exe'],str(core/'Dorado.rbf'),str(core/'Cedar51b.mgl')]
for _ in range(500):
 found=matches(c['main']['exe'],argv)
 if len(found)==1 and not matches(c['main']['exe'],c['main']['argv']):break
 time.sleep(.1)
else:raise RuntimeError('Normal replacement not observed; inspect same Main; never force program')
staging=found[0];time.sleep(1)
assert sha('/proc/%d/exe'%staging)==main_hash
os.kill(staging,signal.SIGTERM);wait_stopped(staging)
assert '/dev/mem'not in Path('/proc/11258/maps').read_text(),'Temporary Main did not relinquish daemon mapping'
assert sha(new/'cedar-work.pack')==seed_hash,'New seed changed before real boot'
trace=Path('/tmp/dorado51-disk.jsonl');rows=[json.loads(x)for x in trace.read_text().splitlines()]
assert rows[-1]['event']=='barrier_end',rows[-8:]
assert not any('error'in r['event']or r['event']=='trace_dropped'for r in rows)
shutil.copyfile(trace,old/'disk-through-native51b-load.jsonl')
shutil.copyfile('/tmp/dorado51-main.log',old/'main-through-native51b-loader.log')
preserve=dict(path=str(old/'cedar-work.pack'),bytes=(old/'cedar-work.pack').stat().st_size,
 sha256=sha(old/'cedar-work.pack'),guest_boot='Normal guest Boot after Viewers/input and rtl51persist.txt close/readback; actual germ812 captured; no cold marker readback or full snapshot claimed',
 host_boundary='Actual normal Main loader active-DDR and disk barriers, followed by normal temporary Main exit',fpga_snapshot=False)
(old/'preservation.json').write_text(json.dumps(preserve,indent=2)+'\n')
os.kill(11258,signal.SIGTERM);wait_stopped(11258)
assert not Path('/tmp/dorado-net.sock.control').exists()
with (new/'server.log').open('ab')as f:
 sp=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=dict(os.environ,**c['server']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
time.sleep(.3);assert sp.poll()is None
c['main']['argv']=argv;c['main']['env']['DORADO_DISK_TRACE']='/tmp/dorado51b-disk.jsonl';c['main_stdout']='/tmp/dorado51b-main.log'
with Path(c['main_stdout']).open('ab')as f:
 mp=subprocess.Popen(argv,cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
(new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
d=dict(time_utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),old_main=11259,old_server=11258,staging_main=staging,
 main_pid=mp.pid,server_pid=sp.pid,main_sha256=main_hash,server_sha256=server_hash,rbf_sha256=rbf_hash,seed_sha256=seed_hash,
 main_log=c['main_stdout'],disk_trace=c['main']['env']['DORADO_DISK_TRACE'],preserved_old_volume=preserve,
 phase='Full Native51b load completed; cache-ready and fresh DDR grant required')
(new/'deployment.json').write_text(json.dumps(d,indent=2)+'\n')
for _ in range(300):
 assert mp.poll()is None
 s=Path(c['main_stdout']).read_text()
 assert 'trace open failed'not in s and 'packet ownership held'not in s,s[-3000:]
 if 'Dorado disk: image cache ready,'in s and 'Dorado DDR: direct frame queues active,'in s:break
 time.sleep(.1)
else:raise RuntimeError('Cache-ready or fresh actual DDR grant missing; inspect same processes')
assert matches(c['main']['exe'],argv)==[mp.pid]
assert '/dev/mem'in Path('/proc/%d/maps'%sp.pid).read_text()
assert 'input-probe.so'not in Path('/proc/%d/maps'%mp.pid).read_text()
print(json.dumps(d,indent=2),flush=True)
print('Native51b cache/DDR active. Select core view and arm new C watcher before original CoolBoot.',flush=True)
