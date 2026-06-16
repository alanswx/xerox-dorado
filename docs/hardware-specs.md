# Hardware implementation specs — unbuilt / stubbed / approximated subsystems

Derived from the Session-3 schematic sweep (`docs/schematic-audit.md`), the
known-gaps punch list (`docs/handoff.md` §A-K), and `dorado/CLAUDE.md`.
Each spec is meant to be buildable from this document without re-reading the
schematic. Specs are ordered **boot-critical first, fidelity-only last**.

Boot route reminder (CLAUDE.md): bring-up boots over **Ethernet** (Mayday
Pup + EFTP), not disk. So the boot-critical ordering favors (a) the small
localized fixes that unblock Stage-2 emulator-selection and display output,
(b) Hold (needed the moment a real emulator world runs), and (c) a real
Ethernet controller; disk and ECC matter later.

Priority legend: **P0** boot-blocking · **P1** needed to run a real OS once
booted · **P2** fidelity / diagnostics / Phase-2 (Verilog) prerequisite.

---

## Quick wins — localized concrete bug fixes (do these first)

These are not subsystems; each is a few lines and several are boot-relevant.
Listed here so they are not lost among the big specs.

**Status (2026-06):** QW1 DONE (commit eeb5807), QW2 DONE (b2f0c72),
QW3 DONE (d286f03, HW-Manual-validated), QW7 DONE (7780d32). QW6 was
already implemented (the SubTask OR into MemBase[2:3] is in the ref path
at `cpu.c:3221`, not missing -- the finding looked only at the RBase site
`cpu.c:339`). Remaining: QW4 (P2, couples with ECC spec HS6 -- needs the
RealPageInRange check confirmed first) and QW5 (P2, ASRN advance gating).
All landed fixes were validated against the HW Manual and kept the gate
green (make test 10/10, Galaxian 121553 exact, NETEXEC in band, germ-6.1
reaches the identical 155-dispatch blocker).

### QW1 — Keyboard/terminal back-channel bit position (Finding 9, gap E2). **P0**
- **Hardware (DispY18/21, DispM10/21):** terminal serial back-channel →
  `OISRcvdData` → gated onto **`IOB.00`** (bit 0, MSB) of the Status input
  device (TIOA 0370 DispY / 0360 DispM). Muffler data is on `IOB.15`.
- **Today:** `display_terminal_keyboard_bit()` (`display.c:211-230`) and the
  boot-button jam (`display.c:430`) return a bare `0/1` that
  `display_input()` (`display.c:432`) places in **bit 15 (LSB)**.
- **Fix:** return `bit ? 0x8000u : 0u`. Verify `ReadTerminal`
  (`DisplayAux.mc!1`, "* Data = IOB[0]") then `LCY`-accumulates the 32-bit
  message MSB-first. Without this, no real keyboard/boot-chord/boot-button
  microcode can ever decode — it blocks Stage-2 emulator selection.

### QW2 — DWTFLAG `20c` IOFetch-signal case (Finding 10, gap E1). **P1**
- **Hardware (DispY15/27):** DWT command RIOB decodes: `20c` (bit 11) →
  IOFetch signal, **do not touch WCB flags**; `1c` → Set CurrentWCB + clear
  NextWCB; `0c` → clear CurrentWCB.
- **Today:** `display.c:305-310` does `current_wcb_flag = data & 1`, so the
  DWT loop's `Output← T` with `T=20c` (every IOFetch pacing pulse) clears
  CurrentWCBFlag.
- **Fix:** `if (data & 0020u) {/* IOFetch signal: leave flags */} else if
  (data & 1u) {cur=1; next=0;} else {cur=0;}`. Likely part of the
  blank-screen symptom once a display list exists.

### QW3 — Pipe4 MapTrouble bit on Vacant/WP fault (Finding 6, gap C2). **P1**
- **Hardware (MemX13):** `MapTrouble` Pipe4 bit .01 set on **any** vacant or
  write-protect reference, not just dirty-victim writeback.
- **Today:** set only in `record_writeback_fault` (`memory.c:838`).
- **Fix:** in `dorado_memory_ref_task`'s fault path, when `va_translate`
  returns `DM_FAULT_PAGE` or `DM_FAULT_WRITE_PROTECT`, call
  `dorado_pipe4_set_error(mem, srn, PIPE4_ERR_MAP_TROUBLE)`.

### QW4 — Pipe4 baseline `0170361` vs `0150361` (Finding 8). **P2**
- `dorado_pipe4_at` returns `0170361 ^ ht` (`memory.c:310`) while the header
  (`memory.h:386`) and HM §5.8 say `0150361`. Reconcile to HM `0150361`
  *after* confirming the boot path's RealPageInRange test still passes
  (the two differ in bit 2 = wProtect). Couple this with the ECC spec
  (HS6) so the `not*` error bits flip to 1=OK polarity.

### QW5 — ASRN advance on I/O cache hits (Finding 7). **P2**
- `memory.c:1275-1282` advances ASRN for all `use_asrn` refs; HM says only
  references that *start the map* advance it (not a cache hit / clean Flush /
  DummyRef). Gate the advance on "this ref started the map" (miss, or a
  ref-kind that always starts the map: IOFetch/IOStore/Map/Store-miss).

### QW6 — SubTask into MemBase[2:3] (Finding 12). **P2**
- ProcL12: SubTask OR's into both RBase[2:3] **and** MemBase[2:3]; emulator
  does RBase only (`cpu.c:339`). Add the MemBase OR in the MemBase
  formation path.

