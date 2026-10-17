// cell_F100181 -- 4-Bit Binary/BCD Arithmetic Logic Unit
//
// Pins: PARC's EclDict.Analyze, which agrees with the data sheet's 24-pin DIP
// assignment on every one. Function: National Semiconductor F100181,
// DoradoDocs/datasheets/F100181.pdf. Used in 8 package positions, all on
// MemC.
//
// THIS IS NOT THE 74181 / MC10181 FUNCTION SET. It looks like an ALU of the
// same family and is not: the F100181 has its own sixteen-entry table, S3
// selecting arithmetic from logic and S2 selecting BCD from binary. Do not
// reuse cell_MC10181's decode here.
//
//   S3 S2 S1 S0   Fn
//    L  L  L  L   A plus B plus Cn      (BCD)
//    L  L  L  H   A minus B plus Cn     (BCD)
//    L  L  H  L   B minus A plus Cn     (BCD)
//    L  L  H  H   0 minus B plus Cn     (BCD)
//    L  H  L  L   A plus B plus Cn      (binary)
//    L  H  L  H   A minus B plus Cn     (binary)
//    L  H  H  L   B minus A plus Cn     (binary)
//    L  H  H  H   0 minus B plus Cn     (binary)
//    H  L  L  L   An AND Bn
//    H  L  L  H   An XOR Bn
//    H  L  H  L   An OR Bn
//    H  L  H  H   An
//    H  H  L  L   NOT Bn
//    H  H  L  H   Bn
//    H  H  H  L   (NOT An) AND Bn
//    H  H  H  H   LOW
//
// PARC's dictionary names the select inputs `F0..F3` MSB-first, so its F0 is
// the data sheet's S3 (pin 21) and its F3 is S0 (pin 16) -- the same
// convention the ALUF field uses everywhere else here.
//
// TWO CONVENTIONS ARE ASSUMED AND NOT YET CONFIRMED, flagged because this
// project has been bitten by exactly this twice:
//
//   * `Cn` (pin 11) is "Carry Input (Active LOW)", so the carry is taken as
//     ~p11. SETTLED 2026-09-08 by the Functional Description: "set Cn' = LOW"
//     forces the initial carry that makes a subtraction two's complement.
//   * `E` (pin 19) is "Latch Enable Input (Active LOW)", and the description
//     says "leaving the Enable (E) input LOW makes the latches transparent".
//     Transparent-while-low is modelled; the latching edge is not exercised.
//
// NOTHING IN THE GATE SUITE EXERCISES THIS PART YET. MemC's eight of them are
// the memory board's address arithmetic, and no test issues a reference.

