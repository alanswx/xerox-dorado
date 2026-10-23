#!/usr/bin/env python3
"""Generate a coupled MemC phase/hold and ContB MIR capture hypothesis.

Append six original package Qs, not a pipeline stage. Close CBHold from the
original wired-OR hold register outputs. The upstream CA/MCR/hold-data producers
remain explicit. Validate capture epochs against the entire coupled reference.
"""
import argparse
import hashlib
import json
from pathlib import Path
from generate_contb_mir_candidate import generate as cb_generate


def generate(out,pin_count):
    assert not out.exists();assert pin_count>=64
    cb=out.parent/(out.stem+'-cb.sv');record=cb_generate(cb)
    assert record['output_sha256']=='a793d9bb15bb12bdab9b0048b94ef69462ac668498b020b890a952ebed102fea'
    raw=f'''// Experimental original MemC phase/hold + ContB MIR composition.
`default_nettype none
module dorado_contb_hold_step #(parameter integer WAVES=16)(
 input wire [45:0] state_q,input wire [{pin_count-1}:0] pins_before,pins_after,
 output wire [45:0] state_next,output wire [30:0] clock_events,repeated_events,
 output wire unsettled,output wire [33:0] flags);
 function automatic hold_at(input [5:0] q);
  hold_at=q[4] | !q[2] | !q[3];
 endfunction
 function automatic [5:0] memory_clocks(input [5:0] q,input [{pin_count-1}:0] p);
  reg phase1;
  begin
   phase1=p[0]|p[56]|!q[0];
   memory_clocks={{phase1,{{3{{phase1|p[57]}}}},p[56],p[0]|p[56]}};
  end
 endfunction
 function automatic [5:0] memory_transfer(input [5:0] q,input [5:0] capture,input [{pin_count-1}:0] p);
  reg [5:0] n;
  begin
   n=q;if(capture[0])n[0]=p[58];
   // Original unused l02 B half: Hia=Q1 OR !Q1 also drives its reset.
   n[1]=0;
   if(p[59])begin n[3:2]=2'b11;n[4]=0;n[5]=1;end
   else begin
    if(capture[2])n[2]=p[60];if(capture[3])n[3]=p[61];
    if(capture[4])n[4]=p[62];if(capture[5])n[5]=p[63];
   end
   memory_transfer=n;
  end
 endfunction
 reg [5:0] q,n,previous,levels,capture,events,repeated;reg done;
 always @*begin
  q=state_q[45:40];n=q;previous=memory_clocks(q,pins_before);levels=previous;capture=0;events=0;repeated=0;done=0;
  for(integer wave=0;wave<WAVES;wave++)begin
   levels=memory_clocks(q,pins_after);capture=levels&~previous;
   repeated|=capture&events;events|=capture;n=memory_transfer(q,capture,pins_after);
   done=(n==q && levels==previous);previous=levels;q=n;
  end
 end
 wire [55:0] before_cb={{pins_before[55:4],hold_at(state_q[45:40]),pins_before[2:0]}};
 wire [55:0] after_cb={{pins_after[55:4],hold_at(q),pins_after[2:0]}};
 wire cb_unsettled;
 dorado_contb_mir_step cb(state_q[39:0],before_cb,after_cb,state_next[39:0],
  clock_events[24:0],repeated_events[24:0],cb_unsettled,flags[25:0]);
 assign state_next[45:40]=q;assign clock_events[30:25]=events;assign repeated_events[30:25]=repeated;
 assign unsettled=!done|cb_unsettled;
 assign flags[33:26]={{q[0],1'b1,{{6{{hold_at(q)}}}}}};
endmodule
`default_nettype wire
'''
    out.write_text(raw)
    return dict(scope=__doc__,output_sha256=hashlib.sha256(raw.encode()).hexdigest(),
                dependencies={cb.name:record['output_sha256']},physical_bits=46,clock_channels=31,
                native_integrated=False,physical_timing_qualified=False)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--pin-count',type=int,required=True)
    a=p.parse_args();r=generate(a.output.resolve(),a.pin_count);print(r['output_sha256'])
