// cell_SN74125 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 2 package position(s) across the sixteen boards.
//
// Quad Bus Buffer with three-state outputs (TTL). TtlDict:
//   a,EN,1 > a,IN,2 > a,OUT,3      b,EN,4 > b,IN,5 > b,OUT,6
//   c,EN,10 > c,IN,9 > c,OUT,8     d,EN,13 > d,IN,12 > d,OUT,11
//
// The '125's enable is ACTIVE LOW -- that is the whole difference from the
// otherwise identical '126 -- so it is treated as such here even though the
// dictionary spells the pin bare rather than primed.
//
// Off the bus a three-state output contributes ZERO rather than high
// impedance, which is the shape every shared net takes in this design; see
// cell_MCS6502 for why.

`default_nettype none

module cell_SN74125 (
    input  wire p1,  // EN
    input  wire p2,  // a_IN0
    output wire p3,  // a_OUT
    input  wire p4,  // EN
    input  wire p5,  // b_IN0
    output wire p6,  // b_OUT
    input  wire p7,  // (no name in EclDict)
    output wire p8,  // c_OUT
    input  wire p9,  // c_IN0
    input  wire p10,  // EN
    output wire p11,  // d_OUT
    input  wire p12,  // d_IN0
    input  wire p13,  // EN
    input  wire p14// (no name in EclDict)
);

  assign p3  = ~p1  & p2;
  assign p6  = ~p4  & p5;
  assign p8  = ~p10 & p9;
  assign p11 = ~p13 & p12;

  wire _unused_pins = &{1'b0, p7, p14, 1'b0};
endmodule

`default_nettype wire
