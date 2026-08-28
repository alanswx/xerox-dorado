#!/usr/bin/env python3
"""Cross-check the RTL keyboard's matrix positions against the C emulator's.

`verilog/cells/dorado_keyboard.v` maps a PS/2 scancode to a position in the
Dorado's 61-key matrix, as {word, bit}. `dorado/src/display.c`'s `key_map`
assigns the same positions to the same keys. If they ever disagree the RTL
puts a keystroke in the wrong bit and the guest sees a DIFFERENT KEY --
silently, because every value involved is plausible.

A FORMULA WAS TRIED FIRST AND IS WRONG. The enumeration order looks purely
positional -- key 1 is word 0 bit 15, key 16 is word 0 bit 0 -- and it holds
for the first 47 keys. It then breaks: word 2 carries only FIFTEEN keys, so
`(idx-1)/16` puts R in word 2 bit 0 where the real matrix starts word 3 with
it. The Alto matrix has a hole, which is exactly the sort of thing Figure 6
shows and arithmetic does not. This file exists because that was caught here
rather than by typing the wrong letter into Smalltalk.

    python3 tools/check_key_matrix.py           # verify the committed RTL
    python3 tools/check_key_matrix.py --emit    # print the case body

The host-key choice below is this project's convention; the POSITIONS come
from display.c. So `--emit` generates and this script then checks, which is
not circular at check time: it compares two files that are edited separately.
"""

import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.join(HERE, '..')
SRC = os.path.join(ROOT, 'dorado', 'src', 'display.c')
RTL = os.path.join(ROOT, 'verilog', 'cells', 'dorado_keyboard.v')

# PS/2 set 2 scancode -> the Dorado key it stands for. Extended codes are
# written as 'E0xx'. This mapping is OURS; five of the entries are pure
# convention because the key has no modern equivalent, and they are marked.
PS2 = [
    ('2E', 'DORADO_KEY_5'),        ('25', 'DORADO_KEY_4'),
    ('36', 'DORADO_KEY_6'),        ('24', 'DORADO_KEY_E'),
    ('3D', 'DORADO_KEY_7'),        ('23', 'DORADO_KEY_D'),
    ('3C', 'DORADO_KEY_U'),        ('2A', 'DORADO_KEY_V'),
    ('45', 'DORADO_KEY_0'),        ('42', 'DORADO_KEY_K'),
    ('4E', 'DORADO_KEY_MINUS'),    ('4D', 'DORADO_KEY_P'),
    ('4A', 'DORADO_KEY_FSLASH'),   ('5D', 'DORADO_KEY_BSLASH'),
    ('0C', 'DORADO_KEY_LF'),       # F4  -- convention
    ('66', 'DORADO_KEY_BS'),
    ('26', 'DORADO_KEY_3'),        ('1E', 'DORADO_KEY_2'),
    ('1D', 'DORADO_KEY_W'),        ('15', 'DORADO_KEY_Q'),
    ('1B', 'DORADO_KEY_S'),        ('1C', 'DORADO_KEY_A'),
    ('46', 'DORADO_KEY_9'),        ('43', 'DORADO_KEY_I'),
    ('22', 'DORADO_KEY_X'),        ('44', 'DORADO_KEY_O'),
    ('4B', 'DORADO_KEY_L'),        ('41', 'DORADO_KEY_COMMA'),
    ('52', 'DORADO_KEY_QUOTE'),    ('5B', 'DORADO_KEY_RBRACKET'),
    ('06', 'DORADO_KEY_BLANKMIDDLE'),  # F2 -- Next, convention
    ('05', 'DORADO_KEY_BLANKTOP'),     # F1 -- Look, convention
    ('16', 'DORADO_KEY_1'),        ('76', 'DORADO_KEY_ESC'),
    ('0D', 'DORADO_KEY_TAB'),      ('2B', 'DORADO_KEY_F'),
    ('14', 'DORADO_KEY_CTRL'),     ('21', 'DORADO_KEY_C'),
    ('3B', 'DORADO_KEY_J'),        ('32', 'DORADO_KEY_B'),
    ('1A', 'DORADO_KEY_Z'),        ('12', 'DORADO_KEY_LSHIFT'),
    ('49', 'DORADO_KEY_PERIOD'),   ('4C', 'DORADO_KEY_SEMICOLON'),
    ('5A', 'DORADO_KEY_RETURN'),
    ('0E', 'DORADO_KEY_ARROW'),    # backtick -- left arrow, convention
    ('E071', 'DORADO_KEY_DEL'),
    ('2D', 'DORADO_KEY_R'),        ('2C', 'DORADO_KEY_T'),
    ('34', 'DORADO_KEY_G'),        ('35', 'DORADO_KEY_Y'),
    ('33', 'DORADO_KEY_H'),        ('3E', 'DORADO_KEY_8'),
    ('31', 'DORADO_KEY_N'),        ('3A', 'DORADO_KEY_M'),
    ('58', 'DORADO_KEY_LOCK'),     ('29', 'DORADO_KEY_SPACE'),
    ('54', 'DORADO_KEY_LBRACKET'), ('55', 'DORADO_KEY_PLUS'),
    ('59', 'DORADO_KEY_RSHIFT'),
    ('04', 'DORADO_KEY_BLANKBOTTOM'),  # F3 -- Swat, convention
]


