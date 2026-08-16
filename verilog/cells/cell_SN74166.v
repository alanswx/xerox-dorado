// cell_SN74166 -- 8-Bit Parallel-In / Serial-Out Shift Register (TTL)
//
// Pins: PARC's EclDict/TtlDict.Analyze. Used in 40 package position(s).

`default_nettype none

module cell_SN74166 (
    input  wire sys_clk,
    input  wire p2,
    input  wire p3,
    input  wire p4,
    input  wire p5,
    input  wire p10,
    input  wire p11,
    input  wire p12,
    input  wire p14,
    input  wire p1,
    input  wire p15,
    input  wire p6,
    input  wire p7,
    input  wire p9,
    output wire p13,
    input  wire p8,
    input  wire p16
);

  // A-H = 2,3,4,5,10,11,12,14 parallel data; SI=1 serial in; SL=15
  // shift/load; CE'=6 clock enable (active low); CK=7 clock; CL'=9 clear
  // (active low); QH=13 serial out.
  reg [7:0] q;

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
  always @(posedge sys_clk) ck_d <= p7;
  wire ck_en = p7 & ~ck_d;

  always @(posedge sys_clk) begin
    if (!p9)      q <= 8'd0;                      // CL'
    else if (ck_en && !p6) begin                  // CE'
      if (!p15) q <= {p2,p3,p4,p5,p10,p11,p12,p14};  // load
      else      q <= {q[6:0], p1};                   // shift, SI in
    end
  end
  assign p13 = q[7];


  wire _unused_pins = &{1'b0, p8, p16, 1'b0};

endmodule

`default_nettype wire
