// cell_MC10159 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 67 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10159 (
    output wire p1,  // Q0'
    output wire p2,  // Q1'
    input  wire p3,  // D1
    input  wire p4,  // B1
    input  wire p5,  // D0
    input  wire p6,  // B0
    input  wire p7,  // E'
    input  wire p9,  // SB
    input  wire p10,  // D3
    input  wire p11,  // B3
    input  wire p12,  // D2
    input  wire p13,  // B2
    output wire p14,  // Q3'
    output wire p15   // Q2'
);

  // TODO: model this part.
endmodule

`default_nettype wire
