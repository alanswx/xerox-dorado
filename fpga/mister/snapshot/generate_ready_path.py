#!/usr/bin/env python3
"""Shorten CPU readiness by using the already qualified normal frontend predicate.

Keep storage initialization and the independent global snapshot hold guard.
The frontend aggregate is NOT complete arbiter/SDRAM transaction drainage.
No clocks, ports, original board state or peripheral behavior are changed.
"""
from generate_serial_boards import once,raw_receipt,ROOT

OLD='wire machine_enable = ~reset && pack_ready && storage_ready && !board_state_hold;'
NEW='wire machine_enable = ~reset && pack_ready && storage_initialized && snapshot_frontends_ready && !board_state_hold;'


def reference():
    return raw_receipt(ROOT/'fpga/mister/reports/2026-10-19-snapshot-serial-chipwidth','terminal-fit/core/Dorado.sv').decode()


def transform(source):
    source=once(source,OLD,'wire snapshot_frontends_ready;\nassign CAPACITY_MEMORY_DRAINED=snapshot_frontends_ready;\n'+NEW)
    return once(source,'.snapshot_memory_drained(CAPACITY_MEMORY_DRAINED)', '.snapshot_memory_drained(snapshot_frontends_ready)')


def inverse(source):
    source=once(source,'wire snapshot_frontends_ready;\nassign CAPACITY_MEMORY_DRAINED=snapshot_frontends_ready;\n'+NEW,OLD)
    return once(source,'.snapshot_memory_drained(snapshot_frontends_ready)', '.snapshot_memory_drained(CAPACITY_MEMORY_DRAINED)')


def expressions():
    old=reference();new=transform(old);assert inverse(new)==old
    return OLD[len('wire machine_enable = '):-1],NEW[len('wire machine_enable = '):-1]
