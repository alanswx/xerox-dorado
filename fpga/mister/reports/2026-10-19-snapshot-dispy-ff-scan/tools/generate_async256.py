#!/usr/bin/env python3
"""Isolated exact-advance/state-access clones of the two async 256-word RAMs.

Freeze borrows the existing asynchronous read address. Outputs can change
during scans, so the caller must hold every consumer and external frontend.
This primitive is not a global pause or a complete machine snapshot.
"""
import argparse
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
SPECS = {
    'MB7071H': dict(width=4, sha256='710b890c91d451c6023ba0396d170402ef95355703175d15417a0ccf2112bc2b',
        process="""  always @(posedge sys_clk)
    if (sel & we) mem[a] <= {p16, p17, p18, p19};   // e, d, c, b""",
        enable='sel & we', data='{p16, p17, p18, p19}',
        read='  wire [3:0] q = mem[a];'),
    'F10414': dict(width=1, sha256='85a32a3fcf0e8026682e94881d85d356b6155f0a8fd6ae5b9503d20264333a3c',
        process="""  always @(posedge sys_clk)
    if (sel && !p14) mem[a] <= p13;                      // WE'""",
        enable='sel && !p14', data='p13',
        read="  assign p15 = sel ? mem[a] : 1'b0;"),
}


def generate(kind, original=None):
    spec = SPECS[kind]
    path = ROOT / 'verilog/cells' / ('cell_' + kind + '.v')
    if original is None:
        original = path.read_text()
    # Pin the entire reviewed primitive, including its only state/process.
    # A future added pipeline/history bit must trigger explicit schema review.
    if hashlib.sha256(original.encode()).hexdigest() != spec['sha256']:
        raise ValueError('unreviewed original primitive: ' + kind)
    text = original.replace('module cell_' + kind + ' (',
                            'module cell_' + kind + '_snapshot (', 1)
    ports = '''    input wire state_host_clk, state_advance, state_freeze, state_guest_held,
    input wire state_request, state_write,
    input wire [8:0] state_index,
    input wire [3:0] state_data,
    output reg state_frozen = 1'b0,
    output reg state_done = 1'b0, state_error = 1'b0,
    output reg [3:0] state_result,
'''
    assert text.count('    input  wire sys_clk,') == 1
    text = text.replace('    input  wire sys_clk,', ports + '    input  wire sys_clk,', 1)
    width = spec['width']
    load = 'state_data' if width == 4 else 'state_data[0]'
    result = 'state_read' if width == 4 else "{3'b0,state_read}"
    process = f'''  // One physical RAM/read port; state scans temporarily borrow its address.
  wire state_hold = state_freeze && state_guest_held;
  wire state_accept = state_hold && state_frozen && state_request;
  wire state_valid = !state_index[8];
  wire [7:0] state_address = state_hold ? state_index[7:0] : a;
  wire [{width-1}:0] state_read = mem[state_address];
  wire state_restore = state_accept && state_write && state_valid;
  wire state_guest_write = !state_hold && state_advance && ({spec['enable']});
  wire [{width-1}:0] state_write_data = state_hold ? {load} : {spec['data']};
  always @(posedge state_host_clk) begin
    state_frozen <= state_hold;
    state_done <= state_accept;
    state_error <= state_accept && !state_valid;
    if (state_accept) begin
      state_result <= state_valid && !state_write ? {result} : 4'b0;
    end
    if (state_restore || state_guest_write)
      mem[state_address] <= state_write_data;
  end'''
    assert text.count(spec['process']) == 1 and text.count(spec['read']) == 1
    text = text.replace(spec['process'], process, 1)
    read = '  wire [3:0] q = state_read;' if width == 4 else "  assign p15 = sel ? state_read[0] : 1'b0;"
    text = text.replace(spec['read'], read, 1)
    manifest = dict(module='cell_' + kind + '_snapshot', words=256, width=width,
        logical_state_bits=256*width, valid_indices=[0,255], invalid_indices=[256,511],
        original_sha256=spec['sha256'], generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
        read_port='Existing asynchronous port borrowed during held freeze; guest output can change during scanning',
        response='Registered read value; writes return zero. Invalid index returns zero/error and cannot mutate',
        boundary='Requests ignored while running, entering freeze or releasing. Actual guest edge selected by state_advance.',
        scope=__doc__)
    return text, manifest


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=False)
    manifests = {}
    for kind in SPECS:
        source, manifests[kind] = generate(kind)
        (args.output / ('cell_' + kind + '_snapshot.sv')).write_text(source)
    (args.output/'manifest.json').write_text(json.dumps(manifests, indent=2)+'\n')
