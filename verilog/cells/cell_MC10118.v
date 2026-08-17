// cell_MC10118 -- Dual 2-Wide 3-Input OR-AND
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 9 package position(s).

`default_nettype none

module cell_MC10118 (
    input  wire p6,
    input  wire p7,
    input  wire p10,
    input  wire p11,
    input  wire p3,
    input  wire p4,
    input  wire p5,
    input  wire p12,
    input  wire p13,
    input  wire p14,
    input  wire p9,
    output wire p2,
    output wire p15
);

  // Dual 2-Wide 3-Input OR-AND (cells/PARTS.md). TWO INDEPENDENT gates that
  // share pin 9, not one gate -- this cell used to compute a single term from
  // every input and hand it to both outputs.
  //
  //   [G (3 4 5 6 7 9)>2, (9 10 11 12 13 14)>15]
  //   EclDict: b,e,3,4,5 > b,IN,6,7 > b,c,9 > b,OUT,2
  //            c,c,9 > c,a,10,11 > c,s,12,13,14 > c,o,15
  //
  // Each gate ANDs two OR groups, and the common pin 9 is the third input of
  // whichever group has only two -- which is what makes both gates 3-input,
  // as the part name says.
  //
  // Polarity by the confirmed rule: `OUT` (pin 2) inverts, `o` (pin 15) does
  // not. NOT verified against a data sheet -- MC10118 is not in DL122.
  assign p2  = ~((p3 | p4 | p5) & (p6 | p7 | p9));
  assign p15 =  ((p9 | p10 | p11) & (p12 | p13 | p14));

endmodule

`default_nettype wire
