// cell_MC10182 -- 2-Bit Arithmetic Logic Unit / Function Generator
//
// Pin numbers/names: PARC's EclDict.Analyze (what ANALYZE used to check the
// boards). Function: DoradoDocs/datasheets/MC10182.pdf (also in the 1978
// MECL data book) -- 4 logic and 4 arithmetic operations on two 2-bit
// words, full internal carry, group carry propagate/generate for an
// MC10179 second-order lookahead.
//
// Used in 3 package positions across the sixteen boards -- ProcL j17, k17,
// l17 -- which together are THE STACK-POINTER ADDER: StkP + RSTK[1:3]'s
// signed delta into StkWadr, through the i16/i17 MC10173 muxes. Left a
// skeleton, StkP could never adjust: pushes and pops moved nothing, Initial
// arrived at DORETURN with the stack pointer still at its STKINITL value
// (offset 0 = empty), and the underflow check held the machine forever --
// the c01 park, 3.57M microinstructions on one address.
//
// Pin map, dictionary name = datasheet name (dict is MSB-first, so its
// bit 0 is the datasheet's bit 1):
//   dict D0 = A1 (p12), D1 = A0 (p5), E0 = B1 (p11), E1 = B0 (p6)
//   dict H0 = F1 (p14), H1 = F0 (p4)
//   dict CIN = Cn (p13), COUT = Cn+2 (p2)
//   dict F0 = S1 (p9), F1 = S0 (p10), M = M (p7)
//   dict Gg = GG (p3), Pg = PG (p15)
//
// Function select (datasheet, positive logic):
//   S1 S0 | M high (logic)   | M low (arithmetic)
//    L  L | F = A XOR B      | F = A  plus B  plus Cn
//    L  H | F = A XNOR B     | F = ~A plus B  plus Cn
//    H  L | F = A AND B      | F = A  plus ~B plus Cn
//    H  H | F = A OR B       | F = A times 2 (A plus A plus Cn)
//
// All three Dorado positions leave S0, S1 and M unconnected; MECL unused
// inputs read LOW, so the part is a plain 2-bit adder here.

`default_nettype none

module cell_MC10182 (
    input  wire p5,   // A0  (dict D1)
    input  wire p12,  // A1  (dict D0)
    input  wire p6,   // B0  (dict E1)
    input  wire p11,  // B1  (dict E0)
    input  wire p13,  // Cn  (dict CIN)
    input  wire p7,   // M
    input  wire p9,   // S1  (dict F0)
    input  wire p10,  // S0  (dict F1)
    output wire p4,   // F0  (dict H1)
    output wire p14,  // F1  (dict H0)
    output wire p2,   // Cn+2 (dict COUT)
    output wire p3,   // GG  (dict Gg)
    output wire p15,  // PG  (dict Pg)
    // Power pins, declared so every board connection lands somewhere.
    input  wire p1,
    input  wire p8,
    input  wire p16
);

  wire [1:0] A = {p12, p5};
  wire [1:0] B = {p11, p6};

  // Arithmetic operand transforms per (S1,S0); see the table above.
  wire [1:0] Aop = (~p9 & p10) ? ~A : A;
  wire [1:0] Bop = (p9 & ~p10) ? ~B :
                   (p9 &  p10) ?  A : B;

  // Ripple with per-bit propagate/generate, which also yields PG/GG for
  // the MC10179 lookahead exactly as the data sheet defines them.
  wire g0 = Aop[0] & Bop[0], pr0 = Aop[0] | Bop[0];
  wire g1 = Aop[1] & Bop[1], pr1 = Aop[1] | Bop[1];
  wire c1 = g0 | (pr0 & p13);

  wire [1:0] sum = {Aop[1] ^ Bop[1] ^ c1, Aop[0] ^ Bop[0] ^ p13};

  // Logic mode (M high): no carry participates.
  wire [1:0] flog = (~p9 & ~p10) ?  (A ^ B)  :
                    (~p9 &  p10) ? ~(A ^ B)  :
                    ( p9 & ~p10) ?  (A & B)  :
                                    (A | B);

  wire [1:0] F = p7 ? flog : sum;

  assign p4  = F[0];
  assign p14 = F[1];
  // Cn+2 and the group terms are arithmetic-path outputs; the data sheet
  // gates the internal carry on M low. All Dorado positions hold M low.
  assign p2  = ~p7 & (g1 | (pr1 & c1));
  assign p3  = g1 | (pr1 & g0);         // GG
  assign p15 = pr1 & pr0;               // PG

  wire _unused_pins = &{1'b0, p1, p8, p16, 1'b0};
endmodule

`default_nettype wire
