// cell_MC10113 -- Quad Exclusive OR
//
// Pin numbers and gate grouping: PARC's EclDict.Analyze. Function: Motorola
// MECL Pocket Book (cells/PARTS.md). Used in 55 package position(s).
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

module cell_MC10113 (
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
    input  wire p1,
    input  wire p9
);

  // Quad Exclusive OR Gate, MECL 10K, data book DL122 rev 7 p.269:
  //
  //   "The MC10113 is a quad Exclusive OR gate, with an ENABLE COMMON TO ALL
  //    FOUR GATES. The outputs may be wire-ORed together to perform a 4-bit
  //    comparison function (A = B). The enable is ACTIVE LOW."
  //
  // Pin 9 is that enable, and EclDict says so twice: the pin block names it
  // once, on gate a (`a,e,9`), and the gate summary spells it out --
  //
  //   [G (4 5)>2, (6 7)>3, (10 11)>14, (12 13)>15]
  //   [G 9>(2 3 14 15)]
  //
  // -- which is Tim's common-pin trap exactly. It went unnoticed longer than
  // it should have because sil_check_cells only read a part's FIRST [G] line,
  // and the enable is stated in the second.
  //
  // Disabled forces the output LOW, which is what makes the wire-OR
  // comparison work: any bit that differs pulls the shared net high.
  wire en = ~p9;

  assign p2  = en & (p4  ^ p5);
  assign p3  = en & (p6  ^ p7);
  assign p14 = en & (p10 ^ p11);
  assign p15 = en & (p12 ^ p13);

  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p1, 1'b0};

endmodule

`default_nettype wire