def key_map():
    """{name: (word, bit)} from display.c's key_map table."""
    text = open(SRC, encoding='utf-8', errors='replace').read()
    m = re.search(r'key_map\[DORADO_KEY_LAST\]\s*=\s*\{(.*?)\n\};', text, re.S)
    if not m:
        sys.exit('display.c: could not find the key_map table')
    out = {}
    for name, word, mask in re.findall(
            r'\[\s*(DORADO_KEY_\w+)\s*\]\s*=\s*\{\s*(-?\d+)\s*,\s*(0x[0-9A-Fa-f]+)\s*\}',
            m.group(1)):
        w, mk = int(word), int(mask, 16)
        if w < 0 or mk == 0:
            continue
        bits = [b for b in range(16) if mk & (1 << b)]
        if len(bits) != 1:
            sys.exit('display.c: %s has mask %#06x, not a single bit' % (name, mk))
        out[name] = (w, bits[0])
    return out


def wanted_lines(kmap):
    """The RTL case body this mapping implies, as {code: (word, bit, name)}."""
    out = {}
    for code, name in PS2:
        if name not in kmap:
            sys.exit('display.c has no key_map entry for %s' % name)
        w, b = kmap[name]
        out[code] = (w, b, name)
    return out


def rtl_lines():
    """{code: (word, bit, name)} as the committed RTL states them."""
    text = open(RTL, encoding='utf-8', errors='replace').read()
    out = {}
    # Each entry is written as:  8'hXX: key_pos_f = 7'b1_WW_BBBB;  // NAME
    for code, word, bit, name in re.findall(
            r"8'h([0-9A-F]{2}):\s*key_pos_f\s*=\s*7'b1_(\d{2})_(\d{4});\s*//\s*(DORADO_KEY_\w+)",
            text):
        out[code] = (int(word, 2), int(bit, 2), name)
    return out


def emit(want):
    print("      // Generated by tools/check_key_matrix.py --emit; the")
    print("      // POSITIONS come from dorado/src/display.c's key_map.")
    for code, name in PS2:
        w, b, _ = want[code]
        pad = ' ' * max(1, 26 - len(name))
        print("      %s: key_pos = 7'b1_%02s_%04s;%s// %s"
              % (("8'h" + code[2:]) if code.startswith('E0') else ("8'h" + code),
                 format(w, '02b'), format(b, '04b'), pad, name))


def main():
    kmap = key_map()
    want = wanted_lines(kmap)
    if '--emit' in sys.argv:
        emit(want)
        return

    got = rtl_lines()
    bad = 0
    for code, name in PS2:
        key = code[2:] if code.startswith('E0') else code
        if key not in got:
            print('MISSING from the RTL: PS/2 %s (%s)' % (code, name))
            bad += 1
            continue
        gw, gb, gname = got[key]
        ww, wb, _ = want[code]
        if gname != name:
            print('PS/2 %s: RTL says %s, this table says %s' % (code, gname, name))
            bad += 1
        elif (gw, gb) != (ww, wb):
            print('MISPLACED %-24s PS/2 %s -- RTL (word %d, bit %d) vs '
                  'display.c (word %d, bit %d)' % (name, code, gw, gb, ww, wb))
            bad += 1

    extra = set(got) - {(c[2:] if c.startswith('E0') else c) for c, _ in PS2}
    for c in sorted(extra):
        print('RTL has PS/2 %s which this table does not: %s' % (c, got[c][2]))
        bad += 1

    seen = {}
    for code, name in PS2:
        pos = want[code][:2]
        if pos in seen:
            print('TWO KEYS ON ONE POSITION: %s and %s both at (word %d, bit %d)'
                  % (seen[pos], name, pos[0], pos[1]))
            bad += 1
        seen[pos] = name

    print('%d keys checked against dorado/src/display.c' % len(PS2))
    if bad:
        sys.exit('check_key_matrix: %d disagreements' % bad)
    print('PASS -- every PS/2 key lands where the C emulator puts it.')
    print('  Limits: this checks PLACEMENT and uniqueness, not which host key')
    print('  should be which -- a scancode assigned to the wrong Dorado key')
    print('  passes here and types the wrong letter. The five marked')
    print('  conventions (F1-F4, backtick) have nothing to check against.')


if __name__ == '__main__':
    main()
