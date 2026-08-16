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



def make_keyboard_map() -> tuple[str, int, list[int]]:
    """IFUProms.bcpl MakeKeyboardMap -- 128 x 8, at IFU-k05 (+ l05 right nibble).

    This is the Dorado's OWN keyboard map, and therefore the authority on it.
    The BCPL builds it in three steps:

        Map = table [ 08;00;16;29;... ]        // 64 entries, addr -> value
        for val 0..63:  buff[val] = the addr where Map[addr] == val   // INVERT
        for val 64..127: buff[val] = (val-64) xor #60
        for val 0..127:  buff[val] = buff[val] xor #77

    Note the inversion: the table is written as addr->value and the PROM holds
    value->addr. Both XORs are octal (#60 = 48, #77 = 63)."""
    Map = [
        8, 0, 16, 29, 36, 30, 37, 11,
        52, 46, 60, 53, 63, 49, 5, 61,
        28, 20, 21, 25, 22, 26, 56, 45,
        31, 57, 58, 59, 62, 41, 51, 33,
        24, 12, 13, 2, 14, 3, 38, 19,
        23, 27, 55, 54, 34, 50, 4, 6,
        1, 9, 10, 17, 18, 44, 39, 47,
        15, 43, 40, 32, 35, 48, 7, 42,
    ]
    buff = [0] * 128
    for val in range(64):
        for addr in range(64):
            if Map[addr] == val:
                buff[val] = addr
    for val in range(64, 128):
        buff[val] = (val - 64) ^ 0o60
    for val in range(128):
        buff[val] ^= 0o77
    return ('Keyboard-Map', 8, buff)


def make_mouse_motion() -> tuple[str, int, list[int]]:
    """IFUProms.bcpl MakeMouseMotion -- 256 x 4, at IFU-i03.

    A QUADRATURE DECODER in a PROM. The address is the four mouse phase
    signals plus their delayed copies, so the PROM sees a transition and
    reports which way each axis moved. The source's own encoding:

        Prom=0: Y=0    X=0        Prom=3: Y=Y+1  X=0     Prom=6: Y=Y-1  X=0
        Prom=1: Y=0    X=X+1      Prom=4: Y=Y+1  X=X+1   Prom=7: Y=Y-1  X=X+1
        Prom=2: Y=0    X=X-1      Prom=5: Y=Y+1  X=X-1   Prom=8: Y=Y-1  X=X-1

    i.e. value = 3*Ydir + Xdir with each direction in {0 none, 1 up, 2 down}.

    Bit order comes from the BCPL structure, which is MSB-first over the low
    byte: MX1=0x80 MX2=0x40 MY1=0x20 MY2=0x10, then the delayed copies
    MX1dly=0x08 MX2dly=0x04 MY1dly=0x02 MY2dly=0x01."""
    MX1, MX2, MY1, MY2 = 0x80, 0x40, 0x20, 0x10
    MX1d, MX2d, MY1d, MY2d = 0x08, 0x04, 0x02, 0x01
    buff = [0] * 256
    for addr in range(256):
        b = lambda m: 1 if (addr & m) else 0
        xdir = 0
        if b(MX1) != b(MX1d):
            xdir = (b(MX1) ^ b(MX2)) + 1
        if b(MX2) != b(MX2d):
            xdir = (b(MX1) ^ b(MX2) ^ 1) + 1
        ydir = 0
        if b(MY1) != b(MY1d):
            ydir = (b(MY1) ^ b(MY2)) + 1
        if b(MY2) != b(MY2d):
            ydir = (b(MY1) ^ b(MY2) ^ 1) + 1
        buff[addr] = (3 * ydir + xdir) & 0xF
    return ('Mouse-Motion', 4, buff)


def make_data_select() -> tuple[str, int, list[int]]:
    """IFUProms.bcpl MakeDataSelect -- 32 x 8, at IFU-a06.

    A literal table (octal in the source): the data-select control per
    instruction type."""
    T = [0o377, 0o377, 0o377, 0o377, 0o377, 0o367, 0o263, 0o221,
         0o377, 0o377, 0o377, 0o377, 0o377, 0o377, 0o225, 0o204,
         0o377, 0o377, 0o377, 0o377, 0o377, 0o377, 0o167, 0o063,
         0o377, 0o377, 0o377, 0o377, 0o377, 0o377, 0o073, 0o031]
    return ('Data-Select', 8, list(T))


