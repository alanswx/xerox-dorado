#!/usr/bin/env python3
"""The Dorado backplane, from PARC's own design-automation output.

THE FINDING THIS TOOL EXISTS TO RECORD. The backplane was thought to need
either a schematic we do not have or an inference from the Term100 packages
in each board's wire list. Neither is necessary -- and the Term100 idea was
wrong at the root, because Term100 is a 100-ohm TERMINATING RESISTOR network,
not a connector. Every board directory in `chm/sil/` states its backplane
interface THREE times:

    <Board>-Rev-Xx.bp        NetName: C96      bare connector and pin
    <Board>-Rev-Xx.wl        a bare `E179` among the net's pins
    <Board>-Rev-Xx-C.nl      NetName: #s05-C.5 the same, slot-qualified
    <Board>-Rev-Xx-E.nl

Across all sixteen boards they agree on 2,052 of 2,054 pins: `.bp` and `.nl`
match exactly, and the five differences against the `.wl` are ground nets it
numbers individually (`GND-26`) where `.bp` collapses them (`GND`). So the
port list of every board is not inferred at all; it is stated three times over
by the tool that built the machine. `sil_netlist.load_bp` reconciles them and
`sil_to_verilog.py` emits exactly that port list.

THE BACKPLANE IS NOT STRAIGHT-THROUGH, and this tool shows why that matters.
The clock distribution proves it in one line: the BaseBoard drives
`CLK.ph'` from its pin C16, `CLK.pl'` from C13, `CLK.mc'` from C20 -- while
every receiving board takes its own clock on pin **C9**. One net, two
different pin numbers, point to point. Of the 639 net names that appear on
more than one board only 446 sit at the same pin, and the same pin number
carries unrelated nets on different boards (E90 is `MemBase.2` on the
processor and memory boards but `TNIA.06` on Control).

So a pin number is a PCB routing detail and cannot join two boards. THE NAME
IS THE CONNECTION -- which is what PARC's own flow assumed: each board was
drawn against agreed signal names, the router assigned pins per board, and
the backplane was wired to suit.

Usage:
    sil_backplane.py                 the report: everything above, measured
    sil_backplane.py --json OUT      the backplane netlist, for a generator
    sil_backplane.py --ports         per-board port list vs what we generate
"""

from __future__ import annotations

import argparse
import collections
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
SIL = os.path.join(HERE, '..', 'chm', 'sil')
GENERATED = os.path.join(HERE, '..', 'verilog', 'generated')

# The twelve boards of a working monochrome machine. msa/PCMSA are the
# storage-module boards, IOTest a test fixture and Music an option; they are
# read too, because MemD and MemX talk to the storage boards and their nets
# look unpaired without them.
CORE = ['ProcH', 'ProcL', 'ContA', 'ContB', 'IFU', 'MemC', 'MemD', 'MemX',
        'DskEth', 'DispY', 'DispM', 'BaseBd']


def _text(path: str) -> list[str]:
    """Xerox files are CR-terminated."""
    raw = open(path, 'rb').read().decode('latin-1')
    return raw.replace('\r\n', '\n').replace('\r', '\n').split('\n')


def load_backplane() -> dict[str, dict[str, str]]:
    """{board: {net: pin}} from the .bp files, checked against the .nl pair.

    Both are read because they are independent statements of the same thing:
    .bp is bare (`C96`), .nl is slot-qualified (`#s05-C.5`). A disagreement
    would mean one of them is stale, and the whole approach rests on them, so
    it is worth the few milliseconds to confirm they agree."""
    boards: dict[str, dict[str, str]] = {}
    for d in sorted(os.listdir(SIL)):
        p = os.path.join(SIL, d)
        if not os.path.isdir(p):
            continue
        bp = [f for f in os.listdir(p) if f.endswith('.bp')]
        if not bp:
            continue
        name = d.split('-Rev')[0]
        m = {}
        for line in _text(os.path.join(p, bp[0])):
            if ':' in line:
                net, pin = line.split(':', 1)
                m[net.strip()] = pin.strip()
        nl = set()
        for f in os.listdir(p):
            if f.endswith('-C.nl') or f.endswith('-E.nl'):
                for line in _text(os.path.join(p, f)):
                    if ':' in line and not line.startswith(';'):
                        nl.add(line.split(':', 1)[0].strip())
        if nl and nl != set(m):
            print(f'  WARNING {name}: .bp and .nl disagree on '
                  f'{len(nl ^ set(m))} nets', file=sys.stderr)
        boards[name] = m
    return boards


