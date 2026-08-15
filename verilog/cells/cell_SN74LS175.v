// cell_SN74LS175 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 2 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74LS175 (
    input  wire p1,  // CL'
    output wire p2,  // Q0
    input  wire p4,  // D0
    input  wire p5,  // D1
    output wire p7,  // Q1
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // CK
    input  wire p16// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
