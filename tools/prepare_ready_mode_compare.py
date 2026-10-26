#!/usr/bin/env python3
"""Expose the original atomic Ready write choice for a parallel comparison.

Keep every original control transfer/output/frame. Track only whether its last
Ready operation retains, clears or loads the frozen-source capture value. The
ordered fast and cached-prefix paths each have such an atomic root transfer.
No clock, storage, readiness barrier or architectural capture changes.
"""
import argparse
import json
from pathlib import Path
import shutil

from prove_cp_prefix_cache import module
from qualify_native_ready_compare import CONTROL, READY, SINGLE
import re
from review_baseboard_clock_consumer import verify
from test_external_r_packet import sha

ROOT=Path(__file__).resolve().parents[1]

HELPER='''// Original Ready outcome is chosen late; compare its three frozen values early.
// No clock/owner/readiness barrier changes. Original comparison is X/Z fallback.
`default_nettype none
module dorado_ready_target_compare(
 input wire [155:0] state_q,input wire clock_before,clock_after,
 input wire [397:0] pins_before,pins_after,input wire refresh,
 input wire fast_path,input wire [268:0] prefix_saved,
 input wire [10:0] prefix_cp_saved,after_cp_saved,
 input wire [15:0] current_ready,original_capture,
 input wire [27:0] child_data,input wire child_event,
 input wire enable,original_mismatch,output reg mismatch);
 wire [155:0] selected_q=fast_path ? state_q:prefix_saved[268:113];
 wire [397:0] before_pins=fast_path ? pins_before:
  {pins_before[397:293],prefix_cp_saved,pins_before[281:0]};
 wire [397:0] after_pins=fast_path ? pins_after:
  {pins_after[397:293],after_cp_saved,pins_after[281:0]};
 wire [1:0] ready_mode;wire [15:0] capture_value;
 localparam [397:0] FALL_STABLE_PIN_SUPPORT=SPARSE_SUPPORT;
 // The existing certified sparse function retains Ready (its only state
 // update is CRam bit30). Preserve that behavior even for arbitrary callers
 // claiming a canonical context. No reachability assumption is introduced.
 wire sparse_hold=fast_path && clock_before && !clock_after &&
  ((pins_before & FALL_STABLE_PIN_SUPPORT)==(pins_after & FALL_STABLE_PIN_SUPPORT));
 dorado_ready_mode_body #(.WAVES(4),.CP_TPC_BLOCK_EARLY(1)) mode(
  .state_q(selected_q),.clock_before(clock_before),.clock_after(clock_after),
  .pins_before(before_pins),.pins_after(after_pins),.refresh_before_run_capture(refresh),
  .ready_mode(ready_mode),.ready_capture_value(capture_value));
 (* keep *) wire [2:0] comparisons;
 dorado_ready_compare_step retained(current_ready,selected_q[63:48],original_capture,
  child_data,child_event,comparisons[0]);
 dorado_ready_compare_step cleared(current_ready,16'b0,original_capture,
  child_data,child_event,comparisons[1]);
 dorado_ready_compare_step loaded(current_ready,capture_value,original_capture,
  child_data,child_event,comparisons[2]);
 always @*begin
  mismatch=original_mismatch;
  case(^{state_q,clock_before,clock_after,pins_before,pins_after,refresh,fast_path,
          prefix_saved,prefix_cp_saved,after_cp_saved,current_ready,original_capture,
          child_data,child_event,enable,original_mismatch,ready_mode,capture_value})
   1'b0,1'b1:if(enable)case(sparse_hold ? 2'd0:ready_mode)
    2'd0:mismatch=comparisons[0];
    2'd1:mismatch=comparisons[1];
    2'd2:mismatch=comparisons[2];
    default:mismatch=original_mismatch;
   endcase
   default:begin end
  endcase
 end
endmodule
`default_nettype wire
'''


