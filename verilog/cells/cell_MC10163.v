// cell_MC10163 -- MECL Error Detection-Correction Circuit (IBM code)
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 4 package position(s) across the sixteen boards, all on MemD:
// a14/b14 check the two bytes of the word coming in from storage (SinD),
// j12/l12 generate check bits for the two bytes going out (EcSout).
//
// Function: Motorola MC10163, DoradoDocs/datasheets/MC10163.pdf, "IBM CODE":
// five parity outputs over an 8-bit byte,
//
//   P0A = B1 + B2 + B4 + B7        (pin 15)
//   P0B = B0 + B3 + B5 + B6        (pin 2)
//   P1  = B1 + B3 + B5 + B7        (pin 14)
//   P2  = B2 + B3 + B6 + B7        (pin 13)
//   P3  = B4 + B5 + B6 + B7        (pin 3)
//
// where + is exclusive-OR ("using eight MC10163's together with eight
// 12-bit parity checkers (MC10160), single-bit error detection/correction
// and double-bit error detection can be done on a word of 64-bit length").
// Inputs: B0 pin 9, B1 7, B2 6, B3 10, B4 12, B5 4, B6 5, B7 11.
//
// ASSUMED: each output is HIGH for an ODD number of high inputs, the sense
// of the MC10160's odd-parity output. The generate and check sides use the
// same part, so a data-dependent inversion cancels in the syndrome; only a
// stored check word's absolute sense would show it.
//
// Unmodelled until 2026-09-08 (every output 0: check bits generated as 0,
// checked as 0, so the syndrome was always clean).

`default_nettype none

module cell_MC10163 (
    output wire p2,  // P0B
    output wire p3,  // P3
    input  wire p4,  // B5
    input  wire p5,  // B6
    input  wire p6,  // B2
    input  wire p7,  // B1
    input  wire p9,  // B0
    input  wire p10,  // B3
    input  wire p11,  // B7
    input  wire p12,  // B4
    output wire p13,  // P2
    output wire p14,  // P1
    output wire p15// P0A
);

  wire b0 = p9, b1 = p7, b2 = p6, b3 = p10, b4 = p12, b5 = p4, b6 = p5, b7 = p11;

  assign p15 = b1 ^ b2 ^ b4 ^ b7;   // P0A
  assign p2  = b0 ^ b3 ^ b5 ^ b6;   // P0B
  assign p14 = b1 ^ b3 ^ b5 ^ b7;   // P1
  assign p13 = b2 ^ b3 ^ b6 ^ b7;   // P2
  assign p3  = b4 ^ b5 ^ b6 ^ b7;   // P3
endmodule

`default_nettype wire
