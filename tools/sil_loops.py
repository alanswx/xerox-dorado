#!/usr/bin/env python3
"""Find the combinational feedback loops in the generated RTL.

WHY THIS EXISTS. When the BaseBoard started genuinely running, the assembled
machine stopped converging: Verilator's settle loop never terminated, at 500
iterations as readily as at 100. Verilator names the symptom -- `UNOPTFLAT ...
Circular combinational logic` -- but it reports one signal per loop and an
"example path", and it reported the same 40 signals before and after the
machine came alive, so it could not say which mattered. The graph had to be
built directly.

WHAT IT FOUND, and the answer was not what the symptom suggested. The loops
were not the design's gate-delay tricks. They were two modelling mistakes,
both of which the archive itself settles:

  * SIX CELLS WERE TRANSPARENT LATCHES. `F10145A`, `F10415A`, `F10470` and
    `i2125` are memories whose write is LEVEL-sensitive, and `MC10173` and
    `SN74LS259` are latches. Written as `always @*` they pass a level straight
    through, so every read-modify-write path in the machine -- which is most of
    a datapath -- was a combinational loop. 405 F10145A packages alone.
    Rewritten on `sys_clk` with the part's own level as an ENABLE, which is
    the convention this design already uses for every clocked element and the
    same change its two DRAM cells already carried, the count went from 1,333
    back edges to 40.

That was the whole of it. A second theory -- that `F10016`'s carry out should
be registered, because the dictionary lists pin 4 only in an `[FF]` output
list -- was tried and is WRONG, and is recorded here because it is a plausible
misreading. An `[FF]` entry is a TIMING ARC from the clock, and a gate after
the register is folded into it rather than given a [G] entry; `S169` shows
this outright by giving RC' its own [FF] block with a clock-to-output delay of
30.8 ns against 16.5 ns for the Q outputs. The machine settles with the carry
combinational, which is also the only way a cascaded counter works.

FOUR STRUCTURAL LOOPS REMAIN, and all of them settle. The processor's:

    DMuxData -> DMData    (h17, MC10158)
    DMData   -> Pdata.00  (f03, MC10164)
    Pdata.00 -> MuxData2  (d11, MU10164)
    MuxData2 -> DMuxData  (l24, MU10164)

-- the processor's own multiplexer chain feeding back. And the same shape on
DispM, DispY and MemD, where a counter's carry out returns to its own count
enable through an MC10195:

    StopWakeCount   -> KillDWTWakeup    (e24 F10016, CE' to CO')
    KillDWTWakeup   -> StopWakeCount    (d24 MC10195)

None of these is a loop in operation. `StopWakeCount` is a wired-OR that d24
also drives from two other gates, so either of those holds it and the path is
broken; the multiplexer chain's selects never route all four legs at once; and
the carry gate only closes at terminal count. They are left alone rather than
cut, because a delay inserted there would be an invention, and the machine
converges with all four present -- over 200,000 cycles as readily as 20,000.

HOW THE GRAPH IS BUILT. Nodes are nets; an edge net_a -> net_b exists when some
package has a COMBINATIONAL path from a pin on net_a to a pin on net_b. The
dependency comes from the cell files themselves -- the same source the RTL is
emitted from -- so this is the emitted design, not a model of it:

  * `assign pN = <expr>` gives an edge from every pin in `expr` to pN, with
    intermediate `wire` definitions inlined, so a factored cell reads like a
    flat one.
  * `always @(posedge sys_clk)` is SEQUENTIAL: no edges, and it already breaks
    any loop through that part.
  * `always @*` passes a level, so everything the block reads reaches
    everything it drives -- but NEVER one of its own outputs to another.
    Without that exclusion every MC10181 slice reports a two-net cycle between
    its own result bits, because the pin numbers on the left of
    `assign p6 = ...` read as pins the block consumes. Sixteen false ALU loops
    per processor board came from exactly that.
  * a stub with no behaviour contributes nothing -- which is why none of this
    was visible until the cell library filled in, and why more will appear as
    the rest is written.
  * a rail, a resistor pack, or a net an `OVERRIDE_DRIVERS` part drives is
    held at one value by a single source, so nothing else on it is a path.
    That last one discounts the BaseBoard's `VCOPhase0`/`VCOPhase1` pair: the
    analog VCO's relaxation loop, already replaced by a fabric-clock divider.

Usage:
    sil_loops.py            report, exit non-zero on a loop that is not known
    sil_loops.py --verbose  also name every back edge found
"""

