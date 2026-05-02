# Boot bring-up plan

A staged plan for getting from "Bootstrap microcode runs against our
microengine" (where we are now) to "Mesa loads Pilot from disk and a
hello-world appears on the display."

This is a *design doc*. It captures decisions we want to make
deliberately rather than rediscover under pressure later. Update it as
phases land.

## The normal boot sequence (HM + Booting Memo Apr 1980)

For reference. Two end-to-end boot paths exist on real Dorado:

### Path A: Power-on / 3-push button (full bootstrap)

1. **BaseBoard wakes up.** 6502 monitors power supplies, temperature,
   boot button, drives green LED.
2. **BB halts the Dorado**, issues IO reset, clears Hold and Fault
   wakeup state.
3. **BB loads Bootstrap microcode** (~50 instructions) into IM via
   *MIR jamming* — the BB drives MCPBus to deposit one
   microinstruction at a time. Slow.
4. **BB starts Bootstrap running**, then *streams Initial microcode*
   (~700 instructions) via CPReg. Bootstrap polls CPReg, decodes
   bytes, writes to IM via `Write IM`.
5. **Bootstrap hands off to Initial.** They occupy disjoint IM regions.
6. **Initial executes**: full hardware init (parity, cache, Map,
   storage, BR), enables I/O tasks (Display, Ethernet, Junk), reads
   keyboard for emulator selection.
7. **Initial contacts Ethernet boot server** (Gateway/IFS): requests
   the chosen emulator microcode (Mesa/Cedar/Lisp/Smalltalk/Alto),
   stores it in main memory.
8. **Initial calls LoadRam** which loads the emulator microcode from
   main memory into IM/IFUM. The emulator REPLACES Bootstrap+Initial.
9. **Emulator runs.** Memory init, disk-partition reset, I/O init,
   then initiates an Alto-style software boot from disk or Ethernet.
10. **Software OS loads** (Alto OS, Pilot, etc.) from disk/Ethernet.

### Path B: LoadMB (microcode self-load, while running)

Faster path used when an emulator is already running:

1. The running emulator (or Bootstrap+Initial) loads a `.MB` file
   from disk into main memory via the file system.
2. A `LoadRam` procedure reads the file's IM/IFUM/RM/ALUFM data and
   writes the new microcode to control store.
3. The new microcode replaces the old; execution jumps to the
   `InitMap` label (= 0o1076) of the new emulator.

`LoadMB.run` is the user-space program that does this. **Our
`probe_aemu` is the LoadMB equivalent**: it directly populates
`mc.im[]` from a chain of layered `.MB` files (Initial + kernel +
memMisc + IfuComplex + AEmu), then starts the CPU at AEmu's `START`
label.

## What's required for each milestone

```
                    ╔════════════════════════════════════╗
                    ║ Bootstrap microengine runs (DONE)  ║
                    ╚════════════════════════════════════╝
                                     │
                                     ▼
        ┌─────────────────────────────────────────────────┐
        │ Bootstrap loads Initial via CPReg byte stream   │
        │ (probe_bootstrap_pure: spin loop works; needs   │
        │  BB to feed CPReg with real Boot1Data bytes)    │
        └─────────────────────────────────────────────────┘
                                     │
                                     ▼
        ┌─────────────────────────────────────────────────┐
        │ Initial: hardware init (Map/Cache/Pipe/BR)      │
        │ + enables I/O tasks (Display, Ether, Junk)      │
        │ + Ethernet client to fetch emulator             │
        └─────────────────────────────────────────────────┘
                              │            │
                              │  Path A    │  Path B (shortcut)
                              ▼            ▼
        ┌──────────────────────┐  ┌──────────────────────┐
        │ Ethernet boot server │  │ LoadMB-style direct  │
        │ Gateway/IFS proto    │  │ load (probe_aemu)    │
        └──────────────────────┘  └──────────────────────┘
                              │            │
                              └─────┬──────┘
                                     ▼
        ┌─────────────────────────────────────────────────┐
        │ Emulator microcode (Mesa/Cedar/Lisp/Smalltalk)  │
        │  + Disk subsystem (T-80 SMD)                    │
        │  + Display (DDC + DHT/DWT)                      │
        │  + IFU running emulator opcodes                 │
        └─────────────────────────────────────────────────┘
                                     │
                                     ▼
        ┌─────────────────────────────────────────────────┐
        │ Alto OS / Mesa Pilot loads from disk            │
        │ Hello-world on screen                           │
        └─────────────────────────────────────────────────┘
```

## Current state (2026-04-30, post Initial display/disk-start fixes)

**Microengine works against real microcode.** Recent fixes (ALUFM
extraction carry bit, `Pd←ALUFMRW` bit mapping, `CPU_QUADRANT_SIZE`,
arithmetic-only Carry'/Overflow update, B←RWCPReg legacy stub,
Config' storage reporting, HM Table 11d ALU one-bit shifts, HM Table
8a memory-reference FF branch conditions, and `Store←/Fetch←T`
A/Mar routing) were all real correctness bugs that unit tests alone
did not catch but real microcode hit. With them in place:

- `probe_aemu` runs **real AEmu microcode** for 200K cycles:
  STARTEMULATOR → RESUMEEMULATOR → SETUPBRS → DOBRS×12 → IFU
  dispatch loop (LRTYPETABLE → LRTYPEIM → LRNOPREFNEXT →
  LRLOOPTOFF → TOFFRET, repeating). The loop is real Mesa-emulator
  startup code waiting for memory references that never complete
  because the memory subsystem isn't fully wired.