### QW7 — Overflow for all 5 arithmetic ops (Finding 13). **P2**
- `alu_op` (`cpu.c:1981`) computes `overflow` only for A+B and A-B-1.
  Compute it (carry-out-of-bit-0 XOR carry-out-of-bit-1, i.e. `sa==sb &&
  sa!=sr` on the signed operands) for 2A, A+1, A-1 as well.

---

## HS1 — Cache Miss / Hold state machine + Hold semantics (gaps B1, C1). **P0/P1**

The single most important structural gap for running any real emulator
world. Boot microcode runs with `mcr.disHold=1` and uses cycle-counted
waits, so it is masked during boot; but the moment AEmu/Mesa runs without
disHold, the engine reads `Md` a cycle early and loops forever
(`probe_aemu` LRTYPETABLE spin; handoff gap I1/B1).

### What the hardware is / does
- **MemC sheet 15 (page 17) "Miss, Hold":** four parallel tag comparators
  (`dHit0..dHit3` → `dHit`) match VA[04:11]/[12:15]/[16:19] against each of
  the 4 cache columns. The master Hold is a wide wire-OR:
  `Hold = (PipeHold ∨ MDhold' ∨ DbufHold ∨ IfuHold ∨ MischHold ∨ CBHold ∨
  RefHold ∨ MXHold ∨ SomeExtHold') ∧ ¬DisHold`, where `SomeExtHold' =
  PrHoldReq ∨ CHoldReq ∨ ExtHoldReq`. When Hold is asserted the processor
  clock stalls and `BLretry` re-runs the held microinstruction next cycle.
- **MemC sheet 16 (page 18):** the `MDhold'` interlock — if the processor
  sources `+MD`/`B←Md` before `DdataGood` (the read has not returned), Hold
  fires until the data arrives (`MDPending'/MDdly`).
- **MemC sheet 18 (page 20) "Next":** the staged reference machine that
  produces `PipeHold/DbufHold`. A reference walks
  `StartMap→AcanhaveD→DbufBusy→StartST→STfree`; a new ref cannot start until
  `Afree`/`RefOutstanding'`. A dirty-victim writeback flows through ST and
  blocks the following reference.
- **Timing (HM §5.5-5.8):** a cache miss holds the engine ~28 cycles while
  Map→cache-fill runs; the munch is 16 words; the victim column is the
  VNV pointer (see Finding 3 / HS2 victim note).

### What the emulator does today
- `include/memory.h:12-14`: "refs are atomic and Md is available
  immediately; full Hold/deferred-reference behavior is not modeled."
- `cpu.c:3278-3283`: a ref issues and `latch_task_md_from_memory(cpu)` runs
  the same cycle — zero delay, even on a miss.
- `dorado_mcr_dishold()` getter exists (`memory.c:476`) but Hold is a no-op.
- Only `MapBufBusy` (9-cycle timer, `memory.c:397-406`) models any stall.

### Spec to implement
Add a minimal but correct Hold model. Two phases:

**Phase A — MD-not-ready Hold (the one AEmu needs).** This alone fixes the
`probe_aemu` spin and is the smallest correct subset.
1. State to add (on `dorado_cpu` or per-task in the memory model):
   - `md_pending[task]` (bool), `md_ready_cycle[task]` (uint64), and a
     global cycle counter `mem->cycle` (increment once per microinstruction
     in the main tick).
2. On a `Fetch←/IFetch←/LongFetch←/IOFetch←` that **misses**: set
   `md_pending[task]=1`, `md_ready_cycle[task]=cycle + MISS_LATENCY`
   (start `MISS_LATENCY=28`, tune against HM §5.5). On a **hit**: deliver
   Md immediately (`md_pending=0`) as today.
3. Before executing any microinstruction of task T that **sources Md**
   (A-mux `Md`, `B←Md`, `T←Md`, `RM/STK←Md` — detect via the existing
   decode that drives `latch_task_md_from_memory`): if `md_pending[T]` and
   `cycle < md_ready_cycle[T]`, assert **Hold** — do NOT advance PC, do NOT
   commit RM/STK/T/StkP, re-execute the same instruction next cycle
   (`BLretry`). When `cycle >= md_ready_cycle[T]`, deliver the real munch
   word and clear `md_pending`.
4. Gate the whole thing on `!dorado_mcr_dishold(mem)` so boot microcode
   (disHold=1) keeps the current immediate behavior.
5. Interaction with tasking: Hold stalls **only the current task**; the
   task scheduler still runs (a held task can be switched away from and the
   memory completes in the background). Simplest correct model: Hold
   re-presents the same instruction; if a task switch occurs the held state
   rides in `md_pending[T]`.

**Phase B — staged reference / Pipe-full Hold (fuller fidelity).** Add a
small reference queue:
1. `struct ref_inflight { uint8_t srn; uint8_t stage; uint64_t done_cycle;
   ... }` and a fixed-size in-flight set sized to the pipe (16).
2. Stages `MAP → MEM → EC1 → EC2` advancing on the cycle counter; the SRN
   shifts with the reference (this also implements HS3 SRN hand-off).
3. `PipeHold` when the pipe slot the microcode wants to read
   (`B←Pipe0..5`) is still busy; `DbufHold` when a store's transport stage
   is occupied and a new store arrives.

