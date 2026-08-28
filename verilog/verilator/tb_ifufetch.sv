// tb_ifufetch -- THE IFU COMES OUT OF HOLD WHEN REAL MICROCODE RUNS.
//
// Derived from tb_memrun, deliberately and wholesale: that bench already has
// the machinery to walk real `AEmu.mb!2` microcode into IM through the
// control-processor bus and execute it, which is a lot of apparatus to
// reproduce. What it does NOT share is the subject -- tb_memrun's assertions
// are about the Flush and the storage path, and they are relaxed to displays
// here. The IFU is what this one is about.
//
// The machine is `dorado_ifu`: ContA, ContB, ProcH, ProcL, MemC, MemD, MemX
// and the IFU. Note `CLK_ifu'` is driven -- the IFU has its own slot clock,
// and leaving it undriven gives the board zero local edges, which looks like
// a gating bug and is not one. That trap has now been hit on three separate
// boards.
//
// WHAT THIS SHOWS.
//
// `IfuHold` is the handshake: the IFU asks with `WantIfuHold'` and MemC
// grants with `IfuHold` (MemC d23.4). In the STATIC bench -- ifu-test, no
// microcode -- it reads 1, the IFU held. With real microcode executing it is
// released on 3000 of 3000 samples. That is the rung this gate keeps.
//
// WHAT IT DOES NOT SHOW, AND WHY THAT MATTERS FOR THE ORDER OF WORK.
//
// The loop issues an `IFetch<-` (IM[3]: ASEL = 001, ff01 = 2, so FF = 0o200 --
// the encoding refdecode-test pinned against cpu.c's DM_REF_IFETCH), and the
// IFU still never receives an opcode: over the run `J` takes ONE distinct
// value and so does `IfuData`. The IFU is enabled and idle, not fetching.
//
// It cannot fetch, because MEMORY CANNOT SERVE THE REFERENCE YET. The same
// run reports `MemWEa` 0 and the storage cycle not completing -- which is
// exactly where the memory work stopped. So the IFU's fetch rung is NOT an
// independent task: it is downstream of the memory section serving a real
// reference. Do that first; this gate will start moving on its own when it
// lands, and the count of distinct `J` values is the thing to watch.
//
// PROGRESS 2026-08-23: THE BYTE-STREAM PATH IS TRACED AND WIRED, AND THE
// BLOCKER IS NARROWER AND NAMED.
//
// The IFU's instruction bytes come off the CACHE, every hop on MemD:
//
//   cache D.00-03,16,17 -> e06 (F register, clocked by Fclk'a)
//                       -> f22 (G register, loaded from F on GLd')
//                       -> f23 (MC10174 dual 4:1 selecting G.00/G.08/F.00/
//                          F.08 under PcFG.15 and GDv', enable EnableFG')
//                       -> FG.0-8 -> the IFU
//
// (`IfuData` is NOT the way in -- it is generated ON the IFU by f05 from
// `AlphaX`, and goes OUT to the processor. That cost a detour.)
//
// The cache is seeded here the way readback-test seeds it, so it answers the
// same word at every address. Measured with that in place:
//
//   D.00 high on 253,038 samples   -- the cache IS producing the seeded data
//   EnableFG' high on 0            -- the FG mux is enabled throughout
//   Fclk'a RISING edges 3, of which with D.00 high: 0
//   F.00 high 0, G.00 high 0, FG.0 edges 0
//   THE PIPE POINTER MOVED 0 TIMES  -- against 9 in tb_memrun
//
// That last line is the real one. The data path is wired and live from the
// cache to F's D inputs; what is missing is that THIS BENCH RECORDS NO
// MEMORY REFERENCE AT ALL, so F's clock barely ticks and never while data is
// present.
//
// AND IT IS `UseAsrn`, GATED BY THE IFU'S OWN `IfuAckIfHit'`.
//
// RETRACTED FIRST: there is NO clock difference. An earlier version of this
// header said MemC's clk0'A ran "379x too fast" with the IFU present. That
// was a MEASUREMENT ARTEFACT, twice over -- see the methodology note below.
// With both benches counting free-running over the same 272,747 sys_clk,
// every hop of the clock chain is IDENTICAL in the two machines:
//
//     CLK.mc' edges     34093 = 34093      MemClkEnable'a high  41 = 41
//     CLKEnable'b high 262953 = 262953     ppclk2'a edges    34089 = 34089
//     preSH'x edges     17045 = 17045      preClk0'B edges   17045 = 17045
//     k02's clk0'A      17045 = 17045
//
// WHAT ACTUALLY DIFFERS IS THE MUX SELECT:
//
//                          tb_memrun         tb_ifufetch
//     UseAsrn high      270,089 (99%)        41 (0.015%)
//     Asrn changed            8                 9
//     PEsrn changed           0 (0000)          0 (0000)
//     dPipe02Ad changed      14                 2
//     Pipe pointer moved     11                 0
//
// MemX h23 selects Asrn or PEsrn on UseAsrn. With the IFU in the machine the
// mux sits on PEsrn, which never leaves 0000, so the pointer is PINNED -- not
// stalled, and nothing about the clock or the reference machinery is wrong.
//
// AND UseAsrn IS DRIVEN BY MemC f24, AN MC10121 4-WIDE OR-AND WHOSE `e` INPUT
// GROUP IS PINS 4,5,6 = IfuAckIfHit', (open), Hit'a. So an IFU SIGNAL gates
// which source the Pipe pointer takes -- which is why the difference appears
// exactly when the IFU board is added and nowhere else.
//
// AND IT IS NOT A BUG -- IT IS THIS BENCH BEING RIGHT AND tb_memrun BEING
// WRONG. The two SRN sources are named in the netlist and they match the C
// emulator's model exactly, from an independent derivation:
//
//   PEsrn <- MemX h10, an MC10158 2:1 selecting ProcSrn or Ec1Srn on
//            EcWantsA -- i.e. the PROCESSOR's SRN path
//   Asrn  <- MemX f19, an F10016 COUNTER -- the auto-advancing I/O ring
//
//   include/memory.h:  proc_srn "4-bit; emulator + fault tasks. Default 0."
//                      asrn     "4-bit; I/O ring. 2..15. Default 2."
//
// And docs/memory-architecture.md states the selection rule, derived from the
// microcode and the manual with no reference to this RTL: ASRN is used for
// IOFetch, IOStore and PreFetch-WITH-MISS; EVERYTHING ELSE USES ProcSRN.
//
// This loop issues <-Map, Store and Flush. None of them is an I/O reference
// or a prefetch, so ALL of them must use ProcSRN -- UseAsrn LOW. That is what
// this bench measures (0.015%) and NOT what tb_memrun measures (99%).
//
// The Pipe pointer follows: ProcSRN is a FIXED slot, so "successive Fetches
// all overwrite the same ProcSRN slot" (memory-architecture.md again) and the
// pointer MUST NOT MOVE. tb_ifufetch's 0 is correct; tb_memrun's 11 is the
// anomaly, and it happens because without the IFU board `IfuAckIfHit'` is
// undriven and reads 0 in the OR tree, which frees UseAsrn to sit high.
//
// So the IFU does not break the memory section. Adding it DRIVES an input
// that was floating, and the Pipe source select becomes correct.
//
// METHODOLOGY, AND IT COST TWO WRONG COMMITS. The "379x" came from counters
// placed in DIFFERENT CONTEXTS -- free-running here, inside the window-gated
// sampling loop in tb_memrun -- so they ran over 272,747 and 704 sys_clk and
// the raw counts were never comparable. Worse, the first "correction" moved
// only PART of the block and left n_k02 behind, so the retraction appeared to
// confirm the artefact. ANY CROSS-BENCH COMPARISON OF A RAW COUNT NEEDS ITS
// DENOMINATOR PRINTED BESIDE IT, and when you move a probe, move ALL of it.
//
// (The earlier framing, kept because the reasoning is still useful:)
// THE CAUSE IS THE IFU BOARD ITSELF, NOT THE MICROCODE. That was
// isolated by making this bench's loop BYTE-IDENTICAL to tb_memrun's -- the
// same four instructions, <-Map / Store / Flush / quiet slot, IM[2] back to
// FF = 0o100:
//
//                          tb_memrun      tb_ifufetch (identical loop)
//   MapFnc.0' / MapFnc.1'    0 / 0            0 / 0     <- now matches
//   Pipe pointer moved       9                0
//   MemRASa                  2                0
//
// Same microcode, same seven boards, and the ONLY difference is that
// `dorado_ifu` adds the IFU. With it in the machine the processor's
// references stop landing in the Pipe altogether. `IfuHold` is released on
// 3000 of 3000 samples, so the IFU is free-running and prefetching, and the
// obvious suspicion is that it is taking the memory section's attention --
// but that is a suspicion, not a measurement, and the next step is to find
// what the IFU asserts that stops a processor reference being recorded.
//
// Two intermediate results on the way, both dead ends worth not repeating:
//   - Moving the IFetch<- from IM[3] to IM[2] and restoring the quiet slot
//     does not help; MemRASa goes 0 and the Pipe still does not move.
//   - Four references back to back (IFetch in IM[3], no quiet slot) is worse
//     still: MapFnc never even leaves 1/1, so no map function is requested.
//     The quiet slot is load-bearing here exactly as it is in tb_memrun.
//
// THE LOOP IS LEFT IDENTICAL TO tb_memrun's on purpose, because that is the
// clean demonstration. Putting the IFetch<- back is one FF field:
// IM[2] 0o100 -> 0o200.


