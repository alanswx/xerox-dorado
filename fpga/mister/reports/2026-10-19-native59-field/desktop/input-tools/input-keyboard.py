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
keycodes=dict(zip('qwertyuiopasdfghjklzxcvbnm',list(range(16,26))+list(range(30,39))+list(range(44,51))))
keycodes.update({'\n':28,' ':57,'.':52,'/':53,'-':12})
keycodes.update(zip('1234567890',range(2,12)))
text=sys.argv[1] if len(sys.argv)>1 else 'Guest\n\n'
interval=float(sys.argv[2]) if len(sys.argv)>2 else .8
sequence=[(keycodes[c.lower()],c.isupper()) for c in text]
try:
 for code,shift in sequence:
  if shift:send('kbdRawDown:42');time.sleep(1.0)
  send(f'kbdRawDown:{code}');time.sleep(interval)
  send(f'kbdRawUp:{code}')
  if shift:send('kbdRawUp:42')
  time.sleep(interval)
 print('Sent',repr(text),'through real virtual USB keyboard',flush=True)
finally:
 for code in {42}|{x[0] for x in sequence}:send(f'kbdRawUp:{code}')
 s.close()
