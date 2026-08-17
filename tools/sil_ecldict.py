#!/usr/bin/env python3
"""Read PARC's own ECL part dictionary (`EclDict.Analyze`).

This is the file Xerox's ANALYZE design-rule checker used to know what each
package IS -- so it is the authority on pin assignments, and means the cell
library does not have to be reconstructed from memory or from datasheets we
do not hold. Recovered at `chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze`
(K. Pier, 12-Sep-1978).

FORMAT (observed; CR-terminated like every Xerox text file):

  * `;` comments, then an ALIAS table mapping short names to the full
    `part/pins/variant` spelling used in the `.wl`/`.lc` files:

        MC164=MC10164/16/E> MU164=MU10164/16/E
        MC110=MC10110/16/E3G > MC210=MC10210/16/E3G > SE210=SE10210/16/E3G

    `>` separates SECOND-SOURCE equivalents: Motorola MC10210, and the
    Signetics SE10210 in the same socket, are the same part. That is why the
    boards mix MC/MU/SE/F prefixes for one function.

  POLARITY. The role letters DO track the sense, and the discriminator is a
  pair of parts with identical pin numbers and swapped letters:

      MC102 (Quad 2-input NOR)   a,IN,4,5 > a,OUT,2   ... d,OUT,15 > d,o,9
      MC103 (Quad 2-input OR)    a,IN,4,5 > a,o,2     ... c,OUT,9  > c,o,15

  Same pins, opposite functions, opposite letters. Working rule: **`o` is the
  non-inverting (OR) output and `OUT` the inverting (NOR) one** -- EXCEPT on
  parts with only one output sense (MC10110 OR and MC10111 NOR have identical
  all-`OUT` blocks), where the letter cannot discriminate and the PART NAME
  decides. See docs/verilog-handoff.md for what is and is not settled, and
  for the one existing cell this rule contradicts.

  * `@` introduces a pin-definition block:

        @
        MC101
        a,IN,4 > a,c,12 > a,OUT,2 > a,o,5
        b,IN,7 > b,OUT,3 > b,o,6

    One line per gate. Fields are `<gate letter>,<role>,<pins...>`, and `>`
    separates clauses for the SAME gate. Roles seen:
      IN   inputs to that gate
      OUT  its output           (see the polarity note below)
      o    its second output    (see the polarity note below)
      c    a COMMON input, shared by every gate in the package (e.g. a clock
           or an enable), given once per gate that uses it
    A header naming several parts (`MC102, MC104`) means they share pinout.

What this gives us: for every package on every board, which pins are inputs,
which are outputs, and how they group into gates -- pin-accurate, from the
source. What it does NOT give is the BOOLEAN FUNCTION; that comes from the
part number (MC10100 is a quad 2-input NOR, and so on) and belongs in the
cell library, where each model cites its part.

Usage:
    sil_ecldict.py DICT [--part MC10173] [--json OUT] [--summary]
"""

from __future__ import annotations

import argparse
import json
import re
import sys


def read_xerox_text(path: str) -> list[str]:
    with open(path, 'rb') as fh:
        raw = fh.read().decode('latin-1')
    return raw.replace('\r\n', '\n').replace('\r', '\n').split('\n')


