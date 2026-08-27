#!/usr/bin/env python3
"""Map ContB's IM packages to (microinstruction field bit, bank).

IM is 4096 x 36 bits, built from 144 F10415A packages -- each 1024 x 1 -- as
FOUR banks of 1024 words. A package therefore holds ONE bit of the
microinstruction for ONE bank, and the netlist says which:

  * its DATA OUTPUT names the field bit. The 36 distinct output nets are
    exactly a Dorado microinstruction:
        RSTK.0-3  ALUF.0-3  BSEL.0-2  LC.0-2  ASEL.0-2
        FF.0-7    JCN.0-7   Block'    IMLH  IMRH
    4+4+3+3+3+8+8+1+2 = 36.
  * its CHIP ENABLE names the bank -- CS0'/CS1'/CS2'/CS3', with a suffix
    (ACa..ACd, BDa..BDd) identifying which fan-out group drives it.
  * its ADDRESS pins are RA.01a..RA.10a, ten bits within a bank. The cell reads
    them MSB-FIRST: pin 2 takes RA.01a, which comes from TNIA.05. Getting that
    backwards is what once made a Write-IM addressing 195 deposit at 780.

THE INTERLEAVE, derived rather than assumed. The four chip selects are driven by
TWO MC10101s, wired-OR, and their inputs name the bits:

    d21  bdRA.11a on all four gate inputs, common DoCBr
    d22  bdRA.00a on all four gate inputs

So the bank is chosen by RA.00 and RA.11 -- the TOP and BOTTOM of the twelve
address bits, PARC numbering MSB-first -- while RA.01..RA.10 are the ten-bit
index within a bank:

    bank  = {addr[11], addr[0]}
    index = addr[10:1]

Consecutive addresses therefore alternate banks on the LSB, which is exactly
what tb_exec records as "copies 0,2 are A and 1,3 are B, so IM[0] = A and
IM[1] = B".

WHY THIS EXISTS: walking a whole 4096-word world into IM over the
control-processor bus takes far too long to simulate -- boot0-test does 16
microinstructions and exec-test 4 hunks. A bench that wants to RUN a real world
must preload the arrays directly. That is legitimate: the CP-bus path is already
gated by boot0-test, so preloading skips nothing unproven. But it must be
VERIFIED the same way boot0-test verifies its load -- read the arrays back and
compare against the .MB.
"""
import collections
import glob
import sys

sys.path.insert(0, __file__.rsplit('/', 1)[0])
import sil_netlist as sn

DI, DO, CE, WE = 15, 1, 14, 13
ADDR_PINS = (2, 3, 4, 5, 6, 7, 9, 10, 11, 12)   # A0..A9, MSB first


def im_packages():
    """(package, field-bit net, bank 0-3, CE net, address nets) per IM package."""
    board = sn.load_board(glob.glob('chm/sil/ContB*/*.wl')[0])
    out = []
    for pkg, rec in sorted(board.packages.items()):
        if rec.get('type') != 'F10415A':
            continue
        pins = {p['pin']: nm for nm, nt in board.nets.items()
                for p in nt['pins'] if p['pkg'] == pkg}
        ce = pins.get(CE) or ''
        bank = int(ce[2]) if len(ce) > 2 and ce[2].isdigit() else None
        out.append((pkg, pins.get(DO), bank, ce,
                    tuple(pins.get(k) for k in ADDR_PINS)))
    return out


def main() -> int:
    rows = im_packages()
    bits = collections.defaultdict(dict)
    for pkg, field, bank, ce, _ in rows:
        bits[field][bank] = (pkg, ce)

    print(f"{len(rows)} IM packages, {len(bits)} field bits, "
          f"{len({b for _, _, b, _, _ in rows})} banks\n")
    print(f"{'field bit':<12} " + "".join(f"bank{b:<8}" for b in range(4)))
    bad = []
    for field in sorted(bits):
        line = f"  {field:<10} "
        for b in range(4):
            ent = bits[field].get(b)
            line += f"{(ent[0] if ent else '--'):<12}"
            if ent is None:
                bad.append(f"{field} has no package for bank {b}")
        print(line)

    # The address nets carry a FAN-OUT SUFFIX -- RA.01a, RA.01b, ... are copies
    # of one bit driven to different parts of the array, not different bits.
    # Strip it before comparing, or 144 packages look like twelve address buses.
    def stem(nets):
        return tuple(n[:-1] if n and n[-1].isalpha() and n[-2].isdigit() else n
                     for n in nets)
    addr = {stem(r[4]) for r in rows}
    if len(addr) != 1:
        bad.append(f"IM packages do not share one address bus: {len(addr)} variants")
    else:
        print(f"\naddress (A0..A9, MSB first): {', '.join(addr.pop())}")
        print(f"  ({len({r[4] for r in rows})} fan-out copies of it across the array)")

    # THE INTERLEAVE. The chip selects must be driven by exactly two address
    # bits, and they must be RA.00 and RA.11 -- the top and bottom of the twelve.
    board = sn.load_board(glob.glob('chm/sil/ContB*/*.wl')[0])
    csdrv = set()
    for cs in ("CS0'ACa", "CS1'ACa", "CS2'ACa", "CS3'ACa"):
        net = board.nets.get(cs)
        if not net:
            bad.append(f"{cs} not found")
            continue
        for p in net['pins']:
            if p['dir'] != 'out':
                continue
            for nm, nt in board.nets.items():
                if any(q['pkg'] == p['pkg'] and q['dir'] == 'in' and
                       nm.startswith('bdRA.') for q in nt['pins']):
                    csdrv.add(nm)
    want = {'bdRA.00a', 'bdRA.11a'}
    print(f"\nbank select driven by: {sorted(csdrv)}")
    if csdrv != want:
        bad.append(f"bank select is driven by {sorted(csdrv)}, not {sorted(want)} "
                   f"-- the interleave is not bank={{addr[11],addr[0]}}")
    else:
        print("  => bank = {addr[11], addr[0]},  index = addr[10:1]")

    if len(rows) != 144 or len(bits) != 36:
        bad.append(f"expected 144 packages and 36 field bits, got "
                   f"{len(rows)} and {len(bits)}")

    if bad:
        for x in bad:
            print(f"FAIL: {x}")
        return 1
    print("\nPASS: 36 field bits x 4 banks = 144 packages, one shared address bus")
    return 0


if __name__ == '__main__':
    sys.exit(main())
