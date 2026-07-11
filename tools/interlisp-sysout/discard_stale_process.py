#!/usr/bin/env python3
"""Discard one unavailable saved PROCESS context from an Interlisp sysout.

Lyric and Medley 1.0 archive sleeping process handles whose stack pages are
not present in the sysout.  On a hard restart, PROCESSWORLD preserves a
deleted process with RESETVARSLST state and RELEASE.PROCESS dereferences its
PROCFX before clearing it.  That enters RAID with an invalid stack address.

This tool makes unrecoverable deleted processes eligible for the ordinary
PROCESSWORLD discard path by clearing PROCESS.PROCFX and RESTARTABLE.  Record
addresses are explicit and the process layout and missing stack page are
validated before writing anything.
"""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import sys


PAGE_BYTES = 512
WORDS_PER_PAGE = 256

IFP_VALID_KEY = 0o17
IFP_N_ACTIVE_PAGES = 0o24
IFP_FPTOVP_START = 0o73
VALID_KEY = 0o12743

PROCESS_PROCFX0 = 0
PROCESS_PROCFX = 1
PROCESS_STATUS_AND_NAME_HI = 2
PROCESS_PRIORITY_AND_QUEUE_HI = 4
PROCESS_FLAGS_AND_TIMER_HI = 8
PROCESS_RESTARTABLE_HI = 20
PROCESS_RESTARTABLE_LO = 21
PROCESS_INFOHOOK_HI = 34
PROCESS_INFOHOOK_LO = 35

PSTAT_DELETED = 2
PROCDELETED = 0x20
STACK_HI = 1


def parse_octal_pointer(text: str) -> tuple[int, int]:
    try:
        hi_text, lo_text = text.split(":", 1)
        hi = int(hi_text.removeprefix("0o"), 8)
        lo = int(lo_text.removeprefix("0o"), 8)
    except (ValueError, AttributeError) as exc:
        raise argparse.ArgumentTypeError(
            "pointer must be HI:LO in octal, for example 74:101600"
        ) from exc
    if not 0 <= hi <= 0xFF or not 0 <= lo <= 0xFFFF:
        raise argparse.ArgumentTypeError("pointer is outside the 24-bit Lisp VA")
    return hi, lo


def word_at(data: bytearray, byte_offset: int) -> int:
    if byte_offset < 0 or byte_offset + 2 > len(data):
        raise ValueError(f"word offset 0x{byte_offset:x} is outside the sysout")
    return int.from_bytes(data[byte_offset : byte_offset + 2], "big")


def set_word(data: bytearray, byte_offset: int, value: int) -> None:
    data[byte_offset : byte_offset + 2] = value.to_bytes(2, "big")


class SysoutMap:
    def __init__(self, data: bytearray) -> None:
        if len(data) < 2 * PAGE_BYTES or len(data) % PAGE_BYTES:
            raise ValueError("sysout is not an integral number of 512-byte pages")

        interface = PAGE_BYTES
        if word_at(data, interface + 2 * IFP_VALID_KEY) != VALID_KEY:
            raise ValueError("sysout interface page has the wrong IFPValidKey")

        self.data = data
        self.n_active_pages = word_at(
            data, interface + 2 * IFP_N_ACTIVE_PAGES
        )
        self.fptovp_start = word_at(
            data, interface + 2 * IFP_FPTOVP_START
        )
        if self.n_active_pages != len(data) // PAGE_BYTES:
            raise ValueError(
                "IFPNActivePages does not match the sysout file length: "
                f"{self.n_active_pages} != {len(data) // PAGE_BYTES}"
            )

        # The sysout omits VMEM file page 0.  Its raw page 1 (the interface
        # page) is installed at VMEM file page 2, so raw page = file page - 1.
        # IFPFPTOVPStart is a VMEM file-page number and therefore appears one
        # raw sysout page earlier.  FPTOVP[file_page] contains the virtual page.
        self.vp_to_raw_page: dict[int, int] = {}
        for file_page in range(2, self.n_active_pages + 1):
            table_raw_page = self.fptovp_start - 1 + file_page // WORDS_PER_PAGE
            table_word = file_page % WORDS_PER_PAGE
            table_offset = table_raw_page * PAGE_BYTES + 2 * table_word
            vp = word_at(data, table_offset)
            self.vp_to_raw_page[vp] = file_page - 1

    def va_word_offset(self, hi: int, lo: int) -> int:
        va = (hi << 16) | lo
        vp = va >> 8
        word = va & 0xFF
        try:
            raw_page = self.vp_to_raw_page[vp]
        except KeyError as exc:
            raise ValueError(f"virtual page {vp:o} is absent from FPTOVP") from exc
        return raw_page * PAGE_BYTES + 2 * word

    def has_rebased_stack_page(self, procfx: int) -> bool:
        live_va = (STACK_HI << 16) | procfx
        return (live_va >> 8) in self.vp_to_raw_page


