// cell_MC10166 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 6 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10166 (
    output wire p2,  // (no name in EclDict)
    output wire p3,  // X<Y
    input  wire p4,  // Y4
    input  wire p5,  // X4
    input  wire p6,  // X3
    input  wire p7,  // Y3
    input  wire p9,  // X0
    input  wire p10,  // Y0
    input  wire p11,  // Y1
    input  wire p12,  // X1
    input  wire p13,  // X2
    input  wire p14,  // Y2
    input  wire p15   // E'
);

  // TODO: model this part.
endmodule

`default_nettype wire
