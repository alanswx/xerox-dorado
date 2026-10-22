#!/usr/bin/env python3
"""Qualify carry selection after the original MC10181 group sum.

Keep the original function table and group-sum adder. Compute the incremented
sum with parallel prefix terms, then select by Cn; do not serialize a second
carry-in adder. Exhaust all16384 binary inputs and all seven output pins.
This differs from the previously rejected all-Boolean X/Y carry rewrite.
Canonical cells, registers, captures and hardware are unchanged.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

from test_mc10181_lookahead import CELL, bench


def sha(b):
    return hashlib.sha256(b).hexdigest()


def carry_select(raw):
    s = raw.decode()
    old = "  wire [4:0] sum  = gsum + {4'd0, Cn};"
    assert s.count(old) == 1
    s = s.replace(old, '''  // Preserve the original group-sum FPGA carry chain. Increment each
  // bit in parallel; a late Cn selects an already computed result.
  wire [4:0] increment_mask={(&gsum[3:0]),(&gsum[2:0]),
                             (&gsum[1:0]),gsum[0],1'b1};
  wire [4:0] incremented=gsum ^ increment_mask;
  wire [4:0] sum  = Cn ? incremented : gsum;''')
    old = "wire [4:0] gsum1 = gsum + 5'd1;"
    assert s.count(old) == 1
    return s.replace(old, 'wire [4:0] gsum1 = incremented;').encode()


def run(parent, out):
    assert not out.exists()
    r = json.loads((parent / 'storage-response-native.json').read_text())
    raw = (parent / CELL).read_bytes()
    assert sha(raw) == r['source_hashes'][CELL]
    out.mkdir(parents=True)
    inp = out / 'inputs'
    inp.mkdir()
    (inp / 'reference.v').write_bytes(raw.replace(b'module cell_MC10181 (', b'module original_MC10181 ('))
    (inp / 'candidate.v').write_bytes(carry_select(raw))
    (inp / 'tb.sv').write_bytes(bench().replace(b'MC10181_LOOKAHEAD', b'MC10181_CARRY_SELECT'))
    for source in [Path(__file__).resolve(), Path(__file__).with_name('test_mc10181_lookahead.py')]:
        (inp / source.name).write_bytes(source.read_bytes())
    proof = dict(scope=__doc__, original_cell_sha256=sha(raw),
                 parent_manifest_sha256=sha((parent / 'storage-response-native.json').read_bytes()),
                 inputs={f.name: sha(f.read_bytes()) for f in inp.iterdir()},
                 binary_cases=16384, outputs=7, registers_changed=False, four_state_qualified=False,
                 native_qualified=False)
    (out / 'provenance.json').write_text(json.dumps(proof, indent=2) + '\n')
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), actions=[])

    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')

    def execute(command, label, negative=False):
        with (out / (label + '.log')).open('x') as log:
            p = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT)
            row = dict(stage=label, pid=p.pid, command=command, started=time.time())
            state['actions'].append(row)
            save()
            rc = p.wait()
            row.update(returncode=rc, finished=time.time())
            save()
        body = (out / (label + '.log')).read_text()
        if negative:
            assert rc != 0 and 'MC10181_CARRY_SELECT differs' in body, (label, rc, body[-1800:])
        else:
            assert rc == 0, (label, rc, body[-1800:])

    save()
    try:
        for engine in ['icarus', 'verilator']:
            for name, before, after in [
                ('positive', None, None),
                ('lost-Cn', b'wire [4:0] sum  = Cn ? incremented : gsum;', b'wire [4:0] sum  = gsum;'),
                ('wrong-GG', b'wire [4:0] gsum1 = incremented;', b'wire [4:0] gsum1 = gsum;'),
            ]:
                candidate = inp / 'candidate.v'
                if before:
                    body = candidate.read_bytes()
                    assert body.count(before) == 1
                    candidate = out / (name + '.v')
                    if not candidate.exists():
                        candidate.write_bytes(body.replace(before, after))
                label = engine + '-' + name
                sources = [str(candidate), str(inp / 'reference.v'), str(inp / 'tb.sv')]
                if engine == 'icarus':
                    exe = out / (label + '.vvp')
                    command = ['iverilog', '-g2012', '-s', 'tb_lookahead', '-o', str(exe), *sources]
                    test = ['vvp', str(exe)]
                else:
                    obj = out / ('obj-' + label)
                    command = ['verilator', '--binary', '--timing', '-j', '2', '-Wno-fatal',
                               '--top-module', 'tb_lookahead', '--Mdir', str(obj), '-o', 'alu-test', *sources]
                    test = [str(obj / 'alu-test')]
                execute(command, label + '-compile')
                execute(test, label + '-test', before is not None)
                if not before:
                    assert 'MC10181_CARRY_SELECT PASS all=16384' in (out / (label + '-test.log')).read_text()
                print(label, 'rejected' if before else 'PASS', flush=True)
        assert all(sha((inp / n).read_bytes()) == h for n, h in proof['inputs'].items())
        state.update(status='PASS_MC10181_CARRY_SELECT_ALL_BINARY_PINS_TWO_SIMULATORS', finished=time.time())
        save()
    except BaseException as e:
        state.update(status='TERMINAL_FAILED', error=repr(e), finished=time.time())
        save()
        raise


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--parent', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    run(a.parent.resolve(), a.output.resolve())
