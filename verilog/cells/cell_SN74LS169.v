// cell_SN74LS169 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 4 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_SN74LS169 (
    input  wire p1,  // UD
    input  wire p2,  // CK
    input  wire p3,  // B3
    input  wire p4,  // B2
    input  wire p5,  // B1
    input  wire p6,  // B0
    input  wire p7,  // EP'
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // LD'
    input  wire p10,  // ET'
    output wire p15,  // RC'
    input  wire p16   // (no name in EclDict)
);

  // TODO: model this part.
endmodule

`default_nettype wire
