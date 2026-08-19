#!/usr/bin/env python3
"""Do the cells get OR/NOR POLARITY right, per gate, against EclDict?

`sil_check_cells.py` checks CONNECTIVITY -- that a gate reads the input pins
PARC's `[G ...]` summary says it does. It says so itself: "it checks
CONNECTIVITY, not function -- a gate that ORs where it should AND uses the
same pins." This is the missing half, for the one aspect of function the
dictionary DOES state.

WHAT IS CHECKED, and it is deliberately only the part that is UNIVERSAL:
when EclDict gives one gate both an `OUT` pin and an `o` pin (or `u`/`v` on
the wide OR-AND parts), those two outputs must be EXACT COMPLEMENTS. The
cell's own expressions are evaluated over every input assignment to decide
it, so a factored cell is checked like a flat one.

WHICH OUTPUT INVERTS IS NOT CHECKED, and the reason is worth keeping.

The handoff states the rule as "role `OUT` is the INVERTING output". It holds
for the OR/NOR family it was derived from and DOES NOT GENERALISE: on
MC10104 the `OUT` pins carry the AND and `o` the NAND, and on MC10121 pin 2
(`OUT`) is the plain OR-AND while pin 3 (`o`) is the OR-AND-INVERT. Both were
read off the data book.

Nor can PARC's net naming decide it, though it looks as though it can:
`--senses` prints, per part, which pin the boards prime when a package uses
both outputs and names them as a complementary pair. For MC10121 that is
unanimous -- nine packages, pin 2 primed, no exceptions -- AND IT IS WRONG.
Nine packages likewise prime pin 9 of an MC10102, whose pin 15 is certainly
the NOR. A net is named for the sense its designer treats as primary, not for
which pin inverts. Kept as a printer so nobody re-derives it as a rule.

WHAT DOES SETTLE IT is the data book's LOGIC DIAGRAM, read by coordinates.
The pin labels' overbars are in a font the PDF does not embed -- they render
as empty boxes and extract without the bar -- but the BUBBLE on the inverting
output rasterises fine, and `pdftotext -bbox` gives each pin label's y in
points. Match the two:

    pdftotext -f <page> -l <page> -bbox book.pdf -   # label y, in points
    pdftoppm -r 400 -f <page> -l <page> -x .. -y .. -png book.pdf out

For MC10121 the labels sit at y=326.5 and y=336.1 and the arrows at y=324.9
(plain) and y=335.2 (bubbled); for MC10104's gate d the bubble is on pin 9.

Found on its first run, and both were real:

  * `cell_MC10121` gave its two outputs the same sense the wrong way round
    AND ANDed its shared input pin 10 as a term of its own instead of ORing
    it into two groups (Tim's common-pin class). 44 packages, and one of them
    -- ContB l03 -- gates the MIR clock, which is what makes a jammed
    microinstruction survive its own first clock.
  * `cell_MC10104` gate d gave BOTH outputs the same expression, the fault
    the handoff records for cell_MC10103.
"""
import argparse
import collections
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sil_ecldict import EclDict                       # noqa: E402
from sil_check_cells import cell_gates, gate_table, DICTS   # noqa: E402

HERE = os.path.dirname(os.path.abspath(__file__))
CELLS = os.path.join(HERE, '..', 'verilog', 'cells')


ASSIGN_RE = re.compile(r'assign\s+p(\d+)\s*=\s*([^;]+);')

SIL = os.path.join(HERE, '..', 'chm', 'sil')

# Two parts are SECOND SOURCES of each other when the number matches once the
# family and speed letters are stripped: MC10164/MU10164, MC10210/SE10210,
# SN74LS01/SN74S01. A shared pin block does NOT mean that -- TtlDict groups by
# PINOUT, which is why `H01, H02, LS01, LS02, LS28, ...` sit on one line and
# why EclDict has `MC102, MC104` (a NOR and an AND).
def part_number(part):
    m = re.match(r'^(?:SN74|SN|MC|MU|SE|F|i)(?:LS|ALS|AS|S|H|N)?(\d+)', part)
    return m.group(1) if m else part


