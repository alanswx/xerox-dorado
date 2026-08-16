// cell_MC1690 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 4 package position(s) across the sixteen boards.
//
// Type D master-slave flip-flop (MECL III), the fastest part in the machine.
// EclDict names the pins the boards use: D1=11, C1=7, Q=2, Q'=3.
//
// All four packages are the BaseBoard's CLOCK GENERATOR. g04 and h04 are
// cross-coupled on one clock phase -- g04.D = h04.Q', h04.D = g04.Q -- which
// is a Johnson counter dividing by four; g03 and h03 delay g04's output on
// the opposite phase. Out of that come `StartClockPulse'` (g04.Q') and
// `EndClockPulse` (h03.Q), which the MC10210 at j02 fans out to every slot
// as CLK.ph', CLK.mc' and the rest.
//
// FPGA: on `sys_clk` with C1 as an enable, like every other clocked cell
// here -- see cell_MC10176 for the reasoning.

`default_nettype none

module cell_MC1690 (
    input  wire sys_clk,
    output wire p2,  // Q
    output wire p3,  // Q'
    input  wire p7,  // C1
    input  wire p11// D1
);

  reg ck_d, q;
  always @(posedge sys_clk) ck_d <= p7;
  wire ck_en = p7 & ~ck_d;
  always @(posedge sys_clk) if (ck_en) q <= p11;

  assign p2 =  q;       // Q
  assign p3 = ~q;       // Q'
endmodule

`default_nettype wire
