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
  // POLARITY: NEITHER OUTPUT INVERTS, and the data sheet says so twice.
  // This used to read `OUT` (pin 2) as inverting, by the rule taken from the
  // OR/NOR family -- a rule already recorded as one that DOES NOT GENERALISE.
  //
  //   * The part is titled "DUAL 2-WIDE 3-INPUT OR-AND GATE" (1978 Motorola
  //     MECL Data Book p.62). Its sibling the MC10117 is titled "OR-AND/
  //     OR-AND-INVERT" -- Motorola names the inverting variant, and this is
  //     not it.
  //   * Read by GEOMETRY, the established method here: pins 2 and 15 each end
  //     in a plain emitter-follower wedge with NO bubble. The MC10117 on the
  //     same convention is the control -- it draws an unmistakable open circle
  //     at pins 3 and 14 and a bare wedge at 2 and 15, and EclDict calls pin 3
  //     `OUT` and pin 2 `o`. So `OUT` really does mark the inverting output
  //     THERE; here there is no inverting output to mark, and the OUT/o pair
  //     is only distinguishing two gates that each have ONE output.
  //
  // ProcH g19 is the check: its pin-2 term is (FB=4' + FA=2') & (FB=5' +
  // FC=2' + FA=1'a), which goes low exactly for FA=1/FB=5/FC=2 -- `TIOA<-B`.
  // h20 NORs that with Curr=Next' to make TIOABypass, so non-inverting gives
  // bypass ON during a TIOA<- and OFF otherwise. Inverting gave exactly the
  // opposite in both cases, which left TIOA reloading from B on EVERY
  // instruction and no I/O address surviving to the Output<- that uses it.
  assign p2  = ((p3 | p4 | p5) & (p6 | p7 | p9));
  assign p15 = ((p9 | p10 | p11) & (p12 | p13 | p14));

endmodule

`default_nettype wire
