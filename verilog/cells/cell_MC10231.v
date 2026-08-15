// cell_MC10231 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 152 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10231 (
    input  wire p1,  // (no name in EclDict)
    output wire p2,  // Q
    output wire p3,  // Q'
    input  wire p4,  // R
    input  wire p5,  // S
    input  wire p6,  // C
    input  wire p7,  // D
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // CC
    input  wire p10,  // D
    input  wire p11,  // C
    input  wire p12,  // S
    input  wire p13,  // R
    output wire p14,  // Q'
    output wire p15,  // Q
    input  wire p16   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
