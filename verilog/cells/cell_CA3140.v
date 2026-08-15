// cell_CA3140 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 10 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_CA3140 (
    input  wire p2,  // -
    input  wire p3,  // +
    input  wire p4,  // -Pwr
    output wire p6,  // a_OUT
    input  wire p7// +Pwr
);

  // TODO: model this part.
endmodule

`default_nettype wire