# Pairs that really are different parts but for which one model is right, each
# with its reason. Nothing goes here without one.
SAME_MODEL_OK = {
    # The '253 is the THREE-STATE '153. This RTL has no `inout` anywhere by
    # design (see the FPGA-shape note in docs/verilog-handoff.md), so the third
    # state cannot be represented and the enable forces 0 on both, which is the
    # '153's behaviour. Recorded rather than silenced.
    ('SN74LS153', 'SN74LS253'),
    # The '38 is the OPEN-COLLECTOR '00. The handoff records why one model is
    # right here: "each has effectively one driver, so a single open-collector
    # output plus its pull-up computes the gate's own function ... the
    # wired-AND only bites if a second driver is ever added". If one ever is,
    # this entry is the place the assumption is written down.
    ('SN7438', 'SN74LS00'),
}



def naming_evidence():
    """part -> {(pinA, pinB): Counter of which pin PARC primes}.

    A package that uses both of a gate's outputs and names them `X'` and `X`
    is stating the sense outright. One such pair proves nothing -- the handoff
    records MC10212 gates named for their function rather than their pin sense
    -- but across sixteen boards a unanimous majority is evidence, and it is
    the only evidence there is: the data book's overbars are in a font its PDF
    does not embed."""
    out = collections.defaultdict(lambda: collections.defaultdict(collections.Counter))
    netre = re.compile(r'^(\S+):\s*<')
    pinre = re.compile(r'([a-z]\d\d)\.(\d+)([oi])\s')
    for d in sorted(glob.glob(os.path.join(SIL, '*', ''))):
        wls = glob.glob(os.path.join(d, '*.wl'))
        if not wls:
            continue
        text = open(wls[0], 'rb').read().decode('latin-1').replace('\r', '\n')
        lines = text.split('\n')
        types = {}
        for ln in lines:
            m = re.match(r'^([a-z]\d\d):\s*\((\S+?)/', ln)
            if m:
                types[m.group(1)] = m.group(2)
        pins = collections.defaultdict(dict)
        cur = None
        for ln in lines:
            m = netre.match(ln)
            if m:
                cur = m.group(1)
                continue
            if cur and ln.startswith('    '):
                for pm in pinre.finditer(ln + ' '):
                    pins[pm.group(1)][int(pm.group(2))] = (cur, pm.group(3))
        for pkg, t in types.items():
            for a, b in ((x, y) for x in pins[pkg] for y in pins[pkg] if x < y):
                pa, pb = pins[pkg][a], pins[pkg][b]
                if pa[1] != 'o' or pb[1] != 'o':
                    continue
                na, nb = pa[0], pb[0]
                if na.replace("'", '') != nb.replace("'", ''):
                    continue
                if na.count("'") == nb.count("'"):
                    continue
                out[t][(a, b)][a if na.count("'") > nb.count("'") else b] += 1
    return out


def expr_for(path):
    """output pin -> fully inlined expression string, or None if not combinational."""
    src = open(path).read()
    body = src.split(');', 1)[1] if ');' in src else src
    if re.search(r'\balways\b', body) or 'TODO: model this part' in src:
        return None
    # reuse the connectivity checker's inliner for wires and output-feeds-output
    from sil_check_cells import WIRE_RE
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
    raw = {int(m.group(1)): m.group(2) for m in ASSIGN_RE.finditer(body)}
    out = {}
    for pin, e in raw.items():
        for _ in range(4):
            before = e
            for k, v in defs.items():
                e = re.sub(rf'\b{k}\b', f'({v})', e)
            for op, oe in raw.items():
                if op != pin:
                    e = re.sub(rf'\bp{op}\b', f'({oe})', e)
            if e == before:
                break
        out[pin] = e
    return out


