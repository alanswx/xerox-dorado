#!/usr/bin/env python3
"""Fold mutually exclusive RIOT RAM ports; retain every physical state bit.

Experimental area optimization, not yet installed. No extra storage, valid
mask, changed reset, latency or state ABI. All 1097 original modeled bits and
the qualified control/timer/PA7 implementation remain intact.
"""
import hashlib
import re
import generate_riot as original

ROOT, SOURCE = original.ROOT, original.SOURCE


def once(text, before, after):
    assert text.count(before) == 1, 'unexpected qualified RIOT source: '+before[:80]
    return text.replace(before, after, 1)


def generate(raw=None):
    text, manifest = original.generate(raw)
    previous = manifest['generated_sha256']
    reset = re.search(r'\tif \(init_7800\)\n\t\triot_ram <=.*?\n\telse\n\t\triot_ram <=.*?;', text, re.S)
    assert reset, 'qualified RAM reset missing'
    reset_text = reset[0]
    text = text[:reset.start()] + text[reset.end():]
    text = once(text, 'if(state_write)riot_ram[state_index[6:0]]<=state_data[7:0];\n'
                      "                else state_result<={10'b0,riot_ram[state_index[6:0]]};",
                      "if(!state_write)state_result<={10'b0,ram_read_data};")
    text = once(text, 'd_out <= riot_ram[addr];', 'd_out <= ram_read_data;')
    text = once(text, 'riot_ram[addr] <= d_in;', 'begin end // RAM write uses the shared port below.')
    shared = '''
// Normal and administrative accesses are mutually exclusive at state_hold.
// A single address/data/write-enable port retains read-before-write behavior.
wire ram_normal = !state_hold && state_advance;
wire ram_admin_write = state_accept && state_valid && !state_index[7] && state_write;
wire ram_guest_write = ram_normal && res_n && ce && CS1 && !CS2_n && !RS_n && !RW_n;
wire [6:0] ram_address = state_hold ? state_index[6:0] : addr;
wire [7:0] ram_write_data = state_hold ? state_data[7:0] : d_in;
wire [7:0] ram_read_data = riot_ram[ram_address];
always @(posedge state_host_clk)begin
    if(ram_normal && !res_n)begin
RAM_RESET
    end else if(ram_admin_write || ram_guest_write)begin
        riot_ram[ram_address] <= ram_write_data;
    end
end
'''.replace('RAM_RESET', reset_text)
    text = once(text, '\nendmodule', shared+'\nendmodule')
    manifest = dict(manifest, generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                    qualified_predecessor_sha256=previous, area_experiment=__doc__,
                    shared_ram_port=True)
    return text, manifest
