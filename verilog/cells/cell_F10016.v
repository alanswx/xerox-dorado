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
  // PE'=5 parallel enable (active low), CO'=4 carry out (active low).
  //
  // BIT ORDER: PARC'S H0 IS THE DATA SHEET'S Q3. EclDict names the pins
  // `H0,14 > H1,15 > H2,2 > H3,3` and `D0,11 > D1,10 > D2,9 > D3,7`, while
  // the Fairchild connection diagram (DoradoDocs/datasheets/F10016.pdf p.1,
  // DIP top view) gives the silicon:
  //
  //     pin  3 = Q0 (LSB)   pin  2 = Q1   pin 15 = Q2   pin 14 = Q3 (MSB)
  //     pin  7 = P0 (LSB)   pin  9 = P1   pin 10 = P2   pin 11 = P3
  //
  // Both lists are EXACT reversals of PARC's, on outputs and on load inputs
  // alike, so the dictionary simply names this part MSB-FIRST the way it
  // names every other field in the machine. A CELL IMPLEMENTS THE DATA
  // SHEET'S FUNCTION PER PIN NUMBER (the rule the MC10141 established), so
  // pin 3 carries the LSB here.
  //
  // Three independent confirmations, none of them appealing to convention:
  //   - RfshAd is a 9-bit counter cascaded across MemX g05/g06/g07, and
  //     `g07.CO' -> g06.CE'` makes g07 the LOW-order package. Its pins carry
  //     RfshAd.5-.8, so H3 holds the whole counter's LSB.
  //   - MemProms.bcpl builds the j13 timing PROM as four groups of eight,
  //     split on {RfshInMem, MemState.0} -- refresh-vs-read and
  //     active-vs-idle -- which requires MemState.0 to be the HIGH bit.
  //   - j13 takes A0=RfshInMem, A1=MemState.0 ... A4=MemState.3, and
  //     cell_SG10139 is MSB-first, so the two agree only under this order.
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
      if (!p5)          q <= {p11, p10, p9, p7};   // PE' load: P3,P2,P1,P0
      else if (!p6)     q <= q + 4'd1;             // CE' count
    end
  end
  assign {p14, p15, p2, p3} = q;               // Q3,Q2,Q1,Q0 per the data sheet

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
