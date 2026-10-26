#!/usr/bin/env python3
"""Prove the actual native packet cache preserves the phase certificate.

The current CP-prefix/control producer drives the real specialized cache RTL.
Control owners and pin/root history publish under the same guarded edge; their
power-up state is unconstrained. Strengthen induction with source-computed
prefix-payload and canonical-identity invariants. No environment, task, firmware
or input-history assumption is added. Other machine barriers may only withhold
publication. This is not device timing or the complete 60 ns phase schedule.
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
from prove_owned_r_packet import observed
from review_baseboard_clock_consumer import verify
from test_r_front_forward_view import sha

CONTROL = 'src/verilog/rtl/dorado_r_front_control_step.sv'
CALLER = 'src/verilog/rtl/caller.sv'
TOP = '''module native_control_phase_cache_contract(
 input wire clk,reset,request,other_ready,after_root,refresh,
 input wire [400:0] raw_after,
 output wire invariant,fast_path,ready,owner_matches,canonical_identity,
 output wire prefix_identity,current_identity);
 SOURCE_FUNCTIONS
 reg [155:0] owner_q;
 reg [400:0] stored_before;
 reg before_root;
 wire [400:0] before_pins=cp_source_pins(stored_before,cp_bus(stored_before));
 wire [400:0] after_pins=cp_source_pins(raw_after,cp_bus(raw_after));
 wire [155:0] next_q,original_next_q;
 wire [2:0] selector;
 assign next_q={original_next_q[155:122],selector[1],selector[0],
                original_next_q[119],selector[2],original_next_q[117:0]};
 wire [268:0] prefix_live,prefix_saved;
 wire [10:0] prefix_cp,after_cp;
 wire cache_hit,owner_settled;
 wire publish=request && ready && other_ready;
 wire [1415:0] context_state;
 actual_cache cache(clk,reset,publish,owner_q,next_q,before_root,after_root,
  before_pins[397:0],after_pins[397:0],refresh,owner_settled,prefix_live,
  ready,fast_path,cache_hit,prefix_saved,prefix_cp,after_cp,after_root,context_state,owner_matches);
 wire producer_unsettled;
 assign owner_settled=!producer_unsettled;
 dorado_capture_hold_ordered_step producer(owner_q,before_root,after_root,
  before_pins[397:0],after_pins[397:0],original_next_q,,,producer_unsettled,refresh,,,,
  fast_path,prefix_saved,prefix_cp,after_cp,prefix_live,);
 dorado_hold_selector_preview selector_preview(owner_q,before_root,after_root,
  before_pins[397:0],after_pins[397:0],refresh,fast_path,prefix_saved,prefix_cp,after_cp,
  {original_next_q[118],original_next_q[121],original_next_q[120]},selector);
 always @(posedge clk)if(publish)begin
  owner_q<=PUBLISHED_Q;stored_before<=after_pins;before_root<=after_root;
 end
 wire canonical_valid=context_state[1415],cache_valid=context_state[1414];
 wire [155:0] canonical_q=context_state[1413:1258];
 wire [397:0] canonical_pins=context_state[1257:860];
 wire canonical_root=context_state[859],canonical_refresh=context_state[858];
 wire [566:0] key=context_state[857:291];
 wire [397:0] key_before=key[410:13];
 wire [397:0] key_after={key_before[397:293],key[11:1],key_before[281:0]};
 wire [268:0] expected_prefix;
 dorado_control_hold_atomic_step #(.WAVES(5)) prefix_check(key[566:411],key[12],key[12],
  key_before,key_after,expected_prefix[268:113],expected_prefix[112:57],
  expected_prefix[56:1],expected_prefix[0],key[0],);
 wire [268:0] canonical_result,current_result;
 dorado_control_hold_atomic_step #(.WAVES(5)) canonical_check(canonical_q,canonical_root,canonical_root,
  canonical_pins,canonical_pins,canonical_result[268:113],canonical_result[112:57],
  canonical_result[56:1],canonical_result[0],canonical_refresh,);
 dorado_control_hold_atomic_step #(.WAVES(5)) current_check(owner_q,before_root,before_root,
  before_pins[397:0],before_pins[397:0],current_result[268:113],current_result[112:57],
  current_result[56:1],current_result[0],refresh,);
 assign canonical_identity=!canonical_valid || canonical_result=={canonical_q,113'b0};
 assign prefix_identity=!cache_valid ||
  (expected_prefix==prefix_saved && prefix_cp==key[11:1]);
 assign current_identity=!fast_path || current_result=={owner_q,113'b0};
 assign invariant=owner_matches && canonical_identity && prefix_identity && current_identity;
endmodule
'''


def run(stage, out, yosys):
    assert not out.exists()
    manifest = stage / 'r-front-control-native.json'
    state = json.loads(manifest.read_text())
    verify(stage, state['source_hashes'])
    raw = (stage / CONTROL).read_text()
    caller = (stage / CALLER).read_text()
    functions = '\n'.join(re.search(r' function automatic [^\n]*\b' + name + r'\(.*? endfunction', caller, re.S)[0]
                          for name in ['cp_set_decode', 'cp_bus', 'cp_source_pins'])
    assert len(re.findall(r'\bfunction\b', functions)) == 3
    # Bind the actual ordinary native cache, producer and common publication.
    assert 'wire cp_fast' in raw and 'assign cp_fast=entry_fast;' in raw
    assert 'wire first_after=use_pair ? 1\'b0:clock_after;' in raw
    assert 'dorado_packet_prefix_cache entry_cache(host_clk,reset,advance && ready,' in raw
    assert 'if(contb_advance && control_ready)begin' in caller
    assert 'r_front_before<=r_front_live;r_front_clock_before<=CLK_ca_p_;' in caller
    single = module(raw, 'dorado_r_front_parallel_fault_step')
    assert '''wire [155:0] joint_next={joint_next_original[155:122],late_producer_selector[1],late_producer_selector[0],
                         joint_next_original[119],late_producer_selector[2],joint_next_original[117:0]};''' in single
    assert 'assign late_producer_selector=pair_fall ? {joint_q[118],joint_q[121],joint_q[120]}:ordinary_selector;' in single
    cache = observed(module(raw, 'dorado_packet_prefix_cache'), 'actual_cache')
    names = ['dorado_control_hold_atomic_step', 'dorado_control_terminal_body',
             'dorado_control_fall_step', 'dorado_capture_hold_atomic_step',
             'dorado_capture_terminal_body', 'dorado_capture_certified_fall_step',
             'dorado_capture_hold_ordered_step', 'dorado_hold_selector_preview',
             'dorado_control_hold_ordered_step', 'dorado_control_certified_fall_step']
    bank = '\n'.join(module(raw, name) for name in names)
    out.mkdir(parents=True)
    (out / 'source.sv').write_text(bank)
    (out / 'cache.sv').write_text(cache)
    shutil.copy2(Path(__file__), out / Path(__file__).name)
    record = dict(status='RUNNING', started=time.time(), driver_pid=os.getpid(),
                  scope=__doc__, source_manifest_sha256=sha(manifest),
                  source_control_sha256=sha(stage / CONTROL), source_caller_sha256=sha(stage / CALLER),
                  owner_power_up='unconstrained', cache_power_up='actual explicit RTL initializers',
                  protocol_or_input_history_assumptions=0, paired_path_in_scope=False,
                  actual_Hold_independent_LC_projection_included=True,
                  other_barriers='arbitrary withholding of common publication',
                  files={p.name: sha(p) for p in out.iterdir()}, actions=[],
                  native_timing_or60ns_schedule_qualified=False)

    def save():
        (out / 'run-state.json').write_text(json.dumps(record, indent=2) + '\n')

    save()
    for label in ['actual-native-certificate', 'stale-published-ASEL']:
        d = out / label
        d.mkdir()
        (d / 'source.sv').write_text(bank)
        (d / 'cache.sv').write_text(cache)
        qexpr = 'next_q' if label == 'actual-native-certificate' else "next_q ^ (156'b1<<111)"
        (d / 'top.sv').write_text(TOP.replace('SOURCE_FUNCTIONS', functions).replace('PUBLISHED_Q', qexpr))
        positive = label == 'actual-native-certificate'
        strategy = '-seq 2 -tempinduct -maxsteps 4' if positive else '-seq 4'
        (d / 'proof.ys').write_text(f'''read_verilog -sv -defer /work/source.sv /work/cache.sv /work/top.sv
hierarchy -check -top native_control_phase_cache_contract
proc
flatten
opt
check -assert
sat {strategy} -timeout 120 -prove invariant 1 -{'verify' if positive else 'falsify'} -show-ports -dump_json /work/counterexample.json
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
        marker = 'Induction step proven: SUCCESS!' if positive else 'model found: FAIL!'
        row.update(returncode=code, passed=code == 0 and marker in (d / 'proof.log').read_text(),
                   finished=time.time(), log_sha256=sha(d / 'proof.log'))
        save()
        print(label, row['passed'], flush=True)
        if not row['passed']:
            break
    passed = len(record['actions']) == 2 and all(a['passed'] for a in record['actions'])
    record.update(status='PASS_ACTUAL_NATIVE_CONTROL_PHASE_CERTIFICATE_INDUCTION' if passed else
                  'TERMINAL_NATIVE_CONTROL_PHASE_CERTIFICATE_NOT_ESTABLISHED', finished=time.time())
    save()
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'output', 'yosys']:
        p.add_argument('--' + name, type=Path, required=True)
    args = p.parse_args()
    raise SystemExit(run(args.stage.resolve(), args.output.resolve(), args.yosys.resolve()))