- `probe_bootstrap_pure` (no BB attached) runs Bootstrap.MB:
  walks BOOTSTRAP through 16 init instructions (including 4
  runtime `Pd←ALUFMRW` writes that re-init ALUFM[8/10/14/...]),
  reaches READBB, enters the spin loop on 0o7747 → 0o7742 → 0o7741.
  With `cpu.cpreg=0x8000` (mimicking BB's AMSync preset), exits
  the loop on the first iteration and proceeds through 0o7746 →
  0o7715 → 0o7702 → re-enters READBB. The loop is genuine — it
  reads a CPReg byte each iteration and Bootstrap writes it to IM.
- `probe_full_boot` (BB + Dorado coupled) is *not* progressing past
  0o7744 because the **BaseBoard ROM contains a NEWER Boot0 binary**
  that differs from Bootstrap.MB. Different bytes, different path.
  Disassembling that newer binary instruction-by-instruction is a
  separate exercise; not blocking the rest of bring-up.

What works
- Microinstruction decode, RM/STK access (push/pop), T/Q/Cnt/ShC,
  full JCN (Local/Global/Long/Conditional/Return/IFUJump), full
  shifter (ShC- and FF-controlled, all four mask ops), FF
  dispatcher (large subset).
- ALU: 16 ALUFM ops, arith vs logical distinction, Carry'/Overflow
  preserved on logical ops per HM page 30.
- BaseBoard 6502 with full RIOT/timer/IRQ + analog comparators.
- BB↔Dorado handshake: MCPBus strobes (Control/Clock/ABMux/MIR),
  CPReg streaming, MIR injection + SetSS single-step, AMSync.
- Real Write IM, BLOCK=1 STK push/pop with HM Table 6 sub-decodes.
- Memory subsystem: cache 4×64×16, Map (16K entries), Pipe (16
  entries), BR (32 entries), Fetch/Store/IFetch, Map faults.
  Config' reports one present 4MW module for the default 64Kx1
  backing store, and physical storage references past installed
  modules fault instead of wrapping.
- IFU: IFUM 1024×24-bit, prefetch+pipeline, IFUJump dispatch,
  4 entry-vector slots, NotReady trap, conditional IFUJump.
- Tasking: 16 tasks, priority-scheduled, T/TPC/MemBase/Link
  replicated, BLOCK clears wakeup, TaskingOff/On, FreezeBC,
  Reschedule trap (*14-17), SubTask OR into RBase[2:3]/MemBase[2:3].
- Slow-IO routing layer (`include/io.h` + `src/io.c`): per-(task,TIOA)
  device table, floating-bus default 0xFFFF + bad parity. Wired
  through Pd←Input / Pd←InputNoPE / Output←B in cpu.c.

What works (continued)
- **Display Phase 1** (`include/display.h` + `src/display.c`):
  808×606 mono framebuffer, DDC catch-all slow-IO handler on tasks
  DHT/AHT/AWT/DWT, per-channel NLCB/CLCB, HRam/Mixer/Statics state
  buckets, per-channel FIFO, PGM snapshot, headless keyboard words,
  symbolic Alto key mapping from Palo's keyboard matrix, and a
  vblank-based frame counter. DDC input currently returns the
  headless keyboard idle word; the real 7-wire terminal back-channel
  and keyboard message decoder are still missing. The boot probe writes
  a viewable PGM framebuffer snapshot to `/tmp/dorado_boot_display.pgm`
  by default, or to `DORADO_BOOT_SNAPSHOT` when that environment
  variable is set.
- **Disk Phase 1/2 subset** (`include/disk.h` + `src/disk.c`):
  Trident T-80/T-300 pack format (2 dummy bytes + 2 header words +
  10 label words + 1024 data words = 2074 bytes/sector), drive struct
  with online/select state, controller registered on task 14₈ TIOA
  10₈-14₈ with DiskControl bit decode + Format RAM auto-increment +
  DiskData FIFO + DiskTag decode + DiskMuff status readout. Drive
  Select now tracks the selected drive's subsector divider, so the
  synthetic sector wakeups use the controller's 117-pulse-derived
  sector cadence instead of conflating it with the 9-sector pack image
  layout. FIFO reads stream a full sector record when exercised by
  tests; Initial reaches the disk boot routine now, but it still does
  not issue DiskData inputs in the full boot probe.

**probe_full_boot_with_bootstrap** (added 2026-04): the BB drives
its real Boot1 byte stream through CPReg while Bootstrap.MB is
substituted in IM. Current bring-up uses a second workaround at
BOOTSTAGE2: canonical Initial.MB is restored before INITIAL runs,
because Bootstrap streaming still does not match the CHM Initial.MB
image. With that in place, the probe now demonstrates:
- BB cold-boots → MIR-jams Boot0 → starts Dorado free-running.
- IM swap to Bootstrap.MB at first IM-fetched cycle.
- BB streams Boot1Data via CPReg; Bootstrap writes 896 unique Initial
  targets beginning at 0o6100, two half-writes each.
- Initial runs through ALUFM init, RMINITL, IFUMINITL, PRESETMAP,
  FINDMODULE, BootMem's memory-reference wait loop, BootEmulator's
  first-64K zeroing loop, display/disk/task init, and the 100 ms
  RTClock wait after Junk task wakeups are modeled.
- The full probe now mounts
  `../AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk300` when
  present. Because terminal keyboard input is not modeled yet, the
  probe forces `ETemp0..3` to all-up and redirects the remaining false
  `GotBootKey` to `DiskHardMicrocodeBoot`. Current probe facts:
  `DISKHARDMICROCODEBOOT`, `BOOTTRANSFER`, and `DISKMBOOTRET` are hit.
  Synthetic AHT scanline wakeups now drive the terminal task:
  `display outs=35414`, frame 59 snapshot written, and boot keyboard
  words stay `FFFF`. With the current DSK normal-mode shim and the
  Md-at-issue LC timing fix, PilotDisk accepts the IOCB seal and reaches
  `KSameDrive`, `KContinueCmmd`, and `KCheckSeek`. DiskData inputs and
  FIFO reads/writes remain zero. Initial is not yet transferring
  hard-disk boot sectors from the mounted pack; the next blocker is the
  disk sector/status path used by `WaitForSector` and `Read1Muff`.
  The hardware manual and DskEth schematic confirm the drive/controller
  sector timing model: 117 drive subsector pulses/rev, divided by the
  selected drive's `Tag[4:9]+1` subsector count. Drive 0 is initialized
  by PilotDisk/Initial with count 3, so firmware counts 29 sector
  pulses per revolution (`117 / 4`, with the remainder consumed by the
  index interval) even when the attached Alto/Trident pack image stores
  9 media sectors/track.
  The full probe also uses a temporary identity-map shim for the first
  256 pages at `DiskHardMicrocodeBoot`; without it, final map entries
  for the first 64K are still vacant and the CSB/IOCB handoff faults.
  Initial's final `SetMCR[mcr.noWake]` currently decodes as `0xFEE7`;
  the emulator treats that source-level value as normal memory
  references enabled with only fault wakeups suppressed until the full
  active-low MCR/schematic decode is finished.
- Source code verified: BootstrapMain.mc (fetched from CHM
  archives at chm/dorado/expanded/BootstrapSources.dm/) confirms
  that ReadBB returns T = ~CPReg via B←RWCPReg (per HM page 31:
  "B←CPReg'"). The LSH/LDF/XOR address-assembly code in
  BootstrapMain.mc must somewhere undo the inversion — but in our
  model still does not reconstruct a matching image; the remaining
  streaming mismatch is separate from the Initial bring-up blocker.
  **Open investigation in task #58**.

**probe_initial** (added 2026-04): bypasses the BB chain and runs
Initial.MB directly with Bootstrap.MB layered for IM[0o7700-0o7777].
Initial's first instruction at 0o7500 globally calls 0o7700
(READBB), so Initial enters the same spin loop. Confirms Initial
and Bootstrap share an IM region by design — Initial reuses
Bootstrap's CPReg-reading subroutine. **Both depend on the BB
CPReg protocol working** to make any progress.

What's stub-or-missing

> Canonical list lives in `docs/handoff.md` under
> "Known gaps — full punch list" (sections A–K). The summary below is
> the boot-bring-up framing of those same items; keep it short and let
> handoff.md drift.

- **Memory timing/errors:** storage modules no longer alias, but ECC,
  per-slot Pipe4 error fields, deferred refs, and Hold/DisHold
  semantics are missing. Long-running AEmu and Initial paths both
  reach memory/fault waits that likely need these details.
- **I/O bring-up:** Display + Disk stubs exist and fast-I/O transport
  is tested. Initial now issues display and disk slow-I/O in the full
  boot probe. Ethernet is not modeled, and that is now the expected
  visible blocker after the hard-disk boot attempt fails.
- **Bootstrap → Initial handoff in progress**: probe_full_boot_
  with_bootstrap demonstrates the mechanism end-to-end, but the
  streamed Initial image still differs from canonical Initial.MB, so
  the probe restores canonical Initial at BOOTSTAGE2.
- **No keyboard / boot-button → emulator selection** (Mesa/Cedar/
  Lisp/Smalltalk/Alto). This rides on Display back-channel.
- **No Ethernet client** for Path A's Initial-fetches-emulator
  step. Punt to Path B (LoadMB-style direct microcode load) — it's
  a much shorter route to running an emulator.

## Why "match the docs"

The Dorado is small enough that most of the design is fully
specified in the September 1981 Hardware Manual. Two reasons to
follow it closely:

1. **Bug parity with hardware.** The microcode we'll run was debugged
   against the real machine over years. If we deviate in a subtle
   way (e.g., Hold timing, branch-condition latching, ALUFM
   encoding), some microprogram somewhere will exercise the
   difference and we'll spend days finding it.

2. **Phase 2 is RTL.** The C model is the reference for the Verilog
   port. Cycle-accurate behavior in C maps to RTL stages. Skipping
   the Pipe or making memory references atomic will eat that
   margin.

Where the manual is silent (mostly: undocumented arith ops, exact
ECC behavior, IFU prefetch latency under cache miss), we use the
microcode source (chm/dorado/expanded) as oracle and document the
inferred behavior.

## Phase A — Get past Boot0

**Goal:** `probe_full_boot` runs Boot0 to its natural end (it ACKs the
BB by leaving 0x01 in MiscByte), and the BB streams Boot1 into the
running Dorado via CPReg.

**Status:** A.1–A.6 LANDED. A.7 (Boot1 ACK) **deeper than just
AMSync** — investigation completed:

  Boot0 starts at 0o7740 (BOOTSTRAP entry). Trail (5 IM-fetched
  cycles before halt at 0o7744 → long-jump 0o4000):

    0o7740 (BOOTSTRAP)     TaskingOff, →7763 (NOT recorded as
                           IM-fetched; happens before saw_first
                           triggers — possibly held)
    0o7761                 Shift op, →7746 (via FF condition)
    0o7746                 Q-based op, →7707
    0o7707                 Conditional jump on R<0 — R is 0,
                           cond=false → 0o7744 (trap reservation)
    0o7744                 All-zero → long jump to 0o4000 → halt

  **The wall is not just AMSync** — Boot0 reads device data via
  `Pd←Input` (slow IO input at TIOA-selected address). Boot0
  uses these reads to populate ALUFM[1..15] and other state
  *before* it can correctly handle the conditional branches.

  Per BB source (`doradoboot.masm`):
  - BB pre-loads ALUFM[0]=0o25 (logical "B")
  - BB sets CPReg[0]=1 to sync (AMSync wired ✓)
  - BB streams Boot1 via ABMux0/ABMux1 strobes (CPReg-only,
    no MIR re-injection) — wired ✓
  - But Boot0 itself reads slow-IO inputs and may also depend on
    the Hold/TaskSim state set up by BB's "40 NOPs to clear cobwebs"

  Specific gaps identified:
  - `Pd←Input` (FA=0 FB=2 FC=3) and `Pd←InputNoPE` (FC=4) stubbed
    in cpu.c → Boot0 sees Pd=0 at 0o7763, writes 0 to ALUFM[N],
    breaks downstream conditional logic.
  - Slow-IO devices (Hold/TaskSim register, MCR, Reset register)
    need modeling so BB-side TIOA-routed reads return real data.

  **A.7 is the right next session** but requires Phase E (slow
  IO) infrastructure first. The HM §7 ("Slow IO") describes the
  TIOA + IOB bus + Output←B / Pd←Input mechanism — that's the
  prerequisite, not just an AMSync byte-toggle.

The infrastructure that *is* in place: CPReg low/high latching matches
`doradoboot.masm`: ABMux1 writes CPRegL, ABMux0 writes CPRegH, and the
Boot1 data-ready transition is the final ABMux0 write of
`(MicroHalf << 1)|extra_bit` into CPRegH. The BB→Dorado handshake is wired at
the CPReg latch level — what's missing is the parallel slow-IO path that
Boot0 polls in addition to CPReg.

**Why this phase first:** the closest visible milestone. Validates
the whole BB↔Dorado handshake, including the CPReg streaming path
that the BB uses for Boot1 (and Mesa, and everything else).

### A.1 Proper STK addressing (HM Table 6, §3.1)

STK is a 256-entry stack with separate StkP. RSTK[0:3] sub-decodes
into push/pop modes:

| RSTK[0:3] | mode (manual)       | C side                                   |
|-----------|---------------------|------------------------------------------|
| 0wxx      | STK[StkP+wxx]       | offset read; StkP unchanged              |
| 1wxx push | STK[StkP←StkP+wxx], | post-increment-by-`wxx`-bits-extended    |
| ...       | (HM Table 6 spec)   |                                          |

(Fill in once we have the full table in front of us — copy it from
HM page 14 verbatim into the comment in `rm_address`.)

`rm_address` returns one of:
- positive RM index (BLOCK=0)
- positive `STK base + StkP` (BLOCK=1, no push/pop)
- positive `STK base + StkP_modified` (BLOCK=1, with post-update)

Decision point: do we update StkP **before** the read (push) or
**after** (pop)? HM Table 6 specifies which mode is which; we honor
that. The `ModStkPBeforeW` FF (FA=0 FB=2 FC=7) further modifies
this for store addresses.

Tests: `test_stk_push`, `test_stk_pop`, `test_stk_offset` —
synthetic microprograms that exercise each RSTK[0:3] mode and
verify StkP / STK[N] state.

### A.2 Memory subsystem stub

Just enough for Boot0 to not walk into NaN. The simplest path:

1. Historical first stub: **flat 4M-word storage array** in
   `src/memory.c`, no Map/cache/faults. Current code has real Map,
   cache, Pipe, Config', and installed-module bounds; physical
   addresses beyond installed storage fault instead of wrapping.
2. **VA = BR[MemBase] + Mar** where `Mar` comes from A on
   Fetch←/Store← references. BR is 32 entries × 28-bit VAs;
   loaded via `BrLo←A` and `BrHi←A` (FA=1 FB=2 FC=3..4).
3. **Md** holds the result of the most recent Fetch. Read
   `B←Md` returns it.
4. **Store←** writes B to `mem[VA]`.
5. **Hold not modeled.** Memory references are atomic:
   - `Fetch←T` puts T's value through Mar, computes VA, loads
     `mem[VA]` into Md. Available immediately on next instruction.
   - Real hardware holds the engine for ~28 cycles on a miss; we
     skip that for now and document.

Pipe: stub a 16-entry ring that records VA on each reference, so
microcode that reads `Pipe0..5` via FF gets non-zero values back.

Tests: `test_mem_fetch_store`, `test_br_loadhi_loadlo`,
`test_membase_dispatch`. Use synthetic microprograms.

### A.3 Hold modeling — minimum viable

HM §4 ("Hold"): hold is asserted when:
- A reference to Md happens before Md is ready (~3 instructions
  after Fetch unless cache hit on a deferred reference). LC loads of
  `Md` now use the Md latch value present at instruction issue; a
  concurrent `Fetch` updates `Md` for later instructions. PilotDisk's
  IOCB seal check depends on this ordering.
- StkError, Pipe full, FreezeBC, IFU map fault concurrent with
  IFUJump, etc.

For Boot0 we probably don't *need* Hold at all (single task, deferred
references). But the FreezeBC FF function needs to actually hold
branch conditions for one cycle, otherwise R<0 at PC=0o7707 evaluates
post-LC instead of pre-LC.

