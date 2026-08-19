// cell_MC10159 -- Quad 2-Input Multiplexer (inverting)
//
// Pin numbers/names: PARC's EclDict.Analyze (what ANALYZE used to check the
// boards). Function: Motorola MECL Pocket Book -- see cells/PARTS.md.
// Used in 67 package position(s) across the sixteen Dorado boards, 25 of
// them on ContA.
//
// MECL is positive logic. Unused inputs have on-chip pulldowns (they read
// LOW), which is why the boards leave so many pins unconnected; Verilator
// treats an unconnected input as 0, which matches.
//
// SETTLED from the data sheet, DoradoDocs/datasheets/MC10159.pdf, whose truth
// table this now matches on all five rows:
//
//     Enable  Select  D0  D1   Q
//        L       L     X   L   H          "Input levels are inverted at the
//        L       L     X   H   L           output"; SELECT high enables D00,
//        L       H     L   X   H           D10, D20, D30 (pins 6, 4, 13, 11)
//        L       H     H   X   L           and low enables D01, D11, D21, D31
//        H       X     X   X   L           (pins 5, 3, 12, 10)
//
// which confirms the select polarity the comment here used to ask to VERIFY:
// SB high does route the B inputs.
//
// THE LAST ROW IS THE ONE THAT WAS WRONG, and it matters more than it looks.
// A DISABLED PART DRIVES ITS OUTPUTS LOW, not high -- that is the whole point
// of an enable on a MECL part, because the outputs are open emitters tied
// together into a wired-OR and a disabled driver has to contribute NOTHING.
// This cell had `~(en & mux)`, which reads 1 when disabled and so jammed every
// bus it sat on to all ones. Found on ContA g22 and i21, which wired-OR into
// TNIA beside the MC10121 Link selectors: those twelve selectors carried
// Link[4:15] EXACTLY RIGHT and the answer was still wrong, because two
// disabled multiplexers were holding eight of the twelve bits high.

`default_nettype none

module cell_MC10159 (
    input  wire p5,  // D0,
    input  wire p3,  // D1,
    input  wire p12,  // D2,
    input  wire p10,  // D3,
    input  wire p6,  // B0,
    input  wire p4,  // B1,
    input  wire p13,  // B2,
    input  wire p11,  // B3,
    input  wire p9,   // SB select,
    input  wire p7,   // E' enable, active LOW,
    output wire p1,  // Q0',
    output wire p2,  // Q1',
    output wire p15,  // Q2',
    output wire p14// Q3'
);

  // Quad 2-input multiplexer, INVERTING outputs, with active-low enable.
  // Disabled, every output is LOW -- see the truth table above.
  wire en = ~p7;
  assign p1  = en & ~(p9 ? p6  : p5);
  assign p2  = en & ~(p9 ? p4  : p3);
  assign p15 = en & ~(p9 ? p13 : p12);
  assign p14 = en & ~(p9 ? p11 : p10);

endmodule

`default_nettype wire
