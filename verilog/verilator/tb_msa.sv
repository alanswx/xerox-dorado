// tb_msa -- A WORD INTO THE STORAGE ARRAY, standalone.
//
// `storage-test` puts PARC's msa board in a machine and shows it clocked and
// addressed. It cannot show a WRITE, because in `dorado_storage` the DRAM
// strobes come from MemX and a testbench cannot force them -- they need a real
// memory reference, which is a separate open question.
//
// So this drives the board ALONE, where every port is the bench's: present a
// word on `Sout`, run a DRAM cycle on `MemAd` + `MemRAS/CAS/WE`, and look
// inside the MK4096 arrays. That is a unit test of the storage array itself,
// independent of whether the memory section can currently ask for one.
//
// THE PATHS ARE ASYMMETRIC, which is what makes the write half tractable:
//
//   WRITE   Sout.00-03 -> b01 (MC10176 register, clk msa01.sil+4)
//                      -> b02 (MC10125 ECL->TTL) -> MK4096 pin 2, DIN
//           no shift register at all.
//
//   READ    eight MK4096 DOUT pins -> a13 (SN74166) PARALLEL inputs
//                      -> shifted out of pin 13 -> c02 (MC10124 TTL->ECL)
//                      -> c01 (MC10176) -> Sin.00-03, EcIn.0
//           twenty '166s, eight bits each.
//
// and everything on the board's own sequencer -- c1, c2, SO, SLa, OutCKa,
// ShiftLoad' -- hangs off `CLK.ms0Even'`, the MSA's slot clock. Leave that
// undriven and the board looks perfectly gated while doing nothing; that
// caught `storage-test` once already.

