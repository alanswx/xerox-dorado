#!/usr/bin/env python3
"""Replace the serial highest-task encoder with a balanced binary selection.

Change only both priority_raw function bodies in an exact frozen control file.
Prove all363 atomic outputs at both actual five-wave and original24-wave
settings, for955 arbitrary binary state/input bits without assumptions.
The original graph, capture schedule and every owner remain unchanged.
This does not establish complete-component simulation or physical timing.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from prove_memory_mir_rtl import atomic, TOP
from test_r_front_forward_view import sha

OLD = ''' function automatic [3:0] priority_raw(input [109:0] source,input [130:0] p);
  reg [14:0] mask;reg [3:0] task_no;
  begin
   mask=is_next(source);task_no=0;
   for(integer i=1;i<16;i=i+1)
    if(p[19+i] || (source[48+4*((i-1)/4)+3-((i-1)%4)] && mask[i-1]))task_no=4'(i);
   priority_raw=reverse_task(task_no);
  end
 endfunction'''
NEW = ''' function automatic [3:0] priority_raw(input [109:0] source,input [130:0] p);
  reg [14:0] mask;reg [15:0] requests;
  reg [7:0] half;reg [3:0] quarter;reg [1:0] pair;
  reg [3:0] task_no;
  begin
   mask=is_next(source);requests=16'b1;
   // Keep procedural IF semantics for unknown original request pins.
   for(integer i=1;i<16;i=i+1)
    if(p[19+i] || (source[48+4*((i-1)/4)+3-((i-1)%4)] && mask[i-1]))requests[i]=1'b1;
   task_no[3]=|requests[15:8];half=task_no[3] ? requests[15:8] : requests[7:0];
   task_no[2]=|half[7:4];quarter=task_no[2] ? half[7:4] : half[3:0];
   task_no[1]=|quarter[3:2];pair=task_no[1] ? quarter[3:2] : quarter[1:0];
   task_no[0]=pair[1];
   priority_raw=reverse_task(task_no);
  end
 endfunction'''


def transform(raw, broken=False):
    assert raw.count(OLD) == 2
    new = NEW.replace('task_no[0]=pair[1];', 'task_no[0]=~pair[1];') if broken else NEW
    changed = raw.replace(OLD, new)
    assert changed.replace(new, OLD) == raw
    return changed


def run(source, out, yosys, broken):
    assert not out.exists()
    original = source.read_text()
    changed = transform(original, broken)
    out.mkdir(parents=True)
    (out / 'original.sv').write_text(original)
    (out / 'candidate.sv').write_text(changed)
    (out / 'original-atomic.sv').write_text(atomic(original, 'original_atomic'))
    (out / 'candidate-atomic.sv').write_text(atomic(changed, 'candidate_atomic'))
    (out / Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    env = os.environ.copy()
    env['YOWASP_CACHE_DIR'] = str(out.parent / 'formal-tool-cache')
    env['YOWASP_MOUNT'] = '/work=' + str(out)
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(),
                 scope=__doc__, source=str(source), original_sha256=sha(source),
                 candidate_sha256=sha(out / 'candidate.sv'), function_body_replacements=2,
                 state_and_input_bits=955, atomic_result_bits=363, assumptions_added=False,
                 broken_low_bit_control=broken, actions=[], results=[],
                 complete_component_simulation_qualified=False, native_qualified=False,
                 timing_qualified=False, speedup_qualified=False, assembly_permitted=False)
    def save():
        (out / 'run-state.json').write_text(json.dumps(state, indent=2) + '\n')
    save()
    for waves in [5, 24]:
        top = TOP.replace('original_atomic original(', f'original_atomic #(.WAVES({waves})) original(')
        top = top.replace('candidate_atomic candidate(', f'candidate_atomic #(.WAVES({waves})) candidate(')
        (out / f'top-{waves}.sv').write_text(top)
        script = f'''read_verilog -sv /work/original-atomic.sv /work/candidate-atomic.sv /work/top-{waves}.sv
hierarchy -check -top memory_mir_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -{'falsify' if broken else 'verify'} -show-inputs -show-outputs -dump_json /work/counterexample-{waves}.json
'''
        (out / f'proof-{waves}.ys').write_text(script)
        with (out / f'proof-{waves}.log').open('x') as log:
            p = subprocess.Popen([str(yosys), '-Q', '-T', '-s', f'/work/proof-{waves}.ys'], env=env,
                                 stdout=log, stderr=subprocess.STDOUT)
            row = dict(waves=waves, pid=p.pid, started=time.time())
            state['actions'].append(row)
            save()
            row.update(returncode=p.wait(), finished=time.time())
        text = (out / f'proof-{waves}.log').read_text()
        expected = 'SAT proof finished - model found: FAIL!' if broken else 'SAT proof finished - no model found: SUCCESS!'
        passed = row['returncode'] == 0 and expected in text
        state['results'].append(dict(waves=waves, passed=passed, log_sha256=sha(out / f'proof-{waves}.log')))
        save()
        if not passed:
            break
    passed = len(state['results']) == 2 and all(r['passed'] for r in state['results'])
    state.update(status=('PASS_BALANCED_PRIORITY_COUNTEREXAMPLES' if broken else 'PASS_BALANCED_PRIORITY_ALL_ATOMIC_OUTPUTS')
                 if passed else 'TERMINAL_PRIORITY_REWRITE_NOT_ESTABLISHED', finished=time.time(),
                 files={p.name:sha(p) for p in out.iterdir() if p.is_file() and p.name != 'run-state.json'})
    save()
    print(state['status'], flush=True)
    return 0 if passed else 1


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['source', 'output', 'yosys']:
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--broken-low-bit', action='store_true')
    args = parser.parse_args()
    raise SystemExit(run(args.source.resolve(), args.output.resolve(), args.yosys.resolve(), args.broken_low_bit))
