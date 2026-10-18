// cell_CLOCKGEN -- current oversampled BaseBoard clock approximation.
// Replaces h06 MPQ3303, g05/h05 MC1660 and g03/g04/h03/h04 MC1690 outputs.
// Downstream chip models still detect clock edges a fabric tick later.
//
// BaseBd11.sil's divider has SCP'=g04.Q' and ECP=h03.Q; its nominal ECP
// lead is 1/8 clock. The original-divider simulation at N=16 gives:
//
//     SCP'  1111 0000 0000 1111      high 0-3, LOW 4-11, high 12-15
//     ECP   1100 0000 0011 1111      high 0-1, LOW 2-9,  high 10-15
//
// These are simulated digital phases, not oscilloscope measurements.
// Integer scaling preserves them at N divisible by 8, but NOT at small N:
// N=4 doubles the relative lead; N=2 coincides; N=1 gives constant outputs.
// See `make -C verilog clock-phase-audit` and docs/verilog-clock-plan.md.
// The experimental direct-enable source is rtl/dorado_clock_phases.sv;
// production logic remains unchanged pending board-level qualification.

`default_nettype none

module cell_CLOCKGEN #(
    // Fabric ticks per 30 ns clock (HM 2.3), TWO clocks per instruction.
    // 16 reproduces nominal divider phases, not analog propagation delays.
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
  // at 4/16: the original divider's two-tick lead at N=16. At small N
  // quantization changes the relative lead and the j02 OR's low window.
  localparam integer HALF   = (N / 2 < 1) ? 1 : N / 2;
  localparam integer ECP_LO = (2 * N) / 16;
  localparam integer SCP_LO = (4 * N) / 16;

  assign EndClockPulse     = !(phase >= ECP_LO[W-1:0] &&
                               phase <  ECP_LO[W-1:0] + HALF[W-1:0]);
  assign StartClockPulse_n = !(phase >= SCP_LO[W-1:0] &&
                               phase <  SCP_LO[W-1:0] + HALF[W-1:0]);

endmodule

`default_nettype wire
