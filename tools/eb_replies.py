#!/usr/bin/env python3
"""Build the MicrocodeBootReply packet stream for a .eb boot file as a
Manchester transition list a testbench can replay onto DskEth's RcvData
(tb_exec +rcvplay=PATH).

Packet layout is `append_reply` in dorado/src/ethernet.c, which Initial's
input task (InitialEther.mc, EIEnd) accepts: word 0 = dest host ,, source
host; 1 = Pup ethernet type 1000B; then the Pup: length (26B + 2n bytes),
type 265B (MicrocodeBootReply), id high 1, id low = sequence number, dest
port (net,,host = local host, socket 1,1), source port (server host, socket
0,4), n payload words, and a nil checksum (177777B) inside the length. The
.eb file is a big-endian word stream: version word 1, then the image from
byte 512, sent in 255-word pieces, ended by an empty reply.

On the wire (HM 11): 340 ns bit cells, phase encoded, one sync bit (1) then
the words MSB first, then the 9401's CRC-16 (x16+x15+x2+1, register zero,
data bits MSB first, sync excluded) as the last 16 bits. `pol` is the
encoding polarity, settled against the recorded request (decode_xmt.py).

Output lines are `<offset sys_clk> <level>` relative to the start of the
stream, plus a `# packet N at <offset>` comment before each packet.
"""
import argparse, struct, sys
sys.path.insert(0, __import__('os').path.dirname(__file__))
from decode_xmt import decode, words as dec_words

PUP_ETHERNET = 0o1000
BOOT_REPLY   = 0o265

def crc16(bits):
    r = 0
    for b in bits:
        fb = b ^ (r >> 15)
        r = ((r << 1) & 0xFFFF)
        if fb: r ^= 0x8005
    return r

def crc16_bits(bits):
    c = crc16(bits); return [(c >> (15 - i)) & 1 for i in range(16)]

def eb_words(path):
    data = open(path, 'rb').read()
    if struct.unpack('>H', data[:2])[0] != 1: raise SystemExit(f"{path}: version word is not 1")
    body = data[512:]
    if len(body) % 2: body = body[:-1]
    return list(struct.unpack('>%dH' % (len(body) // 2), body))

def reply_words(lhost, rhost, seq, payload):
    n = len(payload)
    hdr = [((lhost & 0xff) << 8) | (rhost & 0xff), PUP_ETHERNET, 0o26 + 2 * n, BOOT_REPLY,
           1, seq & 0xffff, lhost & 0xff, 1, 1, rhost & 0xff, 0, 4]
    return hdr + list(payload) + [0o177777]

def packet_bits(wl):
    bits = [int(c) for w in wl for c in format(w & 0xffff, '016b')]
    return [1] + bits + crc16_bits(bits)                 # sync, data, CRC

def encode_bits(bits, cell, t0, pol):
    """Phase-encode a bit list from time t0 (float cell); returns transitions and the end time."""
    trans = []; level = 0; t = t0
    for bit in bits:
        want_mid = bit ^ pol
        if level == want_mid:
            level ^= 1; trans.append((int(round(t)), level))
        level = want_mid; trans.append((int(round(t + cell / 2)), level)); t += cell
    if level != 0: trans.append((int(round(t)), 0))     # back to idle
    return trans, t

def build(words_in, lhost, rhost, cell, gap, pol, chunk=255):
    pkts = []; pos = 0; seq = 0
    while pos < len(words_in):
        pkts.append(reply_words(lhost, rhost, seq, words_in[pos:pos + chunk])); pos += chunk; seq += 1
    pkts.append(reply_words(lhost, rhost, seq, []))
    out = []; t = 0.0; starts = []
    for i, wl in enumerate(pkts):
        starts.append(int(round(t)))
        tr, t = encode_bits(packet_bits(wl), cell, t, pol)
        out.append((i, starts[-1], tr)); t += gap
    return pkts, out

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('eb'); ap.add_argument('out')
    ap.add_argument('--lhost', type=lambda x: int(x, 0), default=0, help='the Dorado (dest) host')
    ap.add_argument('--rhost', type=lambda x: int(x, 0), default=0o100, help='the server (source) host')
    ap.add_argument('--sysper', type=int, default=16); ap.add_argument('--pol', type=int, default=0)
    ap.add_argument('--gap-us', type=float, default=200.0, help='idle between packets')
    ap.add_argument('--selftest', action='store_true')
    a = ap.parse_args()
    cell = 340.0 / (30.0 / a.sysper); gap = a.gap_us * 1000.0 / (30.0 / a.sysper)
    wl = eb_words(a.eb)
    pkts, out = build(wl, a.lhost, a.rhost, cell, gap, a.pol)
    with open(a.out, 'w') as f:
        for i, t0, tr in out:
            f.write(f"# packet {i} at {t0} ({len(pkts[i])} words)\n")
            for t, lv in tr: f.write(f"{t} {lv}\n")
    total = out[-1][2][-1][0]
    print(f"{a.eb}: {len(wl)} words -> {len(pkts)} packets, {total} sys_clk ({total * (30.0 / a.sysper) / 1e6:.1f} ms), pol={a.pol}")
    if a.selftest:
        # round-trip every packet through the decoder and re-check its CRC
        bad = 0
        for i, t0, tr in out:
            for pol in (0, 1):
                for sk in (False, True):
                    pk = decode(tr, cell, pol, sk)
                    if pk and pk[0][0] == 1:
                        ws, rem = dec_words(pk[0])
                        if rem == 0 and ws[:-1] == pkts[i] and crc16(pk[0][1:]) == 0: break
                else: continue
                break
            else: bad += 1
        print(f"selftest: {len(pkts) - bad} of {len(pkts)} packets decode back with the CRC-16 of data+check = 0")
        if bad: sys.exit(1)

if __name__ == '__main__': main()