def slot_of(board_dir: str) -> str | None:
    """The .nl header states it -- but see the report: three boards claim
    s01, so this is the last build's setting, not the machine's layout."""
    p = os.path.join(SIL, board_dir)
    for f in os.listdir(p):
        if f.endswith('-C.nl'):
            head = _text(os.path.join(p, f))[0]
            m = re.search(r'located at (\S+)', head)
            return m.group(1) if m else None
    return None


def drivers(boards) -> dict[str, list[str]]:
    """Which boards DRIVE each backplane net, from the wire lists.

    This is what decides the net type in RTL: the Dorado's ECL is open-emitter
    and its buses are wired-OR, so a net driven by six boards is not an
    error to be resolved but the B bus working as designed."""
    sys.path.insert(0, HERE)
    from sil_netlist import load_board, find_boards
    out = collections.defaultdict(list)
    for wl, lc in find_boards(SIL):
        d = os.path.basename(os.path.dirname(wl))
        short = d.split('-Rev')[0]
        if short not in boards:
            continue
        b = load_board(wl, lc)
        for net in boards[short]:
            if net in b.nets and b.drivers_of(net):
                out[net].append(short)
    return out


def report() -> int:
    boards = load_backplane()
    print(f'{len(boards)} boards, '
          f'{sum(len(v) for v in boards.values())} backplane pins\n')

    byname = collections.defaultdict(dict)
    for b, m in boards.items():
        for net, pin in m.items():
            byname[net][b] = pin
    shared = {n: v for n, v in byname.items() if len(v) > 1}
    same = [n for n, v in shared.items() if len(set(v.values())) == 1]

    print('--- is the backplane straight-through?')
    print(f'{len(byname)} distinct net names, {len(shared)} on more than one '
          f'board, of which {len(same)} sit at the SAME pin everywhere.')
    bypin = collections.defaultdict(set)
    for b, m in boards.items():
        for net, pin in m.items():
            bypin[pin].add(net)
    clash = sum(1 for pin, nets in bypin.items() if len(nets) > 1)
    print(f'{clash} pin positions carry MORE THAN ONE net name across boards, '
          f'so a pin cannot identify a signal.')
    print('the clock distribution settles it -- one net, a different pin at '
          'each end:')
    for net in sorted(n for n in byname if n.startswith('CLK.')):
        v = byname[net]
        if 'BaseBd' in v and len(v) > 1:
            dest = ' '.join(f'{b}={p}' for b, p in sorted(v.items())
                            if b != 'BaseBd')
            print(f'    {net:<16} BaseBd={v["BaseBd"]:<5} -> {dest}')

    print('\n--- do names pair up?')
    print(f'{"board":<9}{"slot":>5}{"nets":>6}{"paired":>8}{"unpaired":>10}   '
          f'the unpaired, by prefix')
    dirs = {d.split('-Rev')[0]: d for d in os.listdir(SIL)
            if os.path.isdir(os.path.join(SIL, d))}
    for b in [x for x in CORE if x in boards] + \
             [x for x in sorted(boards) if x not in CORE]:
        solo = [n for n in boards[b] if len(byname[n]) == 1]
        pref = collections.Counter(re.split(r'[.\d]', n)[0] for n in solo)
        print(f'{b:<9}{slot_of(dirs[b]) or "?":>5}{len(boards[b]):>6}'
              f'{len(boards[b])-len(solo):>8}{len(solo):>10}   '
              + ', '.join(f'{k}x{v}' if v > 1 else k
                          for k, v in pref.most_common(6)))
    print('an unpaired net is not a failed match: it goes to a connector on '
          'the backplane itself')
    print('(disk tag bus and the differential pairs, ethernet transceiver, '
          'monitor DACs and sync,')
    print(' the BaseBoard\'s own ACP bus and serial lines) or is a scope test '
          'point.')

    drv = drivers(boards)
    multi = sorted(n for n in shared if len(drv.get(n, [])) > 1)
    print(f'\n--- how many boards DRIVE each shared net?')
    print(f'one board {len([n for n in shared if len(drv.get(n, []))==1])}, '
          f'several {len(multi)}, none {len([n for n in shared if not drv.get(n)])}')
    print('the several-driver nets are ECL WIRED-OR buses, not a conflict. In '
          'Verilog they are')
    print('`wor` nets, and Verilator accepts that but needs MULTIDRIVEN waived '
          'at the declaration.')
    groups = collections.Counter()
    for n in multi:
        groups[tuple(sorted(drv[n]))] += 1
    for who, cnt in groups.most_common(8):
        print(f'    {cnt:>3} nets driven by {" ".join(who)}')
    print('the driver count depends on what you instantiate. msa and PCMSA are '
          'two generations of')
    print('the storage-module board, so they co-drive 19 nets only in this '
          'listing, never in a')
    print('machine. DispM and DispY are NOT such a pair: 42 nets are shared by '
          'those two boards')
    print('and no other, so the colour board plugs into the monochrome one and '
          'a colour machine')
    print('has both.')
    return 0


