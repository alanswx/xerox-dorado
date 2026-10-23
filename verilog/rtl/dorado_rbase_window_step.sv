// Original ProcL g15 (F10145A) write-window transfer, HM2.3/Table4.
// g14 supplies latched write data; a19 selects Last/Next using PreSHCP C.
// A closing window commits the pre-transition address/data before the mux
// exposes the next task. Retain the original64 storage bits externally.
// No reset, second file, pipeline state or extra architectural clock.
`default_nettype none
module dorado_rbase_window_step(
 input wire [63:0] state_q,
 input wire write_n_before,write_n_after,
 input wire [3:0] address_before,address_after,data_before,data_after,
 output reg [63:0] state_next,
 output wire write_event,closing_event);
 assign closing_event=!write_n_before && write_n_after;
 assign write_event=!write_n_after || closing_event;
 always @*begin
  state_next=state_q;
  if(!write_n_after)state_next[4*address_after+:4]=data_after;
  else if(closing_event)state_next[4*address_before+:4]=data_before;
 end
endmodule
`default_nettype wire
