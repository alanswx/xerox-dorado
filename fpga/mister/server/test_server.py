#!/usr/bin/env python3
"""Exercise the process/socket boundary, without CPU emulation or FPGA state."""
import pathlib
import socket
import struct
import subprocess
import sys
import tempfile
import time

exe, root = (pathlib.Path(p).resolve() for p in sys.argv[1:])
microcode = root / "dorado/worlds/aemu.eb"
boot = root / "chm/bootfiles/CRTTEST.BOOT!1"

def words(data):
    assert len(data) % 2 == 0
    return list(struct.unpack(f">{len(data)//2}H", data))

with tempfile.TemporaryDirectory(prefix="dorado-pup-", dir="/tmp") as tmp:
    server = str(pathlib.Path(tmp) / "server.sock")
    client = str(pathlib.Path(tmp) / "client.sock")
    proc = subprocess.Popen([str(exe), server, str(microcode), str(boot)])
    try:
        for _ in range(100):
            if pathlib.Path(server).exists():
                break
            assert proc.poll() is None, "server exited at startup"
            time.sleep(.01)
        with socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM) as s:
            s.bind(client)
            s.settimeout(3)
            s.sendto(b"", server)  # Main registers without fabricating a frame.
            # A second live client cannot steal a registered session.
            with socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM) as contender:
                contender.bind(str(pathlib.Path(tmp) / "contender.sock"))
                contender.sendto(b"", server)
            def send(packet):
                s.sendto(struct.pack(f">{len(packet)}H", *packet), server)
            def receive():
                return words(s.recv(4096))
            # Microcode boot: the .eb leader stays on the server, and every
            # returned payload word equals the committed AEmu image.
            send([0o42, 0o1000, 0o26, 0o264, 1, 0o110,
                  0, 0, 0, 0, 4, 0o42, 1, 1, 0xffff])
            image = []
            seq = 0
            while True:
                p = receive()
                assert p[0] == (0o42 << 8) | 1
                assert p[1] == 0o1000 and p[3] == 0o265
                assert p[4] == 1 and p[5] == seq
                assert len(p)*2 == p[2] + 4
                assert p[-1] == 0xffff  # nil Pup checksum, not hardware CRC
                payload = p[12:-1]
                if not payload:
                    break
                image.extend(payload)
                seq += 1
            assert image == words(microcode.read_bytes()[512:])
            # EFTP: drop the first Data packet. The server must resend it
            # without a new Mayday, then honor lock-step acknowledgements.
            send([0o42, 0o1000, 0o26, 0o244, 0, 0,
                  0, 0, 4, 0o42, 0, 0, 0, 0, 0xffff])
            first = receive()
            assert first == receive(), "missing or changed EFTP retransmission"
            image = []
            seq = 0
            p = first
            while True:
                assert p[5] == seq
                assert len(p)*2 == p[2] + 4  # Ethernet header plus Pup
                assert p[-1] == 0xffff       # real nil Pup checksum
                assert p[3] in (0o30, 0o32)
                if p[3] == 0o30:
                    image.extend(p[12:-1])
                send([0o42, 0o1000, 0o26, 0o31, 0, seq,
                      0, 0, 0o20, 0o42, 0, 0, 0xffff])
                if p[3] == 0o32:
                    break
                seq += 1
                p = receive()
            assert image == words(boot.read_bytes())
            dally = receive()
            assert dally[3] == 0o32 and dally[5] == seq + 1
            assert receive()[1] == 0o602  # idle breath-of-life, no extra request
            routing = receive()
            assert routing[1] == 0o1000 and routing[3] == 0o201
            assert len(routing)*2 == routing[2] + 4
            print(f"PUP-SERVER PASS: {len(microcode.read_bytes())-512} microcode bytes, "
                  f"{len(image)*2} EFTP bytes, retransmission, idle broadcasts, no controller trailers")
        # Re-exec uses a new PID/path and may leave the old bound pathname.
        # Register immediately after a broadcast, before the idle sender
        # detects closure. With no guest TX, BOL must still reach new Main.
        with socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM) as replacement:
            replacement.bind(str(pathlib.Path(tmp) / "replacement.sock"))
            replacement.settimeout(3)
            replacement.sendto(b"", server)
            packet = words(replacement.recv(4096))
            assert packet[1] == 0o602, "new Main did not receive idle breath-of-life"
            print("PUP-SERVER REEXEC PASS: immediate new-path registration, stale old socket, idle BOL without guest TX")
    finally:
        proc.terminate()
        assert proc.wait(timeout=5) == 0
