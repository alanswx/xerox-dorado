#!/usr/bin/env python3
"""Which memory packages could be merged into one wide RAM, and what it saves.

WHY THIS EXISTS. The generator emits one cell instance per memory package, and
each instance carries its own `reg` array -- so on an FPGA each becomes its own
block RAM. Across the eleven-board machine that is 401 memories holding
681,280 bits total: on a DE10-Nano (Cyclone V, 557 M10K blocks of 10 Kbit) the
capacity is nothing but the COUNT is most of the device. Merging bit-slices
that share an address into one wide RAM is therefore worth far more than the
bit total suggests.

THE CATCH, and it is the whole reason this is a tool rather than a one-liner.
PARC fanned the address out. IM's 144 F10415A packages do NOT share an address
net: ContB carries twelve buffered copies, `RA.01a` through `RA.01l`, because
no ECL output drives 144 loads. Grouping by net NAME therefore finds ~120
groups of one or two bits and concludes nothing can be merged, which is wrong.

So copies are canonicalised first, and provably rather than by naming
convention: two nets are the same signal if they are driven by the SAME
package and that package's cell assigns their pins the SAME expression. For
`RA.01a`/`RA.01b` that is ContB a12, an MC10211, whose cell says
`assign p2 = a; assign p3 = a; assign p4 = a;`. This tool reads the cell
sources to establish that, so a cell that stopped being a pure fan-out buffer
would stop being treated as one.
"""
import sys, os, re, glob, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sil_netlist import load_board

# Address and control pins per memory part, from the cell headers.
MEM = {
    'F10415A': dict(depth=1024, addr=[4,5,6,7,9,10,11,12,13,14], ctl=[2,3]),
    'F10145A': dict(depth=16,   addr=[10,9,7,6],                 ctl=[3,13]),
    'F10470':  dict(depth=4096, addr=[4,5,6,7,9,10,11,12],       ctl=[2,3]),
}
BOARDS = ['ContA-Rev-Cd','ContB-Rev-Cd','ProcH-Rev-Ce','ProcL-Rev-Ci','MemC-Rev-Be',
          'MemD-Rev-Ca','MemX-Rev-Ch','IFU-Rev-Ch','DskEth-Rev-Cf','DispY-Rev-Cl',
          'BaseBd-Rev-Am']
M10K_BITS = 10240


def fanout_classes(cells_dir):
    """part -> {pin: class}, where pins in one class are driven identically."""
    out = {}
    for path in glob.glob(os.path.join(cells_dir, 'cell_*.v')):
        part = os.path.basename(path)[5:-2]
        expr = {}
        for m in re.finditer(r'assign\s+p(\d+)\s*=\s*([^;]+);', open(path).read()):
            expr[int(m.group(1))] = ' '.join(m.group(2).split())
        by = collections.defaultdict(list)
        for pin, e in expr.items():
            by[e].append(pin)
        cls = {}
        for e, pins in by.items():
            if len(pins) > 1:                     # a fan-out group
                for p in pins:
                    cls[p] = min(pins)
        if cls:
            out[part] = cls
    return out


def canon(board, net, fclasses, drivers):
    """The canonical name of a net, following pure fan-out copies to one root."""
    d = drivers.get(net)
    if not d:
        return net
    pos, pin = d
    part = board.packages.get(pos, {}).get('type', '').split('/')[0]
    part = re.sub(r'[^A-Za-z0-9_]', '_', part)
    cls = fclasses.get(part)
    if cls and pin in cls:
        return f'{pos}#{cls[pin]}'               # the class representative
    return net


def main():
    fclasses = fanout_classes('verilog/cells')
    grand_before = grand_after = 0
    for bn in BOARDS:
        d = f'chm/sil/{bn}.dm!1_'
        b = load_board(glob.glob(d + '/*.wl')[0], glob.glob(d + '/*.lc')[0])
        drivers = {}
        for net, info in b.nets.items():
            for p in info['pins']:
                if p['dir'] == 'out':
                    drivers[net] = (p['pkg'], p['pin'])
        groups = collections.defaultdict(list)
        for pos, pk in b.packages.items():
            t = pk.get('type', '').split('/')[0]
            if t not in MEM:
                continue
            pins = {p['pin']: net for net, info in b.nets.items()
                    for p in info['pins'] if p['pkg'] == pos}
            key = (t,
                   tuple(canon(b, pins.get(x, '-'), fclasses, drivers) for x in MEM[t]['addr']),
                   tuple(canon(b, pins.get(x, '-'), fclasses, drivers) for x in MEM[t]['ctl']))
            groups[key].append(pos)
        if not groups:
            continue
        # Cyclone V puts SMALL memories in MLABs (LUT RAM), not M10K, so the
        # 16x4 register files cost no block RAM either way. Only the 1024- and
        # 4096-deep parts are M10K candidates, and they are what this counts.
        deep = {k: v for k, v in groups.items() if MEM[k[0]]['depth'] >= 1024}
        before = sum(len(v) for v in deep.values())
        after = len(deep)
        blocks_before = before          # one M10K each, worst case
        blocks_after = sum(max(1, (MEM[k[0]]['depth'] * len(v) + M10K_BITS - 1) // M10K_BITS)
                           for k, v in deep.items())
        n_small = sum(len(v) for k, v in groups.items() if MEM[k[0]]['depth'] < 1024)
        if before == 0:
            print(f'{b.name:15s} {n_small:3d} small files (MLAB), no M10K candidates')
            continue
        grand_before += blocks_before
        grand_after += blocks_after
        print(f'{b.name:15s} {before:3d} deep memories -> {after:3d} groups '
              f'| M10K {blocks_before:3d} -> {blocks_after:3d}   (+{n_small} small -> MLAB)')
        for k, v in sorted(deep.items(), key=lambda kv: -len(kv[1]))[:3]:
            if len(v) > 2:
                print(f'{"":17s}  {k[0]} x{len(v)} = one {MEM[k[0]]["depth"]}x{len(v)} RAM')
    print(f'{"TOTAL":15s} {"":16s}   M10K worst case {grand_before:3d} -> {grand_after:3d}'
          f'   (DE10-Nano has 557)')
    return 0


if __name__ == '__main__':
    sys.exit(main())
