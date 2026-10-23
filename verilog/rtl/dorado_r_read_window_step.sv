// Original low-transparent R input latches: dRm H/L and dR H/L (32 bits).
// External held_q owns the existing storage; no reset or extra state here.
// Bytes0/2 belong to ProcH and1/3 to ProcL, matching Q571:540.
// Open Q is a combinational view, not a register updated every fabric tick.
// On closing, capture data_before, including changes since the last sample.
// The caller must provide the actual pre-close RAM/mux data epoch and preserve
// setup/hold at the closing gate. Native producer/phase integration is pending.
`default_nettype none
module dorado_r_read_window_step(
 input wire [31:0] held_q,
 input wire [1:0] gate_before,gate_after,
 input wire [31:0] data_before,data_after,
 output wire [31:0] held_next,view_before,view_after,
 output wire [1:0] closing_events);
 assign closing_events=~gate_before & gate_after;
 generate for(genvar byte_no=0;byte_no<4;byte_no=byte_no+1)begin: byte_latch
   localparam integer HALF=byte_no%2;
   assign held_next[8*byte_no+:8]=closing_events[HALF] ? data_before[8*byte_no+:8] : held_q[8*byte_no+:8];
   assign view_before[8*byte_no+:8]=gate_before[HALF] ? held_q[8*byte_no+:8] : data_before[8*byte_no+:8];
   assign view_after[8*byte_no+:8]=gate_after[HALF] ? held_next[8*byte_no+:8] : data_after[8*byte_no+:8];
 end endgenerate
endmodule
`default_nettype wire
