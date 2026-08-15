// cell_SN74LS155 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74LS155 (
    input  wire p1,  // DQ
    input  wire p2,  // EQ'
    input  wire p3,  // S2
    output wire p4,  // Q3'
    output wire p5,  // Q2'
    output wire p6,  // Q1'
    output wire p7,  // Q0'
    input  wire p8,  // (no name in EclDict)
    output wire p9,  // R0'
    output wire p10,  // R1'
    output wire p11,  // R2'
    output wire p12,  // R3'
    input  wire p13,  // S1
    input  wire p14,  // ER'
    input  wire p15,  // DR'
    input  wire p16   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
