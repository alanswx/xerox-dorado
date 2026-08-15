// cell_MC10182 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 3 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MC10182 (
    output wire p2,  // COUT
    output wire p4,  // H1
    input  wire p5,  // D1
    input  wire p6,  // E1
    input  wire p11,  // E0
    input  wire p12,  // D0
    input  wire p13,  // CIN
    output wire p14   // H0
);

  // TODO: model this part.
endmodule

`default_nettype wire
