#!/usr/bin/env python3
"""Extend the frozen MIR-mask candidate with original ContA l17 state.

This is a capture-order hypothesis until the complete original graph agrees.
The six new Qs represent the original MC10176 package, not an added pipeline.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_r_front_mir_mask_candidate import candidate as mask_candidate

ROOT=Path(__file__).resolve().parents[1]
PARENT=ROOT/'verilog/rtl/dorado_r_front_commands_step.sv'
PARENT_SHA='e695318064d80a9e308ff5e76e3fd57d86bb55412a448b163da135856883dd0e'


def generate(output):
    assert not output.exists()
    original=PARENT.read_text()
    assert hashlib.sha256(original.encode()).hexdigest()=='3110f49e6dc9e3eef6b3dcdd341d1d1ee05b3f5d14b1a3613663dfd419eee350'
    old,_=mask_candidate(original)
    assert hashlib.sha256(old.encode()).hexdigest()==PARENT_SHA
    raw=old.replace('dorado_r_front_commands_step','dorado_r_front_l17_step').replace('dorado_conta_commands_step','dorado_conta_l17_step')
    raw=raw.replace('[791:0]','[797:0]').replace('[316:0]','[325:0]').replace('[67:0] clock_events','[68:0] clock_events')
    raw=raw.replace('[103:0]','[109:0]').replace('[129:0]','[130:0]')
    a='r[129]=p[297];conta_pins=r;';assert raw.count(a)==1
    raw=raw.replace(a,'r[129]=p[297];r[130]=p[317];conta_pins=r;')
    a='wire [109:0] cq={state_q[791:786],';assert raw.count(a)==1
    raw=raw.replace(a,'wire [109:0] cq={state_q[797:792],state_q[791:786],')
    raw=raw.replace('wire [22:0] ce,cr','wire [23:0] ce,cr')
    a=' assign state_next[31:0]=conta_next[31:0];';assert raw.count(a)==1
    raw=raw.replace(a,' assign state_next[797:792]=conta_next[109:104];\n'+a)
    a=' assign clock_events[67]=ce[22];';assert raw.count(a)==1
    raw=raw.replace(a,' assign clock_events[68]=ce[23];assign repeated_events[68]=cr[23];\n'+a)
    start=raw.index('module dorado_conta_l17_step')
    top=raw[:start];body=raw[start:]
    body=body.replace('[22:0]','[23:0]')
    a='c[22]=normal0;clock_levels=c;';assert body.count(a)==1
    body=body.replace(a,"c[22]=normal0;c[23]=base | q[6] | !q[7] | !q[20];clock_levels=c;")
    # FA=1 uses the same original a13/a19 gates as TaskingOff/On.
    # a16 UseDMD is FC=0/FB=4. a17 B_Dbuf is FC=5/FB=7.
    helper=''' function automatic [1:0] l17_ff(input [109:0] s,input [130:0] p);
  reg ffok_n,fa1_n;reg [2:0] fb,fc;
  begin
   ffok_n=!p[124] | !(s[85]|s[84]|s[88]|s[89]);
   fa1_n=ffok_n | p[123] | !s[78] | s[79];
   fb={s[77],s[76],s[83]};fc={s[82],s[81],s[80]};
   l17_ff={!fa1_n && fb==3'd7 && fc==3'd5,!fa1_n && fb==3'd4 && fc==3'd0};
  end
 endfunction
'''
    a=' reg [109:0] q,n;';assert body.count(a)==1;body=body.replace(a,helper+a)
    a='reg [2:0] old_memory;';assert body.count(a)==1
    body=body.replace(a,'reg [2:0] old_memory;reg [1:0] old_l17_ff;')
    a='old_local=local_decode(state_q,pins_after);';assert body.count(a)==1
    body=body.replace(a,'old_l17_ff=l17_ff(state_q,pins_after);'+a)
    a="   if(capture[22])n[103:98]={2'b00,pins_after[129],old_local[5],!old_local[0],old_memory[0]};";assert body.count(a)==1
    body=body.replace(a,"   if(capture[22])n[103:98]={2'b00,state_q[106],old_local[5],!old_local[0],old_memory[0]};\n   if(capture[23])n[109:104]={1'b0,old_l17_ff[1],state_q[101],state_q[100]&pins_after[130],state_q[100],old_l17_ff[0]};")
    raw=top+body
    output.parent.mkdir(parents=True,exist_ok=True);output.write_text(raw)
    record=dict(parent_candidate_sha256=PARENT_SHA,output_sha256=hashlib.sha256(raw.encode()).hexdigest(),
                physical_bits=798,clock_channels=69,physical_l17_state='state_q[797:792] / ContA[109:104]',
                l17_clock="base | q[6] | !q[7] | !q[20]",capture_hypothesis='Pre-event MIR/d17/l17 data on newly resolved clock edges',
                native_integrated=False,physical_timing_qualified=False,scope=__doc__)
    return record


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--record',type=Path,required=True)
    a=p.parse_args();r=generate(a.output.resolve());assert not a.record.exists();a.record.write_text(json.dumps(r,indent=2)+'\n');print(r['output_sha256'])
