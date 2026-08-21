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

  always @(posedge sys_clk) begin
    if (p12)            q <= 4'd0;                 // MR
    else if (ck_en) begin
      if (!p5)          q <= {p7, p9, p10, p11};   // PE' load
      else if (!p6)     q <= q + 4'd1;             // CE' count
    end
  end
  assign {p3, p2, p15, p14} = q;

  // PIN 4 IS `TC`, TERMINAL COUNT, AND IT IS NOT GATED BY `CE`. The Fairchild
  // data sheet (DoradoDocs/datasheets/F10016.pdf) names the pins outright:
  //
  //     CE   Count Enable (LOW to Count)
  //     TC   Terminal Count (10010, LOW at HLLH; 10016 LOW at HHHH)
  //
  // -- TC is defined by the Q state alone. The part carries its own expansion
  // logic ("INTERNAL COUNT ENABLE - FOR HIGHEST SPEED EXPANSION", and "up to
  // nine devices can be cascaded"), so a cascade works by feeding TC into the
  // next stage's CE: TC goes LOW at HHHH, CE is LOW to count, so the upper
  // stage counts exactly when the lower one is at terminal. Eight such
  // cascades exist in the machine -- IFU has a three-stage chain, plus ProcH,
  // MemX and DispM.
  //
  // GATING IT WITH CE WAS THE LONG-STANDING CONVERGENCE BUG. MemD wires this
  // counter's TC (d14 pin 4) through an MC10195 whose select pin is
  // unconnected -- so an inverter -- straight back to the same counter's own
  // CE (d14 pin 6): a count-and-stop, stable, and exactly what the data sheet
  // supports. With TC gated by CE that reduces at terminal count to
  // `TC = ~TC`, and the whole machine failed to converge. `machine-test` had
  // been red for that reason, and the seven-board `mem-test +define+MEM_RUN`
  // reproduces it in a smaller machine.
  //
  assign p4 = ~(&q);                           // TC: LOW at HHHH, and NOT gated by CE


  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p1, p8, p16, 1'b0};

endmodule

`default_nettype wire
