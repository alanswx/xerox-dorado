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

// tb_taskrun -- BNT IN A RUNNING MACHINE: the wakeup register, not just the
// encode.
//
// `task-test` gates the COMBINATIONAL priority encoder (PEnc) against cpu.c's
// task_bnt(). It cannot gate BNT, because BNT is a register: ContA h08/h09 are
// MC10141 shift registers, and their parallel load needs `BNextRegsEn'` low on
// both select pins and `clk0'` -- the microinstruction clock -- running. So
// BNT only moves when the machine is EXECUTING, which is what this does.
//
// It reuses tb_exec's startup exactly: init the manifolds, walk four hunks of
// Xerox's own AEmu.mb into IM through the control-processor bus, release the
// MIR clock, put the start address in Link and jam a free-running `Return#`.
// Then, with the machine fetching from IM, it drives the fifteen TWReq lines
// and watches BNT follow.
//
// WHY BNT FOLLOWS WITHOUT TASKING BEING ON. `Return#` is "TaskingOff,Return",
// so the machine starts with tasking off and will not SWITCH. BNT still
// tracks, because ContA h10 is an MC10166 magnitude comparator wired to
// compare BNT against PEnc: `BNextRegsEn'` asserts exactly when the two
// DIFFER, so the register reloads whenever the encoder's answer changes. It is
// a load-on-change, independent of whether a switch is allowed.
//
// AND IT SWITCHES. The machine is started with TaskingOn rather than PARC's
// TaskingOff, and then CTask is required to BE the task that asked.
//
// The encoding is not guessed. ContA a16, an MC10100, decodes
//
//     FF=TaskingOn  = ~(FB=4' | FC=3' | FA=1')   -> FA=1, FB=4, FC=3 = 143 oct
//     FF=TaskingOff = ~(FB=4' | FC=2' | FA=1')   -> FA=1, FB=4, FC=2 = 142 oct
//
// and 142 is exactly the FF the IRTable states for `Return#`
// ("TaskingOff,Return; ... FF[142]"), so the decode checks against PARC's own
// table. FF.7 lives in byte 3 bit 3 of the five-byte format, so 0x42 | 0x08 =
// 0x4A turns Return# into TaskingOn,Return -- verified by decoding both byte
// strings back through the layout doradoboot.masm states.
//
// The control is the same run with 0x42 instead of 0x4A: `TaskingIsOff'` reads
// 0, BNT still tracks the encoder to 15, and CTask STAYS AT 0. One bit of one
// microinstruction decides whether the machine switches, and nothing else
// changes.
//
// AND EACH TASK KEEPS ITS OWN PC. TPC is four F10145A packages -- sixteen
// words of four bits, l13 holding TPC.00-03, i13 04-07, j13 08-11, k13 12-15 --
// addressed by `TPCAd`, which is shown here to follow the current task. The
// check that matters is not that the array exists but that it is SEPARATE
// storage: task 15 is run, its slot recorded, task 7 is then run, and task
// 15's slot must be untouched. A machine with one program counter would have
// overwritten it. Guarded against passing vacuously in both directions -- an
// all-zero array would satisfy "unchanged" without storing anything, and two
// slots reading alike would satisfy it without being separate.
//
// AND THE OTHER PER-TASK FILES ARE INDEXED TOO. ContA a04-e04 hold Link,
// addressed by `TLinkAd`; ProcH and ProcL carry files addressed by `CurrLast'`
// and `LastNext'`, two different task numbers because the Dorado pipelines the
// switch and its stages do not agree on which task is current. All four
// addresses -- TPCAd, TLinkAd, CurrLast, LastNext -- are required to follow
// the requesting task.
//
// For Link the CONTENT check has to be made differently from TPC's, and the
// difference is instructive. TPC advances every instruction, so each task's
// slot fills in by itself. Link changes only on an explicit `Link<-`, which
// this microcode never executes -- so Link[7] and Link[15] reading alike
// proves nothing, because neither was ever written. What proves it is the
// STARTUP: `CPRegToLink#` puts the start address in Link while the machine is
// in task 0, and slot 0 then reads fffb0 against f7f70 everywhere else.
//
// Still to come: T and MemBase, on the Processor and Memory boards.

