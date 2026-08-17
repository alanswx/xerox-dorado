// cell_SN74LS157 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 6 package position(s) across the sixteen boards.
//
// Quad 2-Input Multiplexer (TTL). TtlDict:
//   a,SB,1 > a,E',15
//   a,D0,2 > a,B0,3 > a,D1,5 > a,B1,6 > a,D2,11 > a,B2,10 > a,D3,14 > a,B3,13
//   a,Q0,4 > a,Q1,7 > a,Q2,9 > a,Q3,12
// `SB` selects the B inputs; the enable is active low and forces all four
// outputs low.

`default_nettype none

module cell_SN74LS157 (
    input  wire p1,  // SB
    input  wire p2,  // D0
    input  wire p3,  // B0
    output wire p4,  // Q0
    input  wire p5,  // D1
    input  wire p6,  // B1
    output wire p7,  // Q1
    input  wire p8,  // (no name in EclDict)
    output wire p9,  // Q2
    input  wire p10,  // B2
    input  wire p11,  // D2
    output wire p12,  // Q3
    input  wire p13,  // B3
    input  wire p14,  // D3
    input  wire p15,  // E'
    input  wire p16// (no name in EclDict)
);

  wire en = ~p15;                          // E'
  wire sb =  p1;                           // select B

  assign p4  = en & (sb ? p3  : p2);       // Q0
  assign p7  = en & (sb ? p6  : p5);       // Q1
  assign p9  = en & (sb ? p10 : p11);      // Q2
  assign p12 = en & (sb ? p13 : p14);      // Q3

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};
endmodule

`default_nettype wire
