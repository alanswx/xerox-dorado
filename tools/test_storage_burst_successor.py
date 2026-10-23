#!/usr/bin/env python3
"""Qualify adjacent-beat admission, data, ordering and reset in two simulators.

The protocol bench has a small command-pin functional memory test double.
Independent XSDS timing, original chip arrays and native128-group traffic are
separate gates. No native wrapper, physical clock or architectural state changes.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from prepare_storage_burst_successor import transform

ROOT = Path(__file__).resolve().parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(generated, parent, output, controls):
    assert not output.exists(), 'Inspect existing work rather than restarting it'
    proof = json.loads((generated / 'provenance.json').read_text())
    assert proof['burst_read_successor']
    for name, digest in proof['generated_hashes'].items():
        assert sha(generated / name) == digest, name
    assert transform(parent.read_bytes()) == (generated / 'dorado_storage_sdram_burst.sv').read_bytes()
    output.mkdir(parents=True)
    inputs = output / 'inputs'
    inputs.mkdir()
    for source, name in [(generated / 'dorado_storage_sdram_burst.sv', 'candidate.sv'),
                         (parent, 'parent.sv'),
                         (ROOT / 'verilog/verilator/tb_storage_burst_successor.sv', 'tb.sv'),
                         (Path(__file__), Path(__file__).name),
                         (ROOT / 'tools/prepare_storage_burst_successor.py', 'prepare.py')]:
        shutil.copyfile(source, inputs / name)
    frozen = {path.name: sha(path) for path in inputs.iterdir()}
    (output / 'provenance.json').write_text(json.dumps(dict(
        scope=__doc__, generated_proof=proof, inputs=frozen), indent=2) + '\n')
    state = dict(status='RUNNING', pid=os.getpid(), started=time.time(), actions=[], results={})

    def save():
        temp = output / 'state.tmp'
        temp.write_text(json.dumps(state, indent=2) + '\n')
        os.replace(temp, output / 'run-state.json')

    def execute(label, command):
        command = list(map(str, command))
        with (output / (label + '.log')).open('x') as log:
            process = subprocess.Popen(command, cwd=inputs, stdout=log, stderr=subprocess.STDOUT)
            action = dict(label=label, pid=process.pid, command=command, started=time.time())
            state['actions'].append(action)
            save()
            action.update(returncode=process.wait(), finished=time.time())
            save()
        return action['returncode']

    def case(label, engine, source, enabled=1, hz=57142857, diagnostic=None):
        label = engine + '-' + label
        if engine == 'icarus':
            binary = output / (label + '.vvp')
            compile_command = ['iverilog', '-g2012', '-s', 'tb_storage_burst_successor',
                               '-Ptb_storage_burst_successor.ENABLED=' + str(enabled),
                               '-Ptb_storage_burst_successor.CLOCK_HZ=' + str(hz),
                               '-o', binary, source, inputs / 'tb.sv']
            command = ['vvp', binary]
        else:
            obj = output / ('obj-' + label)
            compile_command = ['verilator', '--binary', '--timing', '--autoflush', '-j', '2',
                               '-Wno-fatal', '-Werror-UNOPTFLAT', '--top-module', 'tb_storage_burst_successor',
                               '-GENABLED=' + str(enabled), '-GCLOCK_HZ=' + str(hz),
                               '--Mdir', obj, '-o', 'test-successor', source, inputs / 'tb.sv']
            command = [obj / 'test-successor']
        assert execute(label + '-compile', compile_command) == 0, label
        assert 'UNOPTFLAT' not in (output / (label + '-compile.log')).read_text()
        code = execute(label + '-test', command)
        log = (output / (label + '-test.log')).read_text()
        if diagnostic:
            expected = [diagnostic] if isinstance(diagnostic, str) else diagnostic
            observed = [message for message in expected if message in log]
            assert code != 0 and observed, (label, code, log[-1800:])
            state['results'][label] = dict(rejected=True, diagnostic=observed)
        else:
            assert code == 0 and 'BURST_SUCCESSOR PASS' in log, (label, code, log[-1800:])
            state['results'][label] = dict(passed=True, output=log)
        save()
        print(label, state['results'][label], flush=True)

    save()
    try:
        for engine in ['icarus', 'verilator']:
            case('off57', engine, inputs / 'candidate.sv', enabled=0)
            case('on57', engine, inputs / 'candidate.sv')
            case('on66', engine, inputs / 'candidate.sv', hz=66666667)
        if controls:
            body = (inputs / 'candidate.sv').read_text()
            changes = [
                ('corrupt-forward', '                rsp_data<=dq_capture;\n                response_pending<=1;',
                 "                rsp_data<=dq_capture ^ 16'h0001;\n                response_pending<=1;", 'SUCCESSOR data/order'),
                ('ignore-held-response', '!aborted && response_pending && rsp_ready && !req_write &&',
                 '!aborted && response_pending && !req_write &&',
                 ['SUCCESSOR response changed', 'SUCCESSOR overwrote outstanding request']),
                ('allow-write', 'rsp_ready && !req_write &&', 'rsp_ready &&',
                 ['SUCCESSOR accepted forbidden', 'SUCCESSOR exclusion failed']),
                ('wrong-line', 'offered_address[25:3]==address[25:3] && ', '',
                 ['SUCCESSOR accepted forbidden', 'SUCCESSOR exclusion failed']),
                ('wrong-beat', ' && offered_address[2:0]==fill_beat;', ';',
                 ['SUCCESSOR accepted forbidden', 'SUCCESSOR exclusion failed']),
                ('ignore-refresh', 'initialized && !refresh_due && !reset &&',
                 'initialized && !reset &&', 'SUCCESSOR accepted while refresh due'),
                ('retain-reset-response', '(rsp_ready || reset)', 'rsp_ready',
                 ['SUCCESSOR orphan or duplicate', 'SUCCESSOR stale reset reply']),
                ('duplicate-tail', 'if(!EARLY_READ_RESPONSE)response_pending<=1;',
                 'response_pending<=1;', 'SUCCESSOR orphan or duplicate'),
            ]
            for label, old, new, diagnostic in changes:
                assert body.count(old) == 1, old
                bad = output / (label + '.sv')
                bad.write_text(body.replace(old, new))
                for engine in ['icarus', 'verilator']:
                    case(label, engine, bad, diagnostic=diagnostic)
        assert all(sha(inputs / name) == digest for name, digest in frozen.items())
        state.update(status='PASS_BURST_SUCCESSOR_PROTOCOL_TWO_SIMULATORS', finished=time.time(),
                     frozen_inputs_unchanged=True, physical_timing_model_qualified=False,
                     native128_traffic_or_hardware_speed_qualified=False)
        save()
    except BaseException as error:
        state.update(status='TERMINAL_FAILED', error=repr(error), finished=time.time())
        save()
        raise


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['generated', 'parent', 'output']:
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--controls', action='store_true')
    args = parser.parse_args()
    run(args.generated.resolve(), args.parent.resolve(), args.output.resolve(), args.controls)
