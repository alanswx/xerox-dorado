#!/usr/bin/env python3
"""Materialize Cedar 6.1 Basic.Loadees dependencies into the local STP root."""
from html import unescape
from pathlib import Path
import re
import sys
from urllib.request import urlopen

ROOT = Path(__file__).resolve().parents[1]
LOADEES = ROOT / "chm/cedar/stp-root/Cedar6.1/Top/Basic.Loadees"
INDEX = ROOT / "chm/cross-reference.html"
OUT = ROOT / "chm/cedar/stp-root"
BASE = "https://xeroxparcarchive.computerhistory.org/"

def main():
    content = LOADEES.read_bytes().split(b"Local switches", 1)[0].decode("latin1")
    wanted = re.findall(r"\[Cedar\]<Cedar6\.1>([^>]+)>([^\r\n]+)", content)
    index = INDEX.read_text(errors="replace").splitlines()
    count = 0
    for directory, filename in wanted:
        # LoaderDriver suffixes (for example End.bcd/e) are command syntax,
        # not part of the IFS filename.
        filename = filename.split("/", 1)[0]
        needle = f"cyan/cedar6.1/{directory.lower()}/.index.html"
        candidates = [line for line in index if needle in line and filename.lower() in line.lower()]
        if not candidates:
            raise SystemExit(f"not found in archive index: {directory}/{filename}")
        line = unescape(candidates[0])
        match = re.search(r'href="([^"]+)/\.index\.html"[^>]*>.*?' + re.escape(filename) + r'(!\d+)', line, re.I)
        if not match:
            raise SystemExit(f"unparseable archive entry: {line.strip()}")
        url = BASE + match.group(1) + "/" + filename + match.group(2)
        target = OUT / "Cedar6.1" / directory / filename
        target.parent.mkdir(parents=True, exist_ok=True)
        if target.exists():
            print(f"exists {target.relative_to(OUT)}")
            continue
        print(f"fetch {url}")
        with urlopen(url) as response:
            target.write_bytes(response.read())
        count += 1
    print(f"materialized {count} files")

if __name__ == "__main__":
    main()
