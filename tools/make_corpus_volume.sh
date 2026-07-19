#!/bin/sh
# make_corpus_volume.sh [OUT.pdi] [MAX_FILES]
#
# Build a BOOTABLE Cedar volume carrying the recovered PARC file corpus.
#
# Why each step exists (all four were found the hard way on 2026-07-19):
#
#   * Repack from CedarDorado-bestof.pdi, NOT CedarDorado-kitchensink.pdi.
#     The kitchensink images give every file a ONE-page header; Cedar 6.1
#     wants two, so it looks for VAM data page 0, does not find it, and dies
#     before painting anything (0 px). bestof is the same corpus already
#     repacked into the correct layout.
#
#   * Cap the file count. bestof is 99.99% full (4 free pages of 65,450), and
#     Cedar cannot complete even a directory lookup without scratch space --
#     it fails with "FS.Error: No more free pages on a local volume" during
#     login. The cap trades corpus files for the room Cedar needs to run.
#
#   * Rebuild cedar_repack first. The committed binary predates the
#     max-files argument and silently ignores it.
#
#   * Rewrite the PV-root boot links to the FLAT convention. The writer emits
#     a proper CHS DiskAddress (germ firstLink = (3,20)); the emulator's
#     polled germ path wants the flat page number ((104,0)) -- the same VDA
#     104, a different encoding. Skip this and the volume renders 0 px.
#
# Known limitation: only ~25 of 1200 files keep their names, and listing
# eventually stops on FSReportImpl $badFP, because cedar_repack copies file
# content without rewriting the name-directory's File.FPs to the new
# placement. Fixing that is the next job; see docs/handoff.md.

set -e
cd "$(dirname "$0")/.."

OUT="${1:-CedarDisk/CedarDorado-corpus.pdi}"
MAX_FILES="${2:-1200}"
GERM=chm/cedar/germ-alt/Dorado.germ-6.1.6
BOOT='chm/cedar/cedar6.1/BasicCedarDorado.boot!22'
SRC=CedarDisk/CedarDorado-bestof.pdi
REPACK=tools/rusty-backup/target/release/examples/cedar_repack

for f in "$GERM" "$BOOT" "$SRC"; do
    [ -r "$f" ] || { echo "missing input: $f" >&2; exit 1; }
done

echo "1/3 building cedar_repack (the committed binary predates --max-files)"
( cd tools/rusty-backup && cargo build --release --example cedar_repack )

echo "2/3 repacking $SRC -> $OUT (cap $MAX_FILES files, leaving free space)"
"$REPACK" "$SRC" "$GERM" "$BOOT" "$OUT" "$MAX_FILES"

echo "3/3 rewriting PV-root boot links to the flat convention"
python3 - "$OUT" <<'PY'
import struct, sys
path = sys.argv[1]
b = bytearray(open(path, 'rb').read())
o = 512 + 20                      # page 0 data (after its 20-byte label)
BASE, W, SECTORS = 0o10, 9, 28    # bootingInfo at 10B; 9 words per DiskFileID
rd = lambda i: struct.unpack_from('>H', b, o + i * 2)[0]
wr = lambda i, v: struct.pack_into('>H', b, o + i * 2, v)
for name, slot in (('germ', 2), ('bootFile', 3)):
    w = BASE + slot * W
    lo, hi = rd(w + 7), rd(w + 8)
    vda = lo * SECTORS + (hi & 0xFF)
    wr(w + 7, vda)
    wr(w + 8, 0)
    print('    %-9s firstLink (%d,%d) -> flat (%d,0)' % (name, lo, hi, vda))

def checksum(words):              # Pilot page checksum (Checksum.ComputeChecksum)
    cs = 0
    for x in words:
        t = cs + x
        cs = (t + 1) & 0xFFFF if t > 0xFFFF else t & 0xFFFF
        cs = ((cs << 1) | 1) & 0xFFFF if cs & 0x8000 else (cs << 1) & 0xFFFF
    return 0 if cs == 0xFFFF else cs
wr(255, checksum([rd(i) for i in range(255)]))
open(path, 'wb').write(b)
print('    page-0 checksum recomputed')
PY

echo "done: $OUT"
echo "  boot it:  make run-cedar-corpus        (SDL)"
echo "  or:       ./build/dorado --boot-reason disk --no-alto-boot \\"
echo "              --eb ../chm/dorado/CedarDorado.eb!6 \\"
echo "              --germ ../$GERM --pilot-disk ../$OUT \\"
echo "              --ftp-root ../chm/cedar/stp-root"
