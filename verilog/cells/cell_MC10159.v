// cell_MC10159 -- Quad 2-Input Multiplexer (inverting)
//
// Pin numbers/names: PARC's EclDict.Analyze (what ANALYZE used to check the
// boards). Function: Motorola MECL Pocket Book -- see cells/PARTS.md.
// Used in 10 package position(s) across the sixteen Dorado boards.
//
// MECL is positive logic. Unused inputs have on-chip pulldowns (they read
// LOW), which is why the boards leave so many pins unconnected; Verilator
// treats an unconnected input as 0, which matches.
//
// VERIFY: the select POLARITY is taken from the pin name -- PARC calls it
// `SB`, read as "select B", so SB high routes the B inputs. The MECL Pocket
// Book gives the part as "Quad 2-Input Multiplexer" without reproducing the
// truth table here. If a board's behaviour disagrees, this is the line to
// check first, and the inverting sibling MC10159 shares the convention.

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
  wire en = ~p7;
  assign p1  = ~(en & (p9 ? p6  : p5));
  assign p2  = ~(en & (p9 ? p4  : p3));
  assign p15 = ~(en & (p9 ? p13 : p12));
  assign p14 = ~(en & (p9 ? p11 : p10));

endmodule

`default_nettype wire
