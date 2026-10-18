#!/usr/bin/env python3
"""Drop an unrequested STP completion tail; the daemon must probe/recover.

No courtesy ACKs: acknowledge only aData/aMark, as a real BSP receiver may.
Use a full32-bit client socket, and verify file bytes through the Unix peer.
"""
import hashlib
import pathlib
import socket
import struct
import subprocess
import sys
import tempfile
import time

exe, root = (pathlib.Path(p).resolve() for p in sys.argv[1:])
with tempfile.TemporaryDirectory(prefix='dorado-bsp-retry-', dir='/tmp') as tmp:
    tmp = pathlib.Path(tmp)
    stp = tmp/'stp'
    (stp/'Fixture').mkdir(parents=True)
    expected = bytes(range(256))*4 + bytes(range(12))
    (stp/'Fixture/font.bin').write_bytes(expected)
    server = str(tmp/'server.sock')
    with (tmp/'server.txt').open('w') as log:
        proc = subprocess.Popen([
            str(exe), server, str(root/'dorado/worlds/aemu.eb'),
            str(root/'chm/bootfiles/CRTTEST.BOOT!1'), str(stp),
        ], cwd=root/'dorado', stdout=log, stderr=subprocess.STDOUT)
        try:
            for _ in range(100):
                if pathlib.Path(server).exists():
                    break
                assert proc.poll() is None, 'server exited during startup'
                time.sleep(.01)
            with socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM) as sock:
                sock.bind(str(tmp/'client.sock'))
                sock.settimeout(6)
                sock.sendto(b'', server)
                tx = rx = 0x12345678
                client_hi, client_lo = 0x12, 0x9426

                def send(typ, ident, payload=b''):
                    header = [0x122, 0o1000, 22+len(payload), typ,
                              ident>>16, ident&65535, 0x101, 0, 3,
                              0x122, client_hi, client_lo]
                    frame = struct.pack('>12H', *header)+payload
                    if len(payload) % 2:
                        frame += b'\0'
                    sock.sendto(frame+b'\xff\xff', server)

                def mark(value, text=b''):
                    global tx
                    payload = bytes([value])+text
                    send(0o23, tx, payload)
                    tx += len(payload)

                def ack():
                    send(0o22, rx, struct.pack('>3H', 1478, 32, 1478*32))

                # Exercise retries in the actual quiet-after-Cedar mode,
                # not just in a default/Alto server session.
                boot = [0o42, 0o1000, 0o26, 0o264, 1, 0o113,
                        0, 0, 0, 0, 4, 0o42, 1, 1, 0xffff]
                sock.sendto(struct.pack('>15H', *boot), server)
                microcode = bytearray()
                seq = 0
                while True:
                    frame = sock.recv(4096)
                    header = struct.unpack('>12H', frame[:24])
                    assert header[3] == 0o265 and header[4:6] == (1, seq)
                    data = frame[24:-2]
                    if not data:
                        break
                    microcode.extend(data)
                    seq += 1
                assert microcode == (root/'chm/dorado/CedarDorado.eb!6').read_bytes()[512:]

                send(0o10, tx, struct.pack('>3H', 0x122, client_hi, client_lo))
                phase = 'rfc'
                payload_bytes = bytearray()
                in_file = False
                dropped = False
                probes = 0
                recovered = False
                deadline = time.monotonic()+8
                while time.monotonic() < deadline:
                    frame = sock.recv(4096)
                    header = struct.unpack('>12H', frame[:24])
                    if header[7:9] != (client_hi, client_lo):
                        continue  # unrelated idle routing/time broadcasts
                    typ = header[3]
                    ident = (header[4]<<16)|header[5]
                    data = frame[24:24+max(0, header[2]-22)]
                    if phase == 'rfc':
                        if typ == 0o10:
                            continue
                        assert typ == 0o22
                        ack()
                        mark(1, b'((Directory Fixture)(Name-Body font.bin)(Desired-property Size))')
                        mark(6)
                        phase = 'plist'
                        continue
                    if typ not in (0o20, 0o21, 0o23, 0o26):
                        continue
                    if typ == 0o21 and not data:
                        probes += 1
                    # Lose the first Yes/text/EOC trailer in its entirety.
                    # The receiver then discards later sequence positions,
                    # and has no reason to ACK until an aData/aMark arrives.
                    if phase == 'file' and ident == rx and data == b'\3' and not dropped:
                        dropped = True
                        continue
                    if ident == rx:
                        rx += len(data)
                        if typ in (0o23, 0o26):
                            if phase == 'plist' and data == b'\6':
                                mark(3, b'Yes, please')
                                mark(6)
                                phase = 'file'
                            elif phase == 'file' and data == b'\5':
                                in_file = True
                            elif phase == 'file' and data == b'\3':
                                assert dropped and probes >= 2
                                in_file = False
                            elif phase == 'file' and data == b'\6' and dropped:
                                recovered = True
                        elif in_file:
                            payload_bytes.extend(data)
                    if typ in (0o21, 0o26):
                        ack()  # including out-of-sequence empty probes
                    if recovered:
                        break
                assert recovered, f'STP completion never recovered (probes={probes}, file_bytes={len(payload_bytes)}, dropped={dropped})'
                assert payload_bytes == expected, 'file changed during retransmission'
                print('BSP-RETRY PASS: dropped completion tail recovered after',
                      probes, 'automatic probes;', len(expected), 'file bytes, SHA256',
                      hashlib.sha256(payload_bytes).hexdigest())
        except socket.timeout:
            raise AssertionError('no automatic BSP probe/recovery after lost completion tail')
        finally:
            proc.terminate()
            assert proc.wait(timeout=5) == 0
