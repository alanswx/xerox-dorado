// tb_datapath -- a FOUR-BOARD Dorado: Control plus both processor boards,
// running, with a jammed microinstruction's fields on the datapath.
//
// tb_run showed the Control section sequencing on its own. This adds ProcH and
// ProcL and asks the two questions that follow:
//
//   * does the microinstruction clock reach the datapath? The Control boards
//     make `clk0'`; ProcH and ProcL have their own fan-out of it
//     (`Clock0'Bc` and `Clock0'Bd`), and they have to step in lockstep with it
//     or nothing downstream can be trusted.
//   * do the MIR FIELDS reach the datapath? The microinstruction register is on
//     the Control boards; `RSTK`, `ALUF`, `BSEL`, `LC` and `ASEL` cross the
//     backplane to the processor. Jamming a distinctive value and reading it
//     off the far side exercises the whole path at once -- the BaseBoard's bus,
//     the function decode, the MC10172 demultiplexers, the MC10231 register,
//     and the backplane.
//
// Note which fields cross COMPLEMENTED: `BSEL'` and `ASEL'` do, `RSTK`, `ALUF`
// and `LC` do not. That is the boards' own choice, not a convention -- it is
// read here from the generated port names rather than assumed.
//
// The boot sequence is tb_run's; see that file for why ClrStop and SetRun go in
// one Control byte.

