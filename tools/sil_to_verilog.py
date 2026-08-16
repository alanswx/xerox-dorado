#!/usr/bin/env python3
"""Generate structural Verilog for a Dorado board from PARC's own Sil output.

Inputs, all of them original Xerox data:
  <board>.wl          the wire list  (nets, package pins, directions)
  <board>.lc          the part list  (part type per package position)
  EclDict.Analyze     the ANALYZE part dictionary (pin roles per part)

Output: one Verilog module per board, instantiating a cell per package and
wiring them with the board's own net names. The cell MODELS live in
`verilog/cells/` and are hand-written per part number; this tool never invents
behaviour, it only places and wires what the netlist says.

Why this shape (see docs/verilog-from-sil.md): the `.wl` gives the direction
of every pin, so the generator can emit real `input`/`output` ports rather
than guessing, and the board's own net names survive into the RTL -- which is
what makes the result diffable against the schematics and against the C
emulator's behaviour.

THREE THINGS THE GENERATOR WILL NOT DO SILENTLY, because each would produce
plausible-looking wrong RTL:

  * **Wired-OR.** MECL 10K open-emitter outputs are legitimately tied
    together (91 such nets on ProcH alone). Verilog `wire` with multiple
    drivers is a multi-driver error, so those nets are emitted as an explicit
    OR of their drivers, and every one is listed in the report.
  * **Missing cells.** A package whose part has no model is emitted as a
    named stub instance with its real ports, and counted. The design does not
    quietly lose logic.
  * **Undriven nets.** A net that does NOT reach a backplane connector and
    has no driver here is left undriven, so simulation reads X rather than a
    plausible zero. It means a cell model is still a skeleton, and the count
    is in the report. It is NOT quietly promoted to a module port -- the port
    list is stated by `<board>.bp`, not inferred from what happens to be
    driven.

Usage:
    sil_to_verilog.py <board.wl> --dict EclDict.Analyze [-o out.v] [--report]
"""

from __future__ import annotations

import argparse
import os
import re
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sil_netlist import load_board, vpart   # noqa: E402
from sil_ecldict import EclDict             # noqa: E402
from dorado_proms import placement, PROM_PARTS   # noqa: E402


# Sil punctuation -> escape. INJECTIVE on purpose: `CTask.0` and `CTask=0`
# are DIFFERENT nets on ContA, and a lossy mapping silently merged them into
# one wire. Verilog caught that as a duplicate declaration, but only by luck --
# had the two names not both been declared, the board would have elaborated
# with two signals shorted together and no diagnostic at all.
#
# `_` is escaped first so no escape sequence can be produced another way; the
# rest stay readable, because being able to grep the RTL for the schematic's
# own signal name is most of the value of generating it this way.
_ESCAPES = [('_', '_u_'), ('.', '_'), ("'", '_p_'), ('=', '_eq_'),
            ('-', '_m_'), ('#', '_h_'), ('/', '_s_'), ('+', '_pl_')]

# Where the per-package PROM images live, repo-relative.
PROM_DIR = 'verilog/proms/packages'


def vname(name: str) -> str:
    """A Sil net or package name -> a legal, UNIQUE Verilog identifier."""
    s = name
    for ch, esc in _ESCAPES:
        s = s.replace(ch, esc)
    s = re.sub(r'[^A-Za-z0-9_]', lambda m: '_x%02x_' % ord(m.group(0)), s)
    if not s or s[0].isdigit():
        s = 'n_' + s
    return s


