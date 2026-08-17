// cell_MC10100 -- Quad 2-Input NOR with Strobe
//
// Pin numbers and gate grouping: PARC's EclDict.Analyze. Function: Motorola
// MECL Pocket Book (cells/PARTS.md). Used in 25 package position(s).
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

module cell_MC10100 (
    input  wire p4,
    input  wire p5,
    input  wire p9,
    output wire p2,
    input  wire p6,
    input  wire p7,
    output wire p3,
    input  wire p10,
    input  wire p11,
    output wire p14,
    input  wire p12,
    input  wire p13,
    output wire p15
);

  // Quad 2-Input NOR with STROBE (cells/PARTS.md). Pin 9 is the strobe and
  // it is an input to all four NORs: [G (4 5 9)>(2), (6 7 9)>(3),
  // (10 11 9)>(14), (12 13 9)>(15)]. A high strobe forces every output low,
  // which is what an extra OR input into a NOR does.

  // The COMMON pin feeds EVERY gate. EclDict names it once, on the first
  // gate, and the [G] summary spells it out per gate -- this cell used to
  // wire it into the first gate only. Found by Tim, then by
  // tools/sil_check_cells.py across the whole library.

  assign p2  = ~(p4  | p5  | p9);
  assign p3  = ~(p6  | p7  | p9);
  assign p14 = ~(p10 | p11 | p9);
  assign p15 = ~(p12 | p13 | p9);

endmodule

`default_nettype wire
