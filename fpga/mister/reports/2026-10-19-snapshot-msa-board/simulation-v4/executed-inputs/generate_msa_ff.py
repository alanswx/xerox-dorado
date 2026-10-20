#!/usr/bin/env python3
"""Physical-state adapters for the MSA's TTL and DRAM-address families.

Pinned original processes run unchanged under host advance. Holding freezes
both data and sampled clock/strobe histories; a held write restores them all.
These are components, not native MSA integration or a memory-service barrier.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import generate_snapshot_ff as ff

ROOT = Path(__file__).resolve().parents[3]
SHA = {
    'cell_SN74166': 'ed87a73b4f61333de3d19ff700e2327d8b9003a8b4d59cae6d62a31b7f2b3232',
    'cell_SN74S174': '88dad140f27625a0aa42ab98b5f1147b8c02ce2ece406f34852523dac0d55746',
    'cell_MK4096P_addr': 'fbb33241670944e9957c9f187bf8102473a1830a5b502f7ad05e66adb4c29d9a',
}
FIELDS = {
    'cell_SN74166': [('q', 8), ('ck_d', 1)],
    'cell_SN74S174': [('q', 6), ('ck_d', 1)],
    'cell_MK4096P_addr': [('row', 'ROW_BITS'), ('col', 'ROW_BITS'), ('ras_d', 1), ('cas_d', 1)],
}
PROCESSES = {
    'cell_SN74166': [
        'always @(posedge sys_clk) ck_d <= p7;',
        '''always @(posedge sys_clk) begin
    if (!p9) q <= 8'd0;
    else if (ck_en && !p6) begin
      if (!p15) q <= {p14,p12,p11,p10,p5,p4,p3,p2};
      else q <= {q[6:0], p1};
    end
  end'''],
    'cell_SN74S174': [
        'always @(posedge sys_clk) ck_d <= p9;',
        '''always @(posedge sys_clk) begin
    if (!p1) q <= 6'd0;
    else if (ck_en) q <= {p14, p13, p11, p6, p4, p3};
  end'''],
    'cell_MK4096P_addr': [
        'always @(posedge sys_clk) begin ras_d <= p4; cas_d <= p15; end',
        '''always @(posedge sys_clk) begin
    if (~p4 & ras_d) row <= a;
    if (~p15 & cas_d) col <= a;
  end'''],
}


def fields(kind, row_bits=8):
    if row_bits not in (7, 8):
        raise ValueError('unqualified DRAM address geometry')
    return [(n, row_bits if w == 'ROW_BITS' else w) for n, w in FIELDS[kind]]


def generate_one(kind, source=None):
    if source is None:
        source = (ROOT / 'verilog/cells' / (kind + '.v')).read_text()
    if hashlib.sha256(source.encode()).hexdigest() != SHA[kind]:
        raise ValueError('unreviewed original source: ' + kind)
    text, normal = source, []
    for fragment in PROCESSES[kind]:
        text, process = ff.extract_once(text, fragment)
        normal.append(re.sub(r'^always\s*@\s*\(\s*posedge\s+sys_clk\s*\)\s*', '', process, count=1))
    if re.search(r'\b(always\w*|initial|final)\b', ff.masked_comments(text)):
        raise ValueError('unexported state process')
    text = ff.contb.replace_once(text, 'module ' + kind + ' ', 'module ' + kind + '_msa_snapshot ')
    old = '    input  wire        sys_clk,' if kind == 'cell_MK4096P_addr' else '    input  wire sys_clk,'
    text = ff.contb.replace_once(text, old, '''    input wire state_host_clk,state_advance,state_hold,state_write,
    input wire [17:0] state_data,
    output wire [17:0] state_result,
''' + old)
    packed = '{' + ','.join(n for n, _ in reversed(FIELDS[kind])) + '}'
    width = '2*ROW_BITS+2' if kind == 'cell_MK4096P_addr' else str(sum(w for _, w in fields(kind)))
    guard = '''  generate if(ROW_BITS!=7 && ROW_BITS!=8)begin:unqualified_geometry
    initial $fatal(1,"MSA snapshot address geometry must be 7 or 8");
  end endgenerate
''' if kind == 'cell_MK4096P_addr' else ''
    process = guard + f'''  assign state_result = {{{{(18-({width})){{1'b0}}}},{packed}}};
  always @(posedge state_host_clk) begin
    if(state_hold)begin
      if(state_write) {packed} <= state_data[({width})-1:0];
    end else if(state_advance)begin
''' + '\n'.join(normal) + '''
    end
  end
'''
    text = ff.contb.replace_once(text, 'endmodule', process + 'endmodule')
    return text


def generate():
    return {k + '_msa_snapshot.sv': generate_one(k) for k in FIELDS}


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    a.output.mkdir(parents=True, exist_ok=False)
    generated = generate()
    for name, source in generated.items():
        (a.output / name).write_text(source)
    (a.output / 'manifest.json').write_text(json.dumps(dict(scope=__doc__, source_sha256=SHA,
        fields=FIELDS, qualified_row_bits=[7, 8], generated_sha256={n: hashlib.sha256(s.encode()).hexdigest()
        for n, s in generated.items()}), indent=2) + '\n')
