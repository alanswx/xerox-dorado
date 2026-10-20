// Component prototype. All consumers capture the SAME pre-edge permission as
// Core's existing falling-edge global clock gate. No pipeline stage is added.
// Each bit is REQUIRED implementation state outside physical board images.
`default_nettype none
module dorado_board_phase_enable #(parameter integer BOARDS=19)(
 input wire clk,permit,
 output wire [BOARDS-1:0] advance
);
 for(genvar b=0;b<BOARDS;b=b+1)begin:boards
  // Preserve the explicitly separate board distribution registers; a later
  // mapped-netlist review must prove the implementation retains them.
  (* preserve, dont_merge *) reg local_step=1'b0;
  always @(negedge clk)local_step<=permit;
  assign advance[b]=local_step;
 end
endmodule
`default_nettype wire