// tb_memrun -- THE MEMORY BOARDS IN A RUNNING MACHINE.
//
// `mem-test` showed MemC + MemD + MemX elaborating and their local clocks
// following `MemClkEnable'`. `refdecode-test` showed MemC's front door -- the
// ASEL decode that says a microinstruction is asking for storage. Neither has
// the boards in a machine that is EXECUTING, and the pieces that decide which
// KIND of reference it is cannot be reached without that: `Dbusy`, `WantCR`,
// `CacheRefInA'` and `IgnoreProc` are all internal to MemC, driven by board
// state rather than by ports, so a standalone bench reads them as whatever
// they power up to.
//
// So this is tb_taskrun's harness moved onto `dorado_mem`, the seven-board
// machine: initialise the manifolds, walk four hunks of Xerox's own AEmu.mb
// into IM over the control-processor bus, release the MIR clock, put the start
// address in Link and jam a free-running Return#. `SetRunRfsh` is asserted --
// it is a BaseBoard backplane line and one half of `MemClkEnable'`, so without
// it the memory boards' clocks stay gated off (see tb_mem).
//
// What it establishes: MemC's local clock runs in step with the processor's
// (187 edges against 181), the reference logic settles to DEFINED levels
// rather than to its power-up state, and -- the part that is more than
// plumbing -- THE RUNNING MICROCODE ACTUALLY ASKS FOR STORAGE. The four hunks
// of AEmu.mb present ASEL=0, `WantProcRef'` asserts, and MemC responds with
// `Dbusy` and `WantCR` set. That is the front-door rule refdecode-test gates,
// now holding against an ASEL the machine chose for itself rather than one a
// bench forced.
//
// AND TWO OF THE REFERENCE KINDS MATCH THE C EMULATOR EXACTLY. With the
// qualifiers live, sweeping ASEL 0-3 against the two FF bits gives
//
//     ASEL=0 ff01=2 -> LFetch<-      cpu.c: DM_REF_LONGFETCH
//     ASEL=1 ff01=2 -> IFetch<-      cpu.c: DM_REF_IFETCH
//
// each asserting there and nowhere else in the sixteen. (`ff01` is FF.0*2 +
// FF.1, MSB first as PARC numbers fields; the `_` suffix is Sil's assignment
// arrow, so these read LFetch<- and IFetch<-.) Two derivations that share no
// code -- a 1979 wire list on one side, the Hardware Manual on the other --
// agreeing on where two opcodes live in a sixteen-entry table.
//
// The rest of the table is NOT gated. Store<-, Prefetch and the two IO kinds
// read 0 throughout because they are not raw decoder outputs: Store<- comes
// off j22, an MC10105 OR, and the IO forms are additionally qualified by
// whether the current task is an I/O task (cpu.c has the same condition:
// `io_task ? DM_REF_IOFETCH : DM_REF_MAP`). Reaching those needs the machine
// in a task that makes them meaningful, which is the next step.
//
// PARC'S STARTUP HAS A STEP THIS WAS SKIPPING, and it is now here -- but it is
// NOT what clears the memory hold, and a first pass claimed it was.
// PrepareProcessor ends with
//
//     LDXI 103o / LDAI TFromCPReg# / JSR SetCPAndDoIRTableInst
//     LDAI SetMcr# / JSR DoIRTableInst        ; "Clear out MCR to DisHold,
//                                             ;  NoWakeups"
//
// and MemC's Hold flip-flops (e22, e23) do take DisHold on their set/reset,
// which made it look causal: add the step, and `PRhold` reads 0. But
// SUPPRESSING THE WHOLE BLOCK LEAVES IT READING 0 TOO. The hold was already
// clear at that point in the startup; the `PRhold`=1 that prompted this was
// sampled at the END of a run, not before it. The step stays because it is
// part of PARC's sequence and being faithful to that is worth something on its
// own, but it is not load-bearing and nothing here depends on it.
//
// The assertion that remains is the honest one: the memory must not be holding
// the processor BEFORE the machine starts. That is true, checkable, and would
// catch a regression that held from reset -- it is simply not caused by
// SetMcr#.
//
// The bytes come from doradoboot.masm, which is radix 2: TFromCPReg# =
// 70 03 0F 04 C0, SetMcr# = 30 02 0B 84 60, checked by decoding Nop#'s
// `01^6.+(11^4.)` the same way and getting the 0x70 that nop_micro already
// uses. And TFromCPReg# needs the PLAIN SetCPReg rather than the tilde form,
// because BMux is the complement of CPReg and `alub` is inverted back off it,
// so T ends up equal to CPReg while IM write data ends up equal to its
// complement -- which is exactly why PARC uses the two forms where it does.
//
// THE MEMORY SECTION RUNS DRAM CYCLES, from PARC's own startup and nothing
// else. What it took, in the order it had to be discovered:
//
//   1. `RfshPeriod` comes from the BASEBOARD (a01, an MC10176 that also makes
//      SetRunRfsh and Pendulum), so this seven-board machine lists it "awaits
//      BaseBd" and it read 0 for ever. Driven here.
//
//   2. `DisHold` is the lever. MemX i20 and l17 are two MC10104 ANDs
//      wired-OR'd to `WantMapWait' = (MapFnc.1' & MapFnc.0') | DisHold`, and
//      MapWait -- CE' on the MapState counter -- cannot fall without it. Note
//      h13's contribution to MapWait comes from PIN 9 OF AN MC10103, which is
//      the INVERTING output (EclDict: `c,IN,12,13 > c,OUT,9 > c,o,15`), so
//      with its inputs low it holds MapWait HIGH. The requirement is the
//      opposite of what it first looks like.
//
//   3. DisHold is set by PARC's own PrepareProcessor: LDXI 103o /
//      TFromCPReg# / SetMcr#. And TFromCPReg# needs the ALU prologue first --
//      its IRTable entry says "requires ALUFM[0]=B" -- LDXI 25o /
//      QFromCPReg# / ALUFM[0]FromQ#.
//
//   4. AND THE PIPELINE HAS TO BE WARMED FIRST. The first microinstruction out
//      of a cold stop runs only its clk0 half, so a jam issued straight after
//      the manifolds loses half its effect. tb_compute does `zero; nop; nop`
//      before the same prologue; this bench did not, and that one omission was
//      why every step of PARC's sequence silently did nothing. THE BENCH WAS
//      WRONG, NOT THE MACHINE.
//
// With all four: Q = 0015 (25 octal), T = 0043 (103 octal), DisHold = 1 on
// both boards, MapState stepping, MemRASa strobing 6 times and MemCASa 4, and
// PRhold back to 0 with all three hold flip-flops deasserted -- the memory no
// longer holds the processor. Those are the gate's assertions.
//
// Mutation-tested, and the second one confirms PARC's own comment: drop the
// warming and DisHold stays 0; drop ALUFM[0]FromQ# and Q still loads (0015)
// while T stays 0000, which is exactly the "requires ALUFM[0]=B" dependency.
//
// STILL TO COME. These are REFRESH cycles, which is why the Pipe pointer does
// not move -- refresh does not record a reference.
//
// A REFERENCE CANNOT BE JAMMED, and that is worth knowing before trying. A
// reference microinstruction IS built here with mi() (ASEL=1, BSEL=4) and
// jammed while the MIR is held, and it does put the A leg on MAR -- but
// `WantProcRef'` stays 1 because `IgnoreProc` reads 1 at that moment, and
// `WantProcRef' = IgnoreProc | ASEL.0`. The ASEL reaches MemC correctly; the
// memory is simply IGNORING THE PROCESSOR while it is jam-stepped. During the
// free run the same signal reads 0.
//
// So a processor reference has to come from microcode EXECUTING OUT OF IM, and
// that is what this bench does now: `build_hunk` packs four copies of a
// microinstruction into PARC's 17-byte hunk format (byte 0 carries the 17th
// bit of each half, which is why 8 half-instructions of 17 bits need 17
// bytes), and `send_a_hunk` walks them into IM[0..3] over the same path the
// AEmu hunks use.
//
// WHERE THAT GETS TO. With four ASEL=1 references executing:
//
//     MemRASa   6 -> 11        MemCASa   4 -> 8
//     WantProcRef' 1 edge -- it asserts and STAYS asserted, which is right
//                            when every instruction is a reference
//     RefHold'     0 edges
//     Pipe pointer 0 changes
//
// So the microcode ASKS continuously and the memory never SERVICES it: no
// reference hold is ever taken and nothing is recorded in the Pipe. The extra
// RAS/CAS is the refresh machinery, which was already running.
//
// AND THE REFERENCE IS SERVICED -- BY THE CACHE. Two wrong conclusions were
// reached on the way here and both are worth keeping, because each looked
// solid:
//
//   WRONG 1: "the memory never services the reference". It does.
//   WRONG 2: "the reference is never latched into the A/B pair". It is.
//
// What is actually measured, once the right signals are looked at:
//
//     LdPair' edges 188      the pair register IS clocked, every
//                            microinstruction (MemC j17: LdPair' =
//                            ~(preClk0'D | AfreeOrEc'a | EcKeepsAbusy), and
//                            neither of the other two is blocking)
//     PairHasA = 1           the A slot is occupied
//     CacheRefInA = 1        and what is in it is a CACHE reference
//     Hit'a = 0, Hit'b = 0   ACTIVE LOW -- THE CACHE HITS
//
// A cache hit is serviced out of the cache. It needs no storage cycle, takes
// no reference hold, and records no Pipe entry -- so every "missing" symptom
// this trace chased is the CORRECT behaviour for a hitting reference.
//
// WHAT MISLED ME. `Map_InPair'` and `VicInPair'` both read 1, which killed two
// of the three gates feeding `AwantsMapFS` and looked like the reference not
// arriving. They are correct: an ASEL=1 reference is a FETCH, not a Map
// operation and not a victim, so the machine correctly does not want the map.
// Reading "InPair = 1" as "not in the pair" was the error; those signals name
// WHICH KIND is in the pair, and this kind is a cache reference.
//
// TO REACH THE STORAGE PATH, FORCE A MISS -- and PARC built a signal for it.
// The hit is a wired-OR of four MC1660 NORs, one per way (MemC f16 pins 3/14,
// f17 pins 3/14):
//
//     way N hits = ~(ForceMiss | dMissN.04-11 | dMissN.12-15 | dMissN.16-19)
//
// so `ForceMiss` alone kills every way. MemC k19 (MC10109) makes it:
//
//     ForceMiss = ForceDirtyMiss | bEcHasA | VictimInA | FlushStore
//
// The hit here is almost certainly spurious -- the cache tags have never been
// initialised, so the dMiss bits read 0 and every way matches -- but the
// mechanism is working, which is the thing worth knowing.
//
// TRIED AND NOT SUFFICIENT: a Flush reference. cpu.c's dispatch calls ASEL=1
// with ff01=1 a FLUSH, and FlushStore is one of ForceMiss's terms, so it
// should force a miss. Loading four of those into IM still gives
// CacheRefInA = 1 -- the kind decodes as a plain cache reference. That is
// task 4's problem, not this one: only LFetch<- and IFetch<- decode cleanly
// from ASEL and the two FF bits, while Store<- and the Flush/IO kinds are
// qualified by board state (and, for the IO kinds, by the current task being
// an I/O task -- a condition cpu.c has too). So the kind table has to be
// finished before a Flush can be issued deliberately.
//
// Then the storage array, whose interface msa.bp specifies completely:
// MemAd.1-8, MemRAS/CAS/WE in a and b copies from DIFFERENT packages (two
// banks, not fan-out), Sin.00-15, Sout.00-15, EcIn/EcOut. Direction comes from
// the netlist because the names mislead: MemD DRIVES `Sout` (write data going
// to storage) and RECEIVES `Sin` (read data coming back).
//
// `+define+FORCE_DISHOLD` is left in as a diagnostic. It is no longer needed --
// the real sequence sets DisHold -- but it is how the lever was found.
//
// ONE THING THIS COST, worth not repeating: `dorado_mem` has three more clock
// ports than `dorado_proc` -- CLK_mc', CLK_md', CLK_mx' -- because the
// BaseBoard fans the clock to every slot. Leaving them undriven gives MemC
// zero local clock edges with BOTH its enables already asserted, which looks
// exactly like a gating bug and is not one.

