from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time

base=Path('/media/fat/games/DoradoRTL')
old=base/'Cedar49c_20261019';new=base/'Cedar52_20261019'
oldcore=Path('/media/fat/_Computer/DoradoRTL_Cedar49c_20261019')
core=Path('/media/fat/_Computer/DoradoRTL_Cedar52_20261019')
main_hash='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
server_hash='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
candidate=Path('/tmp/dorado-native52-staging-20261019')
candidate_q=candidate/'qualification.json'
expected_germ='germ845'
assert expected_germ in ('germ812','germ845','germ855'), 'Capture and review actual49c normal Boot before using this recipe'
rbf_hash=json.loads(candidate_q.read_text())['rbf_sha256']
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
assert not Path('/tmp/dorado52-main.log').exists() and not Path('/tmp/dorado52-disk.jsonl').exists()
c=json.loads((old/'reboot-resume.json').read_text())
assert matches(c['main']['exe'],c['main']['argv'])==[14717]
assert matches(c['server']['exe'],c['server']['argv'])==[14716]
assert sha('/proc/14717/exe')==main_hash and sha('/proc/14716/exe')==server_hash
assert '/dev/mem'in Path('/proc/14716/maps').read_text(),'Expected active old DDR grant; normal loader must stop it'
assert sha(oldcore/'Dorado.rbf')=='6e7866f3d6125e5926c05fa9c4a4a000e1a4aef72b61766ccfe8b2676eecad0d'
assert sha(candidate/'Dorado.rbf')==rbf_hash
q=json.loads(candidate_q.read_text())
proof=json.loads((old/'desktop-qualification.json').read_text())
assert proof['main_pid']==14717 and proof['server_pid']==14716
assert proof['rbf_sha256']=='6e7866f3d6125e5926c05fa9c4a4a000e1a4aef72b61766ccfe8b2676eecad0d'
assert proof['status'].startswith('PASS: original-firmware fresh Viewers')
assert proof['warm_marker']['expected']=='rtl49cpersist' and proof['warm_marker']['status'].startswith('PASS')
boundary=json.loads((old/'normal-boot-qualification.json').read_text())
assert boundary['main_pid']==14717 and boundary['observed']==expected_germ
assert boundary['rbf_sha256']==proof['rbf_sha256']
assert q['rbf_sha256']=='be80e03f26e5b9e5e7984a0d9b5df15b84d264a1f526e95149e085d25a08768c'
assert q['snapshot_admin'].startswith('INACTIVE')
assert len(q['instruction_memories'])==8 and all(m['parameters']['READ_DURING_WRITE_MODE_MIXED_PORTS']=='OLD_DATA' for m in q['instruction_memories'])
assert q['storage_prefetch'] and q['storage_interleave'] and not q['storage_defer_reads']
assert q['frequency_mhz']==50 and q['sysper']==4 and q['raw_line_cache_entries']==0
assert q['network_ddr'] and q['raw_backend'].startswith('SDRAM') and q['operating_conditions_checked']==4
assert q['timing_checks']==31 and q['path_categories_checked']==68 and q['minimum_slack_ns']>0
assert q['illegal_clocks']==q['unconstrained_clocks']==0 and q['extra_timing_exceptions']=='None'
seed=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack';assert sha(seed)==seed_hash
new.mkdir();core.mkdir();shutil.copy2(seed,new/'cedar-work.pack');assert sha(new/'cedar-work.pack')==seed_hash
shutil.copy2(candidate/'Dorado.rbf',core/'Dorado.rbf')
launcher=(oldcore/'Cedar49c.mgl').read_text().replace('Cedar49c_20261019','Cedar52_20261019')
assert 'Cedar49c_20261019'not in launcher
(core/'Cedar52.mgl').write_text(launcher)
ini=Path('/media/fat/MiSTer.ini');s=ini.read_text();assert '[DoradoCedar52_20261019]'not in s
shutil.copy2(ini,new/'MiSTer.ini.before-native52')
ini.write_text(s+'\n[DoradoCedar52_20261019]\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+c['main']['exe']+'\n')
q.update(hardware='Native52 ContA+ContB exact-advance adapters with all state admin inactive; fresh original seed and original firmware boot pending',assistance='None; untouched seed before final cache-ready',source_run='Cedar49c_20261019',disk_trace='/tmp/dorado52-disk.jsonl')
q.pop('input_commands',None);q.pop('latest_counters',None)
(new/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
(new/'preparation.json').write_text(json.dumps(dict(old_main=14717,old_server=14716,initial_network='Active old grant; actual normal loader barriers required',phase='Prepared new seed; normal loader next'),indent=2)+'\n')
print('Prepared Native52. Normal loader will check actual network/disk barriers before programming.',flush=True)
fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
try:os.write(fd,b'load_core /media/fat/_Computer/DoradoRTL_Cedar52_20261019/Cedar52.mgl\n')
finally:os.close(fd)
argv=[c['main']['exe'],str(core/'Dorado.rbf'),str(core/'Cedar52.mgl')]
for _ in range(500):
 found=matches(c['main']['exe'],argv)
 if len(found)==1 and not matches(c['main']['exe'],c['main']['argv']):break
 time.sleep(.1)
else:raise RuntimeError('Normal replacement not observed; inspect same Main; never force program')
staging=found[0];time.sleep(1)
assert sha('/proc/%d/exe'%staging)==main_hash
os.kill(staging,signal.SIGTERM);wait_stopped(staging)
assert '/dev/mem'not in Path('/proc/14716/maps').read_text(),'Temporary Main did not relinquish daemon mapping'
assert sha(new/'cedar-work.pack')==seed_hash,'New seed changed before real boot'
trace=Path('/tmp/dorado49c-disk.jsonl');rows=[json.loads(x)for x in trace.read_text().splitlines()]
assert rows[-1]['event']=='barrier_end',rows[-8:]
assert not any('error'in r['event']or r['event']=='trace_dropped'for r in rows)
shutil.copyfile(trace,old/'disk-through-native49c-load.jsonl')
shutil.copyfile('/tmp/dorado49c-main.log',old/'main-through-native49c-loader.log')
preserve=dict(path=str(old/'cedar-work.pack'),bytes=(old/'cedar-work.pack').stat().st_size,
 sha256=sha(old/'cedar-work.pack'),guest_boot='Normal guest Boot after Viewers/input and rtl49cpersist.txt close/readback; actual MP state recorded in normal-boot-qualification.json; no cold marker readback or full snapshot claimed',
 host_boundary='Actual normal Main loader active-DDR and disk barriers, followed by normal temporary Main exit',fpga_snapshot=False)
(old/'preservation.json').write_text(json.dumps(preserve,indent=2)+'\n')
os.kill(14716,signal.SIGTERM);wait_stopped(14716)
assert not Path('/tmp/dorado-net.sock.control').exists()
with (new/'server.log').open('ab')as f:
 sp=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=dict(os.environ,**c['server']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
time.sleep(.3);assert sp.poll()is None
c['main']['argv']=argv;c['main']['env']['DORADO_DISK_TRACE']='/tmp/dorado52-disk.jsonl';c['main_stdout']='/tmp/dorado52-main.log'
with Path(c['main_stdout']).open('ab')as f:
 mp=subprocess.Popen(argv,cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
(new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
d=dict(time_utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),old_main=14717,old_server=14716,staging_main=staging,
 main_pid=mp.pid,server_pid=sp.pid,main_sha256=main_hash,server_sha256=server_hash,rbf_sha256=rbf_hash,seed_sha256=seed_hash,
 main_log=c['main_stdout'],disk_trace=c['main']['env']['DORADO_DISK_TRACE'],preserved_old_volume=preserve,
 phase='Full Native52 load completed; cache-ready and fresh DDR grant required')
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
print('Native52 cache/DDR active. Select core view and arm new C watcher before original CoolBoot.',flush=True)
