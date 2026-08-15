// cell_MC10107 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 5 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10107 (
    output wire p2,  // a_OUT
    output wire p3,  // a_OUTN
    input  wire p4,  // a_IN0
    input  wire p5,  // a_IN1
    input  wire p6,  // (no name in EclDict)
    input  wire p7,  // b_IN0
    input  wire p9,  // b_IN1
    output wire p10,  // b_OUTN
    output wire p11,  // b_OUT
    output wire p12,  // c_OUT
    output wire p13,  // c_OUTN
    input  wire p14,  // c_IN0
    input  wire p15// c_IN1
);

  // TODO: model this part.
endmodule

`default_nettype wire
