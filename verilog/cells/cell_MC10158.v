// cell_MC10158 -- Quad 2-Input Multiplexer (non-inverting)
//
// Pin numbers/names: PARC's EclDict.Analyze (what ANALYZE used to check the
// boards). Function: Motorola MECL Pocket Book -- see cells/PARTS.md.
// Used in 88 package position(s) across the sixteen Dorado boards.
//
// MECL is positive logic. Unused inputs have on-chip pulldowns (they read
// LOW), which is why the boards leave so many pins unconnected; Verilator
// treats an unconnected input as 0, which matches.
//
// The select POLARITY is confirmed by the MC10159 data sheet
// (DoradoDocs/datasheets/MC10159.pdf), which shares this pinout: SELECT high
// enables D00/D10/D20/D30 (pins 6, 4, 13, 11), low enables D01/D11/D21/D31.
// This part has NO enable -- the dictionary gives it `SB` and no `E'` -- so
// the disabled-outputs-low row of that table does not apply here.
// Historical note, kept because the comment used to ask to VERIFY it: The MECL Pocket
// Book gives the part as "Quad 2-Input Multiplexer" without reproducing the
// truth table here. If a board's behaviour disagrees, this is the line to
// check first, and the inverting sibling MC10159 shares the convention.

`default_nettype none

module cell_MC10158 (
    input  wire p5,  // D0,
    input  wire p3,  // D1,
    input  wire p12,  // D2,
    input  wire p10,  // D3,
    input  wire p6,  // B0,
    input  wire p4,  // B1,
    input  wire p13,  // B2,
    input  wire p11,  // B3,
    input  wire p9,   // SB select,
    output wire p1,  // Q0,
    output wire p2,  // Q1,
    output wire p15,  // Q2,
    output wire p14// Q3
);

  // Quad 2-input multiplexer, NON-inverting outputs.
  // SB selects which input reaches Q; high selects the B inputs.
  assign p1  = p9 ? p6  : p5;
  assign p2  = p9 ? p4  : p3;
  assign p15 = p9 ? p13 : p12;
  assign p14 = p9 ? p11 : p10;

endmodule

`default_nettype wire
