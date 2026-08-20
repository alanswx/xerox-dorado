// cell_MC10119 -- 4-Wide 4-3-3-3-Input OR-AND
//
// Pins: PARC's EclDict.Analyze. Function: the logic diagram in
// DoradoDocs/datasheets/Motorola_MECL_DataBook_1978.pdf p.3-16.
// Used in 10 package position(s).
//
// PIN 10 IS SHARED BY TWO OR GROUPS -- the same trap as MC10121, and the
// dictionary says so if you count: it gives pin 10 the role `c' (common)
// while the four groups it names hold only 4+2+3+3 = 12 slots, and the part
// is a 4-3-3-3, which is 13. The thirteenth is pin 10 counted twice. The
// data book's logic diagram draws the junction dot on pin 10 fanning into
// both the (7,9) and the (11,12) gate, and its electrical table lists pin 10's
// input current on its own line -- 310 uA against 245 uA for every other
// input -- because it drives two gates.
//
// Modelling it as a standalone AND term instead (`... & p10') is not a
// near-miss: it forces the whole output low whenever pin 10 is low. On ProcH
// b17/c17 that pin is FA=0', so every microinstruction with FA=0 -- PARC's own
// Nop among them -- drove QshiftL'/QshiftR' asserted, which is PARALLEL ENTRY
// on the MC10141, and Q was reloaded from a dead B bus every cycle.

`default_nettype none

module cell_MC10119 (
    input  wire p3,
    input  wire p4,
    input  wire p5,
    input  wire p6,
    input  wire p7,
    input  wire p9,
    input  wire p11,
    input  wire p12,
    input  wire p13,
    input  wire p14,
    input  wire p15,
    input  wire p10,
    output wire p2
);

  // OR each input group, then AND the groups (the part's own shape).
  assign p2 = (p3 | p4  | p5  | p6 ) & (p7  | p9  | p10) &
              (p10 | p11 | p12)       & (p13 | p14 | p15);


endmodule

`default_nettype wire
