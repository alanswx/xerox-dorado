// cell_SN74LS00 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// Quad 2-Input NAND (TTL).
// TtlDict groups this pinout as
//   a,IN,1,2 > a,OUT,3   b,IN,4,5 > b,OUT,6
//   c,IN,9,10 > c,OUT,8  d,IN,12,13 > d,OUT,11
// Only the gates the board wires are declared.

`default_nettype none

module cell_SN74LS00 (
    input  wire p1,  // a_IN0
    input  wire p2,  // a_IN1
    output wire p3,  // a_OUT
    input  wire p7,  // (no name in EclDict)
    input  wire p14// (no name in EclDict)
);

  assign p3 = ~(p1 & p2);

  wire _unused_pins = &{1'b0, p7, p14, 1'b0};
endmodule

`default_nettype wire