We've already got the "evaluate next_pc before apply_lc" fix and the
Md-at-issue LC timing fix that cover the common cases seen during
Initial/PilotDisk bring-up. FreezeBC adds a one-cycle latch where the
*previous* instruction's branch conditions are reused. Implement
as `cpu->bc_frozen` flag set by FreezeBC FF, consumed and cleared
in `eval_branch_condition`.

Tests: `test_freezebc` — verifies a known-good freeze pattern.

### A.4 ALUFM ops — fill in the holes

Audit `alu_op` against HM Table 9 page 16. Add missing entries:
`*0o12`, etc., that show up in real microcode. Empirically: write
a small tool that scans every loaded `.MB` for ALUFM data words
and prints the histogram of (carry, op) pairs we've never seen.
Implement until histogram is empty.

### A.5 ALUFM[14] = "NOT A" pre-seed

Currently the shifter ALU index path forces `aluf_idx = 14 + (aluf
& 1)`, expecting ALUFM[14]="NOT A" to complete the
A-bus-low-true → unmasked-shift convention. Real microcode
explicitly initializes ALUFM[14] (typically via Midas at startup);
Boot0 does not. Either:

- (a) Pre-seed ALUFM[14]="NOT A" (= 0o01) at `dorado_microcode_load`
  time, behind a flag set when we know it's missing; OR
- (b) Detect undefined ALUFM and treat it as identity, document
  that real boot would have set it explicitly.

Option (a) matches the "Midas initializes the machine" assumption
the firmware relies on. We model that as a default.

### A.6 Pipe stub for FaultInfo / EventCnt

