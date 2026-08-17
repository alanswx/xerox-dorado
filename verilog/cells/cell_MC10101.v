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

  // Quad OR/NOR with a COMMON INPUT on pin 12 -- the data sheet labels that
  // pin exactly that (DoradoDocs/datasheets/MC10101.pdf), and [G] puts it in
  // every gate: (4 12)>(2 5), (7 12)>(3 6), (10 12)>(11 14), (12 13)>(9 15).
  //
  // Polarity by the confirmed rule: role `OUT` is the inverting output, `o`
  // the non-inverting one. The sheet's own labels agree -- pin 2 is
  // A-bar-OUT, pin 5 is AOUT.

  // The COMMON pin feeds EVERY gate. EclDict names it once, on the first
  // gate, and the [G] summary spells it out per gate -- this cell used to
  // wire it into the first gate only. Found by Tim, then by
  // tools/sil_check_cells.py across the whole library.

  assign p2  = ~(p4  | p12);   assign p5  = (p4  | p12);
  assign p3  = ~(p7  | p12);   assign p6  = (p7  | p12);
  assign p14 = ~(p10 | p12);   assign p11 = (p10 | p12);
  assign p15 = ~(p13 | p12);   assign p9  = (p13 | p12);

endmodule

`default_nettype wire
