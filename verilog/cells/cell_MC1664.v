// cell_MC1664 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 19 package position(s) across the sixteen boards.
//
// Quad 2-Input OR (MECL III). The FUNCTION comes from the dictionary rather
// than a datasheet: all four gates have a single output each with role `o`,
// and `o` is the non-inverting sense -- so every output is an OR. That also
// matches the MECL III pairing, where MC1662 is the NOR part.
//
// EclDict: a,IN,4,5 > a,o,2 | b,IN,6,7 > b,o,3
//          c,IN,10,11 > c,o,14 | d,IN,12,13 > d,o,15
//
// POLARITY, confirmed from the datasheets (2026-08-16): EclDict's role `OUT`
// is the INVERTING (NOR) output and `o` the non-inverting (OR) one. Motorola's
// MC10101 sheet labels the four `OUT` pins A-bar-OUT..D-bar-OUT and the four
// `o` pins AOUT..DOUT; MC10212's labels its `OUT` pins 3,4,12,13 with bars and
// its `o` pins 2,14 without. Eight gates, two parts, unanimous. See
// docs/verilog-handoff.md.

`default_nettype none

module cell_MC1664 (
    output wire p2,  // a_OUTN
    output wire p3,  // b_OUTN
    input  wire p4,  // a_IN0
    input  wire p5,  // a_IN1
    input  wire p6,  // b_IN0
    input  wire p7,  // b_IN1
    input  wire p10,  // c_IN0
    input  wire p11,  // c_IN1
    input  wire p12,  // d_IN0
    input  wire p13,  // d_IN1
    output wire p14,  // c_OUTN
    output wire p15// d_OUTN
);

  assign p2  = p4  | p5;
  assign p3  = p6  | p7;
  assign p14 = p10 | p11;
  assign p15 = p12 | p13;
endmodule

`default_nettype wire
