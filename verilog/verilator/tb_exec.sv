// tb_exec -- THE MACHINE EXECUTES MICROCODE OUT OF IM.
//
// The rung above `boot0-test`. Real Xerox microcode (AEmu.mb!2, packed into
// PARC's hunks by dorado/tests/boot0_hunks.c) is loaded into IM through the
// BaseBoard's control-processor bus, the MIR clock is released, the start
// address is put into Link, and a jammed `Return#` sends the machine there --
// which is exactly what `LoadDoradoCode` does to start Boot0:
//
//     "Put into Link the address of the first instruction in the Boot0
//      microcode ... Start the Boot0 loader running in the Dorado:
//      LDAI Return# ... JSR RunDoradoInstructionStream"
//
// `RunDoradoInstructionStream` IS `DoDoradoMicroInst` with ShouldSingleStep=0:
// same clears, same four MIR bytes, then Control(SetRun) with SS NOT asserted
// and no BasicStopDorado. The machine free-runs from there.
//
// It then runs: over 20,000 fabric cycles `clk0'` turns 1,242 times, `Stop`
// stays clear, and both `TNIA` and the decoded `FF` field take a spread of
// values as successive microinstructions are fetched and decoded -- 000, 009,
// 003, 010, 001, 00b, 00a ... The machine is sequencing through the microcode
// it was given.
//
// THE PARITY ENABLES MUST BE OFF TO RUN, and finding out why is the useful
// part. With them on the machine executes exactly ONE instruction and stops
// dead with `dStop` = 0, `Run'` = 0 and `Stop` = 1 -- a state that looks
// impossible until you notice `bCLKEnable' = Stop | Run'` gates `clk2'`, which
// is the clock that would clear the stop latch. ONCE `Stop` SETS IT GATES THE
// CLOCK THAT WOULD CLEAR IT, and only `rStop` (ClrStop) gets you out. What set
// it was `Error'` reading 0 at the instant the machine started: the jammed
// `Return#` still in the MIR fails the IM parity check, and `dStop` is
//
//     ~[(bpreStartC'b + ContA31.sil+5) . (bpreStartC'b + Error')]
//
// so a parity error stops the machine regardless of the single-step chain.
//
// THAT IS A DISCREPANCY WITH PARC'S ROM AND IT IS THE NEXT QUESTION.
// `InitManifolds` sets `DisableDoradoErrors` = "all except IM parity errors
// disabled" and leaves them on for the whole boot, jamming IRTable
// instructions constantly. So either PARC's IRTable entries carry parity that
// satisfies the generator -- their five-byte format has explicit P015 and
// P1631 bits, and `Nop#` = 70 01 0F 4C 40 sets both -- or our MC10170 parity
// generators on ContB j20/j21 compute something different. Checking a known
// IRTable entry's parity against j20/j21 by hand is the way in.
//
// MUTATIONS, INCLUDING ONE THAT PASSES. Not loading IM leaves TNIA on ONE
// value for ever (a machine fetching zeros from a wiped array), leaving SS
// asserted stops it after one instruction, and leaving the parity enables on
// does the same. Writing `1E0` instead of `1C0` -- leaving StopMIRClkEn set --
// does NOT fail, and the reason is worth knowing: `StopMIRClk` is
// `NOR(parity-error term, StopMIRClkEn')`, so clearing the PARITY ENABLES
// releases the MIR clock on its own and the explicit release is
// belt-and-braces. It is kept because a boot sequence should say what it
// means, not because this test proves it necessary.
//
// The load half of this file is `boot0-test`'s; see tb_boot0.sv for the hunk
// format, and tb_sendmir.sv for why Control strobes must be spaced.

`default_nettype none


module tb_exec;

  // THE OVERSAMPLING RATIO -- sys_clk per microinstruction. Real time on the
  // FPGA needs sys_clk = SYSPER x 16.67 MHz and measured Fmax is 48.99 MHz, so
  // SYSPER=2 is the target and 16 is 0.18x.
  //
  //     make -C verilog/verilator exec-test SYSPER=2
`ifndef SYSPER_OVERRIDE
  `define SYSPER_OVERRIDE 16
`endif
  localparam integer SYSPER = `SYSPER_OVERRIDE;
  // SCALE A FABRIC-CYCLE WAIT so it means the same amount of DORADO time at
  // any ratio. NOT `SYSPER / 16` -- that is integer division and evaluates to
  // ZERO below 16x, which turns every `repeat` into no wait at all and fails
  // the very first jam at time 0.
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

  localparam integer GAP = (200 * SYSPER) / 16;  // sys_clk between Control strobes

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;
  // THE MACHINE CLOCK, AT THE OVERSAMPLING RATIO. This was `ckd[3]` of a
  // free-running 4-bit counter -- a hard-coded divide-by-16 that ignored
  // SYSPER, because SYSPER reaches only `cell_CLOCKGEN` and that lives on the
  // BaseBoard, which `dorado_world` does not contain: its CLK_* ports are
  // inputs the generator marks "awaits BaseBd" and this bench drives them.
  // At SYSPER=16 this is bit for bit the counter it replaces.
  reg [3:0] ckd = 4'd0;
  always @(posedge sys_clk)
    ckd <= (ckd == SYSPER - 1) ? 4'd0 : ckd + 4'd1;
  wire mclk = (ckd >= SYSPER / 2);

  reg [2:0] addr_n = 3'b111;
  reg [8:0] cpout  = 9'd0;
  reg       strb_n = 1'b1;
  reg       setrun = 1'b0, setss_n = 1'b1;
  reg       dmd = 1'b0, dmc = 1'b0, udmd = 1'b0;

  // THE MACHINE. Four boards by default -- ContA, ContB, ProcH, ProcL -- which
  // is enough to fetch and decode microinstructions and nothing else. With
  // `+define+WORLD` it is NINE: the memory section, the storage array and the
  // IFU as well, i.e. everything a microcode world needs except the I/O boards
  // and the BaseBoard. The BaseBoard is deliberately absent even there, because
  // this bench drives the control-processor bus itself.
`ifdef WORLD
  // The BaseBoard fans the clock to EVERY slot, so a bigger configuration has
  // more CLK ports. Leaving them undriven gives those boards zero local clock
  // edges with their enables already asserted, which looks exactly like a
  // gating bug and is not one.
  reg [8:0] rfshdiv = 9'd0;
  reg       rfshper = 1'b0;
  always @(posedge sys_clk) begin
    rfshdiv <= rfshdiv + 9'd1;
    if (rfshdiv == 9'd0) rfshper <= ~rfshper;
  end
  // THE MEMORY SIZE IS A BACKPLANE INPUT: MemX takes these from the MSA and
  // they are the CHIP ENABLES on the two DRAM timing PROMs, so with both
  // undriven the memory state machine has no timing table at all. MemProms.bcpl
  // records the build -- from late 1979 the machines carry 16K parts.
  reg chips16k = 1'b1, chips64k = 1'b0;
`ifdef SCREEN
  // The six signals a monitor is made of. Every one of them read `toggled =
  // False` over 400,000 cycles on the twelve-board machine, which is what
  // "DispY is never initialised" means -- so they are counted here, not
  // sampled. A LEVEL IS NOT AN EVENT: this counts TRANSITIONS.
  wire vid, hsync, vsync_n, hblank, vblank, halfline;
  // 50 MHz on the built Rev Cl sheet (the 20 MHz on the bitsavers scan is
  // the superseded Rev Ci). The accumulator is told what a sys_clk is worth
  // so it holds at any oversampling ratio.
  wire pixel_clk;
  cell_K1115A #(.FREQ_KHZ(50000), .SYSPER(16)) u_pixclk (
      .sys_clk(sys_clk), .p8(pixel_clk), .p7(1'b0), .p14(1'b1));
  integer n_pix = 0; reg d_pix;
  always @(posedge sys_clk) begin
    if (pixel_clk !== d_pix) n_pix = n_pix + 1;
    d_pix <= pixel_clk;
  end
  integer n_vid = 0, n_hs = 0, n_vs = 0, n_hb = 0, n_vb = 0, n_hl = 0;
  reg d_vid, d_hs, d_vs, d_hb, d_vb, d_hl;
  integer n_xtal = 0, n_wdwt = 0, n_wdht = 0;
  reg d_xtal, d_wdwt, d_wdht;
  wire w_xtal = m.b_DispY.DispY25_sil_pl_1;   // a05 pin 8, the 50 MHz crystal
  // THE SYNC PATTERN IS STORED, NOT GENERATED. `preHSync` comes out of h14,
  // an F10415A ECL RAM addressed by a counter -- the HRam. So a blank DispY
  // has no horizontal timing at all until the microcode loads it. Probe the
  // ADDRESS COUNTER separately from the RAM's output: a stuck counter and an
  // empty RAM look identical at the output and need different fixes.
  // ClkHRamAddr' is the pixel clock GATED BY `DoradoHasHRam` (k14, an
  // MC10118). If that gate is shut the address counter cannot advance no
  // matter how well the pixel clock runs -- so read the gate, not just the
  // clock.
  wire w_hasram = m.b_DispY.DoradoHasHRam;
  wire w_clkhra = m.b_DispY.ClkHRamAddr_p_;
  integer n_hasram1 = 0, n_clkhra = 0; reg d_clkhra;
  always @(posedge sys_clk) begin
    if (w_hasram) n_hasram1 = n_hasram1 + 1;
    if (w_clkhra !== d_clkhra) n_clkhra = n_clkhra + 1;
    d_clkhra <= w_clkhra;
  end
  // h13 is an F10016: CE (p6, CountHRamAddr') is LOW TO COUNT, PE (p5) loads,
  // and p12 (ClearHRamAddr) is master reset. A clocked counter that does not
  // count is one of these three, and they are all DispY control bits.
  wire w_cnt = m.b_DispY.CountHRamAddr_p_;
  wire w_ld  = m.b_DispY.LdHRamAddr_p_;
  wire w_clr = m.b_DispY.ClearHRamAddr;
  integer n_cnt1 = 0, n_ld1 = 0, n_clr1 = 0, n_samp = 0;
  always @(posedge sys_clk) begin
    n_samp = n_samp + 1;
    if (w_cnt) n_cnt1 = n_cnt1 + 1;
    if (w_ld ) n_ld1  = n_ld1  + 1;
    if (w_clr) n_clr1 = n_clr1 + 1;
  end
  // ---- PARC'S OWN HRam WAVEFORM -------------------------------------------
  //
  // The horizontal timing is a TABLE, not a counter, and PARC's table
  // survives: `DisplayAux.mc!1 HRamTable` builds seven groups from four
  // numbers in `DisplayDefs.mc!1 HorizontalWaveforms`. For the Alto monitor
  // those are #1=760 total pixels/scan line, #2=656 visible, #3=14 from end
  // of visible to start of sync, #4=70 sync width. Every count is HALVED
  // because "the HRam is clocked on PixelClk/2" -- the macro stores
  // RShift[WF,1].
  //
  //   group          expression                  /2    data
  //   WF1     #4-2                        =  68   34    HSync|HBlank
  //   WF2     #1-#2-#3-#4                 =  20   10    HBlank
  //   WF3     (#2+#3)-(#1/2)              = 290  145    0
  //   WF4     #4                          =  70   35    HalfLine
  //   WF5     (#1/2)-#3-#4                = 296  148    0
  //   WF6     #3                          =  14    7    HBlank
  //   last    1                                    1    HSync|HBlank
  //                                            -----
  //                                              380  = #1/2, exactly.
  //
  // THE BIT ASSIGNMENTS CROSS-CHECK. DisplayDefs says HalfLine=1, HBlank=2,
  // HSync=4; PARC numbers MSB-first, so in a 16-bit word those are RIOB.15,
  // .14 and .13 -- and the netlist gives j14 (preHalfLine) data pin RIOB_15,
  // i14 (preHBlank) RIOB_14, h14 (preHSync) RIOB_13. The microcode and the
  // wire list agree exactly, and neither was derived from the other.
  //
  // This writes the RAMs directly, the way IM/IFUM/ALUFM are preloaded. It
  // stands in for `InitHRam`, which the microcode runs at THT startup -- and
  // which never runs here because the machine is stuck in the fault task.
  task preload_hram;
    integer a, g, k;
    integer cnt [0:6];
    integer dat [0:6];
    begin
      cnt[0]= 34; dat[0]='b110;   // HSync|HBlank
      cnt[1]= 10; dat[1]='b010;   // HBlank
      cnt[2]=145; dat[2]='b000;
      cnt[3]= 35; dat[3]='b001;   // HalfLine
      cnt[4]=148; dat[4]='b000;
      cnt[5]=  7; dat[5]='b010;   // HBlank
      cnt[6]=  1; dat[6]='b110;   // HSync|HBlank
      a = 0;
      for (g = 0; g < 7; g = g + 1)
        for (k = 0; k < cnt[g]; k = k + 1) begin
          m.b_DispY.u_h14.mem[a] = dat[g][2];   // preHSync
          m.b_DispY.u_i14.mem[a] = dat[g][1];   // preHBlank
          m.b_DispY.u_j14.mem[a] = dat[g][0];   // preHalfLine
          a = a + 1;
        end
      // The rest of the 1024 stays 0; the line is 380 HRam steps long.
      $display("tb_exec: +hram -- PARC's Alto waveform loaded, %0d of 1024 HRam locations", a);
    end
  endtask
  initial if ($test$plusargs("hram")) preload_hram;

  // ---- DRAW WHAT THE DISPLAY IS EMITTING ----------------------------------
  //
  // Sample the video signals at the HRam's own rate and write a PGM. There
  // is no VERTICAL timing yet -- that comes from the NLCB, which the display
  // task drives once per field, and no task is running -- so there is no
  // frame boundary to key on. HSync is the only boundary available, so this
  // draws ONE LINE PER HORIZONTAL SYNC: a raster strip, not a picture of
  // anything. What it shows is the SHAPE of a scan line, and that is exactly
  // what has just been made to work.
  //
  // Three greys so the structure is legible: visible, blanked, sync.
  integer pgm, px, py;
  reg [7:0] fb [0:127][0:511];       // 128 lines x 512 samples
  reg d_hs_fb;
  integer pgm_on;
  initial begin
    pgm_on = $test$plusargs("pgm");
    px = 0; py = 0; d_hs_fb = 1'b0;
    for (pgm = 0; pgm < 128; pgm = pgm + 1)
      for (px = 0; px < 512; px = px + 1) fb[pgm][px] = 8'd0;
    px = 0;
  end
  // One sample per PixelClk edge, which is the rate the HRam is walked at.
  reg d_pixfb;
  always @(posedge sys_clk) if (pgm_on) begin
    d_pixfb <= pixel_clk;
    if (pixel_clk && !d_pixfb) begin
      // A new line starts on the FALLING edge of sync.
      if (!hsync && d_hs_fb) begin
        if (py < 127) py = py + 1;
        px = 0;
      end
      d_hs_fb <= hsync;
      if (px < 512 && py < 128) begin
        fb[py][px] = hsync   ? 8'd255 :        // sync pulse: white
                     hblank  ? 8'd128 :        // blanking:   grey
                               8'd0;           // visible:    black
        px = px + 1;
      end
    end
  end
  task write_pgm;
    integer f, r, c;
    begin
      f = $fopen("dorado_raster.pgm", "w");
      $fwrite(f, "P2\n512 128\n255\n");
      for (r = 0; r < 128; r = r + 1) begin
        for (c = 0; c < 512; c = c + 1) $fwrite(f, "%0d ", fb[r][c]);
        $fwrite(f, "\n");
      end
      $fclose(f);
      $display("tb_exec: +pgm -- wrote dorado_raster.pgm, %0d lines captured", py);
    end
  endtask

  // THE DECISIVE EXPERIMENT. If `CountHRamAddr'` is the only thing stopping
  // the display, forcing it low must make the counter count and the sync
  // chain move. If it does not, the diagnosis is incomplete and something
  // further down is also held. `+hramforce` (named so it shares no prefix
  // with another plusarg -- a plusarg queried after one sharing its prefix
  // is silently not found).
  initial if ($test$plusargs("hramforce")) begin
    force m.b_DispY.CountHRamAddr_p_ = 1'b0;
    $display("tb_exec: +hramforce -- CountHRamAddr' forced LOW (F10016 CE is low to count)");
  end
  // l10's p12 is MR -- MASTER RESET, active high on the F10016 -- and the
  // net wired to it is `DoradoHasHRam`, which reads high on every sample. A
  // register held in reset explains RamHSync exactly. Either our polarity
  // for DoradoHasHRam is inverted (g08's Q vs Q-bar) or the part's MR is
  // active low; forcing the pin low decides whether this is the last link.
  initial if ($test$plusargs("mrlow")) begin
    force m.b_DispY.u_l10.p12 = 1'b0;
    $display("tb_exec: +mrlow -- l10's MR forced LOW");
  end

  // preHSync -> l10 (F10016) -> RamHSync -> h04 -> HSync. l10's CE is
  // PC'2ClkEn', so it is the same shape of question one link further on.
  wire w_rhs = m.b_DispY.RamHSync;
  wire w_hs2 = m.b_DispY.HSync;
  wire w_pce = m.b_DispY.PC_s_2ClkEn_p_;
  integer n_rhs = 0, n_hs2 = 0, n_pce1 = 0; reg d_rhs, d_hs2;
  always @(posedge sys_clk) begin
    if (w_rhs !== d_rhs) n_rhs = n_rhs + 1;
    if (w_hs2 !== d_hs2) n_hs2 = n_hs2 + 1;
    if (w_pce) n_pce1 = n_pce1 + 1;
    d_rhs <= w_rhs; d_hs2 <= w_hs2;
  end
  wire w_hra = m.b_DispY.HRamAddr_01;
  wire w_phs = m.b_DispY.preHSync;
  integer n_hra = 0, n_phs = 0; reg d_hra, d_phs;
  always @(posedge sys_clk) begin
    if (w_hra !== d_hra) n_hra = n_hra + 1;
    if (w_phs !== d_phs) n_phs = n_phs + 1;
    d_hra <= w_hra; d_phs <= w_phs;
  end
  wire w_wdwt = m.b_DispY.AltoCSync_p___drv;  // composite sync, DispY's own output
  wire w_wdht = m.b_DispY.DispY25_sil_pl_1;   // (same crystal, as a control)
  always @(posedge sys_clk) begin
    if (w_xtal !== d_xtal) n_xtal = n_xtal + 1;
    if (w_wdwt !== d_wdwt) n_wdwt = n_wdwt + 1;
    if (w_wdht !== d_wdht) n_wdht = n_wdht + 1;
    d_xtal<=w_xtal; d_wdwt<=w_wdwt; d_wdht<=w_wdht;
  end
  always @(posedge sys_clk) begin
    if (vid      !== d_vid) n_vid = n_vid + 1;
    if (hsync    !== d_hs ) n_hs  = n_hs  + 1;
    if (vsync_n  !== d_vs ) n_vs  = n_vs  + 1;
    if (hblank   !== d_hb ) n_hb  = n_hb  + 1;
    if (vblank   !== d_vb ) n_vb  = n_vb  + 1;
    if (halfline !== d_hl ) n_hl  = n_hl  + 1;
    d_vid<=vid; d_hs<=hsync; d_vs<=vsync_n; d_hb<=hblank; d_vb<=vblank; d_hl<=halfline;
  end
