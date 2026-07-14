#!/usr/bin/env python3
"""Stamp the volume ID into a PDI's free-page labels.

Cedar's allocator verifies every allocation candidate against
FileInternal.FreeLabel[volume], whose fileID is the *logical volume's
VolumeID* (FilePagesImpl.mesa: `fileID: [abs[AbsID[File.GetVolumeID[
volume]]]]`), filePage = the logical page, attributes = freePage.  Rusty
Backup's volume builder wrote free-page labels with fileID = 0, so every
VerifyLabels failed and File.Error[volumeFull] was raised with tens of
thousands of pages free ("FS.Error: No more free pages on a local volume").

    python3 tools/pdi_stamp_free_labels.py image.pdi [--dry-run]

Reads the logical volume roots via the physical root's subvolume table and
rewrites every freePage (attr 0o23000) label's fileID words to the owning
volume's vID.  Idempotent.
"""
import sys

FREE_PAGE_ATTR = 0o23000
HDR = 512


def be16(raw, o):
    return (raw[o] << 8) | raw[o + 1]


def put16(raw, o, v):
    raw[o] = (v >> 8) & 0xFF
    raw[o + 1] = v & 0xFF


def main():
    dry = '--dry-run' in sys.argv
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    if len(args) != 1:
        sys.exit(__doc__)
    path = args[0]
    raw = bytearray(open(path, 'rb').read())
    lb, db = be16(raw, 24), be16(raw, 26)
    sect = lb + db
    media = (len(raw) - HDR) // sect

    def label(p):
        return HDR + p * sect

    def data(p):
        return HDR + p * sect + lb

    # Physical root (page 0): subvolume table at word 75, 6 x SubVolumeDesc
    # (13 words: lvID 5, lvSize 2, lvPage 2, pvPage 2, nPages 2).
    pv = data(0)
    if be16(raw, pv) != 0o121212:
        sys.exit('page 0 is not a physical volume root')
    sub_count = be16(raw, pv + 64 * 2)
    subs = []
    for i in range(sub_count):
        base = pv + (75 + i * 13) * 2
        lv_page = be16(raw, base + 7 * 2) | (be16(raw, base + 8 * 2) << 16)
        pv_page = be16(raw, base + 9 * 2) | (be16(raw, base + 10 * 2) << 16)
        n_pages = be16(raw, base + 11 * 2) | (be16(raw, base + 12 * 2) << 16)
        # The logical root page carries the authoritative vID (LV root word 2).
        lr = data(pv_page + lv_page)
        if be16(raw, lr) != 0o131313:
            print(f'  subvolume {i}: no LV root at pv {pv_page + lv_page}, skipped')
            continue
        vid = [be16(raw, lr + (2 + w) * 2) for w in range(5)]
        subs.append((pv_page, n_pages, vid))
        print(f'  subvolume {i}: pvPage={pv_page} nPages={n_pages} vID={vid}')

    stamped = have = 0
    for pv_page, n_pages, vid in subs:
        for p in range(pv_page, pv_page + n_pages):
            if p >= media:
                break
            off = label(p)
            if be16(raw, off + 7 * 2) != FREE_PAGE_ATTR:
                continue
            cur = [be16(raw, off + w * 2) for w in range(5)]
            if cur == vid:
                have += 1
                continue
            for w in range(5):
                put16(raw, off + w * 2, vid[w])
            stamped += 1

    print(f'{stamped} free-page labels stamped, {have} already correct')
    if not dry and stamped:
        open(path, 'wb').write(raw)
        print(f'wrote {path}')


if __name__ == '__main__':
    main()
