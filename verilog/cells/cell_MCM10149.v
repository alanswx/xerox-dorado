// cell_MCM10149 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 15 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MCM10149 (
    input  wire p2,  // A1
    input  wire p3,  // A2
    input  wire p4,  // A0
    input  wire p5,  // A6
    input  wire p6,  // A5
    input  wire p7,  // A7
    input  wire p9,  // A3
    input  wire p10,  // A4
    output wire p11,  // Q3
    output wire p12,  // Q2
    input  wire p13,  // CE'
    output wire p14,  // Q1
    output wire p15   // Q0
);

  // TODO: model this part.
endmodule

`default_nettype wire
