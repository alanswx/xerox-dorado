#!/usr/bin/env python3
"""Fetch the Cedar 6.1 "boot essentials" file closure into the STP root.

After the Basic.Loadees packages load and START, Cedar's InstallerImpl asks
for a DF file of "booting essentials" and installs the files it names: TIP
tables, .icons, JaM programs, User.Profile and a handful of extra BCDs that
the Viewers desktop needs at runtime.  This resolves that closure from the
original DF files and downloads it from the CHM archive.

    python3 tools/fetch_cedar_essentials.py [--dry-run]

DF format (as used here):
  * `Exports [Cedar]<Cedar6.1>Dir>` and `Directory [Cedar]<Cedar6.1>Dir>`
    both open a section; the indented `name!version  date` lines that follow
    live in Dir.  A leading '+' on an entry is a DF bookkeeping marker.
  * `Exports Imports [Cedar]<Cedar6.1>Top>X.df Of ~= Using [a, b, ...]`
    references a sub-DF and the files wanted from it.  The Using list may
    wrap across lines.  A clause with no Using list (EssentialStyles.df,
    whose `Using ALL` is commented out) means take everything that DF
    exports.

Archive URL convention (docs/chm-urls.md):
    https://<host>/cyan/cedar6.1/<dir lowercased>/<name>!<version>
Files land in chm/cedar/stp-root/Cedar6.1/<Dir>/<name>, which is what the
in-process STP server (dorado/src/ethernet.c, --ftp-root) serves.

The sub-DFs themselves must already be in Top/; fetch any new one the same
way BootEssentials.df was fetched.
"""
import os
import re
import sys
import urllib.request

HOST = 'https://xeroxparcarchive.computerhistory.org'
REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROOT = os.path.join(REPO, 'chm', 'cedar', 'stp-root', 'Cedar6.1')
TOP = os.path.join(ROOT, 'Top')

# A sub-DF reference. Must be tested BEFORE SECTION_RE: it also starts "Exports".
SUBDF_RE = re.compile(
    r'^\s*Exports\s+Imports\s+\[Cedar\]<Cedar6\.1>Top>(\S+\.df)\s+Of', re.I)
# A section header: everything indented under it lives in this directory.
SECTION_RE = re.compile(
    r'^\s*(?:Exports|Directory|Imports)\s+\[Cedar\]<Cedar6\.1>([^>]+)>\s*$',
    re.I)
# An exported file entry, e.g. "  Standard.icons!1   14-Dec-83 ..." or " +X.bcd!6 ...".
FILE_RE = re.compile(r'^\s+\+?([^\s+][^\s]*\.[^\s!]+)!(\d+)\s')
USING_RE = re.compile(r'Using\s*\[([^\]]*)\]', re.I | re.S)


def read_df(path):
    with open(path, 'rb') as f:
        return f.read().decode('latin-1').replace('\r', '\n').split('\n')


def parse_exports(path):
    """lowercased name -> (dir, name, version) for every file the DF places.

    Keyed case-insensitively: BootEssentials.df spells some entries in a
    different case than the DF that exports them (`tiogautils.jam` vs
    `TiogaUtils.jam`), and the archive wants the exporting DF's spelling.
    """
    index = {}
    cur = None
    for line in read_df(path):
        if SUBDF_RE.match(line):
            cur = None
            continue
        m = SECTION_RE.match(line)
        if m:
            cur = m.group(1)
            continue
        m = FILE_RE.match(line)
        if m and cur:
            index.setdefault(m.group(1).lower(), (cur, m.group(1), m.group(2)))
    return index


def parse_boot_essentials(path):
    """([(subdf, wanted-names-or-None)], {dir: {name: version}} own files)."""
    lines = read_df(path)
    wants, own = [], {}
    cur = None
    i = 0
    while i < len(lines):
        line = lines[i]
        m = SUBDF_RE.match(line)
        if m:
            subdf = m.group(1)
            cur = None
            # Collect the clause body: following lines up to a blank line or
            # the next clause/section. The Using list may wrap.
            body = ''
            j = i + 1
            while j < len(lines):
                nxt = lines[j]
                if not nxt.strip() or SUBDF_RE.match(nxt) or SECTION_RE.match(nxt):
                    break
                body += ' ' + nxt
                j += 1
            um = USING_RE.search(body)
            names = None
            if um:
                names = [n.strip().lstrip('+') for n in um.group(1).split(',')
                         if n.strip()]
            wants.append((subdf, names))
            i = j
            continue
        m = SECTION_RE.match(line)
        if m:
            cur = m.group(1)
            i += 1
            continue
        m = FILE_RE.match(line)
        if m and cur:
            own.setdefault(cur, {})[m.group(1)] = m.group(2)
        i += 1
    return wants, own


def fetch(directory, name, version, dry, tally):
    def note(res):
        tally[res] = tally.get(res, 0) + 1
        return res

    dest_dir = os.path.join(ROOT, directory)
    dest = os.path.join(dest_dir, name)
    if os.path.exists(dest):
        return note('have')
    url = f'{HOST}/cyan/cedar6.1/{directory.lower()}/{name}!{version}'
    if dry:
        print(f'  would fetch {directory}/{name}!{version}')
        return note('dry')
    try:
        with urllib.request.urlopen(url, timeout=60) as r:
            data = r.read()
    except Exception as e:                                    # noqa: BLE001
        print(f'  FAIL {directory}/{name}!{version}: {e}')
        return note('fail')
    os.makedirs(dest_dir, exist_ok=True)
    with open(dest, 'wb') as f:
        f.write(data)
    print(f'  got  {directory}/{name}!{version}  ({len(data)} bytes)')
    return note('new')


def main():
    dry = '--dry-run' in sys.argv
    be = os.path.join(TOP, 'BootEssentials.df')
    if not os.path.exists(be):
        sys.exit(f'missing {be} — fetch it from '
                 f'{HOST}/cyan/cedar6.1/top/BootEssentials.df!5 first')

    wants, own = parse_boot_essentials(be)
    tally = {}

    print('BootEssentials.df own exports:')
    for directory, files in sorted(own.items()):
        for name, version in sorted(files.items()):
            if name.endswith('.df'):
                continue                    # the DF exports itself
            fetch(directory, name, version, dry, tally)

    for subdf, names in wants:
        sub_path = os.path.join(TOP, subdf)
        if not os.path.exists(sub_path):
            print(f'{subdf}: MISSING sub-DF in Top/ — skipping')
            tally['fail'] = tally.get('fail', 0) + 1
            continue
        index = parse_exports(sub_path)
        selected = names if names is not None else sorted(index)
        print(f'{subdf}: {len(selected)} file(s)')
        for name in selected:
            entry = index.get(name.lower())
            if not entry:
                print(f'  MISSING in {subdf}: {name}')
                tally['fail'] = tally.get('fail', 0) + 1
                continue
            directory, real_name, version = entry
            fetch(directory, real_name, version, dry, tally)

    print('\nsummary:', ', '.join(f'{k}={v}' for k, v in sorted(tally.items())))


if __name__ == '__main__':
    main()
