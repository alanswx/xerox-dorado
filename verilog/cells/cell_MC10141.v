// cell_MC10141 -- Four-Bit Universal Shift Register
//
// Pins: PARC's EclDict.Analyze. Function: the part's TRUTH TABLE, read from
// two independent Motorola books that agree exactly --
// DoradoDocs/datasheets/Motorola_MECL_DataBook_1978.pdf p.3-53 and
// ON_MECL_DataBook_DL122_rev7.pdf p.333.  Used in 60 package position(s).
//
//   SELECT              OUTPUTS (after a positive edge at C)
//   S1 S2  MODE         Q0n+1  Q1n+1  Q2n+1  Q3n+1
//   L  L   Parallel      D0     D1     D2     D3
//   L  H   Shift Right   Q1n    Q2n    Q3n    DR
//   H  L   Shift Left    DL     Q0n    Q1n    Q2n
//   H  H   Stop Shift    Q0n    Q1n    Q2n    Q3n
//
// DIP pin assignment, both books: 2=Q2 3=Q3 4=C 5=DR 6=D3 7=S2 9=D2 10=S1
// 11=D1 12=D0 13=DL 14=Q0 15=Q1.
//
// PARC NAMES TWO PAIRS OF PINS THE OTHER WAY ROUND, and that is not an error
// on either side -- it is which end of a 16-bit register you call "left".
// EclDict calls pin 5 DL and pin 13 DR (Motorola: DR and DL), and pin 10 SL'
// and pin 7 SR' (Motorola: S1 and S2).  The two swaps are consistent with each
// other: the mode PARC calls shift-left is selected by its SL' and consumes
// its DL, exactly as its names promise.  So the BOARD reads correctly under
// PARC's names and the CELL must implement Motorola's function per PIN NUMBER.
// Do not "fix" the dictionary's names into this file; pin numbers are the only
// thing the two sources share, and they agree on every one.

`default_nettype none

module cell_MC10141 (
    input  wire sys_clk,
    input  wire p4,
    input  wire p12,
    input  wire p11,
    input  wire p9,
    input  wire p6,
    input  wire p5,
    input  wire p13,
    input  wire p10,
    input  wire p7,
    output wire p14,
    output wire p15,
    output wire p2,
    output wire p3,
    input  wire p1,
    input  wire p8
);

  reg [3:0] q;   // q[3:0] = Q3,Q2,Q1,Q0

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
  reg ck_d;
  always @(posedge sys_clk) ck_d <= p4;
  wire ck_en = p4 & ~ck_d;

  always @(posedge sys_clk) if (ck_en) begin
    case ({p10, p7})                       // S1, S2
      2'b00: q <= {p6, p9, p11, p12};      // Parallel:    Q3<-D3 Q2<-D2 Q1<-D1 Q0<-D0
      2'b01: q <= {p5, q[3:1]};            // Shift Right: Q3<-DR, each Q takes the next
      2'b10: q <= {q[2:0], p13};           // Shift Left:  Q0<-DL, each Q takes the previous
      default: q <= q;                     // Stop shift
    endcase
  end
  assign {p3, p2, p15, p14} = q;           // Q3, Q2, Q1, Q0


  // Board-wired pins not named above (power and the like)
  wire _unused_pins = &{1'b0, p1, p8, 1'b0};

endmodule

`default_nettype wire
