// cell_MC10135 -- Dual J-K Master-Slave Flip-Flop
//
// Pins: PARC's EclDict/TtlDict.Analyze. Used in 29 package position(s).

`default_nettype none

module cell_MC10135 (
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
  always @(posedge p9 or posedge p4 or posedge p5) begin
    if (p5)      qa <= 1'b1;
    else if (p4) qa <= 1'b0;
    else         qa <= (ja & ~qa) | (~ka & qa);
  end
  always @(posedge p9 or posedge p13 or posedge p12) begin
    if (p12)      qb <= 1'b1;
    else if (p13) qb <= 1'b0;
    else          qb <= (jb & ~qb) | (~kb & qb);
  end
  assign p2 = qa; assign p3 = ~qa;
  assign p15 = qb; assign p14 = ~qb;


endmodule

`default_nettype wire
