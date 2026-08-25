#!/usr/bin/env python3
"""Check DskEth's IOB read multiplexer against the Hardware Manual.

A `Pd<-Input` addressed to the disk board reads through ten MC10174 four-input
multiplexers, one per pair of IOB bits, selected by {TIOA.5a, TIOA.7a}:

    {S2,S1} = 00 -> X0   TIOA 010 / 012   DskData   (the read FIFO)
              01 -> X1   TIOA 011 / 013   the MUFFLER
              10 -> X2   TIOA 014 / 016   Host
              11 -> X3   TIOA 015 / 017   EthData

HM pages 101-102 say the selected muffler signal is driven on **IOB[15]** --
one bit, not sixteen -- and `dorado/src/disk.c` transcribes exactly that:

    /* HM pages 101-102: the selected muffler signal is driven on
     * IOB[15]. ... */
    uint16_t v = bit ? 0x0001 : 0x0000;

So the wire list must leave the X1/Y1 pin OPEN on every IOB data bit but 15.
That is a strong claim about a 1979 netlist made by a C emulator written from
the manual, and neither was derived from the other, so it is worth asserting.

The parity bits corroborate it: IOB.17 takes MufData' (the complement) and
IOB.16 a constant, which is what keeps IOB's parity right when exactly one
data bit is driven.
"""
import glob
import sys

sys.path.insert(0, __file__.rsplit('/', 1)[0])
import sil_netlist as sn

# EclDict @MC174: X0,3 X1,5 X2,4 X3,6 -> OX,2 ; Y0,13 Y1,11 Y2,12 Y3,10 -> OY,15
X1_PIN, Y1_PIN, OX_PIN, OY_PIN = 5, 11, 2, 15
MUFFLER_BIT = 'IOB.15'          # HM pp.101-102, and disk.c's 0x0001
PARITY_BITS = {'IOB.16', 'IOB.17'}


def main() -> int:
    board = sn.load_board(glob.glob('chm/sil/DskEth*/*.wl')[0])
    pin_of = {}
    for name, net in board.nets.items():
        for p in net['pins']:
            pin_of.setdefault(p['pkg'], {})[p['pin']] = name

    connected, failures = [], []
    for pkg, pins in sorted(pin_of.items()):
        if board.packages.get(pkg, {}).get('type') != 'MC10174':
            continue
        for src_pin, out_pin in ((X1_PIN, OX_PIN), (Y1_PIN, OY_PIN)):
            out = pins.get(out_pin)
            if out is None or not out.startswith('IOB.'):
                continue
            src = pins.get(src_pin)
            if src is None:
                continue                      # open, which is the norm
            connected.append((out, src, pkg))
            if out in PARITY_BITS:
                continue                      # parity, not a data bit
            if out != MUFFLER_BIT:
                failures.append(
                    f"{pkg}: the muffler mux input is connected on {out} "
                    f"(to {src}); HM pp.101-102 put it on {MUFFLER_BIT} alone")

    for out, src, pkg in sorted(connected):
        print(f"  {out:<8} X1/Y1 = {src:<12} ({pkg})")

    if not any(out == MUFFLER_BIT for out, _, _ in connected):
        failures.append(f"nothing drives the muffler mux input on {MUFFLER_BIT}")

    if failures:
        for f in failures:
            print(f"FAIL: {f}")
        return 1
    print(f"PASS: the muffler reaches IOB on {MUFFLER_BIT} alone, "
          f"as HM pp.101-102 and src/disk.c both say")
    return 0


if __name__ == '__main__':
    sys.exit(main())
