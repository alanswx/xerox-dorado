// cell_MC10180 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 13 package position(s) across the sixteen boards.
//
// Dual 2-Bit Adder/Subtractor (MECL 10K). Datasheet:
// DoradoDocs/datasheets/MC10180.pdf. Two independent full adders share one
// pair of mode controls; EclDict's own names map straight onto the sheet --
// C0/C1 = Cin, D0/D1 = A, E0/E1 = B, H/H' = Sum and its complement,
// K0/K1 = Cout, M0/M1 = SelA/SelB.
//
// The mode controls INVERT an operand, which is how one part does four jobs.
// The datasheet's function select table:
//
//     SelA SelB   function
//      H    H     S = A plus B
//      H    L     S = A minus B
//      L    H     S = B minus A
//      L    L     S = 0 minus A minus B
//
// and its equations give A' = A XNOR SelA, B' = B XNOR SelB -- so a HIGH
// select passes the operand and a LOW one inverts it. Subtraction is then
// ordinary two's complement, the carry-in supplying the +1. The table is what
// confirms the reading: H,L gives A + ~B, which is A minus B with carry.
//
// After that it is a plain full adder per half.

`default_nettype none

module cell_MC10180 (
    output wire p1,  // H1'
    output wire p2,  // H0'
    output wire p3,  // K0
    input  wire p4,  // C0
    input  wire p5,  // D0
    input  wire p6,  // E0
    input  wire p7,  // M0
    input  wire p9,  // M1
    input  wire p10,  // E1
    input  wire p11,  // D1
    input  wire p12,  // C1
    output wire p13,  // K1
    output wire p14,  // H1
    output wire p15// H0
);

  wire selA = p7, selB = p9;

  wire a0 = ~(p5  ^ selA), b0 = ~(p6  ^ selB);   // bit 0 operands
  wire a1 = ~(p11 ^ selA), b1 = ~(p10 ^ selB);   // bit 1 operands

  wire s0 = a0 ^ b0 ^ p4;                        // C0 = pin 4
  wire s1 = a1 ^ b1 ^ p12;                       // C1 = pin 12
  wire k0 = (a0 & b0) | (p4  & (a0 ^ b0));
  wire k1 = (a1 & b1) | (p12 & (a1 ^ b1));

  assign p15 =  s0;  assign p2 = ~s0;  assign p3  = k0;   // H0, H0', K0
  assign p14 =  s1;  assign p1 = ~s1;  assign p13 = k1;   // H1, H1', K1
endmodule

`default_nettype wire
