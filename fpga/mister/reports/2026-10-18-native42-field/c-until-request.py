import base64,os,socket,struct,time,sys
host='10.0.2.14';port=8182
key=base64.b64encode(os.urandom(16)).decode()
s=socket.create_connection((host,port),timeout=10)
s.sendall((f'GET /api/ws HTTP/1.1\r\nHost: {host}:{port}\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: {key}\r\nSec-WebSocket-Version: 13\r\nOrigin: http://{host}:{port}\r\n\r\n').encode())
headers=b''
while b'\r\n\r\n'not in headers:headers+=s.recv(4096)
if not headers.startswith(b'HTTP/1.1 101'):raise RuntimeError(headers[:200])
def send(text):
 b=text.encode();mask=os.urandom(4)
 if len(b)>=126:raise ValueError('frame too long')
 s.sendall(bytes([0x81,0x80|len(b)])+mask+bytes(x^mask[i%4]for i,x in enumerate(b)))

import subprocess,shlex
remote = r"""import time,sys
f=open('/media/fat/games/DoradoRTL/Cedar42_20261018/server.log','rb')
f.seek(0,2)
print('Request watcher ready',flush=True)
end=time.monotonic()+900
pending=b''
while time.monotonic()<end:
 chunk=f.read()
 if not chunk:
  time.sleep(.01)
  continue
 pending+=chunk
 while b'\n' in pending:
  raw,pending=pending.split(b'\n',1)
  line=raw.decode('ascii','replace')
  words=line.split(': ',1)[1].split() if line.startswith('TX n=13: ') else []
  if len(words)==13 and words[3]=='000264' and words[5]=='000113':
   print('New Cedar request: '+line.strip(),flush=True)
   sys.exit(0)
raise RuntimeError('No new Cedar request within 15 minutes')
"""
poll=None
try:
 send('kbdRawUp:46');time.sleep(.2)
 send('kbdRawDown:46')
 print('Holding C until Initial sends BFN113, then releasing immediately',flush=True)
 poll=subprocess.Popen(['ssh','mister14','python3 -u -c '+shlex.quote(remote)])
 rc=poll.wait(timeout=930)
 if rc:raise RuntimeError('Request watcher failed: '+str(rc))
finally:
 send('kbdRawUp:46')
 print('C released',flush=True)
 s.close()
 if poll is not None and poll.poll() is None:poll.terminate()
