#!/usr/bin/env python3
"""Show a Cedar/Pilot volume's ROOT FILES, and diff two volumes' root files.

Pilot registers a volume's special files in the logical volume root:

    LogicalRoot.bootingInfo (37B): ARRAY VolumeFile[checkpoint..debuggee]
    LogicalRoot.rootFile    (125B): ARRAY VolumeFile[0..16)

`FileBackdoor.GetRoot[volume, checkpoint]` reads that table, so an empty
`checkpoint` slot is how you tell a volume has never been checkpointed.
Note bootingInfo covers ONLY checkpoint..debuggee (six entries) -- indexing
it past `debuggee` reads the following structure and invents changes.

  tools/cedar_root_files.py VOL.pdi [--compare OTHER.pdi]

Written for the checkpoint/rollback investigation (docs/cedar-checkpoint.md):
after `Checkpoint` runs, slot 0 should go from all-zero to a real file.
"""

from __future__ import annotations

import sys

HDR = 512
PR_SEAL = 0o121212
LR_SEAL = 0o131313
LV_BOOTING_INFO = 31      # LogicalRoot.bootingInfo (37B)
LV_ROOT_FILE = 85         # LogicalRoot.rootFile   (125B)
DISK_FILE_ID_WORDS = 9    # fID(5) + firstPage(2) + firstLink(2)
ROOT_FILE_WORDS = 6       # fp(4) + page(2)
BOOTING_INFO_SLOTS = 6    # checkpoint..debuggee, and no further

NAMES = {0: 'checkpoint', 1: 'microcode', 2: 'germ', 3: 'bootFile',
         4: 'debugger', 5: 'debuggee', 6: 'VM', 7: 'VAM', 8: 'client'}


def be16(raw, o):
    return (raw[o] << 8) | raw[o + 1]


def read_roots(path):
    raw = open(path, 'rb').read()
    lb, db = be16(raw, 24), be16(raw, 26)
    sect = lb + db

    def data(p):
        return HDR + p * sect + lb

    pv = data(0)
    if be16(raw, pv) != PR_SEAL:
        raise SystemExit(f'{path}: page 0 is not a physical volume root')
    out = []
    for i in range(be16(raw, pv + 64 * 2)):
        base = pv + (75 + i * 13) * 2
        lv_page = be16(raw, base + 7 * 2) | (be16(raw, base + 8 * 2) << 16)
        pv_page = be16(raw, base + 9 * 2) | (be16(raw, base + 10 * 2) << 16)
        lr = data(pv_page + lv_page)
        if be16(raw, lr) != LR_SEAL:
            continue
        name = bytes(raw[lr + 16:lr + 56]).split(b'\0')[0].decode('ascii', 'replace')
        slots = {}
        for o in range(len(NAMES)):
            bi = ([be16(raw, lr + (LV_BOOTING_INFO + o * DISK_FILE_ID_WORDS) * 2 + 2 * k)
                   for k in range(5)] if o < BOOTING_INFO_SLOTS else None)
            rf = [be16(raw, lr + (LV_ROOT_FILE + o * ROOT_FILE_WORDS) * 2 + 2 * k)
                  for k in range(ROOT_FILE_WORDS)]
            slots[o] = (bi, rf)
        out.append((name, pv_page + lv_page, slots))
    return out


def fmt(v):
    return '-' if v is None else '[' + ' '.join(f'{x:o}' for x in v) + ']'


def main(argv):
    if len(argv) < 2:
        raise SystemExit(__doc__)
    path = argv[1]
    other = None
    if '--compare' in argv:
        other = argv[argv.index('--compare') + 1]

    vols = read_roots(path)
    prev = {v[0]: v[2] for v in read_roots(other)} if other else None

    for name, page, slots in vols:
        print(f'  volume "{name}" (logical root at page {page})')
        for o in sorted(slots):
            bi, rf = slots[o]
            empty = not any(bi or []) and not any(rf)
            mark = ''
            if prev is not None and name in prev and prev[name][o] != slots[o]:
                pbi, prf = prev[name][o]
                mark = f'   CHANGED (was bootingInfo={fmt(pbi)} rootFile={fmt(prf)})'
            state = 'empty' if empty else 'present'
            print(f'    [{o}] {NAMES[o]:11s} {state:8s} '
                  f'bootingInfo={fmt(bi)} rootFile={fmt(rf)}{mark}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
