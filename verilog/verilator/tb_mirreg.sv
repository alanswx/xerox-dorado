// tb_mirreg -- JAM A MICROINSTRUCTION AND READ IT BACK, RTL against the C
// emulator.
//
// This is what the BaseBoard does 475 times during a cold boot: it cannot write
// IM directly, so it jams each microinstruction into the microinstruction
// register a byte at a time and lets the machine execute a Write-IM out of it.
// tb_mir proved the DECODERS -- which CPOut bit reaches which field line. This
// proves the REGISTER: that the bits stick, that four strobes accumulate, and
// that what comes out is the microinstruction the C emulator says it is.
//
// THE MIR IS A BANK OF SET/RESET FLIP-FLOPS, which is the thing worth knowing
// here and is not obvious from the software model. Each field bit is half an
// MC10231 whose
//
//     S  = `s<FIELD>`     the BaseBoard's jam path, from the MC10172 decoders
//     D  = `d<FIELD>`     the execute path, from IM
//     R  = `rMIRa`        the reset
//     C  = `clk0'..`      the microinstruction clock
//     Q  = `<FIELD>`      the signal the datapath actually runs on
//
// So a jam is: reset every bit, then SET the ones the four bytes call for. That
// is exactly why the Control function carries a ClrMIR bit, and why the four
// strobes accumulate rather than each loading a register.
//
// TWO CONSEQUENCES the test has to respect, and both are how the hardware is
// driven rather than conveniences:
//
//   * `rMIRa` is REGISTERED, in an MC10176 at ContA j02 fed from `CPOut.5`. It
//     holds until another Control strobe clears it. So the sequence is assert
//     ClrMIR, release it, and only then send the bytes -- with ClrMIR still
//     asserted, nothing can be set.
//   * THE MACHINE HAS TO BE STOPPED, and it is -- but not for the reason first
//     assumed here. Each MIR flip-flop takes `d<FIELD>` on D and `clk0'..` on
//     C, so a microinstruction clock tick would reload it from the execute
//     path and lose the jam. The first version of this comment claimed the
//     test arranged that by tying `CLK.ca'`/`CLK.cb'` low. It does not:
//     MEASURED, driving those inputs with a square wave toggles them 999 times
//     and moves `clk0'Bc` ONCE. The Control board's clock generator is gated by
//     the run/stop state, which is idle here, so the microinstruction clock
//     does not run whatever the backplane clock does -- and that, rather than
//     anything this testbench does, is why a jam sticks. The Freeze bit exists
//     for the case where the machine IS running.
//
// Run with +vectors=<file>; the make target does that.

