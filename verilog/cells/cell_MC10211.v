// cell_MC10211 -- High-Speed Dual 3-Input 3-Output NOR
//
// Pins: PARC's EclDict/TtlDict.Analyze. Used in 29 package position(s).

`default_nettype none

module cell_MC10211 (
    input  wire p5,
    input  wire p6,
    input  wire p7,
    output wire p2,
    output wire p3,
    output wire p4,
    input  wire p9,
    input  wire p10,
    input  wire p11,
    output wire p12,
    output wire p13,
    output wire p14,
    input  wire p15,
    input  wire p16
);

  // Same pinout as MC10210 (they share one dictionary block); this is the
  // NOR of the pair, so the three outputs per gate are inverted.
  wire a = ~(p5 | p6 | p7);
  wire b = ~(p9 | p10 | p11);
  assign p2 = a; assign p3 = a; assign p4 = a;
  assign p12 = b; assign p13 = b; assign p14 = b;


  wire _unused_pins = &{1'b0, p15, p16, 1'b0};

endmodule

`default_nettype wire
