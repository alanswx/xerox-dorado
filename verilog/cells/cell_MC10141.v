// cell_MC10141 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 60 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10141 (
    input  wire p1,  // (no name in EclDict)
    output wire p2,  // Q2
    output wire p3,  // Q3
    input  wire p4,  // C
    input  wire p5,  // DL
    input  wire p6,  // D3
    input  wire p7,  // SR'
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // D2
    input  wire p10,  // SL'
    input  wire p11,  // D1
    input  wire p12,  // D0
    input  wire p13,  // DR
    output wire p14,  // Q0
    output wire p15// Q1
);

  // TODO: model this part.
endmodule

`default_nettype wire
