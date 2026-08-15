// cell_LM339 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_LM339 (
    output wire p1,  // b_OUT
    output wire p2,  // a_OUT
    input  wire p3,  // +Pwr
    input  wire p4,  // -
    input  wire p5,  // +
    input  wire p6,  // -
    input  wire p7,  // +
    input  wire p8,  // -
    input  wire p9,  // +
    input  wire p10,  // -
    input  wire p11,  // +
    input  wire p12,  // -Pwr
    output wire p13,  // d_OUT
    output wire p14   // c_OUT
);

  // TODO: model this part.
endmodule

`default_nettype wire
