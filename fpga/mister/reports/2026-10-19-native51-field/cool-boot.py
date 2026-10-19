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

try:
 for pulse in range(3):
  for code in (29,56,100):send(f'kbdRawDown:{code}');time.sleep(.05)
  print(f'Physical BaseBoard boot pulse {pulse+1}/3',flush=True)
  time.sleep(1)
  for code in (100,56,29):send(f'kbdRawUp:{code}');time.sleep(.05)
  time.sleep(1)
finally:
 for code in (100,56,29):send(f'kbdRawUp:{code}')
 s.close()
 print('Three pushes request firmware CoolBoot: reload microcode',flush=True)