def ports() -> int:
    """The port list we GENERATE against the port list PARC states.

    This is the gate on the port lists. `sil_to_verilog.py` takes them from
    `.bp` now; it used to INFER them from the wire list (a net whose only
    consumers are Term100 pins leaves the board), which missed 703 backplane
    nets and invented 833 ports. A net that should be a port but comes out an
    internal wire can never reach another board, so this must stay at zero."""
    boards = load_backplane()
    sys.path.insert(0, HERE)
    from sil_to_verilog import vname
    print(f'{"board":<9}{"stated":>8}{"emitted":>9}{"agree":>7}'
          f'{"missing":>9}{"spurious":>10}')
    tot = [0, 0, 0, 0, 0]
    for f in sorted(os.listdir(GENERATED)):
        if not f.endswith('.v'):
            continue
        short = f.split('-Rev')[0]
        if short not in boards:
            continue
        head = open(os.path.join(GENERATED, f)).read().split(');', 1)[0]
        emitted = set(re.findall(r'(?:input|output|inout)\s+wire\s+(\w+)',
                                 head))
        stated = {vname(n) for n in boards[short]}
        agree = stated & emitted
        row = (len(stated), len(emitted), len(agree),
               len(stated - emitted), len(emitted - stated))
        note = ''
        if stated - emitted:
            note = '   ' + ' '.join(sorted(stated - emitted))
        if emitted - stated:
            note += '   +' + ' '.join(sorted(emitted - stated)[:4])
        print(f'{short:<9}{row[0]:>8}{row[1]:>9}{row[2]:>7}{row[3]:>9}'
              f'{row[4]:>10}{note}')
        tot = [a + b for a, b in zip(tot, row)]
    print(f'{"TOTAL":<9}{tot[0]:>8}{tot[1]:>9}{tot[2]:>7}{tot[3]:>9}{tot[4]:>10}')
    print('\nmissing  = on the backplane but emitted as an internal wire; it '
          'cannot leave the board.')
    print('spurious = emitted as a port but not on the backplane.')
    print('\nThe two on DskEth are the known ground-naming difference, not a '
          'gap: the .bp calls')
    print('them GND and GND-, the wire list numbers them individually '
          '(GND-26, GND-39, GND-44),')
    print('so there is no net of that name to make a port of. Power and '
          'ground do not want to be')
    print('ports anyway.')
    return 0


