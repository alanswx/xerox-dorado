#!/usr/bin/env python3
"""Factor the late MemC conditional-Hold pin in the current atomic control step.

The prior whole-component forecast experiment preceded the guarded CP cache.
Here only the156-Q/363-output stateless atomic control function is duplicated;
the prefix cache, owner projections, processor, capture epochs and four SRAMs
are untouched. Both binary input values are computed before final selection.
An exact-case fallback retains the original behavior for an X/Z selector in
four-state simulation. No native timing, clock or firmware claim follows.
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

BODY='dorado_control_hold_cofactor_body'
WRAPPER='''module dorado_control_hold_atomic_step #(parameter integer WAVES=24)(
 input wire [155:0] state_q,input wire clock_before,clock_after,
 input wire [397:0] pins_before,pins_after,output reg [155:0] state_next,
 output reg [55:0] clock_events,repeated_events,output reg unsettled,
 input wire refresh_before_run_capture,output reg [93:0] flags);
 wire [397:0] after_low={pins_after[397:387],1'b0,pins_after[385:0]};
 wire [397:0] after_high={pins_after[397:387],1'b1,pins_after[385:0]};
 wire [155:0] low_q,high_q,unknown_q;
 wire [55:0] low_events,high_events,unknown_events,low_repeated,high_repeated,unknown_repeated;
 wire low_unsettled,high_unsettled,unknown_unsettled;
 wire [93:0] low_flags,high_flags,unknown_flags;
 wire [WAVES-1:0] low_mask,high_mask,unknown_mask;
 wire [WAVES-1:0] low_changed,high_changed,unknown_changed,low_clear,high_clear,unknown_clear;
 // Preserve every original per-wave diagnostic at the same public hierarchy.
 reg [WAVES-1:0] mir_mask_diff,mir_mask_high_changed,mir_mask_low_clear;
 dorado_control_hold_cofactor_body #(.WAVES(WAVES)) low(
  state_q,clock_before,clock_after,pins_before,after_low,
  low_q,low_events,low_repeated,low_unsettled,refresh_before_run_capture,low_flags,
  low_mask,low_changed,low_clear);
 dorado_control_hold_cofactor_body #(.WAVES(WAVES)) high(
  state_q,clock_before,clock_after,pins_before,after_high,
  high_q,high_events,high_repeated,high_unsettled,refresh_before_run_capture,high_flags,
  high_mask,high_changed,high_clear);
 // Exact original transfer for unknown selector values. Binary hardware
 // has only the two complete cases below; no X masking or guessed default.
 dorado_control_hold_cofactor_body #(.WAVES(WAVES)) unknown_selector(
  state_q,clock_before,clock_after,pins_before,pins_after,
  unknown_q,unknown_events,unknown_repeated,unknown_unsettled,refresh_before_run_capture,unknown_flags,
  unknown_mask,unknown_changed,unknown_clear);
 always @*begin
  case(pins_after[386])
   1'b0: {state_next,clock_events,repeated_events,unsettled,flags}=
     LOW_RESULTS;
   1'b1: {state_next,clock_events,repeated_events,unsettled,flags}=
     HIGH_RESULTS;
   default: {state_next,clock_events,repeated_events,unsettled,flags}=
     {unknown_q,unknown_events,unknown_repeated,unknown_unsettled,unknown_flags};
  endcase
  case(pins_after[386])
   1'b0: {mir_mask_diff,mir_mask_high_changed,mir_mask_low_clear}={low_mask,low_changed,low_clear};
   1'b1: {mir_mask_diff,mir_mask_high_changed,mir_mask_low_clear}={high_mask,high_changed,high_clear};
   default: {mir_mask_diff,mir_mask_high_changed,mir_mask_low_clear}={unknown_mask,unknown_changed,unknown_clear};
  endcase
 end
endmodule
'''
LOW='{low_q,low_events,low_repeated,low_unsettled,low_flags}'
HIGH='{high_q,high_events,high_repeated,high_unsettled,high_flags}'


def transform(raw,broken=False):
    original=module(raw,'dorado_control_hold_atomic_step')
    assert 'module '+BODY not in raw
    body=original.replace('module dorado_control_hold_atomic_step ', 'module '+BODY+' ',1)
    old='input wire refresh_before_run_capture,output wire [93:0] flags);'
    new='''input wire refresh_before_run_capture,output wire [93:0] flags,
 output wire [WAVES-1:0] observed_mask,observed_changed,observed_clear);'''
    added=''' assign observed_mask=mir_mask_diff;
 assign observed_changed=mir_mask_high_changed;
 assign observed_clear=mir_mask_low_clear;
'''
    assert body.count(old)==1
    body=body.replace(old,new,1).replace('\nendmodule','\n'+added+'endmodule',1)
    assert body.replace(new,old,1).replace(added,'',1).replace('module '+BODY+' ',
        'module dorado_control_hold_atomic_step ',1)==original
    wrapper=WRAPPER.replace('LOW_RESULTS',HIGH if broken else LOW).replace('HIGH_RESULTS',LOW if broken else HIGH)
    assert raw.count(original)==1
    return raw.replace(original,wrapper+'\n'+body,1)


def run(source,out,yosys,broken):
    assert not out.exists()
    raw=source.read_text();changed=transform(raw,broken);out.mkdir(parents=True)
    (out/'original.sv').write_text(raw);(out/'candidate.sv').write_text(changed)
    original=module(raw,'dorado_control_hold_atomic_step').replace('module dorado_control_hold_atomic_step ', 'module original_atomic ',1)
    candidate=module(changed,'dorado_control_hold_atomic_step').replace('module dorado_control_hold_atomic_step ', 'module candidate_atomic ',1)
    candidate+='\n'+module(changed,BODY)
    (out/'original-atomic.sv').write_text(original);(out/'candidate-atomic.sv').write_text(candidate)
    (out/Path(__file__).name).write_bytes(Path(__file__).read_bytes())
    state=dict(status='RUNNING',driver_pid=os.getpid(),started=time.time(),scope=__doc__,
        source=str(source),source_sha256=sha(source),broken_selection=broken,
        compared_output_bits=363,arbitrary_binary_input_and_state_bits=955,
        assumptions_added=False,extra_state_or_guest_cycles=0,physical_SRAM_changes=False,
        actions=[],results=[],native_timing_qualified=False,firmware_qualified=False,
        speedup_qualified=False,hardware_changed=False)
    def save():(out/'run-state.json').write_text(json.dumps(state,indent=2)+'\n')
    env=os.environ.copy();env['YOWASP_MOUNT']='/work='+str(out)
    env['YOWASP_CACHE_DIR']=str(out.parent/'formal-tool-cache');save()
    for waves in [5,24]:
        top=TOP.replace('original_atomic original(',f'original_atomic #(.WAVES({waves})) original(')
        top=top.replace('candidate_atomic candidate(',f'candidate_atomic #(.WAVES({waves})) candidate(')
        (out/f'top-{waves}.sv').write_text(top)
        script=f'''read_verilog -sv /work/original-atomic.sv /work/candidate-atomic.sv /work/top-{waves}.sv
hierarchy -check -top memory_mir_equivalence
proc
flatten
opt
check -assert
sat -timeout 120 -prove equal 1 -{'falsify'if broken else'verify'} -show-inputs -show-outputs -dump_json /work/counterexample-{waves}.json
'''
        (out/f'proof-{waves}.ys').write_text(script)
        with(out/f'proof-{waves}.log').open('x')as log:
            process=subprocess.Popen([str(yosys),'-Q','-T','-s',f'/work/proof-{waves}.ys'],env=env,stdout=log,stderr=subprocess.STDOUT)
            row=dict(waves=waves,pid=process.pid,started=time.time());state['actions'].append(row);save()
            row.update(returncode=process.wait(),finished=time.time())
        text=(out/f'proof-{waves}.log').read_text()
        expected='SAT proof finished - model found: FAIL!'if broken else'SAT proof finished - no model found: SUCCESS!'
        passed=row['returncode']==0 and expected in text
        state['results'].append(dict(waves=waves,passed=passed,log_sha256=sha(out/f'proof-{waves}.log')));save()
        if not passed:break
    passed=len(state['results'])==2 and all(r['passed']for r in state['results'])
    state.update(status=('PASS_LATE_MEMORY_HOLD_COUNTEREXAMPLES'if broken else'PASS_LATE_MEMORY_HOLD_ATOMIC_EQUIVALENCE')
                 if passed else'TERMINAL_LATE_MEMORY_HOLD_PROOF_NOT_ESTABLISHED',finished=time.time(),
                 files={p.name:sha(p)for p in out.iterdir()if p.is_file()and p.name!='run-state.json'})
    save();print(state['status'],flush=True);return 0 if passed else 1


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for name in ['source','output','yosys']:p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--broken-selection',action='store_true')
    a=p.parse_args();raise SystemExit(run(a.source.resolve(),a.output.resolve(),a.yosys.resolve(),a.broken_selection))
