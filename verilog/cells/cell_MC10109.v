// cell_MC10109 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 21 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10109 (
    output wire p2,  // a_OUTN
    output wire p3,  // a_OUT
    input  wire p4,  // a_IN0
    input  wire p5,  // a_IN1
    input  wire p6,  // a_IN2
    input  wire p7,  // a_IN3
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // b_IN0
    input  wire p10,  // b_IN1
    input  wire p11,  // b_IN2
    input  wire p12,  // b_IN3
    input  wire p13,  // b_IN4
    output wire p14,  // b_OUT
    output wire p15// b_OUTN
);

  // TODO: model this part.
endmodule

`default_nettype wire
