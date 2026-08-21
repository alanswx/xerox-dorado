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
# A backplane connector pin on a net line: `E179`, `C96`. Package positions are
# lowercase (PKG_RE) and coordinates carry no letters, so this cannot collide.
CONN_RE = re.compile(r'\b[CE]\d+\b')
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


# SIX BACKPLANE LINES ARE SPELLED TWO WAYS, and one of them is the memory
# section's hold on the processor.
#
# PARC's draughtsmen were not consistent about capitalisation, and this
# backplane is wired BY NAME -- it is not straight-through, 182 pin positions
# carry different nets on different boards -- so a spelling difference silently
# leaves a line unconnected. `PrHold` on MemC and `PRhold` on ProcH/ProcL are
# ONE WIRE: `#07-E.42`, `#s05-E.42`, `#s04-E.42`, same connector and pin in
# three slots. Until this, MemC drove `PrHold` into nothing and the processor
# read `PRhold` from nothing, so the memory section could not hold the
# processor at all.
#
# CASE-INSENSITIVE MATCHING WOULD BE WRONG, which is why this is a table.
# Among backplane nets there are nine case-variant groups and THREE sit on
# DIFFERENT pins -- `CLKEnable'a` (C16) vs `ClkEnable'a` (C8), `IOIn'` (E70) vs
# `IOin'` (E71), `IOOut'` (E71) vs `IOout'` (E74). Those are separate lines
# that merely look alike. Outside the backplane it would be far worse: 63 net
# names differ only by case, mostly per-board LOCAL clock fan-out such as
# `Clk0'Aa` on MemX against `clk0'Aa` on IFU, and merging those would tie every
# board's clock distribution together.
#
# So the rule is narrow: merge a case variant ONLY where every board that uses
# it agrees on the pin. That yields exactly these six. No board carries both
# spellings, so the rename cannot collide with a local net.
# `tools/sil_backplane.py --case-variants` re-derives the list from the .bp
# files and fails if it no longer matches.
BACKPLANE_CASE_ALIASES = {
    'PrHold':        'PRhold',        # E42  MemC -> ProcH, ProcL
    'MxHold':        'MXHold',        #      MemX <-> MemC
    'HoldMapbuf':    'HoldMapBuf',    #      MemX <-> MemC
    'Subtask.0':     'SubTask.0',     #      MemX <-> ProcL, DispY
    'Subtask.1':     'SubTask.1',     #      MemX <-> ProcL
    'FoutSubtask.0': 'FoutSubTask.0', #      MemX <-> DispY
}


def canon_net(name: str) -> str:
    """The canonical spelling of a backplane net -- see BACKPLANE_CASE_ALIASES."""
    return BACKPLANE_CASE_ALIASES.get(name, name)


class Board:
    def __init__(self, name: str):
        self.name = name
        self.packages: dict[str, dict] = {}   # pos -> {type,pins,variant,...}
        self.nets: dict[str, dict] = {}       # net name -> {id,length,pins[]}
        self.provenance: list[str] = []
        self.skipped_sections: list[str] = []
        # net -> {'C96','E179'}: the backplane connector pins it reaches, i.e.
        # exactly the nets that leave the board. See load_bp.
        self.backplane: dict[str, set[str]] = {}
        self.bp_source = 'wl'
        self.bp_mismatch: list[tuple[str, set, set]] = []

    # ---- .wl -----------------------------------------------------------

    def load_wl(self, path: str) -> None:
        lines = read_xerox_text(path)
        net = None
        net_name = None
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
                    net_name = canon_net(m.group(1))
                    net = {'id': int(m.group(3)) if m.group(3) else None,
                           'length': int(m.group(2)), 'pins': []}
                    self.nets[net_name] = net
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
            # A bare `E179 {457,525}` among the pins is a BACKPLANE CONNECTOR
            # pin, not a package: the net leaves the board there. Package
            # names are lowercase (PKG_RE), so an uppercase C/E followed by
            # digits is unambiguous. These used to be dropped, and the
            # generator inferred the same fact from Term100 packages instead
            # -- which is wrong twice over, because Term100 is a 100-ohm
            # TERMINATOR, not a connector.
            for m in CONN_RE.finditer(line):
                net.setdefault('backplane', set()).add(m.group(0))
                self.backplane.setdefault(net_name, set()).add(m.group(0))

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

    # ---- .bp -----------------------------------------------------------
    def load_bp(self, path: str) -> None:
        """The BACKPLANE file: `ALUCarry: E179`, one line per net that leaves
        the board. This is the module's port list, stated by the tool that
        built the machine rather than inferred from the wiring.

        The same fact is stated THREE times in the archive and all three
        agree: here, as the bare `E179` tokens in the `.wl` (parsed above),
        and slot-qualified in `-C.nl`/`-E.nl` (`#s05-C.5`). Measured across
        all sixteen boards, 2,052 of 2,054 pins are identical; the five
        exceptions are ground nets that the `.wl` numbers individually
        (`GND-26`) and the `.bp` collapses (`GND`).

        `.bp` wins where they differ because it is the file whose only job is
        this, but any disagreement is recorded in `bp_mismatch` rather than
        hidden -- if these two ever diverge on a signal, something is wrong
        with an assumption, not with the machine."""
        from_wl = {n: set(v) for n, v in self.backplane.items()}
        from_bp: dict[str, set[str]] = {}
        for line in read_xerox_text(path):
            if ':' not in line:
                continue
            name, pins = line.split(':', 1)
            name = canon_net(name.strip())
            name = name.strip()
            if not name:
                continue
            from_bp[name] = {p.strip() for p in pins.split(',') if p.strip()}
        for name in sorted(set(from_wl) | set(from_bp)):
            if from_wl.get(name, set()) != from_bp.get(name, set()):
                self.bp_mismatch.append((name, from_wl.get(name, set()),
                                         from_bp.get(name, set())))
        self.backplane = from_bp
        self.bp_source = 'bp'

    # ---- derived -------------------------------------------------------
    def drivers_of(self, net: str) -> list[dict]:
        return [p for p in self.nets[net]['pins'] if p['dir'] == 'out']

    def leaves_board(self, net: str) -> bool:
        """Does this net reach a backplane connector?"""
        return net in self.backplane

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


def load_board(wl_path: str, lc_path: str | None = None,
               bp_path: str | None = None) -> Board:
    name = os.path.basename(wl_path).split('.')[0]
    b = Board(name)
    b.load_wl(wl_path)
    if lc_path is None:
        guess = wl_path[:-3] + '.lc'
        if os.path.exists(guess):
            lc_path = guess
    if lc_path and os.path.exists(lc_path):
        b.load_lc(lc_path)
    # The .bp sits beside the .wl, except on msa where the board directory is
    # `msa-Rev-Bg` but the files are plain `msa.*`; glob rather than assume.
    if bp_path is None:
        guess = wl_path[:-3] + '.bp'
        if not os.path.exists(guess):
            d = os.path.dirname(wl_path)
            cand = [f for f in sorted(os.listdir(d)) if f.endswith('.bp')]
            guess = os.path.join(d, cand[0]) if cand else None
        bp_path = guess
    if bp_path and os.path.exists(bp_path):
        b.load_bp(bp_path)
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