def transform(raw):
    before=module(raw,'dorado_capture_terminal_body');after=before;edits=[]
    def change(a,b):
        nonlocal after
        assert after.count(a)==1,a
        after=after.replace(a,b,1);edits.append((a,b))
    change('module dorado_capture_terminal_body #','module dorado_ready_mode_body #')
    change('output reg [108*WAVES-1:0] capture_frames);',
           'output reg [108*WAVES-1:0] capture_frames,\n output reg [1:0] ready_mode,output wire [15:0] ready_capture_value);')
    change('  capture_frames=0;','  capture_frames=0;ready_mode=0;')
    a='   n[63:48]=ready_transfer(q[109:0],state_q[109:0],ac[21:0],old_ap);'
    change(a,a+"\n   if(q[73])ready_mode=2'd1;else if(ac[18])ready_mode=2'd2;")
    extra=''' // Source data is frozen exactly as in ready_transfer. Disable only its
 // output clear branch to expose the LOAD value independently of the selector.
 assign ready_capture_value=ready_transfer(state_q[109:0] & ~(110'b1<<73),
  state_q[109:0],22'b1<<18,ca_pins(state_q,pins_after));
'''
    change('endmodule',extra+'endmodule')
    restored=after
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored==before
    return before,after,edits


def native_transform(raw,ready_raw):
    before=module(raw,SINGLE)
    functions=[]
    for name in ['current_task','ready_pins']:
        found=re.findall(r' function automatic[^\n]*\b'+name+r'\(.*? endfunction',ready_raw,re.S)
        assert len(found)==1,name
        functions.append(found[0])
    mapper='\n'.join(functions)+'\n'
    mapper=re.sub(r'\bcurrent_task\b','comparison_current_task',mapper)
    mapper=re.sub(r'\bready_pins\b','comparison_ready_pins',mapper)
    extra=mapper+''' wire parallel_ready_comparison;
 // Compare frozen Ready outcomes; the original comparator remains X/Z and
 // paired-fall fallback. Every architectural state/capture output stays exact.
 dorado_ready_target_compare ready_target_comparison(
  joint_q,clock_before,clock_after,source_before[397:0],source_after[397:0],
  refresh_before_run_capture,cp_fast,cp_prefix_saved,cp_prefix_cp_saved,cp_after_cp_saved,
  capture_state_q[751:736],child_next[751:736],
  comparison_ready_pins(capture_state_q[87:0],{after_child[245:236],after_child[75:0]}),
  clock_events[20],!pair_fall,child_ready_comparison,parallel_ready_comparison);
'''
    anchor=" assign shared_state_mismatch=|(({child_next[79:64],child_next[31:0]}^conta_next[47:0]) & 48'hffffffff3fff) | child_ready_comparison;"
    replacement=extra+anchor.replace('| child_ready_comparison;','| parallel_ready_comparison;')
    assert before.count(anchor)==1
    after=before.replace(anchor,replacement,1)
    assert after.replace(replacement,anchor,1)==before
    return raw.replace(before,after,1)


def binary_compare(raw):
    """Use the exact original parallel cases inside an already-known context."""
    before=module(raw,'dorado_ready_compare_step');after=before;edits=[]
    def change(a,b):
        nonlocal after
        assert after.count(a)==1,a
        after=after.replace(a,b,1);edits.append((a,b))
    change('module dorado_ready_compare_step(', 'module dorado_ready_compare_binary_step(')
    change('input wire [15:0] state_q,target,original_capture,','input wire [15:0] state_q,target,')
    change('output reg mismatch);','output wire mismatch);')
    start=after.index(' always @*begin');end=after.rindex('endmodule')
    original=after[start:end]
    change(original,''' // The enclosing target comparator handles every X/Z input. This inner
 // result is selected only for a known binary context; no old capture needed.
 assign mismatch=data[15] ? clear_mismatch :
  event_eligible ? capture_mismatch[data[14:13]] : hold_mismatch;
''')
    restored=after
    for a,b in reversed(edits):restored=restored.replace(b,a,1)
    assert restored==before
    return after,edits