### Edge cases
- `DummyRef←` never holds. `Flush←`/`Map←` follow MapBufBusy not MD Hold.
- StkError also raises Hold (HS-stack / Finding 11) — share the same Hold
  assertion path.
- IFU map fault concurrent with IFUJump holds (HS5).

### Test / what exercises it
- `probe_aemu` should advance past LRTYPETABLE→LRTYPEIM once Phase A lands.
- New unit test: issue `Fetch←` to an un-cached page, immediately
  `T←Md`; assert the engine re-runs the instruction `MISS_LATENCY-1` times
  then latches the stored word. With disHold set, assert immediate delivery.

### Files
`cpu.c` (the ref-issue + Md-source decode path, the main tick loop, the
commit barrier in `next_pc`), `memory.c`/`memory.h` (miss detection,
in-flight set, cycle counter).

---

## HS2 — Cache victim/next-victim (VNV) pseudo-LRU (Finding 3, gap in MemC04). **P1**

### What the hardware is / does
- **MemC sheet 04 (page 6) "Victim + NextVictim":** a per-row pair of 2-bit
  pointers (`VicMem'`/`NVmem'`, addressed by cache row `Aad.0:7`). Update
  rule (HM §5.7, md 2980): on a **miss or hit-in-Victim**, `Victim←NextV`;
  on a **miss, hit-in-Victim, or hit-in-NextV**, `NextV←(¬Victim.0,¬NextV.1)`
  (a value different from both). "Not quite LRU." Written on
  CacheRef/PreFetch/IfuRef that miss, and on any FlushStore/Flush←.
- Pipe5 exposes Victim (bits 12:13) and NextVictim (bits 14:15) for Midas /
  cache self-test.

### What the emulator does today
- `cache_pick_victim()` (`memory.c:761`) returns `lru[3]` of a **true 4-way
  LRU** maintained by `cache_touch_lru()` (`memory.c:661`). The in-code
  comment ("HM doesn't pin down the exact LRU encoding") is wrong — HM §5.7
  and MemC04 pin it down, and VNV ≠ true LRU.

### Spec to implement
1. Replace `lru[4]` per row with `struct { uint8_t victim:2, nextv:2; }`.
2. On lookup, classify the access as miss / hit-Victim / hit-NextV / hit-other.
3. Apply the VNV update exactly:
   - if (miss ∨ hitVic): `victim = nextv;`
   - if (miss ∨ hitVic ∨ hitNV): `nextv = ((~victim) & 2) | ((~nextv) & 1);`
   (Use the post-update `victim` for the nextv computation per the MemC04
   "different from both" annotation; verify against the sheet's exact
   ordering.)
4. `cache_pick_victim` returns `victim`; `cache_line_pipe5_flags`
   (`memory.c:367`) returns `victim`/`nextv` directly (not LRU-derived).
5. `Flush←`/`FlushStore` smash Victim and NextV per HM line 2107 / MemC04.
6. Keep the `usemcrv` (MCR-forced victim) override path.

### Priority note
Data-correctness is unaffected (any column may be evicted), so this is **P1
fidelity**: it matters for Pipe5 victim/next-victim readback parity (Midas,
cache self-test) and for bug-parity with hardware on victim-order-dependent
microcode. Confirm impact by scanning emulator microcode for `B←Pipe5`
Victim/NextV reads and `Flush←` sequences.

### Files
`memory.c` (`cache_pick_victim`, `cache_touch_lru`→rename,
`cache_line_pipe5_flags`), `memory.h` (cache row struct).

---

## HS3 — Staged memory pipeline + SRN hand-off (gap C1). **P1/P2**

### What the hardware is / does
- **MemX sheet 03 (page 5):** a reference's SRN is physically shifted
  through a latch ladder `MapSrn→MemSrn→Ec1Srn→Ec2Srn` (MC141 chain). A
  fault is recorded against the **Ec2-stage** SRN — many cycles after the
  Map read. HM line 2829: "an SRN might report MapTrouble before its
  predecessor reported SE or DE; this could screw up the fault order."
- **MemX sheets 05/06/07/09/10:** the Store-Transport / Map / Memory / Ec1
  / Ec2 automatons, each a 3-4-bit `*State` F16 register sequencing a stage
  (DRAM RAS/CAS, ECC generate/check). A store stages DBuf→shift-register→
  ECC-generate→RAM over the 16-clock transport.

### What the emulator does today
- `dorado_memory_ref_task` (`memory.c:881`) computes the fault synchronously
  on the **current** `srn`; FaultInfo composed statically (`memory.c:413`).
  No per-stage SRN copies, no automaton state. Store is atomic
  (`memory.c:1017,1097`).

### Spec to implement
This is largely subsumed by HS1 Phase B (the in-flight reference set). On
top of that:
1. Each `ref_inflight` carries its `srn` and a `stage` advanced on the
   cycle counter (`MAP→MEM→EC1→EC2`).
2. Fault recording happens when a reference **reaches EC2** (or when MapTrouble
   is detected at the MAP stage, but the *reporting* into FaultInfo/Pipe4
   follows the staged order). Preserve the existing "first fault latches the
   SRN" rule (`memory.c:1263`) but apply it at stage completion.
3. ASRN advance (QW5) becomes "advance when the ref enters the MAP stage and
   actually starts the map."

### Priority note
**P2 for normal running** (no current microcode depends on cross-reference
fault ordering), **P1 only** if a fault-heavy emulator world (Mesa page
faults under load) needs the exact FirstFaultSRN when multiple references
are outstanding. Implement after HS1 since it reuses the in-flight set.

