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
// THE RESET CHAIN IS FULLY TRACED AND NOW FULLY MEASURED. Every part of it is
// in the RTL, and as of this session the timer is actually modelled:
//
//   g21   MC14521B 24-stage divider -- THE WATCHDOG TIMER. Reset tied low,
//         `MCPreClk` into In2, and **Q21 on pin 13** out to `BaseBd09.sil+8`.
//         It was an unmodelled skeleton until now; the cell is written from
//         the data sheet (DoradoDocs/datasheets/MC14521B.pdf).
//   g22   SN74LS74 wired as a TOGGLE flip-flop (1D tied to 1Q'), clocked by
//         that Q21 -- so it flips once per watchdog interval
//   g23   SN7486 XOR of `WatchdogIn` against `WatchdogOut` -- the pacify
//         comparison itself
//   j17   SN74LS01 open-collector NAND of those two, driving `BootMC'`,
//         wire-ORed with a jumper strap at h07
//   j08   SN74LS74, D = `BootMC'`, clock = `MCClk`, async `PwrGood`, output
//         `MCReset'`
//   f63   MCS6532 RIOT: PA7 out as `WatchdogIn`, PA6 in as `WatchdogOut`
//
// WHAT IT MEASURES NOW, and it is no longer the timer:
//
//     MCPreClk edges 99999, divider Q21 edges 0, toggle-FF edges 1
//     WatchdogIn 1, WatchdogOut 37, XOR 36, BootMC' 36
//     resets: 19, every 211,440 sys_clk (= 2,643 MCClk cycles)
//
// MCPreClk runs at one cycle per 80 sys_clk, so Q21 -- a divide by 2^21 -- is
// CORRECTLY quiet across the whole run: the watchdog interval is of order a
// second and should not fire here at all. The resets track `WatchdogOut`
// one-for-one instead. The firmware writes that port during startup, each
// write flips the XOR, and the XOR reaches `BootMC'` unopposed.
//
// THE BLOCKER IS `BootMC'`, AND THE 6532's INPUT-PIN CONVENTION IS WHY.
// Measured in the one-board machine: MCReset'=0, PwrGood=1, TTLTrue.E=1 -- the
// power-up gate is fine and BootMC' is simply LOW. j17 NANDs the g23 XOR
// against g22's Q', so BootMC' is low exactly when WatchdogIn != WatchdogOut
// and g22's Q' is 1.
//
// A CORRECTION. An earlier note here said masking the 6532's port drive with
// its DDR "fixes the reset storm, 19 fetches to ZERO". IT DOES NOT. Zero
// reset-vector fetches means the 6502 NEVER STARTED -- held in reset, which is
// worse than restarting -- and the fffe/ffff hits that looked like an IRQ
// storm are what the bus does with the processor held down.
//
// THE REAL SHAPE OF THE FIX. `PA_out = out_a | ~dir_a` in the core is not a
// bug: it is THE PULL-UP OF A HIGH-Z INPUT PIN, and a 6532 port pin set as an
// input really does present a 1. That is RIGHT for `WatchdogOut`, whose only
// driver is the RIOT (f63.14) -- mask it and the net reads 0, the XOR goes 1,
// and BootMC' sticks low. It is WRONG for `WatchdogIn`, which g22 also drives
// (g22.8, a totem-pole '74 output that in reality beats a pull-up), because in
// a wired-OR net model the RIOT's 1 wins and the watchdog is invisible.
//
// Neither net has a physical pull-up resistor; the pull-up is inside the 6532.
// So THE FIX IS PER-NET, NOT PER-CELL, and belongs in the generator: a 6532
// port pin should contribute `out & dir` where the net has another driver, and
// its pull-up where it is the sole driver. tools/sil_backplane.py already
// knows each net's driver set.
//
// SO THE TOGGLE FLIP-FLOP'S POWER-UP STATE MATTERS TOO.
// SO THE REMAINING QUESTION IS THE TOGGLE FLIP-FLOP'S POWER-UP STATE. j17
// NANDs the XOR against g22's Q', so a Q' of 0 masks the XOR entirely and only
// a real timer expiry can reset the processor -- which is the whole point of
// the design. Our SN74LS74 comes up with Q = 0, hence Q' = 1, which arms the
// watchdog from the first instruction. On the real board g22 has neither its
// preset nor its clear asserted (pins 1 and 4 both go to `TTLTrue.E`), so the
// state is set by the power-up sequence -- `PwrGood`, which is also j08's
// asynchronous input. That sequencing is the thing to model next.

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
  integer n_mcclk = 0, n_wdin = 0, n_bootmc = 0;
  integer n_pre = 0, n_div = 0, n_tog = 0, n_xor = 0, n_wdout = 0;
  reg p_pre = 1'b0, p_div = 1'b0, p_tog = 1'b0, p_xor = 1'b0, p_wdout = 1'b0;
  reg p_mcclk = 1'b0, p_wdin = 1'b0, p_bootmc = 1'b1;
  integer mcclk_at_reset = 0;

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
            $display("tb_firmware:   RESET #%0d at sys_clk %0d (%0d since the last, %0d MCClk edges)",
                     n_reset, i, i - last_reset, n_mcclk - mcclk_at_reset);
          last_reset = i;
          mcclk_at_reset = n_mcclk;
        end
      end
      // Does it touch the Dorado at all?
      if (`BB.MCPreClk !== p_pre) begin p_pre = `BB.MCPreClk; n_pre = n_pre + 1; end
      if (`BB.BaseBd09_sil_pl_8 !== p_div) begin p_div = `BB.BaseBd09_sil_pl_8; n_div = n_div + 1; end
      if (`BB.BaseBd09_sil_pl_3 !== p_tog) begin p_tog = `BB.BaseBd09_sil_pl_3; n_tog = n_tog + 1; end
      if (`BB.BaseBd09_sil_pl_1 !== p_xor) begin p_xor = `BB.BaseBd09_sil_pl_1; n_xor = n_xor + 1; end
      if (`BB.WatchdogOut !== p_wdout) begin p_wdout = `BB.WatchdogOut; n_wdout = n_wdout + 1; end
      if (`BB.MCClk !== p_mcclk) begin p_mcclk = `BB.MCClk; n_mcclk = n_mcclk + 1; end
      if (`BB.WatchdogIn !== p_wdin) begin p_wdin = `BB.WatchdogIn; n_wdin = n_wdin + 1; end
      if (`BB.BootMC_p_ !== p_bootmc) begin p_bootmc = `BB.BootMC_p_; n_bootmc = n_bootmc + 1; end
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
    $display("tb_firmware: MCClk edges %0d in 4,000,000 sys_clk -> 1 MCClk cycle = %0d sys_clk",
             n_mcclk, (n_mcclk > 1) ? (4000000*2)/n_mcclk : 0);
    $display("tb_firmware: MCPreClk edges %0d, divider Q21 edges %0d, toggle-FF edges %0d",
             n_pre, n_div, n_tog);
    $display("tb_firmware: WatchdogIn edges %0d, WatchdogOut edges %0d, XOR edges %0d, BootMC' edges %0d",
             n_wdin, n_wdout, n_xor, n_bootmc);
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