def prepare(stage,out,constant_unused_selector=False,binary_inner=False):
    assert not out.exists();out.mkdir(parents=True)
    m=json.loads((stage/'r-front-control-native.json').read_text());verify(stage,m['source_hashes'])
    before,after,edits=transform((stage/CONTROL).read_text())
    raw=(stage/CONTROL).read_text()
    sparse=module(raw,'dorado_capture_certified_fall_step')
    support=re.search(r'FALL_STABLE_PIN_SUPPORT=\(WAVES==5 \|\| WAVES==24\) \? (398\'h[0-9a-f]+)',sparse)[1]
    fall=module(raw,'dorado_control_fall_step')
    assert 'assign state_next={state_q[155:31],next_cram,state_q[29:0]};'in fall
    assert 'using_sparse=entry_canonical && clock_before && !clock_after && ((pins_before & FALL_STABLE_PIN_SUPPORT)==(pins_after & FALL_STABLE_PIN_SUPPORT));'in sparse
    (out/'original-body.sv').write_text(before);(out/'mode-body.sv').write_text(after)
    helper=HELPER.replace('SPARSE_SUPPORT',support)
    if constant_unused_selector:
        # Source proofs establish mode3 impossible. This optional experiment
        # asks whether making its constant explicit changes synthesis; it is
        # not selected merely because its equivalence proof passes.
        a='    default:mismatch=original_mismatch;';assert helper.count(a)==1
        helper=helper.replace(a,"    default:mismatch=1'b0;",1)
    binary_edits=[]
    if binary_inner:
        assert constant_unused_selector
        assert helper.count('dorado_ready_compare_step ')==3
        helper=helper.replace('dorado_ready_compare_step ','dorado_ready_compare_binary_step ')
        a=',original_capture,\n  child_data';assert helper.count(a)==3
        helper=helper.replace(a,',\n  child_data')
        binary,binary_edits=binary_compare((stage/'src/verilog/rtl/dorado_ready_compare_step.sv').read_text())
        (out/'binary-compare.sv').write_text(binary)
    (out/'compare.sv').write_text(helper)
    (out/'original-sparse-reference.txt').write_text(sparse+fall)
    (out/'native-control-after.txt').write_text(native_transform((stage/CONTROL).read_text(),(stage/READY).read_text()))
    shutil.copy2(stage/'src/verilog/rtl/dorado_ready_compare_step.sv',out/'ready-compare.sv')
    shutil.copy2(stage/'src/verilog/rtl/dorado_task_ready_step.sv',out/'task-ready.sv')
    shutil.copy2(Path(__file__),out/Path(__file__).name)
    result=dict(status='PREPARED_SOURCE_BOUND_READY_MODE_COMPARISON',scope=__doc__,stage=str(stage),
                stage_manifest_sha256=sha(stage/'r-front-control-native.json'),source_control_sha256=sha(stage/CONTROL),
                exact_reversible_body_edits=edits,additional_guest_or_host_cycles=0,architectural_outputs_changed=False,
                fall_stable_pin_support=support,original_sparse_Ready_identity_source_verified=True,
                comparison_guard_retained=True,files={p.name:sha(p)for p in out.iterdir()if p.is_file()},
                constant_unused_selector_experiment=constant_unused_selector,
                binary_inner_comparison_experiment=binary_inner,binary_inner_reversible_edits=binary_edits,
                native_equivalence_timing_or_speed_qualified=False)
    (out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    for n in ['stage','output']:p.add_argument('--'+n,type=Path,required=True)
    p.add_argument('--constant-unused-selector',action='store_true')
    p.add_argument('--binary-inner',action='store_true')
    a=p.parse_args();prepare(a.stage.resolve(),a.output.resolve(),a.constant_unused_selector,a.binary_inner)
