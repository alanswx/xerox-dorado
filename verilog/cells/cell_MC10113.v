// cell_MC10113 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 55 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10113 (
    input  wire p1,  // (no name in EclDict)
    output wire p2,  // a_OUT
    output wire p3,  // b_OUT
    input  wire p4,  // a_IN0
    input  wire p5,  // a_IN1
    input  wire p6,  // b_IN0
    input  wire p7,  // b_IN1
    input  wire p9,  // e
    input  wire p10,  // c_IN0
    input  wire p11,  // c_IN1
    input  wire p12,  // d_IN0
    input  wire p13,  // d_IN1
    output wire p14,  // c_OUT
    output wire p15   // d_OUT
);

  // TODO: model this part.
endmodule

`default_nettype wire
