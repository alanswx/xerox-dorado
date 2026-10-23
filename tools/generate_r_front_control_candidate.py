#!/usr/bin/env python3
"""Compose processor storage with the joint original control/Hold evaluator.

Preserve the 798-Q map and append ContB/MC's46 original Qs. New MIR fields
are visible after their clock-to-Q; closing processor captures use old fields.
The explicit RunRefresh arrival input still requires a native phase producer.
This prototype is not native integration or FPGA timing qualification.
"""
import argparse
import hashlib
from pathlib import Path
import re
from generate_control_hold_ordered import generate as control_generate

ROOT=Path(__file__).resolve().parents[1]
PARENT='665188a29d07ba22981061233a825f496a479d577f5a6794a6b81cfac8c418d2'


def generate(out,mutation=None):
    assert not out.exists()
    original=(ROOT/'verilog/rtl/dorado_r_front_l17_step.sv').read_text()
    assert hashlib.sha256(original.encode()).hexdigest()==PARENT
    split=original.index('\nendmodule')+len('\nendmodule');raw=original[:split];unused_ca=original[split:]
    raw=raw.replace('dorado_r_front_l17_step','dorado_r_front_control_step',1)
    raw=raw.replace('input wire [797:0] state_q','input wire [843:0] state_q')
    raw=raw.replace('input wire [325:0] pins_before,pins_after,',
                    'input wire [400:0] pins_before,pins_after,input wire refresh_before_run_capture,')
    raw=raw.replace('output wire [797:0] state_next','output wire [843:0] state_next')
    raw=raw.replace('output wire [68:0] clock_events,repeated_events','output wire [99:0] clock_events,repeated_events')
    raw=raw.replace('output wire shared_state_mismatch','output wire shared_state_mismatch,output wire [93:0] control_flags')
    # Replace boundary pin views only inside the old top body. The new joint
    # controller below consumes the true raw inputs, not these sink views.
    start=raw.index(' function automatic [2:0] memory_commands')
    header,body=raw[:start],raw[start:]
    body=re.sub(r'\bpins_before\b','effective_before',body)
    body=re.sub(r'\bpins_after\b','capture_after',body)
    settled=body.index(' // Settled observations')
    body=body[:settled]+body[settled:].replace('capture_after','effective_after')
    old=''' dorado_conta_l17_step conta(cq,clock_before,clock_after,conta_pins(effective_before),conta_pins(capture_after),
  conta_next,ce,cr,cu,cf,cn,local_flags);'''
    assert body.count(old)==1
    joint=''' wire [155:0] joint_q={state_q[843:798],cq};
 wire [155:0] joint_next;wire [55:0] joint_events,joint_repeated;
 function automatic [325:0] producer_pins(input [155:0] s,input [400:0] p);
  reg [325:0] r;
  begin
   r=p[325:0];r[68]=s[154]|!s[152]|!s[153];r[69]=r[68];
   r[294]=!s[111];r[202]=!s[113];r[203]=!s[112];r[204]=!s[115];
   r[205]=!s[117];r[206]=!s[116];r[207]=!s[119];
   r[11]=s[118];r[10]=s[121];r[9]=s[120];
   r[293]=s[138]&!((p[333]|!s[133])&(p[334]|!s[132]));
   producer_pins=r;
  end
 endfunction
 wire [325:0] effective_before=producer_pins(joint_q,pins_before);
 wire [325:0] effective_after=producer_pins(joint_next,pins_after);
 wire [325:0] capture_after=clock_after ? producer_pins(joint_q,pins_after) : effective_after;
 dorado_control_hold_ordered_step control(joint_q,clock_before,clock_after,pins_before[397:0],pins_after[397:0],
  joint_next,joint_events,joint_repeated,cu,refresh_before_run_capture,control_flags);
 assign conta_next=joint_next[109:0];assign state_next[843:798]=joint_next[155:110];
 assign ce={joint_events[24:21],joint_events[14:13],joint_events[20:15],joint_events[12],joint_events[10],joint_events[9:0]};
 assign cr={joint_repeated[24:21],joint_repeated[14:13],joint_repeated[20:15],joint_repeated[12],joint_repeated[10],joint_repeated[9:0]};
 assign cf=control_flags[38:0];assign cn=control_flags[42:39];assign local_flags=control_flags[50:43];
 assign clock_events[99:69]=joint_events[55:25];assign repeated_events[99:69]=joint_repeated[55:25];
'''
    body=body.replace(old,joint)
    old='assign shared_state_mismatch={child_next[751:736],child_next[79:64],child_next[31:0]}!=conta_next[63:0];'
    assert body.count(old)==1
    body=body.replace(old,'''// The child's shadow memory-run Q14/15 do not drive any processor
 // sink. Architectural memory state comes from the joint evaluator and is
 // compared in the full844-Q reference, rather than this redundant shadow.
 assign shared_state_mismatch=|(({child_next[751:736],child_next[79:64],child_next[31:0]}^conta_next[63:0]) & 64'hffffffffffff3fff);''')
    old='after_child[65]=STOP_CLOCK_NEW ? after_raw[65] : capture_local[0];';assert body.count(old)==1
    body=body.replace(old,'after_child[65]=STOP_CLOCK_NEW ? local_flags[0] : capture_local[0];')
    control=out.with_name(out.stem+'-control.sv');metadata=control_generate(control)
    raw=header+body+unused_ca+'\n'+control.read_text()
    if mutation:
        assert mutation=='new_fields'
        old='wire [325:0] capture_after=clock_after ? producer_pins(joint_q,pins_after) : effective_after;'
        assert raw.count(old)==1
        raw=raw.replace(old,'wire [325:0] capture_after=effective_after;')
    out.write_text(raw)
    return dict(scope=__doc__,output_sha256=hashlib.sha256(raw.encode()).hexdigest(),processor_parent_sha256=PARENT,
                control=metadata,mutation=mutation,physical_bits=844,ram_bits=9216,clock_channels=100,native_integrated=False,physical_timing_qualified=False)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    print(generate(p.parse_args().output.resolve())['output_sha256'])
