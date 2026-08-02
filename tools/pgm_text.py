#!/usr/bin/env python3
"""Read text off a headless Dorado framebuffer dump.

The emulator's only output in a gate is a .pgm and a pixel count, and a pixel
count cannot tell "Loading {DSK}INIT.LCOM" from "File not found".  Reading the
screen by hand -- segmenting glyphs out of an ASCII dump and matching them
against remembered letter shapes -- worked, but it is slow and not something a
gate can do.  This does the same thing mechanically.

The guest fonts are bitmaps, drawn without antialiasing at a fixed size, so a
glyph is an exact bit pattern: the same letter is byte-identical everywhere it
appears.  That makes recognition a dictionary lookup rather than OCR, provided
the dictionary has been taught the font.  `--learn` teaches it from a line
whose text is already known.

  # what changed on screen relative to a baseline, as text
  pgm_text.py after.pgm --diff before.pgm

  # teach the font from a line you have decoded by hand
  pgm_text.py shot.pgm --rows 261,272 --learn "File not found {DSK}BANNER.DCOM"

Unknown glyphs print as '?' and are dumped with --show-unknown so they can be
identified once and learned.  The font table is JSON next to this script.
"""

import argparse
import json
import os
import sys

FONT_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                         "pgm_text_font.json")


def read_pgm(path):
    """Return (width, height, pixels) from a binary P5 .pgm."""
    data = open(path, "rb").read()
    fields, i = [], 0
    while len(fields) < 4:
        while data[i:i + 1].isspace():
            i += 1
        if data[i:i + 1] == b"#":                      # comment to end of line
            while data[i:i + 1] != b"\n":
                i += 1
            continue
        j = i
        while not data[j:j + 1].isspace():
            j += 1
        fields.append(data[i:j])
        i = j
    i += 1                                             # single whitespace byte
    if fields[0] != b"P5":
        raise SystemExit("%s: not a binary PGM (P5)" % path)
    w, h = int(fields[1]), int(fields[2])
    return w, h, data[i:i + w * h]


def ink(px, w, x, y):
    return px[y * w + x] < 128


def changed_bands(w, h, a, b, min_y=0):
    """Row spans where two frames differ, split at gaps of more than 3 rows.

    Text lands in a few tight bands; the gaps between them are line spacing.
    min_y skips the mouse cursor, which is small, near the top, and moves
    whenever a click is scripted.
    """
    rows = [y for y in range(min_y, h)
            if any(a[y * w + x] != b[y * w + x] for x in range(w))]
    if not rows:
        return []
    bands, cur = [], [rows[0]]
    for y in rows[1:]:
        if y - cur[-1] <= 3:
            cur.append(y)
        else:
            bands.append((cur[0], cur[-1]))
            cur = [y]
    bands.append((cur[0], cur[-1]))
    return bands


def text_lines(px, w, y0, y1, x0, x1):
    """Split a band into individual text lines on blank rows.

    A band comes from diffing two frames, and consecutive lines of a reply
    print close enough together that the whole reply arrives as one band.  A
    glyph must be segmented within ONE line: column-projecting across four
    stacked lines merges the 'L' of line 1 with whatever sits under it, and
    nothing matches the font.  Every row inside a line has ink somewhere, so
    blank rows are the line breaks.
    """
    out, start = [], None
    for y in range(y0, y1 + 1):
        lit = any(ink(px, w, x, y) for x in range(x0, x1))
        if lit and start is None:
            start = y
        elif not lit and start is not None:
            out.append((start, y - 1))
            start = None
    if start is not None:
        out.append((start, y1))
    return out


def segment(px, w, y0, y1, x0, x1):
    """Split a row band into glyph bitmaps plus the gap preceding each.

    A column is "on" if any row in the band has ink.  Two or more blank columns
    end a glyph: one blank column is ordinary intra-glyph spacing in these
    fonts (the gap inside an 'n', say), so splitting on a single blank would
    cut letters in half.
    """
    on = [any(ink(px, w, x, y) for y in range(y0, y1 + 1))
          for x in range(x0, x1)]
    spans, start, blank = [], None, 0
    for i, lit in enumerate(on):
        if lit:
            if start is None:
                start = i
            blank = 0
        elif start is not None:
            blank += 1
            if blank >= 2:
                spans.append((start, i - blank + 1))
                start = None
    if start is not None:
        spans.append((start, len(on)))

    out, prev_end = [], None
    for a, b in spans:
        bitmap = "/".join(
            "".join("#" if ink(px, w, x0 + x, y) else "."
                    for x in range(a, b))
            for y in range(y0, y1 + 1))
        out.append({"gap": 0 if prev_end is None else a - prev_end,
                    "bitmap": trim(bitmap)})
        prev_end = b
    return out


