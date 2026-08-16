// cell_MC10170 -- 9-2-Bit Parity Generator/Checker
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 41 package position(s).

`default_nettype none

module cell_MC10170 (
    input  wire p3,
    input  wire p4,
    input  wire p5,
    input  wire p6,
    input  wire p7,
    input  wire p9,
    input  wire p10,
    input  wire p11,
    input  wire p12,
    input  wire p13,
    input  wire p14,
    output wire p2,
    output wire p15,
    input  wire p1,
    input  wire p8,
    input  wire p16
);

  // Nine data inputs (3-7, 9-12) plus a two-bit carry-in CI = 13,14.
  // A=2 and B=15 are the parity outputs, complementary.
  wire par = p3 ^ p4 ^ p5 ^ p6 ^ p7 ^ p9 ^ p10 ^ p11 ^ p12 ^ p13 ^ p14;
  assign p2  =  par;
  assign p15 = ~par;


  // Board-wired pins not named above (power and the like)
  wire _unused_pins = &{1'b0, p1, p8, p16, 1'b0};

endmodule

`default_nettype wire
