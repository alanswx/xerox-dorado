// cell_MC1690 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 4 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC1690 (
    output wire p2,  // Q
    output wire p3,  // Q'
    input  wire p7,  // C1
    input  wire p11// D1
);

  // TODO: model this part.
endmodule

`default_nettype wire
