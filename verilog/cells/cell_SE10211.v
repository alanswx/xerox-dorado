// cell_SE10211 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 11 package position(s) across the sixteen boards.
//
// High-Speed Dual 3-Input 3-Output NOR (Signetics second source for the
// Motorola MC10211 -- EclDict's alias table groups `MC111, MC211, SE211`, so
// they are one part in three sockets). Body identical to cell_MC10211.
//
// NO POLARITY AMBIGUITY HERE: the part is NOR-only, so all six outputs carry
// the same, inverting sense and the dictionary's role letters do not have to
// be interpreted. See docs/verilog-handoff.md for the parts where they do.

`default_nettype none

module cell_SE10211 (
    output wire p2,  // (no name in EclDict)
    output wire p3,  // (no name in EclDict)
    output wire p4,  // (no name in EclDict)
    input  wire p5,  // (no name in EclDict)
    input  wire p6,  // (no name in EclDict)
    input  wire p7,  // (no name in EclDict)
    input  wire p9,  // (no name in EclDict)
    input  wire p10,  // (no name in EclDict)
    input  wire p11,  // (no name in EclDict)
    output wire p12,  // (no name in EclDict)
    output wire p13,  // (no name in EclDict)
    output wire p14,  // (no name in EclDict)
    input  wire p15,  // (no name in EclDict)
    input  wire p16// (no name in EclDict)
);

  wire a = ~(p5 | p6 | p7);
  wire b = ~(p9 | p10 | p11);
  assign p2 = a; assign p3 = a; assign p4 = a;
  assign p12 = b; assign p13 = b; assign p14 = b;

  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p15, p16, 1'b0};
endmodule

`default_nettype wire
