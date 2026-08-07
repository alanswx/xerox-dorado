#!/usr/bin/env python3
"""Count black pixels inside a rectangle of a headless framebuffer dump.

A full-screen pixel count cannot see a small change: the Interlisp desktop is
~209,000 px and deleting one word from the Exec line moves 145 of them, which
is inside the noise of a cursor blink.  Gates that care about one line of text
need to look at that line.

    pgm_ink.py shot.pgm X0 Y0 X1 Y1     # half-open box, prints one integer

Companion to pgm_text.py, which reads the glyphs when the count is not enough.
"""

import sys


def main(argv):
    if len(argv) != 6:
        sys.exit("usage: pgm_ink.py FILE.pgm X0 Y0 X1 Y1")
    path = argv[1]
    x0, y0, x1, y1 = (int(v) for v in argv[2:6])

    with open(path, "rb") as f:
        if f.readline().strip() != b"P5":
            sys.exit("%s: not a binary PGM (P5)" % path)
        line = f.readline()
        while line.startswith(b"#"):
            line = f.readline()
        w, h = (int(v) for v in line.split())
        f.readline()                      # maxval
        data = f.read(w * h)

    x0 = max(0, min(x0, w)); x1 = max(0, min(x1, w))
    y0 = max(0, min(y0, h)); y1 = max(0, min(y1, h))
    ink = sum(1
              for y in range(y0, y1)
              for x in range(x0, x1)
              if data[y * w + x] < 128)
    print(ink)


if __name__ == "__main__":
    main(sys.argv)
