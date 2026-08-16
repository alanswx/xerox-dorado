#!/usr/bin/env python3
"""Gate the assembled RTL machine on something that cannot get better by luck.

WHY NOT A TOGGLE COUNT. The harness reports how many probed signals move, and
that reads like a progress bar for the cell library. It is not, and 2026-08-16
showed why: modelling four more gate types correctly, and fixing MC10105's
polarity against the datasheet, took the count DOWN from 31 to 27. Nothing
regressed -- an undriven net reads 0 and flaps with whatever else moves, while
correct logic holds it steady. A gate on that number would have punished the
fix.

What IS an invariant is the clock. The BaseBoard generates it and fans it out
to every slot, and if those nets stop moving nothing downstream can compute,
whatever the cell library says. So that is what this asserts, by name, using
the probe map the generator emits beside the RTL.

Usage:
    ./verilog/verilator/obj_dir/Vemu --headless --cycles 20000 | \\
        python3 tools/rtl_machine_check.py verilog/generated/dorado_backplane.probes
"""

from __future__ import annotations

import sys


def main(argv: list[str]) -> int:
    probes = (argv[1] if len(argv) > 1
              else 'verilog/generated/dorado_backplane.probes')
    text = sys.stdin.read()
    sys.stdout.write(text)

    if 'toggle mask' not in text:
        print('machine-check: no toggle mask in the run output')
        return 1
    mask = [int(w, 16) for w in text.split('toggle mask')[1].split()]

    names: dict[int, str] = {}
    for line in open(probes):
        if line.startswith('#') or not line.strip():
            continue
        i, name = line.split()[0], line.split()[1]
        names[int(i)] = name

    def toggled(i: int) -> bool:
        return bool((mask[i // 32] >> (i % 32)) & 1)

    clocks = {n: toggled(i) for i, n in names.items() if n.startswith('CLK.')}
    dead = sorted(n for n, ok in clocks.items() if not ok)
    total = sum(1 for i in names if toggled(i))

    print(f'machine-check: {len(clocks) - len(dead)}/{len(clocks)} clock nets '
          f'toggling, {total} signals moving overall')
    if dead:
        print(f'  FAIL: these clock nets are dead: {" ".join(dead)}')
        print('  The BaseBoard generates the clock (an MPQ3303 VCO substituted '
              'for a fabric divider,')
        print('  shaped by two MC1660s, divided by four MC1690s) and fans it '
              'out through j02. Start there.')
        return 1
    if not clocks:
        print('  FAIL: no CLK.* signals in the probe map at all')
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
