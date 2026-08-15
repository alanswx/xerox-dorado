// cell_SN74125 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 2 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74125 (
    input  wire p1,  // EN
    input  wire p2,  // a_IN0
    output wire p3,  // a_OUT
    input  wire p4,  // EN
    input  wire p5,  // b_IN0
    output wire p6,  // b_OUT
    input  wire p7,  // (no name in EclDict)
    output wire p8,  // c_OUT
    input  wire p9,  // c_IN0
    input  wire p10,  // EN
    output wire p11,  // d_OUT
    input  wire p12,  // d_IN0
    input  wire p13,  // EN
    input  wire p14// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
