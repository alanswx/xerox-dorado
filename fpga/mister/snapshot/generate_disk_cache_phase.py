#!/usr/bin/env python3
"""Prototype opposite-edge cache-tag staging; no new timing exception.

The original disk pipeline and tag updates stay intact. Two implementation
registers sample cyl/head on the host falling edge; cache-miss capture uses
these stable values on the following rising edge. Requires the separately
checked guest-gate/capture exclusion. Adds eighteen REQUIRED snapshot bits.
Not integrated or timing-qualified in the native core.
"""
from generate_serial_boards import ROOT,raw_receipt,once

SOURCE='terminal-fit/src/fpga/mister/dorado_pack_hps.sv'
INSERT='''  // Cache capture excludes a guest edge: preserve the preceding half-cycle
  // values, giving the guest-clock source a full half-cycle hold separation.
  reg [11:0] cache_source_cyl = INITIAL_CYL;
  reg [5:0] cache_source_head = INITIAL_HEAD;
  always @(negedge clk_sys) begin
    cache_source_cyl <= cyl; cache_source_head <= head;
  end
'''


def reference():
    return raw_receipt(ROOT/'fpga/mister/reports/2026-10-19-snapshot-serial-chipwidth',SOURCE).decode()


def transform(source):
    source=once(source,'  reg valid = 0;',INSERT+'  reg valid = 0;')
    return once(source,'cached_cyl <= cyl; cached_head <= head;',
                'cached_cyl <= cache_source_cyl; cached_head <= cache_source_head;')


def inverse(source):
    source=once(source,INSERT,'')
    return once(source,'cached_cyl <= cache_source_cyl; cached_head <= cache_source_head;',
                'cached_cyl <= cyl; cached_head <= head;')
