// cell_MC14521B -- 24-Stage Frequency Divider (CMOS)
//
// Ports: pin numbers from PARC's EclDict.Analyze. Function and pin assignment:
// the Motorola data sheet, DoradoDocs/datasheets/MC14521B.pdf.
//
//   "The MC14521B consists of a chain of 24 flip-flops with an input circuit
//    that allows three modes of operation. The input will function as a
//    crystal oscillator, an RC oscillator, or as an input buffer for an
//    external oscillator. Each flip-flop divides the frequency of the previous
//    flip-flop by two ... The count advances on the negative going edge of the
//    clock. The outputs of the last seven-stages are available."
//
//   1 Q24    2 Reset   3 VSS'   4 Out2   5 VDD'   6 In2   7 Out1   8 VSS
//   9 In1   10 Q18    11 Q19   12 Q20   13 Q21   14 Q22  15 Q23   16 VDD
//
// THIS PACKAGE IS THE DORADO'S WATCHDOG TIMER. There is one of it in the whole
// machine, BaseBd g21, and the board uses it in the third mode: Reset (pin 2)
// tied low, VDD/VDD'/VSS/VSS' wired, `MCPreClk` into In2 (pin 6), and **Q21
// (pin 13)** taken out to `BaseBd09.sil+8`, which clocks the SN74LS74 pair at
// g22. So the watchdog interval is MCPreClk / 2^21 -- of order a second, not
// milliseconds.
//
// What it gates: g23 (an SN7486) XORs `WatchdogIn` against `WatchdogOut`, and
// j17 (an open-collector SN74LS01) NANDs that against this divider's output to
// drive `BootMC'`, which is the D input of the reset flip-flop j08 that makes
// `MCReset'`. `PacifyWatchdog` in doradocontinuous.masm keeps the XOR at zero
// by echoing WatchdogIn (RIOT PA7) onto WatchdogOut (PA6); when the firmware
// stops echoing, the next divider edge resets the 6502.
//
// Leaving this unmodelled -- pin 13 dangling, so the net resolved low -- is
// why `firmware-probe` saw the 6502 restart every 211,440 sys_clk.

`default_nettype none

module cell_MC14521B (
    input  wire sys_clk,
    input  wire p2,   // Reset, active HIGH
    input  wire p3,   // VSS'
    input  wire p5,   // VDD'
    input  wire p6,   // In 2  -- the external clock on this board
    input  wire p8,   // VSS
    output wire p13,  // Q21
    input  wire p16   // VDD
);

  // FPGA: ONE CLOCK, and the modelled clock net becomes an ENABLE -- the same
  // transform every clocked cell here uses. The part counts on the NEGATIVE
  // edge, so the enable is the falling edge of In 2.
  reg [23:0] cnt = 24'd0;
  reg        ck_d = 1'b0;
  always @(posedge sys_clk) ck_d <= p6;
  wire fall = ~p6 & ck_d;

  always @(posedge sys_clk)
    if (p2)        cnt <= 24'd0;      // Reset is active high, and asynchronous
    else if (fall) cnt <= cnt + 24'd1;

  // Qn is the output of the nth flip-flop, i.e. counter bit n-1: the data
  // sheet's "count capacity" column gives Q18 = 2^18, so Q21 divides by 2^21.
  assign p13 = cnt[20];

  wire _unused_pins = &{1'b0, p3, p5, p8, p16, 1'b0};
endmodule

`default_nettype wire
