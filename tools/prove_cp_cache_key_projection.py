#!/usr/bin/env python3
"""Prove a smaller CP-cache key against the complete uncached ordered transfer.

Use the exact generic staged cache and shorter atomic RTL. Preserve saved
CP inputs and all auxiliary initialization. For binary keys compare only the
proved prefix support; for any X/Z keep the original full comparison. Prove
all475 control/event/flag outputs whenever ready, together with cached-payload
and canonical-context invariants, under arbitrary input/reset histories.
No native source, barrier, clock, RAM edge or FPGA image changes here.
"""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from prove_cp_prefix_cache import module, INVARIANTS
from prove_native_cp_context import RTL, MATCH
from test_r_front_forward_view import sha

OLD = ' assign cache_hit=cache_valid && (cache_key==current_key) && (after_cp_saved==after_pins[292:282]);'
NEW = ''' localparam [566:0] PREFIX_KEY_SUPPORT=567'hMASK;
 reg prefix_key_match;
 // Keep the complete original comparison for any unknown key input/state.
 always @*begin
  case(^{cache_key,current_key})
   1'b0,1'b1: prefix_key_match=((cache_key & PREFIX_KEY_SUPPORT)==(current_key & PREFIX_KEY_SUPPORT));
   default: prefix_key_match=(cache_key==current_key);
  endcase
 end
 assign cache_hit=cache_valid && prefix_key_match && (after_cp_saved==after_pins[292:282]);'''

TOP = '''module cache_projection_safety(
 input wire clk,reset,request,
 input wire [155:0] q,input wire before_root,after_root,
 input wire [397:0] before_pins,after_pins,input wire refresh,
 output wire equivalent,ready,fast_path,cache_hit,invariant,
 output wire [474:0] different,output wire ram_permission);
 wire [155:0] old_q,new_q;
 wire [55:0] old_events,new_events,old_repeated,new_repeated;
 wire [55:0] old_cp_events,new_cp_events,old_cp_repeated,new_cp_repeated;
 wire old_unsettled,new_unsettled;
 wire [93:0] old_flags,new_flags;
 wire [268:0] original_prefix,prefix_live,prefix_saved;
 wire [10:0] prefix_cp_saved,after_cp_saved;
 wire split_event=(before_root!=after_root)&&(before_pins[292:282]!=after_pins[292:282]);
 wire [10:0] original_prefix_cp=split_event ? after_pins[292:282] : before_pins[292:282];
 // Combinational original-prefix output feeds its independent root stage.
 // The prefix output itself has no dependency on these feedback inputs.
 dorado_control_hold_ordered_step original(q,before_root,after_root,before_pins,after_pins,
  old_q,old_events,old_repeated,old_unsettled,refresh,old_flags,old_cp_events,old_cp_repeated,
  1'b0,original_prefix,original_prefix_cp,after_pins[292:282],original_prefix);
 projected_cache guard(clk,reset,request,q,new_q,before_root,after_root,before_pins,after_pins,
  refresh,!new_unsettled,prefix_live,ready,fast_path,cache_hit,
  prefix_saved,prefix_cp_saved,after_cp_saved,invariant);
 dorado_control_hold_ordered_step actual(q,before_root,after_root,before_pins,after_pins,
  new_q,new_events,new_repeated,new_unsettled,refresh,new_flags,new_cp_events,new_cp_repeated,
  fast_path,prefix_saved,prefix_cp_saved,after_cp_saved,prefix_live);
 assign different={old_q,old_events,old_repeated,old_unsettled,old_flags,old_cp_events,old_cp_repeated}^
                  {new_q,new_events,new_repeated,new_unsettled,new_flags,new_cp_events,new_cp_repeated};
 assign ram_permission=request && ready;
 assign equivalent=invariant && (!ready || ~|different) && (ready || !ram_permission);
endmodule
'''


def projected(cache, care):
    assert cache.count(OLD) == 1
    replacement = NEW.replace('MASK', format(sum(1 << i for i in care), 'x'))
    result = cache.replace(OLD, replacement, 1)
    assert result.replace(replacement, OLD, 1) == cache
    return result


