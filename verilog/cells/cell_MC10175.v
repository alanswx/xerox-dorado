// cell_MC10175 -- Quint Latch
//
// Pins: PARC's EclDict.Analyze. Function: MECL Pocket Book (cells/PARTS.md).
// Used in 18 package position(s).

`default_nettype none

module cell_MC10175 (
    input  wire sys_clk,
    input  wire p10,
    input  wire p12,
    input  wire p13,
    input  wire p9,
    input  wire p5,
    input  wire p6,
    input  wire p7,
    input  wire p11,
    output wire p14,
    output wire p15,
    output wire p2,
    output wire p3,
    output wire p4
);

  // D0-D4 = 10,12,13,9,5; Q0-Q4 = 14,15,2,3,4; CC = 6,7 clock (two pins,
  // wire-ORed); MR=11 master reset.
  wire clk = p6 | p7;
  reg [4:0] q;
  // A TRANSPARENT LATCH, NOT A FLIP-FLOP. The MECL data book (MC10175 Quint
  // Latch): "Data is transferred on the negative edge of the clock and latched
  // on the positive edge ... Any change on the data input will be reflected at
  // the outputs while the clock is low. The outputs are latched on the
  // positive transition of the clock. While the clock is in the high state, a
  // change in the information present at the data inputs will not affect the
  // output information. The reset input is enabled only when the clock is in
  // the high state." PARC's sheets say the same in their own words -- MemD01:
  // "175 follows input when clock is low".
  //
  // This cell used to be a positive-edge flip-flop: it captured D one sys_clk
  // AFTER the latching edge and showed nothing before it. MemX g01/f01 hold
  // MapBuf on MapbufLd' (low through the first half-cycle), and MemX f04 loads
  // the MCR's B side (NoWake, ReportSE', WakeOnCL) from MapBuf at the same
  // Clk1' edge on which MapbufLd' rises; with the flip-flop model f04 read the
  // OLD MapBuf, so LoadMCR[T,T] with mcr.noWake never disabled fault-task
  // wakeups, and InitMap's first flush woke the fault task into Initial's
  // leftover Block (measured 2026-09-08, MCRWIN at IM 0x213). As a latch the
  // data is at Q throughout the low phase, before the edge, as the part has it.
  //
  // Sampled on sys_clk, as every clocked element here is: while the clock net
  // is low Q follows D one sys_clk behind; the value present at the rising
  // edge is what stays. Synthesises as a flop with enable, no latch primitive.
  always @(posedge sys_clk) begin
    if (clk) begin
      if (p11) q <= 5'd0;                          // MR, only while the clock is high
    end else
      q <= {p5, p9, p13, p12, p10};                 // transparent while low
  end
  assign {p4, p3, p2, p15, p14} = q;


endmodule

`default_nettype wire
