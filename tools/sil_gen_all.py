#!/usr/bin/env python3
"""Generate Verilog for every Dorado board found under a Sil tree."""
import argparse, os, subprocess, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sil_netlist import find_boards

def main(argv):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--sil', default='chm/sil')
    ap.add_argument('--dict', required=True)
    ap.add_argument('--out', default='verilog/generated')
    ap.add_argument('--cells', default=None)
    a = ap.parse_args(argv[1:])
    here = os.path.dirname(os.path.abspath(__file__))
    cells = a.cells or os.path.join(os.path.dirname(a.out), 'cells')
    os.makedirs(a.out, exist_ok=True)
    n = 0
    for wl, lc in find_boards(a.sil):
        name = os.path.basename(wl)[:-3]
        rc = subprocess.call([sys.executable,
                              os.path.join(here, 'sil_to_verilog.py'), wl,
                              '--dict', a.dict, '--cells', cells,
                              '-o', os.path.join(a.out, name + '.v')],
                             stdout=subprocess.DEVNULL)
        if rc == 0:
            n += 1
        else:
            print(f'  FAILED {name}', file=sys.stderr)
    print(f'generated {n} board(s) into {a.out}')
    return 0

if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
