#!/usr/bin/env python3
"""Create a blank PDI container -- an unformatted disk for Iago to work on.

Iago's "Format Disk" / "Create Physical Volume" / "Create Logical Volume"
sequence is how a Cedar system volume is built from scratch, and it needs a
target that is genuinely empty: every label and data word zero, no volume
root, no boot links.  That is what this writes.

The container header follows the PDI layout the emulator's loader reads
(dorado/include/pdi.h):

    byte 0   "PARCDISK"
    word 4   version   = 1
    word 5   fsFamily  = 2 (Pilot)
    word 8   cylinders        page count is the PRODUCT of words 8..10,
    word 9   heads            not word 8 alone
    word 10  sectors
    word 12  labelBytes = 20
    word 13  dataBytes  = 512
    byte 512 first sector; each is label+data, contiguous

Geometry defaults to the Trident T-80 the Dorado's Pilot disk head assumes
(PILOT_DISK_CYLINDERS/HEADS/SECTORS in machine.c: 815 x 5 x 28 = 114,100
pages), so every address Pilot can form on a non-boot drive lands inside
the file.

    python3 tools/pdi_create_blank.py out.pdi [--cylinders N] [--heads N]
                                              [--sectors N]
"""
import argparse
import os

HDR_BYTES = 512
LABEL_BYTES = 20
DATA_BYTES = 512


def put16(raw, word_index, value):
    raw[word_index * 2] = (value >> 8) & 0xFF
    raw[word_index * 2 + 1] = value & 0xFF


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('path')
    ap.add_argument('--cylinders', type=int, default=815)
    ap.add_argument('--heads', type=int, default=5)
    ap.add_argument('--sectors', type=int, default=28)
    args = ap.parse_args()

    for name, v in (('cylinders', args.cylinders), ('heads', args.heads),
                    ('sectors', args.sectors)):
        if not 1 <= v <= 0xFFFF:
            ap.error(f'{name} must be 1..65535 (each is one header word)')

    pages = args.cylinders * args.heads * args.sectors
    header = bytearray(HDR_BYTES)
    header[0:8] = b'PARCDISK'
    put16(header, 4, 1)                 # version
    put16(header, 5, 2)                 # fsFamily = Pilot
    put16(header, 8, args.cylinders)
    put16(header, 9, args.heads)
    put16(header, 10, args.sectors)
    put16(header, 12, LABEL_BYTES)
    put16(header, 13, DATA_BYTES)

    sector_bytes = LABEL_BYTES + DATA_BYTES
    total = HDR_BYTES + pages * sector_bytes
    with open(args.path, 'wb') as f:
        f.write(header)
        # Sparse where the filesystem supports it: truncate past the header
        # rather than writing 60 MB of zeros.
        f.truncate(total)
    print(f'{args.path}: {pages} pages '
          f'({args.cylinders}x{args.heads}x{args.sectors}), '
          f'{total} bytes, all labels and data zero')
    if os.path.getsize(args.path) != total:
        raise SystemExit('short write')


if __name__ == '__main__':
    main()
