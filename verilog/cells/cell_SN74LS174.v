// cell_SN74LS174 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 16 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74LS174 (
    input  wire p1,  // CL'
    output wire p2,  // Q0
    input  wire p3,  // D0
    input  wire p4,  // D1
    output wire p5,  // Q1
    input  wire p6,  // D2
    output wire p7,  // Q2
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // CK
    output wire p10,  // Q3
    input  wire p11,  // D3
    output wire p12,  // Q4
    input  wire p13,  // D4
    input  wire p14,  // D5
    output wire p15,  // Q5
    input  wire p16   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
