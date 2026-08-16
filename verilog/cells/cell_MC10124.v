// cell_MC10124 -- Quad MTTL to MECL Translator
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 41 package position(s).

`default_nettype none

module cell_MC10124 (
    input  wire p5,
    input  wire p6,
    output wire p4,
    output wire p2,
    input  wire p7,
    output wire p3,
    output wire p1,
    input  wire p10,
    output wire p12,
    output wire p15,
    input  wire p11,
    output wire p13,
    output wire p14,
    input  wire p8,
    input  wire p9,
    input  wire p16
);

  // The mirror of MC10125. A translator changes LEVELS, which do not exist
  // in RTL, so each gate is a buffer plus its complement.
  assign p4  =  p5;  assign p2  = ~p5;
  assign p3  =  p7;  assign p1  = ~p7;
  assign p12 =  p10; assign p15 = ~p10;
  assign p13 =  p11; assign p14 = ~p11;
  wire _unused_ref = &{1'b0, p6, 1'b0};


  // Board-wired pins not named above (power and the like)
  wire _unused_pins = &{1'b0, p8, p9, p16, 1'b0};

endmodule

`default_nettype wire
