// Coupled task/T/parity and R operand staging. Component experiment only.
// No native integration, full-board timing or 60ns operating claim.
// State[539:0]: unchanged T/parity slice; [555:540]: j01/j02 dRm;
// [571:556]: g05/h05 dR; [587:572]: g06/g07 visible R, raw bit order;
// [589:588]/[591:590]: H/L l07 {RbBypassDly, sign/odd Q}.
// Pins[151:0] retain the parity interface except [142:127] now carry
// functional RM/STK read data, [148]/[150] RbBypass (not its delayed Q).
// The paired complemented RbBypass pins are generated at the boundary.
// RAM address/selection/storage, bypass and IFU producers remain outside.
`default_nettype none
module dorado_r_operand_step(
 input wire [591:0] state_q,input wire clock_before,clock_after,
 input wire [151:0] pins_before,pins_after,
 output reg [591:0] state_next,output wire [28:0] clock_events,repeated_events,
 output wire unsettled,output wire [1:0] write_n,latch_n,t_latch_n,bypass,
 output wire [3:0] parity_ok,faults,output wire [1:0] r_read_n,r_visible_n,
 output wire [3:0] r_flags);
 wire [151:0] child_before,child_after;
 assign child_before={pins_before[151],state_q[591],pins_before[149],state_q[589],
                      pins_before[147:143],state_q[587:572],pins_before[126:0]};
 assign child_after={pins_after[151],state_q[591],pins_after[149],state_q[589],
                     pins_after[147:143],state_q[587:572],pins_after[126:0]};
 wire [539:0] child_next;
 wire [3:0] child_parity_ok,child_faults;
 dorado_t_parity_step t_path(state_q[539:0],clock_before,clock_after,child_before,child_after,
   child_next,clock_events[24:0],repeated_events[24:0],unsettled,
   write_n,latch_n,t_latch_n,bypass,child_parity_ok,child_faults);
 wire [1:0] old_read_n={clock_before|!state_q[8]|state_q[10]|state_q[48],
                        clock_before|!state_q[13]|state_q[12]|state_q[32]};
 assign r_read_n=latch_n;assign r_visible_n=t_latch_n;
 // MC1668 is HIGH-transparent, not edge-triggered. Its truth table gives
 // the open clock priority over S/R. l07 A follows the non-bypassed sign/odd
 // operand while its gate is high; DsRd sets its closed Q during bypass.
 // l07 B follows the bypass mask throughout the file-read window.
 wire [3:0] old_r_clocks={~old_read_n[1],!(old_read_n[1]|pins_before[150]),
                          ~old_read_n[0],!(old_read_n[0]|pins_before[148])};
 wire [3:0] new_r_clocks={~r_read_n[1],!(r_read_n[1]|pins_after[150]),
                          ~r_read_n[0],!(r_read_n[0]|pins_after[148])};
 assign clock_events[28:25]=new_r_clocks & ~old_r_clocks;
 assign repeated_events[28:25]=0;
 always @* begin
   state_next=state_q;state_next[539:0]=child_next;
   if(!r_read_n[0]) begin
     state_next[547:540]=child_next[34] ? pins_after[99:92] : pins_after[83:76];
     state_next[563:556]=pins_after[149] ? {8{pins_after[118]}} : pins_after[134:127];
   end
   if(!r_read_n[1]) begin
     state_next[555:548]=child_next[51] ? pins_after[107:100] : pins_after[91:84];
     state_next[571:564]=pins_after[151] ? pins_after[126:119] : pins_after[142:135];
   end
   if(new_r_clocks[0]) state_next[588]=pins_after[127];
   if(new_r_clocks[1]) state_next[589]=pins_after[148] & !pins_after[149];
   if(new_r_clocks[2]) state_next[590]=pins_after[142];
   if(new_r_clocks[3]) state_next[591]=pins_after[150] & !pins_after[151];
   if(!r_read_n[0] && pins_after[148]) state_next[588]=1;
   if(!r_read_n[1] && pins_after[150]) state_next[590]=1;
   if(!r_visible_n[0]) state_next[579:572]=state_next[589] ? state_next[547:540] : state_next[563:556];
   if(!r_visible_n[1]) state_next[587:580]=state_next[591] ? state_next[555:548] : state_next[571:564];
 end
 wire high_r_ok=(^state_next[579:572])^child_next[535];
 wire low_r_ok=(^state_next[587:580])^child_next[539];
 assign parity_ok={low_r_ok,child_parity_ok[2],high_r_ok,child_parity_ok[0]};
 assign faults={!(state_next[591]|pins_after[151]|low_r_ok),child_faults[2],
                !(state_next[589]|pins_after[149]|high_r_ok),child_faults[0]};
 assign r_flags={state_next[591:590],state_next[589:588]};
endmodule
`default_nettype wire