`default_nettype none


module tb_memrun;

  localparam integer GAP = 200;   // sys_clk between Control strobes

  reg sys_clk = 1'b0;
  always #1 sys_clk = ~sys_clk;
  reg [3:0] ckd = 4'd0;
  always @(posedge sys_clk) ckd <= ckd + 4'd1;
  wire mclk = ckd[3];

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

  dorado_mem m (
      .sys_clk(sys_clk),
      .CLK_ca_p_(mclk), .CLK_cb_p_(mclk), .CLK_ph_p_(mclk), .CLK_pl_p_(mclk),
      // The BaseBoard fans the clock to EVERY slot, and dorado_mem has three
      // more than dorado_proc. Leaving them undriven is why MemC's local clock
      // showed zero edges with both its enables already asserted.
      .CLK_mc_p_(mclk), .CLK_md_p_(mclk), .CLK_mx_p_(mclk),
      .CPAddr_0_p_(addr_n[2]), .CPAddr_1_p_(addr_n[1]), .CPAddr_2_p_(addr_n[0]),
      .CPOut_0(cpout[8]), .CPOut_1(cpout[7]), .CPOut_2(cpout[6]),
      .CPOut_3(cpout[5]), .CPOut_4(cpout[4]), .CPOut_5(cpout[3]),
      .CPOut_6(cpout[2]), .CPOut_7(cpout[1]), .CPOut_8(cpout[0]),
      .CPStrb_p_(strb_n), .SetRun(setrun), .SetSS_p_(setss_n),
      .SetRunRfsh(1'b1), .RfshPeriod(rfshper)
  );

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
      repeat (4) @(posedge sys_clk);
      strb_n = 1'b0; repeat (6) @(posedge sys_clk);
      strb_n = 1'b1;
      if (fn == 3'd0) begin           // Control: g07 clocks on release
        setrun  =  data[0];
        setss_n = ~ss;
      end
      repeat (4) @(posedge sys_clk);
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
        repeat (4) @(posedge sys_clk); dmc = 1'b1;
        repeat (4) @(posedge sys_clk); dmc = 1'b0;   // the chain shifts on the fall
        repeat (4) @(posedge sys_clk);
      end
      udmd = 1'b1; repeat (12) @(posedge sys_clk);
      udmd = 1'b0; repeat (12) @(posedge sys_clk);
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
  integer nrp, nmr, nms, nsq, nsrc, nwr, nnr, nmrf, nsm, nmw, npsm, nwmw, ng13, nxsm, nwpr, nrh, nldp, npha, ncra, nha, nhb;
  reg psq, psrc, pwr, pnr, pmrf, psm, pmw, ppsm, pwmw, pg13, pxsm, pwpr, prh, pldp, ppha, pcra, pha, phb;
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
      repeat (400) @(posedge sys_clk);
      strobe(3'd1, 8'h21, 1'b0);
      strobe(3'd2, v[15:8], 1'b0); strobe(3'd3, v[7:0], 1'b0);
      strobe(3'd0, 8'h4E, 1'b0);
      strobe(3'd0, 8'h00, 1'b1);
      strobe(3'd4, 8'h13, 1'b0); strobe(3'd5, 8'hEF, 1'b0);
      strobe(3'd6, 8'h04, 1'b0); strobe(3'd7, 8'h40, 1'b0);
      strobe(3'd0, 8'h41, 1'b1);
      repeat (600) @(posedge sys_clk);
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
      repeat (400) @(posedge sys_clk);
      strobe(3'd1, 8'h21, 1'b0);                       // Clock: InhibitCAHolds+ClrReady
      strobe(3'd2, v[15:8], 1'b0); strobe(3'd3, v[7:0], 1'b0);
      strobe(3'd0, 8'h4E, 1'b0);
      strobe(3'd0, 8'h00, 1'b1);
      strobe(3'd4, 8'h13, 1'b0); strobe(3'd5, 8'hEF, 1'b0);
      strobe(3'd6, 8'h03, 1'b1); strobe(3'd7, 8'h4F, 1'b0);
      strobe(3'd0, 8'h41, 1'b1);
      repeat (600) @(posedge sys_clk);
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
  task settle; begin repeat (200) @(posedge sys_clk); end endtask

  // Jam a microinstruction and take the first step: ClrStop+ClrMIR+ClrCT+
  // Freeze, then the four MIR bytes, then SetRun with SS and no ClrStop.
  task jam_step(input [7:0] b0, input [7:0] b1, input [7:0] b2,
                input [7:0] b3, input [7:0] b4);
    begin
      setrun = 0; setss_n = 1;
      repeat (400) @(posedge sys_clk);
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
      repeat (800) @(posedge sys_clk);
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
      b0 = {rstk[3], 1'b1,    jcn[0],  1'b1,    4'b0000};
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

  // BUILD A HUNK from field values, four copies of one microinstruction.
  // PARC's hunk packing (doradoboot.masm, SendAHunk):
  //   Byte 0: 0RSTK.0, 0BLOCK, 1RSTK.0, 1BLOCK, ... -- the 17th bit of each
  //           half, which is why a hunk is 17 bytes for 8 half-instructions
  //   Byte 1: 0RSTK.1-3, 0ALUF.0-3, 0BSEL.0
  //   Byte 2: 0BSEL.1-2, 0LC.0-2, 0ASEL.0-2
  //   Byte 3: 0FF.0-7          Byte 4: 0JCN.0-7
  // The field arguments use mi()'s convention, MSB first: rstk[3] is RSTK.0.
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
    repeat (2000) @(posedge sys_clk);
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
    $display("tb_memrun: at the jam -- PRhold=%b Hold=%b MDhold'=%b MiscHold'=%b RefHold'=%b",
             m.PRhold, m.b_MemC.Hold, m.b_MemC.MDhold_p_, m.b_MemC.MiscHold_p_,
             m.b_MemC.RefHold_p_);
    $display("tb_memrun: after TFromCPReg# -- T=%h (want 0043), Q=%h",
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
    $display("tb_memrun: after SetMcr# -- MemC DisHold=%b MemX DisHold=%b",
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
    $display("tb_memrun: at the reference jam -- MAR=%h WantProcRef'=%b (ASEL=%0d IgnoreProc=%b) PipeAd %0d",
             mar, m.b_MemC.WantProcRef_p_, asel, m.b_MemC.IgnoreProc, pipead);
    nop_micro; nop_micro;
    $display("tb_memrun: after the nops    -- MAR=%h PipeAd %0d -> %0d",
             mar, pipe_before, pipead);
    // PARC's own sequence must set DisHold. Everything the memory section does
    // hangs off it: WantMapWait' = (MapFnc.1' & MapFnc.0') | DisHold, and
    // without it MapWait can never fall and the MapState counter never steps.
    if (m.b_MemC.DisHold !== 1'b1 || m.b_MemX.DisHold !== 1'b1)
      $fatal(1, "SetMcr# did not set DisHold -- the memory cycle cannot start");

    p0 = m.b_ContA.clk0_p_Ca; p1 = m.b_ContA.clk1_p_Ca; p2 = m.b_ContA.clk2_p_Bc;
    zero;
    wipe_im;
    nop_micro; nop_micro;

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
    $display("tb_memrun: loaded %0d hunks (%0d microinstructions)", hcount, hcount*4);

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
    build_hunk(4'd0, 4'd0, 3'd4, 3'd0, 3'd1, 8'd0, 8'o201, 1'b0);
    send_a_hunk(16'd0);
    $display("tb_memrun: IM[0..3] overwritten with ASEL=1 references");

    // RELEASE the MIR clock -- register 7, data bit 0 -- so the MIR can reload
    // from IM. Without this the jam is held and nothing is ever fetched.
    manifold(12'h1C0);
    manifold(12'h000);          // ParityEnables := 0 -- IM parity OFF
    $display("tb_memrun: parity enables IMLH=%b IMRH=%b, StopMIRClkEn=%b StopMIRClk=%b",
             m.b_ContB.IMLHPEenable, m.b_ContB.IMRHPEenable,
             m.b_ContB.StopMIRClkEn, m.StopMIRClk);

    // PARC's PrepareProcessor does one more thing before starting the machine,
    // and skipping it is why the memory holds: "Clear out MCR to DisHold,
    // NoWakeups" -- LDXI 103o / TFromCPReg# / SetMcr#. MemC's Hold flip-flops
    // (e22, e23) take DisHold on their set/reset, so without it Hold stays up
    // and PRhold with it. Bytes from doradoboot.masm, which is radix 2:
    // TFromCPReg# = 70 03 0F 04 C0, SetMcr# = 30 02 0B 84 60 (checked by
    // decoding Nop#'s `01^6.+(11^4.)` the same way and getting 0x70).
    $display("tb_memrun: MCR sequence -- T=%h (want 0043), MCR DisHold=%b",
             {m.b_ProcH.T_00, m.b_ProcH.T_01, m.b_ProcH.T_02, m.b_ProcH.T_03,
              m.b_ProcH.T_04, m.b_ProcH.T_05, m.b_ProcH.T_06, m.b_ProcH.T_07,
              m.b_ProcL.T_08, m.b_ProcL.T_09, m.b_ProcL.T_10, m.b_ProcL.T_11,
              m.b_ProcL.T_12, m.b_ProcL.T_13, m.b_ProcL.T_14, m.b_ProcL.T_15},
             m.b_MemC.DisHold);
    $display("tb_memrun: before start -- PRhold=%b Hold=%b DisHold=%b",
             m.PRhold, m.b_MemC.Hold, m.b_MemC.DisHold);
    // The memory must not be holding the processor before it starts. (This is
    // NOT caused by the SetMcr# above -- suppressing the whole block leaves it
    // reading 0 as well. See the header.)
    if (m.PRhold !== 1'b0 || m.b_MemC.Hold !== 1'b0)
      $fatal(1, "the memory is holding the processor before the machine starts");

    // Put the start address in Link, then Return# to jump there and run.
    set_cpreg_tilde(16'h0000);
    parc_micro(8'h30, 8'h13, 8'hEF, 8'h04, 8'h40);   // CPRegToLink#
    nop_micro;
    $display("tb_memrun: Link[4:15]=%h", link_hi);

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
      if (m.b_MemX.preStartMem_p_    !== ppsm) begin npsm=npsm+1; ppsm=m.b_MemX.preStartMem_p_;    end
      if (m.b_MemX.WantMapWait_p_    !== pwmw) begin nwmw=nwmw+1; pwmw=m.b_MemX.WantMapWait_p_;    end
      if (m.b_MemX.MapWait__g13_3    !== pg13) begin ng13=ng13+1; pg13=m.b_MemX.MapWait__g13_3;    end
      if (m.b_MemX.StartMap_p_       !== pxsm) begin nxsm=nxsm+1; pxsm=m.b_MemX.StartMap_p_;       end
    end
    $display("tb_memrun: storage strobes over the run -- MemRASa %0d, MemCASa %0d, MemWEa %0d",
             nras, ncas, nwe);
    $display("tb_memrun: WantProcRef' edges %0d, RefHold' edges %0d (is the RUNNING microcode asking?)",
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
    $display("tb_memrun:   LdPair' edges %0d -- AfreeOrEc'a=%b EcKeepsAbusy=%b",
             nldp, m.b_MemC.AfreeOrEc_p_a, m.b_MemC.EcKeepsAbusy);
    $display("tb_memrun:   cache -- Hit'a=%b Hit'b=%b (edges %0d/%0d) | PairHasA edges %0d, CacheRefInA edges %0d",
             m.b_MemC.Hit_p_a, m.b_MemC.Hit_p_b, nha, nhb, npha, ncra);
    // The A-SLOT reference kinds -- what actually got latched into the pair.
    $display("tb_memrun:   A slot -- CacheRefInA=%b IfuRefInA=%b Store_InA=%b PrefetchInA=%b IoFetchInA=%b PairHasA=%b",
             m.b_MemC.CacheRefInA, m.b_MemC.IfuRefInA, m.b_MemC.Store_u_InA,
             m.b_MemC.PrefetchInA, m.b_MemC.IoFetchInA, m.b_MemC.PairHasA);
    $display("tb_memrun:   AwantsMapFS=%b terms -- EcHasAb=%b Map_InPair'=%b VicInPair'=%b",
             m.b_MemC.AwantsMapFS, m.b_MemC.EcHasAb, m.b_MemC.Map_u_InPair_p_,
             m.b_MemC.VicInPair_p_);
    // THE MEMORY SECTION RUNS DRAM CYCLES. RAS and CAS both strobe, driven by
    // the refresh the PROM state machine sequences -- no force, no stimulus
    // beyond PARC's own startup.
    if (nras < 2) $fatal(1, "MemRASa never strobed -- no DRAM cycle started");
    if (ncas < 2) $fatal(1, "MemCASa never strobed -- the cycle did not get past RAS");
    if (nms  < 2) $fatal(1, "MapState never advanced -- the PROM sequencer is not running");
    $display("tb_memrun:   MemIdlea=%b MemX clk0' edges=%0d  MemRfsh=%b RfshPeriod=%b SetRunRfsh=1",
             m.b_MemX.MemIdlea, nmx, m.MemRfsh, m.RfshPeriod);
    $display("tb_memrun:   RfshPeriod edges=%0d  MemRfsh edges=%0d", nrp, nmr);
    // MemC k15 is the OR-AND that makes StartMap'. It asserts (goes LOW) when
    // any of its four AND terms is 0:
    //   Hia & (AwantsMapFS'|NoRef|AfreeOrEc'a) & (AfreeOrEc'a|WantVic'|NoRef)
    //        & MapRfsh'
    $display("tb_memrun:   StartMap' terms -- Hia=%b NoRef=%b MapRfsh'=%b AwantsMapFS'=%b AfreeOrEc'a=%b WantVic'=%b",
             m.b_MemC.Hia, m.b_MemC.NoRef, m.b_MemC.MapRfsh_p_,
             m.b_MemC.AwantsMapFS_p_, m.b_MemC.AfreeOrEc_p_a, m.b_MemC.WantVic_p_);
    // MapRfsh' = MapFree' | NeedRfsh' (MemX h20, an MC10105 OR), so it asserts
    // only when BOTH are low. That is the one StartMap' term that can break
    // the circle: MapState needs StartMap', StartMap' needs MapRfsh', and
    // MapRfsh' is the only one of its terms not itself sequenced by MapState.
    $display("tb_memrun:   MapRfsh' = MapFree'(%b) | NeedRfsh'(%b) = %b",
             m.b_MemX.MapFree_p_, m.b_MemX.NeedRfsh_p_, m.b_MemX.MapRfsh_p_);
    // NeedRfsh' comes from MemX j09 (MC10117 OR-AND) off WantRfsh',
    // RfshSqWave and PairFull. RfshSqWave is the periodic refresh square wave
    // -- if that is not running, nothing downstream can be.
    $display("tb_memrun:   refresh chain EDGES -- RfshPeriod %0d, RfshSqWave %0d, StartRfshCycle' %0d,",
             nrp, nsq, nsrc);
    $display("tb_memrun:                          WantRfsh' %0d, NeedRfsh' %0d, MapRfsh' %0d, StartMap' %0d",
             nwr, nnr, nmrf, nsm);
    $display("tb_memrun:   MapWait edges %0d (CE' of the MapState counter -- it must go LOW to advance)", nmw);
    // MapWait is a wired-OR of two MemX gates:
    //   g13 (MC10104 AND): StartMap' & MapFree
    //   h13 (MC10103 OR):  preStartMem' | WantMapWait'
    // so it goes low only when ALL THREE of those are low at once.
    $display("tb_memrun:   MapWait terms -- StartMap'=%b MapFree=%b preStartMem'=%b WantMapWait'=%b (edges: pSM %0d, WMW %0d)",
             m.b_MemC.StartMap_p_, m.b_MemX.MapFree, m.b_MemX.preStartMem_p_,
             m.b_MemX.WantMapWait_p_, npsm, nwmw);
    // WantMapWait' = (MapFnc.1' & MapFnc.0') | DisHold  (MemX i20 and l17,
    // both MC10104 ANDs, wired-OR'd). DisHold is the MCR bit PARC's SetMcr#
    // sets -- "Clear out MCR to DisHold, NoWakeups" in PrepareProcessor.
    $display("tb_memrun:   WantMapWait' terms -- MapFnc.0'=%b MapFnc.1'=%b DisHold(MemX)=%b i20=%b l17=%b",
             m.b_MemX.MapFnc_0_p_, m.b_MemX.MapFnc_1_p_, m.b_MemX.DisHold,
             m.b_MemX.WantMapWait_p___i20_14, m.b_MemX.WantMapWait_p___l17_2);
    // Split the wired-OR: MapWait = MapWait__g13_3 | MapWait__h13_9. And check
    // MemX's OWN view of StartMap' -- the counts above are MemC's, the driver
    // side.
    $display("tb_memrun:   g13 stub=%b (edges %0d)  h13 stub=%b  | MemX StartMap'=%b (edges %0d)",
             m.b_MemX.MapWait__g13_3, ng13, m.b_MemX.MapWait__h13_9,
             m.b_MemX.StartMap_p_, nxsm);
    $display("tb_memrun:   NeedRfsh' inputs -- WantRfsh'=%b RfshSqWave=%b PairFull=%b (sqwave edges=%0d)",
             m.b_MemX.WantRfsh_p_, m.b_MemX.RfshSqWave, m.b_MemX.PairFull, nsq);
    // MemX h15 is the MapState counter: StartMap' (PE', active low) loads it
    // to 0 -- its D pins are open -- and MapWait (CE', active low) makes it
    // count. So the state machine only advances while MapWait is LOW.
    $display("tb_memrun:   MapState=%b%b%b  StartMap'=%b MapWait=%b | MapState changes=%0d",
             m.b_MemX.MapState_0, m.b_MemX.MapState_1, m.b_MemX.MapState_2,
             m.b_MemX.StartMap_p_, m.b_MemX.MapWait, nms);
    $display("tb_memrun:   MemAd=%b%b%b%b%b Sout=%h  (RASa=%b CASa=%b WEa=%b)",
             m.MemAd_0, m.MemAd_1, m.MemAd_2, m.MemAd_3, m.MemAd_4,
             {m.Sout_00,m.Sout_01,m.Sout_02,m.Sout_03,m.Sout_04,m.Sout_05,m.Sout_06,m.Sout_07,
              m.Sout_08,m.Sout_09,m.Sout_10,m.Sout_11,m.Sout_12,m.Sout_13,m.Sout_14,m.Sout_15},
             m.MemRASa, m.MemCASa, m.MemWEa);
    $display("tb_memrun: the Pipe pointer moved %0d times over the run, ending at %0d",
             npipe, pipead);
    $display("tb_memrun: holds -- PrHoldReq=%b CHoldReq=%b ExtHoldReq=%b PRhold=%b",
             m.PrHoldReq, m.CHoldReq, m.ExtHoldReq, m.PRhold);
    // WHICH of the three Hold flip-flops is asserting. `Hold` is a wired-OR of
    // three MC10231 halves and each names itself through its Q' output:
    //   e22 pin 3  / pin 2  = RefHold'    the REFERENCE hold
    //   e22 pin 14 / pin 15 = MDhold'     the MD hold
    //   e23 pin 2  / pin 3  = MiscHold'   the MISC hold
    // Q' low means that half is holding.
    $display("tb_memrun: which hold -- RefHold'=%b MDhold'=%b MiscHold'=%b (0 = that one is holding)",
             m.b_MemC.RefHold_p_, m.b_MemC.MDhold_p_, m.b_MemC.MiscHold_p_);
    // And with the memory running, it stops holding the processor.
    if (m.PRhold !== 1'b0)
      $fatal(1, "the memory is still holding the processor after a run");
    $display("tb_memrun: machine running -- %0d clk0' edges, Stop=%b", n0a, m.b_ContA.Stop);
    if (n0a < 100) $fatal(1, "the microinstruction clock is not free-running");

    // ---- The memory section, with the machine executing.
    tbad = 0;
    // (the kind sweep below sets tbad too)
    $display("tb_memrun: MemC clock edges %0d | MemClkEnable'a=%b CLKEnable'b=%b dStop=%b Stop=%b",
             nmemclk, m.b_MemC.MemClkEnable_p_a, m.b_MemC.CLKEnable_p_b,
             m.b_ContA.dStop, m.b_ContA.Stop);
    if (nmemclk < 50) begin
      $display("tb_memrun: FAIL -- MemC's local clock is not running");
      tbad = tbad + 1;
    end
    // In step with the processor: the memory boards take the same backplane
    // clock, gated by MemClkEnable'. Wildly different counts would mean the
    // gating, not the clock, is what is being observed.
    if (nmemclk < n0a || nmemclk > n0a * 2) begin
      $display("tb_memrun: FAIL -- MemC clock %0d edges against the processor's %0d",
               nmemclk, n0a);
      tbad = tbad + 1;
    end

    // MAR is shown, not asserted: this microcode's reference address is
    // whatever the four AEmu hunks compute, and nothing here has established
    // what that should be. `RefOutstanding'` is deliberately NOT reported --
    // the IFU drives it and the IFU is not in this seven-board machine, so it
    // reads as an undriven input rather than as a statement about the memory.
    $display("tb_memrun: MAR=%h", mar);
    // THE PIPE: every storage reference leaves its VA in a 16-entry ring
    // (MemC g02/g03/h02/h03/i02/i03/k03, addressed by PipeAd.0-3). If the
    // running microcode is making references, the ring pointer moves and the
    // entries stop being uniform.
    $display("tb_memrun: PipeAd=%0d | PipeVA[16:19]=%b%b%b%b dVA[04:07]=%b%b%b%b",
             {m.b_MemC.PipeAd_0, m.b_MemC.PipeAd_1, m.b_MemC.PipeAd_2, m.b_MemC.PipeAd_3},
             m.b_MemC.PipeVA_16, m.b_MemC.PipeVA_17, m.b_MemC.PipeVA_18, m.b_MemC.PipeVA_19,
             m.b_MemC.dVA_04, m.b_MemC.dVA_05, m.b_MemC.dVA_06, m.b_MemC.dVA_07);
    for (kk = 0; kk < 16; kk = kk + 1)
      if (m.b_MemC.u_k03.mem[kk] !== 4'b0000)
        $display("tb_memrun:   Pipe[%0d] VA.16-19 = %b", kk, m.b_MemC.u_k03.mem[kk]);
    $display("tb_memrun: ASEL=%0d WantProcRef'=%b | Dbusy=%b WantCR=%b CacheRefInA'=%b IgnoreProc=%b",
             asel, m.b_MemC.WantProcRef_p_, m.b_MemC.Dbusy, m.b_MemC.WantCR,
             m.b_MemC.CacheRefInA_p_, m.b_MemC.IgnoreProc);

    // Every one of those must be a defined level. Before the boards were in a
    // running machine they were whatever the power-up state left behind.
    if (^{m.b_MemC.WantProcRef_p_, m.b_MemC.Dbusy, m.b_MemC.WantCR,
          m.b_MemC.CacheRefInA_p_, m.b_MemC.IgnoreProc} === 1'bx) begin
      $display("tb_memrun: FAIL -- MemC's reference logic is not settled");
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
      repeat (60) @(posedge sys_clk);
      // cpu.c: ASEL=0 ff01=2 is LONGFETCH, ASEL=1 ff01=2 is IFETCH. Require
      // each line to assert THERE AND NOWHERE ELSE across all sixteen.
      if (m.b_MemC.Lfetch_u_ !== ((kk[3:2] == 2'd0) && (kk[1:0] == 2'd2))) begin
        $display("tb_memrun: FAIL -- LFetch<- at ASEL=%0d ff01=%0d reads %b",
                 kk[3:2], kk[1:0], m.b_MemC.Lfetch_u_);
        tbad = tbad + 1;
      end
      if (m.b_MemC.Ifetch_u_ !== ((kk[3:2] == 2'd1) && (kk[1:0] == 2'd2))) begin
        $display("tb_memrun: FAIL -- IFetch<- at ASEL=%0d ff01=%0d reads %b",
                 kk[3:2], kk[1:0], m.b_MemC.Ifetch_u_);
        tbad = tbad + 1;
      end
    end
    $display("tb_memrun: LFetch<- at (ASEL 0, ff01 2) and IFetch<- at (ASEL 1, ff01 2), and nowhere else");
    release m.ASEL_0; release m.ASEL_1_p_; release m.ASEL_2_p_;
    release m.FF_0mem_p_; release m.FF_1mem;
    repeat (60) @(posedge sys_clk);

    // And the front door must still read the rule refdecode-test gates, now
    // against whatever ASEL the running microcode is presenting.
    if (m.b_MemC.WantProcRef_p_ !== (asel > 3'd3)) begin
      $display("tb_memrun: FAIL -- WantProcRef'=%b for ASEL=%0d",
               m.b_MemC.WantProcRef_p_, asel);
      tbad = tbad + 1;
    end

    if (tbad != 0) $fatal(1, "the memory section is not right in a running machine");
    $display("tb_memrun: PASS -- the memory runs DRAM cycles from PARC's own startup, and holds nothing");
    $finish;
  end

endmodule

`default_nettype wire
