// cell_SIPpackage -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 40 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SIPpackage (
    input  wire p1,  // (no name in EclDict)
    inout  wire p2,  // (no name in EclDict) // BOTH directions seen across boards
    inout  wire p3,  // (no name in EclDict) // BOTH directions seen across boards
    inout  wire p4,  // (no name in EclDict) // BOTH directions seen across boards
    inout  wire p5,  // (no name in EclDict) // BOTH directions seen across boards
    inout  wire p6,  // (no name in EclDict) // BOTH directions seen across boards
    inout  wire p7,  // (no name in EclDict) // BOTH directions seen across boards
    inout  wire p8   // (no name in EclDict) // BOTH directions seen across boards
);

  // TODO: model this part.
endmodule

`default_nettype wire
