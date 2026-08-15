// cell_SN74LS138 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 7 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74LS138 (
    input  wire p1,  // S1
    input  wire p2,  // S2
    input  wire p3,  // S4
    input  wire p4,  // E'
    input  wire p5,  // E'
    input  wire p6,  // E
    output wire p7,  // Q7'
    input  wire p8,  // (no name in EclDict)
    output wire p9,  // Q6'
    output wire p10,  // Q5'
    output wire p11,  // Q4'
    output wire p12,  // Q3'
    output wire p13,  // Q2'
    output wire p14,  // Q1'
    output wire p15,  // Q0'
    input  wire p16// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
