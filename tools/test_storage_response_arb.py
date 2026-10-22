#!/usr/bin/env python3
"""Check 128-group response overlap with independent routing/fairness state.

Run both simulators, retain frozen inputs and rejection controls. Component
transport checks do not establish native timing, firmware or machine speed.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(generated, out):
    assert not out.exists(), 'Inspect the existing process/result before another run'
    parent = json.loads((generated / 'provenance.json').read_text())
    source = generated / 'dorado_storage_arb.sv'
    assert sha(source) == parent['generated_hashes'][source.name]
    out.mkdir(parents=True)
    inputs = out / 'inputs'
    inputs.mkdir()
    for p in [source, ROOT / 'verilog/verilator/tb_storage_response_arb.sv',
              Path(__file__).resolve()]:
        shutil.copyfile(p, inputs / p.name)
    lookahead = parent.get('request_lookahead', False)
    if lookahead:
        test = inputs / 'tb_storage_response_arb.sv'
        body = test.read_text()
        old = '.RESPONSE_OVERLAP(OVERLAP)) dut('
        assert body.count(old) == 1
        test.write_text(body.replace(old, '.RESPONSE_OVERLAP(OVERLAP),.REQUEST_LOOKAHEAD(OVERLAP)) dut('))
        shutil.copyfile(ROOT / 'verilog/verilator/tb_storage_request_lookahead.sv',
                        inputs / 'tb_storage_request_lookahead.sv')
    hashes = {p.name: sha(p) for p in inputs.iterdir()}
    (out / 'provenance.json').write_text(json.dumps(dict(
        scope=__doc__, generated=parent, inputs=hashes), indent=2) + '\n')
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(),
                 actions=[], results={})

    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')

    def execute(command, label):
        with (out / (label + '.log')).open('x') as log:
            process = subprocess.Popen(command, cwd=out, stdout=log, stderr=subprocess.STDOUT)
            row = dict(label=label, pid=process.pid, command=command, started=time.time())
            state['actions'].append(row)
            save()
            try:
                code = process.wait(timeout=180)
            except subprocess.TimeoutExpired:
                process.kill()
                process.wait()
                raise
            row.update(returncode=code, finished=time.time())
            save()
        return code

    def case(label, simulator, source_path, diagnostic=None, dynamic=False):
        top = 'tb_storage_request_lookahead' if dynamic else 'tb_storage_response_arb'
        tb = inputs / (top + '.sv')
        if simulator == 'iverilog':
            executable = out / (label + '.vvp')
            compile_command = ['iverilog', '-g2012', '-s', top,
                               '-o', str(executable), str(source_path), str(tb)]
            command = ['vvp', str(executable)]
        else:
            obj = out / ('obj-' + label)
            compile_command = ['verilator', '--binary', '--timing', '-j', '4', '-Wno-fatal',
                               '--top-module', top, '--Mdir', str(obj),
                               '-o', 'test-arb', str(source_path), str(tb)]
            if lookahead:
                # A settling simulation is not proof of acyclic hardware.
                # The rejected native-v1 fit retained the warned feedback.
                compile_command.insert(2, '-Werror-UNOPTFLAT')
            command = [str(obj / 'test-arb')]
        assert execute(compile_command, label + '-compile') == 0, label
        code = execute(command, label + '-test')
        log = (out / (label + '-test.log')).read_text()
        if diagnostic:
            assert code != 0 and diagnostic in log, (label, code, log[-1500:])
            state['results'][label] = dict(rejected=True, diagnostic=diagnostic)
        else:
            success = 'LOOKAHEAD_DYNAMIC PASS' if dynamic else 'RESPONSE_ARBITER PASS groups=128 waves=16'
            assert code == 0 and success in log, (label, code, log[-1500:])
            state['results'][label] = dict(passed=True, output=log)
        save()
        print(label, state['results'][label], flush=True)

    save()
    try:
        frozen_source = inputs / source.name
        case('icarus', 'iverilog', frozen_source)
        case('verilator', 'verilator', frozen_source)
        if lookahead:
            case('icarus-arrivals', 'iverilog', frozen_source, dynamic=True)
            case('verilator-arrivals', 'verilator', frozen_source, dynamic=True)
        text = frozen_source.read_text()
        selected = 'successor' if lookahead else 'chosen'
        controls = [
            ('old-response-owner', f'owner<={selected}_owner;address_latched<={selected}_address;',
             f'owner<=owner;address_latched<={selected}_address;', 'ARB fairness/owner mismatch'),
            ('disabled-overlap', 'wire overlap_request=RESPONSE_OVERLAP && PREFETCH_NEXT &&',
             'wire overlap_request=1\'b0 && PREFETCH_NEXT &&', 'ARB overlap/backpressure coverage missing'),
        ]
        for label, old, new, diagnostic in controls:
            assert text.count(old) == 1
            bad = out / (label + '.sv')
            bad.write_text(text.replace(old, new))
            case(label, 'iverilog', bad, diagnostic)
        if lookahead:
            old = 'wire cached_choice=REQUEST_LOOKAHEAD && look_valid && look_mask==req_valid;'
            assert text.count(old) == 1
            bad = out / 'ignore-late-arrival.sv'
            bad.write_text(text.replace(old, 'wire cached_choice=REQUEST_LOOKAHEAD && look_valid;'))
            case('ignore-late-arrival', 'iverilog', bad,
                 'LOOK strict overlap priority mismatch', dynamic=True)
        assert all(sha(inputs / n) == h for n, h in hashes.items())
        state.update(status=('PASS_REQUEST_LOOKAHEAD_ARBITER_STATIC_DYNAMIC' if lookahead
                             else 'PASS_RESPONSE_OVERLAP_128_GROUP_ARBITER'),
                     inputs_unchanged=True, finished=time.time())
        save()
    except BaseException as error:
        state.update(status='FAIL', error=repr(error), finished=time.time())
        save()
        raise


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--generated', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    run(args.generated.resolve(), args.out.resolve())