# A working monochrome Dorado. DispM is the COLOUR board and DispY the
# monochrome one (Hardware Manual doc p.110: "on a Dorado with only a 7-wire
# terminal and no color monitor, only the DispY board is present").
#
# They are NOT alternatives, which this tool is what showed: 42 nets are
# shared by DispY and DispM and by no other board -- `AItem.0-7`, `BItem.*`,
# `AOff`, `BByPass` -- so the colour board plugs into the monochrome one's
# item buses and a colour machine has BOTH. Add it with
# `--boards ProcH,...,DispY,DispM`. That matches the C emulator, where the
# colour display is a second screen at its own raster rather than a
# colourisation of the first (src/dispm.c).
#
# msa and PCMSA are two generations of storage-module board; IOTest is a test
# fixture and Music an option.
DEFAULT_MACHINE = ['ProcH', 'ProcL', 'ContA', 'ContB', 'IFU',
                   'MemC', 'MemD', 'MemX', 'DskEth', 'DispY', 'BaseBd']


def _board_dirs() -> dict[str, str]:
    return {d.split('-Rev')[0].split('.dm')[0]: d
            for d in sorted(os.listdir(SIL))
            if os.path.isdir(os.path.join(SIL, d))}


def _classify_boards(names: list[str]):
    """Run the SAME classification the board modules were emitted with.

    Not a reimplementation: `sil_to_verilog.Generator.classify()` is imported
    and called, so the top module cannot disagree with the modules it
    instantiates about which nets are ports or which way they face. Two passes
    over one netlist that answered the same question differently is exactly
    what produced the ASSIGNIN bug inside the generator."""
    sys.path.insert(0, HERE)
    from sil_netlist import load_board
    from sil_ecldict import EclDict
    from sil_to_verilog import (Generator, known_cells, cell_port_dirs,
                                vname)
    dirs = _board_dirs()
    ecl = EclDict()
    ecl.load(os.path.join(SIL, 'msa-Rev-Bg.dm!1_', 'ecldict.analyze'))
    cells_dir = os.path.join(HERE, '..', 'verilog', 'cells')
    cells, cdirs = known_cells(cells_dir), cell_port_dirs(cells_dir)
    out = {}
    for short in names:
        if short not in dirs:
            raise SystemExit(f'no board {short!r} under {SIL}; '
                             f'have {", ".join(sorted(dirs))}')
        d = os.path.join(SIL, dirs[short])
        wl = [f for f in os.listdir(d) if f.endswith('.wl')][0]
        b = load_board(os.path.join(d, wl))
        g = Generator(b, ecl, cells, cdirs)
        g.classify()
        out[short] = {'module': vname(b.name), 'ports': dict(g.ports)}
    return out, vname


