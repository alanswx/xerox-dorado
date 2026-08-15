// cell_MC10179 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10179 (
    output wire p3,  // COUT
    input  wire p4,  // GI3
    input  wire p5,  // GI0
    output wire p6,  // CMD
    input  wire p7,  // GI2
    input  wire p9,  // GI1
    input  wire p10,  // PI2
    input  wire p11,  // CIN
    input  wire p12,  // PI1
    input  wire p13,  // PI0
    input  wire p14   // PI3
);

  // TODO: model this part.
endmodule

`default_nettype wire
