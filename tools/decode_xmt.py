#!/usr/bin/env python3
"""Decode a recorded Ethernet transmit line (tb_exec +xmtrec=PATH) into words.

The recording is one line per transition: `<sys_clk index> <level>`. The
Dorado's 3 Mb/s Ethernet is phase (Manchester) encoded with a 340 ns bit cell
(HM 11), and sys_clk is 30 ns / SYSPER, so a cell is 340 / (30/SYSPER)
sys_clk -- 181.3 at SYSPER=16. Every bit has a transition at mid-cell; a
transition at a cell BOUNDARY appears only when two consecutive bits are
equal. So walking the transitions from the first one: an interval of about a
cell is the next mid-cell (a data bit, its value the transition's direction),
an interval of about half a cell is a boundary transition to skip. The packet
starts with a single sync bit (a 1) and ends when the line goes idle; the
last 16 data bits are the hardware CRC.

Polarity of the recorded line and of the encoding are both unknown a priori,
so both are tried and the reading whose sync bit is 1 and whose first word
looks like a Pup ethernet header is printed. --selftest round-trips a known
word list through an encoder.
"""
import argparse, sys

def decode(trans, cell, pol, skip_first=False):
    """trans: list of (t, level). Returns list of packets, each a list of bits.
    skip_first: the packet's first transition is a cell BOUNDARY (the idle
    level was the one the sync bit's mid-cell transition would have left it
    at), so the sync bit's mid-cell is the SECOND transition."""
    pkts = []; i = 0; n = len(trans)
    while i < n:
        if skip_first and i + 1 < n and trans[i+1][0] - trans[i][0] < 0.75 * cell: i += 1
        t0, lv = trans[i]
        bits = [1 if (lv ^ pol) else 0]
        last_mid = t0; j = i + 1
        while j < n:
            t, lv = trans[j]; d = t - last_mid
            if d > 1.6 * cell: break                      # line idle: packet over
            if d < 0.75 * cell: j += 1; continue          # boundary transition
            bits.append(1 if (lv ^ pol) else 0); last_mid = t; j += 1
        pkts.append(bits); i = j
    return pkts

def words(bits):
    b = bits[1:]                                          # drop the sync bit
    return [int(''.join(str(x) for x in b[k:k+16]), 2) for k in range(0, len(b) - 15, 16)], len(b) % 16

def encode(wordlist, cell, start=1000, pol=0):
    """Manchester-encode sync + words; returns transitions (t, level)."""
    bits = [1] + [int(c) for w in wordlist for c in format(w, '016b')]
    trans = []; level = 0; t = start
    for k, bit in enumerate(bits):
        want_mid = (bit ^ pol)                            # level AFTER the mid-cell transition
        if level == want_mid:                             # need a boundary transition first
            level ^= 1; trans.append((t, level))
        level = want_mid; trans.append((t + cell // 2, level)); t += cell
    return trans

def report(pkts, tag):
    for pi, bits in enumerate(pkts):
        ws, rem = words(bits)
        print(f"{tag} packet {pi}: sync={bits[0]} {len(bits)-1} data bits = {len(ws)} words + {rem} bits")
        print("   " + ' '.join(f"{w:06o}" for w in ws))
        if len(ws) >= 8:
            print(f"   dest host {ws[0]>>8:03o} src host {ws[0]&0xff:03o} ether type {ws[1]:06o}"
                  + (f" | Pup length {ws[2]} type {ws[3]&0xff:03o} ({ws[3]&0xff}) hop {ws[3]>>8}" if ws[1] == 0o1000 else ''))

def main():
    ap = argparse.ArgumentParser(); ap.add_argument('path', nargs='?'); ap.add_argument('--sysper', type=int, default=16)
    ap.add_argument('--selftest', action='store_true'); a = ap.parse_args()
    cell = 340.0 / (30.0 / a.sysper)
    if a.selftest:
        wl = [0o000000, 0o001000, 0o000034, 0o000264, 0o001234, 0o005670, 0o177777, 0o052525]
        for pol in (0, 1):
            tr = encode(wl, int(round(cell)), pol=pol)
            oks = []
            for dp in (0, 1):
                for sk in (False, True):
                    pk = decode(tr, cell, dp, sk); ws, rem = words(pk[0])
                    if ws == wl and rem == 0 and pk[0][0] == 1: oks.append((dp, sk))
            print(f"selftest enc pol={pol}: decoded correctly by {oks}")
        return
    trans = []
    for line in open(a.path):
        p = line.split()
        if len(p) == 2: trans.append((int(p[0]), int(p[1])))
    print(f"{len(trans)} transitions, cell = {cell:.1f} sys_clk")
    if len(trans) < 3: return
    for pol in (0, 1):
        for sk in (False, True):
            pk = decode(trans, cell, pol, sk)
            if pk and pk[0][0] == 1: report(pk, f"pol={pol} skip_first={sk}")
    # interval histogram, to see whether the timing is even Manchester
    from collections import Counter
    iv = Counter(int(round((trans[k+1][0] - trans[k][0]) / (cell / 2))) for k in range(len(trans) - 1))
    print("interval histogram (units of half a cell):", sorted(iv.items())[:12])

if __name__ == '__main__': main()
