// cell_F9401 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 2 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_F9401 (
    input  wire p1,  // CK'
    input  wire p2,  // P'
    input  wire p3,  // S0
    input  wire p4,  // MR
    input  wire p5,  // S1
    input  wire p7,  // (no name in EclDict)
    input  wire p8,  // S2
    input  wire p10,  // CWE
    input  wire p11,  // D
    output wire p12,  // Q
    output wire p13,  // ER
    input  wire p14// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
