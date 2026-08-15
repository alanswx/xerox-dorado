// cell_MC10173 -- Quad 2-Input Multiplexer/Latch
//
// Pin numbers/names: PARC's EclDict.Analyze (what ANALYZE used to check the
// boards). Function: Motorola MECL Pocket Book -- see cells/PARTS.md.
// Used in 81 package position(s) across the sixteen Dorado boards.
//
// MECL is positive logic. Unused inputs have on-chip pulldowns (they read
// LOW), which is why the boards leave so many pins unconnected; Verilator
// treats an unconnected input as 0, which matches.
//
// VERIFY: two conventions here are read from the pin names rather than from
// a reproduced truth table -- SB high selects B (as in the MC10158 family),
// and the latch is TRANSPARENT while C is low, latching on C high. Both are
// the first things to check if a board using this part misbehaves.

`default_nettype none

module cell_MC10173 (
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
  /* verilator lint_off LATCH */
  reg [3:0] q;
  always @* if (!p7) q = d;            // transparent while C low
  /* verilator lint_on LATCH */
  assign {p14, p15, p2, p1} = q;

endmodule

`default_nettype wire
