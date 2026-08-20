// tb_firmware -- LET THE REAL FIRMWARE DRIVE THE MACHINE.
//
// Every other gate here has the TESTBENCH play BaseBoard, strobing the
// control-processor bus by hand from sequences transcribed out of
// `doradoboot.masm`. This one does not. It brings up BaseBd + ContA + ContB +
// ProcH + ProcL as one machine -- `dorado_boot`, generated like the other
// configurations by tools/sil_backplane.py -- and lets the BaseBoard's 6502
// run its OWN firmware out of its OWN EPROMs, then watches what it does.
//
// A DIAGNOSTIC, not a gate. Two build-time switches:
//   +define+LONG_RUN       260 M sys_clk instead of 4 M, enough to cross a
//                          real watchdog interval (2^21 MCPreClk cycles)
//   +define+G22_DISARMED   pokes g22's FF1 to Q=1 at time zero, the disarmed
//                          half of the watchdog's cycle -- an EXPERIMENT, to
//                          separate "reset by the watchdog" from everything
//                          else. Off by default.
//
// ---------------------------------------------------------------- WHAT IT SAYS
//
//                         4 M armed   4 M disarmed   260 M armed
//   MCReset' assertions      19            1            397
//   CPStrb' edges            37          450         27,674
//   PACIFYWATCHDOG F692       0            5            240
//   DMuxClk edges             0            0              0
//
// THE BASEBOARD BOOTS ITSELF PAST POWER-UP. In the 260 M run, resets bucketed
// by watchdog window (each Q21 edge starts a bucket):
//
//   window 0  from           0   g22FF1Q'=1 ARMED      resets 397
//   window 1  from  83,886,119   g22FF1Q'=1 ARMED      resets   0
//   window 2  from 167,772,199   g22FF1Q'=0 disarmed   resets   0
//   window 3  from 251,658,279   g22FF1Q'=0 disarmed   resets   0
//
// EVERY reset is in window 0. After the first watchdog interval there are
// none, across 176 M sys_clk -- including window 1, which is still ARMED. The
// firmware gets far enough to pacify, the XOR stays 0 from then on, and the
// watchdog is satisfied for the rest of the run. That is the design working.
//
// ------------------------------------------- WHAT MADE IT REACH THE DORADO
//
// One fix, and it belongs in the GENERATOR rather than a cell:
// `WEAK_PORT_DRIVERS` in tools/sil_to_verilog.py, symmetric to the existing
// `OVERRIDE_DRIVERS`.
//
// A 6532 port pin is high-Z with an internal pull-up when its DDR makes it an
// input, and the core states that directly -- `PA_out = out_a | ~dir_a`, an
// input pin reading back 1, with the comment that the output "must be fed back
// to input ... for the chip to read properly". That is a WIRE-AND convention;
// these nets resolve as WIRED-OR, so the pull-up won instead of losing and
// pinned every such net HIGH.
//
// It has to be per-NET, not per-cell. Masking inside the cell with the DDR
// holds the 6502 in reset, because `WatchdogOut` is a net whose ONLY driver is
// the RIOT and whose pull-up is real. The rule is: the pull-up loses where
// something else drives, and stands where nothing does -- which is information
// the generator already has.
//
// It is sound because of what those nets are. Across the machine 33 nets have
// a 6532 port pin sharing with another driver, and in EVERY one the other
// driver is a real totem-pole part ('174, '259, '01, '157, '175, '74,
// MC10125) or a strap, with the 6532 pin as the READER: `RCPReg.00-15` -- how
// the BaseBoard reads the Dorado's CP register back -- plus `MCManif.0-3`,
// `TCPI.0-3`, the temperature senses and `WatchdogIn`. All were stuck high.
//
// ------------------------------------------------------------- THE WATCHDOG
//
// The chain, every part of it in the RTL:
//
//   g21  MC14521B 24-stage divider -- THE TIMER. Reset tied low, `MCPreClk`
//        into In2, Q21 (pin 13) out to `BaseBd09.sil+8`. So the interval is
//        MCPreClk / 2^21, of order a second. (It was an unmodelled skeleton
//        until this was written; the cell comes from the data sheet.)
//   g22  SN74LS74 pair, both wired as TOGGLES. FF1 is clocked by that Q21, so
//        it alternates ARMED (Q'=1) and disarmed (Q'=0) every interval.
//   g23  SN7486 XOR of `WatchdogIn` against `WatchdogOut` -- the pacify test.
//   j17  SN74LS01 open-collector NAND of those two, driving `BootMC'`,
//        wire-ORed with a jumper strap at h07.
//   j08  SN74LS74: D = `BootMC'`, clock = `MCClk`, async `PwrGood`, output
//        `MCReset'`.
//   f63  MCS6532 RIOT: PA7 out as `WatchdogIn`, PA6 in as `WatchdogOut`.
//        `PacifyWatchdog` (doradocontinuous.masm) echoes one onto the other.
//
// WHAT TRIPS IT IS A ONE-CYCLE GLITCH, NOT THE TIMER. `WatchdogOut` is driven
// LOW for exactly one MCClk cycle (215,518 -> 215,598) while the firmware sets
// up the RIOT -- the DDR is written before the output register, so the pin
// drives whatever ORA holds. That transient spikes the g23 XOR, pulls
// `BootMC'` low, and j08 latches a reset 521 sys_clk later. It only gets
// through because g22's FF1 Q' is 1; with the machine in its disarmed half,
// j17 masks the XOR entirely and the glitch is harmless. Hence the 397 resets
// confined to window 0, and none afterwards once the firmware pacifies.
//
// `SkipWait'` reads 1, which WOULD make `PacifyWatchdogIfJumper` skip -- but
// that entry is never used; the firmware calls `PacifyWatchdog` directly.
//
// -------------------------------------------------------- THE MANIFOLD PATH
//
// Traced by ROUTINE, in the disarmed build where the firmware runs freely:
//
//   SETMANIFOLD              F95A    4 visits
//   SETMUFFLERADDRESS        F9D0    4
//   TRYGETTINGMUFMANCONTROL  FA0E   52
//   WAITFORCPCONTROL         FA1F    4
//   DATUMTOMANIFOLD          F977    0   (a different entry; unused here)
//   TSetRun                          0   the MufMan gate PASSES
//
// `SetManifold` calls `SetMufflerAddress`, which calls
// `TryGettingMufManControl` and gives up on carry set. That routine reads
// `MCPBusL & SetRunIn` (bit 2, the net `TSetRun`) and fails when the bit is
// SET -- it reads 0, so the gate passes and the routine proceeds.
//
// The shift does not use the `DMux*` nets. `SetMufflerAddress`'s inner loop at
// F9F6 sets `MCPBusL = $10` -- function code 1, `Clock` -- then writes
// `MCPBusH` and pulses `MCPStrb` twelve times. i62 is that RIOT: PA drives
// `MCPBus.00-07`, PB7 `MCPBus.08`, PB6-4 `MCPABus.0-2`, PB0 `MCPStrb`. A
// manifold word therefore goes out as ordinary CP-bus transactions, and the
// BaseBoard's own k22/k17 decode them into `CPDMuxData`/`CPDMuxClk`, through
// l19 and the l24 TTL-to-ECL translator, onto the backplane as
// `DMuxData`/`DMuxClk`.
//
// ------------- AND THE CP-BUS TRAFFIC IS NOT THE MANIFOLD SHIFT -------------
//
// Measured at every `MCPStrb` rising edge, function code read MSB-FIRST as
// PARC numbers it ({MCPABus.0, .1, .2}):
//
//     MCPStrb rising edges 225      fn 0 (Control) 116, fn 7 (MIR3) 109
//     CPDMuxClk edges 0, CPDMuxData edges 0
//
//     strobe 2 at 1,424,958 (last ROM addr ff82): fn=0 data=001000000
//     strobe 3 at 1,425,438 (last ROM addr ff82): fn=7 data=101000000
//     strobe 4 at 1,426,398 (last ROM addr ff82): fn=0 data=000000000
//     ... alternating fn 0 / fn 7, last ROM address ff82 EVERY TIME
//
// NO `Clock` STROBE (fn 1) IS EVER SENT -- which is what `SetMufflerAddress`
// would produce -- and `FF7C..FF87` is all `00`, i.e. BRK: UNUSED FILLER ROM.
// The processor is fetching from filler at every one of these strobes, and
// `FF00` (1024 visits) and `FF80` (669) are among the hottest addresses in the
// run. The `fffe`/`ffff` traffic noticed earlier is consistent with BRK
// vectoring rather than with an interrupt storm.
//
// SO "IT IS DRIVING THE DORADO" OVERSTATES THIS. The strobes are real CP-bus
// transactions and the count is real, but they are Control and MIR3 only, none
// of them the manifold shift, and the PC is in unused ROM when they happen.
// What is established is narrower: the BaseBoard boots, survives its watchdog,
// reaches `SetManifold` four times with the MufMan gate passing -- and then
// execution ends up in filler.
//
// NEXT: find where it leaves real code. Log the last ROM address before the
// first fetch at `FF00`/`FF80`, or watch for the first fetch outside the
// routines the disassembly names. A measurement, not a theory, and it should
// come before any further claim about what the firmware is doing.
//
// ---------------------------------------------------- ALSO WORTH KNOWING
//
// The firmware's hot loop (F84A..F864) is a SUCCESSIVE-APPROXIMATION A/D
// conversion: write `DAC = 400+PA`, settle, read `Comparators = 480+PA`,
// shift. Its analog chain is unmodelled -- CA3140 op-amps (g18, i19, i20,
// i21, j21), CD4051 analog muxes (i2125, j24, k24), AUGATCG16 resistor
// platforms. That is a fact about the RTL; it is a CYCLE COUNT and not a
// proven blocker, since the firmware demonstrably proceeds past it.
//
// THREE INSTRUMENT TRAPS THIS PROBE WALKED INTO, all the same shape -- a proxy
// measured instead of the thing, each of which produced a confident and wrong
// conclusion that survived several rounds of reasoning:
//
//   * counting `0xFFFC` on the address bus as a RESET. It is not one; the
//     firmware reads ROM there. Count `MCReset'` falling edges.
//   * reading a TOTAL as a DISTRIBUTION. "397 resets over 260 M" looked like a
//     machine that never settles; bucketed by window, all 397 are in the first
//     one and the machine settles permanently after it.
//   * inferring the manifold path from a plausible-looking net (`DMuxClk`)
//     instead of reading what the firmware WRITES. The path is the CP bus.

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
  reg [15:0] last_rom = 16'h0;
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
  // Resets bucketed by watchdog window: each Q21 edge flips g22's FF1, so the
  // machine alternates armed (Q'=1) and disarmed (Q'=0). This asks directly
  // whether the 397 resets of the long run cluster in the armed halves.
  integer win = 0;
  integer win_start [0:15];
  integer win_resets [0:15];
  reg     win_armed  [0:15];
  reg     p_q21 = 1'b0;
  integer n_mcclk = 0, n_wdin = 0, n_bootmc = 0;
  integer n_pre = 0, n_div = 0, n_tog = 0, n_xor = 0, n_wdout = 0;
  integer n_tclk = 0, n_tdat = 0, n_cpdc = 0, n_cpdd = 0, n_mcpstrb = 0;
  reg p_tclk = 1'b0, p_tdat = 1'b0, p_cpdc = 1'b0, p_cpdd = 1'b0, p_mcpstrb = 1'b0;
  integer fn_seen [0:7];
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
    for (i = 0; i < 16; i = i + 1) begin
      bucket[i] = 0; win_start[i] = 0; win_resets[i] = 0; win_armed[i] = 1'b0;
    end
    win_armed[0] = 1'b1;   // g22 comes up armed unless G22_DISARMED
    for (i = 0; i < 4096; i = i + 1) hot[i] = 0;
    for (i = 0; i < 8; i = i + 1) fn_seen[i] = 0;

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
        if (mca >= 16'hF000) begin
          hot[mca - 16'hF000] = hot[mca - 16'hF000] + 1;
          last_rom = mca;
        end
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
          win_resets[win] = win_resets[win] + 1;
          if (n_mcreset < 6)
            $display("tb_firmware:   MCReset' ASSERTED #%0d at %0d (+%0d)",
                     n_mcreset, i, i - last_mcreset);
          last_mcreset = i;
        end
      end
      if (`BB.CPDMuxClk  !== p_cpdc) begin p_cpdc = `BB.CPDMuxClk;  n_cpdc = n_cpdc + 1; end
      if (`BB.CPDMuxData !== p_cpdd) begin p_cpdd = `BB.CPDMuxData; n_cpdd = n_cpdd + 1; end
      if (`BB.MCPStrb !== p_mcpstrb) begin
        p_mcpstrb = `BB.MCPStrb;
        if (p_mcpstrb) begin            // rising strobe: latch the function code
          n_mcpstrb = n_mcpstrb + 1;
          fn_seen[{`BB.MCPABus_0, `BB.MCPABus_1, `BB.MCPABus_2}] =
            fn_seen[{`BB.MCPABus_0, `BB.MCPABus_1, `BB.MCPABus_2}] + 1;
          if (n_mcpstrb <= 8)
            $display("tb_firmware:   strobe %0d at %0d (last ROM addr %04h): fn=%0d data=%b%b%b%b%b%b%b%b%b",
                     n_mcpstrb, i, last_rom,
                     {`BB.MCPABus_0, `BB.MCPABus_1, `BB.MCPABus_2},
                     `BB.MCPBus_08, `BB.MCPBus_00, `BB.MCPBus_01, `BB.MCPBus_02,
                     `BB.MCPBus_03, `BB.MCPBus_04, `BB.MCPBus_05, `BB.MCPBus_06,
                     `BB.MCPBus_07);
        end
      end
      if (`BB.TDMuxClk  !== p_tclk) begin p_tclk = `BB.TDMuxClk;  n_tclk = n_tclk + 1; end
      if (`BB.TDMuxData !== p_tdat) begin p_tdat = `BB.TDMuxData; n_tdat = n_tdat + 1; end
      if (`BB.MCPreClk !== p_pre) begin p_pre = `BB.MCPreClk; n_pre = n_pre + 1; end
      if (`BB.BaseBd09_sil_pl_8 !== p_div) begin
        p_div = `BB.BaseBd09_sil_pl_8; n_div = n_div + 1;
        if (win < 15) begin
          win = win + 1; win_start[win] = i; win_resets[win] = 0;
          win_armed[win] = `BB.BaseBd09_sil_pl_3;
        end
      end
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
    $display("tb_firmware: RESETS PER WATCHDOG WINDOW (a window is one Q21 edge to the next):");
    for (j = 0; j <= win; j = j + 1)
      $display("tb_firmware:   window %0d  from %0d  g22FF1Q'=%b (%s)  resets %0d",
               j, win_start[j], win_armed[j], win_armed[j] ? "ARMED" : "disarmed",
               win_resets[j]);
    $display("tb_firmware: SkipWait'(the PacifyWatchdogIfJumper gate)=%b  -- 1 means the firmware SKIPS pacifying",
             m.u_machine.SkipWait_p_);
    $display("tb_firmware: PACIFYWATCHDOG(F692) visits %0d, PACIFYWATCHDOGIFJUMPER(F68B) visits %0d",
             hot[16'h692], hot[16'h68B]);
    $display("tb_firmware: SETMANIFOLD(F95A) %0d  DATUMTOMANIFOLD(F977) %0d  READMUFFLER(F986) %0d  SETMUFFLERADDRESS(F9D0) %0d",
             hot[16'h95A], hot[16'h977], hot[16'h986], hot[16'h9D0]);
    $display("tb_firmware: TDMuxClk edges %0d, TDMuxData edges %0d (the TTL side of the manifold chain)",
             n_tclk, n_tdat);
    $display("tb_firmware: MCPStrb rising edges %0d; CP-BUS FUNCTION CODES seen:", n_mcpstrb);
    for (j = 0; j < 8; j = j + 1)
      if (fn_seen[j] != 0)
        $display("tb_firmware:   fn %0d (%s) : %0d",
                 j, (j==0)?"Control":(j==1)?"Clock":(j==2)?"ABMux0":(j==3)?"ABMux1":
                    (j==4)?"MIR0":(j==5)?"MIR1":(j==6)?"MIR2":"MIR3", fn_seen[j]);
    $display("tb_firmware: CPDMuxClk edges %0d, CPDMuxData edges %0d (BaseBoard's own DMux decode)",
             n_cpdc, n_cpdd);
    $display("tb_firmware: TSetRun(= MCPBusL bit SetRunIn, the TryGettingMufManControl gate)=%b",
             `BB.TSetRun);
    $display("tb_firmware: TRYGETTINGMUFMANCONTROL(FA0E) visits %0d, WAITFORCPCONTROL(FA1F) %0d",
             hot[16'hA0E], hot[16'hA1F]);
    $display("tb_firmware: MCReset' ASSERTIONS: %0d   (0xFFFC seen on the bus %0d times -- not the same thing)",
             n_mcreset, n_reset);
    $display("tb_firmware: I/O addresses touched: %0d distinct", n_io);
    $display("tb_firmware: CPStrb' edges %0d, DMuxClk edges %0d", n_cpstrb, n_dmuxclk);
    if (n_cpstrb == 0 && n_dmuxclk == 0)
      $display("tb_firmware: it has NOT reached the Dorado yet -- still in the BaseBoard's own code.");
    else
      $display("tb_firmware: CP-bus strobes seen -- but read the function codes and the");
      $display("tb_firmware:   PC above before calling that 'driving the Dorado'.");
    $finish;
  end
endmodule
`default_nettype wire
