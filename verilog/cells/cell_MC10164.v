// cell_MC10164 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 42 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10164 (
    input  wire p2,  // E'
    input  wire p3,  // D3
    input  wire p4,  // D2
    input  wire p5,  // D1
    input  wire p6,  // D0
    input  wire p7,  // S1
    input  wire p9,  // S2
    input  wire p10,  // S4
    input  wire p11,  // D4
    input  wire p12,  // D5
    input  wire p13,  // D6
    input  wire p14,  // D7
    output wire p15   // Z
);

  // TODO: model this part.
endmodule

`default_nettype wire
