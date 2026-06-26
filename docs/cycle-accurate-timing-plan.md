# Cycle-Accurate Timing — the major fix for "most games crash"

> **For the scoped, estimated, go/no-go version of this work, read
> [`docs/timing-project-scope.md`](timing-project-scope.md) first** (2026-06-23).
> It defines the end state, proves the work is finite (4 cadences, not infinite
> whack-a-mole), phases it (~2–4 wks with a 1-week go/no-go), and folds in the
> render-path + ethernet findings. This file remains the detailed working log.

Continuation / planning document. **Date: 2026-06-23.**
Read this together with [`docs/fidelity-audit.md`](fidelity-audit.md) (the full
audit + the two ruled-out experiments) and
[`docs/restart-alto-games-no-render.md`](restart-alto-games-no-render.md) (the
deep MissileCommand investigation that reached the same conclusion
independently).

Work-in-progress lives on branch **`fidelity-timing`**. Older notes below refer
to commits `57599e3` (Hold model + audit) and `3cea1de` (cadence finding), where
the then-current non-diagnostic gates were green. Current diagnostic ground
truth is tracked in [`running-diagnostics.md`](running-diagnostics.md).

---

## 0. TL;DR

The Dorado emulator boots and runs *simple* Alto programs correctly (Galaxian,
Boggs, EtherLoad, Calculator, KeyTest render faithfully), but **most games
crash** and the **disk boot never draws an Exec**. The microengine, memory
subsystem, IFU, renderer, and boot loader are sound — the failure is the
**timing model**.

Root cause, proven five independent ways (survey, heuristics audit, test audit,
ContrAlto `tracediff`, and the prior MC investigation): **cumulative
timing/state divergence.** The emulator's device + scheduler cadences are
*approximate constants tuned to pass the boot*, not physically accurate, so
complex programs accumulate enough interrupt-vs-mainline drift to derail while
trivial ones survive.

Two incremental fixes were implemented and **ruled out by experiment**:

| Fix | State | Result |
|---|---|---|
| **Memory Hold** (gated `DORADO_HOLD`) | implemented, validated, committed | **Moot** — AEmu microcode covers the memory latency, so Hold fires ~4M times in *boot* and **zero** times in the running game. |
| **Wakeup cadence** (2-cyc wakeup latency + 2-instr TaskingOn, `DORADO_WLAT`) | implemented, reverted | **Desyncs the boot** (Galaxian → 0px) — the cadences are co-tuned; any subset made accurate breaks the rest. |

**Conclusion: the fix is cycle-accurate device + scheduler timing, done
holistically** — a genuine architectural project. This document plans it so it
can be executed deliberately in a fresh context.

---

## 1. The emulator's state (ether-boot survey, 250M cycles)

Pixel count is a rough proxy (high px can be a real render *or* frozen garbage;
low px can be a crash *or* a sparse render). The reliable signal is the dead
bucket.

- **Dead on arrival (0 px), 17/38:** AlphaMesaMesaNetExec, CedarNetExec, Chat,
  Clock, FTP, GateControl, Johnsson, Kal, MazeWar, MesaNetExec, Murray, Pool,
  PPong, Pupwatch, ShowAIS, StarWars, Trek.
- **Near-blank / crashed early:** AstroRoids 163, Invaders 163, What 301,
  Messenger 383, Scavenger 603, Reversi 831.
- **Render real content:** Galaxian 121553 (verified), Boggs 274967 (a halftoned
  portrait), EtherLoad 428581, EDP 39023, Calculator 14333, KeyTest, MadTest,
  CopyDisk, PupTest, BFSTest, PinBall 89372, Neptune.
- **Frozen garbage despite high px:** MissileCommand 491264 (frozen-black, NOT a
  render — see the MC doc).

