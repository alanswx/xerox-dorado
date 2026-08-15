// cell_SN74LS259 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74LS259 (
    input  wire p1,  // S1
    input  wire p2,  // S2
    input  wire p3,  // S4
    output wire p5,  // Q1
    output wire p7,  // Q3
    input  wire p8,  // (no name in EclDict)
    input  wire p13,  // D
    input  wire p14,  // EN'
    input  wire p15,  // CL'
    input  wire p16   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
