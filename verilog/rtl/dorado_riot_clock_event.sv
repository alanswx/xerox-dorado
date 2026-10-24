// Explicit original PHI2-event consumer for one BaseBoard MCS6532.
// UNCONNECTED to native boards. The caller supplies the bus BEFORE that
// event, after preceding source records have settled. This is not CPU Hold.
// Source/clock-cone proofs bound PHI2 to at most one rise per15ns interval.
// No phase-history register, added guest cycle or new RIOT state is needed.
`default_nettype none
module dorado_riot_clock_event(
 input wire clk,consume,phi_rise,
 input wire reset_n_at_edge,
 input wire [6:0] address_at_edge,
 input wire rw_at_edge,ram_n_at_edge,cs1_at_edge,cs2_n_at_edge,
 input wire [7:0] data_at_edge,pa_at_edge,pb_at_edge,
 output wire [7:0] data_out,pa_out,pb_out,
 output wire irq_n,output_enable
);
 wire commit=consume && phi_rise;
 // M6532 has synchronous reset branches outside its ce guard. An original
 // PHI2-clocked package cannot reset on a nonexistent edge. Mask only that
 // reset between events; a real reset event retains the complete vendor
 // reset behavior, including RAM initialization and old_pa7 preservation.
 wire event_reset_n=!commit || reset_n_at_edge;
 M6532 u_riot(.clk(clk),.ce(commit),.res_n(event_reset_n),
  .addr(address_at_edge),.RW_n(rw_at_edge),.RS_n(ram_n_at_edge),
  .CS1(cs1_at_edge),.CS2_n(cs2_n_at_edge),.d_in(data_at_edge),
  .PA_in(pa_at_edge),.PB_in(pb_at_edge),
  .d_out(data_out),.PA_out(pa_out),.PB_out(pb_out),
  .IRQ_n(irq_n),.oe(output_enable));
endmodule
`default_nettype wire