from __future__ import annotations

import argparse
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
SIL = os.path.join(HERE, '..', 'chm', 'sil')
CELLS = os.path.join(HERE, '..', 'verilog', 'cells')

sys.path.insert(0, HERE)
from sil_netlist import find_boards, load_board, vpart          # noqa: E402
from sil_ecldict import EclDict                                 # noqa: E402
import sil_to_verilog as gen                                    # noqa: E402

# The loops that are understood and deliberately left in place. Board -> the
# nets a depth-first search reports as closing them.
KNOWN = {
    'ProcH-Rev-Ce': {'DMuxData'},
    'DispM-Rev-Ch': {'CursorWindow\'', 'StopWakeCount'},
    'DispY-Rev-Cl': {'CursorWindow\'', 'StopWakeCount'},
    'MemD-Rev-Ca': {'ChkLastPhOrIdle'},
}

ASSIGN_RE = re.compile(r'assign\s+p(\d+)\s*=\s*([^;]+);')
WIRE_RE = re.compile(r'^\s*wire\s*(?:\[[^\]]*\]\s*)?(\w+)\s*=\s*([^;]+);', re.M)
PIN_RE = re.compile(r'\bp(\d+)\b')
PORT_OUT_RE = re.compile(r'output\s+wire\s+(?:\[[^\]]*\]\s*)?p(\d+)')


def cell_paths(path: str) -> dict[int, set[int]]:
    """output pin -> the input pins it depends on COMBINATIONALLY."""
    src = open(path).read()
    head, _, body = src.partition(');')
    if not body:
        return {}
    body = re.sub(r'//[^\n]*', '', body)
    body = re.sub(r'wire\s+_unused[^;]*;', '', body)

    latch = re.search(r'always\s*@\s*\*', body)
    out_pins = {int(m.group(1)) for m in PORT_OUT_RE.finditer(head)}

    paths: dict[int, set[int]] = {}
    if latch:
        reads = {int(x) for x in PIN_RE.findall(body)}
        for o in out_pins:
            paths[o] = reads - out_pins

    defs = {m.group(1): m.group(2) for m in WIRE_RE.finditer(body)}
    for _ in range(8):
        changed = False
        for k, v in list(defs.items()):
            for k2, v2 in defs.items():
                if k2 != k and re.search(rf'\b{k2}\b', v):
                    defs[k] = re.sub(rf'\b{k2}\b', f'({v2})', v)
                    changed = True
        if not changed:
            break

    raw = {int(m.group(1)): m.group(2) for m in ASSIGN_RE.finditer(body)}
    for o, expr in raw.items():
        for _ in range(4):
            before = expr
            for k, v in defs.items():
                expr = re.sub(rf'\b{k}\b', f'({v})', expr)
            for o2, e2 in raw.items():
                if o2 != o:
                    expr = re.sub(rf'\bp{o2}\b', f'({e2})', expr)
            if expr == before:
                break
        paths.setdefault(o, set()).update(
            {int(p) for p in PIN_RE.findall(expr)} - {o})
    return paths


def cell_table() -> dict[str, dict[int, set[int]]]:
    out = {}
    for f in sorted(os.listdir(CELLS)):
        m = re.match(r'cell_(.+)\.v$', f)
        if m:
            out[m.group(1)] = cell_paths(os.path.join(CELLS, f))
    return out


