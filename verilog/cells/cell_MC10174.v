// cell_MC10174 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 106 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10174 (
    input  wire p1,  // (no name in EclDict)
    output wire p2,  // OX
    input  wire p3,  // X0
    input  wire p4,  // X2
    input  wire p5,  // X1
    input  wire p6,  // X3
    input  wire p7,  // S1
    input  wire p9,  // S2
    input  wire p10,  // Y3
    input  wire p11,  // Y1
    input  wire p12,  // Y2
    input  wire p13,  // Y0
    input  wire p14,  // E'
    output wire p15,  // OY
    input  wire p16   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