So well over half don't truly work. **Disk boot:** loads the OS but never reaches
a drawable Exec (separate `disk.c` track — DSK-task wakeup stubbed + PDI shim
forces `HeadOvfl`; see task #8). **Cedar:** boots to its login prompt
(`make run-cedar`).

To re-run the survey: `for f in ../chm/bootfiles/*.boot*; do
./build/dorado --eb worlds/aemu.eb --eftp "$f" --cycles 250000000 --out
/tmp/x.pgm; <count non-255 px>; done`.

---

## 2. Root cause (the proven mechanism)

The AEmu (Alto-emulator) microcode runs on the Dorado, emulating the Alto. A
game's behavior depends on the **ratio of Alto-opcode-execution-rate to the
I/O-interrupt cadence** (display vertical field ~60Hz, RTC, ethernet
completion, disk). When that ratio is wrong, the game's main loop runs out of
phase with its interrupt handlers, and the handlers make different decisions
than on real hardware. This **accumulates**: light games tolerate the drift;
heavy ones diverge into a crash/freeze.

**Empirical proof (ContrAlto `tracediff`).** Watching the Alto interrupt-handler
dispatch cell `M[0o3016]` while both emulators net-boot the *identical* `.boot`:

```
MissileCommand: ours 0,0,2616,2616,1537,1537,... (34651 writes, oscillating) | CA settles at 2616
Invaders:       ours 0,0,2616,2616,2616,...      (66 writes, settles)        | CA settles at 2616
Both: FIRST DIVERGENCE at write #1 -> ours=0, CA=2616, at Alto br31=0o644 pc=2
```

Both games share the **same early divergence**: ours writes `M[3016]=0` one
extra time — it takes one extra interrupt-handler pass before the mainline
commits the dispatch — i.e. **a wakeup/interrupt fires one beat early**. (Note:
this *early* divergence is the no-render doc's "harmless phase slip"; the actual
crash is the *cumulative* effect downstream — Invaders settles here and dies
~66 interrupts later. So the fix is not one line.)

The heuristics audit (`fidelity-audit.md`) inventoried the approximate cadences
behind this: the immediate wakeup latency (HM says 2 cycles), the 1-instruction
TaskingOn gate (HM says 2), the display scanline wakeup `+1000` BB-cycles
(~3× too slow vs a real ~20.6µs scanline), the breath-of-life / time-broadcast
injection cadence, the ethernet EFTP turnaround, and the disk shim.

---

## 3. Why incremental fixes fail (read before trying one)

`cpu.c` (the FF `SetTaskingOn` handler, ~line 1490) says it plainly: the
immediate wakeup latency and the 1-instruction TaskingOn gate are a **consistent
tuned pair** — "the boot is tuned to them, so changing one alone desyncs them
(raising the delay to 3 spins the I/O tasks — disk wakeups 170K → 27M — and the
boot never reaches NetExec)."

Confirmed: making the wakeup latency + TaskingOn gate cycle-exact **together**
still desyncs the boot (`DORADO_WLAT` → Galaxian 0px). So the co-tuning is not
just those two — the boot is tuned to a **web** of approximate cadences, and
making *any subset* accurate desyncs the rest. **Incremental cadence patches do
not converge.** The boot only "works" today because the approximations
collectively compensate for each other (and for other inaccuracies, like
approximate device I/O timing).

The memory Hold is correct but doesn't help, because the AEmu microcode already
overlaps the memory latency (Hold fires in boot, never in-game).

---

## 4. The major fix — strategy

**Goal:** make the device + scheduler timing physically accurate so the
Alto-opcode-rate : I/O-cadence ratio matches real hardware (= ContrAlto, which
is a validated cycle-accurate Alto emulator). Real Alto boots and runs with
accurate timing; ours will too once *everything* is accurate. The difficulty is
purely that it must be done **holistically** — you cannot make one cadence
accurate while the others are tuned approximations.

**The key enabler: decouple the running game from the fragile boot.** The
boot-desync is what blocks incremental work. Solution — add **machine
snapshot/restore**: boot a game *once* with the current tuned cadences, snapshot
the running game's full state, then for every timing experiment *restore the
snapshot and run with the new timing*. The boot is not re-run, so a timing
change can be measured against ContrAlto on the **running game** in isolation.
This makes Phases 1–2 incremental and fast; Phase 3 then re-establishes the boot
with the now-known-correct cadences.

### Phase 0 status (2026-06-23)

- **[DONE] Machine snapshot/restore (step 3, the key enabler).**
  `dorado_machine_snapshot(m, path)` / `dorado_machine_restore(m, path)` in
  `machine.c` serialize the full mutable runtime state — control store (incl.
  the LoadRam'd world), cpu, mem+storage, display, baseboard, ethernet+heap,
  and the machine's scalar boot/timing tail — and restore it into a machine
  freshly created with the same config (pointers re-wired, ethernet heap
  reconstructed). Disk/PDI media state is the one omission (fine for the
  non-disk Alto games this targets). `dorado_machine_state_digest(m)` gives a
  64-bit FNV-1a over storage+framebuffer+registers for equivalence checks.
  Validated by `tests/test_snapshot.c` (in `make test`): boots Galaxian,
  snapshots at 40M, restores into a fresh machine, and proves both *restore
  fidelity* (identical digest at the snapshot point) and *forward
  determinism* (running both to 45M lands on the identical cycle + digest).
  Window overridable via `DORADO_SNAP_N1`/`DORADO_SNAP_N2`.
  - Bringing this up uncovered + fixed **two real latent bugs** that any
    multi-machine / restore workflow would hit: (1) the vendored fake6502
    core kept the BaseBoard's entire 6502 register file (pc/sp/a/x/y/status/
    clockticks) in **file-scope globals**, so it was never part of bb and a
    restore ran the 6502 from the wrong state — now mirrored into
    `bb->cpu6502` with an owner-cache that swaps the globals in/out per
    `baseboard_step` (`baseboard_cpu_flush`/`_reload`); (2) `baseboard_active`
    (the 6502's memory-callback target) was only set at create/reset, so
    running one machine drove another's BaseBoard — now re-pointed at the top
    of `dorado_machine_run_until`. Galaxian still 121553, all 12 suites green.
- **[DONE] tracepcdiff.sh repair + AC diff (steps 1-2).** Repointed from the
  removed `DORADO_TRACEPC`/`DTRACEPC` to the present `DORADO_IFUDISP_TRACE`
  (`pcf` byte cursor → word PC = `pcf>>1`, vs ContrAlto `CATRACEPC` field 2) and
  added per-opcode **AC diffing** (ours `acs=STK[StkP+0..3]` = AC0..3 vs CA
  `acs=r3,r2,r1,r0` reversed; default mapping verified correct). The two boot
  paths start misaligned, so the script **auto-aligns** the boot-phase slip
  (slide ours 0..127 to best-match CA's PC stream; `SKIP_OURS`/`SKIP_CA`/
  `AC_PERM` override). First real result: on MissileCommand the PCs align and
  the **first AC divergence is at the first aligned opcode** — ours carries
  leftover boot ACs (AC1=056623, AC2=121045) where ContrAlto is clean 0
  (the cold-AC clear is gated to the disk-boot path, not the ether games — a
  candidate MC contributor). Tool + plumbing: `tools/nova-trace-diff/`.

### Phase 1 status (2026-06-23) — first divergence localized + one layer peeled

Ran the repaired `tracepcdiff` on **Invaders** (the recommended simple,
non-network game) and **MissileCommand**. Both show the **identical first
divergence**: with the boot-phase slip auto-aligned, the PCs match (3,4,5,6,7)
but ours carries the AEmu's leftover Stack ACs (AC1=056623, AC2=121045) at the
loaded program's first opcode where ContrAlto cold-boots clean 0 — an
**initial-state bug, not a cadence one**.

**Fixed:** extended the salto-verified cold-Alto init (clear Stack[1..4],
Alto I/O page = 177777, bank regs = 177760) from the disk-boot vector
(DiskBoot 0o2005) to the ether-boot vector (EBoot 0o2006), gated to
`alto_ether_boot` so Cedar is untouched (`machine.c`). The tracediff's first
divergence now moves from opcode #0 to opcode #1 (the residual opcode-#1 AC
delta is partly the known one-opcode lag in IFUDISP's dispatch-time AC
snapshot — cpu.h documents it; sharper AC diffing should use `DORADO_ALTOAC_TRACE`).

**Caveat — necessary but not sufficient.** Pixel counts are unchanged
(Invaders/AstroRoids 163, Galaxian 121553, Boggs 274967, MC 491264): this
peeled one cascade layer, it did not un-crash the games (consistent with the
"MC bug is a multi-layer timing cascade" finding). Next: keep walking the
tracediff later (switch to the `ALTOAC` trace for lag-free AC comparison),
correlate the next divergence with its I/O event, and proceed to the Phase 2
device/scheduler cadences — all from a **snapshot** of the running game so the
boot isn't re-run.

### Phase 2 notes (2026-06-23) — scanline cadence probed; need finer measure

First probe of the device cadences:
- The post-fix memory-write `tracediff` on MissileCommand still shows the
  documented **M[3016] write #1 divergence (ours writes 0 one extra time vs
  CA's 2616; CA caseq ~71887)** — the "a wakeup fires one beat early" cumulative
  fault. The cold-AC fix correctly did not touch it. M[600] diverges earlier
  (caseq 12) but that is a leading-write alignment slip (the harmless phase
  slip), not the root.
- **Scanline cadence (machine.c:2221) — a cumulative contributor, NOT the
  seed fix.** Pixel-neutral, but measured against the M[3016] tracediff it has
  a real effect: faster cadence cuts ours' M[3016] *oscillation* count
  (`DORADO_SCANLINE_CYCLES` 1000 → 343 → 250 gives 34649 → 27875 → 24052
  writes, toward ContrAlto's ~1800 settled — the "oscillating vs settles"
  pathology). But the **first-divergence index stays at write #1** in every
  case, so the scanline cadence does not fix the *seed* (the first field
  interrupt's phase). Kept the `DORADO_SCANLINE_CYCLES` knob (default 1000,
  gated to display_active, boot-safe) as the Phase 2 measurement instrument.
  Lesson: measure cadences against the tracediff index, never pixels.
- **The Alto field/RTC heartbeat runs in the display task** (ENDOFFIELD/
  EVENFIELD/RTCCARRY, machine.c:1912), woken via the scanline path. So the
  M[3016] "one beat early" is a display-task-wakeup phase issue — but proving
  which cadence moves it needs the **tracediff (M[3016] write sequence), not
  pixels**, as the measurement. Next: re-add the scanline (and field) cadence
  knobs and tune them watching M[3016]'s first-divergence index move later,
  driven from a snapshot of the running game.

### Phase 2 seed hypothesis + metric reframe (2026-06-23)

Pinned down where the M[3016] seed lives. The AEmu field interrupt
(ENDOFFIELD/EVENFIELD) is posted by the **microcode counting scanlines in the
display list**, not by C — so the scanline *cadence* can't move the seed (the
microcode counts scanlines, not cycles), only the oscillation rate downstream.
The seed (write #1: ours runs the interrupt handler one extra time, clearing
M[3016]=0 while the mainline is still depositing the 2616 dispatch) is the
**phase of the first field interrupt relative to the game's interrupt-enable**,
governed by *when* `display_active` (DASTART at VM 0o420 nonzero, machine.c:2209)
first turns the display task on. A real Alto's field fires at a fixed 60Hz
regardless of display-list install; ours gates it to display-list-installed, so
the first one lands at a different phase. **Candidate next experiment:** phase
the first scanline wakeup to a field boundary after `display_active` turns on
(or hold the field interrupt one field), and watch M[3016] write #1.

