#!/usr/bin/env python3
"""Generate an explicit ContB MIR/maintenance clock-and-capture experiment.

40 original physical Qs: 22 MIR package halves plus c04/g03/h03 maintenance
packages. IM data/parity, raw DMD and upstream board inputs remain boundaries.
No original physical state, RAM or capture edge is replaced by initialization.
"""
import argparse
import hashlib
import json
from pathlib import Path

# Candidate equations are independent of the wire-list extractor. Indices are
# the documented test interface: raw instruction fields at pins28..45.
DATA=[None,28,29,28,45,30,32,31,34,33,36,35,38,37,40,39,28,None,44,43,42,41]
# Each physical half's async set is its CP decoder selection/enable input.
SETS=[None,(0,15),(0,16),(0,15),(2,18),(0,17),(1,14),(2,17),(1,16),(1,15),
      (0,14),(1,17),(2,14),(3,17),(2,16),(2,15),(0,15),None,(3,16),(3,15),(3,14),(3,18)]


def generate(out):
    assert not out.exists()
    text='''// Experimental original ContB MIR/maintenance evaluator, not native RTL.
`default_nettype none
module dorado_contb_mir_step #(parameter integer WAVES=16)(
 input wire [39:0] state_q,input wire [55:0] pins_before,pins_after,
 output reg [39:0] state_next,output reg [24:0] clock_events,repeated_events,
 output reg unsettled,output wire [25:0] flags);
 function automatic stop_mir(input [39:0] q,input [55:0] p);
  stop_mir=q[28] & !((p[7] | !q[23]) & (p[8] | !q[22]));
 endfunction
 function automatic [24:0] levels_at(input [39:0] q,input [55:0] p);
  reg normal,repeat_cur,enable;reg [2:0] selector;reg [24:0] v;
  begin
   normal=p[0]|p[1]|p[2]|stop_mir(q,p);repeat_cur=p[3]&p[4];
   v={25{normal|repeat_cur}};v[0]=repeat_cur;v[17]=repeat_cur;
   enable=p[5]&!p[46];selector={p[19],p[20],p[21]};
   v[22]=!enable|(selector!=3'd0);v[23]=!enable|(selector!=3'd7);v[24]=!enable|(selector!=3'd1);
   levels_at=v;
  end
 endfunction
 function automatic [39:0] transfer(input [39:0] q,input [24:0] capture,input [55:0] p);
  reg [39:0] n;reg reset_mir,cp_enable;reg [1:0] selector;
  begin
   n=q;reset_mir=p[6]|q[34];cp_enable=!(p[10]|p[13]);selector={p[11],p[12]};
'''
    for i,(data,setting)in enumerate(zip(DATA,SETS)):
        if data is None:text+=f"   if(capture[{i}])n[{i}]=0;\n"
        else:
            sel,pin=setting
            text+=f"   if(cp_enable && selector==2'd{sel} && p[{pin}])n[{i}]=1;\n   else if(reset_mir)n[{i}]=0;else if(capture[{i}])n[{i}]=p[{data}];\n"
    text+='''   if(capture[22])n[27:22]=p[27:22];
   if(capture[23])n[33:28]=p[27:22];
   if(capture[24])n[39:34]=p[27:22];
   transfer=n;
  end
 endfunction
 reg [39:0] q,n;reg [24:0] previous,levels,capture;reg settled;
 always @* begin
  q=state_q;n=q;previous=levels_at(q,pins_before);levels=previous;capture=0;
  clock_events=0;repeated_events=0;settled=0;
  for(integer wave=0;wave<WAVES;wave++)begin
   levels=levels_at(q,pins_after);capture=levels & ~previous;
   repeated_events|=capture&clock_events;clock_events|=capture;
   n=transfer(q,capture,pins_after);
   settled=(n==q && levels==previous);previous=levels;q=n;
  end
  state_next=q;unsettled=!settled;
 end
 assign flags={state_next[22],state_next[23],state_next[28],pins_after[3]&pins_after[4],
   pins_after[6]|state_next[34],stop_mir(state_next,pins_after),state_next[4],~state_next[16],
   state_next[18],state_next[19],state_next[20],state_next[21],state_next[14],state_next[15],
   state_next[12],state_next[13],state_next[10],state_next[11],state_next[8],
   ~state_next[9],~state_next[6],~state_next[7],~state_next[5],~state_next[2],~state_next[3],~state_next[1]};
endmodule
`default_nettype wire
'''
    out.parent.mkdir(parents=True,exist_ok=True);out.write_text(text)
    return dict(scope=__doc__,output_sha256=hashlib.sha256(text.encode()).hexdigest(),physical_bits=40,clock_channels=25,native_integrated=False)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--record',type=Path,required=True)
    a=p.parse_args();r=generate(a.output.resolve());assert not a.record.exists();a.record.write_text(json.dumps(r,indent=2)+'\n');print(r['output_sha256'])
