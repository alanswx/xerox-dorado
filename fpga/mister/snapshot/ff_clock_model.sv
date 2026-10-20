`default_nettype none
// Simulation contract model, not an implementation of MiSTer's ALTCLKCTRL.
// Closing the enable latch during the high half prevents runt clock edges.
// Native hardware must use the dedicated global clock resource with its
// raw enable available before the existing falling-edge sampling boundary.
module dorado_ff_clock_model(
 input wire clk,enable,
 output wire gated_clk,
 output reg gate_open=1'b0
);
 always_latch if(!clk)gate_open<=enable;
 assign gated_clk=clk & gate_open;
endmodule
`default_nettype wire
