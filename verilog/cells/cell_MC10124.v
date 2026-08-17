// cell_MC10124 -- Quad MTTL to MECL Translator
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 41 package position(s).

`default_nettype none

module cell_MC10124 (
    input  wire p5,
    input  wire p6,
    output wire p4,
    output wire p2,
    input  wire p7,
    output wire p3,
    output wire p1,
    input  wire p10,
    output wire p12,
    output wire p15,
    input  wire p11,
    output wire p13,
    output wire p14,
    input  wire p8,
    input  wire p9,
    input  wire p16
);

  // Quad TTL-to-MECL Translator. Data book (DL122 rev 7, MC10124): "When the
  // common strobe input is at the low logic level, it forces all true outputs
  // to a MECL low logic state and all inverting outputs to a MECL high logic
  // state." So each channel is AND(data, strobe), brought out both ways --
  // and pin 6 is that strobe, in all four gates:
  //   [G (5 6)>(2 4), (6 7)>(1 3), (6 10)>(12 15), (6 11)>(13 14)]
  //
  // TWO bugs were here: the strobe was missing entirely, and the two senses
  // were the wrong way round. EclDict gives gate a `OUT,4 > o,2`, and `OUT`
  // is the inverting output.

  // The COMMON pin feeds EVERY gate. EclDict names it once, on the first
  // gate, and the [G] summary spells it out per gate -- this cell used to
  // wire it into the first gate only. Found by Tim, then by
  // tools/sil_check_cells.py across the whole library.

  assign p2  =  (p5  & p6);   assign p4  = ~(p5  & p6);
  assign p1  =  (p7  & p6);   assign p3  = ~(p7  & p6);
  assign p15 =  (p10 & p6);   assign p12 = ~(p10 & p6);
  assign p14 =  (p11 & p6);   assign p13 = ~(p11 & p6);

  wire _unused_pins = &{1'b0, p9, p16, 1'b0};   // VCC, GND

endmodule

`default_nettype wire
