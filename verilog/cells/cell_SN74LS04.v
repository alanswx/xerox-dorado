// cell_SN74LS04 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 5 package position(s) across the sixteen boards.
//
// Hex Inverter (TTL). TtlDict: a,IN,1>a,OUT,2 and so on for six gates.
// On the BaseBoard, c08 makes `MCClk'` from the 6502's `MCClk` -- which is
// what strobes the ROM decoder.

`default_nettype none

module cell_SN74LS04 (
    input  wire p1,  // a_IN0
    output wire p2,  // a_OUT
    input  wire p3,  // b_IN0
    output wire p4,  // b_OUT
    input  wire p5,  // c_IN0
    output wire p6,  // c_OUT
    input  wire p7,  // (no name in EclDict)
    output wire p8,  // d_OUT
    input  wire p9,  // d_IN0
    output wire p10,  // e_OUT
    input  wire p11,  // e_IN0
    output wire p12,  // f_OUT
    input  wire p13,  // f_IN0
    input  wire p14// (no name in EclDict)
);

  assign p2  = ~p1;   assign p4  = ~p3;   assign p6  = ~p5;
  assign p8  = ~p9;   assign p10 = ~p11;  assign p12 = ~p13;

  wire _unused_pins = &{1'b0, p7, p14, 1'b0};   // GND, VCC
endmodule

`default_nettype wire
