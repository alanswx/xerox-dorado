// Closed storage and physical open-Q view for the four ProcH/ProcL l07 bits.
// Motorola MECL III MC1668, 1978 data book PDF235/printed4-30: HIGH C follows
// D regardless of Set/Reset; CLOSED C permits Set/Reset. Both original l07
// Reset pins are unwired/pulled down. No undefined simultaneous S/R here.
// No extra state, reset or architectural cycle. The caller owns held_q.
//
// A closed Set is exposed immediately and stored at the same enabled owner
// commit. Boundary inputs must remain frozen while that owner is paused.
// On C falling, retain pre-close physical Q, including late D. Do not sample
// new open-window data into closed storage merely because a host edge occurs.
`default_nettype none
module dorado_r_flag_window_step(
 input wire [3:0] held_q,clock_before,clock_after,data_before,set_before,set_after,
 output wire [3:0] held_next,physical_q,closing_events);
 assign closing_events=clock_before & ~clock_after;
 genvar bit_no;
 generate for(bit_no=0;bit_no<4;bit_no=bit_no+1)begin: flag_latch
  assign physical_q[bit_no]=clock_before[bit_no] ? data_before[bit_no] :
    (set_before[bit_no] ? 1'b1 : held_q[bit_no]);
  assign held_next[bit_no]=(!clock_after[bit_no] && set_after[bit_no]) ? 1'b1 :
    ((clock_before[bit_no] && clock_after[bit_no]) ? held_q[bit_no] : physical_q[bit_no]);
 end endgenerate
endmodule
`default_nettype wire
