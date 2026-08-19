// cell_MC10121 -- 4-Wide OR-AND/OR-AND-INVERT
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 44 package position(s).

`default_nettype none

module cell_MC10121 (
    input  wire p7,
    input  wire p9,
    input  wire p11,
    input  wire p12,
    input  wire p4,
    input  wire p5,
    input  wire p6,
    input  wire p13,
    input  wire p14,
    input  wire p15,
    input  wire p10,
    output wire p2,
    output wire p3
);

  // Four 3-input OR groups feeding one AND -- the part's own shape.
  //
  // PIN 10 IS SHARED BY TWO GROUPS, which is Tim's common-pin class again.
  // The data sheet's DIP pin assignment labels it `A2IN, A3IN` (the same way
  // MC10117 labels its pin 9 `A2IN, B2IN`), and EclDict says it the way it
  // always says a common input -- once, under role `c`:
  //
  //   a,IN,7,9 > a,a,11,12 > a,c,10 > a,e,4,5,6 > a,s,13,14,15 > a,OUT,2 > a,o,3
  //
  // so the groups are {4,5,6}, {7,9,10}, {10,11,12}, {13,14,15}. This used to
  // AND pin 10 in as a term of its own, which made the gate false whenever
  // that one input was low -- on ContB l03 that is `IMRHPE'`, and l03 gates
  // the MIR clock.
  wire y = (p4 | p5 | p6) & (p7 | p9 | p10) & (p10 | p11 | p12) & (p13 | p14 | p15);

  // OR-AND on pin 2, OR-AND-INVERT on pin 3 -- and note this is the OPPOSITE
  // of the handoff's "role `OUT` is the INVERTING output" rule, which puts
  // `OUT` on pin 2 here. The rule holds for the OR/NOR family it was derived
  // from and does not generalise; see docs/verilog-handoff.md.
  //
  // Settled from the data book's LOGIC DIAGRAM, by coordinates rather than by
  // eye, because the pin labels' overbars are in a font the PDF does not
  // embed and render as empty boxes. `pdftotext -bbox` puts the label `2` at
  // y=322.8..330.2 pt and `3` at y=332.4..339.8, and rasterising that strip
  // puts the PLAIN output arrow at y=324.9 and the BUBBLED one at y=335.2.
  // Two independent parts were checked the same way; MC10104's gate d has the
  // bubble on pin 9, which is its `o` pin.
  //
  // PARC's own net naming says the reverse and is NOT evidence: nine packages
  // name pin 2 with the primed name, but nine packages also prime pin 9 of an
  // MC10102, whose `OUT` pin 15 is certainly the NOR. A net is named for the
  // sense the designer treats as primary, not for which pin inverts -- the
  // caveat the handoff already records for MC10212.
  assign p2  =  y;
  assign p3 = ~y;


endmodule

`default_nettype wire
