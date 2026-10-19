#!/usr/bin/env python3
"""Expose static RIOT row write enables without changing state or reset timing.

Area experiment only. Keep all 1097 physical bits, the exact 133-word ABI,
single-edge whole-RAM reset and shared normal/administrative read address.
"""
import hashlib
import re
import generate_riot_shared_port as shared

ROOT, SOURCE = shared.ROOT, shared.SOURCE


def generate(raw=None):
    text, manifest = shared.generate(raw)
    previous = manifest['generated_sha256']
    start = text.index('always @(posedge state_host_clk)begin\n    if(ram_normal && !res_n)begin')
    end = text.index('\nendmodule', start)
    old = text[start:end]
    reset = re.search(r"riot_ram <= '\{(.*?)\};", old, re.S)
    assert reset
    values = re.findall(r"8'h([A-Fa-f0-9]{2})", reset[1])
    assert len(values) == 128
    assert old.count('riot_ram[ram_address] <= ram_write_data;') == 1
    rows = []
    for index, value in enumerate(values):
        rows.append(f'''always @(posedge state_host_clk)begin
    if(ram_reset)riot_ram[7'd{index}] <= init_7800 ? 8'h{value} : 8'h00;
    else if(ram_write && ram_address==7'd{index})riot_ram[7'd{index}] <= ram_write_data;
end''')
    replacement = '''wire ram_reset = ram_normal && !res_n;
wire ram_write = ram_admin_write || ram_guest_write;
'''+ '\n'.join(rows)+'\n'
    text = text[:start]+replacement+text[end:]
    return text, dict(manifest, generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                      shared_port_predecessor_sha256=previous,
                      row_decode_experiment=__doc__)
