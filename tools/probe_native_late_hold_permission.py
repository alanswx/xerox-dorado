#!/usr/bin/env python3
"""Measure actual native validation dependence on late MemC Hold pin386.

Two complete single-step functions share arbitrary binary inputs. The second
has only after-pins386 forced low. Native advance, pair_fall, cp_reset and
refresh_before_run_capture are tied low by the actual packet/backplane.
A counterexample is evidence against removing this dependency,
not a failed qualification to suppress. No RTL or readiness guard is changed.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from qualify_native_ready_compare import SINGLE, CONTROL, ports
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

NATIVE_CONSTANTS = dict(advance=0, pair_fall=0, cp_reset=0, refresh_before_run_capture=0)
BACKPLANE = 'src/build/rtl-executive/native16-tx-cable/dorado_native_backplane.v'


def run(stage, out, yosys):
    assert not out.exists()
    out.mkdir(parents=True)
    manifest = stage / 'r-front-control-native.json'
    m = json.loads(manifest.read_text())
    verify(stage, m['source_hashes'])
    control = (stage/CONTROL).read_text()
    single_call = re.search(r' '+SINGLE+r' single\((.*?)\);', control, re.S)[1]
    for name in ['advance', 'pair_fall', 'cp_reset']:
        assert f'.{name}(1\'b0)' in single_call
    assert '.refresh_before_run_capture(refresh)' in single_call
    backplane = (stage/BACKPLANE).read_text().split('module dorado_native_backplane_ce #', 1)[1].split('endmodule', 1)[0]
    assert '.clock_after(CLK_ca_p_),.refresh(1\'b0),.reset(storage_reset)' in backplane
    assert '.use_pair(1\'b0)' in backplane
    bank, owners = {}, {}
    for path in m['source_hashes']:
        if not path.startswith('src/verilog/rtl/') or Path(path).suffix not in ['.v', '.sv']:
            continue
        for match in re.finditer(r'^module\s+(\w+)\b.*?^endmodule', (stage/path).read_text(), re.M | re.S):
            assert match[1] not in bank
            bank[match[1]] = match[0] + '\n'
            owners[match[1]] = path
    chosen = set()

    def collect(name):
        if name in chosen:
            return
        chosen.add(name)
        for other in re.findall(r'^\s*(\w+)\s+(?:#\([\s\S]*?\)\s+)?\w+\s*\(', bank[name], re.M):
            if other in bank:
                collect(other)

    collect(SINGLE)
    rows = ports(bank[SINGLE])
    inputs = [(w, n) for d, w, n in rows if d == 'input']
    assert sum(w for w, _ in inputs) == 1982
    declarations, args, offset, layout = [], [[], []], 0, {}
    for width, name in inputs:
        if name in NATIVE_CONSTANTS:
            values = ["1'b0"] * 2
        else:
            declarations.append(f' wire [{width-1}:0] {name}=stimulus[{offset}+:{width}];')
            layout[name] = dict(offset=offset, width=width)
            offset += width
            values = [name, name]
            if name == 'pins_after':
                values[1] = "(pins_after & ~(401'b1<<386))"
        for i in range(2):
            args[i].append(f'.{name}({values[i]})')
    for i in range(2):
        args[i].extend([f'.repeated_events(validation_{i}[99:0])',
                        f'.unsettled(validation_{i}[100])',
                        f'.shared_state_mismatch(validation_{i}[101])'])
    top = f'''module late_hold_dependence(input wire [{offset-1}:0] stimulus,
 output wire [101:0] validation_0,validation_1,
 output wire permission_0,permission_1,validation_equal,permission_equal);
{chr(10).join(declarations)}
 {SINGLE} original({','.join(args[0])});
 {SINGLE} forced_low({','.join(args[1])});
 assign permission_0=~|validation_0;
 assign permission_1=~|validation_1;
 assign validation_equal=validation_0==validation_1;
 assign permission_equal=permission_0==permission_1;
endmodule
'''
    state = dict(status='RUNNING', started=time.time(), driver_pid=os.getpid(),
                 scope=__doc__, stage=str(stage), manifest_sha256=sha(manifest),
                 source_control_sha256=sha(stage/CONTROL),
                 source_hashes={p: sha(stage/p) for p in {owners[n] for n in chosen}},
                 native_constant_inputs=NATIVE_CONSTANTS,
                 native_constant_binding_source_sha256=sha(stage/BACKPLANE),
                 arbitrary_binary_input_bits=offset, assumptions=0, stimulus_layout=layout,
                 compared_validation_bits=102, actual_guard='~|{repeated_events,unsettled,shared_state_mismatch}',
                 RTL_or_native_wiring_changed=False, timing_or_speed_qualified=False, actions=[])
    shutil.copy2(Path(__file__), out/Path(__file__).name)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    for label in ['validation', 'permission']:
        d = out/label
        d.mkdir()
        for name in chosen:
            (d/(name+'.sv')).write_text(bank[name])
        (d/'top.sv').write_text(top)
        script = f'''read_verilog -sv -defer {' '.join('/work/'+n+'.sv' for n in sorted(chosen))} /work/top.sv
hierarchy -check -top late_hold_dependence
proc
flatten
opt
check -assert
sat -timeout 120 -prove {label}_equal 1 -show-ports -dump_json /work/counterexample.json
'''
        (d/'proof.ys').write_text(script)
        env = os.environ.copy()
        env['YOWASP_CACHE_DIR'] = str(out.parent/'formal-tool-cache')
        env['YOWASP_MOUNT'] = '/work='+str(d)
        cmd = [str(yosys), '-Q', '-T', '-s', '/work/proof.ys']
        with (d/'proof.log').open('x') as log:
            child = subprocess.Popen(cmd, stdout=log, stderr=log, env=env)
            action = dict(label=label, pid=child.pid, command=cmd, started=time.time(),
                          files={p.name: sha(p) for p in d.iterdir() if p.name != 'proof.log'})
            state['actions'].append(action)
            save()
            code = child.wait()
        log = (d/'proof.log').read_text()
        if code == 0 and 'no model found: SUCCESS!' in log:
            result = 'PROVED_INDEPENDENT_FOR_ALL_BINARY_INPUTS'
        elif code == 0 and 'model found: FAIL!' in log and (d/'counterexample.json').exists():
            result = 'DEPENDENCE_COUNTEREXAMPLE'
        else:
            result = 'INCONCLUSIVE_OR_TOOL_FAILURE'
        action.update(returncode=code, result=result, finished=time.time(), log_sha256=sha(d/'proof.log'))
        if (d/'counterexample.json').exists():
            action['counterexample_sha256'] = sha(d/'counterexample.json')
        save()
        print(label, result, flush=True)
        if result == 'INCONCLUSIVE_OR_TOOL_FAILURE':
            break
    complete = len(state['actions']) == 2 and all(a['result'] != 'INCONCLUSIVE_OR_TOOL_FAILURE' for a in state['actions'])
    state.update(status='TERMINAL_DEPENDENCE_PROBE' if complete else 'TERMINAL_INCONCLUSIVE_PROBE', finished=time.time())
    save()
    return 0 if complete else 1


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'output', 'yosys']:
        parser.add_argument('--'+name, type=Path, required=True)
    a = parser.parse_args()
    raise SystemExit(run(a.stage.resolve(), a.output.resolve(), a.yosys.resolve()))
