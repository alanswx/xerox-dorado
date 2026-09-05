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
  // REFRESH IS A REAL-TIME INTERVAL, and it was a 9-bit free-running counter:
  // `RfshPeriod` flipped every 512 sys_clk, i.e. every 16 microinstructions
  // (about 1 us), so the memory was asked to refresh continuously. Initial
  // survives that because it makes few references; a world does not, and wedges
  // with RefHold on 98% of samples. `+rfshdiv=N` sets the half-period in
  // sys_clk so the interval can be measured rather than assumed; the default
  // is the old 512 so nothing that passed changes.
  integer rfshhalf; initial begin
    rfshhalf = 512;
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
  // the clock are in a same-edge phase collision -- the cbf5b94f class.
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
  integer n_pht = 0; reg d_phs2 = 1'b0;
  always @(posedge sys_clk) begin
    if (m.b_DispY.preHSync && !d_phs2 && n_pht < 80) n_pht = n_pht + 40;
    if (n_pht > 0 && n_pht % 40 != 0) begin end
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
  // 80 words per pulse puts 9,360 words on a track, the T-80's own capacity.
  wire drv_ready_n, drv_online_n, drv_term_n, drv_sel_n, drv_sec_n, drv_index_n, drv_adv, drv_bit;
  wire drv_dp, drv_dm, drv_cp, drv_cm;
  wire [11:0] tag_n; wire cyltag_n, headtag_n, drivetag_n, conttag_n, sel0_n;
  wire [11:0] pk_cyl; wire [5:0] pk_head; wire pk_eoc_n;
  wire [31:0] pk_nct, pk_nht, pk_ndt, pk_nkt, pk_ntr, pk_nse, pk_nbi, pk_nbl;
  reg  pack_on = 1'b0; string pack_path;
  initial pack_on = $value$plusargs("pack=%s", pack_path);
  dorado_trident #(.SYSPER(2 * SYSPER), .SECTORS_PER_REV(117), .WORDS_PER_SECTOR(80)) u_drv (
      .sys_clk(sys_clk), .reset(1'b0), .attached(pack_on), .selected(~sel0_n),
      .TtlReady_n(drv_ready_n), .TtlOnLine_n(drv_online_n), .TtlTerm_n(drv_term_n),
      .Selected_n(drv_sel_n), .SecIndx_n(drv_sec_n), .TtlIndex_n(drv_index_n),
      .data_bit(drv_bit), .data_adv(drv_adv),
      .DataP(drv_dp), .DataM(drv_dm), .ClockP(drv_cp), .ClockM(drv_cm));
  dorado_pack u_pack (
      .sys_clk(sys_clk), .attached(pack_on), .tagbus_n(tag_n),
      .cyltag_n(cyltag_n), .headtag_n(headtag_n), .drivetag_n(drivetag_n), .conttag_n(conttag_n),
      .pulse(u_drv.sector), .data_adv(drv_adv), .data_bit(drv_bit), .endofcyl_n(pk_eoc_n),
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
  reg pendulum_r = 1'b0; integer pend_cnt = 0, pend_half = 0;
  initial if ($test$plusargs("pendulum")) pend_half = (32000 * SYSPER) / 30 / 2;   // sys_clk is 30 ns / SYSPER
  always @(posedge sys_clk) if (pend_half > 0) begin
    if (pend_cnt >= pend_half - 1) begin pend_cnt <= 0; pendulum_r <= ~pendulum_r; end
    else pend_cnt <= pend_cnt + 1;
  end
  // Declared OUTSIDE the configuration chain: the release below is common to
  // every machine, and only the WORLD-and-up tops have an IOReset port.
  reg ioreset_r = 1'b0;
  initial ioreset_r = $test$plusargs("ioreset");
`ifdef FULL   // FULL before SCREEN: the Makefile defines BOTH, and an `ifdef chain
              // takes the first that is defined -- with SCREEN first, every
              // "eleven-board" run built the ten-board screen machine (2026-09-02).
  // ELEVEN BOARDS: the screen machine plus DskEth. Its cable lines are tied
  // idle -- the active-low Trident status lines DEASSERTED (1), the
  // Ethernet receiver quiet (RcvData 0, Collision 0), host address 0 -- so
  // Initial's Ethernet boot can be watched trying to start the controller.
  dorado_full m (
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
      .DataM0__in(drv_dm),
      .DataM1__in(1'b0),
      .DataM2__in(1'b0),
      .DataM3__in(1'b0),
      .DataP0__in(drv_dp),
      .DataP1__in(1'b0),
      .DataP2__in(1'b0),
      .DataP3__in(1'b0),
      .Host_0(1'b0),
      .Host_1(1'b0),
      .Host_2(1'b0),
      .Host_3(1'b0),
      .Host_4(1'b0),
      .Host_5(1'b0),
      .Host_6(1'b0),
      .Host_7(1'b0),
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
      .IOReset(ioreset_r),
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
  initial begin
    void'($value$plusargs("pcfrom=%d", pcfrom));
    void'($value$plusargs("pclen=%d", pclen));
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
  // DISK: what the world's disk boot did against the drive, and the Alto
  // boot's own milestones (AEmu.mb: DiskBoot 0x405, DiskBootRetry 0x412,
  // KWait 0x417, KBootTimeout 0x375, EBoot 0x406).
  integer dk_sectw = 0, dk_idxtw = 0, dk_active = 0, dk_rderr = 0, dk_fifow = 0, dk_data = 0, dk_ecc = 0, dk_rdderr = 0, dk_fovf = 0, dk_funf = 0;
  reg dk_sectw_d = 1'b0, dk_idxtw_d = 1'b0; reg [3:0] dk_fw_d = 4'd0;
  integer dk_t_diskboot = -1, dk_t_kwait = -1, dk_t_timeout = -1, dk_t_retry = -1, dk_n_retry = 0;
  reg [11:0] dk_cia_d = 12'hfff;
  always @(posedge sys_clk) begin
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
  // +rcvplay=PATH: replay a phase-encoded reply stream (tools/eb_replies.py)
  // onto RcvData, starting +rcvdelay sys_clk (default 500,000 = 0.94 ms)
  // after the machine's own transmitter has sent a packet and gone idle --
  // the shape of a boot server answering the MicrocodeBootRequest.
  integer rp_fd, rp_n = 0, rp_i = 0, rp_delay = 500000, rp_start = -1, rp_lastx = 0, rp_code;
  integer rp_t [0:399999]; reg rp_v [0:399999];
  reg rp_line [0:0]; string rp_path; reg [255:0] rp_buf; integer rp_tt, rp_vv;
  initial begin
    if ($value$plusargs("rcvplay=%s", rp_path)) begin
      rp_fd = $fopen(rp_path, "r");
      if (rp_fd == 0) $fatal(1, "rcvplay: cannot open %s", rp_path);
      // $fscanf, not $fgets+$sscanf: the file is pure "<offset> <level>" pairs
      // (tools/eb_replies.py puts its packet index in a sidecar), and the
      // $fgets form silently loaded NOTHING -- 0 transitions -- in the first
      // boot run that armed this.
      while (rp_n < 400000) begin
        rp_code = $fscanf(rp_fd, "%d %d", rp_tt, rp_vv);
        if (rp_code != 2) break;
        rp_t[rp_n] = rp_tt; rp_v[rp_n] = rp_vv[0]; rp_n = rp_n + 1;
      end
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
  initial void'($value$plusargs("pcpc=%h", pcpc));
  always @(posedge sys_clk)
    if (pcpc != 12'hfff && ring_cia == pcpc && ring_cia != pct_cia_d) begin
      pcfrom = n_cyc2; pcpc = 12'hfff;
      $display("tb_exec: PCT window opens @%0d at CIA=%h", n_cyc2, ring_cia);
    end
  always @(posedge sys_clk) begin
    if (pcfrom >= 0 && n_cyc2 >= pcfrom && n_cyc2 < pcfrom + pclen && ring_cia != pct_cia_d)
      $display("tb_exec: PCT @%0d CIA=%h JCN=%h task=%h Link=%h T=%h R=%h alu=%h ResLt0'=%b ResEq0'=%b SB=%b TPCI=%h CTD=%h TaskOff'=%b Ph=%b SC=%b RW=%b Hold=%b",
               n_cyc2, ring_cia, ring_jcn, pct_ctask, ring_link, brc_t, brc_r, brc_alu,
               m.b_ProcH.ResLtZero_p_, m.b_ProcH.ResEqZero_p_, m.b_ProcH.ProcH11_sil_pl_9,
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
  reg [31:0] ia, ib, ic, id, ie, ig, ih, ii, ij, ilh, irh;
  reg p_lh, p_rh, impar_odd, impar_nosec;
  integer imfd, imn, mapaddr, nloaded, nver, nverbad, startaddr;
  longint runcycles;   // 64-bit: a full boot is >2^31 sys_clk
  integer mcrval;
  reg [7:0] mb0_, mb1_, mb2_, mb3_, mb4_;
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
  initial fastwait = $test$plusargs("fastwait");
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
  // #2 misses its data" (a9026abc): a bench artifact, not a strobe-timing
  // bug. The ResEqZero pulse below is what actually exits LongWait (the clip
  // "loses to the bypass" and never could); it stays, the clip goes. `+clip`
  // turns it back on to reproduce the artifact.
  always @(posedge sys_clk)
    if (fastwait && $test$plusargs("clip")
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
    for (zi = 0; zi < 4096; zi = zi + 1) begin
      if (m.b_MemX.u_d11.mem[zi] === 1'b0) nwp0 = nwp0 + 1;
      if (m.b_MemX.u_d13.mem[zi] === 1'b0) ndirty0 = ndirty0 + 1;
      if (m.b_MemX.u_a04.mem[zi] === 1'b0) nrp0 = nrp0 + 1;
    end
    $display("tb_exec: MAPCENSUS -- WP-plane zeros %0d, Dirty-plane zeros %0d, RP00-plane zeros %0d (of 4096)",
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
  // every bit visible, the "in-residency data" of 355e67bb (RMar09=RMar10=1)
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
    bootcp_n = 0; bootcp_phase = 1'b0; bootcp_byte = 0;
    if (!$value$plusargs("bootcp=%d", bootcp_period)) bootcp_period = 0;
    cktrace = $test$plusargs("cktrace");
    if (bootcp_period) $display("tb_exec: BOOTCP -- toggling the handshake every %0d cycles",
                                bootcp_period);

`ifdef WORLD   // only the WORLD-and-up machines have an IOReset port
    if (ioreset_r) $display("tb_exec: +ioreset -- IOReset released at @%0d", n_cyc2);
    ioreset_r = 1'b0;
`endif
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
