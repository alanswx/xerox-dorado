#!/usr/bin/env python3
"""Repeat/delay command Pups through the real server; stream bytes enter once.

Reproduce StorageBoard57's empty CommandTool retrieve: a valid Retrieve mark,
its data, an older retransmitted AMark, then EOC. Also duplicate a data fragment,
send EOC ahead of a missing fragment, and cross the32-bit byte-ID wrap. No CPU,
guest state or daemon-memory injection is involved.
"""
from pathlib import Path
import os
import socket
import struct
import subprocess
import sys
import tempfile
import time

exe, root = (Path(p).resolve() for p in sys.argv[1:])
MASK = (1 << 32) - 1
for scenario in ('duplicate-mark', 'duplicate-data', 'future-EOC'):
    for start in (0x12345678, 0xfffffff8):
        with tempfile.TemporaryDirectory(prefix='dorado-rx-order-', dir='/tmp') as temp:
            temp = Path(temp)
            stp = temp / 'stp'
            (stp / 'Fixture').mkdir(parents=True)
            expected = bytes(range(256)) * 4 + b'byte-stream-once'
            (stp / 'Fixture/font.bin').write_bytes(expected)
            endpoint = str(temp / 'server.sock')
            env = dict(os.environ, DORADO_FTP_TRACE='1')
            log = (temp / 'server.log').open('w')
            proc = subprocess.Popen([str(exe), endpoint, str(root / 'dorado/worlds/aemu.eb'),
                                     str(root / 'chm/bootfiles/CRTTEST.BOOT!1'), str(stp)],
                                    cwd=root / 'dorado', env=env, stdout=log, stderr=subprocess.STDOUT)
            try:
                for _ in range(100):
                    if Path(endpoint).exists():
                        break
                    assert proc.poll() is None
                    time.sleep(.01)
                with socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM) as sock:
                    sock.bind(str(temp / 'client.sock'))
                    sock.settimeout(3)
                    sock.sendto(b'', endpoint)
                    tx = rx = start
                    hi, lo = 0x12, 0x9426

                    def send(typ, ident, payload=b''):
                        header = [0x122, 0o1000, 22 + len(payload), typ,
                                  (ident >> 16) & 65535, ident & 65535, 0x101, 0, 3,
                                  0x122, hi, lo]
                        sock.sendto(struct.pack('>12H', *header) + payload +
                                    (b'\0' if len(payload) & 1 else b'') + b'\xff\xff', endpoint)

                    def receive():
                        while True:
                            frame = sock.recv(4096)
                            h = struct.unpack('>12H', frame[:24])
                            if h[7:9] == (hi, lo):
                                return h[3], (h[4] << 16) | h[5], frame[24:24 + max(0, h[2] - 22)]

                    def ack():
                        send(0o22, rx, struct.pack('>3H', 1478, 32, 32000))

                    def next_bytes(typ, payload):
                        global tx
                        ident = tx
                        send(typ, ident, payload)
                        tx = (tx + len(payload)) & MASK
                        return ident

                    def mark(value, text=b''):
                        return next_bytes(0o23, bytes([value]) + text)

                    send(0o10, tx, struct.pack('>3H', 0x122, hi, lo))
                    assert receive()[0] == 0o10
                    assert receive()[0] == 0o22
                    ack()
                    body = b'((Directory Fixture)(Name-Body font.bin)(Desired-property Size))'
                    retrieve_id = mark(1)
                    split = body.index(b'font.bin') + 4
                    first = body[:split]
                    data_id = next_bytes(0o20, first)
                    if scenario == 'duplicate-mark':
                        next_bytes(0o20, body[split:])
                        send(0o26, retrieve_id, b'\1')
                    elif scenario == 'duplicate-data':
                        send(0o21, data_id, first)
                        next_bytes(0o20, body[split:])
                    else:
                        future = (tx + len(body) - split) & MASK
                        send(0o26, future, b'\6')
                        next_bytes(0o20, body[split:])
                    next_bytes(0o26, b'\6')
                    phase = 'plist'
                    inside = False
                    actual = bytearray()
                    deadline = time.monotonic() + 6
                    while time.monotonic() < deadline:
                        typ, ident, data = receive()
                        if typ == 0o22:
                            continue
                        if typ not in (0o20, 0o21, 0o23, 0o26):
                            continue
                        if ident != rx:
                            if typ in (0o21, 0o26):
                                ack()
                            continue
                        rx = (rx + len(data)) & MASK
                        if typ in (0o23, 0o26):
                            assert data != b'\4', 'valid retrieve became STP fileNotFound after ' + scenario
                            if phase == 'plist' and data == b'\6':
                                phase = 'file'
                                mark(3, b'Yes')
                                mark(6)
                            elif phase == 'file' and data == b'\5':
                                inside = True
                            elif phase == 'file' and data == b'\3':
                                inside = False
                            elif phase == 'file' and data == b'\6':
                                ack()
                                break
                        elif inside:
                            actual.extend(data)
                        if typ in (0o21, 0o26):
                            ack()
                    else:
                        raise AssertionError('receive-order transfer never completed')
                    assert actual == expected, (scenario, start, len(actual))
                    print('BSP-RX-ORDER PASS', scenario, f'start={start:08x}', len(actual), 'exact file bytes', flush=True)
            except (socket.timeout, AssertionError):
                log.flush()
                print('\n'.join((temp / 'server.log').read_text().splitlines()[-35:]), file=sys.stderr)
                raise
            finally:
                proc.terminate()
                assert proc.wait(timeout=5) == 0
                log.close()
