// cell_SN74LS153 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74LS153 (
    input  wire p1,  // EX'
    input  wire p2,  // S2
    input  wire p3,  // X3
    input  wire p4,  // X2
    input  wire p5,  // X1
    input  wire p6,  // X0
    output wire p7,  // OX
    input  wire p8,  // (no name in EclDict)
    output wire p9,  // OY
    input  wire p10,  // Y0
    input  wire p11,  // Y1
    input  wire p12,  // Y2
    input  wire p13,  // Y3
    input  wire p14,  // S1
    input  wire p15,  // EY'
    input  wire p16// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
