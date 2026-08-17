// cell_SN74LS151 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// 8-Input Multiplexer (TTL). TtlDict:
//   a,E',7 > a,S4,9 > a,S2,10 > a,S1,11
//   a,D0,4 > a,D1,3 > a,D2,2 > a,D3,1 > a,D4,15 > a,D5,14 > a,D6,13 > a,D7,12
//   a,Y,5 > a,W',6
// With the enable high Y is low and W' high, whatever the data.
//
// The BaseBoard wires only the low four data inputs, so only those are
// declared -- and the select is still three bits.

`default_nettype none

module cell_SN74LS151 (
    input  wire p1,  // D3
    input  wire p2,  // D2
    input  wire p3,  // D1
    input  wire p4,  // D0
    output wire p5,  // Y
    input  wire p7,  // E'
    input  wire p8,  // (no name in EclDict)
    input  wire p9,  // S4
    input  wire p10,  // S2
    input  wire p11,  // S1
    input  wire p16// (no name in EclDict)
);

  wire [2:0] sel = {p9, p10, p11};        // S4, S2, S1
  wire [3:0] d    = {p1, p2, p3, p4};      // D3, D2, D1, D0

  assign p5 = ~p7 & ((sel[2]) ? 1'b0 : d[sel[1:0]]);   // Y

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};
endmodule

`default_nettype wire
