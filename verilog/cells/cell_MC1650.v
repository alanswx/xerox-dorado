// cell_MC1650 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 4 package position(s) across the sixteen boards.
//
// Function: Motorola MC1650/MC1651 data sheet (DoradoDocs/datasheets/MC1650.pdf).
// A dual differential comparator with an output latch. On DskEth all four
// positions are the DISK READ path: f01/f02 receive drive 0-1 clock and data,
// c01/c02 drive 2-3.

`default_nettype none

module cell_MC1650 (
    input  wire sys_clk,
    input  wire p1,  // (no name in EclDict)
    output wire p2,  // Q
    input  wire p4,  // C
    input  wire p5,  // V2
    input  wire p6,  // V1
    input  wire p7,  // x
    input  wire p8,  // (no name in EclDict)
    input  wire p10,  // x
    input  wire p11,  // V2
    input  wire p12,  // V1
    input  wire p13,  // C
    output wire p14,  // Q
    input  wire p16// (no name in EclDict)
);

  // DUAL A/D CONVERTER -- a differential comparator with an output LATCH.
  // Function: Motorola data sheet, DoradoDocs/datasheets/MC1650.pdf, whose
  // truth table is explicit:
  //
  //     C     V1, V2     Q0(n+1)   Q0'(n+1)
  //     H     V1 > V2       H         L
  //     H     V1 < V2       L         H
  //     L      x    x     Q0(n)     Q0'(n)
  //
  // and the prose: "When Ca is at a logic high level, Q0 will be at a logic
  // high level provided that V1 > V2 (V1 is more positive than V2). Q0 is the
  // logic complement of Q0. When the clock input goes to a low logic level,
  // the outputs are latched in their present state."
  //
  // The pin assignment matches EclDict's exactly -- V1a 6, V2a 5, Ca 4 -> Q 2,
  // Q' 3; V1b 12, V2b 11, Cb 13 -> Q 14, Q' 15.
  //
  // DIGITAL REDUCTION. The part compares analog levels; here each input is a
  // single bit and the two are a DIFFERENTIAL PAIR, so V1 > V2 is (V1 & ~V2)
  // and V1 < V2 is (~V1 & V2). When the two are EQUAL the comparison is
  // undefined and a real comparator holds near its threshold -- so this holds
  // the previous value, which also makes an undriven pair (both 0, as the
  // Dorado's cable nets idle) read as "no transition" rather than a fabricated
  // level. That matters on DskEth, where these four packages ARE the disk read
  // data and clock inputs.
  //
  // FPGA: ONE CLOCK. The latch is level-sensitive, and `always @*` on a
  // level-sensitive part makes a combinational loop out of any feedback path
  // (see cells/PARTS.md and the F10145A/i2125 family). So it runs on sys_clk
  // with C as an ENABLE, the same oversampling transform used everywhere else.
  reg qa, qb;

  always @(posedge sys_clk) begin
    if (p4) begin                       // Ca high: transparent
      if (p6 & ~p5)      qa <= 1'b1;    // V1a > V2a
      else if (~p6 & p5) qa <= 1'b0;    // V1a < V2a
    end
    if (p13) begin                      // Cb high: transparent
      if (p12 & ~p11)      qb <= 1'b1;  // V1b > V2b
      else if (~p12 & p11) qb <= 1'b0;  // V1b < V2b
    end
  end

  assign p2  = qa;
  assign p14 = qb;

endmodule

`default_nettype wire
