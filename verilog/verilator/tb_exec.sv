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
// Historical diagnosis (fixed by physical MC10170 parameters on ContA e18,
// ContB j20 and ContB e01): THE PARITY ENABLES HAD TO BE OFF TO RUN.
// bootstrap-parity-test now checks original CP-loaded words, execution with
// both checks enabled from startup, and rejection of incorrect parity.
// The old observation below explains the first-address stall:
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

  // Fabric samples per 30 ns clock, with two clocks per microinstruction.
  // Instructions/second = sys_clk / (2*SYSPER), per HM 2.3. Passing short
  // tests at a ratio does not establish full boot at that ratio.
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
  wire eth_xmt;   // DskEth's XmtData' in the FULL configuration
  reg  rcv_r = 1'b0;   // DskEth's RcvData in the FULL configuration (+rcvplay)

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
  // REFRESH IS A REAL-TIME INTERVAL. `RfshPeriod` is a BaseBoard line, and no
  // bench configuration carries the BaseBoard, so the bench makes it. HM
  // p.129 (Refresh): "refresh references once every 16 us ... During the
  // first 8 us of a 16 us period, refresh defers to ..." -- so the line is a
  // 16 us square wave, 8 us per half. At SYSPER sys_clk per 30 ns CLOCK that
  // is 8000 / 30 x SYSPER = 4267 sys_clk per half at 16x. It used to be 512
  // (a 9-bit free-running counter, "every 16 microinstructions"), 8.3x too
  // fast: the map did a refresh cycle between EVERY pair of InitMap's map
  // writes (VAWIN, 2026-09-08), a third of the map's bandwidth, and the
  // storage the same. `+rfshdiv=N` overrides the half-period in sys_clk.
  integer rfshhalf; initial begin
    rfshhalf = (8000 * `SYSPER_OVERRIDE) / 30;
    void'($value$plusargs("rfshdiv=%d", rfshhalf));
  end
  integer   rfshdiv = 0;
  reg       rfshper = 1'b0;
  always @(posedge sys_clk) begin
    rfshdiv <= rfshdiv + 1;
    if (rfshdiv >= rfshhalf - 1) begin rfshdiv <= 0; rfshper <= ~rfshper; end
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
  // This oscillator's SYSPER counts samples per instruction, like Trident;
  // the bench's ratio counts samples per clock. HM 2.3: two clocks/cycle.
  cell_K1115A #(.FREQ_KHZ(50000), .SYSPER(2 * SYSPER)) u_pixclk (
      .sys_clk(sys_clk), .p8(pixel_clk), .p7(1'b0), .p14(1'b1));
  integer n_pix = 0; reg d_pix;
  always @(posedge sys_clk) begin
    if (pixel_clk !== d_pix) n_pix = n_pix + 1;
    d_pix <= pixel_clk;
  end
`ifdef FULL
  // Independent units check: 50 MHz has a 20 ns period; a fabric sample
  // represents 30/SYSPER ns. Count transitions, including both pixel edges.
  reg check_clock_domains = 0;
  integer storage_even_edges=0, storage_odd_edges=0;
  reg storage_even_d=0, storage_odd_d=0;
  always @(posedge sys_clk) begin
    if(m.b_msa.CLK_ms0Even_p_ && !storage_even_d) storage_even_edges=storage_even_edges+1;
    if(m.b_msaOdd.CLK_ms0Even_p_ && !storage_odd_d) storage_odd_edges=storage_odd_edges+1;
    storage_even_d<=m.b_msa.CLK_ms0Even_p_;
    storage_odd_d<=m.b_msaOdd.CLK_ms0Even_p_;
  end
  initial check_clock_domains = $test$plusargs("check-clock-domains");
  always @(posedge sys_clk)
    if (check_clock_domains && n_cyc2 == 100000) begin
      if (n_pix < 300000/SYSPER-3 || n_pix > 300000/SYSPER+3)
        $fatal(1,"50 MHz pixel oscillator: %0d transitions, expected %0d",n_pix,300000/SYSPER);
      if (u_drv.SYSPER != 2*SYSPER)
        $fatal(1,"Trident ratio must count both clocks per instruction");
      if (m.b_DskEth.u_a05.TW != 100*SYSPER)
        $fatal(1,"disk index discriminator must last 3 us");
      if(storage_even_edges==0 || storage_even_edges!=storage_odd_edges)
        $fatal(1,"both storage slots must be clocked: even=%0d odd=%0d",storage_even_edges,storage_odd_edges);
      $display("tb_exec: CLOCK-DOMAINS PASS -- pixel=50 MHz, disk index=3 us, two clocks/instruction, both storage slots clocked (SYSPER=%0d)",SYSPER);
    end
`endif
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

  // ---- A WHOLE FRAME, WHEN THERE IS ONE -----------------------------------
  //
  // `+frame`: the same pixel-rate sampling, but organised as a FIELD -- a new
  // line on HSync's falling edge, a new field on VSync's assertion -- into a
  // 1024x896 buffer written as dorado_frame.pgm at the end of the run. The
  // monochrome monitor is paper-white: vid=0 paints white, vid=1 paints
  // black, blanking paints grey so the frame's geometry is legible even
  // empty. If VSync never asserts the buffer degrades to the strip capture's
  // behaviour (lines accumulate to the bottom and hold), which is itself the
  // diagnostic: horizontal timing without vertical timing.
  integer frame_on, n_fields = 0, fpx = 0, fpy = 0, fi, fj;
  reg [7:0] ffb [0:895][0:1023];
  reg fd_hs, fd_vs, fd_pix;
  initial begin
    frame_on = $test$plusargs("frame");
    fpx = 0; fpy = 0; fd_hs = 1'b0; fd_vs = 1'b1; fd_pix = 1'b0;
    // Dedicated loop variables: the first cut borrowed the strip capture's
    // py/px and left them at 896/1024, silently disabling that capture.
    for (fi = 0; fi < 896; fi = fi + 1)
      for (fj = 0; fj < 1024; fj = fj + 1) ffb[fi][fj] = 8'd128;
  end
  always @(posedge sys_clk) if (frame_on) begin
    if (pixel_clk && !fd_pix) begin
      // Field boundary: AltoVSync' asserting (falling, it is active low).
      if (!vsync_n && fd_vs) begin
        n_fields = n_fields + 1;
        fpy = 0; fpx = 0;
      end
      fd_vs <= vsync_n;
      if (!hsync && fd_hs) begin
        if (fpy < 895) fpy = fpy + 1;
        fpx = 0;
      end
      fd_hs <= hsync;
      if (fpx < 1024 && fpy < 896) begin
        ffb[fpy][fpx] = (hblank || vblank) ? 8'd128 :
                        vid                ? 8'd0   : 8'd255;
        fpx = fpx + 1;
      end
    end
    fd_pix <= pixel_clk;
  end
  task write_frame;
    integer f, r, c;
    begin
      f = $fopen("dorado_frame.pgm", "w");
      $fwrite(f, "P2\n1024 896\n255\n");
      for (r = 0; r < 896; r = r + 1) begin
        for (c = 0; c < 1024; c = c + 1) $fwrite(f, "%0d ", ffb[r][c]);
        $fwrite(f, "\n");
      end
      $fclose(f);
      $display("tb_exec: +frame -- wrote dorado_frame.pgm, %0d fields seen, last line %0d (transitions: hsync %0d vsync' %0d vid %0d)",
               n_fields, fpy, n_hs, n_vs, n_vid);
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
  // `+hasram` -- what INITHRAM's control write does, done as state. h15 (an
  // MC10231) is the DoradoHasHRam flip-flop: D = RIOB.00, reset = IOReset,
  // and its clock is clk1'Dc ORed with HRamCommand' -- so it clocks ONLY
  // during an HRam command write, and a deposited value sticks. Power-up
  // leaves qa = 0, whose Q' IS DoradoHasHRam = 1, and that net is l10's
  // MASTER RESET: RamHSync/HSync can never move on an unconfigured board,
  // which is correct hardware behaviour, not a bug. (The older `+mrlow`
  // pin force is INERT under module inlining -- the forced port aliases the
  // whole net -- which is why its green era stopped reproducing.) Setting
  // qa = 1 is exactly the state INITHRAM's control write leaves.
  // ...and h15's OTHER flip-flop is HRamWE (qb), which powers up with
  // HRamWE' = qb = 0 -- ASSERTED. h14/i14/j14 then WRITE RIOB into
  // mem[addr] on every cycle as the address walks, bulldozing the +hram
  // preload: measured as 35 one-sys_clk preHSync slivers per run (one per
  // 1024-walk) where a 34-entry sync run should read high for whole
  // stretches. Both bits are exactly the state the microcode's HRam-load
  // teardown leaves: table valid (qa=1), write path off (qb=1). The +hram
  // preload is REAPPLIED after the deposit because the walk has already
  // clobbered part of the table by then.
  initial if ($test$plusargs("hasram")) begin
    repeat (1000) @(posedge sys_clk);
    m.b_DispY.u_h15.qa = 1'b1;
    m.b_DispY.u_h15.qb = 1'b1;
    if ($test$plusargs("hram")) preload_hram;
    $display("tb_exec: +hasram -- h15 qa,qb deposited: DoradoHasHRam low, HRamWE' deasserted, table reloaded");
  end

  // WHAT DOES L10 SEE AT ITS OWN EDGES? Replicate the cell's oversampled
  // edge detect (q updates at the first sys_clk where the modelled clock
  // already reads high) and sample PE' and D exactly there. If PE' reads
  // high at every detected edge while toggling overall, the load enable and
  // the clock are in a same-edge phase collision -- the d3e337a2 class.
  reg d_l10ck = 1'b0;
  integer n_l10edge = 0, n_l10pe0 = 0, n_l10d1 = 0;
  always @(posedge sys_clk) begin
    if (m.b_DispY.PixelClk_p_Bd && !d_l10ck) begin
      n_l10edge = n_l10edge + 1;
      if (!m.b_DispY.PC_s_2ClkEn_p_) begin
        n_l10pe0 = n_l10pe0 + 1;
        if (m.b_DispY.preHSync) n_l10d1 = n_l10d1 + 1;
      end
    end
    d_l10ck <= m.b_DispY.PixelClk_p_Bd;
  end

  // THE WALK ORDER: the address at consecutive ClkHRamAddr' edges. The
  // preload writes mem[0..379] LINEARLY; if the counter walk is not +1 in
  // the RAM's own address bits {_01.._10}, the two disagree and every sync
  // entry is visited as an isolated step -- which is what the one-sys_clk
  // preHSync slivers say.
  integer n_walk = 0; reg d_walkck = 1'b0;
  always @(posedge sys_clk) begin
    if (m.b_DispY.ClkHRamAddr_p_ && !d_walkck && n_walk < 24 && n_cyc2 > 2000) begin
      n_walk = n_walk + 1;
      $display("tb_exec: WALK addr01_10=%b%b%b%b%b%b%b%b%b%b addr11=%b",
               m.b_DispY.HRamAddr_01, m.b_DispY.HRamAddr_02, m.b_DispY.HRamAddr_03,
               m.b_DispY.HRamAddr_04, m.b_DispY.HRamAddr_05, m.b_DispY.HRamAddr_06,
               m.b_DispY.HRamAddr_07, m.b_DispY.HRamAddr_08, m.b_DispY.HRamAddr_09,
               m.b_DispY.HRamAddr_10, m.b_DispY.HRamAddr_11);
    end
    d_walkck <= m.b_DispY.ClkHRamAddr_p_;
  end

  // One pulse, cycle by cycle: on preHSync's rising edge print the next 40
  // sys_clk of the whole neighbourhood (twice per run).
  integer n_pht = 0, n_pht_pulses = 0; reg d_phs2 = 1'b0;
  always @(posedge sys_clk) begin
    if (m.b_DispY.preHSync && !d_phs2 && n_pht_pulses < 2) begin
      n_pht = 40;
      n_pht_pulses = n_pht_pulses + 1;
    end
    if (n_pht > 0) begin
      $display("tb_exec: PHT pclk=%b clkhra'=%b pce'=%b preHS=%b RamHS=%b",
               m.b_DispY.PixelClk_p_Bd, m.b_DispY.ClkHRamAddr_p_,
               m.b_DispY.PC_s_2ClkEn_p_, m.b_DispY.preHSync, m.b_DispY.RamHSync);
      n_pht = n_pht - 1;
    end
    d_phs2 <= m.b_DispY.preHSync;
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
  // ---- THE TERMINAL, or the keyboard words never arrive -------------------
  // `+terminal`: `dorado_terminal` -- the processor in the keyboard -- on the
  // return channel. Without it DispY's `KeyboardData` port is tied 0 (it
  // "awaits BaseBd"), the guest's ReadTerminal never sees a start bit, and the
  // Alto keyboard words at 177034B-177037B keep the ZERO Initial's storage
  // init left there. Keys are ACTIVE LOW, so zero is EVERY KEY HELD, and
  // AEm0.mc's ABoot chooses the boot by the BS key: `ETemp0, Branch[EBoot, R
  // even]`. The 230 M-cycle disk-boot run therefore went to EBOOT (0x406) and
  // never to DISKBOOT (0x405) -- 16,272 EControl writes, "DiskBoot first
  // @-1" -- while the disk task idled correctly in AltoLoop. Wired as
  // fpga/mister/Dorado.sv wires it: one bit per horizontal blanking (DispY
  // c22 is clocked by RamdHBlank), all five words all-ones = nothing pressed.
  // KeyboardData EQUALS OISData: BaseBd c06 straps OISData to the MC10125's
  // true input (pin 11) and the MC10124's p11 -> p14 channel is the
  // non-inverting one, so the bench connects the terminal's output straight
  // to the port that the BaseBoard would drive. Opt-in, so the gates that
  // predate it do not move.
  reg  term_on = 1'b0; initial term_on = $test$plusargs("terminal");
  reg  term_reset = 1'b1; integer term_rst_cnt = 0;
  always @(posedge sys_clk) if (term_reset) begin
    if (term_rst_cnt >= 63) term_reset <= 1'b0; else term_rst_cnt <= term_rst_cnt + 1;
  end
  reg  term_hb_d = 1'b0; always @(posedge sys_clk) term_hb_d <= hblank;
  wire term_tick = hblank & ~term_hb_d;
  // Host keyboard events use the same PS/2 decoder and terminal serializer
  // as the MiSTer wrapper. The simulation driver supplies physical key
  // transitions here; guest keyboard memory is only written by microcode.
  reg [10:0] terminal_ps2_key /* verilator public_flat_rw */ = 11'd0;
  integer terminal_hold_key;
  initial if($value$plusargs("terminal-key=%h",terminal_hold_key))begin
    wait(!term_reset); @(negedge sys_clk);
    terminal_ps2_key={1'b1,1'b1,1'b0,terminal_hold_key[7:0]};
    $display("tb_exec: physical terminal key held, PS/2 set-2 code %02h",terminal_hold_key[7:0]);
  end
  wire [15:0] terminal_kw0, terminal_kw1, terminal_kw2, terminal_kw3;
  dorado_keyboard u_keyboard (
      .sys_clk(sys_clk), .reset(term_reset), .ps2_key(terminal_ps2_key),
      .key_word0(terminal_kw0), .key_word1(terminal_kw1),
      .key_word2(terminal_kw2), .key_word3(terminal_kw3));
  wire term_ois, term_ois_n;
  dorado_terminal u_term (
      .sys_clk(sys_clk), .reset(term_reset), .line_tick(term_tick),
      .key_word0(terminal_kw0), .key_word1(terminal_kw1),
      .key_word2(terminal_kw2), .key_word3(terminal_kw3),
      .key_word4(16'hFFFF),
      .mouse_dx(9'sd0), .mouse_dy(9'sd0), .mouse_pending(1'b0),
      .mouse_taken(), .mouse_sent_dx(), .mouse_sent_dy(),
      .OISData(term_ois), .OISData_n(term_ois_n));
  wire term_kbd = term_on & term_ois;
  integer n_term_ticks = 0, n_term_ones = 0, n_ois_hi = 0, n_term_smp = 0;
  always @(posedge sys_clk) begin
    n_term_smp = n_term_smp + 1;
    if (term_tick) begin n_term_ticks = n_term_ticks + 1; if (term_kbd) n_term_ones = n_term_ones + 1; end
    if (m.b_DispY.OISRcvdData) n_ois_hi = n_ois_hi + 1;
  end
  final $display("tb_exec: TERMINAL -- %s: %0d line ticks, %0d one-bits sent on KeyboardData, DispY OISRcvdData high %0d of %0d",
                 term_on ? "on" : "off", n_term_ticks, n_term_ones, n_ois_hi, n_term_smp);
`endif
`ifdef FULL
  // ---- A TRIDENT WITH A PACK ON IT, unit 0 of the disk cable ---------------
  // `dorado_trident` turns and clocks bits; `dorado_pack` decodes the tags off
  // the cable, fetches the addressed track from `+pack=PATH` and serves each
  // short sector as AEmu's format program reads it. With no +pack the drive
  // is not attached and every cable line idles DEASSERTED, exactly as the
  // ties did. SYSPER here is sys_clk per MICROINSTRUCTION, so it is 2x the
  // bench's per-clock SYSPER (HM 2.3: two clocks per microinstruction).
  // 117 pulses per revolution is the drive's; the controller's subsector
  // counter divides them by four (disk.c, from TriconD's own test).
  // T-80 capacity is 20,160 bytes = 10,080 words per track (Trident
  // 76205-902, Table 2-1), independently of the 117 electronic pulses.
  wire drv_ready_n, drv_online_n, drv_term_n, drv_sel_n, drv_sec_n, drv_index_n, drv_adv, drv_bit;
  wire drv_dp, drv_dm, drv_cp, drv_cm;
  wire pk_writing;
  wire [11:0] tag_n; wire cyltag_n, headtag_n, drivetag_n, conttag_n, sel0_n;
  wire [11:0] pk_cyl; wire [5:0] pk_head; wire pk_eoc_n;
  wire [31:0] pk_nct, pk_nht, pk_ndt, pk_nkt, pk_ntr, pk_nse, pk_nbi, pk_nbl;
  reg  pack_on = 1'b0; string pack_path;
  initial pack_on = $value$plusargs("pack=%s", pack_path);
  dorado_trident #(.SYSPER(2 * SYSPER), .SECTORS_PER_REV(117), .WORDS_PER_TRACK(10080)) u_drv (
      .sys_clk(sys_clk), .reset(1'b0), .attached(pack_on), .selected(~sel0_n),
      .TtlReady_n(drv_ready_n), .TtlOnLine_n(drv_online_n), .TtlTerm_n(drv_term_n),
      .Selected_n(drv_sel_n), .SecIndx_n(drv_sec_n), .TtlIndex_n(drv_index_n),
      .data_bit(drv_bit), .data_adv(drv_adv),
      .DataP(drv_dp), .DataM(drv_dm), .ClockP(drv_cp), .ClockM(drv_cm));
  // Pilot sectors carry ten label words; Alto's default remains eight.
`ifdef PILOT_PACK
  dorado_pack #(.LABEL_WORDS(10)) u_pack (
`else
  dorado_pack u_pack (
`endif
      .sys_clk(sys_clk), .attached(pack_on), .tagbus_n(tag_n),
      .cyltag_n(cyltag_n), .headtag_n(headtag_n), .drivetag_n(drivetag_n), .conttag_n(conttag_n),
      .pulse(u_drv.sector), .data_adv(drv_adv), .data_bit(drv_bit), .endofcyl_n(pk_eoc_n),
      .selected(~sel0_n), .write_data_p(m.DataP0), .write_data_m(m.DataM0),
      .write_gate(pk_writing), .n_write_blocks(),
      .cyl(pk_cyl), .head(pk_head), .n_cyltag(pk_nct), .n_headtag(pk_nht),
      .n_drivetag(pk_ndt), .n_conttag(pk_nkt), .n_tracks(pk_ntr), .n_sectors(pk_nse),
      .n_bits(pk_nbi), .n_blocks(pk_nbl));
`endif
  // +pendulum: THE 32 us TICK THAT WAKES THE JUNK TASK. HM 12.1: a pendulum
  // wakes task 2 every 32 us, and the junk task's loop adds RTCDelta to
  // RTClock on each wakeup and carries into VM 430, the Alto's real-time
  // clock. `Pendulum` is an INPUT of the IFU board -- it comes from the
  // BaseBoard's timer chain -- so a bench without a BaseBoard must supply it,
  // and none did: every run's JUNK line has read "Pendulum edges 0", and AEmu's
  // ABoot waits 3 x 39 ms on VM 430 before its disk boot, i.e. for ever.
  // Opt-in, so the gates that predate it do not move.
  //
  // THE LINE IS HALVED BEFORE IT BECOMES A WAKEUP. IFU k08 (MC10231) FF A has
  // D tied to its own Q' and is clocked by Pendulum, so TimeToWake toggles
  // once per Pendulum rising edge, and FF B -- the wakeup itself -- clocks on
  // TimeToWake's rising edge: one junk wakeup per TWO Pendulum periods. A
  // 32 us square wave here therefore woke the junk task every 64 us (measured
  // 2026-09-05: TWReq.02 rose every 34,144 sys_clk = 2 x 17,067), and the
  // world's clock ran at half speed. Junk.mc derives RTCDelta from a 32 us
  // wakeup ("Delta = 32 / 38.09524") and cpu.c's DORADO_JUNK_TICK_CYCLES is
  // 533 x 60 ns = 32 us, so the line the BaseBoard sends is a 16 us wave.
  // `+pendulum_us=N` overrides the period (32 reproduces the runs before
  // 2026-09-05, which is what the T-file/MDM windows were measured on).
  reg pendulum_r = 1'b0; integer pend_cnt = 0, pend_half = 0, pend_us = 16;
  initial if ($test$plusargs("pendulum")) begin
    void'($value$plusargs("pendulum_us=%d", pend_us));
    pend_half = (pend_us * 1000 * SYSPER) / 30 / 2;   // sys_clk is 30 ns / SYSPER
  end
  always @(posedge sys_clk) if (pend_half > 0) begin
    if (pend_cnt >= pend_half - 1) begin pend_cnt <= 0; pendulum_r <= ~pendulum_r; end
    else pend_cnt <= pend_cnt + 1;
  end
  // Declared OUTSIDE the configuration chain: the release below is common to
  // every machine, and only the WORLD-and-up tops have an IOReset port.
  reg ioreset_r = 1'b0;
  initial ioreset_r = $test$plusargs("ioreset");
`ifdef BASEBOARD_MANIFOLD
  // Component fixture: use the real BaseBoard serial decoder/latches for
  // processor-issued I/O resets. Its firmware/CP bus and clocks are outside
  // this direct-entry bench. ECL owns the serial interface after handoff.
  wire bb_ioreset, bb_ttl_ioreset_n;
  BaseBd_m_Rev_m_Am #(.SYSPER(SYSPER)) bb_manifold (
    .sys_clk(sys_clk), .DMuxClk(m.DMuxClk), .DMuxData(m.DMuxData),
    .UseDMD(m.UseDMD), .TTLIOReset_p_(bb_ttl_ioreset_n),
    .TTLIOReset_p___drv(bb_ttl_ioreset_n), .IOReset__drv(bb_ioreset));
  initial begin
    force bb_manifold.EclUp=1'b1;
    force bb_manifold.MCReset_p_=1'b1;
  end
  reg bb_ioreset_prev=0;
  always @(negedge sys_clk) if(bb_ioreset!=bb_ioreset_prev)begin
    $display("tb_exec: BASEBOARD IOReset=%b sample=%0d CIA=%h",bb_ioreset,n_cyc2,cia_now);
    bb_ioreset_prev=bb_ioreset;
  end
`endif
`ifdef FULL   // FULL before SCREEN: the Makefile defines BOTH, and an `ifdef chain
              // takes the first that is defined -- with SCREEN first, every
              // "eleven-board" run built the ten-board screen machine (2026-09-02).
  // ELEVEN BOARDS: the screen machine plus DskEth. Its cable lines are tied
  // idle -- the active-low Trident status lines DEASSERTED (1), the
  // Ethernet receiver quiet (RcvData 0, Collision 0), host address 0 -- so
  // Initial's Ethernet boot can be watched trying to start the controller.
  integer full_etherhost=0;
  bit full_largeformat=0;
  initial begin
    void'($value$plusargs("etherhost=%d",full_etherhost));
    full_largeformat=$test$plusargs("largeformat");
  end
  dorado_full #(.SYSPER(SYSPER)) m (
      .XmtData_p_(eth_xmt),
      .CLK_disk_p_(mclk),
      .ClockM0__in(drv_cm),
      .ClockM1__in(1'b0),
      .ClockM2__in(1'b0),
      .ClockM3__in(1'b0),
      .ClockP0__in(drv_cp),
      .ClockP1__in(1'b0),
      .ClockP2__in(1'b0),
      .ClockP3__in(1'b0),
      .Collision(1'b0),
      .DataM0__in(pk_writing ? 1'b0 : drv_dm),
      .DataM1__in(1'b0),
      .DataM2__in(1'b0),
      .DataM3__in(1'b0),
      .DataP0__in(pk_writing ? 1'b0 : drv_dp),
      .DataP1__in(1'b0),
      .DataP2__in(1'b0),
      .DataP3__in(1'b0),
      .Host_0(full_etherhost[7]),
      .Host_1(full_etherhost[6]),
      .Host_2(full_etherhost[5]),
      .Host_3(full_etherhost[4]),
      .Host_4(full_etherhost[3]),
      .Host_5(full_etherhost[2]),
      .Host_6(full_etherhost[1]),
      .Host_7(full_etherhost[0]),
      .TermIsLF(full_largeformat),
      .RcvData(rcv_r),      // +rcvplay=PATH replays a boot-reply stream here
      .SecIndx0_p_(drv_sec_n),
      .SecIndx1_p_(1'b1),
      .SecIndx2_p_(1'b1),
      .SecIndx3_p_(1'b1),
      .Selected0_p_(drv_sel_n),
      .Selected1_p_(1'b1),
      .Selected2_p_(1'b1),
      .Selected3_p_(1'b1),
      .TtlEndOfCyl_p_(pk_eoc_n),      // HeadOvfl: the drive says head >= 5 is illegal
      .TtlIndex_p_(drv_index_n),
      .TtlOnLine_p_(drv_online_n),
      .TtlReadOnly_p_(1'b1),
      .TtlReady_p_(drv_ready_n),
      .TtlTerm_p_(drv_term_n),
      // the tags the controller drives, read by the drive
      .TagBus_000_p_(tag_n[11]), .TagBus_00_p_(tag_n[10]), .TagBus_0_p_(tag_n[9]),
      .TagBus_1_p_(tag_n[8]), .TagBus_2_p_(tag_n[7]), .TagBus_3_p_(tag_n[6]),
      .TagBus_4_p_(tag_n[5]), .TagBus_5_p_(tag_n[4]), .TagBus_6_p_(tag_n[3]),
      .TagBus_7_p_(tag_n[2]), .TagBus_8_p_(tag_n[1]), .TagBus_9_p_(tag_n[0]),
      .CylinderTag_p_(cyltag_n), .HeadTag_p_(headtag_n), .DriveTag_p_(drivetag_n),
      .ContTag_p_(conttag_n), .Select0_p_(sel0_n),
      // DispY IS IN THIS MACHINE TOO, so it needs the display board's own
      // ports -- its clock and the video the bench samples. These used to
      // live only in the SCREEN branch, which was reached because SCREEN is
      // defined; with FULL first they were silently unconnected and DispY had
      // NO CLOCK (2026-09-03). Verilator ties an unconnected input to 0 and
      // only warns, so the board simply froze.
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
      .KeyboardData(term_kbd),        // the terminal's return channel (+terminal), else 0 as before
`elsif SCREEN
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
      .KeyboardData(term_kbd),        // the terminal's return channel (+terminal), else 0 as before
`else
  dorado_world m (
`endif
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      .CLK_mc_p_(mclk), .CLK_md_p_(mclk), .CLK_mx_p_(mclk),
      .CLK_ms0Even_p_(mclk), .CLK_ms0Odd_p_(mclk), .CLK_ifu_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n),
      .SetRunRfsh(1'b1), .RfshPeriod(rfshper),
      .ChipsAre256_s_16K(chips16k), .ChipsAre64K(chips64k),
      // MODULE PRESENCE IS A BACKPLANE INPUT TOO. The msa relays Mb0 to its
      // M0 output (h26, a pure OR buffer: M0 = Mb0), MemX reads M0..M3 as
      // inputs (b02/b41/c03/c42, all receiving pins), and nothing on any
      // board ORIGINATES the token -- the backplane ties it at the module
      // slot ("awaits PCMSA" on the port). Left at 0, B<-Config' reports NO
      // storage modules and Initial's FINDMODULE scan lands at NOSTORAGE
      // (real 6247), the 758k-microinstruction park. Driven, MemX reads
      // M0=1 M1..3=0: one module, which is what this machine has.
      //
      // OPT-IN (`+mb0`), because the AEmu jump-start gates were calibrated
      // on the no-module machine: with the module present their fault-task
      // choreography changes and exec-tasking/exec-init fail their
      // thresholds. The boot targets pass +mb0; recalibrating the AEmu
      // gates on the with-storage machine is a separate, worthwhile task.
      .Mb0(mb0_tie),
      // IOReset is a BASEBOARD line (BaseBd i24 -> backplane): the firmware's
      // manifold table asserts it in DoIOReset, the first InitManifolds entry,
      // and PrepareProcessor re-asserts it before every load. Without a
      // BaseBoard in this machine the line idled low, so the IFU's junk-wakeup
      // flip-flops (a21 ShutUp, j08/k08) kept their power-up states and task 2
      // was requested from the first cycle with no pendulum tick and no ack.
      // `+ioreset` holds the line asserted through the startup jams and
      // releases it when the machine is started, the way the BaseBoard does.
      .IOReset(ioreset_r
`ifdef BASEBOARD_MANIFOLD
        | bb_ioreset
`endif
      ),
      .Pendulum(pendulum_r)
  );
  reg mb0_tie = 1'b0;
  initial mb0_tie = $test$plusargs("mb0");

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
  // ...AND THE MAP PLANES STORE THE COMPLEMENT. Measured (MAPCENSUS vs the
  // DRAMWIN map-stage readback): with a plane holding 1 the translated read
  // (MC10124, inverting) delivers 0 -- the array stores the complement of
  // the logical bit, the same convention as IM parity and dBlock'. So the
  // ALL-ONES seed reads back as RP = 0 (module 0 -- correct for this
  // one-module machine) and WP = 0 (resident -- which is why no reference
  // ever map-faults on the seeded map). A brief 'fix' that zeroed the RP
  // planes to force module 0 did the opposite and is reverted here.
  // ...AND THE SEED MUST BE APPLIED AFTER THE POWER-UP SETTLE. A spurious
  // RTMapWE'b at cycle 0 (the settle out of X) writes one artifact entry --
  // measured: WPdin=0/Dirtydin=1 at index 0 -- and whether it or the
  // time-0 seed wins differs PER PLANE (initial-block ordering), leaving a
  // mixed entry that reads resident-at-garbage-page and never faults. The
  // 1000-cycle delay puts the seed after the settle, the +hasram pattern.
  integer mi;
  initial if (!$test$plusargs("nomapseed")) begin
    repeat (1000) @(posedge sys_clk);
    for (mi = 0; mi < 16384; mi = mi + 1) begin   // the map is 16K deep (cell_MosRam)
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
    for (si = 0; si < 16384; si = si + 1) begin   // 16K x 1 storage chips (cell_MK4096P_6)
      m.b_msa.u_b05.mem[si] = dpat[0];   // A
      m.b_msaOdd.u_b05.mem[si] = dpat[0];   // the odd storage board, same word
      m.b_msa.u_c05.mem[si] = dpat[1];   // B
      m.b_msaOdd.u_c05.mem[si] = dpat[1];   // the odd storage board, same word
      m.b_msa.u_d05.mem[si] = dpat[2];   // C
      m.b_msaOdd.u_d05.mem[si] = dpat[2];   // the odd storage board, same word
      m.b_msa.u_e05.mem[si] = dpat[3];   // D
      m.b_msaOdd.u_e05.mem[si] = dpat[3];   // the odd storage board, same word
      m.b_msa.u_e06.mem[si] = dpat[4];   // E
      m.b_msaOdd.u_e06.mem[si] = dpat[4];   // the odd storage board, same word
      m.b_msa.u_d06.mem[si] = dpat[5];   // F
      m.b_msaOdd.u_d06.mem[si] = dpat[5];   // the odd storage board, same word
      m.b_msa.u_c06.mem[si] = dpat[6];   // G
      m.b_msaOdd.u_c06.mem[si] = dpat[6];   // the odd storage board, same word
      m.b_msa.u_b06.mem[si] = dpat[7];   // H
      m.b_msaOdd.u_b06.mem[si] = dpat[7];   // the odd storage board, same word
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

  // The PLAIN form. B and T carry OPPOSITE senses of CPReg -- BMux is its
  // complement and `alub` inverts that back -- so T ends up EQUAL to CPReg
  // while IM write data ends up equal to its complement. PARC has both
  // `SetCPReg` and `SetCPReg~` for exactly that reason: `SendViaMIR` sends IM
  // data with the tilde form, `PrepareProcessor` loads T with this one.
  task set_cpreg_plain(input [15:0] v);
    begin
      strobe(3'd2, v[15:8], 1'b0);
      strobe(3'd3, v[7:0],  1'b0);
    end
  endtask

  // A GENERAL MICROINSTRUCTION, the byte layout `doradoboot.masm` states and
  // the same encoder tb_compute checks against all thirteen IRTable entries.
  //   0: RSTK.0, P015, JCN.7, P1631, 0,0,0,0
  //   1: RSTK.1, RSTK.2, RSTK.3, ALUF.0, BLOCK, FF.0, FF.1, FF.2
  //   2: ALUF.1, ALUF.2, ALUF.3, BSEL.0, FF.3, FF.4, FF.5, FF.6
  //   3: BSEL.1, BSEL.2, LC.0, LC.1, FF.7, JCN.0, JCN.1, JCN.2
  //   4: LC.2, ASEL.0, ASEL.1, ASEL.2, JCN.3, JCN.4, JCN.5, JCN.6
  // PARC numbers every field MSB-first; the parity bits are odd over the
  // 17-bit halves (left = RSTK ALUF BSEL LC ASEL, right = BLOCK JCN FF).
  function [39:0] mkmi(input [3:0] rstk, input [3:0] aluf, input [2:0] bsel,
                     input [2:0] lc,   input [2:0] asel, input [7:0] ff,
                     input [7:0] jcn,  input block);
    reg [7:0] b0, b1, b2, b3, b4;
    begin
      b0 = {rstk[3], ~(^{rstk, aluf, bsel, lc, asel}), jcn[0],
            ~(^{block, jcn, ff}), 4'b0000};
      b1 = {rstk[2], rstk[1], rstk[0], aluf[3], block, ff[7], ff[6], ff[5]};
      b2 = {aluf[2], aluf[1], aluf[0], bsel[2], ff[4], ff[3], ff[2], ff[1]};
      b3 = {bsel[1], bsel[0], lc[2],   lc[1],   ff[0], jcn[7], jcn[6], jcn[5]};
      b4 = {lc[0],   asel[2], asel[1], asel[0], jcn[4], jcn[3], jcn[2], jcn[1]};
      mkmi = {b0, b1, b2, b3, b4};
    end
  endfunction

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
      // TWICE. With the clk1-family lag, ReadBB's stale-by-one branch
      // condition can consume a half-updated CPReg even with low-first
      // ordering (measured: ChkSum = {feeder tag byte, STARTUP low byte}
      // regardless of payload). Writing the pair again while the tag is
      // unchanged corrects any half-capture: the tag cannot re-match a
      // wrong phase, and the data settles before the real consume.
      if ($test$plusargs("bbinv")) begin
        strobe(3'd3, ~w[7:0],  1'b0);
        strobe(3'd2, ~w[15:8], 1'b0);
        repeat (WT(40)) @(posedge sys_clk);
        strobe(3'd3, ~w[7:0],  1'b0);
        strobe(3'd2, ~w[15:8], 1'b0);
      end else begin
        strobe(3'd3,  w[7:0],  1'b0);
        strobe(3'd2,  w[15:8], 1'b0);
        repeat (WT(40)) @(posedge sys_clk);
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
  bit paritytrace_arm=0;
  integer paritytrace_count=0;
  integer paritysource_count=0;
  bit paritysources_arm=0;
  reg [5:0] paritysource_mask=0;
`ifdef WORLD
  reg [63:0] st_history[0:31];
  integer st_history_pos=0,st_trace_count=0,st_sample=0;
  bit st_seen_reset=0,st_previous=0;
  reg [63:0] ram_history[0:31];
  reg [63:0] ram_phase_history[0:31];
  integer ram_history_pos=0,ram_trace_count=0;
  wire [15:0] parity_r_word={m.b_ProcH.R_00,m.b_ProcH.R_01,m.b_ProcH.R_02,m.b_ProcH.R_03,
    m.b_ProcH.R_04,m.b_ProcH.R_05,m.b_ProcH.R_06,m.b_ProcH.R_07,
    m.b_ProcL.R_08,m.b_ProcL.R_09,m.b_ProcL.R_10,m.b_ProcL.R_11,
    m.b_ProcL.R_12,m.b_ProcL.R_13,m.b_ProcL.R_14,m.b_ProcL.R_15};
  wire [15:0] parity_t_word={m.b_ProcH.T_00,m.b_ProcH.T_01,m.b_ProcH.T_02,m.b_ProcH.T_03,
    m.b_ProcH.T_04,m.b_ProcH.T_05,m.b_ProcH.T_06,m.b_ProcH.T_07,
    m.b_ProcL.T_08,m.b_ProcL.T_09,m.b_ProcL.T_10,m.b_ProcL.T_11,
    m.b_ProcL.T_12,m.b_ProcL.T_13,m.b_ProcL.T_14,m.b_ProcL.T_15};
  always @(negedge sys_clk) if($test$plusargs("ramtrace") && paritysources_arm)begin
    ram_phase_history[ram_history_pos]={st_sample[23:0],cia_now,
      m.b_ProcH.Clock1_p_Da,m.b_ProcH.Clock1_p_Da__lead,m.b_ProcH.u_g16.ck_en,
      m.b_ProcH.ProcH27_sil_pl_1,m.b_ProcL.ProcL27_sil_pl_1,
      m.b_ProcH.ProcH27_sil_pl_7,m.b_ProcL.ProcL27_sil_pl_6,
      m.b_ProcH.PreSHCP_p_B,m.b_ProcH.TbBypass,m.b_ProcL.RbBypassDly,
      m.b_ProcH.Clock0_p_Dd,m.b_ProcH.u_k21.ck_en,
      m.b_ProcH.u_i11.a,m.b_ProcL.u_j08.a,4'b0};
    ram_history[ram_history_pos]={st_sample[23:0],parity_r_word,m.b_ProcH.Rparity,m.b_ProcL.Rparity,
      parity_t_word,m.b_ProcH.Tparity,m.b_ProcL.Tparity,
      m.b_ProcH.R_Perr_p_,m.b_ProcL.R_Perr_p_,m.b_ProcH.T_Perr_p_,m.b_ProcL.T_Perr_p_};
    if(!m.b_ContA.Error_p_ && m.RamPE && ram_trace_count==0)begin
      $display("tb_exec: RAM-FIRST sample=%0d CIA=%h RM-H/L=%b%b T-H/L=%b%b (sample24,R16,Rpar2,T16,Tpar2,good4)",
        st_sample,cia_now,m.b_ProcH.RmPerr,m.b_ProcL.RmPerr,m.b_ProcH.TmPerr,m.b_ProcL.TmPerr);
      for(integer j=1;j<=32;j++)$display("tb_exec: RAM-HISTORY %016h",ram_history[(ram_history_pos+j)%32]);
      for(integer j=1;j<=32;j++)$display("tb_exec: RAM-PHASE %016h",ram_phase_history[(ram_history_pos+j)%32]);
      ram_trace_count++;
    end
    ram_history_pos=(ram_history_pos+1)%32;
  end
  integer mcr_trace_count=0;
  always @(negedge sys_clk)
    if($test$plusargs("mcrtrace") && paritysources_arm &&
       !m.b_MemX.Mcr_u__p_ && mcr_trace_count<128)begin
      $display("tb_exec: MCRCAP sample=%0d CIA=%h PE'=%b clk=%b en=%b D[13:15]=%b%b%b setup=%b NW=%b RMar15=%b B15=%b MapLd'=%b preFH'=%b hold=%b holdD=%b Tlow=%b%b%b%b",
        st_sample,cia_now,m.b_MemX.Mcr_u__p_,m.b_MemX.Clk1_p_Ab,m.b_MemX.u_f04.ck_en,
        m.b_MemX.Mapbuf_13,m.b_MemX.Mapbuf_14,m.b_MemX.Mapbuf_15,m.b_MemX.u_f04.setup_pre,
        m.b_MemX.NoWakeups,m.b_MemC.RMar_15,m.b_MemX.BMux_15,m.b_MemX.MapbufLd_p_c,
        m.b_MemX.preFH_p_A,m.b_MemX.HoldMapBuf,m.b_MemX.MemX20_sil_pl_3,
        m.b_ProcL.T_12,m.b_ProcL.T_13,m.b_ProcL.T_14,m.b_ProcL.T_15);
      mcr_trace_count++;
    end
  wire [17:0] st_sout={~m.b_MemD.EcSout_00_p_,~m.b_MemD.EcSout_01_p_,
    ~m.b_MemD.EcSout_02_p_,~m.b_MemD.EcSout_03_p_,~m.b_MemD.EcSout_04_p_,
    ~m.b_MemD.EcSout_05_p_,~m.b_MemD.EcSout_06_p_,~m.b_MemD.EcSout_07_p_,
    ~m.b_MemD.EcSout_08_p_,~m.b_MemD.EcSout_09_p_,~m.b_MemD.EcSout_10_p_,
    ~m.b_MemD.EcSout_11_p_,~m.b_MemD.EcSout_12_p_,~m.b_MemD.EcSout_13_p_,
    ~m.b_MemD.EcSout_14_p_,~m.b_MemD.EcSout_15_p_,m.b_MemD.EcSout_16,m.b_MemD.EcSout_17};
  always @(negedge sys_clk) if($test$plusargs("sttrace") && paritysources_arm) begin
    st_sample++;
    st_history[st_history_pos]={st_sample[23:0],st_sout,
      m.b_MemX.NoWakeups,m.b_MemX.STPerr,m.b_MemX.EnableSTPerr,m.b_MemX.dSTPerr,
      m.b_MemD.HighAD,m.b_MemD.GenP0A0,m.b_MemD.GenP0B0,m.b_MemD.GenP0A1,m.b_MemD.GenP0B1,
      m.b_MemD.GenPh1,m.b_MemD.GenPh2,m.b_MemD.GenPh3,m.b_MemD.clk2_p_Aa,
      m.b_MemD.MemD04_sil_pl_11,m.b_MemD.MemD04_sil_pl_10,
      m.b_MemX.STPerrNow_p_,6'd0};
    if(m.b_MemX.NoWakeups)st_seen_reset=1;
    if(st_seen_reset && !m.b_MemX.NoWakeups && m.b_MemX.STPerr && !st_previous && st_trace_count<3)begin
      $display("tb_exec: ST-FIRST %0d sample=%0d CIA=%h (24-bit sample,18-bit data,16 control bits,6 pad)",st_trace_count,st_sample,cia_now);
      for(integer j=1;j<=32;j++)$display("tb_exec: ST-HISTORY %016h",st_history[(st_history_pos+j)%32]);
      st_trace_count++;
      if($test$plusargs("stop-first-st"))$fatal(1,"First real serial transport parity latch after reset");
    end
    st_previous=m.b_MemX.STPerr;st_history_pos=(st_history_pos+1)%32;
  end
`endif
  wire [5:0] parity_enables={m.b_ContB.IMLHPEenable,m.b_ContB.IMRHPEenable,
    m.b_ContB.IOPEenable,m.b_ContB.RamPEenable,m.b_ContB.MemPEenable,m.b_ContB.MDPEenable};
  integer all_enabled_samples=0,all_enabled_errors=0;
  always @(negedge sys_clk) if(paritysources_arm && parity_enables==6'b111111)begin
    all_enabled_samples++;
    if(!m.b_ContA.Error_p_)all_enabled_errors++;
  end
  always @(negedge sys_clk)
    if($test$plusargs("paritysources") && paritysources_arm && m.b_ContA.SetRun && paritysource_count<128 &&
       (paritysource_mask!=parity_enables || !m.b_ContA.Error_p_)) begin
      $display("tb_exec: PARITYSOURCES CIA=%h mask[LH,RH,IO,RM,MEM,MD]=%b Error'=%b IO=%b RM=%b MEM=%b MD=%b LH'=%b RH'=%b Stop=%b dStop=%b",
        {m.b_ContA.CIA_04,m.b_ContA.CIA_05,m.b_ContA.CIA_06,m.b_ContA.CIA_07,
         m.b_ContA.CIA_08,m.b_ContA.CIA_09,m.b_ContA.CIA_10,m.b_ContA.CIA_11,
         m.b_ContA.CIA_12,m.b_ContA.CIA_13,m.b_ContA.CIA_14,m.b_ContA.CIA_15},
        parity_enables,m.b_ContA.Error_p_,m.IOPE,m.RamPE,m.MemPE,m.MdPE,
        m.b_ContB.IMLHPE_p_,m.b_ContB.IMRHPE_p_,m.b_ContA.Stop,m.b_ContA.dStop);
`ifdef WORLD
      if(!m.b_ContA.Error_p_)
        $display("tb_exec: MEMORYPARITY STPerr=%b STPerrNow'=%b HitPerr=%b NoWakeups=%b dSTPerr=%b EnableSTPerr=%b dHitPerr=%b Hit'=%b PairHasA'=%b TagParity=%b",
          m.b_MemX.STPerr,m.b_MemX.STPerrNow_p_,m.b_MemX.HitPerr,
          m.b_MemX.NoWakeups,m.b_MemX.dSTPerr,m.b_MemX.EnableSTPerr,
          m.b_MemC.dHitPerr__drv,m.b_MemC.Hit_p_b,m.b_MemC.PairHasA_p_a,m.b_MemC.MemC05_sil_pl_9);
`endif
      paritysource_mask=parity_enables;paritysource_count++;
    end
  always @(negedge sys_clk)
    if(paritytrace_arm && m.b_ContA.SetRun && paritytrace_count<192) begin
      $display("tb_exec: PARITYSTART %0d run'=%b stop=%b dStop=%b rStop=%b ss'=%b Error'=%b LHPE'=%b RHPE'=%b MIRstop=%b preStart'=%b single=%b ck2=%b ck2pre=%b ck0=%b MIRck=%b ck1=%b dpre=%b en=%b FF=%02h CIA=%03h TNIA=%03h",
        paritytrace_count,m.b_ContA.Run_p_,m.b_ContA.Stop,m.b_ContA.dStop,m.b_ContA.rStop,
        m.b_ContA.SetSS_p_,m.b_ContA.Error_p_,m.b_ContB.IMLHPE_p_,m.b_ContB.IMRHPE_p_,m.StopMIRClk,
        m.b_ContA.bpreStartC_p_b,m.b_ContA.ContA31_sil_pl_5,m.b_ContA.clk2_p_Bc,
        m.b_ContA.preclk2_p_Bb,m.b_ContA.clk0_p_Ca,m.b_ContA.h_x2a_clk0_p_Ca,m.b_ContA.clk1_p_Ca,
        m.b_ContA.u_j04.d_pre_a,m.b_ContA.u_j04.cka_en,ff_now,
        {m.b_ContA.CIA_04,m.b_ContA.CIA_05,m.b_ContA.CIA_06,m.b_ContA.CIA_07,
         m.b_ContA.CIA_08,m.b_ContA.CIA_09,m.b_ContA.CIA_10,m.b_ContA.CIA_11,
         m.b_ContA.CIA_12,m.b_ContA.CIA_13,m.b_ContA.CIA_14,m.b_ContA.CIA_15},tnia_now);
      paritytrace_count++;
    end
  task parc_run(input [7:0] b0, input [7:0] b1, input [7:0] b2,
                input [7:0] b3, input [7:0] b4);
    begin
      paritytrace_arm=$test$plusargs("paritytrace");
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

  integer hcount, hunklimit;
  integer n0a, n1a, nff;
  longint j2;      // 64-bit: a full boot is >2^31 sys_clk (2026-09-04)
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

  // ---- +wtpcfix: STAND-IN FOR THE 6-CYCLE WRITE-TPC RESIDENCY -------------
  //
  // A Write-TPC (c46 in the task-init loop) should advance to CIA+1; the RTL
  // takes TNIA<-Link (i24's Return'c path) = the TPC value being written =
  // c61, and executes it as a spurious instruction, because the 6-cycle
  // phase-ring residency that would update Link to CIA+1 BEFORE the
  // next-address latches is collapsed to one cycle in our SYSPER model.
  // taskrun-test proves the WTPC WRITE mechanism is sound, so redirecting
  // ONLY the spurious next-fetch to CIA+1 -- letting the write proceed --
  // is the functional stand-in cpu.c implements (next = real_PC + 1). This
  // forces the combinational TNIA to CIA+1 while a WTPC is in the MIR;
  // dRA (ContB) then latches CIA+1 instead of Link. CIA = the WTPC's own
  // address = the last distinct TNIA before the spurious Link target.
  // CIA (ContA's Current Instruction Address register) IS the WTPC's own
  // address while it executes -- so CIA+1 is the correct successor, no
  // history tracking (which glitched on sys_clk transients in the first cut).
  wire [11:0] cia_now = {m.b_ContA.CIA_04, m.b_ContA.CIA_05, m.b_ContA.CIA_06,
                         m.b_ContA.CIA_07, m.b_ContA.CIA_08, m.b_ContA.CIA_09,
                         m.b_ContA.CIA_10, m.b_ContA.CIA_11, m.b_ContA.CIA_12,
                         m.b_ContA.CIA_13, m.b_ContA.CIA_14, m.b_ContA.CIA_15};
  integer wtpcfix; initial wtpcfix = $test$plusargs("wtpcfix");
  reg [11:0] wtpc_tgt = 12'h0;
  reg wtpc_active = 1'b0; integer n_wtpcfix = 0;
  always @(posedge sys_clk) if (wtpcfix) begin
    if (!m.b_ContA.WTPC_p_ && !wtpc_active) begin
      wtpc_tgt = cia_now + 12'd1;
      wtpc_active <= 1'b1;
      n_wtpcfix = n_wtpcfix + 1;
      if (n_wtpcfix <= 8)
        $display("tb_exec: +wtpcfix -- WTPC at CIA=%h, redirect Link->%h @%0d",
                 cia_now, wtpc_tgt, n_cyc2);
      force m.TNIA_04 = wtpc_tgt[11]; force m.TNIA_05 = wtpc_tgt[10];
      force m.TNIA_06 = wtpc_tgt[9];  force m.TNIA_07 = wtpc_tgt[8];
      force m.TNIA_08 = wtpc_tgt[7];  force m.TNIA_09 = wtpc_tgt[6];
      force m.TNIA_10 = wtpc_tgt[5];  force m.TNIA_11 = wtpc_tgt[4];
      force m.TNIA_12 = wtpc_tgt[3];  force m.TNIA_13 = wtpc_tgt[2];
      force m.TNIA_14 = wtpc_tgt[1];  force m.TNIA_15 = wtpc_tgt[0];
    end else if (m.b_ContA.WTPC_p_ && wtpc_active) begin
      wtpc_active <= 1'b0;
      release m.TNIA_04; release m.TNIA_05; release m.TNIA_06; release m.TNIA_07;
      release m.TNIA_08; release m.TNIA_09; release m.TNIA_10; release m.TNIA_11;
      release m.TNIA_12; release m.TNIA_13; release m.TNIA_14; release m.TNIA_15;
    end
  end

  // ---- +enterat=HEX / +enterworld=HEX: HAND INITIAL'S BOOT OVER TO A WORLD --
  //
  // Initial's job is to set the machine up and then fetch a world over a
  // transport. Everything before the transport is what a world NEEDS and
  // cannot do for itself: `BlessBaseBoard` ends with `T_ mcr.noWake,
  // Call[SetMCR]` -- "now allow holds" -- and only then branches to
  // BootEmulator. A world started cold instead wedges on RefHold with its
  // first `RM/STK<-Md`, which is what happens if you jump straight to
  // AEmu's StartEmulator (measured: RefHold on 5,905,947 of 6,000,000).
  //
  // So this hands over AT BootEmulator (0xc92): the machine is fully
  // initialised, and the 100 ms keyboard wait that opens BootEmulator --
  // 6000B ticks of 32 us, about 52 M sys_clk -- is skipped with it.
  // The world must already be in IM; Initial (0xc00-0xfbf), Bootstrap
  // (0xfc0-0xfff) and AEmu (0x000-0x879) are disjoint, so one preload
  // carries all three.
  //
  // This REPLACES THE TRANSPORT and nothing else. It is not a boot: a real
  // machine gets those same microinstructions off the Ethernet or the disk.
  integer enterw; reg [11:0] ew_at, ew_tgt;
  reg ew_active = 1'b0, ew_done = 1'b0;
  initial begin
    ew_at = 12'hc92; ew_tgt = 12'h25b;
    enterw = $value$plusargs("enterat=%h", ew_at);
    if ($value$plusargs("enterworld=%h", ew_tgt)) enterw = 1;
    if (enterw)
      $display("tb_exec: +enterworld -- at IM[%h] the boot transport is replaced by a branch to IM[%h]",
               ew_at, ew_tgt);
  end
  always @(posedge sys_clk) if (enterw && !ew_done) begin
    if (!ew_active && cia_now == ew_at) begin
      ew_active <= 1'b1;
`ifdef WORLD
      // InitialMain.mc IFUMInitL clears all four opcode tables. The real
      // world transport reloads them AFTER Initial, so an early IM preload
      // alone cannot stand in for that transport.
      restore_world_tables;
`endif
      $display("tb_exec: +enterworld -- Initial reached %h at cycle %0d; entering the world at %h",
               ew_at, n_cyc2, ew_tgt);
      force m.TNIA_04 = ew_tgt[11]; force m.TNIA_05 = ew_tgt[10];
      force m.TNIA_06 = ew_tgt[9];  force m.TNIA_07 = ew_tgt[8];
      force m.TNIA_08 = ew_tgt[7];  force m.TNIA_09 = ew_tgt[6];
      force m.TNIA_10 = ew_tgt[5];  force m.TNIA_11 = ew_tgt[4];
      force m.TNIA_12 = ew_tgt[3];  force m.TNIA_13 = ew_tgt[2];
      force m.TNIA_14 = ew_tgt[1];  force m.TNIA_15 = ew_tgt[0];
    end else if (ew_active && cia_now != ew_at) begin
      ew_active <= 1'b0; ew_done <= 1'b1;
      release m.TNIA_04; release m.TNIA_05; release m.TNIA_06; release m.TNIA_07;
      release m.TNIA_08; release m.TNIA_09; release m.TNIA_10; release m.TNIA_11;
      release m.TNIA_12; release m.TNIA_13; release m.TNIA_14; release m.TNIA_15;
    end
  end

  // ---- +ringpc=HEX / +ringat=N, +ringlen=N: THE PHASE RING, per sys_clk ----
  //
  // HM Figure 7: a normal instruction is Phase0 then StartCycle, one CLOCK
  // (30 ns) each; an IM/TPC read or write inserts Phase1..Phase4 between
  // them (six clocks). The ring is ContA l07/k05/j01/k04 on the clk2' family,
  // seeded by j05 (dStartCycle = Phase0&~RWTPCorRWIM | Phase4). This window
  // prints every net of it, plus the next-address and Link/TPC path, on
  // every sys_clk, from the first time TNIA reads +ringpc (or from cycle
  // +ringat) for +ringlen sys_clk. Raw samples, no interpretation.
  reg [11:0] ringpc = 12'hfff; integer ringat = -1, ringlen = 0, ring_left = 0;
  initial begin
    void'($value$plusargs("ringpc=%h", ringpc));
    void'($value$plusargs("ringat=%d", ringat));
    if (!$value$plusargs("ringlen=%d", ringlen)) ringlen = 0;
  end
  wire [11:0] ring_cia  = {m.b_ContA.CIA_04, m.b_ContA.CIA_05, m.b_ContA.CIA_06, m.b_ContA.CIA_07,
                           m.b_ContA.CIA_08, m.b_ContA.CIA_09, m.b_ContA.CIA_10, m.b_ContA.CIA_11,
                           m.b_ContA.CIA_12, m.b_ContA.CIA_13, m.b_ContA.CIA_14, m.b_ContA.CIA_15};
  wire [11:0] ring_ciainc = {m.b_ContA.CIAInc_04, m.b_ContA.CIAInc_05, m.b_ContA.CIAInc_06, m.b_ContA.CIAInc_07,
                           m.b_ContA.CIAInc_08, m.b_ContA.CIAInc_09, m.b_ContA.CIAInc_10, m.b_ContA.CIAInc_11,
                           m.b_ContA.CIAInc_12, m.b_ContA.CIAInc_13, m.b_ContA.CIAInc_14, m.b_ContA.CIAInc_15};
  wire [11:0] ring_link = ~{m.b_ContA.Link_04_p_, m.b_ContA.Link_05_p_, m.b_ContA.Link_06_p_, m.b_ContA.Link_07_p_,
                           m.b_ContA.Link_08_p_, m.b_ContA.Link_09_p_, m.b_ContA.Link_10_p_, m.b_ContA.Link_11_p_,
                           m.b_ContA.Link_12_p_, m.b_ContA.Link_13_p_, m.b_ContA.Link_14_p_, m.b_ContA.Link_15_p_};
  wire [11:0] ring_tpci = {m.b_ContA.TPCI_04, m.b_ContA.TPCI_05, m.b_ContA.TPCI_06, m.b_ContA.TPCI_07,
                           m.b_ContA.TPCI_08, m.b_ContA.TPCI_09, m.b_ContA.TPCI_10, m.b_ContA.TPCI_11,
                           m.b_ContA.TPCI_12, m.b_ContA.TPCI_13, m.b_ContA.TPCI_14, m.b_ContA.TPCI_15};
  wire [7:0] ring_jcn = {m.b_ContA.JCN_0, m.b_ContA.JCN_1, m.b_ContA.JCN_2, m.b_ContA.JCN_3,
                         m.b_ContA.JCN_4, m.b_ContA.JCN_5, m.b_ContA.JCN_6, m.b_ContA.JCN_7};
  wire [4:0] ring_ph = {m.b_ContA.Phase0, m.b_ContA.Phase1, m.b_ContA.Phase2, m.b_ContA.Phase3, m.b_ContA.Phase4};
  wire [3:0] ring_ctd = {m.b_ContA.CTD_0, m.b_ContA.CTD_1, m.b_ContA.CTD_2, m.b_ContA.CTD_3};
  always @(posedge sys_clk) begin
    if (ringlen > 0 && ring_left == 0 && ringpc != 12'hfff && tnia_now == ringpc) begin
      ring_left = ringlen; ringpc = 12'hfff;
      $display("tb_exec: RING window opens @%0d (TNIA=%h)", n_cyc2, tnia_now);
    end
    if (ringlen > 0 && ring_left == 0 && ringat >= 0 && n_cyc2 == ringat) begin
      ring_left = ringlen; ringat = -1;
      $display("tb_exec: RING window opens @%0d (ringat)", n_cyc2);
    end
    if (ring_left > 0) begin
      ring_left = ring_left - 1;
      $display("tb_exec: RING @%0d clk=%b c2=%b mirclk=%b Ph01234=%b SC=%b pSC=%b dSC=%b RW=%b WTPC'=%b Rep=%b sPh0=%b StopT1=%b CLKen'=%b TNIA=%h CIA=%h CIAinc=%h Link=%h JCN=%h TPCI=%h CTD=%h WrTPC'=%b LdCTD'=%b LdLink'=%b TLinkEn=%b GetTL'=%b bpSC'=%b Ret'c=%b",
               n_cyc2, mclk, m.b_ContA.clk2_p_Be, m.b_ContA.h_x2a_clk0_p_Ca,
               ring_ph, m.b_ContA.StartCycle, m.b_ContA.preStartCyclea, m.b_ContA.dStartCycle,
               m.b_ContA.RWTPCorRWIM, m.b_ContA.WTPC_p_, m.b_ContA.RepeatCur, m.b_ContA.sPhase0,
               m.b_ContA.StopAtT1, m.b_ContA.bCLKEnable_p_e,
               tnia_now, ring_cia, ring_ciainc, ring_link, ring_jcn, ring_tpci, ring_ctd,
               m.b_ContA.WriteTPC_p_a, m.b_ContA.LoadCTD_p_, m.b_ContA.LoadLink_p_,
               m.b_ContA.TLinkEn, m.b_ContA.GetTLink_p_, m.b_ContA.bpreStartC_p_b, m.b_ContA.Return_p_c);
    end
  end

  // ---- BRC: the BRANCH-CONDITION path on ProcH, in the same window ----------
  //
  // HM p.30: ALU=0 / ALU<0 are "the results of the last ALU operation
  // executed by the current task", saved in a RAM at t3. On ProcH that is
  // e13 (F10145A, the per-task saved conditions), e14 (MC10158 mux) and f17
  // (MC10173 latch on Clock1'Cb) whose Q1 IS ResLtZero'. The ALU operands
  // are the R/T/Ain latches (MC10173 on PreSHCP') through the b03/b04
  // MC10174 muxes. Raw samples; +tseed=N seeds the task-0 T-file slot
  // (nibble-reversed, the same slot +fastwait clips) at the first TNIA=c45.
  wire [15:0] brc_alua = {m.b_ProcH.alua_00, m.b_ProcH.alua_01, m.b_ProcH.alua_02, m.b_ProcH.alua_03,
                          m.b_ProcH.alua_04, m.b_ProcH.alua_05, m.b_ProcH.alua_06, m.b_ProcH.alua_07,
                          m.b_ProcL.alua_08, m.b_ProcL.alua_09, m.b_ProcL.alua_10, m.b_ProcL.alua_11,
                          m.b_ProcL.alua_12, m.b_ProcL.alua_13, m.b_ProcL.alua_14, m.b_ProcL.alua_15};
  wire [15:0] brc_alub = {m.b_ProcH.alub_00, m.b_ProcH.alub_01, m.b_ProcH.alub_02, m.b_ProcH.alub_03,
                          m.b_ProcH.alub_04, m.b_ProcH.alub_05, m.b_ProcH.alub_06, m.b_ProcH.alub_07,
                          m.b_ProcL.alub_08, m.b_ProcL.alub_09, m.b_ProcL.alub_10, m.b_ProcL.alub_11,
                          m.b_ProcL.alub_12, m.b_ProcL.alub_13, m.b_ProcL.alub_14, m.b_ProcL.alub_15};
  wire [15:0] brc_alu  = {m.b_ProcH.alu_00, m.b_ProcH.alu_01, m.b_ProcH.alu_02, m.b_ProcH.alu_03,
                          m.b_ProcH.alu_04, m.b_ProcH.alu_05, m.b_ProcH.alu_06, m.b_ProcH.alu_07,
                          m.b_ProcL.alu_08, m.b_ProcL.alu_09, m.b_ProcL.alu_10, m.b_ProcL.alu_11,
                          m.b_ProcL.alu_12, m.b_ProcL.alu_13, m.b_ProcL.alu_14, m.b_ProcL.alu_15};
  wire [7:0] brc_r = {m.b_ProcH.R_00, m.b_ProcH.R_01, m.b_ProcH.R_02, m.b_ProcH.R_03,
                      m.b_ProcH.R_04, m.b_ProcH.R_05, m.b_ProcH.R_06, m.b_ProcH.R_07};
  wire [15:0] brc_t = {m.b_ProcH.T_00, m.b_ProcH.T_01, m.b_ProcH.T_02, m.b_ProcH.T_03,
                       m.b_ProcH.T_04, m.b_ProcH.T_05, m.b_ProcH.T_06, m.b_ProcH.T_07,
                       m.b_ProcL.T_08, m.b_ProcL.T_09, m.b_ProcL.T_10, m.b_ProcL.T_11,
                       m.b_ProcL.T_12, m.b_ProcL.T_13, m.b_ProcL.T_14, m.b_ProcL.T_15};
  wire [15:0] brc_tfile = {m.b_ProcH.u_l03.mem[4'hf], m.b_ProcH.u_l04.mem[4'hf],
                           m.b_ProcL.u_l03.mem[4'hf], m.b_ProcL.u_l04.mem[4'hf]};
  integer tseed = -1; reg tseed_done = 1'b0;
  initial void'($value$plusargs("tseed=%d", tseed));
  always @(posedge sys_clk) begin
    if (tseed >= 0 && !tseed_done && tnia_now == 12'hc45) begin : dotseed
      reg [15:0] t;
      t = tseed[15:0];
      tseed_done = 1'b1;
      m.b_ProcH.u_l03.mem[4'hf] = {t[12], t[13], t[14], t[15]};   // T.00-03
      m.b_ProcH.u_l04.mem[4'hf] = {t[8],  t[9],  t[10], t[11]};   // T.04-07
      m.b_ProcL.u_l03.mem[4'hf] = {t[4],  t[5],  t[6],  t[7]};    // T.08-11
      m.b_ProcL.u_l04.mem[4'hf] = {t[0],  t[1],  t[2],  t[3]};    // T.12-15
      $display("tb_exec: TSEED -- task-0 T-file slot set to %h @%0d", t, n_cyc2);
    end
    if (ring_left > 0)
      $display("tb_exec: BRC @%0d C1'Cb=%b PreSHCP'B=%b SHCP'C=%b Ph0=%b SC=%b ResLt0'=%b ResEq0'=%b alu00=%b aluEq0'=%b SB=%b ramD1=%b liveB1=%b BCWr'=%b BCWrEn'=%b LastEqCurr'=%b HoldDly=%b Amux=%b%b Bmux=%b%b alua=%h alub=%h alu=%h R=%h T=%h Tfile=%h TNIA=%h JCN=%h",
               n_cyc2, m.b_ProcH.Clock1_p_Cb, m.b_ProcH.PreSHCP_p_B, m.b_ProcH.SHCP_p_C,
               m.b_ContA.Phase0, m.b_ContA.StartCycle,
               m.b_ProcH.ResLtZero_p_, m.b_ProcH.ResEqZero_p_, m.b_ProcH.alu_00, m.b_ProcH.aluOut_eq_0_p_,
               m.b_ProcH.ProcH11_sil_pl_9, m.b_ProcH.ProcH11_sil_pl_7, m.b_ProcH.ProcH11_sil_pl_4,
               m.b_ProcH.BCWrite_p_, m.b_ProcH.BCWriteEn_p_, m.b_ProcH.Last_eq_Curr_p_, m.b_ProcH.HoldDly,
               m.b_ProcH.Amux1, m.b_ProcH.Amux0, m.b_ProcH.Bmux1, m.b_ProcH.Bmux0,
               brc_alua, brc_alub, brc_alu, brc_r, brc_t, brc_tfile, tnia_now, ring_jcn);
  end

  // ---- SWP: the TASK-SWITCH fetch, in the same window ------------------------
  // HM 4.2: BNT is clocked at t2, CTASK <- BNT at t4, and on a switch "the
  // program counter for the highest priority competing task (BNPC) addresses
  // IM". BNPC = ContA i20 (TPCO or the TPCI bypass); ContB's dRA (e20/f20,
  // MC1662) picks TNIA or BNPC under SW; the IM outputs land in ContB's own
  // MIR registers (RSTK/ALUF/... on clk0'B/C/D) and, over the backplane
  // (dJCN.x), in ContA's JCN copies on the RepeatCur-gated clk0.
  wire [11:0] swp_tpco = {m.b_ContA.TPCO_04, m.b_ContA.TPCO_05, m.b_ContA.TPCO_06, m.b_ContA.TPCO_07,
                          m.b_ContA.TPCO_08, m.b_ContA.TPCO_09, m.b_ContA.TPCO_10, m.b_ContA.TPCO_11,
                          m.b_ContA.TPCO_12, m.b_ContA.TPCO_13, m.b_ContA.TPCO_14, m.b_ContA.TPCO_15};
  wire [11:0] swp_bnpc = {m.b_ContA.BNPC_04, m.b_ContA.BNPC_05, m.b_ContA.BNPC_06, m.b_ContA.BNPC_07,
                          m.b_ContA.BNPC_08, m.b_ContA.BNPC_09, m.b_ContA.BNPC_10, m.b_ContA.BNPC_11,
                          m.b_ContA.BNPC_12, m.b_ContA.BNPC_13, m.b_ContA.BNPC_14, m.b_ContA.BNPC_15};
  wire [11:0] swp_dra  = ~{m.b_ContB.dRA_00_p_, m.b_ContB.dRA_01_p_, m.b_ContB.dRA_02_p_, m.b_ContB.dRA_03_p_,
                           m.b_ContB.dRA_04_p_, m.b_ContB.dRA_05_p_, m.b_ContB.dRA_06_p_, m.b_ContB.dRA_07_p_,
                           m.b_ContB.dRA_08_p_, m.b_ContB.dRA_09_p_, m.b_ContB.dRA_10_p_, m.b_ContB.dRA_11_p_};
  wire [7:0] swp_djcn = {m.b_ContA.dJCN_0, m.b_ContA.dJCN_1, m.b_ContA.dJCN_2, m.b_ContA.dJCN_3,
                         m.b_ContA.dJCN_4, m.b_ContA.dJCN_5, m.b_ContA.dJCN_6, m.b_ContA.dJCN_7};
  wire [3:0] swp_rstk = {m.RSTK_0, m.RSTK_1, m.RSTK_2, m.RSTK_3};
  wire [3:0] swp_aluf = {m.ALUF_0, m.ALUF_1, m.ALUF_2, m.ALUF_3};
  wire [3:0] swp_bnt  = {m.b_ContA.BNT_0, m.b_ContA.BNT_1, m.b_ContA.BNT_2, m.b_ContA.BNT_3};
  always @(posedge sys_clk)
    if (ring_left > 0)
      $display("tb_exec: SWP @%0d clk=%b c0Da=%b mirclk=%b SC=%b Ph0=%b dSw=%b Swa=%b Sw'a=%b SW=%b SWb=%b StopT=%b CT=%h BNT=%h TPCO=%h BNPC=%h TNIA=%h dRA=%h CIA=%h dJCN=%h JCN=%h RSTK=%h ALUF=%h Rep=%b",
               n_cyc2, mclk, m.b_ContA.clk0_p_Da, m.b_ContA.h_x2a_clk0_p_Ca, m.b_ContA.StartCycle, m.b_ContA.Phase0,
               m.b_ContA.dSwitch, m.b_ContA.Switcha, m.b_ContA.Switch_p_a, m.b_ContB.SW, m.b_ContB.SWb, m.b_ContA.StopTasks,
               pct_ctask, swp_bnt, swp_tpco, swp_bnpc, tnia_now, swp_dra, ring_cia, swp_djcn, ring_jcn, swp_rstk, swp_aluf,
               m.b_ContA.RepeatCur);

  // ---- PCT: one line per MIR load (CIA change) in [+pcfrom, +pcfrom+pclen) --
  // The compact form of the two windows above: what executed, with T, R,
  // Link, the saved branch conditions and the task, so a loop's arithmetic
  // can be followed iteration by iteration against the C emulator's PCDIS.
  integer pcfrom = -1, pclen = 0; reg [11:0] pct_cia_d = 12'hfff;
  // `+pctask=N` prints the PCT line for EVERY MIR load executed by task N,
  // window or not -- a task that runs 81 instructions in a 230 M-cycle run
  // (the junk task, 2026-09-05) is cheap to log in full and its arithmetic
  // is what needs following.
  integer pctask = -1;
  initial begin
    void'($value$plusargs("pcfrom=%d", pcfrom));
    void'($value$plusargs("pclen=%d", pclen));
    void'($value$plusargs("pctask=%d", pctask));
  end
  wire [3:0] pct_ctask = {m.b_ContA.CTask_0, m.b_ContA.CTask_1, m.b_ContA.CTask_2, m.b_ContA.CTask_3};

`ifdef WORLD   // the IFU is only in the WORLD/SCREEN/FULL machines
  // ---- JUNK: the junk-task wakeup on the IFU, counted over the run ----------
  // HM 12.1: a 32 us pendulum wakes task 2; the wakeup is a LEVEL dismissed by
  // AckJunkTW<-B (B[15]=1 enables, 0 disables) or any IFUTest<-B; IOReset
  // leaves it disabled (IFU a21 R = IOReset -> ShutUp, which holds j08 reset).
  integer jk_tw = 0, jk_shut = 0, jk_ior = 0, jk_ack = 0, jk_ttw_edges = 0, jk_first_tw = -1, jk_pend_edges = 0;
  reg jk_ttw_d = 1'b0, jk_pend_d = 1'b0;
  always @(posedge sys_clk) begin
    if (m.TWReq_02) begin jk_tw = jk_tw + 1; if (jk_first_tw < 0) jk_first_tw = n_cyc2; end
    if (m.b_IFU.ShutUp) jk_shut = jk_shut + 1;
    if (m.b_IFU.IOReset) jk_ior = jk_ior + 1;
    if (m.b_IFU.AckJunkTW) jk_ack = jk_ack + 1;
    if (m.b_IFU.TimeToWake && !jk_ttw_d) jk_ttw_edges = jk_ttw_edges + 1;
    jk_ttw_d <= m.b_IFU.TimeToWake;
    if (m.b_IFU.Pendulum && !jk_pend_d) jk_pend_edges = jk_pend_edges + 1;
    jk_pend_d <= m.b_IFU.Pendulum;
  end
  final $display("tb_exec: JUNK -- TWReq.02 high %0d (first @%0d), ShutUp high %0d, IOReset high %0d, AckJunkTW high %0d, TimeToWake edges %0d, Pendulum edges %0d, of %0d",
                 jk_tw, jk_first_tw, jk_shut, jk_ior, jk_ack, jk_ttw_edges, jk_pend_edges, n_cyc2);
  // `+junklog`: every event in the junk-wakeup chain, with the task and
  // address that caused it. ShutUp is IFU a21's Q' (D = B[15] at AckJunkTW'),
  // so an ack with B[15]=0 shuts the pendulum off; the 230 M-cycle disk-boot
  // run saw exactly seven acks and then ShutUp high for the rest of the run.
  reg jl_on = 1'b0; initial jl_on = $test$plusargs("junklog");
  reg jl_shut_d = 1'b1, jl_ack_d = 1'b0, jl_tw_d = 1'b0;
  always @(posedge sys_clk) if (jl_on) begin
    if (m.b_IFU.ShutUp !== jl_shut_d)
      $display("tb_exec: JUNKLOG @%0d ShutUp %b->%b task=%h CIA=%h B15=%b AckJunkTW=%b IOReset=%b",
               n_cyc2, jl_shut_d, m.b_IFU.ShutUp, pct_ctask, ring_cia, m.b_IFU.RcvdBMux_15, m.b_IFU.AckJunkTW, m.b_IFU.IOReset);
    if (m.b_IFU.AckJunkTW && !jl_ack_d)
      $display("tb_exec: JUNKLOG @%0d AckJunkTW task=%h CIA=%h B15=%b ShutUp=%b", n_cyc2, pct_ctask, ring_cia, m.b_IFU.RcvdBMux_15, m.b_IFU.ShutUp);
    if (m.TWReq_02 && !jl_tw_d)
      $display("tb_exec: JUNKLOG @%0d TWReq.02 rises task=%h CIA=%h", n_cyc2, pct_ctask, ring_cia);
    jl_shut_d <= m.b_IFU.ShutUp; jl_ack_d <= m.b_IFU.AckJunkTW; jl_tw_d <= m.TWReq_02;
  end
`endif

`ifdef SCREEN
  // ---- DSW: the DispY command strobe per sys_clk, in the RING window --------
  // h15 FF a (DoradoHasHRam') is clocked by clk1'Dc OR HRamCommand', D =
  // RIOB.00; RIOB (k01) clocks on preclk1'Bb OR RIOOut' OR TIOASaysDDC'.
  // HM 10.7: the IOB data of an Output<-B remains in RIOB until the next
  // output command. Watch what each edge captures.
  wire [7:0] dsw_tioa = {m.TIOA_0, m.TIOA_1, m.TIOA_2, m.TIOA_3, m.TIOA_4, m.TIOA_5, m.TIOA_6, m.TIOA_7};
  always @(posedge sys_clk)
    if (ring_left > 0)
      $display("tb_exec: DSW @%0d clk=%b c1Dc=%b c1Cc=%b pc1Bb=%b HRamCmd'=%b RIOBclk'=%b IOOut'=%b DDCsel'=%b IOB00=%b RIOB00=%b qa=%b qb=%b HasHRam=%b HRamWE'=%b WrHRam'=%b IOBout=%b TIOA=%h CIA=%h CT=%h",
               n_cyc2, mclk, m.b_DispY.clk1_p_Dc, m.b_DispY.clk1_p_Cc, m.b_DispY.preclk1_p_Bb,
               m.b_DispY.HRamCommand_p_, m.b_DispY.RIOBclk1_p_a, m.b_DispY.IOout_p_, m.b_DispY.TIOASaysDDC_p_,
               m.b_DispY.IOB_00, m.b_DispY.RIOB_00, m.b_DispY.u_h15.qa, m.b_DispY.u_h15.qb,
               m.b_DispY.DoradoHasHRam, m.b_DispY.HRamWE_p_, m.b_DispY.WriteHRam_p_, m.b_ProcL.IOBout,
               dsw_tioa, ring_cia, pct_ctask);

  // ---- DISPY: what INITHRAM did to the display board, counted over the run --
  // InitHRam (DisplayAux.mc) writes LoadAddress twice (the first turns on
  // DoradoHasHRam), then the HRam words from a table it reads out of IM with
  // ReadIM/B<-Link, then ReleaseRam. Count the strobes and the two h15 bits.
  integer dp_cmd = 0, dp_has = 0, dp_we = 0, dp_clk = 0, dp_ld = 0, dp_phs = 0, dp_hs = 0, dp_sel = 0, dp_rim = 0, dp_has_fall = -1, dp_out = 0;
  reg dp_clk_d = 1'b1, dp_hs_d = 1'b1, dp_has_d = 1'b0, dp_iobout_d = 1'b0;
  always @(posedge sys_clk) begin
    if (!m.b_DispY.HRamCommand_p_) dp_cmd = dp_cmd + 1;
    if (m.b_DispY.DoradoHasHRam) dp_has = dp_has + 1;
    if (dp_has_d && !m.b_DispY.DoradoHasHRam && dp_has_fall < 0) dp_has_fall = n_cyc2;
    dp_has_d <= m.b_DispY.DoradoHasHRam;
    if (!m.b_DispY.HRamWE_p_) dp_we = dp_we + 1;
    if (m.b_DispY.ClkHRamAddr_p_ && !dp_clk_d) dp_clk = dp_clk + 1;
    dp_clk_d <= m.b_DispY.ClkHRamAddr_p_;
    if (!m.b_DispY.LdHRamAddr_p_) dp_ld = dp_ld + 1;
    if (m.b_DispY.preHSync) dp_phs = dp_phs + 1;
    if (m.b_DispY.HSync_p_ && !dp_hs_d) dp_hs = dp_hs + 1;
    dp_hs_d <= m.b_DispY.HSync_p_;
    if (!m.b_DispY.TIOASaysDDC_p_) dp_sel = dp_sel + 1;
    if (!m.b_ContA.RIM_p_) dp_rim = dp_rim + 1;
    if (m.b_ProcL.IOBout && !dp_iobout_d && !m.b_DispY.TIOASaysDDC_p_) dp_out = dp_out + 1;
    dp_iobout_d <= m.b_ProcL.IOBout;
  end
  final $display("tb_exec: DISPY -- HRamCommand' low %0d, DoradoHasHRam high %0d (first fall @%0d), HRamWE' low %0d, ClkHRamAddr' edges %0d, LdHRamAddr' low %0d, preHSync high %0d, HSync' edges %0d, DispY selected (TIOASaysDDC' low) %0d, IOBout strobes to DispY %0d, ContA RIM' low %0d, of %0d",
                 dp_cmd, dp_has, dp_has_fall, dp_we, dp_clk, dp_ld, dp_phs, dp_hs, dp_sel, dp_out, dp_rim, n_cyc2);
`ifdef SCREEN
  // Is DispY CLOCKED, and does its slow-I/O input register move? Everything
  // else on the board hangs off these two.
  integer n_dyc1 = 0, n_dyc2 = 0, n_riob = 0, n_dyrst = 0;
  reg dyc1_d = 1'b1, dyc2_d = 1'b1, dycin_d = 1'b1, dymclk_d = 1'b1, dytop_d = 1'b1;
  integer n_dycin = 0, n_dyen = 0, n_dymen = 0, n_dymclk = 0, n_dytop = 0;
  always @(posedge sys_clk) begin
    if (m.b_DispY.clk1_p_Cc  && !dyc1_d) n_dyc1 = n_dyc1 + 1;
    if (m.b_DispY.preclk2_p_Da && !dyc2_d) n_dyc2 = n_dyc2 + 1;
    dyc1_d <= m.b_DispY.clk1_p_Cc; dyc2_d <= m.b_DispY.preclk2_p_Da;
    if (m.b_DispY.RIOB_00) n_riob = n_riob + 1;
    if (m.b_DispY.IOReset)  n_dyrst = n_dyrst + 1;
    if (m.b_DispY.CLK_display_p_ && !dycin_d) n_dycin = n_dycin + 1;
    dycin_d <= m.b_DispY.CLK_display_p_;
    if (n_cyc2 > 20000 && n_cyc2 < 20040)
      $display("tb_exec: DYSAMP %0d: mclk=%b top=%b board=%b clk1Cc=%b preclk2Da=%b CLKEn'b=%b", n_cyc2, mclk, m.CLK_display_p_, m.b_DispY.CLK_display_p_, m.b_DispY.clk1_p_Cc, m.b_DispY.preclk2_p_Da, m.b_DispY.CLKEnable_p_b);
    if (mclk && !dymclk_d) n_dymclk = n_dymclk + 1;
    dymclk_d <= mclk;
    if (m.CLK_display_p_ && !dytop_d) n_dytop = n_dytop + 1;
    dytop_d <= m.CLK_display_p_;
    if (m.b_DispY.CLKEnable_p_b) n_dyen = n_dyen + 1;
    if (m.b_DispY.MemClkEnable_p_a) n_dymen = n_dymen + 1;
  end
  final begin
    // A CONFIGURATION CAN LEAVE A BOARD UNCLOCKED AND SAY NOTHING. Verilator
    // ties an unconnected input to 0 and only warns, and the FULL branch of
    // the instantiation below once omitted `.CLK_display_p_(mclk)` -- so the
    // eleven-board machine ran with DispY frozen at its power-up levels and
    // every display counter read as a plausible zero (2026-09-03). Assert it.
    if (n_dycin == 0)
      $display("tb_exec: FAIL -- DispY is in this machine and its CLK_display' port never toggled: it is unconnected in this configuration");
  end
  final $display("tb_exec: DYCLK -- clk1'Cc rising edges %0d, preclk2'Da %0d, RIOB.00 high %0d, IOReset high %0d | CLK_display' edges %0d, CLKEnable'b high %0d, MemClkEnable'a high %0d | mclk edges %0d, top CLK_display' edges %0d, of %0d",
                 n_dyc1, n_dyc2, n_riob, n_dyrst, n_dycin, n_dyen, n_dymen, n_dymclk, n_dytop, n_cyc2);
`endif
`ifdef FULL
  // ---- ETH: does Initial start the Ethernet controller? ----------------------
  integer et_xmt = 0, et_tw06 = 0, et_tw07 = 0, et_out15 = 0, et_out16 = 0, et_in16 = 0;
  reg et_xmt_d = 1'b1, et_iobout_d = 1'b0;
  wire [7:0] et_tioa = {m.TIOA_0, m.TIOA_1, m.TIOA_2, m.TIOA_3, m.TIOA_4, m.TIOA_5, m.TIOA_6, m.TIOA_7};
  always @(posedge sys_clk) begin
    if (eth_xmt != et_xmt_d) et_xmt = et_xmt + 1;
    et_xmt_d <= eth_xmt;
    if (m.TWReq_06) et_tw06 = et_tw06 + 1;
    if (m.TWReq_07) et_tw07 = et_tw07 + 1;
    if (m.b_ProcL.IOBout && !et_iobout_d) begin
      if (et_tioa == 8'o15) et_out15 = et_out15 + 1;
      if (et_tioa == 8'o16) et_out16 = et_out16 + 1;
    end
    et_iobout_d <= m.b_ProcL.IOBout;
  end
`ifdef FULL
  // IOATT: DskEth is the only I/O board here that drives IOAtten, and Initial
  // branches on it in several places. With no drive attached the board's own
  // error summary (ReadError: FifoUnderflow, IOBParityErr, ...) is asserted,
  // so this asks whether an idle, driveless controller is claiming attention.
  integer n_ioatt = 0, n_ioatt_de = 0, n_ioatt_ph = 0, n_dskiob = 0, n_ddc = 0;
  always @(posedge sys_clk) begin
    if (m.IOatt)                    n_ioatt    = n_ioatt + 1;
    if (m.b_DskEth.IOatt__drv)      n_ioatt_de = n_ioatt_de + 1;
    if (m.b_ProcH.IOatt__drv)       n_ioatt_ph = n_ioatt_ph + 1;
    if (!m.b_DskEth.DskEth03_sil_pl_1) n_dskiob = n_dskiob + 1;
    if (!m.b_DispY.TIOASaysDDC_p_)  n_ddc      = n_ddc + 1;
  end
  // Each board's OR-tree contribution to IOB, counted per bit: a board that
  // drives a bit it is not being read from corrupts every other board's
  // decode of that bus.
  integer n_iobd [0:17]; integer n_iobp [0:17]; integer n_iobdy [0:17]; integer bi;
  initial for (bi = 0; bi < 18; bi = bi + 1) begin n_iobd[bi]=0; n_iobp[bi]=0; n_iobdy[bi]=0; end
  wire [17:0] iob_dsk = {m.b_DskEth.IOB_00__drv, m.b_DskEth.IOB_01__drv, m.b_DskEth.IOB_02__drv, m.b_DskEth.IOB_03__drv,
                         m.b_DskEth.IOB_04__drv, m.b_DskEth.IOB_05__drv, m.b_DskEth.IOB_06__drv, m.b_DskEth.IOB_07__drv,
                         m.b_DskEth.IOB_08__drv, m.b_DskEth.IOB_09__drv, m.b_DskEth.IOB_10__drv, m.b_DskEth.IOB_11__drv,
                         m.b_DskEth.IOB_12__drv, m.b_DskEth.IOB_13__drv, m.b_DskEth.IOB_14__drv, m.b_DskEth.IOB_15__drv,
                         m.b_DskEth.IOB_16__drv, m.b_DskEth.IOB_17__drv};
  wire [17:0] iob_dy  = {m.b_DispY.IOB_00__drv, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0,
                         1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, m.b_DispY.IOB_15__drv,
                         m.b_DispY.IOB_16__drv, m.b_DispY.IOB_17__drv};
  always @(posedge sys_clk)
    for (bi = 0; bi < 18; bi = bi + 1) begin
      if (iob_dsk[17-bi]) n_iobd[bi]  = n_iobd[bi] + 1;
      if (iob_dy[17-bi])  n_iobdy[bi] = n_iobdy[bi] + 1;
    end
  final begin
    $write("tb_exec: IOBDRV DskEth --");
    for (bi = 0; bi < 18; bi = bi + 1) $write(" %0d:%0d", bi, n_iobd[bi]);
    $write("\n");
    $write("tb_exec: IOBDRV DispY  --");
    for (bi = 0; bi < 18; bi = bi + 1) if (n_iobdy[bi] != 0) $write(" %0d:%0d", bi, n_iobdy[bi]);
    $write(" (of %0d)\n", n_cyc2);
  end
  integer n_dmx = 0, n_dmx_de = 0, n_dmx_dy = 0, n_men = 0;
  always @(posedge sys_clk) begin
    if (m.DMuxData)                 n_dmx    = n_dmx + 1;
    if (m.b_DskEth.DMuxData__drv)   n_dmx_de = n_dmx_de + 1;
    if (m.b_DispY.DMuxData__drv)    n_dmx_dy = n_dmx_dy + 1;
    if (!m.b_DskEth.MidasEn_01T_02F_03F_04F_p_) n_men = n_men + 1;
  end
  final $display("tb_exec: DMUX -- DMuxData high %0d (DskEth's contribution %0d, DispY's %0d), DskEth MidasEn' asserted %0d, of %0d",
                 n_dmx, n_dmx_de, n_dmx_dy, n_men, n_cyc2);
  final $display("tb_exec: IOATT -- IOatt high %0d (DskEth's contribution %0d, ProcH's %0d), DskEth driving IOB %0d, DispY selected %0d, of %0d",
                 n_ioatt, n_ioatt_de, n_ioatt_ph, n_dskiob, n_ddc, n_cyc2);
`endif
`ifdef FULL
  // ---- TFWIN / MDWIN: two per-task register files, watched slot by slot ----
  // `+tfwin=N +tfwinlen=L` prints, every sys_clk in [N, N+L), the T-file's
  // write pulse (ProcH j05 TbWrite'a), its address (CurrLast', from i12 on
  // DblClock, selected by a18 between LastNext and Curr), the data (dTm), the
  // task-number nets it is built from, and slots 2 and 3 of l03/l04 (T bits
  // 00-07). `+mdwin=N +mdwinlen=L` does the same for MemD's MDM file (h04,
  // dMD bits 00-03): WriteMDM'a, MDMad', the data pins, and slots 0 and 3.
  // Both exist because a task's T and a task's Md were each read back as
  // ANOTHER task's word (2026-09-05), and the F10145A address-move check
  // found the address stable across every pulse -- so the question is which
  // slot each pulse addresses, and what the machine's Last/Curr/Next say.
  longint tfwin = -1, tfwinlen = 0, mdwin = -1, mdwinlen = 0, clkwin = -1, clkwinlen = 0, memwin = -1, memwinlen = 0;
  // `+mdpc=HEX` opens the MDWIN and MEMWIN windows at the first MIR load of
  // that address (a cycle number moves with every cell change; an address
  // does not).
  reg [11:0] mdpc = 12'hfff; initial void'($value$plusargs("mdpc=%h", mdpc));
  always @(posedge sys_clk)
    if (mdpc != 12'hfff && ring_cia == mdpc && ring_cia != pct_cia_d) begin
      mdwin = n_cyc2; memwin = n_cyc2; mdpc = 12'hfff;
      $display("tb_exec: MDWIN/MEMWIN open @%0d at CIA=%h", n_cyc2, ring_cia);
    end
  initial begin
    void'($value$plusargs("clkwin=%d", clkwin)); void'($value$plusargs("clkwinlen=%d", clkwinlen));
    void'($value$plusargs("tfwin=%d", tfwin)); void'($value$plusargs("tfwinlen=%d", tfwinlen));
    void'($value$plusargs("mdwin=%d", mdwin)); void'($value$plusargs("mdwinlen=%d", mdwinlen));
  end
  wire [3:0] tf_curr = ~{m.b_ProcH.Curr_3_p_, m.b_ProcH.Curr_2_p_, m.b_ProcH.Curr_1_p_, m.b_ProcH.Curr_0_p_};
  wire [3:0] tf_last = ~{m.b_ProcH.Last_3_p_, m.b_ProcH.Last_2_p_, m.b_ProcH.Last_1_p_, m.b_ProcH.Last_0_p_};
  wire [3:0] tf_next = ~{m.b_ProcH.Next_3_p_, m.b_ProcH.Next_2_p_, m.b_ProcH.Next_1_p_, m.b_ProcH.Next_0_p_};
  wire [3:0] tf_ln   = ~{m.b_ProcH.LastNext_3_p_, m.b_ProcH.LastNext_2_p_, m.b_ProcH.LastNext_1_p_, m.b_ProcH.LastNext_0_p_};
  wire [3:0] tf_cl   = ~{m.b_ProcH.CurrLast_3_p_, m.b_ProcH.CurrLast_2_p_, m.b_ProcH.CurrLast_1_p_, m.b_ProcH.CurrLast_0_p_};
  wire [7:0] tf_dtm  = {m.b_ProcH.dTm_00, m.b_ProcH.dTm_01, m.b_ProcH.dTm_02, m.b_ProcH.dTm_03, m.b_ProcH.dTm_04, m.b_ProcH.dTm_05, m.b_ProcH.dTm_06, m.b_ProcH.dTm_07};
  wire [7:0] tf_t    = {m.b_ProcH.T_00, m.b_ProcH.T_01, m.b_ProcH.T_02, m.b_ProcH.T_03, m.b_ProcH.T_04, m.b_ProcH.T_05, m.b_ProcH.T_06, m.b_ProcH.T_07};
  always @(posedge sys_clk) begin
    if (tfwin >= 0 && n_cyc2 >= tfwin && n_cyc2 < tfwin + tfwinlen)
      $display("tb_exec: TFWIN @%0d task=%h CIA=%h | curr=%h last=%h next=%h LastNext=%h CurrLast=%h RepeatCurrC=%b DblClk'=%b PreClk0'=%b | TbWrite'a=%b TbWriteEn'=%b HoldDly=%b dTm=%02h T=%02h | l03[t2]=%h l03[t3]=%h l04[t2]=%h l04[t3]=%h Switcha=%b",
               n_cyc2, pct_ctask, ring_cia, tf_curr, tf_last, tf_next, tf_ln, tf_cl, m.b_ProcH.RepeatCurrC, m.b_ProcH.DblClock_p_Da, m.b_ProcH.PreClock0_p_B,
               m.b_ProcH.TbWrite_p_a, m.b_ProcH.TbWriteEn_p_, m.b_ProcH.HoldDly, tf_dtm, tf_t,
               m.b_ProcH.u_l03.mem[13], m.b_ProcH.u_l03.mem[12], m.b_ProcH.u_l04.mem[13], m.b_ProcH.u_l04.mem[12], m.b_ContA.Switcha);
    // +clkwin: the clock-family nets behind Curr/Last, per sys_clk
    if (clkwin >= 0 && n_cyc2 >= clkwin && n_cyc2 < clkwin + clkwinlen)
      $display("tb_exec: CLKWIN @%0d task=%h CIA=%h | PreClock1B=%b ppClk'a=%b pClock1En'=%b PreSHCP'C=%b SHCP'C=%b Clock1'Ca=%b (lead %b) PreClock1'C=%b DblClk'Da=%b PreClk0'B=%b | BNTGtCT=%b Hold'=%b RepeatCurrC=%b Switcha=%b | next=%h curr=%h last=%h LastNext=%h CurrLast=%h",
               n_cyc2, pct_ctask, ring_cia, m.b_ProcH.PreClock1B, m.b_ProcH.ppClk_p_a, m.b_ProcH.pClock1En_p_, m.b_ProcH.PreSHCP_p_C, m.b_ProcH.SHCP_p_C,
               m.b_ProcH.Clock1_p_Ca, m.b_ProcH.Clock1_p_Ca__lead, m.b_ProcH.PreClock1_p_C, m.b_ProcH.DblClock_p_Da, m.b_ProcH.PreClock0_p_B,
               m.b_ProcH.BNTGtCT, m.b_ProcH.Hold_p_, m.b_ProcH.RepeatCurrC, m.b_ContA.Switcha, tf_next, tf_curr, tf_last, tf_ln, tf_cl);
    if (mdwin >= 0 && n_cyc2 >= mdwin && n_cyc2 < mdwin + mdwinlen)
      $display("tb_exec: MDWIN @%0d task=%h CIA=%h | WriteMDM'a=%b MDMad=%h dMDMad=%h MD_u_D=%b clk2'Bc=%b | din h04=%b%b%b%b h15=%b%b%b%b h11=%b%b%b%b h17=%b%b%b%b h06=%b%b | t0 slot h04=%h h15=%h h11=%h h17=%h | t3 slot h04=%h h15=%h h11=%h h17=%h | Md=%04h",
               n_cyc2, pct_ctask, ring_cia, m.b_MemD.WriteMDM_p_a,
               ~{m.b_MemD.MDMad_3_p_, m.b_MemD.MDMad_2_p_, m.b_MemD.MDMad_1_p_, m.b_MemD.MDMad_0_p_},
               ~{m.b_MemD.dMDMad_3_p_, m.b_MemD.dMDMad_2_p_, m.b_MemD.dMDMad_1_p_, m.b_MemD.dMDMad_0_p_},
               m.b_MemD.MD_u_D, m.b_MemD.clk2_p_Bc,
               m.b_MemD.MemD08_sil_pl_16, m.b_MemD.MemD08_sil_pl_1, m.b_MemD.MemD07_sil_pl_8, m.b_MemD.MemD01_sil_pl_8,
               m.b_MemD.MemD10_sil_pl_16, m.b_MemD.MemD10_sil_pl_1, m.b_MemD.MemD09_sil_pl_16, m.b_MemD.MemD09_sil_pl_1,
               m.b_MemD.MemD12_sil_pl_16, m.b_MemD.MemD12_sil_pl_1, m.b_MemD.MemD11_sil_pl_16, m.b_MemD.MemD11_sil_pl_1,
               m.b_MemD.MemD14_sil_pl_16, m.b_MemD.MemD14_sil_pl_1, m.b_MemD.MemD13_sil_pl_16, m.b_MemD.MemD13_sil_pl_1,
               m.b_MemD.MemD15_sil_pl_14, m.b_MemD.MemD15_sil_pl_1,
               m.b_MemD.u_h04.mem[15], m.b_MemD.u_h15.mem[15], m.b_MemD.u_h11.mem[15], m.b_MemD.u_h17.mem[15],
               m.b_MemD.u_h04.mem[12], m.b_MemD.u_h15.mem[12], m.b_MemD.u_h11.mem[12], m.b_MemD.u_h17.mem[12],
               {m.b_ProcH.Md_00, m.b_ProcH.Md_01, m.b_ProcH.Md_02, m.b_ProcH.Md_03, m.b_ProcH.Md_04, m.b_ProcH.Md_05, m.b_ProcH.Md_06, m.b_ProcH.Md_07,
                m.b_ProcL.Md_08, m.b_ProcL.Md_09, m.b_ProcL.Md_10, m.b_ProcL.Md_11, m.b_ProcL.Md_12, m.b_ProcL.Md_13, m.b_ProcL.Md_14, m.b_ProcL.Md_15});
  end
  // +memwin: the Md-file WRITE SIDE on MemD, per sys_clk. MDWIN (run7,
  // 2026-09-05) showed WriteMDM'a pulsing for task 0's slot at +23 of the
  // read instruction, SIX sys_clk after f06 (the MC10176 that registers the D
  // bus for the file) had taken the NEXT reference's word. This prints the
  // pulse's three inputs (i12: DontWriteMDM, preClk0'BD, MakeMDM_u_D'), the
  // raw D bus, f06's clock and the select that goes with it.
  initial begin void'($value$plusargs("memwin=%d", memwin)); void'($value$plusargs("memwinlen=%d", memwinlen)); end
  always @(posedge sys_clk)
    if (memwin >= 0 && n_cyc2 >= memwin && n_cyc2 < memwin + memwinlen)
      $display("tb_exec: MEMWIN @%0d task=%h CIA=%h | WriteMDM'a=%b DontWriteMDM=%b preClk0'BD=%b MakeMDM_u_D'=%b MD_u_D=%b D_u_Dly'=%b clk1'Aa=%b clk2'Bc=%b | D00-03=%b%b%b%b f06=%b%b%b%b | MDMad=%h",
               n_cyc2, pct_ctask, ring_cia, m.b_MemD.WriteMDM_p_a, m.b_MemD.DontWriteMDM, m.b_MemD.preClk0_p_BD, m.b_MemD.MakeMDM_u_D_p_,
               m.b_MemD.MD_u_D, m.b_MemD.D_u_Dly_p_, m.b_MemD.clk1_p_Aa, m.b_MemD.clk2_p_Bc,
               m.b_MemD.D_00, m.b_MemD.D_01, m.b_MemD.D_02, m.b_MemD.D_03,
               m.b_MemD.MemD01_sil_pl_8, m.b_MemD.MemD07_sil_pl_8, m.b_MemD.MemD08_sil_pl_1, m.b_MemD.MemD08_sil_pl_16,
               ~{m.b_MemD.MDMad_3_p_, m.b_MemD.MDMad_2_p_, m.b_MemD.MDMad_1_p_, m.b_MemD.MDMad_0_p_});
  // +holdwin (opened by +mdpc with the Md windows): the Md-HOLD request on
  // MemC, term by term. HM p.42: an immediate <-Md is held in the cycle after
  // a fetch or store, and a <-Md after an intervening Store is held until the
  // store completes; every run's HOLDSRC line has read MDhold 0. dMDhold' is
  // the wired-OR of b22 (MDMtag' XOR ProcTag) and b23 (a 4-wide OR-AND over
  // DdataGood', At=Curt', <-MDI, DcomingForCt', <-MD, WantCR, MemC16.sil+6).
  always @(posedge sys_clk)
    if (mdwin >= 0 && n_cyc2 >= mdwin && n_cyc2 < mdwin + mdwinlen)
      $display("tb_exec: HOLDWIN @%0d task=%h CIA=%h | <-MD=%b <-MDI=%b | MDMtag'=%b ProcTag=%b b22=%b | DdataGood'=%b At=Curt'=%b DcomingForCt'=%b WantCR=%b sil6=%b b23=%b | dMDhold'=%b MDhold'=%b RefHold'=%b DisHold=%b Hold=%b",
               n_cyc2, pct_ctask, ring_cia, m.b_MemC._u_MD, m.b_MemC._u_MDI,
               m.b_MemC.MDMtag_p_, m.b_MemC.ProcTag, m.b_MemC.dMDhold_p___b22_3,
               m.b_MemC.DdataGood_p_, m.b_MemC.At_eq_Curt_p_, m.b_MemC.EcDcomingForCt_p_, m.b_MemC.WantCR, m.b_MemC.MemC16_sil_pl_6, m.b_MemC.dMDhold_p___b23_3,
               m.b_MemC.dMDhold_p_, m.b_MemC.MDhold_p_, m.b_MemC.RefHold_p_, m.b_MemC.DisHold, m.b_ContA.Hold);
  // +memwin also prints the memory pipeline's stage flags (MemX sheet 01 and
  // MemC's front door), per sys_clk: what the fetch and the store each do to
  // the flags that gate the Md file's write (i23 -> g23 MakeMDM_u_D') and the
  // Md hold (g24 -> e19 DdataGood').
  always @(posedge sys_clk)
    if (memwin >= 0 && n_cyc2 >= memwin && n_cyc2 < memwin + memwinlen)
      $display("tb_exec: MEMWIN2 @%0d task=%h CIA=%h | MemX: AWordRefToD=%b EcWordRefToD=%b bHit'=%b CacheRefInPair'=%b AfreeOrEc'b=%b EcKeepsAbusy=%b CacheRef=%b VictimInA'=%b STfree'=%b WantRfsh'=%b SW=%b AcanHaveD=%b Transporta=%b MakeMDM'=%b DdataGood'=%b | MemC: Dbusy=%b WantCR=%b CacheRefInA'=%b StartMap'=%b Store_IfCR'=%b Hit'b=%b Dad00=%b Dad01=%b",
               n_cyc2, pct_ctask, ring_cia,
               m.b_MemX.AWordRefToD, m.b_MemX.EcWordRefToD, m.b_MemX.bHit_p_, m.b_MemX.CacheRefInPair_p_, m.b_MemX.AfreeOrEc_p_b, m.b_MemX.EcKeepsAbusy,
               m.b_MemX.CacheRef, m.b_MemX.VictimInA_p_, m.b_MemX.STfree_p_, m.b_MemX.WantRfsh_p_, m.b_MemX.SW, m.b_MemX.AcanHaveD, m.b_MemX.Transporta,
               m.b_MemX.MakeMDM_u_D_p_, m.b_MemX.DdataGood_p_,
               m.b_MemC.Dbusy, m.b_MemC.WantCR, m.b_MemC.CacheRefInA_p_, m.b_MemC.StartMap_p_, m.b_MemC.Store_u_IfCR_p_, m.b_MemC.Hit_p_b, m.b_MemC.Dad_00__drv, m.b_MemC.Dad_01__drv);
  // +memwin also prints DontWriteMDM's two sources against the pulse: h03
  // (MC10176, clk0'B) registers bFastD_u_Dbuf | D_u_Dbufdly -- "the data
  // section is taking D from DBuf", i.e. a STORE's data phase -- and that is
  // the flag designed to keep a store's word out of the Md file. In run16 it
  // asserted one cycle after the pulse that stored the store's word.
  always @(posedge sys_clk)
    if (memwin >= 0 && n_cyc2 >= memwin && n_cyc2 < memwin + memwinlen)
      $display("tb_exec: MEMWIN3 @%0d task=%h CIA=%h | MemC FastD_Dbuf=%b | MemD bFastD_u_Dbuf=%b D_u_Dbufdly=%b D_u_Dbuf=%b D_u_CD'=%b DontWriteMDM=%b WriteMDM'a=%b MakeMDM'=%b MD_u_D=%b clk0'B=%b",
               n_cyc2, pct_ctask, ring_cia, m.b_MemC.FastD_u_Dbuf__drv,
               m.b_MemD.bFastD_u_Dbuf, m.b_MemD.D_u_Dbufdly, m.b_MemD.D_u_Dbuf, m.b_MemD.D_u_CD_p_, m.b_MemD.DontWriteMDM,
               m.b_MemD.WriteMDM_p_a, m.b_MemD.MakeMDM_u_D_p_, m.b_MemD.MD_u_D, m.b_MemD.clk0_p_B);
  // +memwin also prints the CACHE DATA ADDRESS the F10470 arrays are being
  // read at, plus the array write-enable. The Md file's data pins come
  // straight off these arrays (D = wired-OR of a03/d03 etc.), so which
  // LOCATION is presented decides whether the fetch's or the store's word is
  // on the bus. Measured 2026-09-06: the file's pins carry the store's word
  // 378-408 and the fetch's only at 410, while the write strobe commits at
  // ~391-395 -- so the array is being read at the STORE's address across the
  // strobe. This prints the address so that is confirmed rather than inferred.
  always @(posedge sys_clk)
    if (memwin >= 0 && n_cyc2 >= memwin && n_cyc2 < memwin + memwinlen)
      $display("tb_exec: MEMWIN4 @%0d CIA=%h | Dad_a=%b%b%b%b%b%b%b%b%b%b%b%b | WriteD0'a=%b | D00-03=%b%b%b%b | MD_u_D=%b WriteMDM'a=%b",
               n_cyc2, ring_cia,
               m.b_MemD.Dad_00a, m.b_MemD.Dad_01a, m.b_MemD.Dad_02a, m.b_MemD.Dad_03a,
               m.b_MemD.Dad_04a, m.b_MemD.Dad_05a, m.b_MemD.Dad_06a, m.b_MemD.Dad_07a,
               m.b_MemD.Dad_08a, m.b_MemD.Dad0_10a, m.b_MemD.Dad0_11a, m.b_MemD.Dad0_12a,
               m.b_MemD.WriteD0_p_a,
               m.b_MemD.D_00, m.b_MemD.D_01, m.b_MemD.D_02, m.b_MemD.D_03,
               m.b_MemD.MD_u_D, m.b_MemD.WriteMDM_p_a);
  // MEMWIN5 (2026-09-08): the D bus END TO END, per sys_clk -- MemC's data
  // address register (e14, clk1'Da) and the hit, MemD's own address register
  // (h01/h02/g01/g02 on DadHclk' = preClk1'B gated by DadH_u_'), the array
  // chip enables and write strobes, the full 18-bit D, f06's registered copy
  // (the Md file's data pins), and the D-source select flags from g24 (clk2'Dd).
  // Written to settle WHEN the array's output turns from the fetch's word into
  // the store's, against WHEN f06 samples it -- the F10470 read is registered
  // one sys_clk and the D bus is the OR of the column pairs' outputs.
  always @(posedge sys_clk)
    if (memwin >= 0 && n_cyc2 >= memwin && n_cyc2 < memwin + memwinlen)
      $display("tb_exec: MEMWIN5 @%0d CIA=%h | MemC clk1'Da=%b Dad01=%b%b dDad=%b%b%b%b%b%b%b%b%b%b%b%b Hit'a=%b Hit'b=%b Dbusy=%b s6=%b s3=%b FastD=%b | MemD preClk1'B=%b DadH'=%b DadHclk'b=%b clk1'Aa=%b Dad_a=%b%b%b%b%b%b%b%b%b.%b%b%b Dad09=%b D0ACE'a=%b D1ACE'a=%b WrD0'a=%b WrD1'c=%b | D=%b%b%b%b%b%b%b%b%b%b%b%b%b%b%b%b f06=%b%b%b%b%b%b MD_u_D=%b MakeMDM'=%b DontWr=%b WrMDM'a=%b preClk0'BD=%b | MakeD_Dbuf=%b D_Dbuf=%b MakeD_CD=%b D_CD=%b FastD_CD=%b bFastD=%b clk2'Dd=%b",
               n_cyc2, ring_cia,
               m.b_MemC.clk1_p_Da, m.b_MemC.Dad_00__drv, m.b_MemC.Dad_01__drv,
               m.b_MemC.dDad_02__drv, m.b_MemC.dDad_03__drv, m.b_MemC.dDad_04__drv, m.b_MemC.dDad_05__drv,
               m.b_MemC.dDad_06__drv, m.b_MemC.dDad_07__drv, m.b_MemC.dDad_08__drv, m.b_MemC.dDad_09__drv,
               m.b_MemC.dDad_10__drv, m.b_MemC.dDad_11__drv, m.b_MemC.dDad_12__drv, m.b_MemC.dDad_13__drv,
               m.b_MemC.Hit_p_a, m.b_MemC.Hit_p_b, m.b_MemC.Dbusy, m.b_MemC.MemC15_sil_pl_6, m.b_MemC.MemC15_sil_pl_3, m.b_MemC.FastD_u_Dbuf__drv,
               m.b_MemD.preClk1_p_B, m.b_MemD.DadH_u__p_, m.b_MemD.DadHclk_p_b, m.b_MemD.clk1_p_Aa,
               m.b_MemD.Dad_00a, m.b_MemD.Dad_01a, m.b_MemD.Dad_02a, m.b_MemD.Dad_03a,
               m.b_MemD.Dad_04a, m.b_MemD.Dad_05a, m.b_MemD.Dad_06a, m.b_MemD.Dad_07a,
               m.b_MemD.Dad_08a, m.b_MemD.Dad0_10a, m.b_MemD.Dad0_11a, m.b_MemD.Dad0_12a,
               m.b_MemD.Dad_09, m.b_MemD.D0ACE_p_a, m.b_MemD.D1ACE_p_a, m.b_MemD.WriteD0_p_a, m.b_MemD.WriteD1_p_c,
               m.b_MemD.D_00, m.b_MemD.D_01, m.b_MemD.D_02, m.b_MemD.D_03, m.b_MemD.D_04, m.b_MemD.D_05, m.b_MemD.D_06, m.b_MemD.D_07,
               m.b_MemD.D_08, m.b_MemD.D_09, m.b_MemD.D_10, m.b_MemD.D_11, m.b_MemD.D_12, m.b_MemD.D_13, m.b_MemD.D_14, m.b_MemD.D_15,
               m.b_MemD.MemD01_sil_pl_8, m.b_MemD.MemD07_sil_pl_8, m.b_MemD.MemD08_sil_pl_1, m.b_MemD.MemD08_sil_pl_16, m.b_MemD.MemD15_sil_pl_1, m.b_MemD.MemD15_sil_pl_14,
               m.b_MemD.MD_u_D, m.b_MemD.MakeMDM_u_D_p_, m.b_MemD.DontWriteMDM, m.b_MemD.WriteMDM_p_a, m.b_MemD.preClk0_p_BD,
               m.b_MemD.MakeD_u_Dbuf, m.b_MemD.D_u_Dbuf, m.b_MemD.MakeD_u_CD, m.b_MemD.D_u_CD, m.b_MemD.FastD_u_CD, m.b_MemD.bFastD_u_Dbuf, m.b_MemD.clk2_p_Dd);
  // MCRWIN (2026-09-08): the MCR's B SIDE. HM Figure 10 / Table 4: Mcr[13:15]
  // load from MapBuf, and bit 15 is NoWake; MemX f04 (F10016 on Clk1'Ab,
  // PE' = Mcr<-' from MemC l09) captures Mapbuf.15 as NoWakeups, and g23
  // registers ~NoWakeups as WakeEnable. `+mcrpc=HEX` opens a 200-sys_clk
  // window at the first MIR load of that address (0x213 is InitMap's
  // LoadMCR[T,T] with T = mcr.noWake; 0xc03 is Initial's SetMCR) and prints
  // the chain on every change. Written because a 200 M-cycle boot read
  // WakeEnable high on EVERY sample while memrun-test's SetMcr# jam drops it.
  reg [11:0] mcrpc = 12'hfff; initial void'($value$plusargs("mcrpc=%h", mcrpc));
  longint mcrwin = -1; reg [15:0] mcr_prev = 16'hffff;
  wire [15:0] mcr_now = {m.b_MemC.MemC19_sil_pl_14, m.b_MemC.PrClk1_p_Db, m.b_MemC.Mcr_u__p___drv,
                         m.b_MemX.Mcr_u__p_, m.b_MemX.Clk1_p_Ab, m.b_MemX.u_f04.q,
                         m.b_MemX.NoWakeups, m.b_MemX.Mapbuf_15, m.b_MemX.MapbufLd_p_c,
                         m.b_MemX.BMux_15, m.b_MemX.WakeEnable, m.b_MemX.u_f04.ck_en, 1'b0, 1'b0};
  always @(posedge sys_clk) begin
    if (mcrpc != 12'hfff && ring_cia == mcrpc && ring_cia != pct_cia_d) begin
      mcrwin = n_cyc2; mcrpc = 12'hfff;
      $display("tb_exec: MCRWIN open @%0d at CIA=%h", n_cyc2, ring_cia);
    end
    if (mcrwin >= 0 && n_cyc2 >= mcrwin - 40 && n_cyc2 < mcrwin + 200 && mcr_now != mcr_prev) begin
      mcr_prev <= mcr_now;
      $display("tb_exec: MCRWIN @%0d CIA=%h | MemC: dec(s19+14)=%b PrClk1'Db=%b Mcr<-'=%b | MemX: Mcr<-'=%b Clk1'Ab=%b f04.q=%h ck_en=%b NoWakeups=%b Mapbuf.15=%b MapbufLd'c=%b BMux.15=%b WakeEnable=%b | T=%04h",
               n_cyc2, ring_cia, m.b_MemC.MemC19_sil_pl_14, m.b_MemC.PrClk1_p_Db, m.b_MemC.Mcr_u__p___drv,
               m.b_MemX.Mcr_u__p_, m.b_MemX.Clk1_p_Ab, m.b_MemX.u_f04.q, m.b_MemX.u_f04.ck_en,
               m.b_MemX.NoWakeups, m.b_MemX.Mapbuf_15, m.b_MemX.MapbufLd_p_c, m.b_MemX.BMux_15, m.b_MemX.WakeEnable,
               {m.b_ProcH.T_00, m.b_ProcH.T_01, m.b_ProcH.T_02, m.b_ProcH.T_03, m.b_ProcH.T_04, m.b_ProcH.T_05, m.b_ProcH.T_06, m.b_ProcH.T_07,
                m.b_ProcL.T_08, m.b_ProcL.T_09, m.b_ProcL.T_10, m.b_ProcL.T_11, m.b_ProcL.T_12, m.b_ProcL.T_13, m.b_ProcL.T_14, m.b_ProcL.T_15});
    end
  end
  // SEQWIN (2026-09-08): the map and storage automata on every change in
  // [+seqfrom, +seqfrom+seqlen) -- tb_memrun's +seqwin, in the boot. Written
  // for the wedge in InitMap's Map1to1Loop: 64 map entries written, then
  // WaitForMapBuf spins for ever (HoldMapBuf 139 M samples of 400 M).
  longint seqfrom = -1, seqlen = 0; reg [63:0] seq_prev = 64'hffffffffffffffff;
  initial begin void'($value$plusargs("seqfrom=%d", seqfrom)); void'($value$plusargs("seqlen=%d", seqlen)); end
  wire [63:0] seq_now = {m.b_MemX.MapState_0, m.b_MemX.MapState_1, m.b_MemX.MapState_2,
                         m.b_MemX.MapFnc_0_p_, m.b_MemX.MapFnc_1_p_, m.b_MemX.MapWait,
                         m.b_MemX.StartMap_p_, m.b_MemX.preStartMem_p_, m.b_MemX.StartMem_p_,
                         m.b_MemX.MemState_0, m.b_MemX.MemState_1, m.b_MemX.MemState_2, m.b_MemX.MemState_3,
                         m.b_MemX.MemIdle, m.b_MemX.MemFree, m.b_MemX.RfshInMem, m.b_MemX.StartRfshCycle_p_,
                         m.b_MemX.NeedRfsh_p_, m.b_MemX.MapRfsh_p_, m.b_MemX.MapTroubleInMem, m.b_MemX.WriteInMem_p_,
                         m.b_MemX.MemRfsh, m.b_MemX.MapTrouble, m.b_MemX.MapFree, m.b_MemX.MapFree_p_,
                         m.b_MemX.HoldMapBuf__drv, m.b_MemX.AcanhaveMap_p___drv, m.b_MemX.Map_u_InPair_p_, m.b_MemX.Map_u_InMap,
                         m.b_MemC.CacheRefInA, m.b_MemC.Hit_p_b, m.b_MemC.ForceMiss, m.b_MemC.VictimInA,
                         m.b_MemC.WantVic, m.b_MemC.FlushStore, m.b_MemC.Store_u_InA, m.b_MemC.EcHasAa,
                         m.b_MemC.AcanhaveMap, m.b_MemC.AwantsMapFS, m.b_MemX.WantRfsh_p_,
                         m.MemRASa, m.MemCASa,
                         m.b_MemC.DbufBusy, m.b_MemC.Store_u_, m.MakeD_u_Dbuf, m.b_MemC.FastD_u_Dbuf,
                         m.b_MemX.VicSTPerr_p_, m.b_MemX.STPerr, m.b_MemX.dSTPerr, m.b_MemX.MemX10_sil_pl_8,
                         m.b_MemC.NoRef,
                         m.b_MemC.MDhold_p_, m.b_MemC.Dbusy, m.b_MemX.AcanHaveD, m.b_MemX.MakeTransport0,
                         m.b_MemX.MakeTransport1, m.b_MemX.MakeTransport2, m.b_MemX.Transporta, m.b_MemC.Hit_p_a,
                         m.b_MemC.Store_u_InA, 4'd0};
  always @(posedge sys_clk)
    if (seqfrom >= 0 && n_cyc2 >= seqfrom && n_cyc2 < seqfrom + seqlen && seq_now != seq_prev) begin
      seq_prev <= seq_now;
      $display("tb_exec: SEQ @%0d CIA=%h | MapSt=%b%b%b MapFnc'=%b%b MapWait=%b StartMap'=%b preStartMem'=%b StartMem'=%b | MemSt=%b%b%b%b MemIdle=%b MemFree=%b RfshInMem=%b StartRfsh'=%b NeedRfsh'=%b MapRfsh'=%b MapTrbInMem=%b WrInMem'=%b MemRfsh=%b MapTrb=%b MapFree=%b MapFree'=%b HoldMapBuf=%b AcanhaveMap'=%b Map'InPair'=%b Map'InMap=%b | CRinA=%b Hit'b=%b FMiss=%b VicInA=%b WantVic=%b FlS=%b StInA=%b EcHasA=%b AcanhaveMap=%b AwantsMapFS=%b WantRfsh'=%b | RAS=%b CAS=%b | DbufBusy=%b Store<-=%b MakeD<-Dbuf=%b FastD<-Dbuf=%b VicSTPerr'=%b STPerr=%b dSTPerr=%b MX10+8=%b NoRef=%b | MDhold'=%b Dbusy=%b AcanHaveD=%b MkTr0=%b MkTr1=%b MkTr2=%b Transporta=%b Hit'a=%b StoreInA=%b",
               n_cyc2, ring_cia, m.b_MemX.MapState_0, m.b_MemX.MapState_1, m.b_MemX.MapState_2,
               m.b_MemX.MapFnc_0_p_, m.b_MemX.MapFnc_1_p_, m.b_MemX.MapWait,
               m.b_MemX.StartMap_p_, m.b_MemX.preStartMem_p_, m.b_MemX.StartMem_p_,
               m.b_MemX.MemState_0, m.b_MemX.MemState_1, m.b_MemX.MemState_2, m.b_MemX.MemState_3,
               m.b_MemX.MemIdle, m.b_MemX.MemFree, m.b_MemX.RfshInMem, m.b_MemX.StartRfshCycle_p_,
               m.b_MemX.NeedRfsh_p_, m.b_MemX.MapRfsh_p_, m.b_MemX.MapTroubleInMem, m.b_MemX.WriteInMem_p_,
               m.b_MemX.MemRfsh, m.b_MemX.MapTrouble, m.b_MemX.MapFree, m.b_MemX.MapFree_p_,
               m.b_MemX.HoldMapBuf__drv, m.b_MemX.AcanhaveMap_p___drv, m.b_MemX.Map_u_InPair_p_, m.b_MemX.Map_u_InMap,
               m.b_MemC.CacheRefInA, m.b_MemC.Hit_p_b, m.b_MemC.ForceMiss, m.b_MemC.VictimInA,
               m.b_MemC.WantVic, m.b_MemC.FlushStore, m.b_MemC.Store_u_InA, m.b_MemC.EcHasAa,
               m.b_MemC.AcanhaveMap, m.b_MemC.AwantsMapFS, m.b_MemX.WantRfsh_p_,
               m.MemRASa, m.MemCASa,
               m.b_MemC.DbufBusy, m.b_MemC.Store_u_, m.MakeD_u_Dbuf, m.b_MemC.FastD_u_Dbuf,
               m.b_MemX.VicSTPerr_p_, m.b_MemX.STPerr, m.b_MemX.dSTPerr, m.b_MemX.MemX10_sil_pl_8,
               m.b_MemC.NoRef,
               m.b_MemC.MDhold_p_, m.b_MemC.Dbusy, m.b_MemX.AcanHaveD, m.b_MemX.MakeTransport0,
               m.b_MemX.MakeTransport1, m.b_MemX.MakeTransport2, m.b_MemX.Transporta, m.b_MemC.Hit_p_a,
               m.b_MemC.Store_u_InA);
    end
  // VAWIN (2026-09-08): the virtual address and the map's row/column
  // address at every map operation, in [+vafrom, +vafrom+valen). Written for
  // InitMap's map walk, which wrote 64 entries and then re-wrote them for
  // 290 M sys_clk: is the VA advancing (the BR+Mar adder, whose look-ahead
  // carry block MC10179 was a skeleton), and does what advances reach the
  // map RAM's address pins? Prints on StartMap' falling (a map cycle starts,
  // VA in force), and on MapRAS'/MapCAS' falling (the address the map RAM
  // latched, as the cell latched it).
  longint vafrom = -1, valen = 0;
  initial begin void'($value$plusargs("vafrom=%d", vafrom)); void'($value$plusargs("valen=%d", valen)); end
  wire [19:0] w_dva = {m.b_MemC.dVA_04, m.b_MemC.dVA_05, m.b_MemC.dVA_06, m.b_MemC.dVA_07,
                       m.b_MemC.dVA_08, m.b_MemC.dVA_09, m.b_MemC.dVA_10, m.b_MemC.dVA_11,
                       m.b_MemC.dVA_12, m.b_MemC.dVA_13, m.b_MemC.dVA_14, m.b_MemC.dVA_15,
                       m.b_MemC.dVA_16, m.b_MemC.dVA_17, m.b_MemC.dVA_18, m.b_MemC.dVA_19,
                       m.b_MemC.VA_20, m.b_MemC.VA_21, m.b_MemC.VA_22, m.b_MemC.VA_23};
  wire [8:0] w_mapad = {m.b_MemX.RTMapAd_0b, m.b_MemX.RTMapAd_1b, m.b_MemX.RTMapAd_2b, m.b_MemX.RTMapAd_3b,
                        m.b_MemX.RTMapAd_4b, m.b_MemX.RTMapAd_5b, m.b_MemX.RTMapAd_6b, m.b_MemX.RTMapAd_7b,
                        m.b_MemX.RTMapAd_8b};
  reg va_sm_d = 1'b1, va_ras_d = 1'b1, va_cas_d = 1'b1;
  always @(posedge sys_clk) begin
    if (vafrom >= 0 && n_cyc2 >= vafrom && n_cyc2 < vafrom + valen) begin
      if (!m.b_MemX.StartMap_p_ && va_sm_d)
        $display("tb_exec: VA @%0d CIA=%h StartMap' fall: ProcVA=%07h dVA(04-23)=%05h MapFnc'=%b%b CO8'=%b CO12'=%b CO16'=%b CO20'=%b Add'a=%b Add'b=%b",
                 n_cyc2, ring_cia, w_procva, w_dva, m.b_MemX.MapFnc_0_p_, m.b_MemX.MapFnc_1_p_,
                 m.b_MemC.CO8_p_, m.b_MemC.CO12_p_, m.b_MemC.CO16_p_, m.b_MemC.CO20_p_, m.b_MemC.Add_p_a, m.b_MemC.Add_p_b);
      if (!m.b_MemX.RTMapRAS_p_b && va_ras_d)
        $display("tb_exec: VA @%0d MapRAS' fall: MapAd(0-8)=%03h ProcVA=%07h", n_cyc2, w_mapad, w_procva);
      if (!m.b_MemX.RTMapCAS_p_b && va_cas_d)
        $display("tb_exec: VA @%0d MapCAS' fall: MapAd(0-8)=%03h -> d11 row=%02h col=%02h (WE'=%b)",
                 n_cyc2, w_mapad, m.b_MemX.u_d11.row, m.b_MemX.u_d11.col, m.b_MemX.TDirtyWE_p_);
    end
    va_sm_d <= m.b_MemX.StartMap_p_; va_ras_d <= m.b_MemX.RTMapRAS_p_b; va_cas_d <= m.b_MemX.RTMapCAS_p_b;
  end
  // MDHOLD (2026-09-08): every <-Md the processor issues, with the terms of
  // MemC's Md-hold request beside it. HOLDSRC has read "MDhold 0" in every
  // 400 M boot since the address adders went live -- HM p.42 says a <-Md
  // after a fetch or store by the same task that MISSED must hold until the
  // line is loaded, and p.60 makes that hold the microcode's only protection
  // against issuing Map<- during a store (infinite DBufBusy, which is what
  // InitMap's zeroing loop measures). `+mdfrom=CYC` starts it, `+mdmax=N`
  // caps the lines (default 300). Prints on the sys_clk where <-MD (MemC
  // _u_MD) first rises, i.e. once per instruction.
  longint mdfrom = -1; integer mdmax = 300, n_mdp = 0; reg d_umd = 1'b0;
  initial begin void'($value$plusargs("mdfrom=%d", mdfrom)); void'($value$plusargs("mdmax=%d", mdmax)); end
  always @(posedge sys_clk) begin
    if (mdfrom >= 0 && n_cyc2 >= mdfrom && m.b_MemC._u_MD && !d_umd && n_mdp < mdmax) begin
      n_mdp = n_mdp + 1;
      $display("tb_exec: MDHOLD @%0d CIA=%h <-MD: DdataGood'=%b At=Curt'=%b MC16+6=%b <-MDI=%b DcomingForCt'=%b WantCR=%b | b23.3=%b b22.3=%b dMDhold'=%b MDhold'=%b | Hit'a=%b CRinA=%b StInA=%b DbufBusy=%b EcHasA=%b MDMtag'=%b ProcTag=%b",
               n_cyc2, ring_cia, m.b_MemC.DdataGood_p_, m.b_MemC.At_eq_Curt_p_, m.b_MemC.MemC16_sil_pl_6,
               m.b_MemC._u_MDI, m.b_MemC.EcDcomingForCt_p_, m.b_MemC.WantCR,
               m.b_MemC.dMDhold_p___b23_3, m.b_MemC.dMDhold_p___b22_3, m.b_MemC.dMDhold_p_, m.b_MemC.MDhold_p_,
               m.b_MemC.Hit_p_a, m.b_MemC.CacheRefInA, m.b_MemC.Store_u_InA, m.b_MemC.DbufBusy, m.b_MemC.EcHasAa,
               m.b_MemC.MDMtag_p_, m.b_MemC.ProcTag);
    end
    d_umd <= m.b_MemC._u_MD;
  end
  // TAGWIN (2026-09-08): the Md "tag" (HM glossary: the extra bit in Md
  // readout which complements for successive Fetch<-'es and Store<-'s by the
  // same task; agreement with the current value = reference finished). From
  // `+tagfrom=CYC`, every accepted cache reference (MemX CacheRef' falling)
  // opens a 48-sys_clk window in which every sys_clk prints the tag chain:
  // e20 (the per-task Ptag RAM, written every Clk0'Ce with XNOR(CacheRef',
  // e23's registered Ptag)), a23 (ProcTagInA, loaded into the pair), f21
  // (the Md tag RAM, MDMtag'). `+tagmax=N` caps the windows (default 12).
  // Written for Initial's first Store<- after SETMCR, which the RTL holds for
  // ever with MDMtag'=0 and ProcTag=0 while the pair carries tag 1.
  // `+tagbrief`: one line per CacheRef' fall and one per WriteMDM' fall
  // instead of the 48-line window -- the whole boot's tag history in a few
  // thousand lines.
  longint tagfrom = -1; integer tagmax = 12, n_tagw = 0, tag_left = 0, taglen = 48; reg d_cref = 1'b1, d_wmdm = 1'b1, tagbrief = 1'b0;
  // Initial.mb ResetTags: its Store (6034B) and Fetch (6250B) must each
  // write the completion tag. Inconsistent CVA parity/VA4 stuffing loses
  // both writes, but DisHold hides the fault until SetTestSyn much later.
  reg check_reset_tags = 0;
  reg [1:0] reset_tag_pending = 0, reset_tag_done = 0;
  initial check_reset_tags = $test$plusargs("check-reset-tags");
  // The bounded GetMemConfig probe enters at real 1000B after Initial's
  // ALU/RM/IFUM setup. At 4136B its computed pages/module is still in T.
  reg check_memconfig = 0, memconfig_checked = 0;
  initial check_memconfig = $test$plusargs("check-memconfig");
  // AltoDiabloDisk.mc: real 3007B follows the call to InitRamDiablo.
  // Check the actual microcode's writes, not the old per-word jam stimulus.
  reg check_disk_format = 0, disk_format_checked = 0;
  initial check_disk_format = $test$plusargs("check-disk-format");
  integer disk_err_lines = 0;
  reg disk_err_prev = 0;
  always @(posedge sys_clk) begin
    if (check_disk_format && m.b_DskEth.u_b12.cka_en &&
        !m.b_DskEth.SampleIOBparity_p_ &&
        (^{el2_iob[15:8], m.b_DskEth.bIOB_16}) &&
        (^{el2_iob[7:0], m.b_DskEth.bIOB_17}) && m.b_DskEth.dIOBParityErr)
      $fatal(1, "disk IOB checker rejects a valid odd-parity word %h", el2_iob);
    if (disk_err_trace && n_cyc2 > 270000 && disk_err_lines < 200 &&
        (m.b_DskEth.IOBParityErr != disk_err_prev ||
         (m.b_DskEth.u_b12.cka_en && !m.b_DskEth.SampleIOBparity_p_))) begin
      disk_err_lines = disk_err_lines + 1;
      $display("tb_exec: DISKERR @%0d CIA=%h IOB=%h par=%b%b xor=%b%b sample'=%b clk1'=%b err=%b dErr=%b pre=%b set=%b clear=%b",
        n_cyc2, ring_cia, el2_iob, m.b_DskEth.bIOB_16, m.b_DskEth.bIOB_17,
        ^{el2_iob[15:8], m.b_DskEth.bIOB_16}, ^{el2_iob[7:0], m.b_DskEth.bIOB_17},
        m.b_DskEth.SampleIOBparity_p_, m.b_DskEth.Clock1_p_Cb,
        m.b_DskEth.IOBParityErr, m.b_DskEth.dIOBParityErr,
        m.b_DskEth.u_b12.d_pre_a, m.b_DskEth.TriconD11_sil_pl_1, m.b_DskEth.ClearErrors);
    end
    disk_err_prev <= m.b_DskEth.IOBParityErr;
  end
  localparam [11:0] disk_format_expected [16] = '{
    12'o0001, 12'o0007, 12'o0377, 12'o0000,
    12'o0104, 12'o0204, 12'o0004, 12'o0000,
    12'o0033, 12'o0006, 12'o0011, 12'o0002,
    12'o0002, 12'o0001, 12'o0000, 12'o0000};
  function automatic [3:0] disk_format_rev(input [3:0] v);
    disk_format_rev = {v[0], v[1], v[2], v[3]};
  endfunction
  always @(posedge sys_clk)
    if (check_disk_format && !disk_format_checked && ring_cia == 12'h607) begin
      for (integer k = 0; k < 16; k = k + 1) begin
        if ({disk_format_rev(m.b_DskEth.u_e16.mem[k]),
             disk_format_rev(m.b_DskEth.u_f16.mem[k]),
             disk_format_rev(m.b_DskEth.u_f17.mem[k])} !== disk_format_expected[k])
          $fatal(1, "InitRamDiablo format[%0d]=%04o, expected %04o", k,
            {disk_format_rev(m.b_DskEth.u_e16.mem[k]),
             disk_format_rev(m.b_DskEth.u_f16.mem[k]),
             disk_format_rev(m.b_DskEth.u_f17.mem[k])}, disk_format_expected[k]);
      end
      if (m.b_DskEth.IOBParityErr)
        $fatal(1, "InitRamDiablo latched an IOB parity error on processor-generated odd parity");
      $display("tb_exec: DISK-SUBSECTOR -- loaded %h, counter %h",
        {m.b_DskEth.TriconD06_sil_pl_5,m.b_DskEth.TriconD06_sil_pl_6,
         m.b_DskEth.TriconD06_sil_pl_7,m.b_DskEth.TriconD06_sil_pl_8},m.b_DskEth.u_a01.q);
      disk_format_checked = 1;
      $display("tb_exec: DISK-FORMAT PASS -- InitRamDiablo wrote all sixteen words (HM 9.6)");
    end
  always @(posedge sys_clk)
    if (check_memconfig && !memconfig_checked && ring_cia == 12'h85e) begin
      if (brc_t != 16'h0400)
        $fatal(1, "GetMemConfig reports %0d pages/module; 16K storage chips require 1024", brc_t);
      memconfig_checked = 1;
      $display("tb_exec: MEMCONFIG PASS -- GetMemConfig reports 1024 pages/module (16K chips)");
    end
  initial begin void'($value$plusargs("tagfrom=%d", tagfrom)); void'($value$plusargs("tagmax=%d", tagmax)); void'($value$plusargs("taglen=%d", taglen)); tagbrief = $test$plusargs("tagbrief"); end
  always @(posedge sys_clk) begin
    if (check_reset_tags) begin
      if (!m.b_MemX.CacheRef_p_ && d_cref) begin
        if (reset_tag_pending != 0)
          $fatal(1, "ResetTags reference never wrote its Md completion tag");
        if (ring_cia == 12'hc1c) reset_tag_pending = 2'b01;
        if (ring_cia == 12'hca8) reset_tag_pending = 2'b10;
      end
      if (!m.b_MemX.WriteMDM_p_ && d_wmdm) begin
        reset_tag_done = reset_tag_done | reset_tag_pending;
        reset_tag_pending = 0;
      end
    end
    if (tagbrief && tagfrom >= 0 && n_cyc2 >= tagfrom && n_tagw < tagmax) begin
      if (!m.b_MemX.CacheRef_p_ && d_cref) begin
        n_tagw = n_tagw + 1;
        $display("tb_exec: TAGB @%0d CIA=%h REF  task=%b%b%b%b Ptag=%b ProcTag=%b MDMtag'=%b ProcTagInA=%b Hold=%b MDhold'=%b",
                 n_cyc2, ring_cia, m.b_MemX.CurTask_0, m.b_MemX.CurTask_1, m.b_MemX.CurTask_2, m.b_MemX.CurTask_3,
                 m.b_MemX.Ptag, m.b_MemX.ProcTag, m.b_MemX.MDMtag_p_, m.b_MemX.ProcTagInA, m.Hold, m.b_MemC.MDhold_p_);
      end
      if (!m.b_MemX.WriteMDM_p_ && d_wmdm) begin
        n_tagw = n_tagw + 1;
        $display("tb_exec: TAGB @%0d CIA=%h WMDM task=%b%b%b%b MDMtagAd=%b%b%b%b Dtag'=%b Ptag=%b ProcTag=%b ProcTagInA=%b AcanHaveD=%b TagInEc2=%b -> MDMtag'(before)=%b",
                 n_cyc2, ring_cia, m.b_MemX.CurTask_0, m.b_MemX.CurTask_1, m.b_MemX.CurTask_2, m.b_MemX.CurTask_3,
                 m.b_MemX.MDMtagAd_0, m.b_MemX.MDMtagAd_1, m.b_MemX.MDMtagAd_2, m.b_MemX.MDMtagAd_3,
                 m.b_MemX.Dtag_p_, m.b_MemX.Ptag, m.b_MemX.ProcTag, m.b_MemX.ProcTagInA, m.b_MemX.AcanHaveD, m.b_MemX.TagInEc2, m.b_MemX.MDMtag_p_);
      end
    end
    d_wmdm <= m.b_MemX.WriteMDM_p_;
    if (!tagbrief && tagfrom >= 0 && n_cyc2 >= tagfrom && !m.b_MemX.CacheRef_p_ && d_cref && n_tagw < tagmax) begin
      n_tagw = n_tagw + 1; tag_left = taglen;
      $display("tb_exec: TAG ---- window %0d: CacheRef' fall @%0d CIA=%h CurTask=%b%b%b%b", n_tagw, n_cyc2, ring_cia,
               m.b_MemX.CurTask_0, m.b_MemX.CurTask_1, m.b_MemX.CurTask_2, m.b_MemX.CurTask_3);
    end
    if (tag_left > 0) begin
      tag_left = tag_left - 1;
      $display("tb_exec: TAG @%0d CIA=%h CacheRef'=%b Clk0'Ce=%b Clk1'Cb=%b pre0'Dc=%b | Ptag=%b e23=%b new=%b ProcTag=%b | PLE'=%b PTA=%b CRP'=%b Hita=%b AfOE'b=%b AWRD=%b EcWRD=%b MkMDM'=%b | WMDM'=%b Dtag'=%b Ad=%b%b%b%b MDM'=%b | Hold=%b MDh'=%b",
               n_cyc2, ring_cia, m.b_MemX.CacheRef_p_, m.b_MemX.Clk0_p_Ce, m.b_MemX.Clk1_p_Cb, m.b_MemX.preClk0_p_Dc,
               m.b_MemX.Ptag, m.b_MemX.MemX01_sil_pl_11, m.b_MemX.MemX01_sil_pl_4, m.b_MemX.ProcTag,
               m.b_MemX.PairLdEnable_p_, m.b_MemX.ProcTagInA, m.b_MemX.CacheRefInPair_p_,
               m.b_MemX.Hita, m.b_MemX.AfreeOrEc_p_b, m.b_MemX.AWordRefToD, m.b_MemX.EcWordRefToD, m.b_MemX.MakeMDM_u_D_p_,
               m.b_MemX.WriteMDM_p_, m.b_MemX.Dtag_p_, m.b_MemX.MDMtagAd_0, m.b_MemX.MDMtagAd_1, m.b_MemX.MDMtagAd_2, m.b_MemX.MDMtagAd_3,
               m.b_MemX.MDMtag_p_, m.Hold, m.b_MemC.MDhold_p_);
    end
    d_cref <= m.b_MemX.CacheRef_p_;
  end
  // Observe the address-to-data handoff at every fabric sample. In particular,
  // distinguish a hit that survives to the handoff clock from an idle hit.
  longint cachewin = -1;
  integer cachewinlen = 160;
  initial begin
    void'($value$plusargs("cachewin=%d", cachewin));
    void'($value$plusargs("cachewinlen=%d", cachewinlen));
  end
  // Optional diagnostic watchdog, not a hardware timeout. End long boot
  // experiments promptly on a continuous hold, retaining its source pins.
  integer hold_limit = 0, held_samples = 0;
  initial void'($value$plusargs("hold-limit=%d", hold_limit));
  always @(posedge sys_clk) if (hold_limit > 0 && n_cyc2 > 100000) begin
    if (m.Hold) held_samples = held_samples + 1;
    else held_samples = 0;
    if (held_samples >= hold_limit) begin
      $display("tb_exec: HOLD-WATCH @%0d CIA=%h VA=%h MDhold'=%b RefHold'=%b MiscHold'=%b DbufBusy=%b STPerr=%b dSTPerr=%b MakeMDM'=%b CacheRefInA=%b EcHasA'=%b",
        n_cyc2, ring_cia, w_procva, m.b_MemC.MDhold_p_,
        m.b_MemC.RefHold_p_, m.b_MemC.MiscHold_p_, m.b_MemC.DbufBusy,
        m.b_MemX.STPerr, m.b_MemX.dSTPerr, m.b_MemX.MakeMDM_u_D_p_,
        m.b_MemC.CacheRefInA, m.b_MemC.EcHasA_p_);
      $fatal(1, "diagnostic hold-limit reached (%0d fabric samples)", held_samples);
    end
  end
  always @(negedge sys_clk)
    if (cachewin >= 0 && n_cyc2 >= cachewin && n_cyc2 < cachewin + cachewinlen) begin
      $display("tb_exec: CWIN @%0d CIA=%h clk0=%b clk1=%b SH=%b LdVA=%b dVA=%05h VA=%07h compare=%b hit=%b CRP'=%b Afree'=%b AWRD=%b MkMDM'=%b pipeWE'=%b pipeAd=%h pipeD=%b%b%b%b pipeQ=%b%b%b%b EcHasA'=%b Ec1=%b%b",
        n_cyc2, ring_cia, m.b_MemX.Clk0_p_Cb, m.b_MemC.clk1_p_Dc,
        m.b_MemC.MemSH, m.b_MemC.LdProcVA_p_Ab, w_dva, w_procva,
        m.b_MemC.MemC15_sil_pl_1, m.b_MemC.Hita,
        m.b_MemX.CacheRefInPair_p_, m.b_MemX.AfreeOrEc_p_b,
        m.b_MemX.AWordRefToD, m.b_MemX.MakeMDM_u_D_p_,
        m.b_MemC.WrPipe02_p_c,
        {m.b_MemC.PipeAd_0, m.b_MemC.PipeAd_1, m.b_MemC.PipeAd_2, m.b_MemC.PipeAd_3},
        m.b_MemC.ProcTagInA, m.b_MemC.CacheRefInA, m.b_MemC.Store_u_InA_p_, m.b_MemC.IfuRefInA,
        m.b_MemC.PipeTag, m.b_MemC.PipeCacheRef, m.b_MemC.PipeStore_u__p_, m.b_MemC.PipeIfuRef,
        m.b_MemC.EcHasA_p_, m.b_MemC.CacheRefInEc1__drv, m.b_MemC.Store_u_InEc1_p___drv);
      $display("tb_exec: CWIN2 @%0d NewRef=%b want'=%b latwant'=%b Add'=%b DisBR=%b MAR'=%h hiA=%h hiB=%h hiSum=%h heldVA=%h muxEn=%b rawMiss=%b%b%b%b",
        n_cyc2, m.b_MemC.NewRef, m.b_MemC.WantProcRef_p_, m.b_MemC.MemC16_sil_pl_4,
        m.b_MemC.Add_p_b, m.b_MemC.DisBR,
        {m.b_MemC.MAR_00_p_,m.b_MemC.MAR_01_p_,m.b_MemC.MAR_02_p_,m.b_MemC.MAR_03_p_,m.b_MemC.MAR_04_p_,m.b_MemC.MAR_05_p_,m.b_MemC.MAR_06_p_,m.b_MemC.MAR_07_p_,m.b_MemC.MAR_08_p_,m.b_MemC.MAR_09_p_,m.b_MemC.MAR_10_p_,m.b_MemC.MAR_11_p_,m.b_MemC.MAR_12_p_,m.b_MemC.MAR_13_p_,m.b_MemC.MAR_14_p_,m.b_MemC.MAR_15_p_},
        m.b_MemC.u__h_5e42.a, m.b_MemC.u__h_5e42.b, m.b_MemC.u__h_5e42.f,
        m.b_MemC.u_d02.q, m.b_MemC._u_PrVACVA,
        m.b_MemC.dMiss0_04_m_11, m.b_MemC.dMiss0_12_m_15, m.b_MemC.dMiss0_16_m_19, m.b_MemC.ForceMiss);
      $display("tb_exec: CWIN3 @%0d en'=%b compareVA=%h tagVA=%h xor=%h row=%h",
        n_cyc2, m.b_MemC.PairHasA_p_b,
        {m.b_MemC.u_e20.p6,m.b_MemC.u_e20.p10,m.b_MemC.u_e20.p12,m.b_MemC.u_e20.p5,m.b_MemC.u_e18.p6,m.b_MemC.u_e18.p10,m.b_MemC.u_e18.p12,m.b_MemC.u_e18.p5},
        {m.b_MemC.u_e20.p7,m.b_MemC.u_e20.p11,m.b_MemC.u_e20.p13,m.b_MemC.u_e20.p4,m.b_MemC.u_e18.p7,m.b_MemC.u_e18.p11,m.b_MemC.u_e18.p13,m.b_MemC.u_e18.p4},
        {m.b_MemC.u_e20.p3,m.b_MemC.u_e20.p14,m.b_MemC.u_e20.p15,m.b_MemC.u_e20.p2,m.b_MemC.u_e18.p3,m.b_MemC.u_e18.p14,m.b_MemC.u_e18.p15,m.b_MemC.u_e18.p2},
        m.b_MemC.u_a08.a);
    end

  // PIPEWIN (2026-09-08): the pipe's flag word (MemC l04, an F10145A at
  // PipeAd: Tag, CacheRef, Store', IfuRef) at every write (WrPipe02'c
  // falling) and every corrector takeover (EcHasA' falling, when MemC l03
  // loads the Ec1 flags from it). From `+pipefrom=CYC`, at most `+pipemax=N`
  // lines. Written because MakeMDM<-D (the Md tag/file write) never fires on
  // a miss: EcWordRefToD = Ec2State5 & CacheRefInEc2 needs the pipe's
  // CacheRef bit to come back 1.
  longint pipefrom = -1; integer pipemax = 200, n_pipe = 0; reg d_wp02 = 1'b1, d_echasa = 1'b1;
  initial begin void'($value$plusargs("pipefrom=%d", pipefrom)); void'($value$plusargs("pipemax=%d", pipemax)); end
  wire [3:0] w_pipead = {m.b_MemC.PipeAd_3, m.b_MemC.PipeAd_2, m.b_MemC.PipeAd_1, m.b_MemC.PipeAd_0};
  always @(posedge sys_clk) begin
    if (pipefrom >= 0 && n_cyc2 >= pipefrom && n_pipe < pipemax) begin
      if (!m.b_MemC.WrPipe02_p_c && d_wp02) begin
        n_pipe = n_pipe + 1;
        $display("tb_exec: PIPE @%0d CIA=%h WrPipe02' fall: PipeAd=%h din: ProcTagInA=%b CacheRefInA=%b Store'InA'=%b IfuRefInA=%b | CRinA(now)=%b StInA=%b Hit'b=%b",
                 n_cyc2, ring_cia, w_pipead, m.b_MemC.ProcTagInA, m.b_MemC.CacheRefInA, m.b_MemC.Store_u_InA_p_, m.b_MemC.IfuRefInA,
                 m.b_MemC.CacheRefInA, m.b_MemC.Store_u_InA, m.b_MemC.Hit_p_b);
      end
      if (!m.b_MemC.EcHasA_p_ && d_echasa) begin
        n_pipe = n_pipe + 1;
        $display("tb_exec: PIPE @%0d CIA=%h EcHasA' fall: PipeAd=%h dout: PipeTag=%b PipeCacheRef=%b PipeStore''=%b PipeIfuRef=%b -> Ec1: TagInEc1=%b CacheRefInEc1=%b Store<-InEc1'=%b IfuRefInEc1=%b",
                 n_cyc2, ring_cia, w_pipead, m.b_MemC.PipeTag, m.b_MemC.PipeCacheRef, m.b_MemC.PipeStore_u__p_, m.b_MemC.PipeIfuRef,
                 m.b_MemC.TagInEc1__drv, m.b_MemC.CacheRefInEc1__drv, m.b_MemC.Store_u_InEc1_p___drv, m.b_MemC.IfuRefInEc1__drv);
      end
    end
    d_wp02 <= m.b_MemC.WrPipe02_p_c; d_echasa <= m.b_MemC.EcHasA_p_;
  end
  // CAWIN (2026-09-08): every write of the cache address memory (MemC
  // WrCVA0-3', one per column) and of the cache flags (WrCflags0-3'), with
  // the VA in force, from `+cafrom=CYC`, at most `+camax=N` lines. Written
  // for Initial's PresetMap: "Kick-start the automata by performing two
  // fetches. Cope with 'tag' bit by performing a reference that hits" -- the
  // hit needs CacheA to hold VA 0 after the two forced-miss fetches.
  longint cafrom = -1; integer camax = 200, n_caw = 0;
  reg [3:0] ca_d_cva = 4'b1111, ca_d_cf = 4'b1111; reg ca_d_crina = 1'b0;
  initial begin void'($value$plusargs("cafrom=%d", cafrom)); void'($value$plusargs("camax=%d", camax)); end
  wire [3:0] w_wrcva = {m.b_MemC.WrCVA3_p_a, m.b_MemC.WrCVA2_p_a, m.b_MemC.WrCVA1_p_a, m.b_MemC.WrCVA0_p_a};
  wire [3:0] w_wrcf  = {m.b_MemC.WrCflags3_p_, m.b_MemC.WrCflags2_p_, m.b_MemC.WrCflags1_p_, m.b_MemC.WrCflags0_p_};
  always @(posedge sys_clk) begin
    if (cafrom >= 0 && n_cyc2 >= cafrom && n_caw < camax) begin
      if ((~w_wrcva & ca_d_cva) != 4'b0000) begin
        n_caw = n_caw + 1;
        $display("tb_exec: CA @%0d CIA=%h WrCVA' fall col=%b ProcVA=%07h Aad=%b%b%b%b%b%b%b%b a10.q(VA16-19 col0 at Aad)=%b%b%b%b Hit'b=%b CRinA=%b VicInA=%b MapTrb=%b FDMiss=%b", n_cyc2, ring_cia,
                 ~w_wrcva & ca_d_cva, w_procva, m.b_MemC.Aad_0a, m.b_MemC.Aad_1a, m.b_MemC.Aad_2a, m.b_MemC.Aad_3a, m.b_MemC.Aad_4a, m.b_MemC.Aad_5a, m.b_MemC.Aad_6a, m.b_MemC.Aad_7a,
                 m.b_MemC.MemC12_sil_pl_19, m.b_MemC.MemC12_sil_pl_20, m.b_MemC.MemC12_sil_pl_31, m.b_MemC.MemC12_sil_pl_25,
                 m.b_MemC.Hit_p_b, m.b_MemC.CacheRefInA, m.b_MemC.VictimInA, m.b_MemX.MapTrouble, m.b_MemC.ForceDirtyMiss);
      end
      if (m.b_MemC.CacheRefInA && !ca_d_crina) begin
        n_caw = n_caw + 1;
        $display("tb_exec: CA @%0d CIA=%h CRinA rise: ProcVA=%07h dVA(04-23)=%05h Aad=%b%b%b%b%b%b%b%b col0 stored VA16-19=%b%b%b%b dMiss0(04-11,12-15,16-19)=%b%b%b dMiss1=%b%b%b dMiss2=%b%b%b dMiss3=%b%b%b ForceMiss=%b DisCF=%b", n_cyc2, ring_cia,
                 w_procva, w_dva, m.b_MemC.Aad_0a, m.b_MemC.Aad_1a, m.b_MemC.Aad_2a, m.b_MemC.Aad_3a, m.b_MemC.Aad_4a, m.b_MemC.Aad_5a, m.b_MemC.Aad_6a, m.b_MemC.Aad_7a,
                 m.b_MemC.MemC12_sil_pl_19, m.b_MemC.MemC12_sil_pl_20, m.b_MemC.MemC12_sil_pl_31, m.b_MemC.MemC12_sil_pl_25,
                 m.b_MemC.dMiss0_04_m_11, m.b_MemC.dMiss0_12_m_15, m.b_MemC.dMiss0_16_m_19, m.b_MemC.dMiss1_04_m_11, m.b_MemC.dMiss1_12_m_15, m.b_MemC.dMiss1_16_m_19,
                 m.b_MemC.dMiss2_04_m_11, m.b_MemC.dMiss2_12_m_15, m.b_MemC.dMiss2_16_m_19, m.b_MemC.dMiss3_04_m_11, m.b_MemC.dMiss3_12_m_15, m.b_MemC.dMiss3_16_m_19,
                 m.b_MemC.ForceMiss, m.b_MemC.DisCflags);
      end
      if ((~w_wrcf & ca_d_cf) != 4'b0000) begin
        n_caw = n_caw + 1;
        $display("tb_exec: CA @%0d CIA=%h WrCflags' fall col=%b ProcVA=%07h Hit'b=%b CRinA=%b VicInA=%b", n_cyc2, ring_cia,
                 ~w_wrcf & ca_d_cf, w_procva, m.b_MemC.Hit_p_b, m.b_MemC.CacheRefInA, m.b_MemC.VictimInA);
      end
    end
    ca_d_cva <= w_wrcva; ca_d_cf <= w_wrcf; ca_d_crina <= m.b_MemC.CacheRefInA;
  end
  // FORCEMISS: does the real boot load the MCR correctly? memrun-test never
  // does a LoadMcr at all, so MemC k08 (the MCR bit ForceDirtyMiss, clocked
  // by LdMcr') holds a STARTUP value there -- benign at baseline, stuck HIGH
  // under derived clock depth, which forces every cache reference to miss.
  // Initial DOES load the MCR, so the boot may be unaffected. Counted here to
  // decide whether memrun's depth failure is a bench artifact (2026-09-07).
  integer n_fdm = 0, n_fm = 0, n_hitd = 0, n_fdm_smp = 0;
  always @(posedge sys_clk) begin
    n_fdm_smp = n_fdm_smp + 1;
    if (m.b_MemC.ForceDirtyMiss)  n_fdm = n_fdm + 1;
    if (m.b_MemC.ForceMiss)       n_fm  = n_fm  + 1;
    if (m.b_MemC.MemC15_sil_pl_1) n_hitd = n_hitd + 1;
  end
  final $display("tb_exec: FORCEMISS -- ForceDirtyMiss %0d, ForceMiss %0d, hit-compare %0d, of %0d",
                 n_fdm, n_fm, n_hitd, n_fdm_smp);
  // THE MCR LOAD ITSELF. k08 (MC10176) clocks on the RISING edge of LdMcr'
  // -- the trailing edge of the active-low strobe -- and takes RMar_01 as
  // the ForceDirtyMiss bit. Printed per load so baseline and depth can be
  // compared: does the strobe fire, and what data does it see (2026-09-07)?
  integer n_ldm_rise = 0, n_ldm_r1 = 0; reg ldmx_d = 1'b1;
  always @(posedge sys_clk) begin
    if (!ldmx_d && m.b_MemC.LdMcr_p_) begin
      n_ldm_rise = n_ldm_rise + 1;
      if (m.b_MemC.RMar_01) n_ldm_r1 = n_ldm_r1 + 1;
      if (n_ldm_rise <= 8)
        $display("tb_exec: MCRLD #%0d @%0d RMar_01=%b -> ForceDirtyMiss=%b",
                 n_ldm_rise, n_cyc2, m.b_MemC.RMar_01, m.b_MemC.ForceDirtyMiss);
    end
    ldmx_d <= m.b_MemC.LdMcr_p_;
  end
  final $display("tb_exec: MCRLD -- LdMcr' rising edges %0d, RMar_01 set at %0d of them",
                 n_ldm_rise, n_ldm_r1);
  // DISK: what the world's disk boot did against the drive, and the Alto
  // boot's own milestones (AEmu.mb: DiskBoot 0x405, DiskBootRetry 0x412,
  // KWait 0x417, KBootTimeout 0x375, EBoot 0x406).
  integer dk_sectw = 0, dk_idxtw = 0, dk_active = 0, dk_rderr = 0, dk_fifow = 0, dk_data = 0, dk_ecc = 0, dk_rdderr = 0, dk_fovf = 0, dk_funf = 0;
  reg dk_sectw_d = 1'b0, dk_idxtw_d = 1'b0; reg [3:0] dk_fw_d = 4'd0;
  reg dk_active_d = 0;
  integer dk_nactive = 0;
  // Run options are immutable; don't scan argv on every fabric sample.
  reg disk_cmd_trace, disk_err_trace;
  initial begin
    disk_cmd_trace = $test$plusargs("diskcmdtrace");
    disk_err_trace = $test$plusargs("diskerrtrace");
  end
  wire [3:0] disk_subsector_reload = {m.b_DskEth.TriconD06_sil_pl_5,
    m.b_DskEth.TriconD06_sil_pl_6,m.b_DskEth.TriconD06_sil_pl_7,m.b_DskEth.TriconD06_sil_pl_8};
  reg [3:0] disk_subsector_prev=0;
  wire [15:0] disk_tag_value={m.b_DskEth.DriveTag,m.b_DskEth.CylinderTag,
    m.b_DskEth.HeadTag,m.b_DskEth.ControlTag,m.b_DskEth.Tag_000,m.b_DskEth.Tag_00,
    m.b_DskEth.Tag_0,m.b_DskEth.Tag_1,m.b_DskEth.Tag_2,m.b_DskEth.Tag_3,
    m.b_DskEth.Tag_4,m.b_DskEth.Tag_5,m.b_DskEth.Tag_6,m.b_DskEth.Tag_7,
    m.b_DskEth.Tag_8,m.b_DskEth.Tag_9};
  reg [15:0] disk_tag_prev=0;
  reg disk_clock_trace=0;
  initial disk_clock_trace=$test$plusargs("diskclocktrace");
  reg check_tioa_phase=0;
  reg [7:0] disk_tioa_prev=0;
  integer disk_tioa_changes=0;
  initial check_tioa_phase=$test$plusargs("check-tioa-phase");
  always @(posedge sys_clk) begin
    if(check_tioa_phase && ew_done && et_tioa!=disk_tioa_prev) begin
      disk_tioa_changes <= disk_tioa_changes+1;
      if(!m.b_DskEth.FHCP)
        $fatal(1,"TIOA changed %o -> %o while DskEth address latch was transparent @%0d",
          disk_tioa_prev,et_tioa,n_cyc2);
    end
    disk_tioa_prev <= et_tioa;
  end
  final if(check_tioa_phase) begin
    if(disk_tioa_changes==0) $error("TIOA phase probe observed no address changes");
    $display("tb_exec: TIOA-PHASE checked %0d address changes after world entry",disk_tioa_changes);
  end
  always @(posedge sys_clk) if(disk_clock_trace && n_cyc2>=300000 && n_cyc2<300064)
    $display("tb_exec: DISK-CLOCK @%0d mclk=%b MemSH'=%b ProcH-pre0'=%b post0'=%b ProcL-clk1'=%b Dsk-clk0=%b FHCP=%b pre1'=%b",
      n_cyc2,mclk,m.MemSH_p_,m.b_ProcH.PreClock0_p_B,m.b_ProcH.PostClock0_p_Bc,
      m.b_ProcL.Clock1_p_Ac,m.b_DskEth.Clk0,m.b_DskEth.FHCP,m.b_DskEth.PreClock1_p_Ca);
  reg disk_sec_prev=1;
  reg [35:0] disk_io_history[0:255];
  integer disk_io_pos=0;
  wire [35:0] disk_io_sample={el2_iob,et_tioa,
    m.b_DskEth.TtlTag_0,m.b_DskEth.TtlTag_1,m.b_DskEth.TtlTag_2,m.b_DskEth.TtlTag_3,
    m.b_DskEth.TagClock_p_,m.b_DskEth.Idle,m.b_DskEth.TtlDriveTag_p_,m.b_DskEth.TtlTag_4,
    m.b_DskEth.LdSubSector0_p_,m.b_DskEth.TIOA_eq_Us_p_,m.b_DskEth.IOout_p_,m.b_DskEth.IOin_p_};
  always @(posedge sys_clk) begin
    if(disk_cmd_trace && disk_tag_value!=disk_tag_prev)
      $display("tb_exec: DISK-TAG-LATCH @%0d CIA=%h task=%h %h -> %h IOB=%h TIOA=%o clock'=%b idle=%b out'=%b in'=%b",
        n_cyc2,cia_now,pct_ctask,disk_tag_prev,disk_tag_value,el2_iob,et_tioa,
        m.b_DskEth.TagClock_p_,m.b_DskEth.Idle,m.b_DskEth.IOout_p_,m.b_DskEth.IOin_p_);
    disk_tag_prev <= disk_tag_value;
    if(disk_cmd_trace) begin
      if(disk_subsector_prev==3 && disk_subsector_reload!=3) begin
        $display("tb_exec: RELOAD-HISTORY fields: IOB[35:20] TIOA[19:12] TagData[11:8] TagClock'/Idle/DriveTag'/Tag4/LoadSub'/EqUs'/IOout'/IOin'[7:0]");
        for(integer j=0;j<256;j=j+1)
          $display("tb_exec: RELOAD-HISTORY @%0d %09h",n_cyc2-256+j,disk_io_history[(disk_io_pos+j)%256]);
      end
      disk_io_history[disk_io_pos] <= disk_io_sample;
      disk_io_pos <= (disk_io_pos+1)%256;
    end
    if(disk_cmd_trace && drv_sec_n!=disk_sec_prev && u_drv.sector<5)
      $display("tb_exec: DISK-PULSE @%0d pulse=%0d line'=%b count=%h reload=%h OS=%b timer=%0d selected=%b",
        n_cyc2,u_drv.sector,drv_sec_n,m.b_DskEth.u_a01.q,disk_subsector_reload,
        m.b_DskEth.OS0,m.b_DskEth.u_a05.t1,!sel0_n);
    disk_sec_prev <= drv_sec_n;
    if(disk_cmd_trace && disk_subsector_reload != disk_subsector_prev)
      $display("tb_exec: DISK-RELOAD @%0d CIA=%h %h -> %h IOB=%h load'=%b drive'=%b tag4=%b",
        n_cyc2,cia_now,disk_subsector_prev,disk_subsector_reload,el2_iob,
        m.b_DskEth.LdSubSector0_p_,m.b_DskEth.TtlDriveTag_p_,m.b_DskEth.TtlTag_4);
    disk_subsector_prev <= disk_subsector_reload;
  end
  integer dk_t_diskboot = -1, dk_t_kwait = -1, dk_t_timeout = -1, dk_t_retry = -1, dk_n_retry = 0;
  // +bootio: observe the CPU's reads separately from FIFO refills. HM 9.4:
  // reading DISKDATA consumes OutReg, which may refill immediately afterwards.
  reg bootio = 0, bootio_clk_d = 0, bootio_in_d = 1, bootio_unf_d = 0;
  longint bootio_from = 249000000;
  integer bootio_n = 0, bootio_reads = 0;
  initial begin
    bootio = $test$plusargs("bootio");
    void'($value$plusargs("bootiofrom=%d", bootio_from));
  end
  wire [15:0] bootio_md = {m.b_ProcH.Md_00, m.b_ProcH.Md_01, m.b_ProcH.Md_02, m.b_ProcH.Md_03,
    m.b_ProcH.Md_04, m.b_ProcH.Md_05, m.b_ProcH.Md_06, m.b_ProcH.Md_07,
    m.b_ProcL.Md_08, m.b_ProcL.Md_09, m.b_ProcL.Md_10, m.b_ProcL.Md_11,
    m.b_ProcL.Md_12, m.b_ProcL.Md_13, m.b_ProcL.Md_14, m.b_ProcL.Md_15};
  function automatic [3:0] bootio_rev4(input [3:0] v);
    bootio_rev4 = {v[0],v[1],v[2],v[3]};
  endfunction
  function automatic [15:0] bootio_rm(input [7:0] a);
    // Both processor boards complement the RSTK nibble. ProcH receives
    // the true RBase nibble; ProcL uses RbAdr.0-3' (see h06/j06 pins7..10).
    // The halves therefore have different physical indices.
    bootio_rm = {bootio_rev4(m.b_ProcH.u_h06.mem[a ^ 8'h0f]),
      bootio_rev4(m.b_ProcH.u_j06.mem[a ^ 8'h0f]),
      bootio_rev4(m.b_ProcL.u_h06.mem[a ^ 8'hff]),
      bootio_rev4(m.b_ProcL.u_j06.mem[a ^ 8'hff])};
  endfunction
  // Optional read-only inspection of the actual register and map arrays.
  // Map indices are RAW DRAM row/column addresses, not guest page numbers.
  string inspect_path;
  longint inspect_at=-1;
  bit inspect_on=0,inspect_done=0;
  initial begin
    inspect_on=$value$plusargs("inspect-state=%s",inspect_path);
    void'($value$plusargs("inspect-at=%d",inspect_at));
  end
  task inspect_state;
    integer f;
    begin
      f=$fopen(inspect_path,"w");
      if(!f)$fatal(1,"Cannot open inspection file %s",inspect_path);
      $fwrite(f,"SAMPLE %0d CIA %h TASK %h\n",n_cyc2,ring_cia,pct_ctask);
      for(integer a=0;a<256;a++)$fwrite(f,"RM %02h %04h\n",a[7:0],bootio_rm(a[7:0]));
      $fwrite(f,"MAP-PLANES a04 a05 a06 a07 a08 a09 a10 a11 a12 a13 a14 d04 d05 d06 d07 d08 d09 d10 d11 d12 d13\n");
      for(integer a=0;a<16384;a++)$fwrite(f,"MAP %04h %021b\n",a[13:0],
        {m.b_MemX.u_a04.mem[a],m.b_MemX.u_a05.mem[a],m.b_MemX.u_a06.mem[a],
         m.b_MemX.u_a07.mem[a],m.b_MemX.u_a08.mem[a],m.b_MemX.u_a09.mem[a],
         m.b_MemX.u_a10.mem[a],m.b_MemX.u_a11.mem[a],m.b_MemX.u_a12.mem[a],
         m.b_MemX.u_a13.mem[a],m.b_MemX.u_a14.mem[a],m.b_MemX.u_d04.mem[a],
         m.b_MemX.u_d05.mem[a],m.b_MemX.u_d06.mem[a],m.b_MemX.u_d07.mem[a],
         m.b_MemX.u_d08.mem[a],m.b_MemX.u_d09.mem[a],m.b_MemX.u_d10.mem[a],
         m.b_MemX.u_d11.mem[a],m.b_MemX.u_d12.mem[a],m.b_MemX.u_d13.mem[a]});
      $fclose(f);
      inspect_done=1;
      $display("tb_exec: inspected real RM/map arrays at %0d to %s",n_cyc2,inspect_path);
    end
  endtask
  always @(negedge sys_clk)
    if(inspect_on && !inspect_done && inspect_at>=0 && n_cyc2>=inspect_at)inspect_state;
  final if(inspect_on && !inspect_done)inspect_state;
  // Bounded, passive fabric-cycle trace of Cedar's first map writes. Arm
  // after the loaded-world handoff: Initial uses these same IM addresses.
  bit map_probe=0,map_probe_loaded=0,map_probe_started=0;
  bit check_cedar_mapwrites=0;
  integer map_probe_n=0;
  integer cedar_mapwrites=0;
  longint map_probe_from=16000000;
  wire [17:0] map_probe_b={m.b_MemX.BMux_00,m.b_MemX.BMux_01,m.b_MemX.BMux_02,
    m.b_MemX.BMux_03,m.b_MemX.BMux_04,m.b_MemX.BMux_05,m.b_MemX.BMux_06,
    m.b_MemX.BMux_07,m.b_MemX.BMux_08,m.b_MemX.BMux_09,m.b_MemX.BMux_10,
    m.b_MemX.BMux_11,m.b_MemX.BMux_12,m.b_MemX.BMux_13,m.b_MemX.BMux_14,
    m.b_MemX.BMux_15,m.b_MemX.BMux_16,m.b_MemX.BMux_17};
  wire [17:0] map_probe_q={m.b_MemX.Mapbuf_00,m.b_MemX.Mapbuf_01,m.b_MemX.Mapbuf_02,
    m.b_MemX.Mapbuf_03,m.b_MemX.Mapbuf_04,m.b_MemX.Mapbuf_05,m.b_MemX.Mapbuf_06,
    m.b_MemX.Mapbuf_07,m.b_MemX.Mapbuf_08,m.b_MemX.Mapbuf_09,m.b_MemX.Mapbuf_10,
    m.b_MemX.Mapbuf_11,m.b_MemX.Mapbuf_12,m.b_MemX.Mapbuf_13,m.b_MemX.Mapbuf_14,
    m.b_MemX.Mapbuf_15,m.b_MemX.Mapbuf_16,m.b_MemX.Mapbuf_17};
  initial begin
    map_probe=$test$plusargs("mapprobe");
    check_cedar_mapwrites=$test$plusargs("check-cedar-mapwrites");
    if(check_cedar_mapwrites)map_probe=1;
    void'($value$plusargs("mapprobefrom=%d",map_probe_from));
  end
  // These sixteen RAM input planes are contiguous Mapbuf.00..15 in
  // MemX12/13. Check the DRAMs' actual sampled write data, independent of
  // the software register reader and the diagnostic display above.
  wire [15:0] cedar_map_written={m.b_MemX.u_a04.write_data,m.b_MemX.u_a05.write_data,
    m.b_MemX.u_a06.write_data,m.b_MemX.u_a07.write_data,m.b_MemX.u_a08.write_data,
    m.b_MemX.u_a09.write_data,m.b_MemX.u_a10.write_data,m.b_MemX.u_a11.write_data,
    m.b_MemX.u_a12.write_data,m.b_MemX.u_a13.write_data,m.b_MemX.u_a14.write_data,
    m.b_MemX.u_d04.write_data,m.b_MemX.u_d05.write_data,m.b_MemX.u_d06.write_data,
    m.b_MemX.u_d07.write_data,m.b_MemX.u_d08.write_data};
  always @(negedge sys_clk)
    if(check_cedar_mapwrites && map_probe_started && cedar_mapwrites<16 &&
       m.b_MemX.u_a04.write_pending)begin
      // Original InitMem's first loop installs page p at virtual page p.
      if(cedar_map_written!==cedar_mapwrites[15:0])
        $fatal(1,"Cedar map write%0d stored RP=%h, expected one-to-one page=%h",
          cedar_mapwrites,cedar_map_written,cedar_mapwrites[15:0]);
      cedar_mapwrites++;
      if(cedar_mapwrites==16)$display("CEDAR MAP WRITES PASS: first16 real page writes are one-to-one");
    end
  final if(check_cedar_mapwrites && cedar_mapwrites!=16)
    $error("Cedar map write gate incomplete: %0d/16 writes",cedar_mapwrites);
  always @(negedge sys_clk)begin
    if(map_probe && n_cyc2>=map_probe_from && pct_ctask==0 && ring_cia==12'h238)
      map_probe_loaded=1;
    if(map_probe_loaded && pct_ctask==0 && ring_cia==12'he70)
      map_probe_started=1;
    if(map_probe_started && map_probe_n<2048)begin
      map_probe_n++;
      $display("tb_exec: MAP-PROBE @%0d CIA=%h task=%h B=%h BMux=%h Mapbuf=%h expected=%h hold=%b rawhold=%b pair=%b pairclk=%b mapdec=%b c02clk=%b c02en=%b gate=%b lead=%b fh=%b c1=%b we=%b ras=%b cas=%b din=%b pending=%b addr=%h row=%h col=%h",
        n_cyc2,ring_cia,pct_ctask,brc_alub,map_probe_b,map_probe_q,bootio_rm(8'h4d),
        m.b_MemX.MemX20_sil_pl_3,m.HoldMapBuf,m.Map_u_InPair_p_,
        m.b_MemC.LdPair_p_,m.b_MemC.Map_u__p_,m.b_MemX.Clk1_p_Aa,m.b_MemX.u_c02.ck_en,
        m.b_MemX.MapbufLd_p_a,m.b_MemX.MapbufLd_p_a__lead,
        m.b_MemX.preFH_p_A,m.b_MemX.preClk1_p_A,m.b_MemX.RTMapWE_p_a,
        m.b_MemX.RTMapRAS_p_a,m.b_MemX.RTMapCAS_p_a,m.b_MemX.u_a04.p2,
        m.b_MemX.u_a04.write_pending,m.b_MemX.u_a04.addr,
        m.b_MemX.u_a04.row,m.b_MemX.u_a04.col);
    end
  end
  integer dmux_probe_n=0;
  bit dmux_probe_clk=0;
  always @(negedge sys_clk)begin
    if(map_probe_loaded && !map_probe_started && dmux_probe_n<256 &&
       (m.DMuxClk!=dmux_probe_clk || ring_cia==12'he5f || ring_cia==12'he57))begin
      dmux_probe_n++;
      $display("tb_exec: DMUX-PROBE @%0d CIA=%h clk=%b serial=%b data=%b input=%b dmdata=%b memx=%b addr=%b%b%b%b%b%b%b%b%b%b%b T=%h",
        n_cyc2,ring_cia,m.DMuxClk,m.b_ContA.DMuxData__l24_2,m.DMuxData,
        m.b_ProcH.DMuxData,m.b_ProcH.DMData,m.b_MemX.DMuxData__drv,
        m.b_MemX.DMadr_01__l21_3,m.b_MemX.DMadr_02,m.b_MemX.DMadr_03,
        m.b_MemX.DMadr_04,m.b_MemX.DMadr_05,m.b_MemX.DMadr_06,
        m.b_MemX.DMadr_07,m.b_MemX.DMadr_08,m.b_MemX.DMadr_09,
        m.b_MemX.DMadr_10,m.b_MemX.DMadr_11,brc_t);
    end
    dmux_probe_clk=m.DMuxClk;
  end
  // Observe pipe reads after the modeled clk1 edge, rather than at a MIR
  // change where operand muxes may still contain the preceding instruction.
  reg pipe_probe=0,pipe_probe_clk=0;
  integer pipe_probe_n=0;
  longint pipe_probe_from=49570000;
  initial begin
    pipe_probe=$test$plusargs("pipeprobe");
    void'($value$plusargs("pipeprobefrom=%d",pipe_probe_from));
  end
  always @(negedge sys_clk)begin
    if(pipe_probe && n_cyc2>=pipe_probe_from && pipe_probe_n<1024 &&
       !pipe_probe_clk && m.b_ContA.clk1_p_Ca &&
       pct_ctask==0 && (ring_cia>=12'hb00 && ring_cia<=12'hb3c))begin
      pipe_probe_n++;
      $display("tb_exec: PIPE-PROBE @%0d CIA=%h FF=%h T=%h A=%h B=%h ALU=%h MD=%h VA=%h pipeAddr=%b%b%b%b virtualBanks=%h ITemp17=%h Hold=%b",
        n_cyc2,ring_cia,ff_now,brc_t,brc_alua,brc_alub,brc_alu,bootio_md,w_procva,
        m.b_MemC.PipeAd_0,m.b_MemC.PipeAd_1,m.b_MemC.PipeAd_2,m.b_MemC.PipeAd_3,
        bootio_rm(8'h48),bootio_rm(8'h4f),m.Hold);
    end
    pipe_probe_clk=m.b_ContA.clk1_p_Ca;
  end
  always @(posedge sys_clk) begin
    if (bootio && n_cyc2 >= bootio_from) begin
      if (!bootio_clk_d && m.b_ContA.clk0_p_Ca && bootio_n < 1000000 &&
          (pct_ctask == 0 || pct_ctask == 12)) begin
        bootio_n = bootio_n + 1;
        $display("tb_exec: BOOTIO @%0d CIA=%h task=%h T=%h A=%h B=%h ALU=%h MD=%h VA=%h FF=%h Hold=%b lt'=%b eq'=%b IOB=%h TIOA=%o full=%b empty=%b ETemp0=%h ETemp1=%h KStatus=%h",
          n_cyc2, ring_cia, pct_ctask, brc_t, brc_alua, brc_alub, brc_alu,
          bootio_md, w_procva, ff_now, m.b_ContA.RepeatCur,
          m.b_ProcH.ResLtZero_p_, m.b_ProcH.ResEqZero_p_, el2_iob, et_tioa,
          m.b_DskEth.OutRegFull, m.b_DskEth.FifoEmpty,
          bootio_rm(8'h1a),bootio_rm(8'h1b),bootio_rm(8'h5a));
      end
      if (bootio_in_d && !m.b_DskEth.bIOin_p_ && et_tioa == 8'o12) begin
        bootio_reads = bootio_reads + 1;
        $display("tb_exec: DISK-INPUT #%0d @%0d CIA=%h task=%h data=%h IOB=%h full=%b empty=%b Active=%b",
          bootio_reads,n_cyc2,ring_cia,pct_ctask,dk_word,el2_iob,
          m.b_DskEth.OutRegFull,m.b_DskEth.FifoEmpty,m.b_DskEth.Active);
      end
      if (!bootio_unf_d && m.b_DskEth.FifoUnderflow)
        $display("tb_exec: FIRST-UNDERFLOW @%0d CIA=%h task=%h data=%h TIOA=%o full=%b empty=%b Active=%b",
          n_cyc2,ring_cia,pct_ctask,dk_word,et_tioa,
          m.b_DskEth.OutRegFull,m.b_DskEth.FifoEmpty,m.b_DskEth.Active);
    end
    bootio_clk_d <= m.b_ContA.clk0_p_Ca;
    bootio_in_d <= m.b_DskEth.bIOin_p_;
    bootio_unf_d <= m.b_DskEth.FifoUnderflow;
  end
  reg [11:0] dk_cia_d = 12'hfff;
  always @(posedge sys_clk) begin
    if (disk_cmd_trace && m.b_DskEth.Active && !dk_active_d && dk_nactive < 40) begin
      dk_nactive = dk_nactive + 1;
      $display("tb_exec: DISK-ACTIVE @%0d pulse=%0d angle=%0d divider=%h media-phase=%0d", n_cyc2,
        u_drv.sector, u_drv.sec_count, m.b_DskEth.u_a01.q, u_pack.phase);
    end
    dk_active_d <= m.b_DskEth.Active;
    if (m.b_DskEth.SectorTW && !dk_sectw_d) dk_sectw = dk_sectw + 1; dk_sectw_d <= m.b_DskEth.SectorTW;
    if (m.b_DskEth.IndexTW  && !dk_idxtw_d) dk_idxtw = dk_idxtw + 1; dk_idxtw_d <= m.b_DskEth.IndexTW;
    if (m.b_DskEth.Active)    dk_active = dk_active + 1;
    if (m.b_DskEth.ReadError) dk_rderr = dk_rderr + 1;
    if (m.b_DskEth.ReadDataErr)   dk_rdderr = dk_rdderr + 1;
    if (m.b_DskEth.FifoOverflow)  dk_fovf = dk_fovf + 1;
    if (m.b_DskEth.FifoUnderflow) dk_funf = dk_funf + 1;
    if ({m.b_DskEth.FifoWaddr_0, m.b_DskEth.FifoWaddr_1, m.b_DskEth.FifoWaddr_2, m.b_DskEth.FifoWaddr_3} != dk_fw_d) dk_fifow = dk_fifow + 1;
    dk_fw_d <= {m.b_DskEth.FifoWaddr_0, m.b_DskEth.FifoWaddr_1, m.b_DskEth.FifoWaddr_2, m.b_DskEth.FifoWaddr_3};
    if (cia_now != dk_cia_d) begin
      if (cia_now == 12'h405 && dk_t_diskboot < 0) dk_t_diskboot = n_cyc2;
      if (cia_now == 12'h412) begin dk_n_retry = dk_n_retry + 1; if (dk_t_retry < 0) dk_t_retry = n_cyc2; end
      if (cia_now == 12'h417 && dk_t_kwait < 0) dk_t_kwait = n_cyc2;
      if (cia_now == 12'h375 && dk_t_timeout < 0) dk_t_timeout = n_cyc2;
    end
    dk_cia_d <= cia_now;
  end
  // THE WORDS THE CONTROLLER HANDS THE PROCESSOR. b11's OutRegWrite' loads
  // DskData from the FIFO; each falling edge is one word delivered, and the
  // first few tell whether the pack's header (0000 0000), label (007e ...)
  // and data (8530 4126 ...) arrived intact, or what arrived instead.
  reg dk_orw_d = 1'b1; integer dk_nwords = 0;
  wire [15:0] dk_word = {m.b_DskEth.DskData_00, m.b_DskEth.DskData_01, m.b_DskEth.DskData_02, m.b_DskEth.DskData_03,
                         m.b_DskEth.DskData_04, m.b_DskEth.DskData_05, m.b_DskEth.DskData_06, m.b_DskEth.DskData_07,
                         m.b_DskEth.DskData_08, m.b_DskEth.DskData_09, m.b_DskEth.DskData_10, m.b_DskEth.DskData_11,
                         m.b_DskEth.DskData_12, m.b_DskEth.DskData_13, m.b_DskEth.DskData_14, m.b_DskEth.DskData_15};
  always @(posedge sys_clk) begin
    if (dk_orw_d && !m.b_DskEth.OutRegWrite_p_) begin
      dk_nwords = dk_nwords + 1;
      if (dk_nwords <= 24 || (dk_nwords % 256) == 0)
        $display("tb_exec: DSKDATA #%0d @%0d = %04h (drive at cyl %0d head %0d, pulse %0d)", dk_nwords, n_cyc2, dk_word, pk_cyl, pk_head, u_drv.sector);
    end
    dk_orw_d <= m.b_DskEth.OutRegWrite_p_;
  end
  final $display("tb_exec: DSKDATA -- %0d words delivered to the processor", dk_nwords);
  final begin
    if (pack_on)
      $display("tb_exec: PACK -- %s: tags cyl %0d head %0d drive %0d control %0d, drive at cyl %0d head %0d, tracks loaded %0d, sectors served %0d, blocks %0d, bits %0d",
               pack_path, pk_nct, pk_nht, pk_ndt, pk_nkt, pk_cyl, pk_head, pk_ntr, pk_nse, pk_nbl, pk_nbi);
    $display("tb_exec: DISK -- SectorTW rises %0d, IndexTW rises %0d, Active high %0d, ReadError high %0d (ReadDataErr %0d, FifoOverflow %0d, FifoUnderflow %0d), FIFO write-address changes %0d | DiskBoot first @%0d, DiskBootRetry first @%0d (%0d entries), KWait first @%0d, KBootTimeout first @%0d, of %0d",
             dk_sectw, dk_idxtw, dk_active, dk_rderr, dk_rdderr, dk_fovf, dk_funf, dk_fifow, dk_t_diskboot, dk_t_retry, dk_n_retry, dk_t_kwait, dk_t_timeout, n_cyc2);
  end
`endif
`ifdef FULL
  // ETHLOG2: change-only log of DskEth's EOT wakeup chain, armed by the first
  // Output strobe to EControl (Initial's ResetEther). Bounded.
  reg [16:0] el2, el2_d = 17'bx; integer n_el2 = 0, n_dtag = 0; reg el2_armed = 1'b0;
  wire [15:0] el2_iob = {m.b_DskEth.bIOB_00, m.b_DskEth.bIOB_01, m.b_DskEth.bIOB_02, m.b_DskEth.bIOB_03,
                         m.b_DskEth.bIOB_04, m.b_DskEth.bIOB_05, m.b_DskEth.bIOB_06, m.b_DskEth.bIOB_07,
                         m.b_DskEth.bIOB_08, m.b_DskEth.bIOB_09, m.b_DskEth.bIOB_10, m.b_DskEth.bIOB_11,
                         m.b_DskEth.bIOB_12, m.b_DskEth.bIOB_13, m.b_DskEth.bIOB_14, m.b_DskEth.bIOB_15};
  always @(posedge sys_clk) begin
    if (m.b_ProcL.IOBout && !et_iobout_d && et_tioa == 8'o16) begin
      el2_armed = 1'b1;
      if (n_el2 < 600) $display("tb_exec: ETHLOG2 %0d: EControl <- %04h (task %0d)", n_cyc2, el2_iob, pct_ctask);
    end
    if (m.b_ProcL.IOBout && !et_iobout_d && et_tioa == 8'o14 && n_dtag < 80) begin
      n_dtag = n_dtag + 1;
      $display("tb_exec: DISKTAG <- %04h (task %0d) @%0d", el2_iob, pct_ctask, n_cyc2);
    end
    if (disk_cmd_trace && m.b_ProcL.IOBout && !et_iobout_d && et_tioa == 8'o10)
      $display("tb_exec: DISK-CONTROL <- %04h (task %0d) @%0d pulse=%0d angle=%0d",
        el2_iob, pct_ctask, n_cyc2, u_drv.sector, u_drv.sec_count);
    if (m.b_ProcL.IOBout && !et_iobout_d && et_tioa == 8'o15 && n_el2 < 600)
      $display("tb_exec: ETHLOG2 %0d: EData <- %04h (task %0d)", n_cyc2, el2_iob, pct_ctask);
    el2 = {m.b_DskEth.TxOn, m.b_DskEth.RxOn, m.b_DskEth.Ether06_sil_pl_5, m.TWReq_06, m.TWReq_07,
           m.b_DskEth.Curr_eq_EthTx, m.b_DskEth.Next_eq_EthTx_x3f_, m.b_DskEth.Prev_eq_EthTx,
           m.b_DskEth.Blocked, m.b_DskEth.Ether06_sil_pl_2__h04_14, m.b_DskEth.Ether06_sil_pl_2__l06_14,
           m.b_DskEth.Ether06_sil_pl_2__j04_15, m.b_DskEth.Ether06_sil_pl_2__l03_15, m.b_DskEth.NoWakeups,
           pct_ctask == 4'd6, pct_ctask == 4'd7, m.b_DskEth.bIOReset};
    if (el2_armed && el2 !== el2_d && n_el2 < 600) begin
      $display("tb_exec: ETHLOG2 %0d: TxOn=%b RxOn=%b wakeD=%b TWReq06=%b TWReq07=%b Curr=EthTx %b Next=EthTx? %b Prev=EthTx %b Blocked=%b h04'=%b l06=%b j04=%b l03=%b NoWakeups=%b t6=%b t7=%b bIOReset=%b ctask=%0d",
               n_cyc2, el2[16], el2[15], el2[14], el2[13], el2[12], el2[11], el2[10], el2[9], el2[8], el2[7], el2[6], el2[5], el2[4], el2[3], el2[2], el2[1], el2[0], pct_ctask);
      n_el2 = n_el2 + 1;
    end
    el2_d <= el2;
  end
`endif
`ifdef FULL
  // Passive receive visibility, including the status actually delivered to
  // the FIFO. This does not alter wakeups, completion or error reporting.
  integer rxtrace_words=0,rxtrace_packets=0;
  reg rxtrace_we=1,rxtrace_seen_carrier=0;
  wire [15:0] rxtrace_word={m.b_DskEth.RxSR_00,m.b_DskEth.RxSR_01,
    m.b_DskEth.RxSR_02,m.b_DskEth.RxSR_03,m.b_DskEth.RxSR_04,m.b_DskEth.RxSR_05,
    m.b_DskEth.RxSR_06,m.b_DskEth.RxSR_07,m.b_DskEth.RxSR_08,m.b_DskEth.RxSR_09,
    m.b_DskEth.RxSR_10,m.b_DskEth.RxSR_11,m.b_DskEth.RxSR_12,m.b_DskEth.RxSR_13,
    m.b_DskEth.RxSR_14,m.b_DskEth.RxSR_15};
  always @(negedge sys_clk) if($test$plusargs("rxtrace") && paritysources_arm)begin
    if(m.b_DskEth.PDCarrier)rxtrace_seen_carrier=1;
    if(rxtrace_we && !m.b_DskEth.RxFifoWE_p_ && rxtrace_seen_carrier)begin
      if(rxtrace_words<40 || m.b_DskEth.RxSR_18)
        $display("tb_exec: RXTRACE sample=%0d packet=%0d word=%0d data=%04h EOP=%b full=%b task=%0d",
          n_cyc2,rxtrace_packets,rxtrace_words,rxtrace_word,m.b_DskEth.RxSR_18,m.b_DskEth.RxFifoFull,pct_ctask);
      if(m.b_DskEth.RxSR_18)begin rxtrace_packets++;rxtrace_words=0;end
      else rxtrace_words++;
    end
    rxtrace_we=m.b_DskEth.RxFifoWE_p_;
  end
  integer netbus_events=0;
  reg netbus_in_d=1,netbus_we_d=1;
  reg [63:0] netbus_history[0:31];
  integer netbus_history_pos=0;
  bit netbus_history_dumped=0;
  wire [15:0] netbus_register={m.b_DskEth.u_k14.q[0],m.b_DskEth.u_k14.q[1],
    m.b_DskEth.u_k14.q[2],m.b_DskEth.u_k14.q[3],m.b_DskEth.u_k14.q[4],m.b_DskEth.u_k14.q[5],
    m.b_DskEth.u_k15.q[0],m.b_DskEth.u_k15.q[1],m.b_DskEth.u_k15.q[2],
    m.b_DskEth.u_k15.q[3],m.b_DskEth.u_k15.q[4],m.b_DskEth.u_k15.q[5],
    m.b_DskEth.u_k16.q[0],m.b_DskEth.u_k16.q[1],m.b_DskEth.u_k16.q[2],m.b_DskEth.u_k16.q[3]};
  always @(negedge sys_clk) if($test$plusargs("netbus-trace") && paritysources_arm)begin
    netbus_history[netbus_history_pos]={n_cyc2[23:0],cia_now,netbus_register,
      m.b_DskEth.Clock1_p_Bb,m.b_DskEth.PreClock1_p_D,m.b_DskEth.TxBusRegClk_p_,
      m.b_DskEth.TxBusRegFull_p_,m.b_DskEth.u_l06.ck_en,m.b_DskEth.u_l06.p10,
      m.b_DskEth.u_l06.p11,m.b_DskEth.u_l06.jk_pre[3:2],
      m.b_DskEth.bIOout_p_,m.b_DskEth.TIOA_eq_EthData_p_,m.b_DskEth.TxWriteFifo_p_};
    if(m.b_DskEth.u_k14.ck_en && el2_armed && !netbus_history_dumped)begin
      for(integer j=1;j<=32;j++)$display("tb_exec: NETBUS-HISTORY %016h",netbus_history[(netbus_history_pos+j)%32]);
      netbus_history_dumped=1;
    end
    netbus_history_pos=(netbus_history_pos+1)%32;
    if(netbus_events<128 && (m.b_DskEth.u_k14.ck_en ||
       (netbus_we_d && !m.b_DskEth.TxWriteFifo_p_)))begin
      $display("tb_exec: NETBUS sample=%0d CIA=%h task=%0d en=%b IOB=%04h olddata=%04h reg=%04h MD=%04h WE'=%b addr=%b%b%b%b full'=%b",
        n_cyc2,cia_now,pct_ctask,m.b_DskEth.u_k14.ck_en,el2_iob,
        {m.b_DskEth.u_k14.d_pre[0],m.b_DskEth.u_k14.d_pre[1],m.b_DskEth.u_k14.d_pre[2],
         m.b_DskEth.u_k14.d_pre[3],m.b_DskEth.u_k14.d_pre[4],m.b_DskEth.u_k14.d_pre[5],
         m.b_DskEth.u_k15.d_pre[0],m.b_DskEth.u_k15.d_pre[1],m.b_DskEth.u_k15.d_pre[2],
         m.b_DskEth.u_k15.d_pre[3],m.b_DskEth.u_k15.d_pre[4],m.b_DskEth.u_k15.d_pre[5],
         m.b_DskEth.u_k16.d_pre[0],m.b_DskEth.u_k16.d_pre[1],m.b_DskEth.u_k16.d_pre[2],m.b_DskEth.u_k16.d_pre[3]},
        netbus_register,bootio_md,m.b_DskEth.TxWriteFifo_p_,
        m.b_DskEth.TxFifoAd_0,m.b_DskEth.TxFifoAd_1,m.b_DskEth.TxFifoAd_2,m.b_DskEth.TxFifoAd_3,
        m.b_DskEth.TxBusRegFull_p_);
      netbus_events++;
    end
    netbus_we_d=m.b_DskEth.TxWriteFifo_p_;
  end
  // +rcvplay=PATH: replay a phase-encoded reply stream (tools/eb_replies.py)
  // onto RcvData, starting +rcvdelay sys_clk (default 500,000 = 0.94 ms)
  // after the machine's own transmitter has sent a packet and gone idle --
  // the shape of a boot server answering the MicrocodeBootRequest.
  integer rp_fd, rp_n = 0, rp_i = 0, rp_delay = 500000, rp_start = -1, rp_lastx = 0, rp_code;
  // Cedar's original microcode needs 444,292 transitions; the old 400K
  // limit silently discarded its tail, including the end-of-file reply.
  localparam integer REPLAY_TRANSITIONS=1000000;
  integer rp_t [0:REPLAY_TRANSITIONS-1]; reg rp_v [0:REPLAY_TRANSITIONS-1];
  reg rp_line [0:0]; string rp_path; reg [255:0] rp_buf; integer rp_tt, rp_vv;
  initial begin
    if ($value$plusargs("rcvplay=%s", rp_path)) begin
      rp_fd = $fopen(rp_path, "r");
      if (rp_fd == 0) $fatal(1, "rcvplay: cannot open %s", rp_path);
      // $fscanf, not $fgets+$sscanf: the file is pure "<offset> <level>" pairs
      // (tools/eb_replies.py puts its packet index in a sidecar), and the
      // $fgets form silently loaded NOTHING -- 0 transitions -- in the first
      // boot run that armed this.
      while (rp_n < REPLAY_TRANSITIONS) begin
        rp_code = $fscanf(rp_fd, "%d %d", rp_tt, rp_vv);
        if (rp_code != 2) break;
        rp_t[rp_n] = rp_tt; rp_v[rp_n] = rp_vv[0]; rp_n = rp_n + 1;
      end
      if(rp_n==REPLAY_TRANSITIONS && $fscanf(rp_fd,"%d %d",rp_tt,rp_vv)==2)
        $fatal(1,"rcvplay: reply stream exceeds capacity; refusing a truncated boot image");
      $fclose(rp_fd);
      void'($value$plusargs("rcvdelay=%d", rp_delay));
      $display("tb_exec: +rcvplay -- %0d transitions loaded from %s, delay %0d after the request", rp_n, rp_path, rp_delay);
    end
  end
  always @(posedge sys_clk) begin
    if (eth_xmt != et_xmt_d) rp_lastx = n_cyc2;
    if (rp_n > 0 && rp_start < 0 && et_xmt >= 100 && n_cyc2 - rp_lastx > 4000) begin
      rp_start = n_cyc2 + rp_delay;
      $display("tb_exec: +rcvplay -- request seen (%0d transitions), reply stream starts at cycle %0d", et_xmt, rp_start);
    end
    if (rp_start >= 0 && rp_i < rp_n && n_cyc2 - rp_start >= rp_t[rp_i]) begin
      rcv_r <= rp_v[rp_i]; rp_i = rp_i + 1;
      if (rp_i == rp_n) $display("tb_exec: +rcvplay -- stream finished at cycle %0d", n_cyc2);
    end
  end
`endif
  final $display("tb_exec: ETH -- XmtData' transitions %0d, TWReq.06 (tx) high %0d, TWReq.07 (rx) high %0d, Output strobes to EData(015) %0d, to EControl(016) %0d, of %0d",
                 et_xmt, et_tw06, et_tw07, et_out15, et_out16, n_cyc2);
  // `+xmtrec=PATH`: every transition of XmtData' with its sys_clk time, so a
  // transmitted packet's bit stream can be decoded offline (the phase
  // encoder's own format is the reference for a bench-side responder).
  integer xmt_fd = 0, xmt_n = 0;
  initial begin : xmtrec
    reg [1023:0] xpath;
    if ($value$plusargs("xmtrec=%s", xpath)) xmt_fd = $fopen(xpath, "w");
  end
  always @(posedge sys_clk)
    if (xmt_fd != 0 && eth_xmt != et_xmt_d && xmt_n < 200000) begin
      $fwrite(xmt_fd, "%0d %b\n", n_cyc2, eth_xmt);
      xmt_n = xmt_n + 1;
    end
  final if (xmt_fd != 0) begin $fclose(xmt_fd); $display("tb_exec: XMTREC -- %0d transitions recorded", xmt_n); end
`endif
  // `+hramdump`: the three HRam planes (h14 HSync, i14 HBlank, j14 HalfLine,
  // F10415A 1Kx1 each) as run lengths over the first 400 entries, plus the
  // total set bits. InitialDisplay's table for an Alto monitor is 34 x 6,
  // 10 x 2, 145 x 0, 35 x 1, 148 x 0, 7 x 2, 1 x 6 = 380 entries.
  final if ($test$plusargs("hramdump")) begin : hramdump
    integer a, run, n1, n2, n3; reg [2:0] cur, prev;
    n1 = 0; n2 = 0; n3 = 0;
    for (a = 0; a < 1024; a = a + 1) begin
      if (m.b_DispY.u_h14.mem[a]) n1 = n1 + 1;
      if (m.b_DispY.u_i14.mem[a]) n2 = n2 + 1;
      if (m.b_DispY.u_j14.mem[a]) n3 = n3 + 1;
    end
    $display("tb_exec: HRAM -- set bits: HSync plane %0d, HBlank plane %0d, HalfLine plane %0d (of 1024)", n1, n2, n3);
    $write("tb_exec: HRAM -- runs of {HSync,HBlank,HalfLine} over 0..399:");
    prev = {m.b_DispY.u_h14.mem[0], m.b_DispY.u_i14.mem[0], m.b_DispY.u_j14.mem[0]}; run = 0;
    for (a = 0; a < 400; a = a + 1) begin
      cur = {m.b_DispY.u_h14.mem[a], m.b_DispY.u_i14.mem[a], m.b_DispY.u_j14.mem[a]};
      if (cur !== prev) begin $write(" %0dx%0d", prev, run); prev = cur; run = 0; end
      run = run + 1;
    end
    $write(" %0dx%0d\n", prev, run);
  end
`endif
  // `+pcpc=HEX` opens the PCT window at the first MIR load of that address
  // (an address is a better handle than a cycle number in a 150M-cycle boot).
  reg [11:0] pcpc = 12'hfff;
  // Read-only verification of what Initial received and LoadRam installed.
  // No target data is copied into the machine. LoadRam's own page is protected.
  final if ($test$plusargs("loaded-eb")) begin : verify_loaded_eb
    string path;
    integer fd, c, a, l, r, hi, lo, count, bad, start, ifucount;
    reg [3:0] rstk, aluf;
    reg [2:0] bsel, lc, asel;
    reg [7:0] ff, jcn;
    reg block_n, lp, rp;
    reg [15:0] ifulo, ifuhi;
    void'($value$plusargs("loaded-eb=%s",path));
    fd=$fopen(path,"rb");
    if(!fd) $fatal(1,"loaded-eb: cannot read %s",path);
    c=$fseek(fd,512,0); count=0; bad=0; start=-1; ifucount=0;
    while(!$feof(fd)) begin
      hi=$fgetc(fd); lo=$fgetc(fd); if(hi<0 || lo<0) break; c=(hi<<8)|lo;
      hi=$fgetc(fd); lo=$fgetc(fd); a=(hi<<8)|lo;
      hi=$fgetc(fd); lo=$fgetc(fd); l=(hi<<8)|lo;
      hi=$fgetc(fd); lo=$fgetc(fd); r=(hi<<8)|lo;
      if(lo<0) $fatal(1,"loaded-eb: truncated item");
      if((c&4095)==2) begin start=r; break; end
      if((c&4095)==1) begin
        ifum_readback_word(a[9:0],ifulo,ifuhi); ifucount++;
        if(ifulo !== l[15:0] || ifuhi !== r[15:0]) begin
          if(bad<8) $display("loaded-eb: IFUM %03h got %04h/%04h expected %04h/%04h",a,ifulo,ifuhi,l,r);
          bad++;
        end
        continue;
      end
      if((c&4095)!=0 || (a>=12'hf80 && a<=12'hfbf)) continue;
      im_readback_word(a[11:0],rstk,aluf,bsel,lc,asel,ff,jcn,block_n,lp,rp);
      count++;
      if(rstk !== {c[14],l[15:13]} || aluf !== l[12:9] ||
         bsel !== l[8:6] || lc !== l[5:3] || asel !== l[2:0] ||
         ff !== r[15:8] || jcn !== r[7:0] || block_n !== ~c[12]) begin
        if(bad<8) $display("loaded-eb: IM %03h differs from original item %04h %04h %04h",a,c,l,r);
        bad++;
      end
    end
    $fclose(fd);
    if(count<1000 || bad || start<0 || !visited[start&4095])
      $fatal(1,"loaded-eb: words=%0d bad=%0d entry=%h reached=%b",count,bad,start,start>=0 ? visited[start&4095] : 1'b0);
    $display("LOADED EB PASS: %0d original IM and %0d IFUM items match, protected loader page excluded, entry %03h executed",count,ifucount,start);
  end
  initial void'($value$plusargs("pcpc=%h", pcpc));
  always @(posedge sys_clk)
    if (pcpc != 12'hfff && ring_cia == pcpc && ring_cia != pct_cia_d) begin
      pcfrom = n_cyc2; pcpc = 12'hfff;
      $display("tb_exec: PCT window opens @%0d at CIA=%h", n_cyc2, ring_cia);
    end
  always @(posedge sys_clk) begin
    if (((pcfrom >= 0 && n_cyc2 >= pcfrom && n_cyc2 < pcfrom + pclen) ||
         (pctask >= 0 && pct_ctask == pctask[3:0])) && ring_cia != pct_cia_d)
      $display("tb_exec: PCT @%0d CIA=%h JCN=%h task=%h Link=%h T=%h R=%h alu=%h ResLt0'=%b ResEq0'=%b SB=%b Cout=%b ALUCarry=%b TPCI=%h CTD=%h TaskOff'=%b Ph=%b SC=%b RW=%b Hold=%b",
               n_cyc2, ring_cia, ring_jcn, pct_ctask, ring_link, brc_t, brc_r, brc_alu,
               m.b_ProcH.ResLtZero_p_, m.b_ProcH.ResEqZero_p_, m.b_ProcH.ProcH11_sil_pl_9,
               m.b_ProcH.aluCout, m.b_ProcH.ALUCarry,
               ring_tpci, ring_ctd, m.b_ContA.TaskingIsOff_p_, ring_ph, m.b_ContA.StartCycle,
               m.b_ContA.RWTPCorRWIM, m.b_ContA.RepeatCur);
    pct_cia_d <= ring_cia;
  end

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
  string expect_im_path;
  reg [31:0] ia, ib, ic, id, ie, ig, ih, ii, ij, ilh, irh;
  reg p_lh, p_rh, impar_odd, impar_nosec;
  integer imfd, imn, mapaddr, nloaded, nver, nverbad, startaddr;
  longint runcycles;   // 64-bit: a full boot is >2^31 sys_clk
  string cycle_limit_file;
  bit has_cycle_limit_file = 0;
  integer cycle_limit_fd, cycle_limit_rc;
  longint requested_cycle_limit;
  integer mcrval;
  reg [7:0] mb0_, mb1_, mb2_, mb3_, mb4_;
  integer nalufm, nalufmbad, nifum, nifumbad;
  reg [5:0]  r_alu;
  reg [15:0] r_lo, r_hi, ifum_want;  reg ifum_raw;
`ifdef WORLD
  reg world_ifum_have [0:1023];
  reg [15:0] world_ifum_lo [0:1023], world_ifum_hi [0:1023];
  reg world_alufm_have [0:15];
  reg [5:0] world_alufm [0:15];
  task automatic restore_world_tables;
    integer a, ni, na;
    reg [15:0] lo, hi, old_lo, old_hi;
    reg [5:0] alu;
    begin
      ni=0; na=0;
      ifum_readback_word(0,old_lo,old_hi);
      for(a=0;a<1024;a=a+1) if(world_ifum_have[a]) begin
        ifum_preload_word(a,world_ifum_lo[a],world_ifum_hi[a]);
        ifum_readback_word(a,lo,hi);
        if(lo[10:0]!==world_ifum_lo[a][10:0] || hi!==world_ifum_hi[a])
          $fatal(1,"world handoff IFUM[%h] readback mismatch",a);
        ni=ni+1;
      end
      for(a=0;a<16;a=a+1) if(world_alufm_have[a]) begin
        alufm_preload_word(a,world_alufm[a]);
        alufm_readback_word(a,alu);
        if(alu!==world_alufm[a]) $fatal(1,"world handoff ALUFM[%h] mismatch",a);
        na=na+1;
      end
      $display("tb_exec: WORLD-TABLES @%0d restored/read back %0d IFUM and %0d ALUFM entries; IFUM[0] before=%h/%h after=%h/%h",
        n_cyc2,ni,na,old_lo,old_hi,world_ifum_lo[0],world_ifum_hi[0]);
    end
  endtask
`endif
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
  integer n_repp = 0, n_swup = 0, n_cah20 = 0, n_cahp2 = 0, n_hold2 = 0;
  integer n_hx2a_held = 0, n_tnia_held = 0; reg d_hx2a = 1'b0; reg [11:0] d_tnia_h = 12'h0;
  integer ph_hold [0:15];
  integer n_hgap = 0, gap_run = 0, gap_max = 0, gap_last = 0; reg d_hold3 = 1'b0;
  initial for (integer pi = 0; pi < 16; pi = pi + 1) ph_hold[pi] = 0;
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
  integer trwin = 0;
  initial void'($value$plusargs("trwin=%d", trwin));
  integer fastwait, n_clip = 0;
  reg clip_wait;
  initial begin
    fastwait = $test$plusargs("fastwait");
    clip_wait = $test$plusargs("clip");
  end
  // ---- FAST-FORWARD THE WAITS ---------------------------------------------
  //
  // Initial's LongWait ("wait T+2 cycles") is real time on real hardware --
  // milliseconds of DRAM settling -- which at SYSPER=16 is millions of
  // sys_clk PER CALL, and it is called repeatedly. T was PROVEN to decrement
  // correctly (TWR: 1578, 1577, 1576... one per iteration), so nothing is
  // learned by simulating the whole countdown. `+fastwait` clips T to 4
  // whenever the machine is spinning at LWRETN with a large count. The T
  // file is per-task: ProcH l03/l04 hold bits 0-7, ProcL l03/l04 bits 8-15,
  // and task 0's slot is 0xf (the address pins are primed, like TPC).
  reg fw_said = 1'b0;
  always @(posedge sys_clk)
    if (fastwait && tnia_now == 12'hc0a && !fw_said) begin
      fw_said <= 1'b1;
      $display("tb_exec: FASTWAIT sees c0a: T nibbles H={%h,%h} L={%h,%h}",
               m.b_ProcH.u_l03.mem[4'hf], m.b_ProcH.u_l04.mem[4'hf],
               m.b_ProcL.u_l03.mem[4'hf], m.b_ProcL.u_l04.mem[4'hf]);
    end
  // THE CLIP IS OFF (2026-09-01): it was DESTROYING the SetMCR data. The
  // clip fired on every sys_clk while TNIA read c0a with T > 0x10 -- and the
  // second LoadMCR (InitialSubrs' SetMCR, the noRef family) executes exactly
  // in that window, its MCR value in T (0x67 > 0x10), A<-T. Measured with the
  // full RMar vector: during the LoadMCR's enabled phase the Mar carries all
  // zeros -- T had already been clipped to 4 -- which is the whole of "LoadMCR
  // #2 misses its data" (28ba542f): a bench artifact, not a strobe-timing
  // bug. The ResEqZero pulse below is what actually exits LongWait (the clip
  // "loses to the bypass" and never could); it stays, the clip goes. `+clip`
  // turns it back on to reproduce the artifact.
  always @(posedge sys_clk)
    if (fastwait && clip_wait
        && (tnia_now == 12'hc40 || tnia_now == 12'hc0a)
        && {m.b_ProcL.u_l03.mem[4'hf], m.b_ProcL.u_l04.mem[4'hf]} > 8'h10) begin
      m.b_ProcH.u_l03.mem[4'hf] = 4'h0;  // T.00-03
      m.b_ProcH.u_l04.mem[4'hf] = 4'h0;  // T.04-07
      m.b_ProcL.u_l03.mem[4'hf] = 4'h0;  // T.08-11
      m.b_ProcL.u_l04.mem[4'hf] = 4'h4;  // T.12-15 -> T = 0x0004
      n_clip = n_clip + 1;
    end
  // The file clip cannot beat the BYPASS (the loop forwards its own
  // decrement), so the wait is exited by its CONDITION instead: while the
  // machine sits at LWRETN, pulse the latched ALU=0 the branch tests.
  // Zero-true sense is LOW (measured at the checksum: ResEq0'=0 captured
  // the XOR's zero). Held a few cycles, released, with a gap so each
  // LongWait call exits once rather than every branch mis-taking.
  integer fw_ph = 0;
  always @(posedge sys_clk) begin
    if (fastwait && tnia_now == 12'hc0a) begin
      fw_ph = fw_ph + 1;
      if (fw_ph == 64)  force m.ResEqZero_p_ = 1'b0;
      if (fw_ph == 128) begin release m.ResEqZero_p_; fw_ph = 0; end
    end else if (fw_ph != 0 && tnia_now != 12'hc0a) begin
      release m.ResEqZero_p_; fw_ph = 0;
    end
  end

  // EVERY T-FILE WRITE: strobe = ProcH TbWrite'a falling, slot = CurrLast'
  // (the per-task address), data = dTm. Prints the first 24 so the startup's
  // own writes are visible too -- what T holds at the checksum test is
  // whatever the LAST of these left in slot 0.
  // BOOT-BLOCKER PROBES: first-assert cycles for the hold and the two FF
  // side effects Initial needs -- IFUReset reaching the IFU and LoadMCR
  // reaching MemC's k08. Counts, not samples.
  integer c_ifur = -1, c_mcr = -1, c_mdh = -1, c_dish = -1;
  integer n_noref1 = 0;
  integer n_ifur = 0, n_mcr = 0;
  longint n_cyc2 = 0;   // 64-bit for long runs
  reg d_ifur = 1'b0, d_mcr = 1'b0;
`ifdef WORLD
  // These probes reach into the IFU and MemC, which only the nine-board
  // dorado_world contains -- the four-board exec-test builds without them.
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
      $display("tb_exec: LDMCR edge@%0d %b->%b  RMar[00,01,07,08,09,10]=%b%b_%b%b%b%b  d_pre=%b DisHold=%b NoRef=%b",
               n_cyc2, d_mcr, m.b_MemC.LdMcr_p_,
               m.b_MemC.RMar_00, m.b_MemC.RMar_01, m.b_MemC.RMar_07,
               m.b_MemC.RMar_08, m.b_MemC.RMar_09, m.b_MemC.RMar_10,
               m.b_MemC.u_k08.d_pre,
               m.b_MemC.DisHold__drv, m.b_MemC.NoRef);
      $display("tb_exec: LDMCR2 stubs c24=%b c22=%b b20=%b | ASel21_43'=%b ASel23'=%b ASel57'=%b ASel67'=%b ASEL1'=%b | Amux1'=%b",
               m.b_ProcL.dAmux0__c24_3, m.b_ProcL.dAmux0__c22_2,
               m.b_ProcL.dAmux0__b20_3, 1'b0,
               m.b_ProcL.ASel_eq_2_s_3_p_, m.b_ProcL.ASel_eq_5_s_7_p_,
               m.b_ProcL.ASel_eq_6_s_7_p_, m.b_ProcL.ASEL_1_p_,
               m.b_ProcL.Amux1_p_);
      d_mcr <= m.b_MemC.LdMcr_p_;
    end
    if (!m.b_MemC.MDhold_p_ && c_mdh < 0) c_mdh = n_cyc2;
    if (m.b_MemC.DisHold__drv && c_dish < 0) c_dish = n_cyc2;
    if (m.b_MemC.NoRef) n_noref1 = n_noref1 + 1;
  end
`else
  always @(posedge sys_clk) n_cyc2 = n_cyc2 + 1;
`endif
  // Long runs are watched through a redirected stdout, which the C library
  // buffers in blocks, so a quiet stretch of the boot looks like a stall.
  // Flush once a million sys_clk so the log tracks the machine.
  always @(posedge sys_clk) if (n_cyc2 % 1000000 == 0) begin
    $display("tb_exec: PROGRESS @%0d CIA=%h", n_cyc2, ring_cia);
`ifdef FULL
    if ($test$plusargs("memprogress")) begin
      $display("tb_exec: PROGRESS-MEM @%0d task=%h VA=%h T=%h Hold=%b MDhold'=%b RefHold'=%b ALUa=%h ALUb=%h ALU=%h",
        n_cyc2, pct_ctask, w_procva, brc_t, m.Hold,
        m.b_MemC.MDhold_p_, m.b_MemC.RefHold_p_, brc_alua, brc_alub, brc_alu);
      $display("tb_exec: PROGRESS-DISK @%0d Active=%b ReadError=%b IOBParity=%b FifoParity=%b Underflow=%b ReadDataErr=%b NotReady=%b NotOnLine=%b NotSelected=%b HeadOvfl=%b ClearErrors=%b words=%0d",
        n_cyc2, m.b_DskEth.Active, m.b_DskEth.ReadError,
        m.b_DskEth.IOBParityErr, m.b_DskEth.FifoParityErr,
        m.b_DskEth.FifoUnderflow, m.b_DskEth.ReadDataErr,
        m.b_DskEth.NotReady, m.b_DskEth.NotOnLine, m.b_DskEth.NotSelected,
        m.b_DskEth.HeadOvfl, m.b_DskEth.ClearErrors, dk_nwords);
    end
`endif
    $fflush();
  end
`ifdef WORLD
  // `+stperroff` -- THE DECISIVE EXPERIMENT for the BLretry park: STPerr
  // (stuck 1 on every sample) is the master reset of MemX i10, the register
  // that carries the timing PROM's CAS column to the DRAMs. If the stuck
  // parity error is the whole blocker, forcing it low must let CAS fire,
  // the line load complete, the BL flag clear, and the boot sail past
  // RESETFAULTINFO. The substance behind it is MemC's ST-store parity
  // convention (write side vs checker) -- the IM-parity class of question;
  // this knob decides whether that is the LAST link before fixing it
  // properly.
  initial if ($test$plusargs("stperroff")) begin
    force m.b_MemX.STPerr = 1'b0;
    $display("tb_exec: +stperroff -- STPerr forced LOW (the ST parity latch)");
  end

  // HOW MANY MAP ENTRIES HAVE WP=0 / DIRTY=0 at the end of the run? The
  // seed writes 1 everywhere; Initial's map-reset writes VACANT = WP=1 AND
  // Dirty=1 (HM p.45). Zeros in these planes can only come from the machine's
  // own Map<- writes -- so a large zero count in the WP plane is the write
  // path storing WP INVERTED.
  // Map-DRAM strobe edges in the miss window: does the MAP RAM actually get
  // read for this reference?
  integer n_mras = 0, n_mcas2 = 0; reg d_mras2 = 1'b1, d_mcas3 = 1'b1;
  always @(posedge sys_clk) begin
    if (n_cyc2 >= 82260 && n_cyc2 < 83000) begin
      if (m.b_MemX.RTMapRAS_p_b !== d_mras2) n_mras = n_mras + 1;
      if (m.b_MemX.RTMapCAS_p_b !== d_mcas3) n_mcas2 = n_mcas2 + 1;
    end
    d_mras2 <= m.b_MemX.RTMapRAS_p_b; d_mcas3 <= m.b_MemX.RTMapCAS_p_b;
  end

  reg [5:0] d_mrw = 6'd0;
  always @(posedge sys_clk) begin
    if (n_cyc2 >= 82260 && n_cyc2 < 83000
        && {m.b_MemX.RTMapRAS_p_a, m.b_MemX.RTMapCAS_p_a, m.b_MemX.RTMapWE_p_a,
            m.b_MemX.u_a04.dout, m.b_MemX.u_a04.dout_r, m.b_MemX.MemX12_sil_pl_4} !== d_mrw)
      $display("tb_exec: MAPRAM @%0d RAS'a=%b CAS'a=%b WE'a=%b a04.dout=%b a04.dout_r=%b pl4=%b addr=%h",
               n_cyc2, m.b_MemX.RTMapRAS_p_a, m.b_MemX.RTMapCAS_p_a,
               m.b_MemX.RTMapWE_p_a, m.b_MemX.u_a04.dout, m.b_MemX.u_a04.dout_r,
               m.b_MemX.MemX12_sil_pl_4, m.b_MemX.u_a04.addr);
    d_mrw <= {m.b_MemX.RTMapRAS_p_a, m.b_MemX.RTMapCAS_p_a, m.b_MemX.RTMapWE_p_a,
              m.b_MemX.u_a04.dout, m.b_MemX.u_a04.dout_r, m.b_MemX.MemX12_sil_pl_4};
  end

  // THE MAP WRITE, CAUGHT IN THE ACT: at each RTMapWE'b assertion print the
  // flag-plane DINs against the TIOA bits they should carry (Map<- stores
  // WP = TIOA[0], Dirty = TIOA[1], RP = B -- memory.c's layout from HM p.46).
  reg d_mwe = 1'b1; integer n_mwrite = 0;
  always @(posedge sys_clk) begin
    if (!m.b_MemX.RTMapWE_p_b && d_mwe && n_mwrite < 12) begin
      n_mwrite = n_mwrite + 1;
      $display("tb_exec: MAPWRITE #%0d @%0d pc=%h TIOA01=%b%b dMapbufHi01=%b%b WPdin=%b Dirtydin=%b RP00din=%b Mapbuf00=%b",
               n_mwrite, n_cyc2, tnia_now,
               m.b_MemX.TIOA_0, m.b_MemX.TIOA_1,
               m.b_MemX.dMapbufHi_0, m.b_MemX.dMapbufHi_1,
               m.b_MemX.MemX13_sil_pl_14, m.b_MemX.MemX13_sil_pl_2,
               m.b_MemX.MemX12_sil_pl_5, m.b_MemX.Mapbuf_00);
    end
    d_mwe <= m.b_MemX.RTMapWE_p_b;
  end

  // `+blfix` -- the fault path's cache-flag cleanup, as a bench stand-in.
  // The miss writes its tag + Being-Loaded flag at @82272, 42 cycles BEFORE
  // the map read returns VACANT and faults (@82314): the optimistic
  // address-section write against the later fault verdict is the same
  // zero-delay ordering class as the campaign's other coincidences. On real
  // hardware a vacant-page reference cannot leave a permanent BL flag --
  // demand paging would wedge the machine daily -- so either the write is
  // suppressed in time (nanosecond gates) or the fault path cleans up.
  // Until that is settled from the gates, this knob does the cleanup: on
  // each ReportFault edge, clear the BL bit (bit 3 of the 4-bit flag word)
  // in all four way-planes at every row. A legitimately in-flight load at
  // the same instant would lose its flag; in this bench faults and loads
  // do not overlap.
  integer blfix_i; reg d_rfault = 1'b0;
  initial if ($test$plusargs("blfix")) begin
    forever begin
      @(posedge sys_clk);
      if (m.b_MemX.ReportFault_p_ === 1'b0 && d_rfault === 1'b1) begin
        for (blfix_i = 0; blfix_i < 256; blfix_i = blfix_i + 1) begin
          m.b_MemC.u_a08.mem[blfix_i][3] = 1'b0;
          m.b_MemC.u_b08.mem[blfix_i][3] = 1'b0;
          m.b_MemC.u_c08.mem[blfix_i][3] = 1'b0;
          m.b_MemC.u_d08.mem[blfix_i][3] = 1'b0;
        end
        $display("tb_exec: +blfix -- BL flags cleared after ReportFault @%0d", n_cyc2);
      end
      d_rfault <= m.b_MemX.ReportFault_p_;
    end
  end

  // THE TASK-SWITCH DISCRIMINATOR: during the task-init era, does the junk
  // wakeup (TWReq.02, newly jumpered from the IFU's JunkTW) ever REQUEST,
  // and if so does the switch chain (PEnc -> BNT -> CTask) respond? Counters
  // plus a change-window on the chain.
  integer n_twr02 = 0, n_penc_nz = 0, n_bnt_nz = 0;
  wire [3:0] w_penc = {m.b_ContA.PEnc_0, m.b_ContA.PEnc_1,
                       m.b_ContA.PEnc_2, m.b_ContA.PEnc_3};
  wire [3:0] w_bnt  = {m.b_ContA.BNT_0, m.b_ContA.BNT_1,
                       m.b_ContA.BNT_2, m.b_ContA.BNT_3};
  reg [10:0] d_tsw = 11'd0; integer n_taskon = 0, n_stoptasks = 0, n_pencgt = 0, n_tow = 0;
  always @(posedge sys_clk) begin
    if (m.TWReq_02) n_twr02 = n_twr02 + 1;
    if (w_penc != 4'd0) n_penc_nz = n_penc_nz + 1;
    if (w_bnt != 4'd0) n_bnt_nz = n_bnt_nz + 1;
    // Self-triggered: the FIRST time the decoded FF reads 0o143 (TaskingOn),
    // dump the latch neighbourhood for 60 cycles -- it executes ONCE at the
    // task-init loop's entry and the latch stays set.
    if (ff_now == 8'h63 && n_tow == 0) n_tow = 60;
    if (n_tow > 0) begin
      n_tow = n_tow - 1;
      $display("tb_exec: TOW @%0d pc=%h ff=%h FA1'=%b FB4'=%b FC3'=%b FFTaskOn=%b pl1=%b pl3=%b StopTasks=%b TaskingIsOff'=%b",
               n_cyc2, tnia_now, ff_now,
               m.b_ContA.FA_eq_1_p_, m.b_ContA.FB_eq_4_p_, m.b_ContA.FC_eq_3_p_,
               m.b_ContA.FF_eq_TaskingOn, m.b_ContA.ContA04_sil_pl_1,
               m.b_ContA.ContA04_sil_pl_3, m.b_ContA.StopTasks,
               m.b_ContA.TaskingIsOff_p_);
    end
    if (tce != 0 && n_cyc2 >= tcs && n_cyc2 < tce)
      $display("tb_exec: TSW @%0d pc=%h ff=%h FFBrOnResLt0=%b CBrOnResLt0'=%b ResLtZero'=%b ResEqZero'=%b CondBr'a=%b brTaken(pl9)=%b TNIA15=%b",
               n_cyc2, tnia_now, ff_now,
               m.b_ContA.FFBrOnResLt0, m.b_ContA.CBrOnResLt0_p_,
               m.b_ContA.ResLtZero_p_, m.ResEqZero_p_,
               m.b_ContA.CondBr_p_a, m.b_ContA.ContA05_sil_pl_9,
               m.TNIA_15);
    if (!m.b_ContA.TaskingIsOff_p_) n_taskon = n_taskon + 1;
    if (m.b_ContA.StopTasks) n_stoptasks = n_stoptasks + 1;
    if (!m.b_ContA.PEncGtTrueNext_p_) n_pencgt = n_pencgt + 1;
  end

  // One-shot: the whole TPC file, mid-task-init-spin. Slots are PRIMED
  // (a = ~task); TASKINIT = 0o6141 is what the oracle writes to tasks 1-15.
  reg tpcdump_done = 1'b0;
  always @(posedge sys_clk)
    if (n_cyc2 == 108925000 && !tpcdump_done) begin : tpcdump
      integer ti;
      tpcdump_done = 1'b1;
      for (ti = 0; ti < 16; ti = ti + 1)
        $display("tb_exec: TPCDUMP task=%0d slot=%0d TPC=%h%h%h%h", ti, 15-ti,
                 m.b_ContA.u_l13.mem[4'hf - ti[3:0]], m.b_ContA.u_i13.mem[4'hf - ti[3:0]],
                 m.b_ContA.u_j13.mem[4'hf - ti[3:0]], m.b_ContA.u_k13.mem[4'hf - ti[3:0]]);
    end

  final begin : mapplane_census
    integer zi, nwp0, ndirty0, nrp0;
    nwp0 = 0; ndirty0 = 0; nrp0 = 0;
    for (zi = 0; zi < 16384; zi = zi + 1) begin
      // d13 feeds e17's MapWP; d11 feeds e10's MapDirty copy.
      if (m.b_MemX.u_d13.mem[zi] === 1'b0) nwp0 = nwp0 + 1;
      if (m.b_MemX.u_d11.mem[zi] === 1'b0) ndirty0 = ndirty0 + 1;
      if (m.b_MemX.u_a04.mem[zi] === 1'b0) nrp0 = nrp0 + 1;
    end
    $display("tb_exec: MAPCENSUS -- WP-plane zeros %0d, Dirty-plane zeros %0d, RP00-plane zeros %0d (of 16384)",
             nwp0, ndirty0, nrp0);
    $display("tb_exec: MAPSTROBE -- window 82260-83000: MapRAS' %0d transitions, MapCAS' %0d",
             n_mras, n_mcas2);
    $display("tb_exec: TSWSUM -- TWReq02 high %0d, PEnc nonzero %0d, BNT nonzero %0d, TaskingIsOff' LOW (tasking ON) %0d samples",
             n_twr02, n_penc_nz, n_bnt_nz, n_taskon);
    $display("tb_exec: TSWSUM2 -- StopTasks high %0d, PEncGtTrueNext' LOW (switch wanted) %0d samples",
             n_stoptasks, n_pencgt);
  end

  // ---- THE MAPBUF WEDGE ---------------------------------------------------
  //
  // At the BLretry park a reference retries forever and everything sharing
  // the MapBuf (B<-FaultInfo' included) waits. HoldMapBuf PULSES during the
  // retry loop -- 23M total samples but never 10k continuous, which is why a
  // continuous-run detector never fired -- so the trigger is the TNIA freeze
  // itself: at stuck == 100,000 the main loop calls dump_mapwedge. The
  // instruction-history ring stops writing whenever stuck > 32, so it
  // preserves the APPROACH to the park rather than 64 copies of the parked
  // address.
  reg [11:0] ihist_pc [0:63]; reg [7:0] ihist_ff [0:63];
  integer ihist_w = 0, mbw_i, mbw_dumps = 0;
  // Count cache-flag writes by the BL value they deposit (way 0's RAM; the
  // strobe is WrCflags0' falling). Are BL flags ever CLEARED?
  integer n_blwr1 = 0, n_blwr0 = 0; reg d_wrcf = 1'b1;
  // ...and the whole strobe chain, counted: the CFlags<-A' decode
  // (Cflags^' from MemC l10, the FF decoder), WrCflagsOK', and all four
  // per-way strobes. At each of the first 12 decode assertions, print j16's
  // product terms so the blocking one names itself.
  integer n_cfdec = 0, n_cfok = 0, n_wcf [0:3], n_wvm = 0, n_vicwin = 0;
  integer n_psm = 0, n_stmap = 0;
  integer n_rasa = 0, n_casa = 0, n_lsin = 0, n_ssin = 0, n_lsout = 0,
          n_trans = 0, n_mkmd = 0, n_sin0 = 0, cas_t0 = 0,
          n_lsino = 0, n_m0sin = 0, n_m0str = 0, n_mkt0 = 0;
  reg [3:0] d_tr2 = 4'd0;
  reg [7:0] d_tr = 8'd0;
  reg d_cfdec = 1'b1, d_wvm = 1'b1, d_psm = 1'b1, d_stmap = 1'b1;
  reg [3:0] d_wcf4 = 4'b1111;
  initial begin n_wcf[0]=0; n_wcf[1]=0; n_wcf[2]=0; n_wcf[3]=0; end
  always @(posedge sys_clk) begin
    if (!m.b_MemC.WrCflags0_p_ && d_wrcf) begin
      if (m.b_MemC.NewBL) n_blwr1 = n_blwr1 + 1; else n_blwr0 = n_blwr0 + 1;
    end
    d_wrcf <= m.b_MemC.WrCflags0_p_;
    if (!m.b_MemC.Cflags_u__p_) begin
      n_cfdec = n_cfdec + 1;
      if (n_cfdec <= 12 || (n_cfdec % 100000) == 0)
        $display("tb_exec: CFDEC #%0d @%0d pc=%h WrCflagsOK'=%b | VicIfMiss'=%b PairHasA'b=%b AfreeOrEc'b=%b Hit'b=%b EcHasA'=%b pl12=%b | Cflags0^'=%b Cflags1^'=%b NewBL=%b",
                 n_cfdec, n_cyc2, tnia_now, m.b_MemC.WrCflagsOK_p_,
                 m.b_MemC.VicIfMiss_p_, m.b_MemC.PairHasA_p_b,
                 m.b_MemC.AfreeOrEc_p_b, m.b_MemC.Hit_p_b, m.b_MemC.EcHasA_p_,
                 m.b_MemC.MemC02_sil_pl_12,
                 m.b_MemC.Cflags0_u__p_, m.b_MemC.Cflags1_u__p_, m.b_MemC.NewBL);
      if (n_cfdec <= 12)
        $display("tb_exec: CFDEC+ ColVic=%b%b Victim'=%b%b Col'=%b%b HitOrEc=%b UseMcrV=%b Hold=%b Cflags23^'=%b%b Aad=%b%b%b%b%b%b%b%b",
                 m.b_MemC.ColVic_0, m.b_MemC.ColVic_1,
                 m.b_MemC.Victim_0_p_, m.b_MemC.Victim_1_p_,
                 m.b_MemC.Col_0_p_, m.b_MemC.Col_1_p_,
                 m.b_MemC.HitOrEc, m.b_MemC.UseMcrV, m.b_MemC.Hold,
                 m.b_MemC.Cflags2_u__p_, m.b_MemC.Cflags3_u__p_,
                 m.b_MemC.Aad_0a, m.b_MemC.Aad_1a, m.b_MemC.Aad_2a, m.b_MemC.Aad_3a,
                 m.b_MemC.Aad_4a, m.b_MemC.Aad_5a, m.b_MemC.Aad_6a, m.b_MemC.Aad_7a);
    end
    if (!m.b_MemC.WrCflagsOK_p_) n_cfok = n_cfok + 1;
    if (!m.b_MemC.WrVicMem_p_ && d_wvm) n_wvm = n_wvm + 1;
    d_wvm <= m.b_MemC.WrVicMem_p_;
    if (!m.b_MemX.preStartMem_p_ && d_psm) n_psm = n_psm + 1;
    d_psm <= m.b_MemX.preStartMem_p_;
    if (!m.b_MemX.StartMap_p_ && d_stmap) n_stmap = n_stmap + 1;
    d_stmap <= m.b_MemX.StartMap_p_;
    if (m.MemRASa !== d_tr[0]) n_rasa = n_rasa + 1;
    if (m.MemCASa !== d_tr[1]) begin
      n_casa = n_casa + 1;
      if (cas_t0 == 0 && n_cyc2 > 1000) cas_t0 = n_cyc2 - 60;
    end
    if (n_cyc2 >= 82260 && n_cyc2 < 83000)
      $display("tb_exec: DRAMWIN @%0d pc=%h MemState=%b%b%b%b RAS=%b CAS=%b pSM'=%b StartMap'=%b MemFree=%b MapFree=%b MakeCAS=%b Mod=%b%b RP7_0=%b%b%b%b%b%b%b%b MapWP=%b MapDirty=%b MapTrouble=%b VMFEc2'=%b MapInMap=%b",
               n_cyc2, tnia_now,
               m.b_MemX.MemState_0, m.b_MemX.MemState_1, m.b_MemX.MemState_2, m.b_MemX.MemState_3,
               m.MemRASa, m.MemCASa, m.b_MemX.preStartMem_p_, m.b_MemX.StartMap_p_,
               m.b_MemX.MemFree, m.b_MemX.MapFree,
               m.b_MemX.MakeMemCAS,
               m.b_MemX.Mod_0, m.b_MemX.Mod_1,
               m.b_MemX.RP_07, m.b_MemX.RP_06, m.b_MemX.RP_05, m.b_MemX.RP_04,
               m.b_MemX.RP_03, m.b_MemX.RP_02, m.b_MemX.RP_01, m.b_MemX.RP_00,
               m.b_MemX.MapWP, m.b_MemX.MapDirtya, m.b_MemX.MapTrouble,
               m.b_MemX.ValidMapFltInEc2_p_, m.b_MemX.Map_u_InMap);
    if (m.LoadSinE !== d_tr[2]) n_lsin = n_lsin + 1;
    if (m.ShiftSinE !== d_tr[3]) n_ssin = n_ssin + 1;
    if (m.LoadSoutE_p_ !== d_tr[4]) n_lsout = n_lsout + 1;
    if (m.Transport_p_ !== d_tr[5]) n_trans = n_trans + 1;
    if (m.MakeMDM_u_D_p_ !== d_tr[6]) n_mkmd = n_mkmd + 1;
    if (m.Sin_00 !== d_tr[7]) n_sin0 = n_sin0 + 1;
    if (m.LoadSinO !== d_tr2[0]) n_lsino = n_lsino + 1;
    if (m.Mod0SinEn_p_ !== d_tr2[1]) n_m0sin = n_m0sin + 1;
    if (m.Mod0StrEn_p_ !== d_tr2[2]) n_m0str = n_m0str + 1;
    if (m.b_MemX.MakeTransport0 !== d_tr2[3]) n_mkt0 = n_mkt0 + 1;
    d_tr2 <= {m.b_MemX.MakeTransport0, m.Mod0StrEn_p_, m.Mod0SinEn_p_, m.LoadSinO};
    d_tr <= {m.Sin_00, m.MakeMDM_u_D_p_, m.Transport_p_,
             m.LoadSoutE_p_, m.ShiftSinE, m.LoadSinE,
             m.MemCASa, m.MemRASa};
    // A whole cache-reference residency at the WrVicMem strobe gate: the
    // first 3 CacheRefInA episodes after cycle 80000 (the clear-loop era),
    // every sys_clk.
    if (m.b_MemC.CacheRefInA && n_cyc2 > 80000 && n_vicwin < 120) begin
      n_vicwin = n_vicwin + 1;
      $display("tb_exec: VICWIN @%0d pc=%h CacheRefInA=%b WrVicMem'=%b | LdVNV'=%b UseMcrV=%b McrV=%b%b%b%b ramV=%b%b%b%b Victim=%b%b NextV=%b%b",
               n_cyc2, tnia_now, m.b_MemC.CacheRefInA, m.b_MemC.WrVicMem_p_,
               m.b_MemC.LdVNV_p_, m.b_MemC.UseMcrV,
               m.b_MemC.MemC04_sil_pl_1, m.b_MemC.MemC04_sil_pl_2,
               m.b_MemC.MemC04_sil_pl_3, m.b_MemC.MemC04_sil_pl_4,
               m.b_MemC.MemC04_sil_pl_10, m.b_MemC.MemC04_sil_pl_11,
               m.b_MemC.MemC04_sil_pl_12, m.b_MemC.MemC04_sil_pl_13,
               m.b_MemC.Victim_0, m.b_MemC.Victim_1,
               m.b_MemC.NextV_0, m.b_MemC.NextV_1);
    end
    if (!m.b_MemC.WrCflags0_p_ && d_wcf4[0]) begin
      n_wcf[0] = n_wcf[0] + 1;
      if (n_wcf[0] <= 8)
        $display("tb_exec: W0STROBE #%0d @%0d pc=%h NewBL=%b NewVacant=%b NewDirty=%b NewWP=%b Aad=%b%b%b%b%b%b%b%b ColVic=%b%b UseMcrV=%b",
                 n_wcf[0], n_cyc2, tnia_now, m.b_MemC.NewBL, m.b_MemC.NewVacant,
                 m.b_MemC.NewDirty, m.b_MemC.NewWP,
                 m.b_MemC.Aad_0a, m.b_MemC.Aad_1a, m.b_MemC.Aad_2a, m.b_MemC.Aad_3a,
                 m.b_MemC.Aad_4a, m.b_MemC.Aad_5a, m.b_MemC.Aad_6a, m.b_MemC.Aad_7a,
                 m.b_MemC.ColVic_0, m.b_MemC.ColVic_1, m.b_MemC.UseMcrV);
    end
    if (!m.b_MemC.WrCflags1_p_ && d_wcf4[1]) n_wcf[1] = n_wcf[1] + 1;
    if (!m.b_MemC.WrCflags2_p_ && d_wcf4[2]) n_wcf[2] = n_wcf[2] + 1;
    if (!m.b_MemC.WrCflags3_p_ && d_wcf4[3]) n_wcf[3] = n_wcf[3] + 1;
    d_wcf4 <= {m.b_MemC.WrCflags3_p_, m.b_MemC.WrCflags2_p_,
               m.b_MemC.WrCflags1_p_, m.b_MemC.WrCflags0_p_};
  end
  task dump_mapwedge;
    begin
      $display("tb_exec: MAPWEDGE park confirmed @%0d (stuck=100000)", n_cyc2);
      $display("tb_exec: MAPWEDGE state Map'InPair'=%b Map'InMap=%b MapFree=%b MemFree=%b MapWait=%b preStartMem'=%b WantMapWait'=%b MDhold'=%b RefHold'=%b BLretry=%b HoldMapBuf=%b",
               m.b_MemX.Map_u_InPair_p_, m.b_MemX.Map_u_InMap, m.b_MemX.MapFree,
               m.b_MemX.MemFree, m.b_MemX.MapWait, m.b_MemX.preStartMem_p_,
               m.b_MemX.WantMapWait_p_, m.b_MemC.MDhold_p_, m.b_MemC.RefHold_p_,
               m.b_MemC.BLretry, m.b_MemX.HoldMapBuf__drv);
      // The BL story: dBL0-3 are the BEING-LOADED flags of the four ways at
      // the addressed cache row (a08/b08/c08/d08, the MB7071H flags RAM).
      // BLretry = retry while the line is being loaded; a stuck BL flag is
      // a load that never completed -- or one marked under noRef, whose
      // suppressed storage cycle could never clear it.
      $display("tb_exec: MAPWEDGE BL dBL[0:3]=%b%b%b%b BLreg[1:3]=%b%b%b Aad=%b%b%b%b%b%b%b%b VicIfMiss'=%b FlushInA=%b IoStoreInA=%b FlushStore=%b DbufBusy'=%b Hit'b=%b",
               m.b_MemC.dBL0, m.b_MemC.dBL1, m.b_MemC.dBL2, m.b_MemC.dBL3,
               m.b_MemC.BL1, m.b_MemC.BL2, m.b_MemC.BL3,
               m.b_MemC.Aad_0a, m.b_MemC.Aad_1a, m.b_MemC.Aad_2a, m.b_MemC.Aad_3a,
               m.b_MemC.Aad_4a, m.b_MemC.Aad_5a, m.b_MemC.Aad_6a, m.b_MemC.Aad_7a,
               m.b_MemC.VicIfMiss_p_, m.b_MemC.FlushInA, m.b_MemC.IoStoreInA,
               m.b_MemC.FlushStore, m.b_MemC.DbufBusy_p_, m.b_MemC.Hit_p_b);
      $display("tb_exec: MAPWEDGE BLWR flag-RAM writes: NewBL=1 on %0d, NewBL=0 on %0d (way0 row-writes)",
               n_blwr1, n_blwr0);
      $display("tb_exec: MAPWEDGE CF decode asserted %0d samples, WrCflagsOK %0d; strobes w0=%0d w1=%0d w2=%0d w3=%0d",
               n_cfdec, n_cfok, n_wcf[0], n_wcf[1], n_wcf[2], n_wcf[3]);
      for (mbw_i = 0; mbw_i < 64; mbw_i = mbw_i + 1)
        $display("tb_exec: MAPWEDGE hist[-%0d] pc=%h ff=%h", 63 - mbw_i,
                 ihist_pc[(ihist_w + mbw_i) & 63], ihist_ff[(ihist_w + mbw_i) & 63]);
    end
  endtask

  // ---- THE STACK POINTER'S ADJUST PATH -----------------------------------
  //
  // Initial pushes with delta +3/+1 (BOOTMEM, PRESETMAP, WRITEMAP) and pops
  // with -1, yet StkP changed only THREE times in 150M cycles -- all loads
  // -- and then DORETURN (BLOCK, RSTK=1000: read STK[StkP], check on) trips
  // the underflow hold the moment map init clears DisHold. So the question
  // is whether the adjust path ever produces a new value: StkWadr (the
  // XOR-adder l12/l13 through muxes i16/i17) against StkP, and whether
  // NewStkPVal' (h18) ever selects it into l14/l15. Counters, not samples.
  wire [7:0] w_stkp_v = {m.b_ProcL.StkP_0, m.b_ProcL.StkP_1, m.b_ProcL.StkP_2,
                         m.b_ProcL.StkP_3, m.b_ProcL.StkP_4, m.b_ProcL.StkP_5,
                         m.b_ProcL.StkP_6, m.b_ProcL.StkP_7};
  wire [7:0] w_stkwadr = {m.b_ProcL.StkWadr_0, m.b_ProcL.StkWadr_1,
                          m.b_ProcL.StkWadr_2, m.b_ProcL.StkWadr_3,
                          m.b_ProcL.StkWadr_4, m.b_ProcL.StkWadr_5,
                          m.b_ProcL.StkWadr_6, m.b_ProcL.StkWadr_7};
  integer n_newstkp1 = 0, n_newstkp0 = 0, n_wadr_ne = 0, n_stkprint = 0;
  reg [11:0] d_stkpc = 12'h0;
  always @(posedge sys_clk) begin
    // In the +tcwin window: one line per instruction, the whole stack path.
    if (tce != 0 && n_cyc2 >= tcs && n_cyc2 < tce && tnia_now !== d_stkpc) begin
      $display("tb_exec: STKWIN @%0d pc=%h ff=%h PrBlock'=%b StkSel'a=%b StkSela=%b RSTKa=%b%b%b%b StkP=%h StkWadr=%h New'=%b Curr'=%b",
               n_cyc2, tnia_now, ff_now, m.b_ProcL.PrBlock_p_,
               m.b_ProcL.StkSel_p_a, m.b_ProcL.StkSela,
               m.b_ProcL.RSTK_0a, m.b_ProcL.RSTK_1a, m.b_ProcL.RSTK_2a, m.b_ProcL.RSTK_3a,
               w_stkp_v, w_stkwadr,
               m.b_ProcL.NewStkPVal_p_, m.b_ProcL.CurrStkPVal_p_);
    end
    d_stkpc <= tnia_now;
    if (m.b_ProcL.NewStkPVal_p_) n_newstkp1 = n_newstkp1 + 1;
    else                         n_newstkp0 = n_newstkp0 + 1;
    if (w_stkwadr !== w_stkp_v) begin
      n_wadr_ne = n_wadr_ne + 1;
      if (n_stkprint < 24 && n_cyc2 > 100000) begin
        n_stkprint = n_stkprint + 1;
        $display("tb_exec: STKADJ @%0d pc=%h StkP=%h StkWadr=%h NewStkPVal'=%b CurrStkPVal'=%b",
                 n_cyc2, tnia_now, w_stkp_v, w_stkwadr,
                 m.b_ProcL.NewStkPVal_p_, m.b_ProcL.CurrStkPVal_p_);
      end
    end
  end

  // ---- THE MAR BUS, WATCHED WHOLE ----------------------------------------
  //
  // All eleven MCR-relevant Mar bits, printed on CHANGE inside the +tcwin
  // window. This probe is what closed the LoadMCR saga (2026-09-01): with
  // every bit visible, the "in-residency data" of 198b28f8 (RMar09=RMar10=1)
  // turned out to be the bus's ALL-ONES idle -- every bit was 1, two were
  // read -- and the true data phase is the MarMuxAEn'=0 window, where the
  // bus read ALL ZEROS because `+fastwait`'s T-file clip had already zapped
  // the SetMCR value (T carries it, A<-T, and the clip fired on every
  // sys_clk while TNIA read c0a with T > 0x10 -- the LoadMCR executes
  // exactly there). With the clip off, every LoadMCR captures its true data
  // at its own edge and no deposit knob is needed at all: the strobe timing
  // was never the bug. A `+mcrfix` deposit knob briefly existed and was
  // removed with the diagnosis it encoded.
  integer tcs = 0, tce = 0, tcl = 60;
  initial if ($value$plusargs("tcwin=%d", tcs)) begin
    void'($value$plusargs("tclen=%d", tcl));
    tce = tcs + tcl;
  end
  wire [10:0] w_rmar = {m.b_MemC.RMar_10, m.b_MemC.RMar_09, m.b_MemC.RMar_08,
                        m.b_MemC.RMar_07, m.b_MemC.RMar_06, m.b_MemC.RMar_05,
                        m.b_MemC.RMar_04, m.b_MemC.RMar_03, m.b_MemC.RMar_02,
                        m.b_MemC.RMar_01, m.b_MemC.RMar_00};
  reg [10:0] d_rmarw = 11'd0; reg [7:0] d_ffw = 8'd0; reg d_aenw = 1'b0;
  always @(posedge sys_clk) begin
    if (tce != 0 && n_cyc2 >= tcs && n_cyc2 < tce
        && (w_rmar !== d_rmarw || ff_now !== d_ffw
            || m.b_ProcL.MarMuxAEn_p_ !== d_aenw))
      $display("tb_exec: MCRWIN @%0d ff=%h AEn'=%b RMar[10:0]=%b LdMcr'=%b", n_cyc2,
               ff_now, m.b_ProcL.MarMuxAEn_p_, w_rmar, m.b_MemC.LdMcr_p_);
    d_rmarw <= w_rmar; d_ffw <= ff_now; d_aenw <= m.b_ProcL.MarMuxAEn_p_;
  end
`endif
  // TIME COURSE around the LoadMCR instruction: every sys_clk in a window,
  // the combinational decode (dAmux0, BSel=2/6), the REGISTERED enable
  // (MarMuxAEn'), b11's load strobe (ProcL14_sil_pl_2), and RMar_09.
  // (tcs/tce are declared with the +mcrfix knob above, which shares them.)
`ifdef WORLD
  always @(posedge sys_clk)
    if (tce != 0 && n_cyc2 >= tcs && n_cyc2 < tce)
      $display("tb_exec: TC @%0d pc=%h ff=%h dAmux0=%b(c24=%b c22=%b b20=%b) BSel26=%b MarMuxAEn'=%b pl2=%b RMar09=%b LdMcr'=%b | RMar09=%b RMar10=%b AEn'=%b BEn'=%b Amux1'=%b LdMcr'=%b",
               n_cyc2, tnia_now, ff_now, m.b_ProcL.dAmux0,
               m.b_ProcL.dAmux0__c24_3, m.b_ProcL.dAmux0__c22_2, m.b_ProcL.dAmux0__b20_3,
               m.b_ProcL.BSel_eq_2_s_6,
               m.b_ProcL.MarMuxAEn_p_, m.b_ProcL.ProcL14_sil_pl_2,
               m.b_MemC.RMar_09, m.b_MemC.LdMcr_p_,
               m.b_MemC.RMar_09, m.b_MemC.RMar_10,
               m.b_ProcL.MarMuxAEn_p_, m.b_ProcL.MarMuxBEn_p_,
               m.b_ProcL.Amux1_p_, m.b_MemC.LdMcr_p_);
`endif
  integer n_marA = 0;
  always @(posedge sys_clk) if (m.b_ProcL.MarMuxAEn_p_) n_marA = n_marA + 1;
  integer n_twr = 0; reg d_tbw = 1'b1; reg [15:0] twr_last = 16'hxxxx;
  wire [15:0] w_dtm = {m.b_ProcH.dTm_00, m.b_ProcH.dTm_01, m.b_ProcH.dTm_02,
                       m.b_ProcH.dTm_03, m.b_ProcH.dTm_04, m.b_ProcH.dTm_05,
                       m.b_ProcH.dTm_06, m.b_ProcH.dTm_07,
                       m.b_ProcL.dTm_08, m.b_ProcL.dTm_09, m.b_ProcL.dTm_10,
                       m.b_ProcL.dTm_11, m.b_ProcL.dTm_12, m.b_ProcL.dTm_13,
                       m.b_ProcL.dTm_14, m.b_ProcL.dTm_15};
  wire [3:0] w_tslot = {m.b_ProcH.CurrLast_3_p_, m.b_ProcH.CurrLast_2_p_,
                        m.b_ProcH.CurrLast_1_p_, m.b_ProcH.CurrLast_0_p_};
  always @(posedge sys_clk) begin
    if (cktrace && !m.b_ProcH.TbWrite_p_a && d_tbw && n_twr < 40
        && w_dtm !== twr_last) begin
      $display("tb_exec: TWR slot=%h dTm=%h (pc around %h, @%0d)", w_tslot, w_dtm, tnia_now, n_cyc2);
      n_twr = n_twr + 1;
      twr_last = w_dtm;
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
  integer   n_tinit, fh_c44, fh_c45, fh_c60;
  initial begin fh_c44 = -1; fh_c45 = -1; fh_c60 = -1; end
  integer   nvisited, lastpc, stuck, maxrun, prevpc, maxpc;  reg [3:0] ctnow;

`ifdef WORLD
  wire [27:0] w_procva = {m.b_MemC.ProcVA_04, m.b_MemC.ProcVA_05, m.b_MemC.ProcVA_06, m.b_MemC.ProcVA_07,
                          m.b_MemC.ProcVA_08, m.b_MemC.ProcVA_09, m.b_MemC.ProcVA_10, m.b_MemC.ProcVA_11,
                          m.b_MemC.ProcVA_12, m.b_MemC.ProcVA_13, m.b_MemC.ProcVA_14, m.b_MemC.ProcVA_15,
                          m.b_MemC.ProcVA_16, m.b_MemC.ProcVA_17, m.b_MemC.ProcVA_18, m.b_MemC.ProcVA_19,
                          m.b_MemC.ProcVA_20, m.b_MemC.ProcVA_21, m.b_MemC.ProcVA_22, m.b_MemC.ProcVA_23,
                          m.b_MemC.ProcVA_24, m.b_MemC.ProcVA_25, m.b_MemC.ProcVA_26, m.b_MemC.ProcVA_27,
                          m.b_MemC.ProcVA_28, m.b_MemC.ProcVA_29, m.b_MemC.ProcVA_30, m.b_MemC.ProcVA_31};
  wire [8:0] fg_now = {m.FG_0, m.FG_1, m.FG_2, m.FG_3, m.FG_4,
                       m.FG_5, m.FG_6, m.FG_7, m.FG_8};
  wire [7:0] j_now = {m.b_IFU.J_0a, m.b_IFU.J_1a, m.b_IFU.J_2a, m.b_IFU.J_3a,
                      m.b_IFU.J_4a, m.b_IFU.J_5a, m.b_IFU.J_6a, m.b_IFU.J_7a};
  wire [17:0] ifu_trace_d={m.b_MemD.D_00,m.b_MemD.D_01,m.b_MemD.D_02,m.b_MemD.D_03,m.b_MemD.D_04,m.b_MemD.D_05,
    m.b_MemD.D_06,m.b_MemD.D_07,m.b_MemD.D_08,m.b_MemD.D_09,m.b_MemD.D_10,m.b_MemD.D_11,
    m.b_MemD.D_12,m.b_MemD.D_13,m.b_MemD.D_14,m.b_MemD.D_15,m.b_MemD.D_16,m.b_MemD.D_17};
  wire [17:0] ifu_trace_f={m.b_MemD.F_00,m.b_MemD.F_01,m.b_MemD.F_02,m.b_MemD.F_03,m.b_MemD.F_04,m.b_MemD.F_05,
    m.b_MemD.F_06,m.b_MemD.F_07,m.b_MemD.F_08,m.b_MemD.F_09,m.b_MemD.F_10,m.b_MemD.F_11,
    m.b_MemD.F_12,m.b_MemD.F_13,m.b_MemD.F_14,m.b_MemD.F_15,m.b_MemD.F_16,m.b_MemD.F_17};
  wire [17:0] ifu_trace_g={m.b_MemD.G_00,m.b_MemD.G_01,m.b_MemD.G_02,m.b_MemD.G_03,m.b_MemD.G_04,m.b_MemD.G_05,
    m.b_MemD.G_06,m.b_MemD.G_07,m.b_MemD.G_08,m.b_MemD.G_09,m.b_MemD.G_10,m.b_MemD.G_11,
    m.b_MemD.G_12,m.b_MemD.G_13,m.b_MemD.G_14,m.b_MemD.G_15,m.b_MemD.G_16,m.b_MemD.G_17};
  reg ifu_trace=0, ifu_ack_d=0, ifu_saw_d=0, ifu_fc_d=1;
  integer ifu_trace_n=0;
  longint ifu_trace_from=0;
  initial begin
    ifu_trace=$test$plusargs("ifutrace");
    void'($value$plusargs("ifufrom=%d",ifu_trace_from));
  end
  always @(posedge sys_clk) begin
    if(ifu_trace && n_cyc2>=ifu_trace_from && ifu_trace_n<128 &&
       (!m.b_IFU.FGParityErr_p_ || ifu_ack_d!=m.b_IFU.IfuMemAck ||
        ifu_saw_d!=m.b_IFU.SawFGParityErr || ifu_fc_d!=m.b_MemD.Fclk_p_a)) begin
      ifu_trace_n=ifu_trace_n+1;
      $display("tb_exec: IFUTRACE @%0d CIA=%h task=%h VA=%h FG=%h byte=%h parity=%b xor=%b err'=%b FGDv=%b FDv=%b GDv=%b ack=%b saw=%b opcode=%h Fclk'=%b GLd'=%b SinDpar=%b%b",
        n_cyc2,ring_cia,pct_ctask,w_procva,fg_now,fg_now[8:1],fg_now[0],^fg_now,
        m.b_IFU.FGParityErr_p_,m.b_IFU.FGDv,m.b_IFU.FDv,m.b_IFU.GDv,
        m.b_IFU.IfuMemAck,m.b_IFU.SawFGParityErr,j_now,
        m.b_MemD.Fclk_p_a,m.b_MemD.GLd_p_,m.b_MemD.SinD_16,m.b_MemD.SinD_17);
      $display("tb_exec: IFUWORDS @%0d D=%05h F=%05h G=%05h pre1=%b clk1=%b Fclk=%b F_D=%b PcFG15=%b",
        n_cyc2,ifu_trace_d,ifu_trace_f,ifu_trace_g,m.b_MemD.preClk1_p_A,
        m.b_MemD.clk1_p_Aa,m.b_MemD.Fclk_p_a,m.b_MemD.F_u_D,m.PcFG_15);
    end
    ifu_ack_d<=m.b_IFU.IfuMemAck;
    ifu_saw_d<=m.b_IFU.SawFGParityErr;
    ifu_fc_d<=m.b_MemD.Fclk_p_a;
  end
  longint startup_from=-1, startup_len=256;
  wire [15:0] startup_ifub={m.b_IFU.IfuBMux_00,m.b_IFU.IfuBMux_01,m.b_IFU.IfuBMux_02,m.b_IFU.IfuBMux_03,
    m.b_IFU.IfuBMux_04,m.b_IFU.IfuBMux_05,m.b_IFU.IfuBMux_06,m.b_IFU.IfuBMux_07,
    m.b_IFU.IfuBMux_08,m.b_IFU.IfuBMux_09,m.b_IFU.IfuBMux_10,m.b_IFU.IfuBMux_11,
    m.b_IFU.IfuBMux_12,m.b_IFU.IfuBMux_13,m.b_IFU.IfuBMux_14,m.b_IFU.IfuBMux_15};
  wire [7:0] startup_pcf={m.b_IFU.PcF_08,m.b_IFU.PcF_09,m.b_IFU.PcF_10,m.b_IFU.PcF_11,
    m.b_IFU.PcF_12,m.b_IFU.PcF_13,m.b_IFU.PcF_14,m.b_IFU.PcF_15};
  // AEmu Start/StartIFU executes PCF<-0. Stale H must not replace that
  // setup word when NewPc1 drops and the shared IFU ALU bus takes over.
  bit check_pcf_setup=0, pcf_setup_arm=0, pcf_setup_seen=0, pcf_setup_pass=0;
  integer pcf_setup_delay=-1;
  initial begin
    check_pcf_setup=$test$plusargs("check-pcf-setup");
    if(check_pcf_setup) begin
      force m.b_IFU.H_0=1; force m.b_IFU.H_1=0;
      force m.b_IFU.H_2=1; force m.b_IFU.H_3=1;
      force m.b_IFU.H_4=0; force m.b_IFU.H_5=0;
      force m.b_IFU.H_6=1; force m.b_IFU.H_7=0;
    end
  end
  always @(posedge sys_clk) if(check_pcf_setup && pcf_setup_arm && !pcf_setup_pass) begin
    if(m.b_IFU.NewPc1) pcf_setup_seen=1;
    if(pcf_setup_seen && !m.b_IFU.NewPc1 && pcf_setup_delay<0)
      pcf_setup_delay=2;
    else if(pcf_setup_delay>0) pcf_setup_delay=pcf_setup_delay-1;
    else if(pcf_setup_delay==0) begin
      if(startup_pcf!==0)
        $fatal(1,"PCF<-0 captured %h instead (stale H=b2, expected first word offset0)",startup_pcf);
      pcf_setup_pass=1;
      $display("tb_exec: PCF-SETUP PASS -- real StartIFU loads zero before stale H=b2 takes over the bus");
      $finish;
    end
  end
  wire [15:0] startup_rmar={m.b_MemC.RMar_00,m.b_MemC.RMar_01,m.b_MemC.RMar_02,m.b_MemC.RMar_03,
    m.b_MemC.RMar_04,m.b_MemC.RMar_05,m.b_MemC.RMar_06,m.b_MemC.RMar_07,
    m.b_MemC.RMar_08,m.b_MemC.RMar_09,m.b_MemC.RMar_10,m.b_MemC.RMar_11,
    m.b_MemC.RMar_12,m.b_MemC.RMar_13,m.b_MemC.RMar_14,m.b_MemC.RMar_15};
  initial begin
    void'($value$plusargs("startupfrom=%d",startup_from));
    void'($value$plusargs("startuplen=%d",startup_len));
  end
  always @(posedge sys_clk) if(startup_from>=0 && n_cyc2>=startup_from && n_cyc2<startup_from+startup_len)
    $display("tb_exec: STARTUP @%0d CIA=%h FF=%h BMux=%h IfuBMux=%h PcF=%h ZapFGH'=%b clk1=%b inc'=%b reset=%b jump=%b MarPcF'=%b RMar=%h BrLoWE'=%b MemB=%b%b%b%b%b VA=%h",
      n_cyc2,ring_cia,ff_now,bmux,startup_ifub,startup_pcf,m.b_IFU.ZapFGH_p_,m.b_IFU.clk1_p_Aa,
      m.b_IFU.IncPcF_p_,m.b_IFU.IfuReset,m.b_IFU.DoJump,m.b_IFU.Mar_u_PcF_p_,startup_rmar,
      m.b_MemC.WrBrLo_p_a,m.b_MemC.MemB_0,m.b_MemC.MemB_1,m.b_MemC.MemB_2,m.b_MemC.MemB_3,m.b_MemC.MemB_4,w_procva);
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
`ifdef WORLD
      for (i = 0; i < 1024; i = i + 1) world_ifum_have[i] = 0;
      for (i = 0; i < 16; i = i + 1) world_alufm_have[i] = 0;
`endif
      nalufm = 0; nalufmbad = 0; nifum = 0; nifumbad = 0;
      ifum_raw = $test$plusargs("ifumraw");
      // PARC's IRTable and the MC10170 physical checkers use odd parity.
      // +impareven is a deliberate corrupt-parity fixture, never a boot mode.
      impar_odd   = !$test$plusargs("impareven");
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
          world_ifum_have[ia[9:0]]=1;
          world_ifum_lo[ia[9:0]]=ib[15:0];
          world_ifum_hi[ia[9:0]]=ifum_want;
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
`ifdef WORLD
          world_alufm_have[ia[3:0]]=1;
          world_alufm[ia[3:0]]=ib[5:0];
`endif
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
      // PARC's IRTable specifies odd parity across each 17-bit half. The C
      // emulator does not model it; +impareven deliberately supplies corrupt
      // parity to qualify detection by the actual board checkers.
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
    if (!$value$plusargs("hunklimit=%d", hunklimit)) hunklimit=4;
    while (!$feof(fd) && hcount < hunklimit) begin
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

    // Compare a CP-bus load against the original MB decoder's field image.
    // This checks the real IM write parity, rather than preloading it.
    if ($value$plusargs("expectim=%s", expect_im_path)) begin
      imfd=$fopen(expect_im_path,"r");
      if(!imfd)$fatal(1,"cannot open expected IM image %s",expect_im_path);
      nver=0;
      while($fgets(line,imfd)!=0) begin
        imn=$sscanf(line,"%s %h %h %h %h %h %h %h %h %h %h %h",
          tag,ia,ib,ic,id,ie,ig,ij,ih,ii,ilh,irh);
        if(imn!=12 || tag!="IM")continue;
        im_readback_word(ia,r_rstk,r_aluf,r_bsel,r_lc,r_asel,r_ff,r_jcn,r_blk,r_lh_p,r_rh_p);
        if({r_rstk,r_aluf,r_bsel,r_lc,r_asel,r_ff,r_jcn,r_blk} !==
           {ib[3:0],ic[3:0],id[2:0],ie[2:0],ig[2:0],ih[7:0],ii[7:0],~ij[0]})
          $fatal(1,"CP-loaded IM[%03h] fields differ from original microcode",ia);
        if(r_lh_p!==(~^ilh[16:0]) || r_rh_p!==(~^irh[16:0]))
          $fatal(1,"CP-loaded IM[%03h] has incorrect odd parity: %b/%b expected %b/%b",
            ia,r_lh_p,r_rh_p,~^ilh[16:0],~^irh[16:0]);
        nver++;
      end
      $fclose(imfd);
      if(nver==0)$fatal(1,"expected IM image contained no microinstructions");
      $display("tb_exec: CP-LOAD PASS: %0d original microinstructions and both odd-parity bits match",nver);
    end

    // RELEASE the MIR clock -- register 7, data bit 0 -- so the MIR can reload
    // from IM. Without this the jam is held and nothing is ever fetched.
    // The BaseBoard ROM leaves MidasStopMIRClk enabled during Bootstrap.
    // Retain that configuration when comparing an authentic startup stall;
    // the default keeps the historical isolated execution test unchanged.
    if (!$test$plusargs("keepmirdebug")) manifold(12'h1C0);
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

    // +setmcr=HEX: LOAD THE MEMORY CONTROL REGISTER BEFORE THE MACHINE RUNS.
    //
    // A world cold-started has never had an MCR loaded, and wedges with
    // RefHold asserted on 98% of samples, its first `RM/STK<-Md` held for
    // ever. Initial does this for itself: `BlessBaseBoard` ends with
    // `T_ mcr.noWake, Call[SetMCR]` -- "now allow holds, etc., but no fault
    // task wakeups" -- and only then goes looking for a world. `mcr.noWake`
    // is b15 in PARC's MSB-first numbering, i.e. 0x0001, and every other bit
    // zero means holds on, references on, base registers and cache flags on.
    //
    // `LoadMcr[A,B]` is FA=1 FB=2 FC=6 = FF 0o126 (cpu.c's own dispatch), and
    // takes Mcr[0:10] from A and Mcr[13:15] from B -- Initial passes T for
    // both. So: T <- CPReg, then one jammed LoadMcr[T,T]. SetMCR's own
    // `Cnt_ 6S` wait is not needed here because nothing has touched memory.
    //
    // IT GOES BEFORE THE LINK SETUP: loading T needs a CPReg write and a jam,
    // and doing that after `CPRegToLink#` left the machine starting at 0 (the
    // start address is carried in Link, and Link is what the startup Return
    // fetches).
    if ($value$plusargs("setmcr=%h", mcrval)) begin
      set_cpreg_plain(mcrval[15:0]);
      parc_micro(8'h70, 8'h03, 8'h0F, 8'h04, 8'hC0);   // TFromCPReg#
      nop_micro;
      {mb0_, mb1_, mb2_, mb3_, mb4_} =
        mkmi(4'd0, 4'd0, 3'd2 /*BSEL=T*/, 3'd0 /*LC NoLoad*/,
           3'd6 /*ASEL A<-T*/, 8'o126 /*LoadMcr[A,B]*/, 8'h00, 1'b0);
      parc_micro(mb0_, mb1_, mb2_, mb3_, mb4_);
      nop_micro;
      $display("tb_exec: +setmcr -- MCR loaded with %h through a jammed LoadMcr[T,T] (%h %h %h %h %h)",
               mcrval[15:0], mb0_, mb1_, mb2_, mb3_, mb4_);
    end

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
    // The CP fixture owns these nets while loading. Native startup then
    // uses the CPU's MidasStrobe/UseDMD to change real manifold registers.
    // Retaining the fixture forces suppresses Initial's enable-all-errors.
    if($test$plusargs("cpu-manifold")) begin
      release m.DMuxData; release m.DMuxClk; release m.UseDMD;
      $display("tb_exec: CPU manifold drivers released from CP fixture");
    end
    paritysources_arm=1;
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
    nvisited = 0; stuck = 0; maxrun = 0; prevpc = -1; maxpc = -1; n_tinit = 0;
    p0 = m.b_ContA.clk0_p_Ca; p1 = m.b_ContA.clk1_p_Ca;
    if (!$value$plusargs("cycles=%d", runcycles)) runcycles = 20000;
    has_cycle_limit_file = $value$plusargs("cycle-limit-file=%s", cycle_limit_file);
    bootcp_n = 0; bootcp_phase = 1'b0; bootcp_byte = 0;
    if (!$value$plusargs("bootcp=%d", bootcp_period)) bootcp_period = 0;
    cktrace = $test$plusargs("cktrace");
    if (bootcp_period) $display("tb_exec: BOOTCP -- toggling the handshake every %0d cycles",
                                bootcp_period);

`ifdef WORLD   // only the WORLD-and-up machines have an IOReset port
    if (ioreset_r) $display("tb_exec: +ioreset -- IOReset released at @%0d", n_cyc2);
    ioreset_r = 1'b0;
`endif
`ifdef WORLD
    pcf_setup_arm=1;
`endif
    for (j2 = 0; j2 < runcycles; j2 = j2 + 1) begin
      @(posedge sys_clk);
`ifdef WORLD
      if(check_pcf_setup && j2==runcycles-1 && !pcf_setup_pass)
        $fatal(1,"PCF setup test never observed a completed PCF load");
`endif
      // Optional host-side run limit, sampled once per million clocks.
      // Allows extending a progressing boot without repeating initialization.
      // This controls only when the bench reports/finishes, never hardware.
      if (has_cycle_limit_file && j2 % 1000000 == 0) begin
        cycle_limit_fd = $fopen(cycle_limit_file, "r");
        if (cycle_limit_fd != 0) begin
          cycle_limit_rc = $fscanf(cycle_limit_fd, "%d", requested_cycle_limit);
          $fclose(cycle_limit_fd);
          if (cycle_limit_rc == 1 && requested_cycle_limit >= 10 &&
              requested_cycle_limit != runcycles) begin
            runcycles = (requested_cycle_limit > j2) ? requested_cycle_limit : j2 + 1;
            $display("tb_exec: RUN-LIMIT -- now %0d samples", runcycles);
          end
        end
      end
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
          if (lastpc[11:0] == 12'hc45) n_tinit = n_tinit + 1;
          // First-hit cycles for the task-init era's landmarks.
          if (lastpc[11:0] == 12'hc44 && fh_c44 < 0) begin fh_c44 = n_cyc2; $display("tb_exec: FIRSTHIT c44 @%0d", n_cyc2); end
          if (lastpc[11:0] == 12'hc45 && fh_c45 < 0) begin fh_c45 = n_cyc2; $display("tb_exec: FIRSTHIT c45 @%0d", n_cyc2); end
          if (lastpc[11:0] == 12'hc60 && fh_c60 < 0) begin fh_c60 = n_cyc2; $display("tb_exec: FIRSTHIT c60 @%0d", n_cyc2); end
          if (!visited[lastpc[11:0]]) begin
            visited[lastpc[11:0]] = 1'b1; nvisited = nvisited + 1;
          end
`ifdef WORLD
          // The MAPWEDGE ring freezes once a long repeat begins, so it holds
          // the approach; it resumes if the repeat resolves (stuck resets).
          if (stuck <= 32) begin
            ihist_pc[ihist_w] = lastpc[11:0]; ihist_ff[ihist_w] = ff_now;
            ihist_w = (ihist_w + 1) & 63;
          end
          if (stuck == 100000 && mbw_dumps < 1) begin
            mbw_dumps = mbw_dumps + 1;
            dump_mapwedge;
          end
`endif
          // `+trwin=N` delays trace collection to cycle N, so the 256
          // entries show the STEADY STATE of a late loop instead of the
          // long-known startup.
          if (n_tr < 256 && n_cyc2 >= trwin) begin
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
      // Does the gated MIR clock edge WHILE the repeat is asserted? c19
      // (SE10210) makes h*clk0' = RepeatCurC | StopMIRClk | preclk0', so a
      // high RepeatCurC should pin it high: zero edges. Each copy checked,
      // and TNIA changes counted in the same era.
      if (m.b_ContA.RepeatCurC) begin
        if (m.b_ContA.h_x2a_clk0_p_Ca && !d_hx2a) n_hx2a_held = n_hx2a_held + 1;
        if (tnia_now !== d_tnia_h) n_tnia_held = n_tnia_held + 1;
      end
      d_hx2a  = m.b_ContA.h_x2a_clk0_p_Ca;
      d_tnia_h = tnia_now;
      if (m.b_ContA.RepeatCur_p_)  n_repp  = n_repp + 1;
      if (m.b_ContA.CAHold_p_)     n_cahp2 = n_cahp2 + 1;
      if (m.b_ContA.Hold)          n_hold2 = n_hold2 + 1;
      if (m.b_ContA.Hold)          ph_hold[j2 % 16] = ph_hold[j2 % 16] + 1;
      // EPISODES, not levels: each deassertion is a window in which the
      // machine can advance. Count edges, the longest gap, and the last one.
      if (!m.b_ContA.Hold && d_hold3) n_hgap = n_hgap + 1;
      if (!m.b_ContA.Hold) begin
        gap_run = gap_run + 1;
        if (gap_run > gap_max) gap_max = gap_run;
        gap_last = j2;
      end else gap_run = 0;
      d_hold3 = m.b_ContA.Hold;
      if (m.b_ContA.SwitchUp)      n_swup  = n_swup + 1;
      if (m.b_ContA.CAHold)        n_cah20 = n_cah20 + 1;
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
    $display("tb_exec: STKADJ -- NewStkPVal' hi %0d lo %0d; StkWadr!=StkP on %0d samples",
             n_newstkp1, n_newstkp0, n_wadr_ne);
    $display("tb_exec: CFLAGS -- decode %0d, WrCflagsOK %0d; strobes w0=%0d w1=%0d w2=%0d w3=%0d; way0 NewBL=1 %0d NewBL=0 %0d",
             n_cfdec, n_cfok, n_wcf[0], n_wcf[1], n_wcf[2], n_wcf[3], n_blwr1, n_blwr0);
    $display("tb_exec: VICMEM -- WrVicMem' strobes %0d", n_wvm);
    $display("tb_exec: STARTCHAIN -- preStartMem' pulses %0d, StartMap' pulses %0d",
             n_psm, n_stmap);
    $display("tb_exec: TRANSPORT -- RASa %0d CASa %0d LoadSinE %0d ShiftSinE %0d LoadSoutE' %0d Transport' %0d MakeMD_D' %0d Sin00 %0d",
             n_rasa, n_casa, n_lsin, n_ssin, n_lsout, n_trans, n_mkmd, n_sin0);
    $display("tb_exec: TRANSPORT2 -- LoadSinO %0d Mod0SinEn' %0d Mod0StrEn' %0d MakeTransport0 %0d",
             n_lsino, n_m0sin, n_m0str, n_mkt0);
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
    $display("tb_exec: MIRCLK -- h*clk0'(Ca) rising edges while RepeatCurC=1: %0d; TNIA changes while RepeatCurC=1: %0d",
             n_hx2a_held, n_tnia_held);
    $display("tb_exec: HOLD GAPS -- %0d deassert episodes, longest %0d samples, last not-held at %0d of %0d",
             n_hgap, gap_max, gap_last, runcycles);
    $write("tb_exec: HOLD BY PHASE --");
    for (i = 0; i < 16; i = i + 1) $write(" %0d", ph_hold[i]);
    $write("\n");
    $display("tb_exec: F20 -- RepeatCur %0d RepeatCur' %0d SwitchUp %0d CAHold %0d CAHold' %0d Hold %0d",
             n_rep, n_repp, n_swup, n_cah20, n_cahp2, n_hold2);
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
`ifdef SCREEN
    // Write the captures FIRST, before any gate can $fatal -- the clock-floor
    // assert killed a 300M screen run with the frame unwritten.
    if (pgm_on) write_pgm;
    if (frame_on) write_frame;
`endif
    $display("tb_exec: %0d distinct IM addresses executed; last TNIA=%h; longest run on one address=%0d at TNIA=%h; TASKINITLOOP entries %0d",
             nvisited, lastpc[11:0], maxrun, maxpc[11:0], n_tinit);
    // Name them. A short cycle is the normal shape of a microcode wait loop, and
    // knowing WHICH addresses turns "it loops" into something disassemblable.
    if (nvisited <= 64 || $test$plusargs("addrs")) begin
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
    if($test$plusargs("initial-rom-check"))begin
      if(!$test$plusargs("cpu-manifold") || all_enabled_samples<100000 ||
         all_enabled_errors!=0 || n_tinit!=15)
        $fatal(1,"Initial ROM: manifold=%b all-enabled samples=%0d errors=%0d task initializations=%0d",
          $test$plusargs("cpu-manifold"),all_enabled_samples,all_enabled_errors,n_tinit);
      $display("INITIAL ROM PASS: original payload, CPU manifold ownership, 15 task initializations, all six error sources enabled for %0d samples without error",all_enabled_samples);
    end
    // ...and a LOWER floor once the machine is initialised enough to do real
    // work. The runcycles/20 figure assumes roughly one microinstruction per
    // SYSPER fabric cycles with nothing stalling; a machine actually issuing
    // memory references legitimately issues FEWER. With `+tpcinit` the fault
    // handler runs BEGINENUMERATEMAP and IWRITEMAPFLAGS, which reference
    // storage, and the count falls to 16,676 of a 25,000 ceiling while `Stop`
    // stays clear and `Hold` is asserted on only 96 samples of 400,000. That is
    // work, not a stall, so the floor is halved rather than the machine
    // declared broken.
    // ...and lower again for the FULL BOOT: through TASKINITLOOP the machine
    // issues one instruction per ~20.3 fabric cycles (14,768,760 edges in
    // 300M -- real task-init work with holds, Stop clear throughout), which
    // sat just under the /20 floor and killed the run BEFORE the milestone
    // report. /25 for +bootchain, the same measured-reason widening as the
    // two above.
    if (n0a < (($test$plusargs("tpcinit")) ? runcycles/40 :
               ($test$plusargs("bootchain")) ? runcycles/25 : runcycles/20))
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
    // +worldms: the same kind of milestone report for a WORLD started at its
    // own entry, rather than for Initial. Addresses are AEmu.mb's, read from
    // `mbdis -y` (whose `addr=` is the IMAGE index) through im_image's
    // image->real mapping -- equivalently the low twelve bits of the fourth
    // word mbdis prints, which is where the real address lives.
    if ($test$plusargs("worldms")) begin
      $display("tb_exec: WORLD MILESTONES -- START(000)=%0d STARTEMULATOR(25b)=%0d INITHRAM(7d0)=%0d LOADHRAMLOOP(7ca)=%0d RESETDISPLAYCONFIG(78f)=%0d DWTINITPC(740)=%0d DWTSTART(74e)=%0d DHTINITPC(760)=%0d NEWDCB(3d5)=%0d",
               visited[12'h000], visited[12'h25b], visited[12'h7d0], visited[12'h7ca],
               visited[12'h78f], visited[12'h740], visited[12'h74e], visited[12'h760],
               visited[12'h3d5]);
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
      $display("tb_exec: BOOTFF -- IfuReset first@%0d (%0d edges), LdMcr' first@%0d (%0d edges), MDhold first@%0d, DisHold first@%0d, NoRef high %0d",
               c_ifur, n_ifur, c_mcr, n_mcr, c_mdh, c_dish, n_noref1);
      if (fastwait) $display("tb_exec: FASTWAIT -- clipped LongWait %0d times", n_clip);
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
      // `+nostuck`: Initial's storage-init loop at c96 (0o6226, `T<-T+1,
      // Store<-T`) runs 65,536 times on one address by design, so past task
      // init this heuristic is a false alarm; the summaries below it and the
      // final blocks are what a long run is for.
      if (maxrun > 8 && !$test$plusargs("nostuck"))
        $fatal(1, "the boot chain is stuck: %0d microinstructions on one address", maxrun);
      $display("tb_exec: PARC'S BOOT CHAIN RUNS -- Initial calls Bootstrap, which waits for the control processor.");
`ifdef FULL
      if (check_memconfig && !memconfig_checked)
        $fatal(1, "GetMemConfig probe never reached its result check");
      if (check_disk_format && !disk_format_checked)
        $fatal(1, "InitRamDiablo probe never reached its result check");
      if (check_reset_tags) begin
        if (reset_tag_done != 2'b11 || reset_tag_pending != 0)
          $fatal(1, "ResetTags did not complete both references (done=%b pending=%b)", reset_tag_done, reset_tag_pending);
        $display("tb_exec: RESET-TAGS PASS -- Store and Fetch each wrote their Md completion tag");
      end
`endif
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
    $display("tb_exec: L10 EDGES -- %0d detected; PE' low at %0d of them; D(preHSync)=1 at %0d of those",
             n_l10edge, n_l10pe0, n_l10d1);
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
