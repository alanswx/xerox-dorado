#!/usr/bin/env python3
"""Enumerate and check DskEth's TRIDENT DRIVE INTERFACE.

This is the list a drive model has to satisfy. It is derived from the wire
list rather than from a cable pinout document, because the board states its own
interface twice -- in the packages that translate between ECL and the cable,
and in the backplane pins those nets leave on.

The translators ARE the interface:

    MC1650    differential line receiver   the per-drive read Clock and Data
    MC10124   quad TTL -> ECL              the nine drive STATUS lines in
    MC10125   quad ECL -> TTL              the thirteen TAG lines out
    SN74LS08 / SN74LS153                   SecIndx' and Selected', per drive

Per drive (four of them) the Dorado sees a differential Clock pair, a
differential Data pair (BIDIRECTIONAL -- the board writes as well as reads,
which is why the Data nets have on-board drivers and the Clock nets do not),
plus SecIndx' and Selected'. The nine Ttl*' status lines and the TtlTag.* bus
are shared across drives, selected by the tag protocol.

Gating this matters because a generator change can silently drop a cable
signal: the board port lists were once inferred from Term100 terminator
packages and missed 703 backplane nets while inventing 833.
"""
import glob
import sys

sys.path.insert(0, __file__.rsplit('/', 1)[0])
import sil_netlist as sn

DRIVES = range(4)
STATUS_IN = ["TtlDeviceCk'", "TtlEndOfCyl'", "TtlIndex'", "TtlOffSet'",
             "TtlOnLine'", "TtlReadOnly'", "TtlReady'", "TtlSeekInc'",
             "TtlTerm'"]
TAG_OUT = ([f'TtlTag.{i}' for i in range(10)]
           + ['TtlTag.00', 'TtlTag.000', 'TtlRunOK'])


def main() -> int:
    board = sn.load_board(glob.glob('chm/sil/DskEth*/*.wl')[0])
    types = {pk: rec.get('type', '?') for pk, rec in board.packages.items()}

    def bp(name):
        net = board.nets.get(name)
        if net is None:
            return None
        return ','.join(sorted(str(x).strip("{}'") for x in (net.get('backplane') or ''))) \
            .replace(",,", ",").strip(",") or '-'

    def driven(name):
        net = board.nets.get(name)
        if net is None:
            return None
        return any(p['dir'] == 'out'
                   and types.get(p['pkg']) not in ('SIPpackage', 'Term100')
                   for p in net['pins'])

    missing = []
    print("=== PER DRIVE (x4) ===")
    for d in DRIVES:
        row = []
        for stem, kind in ((f'ClockP{d}', 'in'), (f'ClockM{d}', 'in'),
                           (f'DataP{d}', 'bidir'), (f'DataM{d}', 'bidir'),
                           (f"SecIndx{d}'", 'in'), (f"Selected{d}'", 'in')):
            if stem not in board.nets:
                missing.append(f"drive {d}: {stem} is not on the board")
                row.append(f'{stem}=MISSING')
            else:
                row.append(f'{stem}({"drv" if driven(stem) else "in"})')
        print(f"  drive {d}: " + '  '.join(row))

    print("\n=== SHARED STATUS IN (MC10124, TTL -> ECL) ===")
    for n in STATUS_IN:
        if n not in board.nets:
            missing.append(f"status line {n} is not on the board")
            print(f"  {n:<18} MISSING")
        else:
            print(f"  {n:<18} backplane {bp(n)}")

    print("\n=== SHARED TAG OUT (MC10125, ECL -> TTL) ===")
    for n in TAG_OUT:
        if n not in board.nets:
            missing.append(f"tag line {n} is not on the board")
            print(f"  {n:<18} MISSING")
    print(f"  all {len(TAG_OUT)} present" if not missing else "")

    # The Clock pairs must be pure inputs and the Data pairs must be driven:
    # read clock comes from the drive, data goes both ways.
    for d in DRIVES:
        for stem in (f'ClockP{d}', f'ClockM{d}'):
            if driven(stem):
                missing.append(f"{stem} has an on-board driver; the read clock "
                               f"comes FROM the drive")
        for stem in (f'DataP{d}', f'DataM{d}'):
            if driven(stem) is False:
                missing.append(f"{stem} has no on-board driver; the data pair is "
                               f"BIDIRECTIONAL -- the board writes too")

    if missing:
        for m in missing:
            print(f"FAIL: {m}")
        return 1
    n = 4 * 6 + len(STATUS_IN) + len(TAG_OUT)
    print(f"\nPASS: the Trident drive interface is {n} signals -- 4 drives x "
          f"(Clock pair in, Data pair bidirectional, SecIndx', Selected'), "
          f"{len(STATUS_IN)} shared status in, {len(TAG_OUT)} shared tag out")
    return 0


if __name__ == '__main__':
    sys.exit(main())
