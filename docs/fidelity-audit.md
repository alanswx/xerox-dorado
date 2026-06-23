# Dorado emulator fidelity audit (2026-06-22)

A root-cause audit prompted by: **the netboot doesn't work for most Alto games
(they crash), and the disk boot never reaches a drawable Exec.** Canon = the
September 1981 Hardware Manual (HM) + board schematics.

## Executive summary

The failure is **systemic, not per-game**. One root explains "most games
crash," "MissileCommand is frozen-black," and "the disk OS boots but never
draws": the emulator has **no Hold / memory-timing model** (every memory
reference completes atomically in one cycle), while the I/O task wakeups
(display scanline, disk sector, ethernet completion) are **pinned to fixed
cycle intervals**. So the emulator task advances *too fast and out of phase*
relative to the clock-driven I/O tasks. Simple programs (Galaxian, Boggs)
tolerate the drift; complex ones (the OS, most games) accumulate enough
state/timing divergence to derail. The many hand-tuned constants and
env-gated pokes scattered through the code are **band-aids around this one
root** — each tunes the interleaving at a single point because the global
cycle budget is wrong.

Five independent lines of evidence converge on this:

1. **Game survey** (below): the renderer, boot loader, and simple programs are
   sound; the failures are downstream and broad.
2. **Heuristics audit** (`dorado/src/*.c`): top systemic suspect is "Hold not
   modeled while I/O wakeups are cycle-pinned," plus a self-confessed *tuned*
   TaskingOn/wakeup-latency pair.
3. **Test audit** (`dorado/tests/`): "data-correctness focused; cycle/Hold
   timing is the systemic blind spot" — Hold/timing is essentially untested.
4. **The spec docs themselves** (`docs/memory-architecture.md`): Hold is listed
   as the #1 "known shortcoming," explicitly "not modeled."
5. **`dorado/CLAUDE.md`**: Hold is a documented gap; boot runs with `disHold`
   so it doesn't need it, but *post-boot emulators (the games) do*.

## Game survey (ether boot, 250 M cycles)

Pixel count is only a rough proxy — high px can be a real render (Boggs =
274967, a halftoned portrait) OR frozen-black garbage (MissileCommand =
491264). The reliable signal is the dead bucket:

- **Flat 0 px (17/38, dead on arrival):** AlphaMesaMesaNetExec, CedarNetExec,
  Chat, Clock, FTP, GateControl, Johnsson, Kal, MazeWar, MesaNetExec, Murray,
  Pool, PPong, Pupwatch, ShowAIS, StarWars, Trek.
- **Near-blank (crashed early):** AstroRoids/Invaders 163, What 301, Messenger
  383, Scavenger 603, Reversi 831.
- **Renders real content:** Galaxian 121602, Boggs 274967 (photo), EtherLoad
  428581, EDP 39023, Calculator 14333, KeyTest 13076, MadTest 18892, CopyDisk,
  PupTest, BFSTest. (Each still needs a visual check — some "high px" are
  frozen, e.g. MissileCommand.)

Conclusion: well over half don't work — confirms the report.

## The systemic root, precisely

- **Memory references are atomic.** Real Dorado: cache hit ≈ 3 cycles, clean
  miss ≈ 16, dirty miss ≈ 24+ (HM Table 15 / Fig 9), and the *entire pipeline
  (microengine + IFU) freezes (Hold) until `Md` is ready*. Our model completes
  in one cycle (`dorado/include/memory.h:12-14`, `docs/memory-architecture.md`
  §"Hold is not modeled").
- **I/O wakeups are cycle-pinned.** Display H-task wake every `+1000` BB cycles
  (`machine.c:2198-2207`; real Alto scanline ≈ 20.6 µs ≈ 3× faster);
  breath-of-life / time-broadcast at hand-tuned intervals (`machine.c:2157`);
  ethernet EFTP turnaround a tuned constant (`ethernet.c:122`).
- **The task scheduler picks the highest-priority awake task each microcycle**
  (HM §4.1). With the emulator task running too fast and the I/O tasks woken on
  fixed clocks, the *phase relationship* between a game's main loop and its
  field/RTC/ethernet interrupt handlers drifts — data-dependently, which is why
  light games survive and heavy ones desync.

## Prioritized heuristics to replace (full table in the audit logs)

