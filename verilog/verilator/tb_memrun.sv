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
// WHERE THIS STOPS, stated precisely because the next session should start
// here. The microcode ASKS for storage and nothing COMPLETES:
//
//   * `WantProcRef'` asserts, so the request reaches MemC (above).
//   * The PIPE does not move. Every storage reference leaves its VA in a
//     16-entry ring (MemC g02/g03/h02/h03/i02/i03/k03, addressed by
//     `PipeAd.0-3` and fed from `dVA.04-21`), and over 3000 sys_clk of
//     execution the pointer changes ZERO times. It reads 12, which is where
//     startup left it -- a first pass read that 12 as evidence that references
//     were being recorded, which it is not. Count the CHANGES, not the value.
//   * WHICH hold, and why, is now known. `Hold` is a wired-OR of three MC10231
//     halves, each naming itself through its Q' output: e22 pin 2 `RefHold'`,
//     e22 pin 15 `MDhold'`, e23 pin 3 `MiscHold'`. Measured over a run:
//     RefHold'=1, MDhold'=0, MiscHold'=0 -- so it is the MD hold and the MISC
//     hold that are up, and NOT the reference hold. That is a coherent state
//     rather than a broken one: the processor is being held waiting for
//     memory DATA, which is exactly what should happen when a reference is
//     accepted and never completes. `dMDhold'` comes from b23, an MC10121
//     whose inputs include `_MD`/`_MDI` (the microinstruction asking for Md)
//     and `DdataGood'` -- and `DdataGood'` is properly connected, driven by
//     MemX e19 across the backplane, so this is not another unwired net. It
//     says the data is not good, because nothing has ever completed a storage
//     access.
//
//     So the remaining work is the storage side. AND IT IS FURTHER UPSTREAM
//     THAN "write a storage model": THE MACHINE NEVER STARTS A DRAM CYCLE.
//     Over a whole run MemRASa, MemCASa and MemWEa toggle ZERO times, with
//     MemX's own clock running fine (188 edges against the processor's 181).
//     So attaching a behavioural MSA array would not be reached -- nothing is
//     asking it for anything.
//
//     The storage interface itself is fully specified and small, which is the
//     good news. The MSA boards are not among the eleven and the archive has
//     no wire list for them, but `msa.bp` states their whole backplane
//     interface in 66 nets: MemAd.1-8 (multiplexed address, pins matching
//     MemX's exactly), MemRAS/CAS/WE in a and b copies from DIFFERENT
//     packages (two banks, not fan-out), Sin.00-15 and Sout.00-15, and the
//     EcIn/EcOut correction lines. Direction is settled by the netlist rather
//     than by the names, which mislead: MemD DRIVES `Sout` (write data to
//     storage) and RECEIVES `Sin` (read data back).
//
//     REFRESH WAS THE FIRST SUSPECT, and chasing it found the real shape of
//     the thing. `RfshPeriod` is generated by the BASEBOARD -- a01, an MC10176
//     hex flip-flop clocked by clk0'A, which also makes `SetRunRfsh` and
//     `Pendulum` -- and this seven-board machine has no BaseBoard, so
//     dorado_mem lists it as an input "awaits BaseBd" and it read 0 for ever.
//     It is driven here now, and toggles.
//
//     THAT WAS NOT ENOUGH, and the reason is the useful finding: `MemRfsh`
//     still does not move. It comes from MemX f18, an F10016 whose CE' is tied
//     to `TrueAC` -- a constant 1 -- so that part never COUNTS; it is a
//     parallel-load register, and `MemRfsh` simply follows `dMemRfsh`. And
//     `dMemRfsh` comes from g15, an **SG10139 PROM**, addressed by
//     `MapState.0-2` and `MapFnc.0-1'`.
//
//     SO THE WHOLE MEMORY CYCLE IS A PROM-SEQUENCED STATE MACHINE. MapState
//     lives in h15 (another F10016), g15 decodes it into the refresh request
//     and the MemX14 control signals, f18 registers them. The PROM CONTENTS
//     are already right -- generated from PARC's own BCPL and gated by
//     `prom-test`, with the image loaded as `verilog/proms/packages/
//     MemX-g15.mem`. What is missing is whatever advances `MapState`.
//
//     AND MAPSTATE NEVER MOVES BECAUSE `StartMap'` NEVER ASSERTS. h15 is an
//     F10016 with `StartMap'` on PE' (parallel enable, active low, D pins
//     open so it loads zero) and `MapWait` on CE' (count enable, active low).
//     Measured: MapState = 000, StartMap' = 1, MapWait = 1, zero changes over
//     a whole run. It is neither loaded nor counting.
//
//     `StartMap'` is properly connected -- MemC k15 drives it, both boards on
//     pin E66, and the generated machine wires it. k15 is an MC10121 OR-AND:
//
//       StartMap' = Hia & (AwantsMapFS'|NoRef|AfreeOrEc'a)
//                       & (AfreeOrEc'a|WantVic'|NoRef) & MapRfsh'
//
//     and it asserts when any term is 0. Measured: Hia=1, NoRef=0 (so there IS
//     a reference, consistent with WantProcRef'), MapRfsh'=1, AwantsMapFS'=1,
//     AfreeOrEc'a=0, WantVic'=1 -- all four terms evaluate to 1.
//
//     `Hia` IS EXONERATED. It was the suspect -- its only driver is MemC l02,
//     an MC10231, and the wire list puts pins 13, 14 AND 15 of that one
//     package on the net, so Q and Q' are tied together, which on open-emitter
//     ECL is a constant high. MEMC's SCHEMATIC (MEMC.pdf sheet 22, the l2b
//     block) shows exactly that: pin 15 (Q) labelled `Hia!`, pin 14 (Q') and
//     pin 13 (R) all on one node. So Hia = 1 IS WHAT THE BOARD DOES and the
//     RTL reproduces it correctly. The netlist and the schematic agree; the
//     suspicion was wrong.
//
//     CORRECTION (and the reason this file counts edges everywhere now): THE
//     REFRESH CHAIN RUNS. An earlier pass SAMPLED these signals at one instant,
//     found them all deasserted, and concluded `StartMap'` never asserts.
//     Counting instead shows 6 edges through EVERY stage --
//
//       RfshPeriod 6 -> RfshSqWave 6 -> StartRfshCycle' 6 -> WantRfsh' 6
//                    -> NeedRfsh' 6 -> MapRfsh' 6 -> StartMap' 6
//
//     -- so the request/acknowledge handshake works end to end. (WantRfsh is a
//     JK flip-flop, MemX j08: J' = StartRfshCycle' sets it, K' = MapRfsh'
//     clears it. StartRfshCycle' is just RfshPeriod delayed two clocks through
//     j03, an MC10176 shift chain: D3->Q3 makes RfshSqWave, D4->Q4 makes
//     StartRfshCycle'.) This is the same trap as reading the Pipe pointer's
//     value instead of its changes. COUNT, DO NOT SAMPLE.
//
//     SO THE BLOCKER IS NARROWER AND ELSEWHERE: `MapWait` NEVER MOVES.
//     0 edges over a whole run, stuck at 1. It is CE' on MemX h15, the
//     MapState counter, and CE' must go LOW to advance. StartMap' asserting
//     does nothing visible because it is PE' and h15's D pins are OPEN -- it
//     loads 0000 into a register already holding 000, which is invisible. The
//     machine is being told to reset the state to zero six times and never
//     told to step.
//
//     MapWait is driven from MemC by g13 (MC10104) and h13 (MC10103), a
//     wired-OR, so it goes low only when EVERY contributor is low. START
//     THERE.
//
//     SUPERSEDED (kept because the reasoning was sound and only the sampling
//     was wrong):
//
//       WantRfsh' never asserts (reads 1)
//         -> NeedRfsh' stays 1     MemX j09, an MC10117 2-wide OR-AND whose
//                                  common pin 9 is OPEN, so
//                                  NeedRfsh' = (WantRfsh'|RfshSqWave)
//                                            & (PairFull|WantRfsh')
//                                  and BOTH terms need WantRfsh' = 0
//         -> MapRfsh'  stays 1     MemX h20: MapRfsh' = MapFree' | NeedRfsh',
//                                  and MapFree' already reads 0
//         -> StartMap' stays 1     MemC k15, the OR-AND above
//         -> MapState frozen 000   MemX h15 never loads and never counts
//         -> the PROM sequencer never runs
//         -> no RAS/CAS/WE, ever
//         -> DdataGood' says the data is not good
//         -> MDhold holds the processor
//         -> no reference completes
//
//     RfshSqWave DOES run now (6 edges, following the RfshPeriod this bench
//     drives), so the periodic side is alive. What is missing is WantRfsh'.
//     START THERE.
//
//     SUPERSEDED SUSPICION, kept because the reasoning was reasonable: Its
//     only driver is MemC l02, an MC10231 -- and the wire list puts pins 13,
//     14 AND 15 of that one package on the net, with 13 an input and 14/15 the
//     two outputs of half B. Our cell has `p15 = qb, p14 = ~qb`, so Q and Q'
//     are wired together, which on open-emitter ECL is a CONSTANT HIGH -- the
//     same trick the BaseBoard uses to make `TrueA` from a spare NOR with open
//     inputs. The geometry says the same thing: `{456,52} {460,52} {464,52}`
//     is three ADJACENT pins at one y, four apart in x, which is the
//     wire-wrap strap pattern this project already documented for jumpers.
//
//     So either `Hia` is a deliberate constant 1 -- in which case the name
//     misleads and it is not a cache "hit" at all -- or our MC10231 has half
//     B's outputs wrong. MemC's schematic sheet settles it, exactly as the
//     configuration sheets settled the SIP legs and the broken MemX pin. READ
//     THAT BEFORE CHANGING ANYTHING.
//
//   * `PRhold` is clear at startup now, and comes back UP during the run,
//     with `PrHoldReq`, `CHoldReq` and `ExtHoldReq` all still reading 0. That
//     is a better-shaped question than the one this file used to record: the
//     hold is not a power-up artefact, it is asserted by something the machine
//     does while executing. Since the microcode is asking for storage that
//     never completes, a memory holding the processor for a reference it
//     cannot finish is exactly what one would expect -- so the next move is to
//     find which of the three Hold flip-flops (MemC e22 pin 3, e22 pin 14,
//     e23 pin 2) sets, and on what.
//
// None of those three is ASSERTED here. The Pipe and the holds are printed as
// diagnostics because the CORRECT values are not yet known, and asserting on a
// value one does not understand gates nothing.
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
  integer nrp, nmr, nms, nsq, nsrc, nwr, nnr, nmrf, nsm, nmw;
  reg psq, psrc, pwr, pnr, pmrf, psm, pmw;
  reg [2:0] pms;
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

  initial begin
    force m.DMuxData = dmd;
    force m.DMuxClk  = dmc;
    force m.UseDMD   = udmd;
    repeat (2000) @(posedge sys_clk);
    manifold(12'h030);
    manifold(12'h1E0);                    // SetMidasStopMIRClk ON, to load
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
    set_cpreg_plain(16'h0043);                        // 103 octal
    parc_micro(8'h70, 8'h03, 8'h0F, 8'h04, 8'hC0);    // TFromCPReg#
    nop_micro;
    parc_micro(8'h30, 8'h02, 8'h0B, 8'h84, 8'h60);    // SetMcr#
    nop_micro;
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

    n0a = 0; nmemclk = 0; npipe = 0; ppa = pipead;
    nras = 0; ncas = 0; nwe = 0;
    prasa = m.MemRASa; pcasa = m.MemCASa; pwea = m.MemWEa;
    nmx = 0; pmx = m.b_MemX.Clk0_p_Aa;
    nrp = 0; prp = m.RfshPeriod; nmr = 0; pmr = m.MemRfsh;
    nms = 0; pms = mapst; nsq = 0; psq = m.b_MemX.RfshSqWave;
    nsrc=0; psrc=m.b_MemX.StartRfshCycle_p_; nwr=0; pwr=m.b_MemX.WantRfsh_p_;
    nnr=0; pnr=m.b_MemX.NeedRfsh_p_; nmrf=0; pmrf=m.b_MemX.MapRfsh_p_;
    nsm=0; psm=m.b_MemC.StartMap_p_; nmw=0; pmw=m.b_MemX.MapWait;
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
    end
    $display("tb_memrun: storage strobes over the run -- MemRASa %0d, MemCASa %0d, MemWEa %0d",
             nras, ncas, nwe);
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
    $display("tb_memrun: PASS -- the memory boards run and their reference logic is defined");
    $finish;
  end

endmodule

`default_nettype wire
