// cell_MC10195 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 47 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10195 (
    output wire p2,  // a_OUT
    output wire p3,  // b_OUT
    output wire p4,  // c_OUT
    input  wire p5,  // a_IN0
    input  wire p6,  // b_IN0
    input  wire p7,  // c_IN0
    input  wire p9,  // a_C
    input  wire p10,  // d_IN0
    input  wire p11,  // e_IN0
    input  wire p12,  // f_IN0
    output wire p13,  // d_OUT
    output wire p14,  // e_OUT
    output wire p15// f_OUT
);

  // TODO: model this part.
endmodule

`default_nettype wire
