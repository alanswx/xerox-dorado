// cell_MC10195 -- Hex Inverter/Buffer
//
// Pin numbers and gate grouping: PARC's EclDict.Analyze. Function: Motorola
// MECL Pocket Book (cells/PARTS.md). Used in 47 package position(s).
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

module cell_MC10195 (
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

  // Hex Inverter/Buffer, and it is neither a plain inverter nor a NOR: pin 9
  // SELECTS which. EclDict names that pin twice, `Invert'` and `Buffer`, and
  // the data book's truth table (DL122 rev 7, MC10195) is an XNOR:
  //
  //     A  B  |  Q          A = pin 9, B = the per-gate input
  //     L  L  |  H          A low  -> Q = NOT B   (inverter)
  //     L  H  |  L          A high -> Q = B       (buffer)
  //     H  L  |  L
  //     H  H  |  H
  //
  // [G (5 9)>2, (6 9)>3, (7 9)>4, (9 10)>13, (9 11)>14, (9 12)>15].
  //
  // This cell had pin 9 in the first gate only AND had the function as a NOR.
  assign p2  = ~(p9 ^ p5);
  assign p3  = ~(p9 ^ p6);
  assign p4  = ~(p9 ^ p7);
  assign p13 = ~(p9 ^ p10);
  assign p14 = ~(p9 ^ p11);
  assign p15 = ~(p9 ^ p12);

endmodule

`default_nettype wire
