#!/usr/bin/env python3
"""Remove a proved unused late Hold386 dependency from three selectors.

Keep the complete original ordered control/state/events/flags/cache functions.
The first swapped-cofactor negative control unexpectedly proved equal: these
three outputs do not depend on after-pins386 for binary inputs. Project them
with that input fixed low, for both owners and producer pins11/10/9. Any
unknown among all1247 function inputs retains the original complete result.
No new storage, architectural cycle, clock or SRAM edge. The earlier full
363-output cofactor fit was rejected; this is a three-bit constant projection.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time

from prove_cp_prefix_cache import module
from test_r_front_forward_view import sha

NAME = 'dorado_hold_selector_preview'
HELPER = '''module dorado_hold_selector_preview #(parameter integer WAVES=5)(
 input wire [155:0] state_q,input wire clock_before,clock_after,
 input wire [397:0] pins_before,pins_after,input wire refresh_before_run_capture,
 input wire fast_path,input wire [268:0] prefix_saved,
 input wire [10:0] prefix_cp_saved,after_cp_saved,
 input wire [2:0] original_selector,output reg [2:0] selected);
 wire [397:0] low_pins=pins_after & ~(398'b1<<386);
 wire [155:0] low_q;
 dorado_control_hold_ordered_step #(.WAVES(WAVES)) low_hold(
  .state_q(state_q),.clock_before(clock_before),.clock_after(clock_after),
  .pins_before(pins_before),.pins_after(low_pins),.state_next(low_q),
  .clock_events(),.repeated_events(),.unsettled(),.refresh_before_run_capture(refresh_before_run_capture),
  .flags(),.cp_prefix_events(),.cp_prefix_repeated(),.fast_path(fast_path),
  .prefix_saved(prefix_saved),.prefix_cp_saved(prefix_cp_saved),.after_cp_saved(after_cp_saved),.prefix_live());
 always @*begin
  // Boolean independence does not imply identical X propagation. Keep the
  // original result whenever ANY ordered-function input is unknown.
  case(^{state_q,clock_before,clock_after,pins_before,pins_after,refresh_before_run_capture,fast_path,prefix_saved,prefix_cp_saved,after_cp_saved})
   1'b0,1'b1:selected={low_q[118],low_q[121],low_q[120]};
   default:selected=original_selector;
  endcase
 end
endmodule
'''
OLD = ' wire [155:0] joint_next;wire [55:0] joint_events,joint_repeated;'
NEW = ''' wire [155:0] joint_next_original;wire [55:0] joint_events,joint_repeated;
 wire [2:0] late_producer_selector;
 wire [155:0] joint_next={joint_next_original[155:122],late_producer_selector[1],late_producer_selector[0],
                         joint_next_original[119],late_producer_selector[2],joint_next_original[117:0]};'''
CONTROL_OLD = '  joint_next,joint_events,joint_repeated,cu,refresh_before_run_capture,control_flags,cp_prefix_events,cp_prefix_repeated,'
CONTROL_NEW = '  joint_next_original,joint_events,joint_repeated,cu,refresh_before_run_capture,control_flags,cp_prefix_events,cp_prefix_repeated,'
ANCHOR = '  cp_fast,cp_prefix_saved,cp_prefix_cp_saved,cp_after_cp_saved,cp_prefix_live);'
INSTANCE = '''
 // The same proved three-bit projection supplies owners and producer pins.
 // All other state/flags and every SRAM capture function stay original.
 dorado_hold_selector_preview selector_preview(
  .state_q(joint_q),.clock_before(clock_before),.clock_after(clock_after),
  .pins_before(pins_before[397:0]),.pins_after(pins_after[397:0]),
  .refresh_before_run_capture(refresh_before_run_capture),.fast_path(cp_fast),
  .prefix_saved(cp_prefix_saved),.prefix_cp_saved(cp_prefix_cp_saved),.after_cp_saved(cp_after_cp_saved),
  .original_selector({joint_next_original[118],joint_next_original[121],joint_next_original[120]}),.selected(late_producer_selector));
'''

TOP = '''module selector_equivalence #(parameter integer WAVES=5)(
 input wire [155:0] state_q,input wire clock_before,clock_after,
 input wire [400:0] pins_before,pins_after,input wire refresh,fast_path,
 input wire [268:0] prefix_saved,input wire [10:0] prefix_cp_saved,after_cp_saved,
 output wire equal,output wire [481:0] different,output wire [325:0] original_producer,actual_producer,
 output wire [2:0] original_selector,selected);
 PRODUCER_FUNCTION
 wire [155:0] original_q;
 dorado_control_hold_ordered_step #(.WAVES(WAVES)) original(
  .state_q(state_q),.clock_before(clock_before),.clock_after(clock_after),
  .pins_before(pins_before[397:0]),.pins_after(pins_after[397:0]),.state_next(original_q),
  .clock_events(),.repeated_events(),.unsettled(),.refresh_before_run_capture(refresh),
  .flags(),.cp_prefix_events(),.cp_prefix_repeated(),.fast_path(fast_path),
  .prefix_saved(prefix_saved),.prefix_cp_saved(prefix_cp_saved),.after_cp_saved(after_cp_saved),.prefix_live());
 assign original_selector={original_q[118],original_q[121],original_q[120]};
 dorado_hold_selector_preview #(.WAVES(WAVES)) actual(
  .state_q(state_q),.clock_before(clock_before),.clock_after(clock_after),
  .pins_before(pins_before[397:0]),.pins_after(pins_after[397:0]),.refresh_before_run_capture(refresh),
  .fast_path(fast_path),.prefix_saved(prefix_saved),.prefix_cp_saved(prefix_cp_saved),.after_cp_saved(after_cp_saved),
  .original_selector(original_selector),.selected(selected));
 wire [155:0] actual_q={original_q[155:122],selected[1],selected[0],original_q[119],selected[2],original_q[117:0]};
 assign original_producer=producer_pins(original_q,pins_after);
 assign actual_producer=producer_pins(actual_q,pins_after);
 assign different={original_q,original_producer}^{actual_q,actual_producer};
 assign equal=~|different;
endmodule
'''


def transform(raw, negative=False, bad_unknown=False):
    assert NAME not in raw and not (negative and bad_unknown)
    for text in [OLD, CONTROL_OLD, ANCHOR]:
        assert raw.count(text) == 1
    helper = HELPER
    if negative:
        a = "1'b0,1'b1:selected={low_q[118],low_q[121],low_q[120]};"
        helper = helper.replace(a, "1'b0,1'b1:selected={low_q[118],1'b0,low_q[120]};", 1)
    elif bad_unknown:
        helper = helper.replace('default:selected=original_selector;', "default:selected=3'b0;", 1)
    added = '\n`default_nettype none\n'+helper+'`default_nettype wire\n'
    changed = raw.replace(OLD, NEW, 1).replace(CONTROL_OLD, CONTROL_NEW, 1).replace(ANCHOR, ANCHOR+INSTANCE, 1)+added
    assert changed[:-len(added)].replace(NEW, OLD, 1).replace(CONTROL_NEW, CONTROL_OLD, 1).replace(ANCHOR+INSTANCE, ANCHOR, 1) == raw
    return changed


def sources(raw, negative=False, bad_unknown=False):
    changed = transform(raw, negative, bad_unknown)
    top_module = module(raw, 'dorado_r_front_control_step')
    function = re.search(r' function automatic \[325:0\] producer_pins\(.*? endfunction', top_module, re.S)[0]
    assert 'r[11]=s[118];r[10]=s[121];r[9]=s[120];' in function
    return changed, dict(atomic=module(raw, 'dorado_control_hold_atomic_step')+module(raw, 'dorado_control_terminal_body'),
        ordered=module(raw, 'dorado_control_hold_ordered_step'), preview=module(changed, NAME),
        reference=TOP.replace('PRODUCER_FUNCTION', function))


def run(source, out, yosys, negative):
    assert not out.exists()
    raw = source.read_text()
    actual, parts = sources(raw, negative)
    out.mkdir(parents=True)
    (out/'original.sv').write_text(raw)
    (out/'candidate.sv').write_text(actual)
    for name, text in parts.items():
        (out/(name+'.sv')).write_text(text)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__,
        source=str(source), source_sha256=sha(source), selector_Q_bits=[118,121,120], producer_pins=[11,10,9],
        late_input=386, compared_producer_output_bits=326, compared_next_state_bits=156,
        compared_output_bits=482, arbitrary_binary_input_bits=1253,
        protocol_or_state_assumptions=False, extra_saved_state_or_guest_cycles=0, negative_control=negative,
        files={p.name: sha(p) for p in out.iterdir() if p.is_file()}, results=[],
        native_timing_qualified=False, firmware_qualified=False, speedup_qualified=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    save()
    env = os.environ.copy()
    env['YOWASP_CACHE_DIR'] = str(out.parent/'formal-tool-cache')
    env['YOWASP_MOUNT'] = '/work='+str(out)
    for waves in [5, 24]:
        flag = 'falsify' if negative else 'verify'
        script = f'''read_verilog -sv /work/atomic.sv /work/ordered.sv /work/preview.sv /work/reference.sv
chparam -set WAVES {waves} selector_equivalence
hierarchy -check -top selector_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -{flag} -show-ports -dump_json /work/counterexample-{waves}.json
'''
        (out/f'proof-{waves}.ys').write_text(script)
        command = [str(yosys), '-Q', '-T', '-s', f'/work/proof-{waves}.ys']
        with (out/f'proof-{waves}.log').open('x') as log:
            proc = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, env=env)
            row = dict(waves=waves, command=command, pid=proc.pid, started=time.time())
            state['results'].append(row)
            save()
            code = proc.wait()
        log = (out/f'proof-{waves}.log').read_text()
        marker = 'model found: FAIL!' if negative else 'no model found: SUCCESS!'
        passed = code == 0 and marker in log
        row.update(returncode=code, passed=passed, finished=time.time(), log_sha256=sha(out/f'proof-{waves}.log'))
        save()
        if not passed:
            state.update(status='TERMINAL_SELECTOR_PREVIEW_NOT_ESTABLISHED', finished=time.time())
            save()
            return 1
    state.update(status='PASS_SELECTOR_PREVIEW_COUNTEREXAMPLES' if negative else 'PASS_SELECTOR_PREVIEW_ALL_PRODUCER_BITS', finished=time.time())
    state['files'] = {p.name: sha(p) for p in out.iterdir() if p.is_file() and p.name != 'run-state.json'}
    save()
    print(state['status'], flush=True)
    return 0


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['source', 'output', 'yosys']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--negative-control', action='store_true')
    a = p.parse_args()
    raise SystemExit(run(a.source.resolve(), a.output.resolve(), a.yosys.resolve(), a.negative_control))
