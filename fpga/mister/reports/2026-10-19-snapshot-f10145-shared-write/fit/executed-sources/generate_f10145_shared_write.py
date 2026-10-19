#!/usr/bin/env python3
"""Fold exclusive F10145 normal/restore writes without changing either read port.

Preserves all74physical bits, asynchronous ordinary output, latched write window,
old-value administrative response and existing17-word state ABI. No new state,
pipeline, memory-placement change or global snapshot boundary is introduced.
"""
import hashlib
import generate_f10145 as qualified

ROOT, ORIGINAL = qualified.ROOT, qualified.ORIGINAL


def generate(original=None):
    text, manifest = qualified.generate(original)
    predecessor = manifest['generated_sha256']
    once = qualified.once
    text = once(text, '        else mem[state_index[3:0]] <= state_data[3:0];',
                '        else begin end // RAM write uses the common port below.')
    text = once(text, "    if (p13 && !we_d && ce_lat) mem[a_lat] <= d_lat;   // WE' rising: commit",
                "    // The common port commits this same captured WE' rising edge.")
    declaration = '''  wire ram_restore = state_accept && state_write && state_valid && state_index < 5'd16;
  wire ram_guest_write = !state_hold && state_advance && p13 && !we_d && ce_lat;
  wire [3:0] ram_write_address = state_hold ? state_index[3:0] : a_lat;
  wire [3:0] ram_write_data = state_hold ? state_data[3:0] : d_lat;
'''
    text = once(text, '  always @(posedge state_host_clk) begin',
                declaration+'  always @(posedge state_host_clk) begin\n'
                '    if (ram_restore || ram_guest_write) mem[ram_write_address] <= ram_write_data;')
    manifest = dict(manifest, generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                    qualified_predecessor_sha256=predecessor, shared_write_port=True,
                    area_experiment=__doc__)
    return text, manifest
