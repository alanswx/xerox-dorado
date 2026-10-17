// cell_MC1674 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards.
//
// Triple 2-input EXCLUSIVE-NOR (DoradoDocs/datasheets/MC1674.pdf; the MC1672
// is the exclusive-OR twin). EclDict: [G (3 5)>2, (6 13)>14, (7 11)>15].
//
// ONE PACKAGE IN THE MACHINE, and it is on the Md-hold path. MemX f24: gate a
// makes `CacheRef` from `CacheRef'` (pin 5 is unwired, an open MECL input
// reads 0, so the gate is an inverter), gate b `MemX01.sil+4` from
// `CacheRef'` and e23's output, gate c `AcanhaveMap'` from `MapFree'` and the
// constant TrueAC. Unmodelled, `CacheRef` sat at 0 for ever, so MemX g24's
// OR of it into `DdataGood'` never said "not good", MemC's Md-hold request
// dMDhold' never asserted in any run (HOLDSRC: MDhold 0), and a <-Md after
// an intervening Store read the store's word out of the Md file (HM p.39:
// it must be held until the store completes). Found 2026-09-05.

`default_nettype none

module cell_MC1674 (
    output wire p2,  // a_OUT
    input  wire p3,  // a_IN0
    input  wire p6,  // b_IN1
    input  wire p7,  // c_IN1
    input  wire p11,  // c_IN0
    input  wire p13,  // b_IN0
    output wire p14,  // b_OUT
    output wire p15// c_OUT
);

  // Gate a's second input (pin 5) is not wired on the one board that carries
  // this part, and the port list is generated from the wire lists, so it is
  // a constant 0 here: an open MECL input sits at VEE.
  assign p2  = ~(p3 ^ 1'b0);   // a: CacheRef  = ~CacheRef'
  assign p14 = ~(p13 ^ p6);    // b
  // GATE c: AcanhaveMap' = ~(MapFree' ^ TrueAC) = MapFree' (TrueAC is g04's
  // constant 1) -- the address section is denied the map while the map is
  // busy. This was PARKED at 0 from 2026-09-06 to 2026-09-08 because with it
  // Initial's map initialisation deadlocked and memrun-test's Store never
  // dirtied its line. Both were measured on a machine whose BR+Mar adders
  // were not wired (see docs/verilog-handoff.md, 2026-09-08): every VA was
  // zero, so every reference hit one line and the map sequencer was being
  // asked things no real reference asks. With the adders wired and the gate
  // live, memrun-test's dirty-victim write goes all the way to a DRAM write
  // (MemWEa 2, WriteInMem' 256 samples) for the first time; parked, the
  // victim's map walk is restarted by the next reference at MapState 001 and
  // no storage cycle ever starts.
  assign p15 = ~(p11 ^ p7);    // c: AcanhaveMap' = ~(MapFree' ^ TrueAC)
endmodule

`default_nettype wire
