// cell_MC10106 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 12 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10106 (
    output wire p2,  // b_OUT
    output wire p3,  // a_OUT
    input  wire p4,  // a_IN0
    input  wire p5,  // a_IN1
    input  wire p6,  // a_IN2
    input  wire p7,  // a_IN3
    input  wire p9,  // b_IN0
    input  wire p10,  // b_IN1
    input  wire p11,  // b_IN2
    input  wire p12,  // c_IN0
    input  wire p13,  // c_IN1
    input  wire p14,  // c_IN2
    output wire p15   // c_OUT
);

  // TODO: model this part.
endmodule

`default_nettype wire