### Files
`memory.c` (in-flight set, fault recording at stage completion), `memory.h`.

---

## HS4 — DDC terminal back-channel + keyboard / boot-key decode (gap E2). **P0**

QW1 fixes the bit position; this spec is the full back-channel needed to let
the user **select which emulator to boot** (Mesa/Cedar/Lisp/Smalltalk/Alto)
and to deliver keyboard/mouse to a running OS.

### What the hardware is / does
- **DispY21 / DispM10 (OIS Terminal interface), DispY18 (Slow-Input):** the
  terminal microcomputer drives a serial back-channel on the 7-wire cable
  (`OISDataOut`), routed through the BaseBoard, re-driven as `KeyboardData`
  (backpanel pin 177), clocked (by `AltoHBlank`/`RamdHBlank`) into
  `OISRcvdData`, and presented on `IOB.00` of the Status device (0370 DispY
  / 0360 DispM).
- **Message format (`DisplayAux.mc!1 ReadTerminal`, HM Table 24):** a 32-bit
  message read one bit per scan line, MSB-first:
  `1, x,x,x, type[0:3], body[0:15], 1,0000000` (trailer = `200₈`,
  completion test `(TerminalHi) AND 200C`, validity = 24th bit).
- **Type numbers:** 1=KbdWord@177034, 2=177035, 3=177036, 4=177037,
  5=Buttons/keyset@177033, 6=mouse Δx/Δy, 10=177040, 11=177041(Star),
  **17=boot** (data jammed to 1). Boot button is detected by the start bit
  staying 1; `ReadTerminal` counts `BootTimer` scan lines and rejects
  pushes < `MinimumPush`.

### What the emulator does today
- `display_terminal_keyboard_bit()` (`display.c:211`) builds a message and,
  since QW1 (2026-06), returns it on `IOB.00` correctly; `display_input()`
  returns an idle key word otherwise; no mouse/button/boot-type messages.
- The boot-button jam returns `0x8000` on `IOB.00` (QW1).

### STATUS (2026-06, empirical): serial back-channel has no consumer yet
Instrumenting `display_terminal_keyboard_bit()` during a full AEmu/NETEXEC
boot shows it is **never called** -- the AEmu world's terminal task never
runs `ReadTerminal` to decode the serial stream in our boot. The keyboard /
boot-key / mouse state the running world reads is supplied by the
**memory-seeding** path instead (`machine.c machine_seed_keyboard` /
`machine_seed_mouse` + Initial STK boot-parameter seeding), which is
gate-locked and works. Consequences:
- **QW1 (bit position) was the real, bounded bug here and is done.** It
  makes the serial readout correct for whenever a world *does* run
  `ReadTerminal` (e.g. a future Cedar/Mesa terminal task).
- The rest of HS4 below (serial shift-out queue for types 5/6/17, boot-key
  chord over the serial wire) would be **dead code with no consumer today**,
  so it is deferred per the "no speculative behavior" norm. Revisit only
  when a world we run actually executes `ReadTerminal` (verify by
  re-instrumenting; the probe is trivial to re-add).
- Boot-key *selection* already works functionally via the memory-seeded
  chord (`--boot-keys` / `machine_apply_boot_chord`); it is not blocked.

### Spec to implement (deferred -- see STATUS above)
1. A serial shift-out engine driven at scan-line rate (one bit per
   `display_scanline`/vblank tick — the emulator already advances a frame
   counter at vblank): hold a 32-bit `terminal_msg` and a `bit_index`; each
   scan line return `((terminal_msg >> (31-bit_index)) & 1) ? 0x8000 : 0`
   on the Status TIOA, advancing `bit_index` 0..31 then idle (start bit 1).
2. A message queue so keyboard words (types 1-4), buttons (5), mouse (6),
   and the boot message (17) can be enqueued. Build each per the format
   above (leading 1, 4-bit type, 16-bit body, trailing `200₈`).
3. Boot-key selection: enqueue a type-17 (boot) message, or hold the
   appropriate keyboard chord, so `ReadTerminal`'s boot path fires.
   Expose an API `dorado_display_set_boot_keys(emulator_id)` that maps the
   chosen emulator to the right key chord (the existing keyboard matrix in
   `display.c` already has the Alto key map imported from palo).
4. Deliver keyboard on the TIOA that matches the board-presence answer
   (Finding 9 secondary): if the emulator reports "no DispM," keyboard must
   come back on the DispY Status device 0370, not 0360.

### Edge cases
- The message must be re-presented bit-by-bit at exactly scan-line cadence;
  `ReadTerminal` shifts on its own schedule, so over- or under-running the
  bit clock corrupts the 24th-bit validity check.
- Idle line = start bit high (1) continuously; a real message starts when
  the leading 1 is followed by the 3 don't-care bits then a nonzero type.

### Test / what exercises it
- Run `DisplayAux.mc ReadTerminal` (layered) and assert it assembles the
  enqueued 32-bit message and passes the `200C` completion + validity test.
- Full-boot probe: enqueue a boot-type or keyboard chord and confirm
  Initial's `GotBootKey` path selects the intended emulator (removes the
  probe-side `ETemp0..3 forced all-up + GotBootKey redirect` workaround,
  handoff A4).

### Files
`display.c`/`display.h` (the shift engine, message queue, boot-key API),
and the Status-input TIOA handlers.

