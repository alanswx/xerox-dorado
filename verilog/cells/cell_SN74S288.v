// cell_SN74S288 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74S288 (
    output wire p3,  // Q5
    output wire p4,  // Q4
    output wire p5,  // Q3
    output wire p6,  // Q2
    output wire p7,  // Q1
    input  wire p8,  // (no name in EclDict)
    output wire p9,  // Q0
    input  wire p10,  // A4
    input  wire p11,  // A3
    input  wire p12,  // A2
    input  wire p13,  // A1
    input  wire p14,  // A0
    input  wire p15,  // CS'
    input  wire p16   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
