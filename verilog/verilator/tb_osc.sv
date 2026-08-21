// tb_osc -- the crystal oscillators run at their OWN frequencies.
//
// Four K1115A positions across three boards, and the configuration sheets give
// each a different value: DispY a05 50 MHz, DispM c05 10 MHz (the VCO), DispM
// d13 20 MHz for an Alto-style monitor or 50 for LF. Until CELL_PARAMS existed
// the generator instantiated every cell without parameters, so all four ran at
// one rate -- which is wrong for anything that cares about the RATIO between
// them, and these are display pixel clocks, so eventually everything will.
//
// d13 is taken as the 20 MHz Alto-style value: DispM's two monitor modes are
// 640x480 and 1024x768, whose pixel-rate ratio is 2.5 -- exactly 50/20 -- and
// the C emulator's head reports the 640x480 "standard" one.
//
// This counts edges rather than trusting the parameters, so it fails if the
// generator stops passing them or the cell's arithmetic changes. The check is
// on the RATIO, 1 : 2 : 5, because the absolute rates are quantised by an
// integer divisor of sys_clk (DIV 13, 7, 3 give 10.3, 19.0 and 44.4 MHz) and
// asserting the quantised values would just restate the table.

`default_nettype none

module tb_osc;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  DispM_m_Rev_m_Ch dm (.sys_clk(sys_clk));
  DispY_m_Rev_m_Cl dy (.sys_clk(sys_clk));

  // Oscillator outputs, by the nets their pin 8 drives.
  wire osc10 = dm.DispM23_sil_pl_16;   // c05, 10 MHz VCO
  wire osc20 = dm.DispM25_sil_pl_1;    // d13, 20 MHz
  wire osc50 = dy.DispY25_sil_pl_1;    // a05, 50 MHz

  integer i, n10, n20, n50;
  reg p10, p20, p50;
  real r20, r50, mhz10;
  // 20000 sys_clk at 3.75 ns each is 75 us; an edge is half a period.
  localparam real WINDOW_US = 20000.0 * 3.75 / 1000.0;

  initial begin
    n10 = 0; n20 = 0; n50 = 0;
    p10 = osc10; p20 = osc20; p50 = osc50;
    for (i = 0; i < 20000; i = i + 1) begin
      @(posedge sys_clk);
      if (osc10 !== p10) begin n10 = n10 + 1; p10 = osc10; end
      if (osc20 !== p20) begin n20 = n20 + 1; p20 = osc20; end
      if (osc50 !== p50) begin n50 = n50 + 1; p50 = osc50; end
    end

    $display("tb_osc: edges over 20000 sys_clk -- c05 %0d, d13 %0d, a05 %0d", n10, n20, n50);
    if (n10 == 0) begin
      $display("tb_osc: FAIL -- the 10 MHz oscillator does not run");
      $fatal(1);
    end
    r20 = n20 * 1.0 / n10;
    r50 = n50 * 1.0 / n10;
    $display("tb_osc: ratios against c05 -- d13 %0.2f (want 2.0), a05 %0.2f (want 5.0)", r20, r50);

    // 10 percent, which the integer divisors comfortably meet.
    if (r20 < 1.8 || r20 > 2.2) begin
      $display("tb_osc: FAIL -- d13 is not twice c05; are the per-position parameters reaching the cell?");
      $fatal(1);
    end
    if (r50 < 4.5 || r50 > 5.5) begin
      $display("tb_osc: FAIL -- a05 is not five times c05");
      $fatal(1);
    end
    // The accumulator gets the absolute rate right too, not just the ratio,
    // so hold it to that: edges/2 periods in WINDOW_US microseconds.
    mhz10 = (n10 / 2.0) / WINDOW_US;
    $display("tb_osc: c05 absolute rate %0.2f MHz (want 10.0)", mhz10);
    if (mhz10 < 9.5 || mhz10 > 10.5) begin
      $display("tb_osc: FAIL -- c05 is not running at 10 MHz");
      $fatal(1);
    end
    $display("tb_osc: PASS -- 10 / 20 / 50 MHz in a 1 : 2 : 5 ratio");
    $finish;
  end

endmodule
