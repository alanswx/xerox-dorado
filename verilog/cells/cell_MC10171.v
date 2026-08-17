// cell_MC10171 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 8 package position(s) across the sixteen boards.
//
// Dual Binary to 1-4 Decoder, outputs LOW (MECL 10K, DL122 rev 7 p.373).
//
// "The MC10171 is a binary coded 2 line to dual 4 line decoder with selected
// outputs low. With either E0 or E1 high, the corresponding selected 4 outputs
// are high. The common enable E, when high, forces all outputs high."
//
// Everything is inverted against the '172: the selected output goes LOW, and
// the half enables are ACTIVE LOW -- which EclDict states by priming every one
// of these names where the '172's are bare.
// EclDict gives both parts the same pinout, and the data book's own truth
// table settles the polarities:
//
//   S1 = 7, S2 = 9         the 2-bit code; S2 is the more significant, which
//                          the truth table confirms (A=H,B=L selects output 2)
//   E' = 15                the COMMON enable, high disables everything
//   Q half: enable 14, outputs 13, 12, 11, 10   (index 0..3)
//   R half: enable  2, outputs  6,  5,  4,  3   (index 0..3)
//
// The data book calls the halves Q0x and Q1x, with E0 at pin 14 and E1 at
// pin 2; EclDict's Q half is the E0 one.

`default_nettype none

module cell_MC10171 (
    input  wire p2,  // ER'
    output wire p3,  // R3'
    output wire p4,  // R2'
    output wire p5,  // R1'
    output wire p6,  // R0'
    input  wire p7,  // S1
    input  wire p9,  // S2
    output wire p10,  // Q3'
    output wire p11,  // Q2'
    output wire p12,  // Q1'
    output wire p13,  // Q0'
    input  wire p14,  // EQ'
    input  wire p15// E'
);

  wire [1:0] sel = {p9, p7};                // S2, S1
  wire       en  = ~p15 & ~p14;             // E' and this half's EQ'
  wire       enr = ~p15 & ~p2;              // E' and ER'

  assign p13 = ~(en  & (sel == 2'd0));      // Q0'
  assign p12 = ~(en  & (sel == 2'd1));
  assign p11 = ~(en  & (sel == 2'd2));
  assign p10 = ~(en  & (sel == 2'd3));

  assign p6  = ~(enr & (sel == 2'd0));      // R0'
  assign p5  = ~(enr & (sel == 2'd1));
  assign p4  = ~(enr & (sel == 2'd2));
  assign p3  = ~(enr & (sel == 2'd3));
endmodule

`default_nettype wire
