#!/usr/bin/env python3
"""Inspect Cedar BCD imports and compare their version stamps.

Cedar does not bind an imported interface by basename alone.  A BCD carries
an import table and, for each import, a file-table entry containing the exact
TimeStamp of the interface that the compiler saw.  This tool exposes that
otherwise opaque comparison.

Examples:

  # Inspect all imports in Gargoyle and search the served tree for candidates
  python3 tools/cedar_bcd_versions.py \
      chm/cedar/stp-root/CedarChest6.1/Gargoyle/Gargoyle.bcd \
      --root chm/cedar/stp-root

  # Restrict the report to one interface
  python3 tools/cedar_bcd_versions.py client.bcd --import BiScrollers \
      --candidate chm/cedar/stp-root/CedarChest6.1/BiScrollers/BiScrollers.BCD

The BCD format is the big-endian Mesa format described by BcdDefs.mesa.
This deliberately reads only the stable header, import table, file table,
and packed-string table; it does not try to load or execute Cedar code.
"""

from __future__ import annotations

import argparse
import binascii
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


class BcdError(ValueError):
    """The input is not a BCD layout this small reader understands."""


def u16(data: bytes, offset: int) -> int:
    if offset < 0 or offset + 2 > len(data):
        raise BcdError(f"word at 0x{offset:x} is outside the file")
    return int.from_bytes(data[offset : offset + 2], "big")


def stamp_bytes(data: bytes, offset: int = 2) -> bytes:
    if offset < 0 or offset + 6 > len(data):
        raise BcdError(f"version stamp at 0x{offset:x} is outside the file")
    return data[offset : offset + 6]


def format_stamp(stamp: bytes) -> str:
    if len(stamp) != 6:
        raise ValueError("a Cedar TimeStamp has six bytes")
    # TimeStamp.Stamp = [net: BYTE, host: BYTE, time: LONG CARDINAL].
    return f"{stamp[0]:02x}/{stamp[1]:02x}/{stamp[2:6].hex()}"


def crc32(data: bytes) -> str:
    return f"{binascii.crc32(data) & 0xffffffff:08x}"


@dataclass(frozen=True)
class Import:
    number: int
    name: str
    portable: str
    file_index_words: int
    expected_stamp: bytes


class Bcd:
    """Small reader for the BCD tables needed for version diagnostics."""

    # Header table pairs, in the order specified by BcdDefs.BCD.
    TABLES = (
        "ss", "ct", "mt", "imp", "exp", "ev", "sg", "ft",
        "sp", "nt", "typ", "tm", "fp", "lf", "rf", "tf",
    )

    def __init__(self, path: Path):
        self.path = path
        self.data = path.read_bytes()
        if u16(self.data, 0) != 0x0500:
            raise BcdError(
                f"{path}: version identifier is 0x{u16(self.data, 0):04x}, "
                "not the Cedar BCD identifier 0x0500"
            )
        self.version = stamp_bytes(self.data)
        self.n_imports = u16(self.data, 28)
        self.tables = {}
        # The first table pair follows the packed flags and dummy count.
        for index, name in enumerate(self.TABLES):
            base = 36 + index * 4
            self.tables[name] = (u16(self.data, base), u16(self.data, base + 2))

    def _name(self, name_record: int) -> str:
        """Decode a BcdDefs.NameRecord into its packed-string name.

        The packed string table has two words of preamble.  NameRecords are
        offsets from the first byte after that preamble; this is why the
        record for BiScrollers in Gargoyle is 0x0320 while its length byte is
        at file offset 0x0389.
        """
        ss_offset, ss_limit = self.tables["ss"]
        base = ss_offset * 2 + 3
        offset = base + name_record
        end = ss_limit * 2
        if offset >= end or offset >= len(self.data):
            return f"<name 0x{name_record:04x}>"
        length = self.data[offset]
        finish = offset + 1 + length
        if finish > end or finish > len(self.data):
            return f"<name 0x{name_record:04x}>"
        return self.data[offset + 1 : finish].decode("latin-1", "replace")

    def _file_stamp(self, file_index_words: int) -> bytes:
        ft_offset, ft_limit = self.tables["ft"]
        # Relative pointers in the BCD tables are word offsets.  FTRecord is
        # four words: NameRecord plus a six-byte TimeStamp.
        if file_index_words < 0 or file_index_words + 4 > ft_limit:
            raise BcdError(
                f"{self.path}: file-table pointer 0x{file_index_words:x} "
                f"is outside FT [0, 0x{ft_limit:x})"
            )
        offset = (ft_offset + file_index_words) * 2
        if offset + 8 > len(self.data):
            raise BcdError(f"{self.path}: file-table record is truncated")
        return stamp_bytes(self.data, offset + 2)

    def imports(self) -> Iterable[Import]:
        imp_offset, _ = self.tables["imp"]
        # IMPRecord is three words in Cedar 6.x: NameRecord, packed
        # port/namedInstance/file index, and packed gfi/ngfi.
        offset = imp_offset * 2
        for number in range(self.n_imports):
            record = offset + number * 6
            if record + 6 > len(self.data):
                raise BcdError(f"{self.path}: import table is truncated")
            name_record = u16(self.data, record)
            packed_file = u16(self.data, record + 2)
            file_index = packed_file & 0x3FFF
            portable = "interface" if packed_file & 0x8000 else "module"
            yield Import(
                number=number,
                name=self._name(name_record),
                portable=portable,
                file_index_words=file_index,
                expected_stamp=self._file_stamp(file_index),
            )


