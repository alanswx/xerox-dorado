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

THE WRITE SIDE IS CHECKED TOO. A DISKMUFF output carries four clear controls,
and d19 (MC10173) takes them from four consecutive IOB bits. PARC numbers IOB
MSB-first, so IOB.04 is 0x0800 -- and `disk.c` names the same four values:

    bIOB.04 -> ClearIndexTW    0x0800   DORADO_DISK_MUFF_CLEAR_INDEX_TW
    bIOB.05 -> ClearSectorTW   0x0400   DORADO_DISK_MUFF_CLEAR_SECTOR_TW
    bIOB.06 -> ClearTWs        0x0200   DORADO_DISK_MUFF_CLEAR_SEEKTAG_TW
    bIOB.07 -> ClearErrors     0x0100   DORADO_DISK_MUFF_CLEAR_ERRORS

Every one of d19's other mux inputs is `DisableRun`, so disabling the
controller asserts all four clears -- which is why disk.c's CLR_ENABLE_RUN
branch zeroes index_tw, sector_tw and tag_tw in the same breath.

AND THE FOUR PER-BLOCK OPS. disk.h gives the rest of the control word as four
2-bit operation fields, one per block:

    B[8:9] 1st   B[10:11] 2nd   B[12:13] 3rd   B[14:15] 4th
    0 = Done, 1 = Write, 2 = Read+Check, 3 = Read

The board holds them as TWO PARALLEL 4-BIT SHIFT REGISTERS, one per bit of the
op rather than one per op: f14 takes the EVEN bits (bIOB.08/.10/.12/.14) and
f15 the ODD (bIOB.09/.11/.13/.15). Both are F10000s with PE' = `Active`, so
they PARALLEL-LOAD while the controller is idle and SHIFT while it runs --
stepping one op per block -- and MR = `DisableRun` clears the sequence.

f14's H0 is named `ReadBlock`, and that follows: it is the HIGH bit of the
current op, which is 1 for both Read+Check (2) and Read (3). "This block
reads."
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

    # 3. the DISKMUFF clear controls (write side), d19 -- four consecutive
    #    IOB bits whose values disk.c names independently.
    MUFF_CLEARS = {'IOB.04': ('ClearIndexTW', 0x0800),
                   'IOB.05': ('ClearSectorTW', 0x0400),
                   'IOB.06': ('ClearTWs', 0x0200),
                   'IOB.07': ('ClearErrors', 0x0100)}
    # EclDict @MC173: D0,5 D1,3 D2,12 D3,10 -> Q0,1 Q1,2 Q2,15 Q3,14
    D19 = ((5, 1), (3, 2), (12, 15), (10, 14))
    d19 = pin_of.get('d19', {})
    print()
    for dpin, qpin in D19:
        src, out = d19.get(dpin), d19.get(qpin)
        bit = (src or '').replace('bIOB.', 'IOB.')
        want = MUFF_CLEARS.get(bit)
        print(f"  {src or '-':<10} -> {out or '-':<16} "
              f"{'0x%04x' % want[1] if want else ''}")
        if want is None:
            failures.append(f"d19 pin {dpin} takes {src}, which is not one of "
                            f"the four DISKMUFF clear bits")
        elif out != want[0]:
            failures.append(f"{bit} drives {out}, not {want[0]}; disk.c gives "
                            f"that bit the value 0x{want[1]:04x}")

    # 4. the four per-block ops (write side), f14/f15 -- two 4-bit shift
    #    registers split by BIT of the op, not by op.
    OPS = {'f14': [f'bIOB.{i:02d}' for i in (8, 10, 12, 14)],
           'f15': [f'bIOB.{i:02d}' for i in (9, 11, 13, 15)]}
    D_PINS = (11, 10, 9, 7)                       # F10000 D0..D3
    print()
    for pkg, want in OPS.items():
        got = [pin_of.get(pkg, {}).get(k) for k in D_PINS]
        print(f"  {pkg}: D0-D3 = {', '.join(g or '-' for g in got)}  "
              f"PE'={pin_of.get(pkg, {}).get(5)}  MR={pin_of.get(pkg, {}).get(12)}")
        if got != want:
            failures.append(f"{pkg} takes {got} as the per-block op bits, not {want}")
        if pin_of.get(pkg, {}).get(5) != 'Active':
            failures.append(f"{pkg}'s PE' is {pin_of.get(pkg, {}).get(5)}, not Active -- "
                            f"it must load while idle and shift while running")

    if failures:
        for f in failures:
            print(f"FAIL: {f}")
        return 1
    print(f"PASS: the muffler reaches IOB on {MUFFLER_BIT} alone (HM pp.101-102, "
          f"disk.c's 0x0001), and Host.0-7 occupy IOB.00-07, the high byte "
          f"(ethernet.c's local_host << 8), the four DISKMUFF clear bits sit "
          f"where disk.c's constants put them, and the four per-block ops split "
          f"by BIT across f14/f15")
    return 0


if __name__ == '__main__':
    sys.exit(main())