def emit_top(path: str, names: list[str], module: str = 'dorado_backplane') -> int:
    """Wire the boards together into one module, BY NAME.

    The backplane is not straight-through -- the BaseBoard drives each board's
    clock from a different pin while every receiver takes it on C9 -- so pin
    numbers cannot join two boards and the signal NAME is the connection. See
    this file's module docstring for the evidence.

    Three kinds of net come out of that:

      * driven by SEVERAL boards -> `wor`. MECL 10K outputs are open emitters
        and are deliberately tied together; the B bus `BMux.00-15` is driven
        by ContA, IFU, MemC, MemD, MemX and ProcH/ProcL at once. `wor` is the
        faithful model and Verilator simulates it correctly (verified).
      * driven by exactly one board -> plain `wire`.
      * reaching only one board in this configuration -> a TOP-LEVEL PORT.
        Either it goes to a connector mounted on the backplane (the disk tag
        bus, the ethernet transceiver, the monitor DACs, the keyboard) or the
        board it pairs with is not in this configuration. The emitted comment
        says which, per net, because those two mean very different things.
    """
    info, vname = _classify_boards(names)
    everything = load_backplane()          # all sixteen, to explain absences

    # The port list here is recomputed, not read out of the generated .v. If
    # those files are stale the instances would connect ports the modules no
    # longer have -- and PINMISSING is WAIVED in the lint gate, so an
    # unconnected port would float silently rather than fail. Check instead.
    for short, d in sorted(info.items()):
        gen = os.path.join(GENERATED, d['module']
                           .replace('_m_', '-').replace('_u_', '_') + '.v')
        if not os.path.exists(gen):
            gen = next((os.path.join(GENERATED, f)
                        for f in sorted(os.listdir(GENERATED))
                        if f.split('-Rev')[0].split('.v')[0] == short), None)
        if not gen or not os.path.exists(gen):
            raise SystemExit(f'{short}: no generated module; run `make -C '
                             f'verilog boards` first')
        head = open(gen).read().split(');', 1)[0]
        on_disk = set(re.findall(r'(?:input|output|inout)\s+wire\s+(\w+)', head))
        want = {vname(n) for n in d['ports']}
        if on_disk != want:
            raise SystemExit(
                f'{short}: {os.path.basename(gen)} is STALE -- it declares '
                f'{len(on_disk)} ports, this classification wants {len(want)} '
                f'({len(want - on_disk)} missing, {len(on_disk - want)} extra). '
                f'Run `make -C verilog boards`.')

    net_boards: dict[str, dict[str, str]] = collections.defaultdict(dict)
    for short, d in info.items():
        for net, direction in d['ports'].items():
            net_boards[net][short] = direction

    ports, internal = {}, {}
    absent_partner = {}
    for net, v in net_boards.items():
        drv = [b for b, d in v.items() if d in ('output', 'inout')]
        if len(v) == 1 or not drv:
            elsewhere = sorted(b for b in everything
                               if net in everything[b] and b not in names)
            absent_partner[net] = elsewhere
            ports[net] = ('inout' if 'inout' in v.values() else
                          'output' if drv else 'input')
        else:
            internal[net] = 'wor' if len(drv) > 1 else 'wire'

    L: list[str] = []
    A = L.append
    A("// The Dorado backplane: every board, wired together by signal name.")
    A("//")
    A("// Generated by tools/sil_backplane.py --emit. DO NOT EDIT.")
    A("// Source: <Board>.bp in chm/sil -- PARC's own statement of which nets")
    A("// leave each board, cross-checked against the .wl and the .nl pair.")
    A("//")
    A("// Boards connect BY NAME, not by pin: the backplane is not")
    A("// straight-through (the BaseBoard drives CLK.ph' from C16 into ProcH's")
    A("// C9), and 182 pin positions carry different nets on different boards.")
    A("//")
    A(f"// Configuration: {' '.join(names)}")
    A(f"// {len(internal)} internal nets "
      f"({sum(1 for t in internal.values() if t == 'wor')} wired-OR), "
      f"{len(ports)} top-level ports.")
    A("")
    A('`default_nettype none')
    A("")
    A(f'module {module} (')
    decl = []
    names_sorted = sorted(ports)
    for i, net in enumerate(names_sorted):
        why = ('to a backplane connector (cable)' if not absent_partner[net]
               else 'awaits ' + ' '.join(absent_partner[net]))
        kw = {'input': 'input ', 'output': 'output', 'inout': 'inout '}[ports[net]]
        # The comma goes BEFORE the comment, or it is inside it and gone.
        comma = ',' if i < len(names_sorted) - 1 else ''
        decl.append(f'    {kw} wire {vname(net):<26}{comma:<2} // {why}')
    A('\n'.join(decl) if decl else '    // no external ports')
    A(');')
    A("")
    wor = sorted(n for n, t in internal.items() if t == 'wor')
    plain = sorted(n for n, t in internal.items() if t == 'wire')
    if wor:
        A(f'  // {len(wor)} WIRED-OR nets: MECL open emitters tied together')
        A('  // across boards. Verilator raises MULTIDRIVEN on a `wor`, which')
        A('  // is the point of using one, so it is waived here only.')
        A('  /* verilator lint_off MULTIDRIVEN */')
        for n in wor:
            A(f'  wor  {vname(n)};')
        A('  /* verilator lint_on MULTIDRIVEN */')
        A("")
    if plain:
        A(f'  // {len(plain)} single-driver nets')
        for n in plain:
            A(f'  wire {vname(n)};')
        A("")
    for short in names:
        d = info[short]
        A(f'  // ---- {short}')
        A(f'  {d["module"]} b_{short} (')
        conns = [f'    .{vname(n)}({vname(n)})' for n in sorted(d['ports'])]
        A(',\n'.join(conns))
        A('  );')
        A("")
    A('endmodule')
    A('`default_nettype wire')

    os.makedirs(os.path.dirname(path) or '.', exist_ok=True)
    with open(path, 'w') as fh:
        fh.write('\n'.join(L) + '\n')

    cable = sum(1 for n in ports if not absent_partner[n])
    print(f'wrote {path}: {len(names)} boards, {len(internal)} internal nets '
          f'({len(wor)} wired-OR), {len(ports)} ports')
    print(f'  ports: {cable} to backplane connectors (cables), '
          f'{len(ports)-cable} awaiting a board not in this configuration')
    byabsent = collections.Counter(
        ' '.join(absent_partner[n]) for n in ports if absent_partner[n])
    for who, cnt in byabsent.most_common(6):
        print(f'    {cnt:>4} would connect to {who}')
    return 0