**Metric reframe (per this doc's own caveat).** Write #1 is likely the
"harmless phase slip"; the real crash is the **cumulative oscillation**. The
right success metric is therefore ours' M[3016] write *sequence* converging to
ContrAlto's settled 2616 (CA ~1800 writes vs ours ~24-34k). The scanline
cadence already cuts that ~20% — a genuine step. The holistic fix stacks this
with the scheduler latency + field-phase fixes until the oscillation collapses.

### Phase 2 REFRAME (2026-06-23) — the crashers' seeds are concrete bugs, not cadence

Two cheap experiments (field-phase delay, scanline cadence) failed to move the
seed, then characterizing the seed per-game changed the picture:

- **MissileCommand's M[3016] oscillation is network-specific.** Invaders (the
  recommended non-network game) writes M[3016] exactly **once (=0), matching
  ContrAlto** — no oscillation. So the M[3016] "one beat early" oscillation I
  chased is a MissileCommand symptom = the **ethernet spurious InDone/OutDone
  completions** (the deferred faithful-receiver item), not a display/scheduler
  cadence. Neither the scanline cadence nor a one-field delay on the first field
  interrupt moved it.
- **Invaders' seed is an early per-opcode AC divergence, not cadence.** Clean
  PC-only `tracepcdiff` (`AC_PERM=skip`): PCs align then ours executes PC 0o31
  one extra time at opcode #23 — but the **ACs diverge by ~opcode #2** (ours
  loads 6126/6373 into AC2/AC3 where CA has 0/1, recurring later), well past
  what the one-opcode IFUDISP AC lag explains. That is a per-opcode emulation
  or early Alto-memory-state bug.

