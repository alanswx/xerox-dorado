#!/usr/bin/env python3
"""Inspect an Interlisp sysout: FPTOVP coverage and the words at a VA.

Written to diagnose `Raid: "Bad Array Block" {103,252}` when booting
Full.sysout!6.  Reuses the page/FPTOVP model that discard_stale_process.py
established, so both tools agree on how a sysout is laid out.

  inspect_sysout.py SYSOUT --va 103:252 [--words 32]
  inspect_sysout.py SYSOUT --summary
"""

from __future__ import annotations

import argparse
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
from discard_stale_process import (  # noqa: E402
    PAGE_BYTES,
    SysoutMap,
    parse_octal_pointer,
    word_at,
)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("sysout", type=Path)
    parser.add_argument("--va", type=parse_octal_pointer, metavar="HI:LO",
                        help="virtual address to examine, octal")
    parser.add_argument("--words", type=int, default=24,
                        help="words to dump around --va (default 24)")
    parser.add_argument("--before", type=int, default=8,
                        help="words to dump BEFORE --va (default 8)")
    parser.add_argument("--summary", action="store_true")
    args = parser.parse_args()

    data = bytearray(args.sysout.read_bytes())
    smap = SysoutMap(data)

    if args.summary or not args.va:
        pages = len(data) // PAGE_BYTES
        vps = sorted(smap.vp_to_raw_page)
        print(f"file            : {args.sysout}")
        print(f"bytes           : {len(data):,}  ({pages:,} pages of {PAGE_BYTES})")
        print(f"IFPNActivePages : {smap.n_active_pages:,}")
        print(f"IFPFPTOVPStart  : {smap.fptovp_start:o}o ({smap.fptovp_start})")
        print(f"mapped VPs      : {len(vps):,}")
        if vps:
            print(f"VP range        : {vps[0]:o}o .. {vps[-1]:o}o "
                  f"({vps[0]} .. {vps[-1]})")
            # contiguity: where are the holes?
            holes, run_start, prev = [], vps[0], vps[0]
            for vp in vps[1:]:
                if vp != prev + 1:
                    holes.append((prev, vp))
                prev = vp
            print(f"gaps in VP space: {len(holes)}")
            for a, b in holes[:12]:
                print(f"    {a:o}o -> {b:o}o   (missing {b - a - 1:,} pages)")
            if len(holes) > 12:
                print(f"    ... and {len(holes) - 12} more")

    if args.va:
        hi, lo = args.va
        va = (hi << 16) | lo
        vp, word = va >> 8, va & 0xFF
        print()
        print(f"VA {{{hi:o},{lo:o}}} = 0o{va:o} ({va})  ->  VP 0o{vp:o} ({vp}), "
              f"word 0o{word:o} ({word})")
        if vp not in smap.vp_to_raw_page:
            print(f"  *** VP 0o{vp:o} IS ABSENT from FPTOVP -- "
                  f"this address is not in the sysout at all. ***")
            near = sorted(smap.vp_to_raw_page)
            below = [v for v in near if v < vp]
            above = [v for v in near if v > vp]
            if below:
                print(f"  nearest mapped VP below: 0o{below[-1]:o} ({below[-1]})")
            if above:
                print(f"  nearest mapped VP above: 0o{above[0]:o} ({above[0]})")
            return 0
        raw = smap.vp_to_raw_page[vp]
        print(f"  present: raw sysout page {raw} (file byte 0x{raw*PAGE_BYTES:x})")
        base = raw * PAGE_BYTES
        start = max(0, word - args.before)
        end = min(256, word + args.words)
        print(f"  words 0o{start:o}..0o{end-1:o} of the page:")
        for w in range(start, end):
            v = word_at(data, base + 2 * w)
            mark = "  <== VA" if w == word else ""
            print(f"    [0o{w:03o}] 0o{v:06o}  0x{v:04x}  {v:6d}{mark}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
