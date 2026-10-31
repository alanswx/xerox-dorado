#!/usr/bin/env python3
"""Generate the ROM images the RTL boot server and FPGA preload use.

  bol.hex       the Breath-of-Life packet body, eth_bol_loader[1..253] from
                dorado/src/ethernet.c (word 0 is the per-packet ether header,
                filled in by the server)
  bootfile.hex  an Alto boot file as big-endian 16-bit words, served over
                EFTP after the guest's Mayday (ethernet.c eth_queue_eftp_boot)
  bootfile.len  its length in words (decimal)

usage: make_boot_roms.py OUTDIR [BOOTFILE]
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.abspath(os.path.join(HERE, '..', '..'))


def bol_words():
    src = open(os.path.join(REPO, 'dorado', 'src', 'ethernet.c')).read()
    m = re.search(r'static const uint16_t eth_bol_loader\[(\d+)\]\s*=\s*\{(.*?)\};', src, re.S)
    if not m:
        raise SystemExit('eth_bol_loader not found in ethernet.c')
    n = int(m.group(1))
    body = re.sub(r'/\*.*?\*/', '', m.group(2), flags=re.S)
    words = [int(t, 0) if not t.startswith('0') or t == '0' else int(t, 8)
             for t in (x.strip() for x in body.split(',')) if t]
    if len(words) != n:
        raise SystemExit(f'eth_bol_loader: {len(words)} initializers, declared {n}')
    return words


def main():
    if len(sys.argv) < 2:
        raise SystemExit(__doc__)
    out = sys.argv[1]
    boot = sys.argv[2] if len(sys.argv) > 2 else os.path.join(REPO, 'chm', 'bootfiles', 'Galaxian.boot!1')
    os.makedirs(out, exist_ok=True)

    bol = bol_words()
    with open(os.path.join(out, 'bol.hex'), 'w') as f:
        for w in bol[1:]:
            f.write(f'{w & 0xFFFF:04x}\n')

    data = open(boot, 'rb').read()
    if len(data) % 2:
        data += b'\0'
    words = [(data[i] << 8) | data[i + 1] for i in range(0, len(data), 2)]
    with open(os.path.join(out, 'bootfile.hex'), 'w') as f:
        for w in words:
            f.write(f'{w:04x}\n')
    with open(os.path.join(out, 'bootfile.len'), 'w') as f:
        f.write(f'{len(words)}\n')
    print(f'bol.hex: {len(bol) - 1} words; bootfile.hex: {len(words)} words from {boot}')


if __name__ == '__main__':
    main()
