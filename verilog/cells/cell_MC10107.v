// cell_MC10107 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 5 package position(s) across the sixteen boards.
//
// Triple 2-Input Exclusive OR / Exclusive NOR Gate (MECL 10K, DL122 rev 7
// p.257). The data book prints both outputs of each gate:
//
//     3 = (4 * 5bar) + (4bar * 5)      the OR  output -- Exclusive OR
//     2 = (4 * 5)    + (4bar * 5bar)   the NOR output -- Exclusive NOR
//
// Which of the pair is which follows the polarity rule settled from the
// datasheets and recorded in docs/verilog-handoff.md: EclDict's role `OUT` is
// the INVERTING output and `o` the non-inverting one. Here that is
// `a,OUT,2 > a,o,3`, `b,OUT,11 > b,o,10`, `c,OUT,12 > c,o,13`.
//
// [G (4 5)>(2 3), (7 9)>(10 11), (14 15)>(12 13)]

`default_nettype none

module cell_MC10107 (
    output wire p2,  // a_OUT
    output wire p3,  // a_OUTN
    input  wire p4,  // a_IN0
    input  wire p5,  // a_IN1
    input  wire p6,  // (no name in EclDict)
    input  wire p7,  // b_IN0
    input  wire p9,  // b_IN1
    output wire p10,  // b_OUTN
    output wire p11,  // b_OUT
    output wire p12,  // c_OUT
    output wire p13,  // c_OUTN
    input  wire p14,  // c_IN0
    input  wire p15// c_IN1
);

  assign p2  = ~(p4  ^ p5);    assign p3  = p4  ^ p5;
  assign p11 = ~(p7  ^ p9);    assign p10 = p7  ^ p9;
  assign p12 = ~(p14 ^ p15);   assign p13 = p14 ^ p15;
endmodule

`default_nettype wire
