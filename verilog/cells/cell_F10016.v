// cell_F10016 -- 4-Bit Synchronous Binary Counter with parallel load
//
// Pin numbers and names: PARC's EclDict.Analyze. Used in 228 package
// position(s) across the sixteen Dorado boards.

`default_nettype none

module cell_F10016 (
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
  always @(posedge p13 or posedge p12) begin
    if (p12)        q <= 4'd0;                 // MR, asynchronous
    else if (!p5)   q <= {p7, p9, p10, p11};   // PE' load
    else if (!p6)   q <= q + 4'd1;             // CE' count
  end
  assign {p3, p2, p15, p14} = q;
  assign p4 = ~(&q & ~p6);                     // CO' active low on terminal count


  // Board-wired pins the dictionary does not name (power and the like)
  wire _unused_pins = &{1'b0, p1, p8, p16, 1'b0};

endmodule

`default_nettype wire