`default_nettype none


module tb_ifufetch;
  integer n_ua, n_asrn, n_pesrn, n_k02, n_dpipe, n_hoip, n_brhi, n_hold, n_tot, n_pp, n_sh, n_pc, n_mc, n_mce, n_ceb;
  reg pp_d, sh_d, pc_d, mc_d;
  reg k02clk_d; reg [3:0] dpipe_now, dpipe_last;
  reg [3:0] asrn_now, asrn_last, pesrn_now, pesrn_last;


  localparam integer GAP = (200 * SYSPER) / 16;   // sys_clk between Control strobes

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;
  reg [3:0] ckd = 4'd0;
  // SYS CLOCKS PER MICROINSTRUCTION. The cells recover each distributed ECL
  // clock net by oversampling it, and this is how finely. Every fixed wait
  // below is scaled by it, so a wait means the same amount of DORADO time
  // whatever the ratio -- otherwise halving the ratio doubles how much of the
  // machine's execution each `repeat` covers, and a bench that samples at a
  // chosen moment samples somewhere else entirely.
  // THE OVERSAMPLING RATIO IS NOW OVERRIDABLE, and it is passed DOWN.
  //
  // These benches have always had a local SYSPER for scaling their own waits,
  // and the machine has always had its own `parameter integer SYSPER = 16`.
  // The two were never connected -- they simply both happened to say 16. So a
  // bench could not test the machine at a different ratio at all.
  //
  // That matters for the FPGA: real time needs sys_clk = SYSPER x 16.67 MHz,
  // measured Fmax is 48.99 MHz, so SYSPER=2 is the target and 16 is 0.18x.
  // Lowering it stresses cell fidelity -- one bug "survived at 16x only
  // because the pulse happened to sit inside one stable address" -- which is
  // exactly what these gates exist to catch.
  //
  //     make -C verilog/verilator <gate> SYSPER=8
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
      WT = (n * SYSPER) / 16;
      if (WT < 1) WT = 1;
    end
  endfunction

  // THE MACHINE CLOCK, AT THE OVERSAMPLING RATIO. This was `ckd[3]` of a
  // free-running 4-bit counter -- a hard-coded divide-by-16 that ignored
  // SYSPER entirely. SYSPER reaches only `cell_CLOCKGEN`, which lives on the
  // BaseBoard, and none of the sub-machines (dorado_mem/_ifu/_proc/_control)
  // contains one: their CLK_* ports are inputs marked "awaits BaseBd" and the
  // bench drives them. So lowering SYSPER compressed the BENCH's stimulus
  // while the machine's clock stayed at 16 sys_clk per microinstruction --
  // which is a different experiment, not a lower ratio.
  //
  // One microinstruction is SYSPER sys_clk now, which is what N means in
  // cell_CLOCKGEN. At SYSPER=16 this is bit for bit the counter it replaces:
  // ckd counts 0..15 and mclk is ckd >= 8, i.e. ckd[3].
  always @(posedge sys_clk)
    ckd <= (ckd == SYSPER - 1) ? 4'd0 : ckd + 4'd1;
  wire mclk = (ckd >= SYSPER / 2);

  reg [2:0] addr_n = 3'b111;
  reg [8:0] cpout  = 9'd0;
  reg       strb_n = 1'b1;
  reg       setrun = 1'b0, setss_n = 1'b1;
  reg       dmd = 1'b0, dmc = 1'b0, udmd = 1'b0;

  reg [15:1] req = 15'd0;

  // THE REFRESH TICK, which the BASEBOARD generates and this seven-board
  // machine therefore lacks. BaseBd a01 is an MC10176 hex flip-flop clocked by
  // clk0'A that produces RfshPeriod, SetRunRfsh and Pendulum from the 6502
  // side; dorado_mem lists RfshPeriod as an input "awaits BaseBd". Undriven it
  // reads 0 for ever, MemX's refresh counter (f18, an F10016) never ticks, and
  // RAS never toggles -- which is exactly what was measured before this was
  // driven.
  reg [8:0] rfshdiv = 9'd0;
  reg       rfshper = 1'b0;
  always @(posedge sys_clk) begin
    rfshdiv <= rfshdiv + 9'd1;
    if (rfshdiv == 9'd0) rfshper <= ~rfshper;
  end

  reg chips16k = 1'b1, chips64k = 1'b0;
  dorado_ifu #(.SYSPER(SYSPER)) m (
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      // The BaseBoard fans the clock to EVERY slot, and dorado_mem has three
      // more than dorado_proc. Leaving them undriven is why MemC's local clock
      // showed zero edges with both its enables already asserted.
      .CLK_mc_p_(mclk), .CLK_md_p_(mclk), .CLK_mx_p_(mclk),
      .CLK_ifu_p_(mclk),   // the IFU's own slot clock
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n),
      .SetRunRfsh(1'b1), .RfshPeriod(rfshper),
      // THE MEMORY SIZE IS A BACKPLANE INPUT, not a board property: MemX
      // takes ChipsAre256/16K and ChipsAre64K from the MSA, which is not in
      // this configuration, so the bench must supply them the way it already
      // supplies the clocks. They are the CHIP ENABLES on the two DRAM
      // TIMING PROMs -- j13 (Use256/16KProm') and j14 (Use64KProm') -- so
      // with both undriven NEITHER is enabled and the memory state machine
      // has no timing table at all. MemProms.bcpl records the build:
      // "change tomemx-16k-j13 from -j14. comment-out the memx-4k option.
      // September 26, 1979", i.e. from late 1979 the machines carry 16K
      // parts and only that PROM is blown.
      .ChipsAre256_s_16K(chips16k), .ChipsAre64K(chips64k)
  );

  // SEED THE CACHE, the lever this bench's header predicted. The IFU's
  // instruction bytes come off the cache, every hop on MemD:
  //
  //   cache D.00-03,16,17 -> e06 (F register, clocked by Fclk'a)
  //                       -> f22 (G register, loaded from F on GLd')
  //                       -> f23 (MC10174 dual 4:1, selects G.00/G.08/F.00/
  //                          F.08 under PcFG.15 and GDv', enabled EnableFG')
  //                       -> FG.0-8 -> the IFU
  //
  // Filling every cell of all 36 D-driving F10470s makes the cache answer the
  // same word at every address, so whatever the IFU prefetches it gets this
  // pattern -- the same trick readback-test uses, and it removes the need to
  // land a fill at a known address first.
  localparam [17:0] CPAT = 18'd52045;   // CPAT[k] is D.k
  integer ci;
  initial begin
    for (ci = 0; ci < 4096; ci = ci + 1) begin
      m.b_MemD.u_a03.mem[ci] = CPAT[0];
      m.b_MemD.u_d03.mem[ci] = CPAT[0];
      m.b_MemD.u_a05.mem[ci] = CPAT[1];
      m.b_MemD.u_d05.mem[ci] = CPAT[1];
      m.b_MemD.u_g03.mem[ci] = CPAT[2];
      m.b_MemD.u_j03.mem[ci] = CPAT[2];
      m.b_MemD.u_g05.mem[ci] = CPAT[3];
      m.b_MemD.u_j05.mem[ci] = CPAT[3];
      m.b_MemD.u_a13.mem[ci] = CPAT[4];
      m.b_MemD.u_d13.mem[ci] = CPAT[4];
      m.b_MemD.u_a15.mem[ci] = CPAT[5];
      m.b_MemD.u_d15.mem[ci] = CPAT[5];
      m.b_MemD.u_g13.mem[ci] = CPAT[6];
      m.b_MemD.u_j13.mem[ci] = CPAT[6];
      m.b_MemD.u_g15.mem[ci] = CPAT[7];
      m.b_MemD.u_j15.mem[ci] = CPAT[7];
      m.b_MemD.u_a09.mem[ci] = CPAT[8];
      m.b_MemD.u_d09.mem[ci] = CPAT[8];
      m.b_MemD.u_a11.mem[ci] = CPAT[9];
      m.b_MemD.u_d11.mem[ci] = CPAT[9];
      m.b_MemD.u_g09.mem[ci] = CPAT[10];
      m.b_MemD.u_j09.mem[ci] = CPAT[10];
      m.b_MemD.u_g11.mem[ci] = CPAT[11];
      m.b_MemD.u_j11.mem[ci] = CPAT[11];
      m.b_MemD.u_a17.mem[ci] = CPAT[12];
      m.b_MemD.u_d17.mem[ci] = CPAT[12];
      m.b_MemD.u_a19.mem[ci] = CPAT[13];
      m.b_MemD.u_d19.mem[ci] = CPAT[13];
      m.b_MemD.u_g17.mem[ci] = CPAT[14];
      m.b_MemD.u_j17.mem[ci] = CPAT[14];
      m.b_MemD.u_g19.mem[ci] = CPAT[15];
      m.b_MemD.u_j19.mem[ci] = CPAT[15];
      m.b_MemD.u_a07.mem[ci] = CPAT[16];
      m.b_MemD.u_d07.mem[ci] = CPAT[16];
      m.b_MemD.u_g07.mem[ci] = CPAT[17];
      m.b_MemD.u_j07.mem[ci] = CPAT[17];
    end
  end

  // WATCH THE BYTE STREAM ITSELF, not just the IFU's outputs.
  integer n_fg, n_fgnz, n_f, n_g, n_d, n_fclk, n_enfg, n_fclk_r, n_fclk_coin;
  integer n_wir, n_ro, n_ifra, n_wpr, n_both;
  reg fclk_d;
  reg [8:0] fg_seen, fg_last;

  reg [17:0] f_seen, g_seen;
  initial begin n_ua=0; n_asrn=0; n_pesrn=0; n_k02=0; n_dpipe=0; n_hoip=0; n_brhi=0; n_hold=0; n_tot=0; n_pp=0; n_sh=0; n_pc=0; n_mc=0; n_mce=0; n_ceb=0;
    pp_d=1'bx; sh_d=1'bx; pc_d=1'bx; mc_d=1'bx; k02clk_d=1'bx; dpipe_last=4'bx; asrn_last=4'bx; pesrn_last=4'bx; n_fg=0; n_fgnz=0; n_f=0; n_g=0; n_d=0; n_fclk=0; n_enfg=0; n_fclk_r=0; n_fclk_coin=0; n_wir=0; n_ro=0; n_ifra=0; n_wpr=0; n_both=0; fclk_d=1'bx; fg_last=9'bx; fg_seen=9'd0; f_seen=0; g_seen=0; end
  always @(posedge sys_clk) begin
    if (m.FG_0 !== fg_last[0]) begin n_fg = n_fg + 1; fg_last[0] = m.FG_0; end
    if (m.b_MemD.F_00) n_f = n_f + 1;
    if (m.b_MemD.G_00) n_g = n_g + 1;
    // Is the cache producing anything, and does the F register's clock tick?
    // F is e06 (MC10176) clocked by Fclk'a; a dead clock holds Q forever.
    if (m.b_MemD.D_00) n_d = n_d + 1;
    if (m.b_MemD.Fclk_p_a && !fclk_d) begin
      n_fclk_r = n_fclk_r + 1;
      if (m.b_MemD.D_00) n_fclk_coin = n_fclk_coin + 1;   // COUNT THE COINCIDENCE
    end
    if (m.b_MemD.Fclk_p_a !== fclk_d) begin n_fclk = n_fclk + 1; fclk_d = m.b_MemD.Fclk_p_a; end
    if (m.b_MemD.EnableFG_p_) n_enfg = n_enfg + 1;
    // THE PIPE POINTER'S SOURCE. PipeAd <- k02 (MC10141, parallel load every
    // clk0'A) <- dPipe02Ad <- MemX h23, a quad 2:1 mux selecting Asrn (the
    // A-slot storage reference number) or PEsrn under UseAsrn. If the pointer
    // never moves, one of those three is static.
    if (m.b_MemX.UseAsrn) n_ua = n_ua + 1;
    // WHICH GATING TERM. MemC j08 (SE10210) makes
    //     clk0'A = preClk0'B | HoldOrIP | BrHi_'
    // so the clock is HELD OFF while either qualifier is high. A clock with
    // 379x the edges means a qualifier that is high far less often.
    if (m.b_MemC.HoldOrIP) n_hoip = n_hoip + 1;
    if (m.b_MemC.BrHi_u__p_) n_brhi = n_brhi + 1;
    if (m.b_MemC.Hold)     n_hold = n_hold + 1;
    n_tot = n_tot + 1;   // TOTAL sys_clk -- these counters are NOT window-gated
    // THE CLOCK CHAIN, one hop at a time, EDGES not levels:
    //   preClk0'B <- h13 from ppclk2'a and preSH'x
    //   ppclk2'a  <- l01 from MemClkEnable'a, CLK.mc', CLKEnable'b
    // Whichever of these already differs is where the IFU's effect enters.
    if (m.b_MemC.ppclk2_p_a  !== pp_d)  begin n_pp  = n_pp  + 1; pp_d  = m.b_MemC.ppclk2_p_a;  end
    if (m.b_MemC.preSH_p_x   !== sh_d)  begin n_sh  = n_sh  + 1; sh_d  = m.b_MemC.preSH_p_x;   end
    if (m.b_MemC.preClk0_p_B !== pc_d)  begin n_pc  = n_pc  + 1; pc_d  = m.b_MemC.preClk0_p_B; end
    if (m.b_MemC.CLK_mc_p_   !== mc_d)  begin n_mc  = n_mc  + 1; mc_d  = m.b_MemC.CLK_mc_p_;   end
    if (m.b_MemC.MemClkEnable_p_a) n_mce = n_mce + 1;
    if (m.b_MemC.CLKEnable_p_b)    n_ceb = n_ceb + 1;

    // ...and k02's OWN CLOCK. PipeAd is a parallel load on clk0'A (both
    // select pins open = load), so a dead clock holds the pointer whatever
    // its source does.
    if (m.b_MemC.clk0_p_A !== k02clk_d) begin n_k02 = n_k02 + 1; k02clk_d = m.b_MemC.clk0_p_A; end
    dpipe_now = {m.b_MemC.dPipe02Ad_0, m.b_MemC.dPipe02Ad_1, m.b_MemC.dPipe02Ad_2, m.b_MemC.dPipe02Ad_3};
    if (dpipe_now !== dpipe_last) begin n_dpipe = n_dpipe + 1; dpipe_last = dpipe_now; end
    asrn_now  = {m.b_MemX.Asrn_0,  m.b_MemX.Asrn_1,  m.b_MemX.Asrn_2,  m.b_MemX.Asrn_3};
    pesrn_now = {m.b_MemX.PEsrn_0, m.b_MemX.PEsrn_1, m.b_MemX.PEsrn_2, m.b_MemX.PEsrn_3};
    if (asrn_now  !== asrn_last)  begin n_asrn  = n_asrn  + 1; asrn_last  = asrn_now;  end
    if (pesrn_now !== pesrn_last) begin n_pesrn = n_pesrn + 1; pesrn_last = pesrn_now; end

    // WHAT THE IFU ASSERTS INTO THE MEMORY SECTION. It drives MAR.00'-15'
    // (the address itself), WantIfuRef' and RefOutstanding' into MemC -- so
    // with the IFU in the machine it is COMPETING for the address and the
    // reference slot, which is the obvious way processor references could
    // stop being recorded.
    if (!m.b_MemC.WantIfuRef_p_)     n_wir  = n_wir  + 1;
    if (!m.b_MemC.RefOutstanding_p_) n_ro   = n_ro   + 1;
    if (m.b_MemC.IfuRefInA)          n_ifra = n_ifra + 1;
    if (!m.b_MemC.WantProcRef_p_)    n_wpr  = n_wpr  + 1;
    if (!m.b_MemC.WantIfuRef_p_ && !m.b_MemC.WantProcRef_p_) n_both = n_both + 1;
  end


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
  integer tk, tbad;
  reg [15:0] tpc15, tpc15b, tpc7;
  reg [19:0] link15, link15b, link7;
  integer nmemclk, kk, npipe, nras, ncas, nwe, nmx;
  reg prasa, pcasa, pwea, pmx, prp, pmr;
  integer nrp, nmr, nms, nsq, nsrc, nwr, nnr, nmrf, nsm, nmw, npsm, nwmw, ng13, nxsm, nwpr, nrh, nldp, npha, ncra, nha, nhb, nwcr, nwar, nfl, nmp;
  reg psq, psrc, pwr, pnr, pmrf, psm, pmw, ppsm, pwmw, pg13, pxsm, pwpr, prh, pldp, ppha, pcra, pha, phb, pwcr, pwar, pfl, pmp;
  reg [4:0] i14a; integer i14_hit [0:31];
  reg [3:0] memst_now; reg memst_hit [0:15]; integer runlen, maxrun, nwin, ndd, nidle_lo, winat;
  reg pmfree, pmidle; integer nmfree_e, nmidle_e, nmfree_hi;
  integer npsh, ncoin, nmwlo, lastpsh, lastcoin, lastmwlo, lastmf00;
  reg [1:0] mf_now; integer mf_cnt [0:3];
  reg [2:0] ms3_now; integer ms3_cnt [0:7]; integer ms3_last [0:7];
  reg pmf00, pms3; integer nmf00on, nms3on;
  integer mf00at [0:11]; integer ms3at [0:11];
  reg pdd;
  integer nmemst, nfree, nmemfr, nheld_nz;
  reg memfr_hit [0:15];
  reg [2:0] mapst_now; reg [1:0] mapfn_now;
  reg mapst_hit [0:7]; reg mapfn_hit [0:3];
  integer nmapst, nmapfn, npsm2, nsm2, nload, ncnt, nd0, nwim, nx10, nmti, nwm, nall3, nrw, nwp, ndty, nevn, nckw, nprf, nthi, nmt, nmtp, nmras, nmcas, nmrd, nmwr, nd13w;
  reg pmras, pmcas;
  reg [7:0] ifu_j, ifu_d;
  reg ifu_seen [0:255]; reg ifud_seen [0:255];
  integer nifu_j, nifu_d, nifu_free;
  integer ntnia, nff0, nsamp, nff0_wpr, nff0_cr, nff0_alt, nff0_fl, nff0_a1, nff0_ign, nff0_a0, nff0_ffok, nff0_bad, nff0_fs, nff0_fm, nff0_mia, nff0_mib, nff0_fsp, nff0_ech, nff0_st, nff0_hcd;
  reg tnia_hit [0:4095];
  reg [2:0] pms;
  reg [3:0] pipe_before;
  wire [2:0] mapst = {m.b_MemX.MapState_0, m.b_MemX.MapState_1, m.b_MemX.MapState_2};
  reg [3:0] ppa;
  wire [3:0] pipead = {m.b_MemC.PipeAd_0, m.b_MemC.PipeAd_1,
                       m.b_MemC.PipeAd_2, m.b_MemC.PipeAd_3};
  reg pmc;
  wire [2:0] asel = {m.ASEL_0, ~m.ASEL_1_p_, ~m.ASEL_2_p_};
  // MAR is carried across the backplane active low.
  wire [15:0] mar = ~{m.MAR_00_p_, m.MAR_01_p_, m.MAR_02_p_, m.MAR_03_p_,
                      m.MAR_04_p_, m.MAR_05_p_, m.MAR_06_p_, m.MAR_07_p_,
                      m.MAR_08_p_, m.MAR_09_p_, m.MAR_10_p_, m.MAR_11_p_,
                      m.MAR_12_p_, m.MAR_13_p_, m.MAR_14_p_, m.MAR_15_p_};
  wire [3:0] bnt  = {m.b_ContA.BNT_0,  m.b_ContA.BNT_1,
                     m.b_ContA.BNT_2,  m.b_ContA.BNT_3};
  wire [3:0] penc = {m.b_ContA.PEnc_0, m.b_ContA.PEnc_1,
                     m.b_ContA.PEnc_2, m.b_ContA.PEnc_3};
  wire [3:0] ctask = {m.b_ContA.CTask_0, m.b_ContA.CTask_1,
                      m.b_ContA.CTask_2, m.b_ContA.CTask_3};
  // TPCAd is active low: the address the per-task PC memory is reading.
  wire [3:0] tpcad = ~{m.b_ContA.TPCAd_0_p_, m.b_ContA.TPCAd_1_p_,
                       m.b_ContA.TPCAd_2_p_, m.b_ContA.TPCAd_3_p_};

  // THE PER-TASK PC MEMORY, read straight out of the array. Four F10145A
  // packages, sixteen words of four bits each: l13 holds TPC.00-03, i13
  // TPC.04-07, j13 TPC.08-11, k13 TPC.12-15, and the cell drives
  // {p14,p15,p1,p2} from q so q[0] is the LOW-numbered (most significant)
  // bit of each group. IM is 4096 words, so TPC.04-15 are the address and
  // TPC.00-03 sit above it.
  // The OTHER per-task register files, and their own task addresses. ContA
  // a04-e04 hold Link, addressed by `TLinkAd`; ProcH and ProcL carry files
  // addressed by `CurrLast'` and `LastNext'` -- two different task numbers,
  // because the Dorado pipelines the switch and the stages do not agree on
  // which task is current.
  wire [3:0] tlinkad  =  {m.b_ContA.TLinkAd_0, m.b_ContA.TLinkAd_1,
                          m.b_ContA.TLinkAd_2, m.b_ContA.TLinkAd_3};
  wire [3:0] currlast = ~{m.b_ProcH.CurrLast_0_p_, m.b_ProcH.CurrLast_1_p_,
                          m.b_ProcH.CurrLast_2_p_, m.b_ProcH.CurrLast_3_p_};
  wire [3:0] lastnext = ~{m.b_ProcH.LastNext_0_p_, m.b_ProcH.LastNext_1_p_,
                          m.b_ProcH.LastNext_2_p_, m.b_ProcH.LastNext_3_p_};

  function [15:0] tpc_of(input [3:0] t);
    tpc_of = {m.b_ContA.u_l13.mem[t], m.b_ContA.u_i13.mem[t],
              m.b_ContA.u_j13.mem[t], m.b_ContA.u_k13.mem[t]};
  endfunction

  // Link, all five packages. Their outputs are unnamed sheet-local nets, so
  // the bit order is not recoverable from names -- but the claim being tested
  // is that the storage is PER TASK, and for that the whole twenty bits
  // compared slot against slot is enough.
  function [19:0] link_of(input [3:0] t);
    link_of = {m.b_ContA.u_a04.mem[t], m.b_ContA.u_b04.mem[t],
               m.b_ContA.u_c04.mem[t], m.b_ContA.u_d04.mem[t],
               m.b_ContA.u_e04.mem[t]};
  endfunction
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
      strobe(3'd4, b1, b0[7]); strobe(3'd5, b2, b0[6]);
      strobe(3'd6, b3, b0[5]); strobe(3'd7, b4, b0[4]);
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
  // PARC loads T with the PLAIN SetCPReg and IM data with SetCPReg~, because
  // BMux is the complement of CPReg and `alub` is inverted back off it -- so T
  // ends up equal to CPReg while IM write data ends up equal to its
  // complement. TFromCPReg# therefore needs this form, not the tilde one.
  task set_cpreg_plain(input [15:0] v);
    begin
      strobe(3'd2, v[15:8], 1'b0);
      strobe(3'd3, v[7:0],  1'b0);
    end
  endtask

  task set_cpreg_tilde(input [15:0] v);
    begin
      strobe(3'd2, ~v[15:8], 1'b0);
      strobe(3'd3, ~v[7:0],  1'b0);
    end
  endtask

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

  // PARC's five-byte microinstruction layout, from doradoboot.masm's comment
  // in DoDoradoMicroInst. This encoder is validated in tb_compute against all
  // thirteen IRTable entries, byte for byte. None of those thirteen makes a
  // memory reference, so one has to be built.
  function [39:0] mi(input [3:0] rstk, input [3:0] aluf, input [2:0] bsel,
                     input [2:0] lc,   input [2:0] asel, input [7:0] ff,
                     input [7:0] jcn,  input block);
    reg [7:0] b0, b1, b2, b3, b4;
    begin
      // PARITY, COMPUTED. It used to be two hard-wired 1s, on the reasoning
      // that "a jammed instruction fails IM parity anyway -- that is the jam
      // mechanism". PARC's own IRTable entries show these bits VARYING -- 1/1
      // for Nop#, 1/0 for Return#, 0/1 for CPRegToLink# -- so they are real
      // computed parity, and all eight carry ODD parity over the 17-bit half
      // (make -C verilog im-parity-check).
      //
      // THE HALVES ARE JUST THE FIELDS, and parity does not care about order:
      //   left  = RSTK, ALUF, BSEL, LC, ASEL   (4+4+3+3+3 = 17)
      //   right = BLOCK, JCN, FF               (1+8+8      = 17)
      // fitted bit for bit against 2,148 AEmu addresses, unique for all 34.
      b0 = {rstk[3],
            ~(^{rstk, aluf, bsel, lc, asel}),   // P015
            jcn[0],
            ~(^{block, jcn, ff}),               // P1631
            4'b0000};
      b1 = {rstk[2], rstk[1], rstk[0], aluf[3], block, ff[7], ff[6], ff[5]};
      b2 = {aluf[2], aluf[1], aluf[0], bsel[2], ff[4], ff[3], ff[2], ff[1]};
      b3 = {bsel[1], bsel[0], lc[2],   lc[1],   ff[0], jcn[7], jcn[6], jcn[5]};
      b4 = {lc[0],   asel[2], asel[1], asel[0], jcn[4], jcn[3], jcn[2], jcn[1]};
      mi = {b0, b1, b2, b3, b4};
    end
  endfunction

  task jam_mi(input [39:0] w);
    begin parc_micro(w[39:32], w[31:24], w[23:16], w[15:8], w[7:0]); end
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
        $display("(memrun assertion, not this bench's subject) CPRegToLink# did not put the address in Link");
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

  // BUILD A HUNK from field values, four copies of one microinstruction.
  // PARC's hunk packing (doradoboot.masm, SendAHunk):
  //   Byte 0: 0RSTK.0, 0BLOCK, 1RSTK.0, 1BLOCK, ... -- the 17th bit of each
  //           half, which is why a hunk is 17 bytes for 8 half-instructions
  //   Byte 1: 0RSTK.1-3, 0ALUF.0-3, 0BSEL.0
  //   Byte 2: 0BSEL.1-2, 0LC.0-2, 0ASEL.0-2
  //   Byte 3: 0FF.0-7          Byte 4: 0JCN.0-7
  // The field arguments use mi()'s convention, MSB first: rstk[3] is RSTK.0.
  // BUILD A HUNK OF TWO ALTERNATING MICROINSTRUCTIONS -- copies 0,2 are A and
  // 1,3 are B -- so IM[0] holds A and IM[1] holds B and a pair of Local Jumps
  // can bounce between them. Note byte 0 carries RSTK.0 and BLOCK for all
  // four, so those two fields must match between A and B.
  // FOUR DISTINCT MICROINSTRUCTIONS in one hunk, at consecutive addresses.
  // Fields are passed as packed arrays, one entry per instruction, MSB-first
  // per mi()'s convention. Byte 0 carries RSTK.0 and BLOCK for all four, so
  // those two must be common.
  task build_hunk4(input [3:0] rstk,      input block,
                   input [3:0] aluf [4],  input [2:0] bsel [4],
                   input [2:0] lc   [4],  input [2:0] asel [4],
                   input [7:0] ff   [4],  input [7:0] jcn  [4]);
    integer q;
    begin
      hunk[0] = {rstk[3], block, rstk[3], block, rstk[3], block, rstk[3], block};
      for (q = 0; q < 4; q = q + 1) begin
        hunk[1+4*q] = {rstk[2], rstk[1], rstk[0],
                       aluf[q][3], aluf[q][2], aluf[q][1], aluf[q][0], bsel[q][2]};
        hunk[2+4*q] = {bsel[q][1], bsel[q][0], lc[q][2], lc[q][1], lc[q][0],
                       asel[q][2], asel[q][1], asel[q][0]};
        hunk[3+4*q] = ff[q];
        hunk[4+4*q] = jcn[q];
      end
    end
  endtask

  task build_hunk2(input [3:0] rstk,  input [3:0] alufA, input [2:0] bselA,
                   input [2:0] lcA,   input [2:0] aselA, input [7:0] ffA,
                   input [7:0] jcnA,
                   input [3:0] alufB, input [2:0] bselB,
                   input [2:0] lcB,   input [2:0] aselB, input [7:0] ffB,
                   input [7:0] jcnB,  input block);
    integer q;
    reg [3:0] aluf; reg [2:0] bsel, lc, asel; reg [7:0] ff, jcn;
    begin
      hunk[0] = {rstk[3], block, rstk[3], block, rstk[3], block, rstk[3], block};
      for (q = 0; q < 4; q = q + 1) begin
        if (q[0] == 1'b0) begin
          aluf = alufA; bsel = bselA; lc = lcA; asel = aselA; ff = ffA; jcn = jcnA;
        end else begin
          aluf = alufB; bsel = bselB; lc = lcB; asel = aselB; ff = ffB; jcn = jcnB;
        end
        hunk[1+4*q] = {rstk[2], rstk[1], rstk[0], aluf[3], aluf[2], aluf[1], aluf[0], bsel[2]};
        hunk[2+4*q] = {bsel[1], bsel[0], lc[2], lc[1], lc[0], asel[2], asel[1], asel[0]};
        hunk[3+4*q] = ff;
        hunk[4+4*q] = jcn;
      end
    end
  endtask

  task build_hunk(input [3:0] rstk, input [3:0] aluf, input [2:0] bsel,
                  input [2:0] lc,   input [2:0] asel, input [7:0] ff,
                  input [7:0] jcn,  input block);
    integer q;
    begin
      hunk[0] = {rstk[3], block, rstk[3], block, rstk[3], block, rstk[3], block};
      for (q = 0; q < 4; q = q + 1) begin
        hunk[1+4*q] = {rstk[2], rstk[1], rstk[0], aluf[3], aluf[2], aluf[1], aluf[0], bsel[2]};
        hunk[2+4*q] = {bsel[1], bsel[0], lc[2], lc[1], lc[0], asel[2], asel[1], asel[0]};
        hunk[3+4*q] = ff;
        hunk[4+4*q] = jcn;
      end
    end
  endtask

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

  initial begin
    force m.DMuxData = dmd;
    force m.DMuxClk  = dmc;
    force m.UseDMD   = udmd;
    repeat (WT(2000)) @(posedge sys_clk);
    manifold(12'h030);
    manifold(12'h1E0);                    // SetMidasStopMIRClk ON, to load
    // WARM THE PIPELINE FIRST. The first microinstruction out of a cold stop
    // runs only its clk0 half, so a jam issued straight after the manifolds
    // loses half its effect. tb_compute does exactly this before the same
    // prologue, and that is the difference between the two benches -- this one
    // had no warming and the prologue's first step silently did nothing.
    zero;
    nop_micro; nop_micro;

    // PARC'S ALU PROLOGUE, and TFromCPReg# does not work without it. Its own
    // IRTable comment says "requires ALUFM[0]=B", and LoadDoradoCode sets that
    // up first:
    //     LDXI 25o / QFromCPReg# / ALUFM[0]FromQ#
    // "Set up ALUFM[0] with a 25o, which is the logical function B." Without
    // it T stays 0000 and the MCR write below writes nothing -- which is
    // exactly what was measured.
    set_cpreg_plain(16'h0015);                        // 25 octal
    parc_micro(8'h30, 8'h13, 8'hEF, 8'hC4, 8'h40);    // QFromCPReg#
    nop_micro;                                        // DoIRTableInstAndNop
    parc_micro(8'h30, 8'h05, 8'h09, 8'hC4, 8'h40);    // ALUFM[0]FromQ#

    set_cpreg_plain(16'h0043);                        // 103 octal
    parc_micro(8'h70, 8'h03, 8'h0F, 8'h04, 8'hC0);    // TFromCPReg#
    nop_micro;
    $display("tb_ifufetch: at the jam -- PRhold=%b Hold=%b MDhold'=%b MiscHold'=%b RefHold'=%b",
             m.PRhold, m.b_MemC.Hold, m.b_MemC.MDhold_p_, m.b_MemC.MiscHold_p_,
             m.b_MemC.RefHold_p_);
    $display("tb_ifufetch: after TFromCPReg# -- T=%h (want 0043), Q=%h",
             {m.b_ProcH.T_00, m.b_ProcH.T_01, m.b_ProcH.T_02, m.b_ProcH.T_03,
              m.b_ProcH.T_04, m.b_ProcH.T_05, m.b_ProcH.T_06, m.b_ProcH.T_07,
              m.b_ProcL.T_08, m.b_ProcL.T_09, m.b_ProcL.T_10, m.b_ProcL.T_11,
              m.b_ProcL.T_12, m.b_ProcL.T_13, m.b_ProcL.T_14, m.b_ProcL.T_15},
             {m.b_ProcH.Q_00, m.b_ProcH.Q_01, m.b_ProcH.Q_02, m.b_ProcH.Q_03,
              m.b_ProcH.Q_04, m.b_ProcH.Q_05, m.b_ProcH.Q_06, m.b_ProcH.Q_07,
              m.b_ProcL.Q_08, m.b_ProcL.Q_09, m.b_ProcL.Q_10, m.b_ProcL.Q_11,
              m.b_ProcL.Q_12, m.b_ProcL.Q_13, m.b_ProcL.Q_14, m.b_ProcL.Q_15});
    parc_micro(8'h30, 8'h02, 8'h0B, 8'h84, 8'h60);    // SetMcr#
    nop_micro;
    $display("tb_ifufetch: after SetMcr# -- MemC DisHold=%b MemX DisHold=%b",
             m.b_MemC.DisHold, m.b_MemX.DisHold);

    // ---- A PROCESSOR MEMORY REFERENCE. The memory is awake now (DisHold is
    // set), the MIR is still held so a jam executes, and T holds 0043. ASEL
    // 0-3 is a reference (MemC b24) and BSEL >= 4 puts the A leg -- T or R --
    // on MAR, which compute-test established. None of PARC's thirteen IRTable
    // entries makes a reference, so this is built with mi().
    pipe_before = pipead;
    jam_mi(mi(4'd0, 4'd0, 3'd4, 3'd0, 3'd1, 8'd0, 8'o201, 1'b0));  // ASEL=1
    // The MAR mux enables are active during the jam's OWN window and have
    // dropped by the following Nop -- registered controls, the same timing
    // compute-test records. Sample here, not after.
    $display("tb_ifufetch: at the reference jam -- MAR=%h WantProcRef'=%b (ASEL=%0d IgnoreProc=%b) PipeAd %0d",
             mar, m.b_MemC.WantProcRef_p_, asel, m.b_MemC.IgnoreProc, pipead);
    nop_micro; nop_micro;
    $display("tb_ifufetch: after the nops    -- MAR=%h PipeAd %0d -> %0d",
             mar, pipe_before, pipead);
    // PARC's own sequence must set DisHold. Everything the memory section does
    // hangs off it: WantMapWait' = (MapFnc.1' & MapFnc.0') | DisHold, and
    // without it MapWait can never fall and the MapState counter never steps.
    if (m.b_MemC.DisHold !== 1'b1 || m.b_MemX.DisHold !== 1'b1)
      $display("(memrun assertion, not this bench's subject) SetMcr# did not set DisHold -- the memory cycle cannot start");

// (No map preload here. Poking the arrays from the bench is a dead end and
    // the machine says so -- see item 7 in the header: it WRITES THE MAP
    // ITSELF during startup, overwriting anything planted beforehand.)

    p0 = m.b_ContA.clk0_p_Ca; p1 = m.b_ContA.clk1_p_Ca; p2 = m.b_ContA.clk2_p_Bc;
    zero;
    wipe_im;
    nop_micro; nop_micro;

    // Load real microcode at its own addresses.
    if (!$value$plusargs("vectors=%s", path)) path = "boot0.vec";
    fd = $fopen(path, "r");
    if (fd == 0) $display("(memrun assertion, not this bench's subject) cannot open %s", path);
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
    $display("tb_ifufetch: loaded %0d hunks (%0d microinstructions)", hcount, hcount*4);

    // OVERWRITE IM[0..3] with four MEMORY REFERENCES. A reference cannot be
    // jammed -- IgnoreProc is 1 while the processor is stepped -- so it has to
    // EXECUTE. ASEL=1 is a reference (MemC b24: WantProcRef' = IgnoreProc |
    // ASEL.0) and BSEL >= 4 selects the A leg of the MAR mux, which
    // compute-test established puts a real register value on the address.
    // A plain fetch HITS in the cache (uninitialised tags match), so it is
    // serviced without touching storage. PARC's own lever for the storage path
    // is ForceMiss -- MemC k19 (MC10109) makes it
    //     ForceMiss = ForceDirtyMiss | bEcHasA | VictimInA | FlushStore
    // and cpu.c's dispatch says ASEL=1 with ff01=1 is FLUSH. ff01 is
    // FF.0*2 + FF.1 MSB-first, so ff01=1 is FF = 0o100.
    // (ff01 = 1 was tried here, which cpu.c's dispatch calls FLUSH and which
    // would force a miss -- see below. It still decodes as CacheRefInA, so a
    // Flush needs more than the ff01 bits, which is task 4's problem: only
    // LFetch<- and IFetch<- decode cleanly from ASEL/ff01 alone.)
    // IM[0] = a STORE, to DIRTY a cache line; IM[1] = a FLUSH of the same
    // line, which is the only thing that turns HitColDirty into a FlushStore.
    // Both keep BSEL = 0 so FFok' stays low and the FF field reaches the
    // memory section, and both keep a JCN.0 bit set for the same reason.
    //   Store:  ASEL = 000, FF.0 = FF.1 = 1 (0o300)  -- j22/b24/a24 algebra
    //   Flush:  ASEL = 001, FF   = 0o100             -- j24 Q3
    // The Local Jumps bounce 0 -> 1 -> 0 so both run, over and over.
    // FOUR INSTRUCTIONS: <-Map to write an entry, then Store to dirty the
    // line, then Flush to evict it, then a NON-reference so the loop has a
    // quiet slot. All keep BSEL = 0 so FFok' stays low and the FF field
    // reaches the memory section, and every JCN keeps a JCN.0 bit set for
    // the same reason. The jumps run 0 -> 1 -> 2 -> 3 -> 0.
    //   IM[0] <-Map : ASEL = 000, FF.0 = 0, FF.1 = 1   (j24 Q1)
    //   IM[1] Store : ASEL = 000, FF.0 = 1, FF.1 = 1   (j22/b24/a24)
    //   IM[2] Flush : ASEL = 001, FF.0 = 0, FF.1 = 1   (j24 Q3)
    //   IM[3] none  : ASEL = 100, so WantProcRef' stays high
    if (!$test$plusargs("realucode")) begin
      build_hunk4(4'd0, 1'b0,
                  '{4'd0,   4'd0,   4'd0,   4'd0},
                  '{3'd0,   3'd0,   3'd0,   3'd0},
                  '{3'd0,   3'd0,   3'd0,   3'd0},
                  '{3'd0,   3'd0,   3'd1,   3'd4},
                  '{8'o100, 8'o300, 8'o100, 8'o000},
                  '{8'o201, 8'o202, 8'o203, 8'o200});
                  // IM[2] IS THE `IFetch<-` AND IM[3] IS BACK TO A QUIET SLOT.
                  // Putting the IFetch in IM[3] -- four references back to
                  // back, no quiet slot -- recorded NO reference at all: the
                  // Pipe pointer moved 0 times against tb_memrun's 9, so even
                  // the <-Map/Store at IM[0..1] stopped landing. That is the
                  // same phase-fragility tb_memrun's header documents in the
                  // other direction, and the quiet slot is load-bearing.
                  // IM[3] IS AN `IFetch<-` NOW, not a quiet slot: ASEL = 001 with
                  // ff01 = 2 (FF.0 = 1, FF.1 = 0, so FF = 0o200), which is what
                  // refdecode-test pinned against cpu.c's DM_REF_IFETCH. The IFU
                  // cannot fetch an opcode until the microcode asks memory for the
                  // instruction stream, and with the other three references it now
                  // does, once per pass round the loop.
      send_a_hunk(16'd0);
    end else
      $display("tb_ifufetch: +realucode -- IM[0..15] left as real AEmu.mb!2 microcode");
    $display("tb_ifufetch: IM[0..3] overwritten with ASEL=1 FF=100B references");
    // ARE THE FOUR COPIES ACTUALLY IDENTICAL? IM is four INTERLEAVED banks --
    // the low two address bits pick the bank -- so IM[0..3] are bank 0..3 at
    // idx 0, and build_hunk's four copies land one per bank.
    $display("tb_ifufetch:   IM[0] L=%h R=%h   IM[1] L=%h R=%h",
             rd_L0(0), rd_R0(0), rd_L1(0), rd_R1(0));
    $display("tb_ifufetch:   IM[2] L=%h R=%h   IM[3] L=%h R=%h",
             rd_L2(0), rd_R2(0), rd_L3(0), rd_R3(0));
    // WHICH BANK IS WHICH ADDRESS? find_word SEARCHES the four banks, so the
    // mapping was never established. Scan every bank at the first few idx and
    // print what is actually non-zero.
    for (int bi = 0; bi < 4; bi++)
      for (int ii = 0; ii < 3; ii++) begin
        reg [15:0] lv, rv;
        lv = (bi==0)? rd_L0(ii) : (bi==1)? rd_L1(ii) : (bi==2)? rd_L2(ii) : rd_L3(ii);
        rv = (bi==0)? rd_R0(ii) : (bi==1)? rd_R1(ii) : (bi==2)? rd_R2(ii) : rd_R3(ii);
        if (lv !== 16'h0 || rv !== 16'h0)
          $display("tb_ifufetch:   IM bank %0d idx %0d : L=%h R=%h", bi, ii, lv, rv);
      end

    // RELEASE the MIR clock -- register 7, data bit 0 -- so the MIR can reload
    // from IM. Without this the jam is held and nothing is ever fetched.
    manifold(12'h1C0);
    manifold(12'h000);          // ParityEnables := 0 -- IM parity OFF
    $display("tb_ifufetch: parity enables IMLH=%b IMRH=%b, StopMIRClkEn=%b StopMIRClk=%b",
             m.b_ContB.IMLHPEenable, m.b_ContB.IMRHPEenable,
             m.b_ContB.StopMIRClkEn, m.StopMIRClk);

    // PARC's PrepareProcessor does one more thing before starting the machine,
    // and skipping it is why the memory holds: "Clear out MCR to DisHold,
    // NoWakeups" -- LDXI 103o / TFromCPReg# / SetMcr#. MemC's Hold flip-flops
    // (e22, e23) take DisHold on their set/reset, so without it Hold stays up
    // and PRhold with it. Bytes from doradoboot.masm, which is radix 2:
    // TFromCPReg# = 70 03 0F 04 C0, SetMcr# = 30 02 0B 84 60 (checked by
    // decoding Nop#'s `01^6.+(11^4.)` the same way and getting 0x70).
    $display("tb_ifufetch: MCR sequence -- T=%h (want 0043), MCR DisHold=%b",
             {m.b_ProcH.T_00, m.b_ProcH.T_01, m.b_ProcH.T_02, m.b_ProcH.T_03,
              m.b_ProcH.T_04, m.b_ProcH.T_05, m.b_ProcH.T_06, m.b_ProcH.T_07,
              m.b_ProcL.T_08, m.b_ProcL.T_09, m.b_ProcL.T_10, m.b_ProcL.T_11,
              m.b_ProcL.T_12, m.b_ProcL.T_13, m.b_ProcL.T_14, m.b_ProcL.T_15},
             m.b_MemC.DisHold);
    $display("tb_ifufetch: before start -- PRhold=%b Hold=%b DisHold=%b",
             m.PRhold, m.b_MemC.Hold, m.b_MemC.DisHold);
    // The memory must not be holding the processor before it starts. (This is
    // NOT caused by the SetMcr# above -- suppressing the whole block leaves it
    // reading 0 as well. See the header.)
    if (m.PRhold !== 1'b0 || m.b_MemC.Hold !== 1'b0)
      $display("(memrun assertion, not this bench's subject) the memory is holding the processor before the machine starts");

    // Put the start address in Link, then Return# to jump there and run.
    set_cpreg_tilde(16'h0000);
    parc_micro(8'h30, 8'h13, 8'hEF, 8'h04, 8'h40);   // CPRegToLink#
    nop_micro;
    $display("tb_ifufetch: Link[4:15]=%h", link_hi);

    // TaskingOn,Return. PARC's Return# is 60 13 E1 42 43 = "TaskingOff,Return"
    // with FF[142]; ContA a16 decodes FF=TaskingOn as FA=1,FB=4,FC=3 = 143
    // octal and FF=TaskingOff as FC=2 = 142, which is exactly the IRTable's
    // stated FF for Return#. FF.7 lives in byte 3 bit 3, so 0x42 | 0x08 = 0x4A
    // turns 142 into 143 -- checked by decoding both back through the byte
    // layout doradoboot.masm states.
    parc_run(8'h60, 8'h13, 8'hE1, 8'h4A, 8'h43);      // TaskingOn,Return

`ifdef FORCE_DISHOLD
    // EXPERIMENT: is DisHold the lever? WantMapWait' = (MapFnc.1' & MapFnc.0')
    // | DisHold, and WantMapWait' going HIGH is what lets h13's NOR fall and
    // frees MapWait to follow StartMap'.
    force m.b_MemX.DisHold = 1'b1;
