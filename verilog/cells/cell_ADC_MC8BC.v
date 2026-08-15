// cell_ADC_MC8BC -- MECL model for the Xerox Dorado  (part ADC-MC8BC)
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_ADC_MC8BC (
    input  wire p2,  // CTRSEL
    input  wire p5,  // D0
    input  wire p6,  // D1
    input  wire p7,  // D2
    input  wire p8,  // VCC1
    input  wire p9,  // D3
    input  wire p10,  // D4
    input  wire p11,  // D5
    input  wire p12,  // D6
    input  wire p13,  // D7
    output wire p14,  // ANALOG
    input  wire p15,  // REFi
    output wire p16   // REFo
);

  // TODO: model this part.
endmodule

`default_nettype wire
