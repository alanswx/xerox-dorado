`default_nettype none
// Simulation of the falling-edge enable contract used by native ALTCLKCTRL.
// The hardware fit uses the dedicated clock resource, not this fabric gate.
// Raw guest/scanner requests must be supplied before the falling edge;
// the separately latched guest-advance flag is available AFTER that edge.
module dorado_ff_clock_model(
 input wire clk,enable,
 output wire gated_clk,
 output reg gate_open=1'b0
);
 always @(negedge clk)gate_open<=enable;
 assign gated_clk=clk & gate_open;
endmodule
`default_nettype wire
