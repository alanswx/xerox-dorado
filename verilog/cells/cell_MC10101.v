// cell_MC10101 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 54 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10101 (
    output wire p2,  // a_OUT
    output wire p3,  // b_OUT
    input  wire p4,  // a_IN0
    output wire p5,  // a_OUTN
    output wire p6,  // b_OUTN
    input  wire p7,  // b_IN0
    output wire p9,  // d_OUTN
    input  wire p10,  // c_IN0
    output wire p11,  // c_OUTN
    input  wire p12,  // a_C
    input  wire p13,  // d_IN0
    output wire p14,  // c_OUT
    output wire p15   // d_OUT
);

  // TODO: model this part.
endmodule

`default_nettype wire
