#!/usr/bin/env python3
"""Read PARC's Sil design-automation output for the Dorado boards.

`chm/sil/` holds the output of Xerox's own design-automation tools for all
sixteen boards -- the input the machine was actually built from, not a
reconstruction. Two files per board carry the whole structural design:

  <board>.wl   the WIRE LIST: every net by name, and every package pin it
               touches, WITH DIRECTION -- `f17.15o` drives, `g17.12i`
               receives. This is a gate-level netlist in plain text.
  <board>.lc   the LOCATION/part list: which part type sits in each package
               position (`MC10173/16/E1G:` then `a12 a13 b05 ...`).

Together they give a complete structural netlist: parts, pins, directions and
connectivity. That is the input to Phase 2 (Verilog); see
docs/verilog-from-sil.md.

FILE FORMAT, as observed (there is no surviving format document):

  * The files are **CR-terminated** (Xerox), not LF. Split on \\r.
  * `;`-prefixed lines are comments -- a per-page provenance header
    (`;File=ProcH01.sil Rev=Ce Date=11/15/78 Page=01 MARKED BUILT`).
  * A lone `L` line appears at the top; it is not data.
  * The PACKAGE section lists one package per line:
        a01: (MC10102/16/E) ; 5,7,9,10,11,12,13,15
    i.e. position, (partType/pinCount/variant), then a pin list. The trailing
    list is NOT connectivity -- it repeats for identical spare sockets and is
    recorded here as `note_pins` rather than guessed at.
  * The NET section is a record per net:
        Ain.00: <406> (93)
            c41.6i {89,34}   c02.11i {89,52}   b03.4i {61,58}   b05.1o {73,100}
    header = `NAME: <wireLength> (netId)`, then indented pin references
    `pkg.pin{i|o} {x,y}` where x,y is the physical position on the board.
  * Some sections are not nets at all but manufacturing instructions --
    `DISCONNECT:`, `CALIBRATE:` -- whose bodies are bare point names with no
    `.pin` and no direction. They are skipped by requiring the pin syntax.

Usage:
    sil_netlist.py <board.wl> [--lc <board.lc>] [--json OUT] [--summary]
    sil_netlist.py --all chm/sil            # summarise every board
"""

from __future__ import annotations

import argparse
import json
import os
import re
import sys
from collections import Counter

# `c41.6i {89,34}` -- package, pin number, direction, and board position.
PIN_RE = re.compile(r'\b([a-z]+\d+)\.(\d+)([io])\b(?:\s*\{(-?\d+),(-?\d+)\})?')
# `Ain.00: <406> (93)` -- net name, total wire length, net id.
NET_HEAD_RE = re.compile(r'^(\S+):\s*<(\d+)>\s*(?:\((\d+)\))?\s*$')
# `a01: (MC10102/16/E) ; 5,7,9,10`
PKG_RE = re.compile(r'^([a-z]+\d+):\s*\(([^)]*)\)\s*;\s*(.*)$')
# `MC10173/16/E1G:` heading in the .lc file
LC_HEAD_RE = re.compile(r'^(\S+/\S+/\S+):\s*$')


def vpart(part_type: str) -> str:
    """Part type -> a legal Verilog module-name suffix.

    Real part numbers are not identifiers: `LM3911+20K`, `MK4096P-6`,
    `SIP package`. Sanitise once, here, so the generator and the cell emitter
    cannot disagree about what a cell is called -- a mismatch shows up as a
    missing module at elaboration, long after the point of the mistake."""
    s = re.sub(r'[^A-Za-z0-9_]', '_', part_type)
    if not s or s[0].isdigit():
        s = 'p_' + s
    return s


def read_xerox_text(path: str) -> list[str]:
    """Xerox files are CR-terminated; normalise to a list of lines."""
    with open(path, 'rb') as fh:
        raw = fh.read().decode('latin-1')
    return raw.replace('\r\n', '\n').replace('\r', '\n').split('\n')


