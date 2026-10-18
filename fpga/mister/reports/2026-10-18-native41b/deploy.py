from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time
base=Path('/media/fat/games/DoradoRTL');old=base/'Cedar41_20261018';new=base/'Cedar41b_20261018'
oldcore=Path('/media/fat/_Computer/DoradoRTL_Cedar41_20261018');core=Path('/media/fat/_Computer/DoradoRTL_Cedar41b_20261018')
assert not new.exists() and not core.exists(), 'Inspect prior preparation before retrying'
c=json.loads((old/'reboot-resume.json').read_text())
expected={'main':'1fb2e098e6f0dba64b2ad8f13964e8a6ecef553bab72039ee24e53cb9619e8d2','server':'89007902b619049633233143b133246db9c9ba2a062937bf8ef71eb13dabdacf'}
pids={}
for role in ('main','server'):
 exe=c[role]['exe'];assert hashlib.sha256(Path(exe).read_bytes()).hexdigest()==expected[role]
 matches=[]
 for p in Path('/proc').iterdir():
  if p.name.isdigit():
   try:
    if os.readlink(p/'exe')==exe:matches.append(int(p.name))
   except OSError:pass
 assert len(matches)==1,(role,matches)
 pids[role]=matches[0]
q=json.loads((old/'qualification.json').read_text())
assert q['timing_checks']>=31 and q['path_categories_checked']==36 and q['operating_conditions_checked']==4 and q['minimum_slack_ns']>0
assert q['illegal_clocks']==0 and q['unconstrained_clocks']==0
rbf_hash='a97af93667bf20b2f9bebb0959dc67a4710895f6416e91987242e7486a1f9297'
assert hashlib.sha256((oldcore/'Dorado.rbf').read_bytes()).hexdigest()==rbf_hash
seed=base/'System_20261017/cedar-work-native-hints-f3bce2a8.pack';seed_hash='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
assert hashlib.sha256(seed.read_bytes()).hexdigest()==seed_hash
server=base/'System_20261018_BSPHistory/dorado-server-e237dffc8dfcb446'
server_hash='e237dffc8dfcb446b51d417a8cbb63bcf52812a861b75f16c4454bdaffda6f94'
assert hashlib.sha256(server.read_bytes()).hexdigest()==server_hash
new.mkdir();core.mkdir();shutil.copy2(seed,new/'cedar-work.pack');shutil.copy2(oldcore/'Dorado.rbf',core/'Dorado.rbf')
assert hashlib.sha256((new/'cedar-work.pack').read_bytes()).hexdigest()==seed_hash
mgl=(oldcore/'Cedar41.mgl').read_text().replace('Cedar41_','Cedar41b_').replace('Cedar41/','Cedar41b/')
assert 'Cedar41_20261018' not in mgl
(core/'Cedar41b.mgl').write_text(mgl)
ini=Path('/media/fat/MiSTer.ini');before=ini.read_text();section='[DoradoCedar41b_20261018]'
assert section not in before
shutil.copy2(ini,new/'MiSTer.ini.before-native41b')
ini.write_text(before+'\n'+section+'\nvideo_mode=1920,88,44,148,1080,4,5,36,74250,1,1\nvsync_adjust=0\nmain='+c['main']['exe']+'\n')
q.update(hardware='Fresh same-Native41 core with ARM-tested BSP history daemon; requested, not yet qualified',server_sha256=server_hash,rbf_sha256=rbf_hash,seed_sha256=seed_hash,assistance='None',original_run='Cedar41_20261018')
(new/'qualification.json').write_text(json.dumps(q,indent=2)+'\n')
# Preserve the assisted run and stop only its old file-server process.
shutil.copy2(old/'server.log',old/'server-before-native41b.log')
os.kill(pids['server'],signal.SIGTERM)
for _ in range(100):
 if not Path('/proc',str(pids['server'])).exists():break
 time.sleep(.05)
else:raise RuntimeError('old server did not stop')
c['server']['exe']=str(server);c['server']['argv'][0]=str(server)
env=os.environ.copy();env.update(c['server']['env'])
with (new/'server.log').open('ab')as f:
 child=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=env,stdin=subprocess.DEVNULL,stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
time.sleep(.3);assert child.poll()is None
main_log=Path(os.readlink('/proc/'+str(pids['main'])+'/fd/1'))
assert main_log==old/'main.log'
shutil.copy2(main_log,old/'main-before-native41b.log');main_log.rename(new/'main.log')
c['main']['argv'][1]=str(core/'Dorado.rbf');c['main']['argv'][2]=str(core/'Cedar41b.mgl')
(new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
(new/'deployment.json').write_text(json.dumps(dict(time_utc=time.strftime('%Y-%m-%d %H:%M:%S UTC',time.gmtime()),old_pids=pids,new_server_pid=child.pid,rbf_sha256=rbf_hash,server_sha256=server_hash,seed_sha256=seed_hash),indent=2)+'\n')
fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
try:os.write(fd,b'load_core /media/fat/_Computer/DoradoRTL_Cedar41b_20261018/Cedar41b.mgl\n')
finally:os.close(fd)
print('Requested identical Native41 SDRAM fresh boot, fixed BSP server',child.pid,flush=True)