`default_nettype none

module cell_F100181 (
    input  wire sys_clk,
    input  wire p1,   // A0
    input  wire p24,  // A1
    input  wire p23,  // A2
    input  wire p22,  // A3
    input  wire p12,  // B0
    input  wire p13,  // B1
    input  wire p14,  // B2
    input  wire p15,  // B3
    input  wire p16,  // S0
    input  wire p17,  // S1
    input  wire p20,  // S2
    input  wire p21,  // S3
    input  wire p11,  // Cn'  carry in, active low
    input  wire p19,  // E'   latch enable, active low (transparent while low)
    output wire p2,   // F0
    output wire p3,   // F1
    output wire p4,   // F2
    output wire p5,   // F3
    output wire p8,   // Cn+4'
    output wire p9,   // P'
    output wire p10,  // G'
    input  wire p6,   // VCC
    input  wire p7,   // VCCA
    input  wire p18   // VEE
);

  wire [3:0] a   = {p22, p23, p24, p1};
  wire [3:0] b   = {p15, p14, p13, p12};
  wire [3:0] s   = {p21, p20, p17, p16};   // S3 S2 S1 S0
  wire       cin = ~p11;                   // Cn' active low: LOW = carry in

  // ARITHMETIC IS A + D + Cn WITH D THE (POSSIBLY INVERTED) OPERAND, exactly
  // as the data sheet's Functional Description has it: "A minus B plus Cn.
  // Arguments A and B are directly applied to the inputs. The circuit
  // automatically takes the ones complement of B (by inverting B internally)
  // ... to perform a subtraction with results in the twos complement form, an
  // initial carry should be forced into the lowest order bit, i.e. set Cn' =
  // LOW". So "A minus B plus Cn" is A + ~B + Cn -- NOT A - B + Cn, which is
  // one more. This cell used to compute the latter (2026-09-08), and MemC's
  // address adders run in "B minus A" with Mar' on A: BR + ~(Mar') + 0 =
  // BR + Mar. Off by one there is every virtual address one word high.
  //
  // The per-bit generate/propagate terms are the data sheet's Function Table
  // (Gn, Pn columns), the group outputs its lookahead equations:
  //   P' = P0' + P1' + P2' + P3'
  //   G' = ~(G3 + P3 G2 + P3 P2 G1 + P3 P2 P1 G0)
  //   Cn+4' = G' (P' + Cn')
  // MemC chains its slices through THESE, not only through Cn+4': the low
  // nibble's G' is the carry into the next slice (its own Cn is 0, so
  // C4 = G), and the P'/G' pairs feed an external lookahead.
  reg [3:0] gn, pn, lg;
  reg [3:0] sum;
  reg       arith;
  always @* begin
    arith = ~s[3];
    gn = 4'd0; pn = 4'd0; lg = 4'd0; sum = 4'd0;
    case (s)
      // Arithmetic. BCD (S2 low) is the same add/subtract with the part's
      // +6/-6 correction, NOT modelled -- MemC's use is binary address
      // arithmetic (S2 is tied high on all eight).
      4'b0000, 4'b0100: begin gn = a & b;   pn = a | b;   sum = a + b  + {3'b0, cin}; end
      4'b0001, 4'b0101: begin gn = a & ~b;  pn = a | ~b;  sum = a + ~b + {3'b0, cin}; end
      4'b0010, 4'b0110: begin gn = ~a & b;  pn = ~a | b;  sum = b + ~a + {3'b0, cin}; end
      4'b0011, 4'b0111: begin gn = 4'd0;    pn = ~b;      sum = ~b     + {3'b0, cin}; end
      // Logic, with the table's Gn/Pn for the lookahead outputs.
      4'b1000: begin lg =  a &  b; gn = a & b;  pn = a | b;  end
      4'b1001: begin lg =  a ^  b; gn = a & b;  pn = a | b;  end
      4'b1010: begin lg =  a |  b; gn = a & b;  pn = a | b;  end
      4'b1011: begin lg =  a;      gn = a;      pn = a;      end
      4'b1100: begin lg = ~b;      gn = 4'd0;   pn = ~b;     end
      4'b1101: begin lg =  b;      gn = 4'd0;   pn = b;      end
      4'b1110: begin lg = ~a & b;  gn = ~a & b; pn = ~a | b; end
      4'b1111: begin lg = 4'd0;    gn = 4'd0;   pn = 4'hf;   end
      default: ;
    endcase
  end
  wire [3:0] f = arith ? sum : lg;
  wire grp_p = &pn;
  wire grp_g = gn[3] | (pn[3] & gn[2]) | (pn[3] & pn[2] & gn[1]) | (pn[3] & pn[2] & pn[1] & gn[0]);
  // The output latch: transparent while E is LOW. Modelled on sys_clk with the
  // level as an enable, the convention every clocked cell here uses.
  reg [3:0] q;
  always @(posedge sys_clk) if (!p19) q <= f;
  assign {p5, p4, p3, p2} = q;
  // Carry out and the lookahead terms, all active low.
  assign p8  = ~(grp_g | (grp_p & cin));
  assign p9  = ~grp_p;
  assign p10 = ~grp_g;
  wire _unused = &{1'b0, p6, p7, p18, 1'b0};
endmodule

`default_nettype wire
