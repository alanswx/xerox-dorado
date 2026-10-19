#!/usr/bin/env python3
"""Exercise a generated actual ContB against the unchanged original board.

The fixture alone seeds RAMs. Normal cycles use unmodified board strobes and
feedback, plus a falling-edge-latched clock gate matching the production gate's
logical contract. All other board state stays clocked exactly as before. This
is IM state access, not a complete board restore or analog clock-skew proof.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import tempfile

import audit_contb_im as im
import generate_contb as gen

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]


def fixture(manifest):
    source = im.GENERATED.read_text()
    ports = re.findall(r'^    (input|output)\s+wire (\w+)[,\n]', source, re.M)
    inputs = [n for d, n in ports if d == 'input' and n != 'sys_clk']
    outputs = [n for d, n in ports if d == 'output']
    assert len(inputs) > 100 and len(outputs) > 60
    code = [f'''module contb_snapshot_fixture(
 input wire host_clk,run,
 input wire [{len(inputs)-1}:0] stimulus,
 input wire freeze,held,request,write,
 input wire [13:0] state_index,input wire [17:0] state_data,
 output wire frozen,done,error,output wire [17:0] result,
 output wire [{len(outputs)-1}:0] original_outputs,candidate_outputs,
 output wire advance,
 output wire [79:0] addresses,
 output wire [143:0] data_bits,original_raw,candidate_raw,
 output wire [7:0] ce_n,we_n);
 // Logical altclkctrl falling-edge enable behavior. Analog insertion/skew
 // remains a separate routed whole-core timing requirement.
 reg enabled=0;
 always @(negedge host_clk) enabled <= run;
 wire guest_clk=host_clk & enabled;
 assign advance=enabled;''']
    for name, module in (('original', 'ContB_m_Rev_m_Cd'), ('candidate', gen.MODULE)):
        for n in outputs:
            code.append(f' wire {name}_{n};')
        connections = ['.sys_clk(guest_clk)']
        for i, n in enumerate(inputs):
            # Backplane self-contributions resolve separately, as in the
            # real generated machine, so divergent RAM cannot be masked by
            # driving both boards from the original board's output.
            feedback = f' | {name}_{n}__drv' if n+'__drv' in outputs else ''
            connections.append(f'.{n}(stimulus[{i}]{feedback})')
        connections += [f'.{n}({name}_{n})' for n in outputs]
        if name == 'candidate':
            connections += ['.state_host_clk(host_clk)', '.state_guest_advance(enabled)',
                            '.state_freeze(freeze)', '.state_guest_held(held)',
                            '.state_frozen(frozen)', '.state_request(request)',
                            '.state_write(write)', '.state_index(state_index)',
                            '.state_data(state_data)', '.state_done(done)',
                            '.state_error(error)', '.state_result(result)']
        code += [f' {module} #(.SYSPER(4)) {name} (',
                 ',\n'.join('  '+c for c in connections), ' );']
        code.append(f' assign {name}_outputs = {{'+', '.join(f'{name}_{n}' for n in reversed(outputs))+'};')
    # Deterministic, nonzero test-only fixture initialization of BOTH copies.
    # This is never compiled into the production MiSTer core.
    code.append(' integer row;\n initial begin\n for(row=0;row<1024;row=row+1) begin')
    for g in manifest['groups']:
        i = g['index']
        for b in g['bits']:
            bit, pkg = b['bit'], b['package']
            value = f"((row*32'd1103515245 + 32'd{12345+i*7919}) >> {bit}) & 1"
            code += [f' original.u_{pkg}.mem[row] = {value};',
                     f' candidate.snapshot_bank_{i}.bits[{bit}].mem[row] = {value};']
    code.append(' end\n end')
    for g in manifest['groups']:
        i = g['index']
        code += [f' assign addresses[{i*10}+:10] = {{'+', '.join('original.'+n for n in g['address'])+'};',
                 f' assign ce_n[{i}] = original.{g["ce"]};',
                 f' assign we_n[{i}] = original.{g["we"]};',
                 f' assign candidate_raw[{i*18}+:18] = candidate.snapshot_bank_{i}.logical_q;']
        for b in g['bits']:
            n = i*18+b['bit']
            code += [f' assign data_bits[{n}] = original.{b["data"]};',
                     f' assign original_raw[{n}] = original.u_{b["package"]}.dout_r;']
    code += ['endmodule', '']
    header = '#pragma once\n'+f'constexpr unsigned INPUTS={len(inputs)}, OUTPUTS={len(outputs)};\n'
    header += ''.join(f'constexpr unsigned I_{n}={i};\n' for i, n in enumerate(inputs))
    return '\n'.join(code), header, dict(inputs=inputs, outputs=outputs)


def run(out, mutations):
    out.mkdir(parents=True, exist_ok=False)
    board, manifest = gen.generate()
    top, header, ports = fixture(manifest)
    (out/'manifest.json').write_text(json.dumps(manifest, indent=2)+'\n')
    (out/'ports.json').write_text(json.dumps(ports, indent=2)+'\n')
    kinds = {k for k, _, _ in im.instances(im.GENERATED.read_text())}
    cells = [ROOT/'verilog/cells'/f'{k}.v' for k in sorted(kinds)]
    cases = [('candidate', board, top, None)]
    if mutations:
        for name, before, after, diagnostic in (
            ('wrong_advance', '.state_guest_advance(enabled)', '.state_guest_advance(run)',
             'board raw pipeline differs'),
            ('wrong_output', 'assign dASEL_0__a06_1 = state_bank_q_0[0];',
             'assign dASEL_0__a06_1 = state_bank_q_0[1];', 'board output differs'),
            ('wrong_row', '.address({RA_01a, RA_02a,', '.address({RA_02a, RA_01a,',
             'board raw pipeline differs'),
            ('wrong_response_bank', 'state_response_bank <= state_index[13:11];',
             "state_response_bank <= state_index[13:11] ^ 3'd1;", 'export differs'),
        ):
            target = top if before in top else board
            assert target.count(before) >= 1, name
            # Damage only one half-bank even when several representative
            # addresses use the same fanout copy.
            altered = target.replace(before, after, 1)
            cases.append((name, board if target == top else altered,
                          altered if target == top else top, diagnostic))
    rows = []
    for name, altered, fixture_text, diagnostic in cases:
        case = out/name
        case.mkdir()
        (case/'ContB_snapshot.sv').write_text(altered)
        (case/'fixture.sv').write_text(fixture_text)
        (case/'ports.h').write_text(header)
        command = ['verilator', '--cc', '--exe', '--build', '-j', '4',
                   '--top-module', 'contb_snapshot_fixture', '-Wno-fatal',
                   '-DDORADO_F10415_M10K', '--Mdir', str(case/'obj'),
                   '-CFLAGS', '-I'+str(case),
                   str(case/'fixture.sv'), str(case/'ContB_snapshot.sv'),
                   str(im.GENERATED), str(HERE/'f10415_enable_bank.sv'),
                   *map(str, cells), str(HERE/'test_contb_snapshot.cpp')]
        (case/'command.json').write_text(json.dumps(command, indent=2)+'\n')
        with (case/'build.log').open('w') as log:
            subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, check=True)
        result = subprocess.run([str(case/'obj/Vcontb_snapshot_fixture')],
                                capture_output=True, text=True, timeout=90)
        text = result.stdout+result.stderr
        (case/'test.txt').write_text(text)
        print(name+': '+text.strip(), flush=True)
        if diagnostic:
            assert result.returncode and diagnostic in text, (name, text)
        else:
            assert result.returncode == 0 and 'ACTUAL CONTB IM PASS' in text, text
        rows.append(dict(case=name, returncode=result.returncode, expected_failure=diagnostic))
    paths = [Path(__file__), HERE/'test_contb_snapshot.cpp', HERE/'generate_contb.py',
             HERE/'audit_contb_im.py', HERE/'f10415_enable_bank.sv', im.GENERATED, *cells]
    (out/'results.json').write_text(json.dumps(dict(
        scope='Actual ContB normal board outputs and bounded IM access; fixture RAM seed, logical gated clock; no full-board/global barrier/physical restore or timing claim',
        hashes={str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest() for p in paths},
        cases=rows), indent=2)+'\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--mutations', action='store_true')
    args = parser.parse_args()
    if args.output:
        run(args.output.resolve(), args.mutations)
    else:
        with tempfile.TemporaryDirectory(prefix='dorado-contb-state-', dir='/tmp') as tmp:
            run(Path(tmp)/'test', args.mutations)
