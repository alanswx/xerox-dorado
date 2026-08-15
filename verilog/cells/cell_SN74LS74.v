// cell_SN74LS74 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 3 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74LS74 (
    input  wire p1,  // R'
    input  wire p2,  // D
    input  wire p3,  // C
    input  wire p4,  // S'
    output wire p5,  // Q
    output wire p6,  // Q'
    input  wire p7,  // (no name in EclDict)
    output wire p8,  // Q'
    input  wire p10,  // S'
    input  wire p11,  // C
    input  wire p12,  // D
    input  wire p13,  // R'
    input  wire p14   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
