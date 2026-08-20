// tb_firmware -- LET THE REAL FIRMWARE DRIVE THE MACHINE.
//
// Every gate up to now has had the TESTBENCH play BaseBoard: it strobed the
// control-processor bus by hand, replaying sequences transcribed out of
// `doradoboot.masm`. This one does not. It brings up BaseBd + ContA + ContB +
// ProcH + ProcL as one machine -- `dorado_boot`, generated like the others by
// tools/sil_backplane.py -- and lets the BaseBoard's 6502 run its OWN firmware
// out of its OWN EPROMs, then watches what it does to the Dorado.
//
// A DIAGNOSTIC, not a gate. WHAT IT SAYS NOW:
//
//     ROM 33818, RAM 16459, I/O 30 addresses; ROM span f248..ffff
//     MCPreClk edges 99999, divider Q21 edges 0, toggle-FF edges 1
//     WatchdogIn 1, WatchdogOut 37, XOR 36, BootMC' 36
//     reset-vector fetches: 19, every 211,440 sys_clk
//     **CPStrb' edges 37  --  IT IS DRIVING THE DORADO**
//
// The firmware reaches the control-processor bus. That took one fix, in the
// GENERATOR rather than in a cell: `WEAK_PORT_DRIVERS` in sil_to_verilog.py.
//
// WHY IT IS A NET PROPERTY, NOT A CELL ONE. A 6532 port pin is high-Z with an
// internal pull-up when its DDR makes it an input, and the MiSTer core says so
// directly -- `PA_out = out_a | ~dir_a`, an input pin reading back 1, with the
// comment that the output "must be fed back to input ... for the chip to read
// properly". That is a WIRE-AND convention; these nets resolve as wired-OR, so
// the pull-up won instead of losing and pinned every such net HIGH.
//
// Masking it inside the cell with the DDR does NOT work -- it holds the 6502
// in reset, because `WatchdogOut` is a net whose ONLY driver is the RIOT and
// whose pull-up is real. The rule has to be per-net: the pull-up loses where
// something else drives, and stands where nothing does. The generator already
// knows each net's driver set.
//
// It is sound because of what those nets ARE. Across the machine 33 nets have
// a 6532 port pin sharing with another driver, and in EVERY one the other
// driver is a real totem-pole part ('174, '259, '01, '157, '175, '74,
// MC10125) or a strap, with the 6532 pin as the READER: `RCPReg.00-15` -- how
// the BaseBoard reads the Dorado's CP register back -- plus `MCManif.0-3`,
// `TCPI.0-3`, the temperature senses and `WatchdogIn`. Every one of those was
// stuck high before.
//
// STILL OPEN, AND MEASURED. The watchdog resets the processor every 211,440
// sys_clk (19 times here), so it drives the Dorado only in bursts between
// restarts, and `DMuxClk` is still 0 -- no manifold word shifted yet.
//
// WHAT TRIPS IT IS A ONE-CYCLE GLITCH, NOT THE TIMER. The trace shows
// `WatchdogOut` driven LOW for exactly one MCClk cycle (215,518 -> 215,598)
// while the firmware sets up the RIOT -- the DDR is written before the output
// register, so the pin drives whatever ORA holds -- and that transient spikes
// the g23 XOR, pulls `BootMC'` low, and j08 latches a reset 521 sys_clk later.
// `BaseBd09.sil+3` (g22's FF1 Q') is 1 throughout, which is what lets the
// spike through: j17 NANDs the XOR against it, so Q' = 0 would mask it
// entirely.
//
// AND THAT IS CONFIRMED. Build with `+define+G22_DISARMED` -- the initial
// block below pokes g22's FF1 to Q = 1, i.e. Q' = 0, the disarmed half of its
// cycle -- and the firmware gets twelve times further:
//
//                      default      G22_DISARMED
//     reset-vector       19              7
//     CPStrb' edges      37            450
//
// So g22's power-up state genuinely gates how far the firmware gets. The
// design tolerates either state on real hardware -- FF1 is a toggle clocked by
// the timer, so it alternates armed/disarmed every interval (~1 s), and the
// firmware has a whole disarmed window to reach `PacifyWatchdog` and pin the
// XOR at 0 for good. Our simulation never gets that far because the interval
// is 2^21 cycles and the probe runs 4 M.
//
// THERE IS NO SECOND RESET SOURCE -- that was this probe's own instrument
// lying. It counted a "reset" whenever 0xFFFC appeared on the address bus,
// which is NOT the same as one: with the watchdog disarmed it reported seven
// while `MCReset'` never left 1 and `BootMC'` never moved at all. Counting
// `MCReset'` falling edges instead gives **exactly one** assertion -- the
// power-on reset -- and nothing after it. The firmware then runs continuously
// and drives the Dorado, 450 CPStrb' edges.
//
//                      default      G22_DISARMED
//     MCReset' asserts   19               1
//     CPStrb' edges      37             450
//
// g22's POWER-UP STATE MATTERS, BUT "GIVE IT TIME" IS NOT THE ANSWER -- that
// prediction was made here and is REFUTED by measurement. The reasoning was:
// FF1 is a toggle clocked by the timer, so it alternates armed/disarmed every
// interval; a machine that comes up armed should be reset only until the first
// Q21 edge flips it, after which the firmware has a whole disarmed window to
// reach `PacifyWatchdog` and pin the XOR at 0 for good. Running 260 M sys_clk
// with `+define+LONG_RUN` crosses three such edges. It does not settle:
//
//     MCPreClk cycles          3,249,999
//     divider Q21 edges                3   the timer DID fire, three times
//     g22 toggle-FF edges              3   so DISARMED WINDOWS EXISTED
//     MCReset' assertions            397   and the resets CONTINUED
//     CPStrb' edges               27,674
//
// Each disarmed window is 2^20 MCPreClk cycles -- about a million 6502 cycles
// of completely free running -- and the firmware still never pacifies. So it
// is not being starved of time; it never reaches `PacifyWatchdog` at all.
//
// WHERE IT ACTUALLY SPENDS A DISARMED WINDOW: MEASURING THE POWER SUPPLIES.
// The histogram from the `G22_DISARMED` build names one loop --
//
//     F84A: EOR $A9 / STA $0400   write a trial value to the DAC
//     F84F: LDA #$02 / STA $AA
//     F853: DEC $AA / BNE $F853   a short settling delay
//     F857: LDA ($A6),Y / AND $A8 read the comparator
//     F85B: BEQ / LDA $A9
//     F85F: EOR $0400 / ROR $A9   keep or drop the bit, shift the mask
//     F864: BNE $F84A             ... eight times
//     F86C: JMP ENABLEMIDAS
//
// -- which is a SUCCESSIVE-APPROXIMATION A/D CONVERSION. `doradoio.mdefs`
// names both ports: `DAC = 400+PA` (`DACDDRValue = AllOutput`) and
// `Comparators = 480+PA`. The surrounding code calls `READMUFFLER` and
// `SETMUFFLERADDRESS`, so the firmware is stepping the muffler address and
// converting one analog channel after another: the supply voltages and
// currents `doradomufman.masm` waits on before it will bring the Dorado up.
//
// THE COMPARATORS ARE NOT MODELLED. On the BaseBoard they are seven
// **AM2615** packages -- e23, e24, f23, f24, g24, h23, h24 -- dual
// differential line receivers (DoradoDocs/datasheets/AM2615.pdf), fourteen
// comparators, and `cell_AM2615.v` is still a "TODO: model this part"
// skeleton. So every conversion reads the same dead answer, no rail ever
// measures in range, and the firmware never leaves power sequencing --
// which is why it never reaches `PacifyWatchdog`, and why g22's power-up
// state looked like the blocker when it is a symptom.
//
// NEXT: this is a SUBSTITUTION, the same category as the VCO (see
// cell_MPQ3303 and OVERRIDE_DRIVERS) -- an analog chain has no digital model,
// so the substitute has to answer as healthy supplies would. Read what the
// firmware does with each channel (`ReadMufflerField`, the `F6DF/F6E0` table
// it indexes at F804, and the range checks after `READMUFFLER`) and make
// `cell_AM2615` return comparator results consistent with rails that are up.

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
  // A RESET IS `MCReset'` GOING LOW, not 0xFFFC appearing on the address bus.
  // Counting the address was wrong: with the watchdog disarmed it reported
  // seven "resets" while MCReset' never left 1 and BootMC' never moved.
  integer n_mcreset = 0, last_mcreset = 0;
  reg p_mcreset = 1'b1;
  integer n_mcclk = 0, n_wdin = 0, n_bootmc = 0;
  integer n_pre = 0, n_div = 0, n_tog = 0, n_xor = 0, n_wdout = 0;
  reg p_pre = 1'b0, p_div = 1'b0, p_tog = 1'b0, p_xor = 1'b0, p_wdout = 1'b0;
  reg p_mcclk = 1'b0, p_wdin = 1'b0, p_bootmc = 1'b1;
  integer mcclk_at_reset = 0;

  initial begin
    // EXPERIMENT: does g22's toggle flip-flop coming up with Q=1 (so Q'=0,
    // watchdog DISARMED until the first timer interval) let the firmware run?
    // The cell powers up Q=0, hence Q'=1, which lets a transient XOR spike
    // through to BootMC'.
