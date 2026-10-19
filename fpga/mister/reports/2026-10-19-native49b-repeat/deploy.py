from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time

base=Path('/media/fat/games/DoradoRTL')
old=base/'Cedar50_20261019';new=base/'Cedar49b_20261019'
oldcore=Path('/media/fat/_Computer/DoradoRTL_Cedar50_20261019')
core=Path('/media/fat/_Computer/DoradoRTL_Cedar49b_20261019')
main_hash='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
server_hash='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
candidate=Path('/tmp/dorado49-candidate')
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
assert not Path('/tmp/dorado49b-main.log').exists() and not Path('/tmp/dorado49b-disk.jsonl').exists()
c=json.loads((old/'reboot-resume.json').read_text())
assert matches(c['main']['exe'],c['main']['argv'])==[7660]
assert matches(c['server']['exe'],c['server']['argv'])==[7659]
assert sha('/proc/7660/exe')==main_hash and sha('/proc/7659/exe')==server_hash
assert '/dev/mem'in Path('/proc/7659/maps').read_text(),'Expected active old DDR grant; normal loader must stop it'
assert sha(oldcore/'Dorado.rbf')=='70886931eab01aa26d95cff2578e67602a12e272df26c2471cb5f9b40d4561cb'
assert sha(candidate/'Dorado.rbf')==rbf_hash
q=json.loads((candidate/'qualification.json').read_text())
proof=json.loads((old/'loader-qualification.json').read_text())
assert proof['rbf_sha256']=='70886931eab01aa26d95cff2578e67602a12e272df26c2471cb5f9b40d4561cb'
assert proof['files']==34 and proof['payload_bytes']==1345843
assert proof['decision'].startswith('Do not retain deferred reads in the baseline')
assert proof['guest_boundary'].startswith('Profile prompt after complete base loader; no user files created')
assert not Path('/proc/7959/exe').exists(), 'Old loader observer still present'
assert q['storage_prefetch'] and q['storage_interleave'] and not q['storage_defer_reads']
assert q['frequency_mhz']==50 and q['sysper']==4 and q['raw_line_cache_entries']==0
assert q['network_ddr'] and q['raw_backend'].startswith('SDRAM') and q['operating_conditions_checked']==4
assert q['timing_checks']==31 and q['path_categories_checked']==36 and q['minimum_slack_ns']>0
assert q['illegal_clocks']==q['unconstrained_clocks']==0 and q['extra_timing_exceptions']=='None'
seed=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack';assert sha(seed)==seed_hash
new.mkdir();core.mkdir();shutil.copy2(seed,new/'cedar-work.pack');assert sha(new/'cedar-work.pack')==seed_hash
shutil.copy2(candidate/'Dorado.rbf',core/'Dorado.rbf')
launcher=(oldcore/'Cedar50.mgl').read_text().replace('Cedar50_20261019','Cedar49b_20261019')
assert 'Cedar50_20261019'not in launcher
(core/'Cedar49b.mgl').write_text(launcher)
ini=Path('/media/fat/MiSTer.ini');s=ini.read_text();assert '[DoradoCedar49b_20261019]'not in s
shutil.copy2(ini,new/'MiSTer.ini.before-native49b')
ini.write_text(s+'\n[DoradoCedar49b_20261019]\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+c['main']['exe']+'\n')
q.update(hardware='Native49b fresh Native49 interleaved SDRAM/direct-DDR baseline; deferred reads OFF; original firmware boot pending',assistance='None; untouched seed before final cache-ready',source_run='Cedar50_20261019',disk_trace='/tmp/dorado49b-disk.jsonl')
q.pop('input_commands',None);q.pop('latest_counters',None)
(new/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
(new/'preparation.json').write_text(json.dumps(dict(old_main=7660,old_server=7659,initial_network='Active old grant; actual normal loader barriers required',phase='Prepared new seed; normal loader next'),indent=2)+'\n')
print('Prepared Native49b. Normal loader will check actual network/disk barriers before programming.',flush=True)
fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
try:os.write(fd,b'load_core /media/fat/_Computer/DoradoRTL_Cedar49b_20261019/Cedar49b.mgl\n')
finally:os.close(fd)
argv=[c['main']['exe'],str(core/'Dorado.rbf'),str(core/'Cedar49b.mgl')]
for _ in range(500):
 found=matches(c['main']['exe'],argv)
 if len(found)==1 and not matches(c['main']['exe'],c['main']['argv']):break
 time.sleep(.1)
else:raise RuntimeError('Normal replacement not observed; inspect same Main; never force program')
staging=found[0];time.sleep(1)
assert sha('/proc/%d/exe'%staging)==main_hash
os.kill(staging,signal.SIGTERM);wait_stopped(staging)
assert '/dev/mem'not in Path('/proc/7659/maps').read_text(),'Temporary Main did not relinquish daemon mapping'
assert sha(new/'cedar-work.pack')==seed_hash,'New seed changed before real boot'
trace=Path('/tmp/dorado50-disk.jsonl');rows=[json.loads(x)for x in trace.read_text().splitlines()]
assert rows[-1]['event']=='barrier_end',rows[-8:]
assert not any('error'in r['event']or r['event']=='trace_dropped'for r in rows)
shutil.copyfile(trace,old/'disk-through-native49b-load.jsonl')
shutil.copyfile('/tmp/dorado50-main.log',old/'main-through-native49b-loader.log')
preserve=dict(path=str(old/'cedar-work.pack'),bytes=(old/'cedar-work.pack').stat().st_size,
 sha256=sha(old/'cedar-work.pack'),guest_boot='Profile prompt after complete Native50 base-loader test; no user files created or clean guest shutdown claimed',
 host_boundary='Actual normal Main loader active-DDR and disk barriers, followed by normal temporary Main exit',fpga_snapshot=False)
(old/'preservation.json').write_text(json.dumps(preserve,indent=2)+'\n')
os.kill(7659,signal.SIGTERM);wait_stopped(7659)
assert not Path('/tmp/dorado-net.sock.control').exists()
with (new/'server.log').open('ab')as f:
 sp=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=dict(os.environ,**c['server']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
time.sleep(.3);assert sp.poll()is None
c['main']['argv']=argv;c['main']['env']['DORADO_DISK_TRACE']='/tmp/dorado49b-disk.jsonl';c['main_stdout']='/tmp/dorado49b-main.log'
with Path(c['main_stdout']).open('ab')as f:
 mp=subprocess.Popen(argv,cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
(new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
d=dict(time_utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),old_main=7660,old_server=7659,staging_main=staging,
 main_pid=mp.pid,server_pid=sp.pid,main_sha256=main_hash,server_sha256=server_hash,rbf_sha256=rbf_hash,seed_sha256=seed_hash,
 main_log=c['main_stdout'],disk_trace=c['main']['env']['DORADO_DISK_TRACE'],preserved_old_volume=preserve,
 phase='Full Native49b load completed; cache-ready and fresh DDR grant required')
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
print('Native49b cache/DDR active. Select core view and arm new C watcher before original CoolBoot.',flush=True)
