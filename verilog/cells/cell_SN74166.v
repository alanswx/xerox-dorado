// cell_SN74166 -- 8-Bit Parallel-In / Serial-Out Shift Register (TTL)
//
// Pins: PARC's EclDict/TtlDict.Analyze. Used in 40 package position(s).

`default_nettype none

module cell_SN74166 (
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
  always @(posedge p7 or negedge p9) begin
    if (!p9)      q <= 8'd0;                      // CL'
    else if (!p6) begin                           // CE'
      if (!p15) q <= {p2,p3,p4,p5,p10,p11,p12,p14};  // load
      else      q <= {q[6:0], p1};                   // shift, SI in
    end
  end
  assign p13 = q[7];


  wire _unused_pins = &{1'b0, p8, p16, 1'b0};

endmodule

`default_nettype wire
