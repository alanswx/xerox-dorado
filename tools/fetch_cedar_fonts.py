#!/usr/bin/env python3
"""Fetch the Cedar font files the Installer's TryForFonts step asks for.

Cedar's InstallerImpl.TryForFonts does a BringOver of the fonts DFs named by
the Installer.*DF user-profile tokens ([CedarFonts]<Top>TiogaFonts.df and
friends).  Without fonts the Imager has nothing to paint text with, so the
Viewers desktop never comes up.

    python3 tools/fetch_cedar_fonts.py [--dry-run]

TiogaFonts.df is served from chm/cedar/stp-root/CedarFonts/Top/.  It exports
its files to [Fonts]<Fonts>, which the STP server maps to
chm/cedar/stp-root/Fonts/.  The emulator indexes creation dates out of the
same DF it serves (dorado/src/ethernet.c), so the dates it advertises match
the ones BringOver demands.
"""
import os
import re
import sys
import urllib.request

HOST = 'https://xeroxparcarchive.computerhistory.org'
REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROOT = os.path.join(REPO, 'chm', 'cedar', 'stp-root')
DF = os.path.join(ROOT, 'CedarFonts', 'Top', 'TiogaFonts.df')
XREF = os.path.join(REPO, 'chm', 'cross-reference.html')

SECTION_RE = re.compile(r'^\s*(?:Exports|Directory)\s+\[(\w+)\]<([^>]+)>', re.I)
FILE_RE = re.compile(r'^\s+\+?([^\s+][^\s]*\.[^\s!]+)!(\d+)\s')
# <li><a href="_cd6_/tioga/ksfonts/.index.html">...</a>Tioga10B.ks!1 22528 ...
XREF_RE = re.compile(r'href="([^"]+)/\.index\.html">[^<]*</a>([^\s!]+)!(\d+)\s')


def build_xref():
    """lowercased filename -> [(archive dir, version), ...].

    TiogaFonts.df names the fonts but not where the archive keeps them (the
    .ks and .strike sets live under separate _cd6_/tioga directories, not
    under Indigo), so resolve each file through the archive's own index.
    """
    index = {}
    if not os.path.exists(XREF):
        return index
    with open(XREF, 'rb') as f:
        for line in f:
            m = XREF_RE.search(line.decode('latin-1', 'replace'))
            if m:
                index.setdefault(m.group(2).lower(), []).append(
                    (m.group(1), m.group(3)))
    return index


def main():
    dry = '--dry-run' in sys.argv
    if not os.path.exists(DF):
        sys.exit(f'missing {DF} -- fetch it from '
                 f'{HOST}/indigo/fonts/top/TiogaFonts.df!5 first')
    text = open(DF, 'rb').read().decode('latin-1').replace('\r', '\n')

    cur = None
    want = []
    for line in text.split('\n'):
        m = SECTION_RE.match(line)
        if m:
            cur = m.group(2).replace('>', '/')
            continue
        m = FILE_RE.match(line)
        if m and cur:
            want.append((cur, m.group(1), m.group(2)))

    print(f'{len(want)} font files named by TiogaFonts.df')
    xref = build_xref()
    tally = {}

    def note(res):
        tally[res] = tally.get(res, 0) + 1

    for directory, name, _version in want:
        dest_dir = os.path.join(ROOT, directory)
        dest = os.path.join(dest_dir, name)
        if os.path.exists(dest):
            note('have')
            continue
        cands = xref.get(name.lower())
        if not cands:
            print(f'  MISSING from archive index: {name}')
            note('fail')
            continue
        # Prefer the Tioga font directories; fall back to whatever exists.
        cands.sort(key=lambda c: (0 if 'tioga' in c[0] else 1, -int(c[1])))
        adir, aver = cands[0]
        if dry:
            print(f'  would fetch {directory}/{name} <- {adir}/{name}!{aver}')
            note('dry')
            continue
        data = None
        for adir, aver in cands[:3]:
            try:
                with urllib.request.urlopen(
                        f'{HOST}/{adir}/{name}!{aver}', timeout=60) as r:
                    data = r.read()
                break
            except Exception:                                 # noqa: BLE001
                continue
        if data is None:
            print(f'  FAIL {name}: no candidate URL fetched')
            note('fail')
            continue
        os.makedirs(dest_dir, exist_ok=True)
        with open(dest, 'wb') as f:
            f.write(data)
        note('new')

    print('summary:', ', '.join(f'{k}={v}' for k, v in sorted(tally.items())))


if __name__ == '__main__':
    main()
