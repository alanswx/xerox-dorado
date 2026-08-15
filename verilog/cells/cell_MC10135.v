// cell_MC10135 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 29 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10135 (
    output wire p2,  // Q
    output wire p3,  // Q'
    input  wire p4,  // R
    input  wire p5,  // S
    input  wire p6,  // K'
    input  wire p7,  // J'
    input  wire p9,  // CC
    input  wire p10,  // J'
    input  wire p11,  // K'
    input  wire p12,  // S
    input  wire p13,  // R
    output wire p14,  // Q'
    output wire p15   // Q
);

  // TODO: model this part.
endmodule

`default_nettype wire
