// cell_MC10104 -- Quad 2-Input AND
//
// Pin numbers and gate grouping: PARC's EclDict.Analyze. Function: Motorola
// MECL Pocket Book (cells/PARTS.md). Used in 46 package position(s).
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

`default_nettype none

module cell_MC10104 (
    input  wire p4,
    input  wire p5,
    output wire p2,
    input  wire p6,
    input  wire p7,
    output wire p3,
    input  wire p10,
    input  wire p11,
    output wire p14,
    input  wire p12,
    input  wire p13,
    output wire p15,
    output wire p9,
    input  wire p16
);

  assign p2 = (p4 & p5);
  assign p3 = (p6 & p7);
  assign p14 = (p10 & p11);
  assign p15 = (p12 & p13);
  // Gate d brings out BOTH senses, and this gave them the same expression --
  // the same fault the handoff records for cell_MC10103, which "gave one
  // gate's two outputs the same expression, which cannot be right for a gate
  // the datasheet says has both". EclDict marks p15 `OUT` and p9 `o`.
  //
  // WHICH way round is not the blanket "OUT is inverting" rule: MC10102 and
  // MC10104 SHARE one dictionary pin block and differ only by part name, so
  // the name decides the function and `o` is its complement -- MC10102 is the
  // NOR (p15 = NOR, p9 = OR) and MC10104 is the AND (p15 = AND, p9 = NAND).
  // PARC's usage agrees: of the eleven packages that use both, eight name
  // them as a complementary pair and seven put the PRIMED name on p9
  // (DispM c12, IFU d15, MemX g13/i20/j17/k19/l17). The eighth, MemX l18,
  // is reversed -- a gate fed already-inverted inputs, named for its
  // function rather than its pin sense, which is the caveat the handoff
  // already records for MC10212.
  assign p9 = ~(p12 & p13);

  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p16, 1'b0};

endmodule

`default_nettype wire