def run(stage, support, out, yosys, negative):
    assert not out.exists()
    audit = json.loads((support/'run-state.json').read_text())
    assert audit['status'] == 'PASS_BINARY_CP_PREFIX_SUPPORT_AUDIT'
    assert audit['whole_prefix_projection_proven'] and audit['missing_owner_bit_counterexample']
    assert all(a['returncode'] == 0 for a in audit['actions'])
    original = (support/'actual-source.sv').read_text()
    assert sha(support/'actual-source.sv') == audit['actual_RTL_sha256']
    m = json.loads((stage/'r-front-control-native.json').read_text())
    for name, digest in m['source_hashes'].items():
        assert sha(stage/name) == digest
    raw = (stage/RTL).read_text()
    assert ',host_clk,cp_commit,state_q[751:0],clock_before,clock_after,before_child,after_child,' in raw
    assert 'wire cp_commit=advance && cp_ready;' in raw
    assert '  !cu,cp_prefix_live,cp_ready,' in raw
    cache = module(raw, 'dorado_cp_prefix_cache')
    assert MATCH in cache
    care = set(audit['cared_key_bits'])
    assert all(i in care for i in range(1, 12)), 'All saved CP bits remain compared'
    assert len(care) == 269 and 138 not in care
    omitted = max(care) if negative else None
    if negative:
        care.remove(omitted)
    for name in ['dorado_control_hold_atomic_step', 'dorado_control_terminal_body', 'dorado_control_hold_ordered_step']:
        assert module(raw, name) == module(original, name), name
    candidate = projected(cache, care)
    suffix = ' output reg [10:0] prefix_cp_saved,after_cp_saved);'
    observed_suffix = ' output reg [10:0] prefix_cp_saved,after_cp_saved,output wire invariant);'
    assert candidate.count(suffix) == 1
    observed = candidate.replace(suffix, observed_suffix, 1).replace('\nendmodule', INVARIANTS+'\nendmodule', 1)
    assert observed.replace(INVARIANTS, '', 1).replace(observed_suffix, suffix, 1) == candidate
    observed = observed.replace('module dorado_cp_prefix_cache(', 'module projected_cache(', 1)
    out.mkdir(parents=True)
    sources = dict(atomic=module(raw, 'dorado_control_hold_atomic_step')+module(raw, 'dorado_control_terminal_body'),
        ordered=module(raw, 'dorado_control_hold_ordered_step'), cache=observed, top=TOP)
    for name, value in sources.items():
        (out/(name+'.sv')).write_text(value)
    (out/'actual-source.sv').write_text(raw)
    (out/'candidate-cache.sv').write_text(candidate)
    (out/'support-state.json').write_bytes((support/'run-state.json').read_bytes())
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    strategy = '-seq 4 -set-at 1 reset 1 -falsify' if negative else '-seq 2 -tempinduct -maxsteps 8 -verify'
    script = f'''read_verilog -sv /work/atomic.sv /work/ordered.sv /work/cache.sv /work/top.sv
hierarchy -check -top cache_projection_safety
proc
flatten
opt
check -assert
sat {strategy} -set-init-zero -timeout 120 -prove equivalent 1 -show-ports -dump_json /work/counterexample.json
'''
    (out/'proof.ys').write_text(script)
    env = os.environ.copy()
    env['YOWASP_CACHE_DIR'] = str(out.parent/'formal-tool-cache')
    env['YOWASP_MOUNT'] = '/work='+str(out)
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__,
        stage=str(stage), stage_manifest_sha256=sha(stage/'r-front-control-native.json'),
        support=str(support), support_state_sha256=sha(support/'run-state.json'),
        compared_control_output_bits=475, compared_prefix_bits=269, cared_key_bits=sorted(care),
        negative_control=negative, omitted_required_key_bit=omitted,
        input_constraints='reset first step only' if negative else 'none; arbitrary inputs and reset histories',
        auxiliary_initialization='exact source constant-zero registers only; q/pins are unconstrained inputs',
        source_hashes={p.name: sha(p) for p in out.iterdir() if p.is_file()},
        unbounded_safety_proven=False, four_state_behavior_qualified=False,
        native_commit_interface_integrated=False, native_timing_qualified=False,
        firmware_qualified=False, speedup_qualified=False, assembly_permitted=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    command = [str(yosys), '-Q', '-T', '-s', '/work/proof.ys']
    with (out/'proof.log').open('x') as log:
        process = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, env=env)
        state.update(pid=process.pid, command=command)
        save()
        code = process.wait()
    log = (out/'proof.log').read_text()
    marker = 'model found: FAIL!' if negative else 'Induction step proven: SUCCESS!'
    passed = code == 0 and marker in log
    state.update(status=('PASS_CP_CACHE_KEY_NEGATIVE_CONTROL' if negative else 'PASS_CP_CACHE_KEY_INDUCTIVE_SAFETY')
        if passed else 'TERMINAL_CP_CACHE_KEY_SAFETY_NOT_ESTABLISHED', returncode=code,
        finished=time.time(), log_sha256=sha(out/'proof.log'), unbounded_safety_proven=passed and not negative)
    save()
    print(state['status'], flush=True)
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['stage', 'support', 'output', 'yosys']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--negative-control', action='store_true')
    a = p.parse_args()
    raise SystemExit(run(a.stage.resolve(), a.support.resolve(), a.output.resolve(), a.yosys.resolve(), a.negative_control))
