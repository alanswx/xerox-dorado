// cell_MC10136 -- MECL model for the Xerox Dorado
//
// Ports: pin numbers and signal names from PARC's EclDict.Analyze.
// Directions: observed in the .wl wire lists across all boards.
// Used in 9 package position(s) across the sixteen boards.
//
// Universal Hexadecimal Counter. Data book (DL122 rev 7, MC10136): "Lines S1
// and S2 determine one of four operations; preset (program), increment (count
// up), decrement (count down), or hold (stop count). Note that in the preset
// mode a clock pulse is necessary to load the counter... Carry Out goes low
// on the terminal count, or when the counter is being preset."
//
// EclDict names the two select lines `SU'` and `SD'` -- select-up and
// select-down, both active low -- which decodes the mode table on sight:
//
//     SU'(9)  SD'(7)   operation
//       L       L      preset: Q <- D on the clock
//       L       H      count up
//       H       L      count down
//       H       H      hold
//
// Counting is enabled by Carry In (`CI'`, pin 10), also active low; with it
// high the counter holds, which is what the data book's truth table shows.
//
// PIN INDICES are MSB-first as everywhere in this machine: EclDict's B0 is
// pin 5, which the sheet calls D3.

`default_nettype none

module cell_MC10136 (
    input  wire sys_clk,
    output wire p2,  // H1
    output wire p3,  // H0
    output wire p4,  // CO'
    input  wire p5,  // B0
    input  wire p6,  // B1
    input  wire p7,  // SD'
    input  wire p9,  // SU'
    input  wire p10,  // CI'
    input  wire p11,  // B2
    input  wire p12,  // B3
    input  wire p13,  // C
    output wire p14,  // H3
    output wire p15// H2
);

  wire [3:0] d = {p5, p6, p11, p12};      // D3..D0
  wire su_n = p9, sd_n = p7, cin_n = p10;

  wire preset = ~su_n & ~sd_n;
  wire up     = ~su_n &  sd_n;
  wire down   =  su_n & ~sd_n;

  reg ck_d;
  always @(posedge sys_clk) ck_d <= p13;
  wire ck_en = p13 & ~ck_d;               // CLOCK, positive edge

  reg [3:0] q;
  always @(posedge sys_clk)
    if (ck_en) begin
      if (preset)             q <= d;
      else if (up   & ~cin_n) q <= q + 4'd1;
      else if (down & ~cin_n) q <= q - 4'd1;
    end

  assign {p3, p2, p15, p14} = q;          // Q3..Q0

  // Carry Out, active low: the terminal count -- all ones counting up, all
  // zeros counting down -- or a preset. Gated by Carry In, which the data
  // book's table confirms: at Q=1111 with Cin high the output stays high.
  assign p4 = ~(preset |
                (~cin_n & ((up & (q == 4'hF)) | (down & (q == 4'h0)))));
endmodule

`default_nettype wire
