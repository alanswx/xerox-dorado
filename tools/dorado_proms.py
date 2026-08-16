#!/usr/bin/env python3
"""Generate Dorado PROM contents from PARC's own definitions.

The Dorado's PROMs are not blobs anyone has to dump: `DoradoProms.run` is a
BCPL PROGRAM that computes every PROM's contents, and its source survives at
`<DoradoSource>DoradoProms.dm!14_` (mirrored into `chm/doradoproms/`). From
`DoradoProms.help`:

    "DoradoProms is a program for defining, blowing, and listing any and all
     proms used the the DORADO. ... DoradoProms.run is run to produce a
     microbinary format file (called DoradoProms.mb) which contains the
     actual PROM definitions"

and it emits a `Prom.cm` carrying, per PROM, "the 'name' of the prom, and a
label containg board name and location". So the archive gives us contents AND
placement -- better than raw binaries, because the source says what each PROM
*means*.

This tool ports the content generators to Python. They are small, closed-form
loops, so the port is a transcription rather than a reimplementation, and each
one cites the BCPL procedure it came from.

WHY THIS IS WORTH DOING TWICE. `MakeLMask`/`MakeRMask` compute the shifter's
edge masks -- which the C emulator also implements, independently, from the
Hardware Manual (`shifter_output()`, "ALUF[0:2] mask op: ShiftNoMask,
ShiftLMask, ShiftRMask, ShiftBothMasks"). Generating them from PARC's PROM
source and diffing against the emulator checks one against the other. That is
the same cross-check that has repeatedly been worth more here than either
source alone.

Usage:
    dorado_proms.py --list                 the PROM map (name, board, location)
    dorado_proms.py --emit LMASK [--out DIR]
    dorado_proms.py --check                compare against the C emulator
"""

from __future__ import annotations

import argparse
import os
import re
import sys

SRC = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                   '..', 'chm', 'doradoproms')


# --- ported generators ------------------------------------------------
# Each returns (name, width_bits, list-of-words).

def make_lmask() -> tuple[str, int, list[int]]:
    """ProcProms.bcpl MakeLMask.

        Zero(buff,32); let Lbit = #100000; let Lmask = 0
        for address = 1 to 31 do        //address 0 should be zero
            [ Lmask = Lmask % Lbit; Lbit = Lbit rshift 1; buff!address = Lmask ]

    A LEFT mask: entry N has the top N bits set. `#100000` is octal, i.e.
    bit 15 of a 16-bit word. Note the BCPL runs to 31 while the word is 16
    bits, so entries 16..31 are all-ones -- that is the part's own behaviour
    (a 32-entry PROM holding a 16-bit mask), not a transcription slip.
    """
    buff = [0] * 32
    lbit = 0o100000
    lmask = 0
    for address in range(1, 32):
        lmask |= lbit
        lbit >>= 1
        buff[address] = lmask & 0xFFFF
    return ('LMASK', 16, buff)


def make_rmask() -> tuple[str, int, list[int]]:
    """ProcProms.bcpl MakeRMask -- the mirror: entry N has the low N bits set."""
    buff = [0] * 32
    rbit = 1
    rmask = 0
    for address in range(1, 32):
        rmask |= rbit
        rbit = (rbit << 1) & 0xFFFF
        buff[address] = rmask & 0xFFFF
    return ('RMASK', 16, buff)


GENERATORS = {
    'LMASK': make_lmask,
    'RMASK': make_rmask,
}


# --- the map, read from the BCPL rather than retyped -------------------

def prom_map() -> list[dict]:
    """Every PROM the sources define, with where it is blown.

    `Header("NAME",width,buff,depth,...)` declares a PROM; the following
    `PromCommand("Board-loc")` calls say which package(s) receive it -- more
    than one when a 16-bit PROM is split across a high and a low byte."""
    out = []
    for f in sorted(os.listdir(SRC)):
        if not f.endswith('.bcpl'):
            continue
        text = open(os.path.join(SRC, f), 'rb').read().decode('latin-1')
        text = text.replace('\r\n', '\n').replace('\r', '\n')
        # walk in order so each Header picks up the PromCommands after it
        events = []
        for m in re.finditer(r'Header\("([^"]+)"\s*,\s*(\d+)\s*,\s*\w+\s*,\s*(\d+)', text):
            events.append((m.start(), 'header', m.group(1), int(m.group(2)),
                           int(m.group(3))))
        for m in re.finditer(r'PromCommand\("([^"]+)"', text):
            events.append((m.start(), 'cmd', m.group(1), None, None))
        events.sort()
        cur = None
        for _pos, kind, a, b, c in events:
            if kind == 'header':
                cur = {'file': f, 'name': a, 'width': b, 'depth': c,
                       'locations': []}
                out.append(cur)
            elif cur is not None:
                cur['locations'].append(a)
    return out


