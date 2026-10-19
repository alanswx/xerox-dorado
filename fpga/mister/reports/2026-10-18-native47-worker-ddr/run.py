from pathlib import Path
import json,shlex,subprocess,time
b=Path(__file__).resolve().parent
pre=r'''from pathlib import Path
import hashlib,os
assert hashlib.sha256(Path('/proc/1509/exe').read_bytes()).hexdigest()=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert os.readlink('/proc/1509/fd/20')=='/media/fat/games/DoradoRTL/Cedar47_20261018/cedar-work.pack'
assert hashlib.sha256(Path('/tmp/dorado47-loader-probe.so').read_bytes()).hexdigest()=='55bc9a8c1435779951835aa18ad1a3922066f8efba5ca586554ab6e81333053d'
for p in ('/tmp/dorado47-loader-worker.arm','/tmp/dorado47-loader-worker.jsonl','/tmp/dorado47-loader-worker-observer.json'):
 assert not Path(p).exists(),p
'''
subprocess.run(['ssh','mister14','python3 -c '+shlex.quote(pre)],check=True)
subprocess.run(['scp',str(b/'load.gdb'),'mister14:/tmp/dorado47-load-worker-probe.gdb'],check=True)
with (b/'load.log').open('w')as f:
 subprocess.run(['ssh','mister14','gdb -q -batch /tmp/dorado-main-21e6.elf -p 1509 -x /tmp/dorado47-load-worker-probe.gdb'],stdout=f,stderr=subprocess.STDOUT,check=True)
observer=r'''from pathlib import Path
import json,time,os
p=Path('/tmp/dorado47-loader-worker.jsonl');limit=time.monotonic()+6
while not p.exists() or 'waiting' not in p.read_text():
 assert time.monotonic()<limit,'worker probe did not start';time.sleep(.01)
f=open('/tmp/dorado47-main.log');f.seek(0,2)
s=open('/media/fat/games/DoradoRTL/Cedar47_20261018/server.log');s.seek(0,2)
# Let normal Main polling resume before arming; setup pause is outside the
# measured interval, and a worker waiting for its gate is reported separately.
fresh=0;limit=time.monotonic()+4
while fresh<2:
 text=f.read();fresh+=text.count('Dorado net:')
 assert time.monotonic()<limit,'Main poll heartbeat absent';time.sleep(.01)
s.read();start_read_ns=time.monotonic_ns()
fd=os.open('/tmp/dorado47-loader-worker.arm',os.O_WRONLY|os.O_CREAT|os.O_EXCL,0o600);os.close(fd)
print('ARMED '+json.dumps(dict(ns=time.monotonic_ns(),utc_ns=time.time_ns())),flush=True)
main=[];wire=[];last=start_read_ns;limit=time.monotonic()+9;end=None
while time.monotonic()<limit:
 now=time.monotonic_ns();chunk=f.read()
 if chunk:main.append(dict(interval_begin_ns=last,observed_ns=now,text=chunk))
 chunk=s.read()
 if chunk:wire.append(dict(interval_begin_ns=last,observed_ns=now,text=chunk))
 rows=[json.loads(line)for line in p.read_text().splitlines()]
 end=next((x for x in rows if x['phase']=='end'),None)
 if end and now>end['ns']+1500000000:break
 last=now;time.sleep(.01)
assert end and end['result']==0 and end['error']==0,'probe did not complete real fsync'
q=dict(events=rows,main=main,wire=wire,finished_ns=time.monotonic_ns())
Path('/tmp/dorado47-loader-worker-observer.json').write_text(json.dumps(q,indent=2)+'\n')
print('COMPLETE '+json.dumps(dict(events=rows,main_chunks=len(main),wire_chunks=len(wire))),flush=True)
'''
with (b/'observer.log').open('w')as f:
 p=subprocess.Popen(['ssh','mister14','python3 -u -c '+shlex.quote(observer)],stdout=subprocess.PIPE,stderr=f,text=True)
 line=p.stdout.readline();f.write(line);f.flush();assert line.startswith('ARMED '),line;print(line.strip(),flush=True)
 with (b/'capture.log').open('w')as cap:
  subprocess.run(['/Applications/MiSTerKVMS.app/Contents/MacOS/misterkvms-cli','--mister','10.0.2.14','--stream','mkvm://10.0.2.128/Mister%2014','screenshot',str(b/'during-worker.png')],stdout=cap,stderr=subprocess.STDOUT,check=True)
 check="from pathlib import Path; import json,time; a=[json.loads(x)for x in Path('/tmp/dorado47-loader-worker.jsonl').read_text().splitlines()]; print(json.dumps(dict(ns=time.monotonic_ns(),ended=any(x['phase']=='end'for x in a))))"
 state=json.loads(subprocess.check_output(['ssh','mister14','python3 -c '+shlex.quote(check)]))
 (b/'capture-timing.json').write_text(json.dumps(state,indent=2)+'\n');print('Capture before worker ended:',not state['ended'],flush=True)
 f.write(p.stdout.read());assert p.wait()==0
subprocess.run(['scp','mister14:/tmp/dorado47-loader-worker-observer.json',str(b/'observer.json')],check=True)
subprocess.run(['scp','mister14:/tmp/dorado47-disk.jsonl',str(b/'disk-prefix.jsonl')],check=True)
print('Collected actual Main/daemon/worker interval; inspect and qualify exact records.',flush=True)
