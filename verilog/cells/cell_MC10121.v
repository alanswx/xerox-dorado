// cell_MC10121 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 44 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10121 (
    output wire p2,  // a_OUT
    output wire p3,  // a_OUTN
    input  wire p4,  // e
    input  wire p5,  // e
    input  wire p6,  // e
    input  wire p7,  // a_IN0
    input  wire p9,  // a_IN1
    input  wire p10,  // a_C
    input  wire p11,  // a
    input  wire p12,  // a
    input  wire p13,  // s
    input  wire p14,  // s
    input  wire p15   // s
);

  // TODO: model this part.
endmodule

`default_nettype wire
