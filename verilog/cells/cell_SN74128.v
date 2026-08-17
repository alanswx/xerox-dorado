// cell_SN74128 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// Quad 2-Input NOR Line Driver (TTL), on the '02 pinout:
//   a,IN,2,3 > a,OUT,1    b,IN,5,6 > b,OUT,4
//   c,IN,8,9 > c,OUT,10   d,IN,11,12 > d,OUT,13
// A line driver differs from a plain '02 only in its output drive, which has
// no digital expression.

`default_nettype none

module cell_SN74128 (
    output wire p1,  // (no name in EclDict)
    input  wire p2,  // (no name in EclDict)
    input  wire p3,  // (no name in EclDict)
    output wire p4,  // (no name in EclDict)
    input  wire p5,  // (no name in EclDict)
    input  wire p6,  // (no name in EclDict)
    input  wire p7,  // (no name in EclDict)
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // (no name in EclDict)
    output wire p10,  // (no name in EclDict)
    input  wire p11,  // (no name in EclDict)
    input  wire p12,  // (no name in EclDict)
    output wire p13,  // (no name in EclDict)
    input  wire p14// (no name in EclDict)
);

  assign p1  = ~(p2  | p3);
  assign p4  = ~(p5  | p6);
  assign p10 = ~(p8  | p9);
  assign p13 = ~(p11 | p12);

  wire _unused_pins = &{1'b0, p7, p14, 1'b0};
endmodule

`default_nettype wire
