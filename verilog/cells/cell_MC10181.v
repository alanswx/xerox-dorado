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

  // Carry out, and the look-ahead pair for chaining slices: GG is the group
  // generate (a carry out with no carry in) and PG the group propagate (every
  // bit would pass one along), so Cn+4 = GG + PG.Cn.
  assign p5 = M ? 1'b0 : sum[4];   // Cn+4; logic mode is don't-care per sheet
  assign p4 = gsum[4];             // GG
  assign p8 = &(X | Y);            // PG
endmodule

`default_nettype wire
