// cell_MC10179 -- MECL Look-Ahead Carry Block
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 1 package position(s) across the sixteen boards: MemC g05, the
// second-order carry of the BR+Mar virtual-address adder.
//
// Function: Motorola MC10179, DoradoDocs/datasheets/MC10179.pdf. The data
// sheet gives the outputs as functions of the PIN LEVELS:
//
//   Cn+2 = (Cn + P0 + P1) (G0 + P1) G1
//   Cn+4 = (Cn + P0 + P1 + P2 + P3) (G0 + P1 + P2 + P3) (G1 + P2 + P3) (G2 + P3) G3
//   GG   = (G0 + P1 + P2 + P3) (G1 + P2 + P3) (G2 + P3) G3
//   PG   = P0 + P1 + P2 + P3
//
// where group 0 is the least significant. Those are the standard look-ahead
// equations with every variable COMPLEMENTED: the part is built to take the
// active-low G' and P' outputs of an MC10181/F100181 slice and an active-low
// carry, and to return active-low carries. Read positively,
// Cn+2 = G1 + P1 G0 + P1 P0 Cn. MemC uses it exactly that way -- every net
// on it is primed (G24-27', P20-23', CO20', ...), the F100181's P'/G'/Cn' are
// all active low, and the two outputs feed F100181 Cn' inputs.
//
// PIN NUMBERING: EclDict names the groups MSB-first (its GI0/PI0 is the most
// significant group, pin 5/13), the data sheet LSB-first (its G0/P0 is pin
// 4/14). Both agree on the pins; the names below are EclDict's, the
// equations use the data sheet's order.
//
//   data sheet   pin   EclDict   MemC g05
//   G0           4     GI3       G24-27'   (least significant group)
//   P0          14     PI3       P24-27'
//   G1           7     GI2       G20-23'
//   P1          10     PI2       P20-23'
//   G2           9     GI1       G16-19'
//   P2          12     PI1       P16-19'
//   G3           5     GI0       G12-15'   (most significant group)
//   P3          13     PI0       P12-15'
//   Cn          11     CIN       G28-31'   (carry into group 0: the lowest
//                                          slice's own Cn' is tied high, so
//                                          its carry out is its generate)
//   Cn+2         6     CMD       CO20'     (carry into VA.16-19)
//   Cn+4         3     COUT      CO12'     (carry into VA.08-11)
//   GG           2     --        unused on MemC
//   PG          15     --        unused on MemC
//
// UNMODELLED UNTIL 2026-09-08. With both outputs dead the virtual address
// never carried out of VA.20-23, so a base register walked by 256-word pages
// wrapped after 64 pages: InitMem.mc's Map1to1Loop wrote the same 64 map
// entries for 290 M sys_clk and never reached VirtualBanks.

`default_nettype none

module cell_MC10179 (
    output wire p3,  // COUT  Cn+4'
    input  wire p4,  // GI3   G0'  (least significant group)
    input  wire p5,  // GI0   G3'  (most significant group)
    output wire p6,  // CMD   Cn+2'
    input  wire p7,  // GI2   G1'
    input  wire p9,  // GI1   G2'
    input  wire p10,  // PI2  P1'
    input  wire p11,  // CIN  Cn'
    input  wire p12,  // PI1  P2'
    input  wire p13,  // PI0  P3'
    input  wire p14// PI3   P0'
);

  wire cn = p11;
  wire g0 = p4,  p0 = p14;
  wire g1 = p7,  p1 = p10;
  wire g2 = p9,  p2 = p12;
  wire g3 = p5,  p3_ = p13;

  // Data-sheet equations, pin levels in, pin levels out.
  assign p6 = (cn | p0 | p1) & (g0 | p1) & g1;
  assign p3 = (cn | p0 | p1 | p2 | p3_) & (g0 | p1 | p2 | p3_) & (g1 | p2 | p3_) & (g2 | p3_) & g3;
endmodule

`default_nettype wire
