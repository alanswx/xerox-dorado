// cell_SN74LS251 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 6 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74LS251 (
    input  wire p1,  // D3
    input  wire p2,  // D2
    input  wire p3,  // D1
    input  wire p4,  // D0
    output wire p5,  // Y
    input  wire p7,  // E'
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // S4
    input  wire p10,  // S2
    input  wire p11,  // S1
    input  wire p12,  // D7
    input  wire p13,  // D6
    input  wire p14,  // D5
    input  wire p15,  // D4
    input  wire p16// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
