// cell_MC10166 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 6 package position(s) across the sixteen boards.
//
// 5-Bit Magnitude Comparator (MECL 10K, data book DL122 rev 7 p.363).
//
// "Two outputs are provided: A < B and A > B. A = B can be obtained by NORing
// the two outputs with an additional gate. A high level on the enable function
// forces both outputs low."
//
//   TRUTH TABLE
//     E   A vs B          A<B   A>B
//     H   X                L     L
//     L   Word A = Word B  L     L
//     L   Word A > Word B  L     H
//     L   Word A < Word B  H     L
//
// EclDict names the pins X/Y where the data book says A/B, and MSB-FIRST as
// PARC does everywhere: its `X0` is pin 9, which the data book's own pin
// assignment calls A4 -- the most significant bit of the five.
//
//   X0..X4 = 9, 12, 13, 6, 5      (data book A4, A3, A2, A1, A0)
//   Y0..Y4 = 10, 11, 14, 7, 4     (data book B4, B3, B2, B1, B0)
//   X>Y = 2, X<Y = 3, E' = 15

`default_nettype none

module cell_MC10166 (
    output wire p2,  // (no name in EclDict)
    output wire p3,  // X<Y
    input  wire p4,  // Y4
    input  wire p5,  // X4
    input  wire p6,  // X3
    input  wire p7,  // Y3
    input  wire p9,  // X0
    input  wire p10,  // Y0
    input  wire p11,  // Y1
    input  wire p12,  // X1
    input  wire p13,  // X2
    input  wire p14,  // Y2
    input  wire p15// E'
);

  wire [4:0] x = {p9,  p12, p13, p6, p5};   // MSB first
  wire [4:0] y = {p10, p11, p14, p7, p4};
  wire       en = ~p15;                     // E high forces both outputs low

  assign p2 = en & (x > y);                 // X>Y
  assign p3 = en & (x < y);                 // X<Y
endmodule

`default_nettype wire
