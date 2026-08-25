#!/usr/bin/env python3
"""Check DskEth's IOB read multiplexer against the Hardware Manual.

A `Pd<-Input` addressed to the disk/ethernet board reads through ten MC10174
four-input multiplexers, one per pair of IOB bits. The MC10174 data sheet's
truth table (DoradoDocs/datasheets/MC10174.pdf) makes pin 7 (`A`) the LSB of
the select and pin 9 (`B`) the MSB, and DskEth wires A = TIOA.7a, B = TIOA.5a
-- so TIOA.6 is IGNORED and the eight register addresses pair up on reads:

    {B,A} = 00 -> D00   TIOA 010 / 012   DskData    the read FIFO
            01 -> D01   TIOA 011 / 013   the MUFFLER
            10 -> D02   TIOA 014 / 016   ETHERNET STATUS
            11 -> D03   TIOA 015 / 017   EthData

Both registers `dorado/src/disk.c` implements land exactly there --
DORADO_DISK_TIOA_DISKDATA (012) reads the FIFO, DORADO_DISK_TIOA_DISKMUFF
(011) reads the muffler -- and so does the ethernet half:
DORADO_ETHERNET_TIOA_DATA is 015 and DORADO_ETHERNET_TIOA_CTL is 016.

Two claims are asserted here because two models made them independently:

1. **The muffler arrives on IOB.15 alone.** HM pp.101-102, transcribed in
   disk.c as `uint16_t v = bit ? 0x0001 : 0x0000;` -- one bit, not sixteen.
   The wire list leaves that mux input open on every other data bit, and
   drives the two PARITY bits with a constant and the muffler's COMPLEMENT,
   which is what keeps IOB's parity right when one data bit is ever driven.

2. **The host number occupies IOB.00-07, the HIGH byte.** ethernet.c's
   `eth_read` returns `(uint16_t)eth->local_host << 8` for TIOA 016, and the
   netlist puts Host.0..Host.7 on IOB.00..IOB.07 in order. PARC numbers IOB
   MSB-first, so those eight ARE the high byte.

Note for the C emulator: the same mux carries eight ETHERNET STATUS FLAGS in
the low byte (RxOn, TxOn, LoopBack, TxCollision, NoWakeups, TxDataLate,
SingleStep, TxFifoPE). `eth_read` returns zero for all of them.
"""
import glob
import sys

sys.path.insert(0, __file__.rsplit('/', 1)[0])
import sil_netlist as sn

# MC10174 data sheet pin assignment (NOT EclDict's X/Y labels):
#   D00,3 D01,5 D02,4 D03,6 -> Q0,2      D10,13 D11,11 D12,12 D13,10 -> Q1,15
CHANNELS = ((2, (3, 5, 4, 6)), (15, (13, 11, 12, 10)))
SEL_NAMES = ('DskData', 'muffler', 'EthStatus', 'EthData')

MUFFLER_BIT = 'IOB.15'                                   # HM pp.101-102
PARITY_BITS = {'IOB.16', 'IOB.17'}
HOST_BITS = {f'IOB.{i:02d}': f'Host.{i}' for i in range(8)}   # local_host << 8


def main() -> int:
    board = sn.load_board(glob.glob('chm/sil/DskEth*/*.wl')[0])
    pin_of = {}
    for name, net in board.nets.items():
        for p in net['pins']:
            pin_of.setdefault(p['pkg'], {})[p['pin']] = name

    rows, failures = {}, []
    for pkg, rec in sorted(board.packages.items()):
        if rec.get('type') != 'MC10174':
            continue
        pins = pin_of.get(pkg, {})
        for out_pin, src_pins in CHANNELS:
            out = pins.get(out_pin)
            if not out or not out.startswith('IOB.'):
                continue
            rows[out] = ([pins.get(s) for s in src_pins], pkg)

    hdr = f"{'IOB':<8}" + "".join(f"{n:<14}" for n in SEL_NAMES)
    print(hdr)
    for out in sorted(rows):
        srcs, pkg = rows[out]
        print(f"{out:<8}" + "".join(f"{(s or '-'):<14}" for s in srcs) + pkg)

    # 1. the muffler (select 01) on IOB.15 alone
    for out, (srcs, pkg) in rows.items():
        if srcs[1] is None or out in PARITY_BITS:
            continue
        if out != MUFFLER_BIT:
            failures.append(f"{pkg}: muffler mux input connected on {out} "
                            f"(to {srcs[1]}); HM pp.101-102 put it on "
                            f"{MUFFLER_BIT} alone")
    if MUFFLER_BIT not in rows or rows[MUFFLER_BIT][0][1] is None:
        failures.append(f"nothing drives the muffler mux input on {MUFFLER_BIT}")

    # 2. the host number (select 10) on IOB.00-07, in order -- the HIGH byte
    for out, want in sorted(HOST_BITS.items()):
        got = rows.get(out, ([None] * 4, '?'))[0][2]
        if got != want:
            failures.append(f"{out} takes {got} from the EthStatus mux input, "
                            f"not {want}; ethernet.c returns local_host << 8")

    if failures:
        for f in failures:
            print(f"FAIL: {f}")
        return 1
    print(f"PASS: the muffler reaches IOB on {MUFFLER_BIT} alone (HM pp.101-102, "
          f"disk.c's 0x0001), and Host.0-7 occupy IOB.00-07, the high byte "
          f"(ethernet.c's local_host << 8)")
    return 0


if __name__ == '__main__':
    sys.exit(main())
