#!/usr/bin/env python3
"""Emit Verilog cell skeletons for the MECL parts the Dorado boards use.

The PORT LIST of every cell comes from PARC's own `EclDict.Analyze` (pin
numbers and, for the complex parts, the datasheet signal names), and the
DIRECTION of every pin comes from how the boards actually drive it in the
`.wl` wire lists. So neither is invented -- which matters, because a cell with
a plausible-but-wrong pinout produces RTL that looks right and is silently
mis-wired.

What this tool does NOT write is the BEHAVIOUR. Each generated file carries
its ports, its usage count, and a `TODO` marked body. Filling those in is the
per-part work, and each one should cite the part's function when it lands
(MC10100 = quad 2-input OR/NOR, MC10181 = 4-bit ALU, and so on).

Cells that already exist are NOT overwritten -- hand-written behaviour is
never clobbered by a regeneration. Use --force to re-emit skeletons.

Usage:
    sil_emit_cells.py --sil chm/sil --dict .../ecldict.analyze --out verilog/cells
"""

from __future__ import annotations

import argparse
import os
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sil_netlist import find_boards, load_board, vpart  # noqa: E402
from sil_ecldict import EclDict                     # noqa: E402

SKIP_PREFIX = ('SpareSocket', 'Term')


def collect(sil_dir: str):
    """part type -> (usage count, {pin: set of directions seen})."""
    usage: Counter = Counter()
    pins: dict[str, dict[int, set]] = defaultdict(lambda: defaultdict(set))
    for wl, lc in find_boards(sil_dir):
        b = load_board(wl, lc)
        for pos, pkg in b.packages.items():
            t = pkg.get('type', '')
            if t and not t.startswith(SKIP_PREFIX):
                usage[t] += 1
        for net in b.nets.values():
            for p in net['pins']:
                t = b.packages.get(p['pkg'], {}).get('type', '')
                if t and not t.startswith(SKIP_PREFIX):
                    pins[t][p['pin']].add(p['dir'])
    return usage, pins


def emit_cell(part: str, count: int, pindirs: dict[int, set],
              names: dict[int, str]) -> str:
    L = []
    A = L.append
    A(f'// cell_{vpart(part)} -- MECL model for the Xerox Dorado'
      f'{"  (part " + part + ")" if vpart(part) != part else ""}')
    A(f'//')
    A(f'// Ports: pin numbers and signal names from PARC\'s EclDict.Analyze.')
    A(f'// Directions: observed in the .wl wire lists across all boards.')
    A(f'// Used in {count} package position(s) across the sixteen boards.')
    A(f'//')
    A(f'// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when')
    A(f'// filling this in, and keep the port list generated -- do not retype')
    A(f'// pin numbers by hand.')
    A('')
    A('`default_nettype none')
    A('')
    A(f'module cell_{vpart(part)} (')
    decl = []
    for pin in sorted(pindirs):
        dirs = pindirs[pin]
        # A pin driven anywhere is an output; otherwise an input. Both seen
        # means the part drives it on one board and senses it on another --
        # flagged rather than guessed.
        if 'out' in dirs and 'in' in dirs:
            kind, note = 'inout ', ' // BOTH directions seen across boards'
        elif 'out' in dirs:
            kind, note = 'output', ''
        else:
            kind, note = 'input ', ''
        nm = names.get(pin)
        label = f'  // {nm}' if nm else '  // (no name in EclDict)'
        decl.append(f'    {kind} wire p{pin}{",":<1}{label}{note}')
    # last entry must not carry a comma
    if decl:
        decl[-1] = decl[-1].replace(',  //', '   //', 1)
    L.extend(decl)
    A(');')
    A('')
    A('  // TODO: model this part.')
    A('endmodule')
    A('')
    A('`default_nettype wire')
    return '\n'.join(L) + '\n'


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--sil', default='chm/sil')
    ap.add_argument('--dict', required=True, action='append',
                    help='EclDict.Analyze; repeat for TtlDict.Analyze '
                         '(the ECL dictionary\'s own header points at it: '
                         '"get = TtlDict.Analyze ... if you have TTL logic")')
    ap.add_argument('--out', default='verilog/cells')
    ap.add_argument('--force', action='store_true')
    ap.add_argument('--top', type=int, default=0,
                    help='only the N most-used parts (0 = all)')
    args = ap.parse_args(argv[1:])

    ecl = EclDict()
    for d in args.dict:
        ecl.load(d)
    usage, pins = collect(args.sil)
    os.makedirs(args.out, exist_ok=True)

    order = [t for t, _ in usage.most_common()]
    if args.top:
        order = order[:args.top]

    wrote = skipped = 0
    no_dict = []
    for part in order:
        path = os.path.join(args.out, f'cell_{vpart(part)}.v')
        if os.path.exists(path) and not args.force:
            skipped += 1
            continue
        names = ecl.pin_names(part)
        if not names:
            no_dict.append(part)
        with open(path, 'w') as fh:
            fh.write(emit_cell(part, usage[part], pins.get(part, {}), names))
        wrote += 1

    print(f'wrote {wrote} cell skeletons into {args.out}'
          f'{f", kept {skipped} existing" if skipped else ""}')
    covered = sum(usage[t] for t in order)
    total = sum(usage.values())
    print(f'covers {covered}/{total} logic packages '
          f'({100.0*covered/total:.1f}%) in {len(order)} part types')
    if no_dict:
        print(f'\n{len(no_dict)} part(s) absent from EclDict -- ports come from')
        print(f'the wire lists alone, so review these against a datasheet:')
        for p in no_dict[:20]:
            print(f'  {p:<16} {usage[p]:>4} packages')
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
