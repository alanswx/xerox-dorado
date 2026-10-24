#!/usr/bin/env python3
"""Prove every126 consumer output bits against the package-tested original.

The only domain restriction is the independently proved source geometry:
count<=6, at most one BB transition, endpoint BB consistent with records.
All six imported physical Qs and all debug inputs remain unconstrained.
Negative controls must find counterexamples after successful elaboration.
"""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from test_r_front_forward_view import sha

TOP = '''module equivalence(
 input wire [5:0] q,before_levels,after_levels,
 input wire [2:0] count,input wire [35:0] levels,
 input wire debug_before,debug_boundary,input wire [5:0] debug_events,
 input wire consume,output wire valid,equivalent,
 output wire [125:0] actual,expected);
 dorado_baseboard_clock_step dut(q,before_levels,after_levels,count,levels,
  debug_before,debug_boundary,debug_events,consume,
  actual[5:0],actual[47:6],actual[96:48],actual[103:97],actual[110:104],actual[117:111],actual[124:118],actual[125]);
 original_baseboard_clock_step reference(q,before_levels,count,levels,
  debug_before,debug_boundary,debug_events,consume,
  expected[5:0],expected[47:6],expected[96:48],expected[103:97],expected[110:104],expected[117:111],expected[124:118],expected[125]);
 reg bb_cursor;
 integer transitions;
 always @*begin
  bb_cursor=before_levels[4];transitions=0;
  for(integer i=0;i<6;i++)if(i<int'(count))begin
   if(bb_cursor!=levels[6*i+4])transitions=transitions+1;
   bb_cursor=levels[6*i+4];
  end
 end
 assign valid=!consume || (count<=6 && transitions<=1 && after_levels[4]==bb_cursor);
 assign equivalent=!valid || (actual==expected);
endmodule
'''


def run(stage, out, yosys):
    assert not out.exists()
    state = json.loads((stage/'results.json').read_text())
    assert state['status'] == 'PREPARED_SINGLE_BB_TRANSFER_CANDIDATE'
    for name, digest in state['files'].items():
        assert sha(stage/name) == digest
    out.mkdir(parents=True)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING', driver_pid=os.getpid(), scope=__doc__, started=time.time(),
        stage=str(stage), stage_state_sha256=sha(stage/'results.json'), compared_bits=126,
        physical_Q_bits=6, debug_inputs_unconstrained=True, cases=[],
        native_qualified=False, hardware_changed=False, speedup_qualified=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    for variant in ['positive', 'wrong-endpoint', 'multi-BB-without-contract']:
        d = out/variant
        d.mkdir()
        actual = (stage/'dorado_baseboard_clock_step.sv').read_text()
        top = TOP
        if variant == 'wrong-endpoint':
            assert actual.count('!source_before[4] && source_after[4]') == 1
            actual = actual.replace('!source_before[4] && source_after[4]', '!source_before[4] && source_after[5]')
        if variant == 'multi-BB-without-contract':
            top = top.replace('transitions<=1 && ', '')
        (d/'actual.sv').write_text(actual)
        (d/'original.sv').write_text((stage/'original-dorado_baseboard_clock_step.sv').read_text().replace(
            'module dorado_baseboard_clock_step(', 'module original_baseboard_clock_step(', 1))
        (d/'top.sv').write_text(top)
        flag = 'verify' if variant == 'positive' else 'falsify'
        (d/'proof.ys').write_text(f'''read_verilog -sv /work/actual.sv /work/original.sv /work/top.sv
hierarchy -check -top equivalence
proc
flatten
memory_map
opt
check -assert
sat -timeout 120 -prove equivalent 1 -{flag} -show-ports -dump_json /work/counterexample.json
''')
        env = os.environ.copy()
        env['YOWASP_CACHE_DIR'] = str(out.parent/'formal-tool-cache')
        env['YOWASP_MOUNT'] = '/work='+str(d)
        command = [str(yosys), '-Q', '-T', '-s', '/work/proof.ys']
        with (d/'proof.log').open('x') as log:
            proc = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, env=env)
            row = dict(variant=variant, command=command, pid=proc.pid, started=time.time(),
                files={p.name: sha(p) for p in d.iterdir() if p.suffix != '.log'})
            state['cases'].append(row)
            save()
            code = proc.wait()
        log = (d/'proof.log').read_text()
        marker = 'no model found: SUCCESS!' if variant == 'positive' else 'model found: FAIL!'
        passed = code == 0 and marker in log
        row.update(returncode=code, passed=passed, finished=time.time(), log_sha256=sha(d/'proof.log'))
        save()
        print(variant, 'PASS' if passed else 'FAIL', flush=True)
        if not passed:
            state.update(status='TERMINAL_SINGLE_BB_TRANSFER_NOT_PROVEN', finished=time.time())
            save()
            return 1
    state.update(status='PASS_SINGLE_BB_TRANSFER_FULL_CONSUMER_EQUIVALENCE', finished=time.time())
    save()
    print(state['status'], flush=True)
    return 0


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'output', 'yosys']:
        p.add_argument('--'+name, type=Path, required=True)
    a = p.parse_args()
    raise SystemExit(run(a.stage.resolve(), a.output.resolve(), a.yosys.resolve()))
