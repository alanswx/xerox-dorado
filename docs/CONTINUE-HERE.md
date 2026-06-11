# Continuation handoff — Alto-on-Dorado boot bring-up (2026-06-10)

Read this first if you are a fresh Claude session resuming the Dorado
Alto-emulator boot work. It is self-contained: it tells you exactly where the
boot is stuck, the precise root cause, how to reproduce it in one command, and
the next concrete step. Project-wide context is in `/CLAUDE.md` and
`dorado/CLAUDE.md`; the running narrative is in
`docs/ethernet-local-boot-plan.md` (read its UPDATE-2026-06-10d block — the
others are partly superseded/retracted, see below).

## One-line status

The Alto emulator (`chm/dorado/AEmu.mb!2`) boots over Ethernet through
InitMap -> StartEmulator -> SetupBRs -> InitTasks, then **derails just before
ABoot** because every emulator base register is corrupted to bank `0xD24`
(`mar=0xD24FE1F`). Root cause is pinned to one instruction (below). Fix it and
the boot should reach ABoot -> EBoot -> Mayday -> EFTP -> NetExec.

## The boot vehicle (important — do not use AltoMesaDorado for debugging)

Use **`chm/dorado/AEmu.mb!2`**. It is the COMPLETE, self-contained
Alto-emulator world WITH symbols and the ADefs register layout. `mbdis` shows
it defines everything: `GetEmulatorMapParams`@0o3240, `ABoot`@0o724,
`EBoot`@0o2006, `DiskBoot`@0o2005, `InitMap`@0o1076, `SetupBRs`@0o1140,
`InitTasks`@0o1123, `StartEmulator`@0o1133, `ResumeEmulator`@0o1135,
the junk/display/disk tasks, etc.

`AltoMesaDorado.eb` is the Mesa world (DMesaDefs layout, NO symbols) — it was
making debugging impossible. Only use it for the legacy probe default.

Build the boot `.eb` from AEmu.mb!2:
```
cd dorado && ./build/mb2eb ../chm/dorado/AEmu.mb!2 /tmp/aemu_only.eb 01076
```
(start addr 0o1076 = InitMap. mb2eb reports `RM=0` — the .eb loads NO RM;
this matters, see root cause.)

## One-command repro

```
cd dorado && make build/test_cpu && \
DORADO_ONLY_FULLBOOT=1 DORADO_NO_DISK=1 DORADO_STORAGE_MODULES=1 \
DORADO_ETH_BOOT_110=/tmp/aemu_only.eb \
DORADO_ALTO_BOOT_ETHERNET=1 DORADO_ETH_EFTP_BOOT=../chm/bootfiles/NETEXEC.BOOT!8 \
DORADO_BOOT_BUDGET=120000000 ./build/test_cpu 2>&1 | grep -iE 'BR36|MDS/BR36|Final PC'
```
Expected today: `MDS/BR36=0xD240000` (corrupted) and the boot does NOT reach
ABoot. When fixed you should see BR36 in bank 0 and the boot reach
ABoot/EBoot.

Notes on the env vars:
- `DORADO_ONLY_FULLBOOT=1` is REQUIRED. The multi-scenario probe corrupts
  shared global state (display/io/ethernet singletons + statics); only the
  isolated full-boot run is trustworthy. (This was itself a multi-turn red
  herring — "RTC=0" was a harness artifact, not a bug.)
- `DORADO_NO_DISK=1` keeps the (incomplete) disk task idle.
- `DORADO_STORAGE_MODULES=1` = 4 MW (RealPages=0x4000). The default 16 MW makes
  InitMap zero ~16 MW (very slow) and wraps RealPages to 0.
- Budget is in BB cycles; cpu->cycles is a different (smaller) clock. Use 80M+
  BB-budget to get past SetupBRs (cpu-cyc ~13M).

## ROOT CAUSE (verified, instruction-level)

