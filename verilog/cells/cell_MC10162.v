// cell_MC10162 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 10 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10162 (
    input  wire p2,  // E'
    output wire p3,  // Q3
    output wire p4,  // Q2
    output wire p5,  // Q1
    output wire p6,  // Q0
    input  wire p7,  // S1
    input  wire p9,  // S2
    output wire p10,  // Q7
    output wire p11,  // Q6
    output wire p12,  // Q5
    output wire p13,  // Q4
    input  wire p14,  // S4
    input  wire p15   // E'
);

  // TODO: model this part.
endmodule

`default_nettype wire
