#!/usr/bin/env python3
"""Check every combinational cell against PARC's own gate definitions.

WHY THIS EXISTS. Tim read `cell_MC10100.v` and found that pin 9 -- the strobe
that MC10100 feeds into ALL FOUR gates -- had been wired into only the first
one. `cell_MC10101` had the same bug with its common pin 12. Both were
hand-written from the dictionary's PIN BLOCK, which names the common input
once and leaves you to know it is shared.

The dictionary says it a second way, and that way is unambiguous. Alongside
the pin blocks, `EclDict.Analyze` carries a gate summary per part:

    MC100
    [G (4 5 9)>(2), (6 7 9)>(3), (10 11 9)>(14), (12 13 9)>(15) : 1 3.3 ]

Every gate, with its complete input list and its outputs. So "does this cell
use exactly the inputs PARC says that gate has" is a question a program can
answer, for every cell at once, and that is what this does.

It resolves intermediate wires first (`wire a = ~(p5|p6|p7); assign p2 = a;`),
so a cell that factors its logic is checked the same as one that does not.

WHAT IT CANNOT CHECK. Only combinational cells: a part with an `always` block
is reported as skipped, because a clock or an asynchronous reset is not an
"input to the gate" in the [G] sense. Nor does it check the FUNCTION -- a gate
that ORs where it should AND uses the same pins. It checks connectivity, which
is where the transcription errors have actually been.

Usage:
    sil_check_cells.py            report, exit non-zero on a mismatch
    sil_check_cells.py --verbose  also list the cells it skipped and why
"""

from __future__ import annotations

import argparse
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
SIL = os.path.join(HERE, '..', 'chm', 'sil')
CELLS = os.path.join(HERE, '..', 'verilog', 'cells')
DICT = os.path.join(SIL, 'msa-Rev-Bg.dm!1_', 'ecldict.analyze')

sys.path.insert(0, HERE)
from sil_netlist import read_xerox_text, vpart   # noqa: E402
from sil_ecldict import EclDict                  # noqa: E402


def gate_table(path: str) -> dict[str, list[tuple[set, set]]]:
    """part -> [(inputs, outputs), ...] from the `[G ...]` summaries.

    Two spellings appear: `(4 5 9)>(2)` and the bare `5>2`. Everything from
    the `:` is loading and delay data, not connectivity."""
    out: dict[str, list[tuple[set, set]]] = {}
    names: list[str] = []
    for line in read_xerox_text(path):
        t = line.strip()
        if not t:
            continue
        if t.startswith('[G'):
            body = t[2:]
            body = body.split(':', 1)[0]
            gates = []
            for clause in body.split(','):
                if '>' not in clause:
                    continue
                lhs, rhs = clause.split('>', 1)
                pins = lambda s: {int(n) for n in re.findall(r'\d+', s)}
                gates.append((pins(lhs), pins(rhs)))
            for n in names:
                out[n] = gates
            names = []
        elif t.startswith('['):
            names = []                      # [M ...] memory entries, not gates
        elif not t.startswith(';'):
            # a name line, possibly `MC210, MC211, MC212`
            if re.match(r'^[A-Za-z0-9_, ]+$', t):
                names = [x.strip() for x in t.split(',') if x.strip()]
    return out


ASSIGN_RE = re.compile(r'assign\s+p(\d+)\s*=\s*([^;]+);')
# `wire a = ...` and `wire [7:0] d = ...` alike -- missing the vector form
# made every multiplexer look as though it read no inputs.
WIRE_RE = re.compile(r'^\s*wire\s*(?:\[[^\]]*\]\s*)?(\w+)\s*=\s*([^;]+);', re.M)
PIN_RE = re.compile(r'\bp(\d+)\b')


