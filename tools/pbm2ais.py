#!/usr/bin/env python3
"""Convert a PBM (P4) bitmap to a Xerox AIS raster file.

    pdftoppm -mono -f 10 -l 10 -scale-to-x 1000 -scale-to-y -1 X.pdf page
    python3 tools/pbm2ais.py page-10.pbm out.ais

AIS (Array of Intensity Samples) per AISFormat.mesa
(chm/cedar/stp-root/CedarChest6.1/AIS/AISFormat.mesa): a 2-word Header
[password=-31574, attributeLength], attribute parts each led by a packed
PartHeader word [type:6 bits, length:10 bits], a nil part terminator, the
attribute section padded to aisWordsPerPage (1024 words), then the
uncompressed (uca) raster: scanCount lines of wordsPerScanLine 16-bit
big-endian words, pixel 0 in the MSB — the same packing as PBM rows, so
scanlines copy through with row padding only. 1 = ink (largerIsDarker),
matching PBM.
"""
import struct
import sys

AIS_PASSWORD = -31574
ATTR_WORDS = 1024          # aisWordsPerPage; attributeLength must be a
                           # multiple of the 256-word Mesa page => one full
                           # AIS page keeps CreateFile/ReadAttributes happy


def pbm_read(path):
    with open(path, 'rb') as f:
        data = f.read()
    if not data.startswith(b'P4'):
        sys.exit(f'{path}: not a raw PBM (P4)')
    # Header: P4 <ws> width <ws> height <single-ws> bits
    pos, fields = 2, []
    while len(fields) < 2:
        while data[pos] in b' \t\r\n':
            pos += 1
        if data[pos:pos + 1] == b'#':
            while data[pos] not in b'\r\n':
                pos += 1
            continue
        start = pos
        while data[pos] not in b' \t\r\n':
            pos += 1
        fields.append(int(data[start:pos]))
    pos += 1                                   # the single whitespace
    w, h = fields
    row_bytes = (w + 7) // 8
    rows = [data[pos + y * row_bytes: pos + (y + 1) * row_bytes]
            for y in range(h)]
    return w, h, rows


def ais_write(path, w, h, rows):
    words_per_line = (w + 15) // 16
    attr = struct.pack('>hH', AIS_PASSWORD, ATTR_WORDS)
    # RasterPart(uca): 10 words incl. its PartHeader (type=1).
    attr += struct.pack('>HHHHHHHHhH',
                        (1 << 10) | 10,        # PartHeader[raster, 10]
                        h,                     # scanCount
                        w,                     # scanLength
                        3,                     # scanDir (as period files)
                        1,                     # samplesPerPixel
                        1,                     # codingType = uca
                        1,                     # bitsPerSample
                        words_per_line,
                        -1,                    # scanLinesPerBlock: unblocked
                        0)                     # paddingPerBlock
    attr += struct.pack('>HH', 0, 0)           # nil part: end of attributes
    attr += b'\0' * (ATTR_WORDS * 2 - len(attr))
    line_bytes = words_per_line * 2
    with open(path, 'wb') as f:
        f.write(attr)
        for row in rows:
            f.write(row.ljust(line_bytes, b'\0'))
    print(f'{path}: {w}x{h}, {words_per_line} words/line, '
          f'{ATTR_WORDS * 2 + h * line_bytes} bytes')


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    w, h, rows = pbm_read(sys.argv[1])
    ais_write(sys.argv[2], w, h, rows)


if __name__ == '__main__':
    main()
