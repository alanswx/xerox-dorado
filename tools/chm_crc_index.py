#!/usr/bin/env python3
"""Turn the CHM PARC cross-reference into a (crc32, size, name) index.

The Computer History Museum's Xerox PARC archive ships a giant
`cross-reference.html` (the full IFS filename -> location listing). Every entry
carries the file's exact byte size and a 32-bit checksum which is plain
zlib/IEEE CRC-32 of the content (verified: Clock.bcd!1 -> 5f13dc2e). That is
exactly the fingerprint needed to recover a corpus file's name from its bytes,
WITHOUT downloading anything -- the archive is the source the corpus was
packed from.

Emits `crc32<TAB>size<TAB>name` (one line per distinct filename per key), which
`cedar_repack --crc-names <file>` consumes. PARC-specific HTML parsing lives
here so rusty-backup's matcher stays a generic filesystem tool.

Usage:
    chm_crc_index.py chm/cross-reference.html > corpus-names.tsv
"""

import re
import sys

# An entry looks like (after the anchor tag that ends the directory link):
#   ...>Clock.bcd!1 30394 5f13dc2e 20-May-1985 21:19:33 PDT Binary Wyatt.pa ...
# name!version, size (decimal), crc32 (8 hex), then date/type/author.
ENTRY = re.compile(r">([^<>]+?)!(\d+)\s+(\d+)\s+([0-9a-fA-F]{8})\s")


def main() -> None:
    path = sys.argv[1] if len(sys.argv) > 1 else "chm/cross-reference.html"
    seen: set[tuple[str, int, str]] = set()
    n = 0
    with open(path, errors="replace") as f:
        for line in f:
            m = ENTRY.search(line)
            if not m:
                continue
            name = m.group(1).rsplit(">", 1)[-1].rsplit("<", 1)[-1]
            size = int(m.group(3))
            crc = m.group(4).lower()
            key = (crc, size, name)
            if key in seen:
                continue
            seen.add(key)
            sys.stdout.write(f"{crc}\t{size}\t{name}\n")
            n += 1
    print(f"wrote {n} (crc,size,name) rows", file=sys.stderr)


if __name__ == "__main__":
    main()
