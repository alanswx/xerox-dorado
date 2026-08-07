#!/usr/bin/env python3
"""Fetch one expanded IFS dump archive from the CHM PARC archive.

The mirror expands most `.dm` archives into a browsable directory, so each
member has a raw URL: `<dir>/<Name>.dm!<ver>_/<member>`.  This walks that
listing and downloads every member, plus the packed `.dm` blob itself when it
is available, so the original artifact is kept alongside its contents.

    fetch_chm_archive.py cyan/d0logic/GarageD0Drawings.Dm!1 --dest chm/garage

Casing matters and the cross-reference HTML is NOT a guide to it: that file
lowercases every path, while the live URL wants the IFS spelling
(`ProcH-Rev-Ce.dm`, `GarageD0Drawings.Dm`).  Fetch the parent `.index.html`
and copy the name out of it when a path 404s.
"""

import argparse
import os
import re
import sys
import time
import urllib.parse
import urllib.request

BASE = "https://xeroxparcarchive.computerhistory.org/"
UA = {"User-Agent": "dorado-emulator-archive-fetch/1.0"}


def get(url, binary=True):
    req = urllib.request.Request(url, headers=UA)
    with urllib.request.urlopen(req, timeout=120) as r:
        data = r.read()
    return data if binary else data.decode("utf-8", "replace")


def quote(path):
    return urllib.parse.quote(path, safe="/!")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("path", help="IFS path, e.g. cyan/d0logic/Foo.dm!1")
    ap.add_argument("--dest", default="chm/garage")
    ap.add_argument("--delay", type=float, default=0.05)
    ap.add_argument("--no-blob", action="store_true",
                    help="skip the packed .dm itself")
    args = ap.parse_args()

    name = args.path.rsplit("/", 1)[-1]
    outdir = os.path.join(args.dest, name + "_")
    os.makedirs(outdir, exist_ok=True)

    try:
        html = get(BASE + quote(args.path) + "_/.index.html", binary=False)
    except Exception as e:                            # noqa: BLE001
        sys.exit(f"{args.path}: no expanded listing ({e}) -- "
                 f"check the casing against the parent .index.html")

    seen, members = set(), []
    for href in re.findall(r'href="([^"]+)"', html):
        if href.startswith("http") or href.startswith(".") or "/" in href:
            continue
        if href not in seen:
            seen.add(href)
            members.append(href)

    got = 0
    for m in members:
        path = os.path.join(outdir, m)
        if os.path.exists(path) and os.path.getsize(path) > 0:
            continue
        try:
            data = get(BASE + quote(args.path) + "_/" + urllib.parse.quote(m))
        except Exception as e:                        # noqa: BLE001
            print(f"  {m}: {e}", file=sys.stderr)
            continue
        with open(path, "wb") as f:
            f.write(data)
        got += 1
        time.sleep(args.delay)

    blob = ""
    if not args.no_blob:
        target = os.path.join(args.dest, name)
        if not os.path.exists(target):
            try:
                with open(target, "wb") as f:
                    f.write(get(BASE + quote(args.path)))
                blob = f", packed .dm kept as {name}"
            except Exception as e:                    # noqa: BLE001
                print(f"  packed .dm: {e}", file=sys.stderr)

    print(f"{name}: {len(members)} members, {got} newly fetched "
          f"-> {outdir}{blob}")


if __name__ == "__main__":
    main()
