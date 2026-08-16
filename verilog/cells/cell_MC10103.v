// cell_MC10103 -- Quad 2-Input OR
//
// Pin numbers and gate grouping: PARC's EclDict.Analyze. Function: Motorola
// MECL Pocket Book (cells/PARTS.md). Used in 51 package position(s).
//
// POLARITY RULE, stated because the dictionary does not encode it: the
// dictionary marks which PINS are outputs, not their sense -- MC10210 and
// MC10211 share one pin block and differ only in function. So polarity comes
// from the PART NAME here. Where a gate brings out BOTH outputs, the pin the
// dictionary marks `OUT` carries the inverting (NOR) sense and the pin marked
// `o` the non-inverting (OR) sense -- read off MC10101 (OR/NOR) against
// MC10102 (NOR-only), which share pin 2.
//
// Unused MECL inputs have on-chip pulldowns and read LOW, which is why the
// boards leave so many unconnected; Verilator drives an unconnected input to
// 0, matching.

// PIN 9 IS THE ODD ONE, and it was wrong here until 2026-08-16. The MECL
// Pocket Book says of this part: "One of the gates has both OR and NOR
// outputs", and EclDict marks that gate `c,IN,12,13 > c,OUT,9 > c,o,15` --
// two different role letters, so two different senses. This cell gave pins 9
// and 15 the same expression, which cannot be right whichever way round they
// go. Pin 9 is taken as the NOR by the rule documented in
// docs/verilog-handoff.md, which the mirror-image MC10102 supports.

`default_nettype none

module cell_MC10103 (
    input  wire p4,
    input  wire p5,
    output wire p2,
    input  wire p6,
    input  wire p7,
    output wire p3,
    input  wire p12,
    input  wire p13,
    output wire p9,
    output wire p15,
    input  wire p10,
    input  wire p11,
    output wire p14,
    input  wire p1,
    input  wire p16
);

  assign p2 = (p4 | p5);
  assign p3 = (p6 | p7);
  assign p9 = ~(p12 | p13);   // see the note above: this gate has BOTH senses
  assign p15 = (p12 | p13);
  assign p14 = (p10 | p11);

  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p1, p16, 1'b0};

endmodule

`default_nettype wire