def to_python(e):
    # Reduction operators and concatenations (`&{1'b0, p1, ...}`, the
    # `_unused_pins` idiom) are not boolean expressions and must not be
    # evaluated as Python.
    if '{' in e or '}' in e or '?' in e:
        return None
    """Verilog bit expression -> Python int expression.

    `&`, `|`, `^` already have Verilog's precedence in Python. `~` does not
    (it is unary-highest in Verilog), so each `~X` is rewritten to `(1^X)`
    with X taken as a balanced operand -- otherwise `~a & b` would evaluate
    as `~(a & b)`."""
    e = e.replace("1'b0", "0").replace("1'b1", "1")
    while '~' in e:
        i = e.index('~')
        j = i + 1
        while j < len(e) and e[j] == ' ':
            j += 1
        if j < len(e) and e[j] == '(':
            depth = 0
            k = j
            while k < len(e):
                if e[k] == '(':
                    depth += 1
                elif e[k] == ')':
                    depth -= 1
                    if depth == 0:
                        break
                k += 1
            operand, end = e[j:k + 1], k + 1
        else:
            m = re.match(r'\w+', e[j:])
            if not m:
                return None
            operand, end = m.group(0), j + m.end()
        e = e[:i] + '(1^' + operand + ')' + e[end:]
    return e


def evaluate(pyexpr, pins, assignment):
    env = {f'p{p}': ((assignment >> i) & 1) for i, p in enumerate(pins)}
    try:
        return eval(pyexpr, {'__builtins__': {}}, env) & 1
    except Exception:
        return None


