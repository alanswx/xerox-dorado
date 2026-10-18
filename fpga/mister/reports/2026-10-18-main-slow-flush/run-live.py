from pathlib import Path
import json,os,shlex,subprocess,time
b=Path(__file__).resolve().parent
meta=json.loads((b/'library.json').read_text())
pre='''from pathlib import Path
import hashlib,os
assert hashlib.sha256(Path('/proc/16875/exe').read_bytes()).hexdigest()=='40e14b6b97d62435c6e1ecd5c78bf9959c74b816a8e9452b1e64c28179ca8188'
assert os.readlink('/proc/16875/fd/20')=='/media/fat/games/DoradoRTL/Cedar41c_20261018/cedar-work.pack'
assert hashlib.sha256(Path(LIB).read_bytes()).hexdigest()==SHA
assert not Path('/tmp/dorado41c-slowflush.arm').exists()
assert not Path('/tmp/dorado41c-slowflush.jsonl').exists()
'''.replace('LIB',repr(meta['path'])).replace('SHA',repr(meta['sha256']))
subprocess.run(['ssh','mister14','python3 -c '+shlex.quote(pre)],check=True)
with (b/'load-live.log').open('w') as f:
 subprocess.run(['ssh','mister14','gdb -q -batch /tmp/dorado-main-40e1.elf -p 16875 -x /tmp/dorado41c-load-probe.gdb'],stdout=f,stderr=subprocess.STDOUT,check=True,timeout=12)
observer=r'''from pathlib import Path
import json,time,os
p=Path('/tmp/dorado41c-slowflush.jsonl');deadline=time.monotonic()+5
while not p.exists() or 'waiting' not in p.read_text():
 assert time.monotonic()<deadline,'probe did not enter worker';time.sleep(.01)
f=open('/tmp/dorado41c-main.log');f.seek(0,2)
s=open('/media/fat/games/DoradoRTL/Cedar41c_20261018/server.log');s.seek(0,2)
arm_ns=time.monotonic_ns();fd=os.open('/tmp/dorado41c-slowflush.arm',os.O_WRONLY|os.O_CREAT|os.O_EXCL,0o600);os.close(fd)
print('ARMED '+json.dumps(dict(monotonic_ns=arm_ns,utc_ns=time.time_ns())),flush=True)
lines=[];wire=[];deadline=time.monotonic()+8;done_ns=None
while time.monotonic()<deadline:
 chunk=f.read()
 if chunk:lines.append(dict(observed_ns=time.monotonic_ns(),text=chunk))
 chunk=s.read()
 if chunk:wire.append(dict(observed_ns=time.monotonic_ns(),text=chunk))
 rows=[json.loads(x)for x in p.read_text().splitlines()if x.endswith('}')]
 end=next((x for x in rows if x['phase']=='end'),None)
 if end is not None:
  done_ns=end['ns']
  if time.monotonic_ns()>done_ns+500000000:break
 time.sleep(.01)
assert done_ns is not None,'probe did not complete'
j=dict(arm_ns=arm_ns,events=rows,main_log_samples=lines,server_log_samples=wire,finished_ns=time.monotonic_ns())
Path('/tmp/dorado41c-slowflush-observer.json').write_text(json.dumps(j,indent=2)+'\n')
print('COMPLETE '+json.dumps(dict(events=rows,main_chunks=len(lines),server_chunks=len(wire))),flush=True)
'''
with (b/'observer-console.log').open('w') as out:
 observer_proc=subprocess.Popen(['ssh','mister14','python3 -u -c '+shlex.quote(observer)],stdout=subprocess.PIPE,stderr=out,text=True)
 line=observer_proc.stdout.readline();out.write(line);out.flush();assert line.startswith('ARMED '),line
 print(line.strip(),flush=True)
 started=time.time_ns()
 with (b/'during-keyboard.log').open('w') as k,(b/'during-mouse.log').open('w') as m:
  keyboard=subprocess.Popen(['python3','fpga/mister/reports/2026-10-18-native34/input-keyboard.py','\necho 42\n','0.1'],stdout=k,stderr=subprocess.STDOUT)
  mouse=subprocess.Popen(['python3','fpga/mister/reports/2026-10-18-native34/input-mouse.py','move:40,20'],stdout=m,stderr=subprocess.STDOUT)
  assert keyboard.wait(timeout=10)==0
  assert mouse.wait(timeout=10)==0
 time.sleep(.25)
 capture_before=time.time_ns()
 with (b/'during-capture.log').open('w') as f:
  subprocess.run(['/Applications/MiSTerKVMS.app/Contents/MacOS/misterkvms-cli','--mister','10.0.2.14','--stream','mkvm://10.0.2.128/Mister%2014','screenshot',str(b/'during-flush.png')],stdout=f,stderr=subprocess.STDOUT,check=True,timeout=8)
 # An independent remote check establishes whether capture finished before
 # the flush ended, avoiding any assumption about Mac/ARM clock alignment.
 check="from pathlib import Path; import json,time; a=[json.loads(x)for x in Path('/tmp/dorado41c-slowflush.jsonl').read_text().splitlines()]; print(json.dumps(dict(monotonic_ns=time.monotonic_ns(),ended=any(x['phase']=='end'for x in a))))"
 state=json.loads(subprocess.check_output(['ssh','mister14','python3 -c '+shlex.quote(check)],timeout=5))
 (b/'capture-timing.json').write_text(json.dumps(dict(keyboard_start_utc_ns=started,capture_start_utc_ns=capture_before,remote_check_after_capture=state),indent=2)+'\n')
 out.write(observer_proc.stdout.read());assert observer_proc.wait(timeout=10)==0
 print('Captured during flush:',not state['ended'],flush=True)
print('Live probe complete; inspect captured output and flush trace after the window.',flush=True)