`ifdef G22_DISARMED
    m.u_machine.b_BaseBd.u_g22.qa = 1'b1;
`endif
    for (i = 0; i < 16; i = i + 1) bucket[i] = 0;
    for (i = 0; i < 4096; i = i + 1) hot[i] = 0;

    // Long enough to cross a real watchdog interval (2^21 MCPreClk cycles at
    // 80 sys_clk each = ~168 M) with +define+LONG_RUN.
`ifdef LONG_RUN
    for (i = 0; i < 260_000_000; i = i + 1) begin
`else
    for (i = 0; i < 4_000_000; i = i + 1) begin
`endif
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
          if (n_reset > 1 && n_reset < 9)
            $display("tb_firmware:   RESET #%0d at %0d (+%0d): WatchdogIn=%b WatchdogOut=%b XOR=%b g22Q'=%b BootMC'=%b",
                     n_reset, i, i - last_reset,
                     `BB.WatchdogIn, `BB.WatchdogOut, `BB.BaseBd09_sil_pl_1,
                     `BB.BaseBd09_sil_pl_3, `BB.BootMC_p_);
          if (n_reset > 1 && n_reset < 9)
            $display("tb_firmware:            PwrGood=%b MCReset'=%b g22FF1Q=%b",
                     `BB.PwrGood, `BB.MCReset_p_, m.u_machine.b_BaseBd.u_g22.qa);
          last_reset = i;
          mcclk_at_reset = n_mcclk;
        end
      end
      // Does it touch the Dorado at all?
      if (`BB.MCReset_p_ !== p_mcreset) begin
        p_mcreset = `BB.MCReset_p_;
        if (!p_mcreset) begin
          n_mcreset = n_mcreset + 1;
          if (n_mcreset < 6)
            $display("tb_firmware:   MCReset' ASSERTED #%0d at %0d (+%0d)",
                     n_mcreset, i, i - last_mcreset);
          last_mcreset = i;
        end
      end
      if (`BB.MCPreClk !== p_pre) begin p_pre = `BB.MCPreClk; n_pre = n_pre + 1; end
      if (`BB.BaseBd09_sil_pl_8 !== p_div) begin p_div = `BB.BaseBd09_sil_pl_8; n_div = n_div + 1; end
      if (`BB.BaseBd09_sil_pl_3 !== p_tog) begin p_tog = `BB.BaseBd09_sil_pl_3; n_tog = n_tog + 1; end
      if (`BB.BaseBd09_sil_pl_1 !== p_xor) begin p_xor = `BB.BaseBd09_sil_pl_1; n_xor = n_xor + 1; end
      if (`BB.WatchdogOut !== p_wdout) begin
        p_wdout = `BB.WatchdogOut; n_wdout = n_wdout + 1;
        if (n_wdout < 8)
          $display("tb_firmware:   WatchdogOut -> %b at %0d (In=%b XOR=%b q22Q'=%b BootMC'=%b)",
                   `BB.WatchdogOut, i, `BB.WatchdogIn, `BB.BaseBd09_sil_pl_1,
                   `BB.BaseBd09_sil_pl_3, `BB.BootMC_p_);
      end
      if (`BB.MCClk !== p_mcclk) begin p_mcclk = `BB.MCClk; n_mcclk = n_mcclk + 1; end
      if (`BB.WatchdogIn !== p_wdin) begin p_wdin = `BB.WatchdogIn; n_wdin = n_wdin + 1; end
      if (`BB.BootMC_p_ !== p_bootmc) begin
        p_bootmc = `BB.BootMC_p_; n_bootmc = n_bootmc + 1;
        if (n_bootmc < 10)
          $display("tb_firmware:   BootMC' -> %b at %0d (XOR=%b g22FF1Q'=%b WdIn=%b WdOut=%b)",
                   `BB.BootMC_p_, i, `BB.BaseBd09_sil_pl_1, `BB.BaseBd09_sil_pl_3,
                   `BB.WatchdogIn, `BB.WatchdogOut);
      end
      if (m.u_machine.CPStrb_p_ !== p_cpstrb) begin p_cpstrb = m.u_machine.CPStrb_p_; n_cpstrb = n_cpstrb + 1; end
      if (m.u_machine.DMuxClk   !== p_dmuxclk) begin p_dmuxclk = m.u_machine.DMuxClk;  n_dmuxclk = n_dmuxclk + 1; end
    end

    $display("tb_firmware: %0d sys_clk of the REAL firmware running.", i);
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
    $display("tb_firmware: MCReset' ASSERTIONS: %0d   (0xFFFC seen on the bus %0d times -- not the same thing)",
             n_mcreset, n_reset);
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
