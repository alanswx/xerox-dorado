#!/usr/bin/env python3
"""Complete the served Cedar6.1 packages: fetch every file their DFs export.

    python3 tools/fetch_cedar_df_closure.py [--dry-run]

The boot bring-up fetched only the files Cedar demanded during boot, so the
served tree is a sparse subset of what its own DFs advertise. Anything else
Cedar asks for later (the interpreter's per-module symbol loads — e.g.
`Eval 3+4` fails with NoSymbols[ListImpl.bcd] — Tioga sources, tools) gets
"not found". This walks every DF under stp-root/Cedar6.1/Top and fetches
each missing Exports/Directory file from cyan/cedar6.1/ into place.
"""
import os
import re
import sys
import urllib.request
import urllib.error

HOST = 'https://xeroxparcarchive.computerhistory.org'
REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROOT = os.path.join(REPO, 'chm', 'cedar', 'stp-root')
TOP = os.path.join(ROOT, 'Cedar6.1', 'Top')

SECTION_RE = re.compile(
    r'^\s*(Exports|Directory|Imports)\s+\[(\w+)\]<([^>]+(?:>[^>\s-][^>]*)*)>')
FILE_RE = re.compile(r'^\s*\+?([A-Za-z0-9][^\s!]*\.[A-Za-z0-9]+)!(\d+)\s')


def fetch(url):
    req = urllib.request.Request(url, headers={'User-Agent': 'dorado-mirror'})
    with urllib.request.urlopen(req, timeout=90) as r:
        return r.read()


def main():
    dry = '--dry-run' in sys.argv
    fetched = skipped = missing = 0
    for df in sorted(os.listdir(TOP)):
        if not df.lower().endswith('.df'):
            continue
        text = open(os.path.join(TOP, df), 'rb').read() \
            .decode('latin-1').replace('\r', '\n')
        section, take = None, False
        for line in text.split('\n'):
            m = SECTION_RE.match(line)
            if m:
                kind, vol, path = m.groups()
                # Only complete the Cedar6.1 release tree itself.
                take = (kind in ('Exports', 'Directory') and
                        path.split('>')[0] == 'Cedar6.1')
                section = path.replace('>', '/')
                continue
            fm = FILE_RE.match(line)
            if not (fm and take):
                continue
            fname, fver = fm.groups()
            rel = f'{section}/{fname}'
            dst = os.path.join(ROOT, rel)
            if os.path.exists(dst):
                skipped += 1
                continue
            url = f'{HOST}/cyan/{section.lower()}/{fname}!{fver}'
            if dry:
                print(f'would fetch {rel}!{fver}')
                fetched += 1
                continue
            try:
                data = fetch(url)
            except (urllib.error.HTTPError, urllib.error.URLError) as e:
                print(f'MISSING {rel}!{fver}: {e}')
                missing += 1
                continue
            if data.lstrip()[:9] == b'<!DOCTYPE':
                print(f'MISSING {rel}!{fver}: 404')
                missing += 1
                continue
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            with open(dst, 'wb') as f:
                f.write(data)
            fetched += 1
            print(f'{rel}  ({len(data)} bytes)')
    print(f'-- fetched {fetched}, already present {skipped}, missing {missing}')


if __name__ == '__main__':
    main()
