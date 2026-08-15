// cell_MC10180 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 13 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10180 (
    output wire p1,  // H1'
    output wire p2,  // H0'
    output wire p3,  // K0
    input  wire p4,  // C0
    input  wire p5,  // D0
    input  wire p6,  // E0
    input  wire p7,  // M0
    input  wire p9,  // M1
    input  wire p10,  // E1
    input  wire p11,  // D1
    input  wire p12,  // C1
    output wire p13,  // K1
    output wire p14,  // H1
    output wire p15   // H0
);

  // TODO: model this part.
endmodule

`default_nettype wire