`default_nettype none

module tb_msa;
  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  reg [3:0] div = 4'd0;
  reg       mclk = 1'b0;
  always @(posedge sys_clk) begin
    div <= div + 4'd1;
    if (div == 4'd7) begin mclk <= ~mclk; div <= 4'd0; end
  end

  reg [15:0] sout   = 16'h0000;
  reg [7:0]  memad  = 8'h00;
  reg        ras    = 1'b1, cas = 1'b1, we = 1'b1;
  reg        loadso = 1'b1, shiftso = 1'b0;
  reg        loadsi = 1'b0, shiftsi = 1'b0;
  reg        strEn  = 1'b1, sinEn   = 1'b1;

  msa m (
      .sys_clk(sys_clk),
      .CLK_ms0Even_p_(mclk),
      .Sout_00(sout[0]),  .Sout_01(sout[1]),  .Sout_02(sout[2]),  .Sout_03(sout[3]),
      .Sout_04(sout[4]),  .Sout_05(sout[5]),  .Sout_06(sout[6]),  .Sout_07(sout[7]),
      .Sout_08(sout[8]),  .Sout_09(sout[9]),  .Sout_10(sout[10]), .Sout_11(sout[11]),
      .Sout_12(sout[12]), .Sout_13(sout[13]), .Sout_14(sout[14]), .Sout_15(sout[15]),
      .MemAd_1(memad[0]), .MemAd_2(memad[1]), .MemAd_3(memad[2]), .MemAd_4(memad[3]),
      .MemAd_5(memad[4]), .MemAd_6(memad[5]), .MemAd_7(memad[6]), .MemAd_8(memad[7]),
      .MemRASa(ras), .MemRASb(ras), .MemCASa(cas), .MemCASb(cas),
      .MemWEa(we),   .MemWEb(we),
      .LoadSoutE_p_(loadso), .ShiftSoutE(shiftso),
      .LoadSinE(loadsi),     .ShiftSinE(shiftsi),
      .LoadEcOut_p_(1'b1),   .ShiftEcOut(1'b0),
      .Mod0StrEn_p_(strEn),  .Mod0SinEn_p_(sinEn),
      .EcOut_0(1'b0), .EcOut_1(1'b0), .EcOut_4(1'b0), .EcOut_5(1'b0),
      .ChipsAre16k(1'b1), .ChipsAre4k(1'b0), .ChipsAre64K(1'b0), .Mb0(1'b0)
  );

  integer i, n_ones, n_zeros;
  reg dinb, dinq, dout_seen, sla_seen, sin_one, sin_zero;

  task step(input integer n);
    begin repeat (n) @(posedge sys_clk); end
  endtask

  // One DRAM write cycle: register Sout, then row on RAS' falling, column on
  // CAS' falling, WE' low for an early write.
  task write_word(input [15:0] w, input [7:0] row, input [7:0] col);
    begin
      sout = w; strEn = 1'b0;
      loadso = 1'b0; step(64); loadso = 1'b1; step(64);
      we = 1'b0;  step(8);
      memad = row; ras = 1'b0; step(8);
      memad = col; cas = 1'b0; step(32);
      cas = 1'b1; step(8);
      ras = 1'b1; step(8);
      we = 1'b1;  step(64);
    end
  endtask

  // READ BACK. The '166 loads eight DRAM DOUTs in parallel and its serial
  // output is q[7], which the model loads from pin 2 -- and pin 2 is b05's
  // DOUT. So the bit is at the output the moment the load happens; no shifts
  // are needed to see THIS bit. Pulse LoadSinE (with Mod0SinEn' low) to make
  // the board's sequencer drive SLa, then let c01 clock it out to Sin.
  task read_word(input [7:0] row, input [7:0] col);
    begin
      sinEn = 1'b0;
      we = 1'b1;  step(8);
      memad = row; ras = 1'b0; step(8);
      memad = col; cas = 1'b0; step(32);
      // LoadSinE LOW then HIGH. The other way round leaves SLa high and the
      // '166 stays in SHIFT mode, so it never takes the DRAM outputs: the
      // read then returns 0 while b05's DOUT is plainly 1, which reads as a
      // dead read path and is a polarity mistake in the bench.
      loadsi = 1'b0; step(32); loadsi = 1'b1; step(32);
      dout_seen = m.u_b05.p14; sla_seen = m.SLa;
      shiftsi = 1'b1; step(64); shiftsi = 1'b0;
      cas = 1'b1; step(8);
      ras = 1'b1; step(64);
    end
  endtask

  function integer nonzero_cells;
    integer k, c;
    begin
      c = 0;
      for (k = 0; k < 4096; k = k + 1) if (m.u_b05.mem[k] !== 1'b0) c = c + 1;
      nonzero_cells = c;
    end
  endfunction

  initial begin
    step(400);

    // ---- write ZEROS first, and confirm the array stays empty -------------
    write_word(16'h0000, 8'h2B, 8'h15);
    dinq   = m.u_b05.p2;
    n_zeros = nonzero_cells();

    // ---- then write ONES, through the same path --------------------------
    write_word(16'hFFFF, 8'h2B, 8'h15);
    dinb   = m.u_b05.p2;
    n_ones = nonzero_cells();

    // read the location back -- it holds a 1
    read_word(8'h2B, 8'h15);
    sin_one = m.Sin_00__drv;

    // now write a 0 to the same place and read THAT back, so the gate cannot
    // pass on a stuck-high output
    write_word(16'h0000, 8'h2B, 8'h15);
    read_word(8'h2B, 8'h15);
    sin_zero = m.Sin_00__drv;

    $display("tb_msa: during the read -- b05 DOUT=%b, SLa=%b (low = load), RAS'=%b CAS'=%b WE'=%b",
             dout_seen, sla_seen, m.u_b05.p4, m.u_b05.p15, m.u_b05.p3);
    $display("tb_msa: READ BACK -- a13 Qh (msa04.sil+29) = %b -> c02 (msa04.sil+31) = %b -> Sin.00 = %b",
             m.msa04_sil_pl_29, m.msa04_sil_pl_31, m.Sin_00__drv);
    $display("tb_msa: standalone storage board -- 144 MK4096 DRAMs, 291 packages");
    $display("tb_msa: Sout -> b01 Q (msa04.sil+32/33) = %b%b -> b02 (msa04.sil+8) = %b -> b05 DIN = %b",
             m.msa04_sil_pl_32, m.msa04_sil_pl_33, m.msa04_sil_pl_8, m.u_b05.p2);
    $display("tb_msa: writing 0000 left DIN=%b and %0d cells set; writing FFFF gave DIN=%b and %0d",
             dinq, n_zeros, dinb, n_ones);

    // THE DATA REACHES THE ARRAY'S INPUT. b05 is a DATA bit -- b04 is an ECC
    // bit fed from f01/f02 off EcOut, which is why probing it first showed a
    // permanent 0 and looked like a broken write path.
    if (dinq !== 1'b0 || dinb !== 1'b1)
      $fatal(1, "b05 DIN does not follow Sout (0 gave %b, 1 gave %b)", dinq, dinb);

    // AND THE WRITE LANDS. Zeros leave the array empty; ones put cells in it.
    if (n_zeros != 0)
      $fatal(1, "writing zeros set %0d cells", n_zeros);
    if (n_ones == 0)
      $fatal(1, "writing ones set no cells -- the DRAM never took the write");

    $display("tb_msa: ROUND TRIP -- wrote 1, read Sin.00=%b; wrote 0, read Sin.00=%b",
             sin_one, sin_zero);
    if (sin_one !== 1'b1 || sin_zero !== 1'b0)
      $fatal(1, "the storage array did not read back what was written (1 gave %b, 0 gave %b)",
             sin_one, sin_zero);

    $display("tb_msa: PASS -- A WORD GOES INTO PARC'S STORAGE ARRAY AND COMES BACK:");
    $display("tb_msa:   Sout registered, translated to TTL, written into the MK4096s,");
    $display("tb_msa:   read out through the SN74166 and returned on Sin.");
    $finish;
  end
endmodule
`default_nettype wire
