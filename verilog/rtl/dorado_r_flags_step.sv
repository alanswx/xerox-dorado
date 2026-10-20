// Complete the coupled R operand slice with both l06 sign/odd latches.
// Component experiment; not connected to native clocks. RM/STK/IFU and
// Ready/MIR/Hold producers retain the inherited external boundary contract.
// [591:0] is the unchanged R operand slice. [593:592] H l06 {B,A};
// [595:594] L l06 {B,A}. rm_flags_n is {RmOdd',RmLtZero'}: each original
// net is the wired OR of THREE complemented MC1668 outputs, not l07 alone.
`default_nettype none
module dorado_r_flags_step(
 input wire [595:0] state_q,input wire clock_before,clock_after,
 input wire [151:0] pins_before,pins_after,
 output reg [595:0] state_next,output wire [32:0] clock_events,repeated_events,
 output wire unsettled,output wire [1:0] write_n,latch_n,t_latch_n,bypass,
 output wire [3:0] parity_ok,faults,output wire [1:0] r_read_n,r_visible_n,
 output wire [3:0] r_flags,output wire [1:0] rm_flags_n);
 wire [591:0] child_next;
 dorado_r_operand_step child(state_q[591:0],clock_before,clock_after,
   pins_before,pins_after,child_next,clock_events[28:0],repeated_events[28:0],
   unsettled,write_n,latch_n,t_latch_n,bypass,parity_ok,faults,r_read_n,r_visible_n,r_flags);
 wire [1:0] old_read_n={clock_before|!state_q[8]|state_q[10]|state_q[48],
                        clock_before|!state_q[13]|state_q[12]|state_q[32]};
 wire [1:0] old_md={state_q[51],state_q[34]};
 wire [1:0] new_md={child_next[51],child_next[34]};
 wire [1:0] old_bypass={pins_before[150],pins_before[148]};
 wire [1:0] new_bypass={pins_after[150],pins_after[148]};
 wire [3:0] old_clocks={(!old_read_n[1] && old_bypass[1] && !old_md[1]),
                       (!old_read_n[1] && old_bypass[1] && old_md[1]),
                       (!old_read_n[0] && old_bypass[0] && !old_md[0]),
                       (!old_read_n[0] && old_bypass[0] && old_md[0])};
 wire [3:0] new_clocks={(!r_read_n[1] && new_bypass[1] && !new_md[1]),
                       (!r_read_n[1] && new_bypass[1] && new_md[1]),
                       (!r_read_n[0] && new_bypass[0] && !new_md[0]),
                       (!r_read_n[0] && new_bypass[0] && new_md[0])};
 assign clock_events[32:29]=new_clocks & ~old_clocks;
 assign repeated_events[32:29]=0;
 always @* begin
   state_next=state_q;state_next[591:0]=child_next;
   // Manufacturer priority: HIGH-transparent data first, then closed set.
   for(integer half=0;half<2;half=half+1) begin
     if(new_clocks[2*half]) state_next[592+2*half]=pins_after[half ? 107 : 92];
     else if(!r_read_n[half] && (!new_bypass[half] || !new_md[half]))
       state_next[592+2*half]=1;
     if(new_clocks[2*half+1]) state_next[593+2*half]=pins_after[half ? 91 : 76];
     else if(!r_read_n[half] && (!new_bypass[half] || new_md[half]))
       state_next[593+2*half]=1;
   end
 end
 assign rm_flags_n={!(state_next[595] & state_next[594] & child_next[590]),
                    !(state_next[593] & state_next[592] & child_next[588])};
endmodule
`default_nettype wire