**Consequence:** the working hypothesis that "most games crash from cumulative
*timing* divergence" is only partly right. After the cold-AC fix, the remaining
seeds found so far are **concrete emulation/device bugs** — ethernet spurious
completions (network games) and an early-opcode AC/memory divergence (Invaders)
— that the display/scheduler-cadence Phase 2 would not touch. The cadence work
(scanline reducing the MC oscillation) is real but secondary.

**Next, by game class:**
- Non-network (Invaders/AstroRoids): **the "IFU double-dispatch" was a 3rd tool
  artifact, not a bug** (2026-06-23 latest). The extra dispatch is the AEmu
  `Reschedule` trap (`AEmuReschedule`) -- a Dorado mechanism with no plain-Alto
  equivalent -- diverting a successful IFUJump to the reschedule vector; the
  held-back opcode is re-dispatched (and executed exactly once) afterward, but
  `tracepcdiff` was counting the trapped IFUJump as an executed opcode. Fixed:
  `cpu.c` IFUDISP trace prints `rtrap=%d`, the tool skips `rtrap=1` records, and
  the CA AC-order reversal bug is fixed (CA's `R[3..0]` is already AC0..3). With
  that, **Invaders' IR stream is identical to ContrAlto for 2091 opcodes**, then
  diverges at #2091 on a **3-instr spin-wait loop** (`LDA / MOV#0,0,SNR / JMP .-2`)
  polling a memory flag at `M[@AC2+255]`: ours exits, CA still spins. That flag is
  set asynchronously (interrupt/device), so opcode-count alignment breaks here by
  construction -- the natural horizon of opcode-by-opcode diffing. Next: identify
  what writes that flag and whether ours releases the loop earlier than the
  field-interrupt cadence allows (ties back to this plan's cadence core). Going
  past the wait needs coarser (frame-boundary / architectural-state) comparison,
  not more opcode diffing. See CONTINUE-HERE for the full trace.
- Network (MC): the ethernet faithful receiver (`docs/ethernet-faithful-receiver.md`).

### Phase 0 — Tooling (do this first)

1. **Repair `tracepcdiff.sh`.** It depends on a removed `DORADO_TRACEPC`/
   `DTRACEPC` emitter. Repoint it to the present `DORADO_IFUDISP_TRACE`/`IFUDISP`
   per-Alto-instruction trace (`cpu.c:2916`): grep `IFUDISP`, parse `pcf=`
   (a *byte* cursor) → word PC = `pcf >> 1`, compare to ContrAlto `CATRACEPC`
   `parts[2]` (a *word* PC). Now you can diff the executed Alto PC stream.
2. **Add register/AC diffing** to the tracediff. The ACs are already carried in
   both trace lines (ours `IFUDISP … acs=…`, CA `CATRACEPC … acs=…`); compare
   them, not just the PC. The first AC divergence at a matching PC is a precise
   behavioral fault.
3. **Add machine snapshot/restore** — `dorado_machine_snapshot(m, path)` /
   `_restore(m, path)` dumping the full state: `mem` (storage, cache, map, pipe,
   BR, MCR), `cpu` (all registers + per-task arrays + IFU + tasking), and the
   I/O device state (display, disk, ethernet). The structs are well-defined; this
   is mechanical. Validate: snapshot at cycle N, restore, run — must be
   bit-identical to not-snapshotting. **This is the highest-leverage tool.**

### Phase 1 — Localize the real divergence

- Use a **simple, non-network game** first (Invaders or AstroRoids — no
  post-boot ethernet, so the divergence is display/RTC/scheduler, not the harder
  ether-completion problem). Boot it, snapshot the running game.
- From the snapshot, run ours + ContrAlto and `tracediff` several candidate
  addresses (start with `3016 1104 1100 2625 600`); the divergence with the
  **lowest ContrAlto `caseq`** is closest to the root. Then use the repaired
  `tracepcdiff` + AC diff to find the exact instruction + which AC/cell first
  differs.
- **Correlate the divergence with the I/O event** that caused it: trace the
  Dorado-side events around that cycle — display field/scanline wakeups
  (`machine.c:2198`), RTC (`DORADO_RTC_TRACE`), reschedule (`DORADO_RESCHED_TRACE`),
  ethernet. Identify *which cadence is one beat off*.

### Phase 2 — Make the device I/O + scheduler cycle-accurate

Replace the approximate cadences with physically-grounded timing, **validated
against ContrAlto from the snapshot at each step** (the boot is not re-run, so a
change that would have desynced the boot is instead measured on the running
game; you tune until the tracediff divergence moves *later*). Prime targets,
roughly in order of how concrete the spec is:

- **Display field/scanline timing.** Real Alto scanline ≈ 20.6µs (~343 BB
  cycles), field = 16.67ms (277778 BB cycles, already correct at `machine.c:225`).
  Fix the `+1000` scanline cadence (`machine.c:2206`) and verify the field
  interrupt reaches the Alto OS at the right phase. This is the prime suspect for
  the *non-network* games' early interrupt.
- **Scheduler timing.** HM p27: 2-cycle wakeup→execution latency + 2-instruction
  TaskingOn gate. The reverted `DORADO_WLAT` code (see git history of
  `cpu.c`/`task_schedule`) is a correct *starting point* for both — but it only
  works once the device cadences above are also accurate (that's why it desynced
  alone).
- **Ethernet — the faithful receiver** (for the *network* games like MC; the
  no-render doc's lead). Model the real Alto controller's FIFO + receive state
  machine so completions post **once and settle** instead of oscillating
  (ours spuriously posts InDone/OutDone when there is no real packet; see the MC
  doc §"Conclusion"). High risk — naive completion-suppression regressed
  NetExec; do it from the snapshot with the tracediff watching `M[0o600]`/
  `M[0o3016]`. The faithful-receiver design notes are in
  `docs/ethernet-faithful-receiver.md`.
- **Disk** (separate track, task #8): replace the DSK-task wakeup stub +
  unconditional `HeadOvfl` (`disk.c:1372`, `:1040`) with a real
  sector-index/sequence-PROM tick. Needed for the disk Exec to draw.

### Phase 3 — Re-establish the boot with the accurate timing

Once the running game matches ContrAlto, the cadences are correct. Apply them to
the boot path (remove the `DORADO_*` gates, make accurate the default). The boot
will likely break first, because it had *other* inaccuracies the approximate
cadences compensated for (e.g. the ethernet EFTP lock-step turnaround, the
breath-of-life injection cadence). Fix those using the now-accurate cadences and
the tracediff on the **boot phase** (watch the loader's `EPLOC`/`AC0` poll).
This is the genuinely hard part; budget for it.

### Phase 4 — Validate

Re-run the survey — the crashers should progressively render. Gate every step on
**Galaxian still rendering correctly** (visual, px may shift), **`make test`**,
and **the tracediff first-real-divergence moving later**. Add an asserting
boot-path smoke gate (convert a `probe_*` in `test_cpu.c`, currently
non-asserting) so the boot has regression protection during this work.

---

## 5. Tools, gates, and trace flags

- **Cross-validation harness:** `tools/nova-trace-diff/`. `tracediff.sh <octal-addr>
  [boot] [cycles]` works today (memory-write value-sequence diff vs ContrAlto;
  both net-boot the same `.boot` via a ContrAlto BootServer that mirrors our
  ethernet). `tracepcdiff.sh` is stale (Phase 0.1). ContrAlto headless is built
  at `AltoInfo/contralto-headless/` (run with `DOTNET_ROLL_FORWARD=Major`); its
  patches (`CA_TRACEW`, `CA_TRACEPC`, `BootServer`, `CA_NETBOOT`/`CA_BOOTSERVER`)
  are in the gitignored `AltoInfo/Contralto2-2.0-Beta/`.
- **Gates:** Galaxian = 121553 px (`--eftp ../chm/bootfiles/Galaxian.boot!1
  --cycles 160000000`); `make test` (11 suites — but note: missing `chm/`
  fixtures make some SKIP-and-pass, so a green `make test` ≠ all ran).
- **Useful trace flags** (all gated by `getenv`, octal where noted):
  `DORADO_HOLD` (the committed Hold model), `DORADO_STORE_TRACE_VA=lo,hi`
  (octal; logs stores with Alto pcx/br31/op), `DORADO_IFUDISP_TRACE`
  (per-opcode Alto PC), `DORADO_DCB_TRACE` (display-list walk),
  `DORADO_RESCHED_TRACE`, `DORADO_RTC_TRACE`, `DORADO_TASK_TRACE`,
  `DORADO_TRACE_GATE=lo,hi` (cycle window for gated traces),
  `DORADO_DISK_HDR_TRACE`. Full list: `grep -oE '"DORADO_[A-Z_]+"' src/*.c`.

---

## 6. Key files + code locations

- **Engine step / scheduler:** `cpu.c` — `dorado_cpu_step` (3334), `execute_uinstr`
  (3497, ends with `task_schedule` + `cpu->cycles++` + `baseboard_run` at
  ~4435), `task_schedule` (270, the wakeup eligibility + the `avail = ready |
  wakeup_pending` at ~300), `dorado_cpu_wakeup` (128), the TaskingOn gate
  (`tasking_resume_delay`, FF handler ~1490). `cpu->cycles` advances 1:1 with
  `bb->cycles` (machine loop calls `dorado_cpu_step` at `machine.c:2085`;
  `baseboard_run` advances `bb->cycles`).
- **Hold model (committed, gated):** `cpu.c` — Hold check at the top of
  `execute_uinstr`, `uinstr_reads_md` (after `ref_kind_loads_md`),
  `latch_task_md_from_memory`. `memory.c` — `last_ref_latency` in
  `dorado_memory_ref_task` (the Fetch hit/miss path). `cpu.h` —
  `task_md_ready[16]`. `memory.h` — `last_ref_latency`.
- **Cadences (machine.c run loop, ~1309–2236):** display scanline/DWT wakeups
  (2198), breath-of-life / time broadcast (2157), field interval
  (`CEDAR_FIELD_INTERVAL_CYCLES = 277778`, 225), `machine_cedar_io` (857, the
  Cedar field interrupt). The Alto display tasks are woken here, gated on
  `display_active` (which reads the transient global `br[30]` MDS — a separate
  render-snapshot fragility, see the `alto-disk-boot-no-paint` memory).
- **Ethernet:** `ethernet.c` — eftp turnaround/timing (~122, ~658), the
  completion/wakeup logic, RxOn-clear gating (~1333). Design notes:
  `docs/ethernet-faithful-receiver.md`.
- **Disk:** `disk.c` — DSK-task wakeup stub (~1040), PDI shim `HeadOvfl` (~1372).
- **Render:** `machine.c` — `dorado_machine_render_display_list` (2417).

---

## 7. Hard parts / open questions

- **The co-tuning web** is the central difficulty. The snapshot/restore tool is
  the proposed way around it during development; Phase 3 (re-establishing the
  boot) is where it must finally all be consistent — budget real time there.
- **The ethernet faithful receiver** is high-risk (it shares machinery with the
  working boot path; naive changes regressed NetExec). Do it last among the
  device fixes, snapshot-isolated.
- **Is calibration enough, or is structural modeling required?** Two sub-options
  for Phase 2: (a) *calibrate* the existing approximate cadences to ContrAlto's
  ratios (less code, may not generalize), or (b) *structurally model* each
  device's timing (FIFOs, sequencers, the real scanline clock — more faithful,
  more work). Recommendation: calibrate first to prove the ratio is the cause
  and get games rendering, then structurally model where calibration is fragile.
- **Cross-validation caveat:** the tracediff's first divergence is often the
  harmless phase slip — always order by ContrAlto `caseq` and look *past* it for
  the substantive (cumulative) fault.

---

## 8. Quick-start for a fresh context

1. `git checkout fidelity-timing` (the Hold model + audit + this plan). Confirm
   green: `make test`; Galaxian = 121553.
2. Build the tooling (Phase 0): repair `tracepcdiff.sh` → IFUDISP; add AC diff;
   **add machine snapshot/restore** (the key enabler).
3. Boot **Invaders** (simple, non-network), snapshot the running game.
4. From the snapshot, find the first *real* divergence vs ContrAlto and the I/O
   event behind it (Phase 1).
5. Make that cadence cycle-accurate, validated from the snapshot (Phase 2);
   iterate.
6. Re-establish the boot (Phase 3); re-run the survey (Phase 4).

Do not chase individual cadences against a live boot — that is the trap that
desyncs (proven twice). Decouple with the snapshot, anchor on ContrAlto, and
make the timing accurate holistically.
