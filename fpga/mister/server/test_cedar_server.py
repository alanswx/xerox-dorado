#!/usr/bin/env python3
"""Qualify the Cedar boot mapping and large BSP/STP transfers, without a CPU.

Usage: test_cedar_server.py SERVER_EXECUTABLE REPOSITORY_OR_STAGED_ROOT
The root must carry dorado/worlds and chm/{dorado,bootfiles,cedar/stp-root}.
"""
import pathlib,socket,struct,subprocess,tempfile,time,hashlib,sys
exe,root=(pathlib.Path(p).resolve() for p in sys.argv[1:])
source=root/'chm/dorado/CedarDorado.eb!6'
with tempfile.TemporaryDirectory(prefix='dorado-cedar-bfn-',dir='/tmp') as tmp:
 server=str(pathlib.Path(tmp)/'server.sock')
 proc=subprocess.Popen([str(exe),server,str(root/'dorado/worlds/aemu.eb'),str(root/'chm/bootfiles/CRTTEST.BOOT!1'),str(root/'chm/cedar/stp-root')],cwd=root/'dorado')
 try:
  for _ in range(100):
   if pathlib.Path(server).exists():break
   assert proc.poll() is None
   time.sleep(.01)
  with socket.socket(socket.AF_UNIX,socket.SOCK_DGRAM) as s:
   s.bind(str(pathlib.Path(tmp)/'client.sock'));s.settimeout(3)
   s.sendto(b'',server)
   # Captured Cedar cold-boot routing probe. Pup type200 also names the
   # file-lookup protocol, so an enabled STP root must not swallow socket2.
   routing_id=(0xa55a,0x1234)
   route=[0o42,0o1000,0o26,0o200,*routing_id,0,0,2,0o42,0,2,0xffff]
   s.sendto(struct.pack('>13H',*route),server)
   deadline=time.monotonic()+2
   while True:
    s.settimeout(max(.01,deadline-time.monotonic()))
    frame=s.recv(4096);p=struct.unpack('>'+str(len(frame)//2)+'H',frame)
    if p[3]==0o201 and p[4:6]==routing_id:break
    assert time.monotonic()<deadline,'no matching routing reply with STP enabled'
   assert p[7:9]==(0,2) and p[10:12]==(0,2)
   assert p[12:14]==(0x0101,0x0100) and len(frame)==p[2]+4
   print('CEDAR-ROUTING PASS: captured socket2/type200 probe with STP enabled')
   s.settimeout(3)
   request=[0o42,0o1000,0o26,0o264,1,0o113,0,0,0,0,4,0o42,1,1,0xffff]
   s.sendto(struct.pack('>15H',*request),server)
   image=bytearray();seq=0
   while True:
    data=s.recv(4096);p=struct.unpack('>'+str(len(data)//2)+'H',data)
    assert p[0]==(0o42<<8)|1 and p[1]==0o1000 and p[3]==0o265
    assert p[4]==1 and p[5]==seq and len(data)==p[2]+4 and p[-1]==0xffff
    payload=data[24:-2]
    if not payload:break
    image.extend(payload);seq+=1
   assert image==source.read_bytes()[512:]
   print('CEDAR-BFN113 PASS:',len(image),'payload bytes,',seq,'frames, SHA256',hashlib.sha256(image).hexdigest())
   # Real Cedar drains gatewayInfo until Get times out. A steady stream of
   # idle routing packets prevents that timeout, unlike this socket client.
   # Observe longer than two broadcast periods, with no outstanding request.
   s.settimeout(3.2)
   try:
    unexpected=s.recv(4096)
   except socket.timeout:
    pass
   else:
    raise AssertionError(('unsolicited packet after Cedar boot',unexpected.hex()))
   s.settimeout(3)
   s.sendto(struct.pack('>13H',*route),server)
   frame=s.recv(4096);p=struct.unpack('>'+str(len(frame)//2)+'H',frame)
   assert p[3]==0o201 and p[4:6]==routing_id and p[7:9]==(0,2)
   print('CEDAR-QUIET PASS: no idle broadcasts for3.2s; solicited routing still answers')
  # Same process, replacement Main: configure must release Cedar quiet mode.
  with socket.socket(socket.AF_UNIX,socket.SOCK_DGRAM) as replacement:
   replacement.bind(str(pathlib.Path(tmp)/'replacement.sock'));replacement.settimeout(3)
   replacement.sendto(b'',server)
   frame=replacement.recv(4096);p=struct.unpack('>'+str(len(frame)//2)+'H',frame)
   assert p[1]==0o602,'replacement Main did not receive breath-of-life'
   print('CEDAR-QUIET-REEXEC PASS: fresh Main receives boot broadcast')
 finally:
  proc.terminate();assert proc.wait(timeout=5)==0

with tempfile.TemporaryDirectory(prefix='dorado-cedar-stp-',dir='/tmp') as tmp:
 server=str(pathlib.Path(tmp)/'server.sock')
 original=root/'chm/cedar/stp-root/Cedar6.1/VersionMap/CedarSource.VersionMap'
 proc=subprocess.Popen([str(exe),server,str(root/'dorado/worlds/aemu.eb'),str(root/'chm/bootfiles/CRTTEST.BOOT!1'),str(root/'chm/cedar/stp-root')],cwd=root/'dorado')
 try:
  for _ in range(100):
   if pathlib.Path(server).exists():break
   assert proc.poll() is None
   time.sleep(.01)
  with socket.socket(socket.AF_UNIX,socket.SOCK_DGRAM) as s:
   s.bind(str(pathlib.Path(tmp)/'client.sock'));s.settimeout(5)
   s.sendto(b'',server)
   tx=rx=(0o12345<<16)|0o67123
   max_frame=0;data_frames=0
   def send(typ,ident,payload):
    header=[0x122,0o1000,22+len(payload),typ,ident>>16,ident&65535,0x101,0,3,0x122,0,0o20]
    frame=struct.pack('>12H',*header)+payload+(b'\0' if len(payload)%2 else b'')+b'\xff\xff'
    s.sendto(frame,server)
   def receive():
    global max_frame
    frame=s.recv(4096);max_frame=max(max_frame,len(frame))
    p=struct.unpack('>12H',frame[:24]);return p,frame[24:24+max(0,p[2]-22)]
   def ack():send(0o22,rx,struct.pack('>3H',1478,6,1478*6))
   def mark(value,text=b''):
    global tx
    payload=bytes([value])+text;send(0o23,tx,payload);tx+=len(payload)
   send(0o10,tx,struct.pack('>3H',0x122,0,0o20))
   assert receive()[0][3]==0o10
   assert receive()[0][3]==0o22
   ack()
   mark(1,b'((User-Name Guest.pa)(Server-filename [Cedar]<Cedar6.1>VersionMap>CedarSource.VersionMap)(Desired-property Size))')
   mark(6)
   plist=bytearray();image=bytearray();phase='plist';in_file=False
   while True:
    p,payload=receive();typ=p[3]
    if typ not in (0o20,0o21,0o23,0o26):continue
    ident=(p[4]<<16)|p[5]
    assert ident==rx,(hex(ident),hex(rx),typ)
    rx+=len(payload);ack()
    if typ in (0o23,0o26):
     value=payload[0]
     assert value!=4,('STP refused',plist)
     if phase=='plist' and value==6:
      assert f'(Size {original.stat().st_size})'.encode() in plist,plist
      phase='file';mark(3);mark(6)
     elif phase=='file' and value==5:in_file=True
     elif phase=='file' and value==3:in_file=False
     elif phase=='file' and value==6:break
    elif phase=='plist':plist.extend(payload)
    elif in_file:image.extend(payload);data_frames+=1
   original=root/'chm/cedar/stp-root/Cedar6.1/VersionMap/CedarSource.VersionMap'
   assert image==original.read_bytes(),(len(image),len(original.read_bytes()))
   assert max_frame>1024,max_frame
   print('CEDAR-STP PASS:',len(image),'file bytes,',data_frames,'data frames, largest frame',max_frame,'bytes, SHA256',hashlib.sha256(image).hexdigest())
 finally:
  proc.terminate();assert proc.wait(timeout=5)==0
