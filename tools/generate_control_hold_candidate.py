#!/usr/bin/env python3
"""Generate a joint ContA/ContB/MemC capture-order hypothesis.

Reuse source-bound qualified functions, closing real clock/field/reset/CP
crossings in a joint event resolver. Preserve pre-event task/l17/MemSH data.
No added architectural state, native integration or timing qualification.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT=Path(__file__).resolve().parents[1]
PARENTS={'verilog/rtl/dorado_r_front_l17_step.sv':'665188a29d07ba22981061233a825f496a479d577f5a6794a6b81cfac8c418d2',
         'verilog/rtl/dorado_contb_hold_step.sv':'bd23230a24789c1544e119aaa828bf8c39d12113c8517c2faeede686fb418c90',
         'verilog/rtl/dorado_contb_mir_step.sv':'a793d9bb15bb12bdab9b0048b94ef69462ac668498b020b890a952ebed102fea'}


def generate(out):
    assert not out.exists();sources={}
    for n,h in PARENTS.items():
        sources[n]=(ROOT/n).read_text();assert hashlib.sha256(sources[n].encode()).hexdigest()==h,n
    ca_source=sources['verilog/rtl/dorado_r_front_l17_step.sv']
    rawpins=re.search(r' function automatic \[130:0\] conta_pins\(.*?endfunction',ca_source,re.S)[0].replace('conta_pins','raw_ca_pins')
    ca_source=ca_source[ca_source.index('module dorado_conta_l17_step'):]
    ca_funcs='\n'.join(re.findall(r' function automatic.*?endfunction',ca_source,re.S))
    cb_source=sources['verilog/rtl/dorado_contb_mir_step.sv']
    cb_funcs='\n'.join(re.findall(r' function automatic.*?endfunction',cb_source,re.S))
    for n in ['stop_mir','levels_at','transfer']:cb_funcs=re.sub(r'\b'+n+r'\b','cb_'+n,cb_funcs)
    mem_funcs='\n'.join(re.findall(r' function automatic.*?endfunction',sources['verilog/rtl/dorado_contb_hold_step.sv'],re.S))
    cb_flags=re.search(r' assign flags=(.*?);',cb_source,re.S)[1].replace('state_next','q').replace('pins_after','p').replace('stop_mir','cb_stop_mir')
    raw='''// Experimental original ContA/ContB/MemC phase and Hold composition.
`default_nettype none
module dorado_control_hold_step #(parameter integer WAVES=24)(
 input wire [155:0] state_q,input wire clock_before,clock_after,
 input wire [397:0] pins_before,pins_after,output reg [155:0] state_next,
 output reg [55:0] clock_events,repeated_events,output reg unsettled,
 output wire [93:0] flags);
'''+rawpins+'\n'+ca_funcs+'\n'+cb_funcs+'\n'+mem_funcs+'\n'+'''
 function automatic [130:0] ca_pins(input [155:0] s,input [397:0] p);
  reg [130:0] a;reg [55:0] b;
  begin
   b=p[381:326];a=raw_ca_pins(p[325:0]);
   a[69]=hold_at(s[155:150]);a[123]=!s[111];a[124]=!s[117];
   a[125]=cb_stop_mir(s[149:110],b);ca_pins=a;
  end
 endfunction
 function automatic [71:0] memory_pins(input [155:0] s,input base,input [397:0] p);
  reg [71:0] b;
  begin
   b=p[397:326];b[0]=base;b[1]=!s[9]|s[11];b[2]=!s[23];b[4]=s[43];
   b[5]=s[104]&p[6];b[6]=s[68];b[56]=!s[15]|s[17];b[57]=!s[8]|s[10];b[58]=!s[22];
   b[10]=p[291];b[11]=p[290];b[12]=p[289];b[13]=p[292];
   b[15]=p[282];b[16]=p[283];b[17]=p[284];memory_pins=b;
  end
 endfunction
 function automatic [55:0] cb_pins(input [155:0] s,input base,input [397:0] p);
  reg [71:0] b;
  begin b=memory_pins(s,base,p);b[3]=hold_at(s[155:150]);cb_pins=b[55:0];end
 endfunction
 function automatic [55:0] joint_clocks(input [155:0] s,input base,input [397:0] p,input switch_view);
  reg [23:0] ac;reg [24:0] actual_ac;
  begin
   ac=clock_levels(s[109:0],base,ca_pins(s,p),switch_view);
   actual_ac={ac[23:20],ac[17:12],ac[19:18],ac[11],ac[10],ac[10],ac[9:0]};
   joint_clocks={memory_clocks(s[155:150],memory_pins(s,base,p)),cb_levels_at(s[149:110],cb_pins(s,base,p)),actual_ac};
  end
 endfunction
 function automatic [25:0] cb_flags_at(input [39:0] q,input [55:0] p);
  cb_flags_at='''+cb_flags+''';
 endfunction
 function automatic [59:0] ca_flags_at(input [155:0] s,input [397:0] p);
  reg [130:0] a;reg [14:0] ready,mask;reg [38:0] f;
  begin
   a=ca_pins(s,p);mask=is_next(s[109:0]);ready=0;
   for(integer i=1;i<16;i++)ready[i-1]=s[48+4*((i-1)/4)+3-((i-1)%4)];
   f={mir_decode(s[97:76],a,s[43]),ready,mask};
   ca_flags_at={s[43],!s[23],s[68],s[68]|s[144],s[104]&p[6],!s[22],!s[15]|s[17],!s[8]|s[10],!s[9]|s[11],local_decode(s[109:0],a),next_raw(s[109:0]),f};
  end
 endfunction
 reg [155:0] q,n;reg [55:0] previous,levels,capture;reg [23:0] ac;
 reg [130:0] ap,old_ap;reg [71:0] mp;reg [55:0] bp;
 reg [7:0] old_local;reg [2:0] old_memory;reg [1:0] old_l17;
 reg mir_switch_view,old_view,done;
 always @*begin
  q=state_q;n=q;mir_switch_view=q[43];previous=joint_clocks(q,clock_before,pins_before,mir_switch_view);
  levels=previous;capture=0;ac=0;ap=0;mp=0;bp=0;clock_events=0;repeated_events=0;done=0;old_view=mir_switch_view;
  old_ap=ca_pins(state_q,pins_after);old_local=local_decode(state_q[109:0],old_ap);
  old_memory=memory_commands(state_q[97:76]);old_l17=l17_ff(state_q[109:0],old_ap);
  for(integer wave=0;wave<WAVES;wave++)begin
   levels=joint_clocks(q,clock_after,pins_after,mir_switch_view);capture=levels&~previous;
   repeated_events|=capture&clock_events;clock_events|=capture;
   ac={capture[24:21],capture[14:13],capture[20:15],capture[12],capture[10],capture[9:0]};
   ap=ca_pins(q,pins_after);mp=memory_pins(q,clock_after,pins_after);bp=cb_pins(q,clock_after,pins_after);
   n[31:0]=controller_transfer(q[31:0],ac[8:0],controls(q[109:0],ap));
   n[47:32]=task_transfer(q[109:0],state_q[109:0],ac[21:0],task_data_pins(state_q[109:0],old_ap));
   n[63:48]=ready_transfer(q[109:0],state_q[109:0],ac[21:0],old_ap);
   n[75:64]=cp_transfer(q[75:64],ac[21:0],ap);
   n[97:76]=mir_transfer(q[97:76],ac[21:0],ap,q[68]|q[144]);
   if(ac[22])n[103:98]={2'b00,state_q[106],old_local[5],!old_local[0],old_memory[0]};
   if(ac[23])n[109:104]={1'b0,old_l17[1],state_q[101],state_q[100]&old_ap[130],state_q[100],old_l17[0]};
   n[149:110]=cb_transfer(q[149:110],capture[49:25],bp);
   // Original MemSH source and memory l02 close on the same distributed edge.
   // The original l02 data must precede Control-A's new clock-to-Q result.
   mp[58]=!state_q[22];n[155:150]=memory_transfer(q[155:150],capture[55:50],mp);
   old_view=mir_switch_view;if(q[69])mir_switch_view=1;
   done=(n==q && levels==previous && old_view==mir_switch_view);previous=levels;q=n;
  end
  state_next=q;unsettled=!done;
 end
 assign flags={{state_next[150],1'b1,{6{hold_at(state_next[155:150])}}},
   cb_flags_at(state_next[149:110],cb_pins(state_next,clock_after,pins_after)),ca_flags_at(state_next,pins_after)};
endmodule
`default_nettype wire
'''
    out.parent.mkdir(parents=True,exist_ok=True);out.write_text(raw)
    return dict(scope=__doc__,output_sha256=hashlib.sha256(raw.encode()).hexdigest(),parents=PARENTS,
                physical_bits=156,clock_channels=56,flag_count=94,native_integrated=False,physical_timing_qualified=False)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True)
    print(generate(p.parse_args().output.resolve())['output_sha256'])
