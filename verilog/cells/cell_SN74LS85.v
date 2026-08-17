// cell_SN74LS85 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 3 package position(s) across the sixteen boards.
//
// 4-Bit Magnitude Comparator (TTL). TtlDict:
//   a,X0,15 > a,X1,13 > a,X2,12 > a,X3,10     X0 is the MOST significant,
//   a,Y0,1  > a,Y1,14 > a,Y2,11 > a,Y3,9      as PARC numbers everything
//   a,>,4 > a,=,3 > a,<,2                     cascade inputs from the stage below
//   a,X>Y,5 > a,X=Y,6 > a,X<Y,7
// The cascade inputs decide only when the four bits are equal, which is what
// chains these into a wider comparator.

`default_nettype none

module cell_SN74LS85 (
    input  wire p1,  // Y0
    input  wire p2,  // <
    input  wire p3,  // =
    input  wire p4,  // (no name in EclDict)
    output wire p5,  // (no name in EclDict)
    output wire p6,  // X=Y
    output wire p7,  // X<Y
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // Y3
    input  wire p10,  // X3
    input  wire p11,  // Y2
    input  wire p12,  // X2
    input  wire p13,  // X1
    input  wire p14,  // Y1
    input  wire p15,  // X0
    input  wire p16// (no name in EclDict)
);

  wire [3:0] x = {p15, p13, p12, p10};    // X0..X3, most significant first
  wire [3:0] y = {p1,  p14, p11, p9};

  assign p5 = (x > y) | ((x == y) & p4);  // X>Y
  assign p6 =           (x == y) & p3;    // X=Y
  assign p7 = (x < y) | ((x == y) & p2);  // X<Y

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};
endmodule

`default_nettype wire
