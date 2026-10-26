#!/usr/bin/env python3
"""Check whether the current control producer establishes its next certificate.

Evaluate the exact CP-prefix/root functions with the actual computed prefix,
then test the independent identity function at the published Q/pins/root. This
tests closure of ALL 156 control owners and all 56 event channels, rather than
assuming the caller's settled flag implies a settled physical context. Inputs
and the optional entry request are arbitrary; entry validity is computed by
the original RTL. No architectural state, clock, or caller is modified.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha

CONTROL = 'src/verilog/rtl/dorado_r_front_control_step.sv'
TOP = '''module control_phase_closure(
 input wire [155:0] q,input wire before_root,after_root,
 input wire [397:0] before_pins,after_pins,input wire refresh,entry,
 output wire equivalent,entry_certificate,producer_unsettled,
 output wire [155:0] next_q,identity_q,
 output wire [55:0] identity_events,identity_repeated,output wire identity_unsettled);
 wire [155:0] before_identity;wire [55:0] before_events,before_repeated;wire before_unsettled;
 dorado_control_hold_atomic_step #(.WAVES(5)) before_identity_function(
  q,before_root,before_root,before_pins,before_pins,
  before_identity,before_events,before_repeated,before_unsettled,refresh,);
 assign entry_certificate=(before_identity==q) && !(|before_events) && !(|before_repeated) && !before_unsettled;
 wire fast_path=entry && entry_certificate;
 wire split_event=(before_root!=after_root) && (before_pins[292:282]!=after_pins[292:282]);
 wire [397:0] prefix_pins=split_event ?
  {before_pins[397:293],after_pins[292:282],before_pins[281:0]} : before_pins;
 wire [268:0] prefix_live,prefix;
 dorado_capture_hold_atomic_step #(.WAVES(5)) prefix_function(
  q,before_root,before_root,before_pins,prefix_pins,
  prefix[268:113],prefix[112:57],prefix[56:1],prefix[0],refresh,,);
 dorado_capture_hold_ordered_step producer(
  q,before_root,after_root,before_pins,after_pins,next_q,,,
  producer_unsettled,refresh,,,,fast_path,prefix,prefix_pins[292:282],after_pins[292:282],prefix_live,);
 dorado_control_hold_atomic_step #(.WAVES(5)) after_identity_function(
  IDENTITY_Q,after_root,after_root,after_pins,after_pins,
  identity_q,identity_events,identity_repeated,identity_unsettled,refresh,);
 assign equivalent=producer_unsettled ||
  {identity_q,identity_events,identity_repeated,identity_unsettled}=={next_q,113'b0};
endmodule
'''


def run(stage, out, yosys):
    assert not out.exists()
    manifest = stage / 'r-front-control-native.json'
    state = json.loads(manifest.read_text())
    verify(stage, state['source_hashes'])
    raw = (stage / CONTROL).read_text()
    names = ['dorado_control_hold_atomic_step', 'dorado_control_terminal_body',
             'dorado_control_fall_step', 'dorado_capture_hold_atomic_step',
             'dorado_capture_terminal_body', 'dorado_capture_certified_fall_step',
             'dorado_capture_hold_ordered_step']
    bank = '\n'.join(module(raw, name) for name in names)
    out.mkdir(parents=True)
    (out / 'source.sv').write_text(bank)
    shutil.copy2(Path(__file__), out / Path(__file__).name)
    record = dict(status='RUNNING', started=time.time(), driver_pid=os.getpid(),
                  scope=__doc__, source_manifest_sha256=sha(manifest),
                  source_control_sha256=sha(stage / CONTROL),
                  arbitrary_binary_input_bits=956, protocol_or_state_assumptions=0,
                  identity_output_bits=269, actual_original_prefix_computed=True,
                  files={p.name: sha(p) for p in out.iterdir()}, actions=[],
                  native_integrated=False, physical_phase_or_speed_qualified=False)

    def save():
        (out / 'run-state.json').write_text(json.dumps(record, indent=2) + '\n')

    save()
    for label, qexpr in [('actual-producer', 'next_q'), ('stale-published-owner', "next_q ^ (156'b1<<111)")]:
        d = out / label
        d.mkdir()
        (d / 'source.sv').write_text(bank)
        (d / 'top.sv').write_text(TOP.replace('IDENTITY_Q', qexpr))
        positive = label == 'actual-producer'
        (d / 'proof.ys').write_text(f'''read_verilog -sv -defer /work/source.sv /work/top.sv
hierarchy -check -top control_phase_closure
proc
flatten
opt
check -assert
sat -timeout 120 -prove equivalent 1 -{'verify' if positive else 'falsify'} -show-ports -dump_json /work/counterexample.json
''')
        env = os.environ.copy()
        env['YOWASP_CACHE_DIR'] = str(out.parent / 'formal-tool-cache')
        env['YOWASP_MOUNT'] = '/work=' + str(d)
        command = [str(yosys), '-Q', '-T', '-s', '/work/proof.ys']
        row = dict(label=label, command=command, started=time.time(),
                   files={p.name: sha(p) for p in d.iterdir()})
        with (d / 'proof.log').open('x') as log:
            child = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, env=env)
            row['pid'] = child.pid
            record['actions'].append(row)
            save()
            code = child.wait()
        marker = 'no model found: SUCCESS!' if positive else 'model found: FAIL!'
        row.update(returncode=code, passed=code == 0 and marker in (d / 'proof.log').read_text(),
                   finished=time.time(), log_sha256=sha(d / 'proof.log'))
        save()
        print(label, row['passed'], flush=True)
        if not row['passed']:
            break
    passed = len(record['actions']) == 2 and all(a['passed'] for a in record['actions'])
    record.update(status='PASS_ORIGINAL_CONTROL_PRODUCER_PHASE_CLOSURE' if passed else
                  'TERMINAL_CONTROL_PRODUCER_CLOSURE_NOT_ESTABLISHED', finished=time.time())
    save()
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'output', 'yosys']:
        p.add_argument('--' + name, type=Path, required=True)
    args = p.parse_args()
    raise SystemExit(run(args.stage.resolve(), args.output.resolve(), args.yosys.resolve()))