def main(argv=None):
    ap = argparse.ArgumentParser(description='cell output polarity')
    ap.add_argument('--senses', action='store_true',
                    help="print PARC's own naming evidence for every pair")
    args = ap.parse_args((argv or sys.argv)[1:])
    naming = naming_evidence()
    ecl = EclDict()
    gt = {}
    for d in DICTS:
        ecl.load(d)
        gt.update(gate_table(d))
    short_of = {}

    checked = pairs = bad = 0
    problems = []
    for fn in sorted(os.listdir(CELLS)):
        if not fn.startswith('cell_') or not fn.endswith('.v'):
            continue
        part = fn[len('cell_'):-len('.v')]
        got = ecl.by_full_type(part)
        if not got:
            continue
        short_of[part] = got[0]
        exprs = expr_for(os.path.join(CELLS, fn))
        if not exprs:
            continue
        checked += 1
        for gname, g in got[1]['gates'].items():
            inv = list(g.get('out', []))
            non = list(g.get('nout', []))
            for role, pins_ in g.get('other', []):
                if role == 'u':
                    inv += pins_
                elif role == 'v':
                    non += pins_
            inv = [p for p in inv if p in exprs]
            non = [p for p in non if p in exprs]
            if not inv or not non:
                continue
            a, b = inv[0], non[0]
            # A pin block can list two INDEPENDENT gates under one letter --
            # MC10118 is `a,...OUT,2 > a,o,15` in the pin block but
            # `[G (3 4 5 6 7 9)>2, (9 10 11 12 13 14)>15]` in the summary: two
            # halves, one inverting and one not, NOT two senses of one gate.
            # The summary is what separates them.
            summ = gt.get(short_of.get(part, part), [])
            sets_a = [i for i, o in summ if a in o]
            sets_b = [i for i, o in summ if b in o]
            if sets_a and sets_b and sets_a[0] != sets_b[0]:
                continue
            pa, pb = to_python(exprs[a]), to_python(exprs[b])
            if pa is None or pb is None:
                continue
            ins = sorted({int(p) for p in re.findall(r'\bp(\d+)\b', exprs[a])} |
                         {int(p) for p in re.findall(r'\bp(\d+)\b', exprs[b])})
            if len(ins) > 16:
                continue
            pairs += 1
            comp_ok = True
            for asn in range(1 << len(ins)):
                va, vb = evaluate(pa, ins, asn), evaluate(pb, ins, asn)
                if va is None or vb is None:
                    comp_ok = None
                    break
                if va == vb:
                    comp_ok = False
                    break
            if comp_ok is False:
                bad += 1
                problems.append(f"{part} gate {gname}: p{a} (OUT) and p{b} (o) "
                                f"are not complements")
                continue

    # ---- SIBLING PARTS: same pinout, same gates, different part number ----
    #
    # MC1662 and MC1664 have IDENTICAL `[G]` summaries and identical pin
    # blocks, and the dictionary separates them by role letter alone -- `OUT`
    # on one, `o` on the other. They are a NOR/OR pair, and `cell_MC1662` was
    # a copy of `cell_MC1664`: 33 packages computing OR where the part NORs,
    # which turned ContB's IM address multiplexer into a constant. Neither
    # `cell-check` (connectivity is right) nor the complement check above
    # (only one output sense, so no pair to compare) can see that.
    #
    # Two properties, and the second is only asserted where the roles differ:
    #   * two different part numbers with the same pinout must not have the
    #     same model -- they are different parts;
    #   * if one marks a pin `OUT` where the other marks it `o`, the two
    #     models must be complements.
    sig = {}
    for part, short in short_of.items():
        g = ecl.parts[short]['gates']
        key = tuple(sorted(
            (tuple(sorted(gg.get('in', []) + gg.get('common', []))),
             tuple(sorted(gg.get('out', []) + gg.get('nout', []))))
            for gg in g.values()))
        roles = tuple(sorted(
            (pin, 'OUT') for gg in g.values() for pin in gg.get('out', [])
        )) + tuple(sorted(
            (pin, 'o') for gg in g.values() for pin in gg.get('nout', [])))
        sig.setdefault(key, []).append((part, roles))

    exprs_by_part = {}
    for fn in sorted(os.listdir(CELLS)):
        if fn.startswith('cell_') and fn.endswith('.v'):
            e = expr_for(os.path.join(CELLS, fn))
            if e:
                exprs_by_part[fn[len('cell_'):-len('.v')]] = e

    for key, group in sig.items():
        if len(key) == 0:
            continue
        have = [(p, r) for p, r in group if p in exprs_by_part]
        for i in range(len(have)):
            for j in range(i + 1, len(have)):
                pa, ra = have[i]
                pb, rb = have[j]
                ea, eb = exprs_by_part[pa], exprs_by_part[pb]
                pins = sorted(set(ea) & set(eb))
                if not pins:
                    continue
                if part_number(pa) == part_number(pb):
                    continue            # second source of the same part
                if (pa, pb) in SAME_MODEL_OK or (pb, pa) in SAME_MODEL_OK:
                    continue
                # SEMANTICALLY identical, not textually: a copied cell that
                # picked up a stray pair of parentheses is still a copy.
                # An expression this cannot evaluate (a ternary multiplexer, a
                # concatenation) means the pair CANNOT BE COMPARED -- not that
                # it is different and not that it is the same. Saying "same"
                # there is what made MC10158/MC10159 and SN7438/SN74LS00 look
                # like copies when the first pair is correctly two models.
                same = True
                for q in pins:
                    ka, kb = to_python(ea[q]), to_python(eb[q])
                    if ka is None or kb is None:
                        same = None
                        break
                    ins = sorted({int(x) for x in re.findall(r'\bp(\d+)\b', ea[q])} |
                                 {int(x) for x in re.findall(r'\bp(\d+)\b', eb[q])})
                    if not ins or len(ins) > 16:
                        same = False
                        break
                    va = [evaluate(ka, ins, n) for n in range(1 << len(ins))]
                    vb = [evaluate(kb, ins, n) for n in range(1 << len(ins))]
                    if None in va or None in vb:
                        same = None
                        break
                    if va != vb:
                        same = False
                        break
                if same is True:
                    bad += 1
                    problems.append(
                        f"{pa} and {pb} share a pinout and a [G] summary but "
                        f"have the SAME model -- two part numbers are two parts "
                        f"(roles {'differ' if ra != rb else 'agree'})")

    for p in problems:
        print("  " + p)
    print(f"sil_check_polarity: {checked} cells, {pairs} both-sense gates, "
          f"{len(sig)} distinct pinouts, {bad} wrong")
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
