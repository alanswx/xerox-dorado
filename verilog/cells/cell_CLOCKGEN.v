// cell_CLOCKGEN -- the BaseBoard's clock generator, as a PHASE COUNTER.
//
// A SUBSTITUTION, and a deliberate extension of the one already made for the
// VCO. `cell_MPQ3303`'s header records the original reasoning: an analog
// oscillator has no digital model and an FPGA has no VCO, so the oscillator
// is replaced by a fabric-clock divider and "everything after it is the
// board's own logic". This moves that boundary forward by six packages, and
// the justification is the same in kind: everything between the VCO and
// `StartClockPulse'`/`EndClockPulse` exists ONLY to make clock phases, and an
// FPGA can make them directly and better.
//
// WHAT IT REPLACES: BaseBd h06 (MPQ3303 VCO), g05 and h05 (MC1660 shaping),
// g03/g04/h03/h04 (MC1690 dividers). Seven packages producing two waveforms.
//
// WHY, and it is not tidiness. Every stage after the VCO recovers its clock
// by EDGE DETECTION on `sys_clk`:
//
//     always @(posedge sys_clk) ck_d <= p7;
//     wire ck_en = p7 & ~ck_d;
//
// That is a cascade of oversampled detectors, and each one needs its input
// stable for at least a full `sys_clk`. The margin compounds down the chain,
// and it is what forces 16 `sys_clk` per microinstruction -- which in turn is
// what makes the FPGA Dorado run at a fraction of the real machine's speed,
// since the microinstruction rate is `sys_clk / ratio`. Generating the phases
// from a counter removes the cascade: every clock net then changes ONLY on a
// `sys_clk` edge, so the detection downstream is exact rather than
// approximate, and the ratio can come down.
//
// THE WAVEFORMS ARE MEASURED, NOT INVENTED. `tb_baseboard` captured the real
// chain's output over two periods at 16x:
//
//     SCP'  1111 0000 0000 1111      high 0-3, LOW 4-11, high 12-15
//     ECP   1100 0000 0011 1111      high 0-1, LOW 2-9,  high 10-15
//
// Both 50% duty on a 16-cycle period, with `EndClockPulse` leading
// `StartClockPulse'` by 2. Those are the numbers below, expressed as
// fractions of N so they scale with the ratio.

`default_nettype none

module cell_CLOCKGEN #(
    // sys_clk per microinstruction. 16 reproduces the divider chain exactly.
    parameter integer N = 16
) (
    input  wire sys_clk,
    output wire StartClockPulse_n,     // StartClockPulse'
    output wire EndClockPulse
);

  localparam integer W = (N <= 2) ? 1 : $clog2(N);

  reg [W-1:0] phase = {W{1'b0}};
  always @(posedge sys_clk)
    phase <= (phase == N[W-1:0] - 1'b1) ? {W{1'b0}} : phase + 1'b1;

  // Low for half the period; ECP's low phase starts at 2/16 of it and SCP''s
  // at 4/16, which is the measured two-cycle lead at N = 16. At small N these
  // collapse toward adjacent cycles, which is the most a short period can
  // express and still give each net an edge the detectors can see.
  localparam integer HALF   = (N / 2 < 1) ? 1 : N / 2;
  localparam integer ECP_LO = (2 * N) / 16;
  localparam integer SCP_LO = (4 * N) / 16;

  assign EndClockPulse     = !(phase >= ECP_LO[W-1:0] &&
                               phase <  ECP_LO[W-1:0] + HALF[W-1:0]);
  assign StartClockPulse_n = !(phase >= SCP_LO[W-1:0] &&
                               phase <  SCP_LO[W-1:0] + HALF[W-1:0]);

endmodule

`default_nettype wire
