// cell_MC10101 -- Quad OR/NOR
//
// Pin numbers and gate grouping: PARC's EclDict.Analyze. Function: Motorola
// MECL Pocket Book (cells/PARTS.md). Used in 54 package position(s).
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

module cell_MC10101 (
    input  wire p4,
    input  wire p12,
    output wire p2,
    output wire p5,
    input  wire p7,
    output wire p3,
    output wire p6,
    input  wire p10,
    output wire p14,
    output wire p11,
    input  wire p13,
    output wire p15,
    output wire p9
);

  assign p2 = ~(p4 | p12);
  assign p5 = (p4 | p12);
  assign p3 = ~(p7);
  assign p6 = (p7);
  assign p14 = ~(p10);
  assign p11 = (p10);
  assign p15 = ~(p13);
  assign p9 = (p13);

endmodule

`default_nettype wire
