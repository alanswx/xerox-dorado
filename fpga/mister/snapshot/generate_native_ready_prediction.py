#!/usr/bin/env python3
"""Wire qualified NEXT-ready and eight required external registers into Serial72.

Retain raw memory-drain ACKs, physical board state, guest phase, live guards,
clock periods and all disk/network hold and permission behavior.
"""
from generate_serial_boards import once

WRAPPER='src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
BACKPLANE='src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'
PREFIX='src/fpga/mister/snapshot_serial/'


def core(text):
    text=once(text,'wire snapshot_frontends_ready;','wire snapshot_frontends_ready,snapshot_frontends_cached;')
    text=once(text,'storage_initialized && snapshot_frontends_ready && !board_state_hold;',
        'storage_initialized && snapshot_frontends_cached && !board_state_hold;')
    return once(text,'.snapshot_frontends_ready(snapshot_frontends_ready),',
        '.snapshot_frontends_ready(snapshot_frontends_ready),\n    .snapshot_frontends_cached(snapshot_frontends_cached),')


def wrapper(text):
    text=once(text,'    output wire snapshot_frontends_ready,',
        '    output wire snapshot_frontends_ready,snapshot_frontends_cached,')
    return once(text,'.snapshot_memory_drained(snapshot_frontends_ready),',
        '.snapshot_memory_drained(snapshot_frontends_ready),\n    .snapshot_ready_cached(snapshot_frontends_cached),')


def backplane(text):
    text=once(text,'    output wire snapshot_memory_drained,',
        '    output wire snapshot_memory_drained,snapshot_ready_cached,')
    block='''  // Required implementation state outside the physical MSA scan images.
  // Capture exactly each board's post-edge reset-free readiness. Live reset,
  // initialization, disk readiness and global hold still gate the Core enable.
  wire [7:0] snapshot_msa_flags_next;
  reg [7:0] snapshot_msa_flags_ready=8'b0;
  always @(posedge storage_clk)snapshot_msa_flags_ready<=snapshot_msa_flags_next;
  assign snapshot_ready_cached=&snapshot_msa_flags_ready;
'''
    text=once(text,'  assign snapshot_memory_drained=&snapshot_msa_drained;\n',
        '  assign snapshot_memory_drained=&snapshot_msa_drained;\n'+block)
    for i in range(8):text=once(text,f'.state_memory_drained(snapshot_msa_drained[{i}]),',
        f'.state_memory_drained(snapshot_msa_drained[{i}]),\n    .state_memory_flags_next(snapshot_msa_flags_next[{i}]),')
    return text
