// cell_K1115A -- crystal oscillator module (SUBSTITUTION)
//
// Not a logic part and not in any data book: it is a packaged crystal
// oscillator, and the Dorado schematics say so directly. DispY's sheet
// (bitsavers .../xerox/dorado/schematics/DispY.pdf) notes
//
//     "Crystal oscillator K1115A, location a05, value 20 MHz"
//
// which the wire list matches exactly -- a05 on DispY, and the package is
// wired as a three-pin DIP oscillator: pin 7 GND, pin 8 output, pin 14 VCC,
// nothing else connected. Used in 2 package positions:
//
//     DispY  a05  -> DispY25.sil+1   the display's pixel-clock chain
//     DskEth j20  -> EClk0           the Ethernet clock
//
// A SUBSTITUTION, in the same category as the BaseBoard's VCO (see
// cell_MPQ3303 and OVERRIDE_DRIVERS in tools/sil_to_verilog.py): an analog
// oscillator has no digital model and an FPGA has no crystal, so the substitute
// derives its output from the fabric clock. Everything downstream of it is the
// board's own logic.
//
// THE DIVISOR IS A CHOICE, not a measurement. The other testbenches divide
// `sys_clk` by 16 for the Dorado clock, whose microinstruction rate is
// 16.67 MHz (60 ns), so 20 MHz is 16 * 16.67/20 = 13.3 sys_clk. `DIV` defaults
// to 13, i.e. about 20.5 MHz in those terms. DskEth's crystal value is not
// stated on the DispY sheet; if its own schematic gives a different one, set
// the parameter at that instance rather than changing the default.
//
// Nothing in the gate suite exercises either board yet.

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
