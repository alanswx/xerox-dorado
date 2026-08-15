// cell_MB7071H -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 70 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MB7071H (
    input  wire p2,  // WE'
    input  wire p3,  // BS3'
    input  wire p4,  // BS2'
    input  wire p5,  // BS1'
    input  wire p6,  // BS0'
    input  wire p7,  // A0
    input  wire p8,  // A1
    input  wire p9,  // A2
    input  wire p10,  // A3
    input  wire p11,  // A4
    input  wire p12,  // A5
    input  wire p13,  // A6
    input  wire p14,  // A7
    input  wire p15,  // (no name in EclDict)
    input  wire p16,  // e_IN0
    input  wire p17,  // d_IN0
    input  wire p18,  // c_IN0
    input  wire p19,  // b_IN0
    output wire p20,  // e_OUT
    output wire p21,  // d_OUT
    output wire p22,  // c_OUT
    output wire p23   // b_OUT
);

  // TODO: model this part.
endmodule

`default_nettype wire
