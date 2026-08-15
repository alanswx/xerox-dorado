// cell_MC10175 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 18 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10175 (
    output wire p2,  // Q2
    output wire p3,  // Q3
    output wire p4,  // Q4
    input  wire p5,  // D4
    input  wire p6,  // CC
    input  wire p7,  // CC
    input  wire p9,  // D3
    input  wire p10,  // D0
    input  wire p11,  // MR
    input  wire p12,  // D1
    input  wire p13,  // D2
    output wire p14,  // Q0
    output wire p15   // Q1
);

  // TODO: model this part.
endmodule

`default_nettype wire