---

## HS5 — IFU F/G/H/J/M/X byte pipeline + fill/drain timing (gap B9). **P2**

### What the hardware is / does (enough to implement)
- **6-deep byte pipeline** (IFU sheets 05/03):
  `mem word → F → G(FG) → H&J → AlphaM → AlphaX → MUX → IfuData[0:7] → ←Id`,
  with a parallel **PC pipeline** `PcFG → PcJ → PcM → PcX` (sheets 08/09/10)
  and per-level **valid latches** `FDv/GDv/HDv/JDv/MDv` (sheet 03;
  "HLd=HDv'", `HDv/JDv` cleared by `ZapFGH`) and per-level **fault latches**
  `FFault/GFault/HFault/JFault` (sheet 05).
- **Fill engine (sheet 01):** `RefOutstanding` latch (set on a ref issued,
  cleared on `IfuMemAck`) enforces exactly one outstanding IFU memory ref.
  `WantIfuRef` asserts when a downstream hole exists ∧ `!RefOutstanding` ∧
  the processor isn't using Mar → `Mar←PcF` → on `IfuMemAck`, `IncPcF`. Two
  bytes per word fetched.
- **Jump flush (sheet 02):** a taken jump asserts `ZapFGH` → discards F/G/H,
  reloads `PcF` with the target, clears `HDv/JDv`; the pipe must refill
  before the next opcode is ready (NotReady again) **unless**
  `OneByteJumpInJ` (target byte already buffered).
- **Exceptions (sheet 07):** priority-encoded trap bases (octal)
  `KFault=0, FGParity=4, Resched=14, NotReady=34, RamParity=74`, with
  `KFault = JFault ∨ (HFault ∧ Length>1) ∨ (FGFault ∧ Length=3)`.
- **`←Id` selector (sheet 23, SG139 PROM):** addressed by
  `{N, TwoAlpha, JMP, Length[0:1]}`, selects the operand-export sequence,
  including the TwoAlpha split `Alpha[0:3],Alpha[4:7]` and "jumps export
  only length / no N on 2-byte jumps."

### What the emulator does today
- Fixed `ifu_warmup=5` countdown (`cpu.c:1034`), single `ifu_pcf`/`ifu_pcx`
  (`cpu.h:118`), bytes fetched **live at dispatch** (`cpu.c:2568,2615`), no
  jump-flush refill (`cpu.c:2659` falls straight through), faults surfaced
  at dispatch (`cpu.c:2591`), `←Id` sequence hard-coded (`ifu_id_at`,
  `cpu.c:2321`), TwoAlpha/jump-export rows missing (Finding 14).

### Spec to implement
1. Replace the scalars with a staged structure: arrays `fdv/gdv/hdv/jdv/mdv`
   (valid bits), byte registers `f/g/h/j/m/x`, fault latches
   `ffault/gfault/hfault/jfault`, and a PC pipeline `pcfg/pcj/pcm/pcx`.
2. **Fill:** each cycle, if a hole exists (some DV clear downstream),
   `!ref_outstanding`, and the processor isn't issuing a Mar reference,
   issue `Mar←PcF` (an `IFETCH` ref), set `ref_outstanding`; on memory ack
   (immediate today, staged under HS1) load 2 bytes into F, advance, clear
   the consumed hole, `IncPcF`.
3. **Drain at IFUJump:** consume opcode + operands by Length from the X/M
   end; advance the pipe; recompute readiness.
4. **Jump:** on a taken IFUJump branch, assert ZapFGH — clear F/G/H DVs,
   reload PcF with target, and re-enter NotReady until refilled, **unless**
   `OneByteJumpInJ` (target within the already-buffered J/M range).
5. **Faults:** buffer `FFault/GFault` with the byte at its level; at IFUJump
   compute `KFault` from the staged fault bits and the opcode length, then
   trap to base 0 — instead of issuing a fresh ref and faulting at dispatch.
6. **`←Id` PROM:** drive operand ordering from an SG139-style table indexed
   by `{N, TwoAlpha, JMP, Length}`; implement the TwoAlpha α-byte split and
   the no-N jump rows (Finding 14).

### Priority note
**P2.** The fixed warmup is functionally adequate for opcode dispatch and
the current boot path; this matters for cycle accuracy (the Phase-2 Verilog
prerequisite, gap K1) and for any microcode whose timing depends on
precise prefetch-vs-branch interleaving or per-level IFU map-fault buffering.
The TwoAlpha `←Id` rows (step 6) are the highest-value sub-piece if a Mesa
world uses α-byte-pair opcodes — that part is **P1**.

### Files
`cpu.c` (IFU fetch/decode/dispatch/jump paths), `cpu.h` (IFU state struct).

---

## HS6 — MemD ECC / Hamming generator-checker-corrector + Pipe4 syndrome (gaps C2, C3). **P2**

### What the hardware is / does
- **Geometry (HM §5.9, Fig 11):** a munch = 4 **quadwords**; each quadword =
  64 data bits + 8 check bits. ECC is per-quadword.
- **Generator (MemD sheet 04):** 8 check bits `C0..C7` from MC163 9-input
  parity trees (C7 stored complemented; `TestSyndrome` XORed into the
  written check bits — the self-test hook).