def cell_gates(path: str) -> tuple[dict[int, set], str | None]:
    """output pin -> the set of input pins its expression reads.

    Intermediate wires are inlined so a factored cell reads the same as a
    flat one. Returns (gates, skip_reason)."""
    src = open(path).read()
    body = src.split(');', 1)[1] if ');' in src else src
    if re.search(r'\balways\b', body):
        return {}, 'sequential'
    if 'TODO: model this part' in src:
        return {}, 'skeleton'

    # Inline `wire NAME = expr;` definitions, repeatedly, so nested factoring
    # resolves. Bounded because the definitions are acyclic.
    defs = {m.group(1): m.group(2) for m in WIRE_RE.finditer(body)
            if not m.group(1).startswith('_')}
    for _ in range(8):
        changed = False
        for k, v in list(defs.items()):
            for k2, v2 in defs.items():
                if k2 != k and re.search(rf'\b{k2}\b', v):
                    defs[k] = re.sub(rf'\b{k2}\b', f'({v2})', v)
                    changed = True
        if not changed:
            break

    # An output pin can feed another expression (`assign p15 = ~p2 & ...`),
    # so those are inlined as well before the inputs are counted.
    raw = {}
    for m in ASSIGN_RE.finditer(body):
        raw[int(m.group(1))] = m.group(2)

    gates: dict[int, set] = {}
    for out_pin, expr in raw.items():
        for _ in range(4):
            before = expr
            for k, v in defs.items():
                expr = re.sub(rf'\b{k}\b', f'({v})', expr)
            for op, oe in raw.items():
                if op != out_pin:
                    expr = re.sub(rf'\bp{op}\b', f'({oe})', expr)
            if expr == before:
                break
        gates[out_pin] = {int(p) for p in PIN_RE.findall(expr)}
    return gates, None


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--verbose', action='store_true')
    args = ap.parse_args(argv[1:])

    gt = gate_table(DICT)
    ecl = EclDict()
    ecl.load(DICT)

    # cell file -> the dictionary short name for that part
    short_of = {}
    for short in gt:
        full = ecl.full_name(short)
        if full:
            short_of.setdefault(vpart(full.split('/')[0]), short)

    checked = bad = skipped = 0
    problems: list[str] = []
    notes: list[str] = []
    for f in sorted(os.listdir(CELLS)):
        m = re.match(r'cell_(.+)\.v$', f)
        if not m:
            continue
        part = m.group(1)
        short = short_of.get(part)
        if not short:
            skipped += 1
            if args.verbose:
                print(f'  skip {part:<12} no [G] entry in the dictionary')
            continue
        gates, why = cell_gates(os.path.join(CELLS, f))
        if why:
            skipped += 1
            if args.verbose:
                print(f'  skip {part:<12} {why}')
            continue
        if not gates:
            skipped += 1
            continue

        checked += 1
        for out_pin, used in sorted(gates.items()):
            want = None
            for ins, outs in gt[short]:
                if out_pin in outs:
                    want = ins
                    break
            if want is None:
                continue                    # output not in the summary
            missing = want - used
            extra = used - want
            if missing:
                bad += 1
                problems.append(f'  {part:<12} p{out_pin:<3} IGNORES '
                                + ' '.join(f'p{p}' for p in sorted(missing))
                                + f' -- PARC says that gate reads {sorted(want)}')
            elif extra:
                notes.append(f'  {part:<12} p{out_pin:<3} also reads '
                             + ' '.join(f'p{p}' for p in sorted(extra))
                             + '  (a select, enable or carry the [G] summary '
                               'does not list)')

    if problems:
        print('CELLS THAT IGNORE AN INPUT PARC SAYS THE GATE HAS:')
        for p in problems:
            print(p)
        print()
    if args.verbose and notes:
        print('cells reading more than the [G] summary lists (expected for '
              'selects, enables and carries):')
        for p in notes:
            print(p)
        print()
    print(f'cell check: {checked} combinational cells against the dictionary\'s '
          f'[G] gate lists, {bad} ignoring an input, {len(notes)} reading '
          f'extra control pins, {skipped} skipped')
    return 1 if bad else 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
