// Original Ready storage coupled to the full RBase/stack/RM/STK component.
// Q[735:0] retains the prior physical map; Q[751:736] is ContA a09..d09.
// No additional RAM copy or architectural pipeline cycle.
`default_nettype none
module dorado_r_front_ready_step(
 input wire host_clk,advance,input wire [751:0] state_q,
 input wire clock_before,clock_after,input wire [245:0] pins_before,pins_after,
 output wire [751:0] state_next,output wire [58:0] clock_events,repeated_events,
 output wire unsettled,output wire [1:0] write_n,latch_n,t_latch_n,bypass,
 output wire [3:0] parity_ok,faults,output wire [1:0] r_read_n,r_visible_n,
 output wire [3:0] r_flags,output wire [1:0] rm_flags_n,
 output wire [31:0] address,output wire [3:0] select_n,
 output wire [1:0] forwarded_bypass,r_write_n,
 output wire [31:0] port_address,output wire [3:0] port_select_n,
 output wire [15:0] file_read_data,output wire [1:0] file_read_parity,
 output wire [201:0] source_pins,output wire [7:0] fa_n,
 output wire [29:0] ready_flags);
 // This mapper is identical to the qualified 104-Q task/Ready component.
 function automatic [3:0] current_task(input [87:0] q);
  current_task={q[78],q[79],q[76],q[77]};
 endfunction
 function automatic [27:0] ready_pins(input [87:0] q,input [85:0] p);
  reg [27:0] result;
  begin
   result=0;result[27:24]=q[67:64];result[3:0]=q[71:68];result[7:4]=current_task(q);
   result[11:8]=p[79:76];result[12]=!q[72];result[13]=p[80];
   result[14]=p[81];result[15]=p[82];result[23:22]=p[84:83];
   ready_pins=result;
  end
 endfunction
 wire [85:0] task_before={pins_before[245:236],pins_before[75:0]};
 wire [85:0] task_after={pins_after[245:236],pins_after[75:0]};
 wire [27:0] before_data=ready_pins(state_q[87:0],task_before);
 wire [27:0] capture_data=ready_pins(state_q[87:0],task_after);
 wire [14:0] before_ready,before_mask,pre_ready,pre_mask;
 dorado_task_ready_step #(.INDEPENDENT_BNT(1)) before_view(state_q[751:736],before_data,before_data,
   ,,,before_mask,before_ready);
 dorado_task_ready_step #(.INDEPENDENT_BNT(1)) pre_view(state_q[751:736],capture_data,capture_data,
   ,,,pre_mask,pre_ready);
 reg [235:0] child_before,child_after;
 always @* begin
  child_before=pins_before[235:0];child_after=pins_after[235:0];
  child_before[49:35]=before_ready;child_before[64:50]=before_mask;
  child_after[49:35]=pre_ready;child_after[64:50]=pre_mask;
 end
 wire [201:0] child_source;
 dorado_r_front_step child(host_clk,advance,state_q[735:0],clock_before,clock_after,
   child_before,child_after,state_next[735:0],clock_events[56:0],repeated_events[56:0],unsettled,
   write_n,latch_n,t_latch_n,bypass,parity_ok,faults,r_read_n,r_visible_n,r_flags,rm_flags_n,
   address,select_n,forwarded_bypass,r_write_n,port_address,port_select_n,file_read_data,file_read_parity,
   child_source,fa_n);
 wire [27:0] edge_data=capture_data | (28'(clock_events[20])<<18);
 dorado_task_ready_step #(.INDEPENDENT_BNT(1)) capture(state_q[751:736],capture_data,edge_data,
   state_next[751:736],clock_events[58:57],,,);
 assign repeated_events[58:57]={2{repeated_events[20]}};
 wire [27:0] settled_data=ready_pins(state_next[87:0],task_after);
 dorado_task_ready_step #(.INDEPENDENT_BNT(1)) settled_view(state_next[751:736],settled_data,settled_data,
   ,,,ready_flags[29:15],ready_flags[14:0]);
 // Source exports observe the actual post-capture Ready state. The child
 // consumes the old data epoch; no post-capture feedback is introduced.
 assign source_pins={child_source[201:65],ready_flags[29:15],ready_flags[14:0],child_source[34:0]};
endmodule
`default_nettype wire
