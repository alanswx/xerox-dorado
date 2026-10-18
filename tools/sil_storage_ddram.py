#!/usr/bin/env python3
"""Give the Sil-generated MSA board external raw storage, keeping its logic.

Consume sil_to_verilog.py's pin bindings/shared row-column latches. Only
MK4096 bit arrays and their identical front ends are grouped; all board
selection, serial data, ECC and other packages remain in the emitted RTL.
Never edits the canonical generated board. Writes a separate msa_ddram.
"""
import argparse
import hashlib
import json
import re
from pathlib import Path


def storage64_frontend(source):
    """Select the eighth MSA address pin without changing board logic.

    MemAd.1 is the unused high multiplexed address on the 16K board.
    Preserve the same polarity as that group's existing seven pins.
    The returned full board can also be used as the raw DDR reference.
    """
    source = source.replace('wire [13:0] dram_addr_', 'wire [15:0] dram_addr_')
    source, count = re.subn(r'cell_MK4096P_addr u_dram_addr',
                          'cell_MK4096P_addr #(.ROW_BITS(8)) u_dram_addr', source)
    if count != 16:
        raise ValueError(f'expected16 shared MSA address latches, got{count}')
    def extend(match):
        pins = match[1]
        high = '~MemAd_1' if '_p_' in pins.split(',')[0] else 'MemAd_1'
        return '.a({' + high + ', ' + pins + '})'
    source, count = re.subn(r'\.a\(\{(A6[^}]+)\}\)', extend, source)
    if count != 16:
        raise ValueError(f'expected16 multiplexed MSA address tuples, got{count}')
    source, count = re.subn(r'cell_MK4096P_6 u_',
                          'cell_MK4096P_6 #(.ADDR_BITS(16)) u_', source)
    if count != 144:
        raise ValueError(f'expected144 MSA storage chips, got{count}')
    return source


def generate(source, address_bits=14, defer_reads=False):
    if address_bits not in (14, 16):
        raise ValueError('storage address width must be14 or16')
    if address_bits == 16:
        source = storage64_frontend(source)
    instances = list(re.finditer(
        r'  cell_MK4096P_6(?: #\(\.ADDR_BITS\(16\)\))? u_(\w+) \(\n(.*?)\n  \); // MK4096P-6',
        source, re.S))
    if len(instances) != 144:
        raise ValueError(f'expected 144 MSA storage chips, found {len(instances)}')
    groups = {}
    for inst in instances:
        pins = dict(re.findall(r'\.(\w+)\(([^()]+)\)', inst[2]))
        key = tuple(pins[p] for p in ('addr_i', 'p4', 'p15', 'p3'))
        groups.setdefault(key, []).append((inst, pins))
    if len(groups) != 16:
        raise ValueError(f'expected 16 complete storage control groups, found {len(groups)}')
    edits, mapping = [], []
    for group, (key, members) in enumerate(groups.items()):
        addr, ras, cas, we = key
        if not 1 <= len(members) <= 16:
            raise ValueError('storage group does not fit one raw 16-bit word')
        # List order is MSB first in both input and output concatenations.
        inputs = ', '.join(p['p2'] for _, p in members)
        outputs = ', '.join(p['p14'] for _, p in members)
        deferred = ', .DEFER_READS(1)' if defer_reads else ''
        replacement = f'''  // Raw storage group {group}: unchanged {addr}/RAS/CAS/WE front end.
  dorado_storage_group #(.ADDR_BITS({address_bits}), .WIDTH({len(members)}){deferred}) u_storage_{group} (
    .host_clk(storage_clk), .machine_clk(sys_clk), .reset(storage_reset),
    .ras_n({ras}), .cas_n({cas}), .we_n({we}), .addr({addr}),
    .data_in({{{inputs}}}), .data_out({{{outputs}}}), .ready(storage_group_ready[{group}]),
    .req_valid(storage_req_valid[{group}]), .req_ready(storage_req_ready[{group}]),
    .req_write(storage_req_write[{group}]), .req_addr(storage_req_addr[{group*address_bits}+:{address_bits}]),
    .req_data(storage_req_data[{group*16}+:16]),
    .rsp_valid(storage_rsp_valid[{group}]), .rsp_ready(storage_rsp_ready[{group}]),
    .rsp_data(storage_rsp_data[{group*16}+:16])
  );'''
        for i, (inst, _) in enumerate(members):
            edits.append((inst.start(), inst.end(), replacement if i == 0 else
                          f'  // {inst[1]} storage is in u_storage_{group}.'))
        mapping.append({'group': group, 'address': addr, 'ras': ras, 'cas': cas,
                        'we': we, 'bits': {inst[1]: len(members)-1-i
                                           for i, (inst, _) in enumerate(members)}})
    for start, end, text in sorted(edits, reverse=True):
        source = source[:start] + text + source[end:]
    source, count = re.subn(r'^module msa\b', 'module msa_ddram', source, count=1, flags=re.M)
    if count != 1:
        raise ValueError('input is not the canonical MSA module')
    source = source.replace('    input  wire sys_clk,', '''    input  wire sys_clk,
    // FPGA backing interface; these are not Dorado backplane signals.
    input wire storage_clk, storage_reset,
    output wire storage_ready,
    output wire [15:0] storage_req_valid, storage_req_write,
    input wire [15:0] storage_req_ready,
    output wire [STORAGE_ADDR_HIGH:0] storage_req_addr,
    output wire [255:0] storage_req_data,
    input wire [15:0] storage_rsp_valid,
    output wire [15:0] storage_rsp_ready,
    input wire [255:0] storage_rsp_data,''', 1)
    source = source.replace('STORAGE_ADDR_HIGH', str(address_bits*16-1))
    source = source.replace('  // ---- packages', '''  wire [15:0] storage_group_ready;
  assign storage_ready = &storage_group_ready;
  // ---- packages''', 1)
    return source, mapping


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, default=Path('verilog/generated/msa.v'))
    p.add_argument('-o', '--output', type=Path, required=True)
    p.add_argument('--chip-address-bits', type=int, choices=(14, 16), default=14)
    p.add_argument('--defer-reads', action='store_true',
                   help='materialize the internal DRAM read stage only when observable (experimental)')
    p.add_argument('--reference-output', type=Path,
                   help='also emit the full bit-cell board at the selected density for equivalence tests')
    a = p.parse_args()
    original = a.source.read_text()
    source, mapping = generate(original, a.chip_address_bits, a.defer_reads)
    a.output.parent.mkdir(parents=True, exist_ok=True)
    a.output.write_text('// Generated by tools/sil_storage_ddram.py; do not edit.\n' + source)
    if a.reference_output:
        reference = storage64_frontend(original) if a.chip_address_bits == 16 else original
        a.reference_output.parent.mkdir(parents=True, exist_ok=True)
        a.reference_output.write_text(reference)
    manifest = {
        'source_sha256': hashlib.sha256(original.encode()).hexdigest(),
        'group_words': 1 << a.chip_address_bits, 'raw_word_bits': 16, 'groups': mapping,
    }
    if a.defer_reads:
        manifest['defer_reads'] = True
    a.output.with_suffix('.mapping.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print(f'Generated {a.output}: 144 chips -> 16 raw storage groups; other MSA wiring preserved')


if __name__ == '__main__':
    main()
