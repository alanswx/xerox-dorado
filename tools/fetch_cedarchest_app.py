#!/usr/bin/env python3
"""Mirror a CedarChest6.1 package into the served STP tree.

    python3 tools/fetch_cedarchest_app.py ChessHack.df!3 [More.df!1 ...]
    python3 tools/fetch_cedarchest_app.py --volume=CedarChest6.0 Gargoyle.df!27

Fetches the named DF from [Cyan]<CedarChest6.1>Top>, parses its Exports /
Directory sections, downloads every listed file from the archive, and lays
them out the way dorado's in-process STP server expects
(chm/cedar/stp-root/<dir-path>/<bare-name>; the server strips versions and
indexes creation dates by scanning every <vol>/Top DF, so the DF itself
lands in CedarChest6.1/Top/).

In the running desktop:  Bringover -p [Cedar]<CedarChest6.1>Top>X.df
then whatever the package's doc says (usually `Run X`).
"""
import os
import re
import sys
import urllib.request
import urllib.error

HOST = 'https://xeroxparcarchive.computerhistory.org'
REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROOT = os.path.join(REPO, 'chm', 'cedar', 'stp-root')

# `Exports [Cedar]<CedarChest6.1>AISViewer>` (Cartoon.df appends `--Was ...`
# comments right after the closing `>`).
SECTION_RE = re.compile(r'^\s*(Exports|Directory|Imports)\s+\[(\w+)\]<([^>]+(?:>[^>\s-][^>]*)*)>')
FILE_RE = re.compile(r'^\s*\+?([A-Za-z0-9][^\s!]*\.[A-Za-z0-9]+)!(\d+)\s')


def fetch(url):
    req = urllib.request.Request(url, headers={'User-Agent': 'dorado-mirror'})
    with urllib.request.urlopen(req, timeout=90) as r:
        return r.read()


def save(rel, data):
    path = os.path.join(ROOT, rel)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'wb') as f:
        f.write(data)
    print(f'  {rel}  ({len(data)} bytes)')


def mirror_package(df_spec, vol='CedarChest6.1'):
    """vol picks the CedarChest release. 6.1 is the default because that is
    what our Cedar runs, but 6.0 is worth reaching for when a 6.1 package will
    not bind: the two releases were built at different times and a .bcd from
    one can want an interface version the other does not ship, which is
    exactly the VersionMismatch[BiScrollers] wall Gargoyle hits."""
    name, _, ver = df_spec.partition('!')
    if not ver:
        sys.exit(f'{df_spec}: need an explicit version (Name.df!N)')
    print(f'== {vol} {name}!{ver}')
    df_bytes = fetch(f'{HOST}/cyan/{vol.lower()}/top/{name}!{ver}')
    if df_bytes.lstrip()[:9] == b'<!DOCTYPE':
        sys.exit(f'{df_spec}: archive returned 404')
    save(f'{vol}/Top/{name}', df_bytes)

    text = df_bytes.decode('latin-1').replace('\r', '\n')
    section = None          # (volume, dir-path) of the current section
    fetch_section = False
    failures = []
    for line in text.split('\n'):
        m = SECTION_RE.match(line)
        if m:
            kind, vol, path = m.groups()
            section = path.replace('>', '/')
            # Only mirror files this DF actually owns; Imports come from
            # their own DFs (Cedar6.1 core is already served).
            fetch_section = kind in ('Exports', 'Directory')
            continue
        fm = FILE_RE.match(line)
        if not (fm and section and fetch_section):
            continue
        fname, fver = fm.groups()
        rel = f'{section}/{fname}'
        if fname == name:                     # the DF's own export entry
            continue
        if os.path.exists(os.path.join(ROOT, rel)):
            print(f'  {rel}  (already present)')
            continue
        url = f'{HOST}/cyan/{section.lower()}/{fname}!{fver}'
        try:
            data = fetch(url)
        except (urllib.error.HTTPError, urllib.error.URLError) as e:
            failures.append((rel, fver, str(e)))
            continue
        if data.lstrip()[:9] == b'<!DOCTYPE':
            failures.append((rel, fver, '404'))
            continue
        save(rel, data)
    for rel, fver, err in failures:
        print(f'  MISSING {rel}!{fver}: {err}')
    return failures


def main():
    args = sys.argv[1:]
    vol = 'CedarChest6.1'
    if args and args[0].startswith('--volume='):
        vol = args.pop(0).split('=', 1)[1]
    if not args:
        sys.exit(__doc__)
    all_failures = []
    for spec in args:
        all_failures += mirror_package(spec, vol)
    if all_failures:
        print(f'{len(all_failures)} file(s) missing — see above')


if __name__ == '__main__':
    main()
