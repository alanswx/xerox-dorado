from pathlib import Path
import hashlib,json,os,shutil,signal,subprocess,time
base=Path('/media/fat/games/DoradoRTL')
old=base/'Cedar31_20261018'
new=base/'Cedar32_20261018'
core=Path('/media/fat/_Computer/DoradoRTL_Cedar32_20261018')
q=json.loads((new/'qualification.json').read_text())
assert q['timing_checks']==29 and q['minimum_slack_ns']>0
assert q['illegal_clocks']==0 and q['unconstrained_clocks']==0
assert hashlib.sha256((core/'Dorado.rbf').read_bytes()).hexdigest()==q['rbf_sha256']
assert hashlib.sha256((new/'cedar-work.pack').read_bytes()).hexdigest()=='f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51'
assert not(new/'main.log').exists(), 'Do not replace an earlier hardware run'
c=json.loads((old/'reboot-resume.json').read_text())
expected={'main':'1fb2e098e6f0dba64b2ad8f13964e8a6ecef553bab72039ee24e53cb9619e8d2','server':'1d5e05e9d293b095de14fcb3430980a2956a6cccb712ed860c79571980e0ec17'}
pids={}
for role in ('main','server'):
 exe=c[role]['exe']
 assert hashlib.sha256(Path(exe).read_bytes()).hexdigest()==expected[role]
 matches=[]
 for p in Path('/proc').iterdir():
  if p.name.isdigit():
   try:
    if os.readlink(p/'exe')==exe:matches.append(int(p.name))
   except OSError:pass
 assert len(matches)==1,(role,matches)
 pids[role]=matches[0]
# Stop/restart only the allocated station's external file/boot server. Main
# remains the sole SPI owner. The fresh guest boots from an offline seed.
os.kill(pids['server'],signal.SIGTERM)
for _ in range(100):
 if not Path('/proc',str(pids['server'])).exists():break
 time.sleep(.05)
else:raise RuntimeError('server did not stop')
env=os.environ.copy();env.update(c['server']['env'])
with (new/'server.log').open('ab') as log:
 server=subprocess.Popen(c['server']['argv'],cwd=c['server']['cwd'],env=env,stdin=subprocess.DEVNULL,stdout=log,stderr=subprocess.STDOUT,start_new_session=True)
time.sleep(.3);assert server.poll() is None
main_log=Path(os.readlink('/proc/'+str(pids['main'])+'/fd/1'))
assert main_log.is_relative_to(base) and main_log.is_file()
shutil.copy2(main_log,old/'main-before-native32.log')
main_log.rename(new/'main.log')
c['main']['argv'][1]=str(core/'Dorado.rbf')
c['main']['argv'][2]=str(core/'Cedar32.mgl')
(new/'reboot-resume.json').write_text(json.dumps(c,indent=2)+'\n')
fd=os.open('/dev/MiSTer_cmd',os.O_WRONLY)
try:os.write(fd,b'load_core /media/fat/_Computer/DoradoRTL_Cedar32_20261018/Cedar32.mgl\n')
finally:os.close(fd)
print('Requested qualified Native32 50MHz/SYSPER4 load through sole-owner Main',pids['main'],'fresh server',server.pid,flush=True)
