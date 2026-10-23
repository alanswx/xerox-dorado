#!/usr/bin/env python3
"""Measure pinned synthetic fanout through exact native128 raw arbitration.

Use the 5c65 native GROUPS/address/parameter contract and independent XSDS pins.
This is a transport experiment, not CPU/Cedar execution or hardware speed proof.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time
import csv

ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / 'build/rtl-executive/storage-old-word-native57-v1-20261022/terminal-v1/raw'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run(generated, fixture, output, controls):
    assert not output.exists(), 'Inspect existing work rather than restarting it'
    proof = json.loads((generated / 'provenance.json').read_text())
    for name, digest in proof['generated_hashes'].items():
        assert sha(generated / name) == digest
    arb = generated / 'dorado_storage_arb.sv'
    assert arb.read_bytes() == (BASE / 'src/fpga/mister/dorado_storage_arb.sv').read_bytes()
    native = BASE / 'src/build/rtl-executive/native16-tx-cable/dorado_native_machine.sv'
    qualification = BASE.parents[1] / 'preassembly-qualification-v1.json'
    qualified = json.loads(qualification.read_text())
    for path in [BASE / 'src/fpga/mister/dorado_storage_arb.sv', native]:
        assert sha(path) == qualified['source_hashes'][str(path.relative_to(BASE))]
    wrapper = native.read_text()
    assert '.GROUPS(GROUPS),.GROUP_ADDR_BITS(16),.RAW_ADDR_BITS(RAW_BITS),.PREFETCH_NEXT(1),.INTERLEAVE_GROUPS(1),.RESPONSE_OVERLAP(1),.REQUEST_LOOKAHEAD(1)' in wrapper
    assert 'localparam integer GROUPS=128' in wrapper and 'RAW_BITS=23' in wrapper
    output.mkdir(parents=True)
    inputs = output / 'inputs'
    inputs.mkdir()
    for source, name in [(arb, 'arb.sv'), (generated / 'dorado_storage_sdram_burst.sv', 'backend.sv'),
                         (fixture, 'original-fixture.sv'), (native, 'native-wrapper.sv'),
                         (ROOT / 'verilog/verilator/third_party/xsds/xsds_128mbyte_sdram_model.sv', 'xsds.sv'),
                         (ROOT / 'verilog/verilator/tb_storage_burst_fanout.sv', 'tb.sv'),
                         (Path(__file__), Path(__file__).name)]:
        shutil.copyfile(source, inputs / name)
    fixture_text = fixture.read_text()
    old = '.BURST_READ_SUCCESSOR(0)) dut('
    assert fixture_text.count(old) == 1
    for enabled in [0, 1]:
        (inputs / f'fixture-{enabled}.sv').write_text(fixture_text.replace(old, f'.BURST_READ_SUCCESSOR({enabled})) dut('))
    frozen = {path.name: sha(path) for path in inputs.iterdir()}
    (output / 'provenance.json').write_text(json.dumps(dict(scope=__doc__,
        inputs=frozen, generated_proof=proof,
        native_qualification_sha256=sha(qualification)), indent=2) + '\n')
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

    save()
    try:
        for enabled in [0, 1]:
            label = 'on' if enabled else 'off'
            obj = output / ('obj-' + label)
            compile_command = ['verilator', '--binary', '--timing', '--autoflush', '-j', '2',
                '--top-module', 'tb_storage_burst_fanout', '-Wno-fatal', '-Werror-UNOPTFLAT',
                '-Wno-TIMESCALEMOD', '-Wno-WIDTHTRUNC', '-Wno-WIDTHEXPAND', '-Wno-PINMISSING',
                '--Mdir', obj, '-o', 'test-fanout', inputs / 'arb.sv', inputs / 'backend.sv',
                inputs / f'fixture-{enabled}.sv', inputs / 'xsds.sv', inputs / 'tb.sv']
            assert execute(label + '-compile', compile_command) == 0
            trace = output / (label + '.csv')
            assert execute(label + '-test', [obj / 'test-fanout', '+trace=' + str(trace)]) == 0
            log = (output / (label + '-test.log')).read_text()
            assert 'BURST_FANOUT PASS' in log
            state['results'][label] = dict(
                rounds=[dict(zip(['round', 'mode', 'groups', 'ticks', 'physical_reads', 'forwarded', 'hits'], map(int, row)))
                        for row in re.findall(r'FANOUT_ROUND round=(\d+) mode=(\d+) groups=(\d+) ticks=(\d+) physical_reads=(\d+) forwarded=(\d+) hits=(\d+)', log)],
                classes={int(c): int(t) for c, t in re.findall(r'FANOUT_CLASS id=(\d+) ticks=(\d+)', log)},
                trace_sha256=sha(trace))
            assert len(state['results'][label]['rounds']) == 24
            assert sum(state['results'][label]['classes'].values()) == sum(
                row['ticks'] for row in state['results'][label]['rounds'])
            save()
            print(label, state['results'][label]['classes'], flush=True)
        if controls:
            body = (inputs / 'arb.sv').read_text()
            old = 'rsp_valid[owner]=raw_rsp_valid;'
            assert body.count(old) == 1
            bad = output / 'wrong-owner.sv'
            bad.write_text(body.replace(old, "rsp_valid[owner ^ 7'd1]=raw_rsp_valid;"))
            bad_obj = output / 'obj-wrong-owner'
            command = [str(item).replace(str(obj), str(bad_obj)).replace(str(inputs / 'arb.sv'), str(bad))
                       for item in compile_command]
            assert execute('wrong-owner-compile', command) == 0
            rc = execute('wrong-owner-test', [bad_obj / 'test-fanout', '+trace=' + str(output / 'wrong-owner.csv')])
            log = (output / 'wrong-owner-test.log').read_text()
            diagnostics = [message for message in ['FANOUT wrong owner', 'FANOUT response changed'] if message in log]
            assert rc != 0 and diagnostics, (rc, log[-1800:])
            state['wrong_owner_control'] = dict(rejected=True, diagnostic=diagnostics)
            save()
        traces = []
        for label in ['off', 'on']:
            with (output / (label + '.csv')).open() as stream:
                rows = list(csv.DictReader(stream))
                traces.append({kind: [{k: v for k, v in row.items()
                    if k != 'tick' and not (kind == 'R' and row['write'] == '1' and k == 'data')}
                    for row in rows if row['kind'] == kind] for kind in ['A', 'R']})
        assert traces[0] == traces[1], 'Ordered group requests/completions or valid read data changed'
        assert all(sha(inputs / name) == digest for name, digest in frozen.items())
        state.update(status='PASS_SYNTHETIC_NATIVE128_BURST_FANOUT', finished=time.time(),
                     exact_ordered_operations_and_data=True, inputs_unchanged=True,
                     request_and_completion_channels_compared_separately=True,
                     invalid_write_ack_data_compared=False,
                     native_cpu_cedar_or_hardware_speed_qualified=False)
        save()
    except BaseException as error:
        state.update(status='TERMINAL_FAILED', error=repr(error), finished=time.time())
        save()
        raise


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['generated', 'fixture', 'output']:
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--controls', action='store_true')
    args = parser.parse_args()
    run(args.generated.resolve(), args.fixture.resolve(), args.output.resolve(), args.controls)
