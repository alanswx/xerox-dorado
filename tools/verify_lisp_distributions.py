#!/usr/bin/env python3
"""Validate rebuilt Interlisp-D sysouts against the ORIGINALS in the CHM archive.

The floppy pipeline (build_lisp_distributions.py) is only as good as its
output, and the archive keeps per-release sysouts with size and CRC32 in
`chm/cross-reference.html` -- so every rebuild can be diffed against the
real thing instead of being trusted.

  tools/verify_lisp_distributions.py [DISTDIR]     # default dorado/build/lisp-dist

Exit status is nonzero if any rebuild matches an original in SIZE but not
in content, which is the signature of a decode defect (a wrong-length
rebuild is a different, louder failure the builder already reports).

Measured 2026-08-12:
  harmony, koto, lyric   EXACT  -- these come from .IMD media
  carol, fugue           SIZE-MATCH ONLY, 112 bytes each -- .dmk media
Every differing byte is ours=0x00 where the original is nonzero, and every
one lands on an EVEN file offset (the high byte of a big-endian word), at
a `<len><name>` boundary in the atom table.  See docs/lisp-distributions.md.
"""

from __future__ import annotations

import os
import sys
import zlib

# (size, crc32, date, archive path) harvested from chm/cross-reference.html.
# Add rows here as more originals are identified -- `grep -i sysout` it.
ORIGINALS = [
    (2688000, 0x7EBC62EF, "10-Sep-1984", "eris/lisp/carol/basics/LISP.SYSOUT!1"),
    (2688000, 0xCA16135E, "20-Jun-1984", "eris/lisp/current/LISP.SYSOUT!2"),
    (1849344, 0xC94CF377, "28-Feb-1983", "eris/lisp/chorus/basics/LISP.SYSOUT!1"),
    (2469376, 0xB5E7AB59, "18-Aug-1983", "eris/lisp/fugue.4/basics/LISP.SYSOUT!1"),
    (2659328, 0xC58A37C8, "13-Mar-1984", "eris/lisp/fugue.5/current/LISP.SYSOUT!2"),
    (2647552, 0xC132A0C0, "09-Apr-1984", "eris/lisp/fugue.6/basics/Lisp.sysout!1"),
    (2774016, 0xB0015744, "21-Sep-1984", "eris/lisp/harmony/basics/LISP.SYSOUT!2"),
    (2786816, 0x6DAB178E, "21-Dec-1984", "eris/lisp/harmony/basics/LISP.SYSOUT!15"),
    (3081216, 0x9AA84955, "19-Mar-1985", "eris/lisp/intermezzo/basics/LISP.SYSOUT!11"),
    (3251200, 0x3B59BA69, "07-Feb-1986", "eris/lisp/koto/basics/LISP.SYSOUT!15"),
    (4824064, 0xF2D83907, "27-Apr-1987", "phylum/lisp/lyric/basics/LISP.SYSOUT!1"),
    (5000704, 0x7BCDB920, "09-Nov-1988", "phylum/medley/1.0/LISP.SYSOUT!1"),
]

# Rebuilds that differ from an original for a REASON, with the measured
# evidence.  A tool that cries wolf on a known-benign case teaches people to
# ignore it, so these are classified rather than flagged.
KNOWN_DIFFERENT = {
    # our crc -> (original, why)
    0x33EC336E: ("phylum/medley/1.0/LISP.SYSOUT!1",
                 "different SAVE of the same release: 38,590 bytes differ, "
                 "TWO-directional (2,763 ours-zero vs 2,761 original-zero), "
                 "even/odd offsets balanced, confined to pages 36-240"),
}

# Rebuilds with a measured DECODE DEFECT -- same size, and the differences
# are one-directional zeros, which no honest 'different save' produces.
KNOWN_DEFECT = {
    0x14C93972: ("eris/lisp/current/LISP.SYSOUT!2",
                 "112 bytes, ALL ours=0x00 where the original is nonzero, "
                 "ALL at even file offsets, at <len><name> boundaries in the "
                 "atom table; source media are .dmk"),
    0x15D5120D: ("eris/lisp/fugue.6/basics/Lisp.sysout!1",
                 "112 bytes, identical signature to carol; source media .dmk"),
}

BY_CRC = {(s, c): (d, p) for s, c, d, p in ORIGINALS}
SIZES: dict[int, list[tuple[str, str]]] = {}
for _s, _c, _d, _p in ORIGINALS:
    SIZES.setdefault(_s, []).append((_d, _p))


def main(argv: list[str]) -> int:
    root = argv[1] if len(argv) > 1 else "dorado/build/lisp-dist"
    if not os.path.isdir(root):
        print(f"no distribution tree at {root}", file=sys.stderr)
        return 2

    exact = suspect = unknown = 0
    print(f"{'rebuild':46s} {'bytes':>9s} {'crc32':>9s}  verdict")
    for rel in sorted(os.listdir(root)):
        d = os.path.join(root, rel)
        if not os.path.isdir(d):
            continue
        for name in sorted(os.listdir(d)):
            if "sysout" not in name.lower():
                continue
            path = os.path.join(d, name)
            if not os.path.isfile(path):
                continue
            blob = open(path, "rb").read()
            crc = zlib.crc32(blob) & 0xFFFFFFFF
            hit = BY_CRC.get((len(blob), crc))
            label = f"{rel}/{name}"
            if hit:
                exact += 1
                print(f"{label:46s} {len(blob):9d} {crc:08x}  EXACT = {hit[1]}")
            elif crc in KNOWN_DEFECT:
                suspect += 1
                orig, why = KNOWN_DEFECT[crc]
                print(f"{label:46s} {len(blob):9d} {crc:08x}  "
                      f"*** DECODE DEFECT vs {orig}")
                print(f"{'':46s} {'':9s} {'':8s}      {why}")
            elif crc in KNOWN_DIFFERENT:
                orig, why = KNOWN_DIFFERENT[crc]
                print(f"{label:46s} {len(blob):9d} {crc:08x}  "
                      f"different save, not a defect ({orig})")
                print(f"{'':46s} {'':9s} {'':8s}      {why}")
            elif len(blob) in SIZES:
                suspect += 1
                where = ", ".join(p for _, p in SIZES[len(blob)])
                print(f"{label:46s} {len(blob):9d} {crc:08x}  "
                      f"*** SIZE MATCHES BUT CONTENT DIFFERS (unclassified): {where} ***")
            else:
                unknown += 1
                print(f"{label:46s} {len(blob):9d} {crc:08x}  "
                      f"no original on record")

    print(f"\n{exact} exact, {suspect} size-match-only, {unknown} unverifiable")
    if suspect:
        print("A size match with differing content means the pipeline decoded "
              "that medium wrongly. Diff it against the original to see the "
              "shape of the defect.")
    return 1 if suspect else 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
