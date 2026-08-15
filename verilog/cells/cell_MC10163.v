// cell_MC10163 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 4 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10163 (
    output wire p2,  // P0B
    output wire p3,  // P3
    input  wire p4,  // B5
    input  wire p5,  // B6
    input  wire p6,  // B2
    input  wire p7,  // B1
    input  wire p9,  // B0
    input  wire p10,  // B3
    input  wire p11,  // B7
    input  wire p12,  // B4
    output wire p13,  // P2
    output wire p14,  // P1
    output wire p15// P0A
);

  // TODO: model this part.
endmodule

`default_nettype wire