def board_graph(b, cells: dict, g: gen.Generator) -> dict[str, set[str]]:
    """net -> the nets it combinationally drives, on this board."""
    pinnet: dict[tuple[str, int], str] = {}
    for name, net in b.nets.items():
        for p in net['pins']:
            pinnet[(p['pkg'], p['pin'])] = name

    edges: dict[str, set[str]] = {}
    for pos, pkg in b.packages.items():
        paths = cells.get(vpart(pkg.get('type', '')))
        if not paths:
            continue
        for out_pin, in_pins in paths.items():
            dst = pinnet.get((pos, out_pin))
            if dst is None:
                continue
            for ip in in_pins:
                src = pinnet.get((pos, ip))
                if src is not None and src != dst:
                    edges.setdefault(src, set()).add(dst)

    held = {n for n in edges if g.rail_value(n)}
    for name, net in b.nets.items():
        for p in net['pins']:
            if (p['dir'] == 'out' and b.packages.get(p['pkg'], {})
                    .get('type', '') in g.OVERRIDE_DRIVERS):
                held.add(name)
    for n in held:
        edges.pop(n, None)
        for v in edges.values():
            v.discard(n)
    return edges


def back_edges(edges: dict[str, set[str]]) -> list[tuple[str, str]]:
    """Every back edge, by iterative DFS -- the graphs are thousands of nets
    deep and recursion overflows."""
    WHITE, GREY, BLACK = 0, 1, 2
    colour: dict[str, int] = {}
    found: list[tuple[str, str]] = []
    for root in sorted(edges):
        if colour.get(root, WHITE) != WHITE:
            continue
        stack = [(root, iter(sorted(edges.get(root, ()))))]
        colour[root] = GREY
        while stack:
            node, it = stack[-1]
            advanced = False
            for nxt in it:
                c = colour.get(nxt, WHITE)
                if c == GREY:
                    found.append((node, nxt))
                elif c == WHITE:
                    colour[nxt] = GREY
                    stack.append((nxt, iter(sorted(edges.get(nxt, ())))))
                    advanced = True
                    break
            if not advanced:
                colour[node] = BLACK
                stack.pop()
    return found


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--verbose', action='store_true')
    args = ap.parse_args(argv[1:])

    cells = cell_table()
    ecl = EclDict()
    ecl.load(os.path.join(SIL, 'msa-Rev-Bg.dm!1_', 'ecldict.analyze'))
    ecl.load(os.path.join(SIL, 'msa-Rev-Bg.dm!1_', 'ttldict.analyze'))
    known = gen.known_cells(CELLS)
    dirs = gen.cell_port_dirs(CELLS)
    clocked = gen.cells_wanting_clock(CELLS)

    unexpected: list[str] = []
    print(f'{"board":<16}{"nets":>7}{"edges":>8}{"loops":>7}  closing nets')
    for wl, lc in find_boards(SIL):
        b = load_board(wl, lc)
        g = gen.Generator(b, ecl, known, dirs, clocked)
        edges = board_graph(b, cells, g)
        be = back_edges(edges)
        closing = sorted({t for _s, t in be})
        allowed = KNOWN.get(b.name, set())
        unexpected += [f'{b.name}  {n}' for n in closing if n not in allowed]
        note = ' '.join(f'{n}{"" if n in allowed else " (NEW)"}'
                        for n in closing)
        print(f'{b.name:<16}{len(edges):>7}'
              f'{sum(len(v) for v in edges.values()):>8}{len(be):>7}  {note}')
        if args.verbose and be:
            for s, t in be:
                print(f'    {s} -> {t}')

    print()
    if unexpected:
        print('COMBINATIONAL LOOPS THAT ARE NOT KNOWN:')
        for u in unexpected:
            print('  ' + u)
        print()
        print('A loop is not automatically a bug -- ProcH keeps one, see the '
              'header -- but it')
        print('stops the machine settling if the logic in it can oscillate, '
              'and every one')
        print('found so far has been a cell modelled as a latch that is not '
              'one.')
        return 1
    print('loop check: no combinational feedback beyond the '
          f'{sum(len(v) for v in KNOWN.values())} known and understood')
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
