// cell_SN74LS139 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// Dual 2-to-4 Decoder (TTL). TtlDict names both halves:
//   b,E',1 > b,S1,2 > b,S2,3 > b,Q0',4 > b,Q1',5 > b,Q2',6 > b,Q3',7
//   c,E',15 > c,S1,14 > c,S2,13 > c,Q0',12 ... c,Q3',9
// Outputs are ACTIVE LOW, and all four go high when the enable is high.
// Only the two outputs the board wires are declared.

`default_nettype none

module cell_SN74LS139 (
    input  wire p1,  // EA'
    input  wire p2,  // AS1
    input  wire p3,  // AS2
    output wire p4,  // A0'
    output wire p5,  // A1'
    input  wire p8,  // (no name in EclDict)
    input  wire p16// (no name in EclDict)
);

  wire [1:0] sel = {p3, p2};              // S2 is the more significant
  wire       en   = ~p1;                  // E'

  assign p4 = ~(en & (sel == 2'd0));      // Q0'
  assign p5 = ~(en & (sel == 2'd1));      // Q1'

  wire _unused_pins = &{1'b0, p8, p16, 1'b0};
endmodule

`default_nettype wire
