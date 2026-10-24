#!/usr/bin/env python3
"""Encode the exact ten-state BaseBoard phase without wide phase arithmetic.

The existing 19-bit phase is always 47060 times a value in [0,9], advancing
by three modulo ten. Keep exact timestamps via a constant encoding table;
preserve every source frequency, merged event, reset and transport pause.
Preparation is not qualification: rerun complete references and induction.
"""
import argparse
import json
from pathlib import Path

from test_r_front_forward_view import sha


def prepare(source, out):
    assert not out.exists()
    raw = source.read_text()
    old_decl = ' reg [18:0] bb_phase=0;'
    new_decl = ''' reg [3:0] bb_phase_tenths=0;
 function automatic [18:0] encode_bb_phase(input [3:0] tenths);
  case(tenths)
CASES
   default:encode_bb_phase=19'd0;
  endcase
 endfunction
 wire [18:0] bb_phase=encode_bb_phase(bb_phase_tenths);'''.replace('CASES', '\n'.join(
        f"   4'd{i}:encode_bb_phase=19'd{i*47060};" for i in range(11)))
    replacements = [(old_decl, new_decl),
        (" wire [19:0] bb_sum={1'b0,bb_phase}+20'd141180;", " wire [4:0] bb_tenths_sum={1'b0,bb_phase_tenths}+5'd3;"),
        (" wire bb_due=bb_sum>=20'd470600;", " wire bb_due=bb_phase_tenths>=4'd7;"),
        (" wire [18:0] bb_remaining=19'd470600-bb_phase;", " wire [18:0] bb_remaining=encode_bb_phase(4'd10-bb_phase_tenths);"),
        ('if(reset)begin bb_phase<=0;', 'if(reset)begin bb_phase_tenths<=0;'),
        ("   bb_phase<=19'(bb_due ? bb_sum-20'd470600 : bb_sum);", "   bb_phase_tenths<=4'(bb_due ? bb_tenths_sum-5'd10 : bb_tenths_sum);")]
    candidate = raw
    for before, after in replacements:
        assert candidate.count(before) == 1
        candidate = candidate.replace(before, after, 1)
    restored = candidate
    for before, after in reversed(replacements):
        assert restored.count(after) == 1
        restored = restored.replace(after, before, 1)
    assert restored == raw
    assert candidate[candidate.index(' reg [17:0] candidate_time'):candidate.index(' always @(posedge clk)begin')] == raw[raw.index(' reg [17:0] candidate_time'):raw.index(' always @(posedge clk)begin')]
    out.mkdir(parents=True)
    (out/'original.sv').write_text(raw)
    (out/'dorado_primary_clock_events.sv').write_text(candidate)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    (out/'results.json').write_text(json.dumps(dict(status='PREPARED_EXACT_BASEBOARD_PHASE',
        source=str(source), original_sha256=sha(source), phase_states=10, units_per_tenth=47060,
        merge_body_byte_unchanged=True, Ether_state_byte_unchanged=True, extra_virtual_cycles=0,
        files={p.name: sha(p) for p in out.iterdir() if p.is_file()},
        physical_timing_qualified=False, native_consumers_integrated=False), indent=2)+'\n')
    print('PREPARED_EXACT_BASEBOARD_PHASE', flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['source', 'output']:
        parser.add_argument('--'+name, type=Path, required=True)
    args = parser.parse_args()
    prepare(args.source.resolve(), args.output.resolve())
