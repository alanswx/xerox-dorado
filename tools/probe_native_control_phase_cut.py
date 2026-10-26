#!/usr/bin/env python3
"""Test the current control capture frontier against changing IM data.

The routed failure reaches a sparse-fall eligibility test through dBSEL. Test
whether the original HIGH identity certificate permits the IM data inputs to
change across that falling edge. Compare ALL 795 control/frame result bits,
including intermediate capture epochs, against the exact staged implementation.
The certificate is computed by the original RTL, never assumed for arbitrary Q.
This is a diagnostic component contract, not native integration or 60 ns proof.
"""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import time

from prove_cp_prefix_cache import module
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha

CONTROL = 'src/verilog/rtl/dorado_r_front_control_step.sv'
CONT_B = 'src/fpga/mister/snapshot_native/ContB_full_snapshot.sv'
DATA_PINS = list(range(246, 264)) + list(range(354, 372))
TOP = '''module control_phase_cut_contract(
 input wire [155:0] q,input wire [397:0] before_pins,after_pins,
 input wire refresh,entry,output wire equivalent,certificate,old_sparse,new_sparse,
 output wire [794:0] expected,actual,output wire exercised);
 wire [155:0] identity_q;wire [55:0] identity_events,identity_repeated;
 wire identity_unsettled;
 dorado_control_hold_atomic_step identity(q,1'b1,1'b1,before_pins,before_pins,
  identity_q,identity_events,identity_repeated,identity_unsettled,refresh,);
 assign certificate=(identity_q==q) && !(|identity_events) && !(|identity_repeated) && !identity_unsettled;
 wire certified_entry=entry && certificate;
 dorado_capture_certified_fall_step original(q,1'b1,1'b0,before_pins,after_pins,
  expected[794:639],expected[638:583],expected[582:527],expected[526],refresh,
  expected[525:432],certified_entry,old_sparse,expected[431:0]);
 candidate_capture_fall changed(q,1'b1,1'b0,before_pins,after_pins,
  actual[794:639],actual[638:583],actual[582:527],actual[526],refresh,
  actual[525:432],CANDIDATE_ENTRY,new_sparse,actual[431:0]);
 assign equivalent=expected==actual;
 assign exercised=certified_entry && new_sparse && !old_sparse &&
  before_pins[359]!=after_pins[359] && actual[591];
endmodule
'''


def candidate(raw, missing_cp=False):
    original = module(raw, 'dorado_capture_certified_fall_step')
    found = re.search(r'(localparam \[397:0\] FALL_STABLE_PIN_SUPPORT=.*?\? 398\x27h)([0-9a-f]+)', original)
    assert found
    old_mask = int(found[2], 16)
    assert all(old_mask >> i & 1 for i in DATA_PINS)
    new_mask = old_mask & ~sum(1 << i for i in DATA_PINS)
    if missing_cp:
        assert new_mask >> 345 & 1
        new_mask &= ~(1 << 345)
    new = original[:found.start(2)] + format(new_mask, 'x') + original[found.end(2):]
    renamed = new.replace('module dorado_capture_certified_fall_step #',
                          'module candidate_capture_fall #', 1)
    assert renamed != new
    return renamed, old_mask, new_mask


def run(stage, out, yosys):
    assert not out.exists()
    manifest = stage / 'r-front-control-native.json'
    state = json.loads(manifest.read_text())
    verify(stage, state['source_hashes'])
    raw = (stage / CONTROL).read_text()
    board = (stage / CONT_B).read_text()
    assert 'assign r_front_pins[359]=dBSEL_2;' in board
    assert 'assign LC_0__drv=r_front_q[806];' in board
    assert 'assign LC_1__drv=r_front_q[809];' in board
    assert 'assign LC_2__drv=r_front_q[808];' in board
    names = ['dorado_control_hold_atomic_step', 'dorado_control_terminal_body',
             'dorado_control_fall_step', 'dorado_capture_hold_atomic_step',
             'dorado_capture_terminal_body', 'dorado_capture_certified_fall_step']
    bank = '\n'.join(module(raw, name) for name in names)
    new, old_mask, new_mask = candidate(raw)
    out.mkdir(parents=True)
    (out / 'source.sv').write_text(bank)
    (out / 'candidate.sv').write_text(new)
    shutil.copy2(Path(__file__), out / Path(__file__).name)
    record = dict(status='RUNNING', started=time.time(), driver_pid=os.getpid(),
                  scope=__doc__, source_stage=str(stage), manifest_sha256=sha(manifest),
                  source_control_sha256=sha(stage / CONTROL),
                  source_contb_sha256=sha(stage / CONT_B),
                  compared_bits=795, compared_frames=432, arbitrary_binary_inputs=954,
                  certificate_computed_from_actual_original_RTL=True,
                  excluded_IM_data_pins=DATA_PINS,
                  excluded_IM_data_producers={str(i): state['live_producers'][str(i)] for i in DATA_PINS},
                  original_mask_hex=format(old_mask, 'x'), candidate_mask_hex=format(new_mask, 'x'),
                  files={p.name: sha(p) for p in out.iterdir()}, actions=[],
                  native_integrated=False, timing_or_speed_or60ns_qualified=False)

    def save():
        (out / 'run-state.json').write_text(json.dumps(record, indent=2) + '\n')

    save()
    for label in ['certified-data-change', 'missing-CP-selector', 'without-certificate', 'cover-dBSEL-change']:
        d = out / label
        d.mkdir()
        (d / 'source.sv').write_text(bank)
        (d / 'candidate.sv').write_text(candidate(raw, label == 'missing-CP-selector')[0])
        (d / 'top.sv').write_text(TOP.replace('CANDIDATE_ENTRY',
                                             'entry' if label == 'without-certificate' else 'certified_entry'))
        positive = label == 'certified-data-change'
        signal = 'exercised' if label == 'cover-dBSEL-change' else 'equivalent'
        value = 0 if label == 'cover-dBSEL-change' else 1
        (d / 'proof.ys').write_text(f'''read_verilog -sv -defer /work/source.sv /work/candidate.sv /work/top.sv
hierarchy -check -top control_phase_cut_contract
proc
flatten
opt
check -assert
sat -timeout 120 -prove {signal} {value} -{'verify' if positive else 'falsify'} -show-ports -dump_json /work/counterexample.json
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
        log = (d / 'proof.log').read_text()
        marker = 'no model found: SUCCESS!' if positive else 'model found: FAIL!'
        row.update(returncode=code, passed=code == 0 and marker in log,
                   finished=time.time(), log_sha256=sha(d / 'proof.log'))
        save()
        print(label, row['passed'], flush=True)
        if not row['passed']:
            break
    passed = len(record['actions']) == 4 and all(a['passed'] for a in record['actions'])
    record.update(status='PASS_CERTIFIED_CONTROL_CAPTURE_WITH_CHANGING_IM_DATA' if passed else
                  'TERMINAL_CONTROL_PHASE_CUT_NOT_ESTABLISHED', finished=time.time())
    save()
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'output', 'yosys']:
        p.add_argument('--' + name, type=Path, required=True)
    args = p.parse_args()
    raise SystemExit(run(args.stage.resolve(), args.output.resolve(), args.yosys.resolve()))
