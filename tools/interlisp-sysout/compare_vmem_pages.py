#!/usr/bin/env python3
"""How many of a sysout's pages appear verbatim in a booted pack?

*** READ THIS BEFORE DRAWING A CONCLUSION ***

This was written to ask whether the page behind Full.sysout!6's
`Raid: "Bad Array Block" {103,252}` reaches LISP.VIRTUALMEM intact, and it
CANNOT answer that.  Measured on 2026-08-06: of 72 random non-sparse pages
from a sysout that had just been loaded, only **6 (8%) appear verbatim** in
the resulting pack.

The reason is obvious once seen: the guest does not merely copy the sysout
to disk and stop.  It loads, then RUNS, and a running Interlisp dirties
pages continuously, so the on-disk VMEM reflects live state rather than the
archived image.  A page being absent therefore means nothing -- absence is
the common case even for a perfectly healthy load.

So: an absent page is NOT evidence of a disk-path bug, and this tool must
not be used to claim one.  It is kept because the 8% baseline is worth
knowing, and because a large SWING in that baseline between two sysouts
would be meaningful even though an individual miss is not.

Answering the original question properly needs LispFmap decoded, so the
VMEM file page holding a given virtual page can be located and compared
directly, rather than searched for by content.

  compare_vmem_pages.py SYSOUT PACK [PACK2 ...]

Pages that are mostly zeros are ambiguous (many disk blocks are all-zero)
and are counted separately rather than claimed as hits.
"""

from __future__ import annotations

import argparse
from pathlib import Path
import sys

PAGE = 512


def blocks_of(data: bytes, stride: int = 2) -> set[bytes]:
    """Every PAGE-sized window at `stride` alignment.

    The Trident pack interleaves 512-byte data fields with headers/labels at
    a 2074-byte sector pitch, and the data field is not 512-aligned in the
    file, so a coarse alignment would miss real matches.  stride=2 keeps the
    scan word-aligned (all Alto data is) and finishes in seconds.
    """
    out: set[bytes] = set()
    for off in range(0, len(data) - PAGE + 1, stride):
        out.add(data[off : off + PAGE])
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("sysout", type=Path)
    ap.add_argument("packs", type=Path, nargs="+")
    ap.add_argument("--zero-threshold", type=float, default=0.90,
                    help="pages with at least this fraction of zero bytes are "
                         "reported separately as ambiguous (default 0.90)")
    args = ap.parse_args()

    sysout = args.sysout.read_bytes()
    n_pages = len(sysout) // PAGE

    present: set[bytes] = set()
    for p in args.packs:
        data = p.read_bytes()
        print(f"scanning {p} ({len(data):,} bytes)...", file=sys.stderr)
        present |= blocks_of(data)
    print(f"{len(present):,} distinct 512-byte blocks on the media",
          file=sys.stderr)

    hit = miss = ambiguous = 0
    missing_pages: list[int] = []
    for i in range(n_pages):
        page = sysout[i * PAGE : (i + 1) * PAGE]
        if page.count(0) / PAGE >= args.zero_threshold:
            ambiguous += 1
            continue
        if page in present:
            hit += 1
        else:
            miss += 1
            if len(missing_pages) < 40:
                missing_pages.append(i)

    checked = hit + miss
    print(f"sysout pages           : {n_pages:,}")
    print(f"  ambiguous (mostly 0) : {ambiguous:,}  (not counted either way)")
    print(f"  checked              : {checked:,}")
    print(f"  FOUND on the media   : {hit:,}"
          + (f"  ({hit * 100 / checked:.2f}%)" if checked else ""))
    print(f"  MISSING              : {miss:,}")
    if missing_pages:
        print("  first missing raw pages:", ", ".join(map(str, missing_pages)))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
