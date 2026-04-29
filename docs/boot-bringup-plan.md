# Boot bring-up plan

A staged plan for getting from "Boot0 starts free-running from IM"
(where we are now) to "Mesa loads Pilot from disk and a hello-world
appears on the display."

This is a *design doc*. It captures decisions we want to make
deliberately rather than rediscover under pressure later. Update it as
phases land.

## Current state (anchor)

`probe_full_boot` in `tests/test_cpu.c` runs the BB and the Dorado
microengine tick-by-tick from cycle 0 with empty IM. The BB cold-boot
→ LoadDoradoCode → Boot0-injection-and-Write-IM path works
end-to-end. After Boot0 is loaded into IM[0o7700..0o7777] and Return#
is jammed without single-step, the Dorado free-runs from IM[0o7740]
and executes ~10 microinstructions before walking into IM[0o7744]
(an all-zero trap-reservation slot in the EPROM data); the embedded
long-jump goes to IM[0o4000] which is empty → halt.

What works
- Microinstruction decode, RM access, T/Q/Cnt/ShC, JCN (all forms
  except IFU jump), basic shifter, FF (subset).
- BaseBoard 6502 with full RIOT/timer/IRQ model.
- BB↔Dorado handshake: MCPBus strobes (Control/Clock/ABMux/MIR),
  CPReg streaming, MIR injection + SetSS single-step, Run/Halt gate.
- Real Write IM, BLOCK=1 STK stub.

What's stub-or-missing
- **STK** — `rm_address` returns `STK[StkP]` for any BLOCK=1 access;
  no push/pop semantics, no RSTK[0:3] sub-decode.
- **Memory subsystem** — Md returns 0; Fetch/Store/IFetch/PreFetch
  no-op; no Map; no cache; no Pipe; no BR; no faults.