- **Checker (MemD sheet 05):** recompute + XOR stored `Ci` → syndrome.
  `Syn0-3` point to the erroneous bit (read in reversed order 3,2,1,0 →
  0..15); `Syn4-6` select the word with a 2-of-3 code (word1=011, word2=101,
  word3=110, word4=111); C0-C2 byte-symmetric; C7 = overall parity (`Syn7`).
- **Corrector + Pipe4 (MemD sheet 06):** `SingleError = odd syndrome
  parity`, `DoubleError = syndrome nonzero ∧ even parity`, `NoError =
  syndrome 0`. SE one-hot → a check bit failed; SE multi-bit → a data bit
  (word = Syn4-6, bit = Syn0-3 reversed). Pipe4 write rule: "always write on
  first quadword; subsequently don't write if seen-DE, or seen-error and
  this isn't one" = **report first DE, else last SE** (HM line 2853).
- **Canonical H-matrix: HM Figure 11** (`figures/figure-11-error-
  correction.png`) — the 64×8 grid of which data bits each check bit XORs.

### What the emulator does today
- Storage is a flat `uint16_t*` with no check bits (`memory.h:220`); no
  syndrome computed anywhere; `LoadTestSyndrome` is a stub (`cpu.c:1093`);
  `pipe4_syndrome`/`pipe4_quadword` always 0. Pipe4 error bits set only for
  MapTrouble.

### Spec to implement
1. **Storage layout:** keep data words flat, but add a parallel
   `uint8_t check[quadword]` array (8 bits per 4-word quadword), or compute
   check bits on the fly from a transcribed H-matrix.
2. **Transcribe HM Figure 11** at full resolution into a 64×8 bit table
   `Hmatrix[bit][checkbit]` (do this from the figure, not the photostat).
   Generator: `C[k] = XOR over data bits d where Hmatrix[d][k]`, with C7
   complemented.
3. **TestSyndrome register** (FF=0o... LoadTestSyndrome, `cpu.c:1093`): on a
   store, XOR `TestSyndrome` into the written check bits. On a later read of
   good data the syndrome equals `TestSyndrome` — the primary self-test
   path.
4. **On read:** recompute check bits over the retrieved quadword, XOR the
   stored check bits → 8-bit syndrome. Decode: 0 → no error; odd parity →
   SE (correct the located bit if EC enabled, i.e. `DisableEc'`=1); nonzero
   even → DE → set `PIPE4_ERR_EC_FAULT`/`MemError`, no correction.
5. **Pipe4:** populate `pipe4_syndrome` (b8:15) and `pipe4_quadword`
   (b6:7 = which of the 4 quadwords, 0..3 — **not** "16-word pair"), apply
   the "first DE / last SE" write rule per slot. Flip the `not*` error bits
   to 1=OK polarity and adopt baseline `0150361` (QW4).
6. **DisableEc** from `mcr`/`Dbuf.08` disables correction.

### Priority note
**P2 / fidelity-only** unless an emulator world actively scrubs memory or
runs the memory self-test. No bit errors are injected in normal running, so
syndrome=0 is currently the *correct* answer; the value is enabling Midas /
memory-diagnostic microcode and the Verilog port. Implement TestSyndrome
(step 3) first — it is the cheapest piece and unblocks the memory self-test.

### Files
`memory.c`/`memory.h` (storage check bits, syndrome compute, Pipe4
population), `cpu.c:1093` (LoadTestSyndrome).

---

## HS7 — Disk data-transfer read path: sequence PROM + Fire Code ECC + FIFO/status (gaps F1-F5). **P1**

Needed to read a real Trident pack once an OS boots (and as a fallback boot
route). The disk-architecture detail is in `docs/disk-architecture.md`; this
spec adds the schematic-confirmed concrete numbers.

### What the hardware is / does
- **Sequence PROMs (DskEth sheet 09):** two `SG139` PROMs (a20 read / a21
  write), addressed by a 5-bit PC, decoding to `ShiftIn/ShiftOut/ComputeECC/
  NextBlock/LoadTag/LoadCnt/Tag←Ram/CntDone'`. A 12-bit format-RAM-loaded
  down-counter ticks `WordClock'`; `CntDone'` advances the PC. **The full
  32-step read and write step tables are in HM §9.6** (head-select →
  preamble skip → issue read → [stop until sync word] → data words → 2 ECC
  words → next block …). Format RAM = 16×12.
- **Fire Code ECC (DskEth sheet 16):** a 32-stage LFSR. Polynomials
  silk-screened: **write** `P(X)=X^32+X^23+X^21+X^11+X^2+1`; **read** split
  into `X^11+X^2+1 → ECC[0:10]` and `X^21+1 → ECC[11:31]`. Write feedback
  `fb = data_in XOR reg[31]`, XOR `fb` into reg bits {0,2,11,21,23}. Read
  mode splits into two registers (11-bit: XOR fb into bit0,bit2; 21-bit: XOR
  fb into bit0); both zero ⇒ no error. Recovery math (Chinese-Remainder,
  E0=21/E1=2047/A0=19/M0=2047) in HM §9.10. Sync word: `0o201` standard /
  `0o001` Alto-Trident.
- **FIFO (DskEth sheets 14/15):** 16 deep × 20 bits = 16 data + 2 byte-parity
  + 2 type bits (write / read / read-and-check). `WriteTW`/`ReadTW`
  thresholds (≥3 words normal read, ≥1 read-and-check) and Over/Underflow FFs.