TIER HIGH (systemic / per-instruction cadence):
- `cpu.c:1522`, `:3170` — **Hold is a no-op.** The fix is the whole project.
- `cpu.c:1467-1491` — **tuned TaskingOn (1 instr vs HM 2) + immediate wakeup
  (vs HM 2-cycle)**; comment admits "the boot is tuned to them, changing one
  alone desyncs them." Self-nominated as the residual async-timing bug site.
- `machine.c:2198-2207` — display scanline wake `+1000` (~3× too slow).
- `machine.c:2157-2177` — breath-of-life/time-broadcast injection cadence
  ("a flood derails the world").
- `memory.c:337-342` — **MCR `0xFEE7` magic-match**: only this one MCR value is
  decoded as "normal refs, no fault wakeups"; any other value the world writes
  silently mis-decodes Hold/fault/wakeup enables. (Needs a real MCR bit decode
  from MEMC/MEMX.)
- `machine.c:1559-1568` — **identity-map shim** at PC 06222 installs a VM→phys
  map in C instead of the world's `PresetMap`; a wrong map shape corrupts every
  later reference.
- `ethernet.c:1333-1349` — RxOn-clear gated Cedar-only (fragile boot coupling).
- `disk.c:1372-1382`, `:1040-1045` — **PDI shim forces `HeadOvfl`
  unconditionally + DSK-task wakeup stubbed**; the controller is never ticked
  on the shim path, so no sector wakeups arrive. This is the *disk-specific*
  reason the disk Exec never draws (separate from the games' timing root).

Specific HM contradictions found by the test audit (bounded, verifiable):
- **Pipe4 no-error baseline**: source uses `0170361` (`memory.c:315`, comment
  "corrected baseline") but HM §5.8 + `memory.h:392-393` + `hardware-specs.md`
  QW4 say `0150361` — they differ in **bit 13 = wProtect**, which microcode
  reads. The repo contradicts itself; resolve against the HM before trusting
  either. Four test assertions encode `0170361` as golden.
- **Cache victim policy**: emulator uses true-LRU (`cache_pick_victim`,
  `memory.c:381`); HM §5.7 / MemC04 specify **VNV pseudo-LRU**. Four tests lock
  in the wrong model.

## Test-suite gaps (why none of this was caught)

- **Hold / engine stall: zero coverage.** No test exercises Md-not-ready,
  Pipe-full, DBuf-conflict, the six Address-Section-busy conditions, or
  reference-during-writeback. Every memory test runs references atomically.
- **Tasking/IFU timing: untested** — 2-cycle wakeup latency, TaskingOn 2-cycle
  delay, the IFU 6-deep prefetch pipeline.
- **6 `probe_*` functions in `test_cpu.c` assert nothing** (always `return 0`,
  ~44% of the file) — the boot path has no regression gate.
- **Suite-level:** missing `chm/` fixtures → tests `SKIP` and `return 0`, so a
  green `make test` does not prove the microcode-dependent half ran.

## Cross-validation harness (the verification tool)

`tools/nova-trace-diff/tracediff.sh <octal-addr> <boot-file>` **works today**:
it diffs the ordered value-sequence written to one Alto address between our
emulator and ContrAlto (both net-boot the *identical* `.boot` via a ContrAlto
BootServer that mirrors our ethernet), tagging ContrAlto writes with a global
sequence counter so divergences can be ordered (lowest `caseq` = closest to the
root). `tracepcdiff.sh` is stale (its `DTRACEPC` emitter was removed; repoint
it at the existing `DORADO_IFUDISP_TRACE`, byte→word PC). ContrAlto headless is
built at `AltoInfo/contralto-headless/` (run with `DOTNET_ROLL_FORWARD=Major`).

## Empirical confirmation (tracediff, 2026-06-22)

Ran `tools/nova-trace-diff/tracediff.sh` on the interrupt-handler dispatch
cell `M[0o3016]` (ContrAlto net-boots our exact `.boot` via the BootServer, so
both run identical bytes):

- **MissileCommand:** ours `0,0,2616,2616,1537,1537,…` (34651 writes,
  oscillating) vs ContrAlto `0,2616,2616,…` (1990, settles at 2616).
- **Invaders** (standalone, no post-boot network): ours `0,0,2616,2616,…`
  (66 writes, **settles**) vs ContrAlto `0,2616,2616,…`.

Both games share the **identical first divergence (write #1: ours=0,
CA=2616, at Alto `br31=0o644 pc=2`)**: ours writes `M[3016]=0` one extra time
— it takes one extra early interrupt-handler pass before the mainline commits
the dispatch (2616), where ContrAlto has already committed. So a **wakeup
fires one beat early**, and this is **common, not game-specific**. Invaders
tolerates the extra pass (settles, then crashes later); MC cascades it into
the ethernet-completion oscillation (2616↔1537↔0 forever). This confirms the
timing root and narrows the *first* divergence from "all timing" to the
**interrupt/wakeup cadence** (the cycle-pinned display/RTC/field wakeups in
`machine.c:2157-2207`, and/or the missing Hold that lets the emulator task
reach the interrupt point a beat early).

**Next grounded step (do not guess at the co-tuned cadences):** pin the exact
divergent *instruction* — repoint `tracepcdiff.sh` to the present
`DORADO_IFUDISP_TRACE` (byte→word PC, per the harness assessment) and diff the
executed Alto PC stream for Invaders to see precisely which wakeup/interrupt
fires early relative to ContrAlto. That identifies the single cadence to fix
first, before touching the co-tuned timing set.

## Replacement plan (sequenced, each step gate-verified)

The gates that must stay green throughout: `make test`, Galaxian = 121553 px,
the working renders (Boggs, EtherLoad, Calculator…). Use `tracediff` to confirm
each change moves the first divergence later.

1. **Bounded, low-risk corrections first** (build confidence, fix real data
   bugs): resolve the Pipe4 `0170361`/`0150361` contradiction against the HM and
   fix source+tests; decide true-LRU vs VNV; convert ≥1 `probe_*` into an
   asserting smoke gate so the boot path is protected before timing changes.
2. **Replace the MCR `0xFEE7` magic-match** with a real MCR bit decode (MEMC/MEMX
   schematics + HM) — unblocks worlds that set MCR differently than boot.
3. **Implement Hold incrementally** — the systemic fix. Give each memory
   reference a ready-cycle; assert engine Hold when microcode consumes
   `Md`/`DBuf`/ADDRESS/MAP before the memory section can supply it; model
   cache-miss latency (Table 15); add the Address-Section-busy scoreboard.
   Do it behind the existing `mcr.disHold` so the boot path (which sets disHold)
   is unaffected until each piece is verified.
4. **Fix the cycle-pinned cadences** *together with* Hold (HM 2-cycle wakeup
   latency, TaskingOn 2-cycle delay, real ~20.6 µs scanline) — these are
   co-tuned, so they must move as a set or the boot desyncs.
5. **Re-verify the game survey** — the divergence-driven test is whether the
   crashing games progress further as the timing model lands.

The disk path is a **separate** track: replace the `disk.c` DSK-task wakeup stub
and the unconditional `HeadOvfl` with a real sector-index/sequence-PROM tick so
the disk Exec can draw.

## Hold model — scoped implementation steps (next phase)

Confirmed feasible against the code: `dorado_memory_ref_task` (memory.c:957)
already knows hit/miss (`dorado_cache_lookup`) and the dirty-victim; `task_md[16]`
/ `task_md_valid[16]` exist (cpu.h:294); `dorado_mcr_dishold` exists
(memory.c:488); the IFU already has a Hold-pipeline pattern to reuse (cpu.c:3076).

1. **Expose per-ref Md-ready latency.** In `dorado_memory_ref_task`, record
   `mem->last_ref_latency` = **3** (cache hit) / **16** (clean miss) / **24**
   (dirty-victim miss) per HM Table 15 (the `docs/memory-architecture.md` t0..t24
   ladder).
2. **Record the ready cycle.** Add `cpu->task_md_ready[16]`; where a
   Fetch/IFetch/LongFetch sets `task_md_valid` (cpu.c:1899), also set
   `task_md_ready[task] = cpu->cycles + mem->last_ref_latency`.
3. **Hold enforcement.** Before `execute_uinstr`, if the µinstr reads Md
   (ASEL/BSEL = Md source) AND `!disHold` AND `cpu->cycles < task_md_ready[task]`
   → Hold: don't execute, advance `cpu->cycles`, let the scheduler pick (a
   higher-priority I/O task runs), re-run this instruction when the task is next
   selected (reuse the cpu.c:3076 Hold pattern).
