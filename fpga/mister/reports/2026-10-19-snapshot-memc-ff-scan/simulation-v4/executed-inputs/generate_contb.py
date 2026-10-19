#!/usr/bin/env python3
"""Generate an experimental ContB with bounded IM state access.

Production generated sources remain untouched. Only the144 F10415A instances
are replaced; every other byte in the board body is retained. Eight audited
18-bit half-banks retain original address, data, WE, CE and private output
mapping. The caller must stop ALL guest state and supply the falling-edge
latched enable actually used by its clock gate. This is not a global barrier
or a complete ContB/machine snapshot: non-IM sequential cells are unchanged.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

import audit_contb_im as im

MODULE = 'ContB_snapshot_experiment'
PORTS = '''
    input wire state_host_clk, state_guest_advance,
    input wire state_freeze, state_guest_held,
    output wire state_frozen,
    input wire state_request, state_write,
    input wire [13:0] state_index,
    input wire [17:0] state_data,
    output wire state_done, state_error,
    output wire [17:0] state_result,'''


def generate():
    original = im.GENERATED.read_text()
    report = im.audit(original)
    pins = {name: p for kind, name, p in im.instances(original)
            if kind == 'cell_F10415A'}
    text, count = re.subn(r'\bcell_F10415A\s+\w+\s*\(.*?\)\s*;[^\n]*', '', original, flags=re.S)
    assert count == 144
    text = text.replace('module ContB_m_Rev_m_Cd', 'module '+MODULE, 1)
    text = text.replace('    input  wire sys_clk,', PORTS+'\n    input  wire sys_clk,', 1)
    rows = []
    code = ['\n  // Generated experimental IM state fabric. No other cell changes.',
            '  wire [7:0] state_bank_frozen, state_bank_done, state_bank_error;',
            '  wire [17:0] state_bank_result [0:7];',
            '  reg [2:0] state_response_bank;',
            '  assign state_frozen = &state_bank_frozen;',
            '  wire state_accept = state_freeze && state_guest_held &&',
            '                      state_frozen && state_request;',
            '  always @(posedge state_host_clk)',
            '    if (state_accept) state_response_bank <= state_index[13:11];',
            '  assign state_done = |state_bank_done;',
            '  assign state_error = |state_bank_error;',
            '  assign state_result = state_bank_result[state_response_bank];']
    for i, group in enumerate(report['groups']):
        members = group['members']
        p = pins['u_'+members[0]['package']]
        address = [p['p'+str(n)] for n in im.ADDRESS]
        data = [pins['u_'+m['package']]['p15'] for m in members]
        outputs = [pins['u_'+m['package']]['p1'] for m in members]
        code += [f'  wire [17:0] state_bank_q_{i};',
                 f'  dorado_snapshot_f10415_enable_bank snapshot_bank_{i} (',
                 '    .host_clk(state_host_clk), .guest_advance(state_guest_advance),',
                 '    .address({'+', '.join(address)+'}),',
                 '    .data({'+', '.join(reversed(data))+'}),',
                 f'    .ce_n({p["p14"]}), .we_n({p["p13"]}), .q(state_bank_q_{i}),',
                 '    .freeze(state_freeze), .guest_held(state_guest_held),',
                 f'    .frozen(state_bank_frozen[{i}]),',
                 f"    .request(state_accept && state_index[13:11] == 3'd{i}),",
                 '    .write(state_write), .word_index(state_index[10:0]),',
                 '    .state_data(state_data),',
                 f'    .done(state_bank_done[{i}]), .error(state_bank_error[{i}]),',
                 f'    .result(state_bank_result[{i}])',
                 '  );']
        code += [f'  assign {n} = state_bank_q_{i}[{b}];' for b, n in enumerate(outputs)]
        rows.append(dict(index=i, bank=group['bank'], half=group['half'],
                         address=address, ce=p['p14'], we=p['p13'],
                         bits=[dict(**m, data=data[b], output=outputs[b], bit=b)
                               for b, m in enumerate(members)]))
    text = text.replace('endmodule', '\n'.join(code)+'\nendmodule', 1)
    return text, dict(scope='Experimental actual ContB IM access only; other sequential cells and global barriers not exported',
                      module=MODULE, groups=rows,
                      generated_sha256=hashlib.sha256(text.encode()).hexdigest(),
                      original_sha256=report['generated_sha256'],
                      grouping_sources=report['sources'],
                      wirelist_sha256=report['wirelist_sha256'])


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=False)
    text, manifest = generate()
    (args.output/'ContB_snapshot.sv').write_text(text)
    (args.output/'manifest.json').write_text(json.dumps(manifest, indent=2)+'\n')
    print('Generated eight18-bit banks; production board unchanged.')
