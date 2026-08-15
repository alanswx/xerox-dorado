// cell_SN74123 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 2 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74123 (
    input  wire p1,  // a'
    input  wire p2,  // b
    input  wire p3,  // CL'
    output wire p5,  // Q
    input  wire p6,  // C
    input  wire p7,  // R
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // a'
    input  wire p10,  // b
    input  wire p11,  // CL'
    output wire p13,  // Q
    input  wire p14,  // C
    input  wire p15,  // R
    input  wire p16   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
