#!/usr/bin/env python3
"""Fetch the Cedar font files the Installer's TryForFonts step asks for.

Cedar's InstallerImpl.TryForFonts does a BringOver of the fonts DFs named by
the Installer.*DF user-profile tokens ([Fonts]<CedarFonts>Top>TiogaFonts.df
and friends).  Without fonts the Imager has nothing to paint text with, so
the Viewers desktop never comes up.

    python3 tools/fetch_cedar_fonts.py [--dry-run]

The DFs themselves are served from chm/cedar/stp-root/CedarFonts/Top/
(TiogaFonts.df, FontMetrics.df, PressFonts.df; the archive has no
XC1-2-2-Fonts.df).  Each DF exports its files with a full directory path --
`Exports [Fonts]<Fonts>TiogaFonts>` -- and the STP server maps a demand
fetch's (Directory Fonts)(Name-Body TiogaFonts>Classic10.ks) request to
stp-root/Fonts/TiogaFonts/Classic10.ks, so the destination must keep every
path component after the volume.  The emulator indexes creation dates out of
the same DFs it serves (dorado/src/ethernet.c), so the dates it advertises
match the ones BringOver demands.
"""
import os
import re
import sys
import urllib.request

HOST = 'https://xeroxparcarchive.computerhistory.org'
REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROOT = os.path.join(REPO, 'chm', 'cedar', 'stp-root')
DF_DIR = os.path.join(ROOT, 'CedarFonts', 'Top')
XREF = os.path.join(REPO, 'chm', 'cross-reference.html')

# Capture the whole export path: `Exports [Fonts]<Fonts>TiogaFonts>` ->
# volume "Fonts", path "Fonts>TiogaFonts".
SECTION_RE = re.compile(r'^\s*(?:Exports|Directory)\s+\[(\w+)\]<(.+)>\s*$', re.I)
IMPORTS_RE = re.compile(r'^\s*Imports\b', re.I)
FILE_RE = re.compile(r'^\s+\+?([^\s+][^\s]*\.[^\s!]+)!(\d+)\s')
# <li><a href="_cd6_/tioga/ksfonts/.index.html">...</a>Tioga10B.ks!1 22528 ...
XREF_RE = re.compile(r'href="([^"]+)/\.index\.html">[^<]*</a>([^\s!]+)!(\d+)\s')

# Preferred archive directory substring per extension: the DFs name the files
# but not where the archive keeps them (the .ks sets live under _cd6_/tioga,
# the .tfm metrics under cyan/cedarfonts/fontmetrics, the .sd splines under
# indigo/pressfonts).
PREFER = {
    '.ks': 'tioga',
    '.strike': 'tioga',
    '.tfm': 'fontmetrics',
    '.sd': 'pressfonts',
}


def build_xref():
    """lowercased filename -> [(archive dir, archive-cased name, version), ...].

    The archive URLs are case-sensitive and IFS was not: the DF says
    Classic-mrr.sd where [Indigo]<PressFonts> stores CLASSIC-MRR.sd, so the
    URL must use the archive's own casing.
    """
    index = {}
    if not os.path.exists(XREF):
        return index
    with open(XREF, 'rb') as f:
        for line in f:
            m = XREF_RE.search(line.decode('latin-1', 'replace'))
            if m:
                index.setdefault(m.group(2).lower(), []).append(
                    (m.group(1), m.group(2), m.group(3)))
    return index


def parse_df(path):
    """Yield (export path with '/' separators, filename, version)."""
    text = open(path, 'rb').read().decode('latin-1').replace('\r', '\n')
    cur = None
    for line in text.split('\n'):
        if IMPORTS_RE.match(line):
            cur = None
            continue
        m = SECTION_RE.match(line)
        if m:
            cur = m.group(2).replace('>', '/')
            continue
        m = FILE_RE.match(line)
        if m and cur:
            yield cur, m.group(1), m.group(2)


def main():
    dry = '--dry-run' in sys.argv
    dfs = sorted(f for f in os.listdir(DF_DIR) if f.endswith('.df'))
    if not dfs:
        sys.exit(f'no DFs under {DF_DIR} -- fetch them from '
                 f'{HOST}/indigo/fonts/top/ first')

    want = []
    for df in dfs:
        entries = list(parse_df(os.path.join(DF_DIR, df)))
        # Skip each DF's self-export ([Fonts]<Fonts>Top>X.df); the DFs are
        # already served from CedarFonts/Top.
        entries = [e for e in entries if not e[1].lower().endswith('.df')]
        print(f'{len(entries)} files named by {df}')
        want.extend(entries)

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
            print(f'  MISSING from archive index: {directory}/{name}')
            note('fail')
            continue
        prefer = PREFER.get(os.path.splitext(name)[1].lower(), '')
        cands.sort(key=lambda c: (0 if prefer and prefer in c[0].lower() else 1,
                                  -int(c[2])))
        adir, aname, aver = cands[0]
        if dry:
            print(f'  would fetch {directory}/{name} <- {adir}/{aname}!{aver}')
            note('dry')
            continue
        data = None
        for adir, aname, aver in cands[:3]:
            try:
                with urllib.request.urlopen(
                        f'{HOST}/{adir}/{aname}!{aver}', timeout=60) as r:
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
