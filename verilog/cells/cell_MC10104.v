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
  assign p9 = (p12 & p13);

  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p16, 1'b0};

endmodule

`default_nettype wire