class Board:
    def __init__(self, name: str):
        self.name = name
        self.packages: dict[str, dict] = {}   # pos -> {type,pins,variant,...}
        self.nets: dict[str, dict] = {}       # net name -> {id,length,pins[]}
        self.provenance: list[str] = []
        self.skipped_sections: list[str] = []

    # ---- .wl -----------------------------------------------------------
    def load_wl(self, path: str) -> None:
        lines = read_xerox_text(path)
        net = None
        for line in lines:
            if not line.strip():
                continue
            if line.startswith(';'):
                self.provenance.append(line[1:].strip())
                continue
            if line.strip() == 'L':
                continue

            indented = line[0].isspace()

            if not indented:
                m = PKG_RE.match(line.strip())
                if m:
                    pos, desc, note = m.group(1), m.group(2), m.group(3)
                    bits = desc.split('/')
                    self.packages[pos] = {
                        'type': bits[0] if bits else desc,
                        'pin_count': int(bits[1]) if len(bits) > 1 and
                                     bits[1].isdigit() else None,
                        'variant': bits[2] if len(bits) > 2 else None,
                        'note_pins': [int(p) for p in note.replace(' ', '').split(',')
                                      if p.strip().isdigit()],
                    }
                    net = None
                    continue
                m = NET_HEAD_RE.match(line.strip())
                if m:
                    name = m.group(1)
                    net = {'id': int(m.group(3)) if m.group(3) else None,
                           'length': int(m.group(2)), 'pins': []}
                    self.nets[name] = net
                    continue
                # A section that is not a net: DISCONNECT, CALIBRATE, ...
                if line.rstrip().endswith(':') or ': <' in line or ':' in line:
                    label = line.split(':', 1)[0].strip()
                    if label and label not in self.skipped_sections:
                        self.skipped_sections.append(label)
                net = None
                continue

            # Indented: pin references for the current net.
            if net is None:
                continue
            for m in PIN_RE.finditer(line):
                pkg, pin, dirn, x, y = m.groups()
                net['pins'].append({
                    'pkg': pkg,
                    'pin': int(pin),
                    'dir': 'out' if dirn == 'o' else 'in',
                    'xy': [int(x), int(y)] if x is not None else None,
                })

    # ---- .lc -----------------------------------------------------------
    def load_lc(self, path: str) -> None:
        """Part type -> positions. Confirms/extends what .wl already said."""
        lines = read_xerox_text(path)
        cur = None
        for line in lines:
            if not line.strip():
                continue
            m = LC_HEAD_RE.match(line.strip())
            if m:
                cur = m.group(1)
                continue
            if cur and line[0].isspace():
                for pos in line.split():
                    bits = cur.split('/')
                    entry = self.packages.setdefault(pos, {})
                    entry.setdefault('type', bits[0])
                    entry.setdefault('pin_count',
                                     int(bits[1]) if len(bits) > 1 and
                                     bits[1].isdigit() else None)
                    entry.setdefault('variant', bits[2] if len(bits) > 2 else None)
                    entry['lc_type'] = cur

    # ---- derived -------------------------------------------------------
    def drivers_of(self, net: str) -> list[dict]:
        return [p for p in self.nets[net]['pins'] if p['dir'] == 'out']

    def check(self) -> dict:
        """Structural sanity, reported rather than asserted -- a 1979 board
        legitimately has open-emitter wired-OR nets with several drivers."""
        multi, undriven, dangling = [], [], []
        for name, net in self.nets.items():
            outs = sum(1 for p in net['pins'] if p['dir'] == 'out')
            if outs == 0:
                undriven.append(name)
            elif outs > 1:
                multi.append((name, outs))
            if len(net['pins']) < 2:
                dangling.append(name)
        used = {p['pkg'] for n in self.nets.values() for p in n['pins']}
        return {
            'nets': len(self.nets),
            'packages': len(self.packages),
            'pin_refs': sum(len(n['pins']) for n in self.nets.values()),
            'multi_driver_nets': sorted(multi, key=lambda t: -t[1])[:20],
            'multi_driver_count': len(multi),
            'undriven_nets': len(undriven),
            'single_pin_nets': len(dangling),
            'packages_with_no_net': sorted(set(self.packages) - used)[:20],
            'part_types': Counter(p.get('type', '?')
                                  for p in self.packages.values()),
        }

    def to_dict(self) -> dict:
        return {'board': self.name, 'packages': self.packages,
                'nets': self.nets, 'skipped_sections': self.skipped_sections}


