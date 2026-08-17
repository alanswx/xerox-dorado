// cell_SN74LS253 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 2 package position(s) across the sixteen boards.
//
// Dual 4-Input Multiplexer, three-state (TTL). TtlDict:
//   a,S2,2 > a,S1,14
//   a,X0,6 > a,X1,5 > a,X2,4 > a,X3,3 > a,EX',1 > a,OX,7
//   a,Y0,10 > a,Y1,11 > a,Y2,12 > a,Y3,13 > a,EY',15 > a,OY,9
// The two halves share the select and have their own enables. As with the
// '251, a disabled output is driven low rather than high-Z.

`default_nettype none

module cell_SN74LS253 (
    input  wire p1,  // EX'
    input  wire p2,  // S2
    input  wire p3,  // X3
    input  wire p4,  // X2
    input  wire p5,  // X1
    input  wire p6,  // X0
    output wire p7,  // OX
    input  wire p8,  // (no name in EclDict)
    output wire p9,  // OY
    input  wire p10,  // Y0
    input  wire p11,  // Y1
    input  wire p12,  // Y2
    input  wire p13,  // Y3
    input  wire p14,  // S1
    input  wire p15,  // EY'
    input  wire p16// (no name in EclDict)
);

  wire [1:0] sel = {p2, p14};             // S2, S1

  wire [3:0] x = {p3, p4, p5, p6};        // X3..X0
  wire [3:0] y = {p13, p12, p11, p10};    // Y3..Y0

  assign p7 = ~p1  & x[sel];              // OX
  assign p9 = ~p15 & y[sel];              // OY

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};
endmodule

`default_nettype wire
