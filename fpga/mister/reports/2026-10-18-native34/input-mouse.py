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
 for event in sys.argv[1:]:
  if event.startswith('move:'):
   send('mouseMove:'+event[5:])
  elif event in ('left_down','left_up','right_down','right_up','middle_down','middle_up'):
   send('mouseBtn:'+event)
  else:raise ValueError(event)
  print('Sent',event);time.sleep(.8)
finally:
 send('mouseBtn:left_up');send('mouseBtn:right_up');send('mouseBtn:middle_up');s.close()