def discard_process_context(
    data: bytearray, record: tuple[int, int]
) -> tuple[int, int]:
    hi, lo = record
    sysout_map = SysoutMap(data)
    record_offset = sysout_map.va_word_offset(hi, lo)

    def process_word(index: int) -> int:
        return word_at(data, record_offset + 2 * index)

    procfx0 = process_word(PROCESS_PROCFX0)
    procfx = process_word(PROCESS_PROCFX)
    status = process_word(PROCESS_STATUS_AND_NAME_HI) >> 8
    priority = process_word(PROCESS_PRIORITY_AND_QUEUE_HI) >> 8
    flags = process_word(PROCESS_FLAGS_AND_TIMER_HI) >> 8
    infohook = (
        process_word(PROCESS_INFOHOOK_HI),
        process_word(PROCESS_INFOHOOK_LO),
    )
    problems: list[str] = []
    if procfx0 != hi:
        problems.append(f"PROCFX0={procfx0:o}, expected record segment {hi:o}")
    if procfx == 0:
        problems.append("PROCFX is already zero")
    if status != PSTAT_DELETED:
        problems.append(f"PROCSTATUS={status}, expected PSTAT.DELETED")
    if priority > 4:
        problems.append(f"PROCPRIORITY={priority}, expected 0..4")
    if not flags & PROCDELETED:
        problems.append(f"PROCDELETED is clear in flag byte 0x{flags:02x}")
    if infohook == (0, 0):
        problems.append("PROCINFOHOOK is NIL")
    if sysout_map.has_rebased_stack_page(procfx):
        problems.append(
            f"rebased stack page for {{1,{procfx:o}}} is present; refusing to discard it"
        )
    if problems:
        raise ValueError("record validation failed: " + "; ".join(problems))

    set_word(data, record_offset + 2 * PROCESS_PROCFX, 0)
    set_word(data, record_offset + 2 * PROCESS_RESTARTABLE_HI, 0)
    set_word(data, record_offset + 2 * PROCESS_RESTARTABLE_LO, 0)
    set_word(data, record_offset + 2 * PROCESS_INFOHOOK_HI, 0)
    set_word(data, record_offset + 2 * PROCESS_INFOHOOK_LO, 0)
    return record_offset + 2 * PROCESS_PROCFX, procfx


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path, help="archived Interlisp sysout")
    parser.add_argument("output", type=Path, help="sanitized output sysout")
    parser.add_argument(
        "--record",
        action="append",
        type=parse_octal_pointer,
        metavar="HI:LO",
        help="PROCESS record address in octal; may be repeated",
    )
    args = parser.parse_args()

    try:
        data = bytearray(args.input.read_bytes())
        if not args.record:
            parser.error("at least one --record is required")
        changes = [
            discard_process_context(data, record) for record in (args.record or [])
        ]
        args.output.parent.mkdir(parents=True, exist_ok=True)
        temporary = args.output.with_name(args.output.name + ".tmp")
        temporary.write_bytes(data)
        os.replace(temporary, args.output)
    except (OSError, ValueError) as exc:
        print(f"discard_stale_process: {exc}", file=sys.stderr)
        return 1

    for (hi, lo), (changed_offset, old_procfx) in zip(args.record or [], changes):
        print(
            f"discarded PROCESS context at {{{hi:o},{lo:o}}}: "
            f"PROCFX {{{hi:o},{old_procfx:o}}} -> 0, RESTARTABLE -> NIL, "
            f"PROCINFOHOOK -> NIL "
            f"(file byte 0x{changed_offset:x})"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
