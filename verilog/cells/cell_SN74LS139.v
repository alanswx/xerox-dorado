// cell_SN74LS139 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74LS139 (
    input  wire p1,  // EA'
    input  wire p2,  // AS1
    input  wire p3,  // AS2
    output wire p4,  // A0'
    output wire p5,  // A1'
    input  wire p8,  // (no name in EclDict)
    input  wire p16// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
