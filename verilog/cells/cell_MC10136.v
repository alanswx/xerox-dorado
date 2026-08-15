// cell_MC10136 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 9 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10136 (
    output wire p2,  // H1
    output wire p3,  // H0
    output wire p4,  // CO'
    input  wire p5,  // B0
    input  wire p6,  // B1
    input  wire p7,  // SD'
    input  wire p9,  // SU'
    input  wire p10,  // CI'
    input  wire p11,  // B2
    input  wire p12,  // B3
    input  wire p13,  // C
    output wire p14,  // H3
    output wire p15// H2
);

  // TODO: model this part.
endmodule

`default_nettype wire