def load_board(wl_path: str, lc_path: str | None = None) -> Board:
    name = os.path.basename(wl_path).split('.')[0]
    b = Board(name)
    b.load_wl(wl_path)
    if lc_path is None:
        guess = wl_path[:-3] + '.lc'
        if os.path.exists(guess):
            lc_path = guess
    if lc_path and os.path.exists(lc_path):
        b.load_lc(lc_path)
    return b


def find_boards(root: str) -> list[tuple[str, str | None]]:
    out = []
    for dirpath, _dirs, files in os.walk(root):
        for f in sorted(files):
            if f.endswith('.wl'):
                wl = os.path.join(dirpath, f)
                lc = wl[:-3] + '.lc'
                out.append((wl, lc if os.path.exists(lc) else None))
    return sorted(out)


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('wl', nargs='?', help='board .wl wire list')
    ap.add_argument('--lc', help='board .lc part list (default: alongside)')
    ap.add_argument('--json', help='write the parsed netlist as JSON')
    ap.add_argument('--summary', action='store_true')
    ap.add_argument('--all', metavar='SILDIR', help='summarise every board')
    ap.add_argument('--net', help='print one net in full')
    args = ap.parse_args(argv[1:])

    if args.all:
        boards = find_boards(args.all)
        if not boards:
            print(f'no .wl files under {args.all}', file=sys.stderr)
            return 1
        tot_n = tot_p = tot_r = 0
        types: Counter = Counter()
        print(f'{"board":<24} {"nets":>6} {"pkgs":>6} {"pinrefs":>8} '
              f'{"multi-drv":>10} {"undriven":>9}')
        for wl, lc in boards:
            b = load_board(wl, lc)
            c = b.check()
            tot_n += c['nets']; tot_p += c['packages']; tot_r += c['pin_refs']
            types.update(c['part_types'])
            print(f'{b.name:<24} {c["nets"]:>6} {c["packages"]:>6} '
                  f'{c["pin_refs"]:>8} {c["multi_driver_count"]:>10} '
                  f'{c["undriven_nets"]:>9}')
        print(f'{"TOTAL":<24} {tot_n:>6} {tot_p:>6} {tot_r:>8}')
        print(f'\ndistinct part types across all boards: {len(types)}')
        real = {t: n for t, n in types.items()
                if not t.startswith(('SpareSocket', 'Term'))}
        print(f'excluding spares/terminators:          {len(real)} types, '
              f'{sum(real.values())} packages')
        print('\nmost common:')
        for t, n in types.most_common(15):
            print(f'  {t:<20} {n:>5}')
        return 0

    if not args.wl:
        ap.print_help()
        return 1

    b = load_board(args.wl, args.lc)

    if args.net:
        net = b.nets.get(args.net)
        if not net:
            print(f'no net {args.net!r}', file=sys.stderr)
            return 1
        print(f'{args.net}: id={net["id"]} length={net["length"]} '
              f'pins={len(net["pins"])}')
        for p in net['pins']:
            pkg = b.packages.get(p['pkg'], {})
            print(f'  {p["pkg"]}.{p["pin"]:<3} {p["dir"]:<3} '
                  f'{pkg.get("type","?"):<16} at {p["xy"]}')
        return 0

    if args.json:
        with open(args.json, 'w') as fh:
            json.dump(b.to_dict(), fh, indent=1)
        print(f'wrote {args.json}')

    if args.summary or not args.json:
        c = b.check()
        print(f'board {b.name}')
        print(f'  nets                {c["nets"]}')
        print(f'  packages            {c["packages"]}')
        print(f'  pin references      {c["pin_refs"]}')
        print(f'  nets with >1 driver {c["multi_driver_count"]}  '
              f'(wired-OR is legal in MECL 10K)')
        print(f'  nets with 0 drivers {c["undriven_nets"]}  '
              f'(board inputs arrive on backplane pins)')
        print(f'  single-pin nets     {c["single_pin_nets"]}')
        if b.skipped_sections:
            print(f'  non-net sections    {", ".join(b.skipped_sections)}')
        print(f'  distinct part types {len(c["part_types"])}')
        for t, n in c['part_types'].most_common(10):
            print(f'    {t:<20} {n:>4}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
