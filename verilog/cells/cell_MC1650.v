// cell_MC1650 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 4 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC1650 (
    input  wire p1,  // (no name in EclDict)
    output wire p2,  // Q
    input  wire p4,  // C
    input  wire p5,  // V2
    input  wire p6,  // V1
    input  wire p7,  // x
    input  wire p8,  // (no name in EclDict)
    input  wire p10,  // x
    input  wire p11,  // V2
    input  wire p12,  // V1
    input  wire p13,  // C
    output wire p14,  // Q
    input  wire p16// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