`endif
`ifdef SCREEN
  // The TEN-board machine: the nine of `dorado_world` plus DispY, the
  // monochrome display board. Everything else here is identical, so any
  // difference in what the microcode does is the display board's doing.
  dorado_screen m (
      .CLK_display_p_(mclk),
      // THE VIDEO TIMING CHAIN'S CLOCK, and it does not come from DispY.
      // DispY's own 50 MHz crystal (a05) has exactly ONE consumer on the
      // board -- a04, an MC10124 that sends it straight OFF-BOARD as
      // `Crystal`. Nothing on DispY is clocked by it. What clocks the sync
      // generators (l09, l10) is `PixelClk'Bd`, which descends from
      // `RawPixelClk`/`PixelClkVCO` -- INPUT ports marked "awaits DispM",
      // i.e. the pixel clock is made by the COLOUR board's VCO and a
      // monochrome-only machine has to supply it from somewhere.
      // Unconnected they default to 0 and the whole chain is frozen while
      // the crystal turns, which is exactly what was measured.
      .PixelClkVCO(pixel_clk), .RawPixelClk(pixel_clk),
      .AltoTTLVideo(vid), .AltoHSync(hsync), .AltoVSync_p_(vsync_n),
      .HBlank(hblank), .VBlank(vblank), .HalfLine(halfline),
`else
  dorado_world m (
`endif
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      .CLK_mc_p_(mclk), .CLK_md_p_(mclk), .CLK_mx_p_(mclk),
      .CLK_ms0Even_p_(mclk), .CLK_ifu_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n),
      .SetRunRfsh(1'b1), .RfshPeriod(rfshper),
      .ChipsAre256_s_16K(chips16k), .ChipsAre64K(chips64k)
  );

  // ---- SEED THE MAP --------------------------------------------------------
  //
  // WITHOUT THIS THE FAULT TASK IS REQUESTED FROM CYCLE 0. `TWReq.15` -- MemX's
  // fault-task wakeup -- reads high on 400,000 of 400,000 samples on an
  // unseeded machine, so with tasking on the machine switches to task 15 at
  // cycle 181 and never comes back: task 0 executes TWO instructions.
  //
  // The cause is the one tb_readback already documents at length: an empty Map
  // entry FAILS ITS PARITY CHECK, MapTrouble asserts, and it never clears. The
  // 21 MemX arrays here are that bench's map seeding, lifted -- every entry
  // valid, which is what a world would find after its own InitMem had run.
  //
  // `+nomapseed` leaves it empty, which is how the fault-task wakeup was
  // measured in the first place and is worth keeping as the control.
  integer mi;
  initial if (!$test$plusargs("nomapseed"))
    for (mi = 0; mi < 4096; mi = mi + 1) begin
      m.b_MemX.u_a04.mem[mi] = 1'b1;
      m.b_MemX.u_a05.mem[mi] = 1'b1;
      m.b_MemX.u_a06.mem[mi] = 1'b1;
      m.b_MemX.u_a07.mem[mi] = 1'b1;
      m.b_MemX.u_a08.mem[mi] = 1'b1;
      m.b_MemX.u_a09.mem[mi] = 1'b1;
      m.b_MemX.u_a10.mem[mi] = 1'b1;
      m.b_MemX.u_a11.mem[mi] = 1'b1;
      m.b_MemX.u_a12.mem[mi] = 1'b1;
      m.b_MemX.u_a13.mem[mi] = 1'b1;
      m.b_MemX.u_a14.mem[mi] = 1'b1;
      m.b_MemX.u_d04.mem[mi] = 1'b1;
      m.b_MemX.u_d05.mem[mi] = 1'b1;
      m.b_MemX.u_d06.mem[mi] = 1'b1;
      m.b_MemX.u_d07.mem[mi] = 1'b1;
      m.b_MemX.u_d08.mem[mi] = 1'b1;
      m.b_MemX.u_d09.mem[mi] = 1'b1;
      m.b_MemX.u_d10.mem[mi] = 1'b1;
      m.b_MemX.u_d11.mem[mi] = 1'b1;
      m.b_MemX.u_d12.mem[mi] = 1'b1;
      m.b_MemX.u_d13.mem[mi] = 1'b1;
    end

  // ---- SEED MEMORY ---------------------------------------------------------
  //
  // A microcode world reads its INSTRUCTIONS through the memory section, so an
  // unseeded machine is a machine with nothing to run. Both levels are filled:
  // the MSA's eight DRAMs (storage) and MemD's 72 F10470s (the cache), each
  // cell holding that bit's value, so every address answers the same word --
  // the arrangement tb_readback already uses to gate the return path.
  //
  // The pattern is a RUN-TIME argument, `+dpat=` / `+cpat=`, precisely so two
  // different values can be compared: if what executes depends on what memory
  // holds, the world is reading it. That is the whole experiment.
  reg [7:0]  dpat;      // storage, {H,G,F,E,D,C,B,A}
  reg [17:0] cpat;      // cache, cpat[k] is D.k
  integer    dp, cp, si, ci;
  reg        hi_par, lo_par, prog_cache, prog_swap, prog_even;
  integer    prog_fn;
  reg [17:0] cw;
  initial begin
    if (!$value$plusargs("dpat=%d", dp)) dp = 172;      // 8'b1010_1100
    if (!$value$plusargs("cpat=%d", cp)) cp = 52045;
    dpat = dp[7:0]; cpat = cp[17:0];
    // FG PARITY. The cache word is 16 data bits plus TWO parity bits, one per
    // instruction byte -- D.16 and D.17 -- and the IFU checks them as the bytes
    // come through (FGParityErr). An arbitrary seed pattern cannot satisfy
    // that, so with `+cpatpar` the two are COMPUTED from the data instead:
    // D.16 over one byte and D.17 over the other. Which byte pairs with which
    // bit, and whether the convention is even or odd, are the two things not
    // stated anywhere we have read -- so both are switches (`+cpatswap`,
    // `+cpatodd`) and the measurement decides.
    prog_cache = $test$plusargs("cprog");
    prog_swap  = $test$plusargs("cprogswap");
    prog_even  = $test$plusargs("cprogeven");
    if (!$value$plusargs("cprogfn=%d", prog_fn)) prog_fn = 0;
    if (!$test$plusargs("cpatraw")) begin
      // ODD per byte, D.17 over D.08-15 and D.16 over D.00-07. This
      // MINIMISES FGParityErr across six different constant patterns
      // (52045, 49155, 0, 65535, 21845, 4660), all of which give exactly the
      // same residual 176 -- so the level does respond to these bits, and
      // consistently.
      //
      // IT IS NOT ESTABLISHED AS THE HARDWARE CONVENTION, and the honest
      // reason is worth keeping. With `+cprog` giving each line its own word,
      // WHICH assignment wins changes with the DATA FUNCTION: the plain rule
      // is clean for a byte-swapped word (`+cprogfn=3`) and dirty for the
      // plain one, and vice versa, while `+cprogfn=1` is nearly clean under
      // both. No real parity convention can depend on the data.
      //
      // What that means is that this counter is not measuring what it looks
      // like it measures. `IfuMemRef` makes TWO to SIX transitions over the
      // whole run, so there are essentially NO fetches -- and a count of
      // samples where a combinational signal sits high over 200,000 cycles is
      // an IDLE LEVEL, not a per-fetch check. Establishing the convention
      // needs the error sampled AT a fetch, which needs a machine that
      // fetches. See the IFUM parity above for the contrast: RamPe went to
      // EXACTLY ZERO, and IFUM entries are read continuously, so there the
      // level is meaningful.
      hi_par = ~(^cpat[15:8]);     // D.08-15
      lo_par = ~(^cpat[7:0]);      // D.00-07
      cpat[17:16] = {hi_par, lo_par};
    end
    for (si = 0; si < 4096; si = si + 1) begin
      m.b_msa.u_b05.mem[si] = dpat[0];   // A
      m.b_msa.u_c05.mem[si] = dpat[1];   // B
      m.b_msa.u_d05.mem[si] = dpat[2];   // C
      m.b_msa.u_e05.mem[si] = dpat[3];   // D
      m.b_msa.u_e06.mem[si] = dpat[4];   // E
      m.b_msa.u_d06.mem[si] = dpat[5];   // F
      m.b_msa.u_c06.mem[si] = dpat[6];   // G
      m.b_msa.u_b06.mem[si] = dpat[7];   // H
    end
    for (ci = 0; ci < 4096; ci = ci + 1) begin
      // EVERY ADDRESS ANSWERING THE SAME WORD is a machine that fetches ONE
      // opcode for ever, however well the IFU works. With `+cprog` the cache
      // holds a DIFFERENT word per line -- the line number itself -- so the
      // instruction stream varies. Parity is recomputed per word, odd per
      // byte, because a fixed pair of parity bits is wrong for varying data.
      if (prog_cache) begin
        // The WORD each line holds. Varying it is the point: a parity rule
        // that only holds for `ci` is a coincidence of the data, not a rule.
        case (prog_fn)
          1:       cw[15:0] = ci[15:0] ^ 16'h5A5A;
          2:       cw[15:0] = ci[15:0] * 16'd3;
          3:       cw[15:0] = {ci[7:0], ci[15:8]};
          default: cw[15:0] = ci[15:0];
        endcase
        // The pairing could not be pinned by the CONSTANT-pattern sweep: with
        // one word in every line, F and G always agree and the two bytes are
        // fetched from the same value, so a swapped assignment is invisible
        // unless the byte parities differ -- and then "swap" and "invert both"
        // are the same thing. Varying data separates them, so try it here.
        if (prog_swap) begin
          cw[16] = ~(^cw[15:8]);
          cw[17] = ~(^cw[7:0]);
        end else begin
          cw[16] = ~(^cw[7:0]);
          cw[17] = ~(^cw[15:8]);
        end
        if (prog_even) cw[17:16] = ~cw[17:16];
      end else cw = cpat;
      m.b_MemD.u_a03.mem[ci] = cw[0];
      m.b_MemD.u_d03.mem[ci] = cw[0];
      m.b_MemD.u_a05.mem[ci] = cw[1];
      m.b_MemD.u_d05.mem[ci] = cw[1];
      m.b_MemD.u_g03.mem[ci] = cw[2];
      m.b_MemD.u_j03.mem[ci] = cw[2];
      m.b_MemD.u_g05.mem[ci] = cw[3];
      m.b_MemD.u_j05.mem[ci] = cw[3];
      m.b_MemD.u_a13.mem[ci] = cw[4];
      m.b_MemD.u_d13.mem[ci] = cw[4];
      m.b_MemD.u_a15.mem[ci] = cw[5];
      m.b_MemD.u_d15.mem[ci] = cw[5];
      m.b_MemD.u_g13.mem[ci] = cw[6];
      m.b_MemD.u_j13.mem[ci] = cw[6];
      m.b_MemD.u_g15.mem[ci] = cw[7];
      m.b_MemD.u_j15.mem[ci] = cw[7];
      m.b_MemD.u_a09.mem[ci] = cw[8];
      m.b_MemD.u_d09.mem[ci] = cw[8];
      m.b_MemD.u_a11.mem[ci] = cw[9];
      m.b_MemD.u_d11.mem[ci] = cw[9];
      m.b_MemD.u_g09.mem[ci] = cw[10];
      m.b_MemD.u_j09.mem[ci] = cw[10];
      m.b_MemD.u_g11.mem[ci] = cw[11];
      m.b_MemD.u_j11.mem[ci] = cw[11];
      m.b_MemD.u_a17.mem[ci] = cw[12];
      m.b_MemD.u_d17.mem[ci] = cw[12];
      m.b_MemD.u_a19.mem[ci] = cw[13];
      m.b_MemD.u_d19.mem[ci] = cw[13];
      m.b_MemD.u_g17.mem[ci] = cw[14];
      m.b_MemD.u_j17.mem[ci] = cw[14];
      m.b_MemD.u_g19.mem[ci] = cw[15];
      m.b_MemD.u_j19.mem[ci] = cw[15];
      m.b_MemD.u_a07.mem[ci] = cw[16];
      m.b_MemD.u_d07.mem[ci] = cw[16];
      m.b_MemD.u_g07.mem[ci] = cw[17];
      m.b_MemD.u_j07.mem[ci] = cw[17];
    end
    $display("tb_exec: seeded storage dpat=%b cache cpat=%b", dpat, cpat);
  end

