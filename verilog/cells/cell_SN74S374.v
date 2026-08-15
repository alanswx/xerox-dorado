// cell_SN74S374 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 16 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74S374 (
    input  wire p1,  // OC'
    output wire p2,  // Q0
    input  wire p3,  // D0
    input  wire p4,  // D1
    output wire p5,  // Q1
    output wire p6,  // Q2
    input  wire p7,  // D2
    input  wire p8,  // D3
    input  wire p13,  // D4
    input  wire p14,  // D5
    output wire p15,  // Q5
    output wire p16,  // Q6
    input  wire p17,  // D6
    input  wire p20   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