- **Sector timing (DskEth sheet 12):** per-drive LS169 down-counters loaded
  with the subsector count; index vs sector pulses discriminated by width
  (sector ≈1.2µs decrements, index ≈4.5µs reloads, 3.0µs one-shot
  threshold); 117 subsector jumper.
- **Status mufflers (DskEth sheet 18):** KSTATE 000-017, KSTAT 020-037
  (SeekInc/HeadOvfl/DevCheck/NotSelected/NotOnLine/NotReady/SectorOvfl/Fifo
  Under-Overflow/ReadDataErr/ReadOnly/CylOffset/IOBParityErr/FifoParityErr/
  Write-ReadErr), KRAM 040-057, KTAG 060-077, KFIFO 100-117 (Midas reads +2000).

### What the emulator does today
- `dorado_disk_controller_refill_fifo` (`disk.c:245-276`) dumps
  header+label+data contiguously (1036 words) — no sync/ECC words, no
  per-block sequencing. FIFO is bare 16×16 (`disk.h:194`) — no parity/type.
  KSTAT 020-037 and KRAM/KTAG/KFIFO 040-117 return 0 (`disk.c:623-672`
  decodes only KSTATE 001-017). Sector advance poked synthetically
  (`disk.c:335-376`). No ECC (`disk.h:52-54` TODO).

### Spec to implement (incremental — try the cheap path first)
1. **First, try the existing short-circuit against Mesa.** Per handoff §3,
   Mesa may trust Format-RAM timing and accept the contiguous dump. If it
   faults, proceed.
2. **Read-stream framing:** make `refill_fifo` emit the per-block structure
   the read loop expects: discard 1 sync word, N data words, then 2 checksum
   words to discard, then the ECC remainder — block-by-block with the `Done`
   terminator, driven by the read-PROM step table (HM §9.6) and the
   format-RAM word counts.
3. **Fire Code ECC:** implement the LFSR with the tap positions above;
   compute the 2 ECC words on read (or write) so status reflects
   `ReadDataErr` correctly. Recovery (single-burst correction) only if Mesa
   faults on a deliberate error — otherwise generate-and-check is enough.
4. **FIFO:** add the 2-bit type field and the WriteTW/ReadTW thresholds
   (`rd_fifo_tw` should reflect ≥3 / ≥1 words by type, not just nonzero,
   `disk.c:270`); add Over/Underflow flags.
5. **Status:** decode KSTAT 020-037 (the error summary bits) and at least
   KFIFO/KTAG/KRAM readback so status-polling microcode reads real values.
6. **Write side (F3):** commit the FIFO back to the pack on the write-PROM
   sequence (`disk.c:506` currently just sets `WrFifoTW`).

### Priority note
**P1** for booting/running a real OS from disk; the Ethernet boot route
(HS8) is the primary path, so disk is the second priority. Steps 1-2 are
the boot-relevant minimum; ECC recovery and write are later.

### Files
`disk.c`/`disk.h`.

---

## HS8 — Ethernet controller (gap H1). **P1**

The chosen boot route is Ethernet, but the emulator currently uses an
in-process Pup/EFTP fake (`ethernet.c`) plus a probe-only `.eb` injector — a
real controller would let the Mayday/EFTP software boot run for real.

### What the hardware is / does
- **Registers (DskEth sheets 02/03/35/38):** TIOA 15=EthD (data), 16=EthC
  (control write / status read); tasks 6=EOT (output), 7=EIT (input).
  **EthC control bits** (already matched by `ethernet.h:23-31`): Tx field
  B00 TxCmdEnbl→B01 TxOn/B02 TxEOP/B03 TxCntDwn; Rx field B04 RxCmdEnbl→B05
  RxOn/B06 RxBOP'; Test field B08→B09 LoopBack/B10 SingleStep/B11 NoWakeups/
  B12 TestClock/B13 TestColl'/B14 TestData/B15 ReportCollisions.
  **EthC status read** (NOT implemented): bits 0-7 = host address (backpanel
  pullups), 8 RxOn, 9 TxOn, 10 LoopBack, 11 TxCollision, 12 NoWakeups, 13
  TxDataLate, 14 SingleStep, 15 TxFifoPE.
- **Datapaths (sheets 26-34):** receiver = PhaseDecoder PROM → Rcvr-Control
  FSM → RxSR (18-bit, RxSR.18=EOP) → 16-deep RxFifo → RxBusReg → EthD; CRC
  via F9401. Transmitter = TxBusReg ← IOB → 16-deep TxFifo → TxSR →
  PhaseEncoder → TxData; CRC via F9401; Tx-Control FSM (TxStart/TxEnd/
  TxAbort on collision/FifoPE/DataLate).
- **Wakeups/IOAtten (sheets 28/29/36):** `WakeEthRx = HaveBlocked ∧
  RxBusRegFull ∧ RxOn ∧ RxBOP ∧ ¬NoWakeups ∧ ¬(Next/Curr=EthRx)`;
  `WakeEthTx = HaveBlocked ∧ TxBusRegEmpty ∧ TxOn ∧ ¬TxEOP ∧ ¬TxCntDwn ∧
  ¬NoWakeups` (these match the existing `dorado_ethernet_wakeup_mask`).
  `IOAtten = (Next=EthRx ∧ EthData.18/EOP) ∨ (Next=EthTx ∧ ¬TxAbort)`.

### What the emulator does today
- `ethernet.c` (1306 lines) is a Pup/EFTP boot server; `ethernet.h` models
  the control-register bits + wakeup mask correctly but there is no EthD bus
  register, no Rx/Tx FIFO, no CRC, no status read, no IOAtten.

