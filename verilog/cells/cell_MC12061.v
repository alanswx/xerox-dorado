// cell_MC12061 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC12061 (
    input  wire p1,  // VCC1
    output wire p2,  // SINo
    output wire p3,  // SINo'
    input  wire p4,  // AGC
    input  wire p5,  // Xtal1
    input  wire p6,  // Xtal2
    input  wire p7,  // BIAS
    input  wire p8,  // VEE1
    input  wire p9,  // TVEE
    output wire p10,  // TTtlQ
    input  wire p11,  // TVCC
    input  wire p14,  // SINi'
    input  wire p15,  // SINi
    input  wire p16   // EVCC
);

  // TODO: model this part.
endmodule

`default_nettype wire