class Generator:
    def __init__(self, board, ecl: EclDict, cells: set[str],
                 cell_dirs: dict | None = None):
        self.b = board
        self.ecl = ecl
        self.cells = cells
        self.cell_dirs = cell_dirs or {}
        self.missing: Counter = Counter()
        self.wired_or: list[tuple[str, int]] = []
        self.ports: dict[str, str] = {}      # net -> input|output|inout
        self.undriven_internal: list[str] = []
        self.unknown_pins: Counter = Counter()
        # package position -> the .mem holding that PROM's contents. PARC's
        # own PromCommand labels say which package each PROM is blown into;
        # tools/dorado_proms.py resolves them and slices the word per part.
        short = board.name.split('-Rev')[0].split('.')[0]
        self.prom_image = {
            r['pos']: f'{PROM_DIR}/{r["board"]}-{r["pos"]}.mem'
            for r in placement() if r['board'] == short
        }
        self.proms_wired = 0

    # Neither a driver nor a consumer. `Term100` is a 100-ohm TERMINATING
    # RESISTOR network -- ECL terminates every line -- and an empty socket is
    # nothing at all. Reading a terminator as the board's connector is what
    # made the old port inference wrong.
    _NOT_LOGIC = ('Term', 'SpareSocket')

    def _rtl_dir(self, p: dict) -> str | None:
        """What the EMITTED RTL does at this pin, or None if nothing.

        The CELL's port direction is the truth here, because the cell is what
        gets instantiated -- and it is global, aggregated over all sixteen
        boards, so it can differ from this board's wire-list direction. Where
        the cell says nothing about the pin, fall back to the wire list."""
        ptype = self.b.packages.get(p['pkg'], {}).get('type', '')
        if ptype.startswith(self._NOT_LOGIC):
            return None
        d = self.cell_dirs.get(vpart(ptype), {}).get(p['pin'])
        if d:
            return d
        return 'output' if p['dir'] == 'out' else 'input'

    def drivers_in_rtl(self, name: str) -> list[dict]:
        """The pins that actually DRIVE this net in the emitted RTL.

        THE ONE definition, used by classify() for port direction and by
        emit() for wired-OR resolution. It has to be one function: when the
        two decided separately -- emit() from the wire list's `o` pins,
        classify() from the cell's port directions -- they disagreed on five
        nets and the generator assigned to a module input (`%Error-ASSIGNIN`),
        and on eight more it left BaseBd's `MCD_0..7` with two continuous
        drivers, an `assign` racing a cell output."""
        return [p for p in self.b.nets[name]['pins']
                if self._rtl_dir(p) in ('output', 'inout')]

    def classify(self) -> None:
        """The ports are the nets PARC says leave the board, and nothing else.

        `<Board>.bp` lists exactly those, one per line (`ALUCarry: E179`), and
        the same fact is stated twice more -- as bare `E179` tokens in the
        `.wl` and slot-qualified in `-C.nl`/`-E.nl` -- with all three agreeing
        on 2,052 of 2,054 pins. `sil_netlist.load_bp` reconciles them.

        This replaces an INFERENCE that was wrong in both directions: ports
        were taken to be nets with no local driver, plus nets whose only
        consumers were `Term100` packages. Measured against the `.bp` files
        that missed 703 backplane nets across the sixteen boards, emitting
        them as internal wires -- so those signals could never have reached
        another board -- while inventing 833 ports that are not on the
        backplane at all.

        Direction comes from the wire list, since `.bp` does not state it:

          drives and senses -> `inout`. The board contributes to the net AND
            reads it back, which is one bidirectional connection on the real
            board and must stay one here. 115 backplane nets are driven by
            more than one board -- MECL open emitters wired together, the B
            bus among them -- and a board that declared `output` would read
            back only its own contribution instead of the bus. The top level
            resolves these as `wor`.
          drives only  -> `output`
          neither      -> `input` (including a line the board only terminates)
        """
        for name, net in self.b.nets.items():
            drv = self.drivers_in_rtl(name)
            if len(drv) > 1:                   # on-board wired-OR
                self.wired_or.append((name, len(drv)))

            drives = bool(drv)
            senses = any(self._rtl_dir(p) == 'input' for p in net['pins'])

            if not self.b.leaves_board(name):
                # Stays on the board. If nothing drives it, that is a REAL
                # gap -- the part that should drive it is a cell skeleton with
                # no behaviour -- and it is left undriven so simulation reads
                # X there rather than a plausible zero. Counted in the report.
                if not drives:
                    self.undriven_internal.append(name)
                continue

            self.ports[name] = ('inout' if (drives and senses) else
                                'output' if drives else 'input')

    def emit(self) -> str:
        self.classify()
        mod = vname(self.b.name)
        out: list[str] = []
        A = out.append

        A(f'// Generated by tools/sil_to_verilog.py from PARC\'s own Sil output.')
        A(f'// Board: {self.b.name}')
        A(f'// Source: {self.b.name}.wl (wire list) + .lc (parts)'
          f' + EclDict.Analyze (pin roles)')
        A(f'// DO NOT EDIT -- regenerate. Cell models live in verilog/cells/.')
        A('')
        A('`default_nettype none')
        A('')

        inputs = sorted(n for n, d in self.ports.items() if d == 'input')
        outputs = sorted(n for n, d in self.ports.items() if d == 'output')
        inouts = sorted(n for n, d in self.ports.items() if d == 'inout')
        A(f'// Ports: the {len(self.ports)} nets {self.b.name}.bp says reach a')
        A(f'// backplane connector -- PARC\'s own statement of this module\'s')
        A(f'// boundary, not an inference. An `inout` is a net this board both')
        A(f'// drives and senses: MECL open emitters are wired together across')
        A(f'// boards, so the top level must resolve those as `wor`.')
        A(f'module {mod} (')
        decl = [f'    input  wire {vname(n)}' for n in inputs] + \
               [f'    output wire {vname(n)}' for n in outputs] + \
               [f'    inout  wire {vname(n)}' for n in inouts]
        A(',\n'.join(decl) if decl else '    // no backplane ports detected')
        A(');')
        A('')

        internal = [n for n in sorted(self.b.nets) if n not in self.ports]
        A(f'  // {len(internal)} internal nets')
        for n in internal:
            A(f'  wire {vname(n)};')
        A('')

        # Which net does each (package,pin) belong to?
        pinnet: dict[tuple[str, int], str] = {}
        for name, net in self.b.nets.items():
            for p in net['pins']:
                pinnet[(p['pkg'], p['pin'])] = name

        # Wired-OR resolution
        if self.wired_or:
            A('  // ---- wired-OR nets (MECL 10K open emitters tied together)')
            A('  // Emitted as an explicit OR of the drivers; Verilog forbids')
            A('  // multiple continuous drivers on one wire.')
            for name, _n in self.wired_or:
                drivers = self.drivers_in_rtl(name)
                terms = ' | '.join(f'{vname(name)}__{vname(p["pkg"])}_{p["pin"]}'
                                   for p in drivers)
                for p in drivers:
                    A(f'  wire {vname(name)}__{vname(p["pkg"])}_{p["pin"]};')
                A(f'  assign {vname(name)} = {terms};')
            A('')

        A('  // ---- packages')
        placed = 0
        for pos in sorted(self.b.packages):
            pkg = self.b.packages[pos]
            ptype = pkg.get('type', '')
            if ptype.startswith(('SpareSocket', 'Term')):
                continue                      # not logic
            roles = self.ecl.pin_roles(ptype)
            conns = []
            for (pk, pin), netname in sorted(pinnet.items()):
                if pk != pos:
                    continue
                role = roles.get(pin)
                if role is None:
                    self.unknown_pins[ptype] += 1
                # a driver on a wired-OR net connects to its private stub
                wired = any(netname == w for w, _ in self.wired_or)
                isout = any(p['pkg'] == pos and p['pin'] == pin
                            for p in self.drivers_in_rtl(netname))
                target = (f'{vname(netname)}__{vname(pos)}_{pin}'
                          if (wired and isout) else vname(netname))
                conns.append(f'    .p{pin}({target})')
            cell = f'cell_{vpart(ptype)}'
            if ptype not in self.cells:
                self.missing[ptype] += 1
                A(f'  // NO MODEL for {ptype} -- stub, ports preserved')
            params = ''
            if ptype in PROM_PARTS and pos in self.prom_image:
                # A PROM package with contents PARC's own BCPL computes. The
                # path is repo-relative, so run the simulation from the repo
                # root; $readmemh fails loudly rather than silently if not.
                params = f'#(.INIT_FILE("{self.prom_image[pos]}")) '
                self.proms_wired += 1
            elif ptype in PROM_PARTS:
                A(f'  // PROM with no contents in the archive -- reads X')
            A(f'  {cell} {params}u_{vname(pos)} (')
            A(',\n'.join(conns) if conns else '    // no connections')
            A(f'  ); // {ptype}')
            placed += 1
        A('')
        A(f'endmodule')
        A('`default_nettype wire')
        self.placed = placed
        return '\n'.join(out) + '\n'

    def report(self) -> str:
        r = [f'board {self.b.name}',
             f'  packages placed     {self.placed}',
             f'  ports (from .{self.b.bp_source}) {len(self.ports)}'
             f'  in {sum(1 for d in self.ports.values() if d=="input")}'
             f' / out {sum(1 for d in self.ports.values() if d=="output")}'
             f' / inout {sum(1 for d in self.ports.values() if d=="inout")}',
             f'  PROM packages wired {self.proms_wired}',
             f'  on-board wired-OR   {len(self.wired_or)}',
             f'  undriven internal   {len(self.undriven_internal)} '
             f'(a cell skeleton owes these a driver)',
             f'  parts without model {len(self.missing)} '
             f'({sum(self.missing.values())} packages)']
        if self.missing:
            r.append('  missing cell models, most used first:')
            for t, n in self.missing.most_common(15):
                known = 'in EclDict' if self.ecl.by_full_type(t) else 'NOT in EclDict'
                r.append(f'    {t:<16} {n:>4}   ({known})')
        if self.unknown_pins:
            r.append('  pins with no role in EclDict (per part):')
            for t, n in self.unknown_pins.most_common(8):
                r.append(f'    {t:<16} {n:>4}')
        return '\n'.join(r)


