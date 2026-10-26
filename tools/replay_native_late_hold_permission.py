#!/usr/bin/env python3
"""Replay source-bound late-Hold dependence witnesses in both RTL simulators.

Arbitrary component inputs are not claimed reachable firmware states. These
witnesses reject an unrestricted pin386 deletion; they do not justify a new
assumption about valid machine states or bypass any actual guard.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time

from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha


def signals(path):
    return {s['name']: s['data'][0] if 'data' in s else s['wave'][0]
            for s in json.loads(path.read_text())['signal']}


def run(source, out):
    r = json.loads((source/'run-state.json').read_text())
    assert r['status'] == 'TERMINAL_DEPENDENCE_PROBE'
    verify(Path(r['stage']), r['source_hashes'])
    assert all(a['result'] == 'DEPENDENCE_COUNTEREXAMPLE' for a in r['actions'])
    assert not out.exists()
    out.mkdir(parents=True)
    cases, body = [], []
    names = ['validation_0', 'validation_1', 'permission_0', 'permission_1', 'validation_equal', 'permission_equal']
    width = r['arbitrary_binary_input_bits']
    assert width in [1980, 1978]
    for action in r['actions']:
        d = source/action['label']
        verify(d, action['files'])
        assert sha(d/'counterexample.json') == action['counterexample_sha256']
        s = signals(d/'counterexample.json')
        assert len(s['stimulus']) == width and set(s['stimulus']) <= {'0', '1'}
        for name in names:
            assert len(s[name]) == (102 if name.startswith('validation_') and name != 'validation_equal' else 1)
            assert set(s[name]) <= {'0', '1'}
        if action['label'] == 'permission':
            assert s['permission_0'] != s['permission_1']
        else:
            assert s['validation_0'] != s['validation_1']
        body += [f" stimulus={width}'b{s['stimulus']};", ' #10;']
        for name in names:
            body.append(f" if({name} !== {len(s[name])}'b{s[name]}) $fatal(1,\"{action['label']} {name}=%b\",{name});")
        body.append(f' $display("LATE_HOLD_REPLAY_CASE_PASS {action["label"]} validation0=%h validation1=%h permission0=%b permission1=%b",validation_0,validation_1,permission_0,permission_1);')
        stimulus = int(s['stimulus'], 2)
        decoded = {name: f'{(stimulus >> row["offset"]) & ((1 << row["width"])-1):x}'
                   for name, row in r['stimulus_layout'].items()}
        changed = int(s['validation_0'], 2) ^ int(s['validation_1'], 2)
        cases.append(dict(label=action['label'], source_counterexample_sha256=sha(d/'counterexample.json'),
                          decoded_inputs_hex=decoded, validation_changed_bits=[i for i in range(102) if changed >> i & 1],
                          validation_0_hex=f'{int(s["validation_0"],2):x}', validation_1_hex=f'{int(s["validation_1"],2):x}',
                          permission_0=s['permission_0'], permission_1=s['permission_1']))
        shutil.copy2(d/'counterexample.json', out/(action['label']+'-counterexample.json'))
    # Both experiments use the same complete source module bank and miter.
    for f in (source/'validation').glob('*.sv'):
        assert sha(f) == sha(source/'permission'/f.name)
        shutil.copy2(f, out/f.name)
    top = f'''`timescale 1ns/1ps
module tb_native_late_hold;
 reg [{width-1}:0] stimulus;
 wire [101:0] validation_0,validation_1;
 wire permission_0,permission_1,validation_equal,permission_equal;
 late_hold_dependence dut(stimulus,validation_0,validation_1,permission_0,permission_1,validation_equal,permission_equal);
 initial begin
{chr(10).join(body)}
 $display("NATIVE_LATE_HOLD_COUNTEREXAMPLES_PASS cases=2");
 $finish;
 end
endmodule
'''
    (out/'tb.sv').write_text(top)
    shutil.copy2(Path(__file__), out/Path(__file__).name)
    state = dict(status='RUNNING', started=time.time(), source_state_sha256=sha(source/'run-state.json'),
                 scope=__doc__, cases=cases, source_hashes=r['source_hashes'], actions=[],
                 files={p.name: sha(p) for p in out.iterdir() if p.is_file()},
                 native_RTL_or_guard_changed=False, firmware_reachability_established=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    paths = [str(f) for f in sorted(out.glob('*.sv'))]
    for engine in ['icarus', 'verilator']:
        commands = ([['iverilog', '-g2012', '-s', 'tb_native_late_hold', '-o', str(out/'test.vvp'), *paths],
                     ['vvp', str(out/'test.vvp')]] if engine == 'icarus' else
                    [['verilator', '--binary', '--timing', '--top-module', 'tb_native_late_hold', '-Wno-fatal',
                      '--Mdir', str(out/'obj'), '-o', 'test', '-j', '2', *paths], [str(out/'obj/test')]])
        for i, command in enumerate(commands):
            label = engine + ('-compile' if i == 0 else '-test')
            with (out/(label+'.log')).open('x') as log:
                child = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT)
                action = dict(label=label, pid=child.pid, command=command, started=time.time())
                state['actions'].append(action)
                save()
                code = child.wait()
            action.update(returncode=code, finished=time.time(), log_sha256=sha(out/(label+'.log')))
            save()
            if code:
                state.update(status='TERMINAL_REPLAY_FAILED', finished=time.time())
                save()
                return 1
        assert 'NATIVE_LATE_HOLD_COUNTEREXAMPLES_PASS cases=2' in (out/(engine+'-test.log')).read_text()
    state.update(status='PASS_DUAL_ENGINE_NATIVE_LATE_HOLD_DEPENDENCE_REPLAY', finished=time.time())
    save()
    print(state['status'], flush=True)
    return 0


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['source', 'output']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    raise SystemExit(run(a.source.resolve(), a.output.resolve()))
