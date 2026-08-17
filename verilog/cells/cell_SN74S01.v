// cell_SN74S01 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// Quad 2-Input NAND with OPEN-COLLECTOR outputs (TTL), the Schottky sibling
// of the SN74LS01 -- TtlDict groups them together:
//   a,IN,2,3 > a,OUT,1   b,IN,5,6 > b,OUT,4
//   c,IN,8,9 > c,OUT,10  d,IN,11,12 > d,OUT,13
// See cell_SN74LS01 for why open collector matters here and why the cell
// still drives the gate's own function.

`default_nettype none

module cell_SN74S01 (
    output wire p1,  // a_OUT
    input  wire p2,  // a_IN0
    input  wire p3,  // a_IN1
    output wire p4,  // b_OUT
    input  wire p5,  // b_IN0
    input  wire p6,  // b_IN1
    input  wire p7,  // (no name in EclDict)
    input  wire p8,  // c_IN0
    input  wire p9,  // c_IN1
    output wire p10,  // c_OUT
    input  wire p14// (no name in EclDict)
);

  assign p1  = ~(p2 & p3);
  assign p4  = ~(p5 & p6);
  assign p10 = ~(p8 & p9);

  wire _unused_pins = &{1'b0, p7, p14, 1'b0};
endmodule

`default_nettype wire