### Spec to implement
1. **EthD bus register + 16-deep Rx and Tx FIFOs** (mirror the disk FIFO
   model). Reading EthD pops RxFifo→RxBusReg; writing (Output←B on EOT task)
   pushes TxBusReg→TxFifo.
2. **F9401 CRC-16** appended on Tx, checked on Rx (→ RxCRCError status bit
   in the trailing status word, with `EthData.18`=RxEOP).
3. **EthC status read** with the bit layout above (host address from a
   configurable constant).
4. **Wakeups/IOAtten:** keep the existing wakeup conditions; add IOAtten so
   microcode can distinguish "another word" from "ending status / Tx
   aborted."
5. **Framing:** deliver a received frame as data words then CRC word then a
   status word (RxEOP/RxCollision/RxDataLate/RxCRCError). Wire the existing
   Pup/EFTP fake as the *medium* behind this controller (the fake produces
   the byte stream; the controller shifts it through the FIFOs), removing
   the probe-only injector.

### Priority note
**P1.** The boot already works via the injector; a real controller removes
the `DORADO_ETH_FORCE_ELOAD_ZERO` and `DORADO_ETHER_BOOT_IMAGE` probe guards
(handoff H1/I2) and makes the EFTP software-boot (Stage 2) run through the
actual register/FIFO/wakeup path — which is the bring-up target.

### Files
`ethernet.c`/`ethernet.h`, plus the slow-IO (EthC) and fast-IO (EthD)
dispatch in `io.c`/`fastio.c`.

---

## HS9 — StkError HOLD + fault-task wake + instruction back-out (Finding 11, extends gap). **P2**

### What the hardware is / does
- **ProcL sheet 24 (page 26):** `OVFLerr'/UFLerr'` OR into **StkError**
  *and* **PrHoldReq**. HM (md 730-741): StkError generates **HOLD** and
  **wakes the fault task (task 15)**; the offending instruction **has not
  been executed** when the fault task runs (StkP and the RM/STK write are
  backed out and the instruction re-runs).
- **MemX sheet 03:** `StkError → StkWake` OR'd into `TWReq15` (the same
  fault-task wakeup as memory faults).

### What the emulator does today
- `stk_apply_post` (`cpu.c:444-470`) only sets sticky `stk_und`/`stk_ovf`;
  no HOLD, no task-15 wakeup, and it **commits** the StkP update on error
  (`cpu.c:467`). `stk_apply_post` comment already admits "we don't model the
  HOLD + fault-task wake."

### Spec to implement
1. On StkP overflow/underflow detect: assert the same Hold path as HS1
   (re-run the instruction), raise `wakeup_pending |= 1<<15` (matching the
   memory-fault wake at `cpu.c:3412`), and **suppress** the StkP update and
   the RM/STK write for that instruction.
2. Fix the StkP arithmetic at the boundary: add the sign-extended RSTK[1:3]
   to the full 8-bit StkP so a carry ripples into the region bits (ProcL24),
   instead of wrapping the offset mod 64 (`cpu.c:453`).

### Priority note
**P2** until a stack-using emulator (Mesa) actually overflows; depends on
the HS1 Hold path and the fault-task entry path existing.

### Files
`cpu.c` (`stk_apply_post`, the commit barrier, the wakeup raise).

---

## Summary table

| Spec | Subsystem | Gap | Priority | Depends on |
|------|-----------|-----|----------|-----------|
| QW1  | Keyboard back-channel bit position | E2 | **P0** | — |
| QW2  | DWTFLAG IOFetch-signal case | E1 | P1 | — |
| QW3  | Pipe4 MapTrouble on vacant/WP | C2 | P1 | — |
| QW4  | Pipe4 baseline 0150361 | — | P2 | HS6 |
| QW5  | ASRN advance only on map-start | — | P2 | HS3 |
| QW6  | SubTask into MemBase | — | P2 | — |
| QW7  | Overflow for all 5 arith ops | — | P2 | — |
| HS1  | Cache Miss/Hold + Hold semantics | B1/C1 | **P0/P1** | — |
| HS2  | Cache VNV victim/next-victim | (F3) | P1 | — |
| HS3  | Staged memory pipeline / SRN hand-off | C1 | P1/P2 | HS1 |
| HS4  | DDC terminal back-channel + boot keys | E2 | **P0** | QW1 |
| HS5  | IFU F/G/H/J pipeline timing | B9 | P2 (TwoAlpha P1) | HS1 |
| HS6  | MemD ECC / Hamming + Pipe4 syndrome | C2/C3 | P2 | — |
| HS7  | Disk read path: PROM + Fire Code + status | F1-F5 | P1 | (HS1 for timing) |
| HS8  | Ethernet controller | H1 | P1 | — |
| HS9  | StkError HOLD + fault wake + back-out | (Finding 11) | P2 | HS1 |

**Boot-critical path (do in this order):** QW1 → HS4 (lets the user select
an emulator and removes the GotBootKey probe hack) → HS1 Phase A (lets the
selected emulator world actually run instead of spinning on early Md) →
HS8 (real Ethernet so Stage-2 software boot runs through the hardware path)
→ QW2/QW3 (display + fault fidelity for first pixels) → HS7 (disk, for a
real OS volume). Everything else is fidelity / Phase-2 (Verilog) groundwork.
