// cell_MC1668 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 12 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC1668 (
    output wire p2,  // Q
    output wire p3,  // Q'
    input  wire p4,  // R
    input  wire p5,  // S
    input  wire p6,  // D
    input  wire p7,  // C
    input  wire p9,  // C
    input  wire p11,  // D
    input  wire p12,  // S
    input  wire p13,  // R
    output wire p14,  // Q'
    output wire p15   // Q
);

  // TODO: model this part.
endmodule

`default_nettype wire
