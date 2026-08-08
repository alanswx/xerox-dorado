#!/usr/bin/env python3
"""Inventory a Dorado board's backplane interface from its Sil netlists.

PARC's design-automation output is in `chm/sil/<Board>-Rev-Xx.dm!N_/`:

    <Board>-Rev-Xx-C.nl   nets on backplane connector C   `Signal: #s05-C.12`
    <Board>-Rev-Xx-E.nl   nets on connector E
    <Board>-Rev-Xx.lc     component list: part/pins/package -> grid positions
    <Board>-Rev-Xx.wl     the wire list

The `.nl` files are the board's INTERFACE, in the machine's own signal names,
and those are the same names the C emulator uses -- `IOB`, `RSTK`, `MemBase`,
`Overflow'`. So they can be cross-checked against the emulator directly,
which is the first time anything in this project could be checked against the
hardware's own design data rather than against the manual's prose.

    sil_netlist_report.py ProcH ProcL          # group and compare boards
    sil_netlist_report.py --chips ProcH        # the component list too
    sil_netlist_report.py --grep SYM ProcH     # which nets match a name

Bit numbering is Xerox's: a 16-bit bus appears as .00-.07 on the high board
and .08-.15 on the low one, with a parity bit above (.16 high, .17 low).
"""

import argparse
import collections
import glob
import os
import re
import sys

SIL_ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                        os.pardir, "chm", "sil")


def board_dir(board):
    hits = sorted(glob.glob(os.path.join(SIL_ROOT, board + "-Rev-*.dm!*_")))
    if not hits:
        sys.exit("no archive for %s under %s -- run tools/fetch_dorado_sil.py"
                 % (board, SIL_ROOT))
    return hits[-1]


def read(path):
    with open(path, "rb") as f:
        return f.read().decode("latin-1").replace("\r", "\n")


def nets(bdir):
    """[(signal, location)] across every connector netlist of a board."""
    out = []
    for f in sorted(glob.glob(os.path.join(bdir, "*.nl"))):
        for line in read(f).split("\n"):
            m = re.match(r"\s*([^:;@\s]+):\s*#(\S+)", line)
            if m:
                out.append((m.group(1), m.group(2)))
    return out


def group(signals):
    """Fold `IOB.00`..`IOB.07` into one entry with its bit list. The trailing
    quote (active low) and drive-strength suffix are part of the name."""
    g = collections.OrderedDict()
    for name in signals:
        m = re.match(r"^(.*?)\.(\d+)('?[a-z]?)$", name)
        key, bit = (m.group(1) + m.group(3), int(m.group(2))) if m else (name, None)
        g.setdefault(key, [])
        if bit is not None:
            g[key].append(bit)
    return g


def chips(bdir):
    """part -> [grid positions], from the .lc component list."""
    out = collections.OrderedDict()
    for f in glob.glob(os.path.join(bdir, "*.lc")):
        cur = None
        for tok in read(f).replace(":", ":\n").split("\n"):
            tok = tok.strip()
            if not tok:
                continue
            if tok.endswith(":"):
                cur = tok[:-1]
                out.setdefault(cur, [])
            elif cur:
                out[cur] += tok.split()
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("boards", nargs="+")
    ap.add_argument("--chips", action="store_true", help="also list the parts")
    ap.add_argument("--grep", help="only signals whose name contains this")
    args = ap.parse_args()

    for board in args.boards:
        bdir = board_dir(board)
        n = nets(bdir)
        g = group([s for s, _ in n])
        print("== %s  (%s)  %d nets, %d signals"
              % (board, os.path.basename(bdir), len(n), len(g)))
        for k in sorted(g):
            if args.grep and args.grep.lower() not in k.lower():
                continue
            bits = sorted(set(g[k]))
            if bits:
                # Contiguous runs read better than 16 comma-separated numbers.
                runs, start, prev = [], bits[0], bits[0]
                for b in bits[1:] + [None]:
                    if b != prev + 1:
                        runs.append(str(start) if start == prev
                                    else "%d-%d" % (start, prev))
                        start = b
                    prev = b if b is not None else prev
                print("   %-18s [%s]" % (k, ",".join(runs)))
            else:
                print("   %s" % k)
        if args.chips:
            c = chips(bdir)
            print("   -- %d part types, %d packages"
                  % (len(c), sum(len(v) for v in c.values())))
            for part in sorted(c, key=lambda p: -len(c[p])):
                print("      %-16s x%-3d %s"
                      % (part, len(c[part]), " ".join(c[part][:8])))
        print()


if __name__ == "__main__":
    main()
