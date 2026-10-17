#!/usr/bin/env python3
"""Decode Cedar's three maintenance-panel digits from six cursor words.

Input is a decoded, little-endian 16-bit memory image (cursor at 0431B),
or --rows with six comma-separated hexadecimal words. This does not decode
raw FPGA DDR addressing or include dirty cache data.
"""
import argparse
import json
from pathlib import Path
import re
import struct

# ProcessorHeadDorado.mesa: six rows of four 16-bit words, MSB first.
# Each digit occupies five columns; Cedar copies three digits into the cursor.
FONT = (
    0o30614, 0o61474, 0o167461, 0o117000,
    0o45222, 0o112441, 0o512, 0o57000,
    0o54202, 0o22471, 0o141062, 0o57000,
    0o64214, 0o14405, 0o21111, 0o157000,
    0o44220, 0o117645, 0o22110, 0o57000,
    0o31736, 0o60430, 0o142063, 0o117000,
)


def decode(rows):
    digits = []
    for position in range(3):
        glyph = [(word >> (15 - position * 5 - c)) & 1
                 for word in rows for c in range(5)]
        digit = None
        for d in range(10):
            expected = [(FONT[r * 4 + (d * 5 + c) // 16]
                         >> (15 - (d * 5 + c) % 16)) & 1
                        for r in range(6) for c in range(5)]
            if glyph == expected:
                digit = d
                break
        digits.append(digit)
    return (100 * digits[0] + 10 * digits[1] + digits[2]
            if None not in digits else None)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('memory', type=Path, nargs='?')
    parser.add_argument('--rows', help='six comma-separated hex cursor words')
    parser.add_argument('--word-offset', type=lambda x: int(x, 0), default=0o431)
    args = parser.parse_args()
    if (args.memory is None) == (args.rows is None):
        parser.error('provide either a memory image or --rows')
    if args.rows is not None:
        rows = [int(x, 16) for x in args.rows.split(',')]
        if len(rows) != 6 or any(not 0 <= x <= 0xffff for x in rows):
            parser.error('--rows requires six 16-bit hex words')
    else:
        with args.memory.open('rb') as f:
            f.seek(2 * args.word_offset)
            data = f.read(12)
        if len(data) != 12:
            parser.error('memory image does not contain the six cursor words')
        rows = struct.unpack('<6H', data)
    code = decode(rows)
    names = Path(__file__).resolve().parents[1] / (
        'chm/cedar/stp-root/Cedar6.1/HeadsCommon/MPCodes.mesa')
    labels = {}
    if names.exists():
        labels = {int(value): name for name, value in re.findall(
            r'(\w+):\s*Code\s*=\s*(\d+)\s*;', names.read_text(errors='replace'))}
    print(json.dumps({'rows': [f'{x:04x}' for x in rows], 'code': code,
                      'name': labels.get(code), 'digits_valid': code is not None},
                     indent=2))


if __name__ == '__main__':
    main()
