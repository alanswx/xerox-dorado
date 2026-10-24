#!/usr/bin/env python3
"""Remove a redundant binary settling-recognition pass from atomic control.

Four passes produce all362 state/event/flag bits of the original24-pass
function for955 arbitrary binary state/input bits. The original settled
output is unconditionally clear at five/24 passes. This replacement must
prove ALL363 outputs at BOTH supported depths before native staging.

The unchanged original body supplies every per-wave diagnostic and the
complete result whenever ANY input contains X/Z, or WAVES is not five/24.
No clock, saved state, SRAM port or guest cycle is added. Formal equivalence
alone does not qualify native firmware, physical timing or machine speed.
"""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from prove_cp_prefix_cache import module
from prove_memory_mir_rtl import TOP
from test_r_front_forward_view import sha

BODY = 'dorado_control_terminal_body'
WRAPPER = '''module dorado_control_hold_atomic_step #(parameter integer WAVES=24)(
 input wire [155:0] state_q,input wire clock_before,clock_after,
 input wire [397:0] pins_before,pins_after,output reg [155:0] state_next,
 output reg [55:0] clock_events,repeated_events,output reg unsettled,
 input wire refresh_before_run_capture,output reg [93:0] flags);
 wire [155:0] short_q,original_q;
 wire [55:0] short_events,original_events,short_repeated,original_repeated;
 wire short_unsettled,original_unsettled;
 wire [93:0] short_flags,original_flags;
 // Original observations remain exact, including unknown inputs and widths.
 wire [WAVES-1:0] mir_mask_diff,mir_mask_high_changed,mir_mask_low_clear;
 dorado_control_terminal_body #(.WAVES(WAVES)) original_transfer(
  state_q,clock_before,clock_after,pins_before,pins_after,
  original_q,original_events,original_repeated,original_unsettled,
  refresh_before_run_capture,original_flags,
  mir_mask_diff,mir_mask_high_changed,mir_mask_low_clear);
 generate if(WAVES==5 || WAVES==24)begin: shortened
  wire [3:0] unused_mask,unused_changed,unused_clear;
  dorado_control_terminal_body #(.WAVES(4)) short_transfer(
   state_q,clock_before,clock_after,pins_before,pins_after,
   short_q,short_events,short_repeated,short_unsettled,
   refresh_before_run_capture,short_flags,unused_mask,unused_changed,unused_clear);
  // Both binary parities select the same complete transfer. Exact case keeps
  // the original four-state behavior for any unknown among ALL955 inputs.
  always @*begin
   case(^{state_q,clock_before,clock_after,pins_before,pins_after,refresh_before_run_capture})
    1'b0,1'b1: {state_next,clock_events,repeated_events,unsettled,flags}=
      SHORT_RESULTS;
    default: {state_next,clock_events,repeated_events,unsettled,flags}=
      {original_q,original_events,original_repeated,original_unsettled,original_flags};
   endcase
  end
 end else begin: unchanged_parameter
  always @*begin
   {state_next,clock_events,repeated_events,unsettled,flags}=
    {original_q,original_events,original_repeated,original_unsettled,original_flags};
  end
 end endgenerate
endmodule
'''


def transform(raw, broken=False, bad_unknown=False):
    assert not (broken and bad_unknown)
    original = module(raw, 'dorado_control_hold_atomic_step')
    assert 'module '+BODY not in raw
    body = original.replace('module dorado_control_hold_atomic_step ', 'module '+BODY+' ', 1)
    old = 'input wire refresh_before_run_capture,output wire [93:0] flags);'
    new = '''input wire refresh_before_run_capture,output wire [93:0] flags,
 output wire [WAVES-1:0] observed_mask,observed_changed,observed_clear);'''
    aliases = ''' assign observed_mask=mir_mask_diff;
 assign observed_changed=mir_mask_high_changed;
 assign observed_clear=mir_mask_low_clear;
'''
    assert body.count(old) == 1
    body = body.replace(old, new, 1).replace('\nendmodule', '\n'+aliases+'endmodule', 1)
    assert body.replace(new, old, 1).replace(aliases, '', 1).replace(
        'module '+BODY+' ', 'module dorado_control_hold_atomic_step ', 1) == original
    # Negative control deliberately drops a state bit, without altering settling.
    result = "{short_q ^ (156'b1 << 17),short_events,short_repeated,1'b0,short_flags}" if broken else \
        "{short_q,short_events,short_repeated,1'b0,short_flags}"
    wrapper = WRAPPER.replace('SHORT_RESULTS', result)
    if bad_unknown:
        old_result = '{original_q,original_events,original_repeated,original_unsettled,original_flags};'
        assert wrapper.count(old_result) == 2
        wrapper = wrapper.replace(old_result, "363'b0;", 1)
    assert raw.count(original) == 1
    return raw.replace(original, wrapper+'\n'+body, 1)


