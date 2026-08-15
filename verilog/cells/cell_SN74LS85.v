// cell_SN74LS85 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 3 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74LS85 (
    input  wire p1,  // Y0
    input  wire p2,  // <
    input  wire p3,  // =
    input  wire p4,  // (no name in EclDict)
    output wire p5,  // (no name in EclDict)
    output wire p6,  // X=Y
    output wire p7,  // X<Y
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // Y3
    input  wire p10,  // X3
    input  wire p11,  // Y2
    input  wire p12,  // X2
    input  wire p13,  // X1
    input  wire p14,  // Y1
    input  wire p15,  // X0
    input  wire p16   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
