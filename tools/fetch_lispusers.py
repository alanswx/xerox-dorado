#!/usr/bin/env python3
"""Mirror Interlisp-D library packages from eris/lispusers.

    python3 tools/fetch_lispusers.py BROWSER EDITBITMAP
    python3 tools/fetch_lispusers.py --list

Downloads the highest-numbered version of each named `.DCOM` (a compiled
Interlisp file) into chm/lisp/ftp-root/, which is both the directory the
emulator serves over STP (`--ftp-root`) and the source the Lyric disk pack is
built from (`make lisp-lispusers-pack`).

Why DCOM and not source: Lyric loads a `.DCOM` directly, whereas a `.LSP`
would have to be compiled inside the guest first.  eris/lispusers holds ~295
of them, all built 1983-1986 against the Lyric-era system.

Version choice matters and is not cosmetic.  The archive keeps every version
ever written, and older ones were built against earlier Interlisp releases;
loading one into Lyric can fail on a changed internal interface.  The index
lists them in order, so the highest number is the newest build.

But "newest" is only the right rule WITHIN one tree.  `phylum/lisp/lyric/` is
the Lyric release; `phylum/lispusers/lyric/` and `phylum/lisplibrary/lyric/`
are later contributions that happen to target it, running to 1988.  Taking
the highest version across all of them silently prefers the post-release
build: FILEBROWSER.LCOM is !1 (Mar-1987) in the release and !2 (Jun-1988)
outside it, and the 1988 one wants a TABLEBROWSER the 1987 one does not.
Mixing trees is the same mistake that produced VersionMismatch[BiScrollers]
on the Cedar side, so the release tree wins outright here and the later trees
only supply names the release does not have at all.
"""
import argparse
import os
import re
import sys
import urllib.request

HOST = 'https://xeroxparcarchive.computerhistory.org/eris/lispusers'
REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEST = os.path.join(REPO, 'chm', 'lisp', 'ftp-root')

# The index renders one row per file version:
#   NAME.DCOM!12  someone (Original) 9630 30-Jul-1984 ...
ROW_RE_TMPL = r'{name}!(\d+) \S+ \(Original\) (\d+) (\d\d-\w\w\w-\d{{4}})'


def fetch(url):
    req = urllib.request.Request(url, headers={'User-Agent': 'dorado'})
    return urllib.request.urlopen(req, timeout=120).read()


def index_text():
    raw = fetch(HOST + '/.index.html').decode('latin-1')
    return ' '.join(re.sub(r'<[^>]+>', ' ', raw).split())


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('names', nargs='*', help='package names, with or without .DCOM')
    ap.add_argument('--list', action='store_true', help='list every DCOM in the directory')
    args = ap.parse_args()

    flat = index_text()

    if args.list:
        seen = {}
        for m in re.finditer(r'([A-Z0-9][A-Z0-9-]*\.DCOM)!(\d+)', flat):
            n, v = m.group(1), int(m.group(2))
            seen[n] = max(seen.get(n, 0), v)
        for n in sorted(seen):
            print('%-24s !%d' % (n, seen[n]))
        print('%d packages' % len(seen), file=sys.stderr)
        return 0

    if not args.names:
        ap.error('give package names or --list')

    os.makedirs(DEST, exist_ok=True)
    rc = 0
    for raw_name in args.names:
        name = raw_name if raw_name.upper().endswith('.DCOM') else raw_name + '.DCOM'
        name = name.upper()
        rows = re.findall(ROW_RE_TMPL.format(name=re.escape(name)), flat)
        if not rows:
            print('%s: not in the lispusers index' % name, file=sys.stderr)
            rc = 1
            continue
        ver, size, date = max(rows, key=lambda r: int(r[0]))
        data = fetch('%s/%s!%s' % (HOST, name, ver))
        if len(data) != int(size):
            print('%s!%s: got %d bytes, index says %s' % (name, ver, len(data), size),
                  file=sys.stderr)
            rc = 1
            continue
        open(os.path.join(DEST, name), 'wb').write(data)
        print('%-20s !%-3s %8d bytes  %s' % (name, ver, len(data), date))
    return rc


if __name__ == '__main__':
    sys.exit(main())
