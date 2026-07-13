#!/usr/bin/env python3
"""Verify Cedar loadee (BCD) integrity inside a Dorado emulator snapshot.

Usage:
    python3 tools/cedar_bcd_verify.py <snapshot.snap> <ftp-root>

For every file under <ftp-root>/Cedar6.1/*/*, split it into 256-word file
pages and search the snapshot's guest storage for each page at page-aligned
boundaries.  Reports, per file: pages found byte-exact, pages present but
mismatched (candidate located via an 8-word prefix index), and pages absent
from real memory (normally just swapped out, not an error).

This was written for the 2026-07-12 Cedar loadee bring-up to test whether
the STP -> FS -> disk-cache path corrupts BCD data.  Result then: all 34
Basic.Loadees files verified byte-exact (zero partial-mismatch pages), which
exonerated the transfer/cache path; see docs/handoff.md.

Snapshot layout: dorado_snap_header (see dorado/src/machine.c) followed by
mc, cpu, mem struct blobs, then guest storage as host-endian uint16 words.
"""
import struct, sys, os, glob
from collections import defaultdict


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    snap, root = sys.argv[1], sys.argv[2]

    f = open(snap, 'rb')
    hdr = f.read(16 + 4 + 4 + 9 * 8 + 8 + 4 * 4)
    if not hdr.startswith(b'DORADOSNAPSHOT'):
        sys.exit(f'not a dorado snapshot: {snap}')
    sizes = struct.unpack_from('<9Q', hdr, 24)
    storage_words = struct.unpack_from('<Q', hdr, 24 + 72)[0]
    off = len(hdr) + sizes[0] + sizes[1] + sizes[2]   # mc + cpu + mem
    f.seek(off)
    storage = f.read(storage_words * 2)
    print(f'storage: {storage_words} words at offset {off}')

    import array
    st = array.array('H')
    st.frombytes(storage)
    npages = storage_words // 256

    idx = defaultdict(list)
    for p in range(npages):
        base = p * 256
        idx[tuple(st[base:base + 8])].append(p)

    tot_bad = 0
    for path in sorted(glob.glob(os.path.join(root, 'Cedar6.1', '*', '*'))):
        if os.path.isdir(path):
            continue
        data = open(path, 'rb').read()
        if len(data) % 512:
            data += b'\0' * (512 - len(data) % 512)
        nfp = len(data) // 512
        found = part = missing = 0
        baddetail = []
        for fp in range(nfp):
            words = struct.unpack('>256H', data[fp * 512:(fp + 1) * 512])
            cands = idx.get(tuple(words[:8]), [])
            ok, best = False, None
            for p in cands:
                base = p * 256
                mism = sum(1 for i in range(256) if st[base + i] != words[i])
                if mism == 0:
                    ok = True
                    break
                if best is None or mism < best[0]:
                    best = (mism, p)
            if ok:
                found += 1
            elif cands:
                part += 1
                baddetail.append((fp, best))
            else:
                missing += 1
        name = os.path.relpath(path, root)
        flag = '' if part == 0 else '  <-- PARTIAL-MISMATCH PAGES'
        print(f'{name}: pages={nfp} exact={found} partial={part} '
              f'absent={missing}{flag}')
        for fp, best in baddetail[:5]:
            print(f'    filepage {fp}: best candidate storage page {best[1]} '
                  f'mismatch words={best[0]}')
        tot_bad += part
    print(f'\nTOTAL partial-mismatch pages: {tot_bad}')


if __name__ == '__main__':
    main()
