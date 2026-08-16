// cell_MC10135 -- Dual J-K Master-Slave Flip-Flop
//
// Pins: PARC's EclDict/TtlDict.Analyze. Used in 29 package position(s).

`default_nettype none

module cell_MC10135 (
    input  wire sys_clk,
    input  wire p9,
    input  wire p7,
    input  wire p6,
    input  wire p4,
    input  wire p5,
    output wire p2,
    output wire p3,
    input  wire p10,
    input  wire p11,
    input  wire p13,
    input  wire p12,
    output wire p15,
    output wire p14
);

  // CC=9 common clock; J'/K' are ACTIVE LOW (the dictionary names them with
  // primes); R/S asynchronous, active high.
  reg qa, qb;
  wire ja = ~p7,  ka = ~p6;
  wire jb = ~p10, kb = ~p11;

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
  always @(posedge sys_clk) ck_d <= p9;
  wire ck_en = p9 & ~ck_d;

  always @(posedge sys_clk) begin
    if (p5)         qa <= 1'b1;
    else if (p4)    qa <= 1'b0;
    else if (ck_en) qa <= (ja & ~qa) | (~ka & qa);
  end
  always @(posedge sys_clk) begin
    if (p12)        qb <= 1'b1;
    else if (p13)   qb <= 1'b0;
    else if (ck_en) qb <= (jb & ~qb) | (~kb & qb);
  end
  assign p2 = qa; assign p3 = ~qa;
  assign p15 = qb; assign p14 = ~qb;


endmodule

`default_nettype wire
