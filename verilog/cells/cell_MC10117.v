// cell_MC10117 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 28 package position(s) across the sixteen boards.
//
// Dual 2-Wide 2-3-Input OR-AND/OR-AND-INVERT (MECL 10K). Datasheet:
// DoradoDocs/datasheets/MC10117.pdf -- "designed for use in data control,
// such as digital multiplexing or data distribution. Pin 9 is common to both
// gates."
//
// Each half ANDs two OR groups. Pin 9 joins the "2" group of BOTH halves,
// which is what makes it 2-3-input: one group of two, one of three.
//
//   AOUT (15 in the dictionary's terms, pin 2 here) = (A1IN) AND (A2IN)
//        A1IN = pins 4,5      A2IN = pins 6,7 and the common 9
//   BOUT = (B1IN) AND (B2IN)
//        B1IN = pins 12,13    B2IN = pins 10,11 and the common 9
//
// THIS PART SETTLED THE `u`/`v` ROLE LETTERS, which the OR/NOR rule does not
// reach. EclDict gives the second half `c,u,14 > c,v,15`, and the datasheet
// labels pin 14 B-bar-OUT and pin 15 BOUT -- so `u` is the INVERTING output
// and `v` the non-inverting, exactly as `OUT`/`o` are on the first half
// (`b,OUT,3 > b,o,2`, and the sheet has pin 3 barred, pin 2 not).

`default_nettype none

module cell_MC10117 (
    output wire p2,  // a_OUTN
    output wire p3,  // a_OUT
    input  wire p4,  // a
    input  wire p5,  // a
    input  wire p6,  // a_IN0
    input  wire p7,  // a_IN1
    input  wire p9,  // a_C
    input  wire p10,  // e
    input  wire p11,  // e
    input  wire p12,  // s
    input  wire p13,  // s
    output wire p14,  // u
    output wire p15// v
);

  wire a = (p4  | p5) & (p6  | p7  | p9);   // A1IN AND A2IN
  wire b = (p12 | p13) & (p10 | p11 | p9);   // B1IN AND B2IN

  assign p2  =  a;   // AOUT      (role o)
  assign p3  = ~a;   // A-bar-OUT (role OUT)
  assign p15 =  b;   // BOUT      (role v)
  assign p14 = ~b;   // B-bar-OUT (role u)
endmodule

`default_nettype wire