4. **Gate behind `DORADO_HOLD` env first** so default gates are unchanged; enable
   and verify Galaxian (visual, px may shift) + the survey + `tracediff` (the
   first divergence must move later); then make it the default for disHold-clear
   worlds.
5. **Co-tuned cadences, moved as a set** (else the boot desyncs): HM 2-cycle
   wakeup latency + TaskingOn 2-cycle delay (cpu.c:1467) + real ~343-cycle
   scanline (machine.c:2206). Verify each against `tracediff`.

This is a multi-turn engine build, gate-verified at every step — not a one-shot
patch. The diagnostic/audit/confirmation phase above is complete; this is the
implementation phase.

## Hold model — IMPLEMENTED & VALIDATED (2026-06-22)

Implemented steps 1-3: per-ref Md latency (`memory.c` `last_ref_latency` =
3/16/24), `cpu->task_md_ready[16]` recorded at fetch latch, and a Hold check at
the top of `execute_uinstr` (jump-to-self via `task_schedule`, `cpu->cycles++`
+ `baseboard_run`), gated by `DORADO_HOLD`. Result:

- Builds clean; **all 11 test suites pass**; default path **byte-identical**
  (Galaxian 121553).
- `DORADO_HOLD=1`: Galaxian renders the **full game correctly** (121639 px — a
  tiny starfield timing shift). The working path is preserved.
