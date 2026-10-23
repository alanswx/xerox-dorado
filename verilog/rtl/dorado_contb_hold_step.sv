// Experimental original MemC phase/hold + ContB MIR composition.
`default_nettype none
module dorado_contb_hold_step #(parameter integer WAVES=16)(
 input wire [45:0] state_q,input wire [71:0] pins_before,pins_after,
 output wire [45:0] state_next,output wire [30:0] clock_events,repeated_events,
 output wire unsettled,output wire [33:0] flags);
 function automatic hold_at(input [5:0] q);
  hold_at=q[4] | !q[2] | !q[3];
 endfunction
 function automatic [5:0] memory_clocks(input [5:0] q,input [71:0] p);
  reg phase1;
  begin
   phase1=p[0]|p[56]|!q[0];
   memory_clocks={phase1,{3{phase1|p[57]}},p[56],p[0]|p[56]};
  end
 endfunction
 function automatic [5:0] memory_transfer(input [5:0] q,input [5:0] capture,input [71:0] p);
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
 wire [55:0] before_cb={pins_before[55:4],hold_at(state_q[45:40]),pins_before[2:0]};
 wire [55:0] after_cb={pins_after[55:4],hold_at(q),pins_after[2:0]};
 wire cb_unsettled;
 dorado_contb_mir_step cb(state_q[39:0],before_cb,after_cb,state_next[39:0],
  clock_events[24:0],repeated_events[24:0],cb_unsettled,flags[25:0]);
 assign state_next[45:40]=q;assign clock_events[30:25]=events;assign repeated_events[30:25]=repeated;
 assign unsettled=!done|cb_unsettled;
 assign flags[33:26]={q[0],1'b1,{6{hold_at(q)}}};
endmodule
`default_nettype wire
