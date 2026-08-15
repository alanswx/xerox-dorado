// cell_F10145A -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 105 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_F10145A (
    output wire p1,  // Q1
    output wire p2,  // Q0
    input  wire p3,  // CE'
    input  wire p4,  // D1
    input  wire p5,  // D0
    input  wire p6,  // A3
    input  wire p7,  // A2
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // A1
    input  wire p10,  // A0
    input  wire p11,  // D2
    input  wire p12,  // D3
    input  wire p13,  // WE'
    output wire p14,  // Q3
    output wire p15// Q2
);

  // TODO: model this part.
endmodule

`default_nettype wire
