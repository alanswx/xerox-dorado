#!/usr/bin/env python3
"""A delayed forward ACK crosses a file rewind; the next retrieve must work.

Reproduce Native41's AMEvents->PrintTV failure using only real socket Pups.
The receiver gets the whole first file, delays its final ACK, repeats an
older ACK, then delivers the final ACK while a one-finger replay is active.
The first file and a second retrieve must remain byte-exact. Also cross the
32-bit sequence wrap. No guest CPU or server-memory injection is involved.
"""
from pathlib import Path
import hashlib,os,socket,struct,subprocess,sys,tempfile,time
exe,root=(Path(p).resolve()for p in sys.argv[1:])
MASK=(1<<32)-1
for start in (0x12345678,0xffffd000):
 with tempfile.TemporaryDirectory(prefix='dorado-ack-race-',dir='/tmp')as tmp:
  tmp=Path(tmp);stp=tmp/'stp';(stp/'Fixture').mkdir(parents=True)
  files={'first.bin':bytes(range(256))*142+bytes(range(250)),
         'next.bin':bytes((i*19+7)&255 for i in range(2204)),
         'ring.bin':bytes(range(256))*785}
  for name,data in files.items():(stp/'Fixture'/name).write_bytes(data)
  server=str(tmp/'server.sock');log=(tmp/'server.log').open('w')
  env=os.environ.copy();env['DORADO_FTP_TRACE']='1'
  proc=subprocess.Popen([str(exe),server,str(root/'dorado/worlds/aemu.eb'),str(root/'chm/bootfiles/CRTTEST.BOOT!1'),str(stp)],cwd=root/'dorado',env=env,stdout=log,stderr=subprocess.STDOUT)
  try:
   for _ in range(100):
    if Path(server).exists():break
    assert proc.poll()is None
    time.sleep(.01)
   with socket.socket(socket.AF_UNIX,socket.SOCK_DGRAM)as s:
    s.bind(str(tmp/'client.sock'));s.settimeout(3);s.sendto(b'',server)
    tx=rx=start;hi,lo=0x12,0x9426
    def send(typ,ident,payload=b''):
     header=[0x122,0o1000,22+len(payload),typ,(ident>>16)&65535,ident&65535,0x101,0,3,0x122,hi,lo]
     s.sendto(struct.pack('>12H',*header)+payload+(b'\0'if len(payload)&1 else b'')+b'\xff\xff',server)
    def receive():
     while True:
      frame=s.recv(4096);p=struct.unpack('>12H',frame[:24])
      if p[7:9]!=(hi,lo):continue
      return p[3],(p[4]<<16)|p[5],frame[24:24+max(0,p[2]-22)]
    def ack(value=None,fingers=32):
     send(0o22,rx if value is None else value,struct.pack('>3H',1478,fingers,32000))
    def mark(value,text=b''):
     global tx
     payload=bytes([value])+text;send(0o23,tx,payload);tx=(tx+len(payload))&MASK
    # Enable quiet-after-Cedar mode through the actual boot request.
    request=[0o42,0o1000,0o26,0o264,1,0o113,0,0,0,0,4,0o42,1,1,0xffff]
    s.sendto(struct.pack('>15H',*request),server)
    while True:
     frame=s.recv(4096);p=struct.unpack('>12H',frame[:24])
     assert p[3]==0o265
     if len(frame)==26:break
    send(0o10,tx,struct.pack('>3H',0x122,hi,lo))
    assert receive()[0]==0o10
    assert receive()[0]==0o22
    ack()
    def retrieve(name,delay_tail_ack=False):
     global rx
     mark(1,('((Directory Fixture)(Name-Body %s)(Desired-property Size))'%name).encode())
     mark(6)
     phase='plist';inside=False;data_out=bytearray();cut=None
     deadline=time.monotonic()+6
     while time.monotonic()<deadline:
      typ,ident,data=receive()
      if typ not in (0o20,0o21,0o23,0o26):continue
      if ident!=rx:
       if typ in (0o21,0o26):ack()
       continue
      rx=(rx+len(data))&MASK
      if typ in (0o23,0o26):
       if phase=='plist'and data==b'\6':
        phase='file';mark(3,b'Yes');mark(6)
       elif phase=='file'and data==b'\5':inside=True
       elif phase=='file'and data==b'\3':inside=False
       elif phase=='file'and data==b'\6':
        if not delay_tail_ack:ack()
        assert data_out==files[name],(name,len(data_out))
        return cut
      elif inside:data_out.extend(data)
      if delay_tail_ack and phase=='file'and inside and cut is None and len(data_out)>=len(files[name])-5000:
       cut=rx;ack(cut)
      elif cut is None:ack()
     raise AssertionError('retrieve did not finish: '+name)
    cut=retrieve('first.bin',True);assert cut is not None
    high=rx
    # Advertise one available finger so the replay cannot finish before
    # the delayed forward ACK reaches the sender, independent of scheduling.
    ack(cut,1);ack(cut,1)
    deadline=time.monotonic()+3;replay_end=None
    while time.monotonic()<deadline:
     typ,ident,data=receive()
     if ident==cut and data:
      replay_end=(ident+len(data))&MASK;break
    assert replay_end is not None and 0<((high-replay_end)&MASK)<(1<<31)
    ack(high)
    ack(cut)  # delayed stale ACK must not move the committed cursor backwards
    ack((high+1024)&MASK)  # unsent bytes must not be acknowledged either
    retrieve('next.bin')
    print('BSP-ACK-RACE PASS start=%08x old_ack=%08x replay_end=%08x final_ack=%08x second_file=%d SHA256=%s'%(start,cut,replay_end,high,len(files['next.bin']),hashlib.sha256(files['next.bin']).hexdigest()),flush=True)
    # More than two full history rings: ACK retirement must free entries
    # across wrap without losing the producer's order or payload bytes.
    retrieve('ring.bin')
    send(0o12,tx)
    # Previously queued replays may arrive before the EndReply.
    while receive()[0]!=0o13:pass
    # Reuse the client's socket and therefore its closed context slot.
    tx=rx=(start+0x100000)&MASK
    send(0o10,tx,struct.pack('>3H',0x122,hi,lo))
    assert receive()[0]==0o10
    assert receive()[0]==0o22
    ack()
    retrieve('next.bin')
    print('BSP-HISTORY-RING PASS start=%08x file=%d SHA256=%s; closed-slot reuse byte-exact'%(start,len(files['ring.bin']),hashlib.sha256(files['ring.bin']).hexdigest()),flush=True)
  except (socket.timeout,AssertionError):
   log.flush();lines=(tmp/'server.log').read_text().splitlines()
   print('\n'.join(line for line in lines if line.startswith(('FTP_REWIND','FTP_STALL','FTP_REPLAY','FTP_ACK_IGNORED'))),file=sys.stderr)
   print('\n'.join(lines[-32:]),file=sys.stderr)
   raise
  finally:
   proc.terminate();assert proc.wait(timeout=5)==0;log.close()
