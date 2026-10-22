#!/usr/bin/env python3
"""Prove direct MC10181 carry conditions while retaining the fast result adder.

The original X/Y function table gives X&Y from S[1:0] and X|Y from
S[3:2]. Compute carry outputs from those bit conditions, independently of
the result adder. Keep the exact qualified690f logic/result/carry-select path.
Exhaust all16384 binary inputs/all seven pins in both engines. This is not
four-state equivalence, native timing, firmware or a speed qualification.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

from test_mc10181_lookahead import bench, CELL

QUALIFIED_CELL = '6a1041f76a2262a64b3f7eb803a14c80ae356bb79997928cb8c1b86df714fefa'


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def direct_carry(raw):
    assert sha(raw) == QUALIFIED_CELL
    text = raw.decode()
    start = text.index('  wire [4:0] gsum1 = incremented;')
    end = text.index('\nendmodule', start)
    return (text[:start] + '''  // Algebra of the original X/Y function table (HM3.7):
  // X&Y depends on S1/S0, X|Y on S3/S2. Full case labels retain
  // the original table's default-row choice for unknown select bits;
  // carry lookahead itself is qualified for binary hardware values.
  reg [3:0] bit_generate, bit_propagate;
  always @* begin
    case(S)
      4'd0,4'd4,4'd8,4'd12: bit_generate=4'h0;
      4'd1,4'd5,4'd9,4'd13: bit_generate=A & ~B;
      4'd2,4'd6,4'd10,4'd14: bit_generate=A & B;
      default: bit_generate=A;
    endcase
    case(S)
      4'd0,4'd1,4'd2,4'd3: bit_propagate=A;
      4'd4,4'd5,4'd6,4'd7: bit_propagate=A | B;
      4'd8,4'd9,4'd10,4'd11: bit_propagate=A | ~B;
      default: bit_propagate=4'hf;
    endcase
  end
  wire group_generate=bit_generate[3] |
      (bit_propagate[3]&bit_generate[2]) |
      (bit_propagate[3]&bit_propagate[2]&bit_generate[1]) |
      (bit_propagate[3]&bit_propagate[2]&bit_propagate[1]&bit_generate[0]);
  wire group_propagate=&bit_propagate;
  assign p5=M ? 1'b0 : group_generate | (group_propagate&Cn);
  assign p4=group_generate | group_propagate; // GG: carry out with Cn=1
  assign p8=group_generate;                  // PG: carry out with Cn=0
''' + text[end:]).encode()


def run(parent, output):
    assert not output.exists()
    recipe = json.loads((parent / 'timing-paths-native.json').read_text())
    raw = (parent / CELL).read_bytes()
    assert sha(raw) == recipe['source_hashes'][CELL] == QUALIFIED_CELL
    output.mkdir(parents=True)
    inputs = output / 'inputs'
    inputs.mkdir()
    (inputs / 'reference.v').write_bytes(raw.replace(b'module cell_MC10181 (', b'module original_MC10181 ('))
    (inputs / 'candidate.v').write_bytes(direct_carry(raw))
    (inputs / 'tb.sv').write_bytes(bench())
    (inputs / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    frozen = {p.name: sha(p.read_bytes()) for p in inputs.iterdir()}
    provenance = dict(scope=__doc__, parent_manifest_sha256=sha((parent / 'timing-paths-native.json').read_bytes()),
        original_cell_sha256=sha(raw), binary_cases=16384, outputs=7,
        registers_changed=False, function_table_and_result_path_unchanged=True,
        four_state_equivalent=False, native_qualified=False, inputs=frozen)
    (output / 'provenance.json').write_text(json.dumps(provenance, indent=2) + '\n')
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), actions=[], results={})

    def save():
        (output / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')

    def execute(command, label):
        with (output / (label + '.log')).open('x') as log:
            process = subprocess.Popen(list(map(str, command)), stdout=log, stderr=subprocess.STDOUT)
            row = dict(label=label, pid=process.pid, command=list(map(str, command)), started=time.time())
            state['actions'].append(row)
            save()
            code = process.wait()
            row.update(returncode=code, finished=time.time())
            save()
        return code

    def case(engine, label, source, negative=False):
        label = engine + '-' + label
        sources = [source, inputs / 'reference.v', inputs / 'tb.sv']
        if engine == 'icarus':
            binary = output / (label + '.vvp')
            command = ['iverilog', '-g2012', '-s', 'tb_lookahead', '-o', binary, *sources]
            run_command = ['vvp', binary]
        else:
            obj = output / ('obj-' + label)
            command = ['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal',
                       '-Werror-UNOPTFLAT', '--top-module', 'tb_lookahead', '--Mdir', obj,
                       '-o', 'alu-test', *sources]
            run_command = [obj / 'alu-test']
        assert execute(command, label + '-compile') == 0, label
        assert 'UNOPTFLAT' not in (output / (label + '-compile.log')).read_text(), label
        code = execute(run_command, label + '-test')
        text = (output / (label + '-test.log')).read_text()
        if negative:
            assert code != 0 and 'MC10181_LOOKAHEAD differs' in text, (label, text)
            state['results'][label] = dict(rejected=True, diagnostic=text)
        else:
            assert code == 0 and 'MC10181_LOOKAHEAD PASS all=16384' in text, (label, text)
            state['results'][label] = dict(passed=True, output=text)
        save()
        print(label, 'rejected' if negative else 'PASS', flush=True)

    save()
    try:
        body = (inputs / 'candidate.v').read_bytes()
        variants = [('wrong-select-group', b'bit_generate=A & B;', b'bit_generate=A & ~B;'),
                    ('lost-Cn', b'group_generate | (group_propagate&Cn)', b'group_generate'),
                    ('wrong-GG', b'assign p4=group_generate | group_propagate;', b'assign p4=group_generate;')]
        for engine in ['icarus', 'verilator']:
            case(engine, 'positive', inputs / 'candidate.v')
            for label, old, new in variants:
                assert body.count(old) == 1, label
                path = output / (label + '.v')
                path.write_bytes(body.replace(old, new))
                case(engine, label, path, negative=True)
        assert all(sha((inputs / n).read_bytes()) == h for n, h in frozen.items())
        state.update(status='PASS_MC10181_DIRECT_CARRY_ALL_BINARY_PINS_TWO_SIMULATORS',
                     finished=time.time(), inputs_unchanged=True, native_qualified=False)
        save()
    except BaseException:
        state.update(status='TERMINAL_FAILED', finished=time.time())
        save()
        raise
    return state


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--parent', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    print(run(args.parent.resolve(), args.output.resolve())['status'])
