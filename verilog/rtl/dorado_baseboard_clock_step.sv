// Original BaseBoard clock-cone consumer; not yet connected to native boards.
// e07 LS163 loads1011 via c08's inverted Max, otherwise counts crystal rises.
// Max's rising edge clocks g08a, whose Q' is MCPreClk and whose D is that Q'.
// MCPreClk's rising edge clocks g08b with its own Q' fed back to D.
// Physical state: [3:0]=e07 count,[4]=g08a Q,[5]=g08b Q. No added owner state.
// j02 recomputes SCP'|ECP|dStartClockPulse; source lane2 is diagnostic only.
// consume is the transport commit, NOT architectural processor Hold.
// Six timestamped records plus a boundary-input evaluation share one interval.
// Every simultaneous source group uses ONE pre-event state. The counter/FF
// cascade within that group retains the original derived-clock ordering.
`default_nettype none
module dorado_baseboard_clock_step(
 input wire [5:0] state_q,source_before,
 input wire [2:0] event_count,input wire [35:0] source_after_events,
 input wire debug_before,debug_at_boundary,input wire [5:0] debug_at_events,
 input wire consume,
 output reg [5:0] state_next,
 output reg [41:0] states_after_steps,
 // Per-step bits: e07 capture,g08a capture,g08b capture,root rise,root fall,
 // MCPreClk fall,MCPreClk rise. Step0 evaluates boundary inputs, then records.
 output reg [48:0] events_at_steps,
 output reg [6:0] roots_after_steps,max_after_steps,mcpre_after_steps,aux_after_steps,
 output wire invalid_count
);
 assign invalid_count=event_count>3'd6;
 function automatic [12:0] step(input [5:0] q,before_levels,after_levels,
                              input before_debug,after_debug);
  reg [5:0] n;
  reg [6:0] events;
  reg old_max,new_max,old_pre,new_pre,old_root,new_root;
  begin
   n=q;events=0;old_max=(q[3:0]==4'hf);old_pre=!q[4];
   old_root=before_levels[0]|before_levels[1]|before_debug;
   new_root=after_levels[0]|after_levels[1]|after_debug;
   if(!before_levels[4] && after_levels[4])begin
    events[0]=1;
    n[3:0]=old_max ? 4'hb : q[3:0]+4'd1;
   end
   new_max=(n[3:0]==4'hf);
   if(!old_max && new_max)begin events[1]=1;n[4]=old_pre;end
   new_pre=!n[4];
   if(!old_pre && new_pre)begin events[2]=1;n[5]=!q[5];end
   events[3]=!old_root && new_root;events[4]=old_root && !new_root;
   events[5]=old_pre && !new_pre;events[6]=!old_pre && new_pre;
   step={events,n};
  end
 endfunction
 reg [5:0] cursor,levels_cursor;
 reg debug_cursor;
 reg [12:0] transfer;
 always @*begin
  state_next=state_q;states_after_steps=0;events_at_steps=0;
  roots_after_steps=0;max_after_steps=0;mcpre_after_steps=0;aux_after_steps=0;
  cursor=state_q;levels_cursor=source_before;debug_cursor=debug_before;transfer=0;
  if(consume)for(integer index=0;index<7;index++)begin
   if(index==0 || index<=int'(event_count))begin
    if(index==0)transfer=step(cursor,levels_cursor,levels_cursor,debug_cursor,debug_at_boundary);
    else transfer=step(cursor,levels_cursor,source_after_events[6*(index-1)+:6],debug_cursor,debug_at_events[index-1]);
    cursor=transfer[5:0];events_at_steps[7*index+:7]=transfer[12:6];
    debug_cursor=(index==0) ? debug_at_boundary : debug_at_events[index-1];
    if(index>0)levels_cursor=source_after_events[6*(index-1)+:6];
    states_after_steps[6*index+:6]=cursor;
    roots_after_steps[index]=levels_cursor[0]|levels_cursor[1]|debug_cursor;
    max_after_steps[index]=(cursor[3:0]==4'hf);
    mcpre_after_steps[index]=!cursor[4];aux_after_steps[index]=!cursor[5];
   end
  end
  state_next=cursor;
 end
endmodule
`default_nettype wire
