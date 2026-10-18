from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time
base=Path('/media/fat/games/DoradoRTL');new=base/'Cedar47_20261018';core=Path('/media/fat/_Computer/DoradoRTL_Cedar47_20261018');candidate=Path('/tmp/dorado47-candidate')
system=base/'System_20261018_DDRNetwork'
main=str(system/'MiSTer-21e67a3f5588a875');server=str(system/'dorado-server-c59962490772f12d')
main_hash='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60';server_hash='c59962490772f12d061ec61aa64e6a567144d26d7802a28349a8d9a044eda87e'
seed_hash='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
def processes(exe,argv=None):
 found=[]
 for p in Path('/proc').iterdir():
  if p.name.isdigit():
   try:
    if os.readlink(p/'exe')==exe and (argv is None or (p/'cmdline').read_bytes().split(b'\0')[:len(argv)]==[x.encode()for x in argv]):found.append(int(p.name))
   except OSError:pass
 return found
def wait_stopped(pid):
 for _ in range(300):
  if not Path('/proc/%d/exe'%pid).exists():return
  time.sleep(.1)
 raise RuntimeError('Staging Main refused normal stop; preserve state, do not force kill: '+str(pid))
assert Path('/tmp/CORENAME').read_text().strip()=='MENU'
assert processes('/media/fat/MiSTer')==[876]
assert sha('/proc/876/exe')=='adeae62b09e63537fc33fbebbee5145e66fbecdd950ed6b9f856a441147556ff'
assert not new.exists() and not core.exists(),'Inspect any existing preparation before retry'
assert not Path('/tmp/dorado47-main.log').exists() and not Path('/tmp/dorado47-disk.jsonl').exists()
assert not Path('/tmp/dorado-net.sock').exists() and not Path('/tmp/dorado-net.sock.control').exists()
assert sha(main)==main_hash and sha(server)==server_hash
q=json.loads((candidate/'qualification.json').read_text())
assert q['frequency_mhz']==50 and q['sysper']==4 and q['network_ddr'] and q['raw_backend'].startswith('SDRAM') and not q['raw_line_cache_entries']
assert q['operating_conditions_checked']==4 and q['path_categories_checked']==36 and q['timing_checks']==31 and q['minimum_slack_ns']>0
assert q['illegal_clocks']==q['unconstrained_clocks']==0 and q['extra_timing_exceptions']=='None'
assert sha(candidate/'Dorado.rbf')==q['rbf_sha256']
seed=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack';assert sha(seed)==seed_hash
new.mkdir();core.mkdir();shutil.copy2(seed,new/'cedar-work.pack');assert sha(new/'cedar-work.pack')==seed_hash
shutil.copy2(candidate/'Dorado.rbf',core/'Dorado.rbf')
(core/'Cedar47.mgl').write_text((Path('/media/fat/_Computer/DoradoRTL_Cedar42_20261018/Cedar42.mgl')).read_text().replace('Cedar42','Cedar47'))
ini=Path('/media/fat/MiSTer.ini');s=ini.read_text();assert '[DoradoCedar47_20261018]' not in s
shutil.copy2(ini,new/'MiSTer.ini.before-native47')
ini.write_text(s+'\n[DoradoCedar47_20261018]\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+main+'\n')
shutil.copy2(system/'MiSTer-21e67a3f5588a875.elf','/tmp/dorado-main-21e6.elf')
(new/'iomem-before-grant.txt').write_text(Path('/proc/iomem').read_text())
q.update(hardware='Native47 staging from stock menu after full-board recovery; not yet qualified',main_sha256=main_hash,server_sha256=server_hash,seed_sha256=seed_hash,disk_cache='1',network_ddr='1',assistance='None; original firmware boot only')
(new/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
(new/'preparation.json').write_text(json.dumps(dict(old_main_pid=876,old_core='MENU',new_main=main,new_server=server,phase='Fresh seed and qualified artifacts staged; normal loader next'),indent=2)+'\n')
fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
try:os.write(fd,b'load_core /media/fat/_Computer/DoradoRTL_Cedar47_20261018/Cedar47.mgl\n')
finally:os.close(fd)
argv=[main,str(core/'Dorado.rbf'),str(core/'Cedar47.mgl')]
for _ in range(500):
 found=processes(main,argv)
 if len(found)==1 and not processes('/media/fat/MiSTer'):break
 time.sleep(.1)
else:raise RuntimeError('Normal loader replacement not observed; inspect same processes')
staging=found[0];time.sleep(1);os.kill(staging,signal.SIGTERM);wait_stopped(staging)
assert sha(new/'cedar-work.pack')==seed_hash,'Seed changed before original firmware boot'
assert not Path('/tmp/dorado-net.sock.control').exists()
c=json.loads((base/'Cedar42_20261018/reboot-resume.json').read_text())
c['server']['exe']=server;c['server']['argv'][0]=server;c['server']['env'].update(DORADO_NET_DDR='1',DORADO_NET_CONTROL='/tmp/dorado-net.sock.control')
with (new/'server.log').open('ab')as f:
 sp=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=dict(os.environ,**c['server']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
time.sleep(.3);assert sp.poll()is None
c['main']['exe']=main;c['main']['argv']=argv;c['main']['env'].update(DORADO_NET_DDR='1',DORADO_NET_CONTROL='/tmp/dorado-net.sock.control',DORADO_DISK_TRACE='/tmp/dorado47-disk.jsonl');c['main_stdout']='/tmp/dorado47-main.log'
with Path(c['main_stdout']).open('ab')as f:
 mp=subprocess.Popen(argv,cwd=c['main']['cwd'],env=dict(os.environ,**c['main']['env']),stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
(new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
d=dict(time_utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),old_main=876,staging_main=staging,main_pid=mp.pid,server_pid=sp.pid,main_sha256=main_hash,server_sha256=server_hash,rbf_sha256=q['rbf_sha256'],seed_sha256=seed_hash,main_log=c['main_stdout'],disk_trace=c['main']['env']['DORADO_DISK_TRACE'],phase='Native47 programmed, waiting for physical DDR grant and cache-ready')
(new/'deployment.json').write_text(json.dumps(d,indent=2)+'\n')
for _ in range(300):
 assert mp.poll()is None
 s=Path(c['main_stdout']).read_text();assert 'trace open failed'not in s and 'packet ownership held'not in s,s[-2000:]
 if 'Dorado disk: image cache ready,'in s and 'Dorado DDR: direct frame queues active,'in s:break
 time.sleep(.1)
else:raise RuntimeError('Final cache-ready/DDR grant missing; inspect existing Main and daemon')
print(json.dumps(d,indent=2),flush=True)
print('Physical DDR active and cache ready. Select core video/view and arm fresh C watcher before real CoolBoot.',flush=True)