Several FF functions return `Pipe`, `FaultInfo`, `EventCnt`, etc.
on B. We stub them as 0 today; that's fine for boot, but the moment
microcode reads a value and acts on it (e.g., "if FaultInfo & X
then Y") we'll diverge. For Boot0 specifically, audit which Pipe
reads it does and stub them with sensible defaults:

- `B←FaultInfo'` → 0xFFFF (no faults; the prime means
  active-low, so 1's = "no fault").
- `B←Pipe0..5` → 0 (no pending references, the empty pipe).
- `B←Config'` → 0xFFFF (default config).
- `B←DBuf` → 0 (no last-store data).

Tests: same as A.2.

### A.7 Boot1 streaming verification

Once A.1–A.6 land, expect Boot0 to:
1. Set up its own state from the initial CPReg (= 0x80,0).
2. Start polling CPReg for incoming Boot1 microinstructions.
3. Write each into IM via Write IM.
4. After receiving the checksum block, leave 0x01 in MiscByte ("ACK").

Update `probe_full_boot` to detect the ACK and verify some of the
Boot1-loaded IM entries are non-empty.

**Exit criterion for Phase A:** probe_full_boot reports "ACK
received, Boot1 loaded, %d IM entries written".

## Phase B — Real memory subsystem

**Goal:** Mesa.mb's first hundred microinstructions execute correctly,
including the Map setup that PIlot does on startup.

This is HM §5 in full. The simplest order:

### B.1 BR (Base Registers)  ✓ LANDED

32 × 28-bit registers. Loaded via:
- `BrLo←A` (FA=1 FB=2 FC=3): BR[MemBase][16:31] ← A[0:15].
- `BrHi←A` (FA=1 FB=2 FC=4): BR[MemBase][4:15] ← A[4:15].

Read by VA computation: `VA = BR[MemBase] + Mar` in cpu.c memory-ref
dispatch.

Implemented in `dorado/src/memory.c::dorado_br_lo_load`,
`dorado_br_hi_load`, `dorado_br_get`. BR is shared across tasks (only
MemBase is per-task).

Tests: `test_br_load` in `tests/test_memory.c` covers the load/read
paths and per-MemBase isolation.

### B.2 Map  ✓ LANDED

16K-entry table indexed by VA[10:23] (we picked the 16K-IC × 256-
word-page configuration from HM Table 16). Entry holds 16-bit RP +
WP/Dirty/Ref. Vacant = WP=1 ∧ Dirty=1 (initial state at power-up).

Implemented in `dorado/src/memory.c::va_translate` and the `Map←`
arm of `dorado_memory_ref`. `Map←B` writes RP from B[0:15] and
WP/Dirty from TIOA[0:1]; Ref is zeroed (HM page 46). A Vacant
entry page-faults on any access; a WP entry faults on Store/IOStore.

The fault is surfaced as a `dorado_fault_kind` return value (plus
`mem.last_fault`/`last_fault_va` for inspection) — the actual trap
to *0-3 (HM Table 14) is wired in cpu.c, **TBD** for Phase B.5.

Map fault microcode trap targets are documented in HM Table 14;
those lookups are part of Phase B.5 (faults).

Tests (in `tests/test_memory.c`): `test_map_vacant_page_fault`,
`test_map_write_protect`, `test_map_load`, `test_map_translation`,
`test_no_fault_refs`. All passing.

See `docs/memory-architecture.md` § "The Map" for the full
reference (Table 16 configurations, entry format, Map← semantics,
cache/Map interaction, hold conditions).

### B.3 Cache  ✓ LANDED

4096-word cache organized as 64 rows × 4 ways × 16-word lines.
The cache holds **virtual** addresses (VA[10:27] is the tag; VA[4:9]
is the row index; VA[0:3] is the word offset). Replacement: 4-way
LRU per row.

Reference dispatch in `dorado_memory_ref`:
- **Fetch / IFetch / LongFetch:** lookup → hit returns line[offset]
  with no Map flag update; miss translates, picks LRU victim
  (writeback if dirty, sets Map.Ref+Dirty), fills, returns.
- **Store:** WP-checks via translate. Write-allocate on miss.
  Marks line dirty. **Map.Dirty stays clean** until the munch is
  later evicted or `Flush←`'d (HM page 45).
- **PreFetch:** silent fill on legal page; never faults.
- **Flush:** clean hit invalidates; dirty hit writes back (sets
  Map.Ref AND Map.Dirty) then invalidates; clean miss is no-op.
- **IOFetch / IOStore:** bypass cache, sets Map flags. IOStore
  unconditionally invalidates the cached line (without writeback).

Tests in `tests/test_memory.c`: `test_cache_hit`,
`test_cache_miss_fill`, `test_cache_store_no_map_dirty`,
`test_cache_lru_eviction`, `test_cache_dirty_victim_writeback`,
`test_cache_flush_clean`, `test_iostore_cache_invalidate`,
`test_cflags_load_visible_in_pipe5`,
`test_pipe5_reports_victim_and_nextvictim`,
`test_mcr_dvavic_reads_cache_address_without_storage`.

**Still TBD** (will land alongside Hold/timing in Phase C):
- 28-cycle miss latency (Hold modeling).
- Cache parity, ECC over munches.
- Exact VNV update RAM behavior. Pipe5 now reports Victim/NextVictim,
  and `UseMcrV` overrides both, but normal replacement still derives
  from the emulator's LRU list instead of the hardware's separate VNV
  update equations.

See `docs/memory-architecture.md § "The cache"` for the full
reference.

### B.4 Pipe  ✓ LANDED (basic VA tracking)

16-entry ring buffer of pending storage references. Each entry
records VA + kind (Fetch/Store/Map/IFetch/IOFetch/IOStore/Flush).
`pipe_push` runs on every reference (including ones that fault) so
fault microcode can recover the VA from `Pipe0`/`Pipe1`.

Read by microcode via `B←Pipe0..5` FF functions (FA=1 FB=6 FC=0..5).
The C side has `dorado_pipe_va(mem, n)` returning slot relative to
head (0 = most recent).

`Pipe3'`/`Map'` old-RP snapshots, `Pipe4'`/`Errors'` Ref plus old
WP/Dirty flags, MapBufBusy in `Pipe5`, cache flags in `Pipe5[8:11]`,
and Victim/NextVictim in `Pipe5[12:15]` are modeled enough for current
bring-up probes.

**Still TBD:** error syndrome details and IFU-ref tracking in Pipe5.
These need to land before Pipe-driven fault recovery can work in
microcode.

Tests (in `tests/test_memory.c`): `test_pipe_records`,
`test_pipe_wraps`, `test_map_vacant_page_fault` (verifies fault
still pushes). All passing.

### B.5 Faults  ✓ PARTIALLY LANDED

**Subtle correction to the plan:** Table 14 trap addresses (`*0-3`,
`*4-7`, `*34-37`, `*74-77`) are **IFU** traps — they fire on
IFU-side faults (map fault during opcode fetch, IFUM parity error,
"IFU not ready"). They are dispatched on IFUJump *after* the IFU
itself faults.

**Processor** memory faults (Fetch/Store/IOFetch/IOStore through
the Map) wake **task 15** (the fault task). Task 15 reads
`Pipe0/Pipe1` for the VA, `Pipe3'`/`Map'` for the pre-ref real page,
`Pipe4'`/`Errors'` for Ref/WP/Dirty and error status, and `FaultInfo'`
for the count + first-fault SRN.

So the right Phase B.5 split:

**B.5a ✓ LANDED — fault state visibility for microcode:**
- `dorado_memory.fault_count`, `fault_first_srn`, `fault_emulator`
- `dorado_fault_info(mem)` returns the high-true 16-bit register
  (B[7]=EmulatorFault, B[8:11]=SRN, B[12:15]=NFaults)
- `dorado_fault_clear(mem)` resets fault state
- `dorado_pipe_map_rp_at(mem, n)` returns the old RP snapshot for any
  pipe slot; `dorado_pipe_map_flags(mem, n)` keeps the old WP/Dirty/Ref
  snapshot used by `Pipe4'`
- cpu.c FF override:
  - `B←FaultInfo'` (FA=1 FB=6 FC=0) returns inverted FaultInfo
  - `B←Pipe2'` (FA=1 FB=6 FC=3) returns the same (HM page 51:
    "Pipe2' is simply a convenient decode for [FaultInfo]")
  - `B←Pipe3'` (FA=1 FB=6 FC=4) returns inverted old-RP snapshot
- Tests: `test_fault_info`, `test_pipe3_map_entry_snapshot` in
  `tests/test_memory.c`; `test_cpu_fault_info_visible` in
  `tests/test_cpu.c` (full FF dispatch path).

**B.5b — task-15 wakeup on processor fault:** TBD until tasking
(Phase D) lands. Currently a fault on a processor reference is
silently recorded and the microengine continues with stale Md.

**B.5c — IFU trap to *0-3 (Table 14):** TBD until IFU (Phase C)
lands. Phase C.3 (IFUJump) needs the trap-vector dispatch hook.

**B.5d ✓ LANDED — ProcSRN/ASRN split:**
- `dorado_memory.proc_srn` (4-bit, default 0): selects pipe slot
  for task-0/15 non-prefetch refs and PreFetch-with-hit.
- `dorado_memory.asrn` (4-bit, default 2; ring 2..15): selects pipe
  slot for IOFetch/IOStore and PreFetch-with-miss; advances after
  the ref.
- `ProcSRN←B` FF function (FA=1 FB=2 FC=7) wired in cpu.c via
  `dorado_proc_srn_set(mem, b & 0xF)`.
- `B←Pipei` reads in cpu.c now use `pipe[mem.proc_srn]` instead of
  "most recent" semantics — matches HM page 51 microcode pattern.
- Tests: `test_proc_srn_overwrite`, `test_prefetch_srn_split`,
  updated `test_pipe_records` and `test_pipe_wraps` to use IOFetch
  through the ASRN ring.

See `docs/memory-architecture.md § "The Pipe"` for the full reference.

### B.6 ECC

Hamming SEC/DED on storage words (HM §5.7). Per-word ECC bits in
the storage memory. SyndromeBits visible to microcode for testing
purposes (LoadTestSyndrome FF).

For initial bring-up: implement the generation/check but don't
inject errors. Tests: `test_ecc_clean_roundtrip`,
`test_ecc_single_bit_correct`, `test_ecc_double_bit_detect`.

### B.7 Storage size + I/O paths

256 K × 16-word in 1–4 modules per HM §5. Fast I/O munches (256-bit
chunks via Fin/Fout) for Disk/Ethernet/Display DMA. We can't fully
exercise this until the I/O devices land in Phase E, but the
memory side should provide the hooks.

**Exit criterion for Phase B:** Mesa.mb runs the first 1000
microinstructions without halt. Validate by snapshotting cpu state
and IFUM/Map state every 100 instructions; cross-reference against
salto's behavior on the same .mb if salto can run it (it can't, so
this is a "no halt" milestone).

## Phase C — IFU (Instruction Fetch Unit)

**Goal:** Alto-emulator microcode (AEmu.mb) decodes Alto opcodes
and the emulator runs.

HM §6. Three pieces:

### C.1 IFUM — 1024 × 24-bit decode RAM  ✓ LANDED (load/read)

Indexed by `(InsSet[0:1] || Opcode[0:7])` = 10 bits → 1024 entries.
Each entry is 24-bit (+3 parity), stored as two 16-bit halves
(`mc->ifum_lo`, `mc->ifum_hi`).

Per HM Table 18: Length' (2), TPause' (1), TJump' (1), IFaddr' (10),
RBaseB' (1), MemB (3), Sign (1), Packed-α (1), N (4) = 24 bits.

FF functions wired in cpu.c:
- **`InsSetorEvent←B`** (FA=1 FB=3 FC=0). If B[0]=1, B[6:7]→InsSet[0:1].
- **`BrkIns←B`** (FA=1 FB=3 FC=7). Opcode ← B[0:7].
- **`IFUMRH←B`** (FA=1 FB=3 FC=4). Writes ifum_lo[InsSet||Opcode].
- **`IFUMLH←B`** (FA=1 FB=3 FC=5). Writes ifum_hi[…].
- **`IFUReset`** (FA=1 FB=3 FC=6). Resets InsSet/Opcode.
- **`B←IFUMRH'`** (FA=1 FB=7 FC=2). Reads ~ifum_lo[…].
- **`B←IFUMLH'`** (FA=1 FB=7 FC=3). Reads ~ifum_hi[…].

Test: `test_ifum_load_read` in `tests/test_cpu.c` round-trips an
entry via the full FF protocol.

**TBD (Phase C.2/C.3):** decoded field access during prefetch,
the IFU pipeline (F/G → J → H → M levels), F/G byte ordering
across instruction sets (HM page 64 "Alto compatibility kludge").

### C.2 Instruction prefetch  ✓ FUNCTIONAL + WARMUP

Functional model with cycle-accurate warmup (multi-stage F/G→J→H→M
pipeline is the Verilog target; for the C model we approximate the
observable behavior with a 5-cycle warmup counter per HM page 67):

- `PCF←B` (FA=1 FB=0 FC=0) sets the byte cursor, arms the IFU,
  loads `ifu_warmup = 5`.
- Each CPU step decrements `ifu_warmup` while the IFU is active.
- `ifu_fetch_byte(cpu, pc, *out_faulted)` fetches via
  `dorado_memory_ref(IFETCH)`, using BR[31] as the codebase. Sets
  0/1: byte 0 = high byte; sets 2/3 reverse (not yet exercised).
  Reports map-fault back to caller for trap dispatch.
- IFUJump reads the opcode at PCF, looks up `IFUM[InsSet||opcode]`,
  decodes per Table 18, advances PCF by Length bytes. Operand
  bytes (α, β) are captured into `cpu->ifu_alpha/beta` for later
  ←Id delivery.
- MemBase + RBase are reinitialized at IFUJump per the IFUM
  entry's MemB[0:2] and RBaseB' fields (HM page 65 t0 init).

**Still TBD (Phase C.2 polish):**
- Explicit F/G/J/H/M pipeline stages (Verilog port will need this).
- Cache miss / Hold during prefetch.
- The Alto byte-ordering kludge for sets 2/3 (HM page 64).
- IFUM parity errors (*74-77).
- IFU data parity errors (*4-7).
- Reschedule trap (*14-17) on the second/third successful IFUJump
  after Reschedule FF.

### C.3 IFUJump  ✓ COMPLETE (functional)

JCN encoding `0 0 1 _ _ 1 1 1` with `_ _` = entry-vector slot
n (0..3). Computes TNIA = (IFaddr' << 2) | n in our 12-bit
microstore. Loads Link with CIA+1.

**Conditional IFUJump (HM page 33):** When the FF field encodes a
branch condition (FA=0 FB=6 FC=0..6) AND the condition is true:
- Dispatch goes to entry n|1 of the M-level vector (TNIA[15] OR'd).
- IFU does NOT advance — PCF stays put. The next IFUJump
  re-dispatches the same opcode.

**Trap dispatch:** Computed via `ifu_trap_addr(base, n_slot, insset)`
which OR's `~InsSet[0:1]` into bits 6:7 of the trap address per HM
Table 14 footnote ("actual trap locations for Reschedule, for
example, are 14-17, 114-117, 214-217, and 314-317").
- **NotReady (*34-37)** ✓ — when `ifu_warmup > 0` at IFUJump time.
- **IFU map fault (*0-3)** ✓ — when ifu_fetch_byte gets DM_FAULT
  back from the cache/Map.
- **IFU data parity (*4-7)** — TBD (no parity model yet).
- **IFUM parity (*74-77)** — TBD (no parity model yet).
- **Reschedule (*14-17)** ✓ — `Reschedule` FF (FA=1 FB=3 FC=2)
  arms a flipflop with count=2 (next-or-following IFUJump traps).
  `RescheduleNow` (FA=1 FB=0 FC=3) arms with count=1 (next IFUJump
  traps). `NoReschedule` (FA=1 FB=3 FC=3) clears it. The IOAtten'/
  Reschedule branch condition (cond=6) reads true when count >= 2
  for the emulator (per HM Table 20).

Tests in `tests/test_cpu.c`:
- `test_ifu_dispatch_synthetic` — INC×4 + HALT bytecode dispatch
- `test_ifu_conditional_dispatch` — cond=false advance path
- `test_ifu_conditional_cond_true` — cond=true PCF-hold path
- `test_ifu_notready_trap` — *34-37 trap with InsSet OR'd
- `test_ifu_map_fault_trap` — *0-3 trap on Vacant page
- `test_reschedule_trap` — *14-17 trap from RescheduleNow FF

### C.4 Operand delivery (←Id)  ✓ MINIMAL LANDED

`A←Id` (ASEL=5) and `TIsId`/`RIsId` (FA=0 FB=3 FC=4/5) call
`ifu_consume_id` to deliver the next operand byte:
- N (if N != 17₈) first
- α (or split nibbles if Packed-α=1)
- β (length=3 only)
- Then `Length` forever (used by jump-fallthrough calculations)

Tests:
- `test_ifum_load_read` — IFUM RAM round-trip via the FF protocol
- `test_ifu_dispatch_synthetic` — synthetic 2-opcode instruction
  set (INC, HALT) dispatched through IFUJump; bytecode "10 10 10
  10 20" runs INC×4 then HALT, T ends at 4.

**Exit criterion for Phase C:** AEmu.mb executes the first 100
Alto opcodes from a games.dsk image (Alto-emulator-on-Dorado).
Currently: synthetic IFU pipeline runs, but real microcode bring-
up needs C.2/C.3 polish (NotReady, conditional IFUJump, traps).

### Boot-bypass probe — `probe_aemu` in test_cpu.c

Layer-loads `Initial.mb` + `kernel.mb` + `memMisc.mb` +
`IfuComplex.mb` + `AEmu.mb!2` (14,099 microinstructions across the
4K IM real-address space, with later layers refining earlier
ones), mimics BB's `PrepareProcessor` setup (tasking off, IFU
inactive, no pending wakeups), mounts the first 16 map pages
identity-RW, and runs from candidate entry points (BOOTEMULATOR
→ STARTEMULATOR → AEmu's START in priority order).

**Current result:** Engine runs the full 200,000-cycle budget
through real Xerox PARC microcode without halting. Entry trail:

  STARTEMULATOR (0o1133) → RESUMEEMULATOR (0o1135) → 0o1136 →
  SETUPBRS (0o1140) → DOBRS×N (BR setup loop) → ... →
  LRTYPETABLE / LRTYPEIM / LRLOOPTOFF / LRNOPREFNEXT / TOFFRET
  (loops forever)

**The wall:** AEmu's `LRTYPETABLE` loop reads a TYPE TABLE from
**main memory** via `Fetch←RM/STK`. In real boot, Initial would
have loaded this table data from disk or Ethernet into main
memory before transferring to the emulator. The boot-bypass path
can't replicate that — direct .MB layer-load only populates IM,
RM, ALUFM, IFUM (the microcode-internal state), not main-memory
data tables.

**Three possible paths forward:**
  1. **Fix Phase A.7 AMSync** so the real BB→Boot0→Boot1→Initial
     chain runs faithfully and Initial loads main memory. This
     is the canonical path; needs the CPReg sync handshake
     completed.
  2. **Reverse-engineer the table layout** AEmu's LRTYPETABLE
     expects, plant it in main memory before running. Tractable
     but requires reading the source / disassembly carefully.
  3. **Use a synthetic emulator microcode** (already have the
     pattern in `test_ifu_dispatch_synthetic`) and skip real
     PARC-microcode bring-up. Validates engine semantics but
     doesn't run real software.

**Engine state:** the architectural skeleton is sound. 14k
instructions of original microcode execute through Initial's
kernel-init, IFUM-load, and emulator-dispatch sequences without
hitting an "unsupported" halt. The remaining gap is data-driven,
not engine-driven.

## Phase D — Tasking

**Goal:** I/O devices in Phase E can wake their tasks and the
microcode for them gets dispatched.

HM §4.1, §4.2, Table 22.

### D.1 Per-task state replication  ✓ LANDED

`task_t[16]`, `task_tpc[16]`, `task_link[16]`, `task_membase[16]`,
`task_rbase[16]`, and `task_tioa[16]` on `dorado_cpu`.
Saved/restored by `task_save`/`task_load` in cpu.c. Q, ALUFM, StkP,
ShC, and Cnt are not per-task (HM §4.1; the manual names RBase with
T, MemBase, and TIOA as task-specific).

2026-05-01 note: the EB direct-load probe exposed this. AUT task 1 set
`RBase=4`, then BLOCKed while task 0 resumed in `Start.mc`; without
per-task RBase, `BrHi_ EmuBRHiReg` read `EORegs.VirtualBanks`
(`RM[0x48]`) instead of `AEmRegs.EmuBRHiReg` (`RM[0x18]`), corrupting
`BR37` from `0x20006` to `0x400006`.

### D.2 Wakeup latches + priority encoder  ✓ LANDED

`wakeup_pending` (bitmask of pending wakeups) and `ready` (bitmask
of runnable tasks). Task 0 always Ready (HM page 26: "task 0
always awake"). BNT = highest bit set in (ready | wakeup_pending).
`dorado_cpu_wakeup(cpu, task)` for tests; `Wakeup[task]` FF
function (FA=3 FB=6-7) for microcode.

### D.3 Task-switch sequencing  ✓ LANDED

`task_schedule()` runs at end of every microinstruction. Switches
iff (BNT > CTASK) OR (BLOCK=1 in non-emulator). On switch, save
T/TPC/Link/MemBase/RBase/TIOA to current task slot, load new task's
slot.

**TBD:** Hold/tasking interaction (Hold isn't modeled).

### D.4 TaskingOff/On  ✓ LANDED

`tasking_on` flag. TaskingOff (FA=1 FB=4 FC=2) sets it to 0
atomically. TaskingOn (FA=1 FB=4 FC=3) schedules re-enable after
2 more instructions (HM page 27: "at least two more instructions
will be executed by the same task").

### D.5 LdTPC / RdTPC  ✓ LANDED

JCN-based (return-class encoding `0 1 f f f 1 1 1`):
- fn=4: **RdTPC←B** — task = B[12:15]; reads task_tpc[task] into
  Link, 1's complemented (HM page 34 protocol).
- fn=5: **LdTPC←B** — task = B[12:15]; writes task_tpc[task] from
  Link[at-issue]. No-op if writing the running task's own TPC.

Test: `test_ldtpc_rdtpc` in tests/test_cpu.c.

### D.7 SubTask  ✓ LANDED (HM page 88)

I/O devices may present a 2-bit SubTask along with a wakeup,
selecting one of 4 sub-regions of RM and 4 BR pairs for the
task. The processor OR's SubTask[0:1] into:
- **RBase[2:3]** (= low 2 bits of 4-bit RBase) at register reads
- **MemBase[2:3]** (= LSB bits 2:1 of 5-bit MemBase) at memory ref

Per-task `task_subtask[16]` field; only effective for non-emulator
tasks. `dorado_cpu_set_subtask(cpu, task, subtask)` API for tests
and I/O device modeling.

Test: `test_subtask_or_rm` — task 5 with SubTask=1 reads RM[16]
(via RBase | 1) instead of RM[0].

### D.6 IOAtten' / Reschedule conditions  ✓ PARTIAL

Branch condition 6 (FF=0o66) — current implementation:
- **Emulator (task 0):** returns 1 (no Reschedule pending). The
  Reschedule flipflop set by `Reschedule` FF function is TBD.
- **Non-emulator (tasks 1..15):** returns `(wakeup_pending & (1<<ctask))`
  inverted — i.e., active-low. So if the task has a pending
  wakeup not yet acknowledged, the condition reads 0 (true after
  inversion).

Until I/O devices drive real wakeups (Phase E), this is a
reasonable proxy: tasks woken via `dorado_cpu_wakeup` or
`Wakeup[task]` FF will see IOAtten' true.

Tests in `tests/test_cpu.c`:
- `test_task_switch_on_wakeup` — Wakeup → state save/load → switch
- `test_task_block_returns_to_emulator` — BLOCK=1 in non-emulator
  clears Ready, returns to task 0
- `test_tasking_off_blocks_switch` — TaskingOff stops switching
- `test_wakeup_ff_function` — Wakeup[7] FF dispatches to task 7

**Exit criterion for Phase D:** ✓ core mechanism in place. LdTPC,
RdTPC, IOAtten' to follow when actually needed by microcode.

## Phase E — I/O devices

**Goal:** Boot from disk.

### E.1 Display (DispM monochrome)

Easiest first. HM §11. Framebuffer of 808×606 monochrome bits
streamed via Fout (256-bit munches at the dot rate).

We don't need a real GUI — `dpsub` (display sub-task) writes
into an mmap'd file or stdout that we can render with a tiny
viewer.

Bring up before Disk: gives us "hello world" output without
requiring disk imagery.

### E.2 Disk (T-80 SMD)

Trident T-80 controller. HM §9. Command/Status/SectorAddress
registers; sector buffer of 256 16-bit words. Microcode-driven
sector transfers via Fin/Fout munches.

ContrAlto2 has a Trident emulation in C# that we can lift the
state machine from (port to C, document the differences). The
disk image format is the standard Trident sector layout; same
images that boot Alto on ContrAlto.

Critical for booting Mesa: Pilot reads its kernel from sector 0
of the boot pack.

### E.3 Ethernet (3 Mb/s Alto-style)

Optional for booting (you'd boot from disk first). Useful for
network-boot (BootMesa from server). HM §10. We can defer until
disk works.

**Exit criterion for Phase E:** Mesa.mb reads sector 0 of a Pilot
boot disk and the bootloader's first instruction from disk
executes.

## Phase F — Bring up real microcode

**Goal:** A Pilot kernel image runs in the emulator and outputs
*something* on the display.

### F.1 Alto-emulator-on-Dorado (AEmu.mb)

Smallest microcode (40 KB). Cross-validate against ContrAlto by
running games.dsk on both and diffing architectural state every
N opcodes. Establishes our architecture is right.

### F.2 Mesa (Mesa.mb)

The big one. 100 KB+ of microcode. Boots Pilot from a Trident
pack. Validation: compare microengine state to a checkpoint
captured from real-Dorado documentation (or Midas dumps if we
can find any).

### F.3 Cedar / Smalltalk / Lisp

Once Mesa works, the others should be relatively easy because
they share most of the architecture and just load different
microcode.

## Cross-cutting: testing strategy

### Current disk bring-up checkpoint

- Known shortcomings are now tracked in
  `docs/disk-architecture.md` and `docs/memory-architecture.md`.
  Disk priority order: split leading/trailing sector timing, add a
  Format-RAM/sequence-PROM read path, then tighten FIFO thresholds and
  KSTAT/ECC. Memory priority order: add observable Hold/ready-cycle
  semantics, then ADDRESS/MAP.3/FastOutBus conflict modeling from
  `DoradoB-WEverything.dm!1_`.
- Disk controller sector advance now asserts `IndexTW` at sector wrap,
  asserts `SectorTW` with the index pulse, and honors
  `BlockTillIndex` by masking newly generated non-index sector wakeups
  until the index pulse clears the latch.
- The full-boot probe advances the synthetic spindle on every service
  call while `BlockTillIndex` or a seek is pending. This is probe timing
  only; it keeps the model from missing Initial's short `BootTransfer`
  timeout while still preserving the controller latch behavior.
- Current boot trace reaches `KSameDrive`, `KContinueCmmd`,
  `KCheckSeek`, `SendTagWait`, and the `Read20Muffs` status scan, but
  still does not issue `DiskData` FIFO reads. Fixed items on this path:
  CPU `TIOA[DiskTag]` (`014`) decode, native DiskTag strobe decode,
  native DiskMuff low-byte address/high-byte clear handling,
  right-justified DiskMuff readback for `Read1Muff`, active-low KSTATE
  block-mode readout, delayed `NotReady`/`TagTW` for Cylinder/ReZero,
  and CPU `BLOCK` semantics for non-emulator tasks.
- The latest focused 30M-cycle probe shows `SendTagWait` now does the
  expected `KTemp0_ muffSeekTagTW, Block, Call[Read1Muff]` sequence:
  `KTemp0` becomes `0003`, DiskMuff selects address `003`, and
  `Read1Muff` returns `0001` for `R odd`. The task then reaches the
  disk status/error path (`Read20Muffs`, real PCs `6600..6627`) before
  any DiskControl read command or `DiskData` access. Next debugging
  target: why PilotDisk is entering that status path with no FIFO
  transfer active.
- The full-boot disk probe now uses a configurable compressed spindle
  period, `DORADO_DISK_SECTOR_PERIOD` (default 512 Dorado cycles), so
  the DSK task can see a sector wakeup before Initial's short
  `BootTransferTimeout`. At the older 2000-cycle probe period,
  `BootTransfer` could time out before the next synthetic sector.
- The latest focused 120M-cycle probe reaches `KSameDrive` and
  `KCheckSeek` before the first `BootTransferTimeout`, later reaches
  `SeekAndWaitForReady`/`KNoRestore`; after the non-emulator `BLOCK`
  fix the 30M focused trace reaches `Read20Muffs` status scanning. It
  still has no FIFO reads/writes. `InitialDisk.mc` states that hard disk
  microcode is a private Dorado convention starting at page 4
  (cylinder 0/head 0/sector 4); the mounted `spruce-server.dsk300` is
  an Alto Spruce T-300 pack and is not known to contain that private
  Initial hard-microcode boot file. Treat it as a Trident-controller
  validation pack, not as proof that Initial disk boot should succeed.
- `DORADO_ETHER_BOOT_IMAGE=<path>` now enables a probe-only Ethernet
  boot injector. It copies the EB payload after the first 512-byte
  overhead page into Initial's `BootDataPtr` area, preserving the same
  zero-sum checksum that `CheckChecksumAndLoad` expects, then lets
  Initial run through `LoadRam`. With
  `../chm/microcode/AltoMesaDorado.eb!1`, the injected payload ends at
  `0x44E4` (printed as the wrapped 16-bit pointer) and checksums to
  zero; the probe reaches loaded-image runtime code around
  `PC=0o6000/0o6002/0o6012`. A sampled IM comparison against known
  repository `.mb` files reports `0/6` for `Mesa.mb!3`,
  `TriMesa.mb!3`, `Cedar.mb!6`, `DSemu.mb!1`, and the UnBug Mesa
  image (AEmu has no sampled addresses present), so this EB payload is
  not byte-for-byte any of those known `.mb` binaries; treat it as its
  own AltoMesa LoadRam image.
  The current post-load blocker is runtime display scheduling rather
  than the EB checksum/LoadRam path. After making the probe clocks more
  hardware-like (disk sector service and display scanline timing are
  no longer gated on prior slow-IO writes), the focused EB run reports:
  `loaded-world cycle=24704942`, post-LoadRam task cycles
  `[0]=8882891 [2]=28608 [4]=422755 [14]=201554`,
  `switches=106789`, `ready_or=0x1015`, `wakeup_or=0x1014`,
  display slow-IO writes `+35410`, disk slow-IO writes `+36150`, but
  `display iofetch=0`. A follow-up DHT clock experiment wakes task 3,
  changing post-load counts to include `[3]=35485`, but task 3's hot
  PC is only `0o6006` and all display slow-IO writes still come from
  AHT/task 4. DWT task 13 is never woken. So tasking is alive, but the
  loaded emulator has not initialized the display task PCs yet. The
  immediate blocker is still task-0 startup around
  `0o6000/0o6002/0o6012`, where MCR values such as `0x78E1`,
  `0x7861`, `0x70E1`, and `0x6861` are repeatedly loaded. Re-check
  MCR active-low/source-level decode and cache/reference semantics
  before expecting DHT/DWT to render.
  The loaded IM samples currently printed by the probe are
  `0o6000=00104/71501/00000`, `0o6001=00104/131705/140000`,
  `0o6002=00104/14701/00000`, `0o6012=13116/14105/00000`,
  `0o6100=00204/60005/00000`, `0o5021=05406/77714/40000`.

2026-05-01 loaded-world disk trace update:
- `DORADO_DISK_TRACE=1` now resets its ring at direct EB injection and
  captures task-0 plus task-14 disk-code ranges, so the trace is no
  longer filled by Initial's earlier disk attempt or by display task PC
  overlaps.
- With `AltoMesaDorado.eb!1`, task 14 reaches the read path with
  `KCmmd=0005` and then branches through `KReadBadTW`/`Read20Muffs`.
  The controller state at that point is `EnableRun=1`, `Active=0`,
  `RdFifoTW=0`, and `DiskControl=0100`.
- In the same focused window, the only `DiskControl` outputs are
  `0400`/`0100` setup values; no `O10:0005` command load is observed
  before the first `Read1Muff(muffRdFifoTW)`. The next fix should
  explain why the `KCmmdInTime` command output is missing or why the
  command load is encoded in a CPU output form we still do not route.
- Follow-up bring-up patch: when task 14 polls `muffRdFifoTW` with
  `EnableRun=1`, no active transfer, and a mounted pack, `disk.c` now
  starts a current-sector read stream. This is a constrained substitute
  for the missing DiskControl/Format-RAM sequence-PROM start timing; it
  should be removed once the real leading-edge command sequencer exists.
- DiskTag decode now follows the native strobe bits from
  `DiskDefs.mc`: `0x8000` Drive, `0x4000` Cylinder, `0x2000` Head, and
  `0x1000` Control. Unstrobed preload/idle values such as `0xFFEF` are
  ignored. The next disk gap is the read/check FIFO status sequence:
  `RdFifoTW` thresholds, block-mode status, ECC words, and end-of-block
  `ReadErr`/`WriteErr` summary bits. Offline drive selects are clamped
  during single-pack bring-up so the mounted boot pack remains selected
  until the missing offline-drive KSTAT behavior is modeled.

Three styles of test, used at every phase:

1. **Synthetic microprograms.** Hand-written `dorado_uinstr`
   sequences for unit-testing one feature (we have these for ALU,
   shift, etc.). Cheap, fast, precise.

2. **Real-microcode regression.** Run actual `.mb` files (Bootstrap,
   Boot0, Boot1, Mesa, …) through the engine as-is. Halt on first
   unsupported feature; the halt reason names the gap. The current
   `probe_full_boot` is this; we'll add more as phases land.

3. **Cross-validation.** When ContrAlto2 can run the same workload
   (Alto emulator microcode running an Alto disk), diff
   architectural state. The Alto side is the only place this is
   feasible; for Mesa we don't have an oracle.

## Latest bring-up notes

- `InitialSubrs.mc` `ClearCacheFlags` relies on `NoRef+UseMcrV`
  stores as cache-address writes (`CacheA[VA, selected column] <- VA`).
  The emulator now models that side effect and covers it with
  `test_mcr_noref_store_writes_cache_address`.
- Follow-up cache fix: `CFlags<-A'` `Vacant` entries are now excluded
  from cache-data lookup while still being readable through
  `dVA<-Victim`. This matches Initial's "write CacheA, then mark
  Vacant" sweep and is covered by
  `test_vacant_cache_address_is_not_cache_hit`.
- HM §9 says DiskMuff input returns the selected signal on `IOB[15]`.
  In the emulator's C bit layout, Dorado bit 15 is represented as
  `0x0001`; `DiskSubrs.mc::Read1Muff` explicitly returns 0/1 and tests
  `R odd`. Disk unit tests now pin this right-justified convention.
- Focused EB run with `AltoMesaDorado.eb!1` now moves farther after
  the cache `Vacant` fix. With `DORADO_BOOT_BUDGET=120000000`, it
  reaches LoadRam and the loaded image, produces a headless display
  snapshot (`frame=180`), and task 4 is active around terminal/read
  PCs (`0o6300/0o6301/0o6311/0o6721/0o6744`). It still has
  `display iofetch=0`, DWT wakeups `0`, disk FIFO reads/writes `0`,
  and task 0 remains hot in `SetMCR`/`LongWait` startup code. Next
  likely gaps are display-task wake/PC initialization and disk
  tag/format-RAM transfer start.
- `_cd8_/doradomicrocode/` is not fully mirrored locally, but a focused
  subset now exists under `chm/doradomicrocode/doradomicrocodesources/`
  (`ADefs`, `DisplayDefs`, `DisplayMain`, `DisplayAux`, `LoadRam`,
  `NewMemory`, `PilotDisk`, `PilotDiskDefs`, and build recipes). These
  sources corrected the display diagnosis: task 4 is the terminal
  horizontal path, but the latest 120M-cycle trace shows display outputs
  only at TIOA `0366` (`TNLCB`) and `0367` (`TStatics`), never `0364`
  (`AHTFlag`). `DAStart` low-core words `0420..0427` are all zero, so
  the current blank screen is because no display control block chain has
  been installed yet, not because the WCB flag shim alone was missing.
- 2026-04-30 follow-up: the focused EB probe now records the EB End
  item start address (`AltoMesaDorado.eb!1` reports `start=0o1076`)
  and no longer treats Initial's `0o6046` Ethernet wait loop as the
  loaded world. A temporary probe-side direct LoadRam parser can install
  EB IM/IFUM/RM items without relying on the unfinished Ethernet packet
  path. That direct run reaches real Alto/Mesa initialization/display/
  disk PCs (`0o1077`, `0o4656`, `0o5624`, `0o6744`, etc.) but halts in
  task `0o14` at `PC=0o6`, an IFUJump exit with `IFU active=0`.
  Instrumentation shows `PCF<-B` never armed the IFU in that run
  (`Post-LoadRam IFU transitions: arms=0`). Next target: debug task
  startup/TPC/CoReturn/LdTPC state around the transition from emulator
  initialization into task `0o14`, not disk sector transfer.
- 2026-04-30 later follow-up: two stale-I/O artifacts were removed
  from the direct-LoadRam probe. The probe now resets the modeled disk
  controller at EB takeover so Initial's partially active disk command
  does not leak into the AltoMesa image, and the display scanline stub
  no longer wakes DHT before display microcode has selected a terminal
  task. With those changes, the EB run no longer halts immediately in
  disk task `0o14`; it runs to the cycle budget and writes display
  control outputs from task `0o4`.
- 2026-04-30 memory-map follow-up: the memory sources corrected two
  emulator assumptions. `NewMemory.mc`/`InitMem.mc` use `Pipe3'`/`Map'`
  for old RP and `Pipe4'`/`Errors'` for old WP/Dirty/error status, and
  `EMemDefs.mc` puts Config `ChipSize` in b12/b13 (C bits 3..2) with
  module bits M0=`0200`, M1=`0100`, M2=`0040`, M3=`0020`. After those
  fixes the EB direct run reports `R400=0x0100`, moves BR31/BR36/BR37
  from `0x10000` to `0x20000`, and no longer sits in the old
  `XFER/XFERMD` loop at `0x10000`.
- 2026-04-30 later memory/task follow-up: `B←FaultInfo'` now clears
  latched FaultInfo, matching `InitMem.mc`'s `B_ FaultInfo'; * clear
  faults` comments, and the EB direct LoadRam shortcut maps the
  warm-start bank `0x20000..0x2FFFF`. The focused run now reports
  `Memory: faults=0`; `Mar=0x2FE1F` is mapped (`Map[0x2FE]=rp02FE
  wp0 d0 r1`) and no longer explains the blank screen. The active
  blocker moved to task/display startup: `IFU active` remains 0, no
  `PCF←B` arms occur, `DAStart` in the IOBR bank is still zero, and
  the probe eventually schedules task `014` at invalid TPC `0177037`.
  Next target: trace loaded Mesa task initialization/wakeup ordering and
  stop probe-generated disk/display wakeups from reaching uninitialized
  task TPCs.
- 2026-05-01 direct-EB display reset: the direct LoadRam takeover now
  resets the modeled display controller as well as the disk controller,
  so stale Initial display state cannot generate synthetic scanline
  wakeups in the loaded world. The focused
  `AltoMesaDorado.eb!1` run now stays in task 0 with no post-EB device
  wakeups, `Memory: faults=0`, and `MCR=0x0004 nowake=0`; it halts at
  `CPU_HALT_IFU_NOT_READY`, final `PC=0o4`. Hot PCs are
  `0o4654/0o4656/0o4657` (`Mesa.mb!3:SETDLP`) and
  `0o5724/0o5736`. The active blocker has moved from uninitialized
  task wakeups to Mesa IFU startup/PCF semantics.
- 2026-05-01 trace correction: `DORADO_POST_EB_TRACE=1` now records a
  ring of the loaded-world IM steps. The trace shows the direct EB run
  starts at `InitMap` and halts before the normal `InitTasks` sequence:
  task 0 is still the only task, `PCF<-B` has never executed, and the
  apparent `0o5724/0o5736` hot loop is reached from memory
  configuration/map setup rather than from a running Alto opcode loop.
  The next blocker is therefore earlier than IFU startup: finish
  matching `InitMem.mc`'s memory configuration/map setup side effects
  well enough for `InitMap` to branch to `StartEmulator`, then resume
  task/display wakeup debugging.
- 2026-05-01 memory enumeration fixes: several direct-EB blockers were
  traced back to original source/docs. `LoadRam.mc` item records carry
  IM, IFUM, RM, and End records, but not ALUFM, so the direct LoadRam
  shortcut now restores Initial's standard ALUFM convention before
  entering the EB image. HM §8.3/§12.1 and `Kernel5.mc` show that
  Dorado bit 15 is the low-order C bit: `AckJunkTW` enables periodic
  junk wakeups when that bit is 1, while `IFUTest.15` disables them.
  HM Table 11d also distinguishes `Pd<-ALUFMRW` (read/write) from
  `Pd<-ALUFMEM` (read-only); Mesa `SETDLP` uses the read-only form and
  must not corrupt ALUFM. `InitMem.mc`'s `NextMapEntry` uses
  `DummyRef_ T, T_ MD`, so the compiled `ASEL=1, LC=T<-Md` DummyRef
  form now uses old `T` as Mar. Finally, HM page 29 says FF branch
  conditions OR into TNIA for every non-long JCN; the implementation now
  applies this to subroutine Return as well. This fixes
  `Return[ALU=0]` in `NextMapEntry`, so the map walk exits at
  `VirtualBanks` instead of running VAHi far past `0x40`.
- Focused verification after the Return fix:
  `DORADO_BOOT_BUDGET=26300000 DORADO_ETHER_BOOT_IMAGE=../chm/microcode/AltoMesaDorado.eb!1 DORADO_POST_EB_TRACE=1 ./build/test_cpu`
  passes all CPU tests. The direct EB path now reports
  `STARTEMULATOR(AEmu)=0o1133`, reaches the `STARTEMULATOR`/`DOBRS`
  path, and no longer parks forever in the first `NextMapEntry`
  enumeration. At the sampled budget it is running in later memory/disk
  service code (`SEEKWAIT`, `KREADBADTW`, `WAITFORMAPBUF`), with
  `Memory: faults=0`, `BR36=0x19100`, no display FIFO activity yet, and
  disk FIFO reads/writes still zero. The next useful debugging target is
  the disk/status/muffler path (`Read1Muff`, `Read20Muffs`,
  `SeekWait`, `WaitForSector`) and any remaining memory-map side effects
  those routines expect.
- 2026-05-01 IFUReset/JNK follow-up: `Junk.mc!1` and
  `InitialJunk.mc` are now saved locally under `chm/`. The source and
  HM §8.3 agree that `IFUReset` loads `IFUTest` with 1. In the C word
  layout this is Dorado bit 15 (`0x0001`), which disables optional junk
  timer wakeups. Adding that side effect prevents the 30M bring-up probe
  from being preempted into the JNK/AEmu NotReady path.
- 2026-05-01 disk-loop correction: the suspected `Read20Muffs`
  infinite loop is not the current bug. A 100M full-boot trace shows
  `KTemp0` shifting through `0x8000`, `ALU<0` latching on the following
  instruction, and the conditional branch exiting at `0o6605`. The disk
  task is repeatedly returning to status/error handling because the
  hard-boot path is not producing a useful Dorado boot stream from the
  mounted Spruce Alto T-300 pack, or because command/FIFO timing is still
  too approximate for the real sequence-PROM path.
- 2026-05-01 `Output_ <source>` follow-up: slow-I/O output routing is
  now generalized from the display-only `Output_ T` shape to no-LC
  store-shaped outputs with arbitrary sources, including the
  non-emulator IOStore decode. Unit regressions cover `T`, `RM/STK`,
  and IOStore-shaped output. A 70M disk trace still shows only
  `DiskControl` outputs `0400`/`0100`, not `0005`, so the next task is
  to map the compiled PilotDisk `Output_ KCmmd` sequence and determine
  whether it is skipped/revoked or still decoded with the wrong TIOA or
  source fields.
- 2026-05-01 display/map follow-up: `DORADO_FAULT_TRACE=1` now records
  non-emulator memory-faulting task/ref/VA lines
  (`DORADO_FAULT_TRACE=all` includes task 0), and the final probe output
  prints task MemBase/TIOA plus `EmuBRHiReg`/`EmuXMBRHiReg`. The direct
  EB run with a 120M budget still reaches `NOTEMUFAULT`, but the fault is
  now pinned to display memory, not disk: after Statics gating, synthetic
  scanline and DWT wakeups are suppressed until display microcode clears
  `DHTShutUp/DWTShutUp`; the remaining last fault is `task 3`, `store`,
  `TIOA=0375` (HRam), `VA=0x2010C`, while `BR36=0x20000` and
  `Map[0x200]`/`Map[0x201]` are still vacant. This means the next blocker
  is the ordering/semantics around AEmu display initialization and the
  Alto/Mesa bank-2 map rebuild, not DiskMuff. Next action: trace the
  `JAMHRAM`/`InitHRam` path against `DisplayAux.mc` and the map writes
  immediately before it, and determine why task 3 can run through MDS
  before bank 2 has been restored.
- 2026-05-01 HRam protocol follow-up: rechecked the Sep 1981 Hardware
  Manual display RAM-loading section and `DisplayAux.mc`'s
  `LoadHRamLoop`/`LoadHRamRun`. The display model now has a minimal
  HRam slow-IO loader for TIOA `0375`: `Keep'` low grants Dorado
  ownership, `LoadAddr` latches the slow-IO address while owned,
  `Write'` low writes a 3-bit HRam value, and writes auto-increment.
  `test_hram_load_protocol` pins this. The fault trace also now prints
  the decoded faulting microinstruction and raw IM words, so the next EB
  run can distinguish a true runtime store instruction from a stale/late
  memory-reference report.
- 2026-05-01 slow-IO output follow-up: the `DisplayAux.mc`
  `LoadHRamRun` loop and other display/disk sources use source-level
  `Output_ T` forms. In the loaded image this can decode like an
  alternate-source store-shaped instruction, but it must drive IOB at
  the current TIOA instead of issuing a main-memory store. The CPU now
  routes the narrow no-LC `Store<-T` shape to registered slow-I/O
  writers, and `test_output_t_store_shape_routes_slow_io` pins it.
  Result: the prior task-3 HRam fault is gone. A 30M-cycle
  `AltoMesaDorado.eb!1` run now reaches `display outs=+1090` and
  `scanline wakeups=+970`; the next blocker is task 0 fetching through
  MDS at `VA=0x20004` (`pc=0o573`) after bank 2 has become vacant.
  That points back to warm-start/LoadRam map restoration for the
  Alto/Mesa bank, not to HRam slow-I/O.
- 2026-05-01 PNG snapshot check: `/tmp/dorado_boot_display.png`
  is generated from the current EB boot probe, but it is still all
  white (`unique=1`, `nonwhite=0`). The display renderer itself is
  not blank: `/tmp/test_dorado_display.png` and
  `/tmp/test_dorado_render.png` contain nonwhite unit-test pixels.
- Direct LoadRam shortcut correction: `InitMem.mc` says warm-start
  setup assumes the first 64K of **real** memory contains the Alto
  memory, then maps the emulator's virtual bank onto it. The EB
  shortcut now maps AltoMesaDorado's virtual bank 2 (`0x20000`) onto
  real pages 0..255 instead of real pages `0x200..0x2ff`, and reapplies
  that probe map after the replacement RAM image is loaded. This is
  still a probe-side shortcut until the real LoadRam path owns the
  warm-map handoff.
- Follow-up diagnostic: the loaded EB image still rewrites and later
  vacates `Map[0x200]` during its map-init path before the emulator
  task fetches from MDS. The probe now records the cycle/task/PC of
  the first bank-2 vacant transition and, for EB-only bring-up, remaps
  bank 2 to real pages 0..255 so debugging can continue past the
  task-0 IFetch fault. This shim must be removed/replaced by the real
  LoadRam/InitMap behavior.
- Next fault after that shim was task 2 IFetch at `pc=0o3656`,
  `BR=0x60b7e`, `VA=0x624aa`: another vacant page outside bank 2.
  The direct probe now maps the installed real-storage page range
  before overlaying bank 2 to real page 0. This is intentionally still
  restricted to the EB probe path.
- Latest EB probe with the installed-storage watch shim gets further:
  `display outs=+48333`, `dwt wakeups=+1`, disk slow I/O is active
  (`outs=+3020`, `ins=+2984`), and IOBR display-list words are no
  longer all `FFFF`. The framebuffer is still all white because
  display fast-I/O fetches remain `0`. The next blocker is an emulator
  IFetch fault after the Code BR goes bad (`BR37=0xfff0006` in the
  summary); decode/trace the BR load path before adding more display
  behavior.
- 2026-05-01 follow-up: RBase is now saved per task. This removed the
  EB direct-load `BR37=0x400006` corruption caused by task 0 resuming
  from `Start.mc` with task 1's `RBase=4`. The current framebuffer is
  still all white (`display iofetch=0`, low-core `DAStart=0`), so the
  display is waiting for software/disk progress rather than failing to
  render FIFO data. The next disk trace showed Pilot DriveTag word
  `0x08f0`; native bit `0x0800` is KSelect bookkeeping, so DriveTag
  subsector-count decode now masks to the documented tag-bus field.
- 2026-05-01 disk-to-LoadRam update: the `muffRdFifoTW` fallback moves
  the full boot past the old `KCmmdRead`/`KReadBadTW` wait. Initial's IM
  loaded from disk now matches canonical `Initial.mb` for 925/926
  overlapping entries (only `0o7512` differs), and the final hot loop is
  LoadRam (`0o7605/7606/7611/7612/7614/7620/7673/7675/7676`) rather than
  disk microcode. New opt-in `DORADO_LOADRAM_TRACE=1` shows LoadRam
  enters with `BR31=0x00200` and reads the boot item stream from
  `0x200`, but those words are zero, so it interprets an endless run of
  zero IM items instead of seeing an End item. Next: determine whether
  `spruce-server.dsk300` lacks the Dorado hard-microcode boot file, or
  whether our disk boot path is failing to write the boot image into the
  `01000B` buffer before `CheckChecksumAndLoad`.
- EB snapshot follow-up: the direct EB probe reinitialized the display
  after `loadram_image_direct` but left `display.attached=0`, so the
  end-of-run headless snapshot was skipped even though the I/O callbacks
  still referenced the same display object. The probe now reattaches the
  display after that reset. With `AltoMesaDorado.eb!1` and a 120M budget,
  `/tmp/dorado_direct_eb.pgm` is written at frame 158, still all white
  (`unique=1`, `nonwhite=0`), with `display iofetch=0` and no memory
  faults.
- Display word-task follow-up: the synthetic WCB wakeup now targets AWT
  (`011`) when the active terminal horizontal task is AHT (`004`), and
  DWT (`013`) otherwise. This is the correct `DisplayMain.mc` THT/TWT
  relationship, but it does not change the current EB result: task 3 is
  still hot in `COLORBITMAPNIL`, `DAStart[0420..0427]` remains zero, and
  there are no display munches for fast I/O to fetch.
- 2026-05-01 AWT fast-I/O follow-up: the fast-I/O router now treats AWT
  (`011`) as the DispM peer of DWT (`013`) for display `IOFetch<-`
  delivery. `test_fastio` covers both word tasks. An 80M full-bootstrap
  run still has `display iofetch=0`, `unrouted_iofetch=0`,
  `DAStart[0420..0427]=0`, and an all-white
  `/tmp/dorado_boot_display.pgm`; AWT runs only briefly (`31` task
  cycles) and no word task issues a real display munch. This rules out a
  missing AWT FIFO route as the current blank-screen cause. Continue
  debugging the upstream AEmu/Pilot disk/status path that should install
  the display control block chain.

## Cross-cutting: don't drift from "match the docs"

A few rules to keep ourselves honest:

- **HM section numbers in commit messages and comments** when
  implementing a piece.
- **Field names from the manual** in code (RSTK, ALUF, BSEL, FF,
  JCN, IFUM, ALUFM, MemBase, MemBX, BR, Link, TPC, ShC, Q, Cnt,
  StkP, Pipe, Mar, Pd, Md). Don't rename.
- **Octal where the manual is octal.** Microcode addresses, RSTK
  values, ALUF entries, JCN: octal literals.
- **Cite the source.** When we infer behavior from microcode (not
  the manual), say so explicitly in a comment with the file +
  line of the source we read. Example: "// inferred from
  doradoboot.masm line 47 — firmware leaves Y=4 here, see
  cpu.c:ALUFMRW write."
- **Empirical fits get flagged.** When the manual is wrong/
  ambiguous and we patch behavior to match firmware, the comment
  says "EMPIRICAL: " and explains.

## Estimate

## Current Next Step (2026-05-01)

The real Ethernet path now reaches the loaded Mesa/AEmu world:
Initial requests boot file `0110`, receives `AltoMesaDorado.eb!1`,
verifies the zero checksum, and `LoadRam` jumps to the EB End-item
start address. The next blocker is no longer Ethernet or LoadRam.

Current default 80M Ethernet run no longer halts and no longer reaches
the old forced Alto `KWait` shim (`alto-boot shims=0`). It finishes in
the display/disk task mix around `PC=0o7321`, with
`display outs=671639`, `display iofetch=0`, `DAStart[0420..0427]=0`,
and an all-white frame 114 snapshot. The older KWait shim path still
has an important correction from `AEm0.mc`: it masks `DoneStatus`
(`07400`) first, then treats the low byte as error bits, so a shimmed
successful read must write `07400`, not `07401`.

2026-05-02 disk trace update: `tests/test_cpu` now keeps the
`DORADO_DISK_TRACE` buffer as a last-N ring instead of only the first
8192 samples. This makes the late loaded-world disk loop visible. The
current late trace is task 14 in the Alto disk loop/status path
(`AEmu.mb` `AltoLoop`/`Read1Muff`/`Read20Muffs` region after relocation),
not in a data-transfer loop: `fifo reads=0`, `fifo writes=0`,
`DiskControl=0400/0100` setup values, and no `DiskData` reads. The
temporary first-sector shim was made MDS-aware (`ADefs.mc` says Alto
`IOBR` is `MDS`, BR `036`), but it still does not fire in the default
run; final MDS low-core `[0431..0440]` and `[0521..0523]` are already
zero. This suggests the DSK task is consuming/clearing the AEmu command
before the harness shim sees it, or the command handoff is not being
posted where the current probe expects. Next step: trace the memory
stores to MDS `0431..0440`/`0521..0523` and the `AltoLoop` fetch that
decides whether to branch to `DoACmmd`.

2026-05-02 cache-visible diagnostics update: dirty cache lines can hide
boot handoff words from storage-only probes, so the harness now uses
`dorado_visible_word_at_va()` for low-core/display/disk control-block
diagnostics. This changed one important observation: MDS `0523` is
cache-visible as `177776`, even though backing storage still reads zero.
The old Alto `0521=0431` command pointer still never appears, so the
first-sector shim remains inactive.

2026-05-02 forced-shim correction: the apparent loaded-world `KWait`
PC `0o5550` was a bad symbol match. A focused `DORADO_POST_EB_TRACE=1`
run shows `0o5550` is a store/clear loop with `MemBase=30`, not the
`AEm0.mc` disk wait. The harness no longer forces the first-sector shim
there; it only keeps the older `0o1017` force point and the command-block
scan path. The scan now also checks BR `030` because the live trace uses
that memory base in the loaded world. Current 80M runs are therefore
honest again: `alto-boot shims=0`, no `DiskData` reads, no read streams,
and the display still ends at `COLORBITMAPNIL`. The next blocker is to
find where the loaded world should post the Alto command block, or why
the DSK task never branches from the status scan into `DoACmmd`.

2026-05-02 Spruce-pack note: the old default forced shim used head 18,
sector 0, but `spruce-server.dsk300` has free/blank labels and zero data
there. The first nonzero executable-looking sectors are near cylinder 0,
head 0, sector 2 and later. The shim now has `DORADO_ALTO_BOOT_CYL`,
`DORADO_ALTO_BOOT_HEAD`, and `DORADO_ALTO_BOOT_SECTOR` knobs, defaulting
to `0,0,2` for local experiments. This is only bring-up scaffolding and
does not replace the missing real command handoff.

The PilotDisk CSB hypothesis was checked against `PilotDiskDefs.mc` and
`PilotDisk.mc`: `CSB.next` is at `0177520` relative to `IOBR`, not the
absolute page. New diagnostics print both. Absolute `0177520` remains
`FFFF`, but the real `IOBR+0177520` CSB is `[0000 0000 0000 0000]`;
`DORADO_CSB_TRACE=1` shows Initial/DSK clearing it and no software
posting an IOCB. Therefore the current blank screen is not blocked on
PilotDisk command-chain setup. A 120M focused disk trace still loops in
the Alto disk status/sector path with no `DiskData` transfer, while the
display task spends its time in `COLORBITMAPNIL` and `DAStart` stays
zero. Next step: debug the Alto `ACmmdCheck`/`Read20Muffs` status path
and why it never reaches `DoACmmd`/data transfer for the boot command.

These are weeks-of-work each, in rough order of effort:

| Phase | Effort | Dependency      |
|-------|--------|-----------------|
| A     | 1 wk   | none            |
| B     | 3 wk   | A               |
| C     | 2 wk   | B               |
| D     | 2 wk   | A               |
| E     | 3 wk   | A, B, D         |
| F.1   | 1 wk   | C, D            |
| F.2   | open   | A–E + cross-val |

Phase A is high-leverage — it confirms the BB↔Dorado handshake all
the way through Boot1, which proves the whole architectural skeleton
is right. Start there.
