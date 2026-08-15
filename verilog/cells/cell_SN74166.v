// cell_SN74166 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 40 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74166 (
    input  wire p1,  // SI
    input  wire p2,  // A
    input  wire p3,  // B
    input  wire p4,  // C
    input  wire p5,  // D
    input  wire p6,  // CE'
    input  wire p7,  // CK
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // CL'
    input  wire p10,  // E
    input  wire p11,  // F
    input  wire p12,  // G
    output wire p13,  // QH
    input  wire p14,  // H
    input  wire p15,  // SL
    input  wire p16   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
