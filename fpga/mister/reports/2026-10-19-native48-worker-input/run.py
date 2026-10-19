"""One-shot actual-worker test. Require a verified ready second CommandTool
with `echo worker48` fully typed but Return withheld, pretyped.png captured.
No Main/FPGA guest-state patches; only audited worker enqueue and real input.
The DSO MUST stay loaded until Main exits. Never blindly rerun this script.
"""
from pathlib import Path
import base64,json,os,shlex,socket,subprocess,time

b=Path(__file__).resolve().parent
assert (b/'pretyped.png').exists(),'Inspect the full typed command/idle tool first'
pre=r'''from pathlib import Path
import hashlib,os
assert hashlib.sha256(Path('/proc/3781/exe').read_bytes()).hexdigest()=='21e67a3f5588a875320437133df316b7e8f87061eb06b6dcaf63ecb139e29b60'
assert os.readlink('/proc/3781/fd/20')=='/media/fat/games/DoradoRTL/Cedar48_20261019/cedar-work.pack'
assert hashlib.sha256(Path('/tmp/dorado48-input-probe.so').read_bytes()).hexdigest()=='55bc9a8c1435779951835aa18ad1a3922066f8efba5ca586554ab6e81333053d'
assert 'dorado48-input-probe.so'not in Path('/proc/3781/maps').read_text()
for name in ('arm','jsonl'):
 assert not Path('/tmp/dorado48-input-worker.'+name).exists()
assert not Path('/tmp/dorado48-input-worker-observer.json').exists()
'''
subprocess.run(['ssh','mister14','python3 -c '+shlex.quote(pre)],check=True)
key=base64.b64encode(os.urandom(16)).decode()
s=socket.create_connection(('10.0.2.14',8182),timeout=10)
s.sendall((f'GET /api/ws HTTP/1.1\r\nHost: 10.0.2.14:8182\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: {key}\r\nSec-WebSocket-Version: 13\r\nOrigin: http://10.0.2.14:8182\r\n\r\n').encode())
headers=b''
while b'\r\n\r\n'not in headers:headers+=s.recv(4096)
assert headers.startswith(b'HTTP/1.1 101'),headers[:100]
def send(text):
 data=text.encode();mask=os.urandom(4)
 assert len(data)<126
 s.sendall(bytes([0x81,0x80|len(data)])+mask+bytes(x^mask[i%4]for i,x in enumerate(data)))
for local,remote in [('load.gdb','/tmp/dorado48-input-load.gdb'),('observe.py','/tmp/dorado48-input-observe.py')]:
 subprocess.run(['scp',str(b/local),'mister14:'+remote],check=True)
with (b/'load.log').open('w')as f:
 subprocess.run(['ssh','mister14','gdb -q -batch /tmp/dorado-main-21e6.elf -p 3781 -x /tmp/dorado48-input-load.gdb'],stdout=f,stderr=subprocess.STDOUT,check=True)
with (b/'observer.log').open('w')as log:
 p=subprocess.Popen(['ssh','mister14','python3 -u /tmp/dorado48-input-observe.py'],stdout=subprocess.PIPE,stderr=log,text=True)
 line=p.stdout.readline();log.write(line);log.flush()
 assert line.startswith('BEGIN '),line
 origin=time.monotonic();print(line.strip(),flush=True)
 try:
  send('kbdRawDown:28');time.sleep(.2);send('kbdRawUp:28')
  send('mouseMove:40,20')
  captures=[]
  for number,target in enumerate((2.1,3.0),1):
   time.sleep(max(0,target-(time.monotonic()-origin)))
   with (b/f'capture-{number}.log').open('w')as cap:
    subprocess.run(['/Applications/MiSTerKVMS.app/Contents/MacOS/misterkvms-cli','--mister','10.0.2.14','--stream','mkvm://10.0.2.128/Mister%2014','screenshot',str(b/f'during-{number}.png')],stdout=cap,stderr=subprocess.STDOUT,check=True)
   code="from pathlib import Path; import json,time; a=[json.loads(x)for x in Path('/tmp/dorado48-input-worker.jsonl').read_text().splitlines()]; print(json.dumps(dict(ns=time.monotonic_ns(),ended=any(x['phase']=='end'for x in a))))"
   state=json.loads(subprocess.check_output(['ssh','mister14','python3 -c '+shlex.quote(code)]))
   captures.append(dict(number=number,**state));print('CAPTURE '+json.dumps(captures[-1]),flush=True)
  (b/'capture-timing.json').write_text(json.dumps(captures,indent=2)+'\n')
 finally:
  send('kbdRawUp:28');s.close()
  log.write(p.stdout.read());assert p.wait()==0
subprocess.run(['scp','mister14:/tmp/dorado48-input-worker-observer.json',str(b/'observer.json')],check=True)
data=subprocess.check_output(['ssh','mister14','cat /tmp/dorado48-disk.jsonl'])
(b/'disk-prefix.jsonl').write_bytes(data[:data.rfind(b'\n')+1])
print('Collected real-worker/input/captures. Qualify intervals and visually verify new output/prompt/mouse before claiming success.',flush=True)
