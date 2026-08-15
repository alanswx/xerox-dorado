// cell_MCS6502 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// TODO: BEHAVIOUR IS NOT MODELLED YET. Cite the part function when
// filling this in, and keep the port list generated -- do not retype
// pin numbers by hand.

`default_nettype none

module cell_MCS6502 (
    input  wire p2,  // RDY
    input  wire p4,  // IRQ'
    input  wire p6,  // NMI'
    input  wire p8,  // VCC1
    output wire p9,  // A00
    output wire p10,  // A01
    output wire p11,  // A02
    output wire p12,  // A03
    output wire p13,  // A04
    output wire p14,  // A05
    output wire p15,  // A06
    output wire p16,  // A07
    output wire p17,  // A08
    output wire p18,  // A09
    output wire p19,  // A10
    output wire p20,  // A11
    input  wire p21,  // GND2
    output wire p22,  // A12
    output wire p23,  // A13
    output wire p24,  // A14
    output wire p25,  // A15
    output wire p26,  // D7
    output wire p27,  // D6
    output wire p28,  // D5
    output wire p29,  // D4
    output wire p30,  // D3
    output wire p31,  // D2
    output wire p32,  // D1
    output wire p33,  // D0
    output wire p34,  // R/W
    input  wire p37,  // CK2i
    input  wire p38,  // SO
    output wire p39,  // CK2o
    input  wire p40// RESET'
);

  // TODO: model this part.
endmodule

`default_nettype wire
