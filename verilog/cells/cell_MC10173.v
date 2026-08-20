// cell_MC10173 -- Quad 2-Input Multiplexer/Latch
//
// Pin numbers/names: PARC's EclDict.Analyze (what ANALYZE used to check the
// boards). Function: the part's TRUTH TABLE and logic diagram in
// DoradoDocs/datasheets/Motorola_MECL_DataBook_1978.pdf p.3-79.
// Used in 81 package position(s) across the sixteen Dorado boards.
//
//   SELECT  CLOCK  Q0(n+1)
//     H       L      D00        "A high level enables data inputs D00, D10,
//     L       L      D01         D20, D30 and a low level enables D01, D11,
//     x       H      Q0n         D21, D31."
//
// and "any change on the data input will be reflected at the outputs while the
// clock is LOW. The outputs are latched on the positive transition of the
// clock." Outputs are non-inverting -- the logic diagram's latch box carries
// no bubbles.
//
// PARC's `B' pins ARE the datasheet's D_0 pins and its `D' pins the D_1 pins:
// dict B0/B1/B2/B3 = 6/4/13/11 = D00/D10/D20/D30, dict D0/D1/D2/D3 = 5/3/12/10
// = D01/D11/D21/D31. So `SB' high selecting B matches the part exactly.
//
// MECL is positive logic. Unused inputs have on-chip pulldowns (they read
// LOW), which is why the boards leave so many pins unconnected; Verilator
// treats an unconnected input as 0, which matches.
//
// Both conventions -- SB high selects B, and the latch is transparent while C
// is LOW -- were unverified here until 2026-08-19 and are now read off the
// datasheet above AND gated: `make -C verilog compute-test` fails on either
// one reversed, because T is loaded through this part.

// EDGE-TRIGGERED on the fabric clock with the latch enable as an ENABLE, the
// convention this design uses for every clocked element.
//
// The part is a TRANSPARENT latch: while its enable is asserted the output
// follows the input, which is a combinational path straight through it and
// closes a loop wherever the machine feeds a latch from something downstream
// of it. sys_clk heavily oversamples every signal on the board, so the
// behaviour while transparent is the same to a fabric clock's precision -- and
// unlike a latch it synthesises.

`default_nettype none

module cell_MC10173 (
    input  wire sys_clk,
    input  wire p5,  // D0,
    input  wire p3,  // D1,
    input  wire p12,  // D2,
    input  wire p10,  // D3,
    input  wire p6,  // B0,
    input  wire p4,  // B1,
    input  wire p13,  // B2,
    input  wire p11,  // B3,
    input  wire p9,   // SB select,
    input  wire p7,   // C  latch enable,
    output wire p1,  // Q0,
    output wire p2,  // Q1,
    output wire p15,  // Q2,
    output wire p14// Q3
);

  // Quad 2-input multiplexer feeding a transparent latch.
  // The mux picks D or B per SB; C gates the latch.
  wire [3:0] d = { p9 ? p11 : p10,     // Q3
                   p9 ? p13 : p12,     // Q2
                   p9 ? p4  : p3,      // Q1
                   p9 ? p6  : p5 };    // Q0
  // The inferred latch here IS the part: MC10173 is a multiplexer/LATCH, so
  // this is intended hardware, not the usual accident of an incomplete
  // always block. (Do not begin a comment line with the tool's own name --
  // it is parsed as a pragma.)
  reg [3:0] q;
  always @(posedge sys_clk) if (!p7) q <= d;   // C low = transparent
  assign {p14, p15, p2, p1} = q;

endmodule

`default_nettype wire
