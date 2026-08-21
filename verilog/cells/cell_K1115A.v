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
// THE FREQUENCY IS SET PER POSITION -- see CELL_PARAMS in
// tools/sil_to_verilog.py, which passes FREQ_KHZ at each instantiation.
//
// It is a PHASE ACCUMULATOR rather than an integer divider, because an
// integer divisor of sys_clk cannot express these three values. The
// testbenches run 16 sys_clk to a 60 ns microinstruction, so sys_clk is
// 266.667 MHz and the divisors for 10, 20 and 50 MHz would be 13.33, 6.67 and
// 2.67 -- rounded to 13, 7 and 3 they give a ratio of 1 : 1.86 : 4.33 where
// the parts are 1 : 2 : 5, an 13 percent error on the fastest. The
// accumulator carries the remainder instead, so the AVERAGE frequency is
// right to within one sys_clk of jitter per edge.
//
// (An earlier note here derived a divisor from the FULL period rather than
// the half and so was out by two -- it called DIV = 13 "20 MHz" when that is
// about 10.)
//
// Gate: make -C verilog osc-test, which counts edges and requires the three
// known positions to come out 1 : 2 : 5.

`default_nettype none

module cell_K1115A #(
    // DskEth j20 keeps this default: Ether12.sil draws it driving EClk0 with
    // no value stated, and the 23.530 MHz elsewhere in DskEth belongs to a
    // different xtalosc in the parts list.
    parameter integer FREQ_KHZ = 20000
) (
    input  wire sys_clk,
    output wire p8,     // oscillator output
    input  wire p7,     // GND
    input  wire p14     // VCC
);

  // 16 sys_clk to a 60 ns microinstruction.
  localparam integer SYS_KHZ = 266667;
  localparam integer STEP    = 2 * FREQ_KHZ;   // two edges to a period

  integer acc = 0;
  reg     osc = 1'b0;
  always @(posedge sys_clk) begin
    if (acc + STEP >= SYS_KHZ) begin
      acc <= acc + STEP - SYS_KHZ;
      osc <= ~osc;
    end else begin
      acc <= acc + STEP;
    end
  end
  assign p8 = osc;

  wire _unused = &{1'b0, p7, p14, 1'b0};
endmodule

`default_nettype wire
