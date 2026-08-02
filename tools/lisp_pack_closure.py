#!/usr/bin/env python3
"""Choose Interlisp packages for the Lyric pack, dependency closures included.

    python3 tools/lisp_pack_closure.py --budget 4500 > dorado/lisp-lispusers-packages.txt

The first cut of this selection sorted by size and took the smallest 194
files, which maximises the package COUNT and is exactly the wrong objective:
a package whose dependencies did not fit is not a package, it is an error
window.  HELPSYS without DINFO and ADDRESSBOOK without OPENTEXTSTREAM both
failed that way.

An Interlisp file's COMS carries a `(FILES A B C)` clause naming what it
loads, and those names survive into the compiled `.LCOM` as plain ASCII --
so the graph can be built from the mirrored binaries with no network and no
source parsing.  Scanning for *any* known package name rather than parsing
the FILES clause over-approximates: a name mentioned in a comment pulls in a
file that was not needed.  That is the safe direction to be wrong in, since
the cost is disk pages and the alternative is a broken package.

Names absent from the mirror are NOT missing: ATTACHEDWINDOW and ICONW are
base-system modules already inside the sysout, which is why FILEBROWSER loads
without them.  Only what the guest actually reports as "not found" needs
shipping.
"""

import argparse
import math
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MIRROR = os.path.join(REPO, 'chm', 'lisp', 'ftp-root')

# Seeds worth spending pages on, most important first.  Everything reachable
# from these comes along; whatever budget is left afterwards is filled with
# small extras.
SEEDS = [
    'FILEBROWSER', 'HELPSYS', 'CALENDAR', 'SKETCH', 'PACMAN', 'SOLITAIRE',
    'GRAPHER', 'MASTERSCOPE', 'DEDIT', 'PLOT', 'TEDITFIND', 'GREP',
    'WHOCALLS', 'HRULE', 'BITMAPFNS', 'TINYTIDY', 'SAMEDIR', 'FONTSAMPLE',
]

# Screen fonts, shipped alongside the packages. They are not .LCOMs so they
# have no dependencies and never appear in a closure, but a package that
# FONTCREATEs a family the sysout lacks draws nothing: SOLITAIRE and CALENDAR
# want HELVETICA and TIMESROMAN, FILEBROWSER wants GACHA, PACMAN wants LOGO.
# 67 pages for all of them.
FONTS = [
    'HELVETICA8-C0.DISPLAYFONT', 'HELVETICA10-C0.DISPLAYFONT',
    'HELVETICA10-B-C0.DISPLAYFONT', 'HELVETICA12-C0.DISPLAYFONT',
    'HELVETICA12-B-C0.DISPLAYFONT', 'HELVETICA14-C0.DISPLAYFONT',
    'TIMESROMAN8-C0.DISPLAYFONT', 'TIMESROMAN10-C0.DISPLAYFONT',
    'TIMESROMAN10-B-C0.DISPLAYFONT', 'TIMESROMAN12-C0.DISPLAYFONT',
    'GACHA8-C0.DISPLAYFONT', 'GACHA10-C0.DISPLAYFONT',
    'GACHA12-C0.DISPLAYFONT', 'LOGO24-C0.DISPLAYFONT',
]


def pages(size):
    """Alto disk cost: one leader page plus the data pages."""
    return 1 + math.ceil(size / 512)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--budget', type=int, default=4500,
                    help='disk pages available on the pack')
    ap.add_argument('--report', action='store_true',
                    help='write a per-seed breakdown to stderr')
    args = ap.parse_args()

    files = {f[:-5].upper(): f for f in os.listdir(MIRROR)
             if f.upper().endswith('.LCOM')}
    size = {k: os.path.getsize(os.path.join(MIRROR, v)) for k, v in files.items()}

    # Build the graph. A name only counts as a dependency if we actually have
    # a .LCOM for it; anything else is either base-system or genuinely absent,
    # and in both cases shipping is not the answer.
    names = sorted(files, key=len, reverse=True)
    pattern = re.compile(rb'\b(' + b'|'.join(re.escape(n.encode()) for n in names) + rb')\b')
    deps = {}
    for k, v in files.items():
        blob = open(os.path.join(MIRROR, v), 'rb').read()
        found = {m.group(1).decode() for m in pattern.finditer(blob)}
        deps[k] = found - {k}

    def closure(seed):
        seen, stack = set(), [seed]
        while stack:
            n = stack.pop()
            if n in seen or n not in files:
                continue
            seen.add(n)
            stack += list(deps.get(n, ()))
        return seen

    fonts = [f for f in FONTS if os.path.exists(os.path.join(MIRROR, f))]
    font_pages = sum(pages(os.path.getsize(os.path.join(MIRROR, f)))
                     for f in fonts)
    args.budget -= font_pages          # fonts come first, not last
    chosen, used = set(), 0
    for seed in SEEDS:
        if seed not in files:
            print(f'seed {seed}: not mirrored, skipped', file=sys.stderr)
            continue
        want = closure(seed) - chosen
        cost = sum(pages(size[n]) for n in want)
        if used + cost > args.budget:
            print(f'seed {seed}: needs {cost} pages, only '
                  f'{args.budget - used} left -- SKIPPED WHOLE', file=sys.stderr)
            continue
        chosen |= want
        used += cost
        if args.report:
            print(f'seed {seed:16s} +{len(want):3d} files {cost:5d} pages '
                  f'(total {used})', file=sys.stderr)

    # Fill the remainder with the smallest packages that still fit, so the
    # pack is not left half empty.
    for n in sorted(files, key=lambda k: size[k]):
        if n in chosen:
            continue
        c = pages(size[n])
        if used + c <= args.budget:
            chosen.add(n)
            used += c

    print('# Interlisp-D Lyric packages for the Lyric pack, generated by')
    print('# tools/lisp_pack_closure.py -- do not hand-edit; change SEEDS there.')
    print('#')
    print('# Chosen as dependency CLOSURES of the seed packages, not by size:')
    print('# a package whose dependencies did not fit is an error window, not')
    print('# a package (HELPSYS needs DINFO, FILEBROWSER needs TABLEBROWSER).')
    print(f'# {len(chosen)} packages + {len(fonts)} fonts, '
          f'{used + font_pages} of {args.budget + font_pages} pages.')
    for f in fonts:
        print(f)
    for n in sorted(chosen):
        print(files[n])
    print(f'selected {len(chosen)} files, {used}/{args.budget} pages',
          file=sys.stderr)
    return 0


if __name__ == '__main__':
    sys.exit(main())
