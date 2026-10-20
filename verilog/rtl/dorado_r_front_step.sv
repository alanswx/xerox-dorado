// Original front RBase/stack storage coupled to the qualified R-memory slice.
// Component only. MIR/IFU/ALU/counter/I/O producer boundaries remain explicit.
// Q[635:0] = existing processor. Q[735:636] = g14,g15,h14,l14,l15,
// i16,i17,l16,l18,j19 in original raw package-bit order.
// The generated combinational cone shares original gate wiring/functions;
// independent delayed sequential models establish capture/storage ordering.
`default_nettype none
module dorado_r_front_step(
 input wire host_clk,advance,input wire [735:0] state_q,
 input wire clock_before,clock_after,input wire [235:0] pins_before,pins_after,
 output wire [735:0] state_next,output wire [56:0] clock_events,repeated_events,
 output wire unsettled,output wire [1:0] write_n,latch_n,t_latch_n,bypass,
 output wire [3:0] parity_ok,faults,output wire [1:0] r_read_n,r_visible_n,
 output wire [3:0] r_flags,output wire [1:0] rm_flags_n,
 output wire [31:0] address,output wire [3:0] select_n,
 output wire [1:0] forwarded_bypass,r_write_n,
 output wire [31:0] port_address,output wire [3:0] port_select_n,
 output wire [15:0] file_read_data,output wire [1:0] file_read_parity,
 output wire [201:0] source_pins,output wire [7:0] fa_n);
 function automatic [3:0] next_raw(input [87:0] q);
   next_raw=q[73] ? ~q[67:64] : {q[78],q[79],q[76],q[77]};
 endfunction
 function automatic [201:0] effective_pins(input [201:0] p,input [7:0] fa);
   reg [201:0] result;
   begin
     result=p;
     // The existing evaluator accepts decoded FA/FB/FC. Memory-reference
     // ASEL forms decode FA as zero regardless of the raw FF high bits.
     // Its FA0-specific functions remain disabled when no FA0 decode exists.
     result[19:18]=fa[0] ? 2'b11 : 2'b00;
     effective_pins=result;
   end
 endfunction
 function automatic [27:0] processor_pins(input [87:0] q,input [201:0] p);
   processor_pins={!q[75],p[69],p[68],q[75],next_raw(q),p[19:0]};
 endfunction
 wire [201:0] before_source,pre_source;
 wire [7:0] before_fa,pre_fa;
 wire [56:0] before_clocks,after_clocks;
 wire [43:0] pre_data;
 dorado_r_front_cone before_view(.state_q(state_q),.base(clock_before),.pins(pins_before),
   .source_pins(before_source),.fa_n(before_fa),.clocks(before_clocks));
 // Data presented to edge-triggered address/pointer stages precedes the
 // other registers' clock-to-Q updates at that same root event.
 dorado_r_front_cone pre_view(.state_q(state_q),.base(clock_before),.pins(pins_after),
   .source_pins(pre_source),.fa_n(pre_fa),.front_data(pre_data));
 wire [201:0] core_before=effective_pins(before_source,before_fa);
 wire [201:0] core_pre=effective_pins(pre_source,pre_fa);
 wire [63:0] phase_next;
 wire [21:0] phase_events,phase_repeated;
 wire phase_unsettled;
 dorado_processor_capture_step phase(state_q[63:0],clock_before,clock_after,
   processor_pins(state_q[87:0],core_before),processor_pins(state_q[87:0],core_pre),
   phase_next,phase_events,phase_repeated,phase_unsettled);
 wire [735:0] phase_state={state_q[735:64],phase_next};
 dorado_r_front_cone clock_view(.state_q(phase_state),.base(clock_after),.pins(pins_after),
   .clocks(after_clocks));
 reg [99:0] front_next;
 wire [11:0] captures=after_clocks[56:45] & ~before_clocks[56:45];
 always @* begin
   front_next=state_q[735:636];
   // LOW-transparent g14 and both StkWadr mux latches.
   if(!after_clocks[45]) front_next[3:0]=pre_data[3:0];
   if(!after_clocks[51]) front_next[85:82]=pre_data[29:26];
   if(!after_clocks[52]) front_next[89:86]=pre_data[33:30];
   if(captures[2]) front_next[73:68]=pre_data[17:12];
   if(captures[3]) front_next[79:74]=pre_data[23:18];
   if(captures[4]) front_next[80]=pre_data[24];
   if(captures[5]) front_next[81]=pre_data[25];
   if(captures[8]) front_next[93:90]=pre_data[37:34];
   if(captures[9]) front_next[97:94]=pre_data[41:38];
   if(captures[10]) front_next[98]=pre_data[42];
   if(captures[11]) front_next[99]=pre_data[43];
   // The level-written 16x4 RBase file sees the resolved g14 latch output.
   if(!after_clocks[46]) front_next[4+4*pre_data[11:8]+:4]=front_next[3:0];
 end
 wire [735:0] low_state={front_next,phase_state[635:0]};
 wire [201:0] low_source;
 wire [7:0] low_fa;
 dorado_r_front_cone low_view(.state_q(low_state),.base(clock_after),.pins(pins_after),
   .source_pins(low_source),.fa_n(low_fa));
 // During the closed HIGH root use the pre-arrival capture source. During
 // LOW transparent windows use the resolved front latches and RAM contents.
 wire [201:0] core_after=clock_after ? core_pre : effective_pins(low_source,low_fa);
 wire [635:0] child_next;
 wire [31:0] child_address;
 wire [3:0] child_select;
 wire [1:0] child_forward;
 wire child_unsettled;
 dorado_r_memory_step child(host_clk,advance,state_q[635:0],clock_before,clock_after,
   core_before,core_after,child_next,clock_events[44:0],repeated_events[44:0],child_unsettled,
   write_n,latch_n,t_latch_n,bypass,parity_ok,faults,r_read_n,r_visible_n,r_flags,rm_flags_n,
   child_address,child_select,child_forward,r_write_n,port_address,port_select_n,file_read_data,file_read_parity);
 assign state_next={front_next,child_next};
 // Settled outputs use the actual new Qs, including the real bypass compare.
 wire [56:0] settled_clocks;
 dorado_r_front_cone settled_view(.state_q(state_next),.base(clock_after),.pins(pins_after),
   .source_pins(source_pins),.fa_n(fa_n),.clocks(settled_clocks),
   .address(address),.select_n(select_n),.forwarded_bypass(forwarded_bypass));
 assign clock_events[56:45]=captures;
 assign repeated_events[56:45]=0;
 assign unsettled=child_unsettled|phase_unsettled|(|phase_repeated)|
   (before_fa[0]!=before_fa[4])|(pre_fa[0]!=pre_fa[4])|(low_fa[0]!=low_fa[4])|
   (settled_clocks[56:45]!=after_clocks[56:45])|
   (child_address!=address)|(child_select!=select_n);
endmodule
`default_nettype wire