`endif
    n0a = 0; nmemclk = 0; npipe = 0; ppa = pipead;
    nras = 0; ncas = 0; nwe = 0;
    prasa = m.MemRASa; pcasa = m.MemCASa; pwea = m.MemWEa;
    nmx = 0; pmx = m.b_MemX.Clk0_p_Aa;
    nrp = 0; prp = m.RfshPeriod; nmr = 0; pmr = m.MemRfsh;
    nms = 0; pms = mapst; nsq = 0; psq = m.b_MemX.RfshSqWave;
    nsrc=0; psrc=m.b_MemX.StartRfshCycle_p_; nwr=0; pwr=m.b_MemX.WantRfsh_p_;
    nnr=0; pnr=m.b_MemX.NeedRfsh_p_; nmrf=0; pmrf=m.b_MemX.MapRfsh_p_;
    nsm=0; psm=m.b_MemC.StartMap_p_; nmw=0; pmw=m.b_MemX.MapWait;
    npsm=0; ppsm=m.b_MemX.preStartMem_p_; nwmw=0; pwmw=m.b_MemX.WantMapWait_p_;
    ng13=0; pg13=m.b_MemX.MapWait__g13_3; nxsm=0; pxsm=m.b_MemX.StartMap_p_;
    nwpr=0; pwpr=m.b_MemC.WantProcRef_p_; nrh=0; prh=m.b_MemC.RefHold_p_;
    nldp=0; pldp=m.b_MemC.LdPair_p_;
    npha=0; ppha=m.b_MemC.PairHasA; ncra=0; pcra=m.b_MemC.CacheRefInA;
    nha=0; pha=m.b_MemC.Hit_p_a; nhb=0; phb=m.b_MemC.Hit_p_b;
    for (int zi4 = 0; zi4 < 32; zi4++) i14_hit[zi4] = 0;
    runlen=0; maxrun=0; nwin=0; winat=-1;
    pmfree=m.b_MemX.MemFree; pmidle=m.b_MemX.MemIdle;
    nmfree_e=0; nmidle_e=0; nmfree_hi=0;
    npsh=0; ncoin=0; nmwlo=0; lastpsh=-1; lastcoin=-1; lastmwlo=-1; lastmf00=-1;
    for (int zf2 = 0; zf2 < 4; zf2++) mf_cnt[zf2] = 0;
    for (int zs3 = 0; zs3 < 8; zs3++) begin ms3_cnt[zs3]=0; ms3_last[zs3]=-1; end
    pmf00=0; pms3=0; nmf00on=0; nms3on=0;
    for (int zo = 0; zo < 12; zo++) begin mf00at[zo]=-1; ms3at[zo]=-1; end ndd=0; nidle_lo=0; pdd=m.b_MemX.Clk0_p_Dd;
    nmemst=0; nfree=0; nmemfr=0; nheld_nz=0;
    for (int zf = 0; zf < 16; zf++) memfr_hit[zf] = 1'b0;
    for (int zm = 0; zm < 16; zm++) memst_hit[zm] = 1'b0;
    nmapst=0; nmapfn=0; npsm2=0; nsm2=0; nload=0; ncnt=0; nd0=0; nwim=0; nx10=0; nmti=0; nwm=0; nall3=0; nrw=0; nwp=0; ndty=0; nevn=0; nckw=0; nprf=0; nthi=0; nmt=0; nmtp=0;
    nmras=0; nmcas=0; nmrd=0; nmwr=0; nd13w=0; pmras=m.b_MemX.u_a04.p4; pmcas=m.b_MemX.u_a04.p15;
    for (int zj = 0; zj < 8; zj++) mapst_hit[zj] = 1'b0;
    for (int zk = 0; zk < 4; zk++) mapfn_hit[zk] = 1'b0;
    nifu_j=0; nifu_d=0; nifu_free=0;
    for (int zj=0; zj<256; zj++) begin ifu_seen[zj]=1'b0; ifud_seen[zj]=1'b0; end
    ntnia=0; nff0=0; nsamp=0;
    nff0_wpr=0; nff0_cr=0; nff0_alt=0; nff0_fl=0; nff0_a1=0; nff0_ign=0; nff0_a0=0; nff0_ffok=0; nff0_bad=0; nff0_fs=0; nff0_fm=0; nff0_mia=0; nff0_mib=0; nff0_fsp=0; nff0_ech=0; nff0_st=0; nff0_hcd=0;
    for (int zi = 0; zi < 4096; zi++) tnia_hit[zi] = 1'b0;
    nwcr=0; pwcr=m.b_MemC.WantCR; nwar=0; pwar=m.b_MemC.WantAltRef_p_;
    nfl=0; pfl=m.b_MemC.Flush_u__p_; nmp=0; pmp=m.b_MemC.Map_u__p_;
    // PRELOAD THE MAP PLANES TO 1. The MosRam arrays are one bit each, 21 of
    // them, and Verilator starts them at 0 -- which is a map entry that fails
    // its parity, so MapTrouble asserts and the MC10124 translators show
    // nothing moving. This is AND IT IS LOAD-BEARING: removing it fails
    // memrun-test's "the map outputs never varied" check.
    //
    // (An earlier note here called it inert, on the strength of a preload to
    // ZERO reading back as 1 -- but that was the ARRAY'S OWN default showing
    // through, not the machine overwriting the plant. Preloading to 1 plainly
    // does reach the logic. Retracted.)
    for (int mi2 = 0; mi2 < 4096; mi2++) begin
      m.b_MemX.u_a04.mem[mi2]=1'b1;
      m.b_MemX.u_a05.mem[mi2]=1'b1;
      m.b_MemX.u_a06.mem[mi2]=1'b1;
      m.b_MemX.u_a07.mem[mi2]=1'b1;
      m.b_MemX.u_a08.mem[mi2]=1'b1;
      m.b_MemX.u_a09.mem[mi2]=1'b1;
      m.b_MemX.u_a10.mem[mi2]=1'b1;
      m.b_MemX.u_a11.mem[mi2]=1'b1;
      m.b_MemX.u_a12.mem[mi2]=1'b1;
      m.b_MemX.u_a13.mem[mi2]=1'b1;
      m.b_MemX.u_a14.mem[mi2]=1'b1;
      m.b_MemX.u_d04.mem[mi2]=1'b1;
      m.b_MemX.u_d05.mem[mi2]=1'b1;
      m.b_MemX.u_d06.mem[mi2]=1'b1;
      m.b_MemX.u_d07.mem[mi2]=1'b1;
      m.b_MemX.u_d08.mem[mi2]=1'b1;
      m.b_MemX.u_d09.mem[mi2]=1'b1;
      m.b_MemX.u_d10.mem[mi2]=1'b1;
      m.b_MemX.u_d11.mem[mi2]=1'b1;
      m.b_MemX.u_d12.mem[mi2]=1'b1;
      m.b_MemX.u_d13.mem[mi2]=1'b1;
    end
    $display("tb_ifufetch: ALL 21 map bit planes preloaded to 1 (parity experiment)");

    p0 = m.b_ContA.clk0_p_Ca; pmc = m.b_MemC.clk0_p_A;
    for (j2 = 0; j2 < 3000; j2 = j2 + 1) begin
      @(posedge sys_clk);
      if (m.b_ContA.clk0_p_Ca !== p0) begin n0a = n0a + 1; p0 = m.b_ContA.clk0_p_Ca; end
      if (m.b_MemC.clk0_p_A !== pmc) begin nmemclk = nmemclk + 1; pmc = m.b_MemC.clk0_p_A; end
      // THE PIPE POINTER. Every storage reference advances it, so counting its
      // changes counts references actually recorded by the memory section.
      if (pipead !== ppa) begin npipe = npipe + 1; ppa = pipead; end
      // THE STORAGE INTERFACE. The MSA boards are not among the eleven -- the
      // archive has no wire list for them -- so MemAd/RAS/CAS/WE/Sout leave
      // the machine and Sin comes back. Count the strobes to see whether the
      // memory section is actually driving a DRAM cycle.
      if (m.MemRASa !== prasa) begin nras = nras + 1; prasa = m.MemRASa; end
      if (m.MemCASa !== pcasa) begin ncas = ncas + 1; pcasa = m.MemCASa; end
      if (m.MemWEa  !== pwea ) begin nwe  = nwe  + 1; pwea  = m.MemWEa;  end
      if (m.b_MemX.Clk0_p_Aa !== pmx) begin nmx = nmx + 1; pmx = m.b_MemX.Clk0_p_Aa; end
      if (m.RfshPeriod !== prp) begin nrp = nrp + 1; prp = m.RfshPeriod; end
      if (m.MemRfsh   !== pmr) begin nmr = nmr + 1; pmr = m.MemRfsh;   end
      if (mapst !== pms) begin nms = nms + 1; pms = mapst; end
      if (m.b_MemX.RfshSqWave !== psq) begin nsq = nsq + 1; psq = m.b_MemX.RfshSqWave; end
      // COUNT the whole refresh chain rather than sampling it. Reading an
      // instant is what made the Pipe pointer look like it was advancing.
      if (m.b_MemX.StartRfshCycle_p_ !== psrc) begin nsrc=nsrc+1; psrc=m.b_MemX.StartRfshCycle_p_; end
      if (m.b_MemX.WantRfsh_p_       !== pwr ) begin nwr =nwr +1; pwr =m.b_MemX.WantRfsh_p_;       end
      if (m.b_MemX.NeedRfsh_p_       !== pnr ) begin nnr =nnr +1; pnr =m.b_MemX.NeedRfsh_p_;       end
      if (m.b_MemX.MapRfsh_p_        !== pmrf) begin nmrf=nmrf+1; pmrf=m.b_MemX.MapRfsh_p_;        end
      if (m.b_MemC.StartMap_p_       !== psm ) begin nsm =nsm +1; psm =m.b_MemC.StartMap_p_;       end
      if (m.b_MemX.MapWait           !== pmw ) begin nmw =nmw +1; pmw =m.b_MemX.MapWait;           end
      if (m.b_MemC.WantProcRef_p_    !== pwpr) begin nwpr=nwpr+1; pwpr=m.b_MemC.WantProcRef_p_;    end
      if (m.b_MemC.RefHold_p_        !== prh ) begin nrh =nrh +1; prh =m.b_MemC.RefHold_p_;        end
      if (m.b_MemC.LdPair_p_         !== pldp) begin nldp=nldp+1; pldp=m.b_MemC.LdPair_p_;         end
      if (m.b_MemC.PairHasA          !== ppha) begin npha=npha+1; ppha=m.b_MemC.PairHasA;          end
      if (m.b_MemC.CacheRefInA       !== pcra) begin ncra=ncra+1; pcra=m.b_MemC.CacheRefInA;       end
      if (m.b_MemC.Hit_p_a           !== pha ) begin nha =nha +1; pha =m.b_MemC.Hit_p_a;           end
      if (m.b_MemC.Hit_p_b           !== phb ) begin nhb =nhb +1; phb =m.b_MemC.Hit_p_b;           end
      if (m.b_MemC.WantCR            !== pwcr) begin nwcr=nwcr+1; pwcr=m.b_MemC.WantCR;            end
      nsamp = nsamp + 1;
      // THE IFU, over the whole run -- not at one instant, which is the trap
      // this file's header records three times over.
      ifu_j = {m.b_IFU.J_0a, m.b_IFU.J_1a, m.b_IFU.J_2a, m.b_IFU.J_3a,
               m.b_IFU.J_4a, m.b_IFU.J_5a, m.b_IFU.J_6a, m.b_IFU.J_7a};
      if (!ifu_seen[ifu_j]) begin ifu_seen[ifu_j] = 1'b1; nifu_j = nifu_j + 1; end
      ifu_d = {m.IfuData_7, m.IfuData_6, m.IfuData_5, m.IfuData_4,
               m.IfuData_3, m.IfuData_2, m.IfuData_1, m.IfuData_0};
      if (!ifud_seen[ifu_d]) begin ifud_seen[ifu_d] = 1'b1; nifu_d = nifu_d + 1; end
      if (!m.b_IFU.IfuHold) nifu_free = nifu_free + 1;
      // MemX i14 (an SG10139 PROM) makes preStartMem' from
      // {MapFnc.0', MapFnc.1', MapState.0-2}: the storage cycle is a PROM
      // state machine, so watch the STATE, not an instant.
      mapst_now = {m.b_MemX.MapState_0, m.b_MemX.MapState_1, m.b_MemX.MapState_2};
      mapfn_now = {m.b_MemX.MapFnc_0_p_, m.b_MemX.MapFnc_1_p_};
      if (!mapst_hit[mapst_now]) begin mapst_hit[mapst_now]=1'b1; nmapst=nmapst+1; end
      // THE ACTUAL i14 ADDRESS. cell_SG10139 is MSB-FIRST -- A0 is the MOST
      // significant address bit -- and i14's pins are A0=MapFnc.0',
      // A1=MapFnc.1', A2=MapState.0, A3=MapState.1, A4=MapState.2. So the
      // index is {MapFnc.0', MapFnc.1', MapState.0, MapState.1, MapState.2}.
      // Q2 is preStartMem', and Q0 being the MOST significant DATA bit makes
      // it bit 5 of the byte -- HIGH at EXACTLY ONE address, 3.
      i14a = {m.b_MemX.MapFnc_0_p_, m.b_MemX.MapFnc_1_p_, m.b_MemX.MapState_0,
              m.b_MemX.MapState_1, m.b_MemX.MapState_2};
      i14_hit[i14a] = i14_hit[i14a] + 1;
      if (!mapfn_hit[mapfn_now]) begin mapfn_hit[mapfn_now]=1'b1; nmapfn=nmapfn+1; end
      if (!m.b_MemX.preStartMem_p_) npsm2 = npsm2 + 1;
      // j22's PE' is MapWait and it LOADS when LOW, so a window opens only
      // where preStartMem' is HIGH and MapWait is LOW at the same time.
      if (m.b_MemX.preStartMem_p_) begin
        npsh = npsh + 1; lastpsh = nsamp;
        if (!m.b_MemX.MapWait) begin ncoin = ncoin + 1; lastcoin = nsamp; end
      end
      if (!m.b_MemX.MapWait) begin nmwlo = nmwlo + 1; lastmwlo = nsamp; end
      // A memory cycle needs BOTH MapFnc bits LOW (a map function pending)
      // at MapState 3. Which values does MapFnc actually take, and when?
      mf_now = {m.b_MemX.MapFnc_0_p_, m.b_MemX.MapFnc_1_p_};
      mf_cnt[mf_now] = mf_cnt[mf_now] + 1;
      if (mf_now == 2'b00) begin
        lastmf00 = nsamp;
        if (!pmf00) begin                       // ONSET of a MapFnc=00 run
          if (nmf00on < 12) mf00at[nmf00on] = nsamp;
          nmf00on = nmf00on + 1;
        end
        pmf00 = 1'b1;
      end else pmf00 = 1'b0;
      // MapState, PARC's numbering: MapState.0 is the MSB.
      ms3_now = {m.b_MemX.MapState_0, m.b_MemX.MapState_1, m.b_MemX.MapState_2};
      ms3_cnt[ms3_now] = ms3_cnt[ms3_now] + 1;
      ms3_last[ms3_now] = nsamp;
      if (ms3_now == 3'd3) begin
        if (!pms3) begin                        // ONSET of a MapState=3 run
          if (nms3on < 12) ms3at[nms3on] = nsamp;
          nms3on = nms3on + 1;
        end
        pms3 = 1'b1;
      end else pms3 = 1'b0;
      if (!m.b_MemX.StartMem_p_)    nsm2  = nsm2  + 1;
      // F10016 i10: PE'=MemIdle loads when LOW, CE'=TrueBD counts when LOW.
      if (!m.b_MemX.MemIdle)        nload = nload + 1;   // held in LOAD
      if (!m.b_MemX.TrueBD)         ncnt  = ncnt  + 1;   // allowed to COUNT
      if (m.b_MemX.MemX07_sil_pl_19) nd0  = nd0   + 1;   // the D0 it loads
      if (!m.b_MemX.WriteInMem_p_ && !m.b_MemX.MapTroubleInMem) nwm = nwm + 1;
      if (!m.b_MemX.WriteInMem_p_ && !m.b_MemX.MapTroubleInMem
          && !m.b_MemX.MemX07_sil_pl_10) nall3 = nall3 + 1;
      // i11 (MC10105 gate b, a NOR):
      //   MemX07.sil+19 = ~(WriteInMem' | MemX07.sil+10 | MapTroubleInMem)
      if (!m.b_MemX.WriteInMem_p_)      nwim = nwim + 1;
      if (!m.b_MemX.MemX07_sil_pl_10)   nx10 = nx10 + 1;
      if (!m.b_MemX.MapTroubleInMem)    nmti = nmti + 1;
      // MapTrouble comes from g14, an MC10121 4-wide OR-AND whose COMMON
      // input is ReadOrWriteInMap' -- high forces every term, so MapTrouble
      // asserts BY DEFAULT unless a read or write is in the map stage.
      if (!m.b_MemX.ReadOrWriteInMap_p_) nrw  = nrw  + 1;
      if (!m.b_MemX.MapWP_p_)            nwp  = nwp  + 1;
      if (!m.b_MemX.MapDirty_p_)         ndty = ndty + 1;
      if (!m.b_MemX.MapEven_p_)          nevn = nevn + 1;
      if (!m.b_MemX.CheckWP_p_)          nckw = nckw + 1;
      if (m.b_MemX.THi)                  nthi = nthi + 1;
      // The FOURTH group of g14's OR-AND is (MapEven' | preRfshInMem | ROWIM').
      // MapEven' and ROWIM' are already low all run, so this term -- and with
      // it MapTrouble -- turns entirely on preRfshInMem.
      if (!m.b_MemX.preRfshInMem)        nprf = nprf + 1;
      if (m.b_MemX.MapTrouble)           nmt  = nmt  + 1;
      if (m.b_MemX.MapTrouble_p_)        nmtp = nmtp + 1;
      // Which MemStates does the machine reach? j13's Q0 -- MemX07.sil+10,
      // the third term of MemWEa's D0 -- is 0 only in particular ones.
      memst_now = {m.b_MemX.MemState_3, m.b_MemX.MemState_2,
                   m.b_MemX.MemState_1, m.b_MemX.MemState_0};
      if (!memst_hit[memst_now]) begin memst_hit[memst_now]=1'b1; nmemst=nmemst+1; end
      // j16 is an F10016 whose PE' is StartMem' with ALL FOUR D INPUTS OPEN,
      // so a low StartMem' parallel-loads ZERO and overrides the count.
      if (m.b_MemX.StartMem_p_) begin
        nfree = nfree + 1;
        runlen = runlen + 1;
        if (runlen > maxrun) maxrun = runlen;
        // Inside the free window: is the counter CLOCKED, and ENABLED?
        if (m.b_MemX.Clk0_p_Dd !== pdd) begin ndd = ndd + 1; pdd = m.b_MemX.Clk0_p_Dd; end
        if (!m.b_MemX.MemIdle) nidle_lo = nidle_lo + 1;
        // DO THEY TOGGLE AT ALL? j13's Q5 says MemFree should be 0 at every
        // EVEN MemState -- half the time -- so both should alternate. j12
        // latches MemFree on MemIdle and MemIdle is derived from MemFree
        // through g13, so this feedback may be settling instead.
        if (m.b_MemX.MemFree !== pmfree) begin nmfree_e = nmfree_e + 1; pmfree = m.b_MemX.MemFree; end
        if (m.b_MemX.MemIdle !== pmidle) begin nmidle_e = nmidle_e + 1; pmidle = m.b_MemX.MemIdle; end
        if (m.b_MemX.MemFree) nmfree_hi = nmfree_hi + 1;
      end else runlen = 0;
      if (m.b_MemX.StartMem_p_) begin
        if (!memfr_hit[memst_now]) begin memfr_hit[memst_now]=1'b1; nmemfr=nmemfr+1; end
      end else if (memst_now != 4'd0) nheld_nz = nheld_nz + 1;
      if (m.b_MemX.StartMem_p_ && runlen == 1) begin
        nwin = nwin + 1;
        winat = nsamp;            // WHERE in the run does the window open?
      end
      // Is the MAP ARRAY ever strobed? MosRam latches dout only on
      // !RAS' && !CAS' && WE'. Count edges, do not sample.
      if (m.b_MemX.u_a04.p4  !== pmras) begin nmras=nmras+1; pmras=m.b_MemX.u_a04.p4;  end
      if (m.b_MemX.u_a04.p15 !== pmcas) begin nmcas=nmcas+1; pmcas=m.b_MemX.u_a04.p15; end
      if (!m.b_MemX.u_a04.p4 && !m.b_MemX.u_a04.p15 && m.b_MemX.u_a04.p3) nmrd = nmrd + 1;
      if (!m.b_MemX.u_a04.p4 && !m.b_MemX.u_a04.p15 && !m.b_MemX.u_a04.p3) nmwr = nmwr + 1;
      if (!m.b_MemX.u_d13.p4 && !m.b_MemX.u_d13.p15 && !m.b_MemX.u_d13.p3) nd13w = nd13w + 1;
      if (!m.b_MemC.ASEL_0 && !m.b_MemC.ASEL_2 && m.b_MemC.Store_u_)
        nff0_st = nff0_st + 1;
      // CONDITION ON THE INSTRUCTION ACTUALLY RUNNING. IM[0] and IM[1] both
      // hold the ASEL=1 reference this bench built (read back as L=0101
      // R=0081), so the running instruction is identified by its ASEL --
      // ASEL.0 = 0 and ASEL.2 = 1 -- NOT by the FF field, which selected the
      // startup cycles instead and made the whole chain look dead.
      if (!m.b_MemC.ASEL_0 && m.b_MemC.ASEL_2) begin   // the FLUSH
        nff0 = nff0 + 1;
        // Conditioned on the FLUSH instruction actually being in force.
        if (!m.b_MemC.WantProcRef_p_) nff0_wpr = nff0_wpr + 1;
        if (!m.b_MemC.WantCR)         nff0_cr  = nff0_cr  + 1;
        if (!m.b_MemC.WantAltRef_p_)  nff0_alt = nff0_alt + 1;
        if (!m.b_MemC.Flush_u__p_)    nff0_fl  = nff0_fl  + 1;
        if (m.b_MemC.FlushStore)      nff0_fs  = nff0_fs  + 1;
        if (m.b_MemC.HitColDirty)     nff0_hcd = nff0_hcd + 1;
        // MemC l19 gate d (MC10100, pin 9 COMMON):
        //     FlushStore = ~(FSinPair' | EcHasAb)
        // so the flush must be LATCHED INTO THE A/B PAIR first.
        if (!m.b_MemC.FSinPair_p_)    nff0_fsp = nff0_fsp + 1;
        if (!m.b_MemC.EcHasAb)        nff0_ech = nff0_ech + 1;
        if (m.b_MemC.ForceMiss)       nff0_fm  = nff0_fm  + 1;
        if (m.b_MemC.Hit_p_a)         nff0_mia = nff0_mia + 1;   // Hit' high = MISS
        if (m.b_MemC.Hit_p_b)         nff0_mib = nff0_mib + 1;
        if (m.b_MemC.ASEL_1_p_)       nff0_a1  = nff0_a1  + 1;
        // WantProcRef' = IgnoreProc | ASEL.0  (MemC b24 gate c, an MC10103:
        // pin 15 is the OR despite the primed name, pin 9 the NOR).
        if (m.b_MemC.IgnoreProc)      nff0_ign = nff0_ign + 1;
        if (m.b_MemC.ASEL_0)          nff0_a0  = nff0_a0  + 1;
        // ProcH d24/d23 (MC10101, pin 12 COMMON) qualify the FF bits the
        // memory section sees:  FF.0mem' = ~(FFok'a | FF.0),
        // FF.1mem = FFok'a | FF.1.  FFok' high FORCES both high.
        if (m.b_ProcH.FFok_p_a) begin
          nff0_ffok = nff0_ffok + 1;
          // Cross-check the gate against PARC's wire list, every cycle.
          if (m.b_MemC.FF_0mem_p_ !== 1'b0) nff0_bad = nff0_bad + 1;
          if (m.b_MemC.FF_1mem   !== 1'b1) nff0_bad = nff0_bad + 1;
        end
      end
      if (!tnia_hit[tnia_now]) begin
        tnia_hit[tnia_now] = 1'b1; ntnia = ntnia + 1;
        $display("tb_ifufetch:   TNIA visits %h (FF.0mem'=%b ASEL.0=%b ASEL.1'=%b ASEL.2=%b)",
                 tnia_now, m.b_MemC.FF_0mem_p_, m.b_MemC.ASEL_0,
                 m.b_MemC.ASEL_1_p_, m.b_MemC.ASEL_2);
      end
      if (m.b_MemC.WantAltRef_p_     !== pwar) begin nwar=nwar+1; pwar=m.b_MemC.WantAltRef_p_;     end
      if (m.b_MemC.Flush_u__p_       !== pfl ) begin nfl =nfl +1; pfl =m.b_MemC.Flush_u__p_;       end
      if (m.b_MemC.Map_u__p_         !== pmp ) begin nmp =nmp +1; pmp =m.b_MemC.Map_u__p_;         end
      if (m.b_MemX.preStartMem_p_    !== ppsm) begin npsm=npsm+1; ppsm=m.b_MemX.preStartMem_p_;    end
      if (m.b_MemX.WantMapWait_p_    !== pwmw) begin nwmw=nwmw+1; pwmw=m.b_MemX.WantMapWait_p_;    end
      if (m.b_MemX.MapWait__g13_3    !== pg13) begin ng13=ng13+1; pg13=m.b_MemX.MapWait__g13_3;    end
      if (m.b_MemX.StartMap_p_       !== pxsm) begin nxsm=nxsm+1; pxsm=m.b_MemX.StartMap_p_;       end
    end
    $display("tb_ifufetch: storage strobes over the run -- MemRASa %0d, MemCASa %0d, MemWEa %0d",
             nras, ncas, nwe);
    $display("tb_ifufetch: WantProcRef' edges %0d, RefHold' edges %0d (is the RUNNING microcode asking?)",
             nwpr, nrh);
    // AwantsMapFS comes from MemC k20, an MC10101 driving it from three
    // INVERTING outputs wired together, each sharing pin 12 = EcHasAb:
    //     pin 2  = ~(Map_InPair'   | EcHasAb)
    //     pin 3  = ~(MemC17.sil+12 | EcHasAb)
    //     pin 14 = ~(VicInPair'    | EcHasAb)
    // For AwantsMapFS to rise, one of those pairs must be BOTH low.
    // LdPair' is the CLOCK on j21/k21, the pair registers whose D inputs are
    // the reference kinds. MemC j17 (SE10212): LdPair' = ~(preClk0'D |
    // AfreeOrEc'a | EcKeepsAbusy). preClk0'D is a clock, so LdPair' should
    // toggle every microinstruction unless one of the other two holds it.
    $display("tb_ifufetch:   LdPair' edges %0d -- AfreeOrEc'a=%b EcKeepsAbusy=%b",
             nldp, m.b_MemC.AfreeOrEc_p_a, m.b_MemC.EcKeepsAbusy);
    $display("tb_ifufetch:   cache -- Hit'a=%b Hit'b=%b (edges %0d/%0d) | PairHasA edges %0d, CacheRefInA edges %0d",
             m.b_MemC.Hit_p_a, m.b_MemC.Hit_p_b, nha, nhb, npha, ncra);
    // THE KIND DECODER IS TASK-GATED. MemC j24 is an MC10161 1-of-8 decoder
    // addressed by {S4,S2,S1} = {EmuOrFT', ASEL.2, FF.1mem}, enabled by
    // WantAltRef' and HoldOrIP, with Flush_' on Q3 and Map_' on Q1. So Flush
    // needs EmuOrFT'=0, ASEL.2=1, FF.1mem=1 -- and EmuOrFT' is the EMULATOR OR
    // FAULT TASK condition, which is exactly what cpu.c marks on DM_REF_MAP and
    // DM_REF_FLUSH ("emulator/fault") and on the IO kinds ("io task").
    // WantCR (MemC d22, MC10117 2-wide OR-AND, pin 3 = the INVERTING output
    // despite the name) =
    //     ~[(FF.0mem' | WantProcRef') & (ASEL.1' | WantProcRef')]
    // so with a reference wanted it goes LOW only when ASEL.1 = 0 AND FF.0 = 0.
    $display("tb_ifufetch:   WHERE IS IT -- TNIA hit %0d distinct values; FF.0mem'=1 on %0d of %0d samples",
             ntnia, nff0, nsamp);
    $display("tb_ifufetch:   WHILE THE ASEL=1 REFERENCE IS RUNNING (%0d samples) -- ASEL.1'=1 %0d, WantProcRef'=0 %0d, WantCR=0 %0d, WantAltRef'=0 %0d, Flush'=0 %0d",
             nff0, nff0_a1, nff0_wpr, nff0_cr, nff0_alt, nff0_fl);
    // GATE: with the ASEL=1 reference actually executing, the front door must
    // be open on EVERY cycle it runs -- WantProcRef' low, from IgnoreProc = 0
    // and ASEL.0 = 0, which is b24 gate c's algebra exactly.
    if (nff0 < 100)
      $display("(memrun assertion, not this bench's subject) the ASEL=1 reference barely ran (%0d samples) -- it must be the executing instruction", nff0);
    if (nff0_wpr !== nff0)
      $display("(memrun assertion, not this bench's subject) WantProcRef' was not asserted on every cycle the reference ran (%0d of %0d)",
             nff0_wpr, nff0);
    if (nff0_ign !== 0 || nff0_a0 !== 0)
      $display("(memrun assertion, not this bench's subject) WantProcRef' low requires IgnoreProc = 0 and ASEL.0 = 0 (saw %0d, %0d)",
             nff0_ign, nff0_a0);
    // GATE: ProcH d24 gate a is an MC10101 whose pin 12 is the COMMON input,
    // so  FF.0mem' = ~(FFok'a | FF.0)  and  FF.1mem = FFok'a | FF.1.  With
    // FFok'a high both memory FF bits are FORCED, and the RTL must show it.
    if (nff0_bad !== 0)
      $display("(memrun assertion, not this bench's subject) FFok'a high did not force FF.0mem'=0 and FF.1mem=1 (%0d violations)", nff0_bad);
    // GATE: THE MICROCODE MAKES A FLUSH. With BSEL.0 = 0 pulling FFok' low,
    // the FF field reaches the memory section, WantCR falls, WantAltRef'
    // enables j24 and Flush' asserts -- on EVERY cycle the reference runs.
    if (nff0_cr !== nff0)
      $display("(memrun assertion, not this bench's subject) WantCR did not fall on every running cycle (%0d of %0d)", nff0_cr, nff0);
    if (nff0_alt !== nff0)
      $display("(memrun assertion, not this bench's subject) WantAltRef' did not enable the alternate decoder (%0d of %0d)", nff0_alt, nff0);
    if (nff0_fl !== nff0)
      $display("(memrun assertion, not this bench's subject) Flush' did not assert on every running cycle (%0d of %0d)", nff0_fl, nff0);
    if (nff0_ffok !== 0)
      $display("(memrun assertion, not this bench's subject) FFok' must be LOW for the FF field to reach the memory section (high on %0d)", nff0_ffok);
    // GATE: THE STORE DIRTIES A LINE AND THE FLUSH MISSES ON IT.
    // Store<- comes off j22 (MC10105) as ~(CacheRef' | Store<-IfCR'), and
    // b24/a24 make Store<-IfCR' = Q0 | ASEL.2 -- so ASEL = 000 with
    // FF.0 = FF.1 = 1. Then the flush finds HitColDirty, k21 latches
    // FSinPair' on LdPair', l19 makes FlushStore, and k19 makes ForceMiss.
    if (nff0_st  == 0) $display("(memrun assertion, not this bench's subject) the Store never asserted -- nothing dirties a line");
    if (nff0_hcd == 0) $display("(memrun assertion, not this bench's subject) HitColDirty never true -- the Store did not dirty the flushed line");
    if (nff0_fsp == 0) $display("(memrun assertion, not this bench's subject) FSinPair' never fell -- the flush was not latched into the pair");
    if (nff0_fs  == 0) $display("(memrun assertion, not this bench's subject) FlushStore never asserted");
    if (nff0_fm  == 0) $display("(memrun assertion, not this bench's subject) ForceMiss never asserted");
    if (nff0_mia == 0 || nff0_mib == 0)
      $display("(memrun assertion, not this bench's subject) the cache never missed (a %0d, b %0d)", nff0_mia, nff0_mib);
    // GATE: the MapState sequencer walks and the start signal asserts. MemX
    // i14 is an SG10139 PROM making preStartMem' from {MapFnc.0', MapFnc.1',
    // MapState.0-2}, so require the state to WALK rather than sampling it.
    //
    // NOTE, against an earlier version of this comment: `StartMem'` asserting
    // is NOT by itself "a storage cycle is running". It is MemX j16's PE',
    // and that F10016's D inputs are all open, so a LOW StartMem' LOADS ZERO
    // into MemState and overrides the count. Held low -- which is what this
    // loop does, 2680 samples of 3000 -- it is a HOLD, not a start. See item
    // 9 in the header.
    if (nmapst < 4)
      $display("(memrun assertion, not this bench's subject) the map sequencer barely moved (%0d of 8 states)", nmapst);
    if (npsm2 == 0) $display("(memrun assertion, not this bench's subject) preStartMem' never asserted -- no storage cycle was started");
    if (nsm2  == 0) $display("(memrun assertion, not this bench's subject) StartMem' never asserted");
    // MemX i10 (F10016 counter): PE' = MemIdle, CE' = TrueBD, MR = STPerr.
    // The data sheet: CE' is "Count Enable (LOW to Count)", PE' loads.
    // GATE: THE WRITE REACHES THE MEMORY PIPELINE. WriteInMem' asserting is
    // the write-back being scheduled; what stops it short of the array is
    // MapTroubleInMem, below.
    if (nwim == 0)
      $display("(memrun assertion, not this bench's subject) WriteInMem' never asserted -- the write-back never reached the memory stage");
    // GATE: a map operation IS in the stage -- ReadOrWriteInMap' is the
    // COMMON input of g14's OR-AND, and high would force MapTrouble by
    // default, which would make the diagnosis below meaningless.
    if (nrw == 0)
      $display("(memrun assertion, not this bench's subject) ReadOrWriteInMap' never asserted -- no map operation, so MapTrouble says nothing");
    $display("tb_ifufetch:   MapTrouble terms low on -- ReadOrWriteInMap' %0d, MapWP' %0d, MapDirty' %0d, MapEven' %0d, CheckWP' %0d of %0d",
             nrw, nwp, ndty, nevn, nckw, nsamp);
    $display("tb_ifufetch:   ...and the fourth term: preRfshInMem low on %0d of %0d", nprf, nsamp);
    // GATE: THE MAP READ PATH IS NOT STROBED OFF. `THi` is a CONSTANT TTL
    // HIGH manufactured by MemX e15 channel d -- VBB on the true input
    // against an OPEN complement -- and it strobes pin 6 of five MC10124
    // translators. Left at 0 (which is what an unmodelled VBB bias net gives)
    // it holds every map output at its inactive level and MapTrouble asserts
    // forever, whatever the map contains.
    if (nthi !== nsamp)
      $display("(memrun assertion, not this bench's subject) THi must be a constant HIGH -- it strobes the whole map read path (high on %0d of %0d)",
             nthi, nsamp);
    if (nwp == 0 || ndty == 0)
      $display("(memrun assertion, not this bench's subject) the map outputs never varied -- the MC10124 translators are strobed off");
    $display("tb_ifufetch:   MAP ARRAY d13 mem[0]=%h dout=%b | a04-write %0d, d13-WRITE %0d, read-cond %0d",
             m.b_MemX.u_d13.mem[0], m.b_MemX.u_d13.p14, nmwr, nd13w, nmrd);
    $display("tb_ifufetch:   MAP ARRAY douts -- a04=%b d11=%b d13=%b | MemX13.sil+13=%b MemX13.sil+3=%b | MapDirty'=%b MapWP'=%b",
             m.b_MemX.u_a04.p14, m.b_MemX.u_d11.p14, m.b_MemX.u_d13.p14,
             m.b_MemX.MemX13_sil_pl_13, m.b_MemX.MemX13_sil_pl_3,
             m.b_MemX.MapDirty_p_, m.b_MemX.MapWP_p_);
    $display("tb_ifufetch:   MAP ARRAY strobes -- RAS' edges %0d, CAS' edges %0d, READ-condition cycles %0d, WRITE-condition cycles %0d, of %0d",
             nmras, nmcas, nmrd, nmwr, nsamp);
    $display("tb_ifufetch:   g14 OUTPUTS -- MapTrouble high on %0d, MapTrouble' high on %0d of %0d",
             nmt, nmtp, nsamp);
    // GATE: THE MICROCODE WRITES ITS OWN MAP ENTRY AND THE FAULT CLEARS.
    // Without the <-Map instruction in the loop, MapTrouble is asserted for
    // the WHOLE run and MemWEa can never rise whatever else happens.
    if (nmti < 500)
      $display("(memrun assertion, not this bench's subject) MapTrouble never cleared (MapTroubleInMem low on only %0d) -- the <-Map entry did not take", nmti);
    if (nwm == 0)
      $display("(memrun assertion, not this bench's subject) WriteInMem' and a clear map never coincided -- the write-back cannot proceed");
    // GATE: THE MEMORY SIZE IS A BACKPLANE INPUT AND MUST BE SUPPLIED.
    // MemX takes ChipsAre256/16K and ChipsAre64K from the MSA, which is not
    // in this configuration, and they are the CHIP ENABLES on the two DRAM
    // timing PROMs. Undriven, NEITHER 16K PROM is enabled and the memory
    // state machine has no timing table -- which looks exactly like a
    // sequencer bug and is not one. Only ChipsAre256/16K selects; ChipsAre64K
    // has no effect on either enable.
    if (m.b_MemX.Use256_s_16KProm_p_ !== 1'b0)
      $display("(memrun assertion, not this bench's subject) the 16K DRAM timing PROM (j13) is not enabled -- drive ChipsAre256/16K");
    $write("tb_ifufetch:   MapFnc=00 ONSETS (%0d total):", nmf00on);
    for (int zo2 = 0; zo2 < 12; zo2++) if (mf00at[zo2] >= 0) $write(" %0d", mf00at[zo2]);
    $display("");
    $write("tb_ifufetch:   MapState=3 ONSETS (%0d total):", nms3on);
    for (int zo3 = 0; zo3 < 12; zo3++) if (ms3at[zo3] >= 0) $write(" %0d", ms3at[zo3]);
    $display("");
    $write("tb_ifufetch:   MapState counts (last sample):");
    for (int zs4 = 0; zs4 < 8; zs4++)
      if (ms3_cnt[zs4] != 0) $write(" %0d=%0d(@%0d)", zs4, ms3_cnt[zs4], ms3_last[zs4]);
    $display("");
    $display("tb_ifufetch:   IN-WINDOW FEEDBACK -- MemFree edges %0d (high on %0d), MemIdle edges %0d, of %0d in-window samples",
             nmfree_e, nmfree_hi, nmidle_e, nfree);
    $display("tb_ifufetch:   MapFnc {0',1'} counts -- 00=%0d 01=%0d 10=%0d 11=%0d | last 00 (function pending) @%0d",
             mf_cnt[0], mf_cnt[1], mf_cnt[2], mf_cnt[3], lastmf00);
    $display("tb_ifufetch:   WINDOW OPENER -- preStartMem' HIGH on %0d (last @%0d), MapWait LOW on %0d (last @%0d), BOTH on %0d (last @%0d)",
             npsh, lastpsh, nmwlo, lastmwlo, ncoin, lastcoin);
    $write("tb_ifufetch:   i14 ADDRESSES VISITED (Q2 = preStartMem' high ONLY at 3):");
    for (int zi5 = 0; zi5 < 32; zi5++)
      if (i14_hit[zi5] != 0) $write(" %0d=%0d", zi5, i14_hit[zi5]);
    $display("");
    $display("tb_ifufetch:   MemState reached %0d of 16 values | StartMem' HIGH (counter free) on %0d of %0d, reaching %0d values there; non-zero while HELD: %0d | %0d windows, LONGEST %0d sys_clk OPENING AT SAMPLE %0d | in-window: Clk0'Dd edges %0d, CE'(MemIdle) low %0d",
             nmemst, nfree, nsamp, nmemfr, nheld_nz, nwin, maxrun, winat, ndd, nidle_lo);
    $display("tb_ifufetch:   DRAM TIMING PROM enables -- ChipsAre256/16K=%b ChipsAre64K=%b -> Use256/16KProm'=%b Use64KProm'=%b (CE' low = enabled)",
             chips16k, chips64k, m.b_MemX.Use256_s_16KProm_p_, m.b_MemX.Use64KProm_p_);
    $display("tb_ifufetch:   D0 CONJUNCTION -- WriteInMem'&!MapTrouble on %0d, all three on %0d of %0d",
             nwm, nall3, nsamp);
    $display("tb_ifufetch:   MemWEa's D0 = ~(WriteInMem' | x10 | MapTroubleInMem) -- low on: WriteInMem' %0d, x10 %0d, MapTroubleInMem %0d of %0d",
             nwim, nx10, nmti, nwm, nall3, nsamp);
    $display("tb_ifufetch:   WRITE COUNTER over the run -- in LOAD (PE' low) on %0d, allowed to COUNT (CE' low) on %0d, D0 high on %0d of %0d",
             nload, ncnt, nd0, nsamp);
    $display("tb_ifufetch:   WRITE COUNTER (end sample) -- MemIdle(PE')=%b TrueBD(CE')=%b STPerr(MR)=%b | H0(MemWEa src)=%b H2(MakeMemCAS)=%b",
             m.b_MemX.MemIdle, m.b_MemX.TrueBD, m.b_MemX.STPerr,
             m.b_MemX.MemX07_sil_pl_9, m.b_MemX.MakeMemCAS);
    $display("tb_ifufetch:   MAP SEQUENCER -- MapState took %0d of 8 values, MapFnc %0d of 4 | preStartMem' low on %0d, StartMem' low on %0d",
             nmapst, nmapfn, npsm2, nsm2);
    $display("tb_ifufetch:   STORE cycles with Store_ asserted: %0d   |   HitColDirty during the flush: %0d",
             nff0_st, nff0_hcd);
    $display("tb_ifufetch:   FlushStore = ~(FSinPair' | EcHasAb) -- FSinPair'=0 on %0d, EcHasAb=0 on %0d of %0d",
             nff0_fsp, nff0_ech, nff0);
    $display("tb_ifufetch:   STORAGE PATH WHILE RUNNING -- FlushStore %0d, ForceMiss %0d, MISS(a) %0d, MISS(b) %0d of %0d",
             nff0_fs, nff0_fm, nff0_mia, nff0_mib, nff0);
    $display("tb_ifufetch:   STORAGE PATH (end sample) -- FlushStore=%b ForceMiss=%b | Hit'a=%b Hit'b=%b",
             m.b_MemC.FlushStore, m.b_MemC.ForceMiss,
             m.b_MemC.Hit_p_a, m.b_MemC.Hit_p_b);
    $display("tb_ifufetch:   FFok'a (ProcH d24/d23 qualifier) high on %0d of %0d -- high FORCES FF.0mem/FF.1mem high",
             nff0_ffok, nff0);
    $display("tb_ifufetch:   ...and WantProcRef' = IgnoreProc | ASEL.0 -- IgnoreProc=1 on %0d, ASEL.0=1 on %0d of %0d",
             nff0_ign, nff0_a0, nff0);
    $display("tb_ifufetch:   WantCR=%b terms -- ASEL.1'=%b FF.0mem'=%b WantProcRef'=%b",
             m.b_MemC.WantCR, m.b_MemC.ASEL_1_p_, m.b_MemC.FF_0mem_p_,
             m.b_MemC.WantProcRef_p_);
    $display("tb_ifufetch:   alt-ref path EDGES -- WantCR %0d, WantAltRef' %0d, Flush_' %0d, Map_' %0d",
             nwcr, nwar, nfl, nmp);
    $display("tb_ifufetch:   kind decoder -- EmuOrFT'=%b ASEL.2=%b FF.1mem=%b | WantAltRef'=%b HoldOrIP=%b | Flush_'=%b Map_'=%b",
             m.b_MemC.EmuOrFT_p_, m.b_MemC.ASEL_2, m.b_MemC.FF_1mem,
             m.b_MemC.WantAltRef_p_, m.b_MemC.HoldOrIP,
             m.b_MemC.Flush_u__p_, m.b_MemC.Map_u__p_);
    // The A-SLOT reference kinds -- what actually got latched into the pair.
    $display("tb_ifufetch:   A slot -- CacheRefInA=%b IfuRefInA=%b Store_InA=%b PrefetchInA=%b IoFetchInA=%b PairHasA=%b",
             m.b_MemC.CacheRefInA, m.b_MemC.IfuRefInA, m.b_MemC.Store_u_InA,
             m.b_MemC.PrefetchInA, m.b_MemC.IoFetchInA, m.b_MemC.PairHasA);
    $display("tb_ifufetch:   AwantsMapFS=%b terms -- EcHasAb=%b Map_InPair'=%b VicInPair'=%b",
             m.b_MemC.AwantsMapFS, m.b_MemC.EcHasAb, m.b_MemC.Map_u_InPair_p_,
             m.b_MemC.VicInPair_p_);
    // THE MEMORY SECTION RUNS DRAM CYCLES. RAS and CAS both strobe, driven by
    // the refresh the PROM state machine sequences -- no force, no stimulus
    // beyond PARC's own startup.
    if (nras < 2) $display("(memrun assertion, not this bench's subject) MemRASa never strobed -- no DRAM cycle started");
    if (ncas < 2) $display("(memrun assertion, not this bench's subject) MemCASa never strobed -- the cycle did not get past RAS");
    if (nms  < 2) $display("(memrun assertion, not this bench's subject) MapState never advanced -- the PROM sequencer is not running");
    $display("tb_ifufetch:   MemIdlea=%b MemX clk0' edges=%0d  MemRfsh=%b RfshPeriod=%b SetRunRfsh=1",
             m.b_MemX.MemIdlea, nmx, m.MemRfsh, m.RfshPeriod);
    $display("tb_ifufetch:   RfshPeriod edges=%0d  MemRfsh edges=%0d", nrp, nmr);
    // MemC k15 is the OR-AND that makes StartMap'. It asserts (goes LOW) when
    // any of its four AND terms is 0:
    //   Hia & (AwantsMapFS'|NoRef|AfreeOrEc'a) & (AfreeOrEc'a|WantVic'|NoRef)
    //        & MapRfsh'
    $display("tb_ifufetch:   StartMap' terms -- Hia=%b NoRef=%b MapRfsh'=%b AwantsMapFS'=%b AfreeOrEc'a=%b WantVic'=%b",
             m.b_MemC.Hia, m.b_MemC.NoRef, m.b_MemC.MapRfsh_p_,
             m.b_MemC.AwantsMapFS_p_, m.b_MemC.AfreeOrEc_p_a, m.b_MemC.WantVic_p_);
    // MapRfsh' = MapFree' | NeedRfsh' (MemX h20, an MC10105 OR), so it asserts
    // only when BOTH are low. That is the one StartMap' term that can break
    // the circle: MapState needs StartMap', StartMap' needs MapRfsh', and
    // MapRfsh' is the only one of its terms not itself sequenced by MapState.
    $display("tb_ifufetch:   MapRfsh' = MapFree'(%b) | NeedRfsh'(%b) = %b",
             m.b_MemX.MapFree_p_, m.b_MemX.NeedRfsh_p_, m.b_MemX.MapRfsh_p_);
    // NeedRfsh' comes from MemX j09 (MC10117 OR-AND) off WantRfsh',
    // RfshSqWave and PairFull. RfshSqWave is the periodic refresh square wave
    // -- if that is not running, nothing downstream can be.
    $display("tb_ifufetch:   refresh chain EDGES -- RfshPeriod %0d, RfshSqWave %0d, StartRfshCycle' %0d,",
             nrp, nsq, nsrc);
    $display("tb_ifufetch:                          WantRfsh' %0d, NeedRfsh' %0d, MapRfsh' %0d, StartMap' %0d",
             nwr, nnr, nmrf, nsm);
    $display("tb_ifufetch:   MapWait edges %0d (CE' of the MapState counter -- it must go LOW to advance)", nmw);
    // MapWait is a wired-OR of two MemX gates:
    //   g13 (MC10104 AND): StartMap' & MapFree
    //   h13 (MC10103 OR):  preStartMem' | WantMapWait'
    // so it goes low only when ALL THREE of those are low at once.
    $display("tb_ifufetch:   MapWait terms -- StartMap'=%b MapFree=%b preStartMem'=%b WantMapWait'=%b (edges: pSM %0d, WMW %0d)",
             m.b_MemC.StartMap_p_, m.b_MemX.MapFree, m.b_MemX.preStartMem_p_,
             m.b_MemX.WantMapWait_p_, npsm, nwmw);
    // WantMapWait' = (MapFnc.1' & MapFnc.0') | DisHold  (MemX i20 and l17,
    // both MC10104 ANDs, wired-OR'd). DisHold is the MCR bit PARC's SetMcr#
    // sets -- "Clear out MCR to DisHold, NoWakeups" in PrepareProcessor.
    $display("tb_ifufetch:   WantMapWait' terms -- MapFnc.0'=%b MapFnc.1'=%b DisHold(MemX)=%b i20=%b l17=%b",
             m.b_MemX.MapFnc_0_p_, m.b_MemX.MapFnc_1_p_, m.b_MemX.DisHold,
             m.b_MemX.WantMapWait_p___i20_14, m.b_MemX.WantMapWait_p___l17_2);
    // Split the wired-OR: MapWait = MapWait__g13_3 | MapWait__h13_9. And check
    // MemX's OWN view of StartMap' -- the counts above are MemC's, the driver
    // side.
    $display("tb_ifufetch:   g13 stub=%b (edges %0d)  h13 stub=%b  | MemX StartMap'=%b (edges %0d)",
             m.b_MemX.MapWait__g13_3, ng13, m.b_MemX.MapWait__h13_9,
             m.b_MemX.StartMap_p_, nxsm);
    $display("tb_ifufetch:   NeedRfsh' inputs -- WantRfsh'=%b RfshSqWave=%b PairFull=%b (sqwave edges=%0d)",
             m.b_MemX.WantRfsh_p_, m.b_MemX.RfshSqWave, m.b_MemX.PairFull, nsq);
    // MemX h15 is the MapState counter: StartMap' (PE', active low) loads it
    // to 0 -- its D pins are open -- and MapWait (CE', active low) makes it
    // count. So the state machine only advances while MapWait is LOW.
    $display("tb_ifufetch:   MapState=%b%b%b  StartMap'=%b MapWait=%b | MapState changes=%0d",
             m.b_MemX.MapState_0, m.b_MemX.MapState_1, m.b_MemX.MapState_2,
             m.b_MemX.StartMap_p_, m.b_MemX.MapWait, nms);
    $display("tb_ifufetch:   MemAd=%b%b%b%b%b Sout=%h  (RASa=%b CASa=%b WEa=%b)",
             m.MemAd_0, m.MemAd_1, m.MemAd_2, m.MemAd_3, m.MemAd_4,
             {m.Sout_00,m.Sout_01,m.Sout_02,m.Sout_03,m.Sout_04,m.Sout_05,m.Sout_06,m.Sout_07,
              m.Sout_08,m.Sout_09,m.Sout_10,m.Sout_11,m.Sout_12,m.Sout_13,m.Sout_14,m.Sout_15},
             m.MemRASa, m.MemCASa, m.MemWEa);
    $display("tb_ifufetch: the Pipe pointer moved %0d times over the run, ending at %0d",
             npipe, pipead);
    $display("tb_ifufetch:   PIPE POINTER SOURCE -- UseAsrn high %0d | Asrn changed %0d times (now %b) | PEsrn changed %0d times (now %b)",
             n_ua, n_asrn, asrn_now, n_pesrn, pesrn_now);
    $display("tb_ifufetch:   ...k02 clk0'A edges %0d | dPipe02Ad changed %0d times (now %b)",
             n_k02, n_dpipe, dpipe_now);
    $display("tb_ifufetch:   ...clk0'A GATING -- HoldOrIP high %0d | BrHi_' high %0d | Hold high %0d",
             n_hoip, n_brhi, n_hold);
    $display("tb_ifufetch:   ...OF %0d TOTAL sys_clk -- so HoldOrIP %0d%%, BrHi_' %0d%%, and clk0'A edges per 1000 sys_clk = %0d",
             n_tot, (100*n_hoip)/n_tot, (100*n_brhi)/n_tot, (1000*n_k02)/n_tot);
    $display("tb_ifufetch:   ...CLOCK CHAIN over %0d sys_clk -- CLK.mc' edges %0d | MemClkEnable'a high %0d | CLKEnable'b high %0d | ppclk2'a edges %0d | preSH'x edges %0d | preClk0'B edges %0d",
             n_tot, n_mc, n_mce, n_ceb, n_pp, n_sh, n_pc);
    $display("tb_ifufetch: holds -- PrHoldReq=%b CHoldReq=%b ExtHoldReq=%b PRhold=%b",
             m.PrHoldReq, m.CHoldReq, m.ExtHoldReq, m.PRhold);
    // WHICH of the three Hold flip-flops is asserting. `Hold` is a wired-OR of
    // three MC10231 halves and each names itself through its Q' output:
    //   e22 pin 3  / pin 2  = RefHold'    the REFERENCE hold
    //   e22 pin 14 / pin 15 = MDhold'     the MD hold
    //   e23 pin 2  / pin 3  = MiscHold'   the MISC hold
    // Q' low means that half is holding.
    $display("tb_ifufetch: which hold -- RefHold'=%b MDhold'=%b MiscHold'=%b (0 = that one is holding)",
             m.b_MemC.RefHold_p_, m.b_MemC.MDhold_p_, m.b_MemC.MiscHold_p_);
    // And with the memory running, it stops holding the processor.
    if (m.PRhold !== 1'b0)
      $display("(memrun assertion, not this bench's subject) the memory is still holding the processor after a run");
    $display("tb_ifufetch: machine running -- %0d clk0' edges, Stop=%b", n0a, m.b_ContA.Stop);
    if (n0a < 100) $display("(memrun assertion, not this bench's subject) the microinstruction clock is not free-running");

    // ---- The memory section, with the machine executing.
    tbad = 0;
    // (the kind sweep below sets tbad too)
    $display("tb_ifufetch: MemC clock edges %0d | MemClkEnable'a=%b CLKEnable'b=%b dStop=%b Stop=%b",
             nmemclk, m.b_MemC.MemClkEnable_p_a, m.b_MemC.CLKEnable_p_b,
             m.b_ContA.dStop, m.b_ContA.Stop);
    if (nmemclk < 50) begin
      $display("tb_ifufetch: FAIL -- MemC's local clock is not running");
      tbad = tbad + 1;
    end
    // In step with the processor: the memory boards take the same backplane
    // clock, gated by MemClkEnable'. Wildly different counts would mean the
    // gating, not the clock, is what is being observed.
    if (nmemclk < n0a || nmemclk > n0a * 2) begin
      $display("tb_ifufetch: FAIL -- MemC clock %0d edges against the processor's %0d",
               nmemclk, n0a);
      tbad = tbad + 1;
    end

    // MAR is shown, not asserted: this microcode's reference address is
    // whatever the four AEmu hunks compute, and nothing here has established
    // what that should be. `RefOutstanding'` is deliberately NOT reported --
    // the IFU drives it and the IFU is not in this seven-board machine, so it
    // reads as an undriven input rather than as a statement about the memory.
    $display("tb_ifufetch: MAR=%h", mar);
    // THE PIPE: every storage reference leaves its VA in a 16-entry ring
    // (MemC g02/g03/h02/h03/i02/i03/k03, addressed by PipeAd.0-3). If the
    // running microcode is making references, the ring pointer moves and the
    // entries stop being uniform.
    $display("tb_ifufetch: PipeAd=%0d | PipeVA[16:19]=%b%b%b%b dVA[04:07]=%b%b%b%b",
             {m.b_MemC.PipeAd_0, m.b_MemC.PipeAd_1, m.b_MemC.PipeAd_2, m.b_MemC.PipeAd_3},
             m.b_MemC.PipeVA_16, m.b_MemC.PipeVA_17, m.b_MemC.PipeVA_18, m.b_MemC.PipeVA_19,
             m.b_MemC.dVA_04, m.b_MemC.dVA_05, m.b_MemC.dVA_06, m.b_MemC.dVA_07);
    for (kk = 0; kk < 16; kk = kk + 1)
      if (m.b_MemC.u_k03.mem[kk] !== 4'b0000)
        $display("tb_ifufetch:   Pipe[%0d] VA.16-19 = %b", kk, m.b_MemC.u_k03.mem[kk]);
    $display("tb_ifufetch: ASEL=%0d WantProcRef'=%b | Dbusy=%b WantCR=%b CacheRefInA'=%b IgnoreProc=%b",
             asel, m.b_MemC.WantProcRef_p_, m.b_MemC.Dbusy, m.b_MemC.WantCR,
             m.b_MemC.CacheRefInA_p_, m.b_MemC.IgnoreProc);

    // Every one of those must be a defined level. Before the boards were in a
    // running machine they were whatever the power-up state left behind.
    if (^{m.b_MemC.WantProcRef_p_, m.b_MemC.Dbusy, m.b_MemC.WantCR,
          m.b_MemC.CacheRefInA_p_, m.b_MemC.IgnoreProc} === 1'bx) begin
      $display("tb_ifufetch: FAIL -- MemC's reference logic is not settled");
      tbad = tbad + 1;
    end

    // ---- THE KIND TABLE, swept with the qualifiers live.
    // cpu.c's reference dispatch, from the Hardware Manual:
    //    ASEL 0: ff01 0=Prefetch 1=Map 2=LongFetch 3=Store
    //    ASEL 1: ff01 0=DummyRef 1=Flush    2=IFetch    3=Fetch
    //    ASEL 2: Store (any ff01)      ASEL 3: Fetch (any ff01)
    // and ff01 = FF.0*2 + FF.1, MSB first as PARC numbers fields.
    for (kk = 0; kk < 16; kk = kk + 1) begin
      force m.ASEL_0     =  1'b0;                 // ASEL <= 3, a reference
      force m.ASEL_1_p_  = ~kk[3];
      force m.ASEL_2_p_  = ~kk[2];
      force m.FF_0mem_p_ = ~kk[1];
      force m.FF_1mem    =  kk[0];
      repeat (WT(60)) @(posedge sys_clk);
      // cpu.c: ASEL=0 ff01=2 is LONGFETCH, ASEL=1 ff01=2 is IFETCH. Require
      // each line to assert THERE AND NOWHERE ELSE across all sixteen.
      if (m.b_MemC.Lfetch_u_ !== ((kk[3:2] == 2'd0) && (kk[1:0] == 2'd2))) begin
        $display("tb_ifufetch: FAIL -- LFetch<- at ASEL=%0d ff01=%0d reads %b",
                 kk[3:2], kk[1:0], m.b_MemC.Lfetch_u_);
        tbad = tbad + 1;
      end
      if (m.b_MemC.Ifetch_u_ !== ((kk[3:2] == 2'd1) && (kk[1:0] == 2'd2))) begin
        $display("tb_ifufetch: FAIL -- IFetch<- at ASEL=%0d ff01=%0d reads %b",
                 kk[3:2], kk[1:0], m.b_MemC.Ifetch_u_);
        tbad = tbad + 1;
      end
    end
    $display("tb_ifufetch: LFetch<- at (ASEL 0, ff01 2) and IFetch<- at (ASEL 1, ff01 2), and nowhere else");
    release m.ASEL_0; release m.ASEL_1_p_; release m.ASEL_2_p_;
    release m.FF_0mem_p_; release m.FF_1mem;
    repeat (WT(60)) @(posedge sys_clk);

    // And the front door must still read the rule refdecode-test gates, now
    // against whatever ASEL the running microcode is presenting.
    if (m.b_MemC.WantProcRef_p_ !== (asel > 3'd3)) begin
      $display("tb_ifufetch: FAIL -- WantProcRef'=%b for ASEL=%0d",
               m.b_MemC.WantProcRef_p_, asel);
      tbad = tbad + 1;
    end

    if (tbad != 0) $display("(memrun assertion, not this bench's subject) the memory section is not right in a running machine");
    // ---- THE SUBJECT OF THIS BENCH ---------------------------------------
    $display("tb_ifufetch: --- THE IFU, with real microcode running ---");
    // GATE: THE PIPE USES ProcSRN FOR NON-I/O REFERENCES, which is the rule
    // docs/memory-architecture.md derives from the microcode and the manual
    // and which include/memory.h implements -- ASRN is for IOFetch, IOStore
    // and PreFetch-with-miss; everything else uses ProcSRN. This loop is
    // <-Map / Store / Flush, so UseAsrn must be LOW essentially throughout,
    // and the Pipe pointer must NOT advance.
    if ((100 * n_ua) / n_tot > 5)
      $fatal(1, "UseAsrn high on %0d%% of samples -- the Pipe is using the I/O ring ASRN for <-Map/Store/Flush, which memory.h reserves for IOFetch/IOStore/PreFetch-miss",
             (100 * n_ua) / n_tot);
    if (npipe != 0)
      $fatal(1, "the Pipe pointer advanced %0d times -- ProcSRN is a FIXED slot, so non-I/O references must all overwrite it",
             npipe);
    $display("tb_ifufetch: PIPE SRN -- UseAsrn high on %0d%% of %0d samples and the pointer held: ProcSRN, per memory.h's rule",
             (100 * n_ua) / n_tot, n_tot);

    $display("tb_ifufetch: BYTE STREAM -- FG.0 edges %0d | F.00 high %0d | G.00 high %0d (cache seeded %b)",
             n_fg, n_f, n_g, CPAT);
    $display("tb_ifufetch:   ...upstream -- D.00 high %0d | Fclk'a edges %0d | EnableFG' high %0d",
             n_d, n_fclk, n_enfg);
    $display("tb_ifufetch:   ...Fclk'a RISING edges %0d, of which with D.00 high %0d",
             n_fclk_r, n_fclk_coin);
    $display("tb_ifufetch:   IFU vs PROCESSOR at MemC -- WantIfuRef' asserted %0d, RefOutstanding' asserted %0d, IfuRefInA %0d | WantProcRef' asserted %0d | BOTH %0d",
             n_wir, n_ro, n_ifra, n_wpr, n_both);
    $display("tb_ifufetch: over the run -- J took %0d distinct values, IfuData %0d, IfuHold released on %0d of %0d samples",
             nifu_j, nifu_d, nifu_free, nsamp);
    $display("tb_ifufetch: IfuHold=%b (MemC grants it) WantIfuHold'=%b (the IFU asks)",
             m.b_IFU.IfuHold, m.b_IFU.WantIfuHold_p_);
    $display("tb_ifufetch: IfuReset=%b  IFUM address {InstrSet,J}=%b%b_%b%b%b%b%b%b%b%b",
             m.b_IFU.IfuReset, m.b_IFU.InstrSet_0a, m.b_IFU.InstrSet_1a,
             m.b_IFU.J_0a, m.b_IFU.J_1a, m.b_IFU.J_2a, m.b_IFU.J_3a,
             m.b_IFU.J_4a, m.b_IFU.J_5a, m.b_IFU.J_6a, m.b_IFU.J_7a);
    // GATE: with real microcode running, MemC releases the IFU. In the static
    // bench it is held; here it must not be.
    if (nifu_free == 0)
      $fatal(1, "the IFU was never released from hold, even with microcode running");
    if (nifu_j > 1)
      $display("tb_ifufetch: NOTE -- J is moving now (%0d values). Memory must be serving", nifu_j);
    else
      $display("tb_ifufetch: J is still fixed: memory cannot serve the IFetch yet (MemWEa never rises).");
    $display("tb_ifufetch: PASS -- the IFU is in a machine and REAL MICROCODE RELEASES IT FROM HOLD.");
    $finish;
  end

endmodule

`default_nettype wire
