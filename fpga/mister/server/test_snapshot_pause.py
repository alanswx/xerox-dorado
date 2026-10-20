#!/usr/bin/env python3
"""Real Pup process: preserve EFTP and BSP/STP across DDR snapshot pause.

Only the linked mapper is a fixture; control/ring/server and Ethernet code are
the actual implementations. This is a host functional test, not physical DDR
coherence, a serialized server checkpoint, or FPGA snapshot qualification.
"""
import hashlib
import mmap
import os
from pathlib import Path
import socket
import struct
import subprocess
import sys
import tempfile
import time

exe,root=(Path(p).resolve()for p in sys.argv[1:])
MAGIC=0x31474e49524f4444;ABI=0x0001080000100008;CTRL=0x00314c5254434444


class FPGA:
    def __init__(self,path):
        self.file=path.open('w+b');self.file.truncate(65536);self.mem=mmap.mmap(self.file.fileno(),65536)
    def get(self,offset):return struct.unpack_from('<I',self.mem,offset)[0]
    def put(self,offset,value):struct.pack_into('<I',self.mem,offset,value&0xffffffff)
    def seed(self,epoch):
        self.epoch=epoch;self.mem[:]=b'\0'*65536
        struct.pack_into('<QQ',self.mem,0,MAGIC,epoch);self.put(0x30,1);struct.pack_into('<Q',self.mem,0x38,ABI);self.mem.flush()
    def send(self,words=None,frame=None):
        if frame is None:frame=struct.pack('>'+str(len(words))+'H',*words)
        assert 4<=len(frame)<=2048 and not len(frame)%2
        prod=self.get(0x10);assert ((prod-self.get(0x18))&0xffffffff)<8,'test TX overflow'
        slot=prod&7;at=0x1000+slot*2048;self.mem[at:at+len(frame)]=frame
        struct.pack_into('<IIQ',self.mem,0x100+slot*16,len(frame),prod,self.epoch)
        # File-backed host oracle only. Flush before publishing ownership;
        # physical ARM/FPGA MMIO ordering is covered by the separate ring gates.
        self.mem.flush();self.put(0x10,prod+1);self.mem.flush()
    def receive(self,timeout=2):
        deadline=time.monotonic()+timeout
        while time.monotonic()<deadline:
            cons=self.get(0x28);pending=(self.get(0x20)-cons)&0xffffffff
            if pending:
                assert pending<=16,'test RX overflow';slot=cons&15
                size,sequence,epoch=struct.unpack_from('<IIQ',self.mem,0x200+slot*16)
                assert sequence==cons and epoch==self.epoch and 4<=size<=2048 and not size%2
                frame=self.mem[0x5000+slot*2048:0x5000+slot*2048+size]
                self.put(0x28,cons+1);self.mem.flush();return frame
            time.sleep(.001)
        raise AssertionError('real server response timeout')
    def words(self):
        frame=self.receive();return list(struct.unpack('>'+str(len(frame)//2)+'H',frame))
    def close(self):self.mem.close();self.file.close()


with tempfile.TemporaryDirectory(prefix='dorado-snapshot-pause-',dir='/tmp')as tmp:
    tmp=Path(tmp);fpga=FPGA(tmp/'ring.bin');epoch=0x591ceda700123456;fpga.seed(epoch)
    env=dict(os.environ,DORADO_NET_DDR='1',DORADO_TEST_DDR_MEMORY=str(tmp/'ring.bin'),
             DORADO_NET_CONTROL=str(tmp/'server.sock.control'))
    server=str(tmp/'server.sock');control=socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)
    proc=subprocess.Popen([str(exe),server,str(root/'dorado/worlds/aemu.eb'),str(root/'chm/bootfiles/CRTTEST.BOOT!1'),str(root/'chm/cedar/stp-root')],env=env)
    try:
        for _ in range(200):
            if Path(server+'.control').exists():break
            assert proc.poll()is None,'pause fixture exited at startup';time.sleep(.005)
        control.connect(server+'.control');control.settimeout(2);sequence=0
        def rpc(command,cookie=epoch,flags=1):
            global sequence
            sequence+=1;control.sendall(struct.pack('<QIIQII',CTRL,command,sequence,cookie,0x1ff00000,65536))
            reply=b''
            while len(reply)<32:
                part=control.recv(32-len(reply));assert part,'control EOF';reply+=part
            magic,ack,seq,actual,error,active=struct.unpack('<QIIQII',reply)
            assert (magic,ack,seq,actual,error,active)==(CTRL,command|0x80000000,sequence,cookie,0,flags),(command,actual,error,active)
        rpc(2)
        fpga.send([0o42,0o1000,0o26,0o244,0,0,0,0,4,0o42,0,0,0,0,0xffff])
        first=fpga.words();assert first[3]==0o30 and first[5]==0
        rpc(4,flags=3);saved=bytes(fpga.mem);time.sleep(.25);rpc(3,flags=3)
        assert bytes(fpga.mem)==saved,'paused daemon mutated DDR'
        rpc(5);prod=fpga.get(0x20);time.sleep(.015)
        assert fpga.get(0x20)==prod,'retry timer advanced across pause'
        rpc(4,flags=3)
        fpga.send([0o42,0o1000,0o26,0o31,0,0,0,0,0o20,0o42,0,0,0xffff])
        saved=bytes(fpga.mem);time.sleep(.2)
        assert bytes(fpga.mem)==saved,'paused daemon consumed queued ACK'
        rpc(5);packet=fpga.words();assert len(packet)>=13 and packet[3]in(0o30,0o32)and packet[5]==1,'continue reset EFTP or expired retry timer'
        image=first[12:-1];block=1
        while True:
            assert packet[5]==block and packet[3]in(0o30,0o32)
            if packet[3]==0o30:image.extend(packet[12:-1])
            fpga.send([0o42,0o1000,0o26,0o31,0,block,0,0,0o20,0o42,0,0,0xffff])
            if packet[3]==0o32:break
            block+=1;packet=fpga.words()
        boot=root/'chm/bootfiles/CRTTEST.BOOT!1'
        assert struct.pack('>'+str(len(image))+'H',*image)==boot.read_bytes()
        print('SNAPSHOT PAUSE EFTP PASS: pending data/ACK, frozen retries, repeated pause, full boot payload',len(image)*2,flush=True)
        # A real lifecycle replacement deliberately clears the previous session.
        rpc(1,cookie=epoch,flags=0);epoch+=1;fpga.seed(epoch);rpc(2,cookie=epoch)
        tx=rx=(0o12345<<16)|0o67123;paused=False;in_file=False;phase='plist';plist=bytearray();image=bytearray();frames=0
        def send(typ,ident,payload):
            header=[0x122,0o1000,22+len(payload),typ,ident>>16,ident&65535,0x101,0,3,0x122,0,0o20]
            fpga.send(frame=struct.pack('>12H',*header)+payload+(b'\0'if len(payload)%2 else b'')+b'\xff\xff')
        def receive():
            frame=fpga.receive();p=struct.unpack('>12H',frame[:24]);return p,frame[24:24+max(0,p[2]-22)]
        def ack():send(0o22,rx,struct.pack('>3H',1478,6,1478*6))
        def mark(value,text=b''):
            global tx
            payload=bytes([value])+text;send(0o23,tx,payload);tx+=len(payload)
        send(0o10,tx,struct.pack('>3H',0x122,0,0o20));assert receive()[0][3]==0o10;assert receive()[0][3]==0o22;ack()
        original=root/'chm/cedar/stp-root/Cedar6.1/VersionMap/CedarSource.VersionMap'
        mark(1,b'((User-Name Guest.pa)(Server-filename [Cedar]<Cedar6.1>VersionMap>CedarSource.VersionMap)(Desired-property Size))');mark(6)
        while True:
            p,payload=receive();typ=p[3]
            if typ not in(0o20,0o21,0o23,0o26):continue
            ident=(p[4]<<16)|p[5];assert ident==rx,(hex(ident),hex(rx),typ)
            if phase=='file'and in_file and typ in(0o20,0o21)and not paused:
                rpc(4,cookie=epoch,flags=3);saved=bytes(fpga.mem);time.sleep(.2);rpc(3,cookie=epoch,flags=3)
                assert bytes(fpga.mem)==saved,'paused BSP state accessed DDR'
                rpc(5,cookie=epoch);paused=True
            rx+=len(payload);ack()
            if typ in(0o23,0o26):
                value=payload[0];assert value!=4,('STP refused',plist)
                if phase=='plist'and value==6:
                    assert f'(Size {original.stat().st_size})'.encode()in plist
                    phase='file';mark(3);mark(6)
                elif phase=='file'and value==5:in_file=True
                elif phase=='file'and value==3:in_file=False
                elif phase=='file'and value==6:break
            elif phase=='plist':plist.extend(payload)
            elif in_file:image.extend(payload);frames+=1
        assert paused and image==original.read_bytes(),('BSP continuation differs',len(image),original.stat().st_size)
        print('SNAPSHOT PAUSE BSP PASS:',len(image),'bytes,',frames,'frames, SHA256',hashlib.sha256(image).hexdigest(),flush=True)
        rpc(1,cookie=epoch,flags=0)
    finally:
        control.close();proc.terminate();assert proc.wait(timeout=5)==0;fpga.close()