def known_cells(cells_dir: str) -> set[str]:
    out = set()
    if not os.path.isdir(cells_dir):
        return out
    for f in os.listdir(cells_dir):
        m = re.match(r'cell_(.+)\.v$', f)
        if m:
            out.add(m.group(1))
    return out


CELL_PORT_RE = re.compile(r'^\s*(input|output|inout)\s+wire\s+p(\d+)')


def cell_port_dirs(cells_dir: str) -> dict[str, dict[int, str]]:
    """cell module suffix -> {pin: 'input'|'output'|'inout'}.

    The generator needs these because a cell's port direction is GLOBAL --
    aggregated over all sixteen boards -- while a net's classification is
    per-board. A part pin that drives on MemC and is merely sensed on IFU is
    `output` in the shared cell, so a board where nothing drives that net
    would otherwise declare it a module input and then connect an output to
    it: `%Error-ASSIGNIN`. Reading the cells back closes that loop."""
    out: dict[str, dict[int, str]] = {}
    if not os.path.isdir(cells_dir):
        return out
    for f in os.listdir(cells_dir):
        m = re.match(r'cell_(.+)\.v$', f)
        if not m:
            continue
        pins: dict[int, str] = {}
        with open(os.path.join(cells_dir, f)) as fh:
            for line in fh:
                pm = CELL_PORT_RE.match(line)
                if pm:
                    pins[int(pm.group(2))] = pm.group(1)
        out[m.group(1)] = pins
    return out


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('wl')
    ap.add_argument('--dict', required=True, help='EclDict.Analyze')
    ap.add_argument('--lc')
    ap.add_argument('--cells', default='verilog/cells')
    ap.add_argument('-o', '--out')
    ap.add_argument('--report', action='store_true')
    args = ap.parse_args(argv[1:])

    board = load_board(args.wl, args.lc)
    ecl = EclDict()
    ecl.load(args.dict)
    gen = Generator(board, ecl, known_cells(args.cells),
                    cell_port_dirs(args.cells))
    text = gen.emit()

    if args.out:
        os.makedirs(os.path.dirname(args.out) or '.', exist_ok=True)
        with open(args.out, 'w') as fh:
            fh.write(text)
        print(f'wrote {args.out}  ({text.count(chr(10))} lines)')
    else:
        sys.stdout.write(text)

    if args.report:
        print(gen.report(), file=sys.stderr)
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
