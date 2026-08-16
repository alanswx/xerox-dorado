// cell_MC10175 -- Quint Latch
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 18 package position(s).

`default_nettype none

module cell_MC10175 (
    input  wire sys_clk,
    input  wire p10,
    input  wire p12,
    input  wire p13,
    input  wire p9,
    input  wire p5,
    input  wire p6,
    input  wire p7,
    input  wire p11,
    output wire p14,
    output wire p15,
    output wire p2,
    output wire p3,
    output wire p4
);

  // D0-D4 = 10,12,13,9,5; Q0-Q4 = 14,15,2,3,4; CC = 6,7 clock (two pins,
  // wire-ORed); MR=11 master reset.
  wire clk = p6 | p7;
  reg [4:0] q;

  // FPGA: ONE CLOCK, and the ECL clock net becomes an ENABLE.
  //
  // The Dorado clocks this part from a distributed ECL clock net (CLK.ph'
  // and friends, fanned out by the BaseBoard). Taking that literally gives
  // the fabric a gated clock off combinational logic -- 1,201 packages across
  // the machine do it -- which no FPGA can route. So every flip-flop here
  // runs on `sys_clk` and transfers on the sys_clk edge FOLLOWING an edge of
  // the modelled clock net. That is the usual oversampling transform, and it
  // matches the part as long as sys_clk is faster than the clock net, which
  // dorado_machine guarantees by dividing.
  //
  // Asynchronous inputs (MR, S/R, CL') are LEVEL-tested every sys_clk edge --
  // the same treatment, and it keeps them out of the fabric's reset network.
  reg ck_d;
  always @(posedge sys_clk) ck_d <= clk;
  wire ck_en = clk & ~ck_d;

  always @(posedge sys_clk) begin
    if (p11)        q <= 5'd0;
    else if (ck_en) q <= {p5, p9, p13, p12, p10};
  end
  assign {p4, p3, p2, p15, p14} = q;


endmodule

`default_nettype wire
