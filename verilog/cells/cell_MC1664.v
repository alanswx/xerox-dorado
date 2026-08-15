// cell_MC1664 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 19 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC1664 (
    output wire p2,  // a_OUTN
    output wire p3,  // b_OUTN
    input  wire p4,  // a_IN0
    input  wire p5,  // a_IN1
    input  wire p6,  // b_IN0
    input  wire p7,  // b_IN1
    input  wire p10,  // c_IN0
    input  wire p11,  // c_IN1
    input  wire p12,  // d_IN0
    input  wire p13,  // d_IN1
    output wire p14,  // c_OUTN
    output wire p15   // d_OUTN
);

  // TODO: model this part.
endmodule

`default_nettype wire
