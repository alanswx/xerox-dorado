// cell_MC10212 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 15 package position(s) across the sixteen boards.
//
// High-Speed Dual 3-Input 3-Output OR/NOR (MECL 10K). Built to drive up to
// six transmission lines, which is why the Dorado uses it for clock and
// control distribution: the datasheet calls out "clock distribution
// applications where minimum clock skew is desired".
//
// Each gate gives ONE OR output and TWO NOR outputs -- pins 2 (OR) and 3,4
// (NOR) for the a side, 14 (OR) and 12,13 (NOR) for the b side. Its sibling
// MC10210 is the all-OR version and MC10211 the all-NOR one, and those two
// have IDENTICAL dictionary entries (every output role `OUT`), which is why
// single-sense parts take their polarity from the part name instead.
//
// POLARITY, confirmed from the datasheets (2026-08-16): EclDict's role `OUT`
// is the INVERTING (NOR) output and `o` the non-inverting (OR) one. Motorola's
// MC10101 sheet labels the four `OUT` pins A-bar-OUT..D-bar-OUT and the four
// `o` pins AOUT..DOUT; MC10212's labels its `OUT` pins 3,4,12,13 with bars and
// its `o` pins 2,14 without. Eight gates, two parts, unanimous. See
// docs/verilog-handoff.md.

`default_nettype none

module cell_MC10212 (
    output wire p2,  // a_OUTN
    output wire p3,  // a_OUT
    output wire p4,  // a_OUT
    input  wire p5,  // a_IN0
    input  wire p6,  // a_IN1
    input  wire p7,  // a_IN2
    input  wire p9,  // b_IN0
    input  wire p10,  // b_IN1
    input  wire p11,  // b_IN2
    output wire p12,  // b_OUT
    output wire p13,  // b_OUT
    output wire p14,  // b_OUTN
    input  wire p15,  // (no name in EclDict)
    input  wire p16// (no name in EclDict)
);

  wire a = p5 | p6 | p7;
  wire b = p9 | p10 | p11;
  assign p3  = ~a;  assign p4  = ~a;   // A-bar-OUT, two of them
  assign p2  =  a;                     // AOUT
  assign p12 = ~b;  assign p13 = ~b;   // B-bar-OUT
  assign p14 =  b;                     // BOUT

  wire _unused_pins = &{1'b0, p15, p16, 1'b0};
endmodule

`default_nettype wire
