// cell_MC10125 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 66 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10125 (
    input  wire p1,  // v
    input  wire p2,  // a_C
    input  wire p3,  // a_IN0
    output wire p4,  // a_OUT
    output wire p5,  // b_OUT
    input  wire p6,  // b_C
    input  wire p7,  // b_IN0
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // x
    input  wire p10,  // c_C
    input  wire p11,  // c_IN0
    output wire p12,  // c_OUT
    output wire p13,  // d_OUT
    input  wire p14,  // d_C
    input  wire p15,  // d_IN0
    input  wire p16// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
