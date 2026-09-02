// cell_F9401 -- Fairchild 9401 CRC Generator/Checker (TTL Macrologic)
//
// Pin numbers: PARC's EclDict. Function: the Fairchild data sheet,
// DoradoDocs/datasheets/F9401.pdf -- a 16-bit register clocked on the
// HIGH-to-LOW transition of CP (pin 1, EclDict's CK'), a ROM-selected
// polynomial on S0-S2 (pins 3, 5, 8), Data In D (11), Check Word Enable CWE
// (10), Master Reset MR (4, active HIGH, asynchronous clear), Preset P' (2,
// active LOW, asynchronously sets the register for a 16-bit polynomial),
// Data Output Q (12) and Error ER (13).
//
// Figure 1 of the data sheet (the equivalent circuit for X16+X15+X2+1): the
// feedback term is CWE AND (D XOR Q15); it enters stage 0 and is XORed into
// the inputs of stage 2 and stage 15. Q is the most significant stage, so
// with CWE low (feedback off) the register shifts the check word out on Q,
// MSB first, while the data path's external gating appends it. "To check an
// incoming message, both the data and check bits are entered ... with CWE
// held HIGH ... If there are no detectable errors, the resultant internal
// register bits are all LOW and the Error Output (ER) is LOW."
//
// Used in 2 package positions, both on DskEth: g17 generates the transmit
// CRC, j05 checks the receive CRC. Both have S2 S1 S0 = L L L, CRC-16. The
// other three 16-bit polynomials in Table 1 are implemented from their tap
// lists; the 12- and 8-bit ones (right-justified in the register) are not
// used on any Dorado board and are left unmodelled.
//
// Modelled on sys_clk with the clock edge detected by sampling, like every
// other clocked cell here; MR and P' are level-tested every sys_clk.
`default_nettype none
module cell_F9401 (
    input  wire sys_clk,
    input  wire p1,   // CP (CK'): shifts on the HIGH-to-LOW transition
    input  wire p2,   // P'  preset, active low
    input  wire p3,   // S0
    input  wire p4,   // MR  master reset, active high
    input  wire p5,   // S1
    input  wire p7,   // GND
    input  wire p8,   // S2
    input  wire p10,  // CWE check word enable
    input  wire p11,  // D   data in
    output wire p12,  // Q   data out (the check word, MSB first)
    output wire p13,  // ER  error: register not all zero
    input  wire p14   // VCC
);
  reg [15:0] r = 16'd0;
  reg        cp_d = 1'b1;
  wire [2:0] sel = {p8, p5, p3};
  // Tap masks: bit k set means the feedback is XORed into the input of
  // stage k (stage 0 always takes it). Table 1 of the data sheet.
  wire [15:0] taps = (sel == 3'b000) ? 16'h8004 :   // X16+X15+X2+1      CRC-16
                     (sel == 3'b001) ? 16'h4002 :   // X16+X14+X+1       CRC-16 reverse
                     (sel == 3'b010) ? 16'hA096 :   // X16+X15+X13+X7+X4+X2+X+1
                     (sel == 3'b110) ? 16'h1020 :   // X16+X12+X5+1      CRC-CCITT
                     (sel == 3'b111) ? 16'h0810 :   // X16+X11+X4+1      CRC-CCITT reverse
                                       16'h8004;
  wire fb = p10 & (p11 ^ r[15]);
  wire [15:0] shifted = {r[14:0], 1'b0} ^ (fb ? (taps | 16'h0001) : 16'h0000);
  always @(posedge sys_clk) begin
    cp_d <= p1;
    if (p4)             r <= 16'd0;          // MR
    else if (!p2)       r <= 16'hFFFF;       // P'
    else if (cp_d && !p1) r <= shifted;      // CP high-to-low
  end
  assign p12 = r[15];
  assign p13 = |r;
endmodule
`default_nettype wire