`else
  dorado_proc m (
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n)
  );

`endif
  // THE BASEBOARD'S CONTROL REGISTER, modelled here because this machine has no
  // BaseBoard. `SetRun` and `SetSS'` are BACKPLANE wires, not CP-bus lines --
  // BaseBd f02 (an MC10124 TTL-to-ECL translator) drives them, and what it
  // translates is latched by g07, an SN74LS175 clocked by `TControlStrb'`:
  //
  //     g07 .p9  = TControlStrb'   (clock -- so it latches on strobe RELEASE)
  //     g07 .p4  = TCPBus.07  -> .p2 TSetRun          -> f02 -> SetRun
  //     g07 .p5  = TCPBus.08  -> .p7 (Q')             -> f02 -> SetSS'
  //
  // PARC numbers MSB first, so over the nine CP-bus bits .07 is the low bit of
  // the eight-bit Control byte -- `SetRun = 1` in doradoio.mdefs -- and .08 is
  // the ninth bit, where DoControl's CARRY lands ("LDAI Control^1 / RORA"),
  // i.e. SetSS. So a Control strobe ALREADY carries both, and setting the
  // ports by hand as well was a second, unsynchronised copy of the same state.
  task strobe(input [2:0] fn, input [7:0] data, input ss);
    begin
      addr_n = ~fn; cpout = {data, ss};
      repeat (WT(4)) @(posedge sys_clk);
      strb_n = 1'b0; repeat (WT(6)) @(posedge sys_clk);
      strb_n = 1'b1;
      if (fn == 3'd0) begin           // Control: g07 clocks on release
        setrun  =  data[0];
        setss_n = ~ss;
      end
      repeat (WT(4)) @(posedge sys_clk);
    end
  endtask

  // SetMufflerAddress: twelve bits, MSB first, then the ManClk pulse that
  // DoClock(UseDMD)/DoClock(0) makes. The first bit shifted in ends up at
  // DMD.00, the last at DMD.11.
  integer k;
  task manifold(input [11:0] word);
    begin
      for (k = 11; k >= 0; k = k - 1) begin
        dmd = word[k];
        repeat (WT(4)) @(posedge sys_clk); dmc = 1'b1;
        repeat (WT(4)) @(posedge sys_clk); dmc = 1'b0;   // the chain shifts on the fall
        repeat (WT(4)) @(posedge sys_clk);
      end
      udmd = 1'b1; repeat (WT(12)) @(posedge sys_clk);
      udmd = 1'b0; repeat (WT(12)) @(posedge sys_clk);
    end
  endtask

  // BMux and RBMux, most significant bit first as PARC numbers them.
  wire [15:0] bmux = {m.BMux_00, m.BMux_01, m.BMux_02, m.BMux_03,
                      m.BMux_04, m.BMux_05, m.BMux_06, m.BMux_07,
                      m.BMux_08, m.BMux_09, m.BMux_10, m.BMux_11,
                      m.BMux_12, m.BMux_13, m.BMux_14, m.BMux_15};
  wire [15:0] rbmux = {m.b_ContB.RBMux_00, m.b_ContB.RBMux_01, m.b_ContB.RBMux_02,
                       m.b_ContB.RBMux_03, m.b_ContB.RBMux_04, m.b_ContB.RBMux_05,
                       m.b_ContB.RBMux_06, m.b_ContB.RBMux_07, m.b_ContB.RBMux_08,
                       m.b_ContB.RBMux_09, m.b_ContB.RBMux_10, m.b_ContB.RBMux_11,
                       m.b_ContB.RBMux_12, m.b_ContB.RBMux_13, m.b_ContB.RBMux_14,
                       m.b_ContB.RBMux_15};

  integer i, hits, hits_a, hits_b;
  reg [15:0] pat_a, pat_b;

  // Link[4:15] -- twelve bits, the whole IM address space, and exactly what
  // cpu.c takes the Write-IM address from (`link_at_issue & 0xFFF`). PARC
  // numbers the register MSB-first, so .04 is the top bit.
  wire [11:0] link_hi = ~{m.b_ContA.Link_04_p_, m.b_ContA.Link_05_p_,
                          m.b_ContA.Link_06_p_, m.b_ContA.Link_07_p_,
                          m.b_ContA.Link_08_p_, m.b_ContA.Link_09_p_,
                          m.b_ContA.Link_10_p_, m.b_ContA.Link_11_p_,
                          m.b_ContA.Link_12_p_, m.b_ContA.Link_13_p_,
                          m.b_ContA.Link_14_p_, m.b_ContA.Link_15_p_};
  wire [11:0] tnia = {m.TNIA_04, m.TNIA_05, m.TNIA_06, m.TNIA_07,
                      m.TNIA_08, m.TNIA_09, m.TNIA_10, m.TNIA_11,
                      m.TNIA_12, m.TNIA_13, m.TNIA_14, m.TNIA_15};
  // dRA' is the address as ContB's multiplexer leaves it; the MC10211 buffers
  // invert it again into the array's RA lines.
  wire [11:0] dra = ~{m.b_ContB.dRA_00_p_, m.b_ContB.dRA_01_p_,
                      m.b_ContB.dRA_02_p_, m.b_ContB.dRA_03_p_,
                      m.b_ContB.dRA_04_p_, m.b_ContB.dRA_05_p_,
                      m.b_ContB.dRA_06_p_, m.b_ContB.dRA_07_p_,
                      m.b_ContB.dRA_08_p_, m.b_ContB.dRA_09_p_,
                      m.b_ContB.dRA_10_p_, m.b_ContB.dRA_11_p_};

  // Jam CPRegToLink# -- PARC's IRTable, `30 13 EF 04 40`.
  task jam_link(input [15:0] v);
    begin
      setrun = 0; setss_n = 1;
      repeat (WT(400)) @(posedge sys_clk);
      strobe(3'd1, 8'h21, 1'b0);
      strobe(3'd2, v[15:8], 1'b0); strobe(3'd3, v[7:0], 1'b0);
      strobe(3'd0, 8'h4E, 1'b0);
      strobe(3'd0, 8'h00, 1'b1);
      strobe(3'd4, 8'h13, 1'b0); strobe(3'd5, 8'hEF, 1'b0);
      strobe(3'd6, 8'h04, 1'b0); strobe(3'd7, 8'h40, 1'b0);
      strobe(3'd0, 8'h41, 1'b1);
      repeat (WT(600)) @(posedge sys_clk);
    end
  endtask

  task wipe_im;
    begin
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_a19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_b19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_c19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_d19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_e19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_f19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_g19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_h19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_i19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_j19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_k19.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l06.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l07.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l08.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l09.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l10.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l11.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l14.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l15.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l16.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l17.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l18.mem[i] = 1'b0;
      for (i=0;i<1024;i=i+1) m.b_ContB.u_l19.mem[i] = 1'b0;
    end
  endtask

  task count_im(output integer n);
    begin
      n = 0;
      for (i=0;i<1024;i=i+1) begin
        if (m.b_ContB.u_a10.mem[i]) n = n + 1;
        if (m.b_ContB.u_a11.mem[i]) n = n + 1;
        if (m.b_ContB.u_a14.mem[i]) n = n + 1;
        if (m.b_ContB.u_a15.mem[i]) n = n + 1;
        if (m.b_ContB.u_a16.mem[i]) n = n + 1;
        if (m.b_ContB.u_a17.mem[i]) n = n + 1;
        if (m.b_ContB.u_a18.mem[i]) n = n + 1;
        if (m.b_ContB.u_a19.mem[i]) n = n + 1;
        if (m.b_ContB.u_b10.mem[i]) n = n + 1;
        if (m.b_ContB.u_b11.mem[i]) n = n + 1;
        if (m.b_ContB.u_b14.mem[i]) n = n + 1;
        if (m.b_ContB.u_b15.mem[i]) n = n + 1;
        if (m.b_ContB.u_b16.mem[i]) n = n + 1;
        if (m.b_ContB.u_b17.mem[i]) n = n + 1;
        if (m.b_ContB.u_b18.mem[i]) n = n + 1;
        if (m.b_ContB.u_b19.mem[i]) n = n + 1;
        if (m.b_ContB.u_c10.mem[i]) n = n + 1;
        if (m.b_ContB.u_c11.mem[i]) n = n + 1;
        if (m.b_ContB.u_c14.mem[i]) n = n + 1;
        if (m.b_ContB.u_c15.mem[i]) n = n + 1;
        if (m.b_ContB.u_c16.mem[i]) n = n + 1;
        if (m.b_ContB.u_c17.mem[i]) n = n + 1;
        if (m.b_ContB.u_c18.mem[i]) n = n + 1;
        if (m.b_ContB.u_c19.mem[i]) n = n + 1;
        if (m.b_ContB.u_d10.mem[i]) n = n + 1;
        if (m.b_ContB.u_d11.mem[i]) n = n + 1;
        if (m.b_ContB.u_d14.mem[i]) n = n + 1;
        if (m.b_ContB.u_d15.mem[i]) n = n + 1;
        if (m.b_ContB.u_d16.mem[i]) n = n + 1;
        if (m.b_ContB.u_d17.mem[i]) n = n + 1;
        if (m.b_ContB.u_d18.mem[i]) n = n + 1;
        if (m.b_ContB.u_d19.mem[i]) n = n + 1;
        if (m.b_ContB.u_e06.mem[i]) n = n + 1;
        if (m.b_ContB.u_e07.mem[i]) n = n + 1;
        if (m.b_ContB.u_e08.mem[i]) n = n + 1;
        if (m.b_ContB.u_e09.mem[i]) n = n + 1;
        if (m.b_ContB.u_e10.mem[i]) n = n + 1;
        if (m.b_ContB.u_e11.mem[i]) n = n + 1;
        if (m.b_ContB.u_e14.mem[i]) n = n + 1;
        if (m.b_ContB.u_e15.mem[i]) n = n + 1;
        if (m.b_ContB.u_e16.mem[i]) n = n + 1;
        if (m.b_ContB.u_e17.mem[i]) n = n + 1;
        if (m.b_ContB.u_e18.mem[i]) n = n + 1;
        if (m.b_ContB.u_e19.mem[i]) n = n + 1;
        if (m.b_ContB.u_f10.mem[i]) n = n + 1;
        if (m.b_ContB.u_f11.mem[i]) n = n + 1;
        if (m.b_ContB.u_f14.mem[i]) n = n + 1;
        if (m.b_ContB.u_f15.mem[i]) n = n + 1;
        if (m.b_ContB.u_f16.mem[i]) n = n + 1;
        if (m.b_ContB.u_f17.mem[i]) n = n + 1;
        if (m.b_ContB.u_f18.mem[i]) n = n + 1;
        if (m.b_ContB.u_f19.mem[i]) n = n + 1;
        if (m.b_ContB.u_g10.mem[i]) n = n + 1;
        if (m.b_ContB.u_g11.mem[i]) n = n + 1;
        if (m.b_ContB.u_g14.mem[i]) n = n + 1;
        if (m.b_ContB.u_g15.mem[i]) n = n + 1;
        if (m.b_ContB.u_g16.mem[i]) n = n + 1;
        if (m.b_ContB.u_g17.mem[i]) n = n + 1;
        if (m.b_ContB.u_g18.mem[i]) n = n + 1;
        if (m.b_ContB.u_g19.mem[i]) n = n + 1;
        if (m.b_ContB.u_h10.mem[i]) n = n + 1;
        if (m.b_ContB.u_h11.mem[i]) n = n + 1;
        if (m.b_ContB.u_h14.mem[i]) n = n + 1;
        if (m.b_ContB.u_h15.mem[i]) n = n + 1;
        if (m.b_ContB.u_h16.mem[i]) n = n + 1;
        if (m.b_ContB.u_h17.mem[i]) n = n + 1;
        if (m.b_ContB.u_h18.mem[i]) n = n + 1;
        if (m.b_ContB.u_h19.mem[i]) n = n + 1;
        if (m.b_ContB.u_i16.mem[i]) n = n + 1;
        if (m.b_ContB.u_i17.mem[i]) n = n + 1;
        if (m.b_ContB.u_i18.mem[i]) n = n + 1;
        if (m.b_ContB.u_i19.mem[i]) n = n + 1;
      end
    end
  endtask

  // Jam CPRegToIM# (PARC's IRTable, `20 13 EF 03 4F`) with `v` in CPReg.
  task jam_write(input [15:0] v);
    begin
      // Stop first. With the MIR clock held, a running machine repeats the
      // jammed instruction forever, and the next jam has to start from rest.
      setrun = 0; setss_n = 1;
      repeat (WT(400)) @(posedge sys_clk);
      strobe(3'd1, 8'h21, 1'b0);                       // Clock: InhibitCAHolds+ClrReady
      strobe(3'd2, v[15:8], 1'b0); strobe(3'd3, v[7:0], 1'b0);
      strobe(3'd0, 8'h4E, 1'b0);
      strobe(3'd0, 8'h00, 1'b1);
      strobe(3'd4, 8'h13, 1'b0); strobe(3'd5, 8'hEF, 1'b0);
      strobe(3'd6, 8'h03, 1'b1); strobe(3'd7, 8'h4F, 1'b0);
      strobe(3'd0, 8'h41, 1'b1);
      repeat (WT(600)) @(posedge sys_clk);
    end
  endtask

  integer n0, n1, n2, j, s;
  reg p0, p1, p2;
  always @(posedge sys_clk) begin
    if (m.b_ContA.clk0_p_Ca !== p0) begin n0 = n0 + 1; p0 = m.b_ContA.clk0_p_Ca; end
    if (m.b_ContA.clk1_p_Ca !== p1) begin n1 = n1 + 1; p1 = m.b_ContA.clk1_p_Ca; end
    if (m.b_ContA.clk2_p_Bc !== p2) begin n2 = n2 + 1; p2 = m.b_ContA.clk2_p_Bc; end
  end
  task zero; begin n0 = 0; n1 = 0; n2 = 0; end endtask
  task settle; begin repeat (WT(200)) @(posedge sys_clk); end endtask

  // Jam a microinstruction and take the first step: ClrStop+ClrMIR+ClrCT+
  // Freeze, then the four MIR bytes, then SetRun with SS and no ClrStop.
  task jam_step(input [7:0] b0, input [7:0] b1, input [7:0] b2,
                input [7:0] b3, input [7:0] b4);
    begin
      setrun = 0; setss_n = 1;
      repeat (WT(400)) @(posedge sys_clk);
      strobe(3'd1, 8'h21, 1'b0); repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h4E, 1'b0);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h00, 1'b1);
      repeat (GAP) @(posedge sys_clk);
      // BYTE 0 IS FOUR BITS, AND THEY RIDE AS THE NINTH CP-BUS BIT of the four
      // data strobes -- which is why its low four bits are always zero:
      //     b0[7] RSTK.0 -> fn 4      b0[6] P015  -> fn 5
      //     b0[5] JCN.7  -> fn 6      b0[4] P1631 -> fn 7
      // Capture the MIR's parity bits AS THEY ARE SET, not after the task
      // returns: parc_micro ends with a Control strobe and 800 settling cycles,
      // by which time the MIR has reloaded from IM and the jam's bits are gone.
      // Reading them afterwards is how they first looked stuck at 0.
      strobe(3'd4, b1, b0[7]);  jt[0] = m.b_ContB.IMLH; jt[4] = m.b_ContA.IMRH;
      strobe(3'd5, b2, b0[6]);  jt[1] = m.b_ContB.IMLH; jt[5] = m.b_ContA.IMRH;
      strobe(3'd6, b3, b0[5]);  jt[2] = m.b_ContB.IMLH; jt[6] = m.b_ContA.IMRH;
      strobe(3'd7, b4, b0[4]);  jt[3] = m.b_ContB.IMLH; jt[7] = m.b_ContA.IMRH;
      jam_imlh = jt[1]; jam_imrh = jt[3];
      strobe(3'd0, 8'h01, 1'b1);
    end
  endtask

  // One more microinstruction, jam untouched. PARC's three strobes; what the
  // machine needs is the FIRST plus at least one more without ClrStop.
  task step_again;
    begin
      strobe(3'd0, 8'h41, 1'b1);
      strobe(3'd0, 8'h01, 1'b1);
      strobe(3'd0, 8'h00, 1'b1);
    end
  endtask


  reg [11:0] link_seen;
  integer total_written;

  // Bits set at ONE within-bank index, across the right half's packages.
  task scan_one(input integer idx);
    begin
      i = idx;
      hits = 0;
        if (m.b_ContB.u_a10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_a19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_b19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_c19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_d19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e06.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e07.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e08.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e09.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_e19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_f19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_g19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h10.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h11.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h14.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h15.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_h19.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i16.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i17.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i18.mem[i]) hits = hits + 1;
        if (m.b_ContB.u_i19.mem[i]) hits = hits + 1;
    end
  endtask

  task scan_all;
    begin
      total_written = 0;
      for (i = 0; i < 1024; i = i + 1) begin
        scan_one(i);
        if (hits != 0) begin
          $display("   IM[%0d] (0x%h): %0d right-half bits", i, i[11:0], hits);
          total_written = total_written + hits;
        end
      end
    end
  endtask

  task count_at(input integer addr, output integer n);
    begin scan_one(addr); n = hits; end
  endtask

  // ---- PARC's own routines, transcribed from doradoboot.masm/doradocpint.masm

  // SetCPReg~ : "LDA ToCPRegH / EORI 0ff / ... / LDA ToCPRegL / EORI 0ff".
  // The TILDE IS THE POINT -- the BaseBoard writes the COMPLEMENT, because
  // ContA b02's MC10159 inverts on the way to BMux. Write ~v and the datapath
  // delivers v.
  task set_cpreg_tilde(input [15:0] v);
    begin
      strobe(3'd2, ~v[15:8], 1'b0);
      strobe(3'd3, ~v[7:0],  1'b0);
    end
  endtask

  // ---- BEING THE BASEBOARD: the ReadBB sync-bit handshake ------------------
  //
  // Bootstrap's ReadBB (BootstrapMain.mc at ReadBBLoc) is a one-byte-at-a-
  // time handshake with no ready/ack line -- the SYNC BIT is the whole
  // protocol:
  //
  //     T_ RWCPReg;  PD_ (Tag) XOR T;
  //     T_ RWCPReg, Branch[.-2, ALU<0];   * wait until CPReg[0] = Tag[0]
  //     Tag_ NOT (Tag), ...  ChkSum_ (ChkSum) XOR T, Return;
  //
  // `CPReg[0]` is the MOST SIGNIFICANT bit -- PARC numbers MSB-first and the
  // test is `ALU<0`, a SIGN test, so only the top bit can satisfy it.
  //
  // The byte must be HELD STABLE, because the microcode reads RWCPReg TWICE:
  // once to test the tag and again "before believing the data", the source's
  // own caution about unsynchronized data on the BMux. So this presents each
  // word and then waits, rather than pulsing it.
  //
  // POLARITY IS NOT GUESSED. B carries the COMPLEMENT of CPReg while T ends
  // up equal to it, which is why PARC has both `SetCPReg` and `SetCPReg~`;
  // rather than reason about which applies to a B-source read, `+bbinv`
  // sends the complement and the run reports whether the poll loop breaks.
  task bb_word(input [15:0] w);
    begin
      // LOW BYTE FIRST, TAG BYTE LAST. The sync bit lives in the HIGH byte,
      // and ReadBB polls continuously -- it will happily sample in the window
      // between the two strobes. Strobed high-first, the tag flips while the
      // LOW byte is still stale, and the consumed word is {new tag, old
      // data}: measured as ChkSum = 0x00bf for EVERY payload, the stale low
      // byte of the startup's last jam. Data first, tag last, and the word
      // is stable by the time the tag says it is there.
      if ($test$plusargs("bbinv")) begin
        strobe(3'd3, ~w[7:0],  1'b0);
        strobe(3'd2, ~w[15:8], 1'b0);
      end else begin
        strobe(3'd3,  w[7:0],  1'b0);
        strobe(3'd2,  w[15:8], 1'b0);
      end
      // Hold well past the two reads and the branch between them.
      repeat (WT(600)) @(posedge sys_clk);
    end
  endtask

  // Feed the handshake while the machine runs. The starting PHASE does not
  // matter: if the first word carries the wrong sync bit the microcode simply
  // keeps waiting, and the next word -- which carries the other -- satisfies
  // it. Alternating from either phase therefore synchronises within one word.
  integer bb_sent = 0;
  initial if ($test$plusargs("bbfeed")) begin : bbfeeder
    integer k;
    reg [15:0] bbw;
    repeat (WT(20000)) @(posedge sys_clk);   // let it reach the poll loop
    // THE CHECKSUM WORD. Initial's entry (InitialMain.mc) calls ReadBB
    // exactly ONCE: "Read checksum word -- that should make computed sum
    // zero", then tests `LSH[ChkSum, 1]` -- the low 15 bits, the tag bit
    // ignored. In this preloaded configuration nothing was ever streamed,
    // ChkSum is an RM cell and Verilator zeroes RM at power-up, so the
    // word that cancels it is 0x0000. The MSB alternates as always (it is
    // the sync bit); the check ignores it by construction.
    // `+bbword=<hex>` sets the low 15 bits. The bench is deterministic, so
    // whatever ChkSum accumulates to before the read, the cancelling word is
    // a constant of the configuration -- observed once via the CKT trace at
    // f41 (`T_ LSH[ChkSum,1]` puts ChkSum on B) and then fed back here.
    if (!$value$plusargs("bbword=%h", bbw)) bbw = 15'h0000;
    for (k = 0; k < 8; k = k + 1) begin
      bb_word({k[0], bbw[14:0]});
      bb_sent = bb_sent + 1;
    end
    $display("tb_exec: +bbfeed -- sent %0d checksum words (low15=%h) to ReadBB", bb_sent, bbw[14:0]);
  end

  // DoDoradoMicroInst, single-step variant: DoClock(InhibitCAHolds+ClrReady),
  // Control(ClrStop+ClrMIR+ClrCT+Freeze) SS=0, Control(0) SS=1, the four MIR
  // bytes, Control(SetRun) SS=1, then BasicStopDorado's Control(SetRun) SS=1
  // and Control(0) SS=1.
  task parc_micro(input [7:0] b0, input [7:0] b1, input [7:0] b2,
                  input [7:0] b3, input [7:0] b4);
    begin
      strobe(3'd1, 8'h21, 1'b0); repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h4E, 1'b0);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h00, 1'b1);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd4, b1, b0[7]); strobe(3'd5, b2, b0[6]);
      strobe(3'd6, b3, b0[5]); strobe(3'd7, b4, b0[4]);
      zero;
      // The real BaseBoard is a 1 MHz 6502 running JSR DoControl between
      // these; the gap is MICROSECONDS, not the fourteen sys_clk a testbench
      // takes. GAP models that.
      strobe(3'd0, 8'h01, 1'b1);   // Control(SetRun), SS
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h01, 1'b1);               // BasicStopDorado
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h00, 1'b1);
      repeat (WT(800)) @(posedge sys_clk);
      $display("      micro %02h: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%h FF=%b",
        b1, n0, n1, n2, m.b_ContA.Stop, link_hi,
        ~{m.b_ContA.FF_0_p_,m.b_ContA.FF_1_p_,m.b_ContA.FF_2_p_,m.b_ContA.FF_3_p_,
          m.b_ContA.FF_4_p_,m.b_ContA.FF_5_p_,m.b_ContA.FF_6_p_,m.b_ContA.FF_7_p_});
    end
  endtask

  task nop_micro;
    begin parc_micro(8'h70, 8'h01, 8'h0F, 8'h4C, 8'h40); end
  endtask

  // SendViaMIR: the address into Link (with a Nop after it, which "holds CPReg
  // constant through T3 of the previous instruction"), then the data and the
  // IMStoreTable entry for this half.
  //   IMStoreTable[0] IMLHRSTK.0Is0#   60 33 EF 03 4F
  //   IMStoreTable[1] IMLHRSTK.0Is1#   20 73 EF 03 4F
  //   IMStoreTable[2] IMRHBLOCKIs0#    20 13 EF 03 4F
  //   IMStoreTable[3] IMRHBLOCKIs1#    60 53 EF 03 4F
  task send_via_mir(input [15:0] imaddr, input [15:0] word,
                    input right_half, input extra);
    begin
      set_cpreg_tilde(imaddr);
      parc_micro(8'h30, 8'h13, 8'hEF, 8'h04, 8'h40);   // CPRegToLink#
      nop_micro;                                        // ...AndNop
      // The address is in Link HERE. The store below carries FF=176 too and
      // reloads Link with its own DATA -- the link_at_issue hazard -- so this
      // is the only moment it can be checked.
      link_seen = link_hi;
      if (link_seen !== imaddr[11:0])
        $fatal(1, "CPRegToLink# did not put the address in Link");
      set_cpreg_tilde(word);
      if (!right_half && !extra) parc_micro(8'h60, 8'h33, 8'hEF, 8'h03, 8'h4F);
      if (!right_half &&  extra) parc_micro(8'h20, 8'h73, 8'hEF, 8'h03, 8'h4F);
      if ( right_half && !extra) parc_micro(8'h20, 8'h13, 8'hEF, 8'h03, 8'h4F);
      if ( right_half &&  extra) parc_micro(8'h60, 8'h53, 8'hEF, 8'h03, 8'h4F);
    end
  endtask


  function [15:0] rd_L0(input integer idx);
    rd_L0 = {m.b_ContB.u_j06.mem[idx], m.b_ContB.u_k06.mem[idx], m.b_ContB.u_l06.mem[idx], m.b_ContB.u_i10.mem[idx], m.b_ContB.u_j10.mem[idx], m.b_ContB.u_k10.mem[idx], m.b_ContB.u_l10.mem[idx], m.b_ContB.u_f06.mem[idx], m.b_ContB.u_g06.mem[idx], m.b_ContB.u_h06.mem[idx], m.b_ContB.u_j16.mem[idx], m.b_ContB.u_k16.mem[idx], m.b_ContB.u_l16.mem[idx], m.b_ContB.u_a06.mem[idx], m.b_ContB.u_b06.mem[idx], m.b_ContB.u_c06.mem[idx]};
  endfunction
  function [15:0] rd_R0(input integer idx);
    rd_R0 = {m.b_ContB.u_a10.mem[idx], m.b_ContB.u_b10.mem[idx], m.b_ContB.u_c10.mem[idx], m.b_ContB.u_d10.mem[idx], m.b_ContB.u_a16.mem[idx], m.b_ContB.u_b16.mem[idx], m.b_ContB.u_c16.mem[idx], m.b_ContB.u_d16.mem[idx], m.b_ContB.u_e06.mem[idx], m.b_ContB.u_e10.mem[idx], m.b_ContB.u_f10.mem[idx], m.b_ContB.u_g10.mem[idx], m.b_ContB.u_h10.mem[idx], m.b_ContB.u_g16.mem[idx], m.b_ContB.u_h16.mem[idx], m.b_ContB.u_i16.mem[idx]};
  endfunction
  function [15:0] rd_L1(input integer idx);
    rd_L1 = {m.b_ContB.u_j07.mem[idx], m.b_ContB.u_k07.mem[idx], m.b_ContB.u_l07.mem[idx], m.b_ContB.u_i11.mem[idx], m.b_ContB.u_j11.mem[idx], m.b_ContB.u_k11.mem[idx], m.b_ContB.u_l11.mem[idx], m.b_ContB.u_f07.mem[idx], m.b_ContB.u_g07.mem[idx], m.b_ContB.u_h07.mem[idx], m.b_ContB.u_j17.mem[idx], m.b_ContB.u_k17.mem[idx], m.b_ContB.u_l17.mem[idx], m.b_ContB.u_a07.mem[idx], m.b_ContB.u_b07.mem[idx], m.b_ContB.u_c07.mem[idx]};
  endfunction
  function [15:0] rd_R1(input integer idx);
    rd_R1 = {m.b_ContB.u_a11.mem[idx], m.b_ContB.u_b11.mem[idx], m.b_ContB.u_c11.mem[idx], m.b_ContB.u_d11.mem[idx], m.b_ContB.u_a17.mem[idx], m.b_ContB.u_b17.mem[idx], m.b_ContB.u_c17.mem[idx], m.b_ContB.u_d17.mem[idx], m.b_ContB.u_e07.mem[idx], m.b_ContB.u_e11.mem[idx], m.b_ContB.u_f11.mem[idx], m.b_ContB.u_g11.mem[idx], m.b_ContB.u_h11.mem[idx], m.b_ContB.u_g17.mem[idx], m.b_ContB.u_h17.mem[idx], m.b_ContB.u_i17.mem[idx]};
  endfunction
  function [15:0] rd_L2(input integer idx);
    rd_L2 = {m.b_ContB.u_j08.mem[idx], m.b_ContB.u_k08.mem[idx], m.b_ContB.u_l08.mem[idx], m.b_ContB.u_i14.mem[idx], m.b_ContB.u_j14.mem[idx], m.b_ContB.u_k14.mem[idx], m.b_ContB.u_l14.mem[idx], m.b_ContB.u_f08.mem[idx], m.b_ContB.u_g08.mem[idx], m.b_ContB.u_h08.mem[idx], m.b_ContB.u_j18.mem[idx], m.b_ContB.u_k18.mem[idx], m.b_ContB.u_l18.mem[idx], m.b_ContB.u_a08.mem[idx], m.b_ContB.u_b08.mem[idx], m.b_ContB.u_c08.mem[idx]};
  endfunction
  function [15:0] rd_R2(input integer idx);
    rd_R2 = {m.b_ContB.u_a14.mem[idx], m.b_ContB.u_b14.mem[idx], m.b_ContB.u_c14.mem[idx], m.b_ContB.u_d14.mem[idx], m.b_ContB.u_a18.mem[idx], m.b_ContB.u_b18.mem[idx], m.b_ContB.u_c18.mem[idx], m.b_ContB.u_d18.mem[idx], m.b_ContB.u_e08.mem[idx], m.b_ContB.u_e14.mem[idx], m.b_ContB.u_f14.mem[idx], m.b_ContB.u_g14.mem[idx], m.b_ContB.u_h14.mem[idx], m.b_ContB.u_g18.mem[idx], m.b_ContB.u_h18.mem[idx], m.b_ContB.u_i18.mem[idx]};
  endfunction
  function [15:0] rd_L3(input integer idx);
    rd_L3 = {m.b_ContB.u_j09.mem[idx], m.b_ContB.u_k09.mem[idx], m.b_ContB.u_l09.mem[idx], m.b_ContB.u_i15.mem[idx], m.b_ContB.u_j15.mem[idx], m.b_ContB.u_k15.mem[idx], m.b_ContB.u_l15.mem[idx], m.b_ContB.u_f09.mem[idx], m.b_ContB.u_g09.mem[idx], m.b_ContB.u_h09.mem[idx], m.b_ContB.u_j19.mem[idx], m.b_ContB.u_k19.mem[idx], m.b_ContB.u_l19.mem[idx], m.b_ContB.u_a09.mem[idx], m.b_ContB.u_b09.mem[idx], m.b_ContB.u_c09.mem[idx]};
  endfunction
  function [15:0] rd_R3(input integer idx);
    rd_R3 = {m.b_ContB.u_a15.mem[idx], m.b_ContB.u_b15.mem[idx], m.b_ContB.u_c15.mem[idx], m.b_ContB.u_d15.mem[idx], m.b_ContB.u_a19.mem[idx], m.b_ContB.u_b19.mem[idx], m.b_ContB.u_c19.mem[idx], m.b_ContB.u_d19.mem[idx], m.b_ContB.u_e09.mem[idx], m.b_ContB.u_e15.mem[idx], m.b_ContB.u_f15.mem[idx], m.b_ContB.u_g15.mem[idx], m.b_ContB.u_h15.mem[idx], m.b_ContB.u_g19.mem[idx], m.b_ContB.u_h19.mem[idx], m.b_ContB.u_i19.mem[idx]};
  endfunction

  function sec_L0(input integer idx);
    sec_L0 = m.b_ContB.u_i06.mem[idx];
  endfunction
  function sec_R0(input integer idx);
    sec_R0 = m.b_ContB.u_f16.mem[idx];
  endfunction
  function sec_L1(input integer idx);
    sec_L1 = m.b_ContB.u_i07.mem[idx];
  endfunction
  function sec_R1(input integer idx);
    sec_R1 = m.b_ContB.u_f17.mem[idx];
  endfunction
  function sec_L2(input integer idx);
    sec_L2 = m.b_ContB.u_i08.mem[idx];
  endfunction
  function sec_R2(input integer idx);
    sec_R2 = m.b_ContB.u_f18.mem[idx];
  endfunction
  function sec_L3(input integer idx);
    sec_L3 = m.b_ContB.u_i09.mem[idx];
  endfunction
  function sec_R3(input integer idx);
    sec_R3 = m.b_ContB.u_f19.mem[idx];
  endfunction

  // The SECONDARY bit of a half -- RSTK[0] on the left, BLOCK on the right --
  // which the array stores beside the sixteen data bits. Checking only the
  // data leaves it unverified: a mutation that reversed the ExtraBits shift
  // passed until this was added.
  function sec_at(input integer bank, input right, input integer idx);
    begin
      sec_at = 1'bx;
      if (!right) begin
        case (bank)
          0: sec_at = sec_L0(idx); 1: sec_at = sec_L1(idx);
          2: sec_at = sec_L2(idx); 3: sec_at = sec_L3(idx);
        endcase
      end else begin
        case (bank)
          0: sec_at = sec_R0(idx); 1: sec_at = sec_R1(idx);
          2: sec_at = sec_R2(idx); 3: sec_at = sec_R3(idx);
        endcase
      end
    end
  endfunction

  // Search every bank for a word at one index. Returns the bank, or -1.
  function integer find_word(input integer idx, input right, input [15:0] want);
    begin
      find_word = -1;
      if (!right) begin
        if (rd_L0(idx) === want) find_word = 0;
        else if (rd_L1(idx) === want) find_word = 1;
        else if (rd_L2(idx) === want) find_word = 2;
        else if (rd_L3(idx) === want) find_word = 3;
      end else begin
        if (rd_R0(idx) === want) find_word = 0;
        else if (rd_R1(idx) === want) find_word = 1;
        else if (rd_R2(idx) === want) find_word = 2;
        else if (rd_R3(idx) === want) find_word = 3;
      end
    end
  endfunction

  // ---- SendAHunk ----------------------------------------------------------
  reg [7:0]  hunk [0:16];
  reg [7:0]  extrabits;
  integer    yy, kk2;
  reg [15:0] imaddr;

  task send_a_hunk(input [15:0] start_addr);
    begin
      extrabits = hunk[0];
      yy = 1;
      imaddr = start_addr;
      for (kk2 = 0; kk2 < 4; kk2 = kk2 + 1) begin
        // left half, then right, both at the SAME IMAddress
        send_via_mir(imaddr, {hunk[yy], hunk[yy+1]}, 1'b0, extrabits[7]);
        extrabits = {extrabits[6:0], 1'b0}; yy = yy + 2;
        send_via_mir(imaddr, {hunk[yy], hunk[yy+1]}, 1'b1, extrabits[7]);
        extrabits = {extrabits[6:0], 1'b0}; yy = yy + 2;
        imaddr = imaddr + 16'd1;
      end
    end
  endtask

  integer bnk, nfound, w;
  reg [15:0] expect_l [0:3];
  reg [15:0] expect_r [0:3];

  // ---- REAL MICROCODE, from the C emulator's own .MB loader ---------------
  integer fd, nf, hunks, halves, bad;
  string  path, tag, line;
  integer ha;
  reg [7:0]  b [0:16];
  reg [15:0] hw [0:7];


  // RunDoradoInstructionStream: DoDoradoMicroInst with ShouldSingleStep = 0 --
  // DoClock(0), the clears, the four MIR bytes, then Control(SetRun) with SS
  // NOT asserted, and no BasicStopDorado. The machine free-runs from there.
  task parc_run(input [7:0] b0, input [7:0] b1, input [7:0] b2,
                input [7:0] b3, input [7:0] b4);
    begin
      strobe(3'd1, 8'h00, 1'b0); repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h4E, 1'b0);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h00, 1'b1);
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd4, b1, b0[7]); strobe(3'd5, b2, b0[6]);
      strobe(3'd6, b3, b0[5]); strobe(3'd7, b4, b0[4]);
      // SS off, and GIVE IT TIME: ContA i03's chain needs three RunClk' edges
      // to walk the deasserted SS through before SetRun lands.
      setss_n = 1;
      repeat (GAP) @(posedge sys_clk);
      strobe(3'd0, 8'h01, 1'b0);
    end
  endtask

  integer hcount;
  integer n0a, n1a, j2, nff;
  // How many DISTINCT addresses and decoded FF fields go by -- a machine
  // fetching from wiped IM would show one of each for ever.
  integer n_tnia, n_ff, q;
  reg [11:0] tnia_seen [0:31];
  reg [7:0]  ff_seen   [0:31];
  wire [11:0] tnia_now = {m.TNIA_04,m.TNIA_05,m.TNIA_06,m.TNIA_07,m.TNIA_08,
                          m.TNIA_09,m.TNIA_10,m.TNIA_11,m.TNIA_12,m.TNIA_13,
                          m.TNIA_14,m.TNIA_15};
  wire [7:0] ff_now = ~{m.b_ContA.FF_0_p_,m.b_ContA.FF_1_p_,m.b_ContA.FF_2_p_,
                        m.b_ContA.FF_3_p_,m.b_ContA.FF_4_p_,m.b_ContA.FF_5_p_,
                        m.b_ContA.FF_6_p_,m.b_ContA.FF_7_p_};

  // ---- PRELOAD: a WHOLE WORLD in IM ---------------------------------------
  //
  // The control-processor bus above is the REAL loader and boot0-test gates it,
  // but 4,096 microinstructions of it will not simulate in reasonable time --
  // sixteen hunks already take a couple of million fabric cycles. So with
  // `+preload=<file>` the arrays are written directly, through the map
  // tools/sil_im_map.py derives from ContB's own wire list and boot0-test
  // checks against a CP-bus load of the same microcode.
  //
  // NOTHING IS TAKEN ON TRUST: every address written is read back through the
  // same map and compared against the file before the machine is started. A
  // wrong interleave gives a machine executing garbage, which is
  // INDISTINGUISHABLE from a boot that fails for its own reasons.
`include "im_preload.vh"
  reg [3:0] f_rstk [0:4095];   reg [3:0] f_aluf [0:4095];
  reg [2:0] f_bsel [0:4095];   reg [2:0] f_lc   [0:4095];
  reg [2:0] f_asel [0:4095];   reg [7:0] f_ff   [0:4095];
  reg [7:0] f_jcn  [0:4095];   reg       f_blk  [0:4095];
  reg       f_have [0:4095];
  reg [3:0] r_rstk, r_aluf;  reg [2:0] r_bsel, r_lc, r_asel;
  reg [7:0] r_ff, r_jcn;     reg r_blk;
  reg [1023:0] impath;
  reg [31:0] ia, ib, ic, id, ie, ig, ih, ii, ij, ilh, irh;
  reg p_lh, p_rh, impar_odd, impar_nosec;
  integer imfd, imn, mapaddr, nloaded, nver, nverbad, runcycles, startaddr;
  integer nalufm, nalufmbad, nifum, nifumbad;
  reg [5:0]  r_alu;
  reg [15:0] r_lo, r_hi, ifum_want;  reg ifum_raw;
  reg r_lh_p, r_rh_p;
  integer n_ifuref, n_ifuack, n_jchg;
  reg ifu_prev_ref, ifu_prev_ack; reg [7:0] j_prev;
  // THE INSTRUCTION BYTE STREAM, the path tb_ifufetch traced on MemD:
  //   cache -> e06 (F register, clocked by Fclk'a) -> f22 (G, loaded on GLd')
  //   -> f23 (the 4:1 mux, enabled by EnableFG') -> FG.0-8 -> the IFU
  // tb_ifufetch found this never clocks in a SYNTHETIC four-instruction loop
  // and said the reason was that memory could not serve the reference. A real
  // world issues real IFetches, so measuring it here is a new datum.
  integer n_fclk, n_gld, n_fg, n_enfg;
  // IFU PARITY. The run visits AEMUIFURAMPE and AEMUIFUFGPARITY early, and the
  // IM parity generators are already a known open question (the machine stops
  // dead unless exec-test clears the IM parity enables). IFUM carries three
  // RamParity bits which the preload takes verbatim from the .MB -- so if our
  // generator computes something different, the IFU raises a RAM parity error
  // and stops fetching, which is exactly the shape measured.
  integer n_rampe, n_sawram, n_fgpe, n_sawfg;
  // THE CACHE IS TWO HALVES, and the netlist says so plainly: a03 and d03 both
  // drive D.00, but a03 takes Dad0.10-12 with enable D0ACE' and d03 takes
  // Dad1.10-12 with enable D1ACE'. They are separate 4096x1 arrays wired-OR
  // onto one data bus, so exactly ONE should be enabled at a time. If both
  // are, a CONSTANT seed hides it completely -- both halves hold the same word
  // -- and varying data does not. That is the candidate for +cprog failing FG
  // parity, so count it rather than argue about it.
  integer n_d0, n_d1, n_both, n_neither, n_addrdiff;
  // DOES THE IFU PIPELINE EVER GET STARTED? cpu.c on FA=1 FB=0 FC=0: "PCF <- B
  // ... Loads PCF and starts the IFU pipeline. The pipeline needs ~5 cycles to
  // fill before the first IFUJump can succeed (HM page 67). Until then,
  // IFUJump traps to *34-37 NotReady." Our visited set is FULL of octal 10-17,
  // which is exactly that trap region -- so count the instruction that starts
  // it. FF = {FA,FB,FC} = {1,0,0} = 0o100.
  integer n_pcf;
  // IS THE MIR FROZEN? At the lock the decoded FF is 073 while IM[0o100] holds
  // 144, so the machine is NOT executing what IM contains -- which is what a
  // held MIR looks like: constant FF, constant TNIA, clk0' still running, no
  // hold asserted. This bench's header states StopMIRClk = NOR(parity-error
  // term, StopMIRClkEn'), and THE PRELOAD WRITES IM PARITY AS ZERO for every
  // word, which is wrong for most of them. The enables are cleared at startup;
  // whether the error TERM still reaches the MIR clock is the question.
  integer n_smc, n_smce, n_imlhpe, n_imrhpe, n_err;
  // WHEN, not just how often. A total says 85 of 400,000; a distribution says
  // whether that is a transient at the start or a fault throughout. Reading a
  // total instead of a distribution has produced wrong conclusions in this
  // project twice already.
  integer first_pe, last_pe, first_stop;  reg late_par;
  reg jam_imlh, jam_imrh, sw_l0, sw_r0;  reg [7:0] jt;
  // WHY IS TNIA STUCK AT 0x040? It is not the instruction there: IM[0x040]'s
  // JCN is a LOCAL JUMP to 0x041, and 0x041 never appears. Nothing else in the
  // world local-jumps to 0x040 either. So something is HOLDING the next
  // address. Holds read 0, so the candidates are Freeze (which holds the
  // current task's PC) and tasking switching back to a task whose TPC is 0x040.
  integer n_freeze, n_ctask [0:15]; reg [3:0] ct_prev; integer n_ctchg;
  // ...and RepeatCur, which is the MIR-level mechanism for re-executing the
  // current microinstruction. A CONSTANT MIR with a RUNNING clock is exactly
  // what it looks like, and it would explain a decoded FF that never matches
  // IM at the address TNIA names.
  integer n_rep;
  // ...and the three signals AT THE GATE. cell_MC1660 gives f20 as
  //     gate b: CAHold' = ~Hold          (pin 10 -> pin 14)
  //     gate a: RepeatCur = ~(CAHold' | SwitchUp)
  // so with Hold=0 -> CAHold'=1 -> RepeatCur=0. RepeatCur is measured HIGH,
  // so one of the three does not read what the top-level probe said.
  integer n_cah, n_swu, n_holdA, n_hold_top;
  integer n_h_hold, n_h_pr, n_h_cb, n_h_ifu, n_h_io, n_h_mx, n_h_dis,
          n_h_creq, n_h_lg, n_h_hmb, n_h_ext;
  // WHICH of MemC's THREE hold sources. `Hold` is a wired-OR of three MC10231
  // flip-flop outputs -- e22 (RefHold' and MDhold') and e23 (MiscHold') -- so
  // the primed siblings name the cause: waiting for memory DATA, waiting for
  // REFRESH, or the misc/retry path.
  integer n_md, n_ref, n_misc, n_blr;
  // WHAT REQUESTS THE MISC HOLD. MemC c24 (MC10212) ORs PrHoldReq, ExtHoldReq,
  // CHoldReq, WantCR' and MXHold -- and MXHold comes from Hold itself, so the
  // chain LATCHES: once set it sustains itself until DisHold resets it, and
  // DisHold is a MODE BIT in a control register (k08, clocked by LdMcr' from
  // the MAR), not a per-cycle clear. So the interesting question is which
  // REQUEST set it in the first place.
  integer n_prq, n_wcr;
  // AND WHAT THE PROCESSOR IS WAITING FOR. ProcL j20 drives PrHoldReq from
  // StkP.6/7 and the RSTK field -- StkP is the STACK POINTER, so this is the
  // Dorado's stack overflow/underflow hold: a stack operation that would run
  // off either end holds the processor instead. Read the pointer.
  integer n_stk [0:255]; reg [7:0] stk_prev; integer n_stkchg;
  // WHEN it moves, and what was executing. StkP reads 0x3f for 565 samples and
  // then 0x00 for the rest, so SOMETHING moves it exactly once -- and whether
  // that was a legitimate stack operation or an uninitialised register settling
  // is the whole question.
  integer stk_when [0:7]; reg [7:0] stk_from [0:7], stk_to [0:7];
  reg [11:0] stk_pc [0:7]; reg [7:0] stk_ff [0:7]; reg [3:0] stk_rstk [0:7];
  integer swf, swdata;
  // A SHORT PC TRACE. Counters say how often; a trace says in what ORDER, and
  // for "why is it stuck" that is the difference between theorising and
  // looking. First N microinstructions only -- the interesting part is the
  // approach to the loop, not the loop.
  //
  // READ THE TWO COLUMNS SEPARATELY UNTIL THE OFFSET IS PINNED. `TNIA` and the
  // decoded `FF` are sampled from DIFFERENT PIPELINE STAGES, so the pair
  // printed on one line is not one instruction: the first three entries line
  // up as (next-address of A, FF of A-1), and the FF that appears beside the
  // stuck PC is not that address's own field. The PC column is the one to
  // trust for the path; pairing them needs the offset established first, and
  // reading them as one instruction produced a wrong lookup.
  integer n_tr, koff, nmatch, ncmp;
  reg [11:0] tr_pc [0:255]; reg [7:0] tr_ff [0:255]; reg [3:0] tr_ct [0:255];
  // ...and per-task address coverage. The visited bitmap does not separate by
  // task, so "25 addresses" hides whether the emulator ran at all or the fault
  // handler ran 25 of its own. Count both.
  integer nvis_t [0:15]; reg vis_t [0:15][0:4095];
  // WHAT WAKES TASK 15, AND WHEN. The switch happens within TWO
  // microinstructions of START -- long before the stack underflow at cycle 565
  // -- so the underflow is not what summoned it. MemX drives TWReq15 (the
  // fault task, HM section 4.1), so a memory fault at startup is the
  // candidate. Record the first switch and count the request line.
  integer n_twr15, first_switch; reg [3:0] first_switch_to;
  // ...and its three inputs. MemX h03 is an MC10104 (quad AND) wired
  //     TWReq.15 = (Faults & WakeEnable) | (WakeEnable & StkWake)
  // and StkWake is exactly the stack wakeup HM Table 6 names as the response to
  // an empty-stack operation. Count all three; do not sample them.
  integer n_faults, n_wen, n_stkwake, tpcslot, nfault_region, nboot_region, bootfeed;
  integer cktrace, n_ckt = 0;
  // EVERY T-FILE WRITE: strobe = ProcH TbWrite'a falling, slot = CurrLast'
  // (the per-task address), data = dTm. Prints the first 24 so the startup's
  // own writes are visible too -- what T holds at the checksum test is
  // whatever the LAST of these left in slot 0.
  // BOOT-BLOCKER PROBES: first-assert cycles for the hold and the two FF
  // side effects Initial needs -- IFUReset reaching the IFU and LoadMCR
  // reaching MemC's k08. Counts, not samples.
  integer c_ifur = -1, c_mcr = -1, c_mdh = -1, c_dish = -1;
  integer n_ifur = 0, n_mcr = 0, n_cyc2 = 0;
  reg d_ifur = 1'b0, d_mcr = 1'b0;
  always @(posedge sys_clk) begin
    n_cyc2 = n_cyc2 + 1;
    if (m.b_IFU.IfuReset !== d_ifur) begin
      if (m.b_IFU.IfuReset && c_ifur < 0) c_ifur = n_cyc2;
      if (m.b_IFU.IfuReset) n_ifur = n_ifur + 1;
      d_ifur <= m.b_IFU.IfuReset;
    end
    if (m.b_MemC.LdMcr_p_ !== d_mcr) begin
      if (!m.b_MemC.LdMcr_p_ && c_mcr < 0) c_mcr = n_cyc2;  // active low assumed
      n_mcr = n_mcr + 1;
      // At each edge: what data does k08 see? RMar_09 should carry disHold
      // (manual Mcr[9], memory.c's "mcr.disHold = b9") and A = T = 0x0041.
      $display("tb_exec: LDMCR edge@%0d %b->%b  RMar[00,01,07,08,09,10]=%b%b_%b%b%b%b  DisHold=%b NoRef=%b MarMuxAEn'=%b dAmux0=%b T09=%b",
               n_cyc2, d_mcr, m.b_MemC.LdMcr_p_,
               m.b_MemC.RMar_00, m.b_MemC.RMar_01, m.b_MemC.RMar_07,
               m.b_MemC.RMar_08, m.b_MemC.RMar_09, m.b_MemC.RMar_10,
               m.b_MemC.DisHold__drv, m.b_MemC.NoRef,
               m.b_ProcL.MarMuxAEn_p_, m.b_ProcL.dAmux0, m.b_ProcL.T_09);
      d_mcr <= m.b_MemC.LdMcr_p_;
    end
    if (!m.b_MemC.MDhold_p_ && c_mdh < 0) c_mdh = n_cyc2;
    if (m.b_MemC.DisHold__drv && c_dish < 0) c_dish = n_cyc2;
  end
  integer n_marA = 0;
  always @(posedge sys_clk) if (m.b_ProcL.MarMuxAEn_p_) n_marA = n_marA + 1;
  integer n_twr = 0; reg d_tbw = 1'b1;
  wire [15:0] w_dtm = {m.b_ProcH.dTm_00, m.b_ProcH.dTm_01, m.b_ProcH.dTm_02,
                       m.b_ProcH.dTm_03, m.b_ProcH.dTm_04, m.b_ProcH.dTm_05,
                       m.b_ProcH.dTm_06, m.b_ProcH.dTm_07,
                       m.b_ProcL.dTm_08, m.b_ProcL.dTm_09, m.b_ProcL.dTm_10,
                       m.b_ProcL.dTm_11, m.b_ProcL.dTm_12, m.b_ProcL.dTm_13,
                       m.b_ProcL.dTm_14, m.b_ProcL.dTm_15};
  wire [3:0] w_tslot = {m.b_ProcH.CurrLast_3_p_, m.b_ProcH.CurrLast_2_p_,
                        m.b_ProcH.CurrLast_1_p_, m.b_ProcH.CurrLast_0_p_};
  always @(posedge sys_clk) begin
    if (cktrace && !m.b_ProcH.TbWrite_p_a && d_tbw && n_twr < 24) begin
      $display("tb_exec: TWR slot=%h dTm=%h (pc around %h)", w_tslot, w_dtm, tnia_now);
      n_twr = n_twr + 1;
    end
    d_tbw <= m.b_ProcH.TbWrite_p_a;
  end
  // A CONTROL PROCESSOR THAT ANSWERS. `+bootfeed` leaves ONE value in CPReg
  // and never changes it, which is enough to prove the poll loop is
  // data-dependent (bit 15 clear -> it polls, longest run 0; bit 15 set ->
  // it leaves the loop and sticks for 12,485 cycles) but not enough to boot:
  // Bootstrap takes the byte and then waits for the handshake to come back.
  // `+bootcp` plays the other half -- present a byte with bit 15 SET, wait,
  // clear bit 15, wait, and repeat -- which is what a BaseBoard does.
  integer bootcp_period, bootcp_n, bootcp_byte;
  reg     bootcp_phase;
  // ...and WHICH FAULT. `Faults` is an F10016 (k09) whose parallel inputs are
  // TrueBD -- hardwired true -- so it reads 1 whenever it loads; the load is
  // gated by `_FaultInfoDly'` and `ReportFault'`. ReportFault' is k07, an
  // MC10121 whose inputs NAME the fault classes:
  //   ChkLastPh6' | ReportSE' | ECFault' | MemError' | ValidMapFltInEc2'
  //   | CacheLoad' | WakeOnCL'
  // all active-low, so count them ASSERTED.
  integer n_rf, n_fid, n_se, n_ecf, n_merr, n_vmf, n_cl, n_wocl, n_chk;
  reg fclk_prev, gld_prev; reg [8:0] fg_prev;
  // HOW MUCH OF THE WORLD ACTUALLY EXECUTES. The distinct-value lists above cap
  // at 32, which is fine for "is it sequencing at all" and useless for "how far
  // does it get". A bitmap over the whole address space answers the second.
  reg       visited [0:4095];
  integer   nvisited, lastpc, stuck, maxrun, prevpc, maxpc;  reg [3:0] ctnow;

`ifdef WORLD
  wire [8:0] fg_now = {m.FG_0, m.FG_1, m.FG_2, m.FG_3, m.FG_4,
                       m.FG_5, m.FG_6, m.FG_7, m.FG_8};
  wire [7:0] j_now = {m.b_IFU.J_0a, m.b_IFU.J_1a, m.b_IFU.J_2a, m.b_IFU.J_3a,
                      m.b_IFU.J_4a, m.b_IFU.J_5a, m.b_IFU.J_6a, m.b_IFU.J_7a};
`endif
  initial begin
    force m.DMuxData = dmd;
    force m.DMuxClk  = dmc;
    force m.UseDMD   = udmd;
    repeat (WT(2000)) @(posedge sys_clk);
    manifold(12'h030);
    manifold(12'h1E0);                    // SetMidasStopMIRClk ON, to load
    p0 = m.b_ContA.clk0_p_Ca; p1 = m.b_ContA.clk1_p_Ca; p2 = m.b_ContA.clk2_p_Bc;
    zero;
    wipe_im;
    nop_micro; nop_micro;

    nloaded = 0; nver = 0; nverbad = 0;
    if ($value$plusargs("preload=%s", impath)) begin
      imfd = $fopen(impath, "r");
      if (imfd == 0) $fatal(1, "cannot open %s", impath);
      for (i = 0; i < 4096; i = i + 1) f_have[i] = 1'b0;
      nalufm = 0; nalufmbad = 0; nifum = 0; nifumbad = 0;
      ifum_raw = $test$plusargs("ifumraw");
      impar_odd   = $test$plusargs("imparodd");
      impar_nosec = $test$plusargs("imparnosec");
      // $fgets RETURNS 0 AT END OF FILE and leaves `line` alone, so the
      // `while (!$feof(...))` idiom processes the LAST line TWICE -- which
      // counted 17 ALUFM entries out of a 16-entry file. Gate on the read.
      while ($fgets(line, imfd) != 0) begin
        // ELEVEN values after the tag: addr, the eight fields, and the two
        // 17-bit HALF-WORDS the hardware computes IM parity over.
        imn = $sscanf(line, "%s %h %h %h %h %h %h %h %h %h %h %h",
                      tag, ia, ib, ic, id, ie, ig, ij, ih, ii, ilh, irh);
        // ALUFM -- 16 entries of 6 bits, ProcL e13/e14. ALUF is a POINTER into
        // this memory, so a world loaded without it runs on whatever the array
        // powered up with. Same file, its own tag.
        // NOTE $sscanf RETURNS -1 ON A PARTIAL MATCH, not the number of
        // conversions it made -- while still filling the arguments it DID
        // convert. A three-field ALUFM line read with a ten-field format comes
        // back as -1 with tag, ia and ib all correct, so gating on the count
        // silently skipped every one of them. Gate on the TAG.
        // IFUM -- 1024 x 27 on the IFU board, two words per entry because the
        // board has two write enables. Without it the IFU cannot decode an
        // opcode at all. GUARDED: the four-board machine has no IFU, and
        // tb_boot0 and the default tb_exec include the same generated map.
`ifdef WORLD
        if (tag == "IFUM") begin
          // THE .MB DOES NOT CARRY IFUM PARITY -- the real machine computes it
          // in its LOAD microcode (ifuRamSubrs.mc:ifuAddParity), and 248 of
          // AEmu's 256 entries fail cpu.c's own check as stored. Copying them
          // verbatim leaves the IFU in a permanent RAM parity error. The
          // fourth column is the fields word with the three IPar bits
          // recomputed; `+ifumraw` takes the stored ones instead, so the two
          // can be compared.
          ifum_want = ifum_raw ? ic[15:0] : id[15:0];
          ifum_preload_word(ia[9:0], ib[15:0], ifum_want);
          ifum_readback_word(ia[9:0], r_lo, r_hi);
          nifum = nifum + 1;
          // DecHi' holds only bits 05-15 of word 0, so compare that mask.
          if ((r_lo & 16'h07FF) !== (ib[15:0] & 16'h07FF) || r_hi !== ifum_want) begin
            nifumbad = nifumbad + 1;
            if (nifumbad < 6)
              $display("tb_exec: IFUM[%h] read %h/%h want %h/%h",
                       ia[9:0], r_lo, r_hi, ib[15:0], ifum_want);
          end
          continue;
        end
`endif
        if (tag == "ALUFM") begin
          alufm_preload_word(ia[3:0], ib[5:0]);
          alufm_readback_word(ia[3:0], r_alu);
          nalufm = nalufm + 1;
          if (r_alu !== ib[5:0]) begin
            nalufmbad = nalufmbad + 1;
            $display("tb_exec: ALUFM[%h] read %h want %h", ia[3:0], r_alu, ib[5:0]);
          end
          continue;
        end
        if (imn != 12 || tag != "IM") continue;
        mapaddr = ia[11:0];
        f_rstk[mapaddr] = ib[3:0];  f_aluf[mapaddr] = ic[3:0];
        f_bsel[mapaddr] = id[2:0];  f_lc[mapaddr]   = ie[2:0];
        f_asel[mapaddr] = ig[2:0];  f_ff[mapaddr]   = ih[7:0];
        f_jcn[mapaddr]  = ii[7:0];  f_blk[mapaddr]  = ij[0];
        f_have[mapaddr] = 1'b1;
        // dBlock' -- the array stores the COMPLEMENT (tb_boot0 measured all 64
        // right-half secondary bits inverted; BLOCK is the one field bit the
        // MIR wires through Q' rather than Q).
        // IM PARITY MUST BE SUPPLIED BY THE PRELOAD. A real Write-IM generates
        // it in hardware; a direct array write does not, and IMLHPE/IMRHPE
        // then sit asserted for the entire run (measured: 399,808 of 400,000).
        // The convention is not stated anywhere we have read and the C
        // emulator does not model it at all, so it is computed here with the
        // sense and the secondary-bit inclusion as SWITCHES and the
        // measurement decides -- exactly as the IFUM parity was settled.
        p_lh = impar_nosec ? (^ilh[16:1]) : (^ilh[16:0]);
        p_rh = impar_nosec ? (^irh[16:1]) : (^irh[16:0]);
        if (impar_odd) begin p_lh = ~p_lh; p_rh = ~p_rh; end
        im_preload_word(mapaddr, ib[3:0], ic[3:0], id[2:0], ie[2:0], ig[2:0],
                        ih[7:0], ii[7:0], ~ij[0], p_lh, p_rh);
        nloaded = nloaded + 1;
      end
      $fclose(imfd);

      // READ IT ALL BACK. This is the part not to skip.
      for (i = 0; i < 4096; i = i + 1) begin
        if (!f_have[i]) continue;
        im_readback_word(i, r_rstk, r_aluf, r_bsel, r_lc, r_asel, r_ff, r_jcn,
                         r_blk, r_lh_p, r_rh_p);
        nver = nver + 1;
        if (r_rstk !== f_rstk[i] || r_aluf !== f_aluf[i] || r_bsel !== f_bsel[i] ||
            r_lc !== f_lc[i] || r_asel !== f_asel[i] || r_ff !== f_ff[i] ||
            r_jcn !== f_jcn[i] || r_blk !== ~f_blk[i]) begin
          nverbad = nverbad + 1;
          if (nverbad < 8)
            $display("tb_exec: PRELOAD IM[0x%h] mismatch: rstk %h/%h ff %h/%h jcn %h/%h blk %b/%b",
                     i[11:0], r_rstk, f_rstk[i], r_ff, f_ff[i], r_jcn, f_jcn[i],
                     r_blk, ~f_blk[i]);
        end
      end
      $display("tb_exec: PRELOAD -- %0d microinstructions written, %0d read back, %0d wrong",
               nloaded, nver, nverbad);
      $display("tb_exec: PRELOAD -- %0d ALUFM entries written, %0d wrong", nalufm, nalufmbad);
`ifdef WORLD
      $display("tb_exec: PRELOAD -- %0d IFUM entries written, %0d wrong", nifum, nifumbad);
      if (nifumbad != 0) $fatal(1, "IFUM does not hold what was preloaded");
`endif
      if (nalufmbad != 0) $fatal(1, "ALUFM does not hold what was preloaded");
      if (nloaded == 0)  $fatal(1, "the preload file held no microinstructions");
      if (nverbad != 0)  $fatal(1, "IM does not hold what was preloaded");
    end else begin
    // Load real microcode at its own addresses.
    if (!$value$plusargs("vectors=%s", path)) path = "boot0.vec";
    fd = $fopen(path, "r");
    if (fd == 0) $fatal(1, "cannot open %s", path);
    hcount = 0;
    while (!$feof(fd) && hcount < 4) begin
      void'($fgets(line, fd));
      nf = $sscanf(line,
        "%s %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h %h",
        tag, ha, b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8],
        b[9], b[10], b[11], b[12], b[13], b[14], b[15], b[16],
        hw[0], hw[1], hw[2], hw[3], hw[4], hw[5], hw[6], hw[7]);
      if (nf != 27 || tag != "HUNK") continue;
      for (w = 0; w < 17; w = w + 1) hunk[w] = b[w];
      send_a_hunk(ha[15:0]);
      hcount = hcount + 1;
    end
    $fclose(fd);
    $display("tb_exec: loaded %0d hunks (%0d microinstructions)", hcount, hcount*4);
    end

    // RELEASE the MIR clock -- register 7, data bit 0 -- so the MIR can reload
    // from IM. Without this the jam is held and nothing is ever fetched.
    manifold(12'h1C0);
    // IM PARITY OFF -- but only because a preload could not supply it. Now
    // that it can (even parity over the 17-bit half, measured), `+imparityon`
    // leaves the enables as InitManifolds does, which is how PARC's own
    // firmware runs the machine.
    if (!$test$plusargs("imparityon"))
      manifold(12'h000);        // ParityEnables := 0 -- IM parity OFF
    $display("tb_exec: parity enables IMLH=%b IMRH=%b, StopMIRClkEn=%b StopMIRClk=%b",
             m.b_ContB.IMLHPEenable, m.b_ContB.IMRHPEenable,
             m.b_ContB.StopMIRClkEn, m.StopMIRClk);

    // Put the start address in Link, then Return# to jump there and run.
    // The .MB carries no entry point -- that comes from the loader's STAMP, not
    // the file (mb2eb takes `start 01070` as a separate argument). So the start
    // address is an input here; 0 is what this bench has always used.
    if (!$value$plusargs("start=%d", startaddr)) startaddr = 0;
    $display("tb_exec: starting at IM[0x%h]", startaddr[11:0]);
    // WHICH FUNCTION CODE ROUTES THE NINTH CP-BUS BIT TO WHICH MIR BIT?
    //
    // RESULT: exactly what the byte layout implies. fn 5's ninth bit sets IMLH
    // and fn 7's sets IMRH -- which is b0[6] = P015 and b0[4] = P1631. The
    // routing WORKS, and it works under every variant tried: one strobe or
    // two, preceded by fn 4 or not, and with any data on the eight data bits
    // (`+jamsweep1`, `+swdata=`).
    //
    // THIS RETRACTS an earlier reading of mine. Capturing IMLH inside
    // parc_micro shows 0 after all four data strobes even for Nop#, whose
    // P015 is 1, and I wrote that up as "the jam never delivers a parity bit".
    // The sweep says the delivery path is fine, so what parc_micro's full
    // sequence does to the bit between the strobe and the capture is the real
    // question -- a narrower one, and NOT the same claim.
    if ($test$plusargs("jamsweep")) begin
      // The DATA the strobe carries, because that is the last difference
      // between this sweep and a real jam: parc_micro sends Nop#'s 0x0F on
      // fn 5, this sent 0x00.
      if (!$value$plusargs("swdata=%d", swdata)) swdata = 0;
      for (swf = 0; swf < 8; swf = swf + 1) begin
        strobe(3'd1, 8'h21, 1'b0); repeat (GAP) @(posedge sys_clk);
        strobe(3'd0, 8'h4E, 1'b0); repeat (GAP) @(posedge sys_clk);
        strobe(3'd0, 8'h00, 1'b1); repeat (GAP) @(posedge sys_clk);
        // TWO VARIANTS, because the first sweep and the real jam disagreed and
        // the only difference was the strobe SEQUENCE. `+jamsweep1` sends ONE
        // strobe on the function code, the way parc_micro does; the default
        // sends a ninth=0 strobe first, which is what made IMLH set.
        if ($test$plusargs("jamsweep1")) begin
          strobe(3'd4, swdata[7:0], 1'b0);    // ...preceded by fn 4, as in a jam
          sw_l0 = m.b_ContB.IMLH; sw_r0 = m.b_ContA.IMRH;
          strobe(swf[2:0], swdata[7:0], 1'b1);
        end else begin
          strobe(swf[2:0], 8'h00, 1'b0);
          sw_l0 = m.b_ContB.IMLH; sw_r0 = m.b_ContA.IMRH;
          strobe(swf[2:0], 8'h00, 1'b1);
        end
        $display("tb_exec: JAMSWEEP fn=%0d  ninth=0 -> IMLH=%b IMRH=%b   ninth=1 -> IMLH=%b IMRH=%b",
                 swf, sw_l0, sw_r0, m.b_ContB.IMLH, m.b_ContA.IMRH);
      end
    end

    // DOES A JAM'S PARITY BIT REACH THE MIR IN THE SAME SENSE?
    //
    // The checker (ContB j20/j21, an MC10170 cascade) covers 17 field bits plus
    // the stored bit as `IMLH'`, and works out to IMLHPE' = ~(XOR(17) ^ IMLH):
    // no error requires IMLH = XOR(17), i.e. the EVEN parity bit. That is what
    // the preload measurement found independently.
    //
    // But PARC's IRTable entries carry ODD parity, and their bit reaches the
    // MIR by a different route -- c24's SET input `sIMLH`, driven from CPOut.8
    // -- rather than through `dIMLH` from the array. So jam entries whose P015
    // is KNOWN and read the MIR back. Three points, both senses covered:
    //     CPRegToLink#  30 13 EF 04 40   P015=0  P1631=1
    //     Nop#          70 01 0F 4C 40   P015=1  P1631=1
    //     Return#       60 13 E1 42 43   P015=1  P1631=0
    parc_micro(8'h30, 8'h13, 8'hEF, 8'h04, 8'h40);   // CPRegToLink#, P015=0
    $display("tb_exec: JAMPAR CPRegToLink# P015=0 P1631=1 -> MIR IMLH=%b IMRH=%b",
             jam_imlh, jam_imrh);
    nop_micro;                                        // Nop#, P015=1
    $display("tb_exec: JAMPAR Nop#         P015=1 P1631=1 -> MIR IMLH=%b IMRH=%b",
             jam_imlh, jam_imrh);
    $display("tb_exec: JAMPAR   IMLH after each of the four data strobes: %b%b%b%b",
             jt[0], jt[1], jt[2], jt[3]);
    $display("tb_exec: JAMPAR   IMRH after each: %b%b%b%b", jt[4], jt[5], jt[6], jt[7]);

    set_cpreg_tilde(startaddr[15:0]);
    parc_micro(8'h30, 8'h13, 8'hEF, 8'h04, 8'h40);   // CPRegToLink#
    nop_micro;
    $display("tb_exec: Link[4:15]=%h", link_hi);

    // PROVE THE DIAGNOSIS. Both parity checkers -- ContB j20/j21 for the left
    // half, ContA e18/e19 for the right -- work out to
    //     IM?HPE' = ~( XOR(17 field bits) ^ IM?H )
    // so NO ERROR requires the MIR's parity bit to be the EVEN one. Our jam
    // leaves both at 0 (measured: jamming P015=0 and P015=1 both give MIR
    // IMLH=0), so only an entry whose field-XOR is zero can pass -- Nop# alone
    // of PARC's eight. Return#, which the startup jams, has XOR(rh)=1.
    //
    // So supply that one bit and see whether the machine runs with the enables
    // ON. Forced only across the jam and released immediately, because once
    // the machine is fetching from IM the array carries correct parity itself.
    if ($test$plusargs("jampar")) begin
      force m.b_ContA.IMRH = 1'b1;          // Return#: XOR(rh) = 1, XOR(lh) = 0
      $display("tb_exec: JAMPAR forcing IMRH=1 across the jam (Return# even parity)");
    end
    // THE FAULT COUNTER, PRESET AFTER STARTUP.
    //
    // MemX k09 is an F10016 whose TERMINAL COUNT output is the net `Faults`,
    // and TC is LOW only at 1111. Its four parallel inputs are all `TrueBD`, a
    // hardwired true, so the design is: LOAD 1111 = no fault; a `ReportFault'`
    // pulse COUNTS it, and reaching 0000 asserts the wakeup, since
    // `TWReq.15 = Faults & WakeEnable`.
    //
    // Measured: the counter reads 0001 by the time the run starts, so ONE
    // FAULT IS REPORTED DURING STARTUP and nothing ever reloads it -- on real
    // hardware the fault task reads the fault info and the load re-arms it,
    // and here the fault task runs emulator code because TPC[15] was never
    // initialised. Presetting it in an `initial` is too early; it counts away.
`ifdef WORLD
    // THE STACK POINTER. HM Table 6: StkP[0:1] is the region, StkP[2:7] the
    // per-region offset with valid range 1..77 octal, and offset 0 DENOTES AN
    // EMPTY STACK. Out of reset it reads 0x3f -- region 0, offset 63, the top
    // of the region -- and one stack operation takes it to 0, which underflows
    // and holds the processor (correctly). A world would set it up in its own
    // initialisation; this bench never gets that far.
    //
    // ProcL l14 is an MC10176 holding StkP.0-5 as {p15,p14,p13,p4,p3,p2} = q,
    // so StkP.n is q[n]; l15 is an MC10231 with StkP.6 = qb and StkP.7 = qa.
    // Region 0, offset 32 = mid-range: q = 000100, both l15 bits 0.
    if ($test$plusargs("stkinit")) begin
      m.b_ProcL.u_l14.q  = 6'b000100;
      m.b_ProcL.u_l15.qa = 1'b0;
      m.b_ProcL.u_l15.qb = 1'b0;
      $display("tb_exec: STKINIT -- StkP preset to region 0 offset 32");
    end
    // TPC[15], the FAULT TASK's program counter. When a task is woken it
    // resumes at its own TPC -- there is no separate vector -- so an
    // uninitialised TPC[15] makes the fault task run whatever it lands on,
    // which is why it executes EMULATOR handlers here. AEmu's own handler is
    // `FAULTTASK` at octal 3747.
    //
    // ContA i13/j13/k13/l13 are F10145A holding TPC in four-bit slices:
    //   l13 TPC.00-03   i13 TPC.04-07   j13 TPC.08-11   k13 TPC.12-15
    // with q = {q3,q2,q1,q0} = {p14,p15,p1,p2}. TPC.04-15 is the 12-bit IM
    // address, MSB first (TNIA bits 4:15; jcn-encoding.md).
    //
    // AND THE ADDRESS PINS ARE PRIMED -- `TPCAd.0-3'` -- so a = ~task and
    // TPC[15] lives at mem[0]. Same reversal as RM's ~RSTK; `+tpcslot=N`
    // overrides the slot so the derivation can be checked rather than trusted.
    if ($test$plusargs("tpcinit")) begin
      if (!$value$plusargs("tpcslot=%d", tpcslot)) tpcslot = 0;   // ~15 & 0xF
      m.b_ContA.u_l13.mem[tpcslot] = 4'b0000;   // TPC.00-03
      m.b_ContA.u_i13.mem[tpcslot] = 4'b1110;   // TPC.04-07 = 0,1,1,1
      m.b_ContA.u_j13.mem[tpcslot] = 4'b0111;   // TPC.08-11 = 1,1,1,0
      m.b_ContA.u_k13.mem[tpcslot] = 4'b1110;   // TPC.12-15 = 0,1,1,1
      $display("tb_exec: TPCINIT -- TPC[15] (mem[%0d]) preset to octal 3747 = FAULTTASK",
               tpcslot);
    end
    if ($test$plusargs("faultinit")) begin
      m.b_MemX.u_k09.q = 4'b1111;
      $display("tb_exec: FAULTINIT -- k09 preset to 1111, Faults now %b", m.b_MemX.Faults);
    end
`endif

    // TASKING ON, so fault task 15 can service the stack underflow.
    //
    // The world holds forever on StkP = 0, which HM Table 6 calls the EMPTY
    // STACK sentinel; the manual's response is HOLD *and wake fault task 15*,
    // and task 15 is what services it and releases the hold. `Return#` is
    // "TaskingOff,Return" -- its FF is 0o142 = FA 1, FB 4, FC 2, which cpu.c
    // names TaskingOff -- so with it the fault task can never run and CTask
    // sits at 0 for the whole run.
    //
    // FC 3 is TaskingOn. Re-encoding Return# with FF = 0o143 through the same
    // encoder that reproduces all eight of PARC's IRTable entries byte for
    // byte gives `70 13 E1 4A 43`, parity included; it re-decodes to the same
    // fields with only FF changed.
    // FEED BOOTSTRAP ONE VALUE. Its poll loop reads the control-processor
    // register: cpu.c gives FB=7 FC=6 as `B <- RWCPReg`, "HM page 31:
    // B<-RWCPReg = Link<-B, B<-CPReg'" -- the COMPLEMENT -- and Bootstrap's
    // loop carries FF=176, exactly that. `+bootfeed=N` leaves N in CPReg
    // before the machine starts, so whether the loop is DATA-DEPENDENT can be
    // measured rather than assumed.
    if ($value$plusargs("bootfeed=%d", bootfeed)) begin
      set_cpreg_tilde(bootfeed[15:0]);
      $display("tb_exec: BOOTFEED -- CPReg left holding %h", bootfeed[15:0]);
    end

    if ($test$plusargs("taskingon"))
      parc_run(8'h70, 8'h13, 8'hE1, 8'h4A, 8'h43);    // Return# with TaskingOn
    else
      parc_run(8'h60, 8'h13, 8'hE1, 8'h42, 8'h43);    // Return#, free-running
    if ($test$plusargs("jampar")) begin
      repeat (WT(40)) @(posedge sys_clk);
      release m.b_ContA.IMRH;
    end

    // TURN PARITY ON *AFTER* THE TRANSIENT -- see the sampling loop below.
    late_par = $test$plusargs("imparitylate");

    n0a = 0; n1a = 0; nff = 0; n_tnia = 0; n_ff = 0;
`ifdef WORLD
    n_ifuref = 0; n_ifuack = 0; n_jchg = 0;
    ifu_prev_ref = m.b_IFU.IfuMemRef; ifu_prev_ack = m.b_IFU.IfuMemAck; j_prev = j_now;
    n_fclk = 0; n_gld = 0; n_fg = 0; n_enfg = 0;
    n_rampe = 0; n_sawram = 0; n_fgpe = 0; n_sawfg = 0;
    n_d0 = 0; n_d1 = 0; n_both = 0; n_neither = 0; n_addrdiff = 0;
    n_pcf = 0; n_tr = 0;
    n_smc = 0; n_smce = 0; n_imlhpe = 0; n_imrhpe = 0; n_err = 0;
    first_pe = -1; last_pe = -1; first_stop = -1;
    n_freeze = 0; n_ctchg = 0; n_rep = 0; n_twr15 = 0; first_switch = -1;
    n_faults = 0; n_wen = 0; n_stkwake = 0;
    n_rf = 0; n_fid = 0; n_se = 0; n_ecf = 0; n_merr = 0; n_vmf = 0; n_cl = 0; n_wocl = 0; n_chk = 0;
    n_cah = 0; n_swu = 0; n_holdA = 0; n_hold_top = 0;
    n_h_hold = 0; n_h_pr = 0; n_h_cb = 0; n_h_ifu = 0; n_h_io = 0; n_h_mx = 0;
    n_h_dis = 0; n_h_creq = 0; n_h_lg = 0; n_h_hmb = 0; n_h_ext = 0;
    n_md = 0; n_ref = 0; n_misc = 0; n_blr = 0; n_prq = 0; n_wcr = 0;
    n_stkchg = 0; for (i = 0; i < 256; i = i + 1) n_stk[i] = 0;
    stk_prev = 8'hFF;
    for (i = 0; i < 16; i = i + 1) n_ctask[i] = 0;
    ct_prev = {m.b_ContA.CTask_0, m.b_ContA.CTask_1,
               m.b_ContA.CTask_2, m.b_ContA.CTask_3};
    fclk_prev = m.b_MemD.Fclk_p_a; gld_prev = m.b_MemD.GLd_p_; fg_prev = fg_now;
`endif
    for (i = 0; i < 4096; i = i + 1) visited[i] = 1'b0;
    for (i = 0; i < 16; i = i + 1) begin
      nvis_t[i] = 0;
      for (q = 0; q < 4096; q = q + 1) vis_t[i][q] = 1'b0;
    end
    nvisited = 0; stuck = 0; maxrun = 0; prevpc = -1; maxpc = -1;
    p0 = m.b_ContA.clk0_p_Ca; p1 = m.b_ContA.clk1_p_Ca;
    if (!$value$plusargs("cycles=%d", runcycles)) runcycles = 20000;
    bootcp_n = 0; bootcp_phase = 1'b0; bootcp_byte = 0;
    if (!$value$plusargs("bootcp=%d", bootcp_period)) bootcp_period = 0;
    cktrace = $test$plusargs("cktrace");
    if (bootcp_period) $display("tb_exec: BOOTCP -- toggling the handshake every %0d cycles",
                                bootcp_period);

    for (j2 = 0; j2 < runcycles; j2 = j2 + 1) begin
      @(posedge sys_clk);
      // THE CONTROL PROCESSOR'S HALF OF THE HANDSHAKE. Bit 15 says "a byte is
      // ready"; Bootstrap reads it, and the sender must then take it away
      // before offering the next. Driving CPReg from here is exactly what the
      // BaseBoard's 6502 does over the same bus -- see set_cpreg_tilde, which
      // is PARC's own two-strobe sequence.
      if (bootcp_period && (j2 % bootcp_period) == 0) begin
        bootcp_phase = ~bootcp_phase;
        if (bootcp_phase) begin
          set_cpreg_tilde({1'b1, 7'd0, bootcp_byte[7:0]});
          bootcp_byte = bootcp_byte + 1;
          bootcp_n = bootcp_n + 1;
        end else begin
          set_cpreg_tilde(16'h0000);
        end
      end
      if (m.b_ContA.clk0_p_Ca !== p0) begin
        n0a = n0a + 1; p0 = m.b_ContA.clk0_p_Ca;
        if (p0 === 1'b1) begin           // one sample per microinstruction
          lastpc = tnia_now;
          // CHECKSUM TRACE: the Initial entry sequence and ReadBB, with the
          // B bus alongside -- ProcH carries alub.00-07, ProcL .08-15.
          if (cktrace && ((lastpc[11:0] >= 12'hf40 && lastpc[11:0] <= 12'hf4f)
                       || (n_ckt < 40 && lastpc[11:0] >= 12'hfc0)))
          begin
            $display("tb_exec: CKT pc=%h ff=%h alua=%h pd=%h alub=%h eq0'=%b(H%b L%b) ResEq0'=%b", lastpc[11:0], ff_now,
                     {m.b_ProcH.alua_00, m.b_ProcH.alua_01, m.b_ProcH.alua_02,
                      m.b_ProcH.alua_03, m.b_ProcH.alua_04, m.b_ProcH.alua_05,
                      m.b_ProcH.alua_06, m.b_ProcH.alua_07,
                      m.b_ProcL.alua_08, m.b_ProcL.alua_09, m.b_ProcL.alua_10,
                      m.b_ProcL.alua_11, m.b_ProcL.alua_12, m.b_ProcL.alua_13,
                      m.b_ProcL.alua_14, m.b_ProcL.alua_15},
                     {m.b_ProcH.Pdata_00, m.b_ProcH.Pdata_01, m.b_ProcH.Pdata_02,
                      m.b_ProcH.Pdata_03, m.b_ProcH.Pdata_04, m.b_ProcH.Pdata_05,
                      m.b_ProcH.Pdata_06, m.b_ProcH.Pdata_07,
                      m.b_ProcL.Pdata_08, m.b_ProcL.Pdata_09, m.b_ProcL.Pdata_10,
                      m.b_ProcL.Pdata_11, m.b_ProcL.Pdata_12, m.b_ProcL.Pdata_13,
                      m.b_ProcL.Pdata_14, m.b_ProcL.Pdata_15},
                     {m.b_ProcH.alub_00, m.b_ProcH.alub_01, m.b_ProcH.alub_02,
                      m.b_ProcH.alub_03, m.b_ProcH.alub_04, m.b_ProcH.alub_05,
                      m.b_ProcH.alub_06, m.b_ProcH.alub_07,
                      m.b_ProcL.alub_08, m.b_ProcL.alub_09, m.b_ProcL.alub_10,
                      m.b_ProcL.alub_11, m.b_ProcL.alub_12, m.b_ProcL.alub_13,
                      m.b_ProcL.alub_14, m.b_ProcL.alub_15},
                     m.aluOut_eq_0_p_, m.aluOut_eq_0_p___ProcH,
                     m.aluOut_eq_0_p___ProcL, m.ResEqZero_p_);
            n_ckt = n_ckt + 1;
          end
          if (!visited[lastpc[11:0]]) begin
            visited[lastpc[11:0]] = 1'b1; nvisited = nvisited + 1;
          end
          if (n_tr < 256) begin
            tr_pc[n_tr] = lastpc[11:0]; tr_ff[n_tr] = ff_now;
            tr_ct[n_tr] = {m.b_ContA.CTask_0, m.b_ContA.CTask_1,
                           m.b_ContA.CTask_2, m.b_ContA.CTask_3};
            n_tr = n_tr + 1;
          end
          ctnow = {m.b_ContA.CTask_0, m.b_ContA.CTask_1,
                   m.b_ContA.CTask_2, m.b_ContA.CTask_3};
          if (!vis_t[ctnow][lastpc[11:0]]) begin
            vis_t[ctnow][lastpc[11:0]] = 1'b1;
            nvis_t[ctnow] = nvis_t[ctnow] + 1;
          end
          if (lastpc === prevpc) begin
            stuck = stuck + 1;
            // WHICH address it spins on, not just how long. A spin is a
            // microcode wait loop, and naming the address turns it into
            // something mbdis can disassemble.
            if (stuck > maxrun) begin maxrun = stuck; maxpc = lastpc; end
          end else stuck = 0;
          prevpc = lastpc;
        end
      end
      if (m.b_ContA.clk1_p_Ca !== p1) begin n1a = n1a + 1; p1 = m.b_ContA.clk1_p_Ca; end
      if (n_tnia < 32) begin
        for (q = 0; q < n_tnia; q = q + 1)
          if (tnia_seen[q] === tnia_now) q = 1000;
        if (q < 1000) begin tnia_seen[n_tnia] = tnia_now; n_tnia = n_tnia + 1; end
      end
      if (ff_now === 8'o100) n_pcf = n_pcf + 1;
      if (m.StopMIRClk)              n_smc    = n_smc + 1;
      if (m.b_ContB.StopMIRClkEn)    n_smce   = n_smce + 1;
      if (!m.b_ContB.IMLHPE_p_)      n_imlhpe = n_imlhpe + 1;
      if (!m.b_ContB.IMRHPE_p_)      n_imrhpe = n_imrhpe + 1;
      if (!m.b_ContA.Error_p_)       n_err    = n_err + 1;
      if (!m.b_ContB.IMLHPE_p_ || !m.b_ContB.IMRHPE_p_) begin
        if (first_pe < 0) first_pe = j2;
        last_pe = j2;
      end
      if (m.b_ContA.Stop && first_stop < 0) first_stop = j2;
`ifdef WORLD
      if (m.Freeze) n_freeze = n_freeze + 1;
      if (m.b_ContA.RepeatCur) n_rep = n_rep + 1;
      if (m.TWReq_15) n_twr15 = n_twr15 + 1;
      if (m.b_MemX.Faults)     n_faults   = n_faults + 1;
      if (m.b_MemX.WakeEnable) n_wen      = n_wen + 1;
      if (m.b_MemX.StkWake)    n_stkwake  = n_stkwake + 1;
      if (!m.b_MemX.ReportFault_p_)        n_rf   = n_rf + 1;
      if (!m.b_MemX._u_FaultInfoDly_p_)      n_fid  = n_fid + 1;
      if (!m.b_MemX.ReportSE_p_)           n_se   = n_se + 1;
      if (!m.b_MemX.ECFault_p_)            n_ecf  = n_ecf + 1;
      if (!m.b_MemX.MemError_p_)           n_merr = n_merr + 1;
      if (!m.b_MemX.ValidMapFltInEc2_p_)   n_vmf  = n_vmf + 1;
      if (!m.b_MemX.CacheLoad_p_)          n_cl   = n_cl + 1;
      if (!m.b_MemX.WakeOnCL_p_)           n_wocl = n_wocl + 1;
      if (!m.b_MemX.ChkLastPh6_p_)         n_chk  = n_chk + 1;
`endif
      if (m.b_ContA.CAHold_p_) n_cah    = n_cah + 1;
      if (m.b_ContA.SwitchUp)  n_swu    = n_swu + 1;
      if (m.b_ContA.Hold)      n_holdA  = n_holdA + 1;
      if (m.Hold)              n_hold_top = n_hold_top + 1;
      n_ctask[{m.b_ContA.CTask_0, m.b_ContA.CTask_1,
               m.b_ContA.CTask_2, m.b_ContA.CTask_3}] =
        n_ctask[{m.b_ContA.CTask_0, m.b_ContA.CTask_1,
                 m.b_ContA.CTask_2, m.b_ContA.CTask_3}] + 1;
      if ({m.b_ContA.CTask_0, m.b_ContA.CTask_1,
           m.b_ContA.CTask_2, m.b_ContA.CTask_3} !== ct_prev) begin
        if (first_switch < 0) begin
          first_switch = j2;
          first_switch_to = {m.b_ContA.CTask_0, m.b_ContA.CTask_1,
                             m.b_ContA.CTask_2, m.b_ContA.CTask_3};
        end
        n_ctchg = n_ctchg + 1;
        ct_prev = {m.b_ContA.CTask_0, m.b_ContA.CTask_1,
                   m.b_ContA.CTask_2, m.b_ContA.CTask_3};
      end
      if (n_ff < 32) begin
        for (q = 0; q < n_ff; q = q + 1)
          if (ff_seen[q] === ff_now) q = 1000;
        if (q < 1000) begin ff_seen[n_ff] = ff_now; n_ff = n_ff + 1; end
      end
`ifdef WORLD
      // IS THE IFU ACTUALLY FETCHING? The opcode it decodes is J.0-7, and the
      // reference it makes to get it is IfuMemRef/IfuMemAck. If the opcode
      // never changes, the machine is dispatching on a stuck value rather than
      // on an instruction stream -- which is exactly the difference between
      // "it reaches an opcode handler" and "it is running a program".
      if (m.b_IFU.IfuMemRef !== ifu_prev_ref) begin
        n_ifuref = n_ifuref + 1; ifu_prev_ref = m.b_IFU.IfuMemRef;
      end
      if (m.b_IFU.IfuMemAck !== ifu_prev_ack) begin
        n_ifuack = n_ifuack + 1; ifu_prev_ack = m.b_IFU.IfuMemAck;
      end
      if (j_now !== j_prev) begin n_jchg = n_jchg + 1; j_prev = j_now; end
      if (m.b_MemD.Fclk_p_a !== fclk_prev) begin
        n_fclk = n_fclk + 1; fclk_prev = m.b_MemD.Fclk_p_a;
      end
      if (m.b_MemD.GLd_p_ !== gld_prev) begin n_gld = n_gld + 1; gld_prev = m.b_MemD.GLd_p_; end
      if (fg_now !== fg_prev) begin n_fg = n_fg + 1; fg_prev = fg_now; end
      if (!m.b_MemD.EnableFG_p_) n_enfg = n_enfg + 1;
      if (m.b_IFU.RamPe)             n_rampe  = n_rampe + 1;
      if (m.b_IFU.SawRamParityErr)   n_sawram = n_sawram + 1;
      if (!m.b_IFU.FGParityErr_p_)   n_fgpe   = n_fgpe + 1;
      if (m.b_IFU.SawFGParityErr)    n_sawfg  = n_sawfg + 1;
      if (!m.b_MemD.D0ACE_p_a &&  m.b_MemD.D1ACE_p_a) n_d0 = n_d0 + 1;
      if ( m.b_MemD.D0ACE_p_a && !m.b_MemD.D1ACE_p_a) n_d1 = n_d1 + 1;
      if (!m.b_MemD.D0ACE_p_a && !m.b_MemD.D1ACE_p_a) n_both = n_both + 1;
      if ( m.b_MemD.D0ACE_p_a &&  m.b_MemD.D1ACE_p_a) n_neither = n_neither + 1;
      // ...and whether the two halves are even addressed alike.
      if ({m.b_MemD.Dad0_10a, m.b_MemD.Dad0_11a, m.b_MemD.Dad0_12a} !==
          {m.b_MemD.Dad1_10a, m.b_MemD.Dad1_11a, m.b_MemD.Dad1_12a})
        n_addrdiff = n_addrdiff + 1;
      // THE HOLD CHAIN, COUNTED. This used to print the eight signals at ten
      // instants and conclude "all zero, whole run" -- which was true of the
      // machine BEFORE IFUM was loaded and the parity fixed, and is stale now.
      // `Hold` is asserted 99.85% of the time here. COUNT, do not sample.
      if (m.Hold)     n_h_hold  = n_h_hold + 1;
      if (m.PRhold)   n_h_pr    = n_h_pr + 1;
      if (m.CBHold)   n_h_cb    = n_h_cb + 1;
      if (m.IfuHold)  n_h_ifu   = n_h_ifu + 1;
      if (m.IOHold)   n_h_io    = n_h_io + 1;
      if (m.MXHold)   n_h_mx    = n_h_mx + 1;
      if (m.DisHold)  n_h_dis   = n_h_dis + 1;
      if (m.CHoldReq) n_h_creq  = n_h_creq + 1;
      if (m.LargeHold) n_h_lg   = n_h_lg + 1;
      if (m.HoldMapBuf) n_h_hmb = n_h_hmb + 1;
      if (m.ExtHoldReq) n_h_ext = n_h_ext + 1;
      if (!m.b_MemC.MDhold_p_)   n_md   = n_md + 1;
      if (!m.b_MemC.RefHold_p_)  n_ref  = n_ref + 1;
      if (!m.b_MemC.MiscHold_p_) n_misc = n_misc + 1;
      if (m.b_MemC.BLretry)      n_blr  = n_blr + 1;
      if (m.PrHoldReq)      n_prq = n_prq + 1;
      if (m.b_MemC.WantCR)  n_wcr = n_wcr + 1;
      n_stk[{m.b_ProcL.StkP_0, m.b_ProcL.StkP_1, m.b_ProcL.StkP_2,
             m.b_ProcL.StkP_3, m.b_ProcL.StkP_4, m.b_ProcL.StkP_5,
             m.b_ProcL.StkP_6, m.b_ProcL.StkP_7}] =
        n_stk[{m.b_ProcL.StkP_0, m.b_ProcL.StkP_1, m.b_ProcL.StkP_2,
               m.b_ProcL.StkP_3, m.b_ProcL.StkP_4, m.b_ProcL.StkP_5,
               m.b_ProcL.StkP_6, m.b_ProcL.StkP_7}] + 1;
      if ({m.b_ProcL.StkP_0, m.b_ProcL.StkP_1, m.b_ProcL.StkP_2,
           m.b_ProcL.StkP_3, m.b_ProcL.StkP_4, m.b_ProcL.StkP_5,
           m.b_ProcL.StkP_6, m.b_ProcL.StkP_7} !== stk_prev) begin
        if (n_stkchg < 8) begin
          stk_when[n_stkchg] = j2;  stk_from[n_stkchg] = stk_prev;
          stk_to[n_stkchg]   = {m.b_ProcL.StkP_0, m.b_ProcL.StkP_1, m.b_ProcL.StkP_2,
                                m.b_ProcL.StkP_3, m.b_ProcL.StkP_4, m.b_ProcL.StkP_5,
                                m.b_ProcL.StkP_6, m.b_ProcL.StkP_7};
          stk_pc[n_stkchg]   = tnia_now[11:0];
          stk_ff[n_stkchg]   = ff_now;
          stk_rstk[n_stkchg] = {m.RSTK_0, m.RSTK_1, m.RSTK_2, m.RSTK_3};
        end
        n_stkchg = n_stkchg + 1;
        stk_prev = {m.b_ProcL.StkP_0, m.b_ProcL.StkP_1, m.b_ProcL.StkP_2,
                    m.b_ProcL.StkP_3, m.b_ProcL.StkP_4, m.b_ProcL.StkP_5,
                    m.b_ProcL.StkP_6, m.b_ProcL.StkP_7};
      end
`endif
      if (j2 % (runcycles/10) == 0)
        $display("      Error'=%b IMLHPE'=%b IMRHPE'=%b A31+5=%b dStop=%b Run'=%b",
          m.b_ContA.Error_p_, m.b_ContB.IMLHPE_p_, m.b_ContB.IMRHPE_p_,
          m.b_ContA.ContA31_sil_pl_5, m.b_ContA.dStop, m.b_ContA.Run_p_);
      if (j2 % (runcycles/10) == 0)
        $display("   t=%5d  clk0'=%0d  FF=%b RSTK=%b%b%b%b TNIA=%h Stop=%b",
          j2, n0a,
          ~{m.b_ContA.FF_0_p_,m.b_ContA.FF_1_p_,m.b_ContA.FF_2_p_,m.b_ContA.FF_3_p_,
            m.b_ContA.FF_4_p_,m.b_ContA.FF_5_p_,m.b_ContA.FF_6_p_,m.b_ContA.FF_7_p_},
          m.RSTK_0, m.RSTK_1, m.RSTK_2, m.RSTK_3,
          {m.TNIA_04,m.TNIA_05,m.TNIA_06,m.TNIA_07,m.TNIA_08,m.TNIA_09,
           m.TNIA_10,m.TNIA_11,m.TNIA_12,m.TNIA_13,m.TNIA_14,m.TNIA_15},
          m.b_ContA.Stop);
    end
    $display("tb_exec: %0d clk0' edges, %0d clk1' over %0d cycles, Stop=%b",
             n0a, n1a, runcycles, m.b_ContA.Stop);
    $display("tb_exec: %0d distinct TNIA values, %0d distinct FF values seen",
             n_tnia, n_ff);
`ifdef WORLD
    // PIN THE OFFSET, rather than eyeballing it. The preload already loaded
    // every address's FF from the .MB, so the question "how many stages apart
    // are TNIA and the decoded FF" is answerable by correlation: for each
    // candidate offset, count the samples where the observed FF equals the FF
    // the .MB gives for the address at that offset. The right offset should
    // stand far above the others; if none does, the two are not simply skewed
    // and pairing them at all is wrong.
    for (koff = -3; koff <= 3; koff = koff + 1) begin
      nmatch = 0; ncmp = 0;
      for (i = 0; i < n_tr; i = i + 1) begin
        if (i + koff < 0 || i + koff >= n_tr) continue;
        if (!f_have[tr_pc[i + koff]]) continue;
        // ONLY WHERE THE PC MOVES. 240 of the 256 samples are the identical
        // stuck pair, and counting them drowns the signal in a constant
        // mismatch -- the first attempt scored 14 of 255 at best and looked
        // like "no offset fits" when it really meant "the sample is 94% one
        // repeated point".
        if (i > 0 && tr_pc[i] === tr_pc[i-1]) continue;
        ncmp = ncmp + 1;
        if (tr_ff[i] === f_ff[tr_pc[i + koff]]) nmatch = nmatch + 1;
      end
      $display("tb_exec: OFFSET %0d: FF matches the .MB at pc[i%0s%0d] on %0d of %0d",
               koff, (koff < 0) ? "-" : "+", (koff < 0) ? -koff : koff, nmatch, ncmp);
    end
    $write("tb_exec: TRACE (pc/ff, first %0d):", n_tr);
    for (i = 0; i < n_tr; i = i + 1) $write(" t%0d:%h", tr_ct[i], tr_pc[i]);
    $write("\n");
    for (i = 0; i < n_stkchg && i < 8; i = i + 1)
      $display("tb_exec: STKP change %0d at cycle %0d: %02h -> %02h  (TNIA=%h FF=%03o RSTK=%h)",
               i, stk_when[i], stk_from[i], stk_to[i], stk_pc[i], stk_ff[i], stk_rstk[i]);
    $write("tb_exec: STKP -- changed %0d times; values held:", n_stkchg);
    for (i = 0; i < 256; i = i + 1)
      if (n_stk[i] != 0) $write(" %02h(x%0d)", i[7:0], n_stk[i]);
    $write("\n");
    $display("tb_exec: HOLDREQ of %0d -- PrHoldReq %0d, WantCR %0d (CHoldReq/ExtHoldReq are 0)",
             runcycles, n_prq, n_wcr);
    $display("tb_exec: HOLDSRC of %0d -- MDhold %0d, RefHold %0d, MiscHold %0d, BLretry %0d",
             runcycles, n_md, n_ref, n_misc, n_blr);
    $display("tb_exec: HOLDS of %0d -- Hold %0d PRhold %0d CBHold %0d IfuHold %0d IOHold %0d",
             runcycles, n_h_hold, n_h_pr, n_h_cb, n_h_ifu, n_h_io);
    $display("tb_exec: HOLDS       MXHold %0d DisHold %0d CHoldReq %0d LargeHold %0d HoldMapBuf %0d ExtHoldReq %0d",
             n_h_mx, n_h_dis, n_h_creq, n_h_lg, n_h_hmb, n_h_ext);
    $display("tb_exec: REPEAT -- at f20: CAHold'=%0d SwitchUp=%0d ContA.Hold=%0d (of %0d); top Hold=%0d",
             n_cah, n_swu, n_holdA, runcycles, n_hold_top);
    $display("tb_exec: FAULTSRC of %0d -- ReportFault %0d, _FaultInfoDly %0d | ReportSE %0d ECFault %0d MemError %0d ValidMapFlt %0d CacheLoad %0d WakeOnCL %0d ChkLastPh6 %0d",
             runcycles, n_rf, n_fid, n_se, n_ecf, n_merr, n_vmf, n_cl, n_wocl, n_chk);
`ifdef WORLD
    $display("tb_exec: FAULTCTR -- k09 q=%b, Faults=%b (TC is LOW only at 1111)",
             m.b_MemX.u_k09.q, m.b_MemX.Faults);
`endif
    $display("tb_exec: WAKESRC of %0d -- Faults %0d, WakeEnable %0d, StkWake %0d",
             runcycles, n_faults, n_wen, n_stkwake);
    $display("tb_exec: WAKE -- TWReq.15 high on %0d of %0d; first task switch at cycle %0d, to task %0d",
             n_twr15, runcycles, first_switch, first_switch_to);
    $write("tb_exec: TASK -- Freeze %0d, RepeatCur %0d of %0d; CTask changed %0d times; occupancy:",
           n_freeze, n_rep, runcycles, n_ctchg);
    for (i = 0; i < 16; i = i + 1) if (n_ctask[i] != 0) $write(" t%0d=%0d", i, n_ctask[i]);
    $write("\n");
    $display("tb_exec: WHEN -- first parity error at cycle %0d, last at %0d; Stop first set at %0d",
             first_pe, last_pe, first_stop);
    $display("tb_exec: MIR -- StopMIRClk %0d, StopMIRClkEn %0d, IMLHPE %0d, IMRHPE %0d, Error %0d, of %0d",
             n_smc, n_smce, n_imlhpe, n_imrhpe, n_err, runcycles);
    $display("tb_exec: PCF -- `PCF<-B` (FF=0o100, starts the IFU pipeline) seen on %0d samples",
             n_pcf);
    $display("tb_exec: CACHE -- D0 only %0d, D1 only %0d, BOTH enabled %0d, neither %0d, addr differs %0d, of %0d",
             n_d0, n_d1, n_both, n_neither, n_addrdiff, runcycles);
    $display("tb_exec: IFUPE -- RamPe high %0d, SawRamParityErr %0d, FGParityErr %0d, SawFGParityErr %0d, of %0d",
             n_rampe, n_sawram, n_fgpe, n_sawfg, runcycles);
    $display("tb_exec: BYTES -- Fclk'a %0d edges, GLd' %0d, FG changed %0d, EnableFG' low on %0d of %0d",
             n_fclk, n_gld, n_fg, n_enfg, runcycles);
    $display("tb_exec: IFU -- IfuMemRef %0d transitions, IfuMemAck %0d, opcode J changed %0d times",
             n_ifuref, n_ifuack, n_jchg);
`endif
    $display("tb_exec: %0d distinct IM addresses executed; last TNIA=%h; longest run on one address=%0d at TNIA=%h",
             nvisited, lastpc[11:0], maxrun, maxpc[11:0]);
    // Name them. A short cycle is the normal shape of a microcode wait loop, and
    // knowing WHICH addresses turns "it loops" into something disassemblable.
    if (nvisited <= 64) begin
      $write("tb_exec: PER TASK --");
    for (i = 0; i < 16; i = i + 1)
      if (nvis_t[i] != 0) $write(" t%0d:%0d addresses", i, nvis_t[i]);
    $write("\n");
    $write("tb_exec: addresses executed:");
      for (i = 0; i < 4096; i = i + 1) if (visited[i]) $write(" %h", i[11:0]);
      $write("\n");
    end
    if (m.b_ContA.Stop !== 1'b0)
      $fatal(1, "the machine stopped -- with the parity enables on it stops after one instruction");
    // ...and a LOWER floor once the machine is initialised enough to do real
    // work. The runcycles/20 figure assumes roughly one microinstruction per
    // SYSPER fabric cycles with nothing stalling; a machine actually issuing
    // memory references legitimately issues FEWER. With `+tpcinit` the fault
    // handler runs BEGINENUMERATEMAP and IWRITEMAPFLAGS, which reference
    // storage, and the count falls to 16,676 of a 25,000 ceiling while `Stop`
    // stays clear and `Hold` is asserted on only 96 samples of 400,000. That is
    // work, not a stall, so the floor is halved rather than the machine
    // declared broken.
    if (n0a < (($test$plusargs("tpcinit")) ? runcycles/40 : runcycles/20))
      $fatal(1, "the microinstruction clock is not free-running (%0d edges)", n0a);
    // ONE OF EACH PER MICROINSTRUCTION, to within the window boundary. The
    // sample window is a fixed number of FABRIC cycles, so it can close
    // between clk0' and clk1' of the same microinstruction and leave the
    // counts one apart -- which is the property holding, not failing. This
    // used to demand exact equality, and that is a boundary artifact rather
    // than a real difference: at 8 sys_clk per microinstruction it read
    // 2492 against 2493 out of ~2492.
    // ...and a WIDER slack when the parity enables are switched on mid-run,
    // because `manifold()` BLOCKS the sampling loop for many sys_clk while it
    // shifts twelve bits out, so edges of both clocks go uncounted and not
    // evenly. That is the bench's own instrumentation gap, not the machine:
    // measured 24991 against 24994. Not widened for the ordinary run.
    // The base slack is TWO, not one. The existing note explains one: the
    // sample window is a fixed number of FABRIC cycles, so it can close between
    // clk0' and clk1' of the same microinstruction. Once instructions can
    // REPEAT (RepeatCur) or a reference stalls, a second boundary lands the same
    // way -- measured 16,676 against 16,678 with the fault handler running. Two
    // is still a property holding; a real divergence is thousands.
    if (n1a > n0a + (late_par ? 8 : 2) || n0a > n1a + (late_par ? 8 : 2))
      $fatal(1, "clk1' must run with clk0', one of each per microinstruction (%0d vs %0d)",
             n0a, n1a);
    // A machine fetching from WIPED IM would sit on one instruction for ever.
    if (n_tnia < 4)
      $fatal(1, "TNIA is not sequencing -- the machine is not fetching from IM");
    if (n_ff < 4)
      $fatal(1, "the decoded FF field never changes -- nothing is being fetched");
    if (late_par) begin
      // THE POINT OF THE MODE: the machine runs with IM parity ENABLED. The
      // errors counted are the power-up transient BEFORE the enables went on
      // at cycle 400; what matters is that `Error'` never propagated and the
      // machine never stopped.
      $display("tb_exec: IM PARITY ENABLED -- Error propagated on %0d samples, Stop=%b, %0d clk0' edges",
               n_err, m.b_ContA.Stop, n0a);
      if (n_err != 0)
        $fatal(1, "an IM parity error propagated with the enables on");
      if (m.b_ContA.Stop !== 1'b0)
        $fatal(1, "the machine stopped with the IM parity enables on");
      if (last_pe > 400)
        $fatal(1, "IM parity errors continue past the enable point -- the preloaded array's parity is wrong");
      $display("tb_exec: THE MACHINE RUNS WITH IM PARITY ON.");
    end
    if ($test$plusargs("bootchain")) begin
      // PARC'S OWN BOOT CHAIN, first two stages. Initial.mb occupies real
      // 0xc00-0xfbf and Bootstrap.mb 0xfc0-0xfff -- adjacent and disjoint, so
      // they coexist by design. `INITIAL` is image 1 = real 0xf40, and its
      // JCN is 0xff, a GLOBAL CALL: TNIA = CIA[2:3] || JCN[2:7] || 000000 =
      // 0b11 || 111111 || 000000 = 0xfc0, which is Bootstrap image 54,
      // `READBB` -- read BaseBoard.
      //
      // So the machine runs Initial's entry, calls into Bootstrap, and sits in
      // the read-from-control-processor loop (images 55/56/57/60 = real
      // 0xfe7/0xfe2/0xfe1/0xfe6) waiting to be fed. THAT IS THE CORRECT PLACE
      // TO STOP: this configuration has no BaseBoard, and nothing is sending.
      // INITIAL'S MILESTONES, each a label from InitialMain/InitialDisplay
      // mapped through im_image (whose image indices print in OCTAL, the
      // same trap as mbdis). visited[] is indexed by real IM address.
      $display("tb_exec: MARMUX -- MarMuxAEn' high on %0d of %0d", n_marA, n_cyc2);
      $display("tb_exec: BOOTFF -- IfuReset first@%0d (%0d edges), LdMcr' first@%0d (%0d edges), MDhold first@%0d, DisHold first@%0d",
               c_ifur, n_ifur, c_mcr, n_mcr, c_mdh, c_dish);
      $display("tb_exec: INITIAL MILESTONES -- ChkSumErr(f46)=%0d RMINITL(c42)=%0d STKINITL(c4a)=%0d IFUMINITL(c65)=%0d TASKINITLOOP(c45)=%0d BOOTEMULATOR(c92)=%0d INITHRAM(e20)=%0d",
               visited[12'hf46], visited[12'hc42], visited[12'hc4a],
               visited[12'hc65], visited[12'hc45], visited[12'hc92],
               visited[12'he20]);
      nboot_region = 0;
      for (i = 12'hfe0; i <= 12'hfff; i = i + 1)
        if (visited[i]) nboot_region = nboot_region + 1;
      $display("tb_exec: BOOTCHAIN -- INITIAL(0xf40)=%b READBB(0xfc0)=%b, %0d addresses in Bootstrap's poll loop, longest run %0d",
               visited[12'hf40], visited[12'hfc0], nboot_region, maxrun);
      if (!visited[12'hf40])
        $fatal(1, "Initial's entry never executed");
      if (!visited[12'hfc0])
        $fatal(1, "Initial did not call Bootstrap's READBB");
      if (nboot_region < 3)
        $fatal(1, "Bootstrap is not running its read loop -- %0d addresses", nboot_region);
      if (maxrun > 8)
        $fatal(1, "the boot chain is stuck: %0d microinstructions on one address", maxrun);
      $display("tb_exec: PARC'S BOOT CHAIN RUNS -- Initial calls Bootstrap, which waits for the control processor.");
    end
    if ($test$plusargs("tpcinit")) begin
      // THE FAULT TASK RUNS ITS OWN HANDLER. With TPC[15] preset to AEmu's
      // `FAULTTASK` (octal 3747) the fault task executes the fault-handler
      // region -- octal 3700-3733, which mbdis names BEGINENUMERATEMAP and
      // IWRITEMAPFLAGS -- instead of the emulator handlers it lands on when
      // TPC[15] is left uninitialised.
      //
      // AND THE SLOT PROVES THE ADDRESS REVERSAL. The TPC array's address pins
      // are `TPCAd.0-3'`, primed, so a = ~task and TPC[15] is mem[0].
      // `+tpcslot=15` writes the un-reversed slot and has NO effect at all --
      // identical counts to not presetting -- which is the measurement that
      // settles it rather than an appeal to convention.
      nfault_region = 0;
      for (i = 12'h700; i < 12'h800; i = i + 1)
        if (vis_t[15][i]) nfault_region = nfault_region + 1;
      $display("tb_exec: FAULTPC -- task 15 executed %0d addresses in the fault-handler region (0x700-0x7ff)",
               nfault_region);
`ifdef SCREEN
      // NOT this gate's business. With DispY in the machine the winning task
      // is 3, not 15 -- the display board requests and outranks the fault
      // task -- so "task 15 never ran its handler" is the expected outcome
      // here rather than a failure.
      $display("tb_exec: (SCREEN) task 15 ran %0d handler addresses; with DispY present task 3 wins", nfault_region);
`else
      if (nfault_region < 5)
        $fatal(1, "the fault task is not running its handler -- only %0d addresses above 0x700",
               nfault_region);
`endif
      if (nvis_t[0] < 10)
        $fatal(1, "the emulator task barely runs: %0d addresses", nvis_t[0]);
      $display("tb_exec: THE FAULT TASK RUNS ITS OWN HANDLER.");
    end
    if ($test$plusargs("taskingon")) begin
      // WITH TASKING ON, the stack underflow is SERVICED. HM Table 6's response
      // to StkP = 0 is HOLD *and wake fault task 15*; task 15 is what releases
      // it. With TaskingOff,Return the fault task can never run and the machine
      // freezes on one instruction for ever; with TaskingOn it does run.
      $display("tb_exec: TASKING -- RepeatCur %0d (was 399419 with tasking off), fault task 15 held CTask on %0d samples",
               n_rep, n_ctask[15]);
      // RepeatCur must be EXACTLY zero on the bare tasking run -- that is the
      // whole point of it, against 399,419 with tasking off. Once the fault
      // counter is armed and TPC[15] is set (`+tpcinit`) the machine does real
      // work and takes brief, legitimate holds while references complete:
      // measured 64 of 400,000, with `Hold` on 96. A budget, not zero.
      if (n_rep > ($test$plusargs("tpcinit") ? 1000 : 0))
        $fatal(1, "the machine is still being held with tasking on (%0d)", n_rep);
      if (n_ctask[15] == 0)
        $fatal(1, "fault task 15 never ran -- the underflow was not serviced");
      if (nvisited < 20)
        $fatal(1, "only %0d distinct addresses -- the machine is not sequencing", nvisited);
      if (maxrun > 100)
        $fatal(1, "still spinning: %0d microinstructions on one address", maxrun);
      $display("tb_exec: THE FAULT TASK SERVICES THE STACK UNDERFLOW AND THE MACHINE RUNS.");
    end
`ifdef SCREEN
    // IS THE CRYSTAL EVEN TURNING? DispY carries its own 50 MHz oscillator
    // (a05, a K1115A), which free-runs regardless of what the microcode does
    // -- so if the horizontal timing chain is dead while the crystal turns,
    // the fault is downstream, and if the crystal is dead nothing else can
    // matter. Check the source before the sink.
    if (pgm_on) write_pgm;
    $display("tb_exec: DISPY CLOCK -- PixelClk %0d, crystal a05 out %0d transitions, AltoCSync' %0d (control: same crystal %0d)",
             n_pix, n_xtal, n_wdwt, n_wdht);
    $display("tb_exec: HRAM GATE -- DoradoHasHRam high on %0d of %0d samples; ClkHRamAddr' %0d transitions",
             n_hasram1, n_samp, n_clkhra);
    $display("tb_exec: HRAM COUNTER -- CountHRamAddr' high (NOT counting) on %0d, LdHRamAddr' on %0d, ClearHRamAddr on %0d, of %0d",
             n_cnt1, n_ld1, n_clr1, n_samp);
    $display("tb_exec: HRAM -- address counter bit01 %0d transitions, preHSync (the RAM's output) %0d",
             n_hra, n_phs);
    $display("tb_exec: SYNC CHAIN -- RamHSync %0d transitions, HSync %0d; PC'2ClkEn' high (l10 NOT counting) on %0d of %0d",
             n_rhs, n_hs2, n_pce1, n_samp);
    $display("tb_exec: VIDEO transitions -- AltoTTLVideo %0d, AltoHSync %0d, AltoVSync' %0d, HBlank %0d, VBlank %0d, HalfLine %0d",
             n_vid, n_hs, n_vs, n_hb, n_vb, n_hl);
    if (n_vid + n_hs + n_vs + n_hb + n_vb + n_hl <= 6)
      $display("tb_exec: the display board is DEAD -- no video signal moved (a count of 1 is the power-up settle out of X, not a transition).");
    else
      $display("tb_exec: THE DISPLAY BOARD IS ALIVE.");
`endif
    $display("tb_exec: the machine executes microcode out of IM.");
    $finish;
  end

endmodule

`default_nettype wire
