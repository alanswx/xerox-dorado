// cell_F10000 -- Four-Bit Shift Register with parallel load
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 58 package position(s).
//
// VERIFY: the MODE ENCODING is read from the pin names (SL'/SR' = shift
// left / shift right, both active low) rather than from a reproduced truth
// table. If a board using this part misbehaves, check this first.

`default_nettype none

module cell_F10000 (
    input  wire sys_clk,
    input  wire p4,
    input  wire p13,
    input  wire p11,
    input  wire p10,
    input  wire p9,
    input  wire p7,
    input  wire p6,
    input  wire p12,
    input  wire p5,
    output wire p14,
    output wire p15,
    output wire p2,
    output wire p3
);

  // CC = 4,13 clock (two pins, wire-ORed on the board); D0-D3 = 11,10,9,7
  // parallel data; DS=6 serial data in; MR=12 master reset; PE'=5 parallel
  // enable (active low); H0-H3 = 14,15,2,3 outputs.
  //
  // Distinguished from F10016 by its pins: that part has CE'/CO' (count
  // enable, carry out) and is a counter; this one has DS, a serial input,
  // so it shifts.
  wire clk = p4 | p13;
  reg [3:0] q;

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
    if (p12)            q <= 4'd0;              // MR
    else if (ck_en) begin
      if (!p5)          q <= {p7, p9, p10, p11};// PE' parallel load
      else              q <= {q[2:0], p6};      // shift, DS in
    end
  end
  assign {p3, p2, p15, p14} = q;


endmodule

`default_nettype wire
