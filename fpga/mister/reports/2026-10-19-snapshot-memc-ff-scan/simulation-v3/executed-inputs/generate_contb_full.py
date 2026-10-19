#!/usr/bin/env python3
"""Generate full modeled ContB state access without changing production RTL.

The original gated fabric edge becomes the SAME host-clock advance for RAM
and the22 sequential packages. Every q, pre-capture and clock-history register
is exported. This local board experiment still requires a global hold and
external-input/clock/device/service state from the eventual machine snapshot.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

import audit_contb_im as im
import generate_contb as ram

MODULE = 'ContB_full_snapshot_experiment'
FIELDS = {
    'cell_MC10176': [('q', 6), ('d_pre', 6), ('ck_d', 1)],
    'cell_MC10231': [('qb', 1), ('qa', 1), ('d_pre_b', 1),
                     ('d_pre_a', 1), ('ckb_d', 1), ('cka_d', 1)],
}


def replace_once(text, before, after):
    if text.count(before) != 1:
        raise ValueError('unreviewed source shape: '+before[:100])
    return text.replace(before, after, 1)


def ff_sources():
    """Keep original port/comb declarations; merge only reviewed NBA processes.

    All original destinations have one writer. Merging their nonblocking
    assignments preserves pre-edge values and original S/R priority. Exact
    fragments fail closed if an original cell changes; the board/reference
    and state-inventory tests independently check behavior and coverage.
    """
    fragments = {
        'cell_MC10176': [
            'always @(posedge sys_clk) ck_d <= p9;',
            'always @(posedge sys_clk) d_pre <= {p12, p11, p10, p7, p6, p5};',
            'always @(posedge sys_clk) if (ck_en) q <= PRECAP ? d_pre : {p12, p11, p10, p7, p6, p5};',
        ],
        'cell_MC10231': [
            'always @(posedge sys_clk) begin cka_d <= clka; ckb_d <= clkb; end',
            'always @(posedge sys_clk) begin d_pre_a <= p7; d_pre_b <= p10; end',
            '''always @(posedge sys_clk) begin
    if (p5)          qa <= 1'b1;      // S
    else if (p4)     qa <= 1'b0;      // R
    else if (cka_en) qa <= d_a;       // D
  end''',
            '''always @(posedge sys_clk) begin
    if (p12)         qb <= 1'b1;
    else if (p13)    qb <= 1'b0;
    else if (ckb_en) qb <= d_b;
  end''',
        ],
    }
    result = {}
    for kind, blocks in fragments.items():
        source = (im.ROOT/'verilog/cells'/f'{kind}.v').read_text()
        text = replace_once(source, 'module '+kind+' ', 'module '+kind+'_snapshot ')
        ports = '''    input wire state_host_clk, state_advance, state_hold, state_write,
    input wire [17:0] state_data,
    output wire [17:0] state_result,
'''
        text = replace_once(text, '    input  wire sys_clk,', ports+'    input  wire sys_clk,')
        normal = []
        for block in blocks:
            text = replace_once(text, block, '')
            body = block.removeprefix('always @(posedge sys_clk) ')
            normal.append(body)
        stripped = re.sub(r'/\*.*?\*/|//[^\n]*', '', text, flags=re.S)
        if re.search(r'\b(always\w*|initial)\b', stripped):
            raise ValueError('unreviewed extra state process in '+kind)
        fields = FIELDS[kind]
        width = sum(w for _, w in fields)
        packed = '{'+', '.join(n for n, _ in reversed(fields))+'}'
        process = f'''
  // Experimental full-state clock-enable adapter. No guest warm-up edge.
  assign state_result = {{{18-width}'b0, {packed}}};
  always @(posedge state_host_clk) begin
    if (state_hold) begin
      if (state_write) {packed} <= state_data[{width-1}:0];
    end else if (state_advance) begin
'''+ '\n'.join('      '+b for b in normal)+'''
    end
  end
'''
        text = replace_once(text, 'endmodule', process+'endmodule')
        result[kind+'_snapshot.v'] = text
    return result


def generate():
    text, manifest = ram.generate()
    text = replace_once(text, 'module '+ram.MODULE+' ', 'module '+MODULE+' ')
    text = replace_once(text, 'input wire [13:0] state_index,', 'input wire [14:0] state_index,')
    packages = [(kind, name, pins) for kind, name, pins in im.instances(im.GENERATED.read_text())
                if kind in FIELDS]
    if len(packages) != 22 or any(sum(k == kind for k, _, _ in packages) != 11 for kind in FIELDS):
        raise ValueError('expected eleven MC10176 and eleven MC10231 packages')
    rows = []
    for i, (kind, name, pins) in enumerate(packages):
        fields, offset = [], 0
        for field, width in FIELDS[kind]:
            fields.append(dict(name=field, width=width, lsb=offset))
            offset += width
        rows.append(dict(index=i, word=(8 << 11) | i, package=name[2:],
                         kind=kind, width=offset, fields=fields))
        pattern = r'\b'+kind+r'\s+'+name+r'\s*\((.*?)\)\s*;'
        match = re.search(pattern, text, re.S)
        if not match:
            raise ValueError('sequential package not found: '+name)
        body = match[1]
        extra = f'''
    .state_host_clk(state_host_clk), .state_advance(state_guest_advance),
    .state_hold(state_freeze && state_guest_held),
    .state_write(state_ff_write && state_index[10:0] == 11'd{i}),
    .state_data(state_data), .state_result(state_ff_result[{i}]),'''
        replacement = kind+'_snapshot '+name+' ('+extra+body+');'
        text = text[:match.start()]+replacement+text[match.end():]
    start = text.index('  // Generated experimental IM state fabric.')
    end = text.index('  wire [17:0] state_bank_q_0;', start)
    fabric = '''  // Full modeled ContB state: group0..7 IM, group8 sequential packages.
  wire [7:0] state_bank_frozen, state_bank_done, state_bank_error;
  wire [17:0] state_bank_result [0:7];
  wire [17:0] state_ff_result [0:21];
  reg [2:0] state_response_bank;
  reg state_response_aux;
  reg state_aux_done = 0, state_aux_error = 0;
  reg [17:0] state_aux_result;
  assign state_frozen = &state_bank_frozen;
  wire state_accept = state_freeze && state_guest_held && state_frozen && state_request;
  wire state_is_ff = state_index[14:11] == 4'd8;
  wire state_valid_ff = state_is_ff && state_index[10:0] < 11'd22;
  wire state_ff_write = state_accept && state_write && state_valid_ff;
  always @(posedge state_host_clk) begin
    state_aux_done <= state_accept && state_index[14];
    state_aux_error <= state_accept && state_index[14] && !state_valid_ff;
    if (state_accept) begin
      state_response_bank <= state_index[13:11];
      state_response_aux <= state_index[14];
      state_aux_result <= state_valid_ff ? state_ff_result[state_index[10:0]] : 18'b0;
    end
  end
  assign state_done = (|state_bank_done) | state_aux_done;
  assign state_error = (|state_bank_error) | state_aux_error;
  assign state_result = state_response_aux ? state_aux_result : state_bank_result[state_response_bank];
'''
    text = text[:start]+fabric+text[end:]
    for i in range(8):
        before = f".request(state_accept && state_index[13:11] == 3'd{i})"
        text = replace_once(text, before,
                            f".request(state_accept && !state_index[14] && state_index[13:11] == 3'd{i})")
    clones = ff_sources()
    manifest.update(scope='Experimental full modeled ContB state access; no global barrier, external input/clock capture or machine restore',
                    module=MODULE, ff=rows, sequential_bits=sum(r['width'] for r in rows),
                    logical_state_bits=147456+144+sum(r['width'] for r in rows),
                    generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                    cloned_sources={n:hashlib.sha256(s.encode()).hexdigest() for n,s in clones.items()})
    return text, manifest, clones


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=False)
    board, manifest, clones = generate()
    (args.output/'ContB_full_snapshot.sv').write_text(board)
    (args.output/'manifest.json').write_text(json.dumps(manifest, indent=2)+'\n')
    for name, source in clones.items():
        (args.output/name).write_text(source)