The derail is because `SetupBRs`' DoBRs loop (@0o1176) reads `EmuBRHiReg` and
`BrHi`s it into every emulator BR. `EmuBRHiReg` = `0xD24`, a STALE value.

`EmuBRHiReg` lives in region **AEmRegs** (`RegisterDefs.mc`:
`Set[rbAemRegs, 1]`), i.e. RBase=1, absolute RM slot **RM[0o30]**.

Two accesses, both traced to the actual RM index/value:
- **READ** by SetupBRs @0o1176 at RBase=1 -> RM[0o30] = 0x0D24 (stale). This
  addressing is CORRECT: `ResumeEmulator` does `RBase_ RBase[AEmRegs]` (=1)
  before `Call[SetupBRs]`.
- **WRITE** by InitMem `EndOfStorage`'s `EmuBRHiReg_ T` ran at **RBase=4** and
  wrote **RM[0o101]** — the WRONG slot. `InitMap` set `RBase_ RBase[ITemp0]`
  (ITemp0 is in the `EORegs` region, runtime RBase=4); `EndOfStorage`'s
  `EmuBRHiReg_ T` has no RBase clause, so it executes at RBase=4. The real
  `EmuBRHiReg` (RM[0o30]) is never updated.
- RM[0o30] keeps a stale 0x0D24 (put there earlier, RM_WRITE at pc=0o6440,
  RBase=1). SetupBRs reads it -> all BRs become bank 0xD24 -> `mar=0xD24FE1F`
  -> derail before ABoot.

The 256-entry RM is addressed as `RBase[0:3] || RSTK[0:3]`, so RBase=4 and
RBase=1 are genuinely different cells: a write at RBase=4 CANNOT reach the
RBase=1 cell. Therefore the microcode MUST have RBase=1 (AEmRegs) at the
`EmuBRHiReg_ T` instruction, and our emulator has 4.

### RETRACTED hypothesis (do not chase)
An earlier commit blamed a carry-flag bug at `GetEmulatorMapParams`@0o3240
(claiming a `Carry'`-conditional branch). That was DISPROVEN: forcing
`alu_carry=1` at the 2nd 0o3240 entry did NOT change the branch (still ->
0o4004). 0o3240's branch is a Return/dispatch to the correct caller
continuation; its target difference (0o3256 vs 0o4004) is just the per-call
Link, not carry. Commits: c6cc31d (wrong), bf1267c (retraction),
ce88c87 (confirmed RBase root cause).

## THE OPEN QUESTION = the fix

Why is our RBase 4 (not 1) at `EmuBRHiReg_ T`? Candidates, cheapest first:

1. **Verify our `RBase_ RBase[ITemp0]` decode** (InitMap, ~0o1031). Does it set
   the RBase the assembler intended? If our RBase-constant decode is wrong,
   that's a direct emulator bug. (Cheapest check.)
2. **GetEmulatorMapParams side effect.** `EmuBRHiReg_ T` runs right after
   `Call[GetEmulatorMapParams]` (InitMem.mc line 74). That subroutine is
   emulator-specific and its SOURCE IS NOT in our local trees (only referenced
   in InitMem.mc; `mbdis` shows the symbol @0o3240 but our execution treats it
   as a 1-instruction T-set+return). On real hardware it may set
   RBase=AEmRegs(1) before returning. If so, our model misses it. Confirm by
   checking whether 0o3240 (FF=0o200; BSEL=FF,,0 is a CONSTANT, so FF is NOT a
   function there) should instead run a body.
3. **Micro-assembler region->RBase resolution.** The Micro source uses
   `SetRMRegion`/`RVN`/`RBase[reg]`; our model may not replicate how a register
   in AEmRegs is reached while the active region is EORegs. Look at how
   `rm_address` in `dorado/src/cpu.c` combines RBase/RSTK/StkP — the read used
   (rb=1,rstk=0o10)->0o30 and the write used (rb=4,rstk=0o11)->0o101, which
   shows the StkP/RSTK[0] handling is non-obvious; make sure it matches HM
   §3.1 / Table 6.

