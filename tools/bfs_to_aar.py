#!/usr/bin/env python3
"""Convert a CHM Alto BFS transfer stream into AAR disk halves.

The archive's Smalltalk14 BFS is a two-drive 406-cylinder/14-sector stream.
Palo's historical ``par`` command defaults to a single 203x2x12 Diablo and
therefore rejects that otherwise valid stream.  This converter preserves the
filesystem page records and writes the two full AAR halves expected by
``altofs``/``dsk2trident``.

The output is deliberately sparse only in the filesystem sense: every sector
is emitted, with free sectors carrying the same header/label values Palo uses
when it wipes a disk.  AAR words are little-endian, including the dummy word
before each sector.
"""

from __future__ import annotations

import argparse
import struct
from pathlib import Path


DISK_PARAMS_REPLY = 3
DISK_PAGE_REPLY = 6
END_OF_TRANSFER = 7
DIABLO_DISK_TYPE = 10
HEADER_WORDS = 2
LABEL_WORDS = 8
SECTOR_WORDS = 256
SECTOR_BYTES = SECTOR_WORDS * 2
VERSION_FREE = 0xFFFF


def words(data: bytes) -> list[int]:
    if len(data) % 2:
        raise ValueError("BFS command has an odd byte length")
    return list(struct.unpack(f">{len(data) // 2}H", data))


def real_to_virtual(rda: int, disks: int, cylinders: int,
                    heads: int, sectors: int) -> int:
    cylinder = (rda >> 3) & 0x1FF
    head = (rda >> 2) & 1
    sector = (rda >> 12) & 0xF
    disk = (rda >> 1) & 1
    if (disk >= disks or cylinder >= cylinders or head >= heads or
            sector >= sectors or (rda & 1)):
        raise ValueError(f"invalid BFS real disk address {rda:o}")
    return (((disk * cylinders + cylinder) * heads + head) * sectors + sector)


def virtual_to_real(vda: int, cylinders: int, heads: int, sectors: int) -> int:
    sector = vda % sectors
    vda //= sectors
    head = vda % heads
    vda //= heads
    cylinder = vda % cylinders
    disk = vda // cylinders
    return (cylinder << 3) | (head << 2) | (sector << 12) | (disk << 1)


def parse_bfs(path: Path):
    data = path.read_bytes()
    pos = 0
    params = None
    pages: dict[int, tuple[list[int], list[int], bytes]] = {}
    while pos < len(data):
        if pos + 2 > len(data):
            raise ValueError("truncated BFS command length")
        command_len = struct.unpack_from(">H", data, pos)[0]
        pos += 2
        if command_len < 2 or pos + 2 * (command_len - 1) > len(data):
            raise ValueError("truncated BFS command")
        command = words(data[pos:pos + 2 * (command_len - 1)])
        pos += 2 * (command_len - 1)
        if not command:
            raise ValueError("empty BFS command")
        kind = command[0]
        if kind == DISK_PARAMS_REPLY:
            if command_len != 7 or len(command) != 6 or command[1] != DIABLO_DISK_TYPE:
                raise ValueError(f"unsupported disk-parameter command: {command}")
            params = tuple(command[2:])  # cylinders, heads, sectors, disks
        elif kind == DISK_PAGE_REPLY:
            short_page = 1 + HEADER_WORDS + LABEL_WORDS
            full_page = short_page + SECTOR_WORDS
            if len(command) not in (short_page, full_page):
                raise ValueError(f"page has {len(command)} words, expected {short_page} or {full_page}")
            if params is None:
                raise ValueError("page appeared before disk parameters")
            header = command[1:1 + HEADER_WORDS]
            label = command[1 + HEADER_WORDS:1 + HEADER_WORDS + LABEL_WORDS]
            if len(command) == full_page:
                payload = struct.pack(f">{SECTOR_WORDS}H", *command[-SECTOR_WORDS:])
            else:
                # BFS omits the all-zero data area for free pages.
                payload = bytes(SECTOR_BYTES)
            cylinders, heads, sectors, disks = params
            vda = real_to_virtual(header[1], disks, cylinders, heads, sectors)
            pages[vda] = (header, label, payload)
        elif kind == END_OF_TRANSFER:
            if pos != len(data):
                raise ValueError("data follows BFS end-of-transfer command")
            break
        else:
            raise ValueError(f"unsupported BFS command {kind:o}")
    if params is None:
        raise ValueError("BFS has no disk parameters")
    cylinders, heads, sectors, disks = params
    if disks != 2 or heads != 2 or sectors not in (12, 14):
        raise ValueError(f"expected a two-drive Alto disk, got {params}")
    disk_length = cylinders * heads * sectors
    expected_pages = disk_length * disks
    if len(pages) != expected_pages:
        raise ValueError(f"BFS has {len(pages)} pages, expected {expected_pages}")
    return params, pages


def write_aar(path: Path, disk: int, params, pages) -> None:
    cylinders, heads, sectors, disks = params
    disk_length = cylinders * heads * sectors
    with path.open("wb") as fp:
        for local_vda in range(disk_length):
            vda = disk * disk_length + local_vda
            page = pages.get(vda)
            if page is None:
                header = [0, virtual_to_real(vda, cylinders, heads, sectors)]
                label = [0, 0, 0, 0, 0, VERSION_FREE, VERSION_FREE, VERSION_FREE]
                payload = bytes(SECTOR_BYTES)
            else:
                header, label, payload = page
            fp.write(struct.pack("<H", vda))
            fp.write(struct.pack(f"<{HEADER_WORDS}H", *header))
            fp.write(struct.pack(f"<{LABEL_WORDS}H", *label))
            # BFS stores payload words big-endian; AAR stores each word LE.
            for i in range(0, len(payload), 2):
                fp.write(payload[i:i + 2][::-1])


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("bfs", type=Path)
    parser.add_argument("disk0", type=Path)
    parser.add_argument("disk1", type=Path)
    args = parser.parse_args()
    params, pages = parse_bfs(args.bfs)
    write_aar(args.disk0, 0, params, pages)
    write_aar(args.disk1, 1, params, pages)
    cylinders, heads, sectors, disks = params
    size = cylinders * heads * sectors * (1 + HEADER_WORDS + LABEL_WORDS + SECTOR_WORDS) * 2
    print(f"bfs_to_aar: {args.bfs} -> {args.disk0}, {args.disk1}")
    print(f"bfs_to_aar: geometry {disks} drives x {cylinders} cylinders x {heads} heads x {sectors} sectors; {size} bytes/half")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