# --- MemX: the memory system's control PROMs --------------------------
# All seven are literal tables in MemProms.bcpl, and their ROW COMMENTS are
# the documentation: each 8-entry row is one memory operation, so the tables
# read as a state machine indexed by (operation, cycle).

def _table(name, width, rows) -> tuple[str, int, list[int]]:
    words = [w for _label, row in rows for w in row]
    return (name, width, words)


def make_st() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeST -- 32 x 16, MemX-h11 (left half) + i12 (right).

    The STATE memory: the memory system's sequencer. Its two halves are
    commented "11 cycles IO store" and "12 cycles Victim store" -- i.e. the
    cycle counts for a store that goes to I/O versus one that has to evict a
    dirty victim line first."""
    return _table('ST', 16, [
        ('11 cycles IO store', [
            0o052600, 0o012600, 0o052600, 0o012600, 0o056600, 0o016600,
            0o056200, 0o136000, 0o177000, 0o175000, 0o063600, 0o063600,
            0o177000, 0o177000, 0o177000, 0o177000]),
        ('12 cycles Victim store', [
            0o063406, 0o052406, 0o012406, 0o052406, 0o012406, 0o056406,
            0o016406, 0o056404, 0o136000, 0o177000, 0o175000, 0o173002,
            0o177000, 0o177000, 0o177000, 0o177000]),
    ])


def make_ec1() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeEC1 -- 32 x 8, MemX-l12.

    Row comments name the four cache operations the memory system
    distinguishes, which is the cache state machine in four lines."""
    return _table('EC-1', 8, [
        ('Write',                  [0o003, 0o207, 0o203, 0o203, 0o203, 0o202, 0o201, 0o203]),
        ('Cache Load',             [0o003, 0o207, 0o313, 0o313, 0o313, 0o312, 0o201, 0o203]),
        ('Clean or miss IO fetch', [0o003, 0o207, 0o203, 0o223, 0o223, 0o222, 0o201, 0o203]),
        ('Dirty hit IO fetch',     [0o003, 0o207, 0o303, 0o343, 0o363, 0o362, 0o201, 0o203]),
    ])


def make_ec2() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeEC2 -- 32 x 8, MemX-l11. Same four operations."""
    return _table('EC-2', 8, [
        ('Write',                  [0o005, 0o001, 0o001, 0o001, 0o001, 0o001, 0o011, 0o001]),
        ('Cache Load',             [0o226, 0o222, 0o222, 0o002, 0o000, 0o000, 0o011, 0o222]),
        ('Clean or miss IO fetch', [0o045, 0o041, 0o041, 0o041, 0o001, 0o001, 0o011, 0o041]),
        ('Dirty hit IO fetch',     [0o347, 0o343, 0o343, 0o143, 0o041, 0o001, 0o011, 0o343]),
    ])


def make_map_mem() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeMapMem -- 32 x 8, MemX-i14.

    Rows: Refresh / Read / Write / Map write -- the four things the MAP can be
    doing to storage."""
    return _table('Map-Mem', 8, [
        ('Refresh',   [0o12, 0o12, 0o12, 0o46, 0o12, 0o12, 0o10, 0o12]),
        ('Read',      [0o12, 0o12, 0o13, 0o07, 0o12, 0o12, 0o10, 0o12]),
        ('Write',     [0o12, 0o12, 0o13, 0o07, 0o12, 0o12, 0o10, 0o12]),
        ('Map write', [0o12, 0o12, 0o13, 0o13, 0o12, 0o12, 0o10, 0o12]),
    ])


def make_map_map() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeMapMap -- 32 x 8, MemX-g15. Same four rows."""
    return _table('Map-Map', 8, [
        ('Refresh',   [0o037, 0o037, 0o037, 0o235, 0o275, 0o337, 0o327, 0o007]),
        ('Read',      [0o007, 0o007, 0o007, 0o005, 0o045, 0o107, 0o127, 0o007]),
        ('Write',     [0o007, 0o007, 0o007, 0o004, 0o044, 0o107, 0o127, 0o007]),
        ('Map write', [0o007, 0o007, 0o007, 0o000, 0o040, 0o107, 0o127, 0o007]),
    ])


def make_mem16() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeMem16 -- 32 x 8, MX16k-j13.

    DRAM timing for 16K parts: read/write, idle, refresh, idle."""
    return _table('16k-Mem', 8, [
        ('Read or write', [0o353, 0o053, 0o153, 0o373, 0o352, 0o340, 0o306, 0o303]),
        ('Idle state',    [0o306, 0o306, 0o306, 0o306, 0o306, 0o306, 0o306, 0o306]),
        ('Refresh',       [0o303, 0o303, 0o303, 0o303, 0o302, 0o300, 0o306, 0o303]),
        ('Idle state',    [0o306, 0o306, 0o306, 0o306, 0o306, 0o306, 0o306, 0o306]),
    ])