`default_nettype none

module tb_mirreg;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  reg [2:0] addr_n;
  reg [8:0] cpout;
  reg       strb_n;

  // BOTH Control boards, wired to each other BY THE BACKPLANE, because the MIR
  // reset crosses it: ContA makes `rMIRa` and ContB receives it, and both drive
  // the net, so it resolves as a wired-OR. Instantiating the two boards side by
  // side and hoping is not enough -- that is exactly what failed first, with
  // ContA's reset never reaching its own flip-flops. `dorado_control` is
  // tools/sil_backplane.py's two-board machine and it does the resolution.
  //
  // The board clocks are held static: a running microinstruction clock would
  // reload every MIR bit from the execute path. See the header.
  dorado_control m (
      .sys_clk(sys_clk), .CLK_ca_p_(1'b0), .CLK_cb_p_(1'b0),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n)
  );

  `define UA m.b_ContA
  `define UB m.b_ContB

  // The register's outputs -- the microinstruction the datapath sees. Most of
  // them leave the board, so they are read as this board's backplane
  // contribution; the few that stay on it are plain wires.
  wire [3:0] rstk  = {`UB.RSTK_0__drv, `UB.RSTK_1__drv,
                      `UB.RSTK_2__drv, `UB.RSTK_3__drv};
  wire [3:0] aluf  = {`UB.ALUF_0__drv, `UB.ALUF_1__drv,
                      `UB.ALUF_2__drv, `UB.ALUF_3__drv};
  wire [2:0] bsel  = {`UB.BSEL_0, `UB.BSEL_1, `UB.BSEL_2};
  wire [2:0] lc    = {`UB.LC_0__drv, `UB.LC_1__drv, `UB.LC_2__drv};
  wire [2:0] asel  = {`UB.ASEL_0__drv, `UB.ASEL_1, `UB.ASEL_2};
  wire       block = `UA.Block__drv;
  wire [7:0] ff    = {`UA.FF_0__drv, `UA.FF_1__drv, `UA.FF_2__drv, `UA.FF_3__drv,
                      `UA.FF_4__drv, `UA.FF_5__drv, `UA.FF_6__drv, `UA.FF_7__drv};
  wire [7:0] jcn   = {`UA.JCN_0, `UA.JCN_1, `UA.JCN_2, `UA.JCN_3,
                      `UA.JCN_4, `UA.JCN_5, `UA.JCN_6, `UA.JCN_7};

  task strobe(input [2:0] fn, input [7:0] data, input extra);
    begin
      addr_n = ~fn;
      cpout  = {data, extra};
      repeat (4) @(posedge sys_clk);
      strb_n = 1'b0;
      repeat (6) @(posedge sys_clk);
      strb_n = 1'b1;
      repeat (4) @(posedge sys_clk);
    end
  endtask

  // ClrMIR is CPOut.5, which is the data byte's bit 2 -- MSB-first, so
  // `data & 0x04`, exactly what the C emulator tests.
  task clear_mir;
    begin
      strobe(3'd0, 8'h04, 1'b0);      // assert the MIR reset
      strobe(3'd0, 8'h00, 1'b0);      // and release it
    end
  endtask

  integer fd, code, cases, errors, shown, i;
  reg [7:0] b1, b2, b3, b4, w_ff, w_jcn;
  integer   e0, e1, e2, e3, w_block;
  reg [3:0] w_rstk, w_aluf;
  reg [2:0] w_bsel, w_lc, w_asel;
  string    path, line, tag;

  task fail(input string what, input [7:0] got, input [7:0] want);
    begin
      errors = errors + 1;
      if (shown < 12) begin
        shown = shown + 1;
        $display("  FAIL %02h %02h %02h %02h e=%0d%0d%0d%0d : %s RTL %02h, emulator %02h",
                 b1, b2, b3, b4, e0, e1, e2, e3, what, got, want);
      end
    end
  endtask

  initial begin
    addr_n = 3'b111; cpout = 9'd0; strb_n = 1'b1;
    cases = 0; errors = 0; shown = 0;
    repeat (40) @(posedge sys_clk);

    if (!$value$plusargs("vectors=%s", path)) path = "cpreg.vec";
    fd = $fopen(path, "r");
    if (fd == 0) $fatal(1, "tb_mirreg: cannot open %s", path);

    while (!$feof(fd)) begin
      void'($fgets(line, fd));
      code = $sscanf(line,
                     "%s %h %h %h %h %d %d %d %d %h %h %h %h %h %d %h %h",
                     tag, b1, b2, b3, b4, e0, e1, e2, e3,
                     w_rstk, w_aluf, w_bsel, w_lc, w_asel, w_block,
                     w_ff, w_jcn);
      if (code != 17 || tag != "REG") continue;

      cases = cases + 1;
      clear_mir();
      strobe(3'd4, b1, e0[0]);
      strobe(3'd5, b2, e1[0]);
      strobe(3'd6, b3, e2[0]);
      strobe(3'd7, b4, e3[0]);

      if (rstk  !== w_rstk)  fail("RSTK",  {4'd0, rstk}, {4'd0, w_rstk});
      if (aluf  !== w_aluf)  fail("ALUF",  {4'd0, aluf}, {4'd0, w_aluf});
      if (bsel  !== w_bsel)  fail("BSEL",  {5'd0, bsel}, {5'd0, w_bsel});
      if (lc    !== w_lc)    fail("LC",    {5'd0, lc},   {5'd0, w_lc});
      if (asel  !== w_asel)  fail("ASEL",  {5'd0, asel}, {5'd0, w_asel});
      if (block !== w_block[0]) fail("BLOCK", {7'd0, block}, {7'd0, w_block[0]});
      if (ff    !== w_ff)    fail("FF",  ff,  w_ff);
      if (jcn   !== w_jcn)   fail("JCN", jcn, w_jcn);
    end
    $fclose(fd);

    $display("tb_mirreg: %0d microinstructions jammed and read back, %0d field mismatches",
             cases, errors);
    if (cases == 0) $fatal(1, "no REG vectors were read");
    if (errors != 0)
      $fatal(1, "the RTL microinstruction register and the C emulator disagree");
    $finish;
  end

endmodule

`default_nettype wire
