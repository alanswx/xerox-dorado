// tb_run -- THE MACHINE RUNS. The Control section executes microinstruction
// cycles, started by PARC's own boot sequence.
//
// Everything before this proved the machine could be WRITTEN: the boot
// interface, all 36 microinstruction bits, the register they land in. This is
// the first test in which the Dorado does something by itself -- its clock
// runs, its phases sequence, and it reports itself running on the readout bit
// PARC's `doradoio.mdefs` calls `Stopped`.
//
// The sequence is `DoDoradoMicroInst` from `doradocpint.masm` (see
// tb_startseq.sv for the transcription), with ONE difference, and that
// difference is the whole content of this test:
//
//   ClrStop AND SetRun GO IN THE SAME CONTROL BYTE, 0x41. `rStop` is a LEVEL
//   out of the latch at ContA j02 and it lasts only until the next Control
//   strobe. PARC issues ClrStop and SetRun as separate strobes, so by the time
//   SetRun lands ClrStop has been withdrawn -- and `Stop` re-latches `dStop`
//   before the machine gets going. Issuing them together holds the stop latch
//   clear across the moment the run latch sets. Splitting them back into two
//   strobes is the mutation that fails this test, and it is the only one that
//   does.
//
// TWO THINGS THAT ARE NOT LOAD-BEARING HERE, measured rather than assumed,
// because both looked as though they would be:
//
//   * The microinstruction PARITY BITS. The extra bits of MIR1 and MIR3 are
//     the left- and right-half parity -- `sIMLH` and `sIMRH` on the board,
//     "parity P015" and "P1631" in the C emulator's comments -- and they do
//     reach two MC10170 parity generators on ContB, where `IMLHPE'` tracks the
//     MIR1 extra bit exactly. But setting either wrong does NOT stop the
//     machine in this configuration: it runs for 40,000 cycles either way. The
//     parity path evidently needs `IMLHPEenable`, which comes off the Midas
//     diagnostic-mux chain and is 0 here.
//   * The `Clock` function. `DoDoradoMicroInst` opens with
//     `DoClock(InhibitCAHolds+ClrReady)` and it is issued here for fidelity,
//     but removing it changes nothing.
//
// WHAT IT ASSERTS. That the clock runs at all; that `clk2'` runs at exactly
// twice `clk0'`, which is the two-phase relationship the whole machine is
// built on; that `Phase0` and `StartCycle'a` step once per `clk0'`; and that
// `DoradoStopped` clears. A machine that merely wiggled would fail the ratios.

`default_nettype none

module tb_run;


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
  wire mclk = (ckd >= SYSPER / 2);                     // the backplane clock

  reg [2:0] addr_n = 3'b111;
  reg [8:0] cpout  = 9'd0;
  reg       strb_n = 1'b1;
  reg       setrun = 1'b0, setss_n = 1'b1;

  dorado_control m (
      .sys_clk(sys_clk), .CLK_ca_p_(mclk), .CLK_cb_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n)
  );

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

  localparam integer RUN = 40000;
  integer i, n0, n2, np, nsc;
  reg     p0, p2, pp, psc;

  initial begin
    repeat (WT(60)) @(posedge sys_clk);

    strobe(3'd1, 8'h21, 1'b0);              // Clock: InhibitCAHolds+ClrReady
    strobe(3'd0, 8'h4E, 1'b0); setrun = 0;  // ClrStop+ClrMIR+ClrCT+Freeze
    strobe(3'd0, 8'h00, 1'b1); setss_n = 0; // undo the clears, SetSS
    strobe(3'd4, 8'h00, 1'b0);              // MIR0
    strobe(3'd5, 8'h00, 1'b1);              // MIR1, extra bit = left parity
    strobe(3'd6, 8'h00, 1'b0);              // MIR2
    strobe(3'd7, 8'h00, 1'b1);              // MIR3, extra bit = right parity
    strobe(3'd0, 8'h41, 1'b1); setrun = 1;  // ClrStop AND SetRun together

    n0 = 0; n2 = 0; np = 0; nsc = 0;
    for (i = 0; i < RUN; i = i + 1) begin
      @(posedge sys_clk);
      if (m.b_ContB.clk0_p_Bc !== p0)  begin n0  = n0  + 1; p0  = m.b_ContB.clk0_p_Bc;  end
      if (m.b_ContA.clk2_p_Bc !== p2)  begin n2  = n2  + 1; p2  = m.b_ContA.clk2_p_Bc;  end
      if (m.b_ContA.Phase0    !== pp)  begin np  = np  + 1; pp  = m.b_ContA.Phase0;     end
      if (m.StartCycle_p_a    !== psc) begin nsc = nsc + 1; psc = m.StartCycle_p_a;     end
    end

    $display("tb_run: over %0d fabric cycles -- clk0' %0d edges, clk2' %0d, Phase0 %0d, StartCycle'a %0d",
             RUN, n0, n2, np, nsc);
    $display("tb_run: DoradoStopped=%b CLKEnable'a=%b Stop=%b Run'=%b",
             m.b_ContB.DoradoStopped, m.CLKEnable_p_a, m.b_ContA.Stop,
             m.b_ContA.Run_p_);

    if (m.b_ContB.DoradoStopped !== 1'b0)
      $fatal(1, "the machine still reports itself stopped");
    if (n0 < RUN/40)
      $fatal(1, "the microinstruction clock is not running");
    if (n2 < 2*n0 - n0/8 || n2 > 2*n0 + n0/8)
      $fatal(1, "clk2' is not twice clk0' -- the two phases are not sequencing");
    if (np < n0 - n0/8 || np > n0 + n0/8)
      $fatal(1, "Phase0 does not step once per microinstruction clock");
    if (nsc < n0 - n0/8 || nsc > n0 + n0/8)
      $fatal(1, "StartCycle'a does not step once per microinstruction clock");
    $display("tb_run: the Dorado's Control section is executing cycles.");
    $finish;
  end

endmodule

`default_nettype wire