def run(source, out, yosys, broken):
    assert not out.exists()
    raw = source.read_text()
    changed = transform(raw, broken)
    out.mkdir(parents=True)
    (out/'original.sv').write_text(raw)
    (out/'candidate.sv').write_text(changed)
    original = module(raw, 'dorado_control_hold_atomic_step').replace(
        'module dorado_control_hold_atomic_step ', 'module original_atomic ', 1)
    candidate = module(changed, 'dorado_control_hold_atomic_step').replace(
        'module dorado_control_hold_atomic_step ', 'module candidate_atomic ', 1)
    candidate += '\n'+module(changed, BODY)
    (out/'original-atomic.sv').write_text(original)
    (out/'candidate-atomic.sv').write_text(candidate)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state = dict(status='RUNNING', driver_pid=os.getpid(), started=time.time(), scope=__doc__,
        source=str(source), source_sha256=sha(source), negative_control=broken,
        compared_output_bits=363, arbitrary_binary_input_and_state_bits=955,
        assumptions_added=False, extra_state_or_guest_cycles=0, physical_SRAM_changes=False,
        original_diagnostics_preserved=True, unknown_input_fallback='exact original body',
        actions=[], results=[], native_timing_qualified=False, firmware_qualified=False,
        speedup_qualified=False, hardware_changed=False)

    def save():
        (out/'run-state.json').write_text(json.dumps(state, indent=2)+'\n')

    env = os.environ.copy()
    env['YOWASP_MOUNT'] = '/work='+str(out)
    env['YOWASP_CACHE_DIR'] = str(out.parent/'formal-tool-cache')
    save()
    for waves in [5, 24]:
        top = TOP.replace('original_atomic original(', f'original_atomic #(.WAVES({waves})) original(')
        top = top.replace('candidate_atomic candidate(', f'candidate_atomic #(.WAVES({waves})) candidate(')
        (out/f'top-{waves}.sv').write_text(top)
        script = f'''read_verilog -sv /work/original-atomic.sv /work/candidate-atomic.sv /work/top-{waves}.sv
hierarchy -check -top memory_mir_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -{'falsify' if broken else 'verify'} -show-inputs -show-outputs -dump_json /work/counterexample-{waves}.json
'''
        (out/f'proof-{waves}.ys').write_text(script)
        with (out/f'proof-{waves}.log').open('x') as log:
            command = [str(yosys), '-Q', '-T', '-s', f'/work/proof-{waves}.ys']
            process = subprocess.Popen(command, env=env, stdout=log, stderr=subprocess.STDOUT)
            row = dict(waves=waves, pid=process.pid, command=command, started=time.time())
            state['actions'].append(row)
            save()
            row.update(returncode=process.wait(), finished=time.time())
        log = (out/f'proof-{waves}.log').read_text()
        expected = 'SAT proof finished - model found: FAIL!' if broken else 'SAT proof finished - no model found: SUCCESS!'
        passed = row['returncode'] == 0 and expected in log
        state['results'].append(dict(waves=waves, passed=passed, log_sha256=sha(out/f'proof-{waves}.log')))
        save()
        if not passed:
            break
    passed = len(state['results']) == 2 and all(r['passed'] for r in state['results'])
    state.update(status=('PASS_SHORT_CONTROL_COUNTEREXAMPLES' if broken else 'PASS_SHORT_CONTROL_ALL_OUTPUTS')
                 if passed else 'TERMINAL_SHORT_CONTROL_PROOF_NOT_ESTABLISHED', finished=time.time(),
                 files={p.name: sha(p) for p in out.iterdir() if p.is_file() and p.name != 'run-state.json'})
    save()
    print(state['status'], flush=True)
    return 0 if passed else 1


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    for name in ['source', 'output', 'yosys']:
        p.add_argument('--'+name, type=Path, required=True)
    p.add_argument('--negative-control', action='store_true')
    a = p.parse_args()
    raise SystemExit(run(a.source.resolve(), a.output.resolve(), a.yosys.resolve(), a.negative_control))
