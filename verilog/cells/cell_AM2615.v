// cell_AM2615 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 7 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_AM2615 (
    output wire p1,  // Q'
    output wire p2,  // PU
    input  wire p5,  // +
    input  wire p7,  // -Ref
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // -Ref
    input  wire p11,  // +
    output wire p14,  // PU
    output wire p15,  // Q'
    input  wire p16// (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
