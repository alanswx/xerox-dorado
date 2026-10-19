"""Measure three idle-login issue windows; run before Guest input.

Usage: measure-login.py MAIN_PID REMOTE_MAIN_LOG OUTPUT_DIRECTORY
Main is the sole SPI owner. Status changes use its existing setter at input_poll.
"""
from pathlib import Path
import datetime,json,shlex,statistics,subprocess,sys
pid=int(sys.argv[1]);logfile=sys.argv[2];out=Path(sys.argv[3]);out.mkdir(exist_ok=True,parents=True)
verify="from pathlib import Path; import hashlib; p=Path('/proc/%d/exe'); assert hashlib.sha256(p.read_bytes()).hexdigest()=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'"%pid
subprocess.run(['ssh','mister14','python3 -c '+shlex.quote(verify)],check=True)
control=Path(__file__).with_name('select-issued.gdb')
assert control.exists(),control
subprocess.run(['scp',str(control),'mister14:/tmp/dorado-login-select-issued.gdb'],check=True)
with (out/'select-issued-control.log').open('w')as f:
 subprocess.run(['ssh','mister14','gdb -q -batch /tmp/dorado-main-21e6.elf -p %d -x /tmp/dorado-login-select-issued.gdb'%pid],stdout=f,stderr=subprocess.STDOUT,check=True)
remote="""import json,re,time
f=open(LOGFILE);f.seek(0,2)
p=re.compile(r'machine=([0-9a-f]{8}) state=([0-9a-f]{4}) cpu=([0-9a-f]{8})')
a=[];end=time.monotonic()+50
while len(a)<33 and time.monotonic()<end:
 line=f.readline()
 if not line:time.sleep(.01);continue
 m=p.search(line)
 if m:
  assert 'crc_bad=0' in line and 'rx_bad=0' in line and 'aborted=0' in line,line
  assert not(int(m[2],16)&16),line
  a.append(dict(monotonic=time.monotonic(),issued=int(m[1],16),status=int(m[2],16),host_edges=int(m[3],16),line=line.strip()))
assert len(a)==33,len(a)
print(json.dumps(a[2:]))
""".replace('LOGFILE',repr(logfile))
a=json.loads(subprocess.check_output(['ssh','mister14','python3 -c '+shlex.quote(remote)],timeout=60))
windows=[]
for k in (0,10,20):
 first,last=a[k],a[k+10];edges=(last['host_edges']-first['host_edges'])&0xffffffff;issues=(last['issued']-first['issued'])&0xffffffff;dt=last['monotonic']-first['monotonic']
 assert 9<dt<11 and 49<edges/dt/1e6<51,(edges,dt)
 rate=issues*50000000/edges
 windows.append(dict(delta_host_edges=edges,delta_issued=issues,elapsed_observer_s=dt,implied_host_mhz=edges/dt/1e6,issued_per_second=rate,ordinary_nominal_rate_fraction=rate/(1e9/60)))
q=dict(utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),units='StartCycle issues qualified Run/Jam, excluding RepeatCur; NOT pipeline-drained retirements',selected_instruction_view=1,host_clock_hz=50000000,workload='Fresh Native49b original-firmware Cedar login, no Guest input; SDRAM/no raw cache, Main21e6 disk-cache+trace, direct-DDR network serverc599, original f3bce2a8 seed, Pup time1792324800;50MHz/SYSPER4. RBF exact hash in deployment.json; matched against Native49 with the same Main/server/DDR transport and clean processes; this is a repeat of the exact Native49 RBF with fresh processes and seed; deferred reads OFF, interleaving and response-boundary successor selection ON.',samples=a,windows=windows,limits='Three adjacent ten-second windows in ONE boot; not three independent cold boots. Separate cause views are not one atomic all-counter measurement.')
(out/'idle-login-issues.json').write_text(json.dumps(q,indent=2)+'\n')
print('Idle login issues/s:',[round(w['issued_per_second'],2)for w in windows], 'mean',statistics.mean(w['issued_per_second']for w in windows),flush=True)
