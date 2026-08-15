// cell_MC10171 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 8 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10171 (
    input  wire p2,  // ER'
    output wire p3,  // R3'
    output wire p4,  // R2'
    output wire p5,  // R1'
    output wire p6,  // R0'
    input  wire p7,  // S1
    input  wire p9,  // S2
    output wire p10,  // Q3'
    output wire p11,  // Q2'
    output wire p12,  // Q1'
    output wire p13,  // Q0'
    input  wire p14,  // EQ'
    input  wire p15// E'
);

  // TODO: model this part.
endmodule

`default_nettype wire
