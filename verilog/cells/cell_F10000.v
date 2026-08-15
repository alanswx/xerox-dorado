// cell_F10000 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 58 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_F10000 (
    output wire p2,  // H2
    output wire p3,  // H3
    input  wire p4,  // CC
    input  wire p5,  // PE'
    input  wire p6,  // DS
    input  wire p7,  // D3
    input  wire p9,  // D2
    input  wire p10,  // D1
    input  wire p11,  // D0
    input  wire p12,  // MR
    input  wire p13,  // CC
    output wire p14,  // H0
    output wire p15   // H1
);

  // TODO: model this part.
endmodule

`default_nettype wire
