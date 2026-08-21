// cell_K1115A -- crystal oscillator module (SUBSTITUTION)
//
// Not a logic part and not in any data book: it is a packaged crystal
// oscillator, wired as a three-pin DIP -- pin 7 GND, pin 8 output, pin 14 VCC,
// nothing else connected. The boards' configuration sheets give the values,
// and TAKE THE SHEET THE WIRE LIST NAMES: the scanned
// DoradoDocs/schematics/DispY.pdf is Rev Ci 11/02/79 and says 20 MHz, but the
// wire list is built from DispY31.sil Rev Cl 3/25/82, which says 50.
//
//   DispY  a05  50 MHz         DispY31.sil  Rev Cl 3/25/82 note 5
//   DispM  c05  10 MHz, VCO    DispM30.sil  Rev Ch 11/09/82
//   DispM  d13  20 MHz Alto / 50 MHz LF   -- same sheet, monitor-dependent
//   DskEth j20  not stated     Ether12.sil draws it driving EClk0 with no
//                              value; the 23.530 MHz on DskEth sheet 7 is a
//                              separate xtalosc in the parts list.
//
// A SUBSTITUTION, in the same category as the BaseBoard's VCO (see
// cell_MPQ3303 and OVERRIDE_DRIVERS in tools/sil_to_verilog.py): an analog
// oscillator has no digital model and an FPGA has no crystal, so the substitute
// derives its output from the fabric clock. Everything downstream of it is the
// board's own logic.
//
// THE DIVISOR IS A CHOICE, not a measurement, and it is currently ONE choice
// for all four positions -- the generator instantiates cells without
// per-position parameters, so DispM's 10 MHz VCO and DispY's 50 MHz pixel
// clock presently run at the same rate. That is wrong for any test that cares
// about the RATIO between them; it is harmless only because no gate exercises
// these boards' timing yet. Fixing it means teaching the generator a
// per-(board, position) parameter override, not editing this default. The
// other testbenches divide `sys_clk` by 16 for the Dorado clock, whose
// microinstruction rate is 16.67 MHz (60 ns), so DIV = 13 is about 20.5 MHz
// in those terms.

`default_nettype none

module cell_K1115A #(
    parameter integer DIV = 13
) (
    input  wire sys_clk,
    output wire p8,     // oscillator output
    input  wire p7,     // GND
    input  wire p14     // VCC
);

  integer n = 0;
  reg     osc = 1'b0;
  always @(posedge sys_clk) begin
    if (n >= (DIV - 1)) begin n <= 0; osc <= ~osc; end
    else                       n <= n + 1;
  end
  assign p8 = osc;

  wire _unused = &{1'b0, p7, p14, 1'b0};
endmodule

`default_nettype wire
