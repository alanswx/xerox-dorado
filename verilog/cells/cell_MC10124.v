// cell_MC10124 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 41 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10124 (
    output wire p1,  // b_OUTN
    output wire p2,  // a_OUTN
    output wire p3,  // b_OUT
    output wire p4,  // a_OUT
    input  wire p5,  // a_IN0
    input  wire p6,  // a_C
    input  wire p7,  // b_IN0
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // x
    input  wire p10,  // c_IN0
    input  wire p11,  // d_IN0
    output wire p12,  // c_OUT
    output wire p13,  // d_OUT
    output wire p14,  // d_OUTN
    output wire p15,  // c_OUTN
    input  wire p16// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