- **ALUFM** — only 16 entries pre-seeded by tests; the firmware-
  injected `ALUFM[0]FromQ` only writes `[0]`; entries 1..14 stay
  zero. Several real ALUFM ops (notably arithmetic ops outside
  the *0/*6/*14/*22/*36 set) aren't in the alu_op switch.
- **Hold** — not modeled at all; references that *should* hold the
  microengine just produce 0 on Md.
- **IFU** — completely absent; IFUJump halts.
- **Tasking** — single-task only; no wakeup, no T/TPC/MemBase/Link
  replication, no FreezeBC.
- **I/O** — no Display, Disk, Ethernet.

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

**Status:** A.1–A.6 LANDED. A.7 (Boot1 ACK) **blocked** — Boot0
runs a wait-loop polling for an AMSync/MASync edge on CPReg before
entering the Boot1-load loop. Without modeling those sync bits in
the CPReg latches (and the corresponding read paths the BB-side
exercises during SendIMBlockToDorado(ViaCP=1)), Boot0 falls through
its wait-loop into the trap-reservation slot at 0o7744 and long-
jumps to IM[0o4000] — empty, halt. The wait loop itself is now
visible in the trace (PC walk: 7740 → 7761 → 7747 → 7740, with
RM[1] driven by the shifter as a soft counter), so the
infrastructure is correct; what's missing is the AMSync/MASync
hardware modeling. Defer to a sub-phase A.7 when we revisit.

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

1. **Flat 4M-word storage array** in `src/memory.c`. No Map, no
   cache, no faults — just `mem[VA & 0x3FFFFFF]`.
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
  after Fetch unless cache hit on a deferred reference).
- StkError, Pipe full, FreezeBC, IFU map fault concurrent with
  IFUJump, etc.

For Boot0 we probably don't *need* Hold at all (single task, deferred
references). But the FreezeBC FF function needs to actually hold
branch conditions for one cycle, otherwise R<0 at PC=0o7707 evaluates
post-LC instead of pre-LC.

We've already got the "evaluate next_pc before apply_lc" fix that
covers the common case. FreezeBC adds a one-cycle latch where the
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

### B.1 BR (Base Registers)

32 × 28-bit registers. Loaded via:
- `BrLo←A` (FA=1 FB=2 FC=3): BR[MemBase][16:31] ← A[0:15].
- `BrHi←A` (FA=1 FB=2 FC=4): BR[MemBase][4:15] ← A[4:15] +
  some sign-extension.

Read: `VA = BR[MemBase] + Mar` for Fetch/Store; read on Pipe.

Tests: `test_br_load_consistency`, `test_br_pertask` (BR is shared
across tasks, contrary to T/TPC/MemBase/Link which are per-task).

### B.2 Map

16K- (or 64K-) entry × 19-bit table. VA[10:23] indexes; entry
contains a 12-bit real page + flags (W=writeable, D=dirty,
R=referenced, ECC enable). Loaded via `Map←` reference (ASEL
encodes a Map write); read via `ReadMap` FF function.

Page faults when entry's W bit is 0 on a write, etc. Map fault
microcode lives at IM trap address *0-3 (per HM Table 14).

Tests: `test_map_load`, `test_map_fault_write_to_ro`,
`test_map_referenced_dirty_bits`.

### B.3 Cache

4 K-words organized as 64 rows × 4 columns × 16-word lines. Hit
on VA[4:19]. Tags + V/D bits per line. Replacement: write-back
(victim eviction on miss).

The tricky parts:
- 28-cycle storage latency (cache miss). Determines Hold time.
- Victim writes happen lazily.
- Cache hits on deferred references don't Hold (HM page 37).
- Flush← removes a line without storing.

Tests: `test_cache_hit_miss`, `test_cache_dirty_writeback`,
`test_cache_flush`.

### B.4 Pipe

16-entry ring buffer of pending storage references. Each entry
records VA, kind (Fetch/Store/Map/IFetch/IOFetch/IOStore/Flush),
and trap info. Read by microcode via `B←Pipe0..5` FF functions
(FA=1 FB=6 FC=0..5). The ring's tail advances every storage cycle.

Tests: `test_pipe_records_va`, `test_pipe_wraps`,
`test_pipe_records_iotype`.

### B.5 Faults

Per HM Table 14: page faults, cache parity, ECC errors, IFU map
fault, etc. Each traps to a reserved IM address. The microcode
sees the trap reason via `B←FaultInfo'`.

Tests: induce each fault type, verify trap entry + FaultInfo.

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

### C.1 IFUM — 1024 × 24-bit decode RAM

Indexed by `(InstructionSet[2] || Opcode[8])`. Each entry is
24-bit, broken into 8 sub-fields per HM Table 18:
N (entry-vector index), TJump, TPause, MemB, NotRBaseB, NotLength,
IPar, Sign, PA bit 6, NotIFADr2.

Loaded by microcode via `IFIMRH←B` / `IFUMLH←B` FF functions.

### C.2 Instruction prefetch

The IFU runs in parallel with the processor. It prefetches the
next opcode against base register 31, decodes via IFUM, and caches
4 entry vectors (`IFUJump[0..3]`).

Read by microcode via `B←IFUMRH'` / `B←IFUMLH'` (for inspection)
and by JCN=`IFU Jump` (the dispatch that pops the prefetch).

### C.3 IFUJump

Per HM §6.2 + Figure 6's IFU Jump encoding: TNIA = CIA[2:3] ||
InstrAddr[4:13] || JCN[3:4]. The InstrAddr comes from the IFU's
prefetch slot N (N = JCN[3:4] from this instruction's JCN).

When prefetch hasn't completed, IFUJump traps to the "IFU not
ready" addresses (*34-37 per Table 14). Conditional IFUJump (with
FF-encoded condition) further allows the IFU to *not* advance on
the false branch.

### C.4 Opcode advancement

`TIsId` / `RIsId` in FF (FA=1 FB=3 FC=4..5) advances the IFU one
opcode and replaces a register with the operand byte. Without
this, the emulator can't progress through bytecode.

Tests: `test_ifum_decode`, `test_ifu_jump`, `test_ifu_prefetch_miss`,
`test_ifu_advance`. Validate by running AEmu.mb against a tiny
synthetic Alto program (NOP loop + halt opcode).

**Exit criterion for Phase C:** AEmu.mb executes the first 100
Alto opcodes from a games.dsk image (Alto-emulator-on-Dorado).

## Phase D — Tasking

**Goal:** I/O devices in Phase E can wake their tasks and the
microcode for them gets dispatched.

HM §4.1, §4.2, Table 22.

### D.1 Per-task state replication

T, TPC, MemBase, Link all become 16-element arrays indexed by
current task number. The Q register and ALUFM are *not* per-task
(only emulator can use Q).

`cpu->task_t[16]`, `cpu->task_tpc[16]`, `cpu->task_membase[16]`,
`cpu->task_link[16]`. The "active" view (cpu->T etc.) is written
back on task switch.

### D.2 Wakeup latches + priority encoder

16 wakeup signals from I/O devices. Highest-priority pending
wakeup with a non-locked task wins. Locked tasks are implementation-
specific (HM §4.2).

### D.3 Task-switch sequencing

Switches happen at the end of every microinstruction. The PC for
the next instruction is the new task's TPC; T/MemBase/Link are
restored from the new task's saved state. The previous task's T
etc. are saved.

Hold complicates this: a held instruction repeats with the *same*
task, but if the held task is at lower priority and a higher one
wants to run, the higher one runs first (per HM "Hold" remarks).

### D.4 LdTPC / RdTPC

Load/read the saved TPC of any task. Per HM §4.7. Used by Initial
to set up emulator entry points.

### D.5 IOAtten' / Reschedule conditions

Branch conditions 6 (cond 0o66 in FF) — IOAtten' for non-emulator
tasks, Reschedule for emulator. We currently stub IOAtten' = 1
(no I/O attention). Once tasking lands, IOAtten' becomes
"I/O wants attention for this task".

Tests: `test_task_switch`, `test_task_locked`, `test_ldtpc_rdtpc`,
`test_wakeup_priority`.

**Exit criterion for Phase D:** AEmu.mb + a stub disk task can
read one sector of an Alto disk image. Mostly: prove that I/O
microcode actually gets dispatched.

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
