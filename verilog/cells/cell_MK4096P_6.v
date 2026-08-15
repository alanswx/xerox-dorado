// cell_MK4096P_6 -- MECL model for the Xerox Dorado  (part MK4096P-6)
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 144 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MK4096P_6 (
    input  wire p2,  // DIN
    input  wire p3,  // WE'
    input  wire p4,  // RAS'
    input  wire p5,  // A0
    input  wire p6,  // A2
    input  wire p7,  // A1
    input  wire p10,  // A5
    input  wire p11,  // A4
    input  wire p12,  // A3
    input  wire p13,  // CS'
    output wire p14,  // DOUT
    input  wire p15// CAS'
);

  // TODO: model this part.
endmodule

`default_nettype wire
