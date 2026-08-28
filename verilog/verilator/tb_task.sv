// tb_task -- THE PRIORITY ENCODER: which task runs next.
//
// The first gate on the Dorado's 16-way hardware tasking, and the first thing
// that has to be true of it: given a set of wakeup requests, the machine must
// pick the right task.
//
// HOW THE REQUESTS GET HERE, because it is not obvious from the netlists and
// it is why this could not be tested before. Every I/O board puts its wakeup
// on the SAME two connector pins, C120 and C121, under its own local name --
// DispY calls them `WakeDWT`/`WakeDHT`, DispM `WakeAWT`/`WakeAHT`, DskEth
// `WakeEthRx`/`WakeEthTx` -- while ContA receives fifteen lines
// `TWReq.01`..`TWReq.15` on pins of its own (C44..C140). Nothing matches, by
// name or by pin. `Backplane.pdf` (BPRight04.sil, 7/24/80) shows why: pins
// 120/121 carry `TWReq.xx*` across the generic I/O slots, annotated "for
// desired Task wake up". THE ROUTING IS A JUMPER, chosen when the machine is
// wired, so a board's task number is a property of its SLOT -- which is
// exactly why each board also carries a task-number STRAP, and why the strap
// is the authority for which line the jumper takes. See
// BACKPLANE_WAKEUP_JUMPERS in tools/sil_netlist.py.
//
// WHAT THIS CHECKS. ContA's a10/b10/c10/d10 (four MC1664 packages, 4+4+4+3 =
// fifteen inputs) reduce the requests to `BNT`, the best next task; h14, an
// MC10159 quad 2:1 mux, then selects between BNT and CTask under `Switcha` to
// make `Next`. The rule the C emulator implements, written from the Hardware
// Manual years earlier and from a document rather than a netlist, is
//
//     BNT = the highest-numbered task with a request; task 0 is always
//           available, so worst case BNT = 0                (cpu.c task_bnt)
//     switch iff BNT > CTASK                                (cpu.c line 1015)
//
// This gates the COMBINATIONAL half -- `PEnc`, and its buffered copy `bPEnc`,
// which is what the machine actually encodes from the fifteen request lines.
// Every single request, then multi-request patterns where only the highest may
// win, all against cpu.c's rule.
//
// `BNT` itself is NOT asserted here, and the reason is worth writing down:
// h08/h09 are MC10141 SHIFT REGISTERS whose parallel-load mode needs
// `BNextRegsEn'` low on both select pins, clocked by `clk0'` -- the
// microinstruction clock. So BNT only moves when the machine is RUNNING, and a
// static bench reads it as 0 no matter what the requests are. Asserting on
// BNT here would gate nothing. Wiring it up is the next step and belongs with
// a running-machine testbench, alongside `Switcha` and `BNTGtCT'`.

`default_nettype none

module tb_task;


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

  reg [15:1] req = 15'd0;

  // BNT is a REGISTER, not a combinational encode: ContA h08/h09 are MC10141
  // shift registers, so the board clock has to run for a request to reach it.
  // (a10/b10/c10/d10 are the MC1664 NORs ahead of it, turning TWReq into
  // `ToPE`, "to priority encoder".) The other Control testbenches hold these
  // static on purpose -- tb_mirreg does, because a running microinstruction
  // clock would reload every MIR bit -- which is why this one cannot copy them.
  reg [3:0] ckd = 4'd0;
  // At SYSPER=16 this is bit for bit the counter it replaces:
  // ckd counts 0..15 and mclk is ckd >= 8, i.e. ckd[3].
  always @(posedge sys_clk)
    ckd <= (ckd == SYSPER - 1) ? 4'd0 : ckd + 4'd1;
  wire mclk = (ckd >= SYSPER / 2);

  dorado_control m (
      .sys_clk(sys_clk), .CLK_ca_p_(mclk), .CLK_cb_p_(mclk),
      .CPAddr_0_p_(1'b1), .CPAddr_1_p_(1'b1), .CPAddr_2_p_(1'b1),
      .CPOut_0(1'b0), .CPOut_1(1'b0), .CPOut_2(1'b0), .CPOut_3(1'b0),
      .CPOut_4(1'b0), .CPOut_5(1'b0), .CPOut_6(1'b0), .CPOut_7(1'b0),
      .CPOut_8(1'b0), .CPStrb_p_(1'b1),
      .TWReq_01(req[1]),  .TWReq_02(req[2]),  .TWReq_03(req[3]),
      .TWReq_04(req[4]),  .TWReq_05(req[5]),  .TWReq_06(req[6]),
      .TWReq_07(req[7]),  .TWReq_08(req[8]),  .TWReq_09(req[9]),
      .TWReq_10(req[10]), .TWReq_11(req[11]), .TWReq_12(req[12]),
      .TWReq_13(req[13]), .TWReq_14(req[14]), .TWReq_15(req[15])
  );

  // PARC numbers a field MSB first, so BNT.0 is the top bit.
  wire [3:0] bnt   = {m.b_ContA.BNT_0, m.b_ContA.BNT_1,
                      m.b_ContA.BNT_2, m.b_ContA.BNT_3};
  wire [3:0] penc  = {m.b_ContA.PEnc_0, m.b_ContA.PEnc_1,
                      m.b_ContA.PEnc_2, m.b_ContA.PEnc_3};
  wire [3:0] bpenc = {m.b_ContA.bPEnc_0, m.b_ContA.bPEnc_1,
                      m.b_ContA.bPEnc_2, m.b_ContA.bPEnc_3};

  // cpu.c task_bnt(): highest bit set, task 0 always available.
  function [3:0] want_bnt(input [15:1] r);
    integer k;
    begin
      want_bnt = 4'd0;
      for (k = 15; k >= 1; k = k - 1)
        if (want_bnt == 4'd0 && r[k]) want_bnt = k[3:0];
    end
  endfunction

  integer i, bad;
  reg [15:1] pat;

  task check(input [15:1] r, input [200*8-1:0] why);
    begin
      req = r;
      repeat (WT(40)) @(posedge sys_clk);
      if (penc !== want_bnt(r) || bpenc !== want_bnt(r)) begin
        $display("tb_task: FAIL req=%b -> PEnc %0d bPEnc %0d, want %0d  (%0s)",
                 r, penc, bpenc, want_bnt(r), why);
        bad = bad + 1;
      end
    end
  endtask

  initial begin
    bad = 0;
    repeat (WT(200)) @(posedge sys_clk);

    // No request at all: task 0, the emulator, which is always available.
    check(15'd0, "idle");
    $display("tb_task: no requests -> BNT %0d (the emulator)", bnt);

    // Each task alone.
    for (i = 1; i <= 15; i = i + 1) begin
      pat = 15'd0; pat[i] = 1'b1;
      check(pat, "single request");
    end
    $display("tb_task: fifteen single requests, each selects its own task");

    // Highest wins. Each of these has several requesters.
    check(15'b000000000000011, "1 and 2");
    check(15'b000000000001111, "1..4");
    check(15'b000000011111111, "1..8");
    check(15'b111111111111111, "all fifteen");
    check(15'b100000000000001, "1 and 15 -- the fault task must win");
    check(15'b010000000000001, "1 and 14");
    check(15'b000010001000100, "3, 7 and 12");
    $display("tb_task: with several requesting, the highest-numbered wins");

    if (bad != 0) begin
      $display("tb_task: FAIL -- %0d patterns disagree with cpu.c task_bnt()", bad);
      $fatal(1);
    end
    $display("tb_task: PASS -- BNT matches the C emulator on 23 patterns");
    $finish;
  end

endmodule
