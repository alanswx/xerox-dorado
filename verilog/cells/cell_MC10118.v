// cell_MC10118 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 9 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10118 (
    output wire p2,  // a_OUT
    input  wire p3,  // e
    input  wire p4,  // e
    input  wire p5,  // e
    input  wire p6,  // a_IN0
    input  wire p7,  // a_IN1
    input  wire p9,  // a_C
    input  wire p10,  // a
    input  wire p11,  // a
    input  wire p12,  // s
    input  wire p13,  // s
    input  wire p14,  // s
    output wire p15// a_OUTN
);

  // TODO: model this part.
endmodule

`default_nettype wire