def trim(bitmap):
    """Drop all-blank rows so the same letter matches across band heights.

    Bands are found by diffing, so their vertical extent depends on what else
    changed on that line; without trimming, an 'e' from a 12-row band and an
    'e' from a 10-row band would be different dictionary keys.
    """
    rows = bitmap.split("/")
    while rows and "#" not in rows[0]:
        rows.pop(0)
    while rows and "#" not in rows[-1]:
        rows.pop()
    return "/".join(rows)


def load_font():
    if os.path.exists(FONT_PATH):
        return json.load(open(FONT_PATH))
    return {}


def render(glyphs, font, space_gap=4):
    text, unknown = "", []
    for g in glyphs:
        if g["gap"] >= space_gap:
            text += " "
        ch = font.get(g["bitmap"])
        if ch is None:
            text += "?"
            unknown.append(g["bitmap"])
        else:
            text += ch
    return text, unknown


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("pgm")
    ap.add_argument("--diff", metavar="BASELINE",
                    help="only report bands that differ from this frame")
    ap.add_argument("--rows", metavar="Y0,Y1", help="explicit row band")
    ap.add_argument("--cols", metavar="X0,X1", default=None)
    ap.add_argument("--min-y", type=int, default=100,
                    help="ignore changes above this row (mouse cursor)")
    ap.add_argument("--space-gap", type=int, default=4,
                    help="column gap that counts as a space")
    ap.add_argument("--learn", metavar="TEXT",
                    help="record glyph->char for one band of known text")
    ap.add_argument("--show-unknown", action="store_true",
                    help="dump the bitmap of every unrecognised glyph")
    args = ap.parse_args()

    w, h, px = read_pgm(args.pgm)
    x0, x1 = (0, w) if not args.cols else map(int, args.cols.split(","))

    if args.rows:
        y0, y1 = map(int, args.rows.split(","))
        bands = [(y0, y1)]
    elif args.diff:
        bw, bh, base = read_pgm(args.diff)
        if (bw, bh) != (w, h):
            raise SystemExit("frame sizes differ: %dx%d vs %dx%d"
                             % (w, h, bw, bh))
        bands = changed_bands(w, h, base, px, args.min_y)
        if not bands:
            print("(no change)")
            return 0
    else:
        raise SystemExit("give --rows or --diff")

    font = load_font()

    if args.learn:
        if len(bands) != 1:
            raise SystemExit("--learn needs exactly one band; got %d" % len(bands))
        glyphs = segment(px, w, bands[0][0], bands[0][1], x0, x1)
        want = [c for c in args.learn if c != " "]
        if len(glyphs) != len(want):
            print("%d glyphs but %d non-space chars; nothing learned"
                  % (len(glyphs), len(want)), file=sys.stderr)
            for g in glyphs:
                print("  gap=%d" % g["gap"], file=sys.stderr)
                for r in g["bitmap"].split("/"):
                    print("    " + r, file=sys.stderr)
            return 1
        for g, ch in zip(glyphs, want):
            font[g["bitmap"]] = ch
        json.dump(font, open(FONT_PATH, "w"), indent=0, sort_keys=True)
        print("learned %d glyphs (font now %d)" % (len(want), len(font)))
        return 0

    unknown_all = []
    for by0, by1 in bands:
        for y0, y1 in text_lines(px, w, by0, by1, x0, x1):
            glyphs = segment(px, w, y0, y1, x0, x1)
            text, unknown = render(glyphs, font, args.space_gap)
            unknown_all += unknown
            print("%4d-%-4d %s" % (y0, y1, text))

    if args.show_unknown and unknown_all:
        print("\n%d unrecognised glyph(s):" % len(unknown_all), file=sys.stderr)
        for bm in dict.fromkeys(unknown_all):
            print("---", file=sys.stderr)
            for r in bm.split("/"):
                print("  " + r, file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
