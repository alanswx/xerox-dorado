// cell_MC10141 -- Four-Bit Universal Shift Register
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 60 package position(s).
//
// VERIFY: the MODE ENCODING is read from the pin names (SL'/SR' = shift
// left / shift right, both active low) rather than from a reproduced truth
// table. If a board using this part misbehaves, check this first.

`default_nettype none

module cell_MC10141 (
    input  wire sys_clk,
    input  wire p4,
    input  wire p12,
    input  wire p11,
    input  wire p9,
    input  wire p6,
    input  wire p5,
    input  wire p13,
    input  wire p10,
    input  wire p7,
    output wire p14,
    output wire p15,
    output wire p2,
    output wire p3,
    input  wire p1,
    input  wire p8
);

  // C=4 clock; D0-D3 = 12,11,9,6 parallel data; DL=5 / DR=13 serial ins;
  // SL'=10 / SR'=7 mode selects (active low); Q0-Q3 = 14,15,2,3.
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
  always @(posedge sys_clk) ck_d <= p4;
  wire ck_en = p4 & ~ck_d;

  always @(posedge sys_clk) if (ck_en) begin
    case ({p10, p7})                       // SL', SR'
      2'b00: q <= {p9, p11, p12, p6};      // both asserted: parallel load
      2'b01: q <= {q[2:0], p13};           // SL' only: shift left,  DR in
      2'b10: q <= {p5, q[3:1]};            // SR' only: shift right, DL in
      default: q <= q;                     // neither: hold
    endcase
  end
  assign {p3, p2, p15, p14} = q;


  // Board-wired pins not named above (power and the like)
  wire _unused_pins = &{1'b0, p1, p8, 1'b0};

endmodule

`default_nettype wire
