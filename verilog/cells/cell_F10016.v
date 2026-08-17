// cell_F10016 -- 4-Bit Synchronous Binary Counter with parallel load
//
// Pin numbers and names: PARC's EclDict.Analyze. Used in 228 package
// position(s) across the sixteen Dorado boards.

`default_nettype none

module cell_F10016 (
    input  wire sys_clk,
    input  wire p13,
    input  wire p6,
    input  wire p12,
    input  wire p5,
    input  wire p11,
    input  wire p10,
    input  wire p9,
    input  wire p7,
    output wire p14,
    output wire p15,
    output wire p2,
    output wire p3,
    output wire p4,
    input  wire p1,
    input  wire p8,
    input  wire p16
);

  // C=13 clock, CE'=6 count enable (active low), MR=12 master reset,
  // PE'=5 parallel enable (active low), D0-D3 = 11,10,9,7 load data,
  // H0-H3 = 14,15,2,3 outputs, CO'=4 carry out (active low).
  reg [3:0] q;

  // FPGA: ONE CLOCK, and the ECL clock net becomes an ENABLE.
  //
  // The Dorado clocks this part from a distributed ECL clock net (CLK.ph'
  // and friends, fanned out by the BaseBoard). Taking that literally gives
  // the fabric a gated clock off combinational logic -- 1,201 packages across
  // the machine do it -- which no FPGA can route. So every flip-flop here
  // runs on `sys_clk` and transfers on the sys_clk edge FOLLOWING an edge of
  // the modelled clock net. That is the usual oversampling transform, and it
  // matches the part as long as sys_clk is faster than the clock net, which
  // dorado_machine guarantees by dividing.
  //
  // Asynchronous inputs (MR, S/R, CL') are LEVEL-tested every sys_clk edge --
  // the same treatment, and it keeps them out of the fabric's reset network.
  reg ck_d;
  always @(posedge sys_clk) ck_d <= p13;
  wire ck_en = p13 & ~ck_d;

  // CO' IS REGISTERED, and that is the dictionary's own statement. EclDict
  // gives this part an `[FF ...]` entry and NO `[G ...]` entry at all, and pin
  // 4 appears in the FF's output list beside the four Q pins:
  //
  //   [FF 7 {2.2 1.1}>3, 9>2, 10>15, 11>14, (7 9 10 11)>4,
  //        (5 6){2.8 .6}>(2 3 14 15 4) : CLK 13 (1 5.5) RS 12 ... ]
  //
  // The dictionary states combinational paths under [G] and clocked ones
  // under [FF]; pin 4 is only ever in the second. It also says which inputs
  // reach it -- the four D pins and both enables -- which is a carry computed
  // from the state the counter is ENTERING and then clocked out, the usual
  // ECL arrangement so a cascaded stage sees its carry in time.
  //
  // Modelling it as a gate on `q` and `CE'` instead put a combinational path
  // from this package's CE' straight to its CO', and that closed real loops:
  // on DispM and DispY an MC10195 feeds `KillDWTWakeup` back to
  // `StopWakeCount`, and on MemD `ChkLastPhOrIdle` returns to d14 the same
  // way. Three of the machine's seven remaining feedback loops were this one
  // cell.
  reg co_n;
  wire [3:0] q_next = !p5    ? {p7, p9, p10, p11}   // PE' load
                    : !p6    ? q + 4'd1            // CE' count
                             : q;
  always @(posedge sys_clk) begin
    if (p12) begin                                 // MR
      q    <= 4'd0;
      co_n <= 1'b1;
    end else if (ck_en) begin
      q    <= q_next;
      co_n <= ~(&q_next & ~p6);
    end
  end
  assign {p3, p2, p15, p14} = q;
  assign p4 = co_n;                            // CO' active low on terminal count


  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p1, p8, p16, 1'b0};

endmodule

`default_nettype wire