class EclDict:
    def __init__(self) -> None:
        self.alias: dict[str, str] = {}      # short name -> full spelling
        self.equiv: dict[str, list[str]] = {}  # canonical -> second sources
        self.parts: dict[str, dict] = {}     # short name -> {gates: {...}}
        # Short names that carry a bracketed behaviour summary -- `[G ...]`
        # gates, `[FF ...]`/`[L ...]` clocked banks, `[M ...]` memories. A part
        # WITHOUT one is not digital: the MC10318 on DispM is a D/A converter,
        # and its output is a voltage, not a level.
        self.behavioural: set[str] = set()

    def load(self, path: str) -> None:
        lines = read_xerox_text(path)
        in_pins = False
        in_summaries = False
        cur: list[str] = []
        for raw in lines:
            line = raw.strip()
            if not line or line.startswith(';'):
                continue
            if line == '@':
                in_pins = True
                cur = []
                continue
            # A THIRD SECTION, which this loader used to walk straight past.
            # After `#` the file lists each part's behaviour summaries -- a
            # bare name line, then its `[G ...]` / `[FF ...]` / `[M ...]`
            # lines. Without noticing the marker, `cur` stayed on whatever pin
            # block came last and every summary in the file was attributed to
            # that one part.
            if line == '#':
                in_pins = True
                in_summaries = True
                cur = []
                continue
            if in_summaries:
                if line.startswith('['):
                    self.behavioural.update(cur)
                elif re.match(r'^[A-Za-z0-9_, +.\-]+$', line):
                    cur = [p.strip() for p in line.split(',') if p.strip()]
                continue
            if not in_pins:
                # alias table; `>` separates second-source equivalents
                if '=' in line:
                    group = [c.strip() for c in line.split('>') if c.strip()]
                    names = []
                    for clause in group:
                        if '=' not in clause:
                            continue
                        short, full = clause.split('=', 1)
                        short, full = short.strip(), full.strip()
                        self.alias[short] = full
                        names.append(short)
                    if len(names) > 1:
                        for n in names:
                            self.equiv[n] = [m for m in names if m != n]
                continue

            # inside a pin block: first line names the part(s)
            if not cur:
                cur = [p.strip() for p in line.split(',') if p.strip()]
                for name in cur:
                    self.parts.setdefault(name, {'gates': {}, 'shared_with':
                                                 [n for n in cur if n != name]})
                continue

            # A bracketed summary inside a pin block: record it and move on
            # rather than parsing it as pin clauses, which invented gates
            # named after pin numbers.
            if line.startswith('['):
                self.behavioural.update(cur)
                continue

            # gate clauses
            for clause in line.split('>'):
                bits = [b.strip() for b in clause.split(',') if b.strip()]
                if len(bits) < 2:
                    continue
                gate, role = bits[0], bits[1]
                pins = [int(b) for b in bits[2:] if b.isdigit()]
                for name in cur:
                    g = self.parts[name]['gates'].setdefault(
                        gate, {'in': [], 'out': [], 'nout': [], 'common': []})
                    if role == 'IN':
                        g['in'].extend(pins)
                    elif role == 'OUT':
                        g['out'].extend(pins)
                    elif role == 'o':
                        g['nout'].extend(pins)
                    elif role == 'c':
                        g['common'].extend(pins)
                    else:                       # unknown role: keep it visible
                        g.setdefault('other', []).append((role, pins))

    # -- lookups ---------------------------------------------------------
    def full_name(self, short: str) -> str | None:
        return self.alias.get(short)

    def by_full_type(self, part_type: str) -> tuple[str, dict] | None:
        """`.wl` gives `MC10173`; the dictionary keys on `MC173`. Match on the
        full spelling's leading part field."""
        for short, full in self.alias.items():
            if full.split('/')[0] == part_type and short in self.parts:
                return short, self.parts[short]
        # second sources share a pin block under the canonical short name
        for short, full in self.alias.items():
            if full.split('/')[0] == part_type:
                for eq in self.equiv.get(short, []):
                    if eq in self.parts:
                        return eq, self.parts[eq]
        return None

    def pin_roles(self, part_type: str) -> dict[int, str]:
        """pin number -> 'in' | 'out' | 'nout' | 'common', for one part."""
        got = self.by_full_type(part_type)
        if not got:
            return {}
        roles: dict[int, str] = {}
        for g in got[1]['gates'].values():
            for r in ('in', 'out', 'nout', 'common'):
                for p in g.get(r, []):
                    roles[p] = r
            for nm, pins in g.get('other', []):
                for p in pins:
                    roles.setdefault(p, 'named')
        return roles

    def pin_names(self, part_type: str) -> dict[int, str]:
        """pin number -> the DATASHEET signal name, where the dictionary
        gives one.

        Simple gates are described positionally (`a,IN,4,5 > a,OUT,2`), so
        their pins get synthesised names like `a_IN0`, `a_OUT`. Complex parts
        are described by signal name instead (MC10173: `a,D0,5 > a,Q0,1`),
        and those names are the datasheet's own -- which is what makes a
        generated cell port list reviewable against the part.

        Direction is deliberately NOT taken from here: the `.wl` marks every
        pin `i` or `o` per INSTANCE, which is both authoritative and finer
        grained (a part pin can be an output on one board and unused on
        another)."""
        got = self.by_full_type(part_type)
        if not got:
            return {}
        names: dict[int, str] = {}
        for gate, g in got[1]['gates'].items():
            for i, p in enumerate(g.get('in', [])):
                names.setdefault(p, f'{gate}_IN{i}')
            for p in g.get('out', []):
                names.setdefault(p, f'{gate}_OUT')
            for p in g.get('nout', []):
                names.setdefault(p, f'{gate}_OUTN')
            for p in g.get('common', []):
                names.setdefault(p, f'{gate}_C')
            for nm, pins in g.get('other', []):
                for p in pins:
                    names.setdefault(p, nm)
        return names


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('dict', help='EclDict.Analyze')
    ap.add_argument('--part', help='show one part (short or full name)')
    ap.add_argument('--json', help='dump the parsed dictionary')
    ap.add_argument('--summary', action='store_true')
    args = ap.parse_args(argv[1:])

    d = EclDict()
    d.load(args.dict)

    if args.part:
        p = args.part
        got = d.parts.get(p) or (d.by_full_type(p)[1] if d.by_full_type(p) else None)
        short = p if p in d.parts else (d.by_full_type(p) or ('?', None))[0]
        if not got:
            print(f'no part {p!r} in the dictionary', file=sys.stderr)
            return 1
        print(f'{short}  ({d.alias.get(short, "?")})')
        if d.equiv.get(short):
            print(f'  second sources: {", ".join(d.equiv[short])}')
        if got.get('shared_with'):
            print(f'  same pinout as: {", ".join(got["shared_with"])}')
        for gate, g in sorted(got['gates'].items()):
            parts = []
            if g['in']:     parts.append(f'in={g["in"]}')
            if g['common']: parts.append(f'common={g["common"]}')
            if g['out']:    parts.append(f'out={g["out"]}')
            if g['nout']:   parts.append(f"out'={g['nout']}")
            if g.get('other'): parts.append(f'other={g["other"]}')
            print(f'  gate {gate}: ' + '  '.join(parts))
        return 0

    if args.json:
        with open(args.json, 'w') as fh:
            json.dump({'alias': d.alias, 'equiv': d.equiv, 'parts': d.parts},
                      fh, indent=1)
        print(f'wrote {args.json}')

    if args.summary or not args.json:
        print(f'aliases      {len(d.alias)}')
        print(f'pin blocks   {len(d.parts)}')
        multi = sum(1 for v in d.equiv.values() if v)
        print(f'parts with second sources  {multi}')
        gated = sum(len(p["gates"]) for p in d.parts.values())
        print(f'gates defined {gated}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
