#!/usr/bin/env python3
"""Fetch the Dorado boards' Sil design files from the CHM PARC archive.

Tim's recommendation for Phase 2 is to build the Verilog FROM the Sil files
rather than from the Hardware Manual: the Sil drawings were the design input,
they tracked the bug fixes, and PARC's own ANALYZE turned them into netlists.
We held none of them -- only rendered PDFs -- which was the blocker.

They live in `[Io]<DoradoLogic>` as one IFS dump archive PER BOARD REVISION,
already expanded by the CHM mirror, so each member has a raw URL. An archive
holds more than the sheets:

    ProcH-Rev-Ce.dm!1_/
        ProcH01.sil .. ProcH24.sil   the sheets
        ProcH-Rev-Ce.sil             the assembled drawing
        ProcH-Rev-Ce-C.nl            netlists, which is the point
        ProcH-Rev-Ce-E.nl
        ProcH-Rev-Ce.wl              wire list
        ProcH-Rev-Ce.ad .bp .lc      board/layout
        Build.cm PrintProcH.cm       the design-automation scripts

Three variants of each board exist -- bare `-Rev-`, `-mwRev-` (multiwire
prototype) and `-apcRev-` (the printed-circuit rev). This fetches the bare
one at its highest revision by default, which is the design of record; pass
--all-variants for everything.

    fetch_dorado_sil.py [--dest chm/sil] [--all-variants] [--list] [--board X]

Idempotent: a member already on disk is not re-fetched, so an interrupted run
is resumed by running it again.
"""

import argparse
import os
import re
import sys
import time
import urllib.parse
import urllib.request

BASE = "https://xeroxparcarchive.computerhistory.org/io/doradologic/"
UA = {"User-Agent": "dorado-emulator-archive-fetch/1.0"}


def get(url, binary=True):
    req = urllib.request.Request(url, headers=UA)
    with urllib.request.urlopen(req, timeout=60) as r:
        data = r.read()
    return data if binary else data.decode("utf-8", "replace")


def archive_list():
    """Every expanded `.dm` archive in the directory, as (name, version)."""
    html = get(BASE + ".index.html", binary=False)
    out = []
    for href in re.findall(r'href="([^"]+)"', html):
        m = re.match(r"([^/]+)\.dm!(\d+)_/\.index\.html$", href)
        if m:
            out.append((m.group(1), m.group(2)))
    return out


def pick(archives, all_variants):
    """Bare `-Rev-` archives only, highest revision per board, unless
    --all-variants.  Board name is what precedes the first `-`."""
    if all_variants:
        return sorted(archives)
    best = {}
    for name, ver in archives:
        m = re.match(r"(.+?)-Rev-([A-Za-z]+)$", name)
        if not m:
            continue
        # Board key is case-folded: the archive spells the same board both
        # `IOTest-Rev-Ad` and `IOtest-Rev-Ac`, and those are one board with
        # two revisions, not two boards.
        board, rev = m.group(1).lower(), m.group(2)
        if board not in best or rev.lower() > best[board][1].lower():
            best[board] = (name, rev, ver)
    return sorted((v[0], v[2]) for v in best.values())


def members(name, ver):
    html = get(f"{BASE}{urllib.parse.quote(name)}.dm!{ver}_/.index.html",
               binary=False)
    seen, out = set(), []
    for href in re.findall(r'href="([^"]+)"', html):
        # skip the HTML octal-dump viewers (leading dot) and absolute links
        if href.startswith("http") or href.startswith(".") or "/" in href:
            continue
        if href not in seen:
            seen.add(href)
            out.append(href)
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dest", default="chm/sil")
    ap.add_argument("--all-variants", action="store_true")
    ap.add_argument("--board", action="append",
                    help="fetch only these boards (repeatable, e.g. ProcH)")
    ap.add_argument("--list", action="store_true",
                    help="print what would be fetched and stop")
    ap.add_argument("--delay", type=float, default=0.05)
    args = ap.parse_args()

    chosen = pick(archive_list(), args.all_variants)
    if args.board:
        want = [b.lower() for b in args.board]
        chosen = [(n, v) for (n, v) in chosen
                  if any(n.lower().startswith(b) for b in want)]
    if not chosen:
        sys.exit("no archives matched")

    if args.list:
        for n, v in chosen:
            print(f"{n}.dm!{v}")
        return

    total = new = 0
    for name, ver in chosen:
        outdir = os.path.join(args.dest, f"{name}.dm!{ver}_")
        os.makedirs(outdir, exist_ok=True)
        try:
            names = members(name, ver)
        except Exception as e:                       # noqa: BLE001
            print(f"  {name}: index failed: {e}", file=sys.stderr)
            continue
        got = 0
        for m in names:
            total += 1
            path = os.path.join(outdir, m)
            if os.path.exists(path) and os.path.getsize(path) > 0:
                continue
            url = (f"{BASE}{urllib.parse.quote(name)}.dm!{ver}_/"
                   f"{urllib.parse.quote(m)}")
            try:
                data = get(url)
            except Exception as e:                   # noqa: BLE001
                print(f"  {name}/{m}: {e}", file=sys.stderr)
                continue
            with open(path, "wb") as f:
                f.write(data)
            got += 1
            new += 1
            time.sleep(args.delay)
        sil = sum(1 for m in names if m.lower().endswith(".sil"))
        nl = sum(1 for m in names if m.lower().endswith(".nl"))
        print(f"{name}.dm!{ver}: {len(names)} members "
              f"({sil} .sil, {nl} .nl), {got} newly fetched")
    print(f"\n{len(chosen)} archives, {total} members, {new} newly fetched "
          f"-> {args.dest}")


if __name__ == "__main__":
    main()
