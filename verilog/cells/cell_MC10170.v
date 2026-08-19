// cell_MC10170 -- 9-2-Bit Parity Generator/Checker
//
// Pins: PARC's EclDict.Analyze. Function: the Motorola data sheet, 1978 MECL
// book p.123 (DoradoDocs/datasheets/Motorola_MECL_DataBook_1978.pdf):
//
//   "The MC10170/MC10570 is an 11-bit parity circuit, which is segmented into
//    9 data bits and 2 control bits. Output A generates odd parity on 9 bits
//    ... The Control Inputs can be used to expand parity to larger numbers of
//    bits with minimal delay or can be used to generate even parity."
//
//        Sum of D inputs at High Level | Odd Parity A | Even Parity B
//                    Even              |     Low      |     High
//                    Odd               |     High     |     Low
//
// THE CONTROL INPUTS REACH PIN 15 ONLY, and this cell used to XOR them into
// BOTH outputs. EclDict says so in as many words --
//
//     [G (3 4 5 6 7 9 10 11 12)>2]  [G (3 4 5 6 7 9 10 11 12)>15]  [G (13 14)>15]
//
// -- and the cascade is the whole point of the part: ContB j21 feeds j20's
// pin 13 to make an 18-bit check out of two 9-bit ones, and ContA e19 feeds
// e18's pin 14 the same way. With the controls folded into pin 2 as well, the
// LOCAL parity was wrong wherever a package cascades, and eleven of PARC's
// thirteen IRTable microinstructions failed the IM right-half parity check
// that the real machine passes.
//
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

  // Nine data inputs (3-7, 9-12); the two control inputs are 13 and 14.
  wire par9 = p3 ^ p4 ^ p5 ^ p6 ^ p7 ^ p9 ^ p10 ^ p11 ^ p12;
  assign p2  = par9;                       // A: ODD parity, the nine data bits
  assign p15 = ~(par9 ^ p13 ^ p14);        // B: EVEN parity, with the controls


  // Board-wired pins not named above (power and the like)
  wire _unused_pins = &{1'b0, p1, p8, p16, 1'b0};

endmodule

`default_nettype wire