def to_json(path: str) -> int:
    boards = load_backplane()
    drv = drivers(boards)
    byname = collections.defaultdict(dict)
    for b, m in boards.items():
        for net, pin in m.items():
            byname[net][b] = pin
    doc = {
        'source': 'chm/sil/<Board>-Rev-Xx.bp, cross-checked against -C.nl/-E.nl',
        'note': 'Boards connect BY NAME. Pin numbers are per-board routing '
                'and differ across a single net; see the module docstring.',
        'boards': {b: {'pins': len(m)} for b, m in sorted(boards.items())},
        'nets': {
            net: {
                'boards': dict(sorted(v.items())),
                'drivers': sorted(drv.get(net, [])),
                'wired_or': len(drv.get(net, [])) > 1,
                'off_board': len(v) > 1,
            }
            for net, v in sorted(byname.items())
        },
    }
    with open(path, 'w') as fh:
        json.dump(doc, fh, indent=1, sort_keys=False)
        fh.write('\n')
    shared = sum(1 for v in doc['nets'].values() if v['off_board'])
    print(f'wrote {path}: {len(doc["nets"])} nets, {shared} reaching more '
          f'than one board')
    return 0


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--json', metavar='OUT')
    ap.add_argument('--ports', action='store_true')
    ap.add_argument('--emit', metavar='OUT', nargs='?',
                    const='verilog/generated/dorado_backplane.v',
                    help='write the top module wiring the boards together')
    ap.add_argument('--boards', metavar='A,B,...',
                    help='which boards to instantiate (default: a working '
                         'monochrome machine, ' + ' '.join(DEFAULT_MACHINE))
    ap.add_argument('--module', default='dorado_backplane')
    args = ap.parse_args(argv[1:])
    if args.emit:
        names = ([b.strip() for b in args.boards.split(',') if b.strip()]
                 if args.boards else DEFAULT_MACHINE)
        return emit_top(args.emit, names, args.module)
    if args.json:
        return to_json(args.json)
    if args.ports:
        return ports()
    return report()


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