Success test: after the fix, RM[0o30] should be written 0 at EndOfStorage,
SetupBRs should BrHi bank 0 into the BRs, `BR36/MDS` should be bank 0, and the
boot should pass ABoot. Then the BS-down keyboard seed (already in
`test_cpu.c`, see `force_alto_ether_boot`) should drive ABoot -> EBoot ->
Mayday(0244) -> EFTP(030) -> NetExec (the EFTP server in `src/ethernet.c` is
byte-exact and unit-tested).

## Files / pointers

- Boot vehicle: `chm/dorado/AEmu.mb!2`  (build `/tmp/aemu_only.eb` via mb2eb)
- Microcode source (have): `chm/doradosource/AemuSources.dm!82_/InitMem.mc`
  (InitMap/EndOfStorage, lines ~49-90), `.../AEm0.mc` (SetupBRs @ line 131-141,
  ResumeEmulator @ line 22-24: `RBase_ RBase[AEmRegs]`),
  `.../RegisterDefs.mc` (`Set[rbAemRegs, 1]`, region defs).
- Microcode source (DO NOT HAVE): `GetEmulatorMapParams` body. Only referenced
  in InitMem.mc. Would need a CHM fetch or a MicroD rebuild to obtain.
- Emulator: `dorado/src/cpu.c` (microengine; `rm_address`, `a_bus`, FF
  dispatcher, JCN/next_pc, ALU), `dorado/src/memory.c` (BR/Map/Pipe;
  `dorado_br_hi_load`/`dorado_br_lo_load`), `dorado/tests/test_cpu.c`
  (probe_full_boot_with_bootstrap; the boot loop, keyboard seed at
  `force_alto_ether_boot`, ~line 4535).
- Disassembler: `dorado/build/mbdis` (`mbdis <file>`, `mbdis --disasm <file>`,
  `mbdis -d <file>` raw; real addr is column 5).

## Debug-trace recipe (how the root cause was found; rebuild as needed)

All temporary traces were removed (suite must stay green). To re-derive:
add an env-gated `fprintf(stderr,...)` at the top of `dorado_cpu_step` in
`cpu.c` keyed on `cpu->real_PC` and `(cpu->ctask & 017)==0`. Useful probes:
- Landmarks: first-hit of 0o1133/0o1140/0o1123/0o724/0o2006/0o2005.
- `EmuBRHiReg` read: at `a_bus` when `real_PC==01176`, log `u->asel`,
  `rm_address(cpu,u)`, `cpu->RBase`, `u->rstk`, `rm_stk_read(...)`.
- RM writes: the BUILT-IN `DORADO_RM_TRACE=1` prints every RM write with
  `pc/addr/old/new/rb/rstk`. Grep `addr=030` (EmuBRHiReg) and the EndOfStorage
  PCs (0o412x).
- BrHi loads: log in the `case 4: /* BrHi */` of the FF dispatcher (cpu.c
  ~950) when `(cpu->ctask&017)==0`: `cpu->MemBase`, `a`, `cpu->real_PC`.
Always `git checkout dorado/src/cpu.c` and re-run `make test` (expect
"All CPU tests passed." / "All BaseBoard tests passed.") before committing.

## Working norms (from CLAUDE.md)

No emojis in code/docs. Use original Dorado terminology (RM, BR, RBase, RSTK,
EmuBRHiReg, etc.). Cite HM section numbers in tricky code/commits. Commit only
when asked; branch off `main` if needed; end commit messages with
`Co-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>`
(update the model name to whatever model you are). Push only when asked.

## Recent commits (newest last)

- `b0d0cad` DWT display-spin refill fix (idles instead of starving junk task)
- (scenario isolation gates: DORADO_ONLY_FULLBOOT / DORADO_NO_DISK)
- `c6cc31d` carry diagnosis  [WRONG]
- `bf1267c` retract carry hypothesis
- `ce88c87` CONFIRMED root cause: EmuBRHiReg write hits wrong RM slot (RBase 4 vs 1)
