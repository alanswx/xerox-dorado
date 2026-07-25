#!/usr/bin/env python3
"""Rewrite a PDI's physical-root boot links from the flat convention to CHS.

Rusty Backup writes each `BootFile.DiskFileID.firstLink` in the physical
volume root as a flat PDI page number `(vda, 0)`.  Pilot -- and therefore
anything Othello or Iago installs -- writes a real Dorado DiskAddress
instead: `(cylinder, head||sector)`, where the Alto-environment-compatible
boot drive advances cylinder every 28 sectors, so `vda = cylinder*28 +
sector` (VolumeFormat.mesa PhysicalRoot; FileImpl.ComputeVMBackingLocation).

That is the same VDA either way -- page 104 is `(104,0)` flat and `(3,20)`
CHS -- but the two encodings are not interchangeable, and the corpus recipe
used to convert CHS *back* to flat by hand because the emulator's germ path
only understood flat.  It now detects the convention from the medium, so
this tool exists to produce authentically-encoded volumes for testing that
path (and as the inverse of the old stopgap).

    python3 tools/pdi_boot_links_to_chs.py image.pdi [--dry-run]

Idempotent: a link already in range as CHS is left alone.
"""
import sys

HDR = 512
PR_SEAL = 0o121212
PV_BOOTING_INFO = 8      # PhysicalRoot.bootingInfo (10B)
DISK_FILE_ID_WORDS = 9   # fID(5) + firstPage(2) + firstLink(2)
SLOTS = 4                # checkpoint, microcode, germ, bootFile
SECTORS = 28             # PilotDiskDefs.mc sectors per (virtual) cylinder
CHECKSUM_WORD = 255


def be16(raw, o):
    return (raw[o] << 8) | raw[o + 1]


def put16(raw, o, v):
    raw[o] = (v >> 8) & 0xFF
    raw[o + 1] = v & 0xFF


def pilot_checksum(words):
    """16-bit ones-complement add, left-rotate after each word, FFFF->0."""
    cs = 0
    for w in words:
        t = cs + w
        cs = (t + 1) & 0xFFFF if t > 0xFFFF else t
        cs = ((cs << 1) | 1) & 0xFFFF if cs & 0x8000 else (cs << 1) & 0xFFFF
    return 0 if cs == 0xFFFF else cs


def main():
    dry = '--dry-run' in sys.argv
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    if len(args) != 1:
        sys.exit(__doc__)
    path = args[0]
    raw = bytearray(open(path, 'rb').read())
    lb, db = be16(raw, 24), be16(raw, 26)
    sect = lb + db
    pv = HDR + lb                      # data area of page 0
    if be16(raw, pv) != PR_SEAL:
        sys.exit('page 0 is not a physical volume root')

    changed = 0
    for slot in range(SLOTS):
        base = pv + (PV_BOOTING_INFO + slot * DISK_FILE_ID_WORDS) * 2
        fid = [be16(raw, base + i * 2) for i in range(5)]
        lo, hi = be16(raw, base + 7 * 2), be16(raw, base + 8 * 2)
        if not any(fid) and not lo and not hi:
            continue
        if hi != 0:
            print(f'  slot {slot}: firstLink=({lo},{hi}) already CHS, left alone')
            continue
        vda = lo
        cyl, sec = divmod(vda, SECTORS)
        print(f'  slot {slot}: firstLink ({lo},{hi}) flat -> ({cyl},{sec}) CHS '
              f'[vda {vda}]')
        if not dry:
            put16(raw, base + 7 * 2, cyl)
            put16(raw, base + 8 * 2, sec)
        changed += 1

    if changed and not dry:
        words = [be16(raw, pv + i * 2) for i in range(CHECKSUM_WORD)]
        put16(raw, pv + CHECKSUM_WORD * 2, pilot_checksum(words))
        open(path, 'wb').write(raw)
    print(f'{changed} link(s) {"would be " if dry else ""}rewritten in {path}')


if __name__ == '__main__':
    main()
