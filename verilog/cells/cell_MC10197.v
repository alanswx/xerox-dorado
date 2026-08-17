// cell_MC10197 -- Hex AND
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

`default_nettype none

module cell_MC10197 (
    input  wire p5,
    input  wire p9,
    output wire p2,
    input  wire p6,
    output wire p3,
    input  wire p7,
    output wire p4,
    input  wire p10,
    output wire p13,
    input  wire p11,
    output wire p14,
    input  wire p12,
    output wire p15
);

  // Hex AND Gate, MECL 10K, data book DL122 rev 7 p.409: "provides a high
  // speed hex AND function with STROBE capability". Pin 9 is that strobe, and
  // it is common to all six gates:
  //
  //   [G 5>2, 6>3, 7>4, 10>13, 11>14, 12>15]
  //   [G 9>(2 3 4 13 14 15)]
  //
  // Only the first gate had it -- Tim's common-pin trap again -- and the pin
  // block is what makes it easy to miss, naming the strobe on gate a
  // (`a,c,9`) and once more at the end (`g,Enable,9`).
  //
  // That trailing `h,IN,5 > h,OUT,2` in the pin block is a REPEAT of gate a,
  // not a seventh gate, and taking it literally had emitted a second
  // `assign p2` -- a multiply-driven net inside the cell, which only went
  // unseen because MULTIDRIVEN is waived for the wired-OR nets.
  assign p2  = p9 & p5;
  assign p3  = p9 & p6;
  assign p4  = p9 & p7;
  assign p13 = p9 & p10;
  assign p14 = p9 & p11;
  assign p15 = p9 & p12;

endmodule

`default_nettype wire
