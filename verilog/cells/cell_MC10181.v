// cell_MC10181 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 8 package position(s) across the sixteen boards.
//
// 4-Bit Arithmetic Logic Unit / Function Generator. Datasheet:
// DoradoDocs/datasheets/MC10181.pdf. THIS IS THE DORADO'S ALU: two slices on
// ProcH and two on ProcL make the 16-bit datapath (docs/sil-netlist-
// crosscheck.md), and four more on the IFU do its address arithmetic.
//
// A NICE CONFIRMATION of the architecture docs falls out of the pin list.
// CLAUDE.md says "ALUF is a 4-bit pointer into ALUFM (16 x 6 bits) which the
// microcoder loads at startup with the 16 ALU operations actually used".
// Six bits per entry is exactly what this part takes: S0-S3, M and Cn.
//
// PIN NAMES: EclDict indexes MSB-FIRST, the datasheet LSB-first, so they look
// reversed and are not. Dictionary D0 is pin 10, which the sheet calls A3.
// Same for E (operand B), F (the SELECT inputs) and H (the result). Xerox
// numbers bit 0 as the most significant throughout this machine.
//
//   dict   D0-D3 = A3..A0   pins 10,16,18,21     E0-E3 = B3..B0   9,11,19,20
//          F0-F3 = S3..S0   pins 13,15,17,14     H0-H3 = F3..F0   6,7,3,2
//          M 23   CIN 22    COUT 5 (Cn+4)   Gg 4 (GG)   Pg 8 (PG)
//
// Cn is an ACTIVE-HIGH carry in: the datasheet's arithmetic column is headed
// "Cn is low" and its first row gives F = A, so a high Cn adds one. That is
// what makes "A minus B minus 1" at Cn=0 into "A minus B" at Cn=1.
//
// Written as a 16-way MUX over the select rather than as the part's gate
// network: it is the datasheet's own table, one row per line, so it can be
// checked by eye against the sheet -- and a mux is one LUT level on an FPGA.

`default_nettype none

module cell_MC10181 (
    output wire p2,  // H3
    output wire p3,  // H2
    output wire p4,  // Gg
    output wire p5,  // COUT
    output wire p6,  // H0
    output wire p7,  // H1
    output wire p8,  // Pg
    input  wire p9,  // E0
    input  wire p10,  // D0
    input  wire p11,  // E1
    input  wire p13,  // F0
    input  wire p14,  // F3
    input  wire p15,  // F1
    input  wire p16,  // D1
    input  wire p17,  // F2
    input  wire p18,  // D2
    input  wire p19,  // E2
    input  wire p20,  // E3
    input  wire p21,  // D3
    input  wire p22,  // CIN
    input  wire p23// M
);

  wire [3:0] A  = {p10, p16, p18, p21};   // A3..A0
  wire [3:0] B  = {p9,  p11, p19, p20};   // B3..B0
  wire [3:0] S  = {p13, p15, p17, p14};   // S3..S0
  wire       M  = p23;
  wire       Cn = p22;

  // The datasheet's function table. `lg` is the logic column (M high); the
  // arithmetic column (M low) is X plus Y plus Cn.
  reg [3:0] lg, X, Y;
  always @* begin
    case (S)
      4'd0:  begin lg =  ~A;      X = A;       Y = 4'h0;   end // A
      4'd1:  begin lg = ~A | ~B;  X = A;       Y = A & ~B; end // A plus A.B'
      4'd2:  begin lg = ~A |  B;  X = A;       Y = A &  B; end // A plus A.B
      4'd3:  begin lg =  4'hF;    X = A;       Y = A;      end // A times 2
      4'd4:  begin lg = ~A & ~B;  X = A |  B;  Y = 4'h0;   end // (A+B) plus 0
      4'd5:  begin lg =  ~B;      X = A |  B;  Y = A & ~B; end
      4'd6:  begin lg = ~(A ^ B); X = A;       Y = B;      end // A plus B
      4'd7:  begin lg = A | ~B;   X = A;       Y = A |  B; end
      4'd8:  begin lg = ~A &  B;  X = A | ~B;  Y = 4'h0;   end
      4'd9:  begin lg = A ^ B;    X = A;       Y = ~B;     end // A minus B minus 1
      4'd10: begin lg = B;        X = A | ~B;  Y = A &  B; end
      4'd11: begin lg = A | B;    X = A;       Y = A | ~B; end
      4'd12: begin lg = 4'h0;     X = 4'hF;    Y = 4'h0;   end // minus 1
      4'd13: begin lg = A & ~B;   X = A & ~B;  Y = 4'hF;   end // (A.B') minus 1
      4'd14: begin lg = A &  B;   X = A &  B;  Y = 4'hF;   end // (A.B) minus 1
      default: begin lg = A;      X = A;       Y = 4'hF;   end // A minus 1
    endcase
  end

  wire [4:0] gsum = {1'b0, X} + {1'b0, Y};   // the group, carry-in 0
  wire [4:0] sum  = gsum + {4'd0, Cn};
  // (Gating Cn by M here would be dead code -- M already selects `lg` for the
  // result and forces the carry out below. A mutation test found that.)

  wire [3:0] F = M ? lg : sum[3:0];
  assign {p6, p7, p3, p2} = F;                      // H0..H3 = F3..F0

  // Carry out, and the look-ahead pair for chaining slices.
  //
  // THE LOOK-AHEAD PINS ARE NOT THE TTL '181's G AND P, AND THE NAMES ARE
  // INTERCHANGED (2026-09-02). This part's positive-logic function table is
  // the 74181 family's ACTIVE-LOW-DATA table (S=0001 gives A'+B', S=0110
  // gives A plus B), and in that convention the two look-ahead outputs
  // exchange roles -- the same silicon, read the other way up. In positive
  // logic they are the two carry-out conditions of the slice:
  //
  //     pin 4 "GG" = carry out IF Cn were 1   = (X plus Y) >= 15
  //     pin 8 "PG" = carry out IF Cn were 0   = (X plus Y) >= 16
  //
  // Derived from the board, not from the datasheet's names. ProcH computes
  // the carries into its two slices with discrete OR-AND gates (d12, an
  // MC10118, and e12, an MC10121) from ProcL's GG/PG and ripple carry:
  //
  //     Cin(04-07) = (PG1 + C0) . GG1
  //     Cin(00-03) = (PG1 + C0 + PG2) . GG2 . (PG2 + GG1)
  //
  // and those are a correct 16-bit adder ONLY under the reading above (an
  // exhaustive polarity search found no other assignment; 8,112 vectors, 0
  // mismatches). The MC10179's own printed equations, "Cn+2 = (Cn+P0+P1)
  // (G0+P1) G1", are the same form and are correct under the same reading.
  // With the previous definitions (GG = generate, PG = all-bits-propagate) a
  // carry GENERATED in the adjacent slice arrived but one PROPAGATED through
  // it was lost: fff1 + 000f read ff00, fff1 + 0010 read f001 -- which is what
  // held Initial's task-init loop open (its exit test is RM[1]+T = T-15 < 0).
  // `alu-diff` never saw it because it chains the slices by ripple carry.
  wire [4:0] gsum1 = gsum + 5'd1;               // the group, carry-in 1
  assign p5 = M ? 1'b0 : sum[4];   // Cn+4; logic mode is don't-care per sheet
  assign p4 = gsum1[4];            // GG: carry out with Cn = 1
  assign p8 = gsum[4];             // PG: carry out with Cn = 0
endmodule

`default_nettype wire
