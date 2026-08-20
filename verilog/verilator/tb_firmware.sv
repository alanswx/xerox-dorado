// tb_firmware -- LET THE REAL FIRMWARE DRIVE THE MACHINE.
//
// Every gate up to now has had the TESTBENCH play BaseBoard: it strobed the
// control-processor bus by hand, replaying sequences transcribed out of
// `doradoboot.masm`. This one does not. It brings up BaseBd + ContA + ContB +
// ProcH + ProcL as one machine -- `dorado_boot`, generated like the others by
// tools/sil_backplane.py -- and lets the BaseBoard's 6502 run its OWN firmware
// out of its OWN EPROMs, then watches what it does to the Dorado.
//
// A DIAGNOSTIC, not a gate, and here is what it currently says:
//
//     bus activity -- 33818 ROM, 16459 RAM/zero-page, 30 I/O addresses
//     ROM addresses touched span f248..ffff
//     the ten ROM addresses it visits most: f3ac..f3b1, 4864 visits each
//     reset-vector fetches: 19
//     CPStrb' edges 0, DMuxClk edges 0
//
// SO THE FIRMWARE RUNS, AND THE WATCHDOG KEEPS RESETTING IT. `f3ac..f3b1` is
// the zero-page clear loop at the top of the reset routine
// (`STA $00,X / DEX / BNE $F3AC`), and 4864 visits is 19 x 256 -- one pass per
// reset. The resets are exactly periodic: **every 211,440 sys_clk**, which is
// a hardware timer, not a firmware loop. It never reaches the Dorado at all --
// no CPReg strobe, no manifold shift.
//
// THE HANDSHAKE IT IS FAILING is `PacifyWatchdog` in `doradocontinuous.masm`:
//
//     LDA Watchdog / ANDI WatchdogIn / RORA  ; carries WatchdogIn to WatchdogOut
//     STA WatchdogTemp / SEI / LDA Watchdog
//     ANDI 0ff-WatchdogOut / ORA WatchdogTemp / STA Watchdog / CLI
//
// and `doradoio.mdefs` gives the register: `Watchdog = 600+PA` -- a 6532 RIOT
// port -- with `WatchdogIn = 80` (bit 7, an input) and `WatchdogOut = 40`
// (bit 6, an output, per `WatchdogDDRValue`). The hardware drives bit 7 and
// expects the firmware to echo it on bit 6; when the echo stops, it resets
// the processor.
//
// NEXT: find out whether our RTL models that timer and its echo at all, and if
// it does, why the firmware is not satisfying it -- in 211,440 sys_clk it only
// gets through the reset routine and into MIDASSETUP (`JSR $F248`, which is
// where the f254..f257 visits come from), so it may simply never reach a
// pacify call. Either way this is now ONE named blocker between the real
// firmware and the machine, rather than a guess about power sequencing.

`default_nettype none
`define BB  m.u_machine.b_BaseBd

module tb_firmware;

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;

  dorado_boot_machine m (.sys_clk(sys_clk), .probe_sel(16'd0),
                         .probe_val(), .probe_words());

  wire [15:0] mca = {`BB.MCA_15, `BB.MCA_14, `BB.MCA_13, `BB.MCA_12,
                     `BB.MCA_11, `BB.MCA_10, `BB.MCA_09, `BB.MCA_08,
                     `BB.MCA_07, `BB.MCA_06, `BB.MCA_05, `BB.MCA_04,
                     `BB.MCA_03, `BB.MCA_02, `BB.MCA_01, `BB.MCA_00};

  integer i;
  integer n_cpstrb = 0, n_manclk = 0, n_dmuxclk = 0;
  reg     p_cpstrb = 1'b1, p_manclk = 1'b1, p_dmuxclk = 1'b0;
  reg [15:0] pc_lo = 16'hFFFF, pc_hi = 16'h0000, last_a = 16'h0;
  integer n_rom = 0, n_ram = 0, n_io = 0;

  // Where does it SPEND its time? A 16-bucket histogram over the 64K space is
  // enough to say "parked in ROM around F3xx" versus "walking RAM".
  integer bucket [0:15];
  integer hot [0:4095];      // per-address counts for the F000..FFFF page
  integer j, best, besta;
  integer n_reset = 0, last_reset = 0;

  initial begin
    for (i = 0; i < 16; i = i + 1) bucket[i] = 0;
    for (i = 0; i < 4096; i = i + 1) hot[i] = 0;

    for (i = 0; i < 4_000_000; i = i + 1) begin
      @(posedge sys_clk);
      if (mca !== last_a) begin
        last_a = mca;
        bucket[mca[15:12]] = bucket[mca[15:12]] + 1;
        if (mca < 16'h1000)                        n_ram = n_ram + 1;
        else if (mca >= 16'hC000)                  n_rom = n_rom + 1;
        else                                       n_io  = n_io + 1;
        if (mca >= 16'hC000) begin
          if (mca < pc_lo) pc_lo = mca;
          if (mca > pc_hi) pc_hi = mca;
        end
        if (mca >= 16'hF000) hot[mca - 16'hF000] = hot[mca - 16'hF000] + 1;
        if (mca == 16'hFFFC) begin
          n_reset = n_reset + 1;
          if (n_reset > 1)
            $display("tb_firmware:   RESET #%0d at sys_clk %0d (%0d since the last)",
                     n_reset, i, i - last_reset);
          last_reset = i;
        end
      end
      // Does it touch the Dorado at all?
      if (m.u_machine.CPStrb_p_ !== p_cpstrb) begin p_cpstrb = m.u_machine.CPStrb_p_; n_cpstrb = n_cpstrb + 1; end
      if (m.u_machine.DMuxClk   !== p_dmuxclk) begin p_dmuxclk = m.u_machine.DMuxClk;  n_dmuxclk = n_dmuxclk + 1; end
    end

    $display("tb_firmware: 4,000,000 sys_clk of the REAL firmware running.");
    $display("tb_firmware: bus activity -- %0d ROM, %0d RAM/zero-page, %0d I/O addresses",
             n_rom, n_ram, n_io);
    $display("tb_firmware: ROM addresses touched span %04h..%04h", pc_lo, pc_hi);
    $display("tb_firmware: page histogram (16 buckets of 4K):");
    for (i = 0; i < 16; i = i + 1)
      if (bucket[i] != 0)
        $display("tb_firmware:   %01hxxx : %0d", i, bucket[i]);
    $display("tb_firmware: the ten ROM addresses it visits most (i.e. the loop):");
    for (i = 0; i < 10; i = i + 1) begin
      best = 0; besta = 0;
      for (j = 0; j < 4096; j = j + 1)
        if (hot[j] > best) begin best = hot[j]; besta = j; end
      if (best > 0) begin
        $display("tb_firmware:   %04h  %0d visits", 16'hF000 + besta[15:0], best);
        hot[besta] = 0;
      end
    end
    $display("tb_firmware: reset-vector fetches: %0d -- the processor is RESTARTING, not looping.", n_reset);
    $display("tb_firmware: I/O addresses touched: %0d distinct", n_io);
    $display("tb_firmware: CPStrb' edges %0d, DMuxClk edges %0d", n_cpstrb, n_dmuxclk);
    if (n_cpstrb == 0 && n_dmuxclk == 0)
      $display("tb_firmware: it has NOT reached the Dorado yet -- still in the BaseBoard's own code.");
    else
      $display("tb_firmware: IT IS DRIVING THE DORADO.");
    $finish;
  end
endmodule
`default_nettype wire