def make_mem4() -> tuple[str, int, list[int]]:
    """MemProms.bcpl MakeMem4 -- 32 x 8, MX4k-j14. RETIRED.

    DRAM timing for 4K parts. The generator survives but its call site is
    COMMENTED OUT, and the file header says why:

        "change change tomemx-16k-j13 from -j14. comment-out the memx-4k
         option.  September 26, 1979"

    So by late 1979 the memory boards were built with 16K DRAMs and the 4K
    timing PROM was no longer blown. Kept here because it is real data about
    an earlier configuration, and because it explains why the board has two
    PROM sockets (j13 and j14) for one function."""
    return _table('4k-Mem', 8, [
        ('Read or write', [0o303, 0o313, 0o113, 0o153, 0o153, 0o053, 0o153, 0o363]),
        ('(second half)', [0o302, 0o302, 0o302, 0o306, 0o303, 0o306, 0o306, 0o306]),
        ('Refresh',       [0o303, 0o303, 0o303, 0o343, 0o343, 0o343, 0o343, 0o343]),
        ('(second half)', [0o302, 0o302, 0o302, 0o306, 0o303, 0o306, 0o306, 0o306]),
    ])


GENERATORS = {
    'LMASK': make_lmask,
    'RMASK': make_rmask,
    'Keyboard-Map': make_keyboard_map,
    'Mouse-Motion': make_mouse_motion,
    'Data-Select': make_data_select,
    'ST': make_st,
    'EC-1': make_ec1,
    'EC-2': make_ec2,
    'Map-Mem': make_map_mem,
    'Map-Map': make_map_map,
    '16k-Mem': make_mem16,
    '4k-Mem': make_mem4,
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

    # Keyboard-Map: the BCPL INVERTS a 64-entry table, so the low half must
    # come out a permutation of 0..63. If the inversion or either XOR were
    # mis-transcribed, that property would break -- it is a bijection check,
    # which is much stronger than eyeballing a few entries.
    _n, _w, kb = make_keyboard_map()
    if sorted(kb[:64]) != list(range(64)):
        print('  Keyboard-Map low half is NOT a permutation of 0..63'); bad += 1
    if kb[64:] != [((v - 64) ^ 0o60) ^ 0o77 for v in range(64, 128)]:
        print('  Keyboard-Map high half does not match its formula'); bad += 1
    print(f'keyboard map: low half is a permutation of 0..63, high half '
          f'matches (val-64)^60^77')

    # Mouse-Motion: value = 3*Ydir + Xdir over {0,1,2} each, so 0..8 and no
    # more. And exactly 2**4 = 16 addresses can mean "no motion" -- the ones
    # where all four phase signals equal their delayed copies.
    _n, _w, mm = make_mouse_motion()
    if set(mm) - set(range(9)):
        print(f'  Mouse-Motion has values outside 0..8: '
              f'{sorted(set(mm) - set(range(9)))}'); bad += 1
    zeros = sum(1 for v in mm if v == 0)
    if zeros != 16:
        print(f'  Mouse-Motion has {zeros} no-motion entries, expected 16'); bad += 1
    print(f'mouse motion: values 0..8 only, and exactly 16 no-motion entries '
          f'(= the 2^4 states where nothing changed)')

    # Every generator must match the depth and width the BCPL's own Header()
    # declares. This catches a truncated or over-long table transcription,
    # which is the likeliest way a literal-table port goes wrong.
    m = {r['name']: r for r in prom_map()}
    sized = 0
    for name, fn in GENERATORS.items():
        _n, _w, words = fn()
        r = m.get(name)
        if not r:
            print(f'  {name}: no Header() in the BCPL'); bad += 1; continue
        if len(words) != r['depth']:
            print(f'  {name}: {len(words)} words, Header declares {r["depth"]}')
            bad += 1
        elif max(words) >= (1 << r['width']):
            print(f'  {name}: value {max(words):o}o exceeds {r["width"]} bits')
            bad += 1
        else:
            sized += 1
    print(f'size check: {sized}/{len(GENERATORS)} generators match the depth '
          f'and width their own Header() declares')
    print(f'ported: {len(GENERATORS)} of {len(m)} PROMs')
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
