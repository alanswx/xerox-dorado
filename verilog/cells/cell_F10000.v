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

  // BIT ORDER: EclDict's H0 is the DATA SHEET's Q3, exactly as for the F10016.
  // The two parts share a pinout -- EclDict gives both `D0,11 D1,10 D2,9 D3,7`
  // and `H0,14 H1,15 H2,2 H3,3` -- and cell_F10016 was corrected from the
  // Fairchild connection diagram, which makes pin 3 = Q0 (LSB) and pin 14 = Q3
  // (MSB), pin 7 = P0 and pin 11 = P3.
  //
  // For PARALLEL LOAD AND HOLD this is a permutation that CANCELS: q[3] was
  // p11 and drove p14 before, and still does. Every F10000 used as a plain
  // register (ProcH g10/h10, the TIOA output register, with PE' an open pin)
  // is unaffected.
  //
  // For a SHIFT it is the whole difference. DskEth's input register is four of
  // these chained -- f09 takes ShiftReg.in and drives .12-.15, f10 takes
  // ShiftReg.12 and drives .08-.11, and so on. Read the old way, DS entered
  // q[0] = pin 14 = ShiftReg.12, so f10's DS took f09's FIRST stage: all four
  // packages saw the same bit and the chain was not a chain. Read this way DS
  // enters q[0] = pin 3 = ShiftReg.15, shifts toward pin 14 = ShiftReg.12, and
  // f10 continues from there -- a proper 16-stage shift register, with data
  // entering at the LSB end as PARC's MSB-first numbering implies.
  always @(posedge sys_clk) begin
    if (p12)            q <= 4'd0;              // MR
    else if (ck_en) begin
      if (!p5)          q <= {p11, p10, p9, p7};// PE' load: P3,P2,P1,P0
      else              q <= {q[2:0], p6};      // shift, DS into Q0 (pin 3)
    end
  end
  assign {p14, p15, p2, p3} = q;                // Q3,Q2,Q1,Q0 per the data sheet


endmodule

`default_nettype wire