def candidate_paths(root: Path, module_name: str) -> list[Path]:
    """Find a module's BCD case-insensitively in a served tree.

    BCD import names omit the file extension (the import table says
    ``BiScrollers`` while the file is ``BiScrollers.bcd``).
    """
    basename = module_name if "." in module_name else module_name + ".bcd"
    wanted = basename.casefold()
    return sorted(
        p for p in root.rglob("*")
        if p.is_file() and p.name.casefold() == wanted
    )


def report(client: Path, root: Path | None, selected: str | None,
           explicit: Path | None) -> int:
    bcd = Bcd(client)
    imports = [
        item for item in bcd.imports()
        if (selected is None or item.name.casefold() == selected.casefold())
    ]
    if selected and not imports:
        print(f"no import named {selected!r} in {client}")
        return 1

    print(f"client: {client}")
    print(f"client BCD header version: {format_stamp(bcd.version)}")
    print(f"imports shown: {len(imports)} of {bcd.n_imports}")
    status = 0
    for item in imports:
        print(
            f"\nimport #{item.number}: {item.name} "
            f"({item.portable})\n"
            f"  expected: {format_stamp(item.expected_stamp)} "
            f"(file-table word index 0x{item.file_index_words:x})"
        )
        paths = [explicit] if explicit else (
            candidate_paths(root, item.name) if root else []
        )
        if not paths:
            print("  candidate: NOT FOUND")
            status = 1
            continue
        for path in paths:
            try:
                candidate = Bcd(path)
                actual = candidate.version
                result = "MATCH" if actual == item.expected_stamp else "MISMATCH"
                print(
                    f"  candidate: {path}\n"
                    f"    size={len(candidate.data)} crc32={crc32(candidate.data)} "
                    f"header={format_stamp(actual)} -> {result}"
                )
                if result != "MATCH":
                    status = 1
            except (OSError, BcdError) as error:
                print(f"  candidate: {path}\n    ERROR: {error}")
                status = 1
    return status


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("client", type=Path, help="client/interface BCD to inspect")
    parser.add_argument("--root", type=Path,
                        help="served tree to search for imported BCD basenames")
    parser.add_argument("--import", dest="selected",
                        help="show only this imported interface/module")
    parser.add_argument("--candidate", type=Path,
                        help="compare against this one BCD instead of --root")
    args = parser.parse_args()
    if args.root and args.candidate:
        parser.error("use --root or --candidate, not both")
    try:
        return report(args.client, args.root, args.selected, args.candidate)
    except (OSError, BcdError) as error:
        parser.error(str(error))
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
