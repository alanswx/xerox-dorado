// cell_MC1672 -- MECL III Triple 2-Input Exclusive-OR Gate
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 3 package position(s) across the sixteen boards: MemC h17, h18
// and IFU d07.
//
// Function: Motorola MC1672, DoradoDocs/datasheets/MC1672.pdf -- the
// exclusive-OR twin of the MC1674 (cell_MC1674, exclusive-NOR), same
// pinout: gate a (3, 5) -> 2, gate b (13, 6) -> 14, gate c (11, 7) -> 15.
//
// What the two MemC packages compute (from the wire list): h17 gate a
// `HitOrEc` = Hia ^ EcHasA', gates b and c wired-OR into MemC04.sil+5 =
// (Col.0' ^ Victim.0') | (Col.1' ^ Victim.1'), "the hit column is not the
// victim column"; h18 the same against NextV (the next victim) and preMCSb.
// Unmodelled, every one of those read 0 -- "hit column equals victim" on
// every reference -- which is the cache's victim-pointer maintenance.
// Modelled 2026-09-08.
//
// A gate whose second input is unwired on a board (IFU d07 gate b, pin 6)
// sees a constant 0 there -- an open MECL input sits at VEE -- and the
// generated port list omits the pin, so the XOR is a buffer.

`default_nettype none

module cell_MC1672 (
    output wire p2,  // a_OUT
    input  wire p3,  // a_IN0
    input  wire p5,  // a_IN1
    input  wire p6,  // b_IN1
    input  wire p7,  // c_IN1
    input  wire p11,  // c_IN0
    input  wire p13,  // b_IN0
    output wire p14,  // b_OUT
    output wire p15// c_OUT
);

  assign p2  = p3 ^ p5;    // a
  assign p14 = p13 ^ p6;   // b
  assign p15 = p11 ^ p7;   // c
endmodule

`default_nettype wire
