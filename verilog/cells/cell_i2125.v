// cell_i2125 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 32 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_i2125 (
    input  wire p1,  // CS'
    input  wire p2,  // A0
    input  wire p3,  // A1
    input  wire p4,  // A2
    input  wire p5,  // A3
    input  wire p6,  // A4
    output wire p7,  // DO
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // A5
    input  wire p10,  // A6
    input  wire p11,  // A7
    input  wire p12,  // A8
    input  wire p13,  // A9
    input  wire p14,  // WE'
    input  wire p15,  // DI
    input  wire p16// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
