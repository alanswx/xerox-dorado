#!/usr/bin/env python3
"""Decode original BaseBoard BootBlocks into an RTL component-test fixture.

Input is `dorado/build/baseboard_rom_dump ROM.mb` output. This is not a
native-boot state image: direct-entry tests still need explicit bench setup.
An optional im_image fixture supplies ALUFM only, never its IM or IFUM.
"""
import argparse
from pathlib import Path


def decode(rom, header):
    def word(address):
        return rom[address] | rom[address + 1] << 8

    data, origin, count = (word(header + offset) for offset in (0, 2, 4))
    if origin + count * 4 > 4096 or data + count * 17 > 65536:
        raise ValueError(f"invalid BootBlock at {header:04x}")
    yield f"# ROM BootBlock {header:04x}: payload {data:04x}, origin {origin:03x}, {count} hunks"
    for hunk in range(count):
        extra = rom[data + hunk * 17]
        for k in range(4):
            address = data + hunk * 17 + 1 + k * 4
            left = rom[address] << 8 | rom[address + 1]
            right = rom[address + 2] << 8 | rom[address + 3]
            sl, sr = extra >> (7 - 2 * k) & 1, extra >> (6 - 2 * k) & 1
            fields = (sl << 3 | left >> 13, left >> 9 & 15,
                      left >> 6 & 7, left >> 3 & 7, left & 7, sr,
                      right >> 8, right & 255, left << 1 | sl,
                      right << 1 | sr)
            yield f"IM {origin + hunk * 4 + k:03x} " + " ".join(f"{v:x}" for v in fields)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("dump", type=Path)
    parser.add_argument("--alu-fixture", type=Path)
    args = parser.parse_args()
    rom = {}
    for line in args.dump.read_text().splitlines():
        address, value = (int(field, 16) for field in line.split())
        rom[address] = value
    for header in (0xc00b, 0xc000):
        print("\n".join(decode(rom, header)))
    if args.alu_fixture:
        print("# Component fixture ALUFM; original ROM IM above remains unchanged.")
        entries = {}
        for line in args.alu_fixture.read_text().splitlines():
            if line.startswith("ALUFM "):
                _, address, value = line.split()
                index, data = int(address, 16), int(value, 16)
                if index in entries and entries[index] != data:
                    raise ValueError(f"conflicting ALUFM entry {index:x}")
                entries[index] = data
        for index, data in sorted(entries.items()):
            print(f"ALUFM {index:x} {data:02x}")


if __name__ == "__main__":
    main()
