#!/usr/bin/env python3
"""Recompute the physical- and logical-volume root page checksums in a PDI.

Cedar zeroes a volume root page's checksum word while the volume is mounted
and rewrites it on a clean dismount.  The emulator never dismounts cleanly --
a run ends by the process exiting, and `DORADO_PDI_SAVE` writes the medium
back exactly as it stood -- so a volume that Cedar WROTE during the session
(one Iago built, for instance: SetRoot for the boot file, germ and microcode,
then Create VM Backing File) is left with `checksum = 0`.  On the next boot
FileImpl reads that root, sees the checksum disagree, and reports
`File.Error[inconsistent]`, which FS surfaces as

    FS.Error: Local volume's permanent data structures are inconsistent

and the boot stops before it can look for `Basic.Loadees`.

The page contents are correct -- only the checksum was never written back.
Recomputing it offline is exactly what the clean dismount would have done.

    python3 tools/pdi_fix_root_checksums.py image.pdi [--dry-run]

VolumeFormat.mesa: PhysicalRoot seal 121212B, LogicalRoot seal 131313B, and
in both the checksum is word 377B (255), computed over words 0..254 with
Checksum.ComputeChecksum.  Idempotent.
"""
import sys

HDR = 512
PR_SEAL = 0o121212
LR_SEAL = 0o131313
CHECKSUM_WORD = 255


def be16(raw, o):
    return (raw[o] << 8) | raw[o + 1]


def put16(raw, o, v):
    raw[o] = (v >> 8) & 0xFF
    raw[o + 1] = v & 0xFF


def pilot_checksum(words):
    """16-bit ones-complement add, left rotate after each word, FFFF -> 0."""
    cs = 0
    for w in words:
        t = cs + w
        cs = (t + 1) & 0xFFFF if t > 0xFFFF else t & 0xFFFF
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
    pages = be16(raw, 16) * max(1, be16(raw, 18)) * max(1, be16(raw, 20))

    fixed = 0
    for p in range(pages):
        o = HDR + p * sect + lb
        seal = be16(raw, o)
        if seal not in (PR_SEAL, LR_SEAL):
            continue
        kind = 'PhysicalRoot' if seal == PR_SEAL else 'LogicalRoot '
        stored = be16(raw, o + CHECKSUM_WORD * 2)
        want = pilot_checksum([be16(raw, o + i * 2) for i in range(CHECKSUM_WORD)])
        if stored == want:
            print(f'  page {p:6d}  {kind} checksum {stored:o} already correct')
            continue
        print(f'  page {p:6d}  {kind} checksum {stored:o} -> {want:o}')
        if not dry:
            put16(raw, o + CHECKSUM_WORD * 2, want)
        fixed += 1

    if fixed and not dry:
        open(path, 'wb').write(raw)
    print(f'{fixed} root page(s) {"would be " if dry else ""}corrected in {path}')


if __name__ == '__main__':
    main()