`default_nettype none

module tb_datapath;


  // THE OVERSAMPLING RATIO -- sys_clk per microinstruction. SYSPER reaches only
  // `cell_CLOCKGEN`, which lives on the BaseBoard, so a sub-machine's clock is
  // whatever its bench drives; this one used to hard-code a divide-by-16 and
  // therefore only ever ran at 16x. Real time needs sys_clk = SYSPER x 16.67
  // MHz and measured Fmax is 48.99 MHz, so SYSPER=2 is the target.
  //
  //     make -C verilog/verilator <gate> SYSPER=2
`ifndef SYSPER_OVERRIDE
  `define SYSPER_OVERRIDE 16
`endif
  localparam integer SYSPER = `SYSPER_OVERRIDE;
  // SCALE A FABRIC-CYCLE WAIT so it means the same amount of DORADO time at any
  // ratio. NOT `SYSPER / 16` -- integer division gives ZERO below 16x, which
  // turns every `repeat` into no wait at all.
  function automatic integer WT(input integer n);
    begin
      // ROUND TO NEAREST, not toward zero. Truncation flattens the SHAPE of a
      // waveform at low ratios -- at SYSPER=4 a 4-cycle low and a 6-cycle
      // setup both truncate to 1, turning 4:6 into 1:1 -- and the single-step
      // chain depends on strobe SPACING (SetRun must survive three RunClk'
      // edges). That cost step-test at 4x and nothing else. Exact at
      // SYSPER=16, where WT(n) = n with no remainder.
      WT = (n * SYSPER + 8) / 16;
      if (WT < 1) WT = 1;
    end
  endfunction

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  reg [3:0] ckd = 4'd0;
  // At SYSPER=16 this is bit for bit the counter it replaces:
  // ckd counts 0..15 and mclk is ckd >= 8, i.e. ckd[3].
  always @(posedge sys_clk)
    ckd <= (ckd == SYSPER - 1) ? 4'd0 : ckd + 4'd1;
  wire mclk = (ckd >= SYSPER / 2);

  reg [2:0] addr_n = 3'b111;
  reg [8:0] cpout  = 9'd0;
  reg       strb_n = 1'b1;
  reg       setrun = 1'b0, setss_n = 1'b1;
  reg       dmd = 1'b0, dmc = 1'b0, udmd = 1'b0;

  dorado_proc m (
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n)
  );

  // PARC's SetMufflerAddress: twelve bits MSB-first into ContB's shift chain,
  // then the ManClk pulse DoClock(UseDMD)/DoClock(0) makes. `DMuxData`,
  // `DMuxClk` and `UseDMD` are forced because BaseBd l24 drives all three and
  // this configuration has no BaseBoard. See tb_operand.sv.
  integer k;
  task manifold(input [11:0] word);
    begin
      for (k = 11; k >= 0; k = k - 1) begin
        dmd = word[k];
        repeat (WT(4)) @(posedge sys_clk); dmc = 1'b1;
        repeat (WT(4)) @(posedge sys_clk); dmc = 1'b0;
        repeat (WT(4)) @(posedge sys_clk);
      end
      udmd = 1'b1; repeat (WT(12)) @(posedge sys_clk);
      udmd = 1'b0; repeat (WT(12)) @(posedge sys_clk);
    end
  endtask

  task strobe(input [2:0] fn, input [7:0] data, input ss);
    begin
      addr_n = ~fn;
      cpout  = {data, ss};
      repeat (WT(6)) @(posedge sys_clk);
      strb_n = 1'b0;
      repeat (WT(8)) @(posedge sys_clk);
      strb_n = 1'b1;
      repeat (WT(6)) @(posedge sys_clk);
    end
  endtask

  // The fields as they arrive at the processor, most significant bit first.
  wire [3:0] rstk = {m.RSTK_0, m.RSTK_1, m.RSTK_2, m.RSTK_3};
  wire [3:0] aluf = {m.ALUF_0, m.ALUF_1, m.ALUF_2, m.ALUF_3};
  wire [2:0] bsel = ~{m.BSEL_0_p_, m.BSEL_1_p_, m.BSEL_2_p_};
  wire [2:0] lc   = {m.LC_0, m.LC_1, m.LC_2};
  wire [2:0] asel = ~{m.ASEL_0_p_, m.ASEL_1_p_, m.ASEL_2_p_};

  localparam integer RUN = 40000;
  integer i, n0, nph, npl;
  reg     p0, pph, ppl;

  initial begin
    force m.DMuxData = dmd;
    force m.DMuxClk  = dmc;
    force m.UseDMD   = udmd;
    repeat (WT(60)) @(posedge sys_clk);

    // HOLD THE MIR, which is what makes a jam survive its own first clock --
    // the two manifold words doradomufman.masm writes just after the supplies
    // come up. Without them the MIR reloads from IM and the fields read here
    // are whatever IM holds, which is why this test used to depend on the IM
    // address being stuck at 0 (a cell_MC1662 bug, fixed 2026-08-18).
    manifold(12'h030);          // DisableDoradoErrors
    manifold(12'h1E0);          // SetMidasStopMIRClk

    strobe(3'd1, 8'h21, 1'b0);
    strobe(3'd0, 8'h4E, 1'b0); setrun = 0;
    strobe(3'd0, 8'h00, 1'b1); setss_n = 0;

    // A distinctive microinstruction. Byte bits are MSB-first onto CPOut, and
    // the field mapping is the one mir-diff pins down:
    //   MIR0 bits 0..2 -> RSTK.1,2,3   MIR1 bits 0..2 -> ALUF.1,2,3
    //   MIR2 bits 0,1  -> BSEL.1,2     MIR3 bits 1..3 -> ASEL.0,1,2
    strobe(3'd4, 8'hE0, 1'b1);   // RSTK.1,2,3 set; extra bit -> RSTK.0
    strobe(3'd5, 8'hE0, 1'b1);   // ALUF.1,2,3 set
    strobe(3'd6, 8'hC0, 1'b0);   // BSEL.1,2 set
    strobe(3'd7, 8'h70, 1'b1);   // ASEL.0,1,2 set

    strobe(3'd0, 8'h41, 1'b1); setrun = 1;

    n0 = 0; nph = 0; npl = 0;
    for (i = 0; i < RUN; i = i + 1) begin
      @(posedge sys_clk);
      // ContA's clk0'Ca, NOT ContB's clk0'Bc: the latter is the MIR clock
      // (ContB j05 takes StopMIRClkBD), which the manifold hold above stops on
      // purpose. ContA c19's h*clk0'Ca is gated the same way; d18's clk0'Ca is
      // not, and it is the microinstruction clock the datapath runs on.
      if (m.b_ContA.clk0_p_Ca  !== p0)  begin n0  = n0  + 1; p0  = m.b_ContA.clk0_p_Ca;  end
      if (m.b_ProcH.Clock0_p_Bc !== pph) begin nph = nph + 1; pph = m.b_ProcH.Clock0_p_Bc; end
      if (m.b_ProcL.Clock0_p_Bd !== ppl) begin npl = npl + 1; ppl = m.b_ProcL.Clock0_p_Bd; end
    end

    $display("tb_datapath: clk0'Ca %0d edges, ProcH Clock0'Bc %0d, ProcL Clock0'Bd %0d",
             n0, nph, npl);
    $display("tb_datapath: at the processor -- RSTK=%b ALUF=%b BSEL=%b ASEL=%b, DoradoStopped=%b",
             rstk, aluf, bsel, asel, m.b_ContB.DoradoStopped);

    if (m.b_ContB.DoradoStopped !== 1'b0) $fatal(1, "the machine is stopped");
    if (n0 < RUN/40)                      $fatal(1, "the Control clock is not running");
    if (nph !== n0)                       $fatal(1, "ProcH does not step with the Control clock");
    if (npl !== n0)                       $fatal(1, "ProcL does not step with the Control clock");
    if (rstk !== 4'b1111)                 $fatal(1, "RSTK did not reach the processor");
    if (aluf !== 4'b0111)                 $fatal(1, "ALUF did not reach the processor");
    if (bsel !== 3'b011)                  $fatal(1, "BSEL did not reach the processor");
    if (asel !== 3'b111)                  $fatal(1, "ASEL did not reach the processor");
    $display("tb_datapath: four boards running, and the microinstruction is on the datapath.");
    $finish;
  end

endmodule

`default_nettype wire