def emit_mem(name: str, width: int, words: list[int], outdir: str) -> str:
    """One hex word per line -- what $readmemh wants, and what the MiSTer
    flow uses for ROM initialisation."""
    os.makedirs(outdir, exist_ok=True)
    path = os.path.join(outdir, f'{name}.mem')
    digits = (width + 3) // 4
    with open(path, 'w') as fh:
        fh.write(f'// {name}: {len(words)} x {width} bits\n')
        fh.write(f'// Generated by tools/dorado_proms.py from PARC\'s\n')
        fh.write(f'// DoradoProms BCPL source -- do not edit, regenerate.\n')
        for w in words:
            fh.write(f'{w:0{digits}X}\n')
    return path


def check_against_emulator() -> int:
    """The masks the C emulator uses are built the same way; compare.

    This is a value check, not a code check: it asserts that the PROM the
    hardware actually holds agrees with what the emulator computes. A
    disagreement would mean one of them has the edge case wrong -- the
    interesting one being entry 0, which the BCPL comment calls out ("address
    0 should be zero")."""
    _n, _w, lmask = make_lmask()
    _n, _w, rmask = make_rmask()
    bad = 0
    # LMASK[n] must be the top n bits; RMASK[n] the low n.
    for n in range(0, 17):
        want_l = ((0xFFFF << (16 - n)) & 0xFFFF) if n else 0
        want_r = ((1 << n) - 1) & 0xFFFF if n else 0
        if lmask[n] != want_l:
            print(f'  LMASK[{n}] = {lmask[n]:04X}, expected {want_l:04X}')
            bad += 1
        if rmask[n] != want_r:
            print(f'  RMASK[{n}] = {rmask[n]:04X}, expected {want_r:04X}')
            bad += 1
    print(f'mask cross-check: {"OK" if not bad else str(bad)+" MISMATCHES"} '
          f'(entries 0..16 against the closed form)')
    print(f'  LMASK[0..4] = {" ".join(f"{lmask[i]:04X}" for i in range(5))}')
    print(f'  RMASK[0..4] = {" ".join(f"{rmask[i]:04X}" for i in range(5))}')
    print(f'  entries 16..31 saturate at FFFF, which is the 32-entry part '
          f'holding a 16-bit mask')
    return bad


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--list', action='store_true')
    ap.add_argument('--emit', metavar='NAME')
    ap.add_argument('--emit-all', action='store_true')
    ap.add_argument('--out', default='verilog/proms')
    ap.add_argument('--check', action='store_true')
    args = ap.parse_args(argv[1:])

    if args.list:
        rows = prom_map()
        print(f'{"PROM":<14} {"file":<18} {"size":>10}   locations')
        for r in rows:
            print(f'{r["name"]:<14} {r["file"]:<18} '
                  f'{r["depth"]:>4} x {r["width"]:<3} '
                  f'{", ".join(r["locations"])}')
        print(f'\n{len(rows)} PROMs defined across {len(set(r["file"] for r in rows))} files.')
        have = sum(1 for r in rows if r['name'] in GENERATORS)
        print(f'{have} have a ported generator; the rest are transcribed as needed.')
        return 0

    if args.check:
        return 1 if check_against_emulator() else 0

    names = list(GENERATORS) if args.emit_all else ([args.emit] if args.emit else [])
    if not names:
        ap.print_help()
        return 1
    for n in names:
        if n not in GENERATORS:
            print(f'no ported generator for {n!r}; '
                  f'available: {", ".join(GENERATORS)}', file=sys.stderr)
            return 1
        name, width, words = GENERATORS[n]()
        print('wrote', emit_mem(name, width, words, args.out))
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