- The memory timing is now modeled — Hold fires ~4M times during boot.

**CRUCIAL VALIDATION FINDING — the memory Hold is MOOT for the running games.**
Instrumented (`DORADO_HOLD_TRACE`): Hold fires 4M times in the boot
(0-37.7M cyc) then **ZERO times once the AEmu emulator loop runs**, with **ZERO
`disHold`-blocks**. So the AEmu microcode *covers* the memory latency (reads Md
only after it is ready). The memory Hold, though correct, is **not** the
systemic game divergence — this rules the memory-timing hypothesis OUT for the
games.

**The real divergence is the I/O WAKEUP CADENCE.** The first divergence (both
MC and Invaders write `M[3016]=0` one extra time — a wakeup fires one beat
early) is the cycle-pinned wakeup timing + the **immediate wakeup latency**
that `cpu.c:1478` itself flags as "the right place to look for residual
async-timing bugs". Naively changing it desyncs the boot (the comment: raising
the delay "spins the I/O tasks … boot never reaches NetExec"), so the game-fix
needs the full cycle-exact wakeup model (2-cycle wakeup latency + 2-instruction
TaskingOn gate, moved **together** + boot re-verified) — a distinct, delicate
effort, NOT the memory Hold.

The Hold model is kept (correct, gated, a fidelity improvement and a foundation
for any Hold-dependent microcode), but it is not the game fix.

## Wakeup-cadence fix attempted — DESYNCS the boot (2026-06-23)

Implemented the cycle-exact wakeup cadence behind `DORADO_WLAT`: the HM p27
**2-cycle wakeup→execution latency** (gate wakeup eligibility by raise-cycle in
`task_schedule`) **paired with** the HM **2-instruction TaskingOn gate**
(`tasking_resume_delay` 2→3). Result:

- Default (WLAT off): unchanged (Galaxian 121553).
- WLAT on: Galaxian **0 px — the boot DESYNCS**, both alone and paired with
  Hold.

This confirms the deep co-tuning `cpu.c:1478` and the no-render doc describe:
the boot is tuned to a **web** of approximate cadences (wakeup latency,
TaskingOn gate, display scanline `+1000`, RTC/field, I/O-completion, disk,
ethernet). Making *any subset* cycle-exact desyncs the rest — the
wakeup-latency + TaskingOn pair alone is **not** sufficient. The WLAT
experiment was reverted (non-working); the Hold model stays.

**Conclusion (both incremental candidates now ruled out):** the memory Hold is
moot (microcode covers the latency) and the wakeup cadence desyncs (the
cadences are not independently changeable). The most-games-crash fix is the
**full cycle-accurate timing model** — all device/scheduler cadences made
consistent *simultaneously* — a large architectural project, exactly the
independent conclusion the MissileCommand investigation reached
(`docs/restart-alto-games-no-render.md`: "cycle-accurate emulation timing … an
architectural project"). Incremental cadence fixes do not converge.
