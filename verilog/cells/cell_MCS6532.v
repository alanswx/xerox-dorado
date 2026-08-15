// cell_MCS6532 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 5 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MCS6532 (
    input  wire p2,  // A05
    input  wire p3,  // A04
    input  wire p4,  // A03
    input  wire p5,  // A02
    input  wire p6,  // A01
    input  wire p7,  // A00
    output wire p8,  // PA0
    output wire p9,  // PA1
    output wire p10,  // PA2
    output wire p11,  // PA3
    output wire p12,  // PA4
    output wire p13,  // PA5
    output wire p14,  // PA6
    output wire p15,  // PA7
    output wire p16,  // PB7
    output wire p17,  // PB6
    output wire p18,  // PB5
    output wire p19,  // PB4
    input  wire p20,  // VCC1
    output wire p21,  // PB3
    output wire p22,  // PB2
    output wire p23,  // PB1
    output wire p24,  // PB0
    output wire p25,  // IRQ'
    input  wire p26,  // D7
    input  wire p27,  // D6
    input  wire p28,  // D5
    input  wire p29,  // D4
    input  wire p30,  // D3
    input  wire p31,  // D2
    input  wire p32,  // D1
    input  wire p33,  // D0
    input  wire p34,  // RESET'
    input  wire p35,  // R/W
    input  wire p36,  // RS'
    input  wire p37,  // CS'
    input  wire p38,  // CS
    input  wire p39,  // CK2
    input  wire p40// A06
);

  // TODO: model this part.
endmodule

`default_nettype wire
