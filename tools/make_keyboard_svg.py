#!/usr/bin/env python3
"""Draw the Alto/Dorado keyboard map as an SVG.

Two sources, kept separate on purpose:

* The word/bit label under each keytop is Alto Hardware Manual Figure 6 (doc
  p.27 = PDF p.34) -- four 16-bit words at KBDAD = 177034B, Xerox bit
  numbering (bit 0 = MSB), depressed = 0.  It is also what
  dorado/src/display.c's key_map implements, and the audit that produced this
  file checked all 61 entries against Figure 6, ContrAlto and Cedar's
  TerminalDefs.mesa KeyName.
* The physical ARRANGEMENT follows the placement notes in TerminalDefs.mesa
  ("CTRL (large key left of A)", "LOCK (Alto lower left corner)", "LF (upper
  right)", 'Spare1 "Look" (blank key right of BS)', and so on) over the
  standard QWERTY body.  It is a reconstruction, not a traced photograph.

The 61 keys below are the whole keyboard: "The Alto keyboard contains 61
keys" (HW Manual 5.1), and this table has exactly 61 entries, which is the
cheapest check that nothing is missing.

    make_keyboard_svg.py OUT.svg
"""

import sys

U = 62          # one key unit, px
GAP = 5
PAD = 26
ROW_H = 62

# (keytop, word, xerox_bit, width_units, host_key_or_None)
ROWS = [
    [("ESC", 2, 1, 1, None), ("1", 2, 0, 1, None), ("2", 1, 1, 1, None),
     ("3", 1, 0, 1, None), ("4", 0, 1, 1, None), ("5", 0, 0, 1, None),
     ("6", 0, 2, 1, None), ("7", 0, 4, 1, None), ("8", 3, 5, 1, None),
     ("9", 1, 6, 1, None), ("0", 0, 8, 1, None), ("-", 0, 10, 1, None),
     ("=", 3, 11, 1, None), ("←", 2, 13, 1, "Backquote"), ("DEL", 2, 14, 1, None),
     ("LF", 0, 14, 1, "F6"), ("BS", 0, 15, 1, None), ("Look", 1, 15, 1, "F2")],

    [("TAB", 2, 2, 1.5, None), ("Q", 1, 3, 1, None), ("W", 1, 2, 1, None),
     ("E", 0, 3, 1, None), ("R", 3, 0, 1, None), ("T", 3, 1, 1, None),
     ("Y", 3, 3, 1, None), ("U", 0, 6, 1, None), ("I", 1, 7, 1, None),
     ("O", 1, 9, 1, None), ("P", 0, 11, 1, None), ("[", 3, 10, 1, None),
     ("]", 1, 13, 1, None), ("\\", 0, 13, 1, None),
     ("RETURN", 2, 12, 1.5, None), ("Next", 1, 14, 1, "F3")],

    [("CTRL", 2, 4, 2, None), ("A", 1, 5, 1, None), ("S", 1, 4, 1, None),
     ("D", 0, 5, 1, None), ("F", 2, 3, 1, None), ("G", 3, 2, 1, None),
     ("H", 3, 4, 1, None), ("J", 2, 6, 1, None), ("K", 0, 9, 1, None),
     ("L", 1, 10, 1, None), (";", 2, 11, 1, None), ("'", 1, 12, 1, None)],

    [("SHIFT", 2, 9, 2.5, None), ("Z", 2, 8, 1, None), ("X", 1, 8, 1, None),
     ("C", 2, 5, 1, None), ("V", 0, 7, 1, None), ("B", 2, 7, 1, None),
     ("N", 3, 6, 1, None), ("M", 3, 7, 1, None), (",", 1, 11, 1, None),
     (".", 2, 10, 1, None), ("/", 0, 12, 1, None),
     ("SHIFT", 3, 12, 2, None), ("Swat", 3, 13, 1, "F4")],

    [("LOCK", 3, 8, 2, "CapsLock"), ("SPACE", 3, 9, 10, None)],
]

INK = "#1b1f24"
SUB = "#6b7580"
HOST = "#1a5fb4"
FILL = "#ffffff"
EDGE = "#aab2bb"
BG = "#f4f6f8"


def esc(s):
    return (s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")
             .replace('"', "&quot;"))


def main(argv):
    if len(argv) != 2:
        sys.exit("usage: make_keyboard_svg.py OUT.svg")

    width = int(PAD * 2 + 18 * (U + GAP) - GAP)
    height = PAD * 2 + len(ROWS) * ROW_H + 66

    out = ['<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="%d" '
           'viewBox="0 0 %d %d" font-family="ui-sans-serif,Helvetica,Arial,'
           'sans-serif">' % (width, height, width, height)]
    out.append('<rect width="%d" height="%d" rx="10" fill="%s"/>'
               % (width, height, BG))
    out.append('<text x="%d" y="30" font-size="15" font-weight="600" '
               'fill="%s">Alto keyboard as the Dorado presents it — '
               'four active-low words at KBDAD = 177034B</text>'
               % (PAD, INK))
    out.append('<text x="%d" y="50" font-size="12" fill="%s">'
               'Small grey label is word.bit in Xerox numbering (bit 0 = MSB, '
               'depressed = 0). Blue is the host key where it differs.'
               '</text>' % (PAD, SUB))

    n = 0
    for r, row in enumerate(ROWS):
        y = PAD + 44 + r * ROW_H
        x = PAD
        if r == 4:
            x = PAD + int(2.5 * (U + GAP))
        for (top, word, bit, w, host) in row:
            kw = int(w * U + (w - 1) * GAP)
            out.append('<rect x="%d" y="%d" width="%d" height="%d" rx="7" '
                       'fill="%s" stroke="%s"/>'
                       % (x, y, kw, ROW_H - GAP * 2, FILL, EDGE))
            size = 15 if len(top) <= 2 else 11
            out.append('<text x="%d" y="%d" font-size="%d" font-weight="600" '
                       'text-anchor="middle" fill="%s">%s</text>'
                       % (x + kw // 2, y + 22, size, INK, esc(top)))
            out.append('<text x="%d" y="%d" font-size="10" '
                       'text-anchor="middle" fill="%s">%d.%d</text>'
                       % (x + kw // 2, y + 37, SUB, word, bit))
            if host:
                out.append('<text x="%d" y="%d" font-size="10" '
                           'font-weight="600" text-anchor="middle" '
                           'fill="%s">%s</text>'
                           % (x + kw // 2, y + 49, HOST, esc(host)))
            x += kw + GAP
            n += 1

    ly = PAD + 44 + len(ROWS) * ROW_H + 16
    out.append('<text x="%d" y="%d" font-size="12" fill="%s">'
               'Mouse (UTILIN, 177033B, also active low): '
               'bit 13 Red = left, bit 14 Blue = right, bit 15 Yellow = '
               'middle. %d keys, which is the whole keyboard.'
               '</text>' % (PAD, ly, SUB, n))
    out.append('</svg>')

    with open(argv[1], "w") as f:
        f.write("\n".join(out) + "\n")
    print("wrote %s (%d keys)" % (argv[1], n))


if __name__ == "__main__":
    main(sys.argv)
