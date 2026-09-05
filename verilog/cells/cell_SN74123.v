// cell_SN74123 -- dual retriggerable monostable multivibrator (TTL).
//
// Pin numbers and signal names from PARC's EclDict.Analyze; function from the
// TI data sheet. Each half: A' (active-low trigger), B (active-high trigger),
// CLR' (clear), Q; the external R and C on pins 14/15 and 6/7 set the width.
//
// Used in 2 package positions, both on DskEth: a05 and a03, one half per
// disk unit. Each half is the RELOAD of that unit's subsector counter: the
// unit's sector pulse (TriconD06.sil+n) triggers it and clocks the LS169
// (a01/a02), whose LD' is this Q, and whose terminal count RC' -- the divided
// "sector" pulse the controller wakes on -- is this CLR'. So at terminal
// count the clear drops Q, the next pulse LOADS the subsector count, the
// clear is released, and the following pulses count. The width only has to
// outlast the sector pulse itself (each pulse re-triggers before its own
// clocking edge); the real R and C are not on the sheets we have, so it is a
// parameter, 50 us, scaled by SYSPER like cell_K1115A.
//
// This was a SKELETON until 2026-09-04: Q undriven read 0, LD' was held
// asserted, and the controller raised ONE sector wakeup in seven revolutions
// against a drive giving 117 pulses each.
//
// Data-sheet behaviour modelled: a trigger is A' falling with B high, B
// rising with A' low, or CLR' rising with A' low and B high; each trigger
// (re)starts the timing; CLR' low forces Q low and blocks triggering.
`default_nettype none
module cell_SN74123 #(
    parameter integer TW_NS  = 50000,
    parameter integer SYSPER = 16
) (
    input  wire sys_clk,
    input  wire p1,   // a'  A input, half 1
    input  wire p2,   // b   B input, half 1
    input  wire p3,   // CL' clear, half 1
    output wire p5,   // Q   half 2
    input  wire p6,   // C   (timing, half 2)
    input  wire p7,   // R
    input  wire p8,   // GND
    input  wire p9,   // a'  half 2
    input  wire p10,  // b   half 2
    input  wire p11,  // CL' half 2
    output wire p13,  // Q   half 1
    input  wire p14,  // C   (timing, half 1)
    input  wire p15,  // R
    input  wire p16   // VCC
);
  // sys_clk is 30 ns / SYSPER, so a width of TW_NS is TW_NS * SYSPER / 30.
  localparam integer TW = (TW_NS * SYSPER) / 30;
  reg a1_d = 1'b1, b1_d = 1'b0, c1_d = 1'b1, a2_d = 1'b1, b2_d = 1'b0, c2_d = 1'b1;
  integer t1 = 0, t2 = 0;
  wire trig1 = (a1_d && !p1 && p2) || (!b1_d && p2 && !p1) || (!c1_d && p3 && !p1 && p2);
  wire trig2 = (a2_d && !p9 && p10) || (!b2_d && p10 && !p9) || (!c2_d && p11 && !p9 && p10);
  always @(posedge sys_clk) begin
    a1_d <= p1; b1_d <= p2; c1_d <= p3; a2_d <= p9; b2_d <= p10; c2_d <= p11;
    if (!p3)        t1 <= 0;
    else if (trig1) t1 <= TW;
    else if (t1 > 0) t1 <= t1 - 1;
    if (!p11)       t2 <= 0;
    else if (trig2) t2 <= TW;
    else if (t2 > 0) t2 <= t2 - 1;
  end
  assign p13 = (t1 > 0);
  assign p5  = (t2 > 0);
endmodule
`default_nettype wire
