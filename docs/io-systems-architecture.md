# Dorado I/O subsystems — reference

What we'll need to build to get past Boot0, then to bring up Display,
Disk, Ethernet, and the BaseBoard's device-side handshake. Companion
to `docs/memory-architecture.md`. **Almost nothing here is yet built**
— this is the design plan and the parts list.

Sources: HM §7 (Slow I/O, pp. 86–89), §8 (Fast I/O, pp. 90–91), §9
(Disk Controller, pp. 92–112), §10 (Ethernet), §11 (Display
Controller, pp. 113–120), §4.1–4.2 (Tasking).

---

## At a glance

```
   Microcode in IM
        │  TIOA[0:7]        IOB[0:15] (data + parity)
        │  (per task)         │
        ▼                     ▼
   ┌────────────────────────────────────┐
   │       Slow I/O bus (IOB)            │   shared, 16+2 bits, ~5-cycle txn
   │       Output←B   /   Pd←Input       │
   └──┬──────────┬─────────┬─────────┬───┘
      │          │         │         │
      ▼          ▼         ▼         ▼
   Disk      Display    Ether    BaseBoard / CPReg
   (DSK 14₈) (DDC)      (EOT 6,  (CON 1, JNK 2, FLT 17₈)
                          EIT 7) 
       │                        │
       │ IOFetch← / IOStore←    │ same
       ▼                        ▼
   ┌────────────────────────────────────┐
   │   Fast I/O busses (Fin, Fout)       │  18 bits each, 16-word munch
   │   memory ⇄ device, no processor in  │
   │   the data path                      │
   └────────────────────────────────────┘
            ▲                     │
            │ wakeup              │
   ┌────────┴─────────────────────┘
   │  Task scheduler — 16 tasks, priority by index, hardware preempt
   │  Per task: T, TPC, MemBase, RBase[2:3]+SubTask, Link, Wakeup latch
   └──────────────────────────────────────
```

Three independent things tangle here:

1. **Slow I/O** — TIOA + IOB, all device control happens here.
2. **Fast I/O** — Fin/Fout, only data, used by Disk/Display/Ether.
3. **Task scheduler + Wakeup/Block/Next protocol** — the cycle-by-cycle
   plumbing that makes microcoded I/O work. Cannot be skipped.

The disk, display, and ethernet controllers are built **on top of all
three**. The BaseBoard talks via slow I/O only.

---

## 1. Slow I/O (HM §7)

### Bus shape

- **TIOA** — 8-bit, **task-specific**: each task has its own copy. The
  current task's TIOA decodes to "this device." Loaded by either:
  - `TIOA←B` (FF function): low 8 bits from B[0:7], at t₂.
  - `TIOA←small constant` (FA=2 alt encoding): TIOA[5:7] from FF[5:7]
    while preserving TIOA[0:4]. Used for "next register on the same
    device."
- **IOB** — 16-bit data + odd byte parity (2 bits). Shared bus.
- **Pd←Input** — read IOB into Pd; checks parity, raises a fault on
  bad parity. Used after TIOA points at an input register.
- **Pd←InputNoPE** — read IOB without parity check. Used to *probe*
  whether a device responds (a missing device leaves IOB floating
  high → parity bad).
- **Output←B** — drive 16 bits + parity onto IOB to the register
  selected by TIOA. Many controllers parity-check the data and report
  errors as status.

Sequencing rule (HM p. 86, "Input/Output Functions"):

> `Pd←Input`, `Pd←InputNoPE`, or `Output←B` functions can be issued in
> the instruction immediately following the one that loads TIOA.

So a typical access is two cycles: load TIOA, then read/write.

Floating-bus default: with no device asserting, IOB pulls high. Real
microcode uses this as a presence test — a "probe" reads
`Pd←InputNoPE` and treats `0xFFFF` (or bad-parity) as "no device." Our
`cpu.c` returns `0xFFFF` for both Input variants when no device is
mapped (matches HM §7).

### Task assignments (Table 22)

This table is canon. Reproduced verbatim:

| Number | Name | Comment                                            |
|-------:|------|----------------------------------------------------|
| 0      | EMU  | The emulator                                       |
| 1      | CON  | Special task for restarting emulator after faults  |
| 2      | JNK  | Junk task (awakened every 32 µs)                   |
| 3      | DHT  | Display horizontal task                            |
| 4      | AHT  | DispM terminal interface horizontal task           |
| 6      | EOT  | Ethernet output task                               |
| 7      | EIT  | Ethernet input task                                |
| 11₈    | AWT  | DispM terminal interface word task                 |
| 12₈    | SIM  | Task simulator                                     |
| 13₈    | DWT  | Display word task                                  |
| 14₈    | DSK  | Disk i/o                                           |
| 17₈    | FLT  | The fault task                                     |

Tasks 5, 10₈, 15₈, 16₈ are unassigned. **Task numbers are the priority
order** — lower number = higher priority? No: HM §4.2 makes the highest
*ready* task win; FLT (17₈) is highest priority because faults must
preempt. EMU is lowest (0) because everything else preempts it. The
runnable-with-highest-priority arbitration is what the scheduler must
implement.

### Wakeup, Block, Next (HM pp. 87–88)

This is the cycle-accurate handshake between an I/O device and its
task. Get this wrong and microcode deadlocks; we can't paper over it.

**Latency invariant.** A wakeup at t₀ → first instruction of that
task at t₄. **Two cycles minimum** elapse between wakeup assertion
and the task's first instruction. The task then runs until it does
`Block`. *To prevent an erroneous extra wakeup, the device must lower
its wakeup at least one cycle before the task issues Block.*

**Three turn-off strategies** (a device picks one, microcode must
match):

1. **Next-driven.** Device watches the `Next` bus; when `Next == its
   task#`, lower wakeup at t₀. Task must NOT block until the second
   instruction. Special case: "Next Lies" — if a task blocks with a
   memory reference and is held by cache miss, the held-instruction
   re-execution falsely broadcasts the held task on Next; controllers
   detect this and ignore.
2. **TIOA-driven.** Device watches its own TIOA; lower wakeup at t₀ of
   the *second* instruction after wakeup. *Disk controller uses this.*
   Side effect: ANY task that loads that device's TIOA value will
   trigger turn-off — all tasks must initialize TIOA carefully.
3. **Output/Input-driven.** Device waits for `Output←B` or `Pd←Input`
   to its address; resets condition at t₃/t₅, lowers wakeup at t₄/t₆.
   *Disk uses this for transfers, Display horizontal uses it.* Task
   must not block until the third or fourth instruction.

**Pathological "Next Lies":**
> Suppose a task blocks with: `Branch[Loop], Fetch←Address, Block;`.
> If Fetch← causes hold and Bnt < Ctask, no task switch occurs but
> Next still broadcasts Bnt. Hold happens after t₁, leaving no time
> to revert Next. **Consequence: branch-on-IOAtten is illegal in any
> instruction that blocks and might be held** (HM p. 87 + §7 illegal
> #2).

### SubTasks (HM p. 88)

Two-bit value the device drives onto the SubTask bus when `Next ==
its task#`. Clocked at t₀ into per-section flipflops. Then:

- **Processor**: `SubTask[0:1]` ORs into both `RBase[2:3]` and
  `MemBase[2:3]`. Lets one block of microcode service N identical
  controllers, each with its own RM region and pair of MemBase
  registers. With only SubTask[0] driven: 2 RM regions × 4 MemBases
  per task. Caveat: 16-change-RBase-for-write functions do *not* OR
  in SubTask, so they can't be used for per-subtask state writes.
  Reading RBase reflects the OR'd value, but the 16-change-RSTK-for-
  write functions DO work.
- **Memory section**: SubTask is bussed to fast-output devices on the
  Fin/Fout transport. The receiving device identifies itself via
  task||SubTask.
- **Debugger override**: when BaseBoard or Midas asserts `Freeze`,
  SubTask's effect on RBase[2:3] is *disabled* but its effect on
  MemBase[2:3] continues. Debugging quirk we'll need to model when
  implementing the BB→Dorado debug path.
- **Single TPC**: all SubTasks of a task share one TPC. Microcode for
  multi-subtask tasks must be coded as if the wakeup were a priority
  interrupt — it always re-enters at the same point.

### Illegal things (HM p. 89)

These are constraints microcode must obey; we don't need to enforce
them in the emulator (correct microcode will be correct), but we do
need to make sure our implementation doesn't *accidentally* mask their
effects:

1. Block in an instruction with `B←ExternalSource` (other than IFU
   sources) — illegal so emulator can compute on `B←PCX'`.
2. Branch on IOAtten in an instruction that blocks and might be held
   (NextLies hazard).
3. A task may not Block on an instruction that might be held if its
   wakeup might drop at t₀ — the held instruction would repeat after
   the wakeup is gone.
4. Block with TaskingOff in force.
5. A task must not Block until one cycle after its wakeup is turned off.
6. Wakeup[n] is illegal if task n might run next cycle — wrap with
   TaskingOff in such cases.

### IO opcodes (Mesa)

Mesa exposes two opcodes that bypass firmware drivers — `INPUT` and
`OUTPUT`:

```
INPUT:  TIOA←α; Stkp←Stkp+1; Stack←Input, IFUJump[0];
OUTPUT: TIOA←α; Output←Stack&-1, IFUJump[0];
```

Used for diagnostics and registers not driven by firmware. We need to
support these eventually; trivial once the slow I/O bus is wired.

### What we need to build (slow I/O)

**Status**: routing layer **built**. See `dorado/include/io.h` +
`dorado/src/io.c`. `dorado_io` holds a sparse `[task][TIOA]` table of
`dorado_io_device` callbacks (`read`, `write`, `ctx`, `name`). cpu.c's
Pd←Input / Pd←InputNoPE / Output←B dispatch through it. Default
(no device mapped) returns `0xFFFF` with `bad_parity=1`, matching
HM §7 floating-bus semantics. `test_slow_io_routing` exercises the
round-trip with a registered echo device.

**Important Boot0 finding** (probe_full_boot disasm scan, 2026-04):
The BB-loaded Boot0 contains **zero** slow-IO operations in
IM[0o7700..0o7777]. The Boot0 dead-end at 0o7744 (empty trap slot)
turned out NOT to be a slow-IO issue. Subsequent investigation of
**Bootstrap.MB** (the source for the BB ROM's Boot0) showed Bootstrap
runtime-initializes its own ALUFM via `Pd←ALUFMRW`, and the trail
crash was caused by a bit-mapping bug in our `Pd←ALUFMRW` handler
(HM Table 11d): cpu.c was mapping the manual `B[8],B[11:15]` →
`ALUFMEM[5..0]` field with the bits reversed, so runtime ALUFM
writes produced garbage values. With that fixed, `probe_bootstrap_pure`
runs Bootstrap.MB end-to-end through its READBB loader pattern.

The BB-loaded Boot0 (probe_full_boot) is a **different binary**
than Bootstrap.MB — it's the post-mbtobase output that the BB
flashes into EPROM, and it has different bytes / different path.
Slow-I/O still doesn't appear in it; the divergence is somewhere
else and remains an open investigation. **Slow-I/O is still
infrastructure prereq for Display, Disk, Ether — not the actual
Boot0 blocker.**

Still TBD:

- **Per-task TIOA register storage**. Currently `cpu->TIOA` is a
  single 8-bit value shared across all tasks. Should be 16 copies
  saved/restored on task switch (HM §7 "TIOA is task-specific").
- **The Wakeup/Block/Next protocol implementation** in the task
  scheduler. Needs `Block` to clear the running task's wakeup latch,
  `Next == ctask` broadcast at t₀, hold-handling for "Next Lies."
- **SubTask muxing** into RBase/MemBase + Freeze override.
- **BB-side TIOA stubs** for fault registers, manifolds, etc. (when
  we actually identify which addresses microcode reads).

---

## 2. Fast I/O (HM §8)

The fast I/O system moves blocks of data between main storage and
device, **with no processor in the data path**. Used by Disk, Display
DDC, Ethernet, and any other high-bandwidth controller.

### Transfer unit: the munch

- **Munch** = 16 words at addresses `i, i+1, …, i+15`, with `i mod
  16 = 0` (i.e. cache-line aligned).
- **One word per Dorado clock** during transport → 16 clocks per
  munch, 533 × 10⁶ bits/s peak.
- One transaction = one munch. Transactions are independent of each
  other except that storage references happen in issue order.

### Initiation

- `IOFetch←` (from storage to device) and `IOStore←` (device to
  storage) are coded in **ASEL** (HM Table 6, ASEL=6 / 7 — values
  TBD, confirm against HM Table 6 when wiring). Once issued, the
  reference proceeds without further processor interaction except
  for fault reporting.
- Each fast device is *also* on the slow bus — that's where it gets
  its control info, since fast bus has no path back to the processor.

### Transport

- **Fin bus** = IOFetch← words, memory → device.
- **Fout bus** = IOStore← words, device → memory.
- Both are **18 bits**: 16 data + 2 parity (one per byte).
- Independent — Fin and Fout transports can overlap.
- Each carries `Task` + `SubTask` + `Next` lines so the device that
  issued the IOFetch/IOStore can identify "its" data on the bus. The
  Fout bus also carries a `Fault` line, raised on the last word if a
  storage fault was caught.
- A device must **track the number of wakeups it has issued** because
  data may not arrive for several µs after IOFetch (no flow control
  back).

### Latency

Worst case (HM p. 91): first word delivered ~24 cycles after
IOFetch starts; full munch transferred over 23.5 cycles. **Total
worst-case = 49.5 cycles** for an IOFetch, **40 cycles** for an
IOStore. Lower-priority tasks can be delayed further by 8 cycles per
intervening higher-priority reference.

These numbers matter because:
- Cache fill bandwidth limits — we can pipe IOFetch every 8 cycles
  for 16 KW modules, every 13 cycles for 4 KW modules.
- Display word task (DWT) microcode is tuned to this cadence.

### What we need to build (fast I/O)

**Status: DONE (synchronous-transport variant).** `dorado/include/
fastio.h` + `src/fastio.c` provides:

- A `fast_io_cb` callback on `dorado_memory` that fires on every
  IOFetch / IOStore. Memory hands a 16-word munch buffer to the
  callback (filled from storage on IOFetch, to-be-filled by the
  device on IOStore).
- `dorado_fastio_dispatch` routes by task ID:
  - DWT (13₈) → push to display FIFO (channel selected by subtask)
  - DSK (14₈) → push to / drain from disk controller FIFO
- New `dorado_memory_ref_task()` takes (task, subtask) so the
  dispatcher knows which device to hand the munch to. cpu.c now
  uses this on every memory ref.

End-to-end test (`tests/test_fastio.c`) verifies disk → FIFO →
IOStore(DSK) → main memory and main memory → IOFetch(DWT) →
display FIFO → framebuffer.

Phase 3 (later) will add the cycle-accurate timing model for
real hardware behavior (49.5-cycle worst-case latency per HM §8
page 91); the MiSTer port will require the full pipeline.

---

## 3. Task scheduler + per-task state (HM §4.1, §4.2 + §7)

This is the foundation; nothing else works without it.

### Per-task replicated state

Each of the 16 tasks has its own copy of:

- **TPC** — micro-PC (12 bits). Saved/restored on task switch.
- **T** — accumulator (16 bits).
- **MemBase** — 5-bit base register selector.
- **RBase[2:3]** — when SubTasks are in use (OR'd, see §1).
- **TIOA** — 8 bits, see §1.
- **Link** — return register (12 bits).
- **Wakeup latch** — 1 bit, set by device, cleared by Block.

Per-task is real **board-level RAM**, not flipflops; HM §4 calls them
"the task RAMs" (TRAM_TPC, TRAM_T, TRAM_MB, TRAM_LINK, TRAM_TIOA).

### Switching protocol (HM §4.2 + §7)

Every clock, the scheduler picks `Next = highest-priority ready
task`. If a `Block` is in the current instruction, the running task's
wakeup is cleared. The hardware compares `Ctask` (current) vs `Next`:
if different, save (Ctask) and restore (Next) the task RAMs at t₂…t₄
of the *current* instruction so the next instruction fetches from
TPC[Next].

**Hold** stalls the current task one cycle without switching context.
Hold + Block in the same cycle is the source of the "Next Lies"
pathology described in §1.

**TaskingOff/On** (FF FA=1 FB=4 FC=2/3) gate the scheduler. TaskingOn
delays effect by 2 cycles. Used during emulator restart sequences.

### What we need to build (scheduler)

Phase 5 (per CLAUDE.md). Concretely:

- 16-element arrays for TPC, T, MemBase, RBase, TIOA, Link, Wakeup.
- A `dorado_task_step()` that runs every cycle: clear wakeup if Block,
  evaluate priority, save/restore registers, drive Next + SubTask.
- `Wakeup[n]` FF function (FA=3 FB=6/7) wired to set wakeup[n].
- IOAtten branch condition: OR of all wakeup latches != 0 (Table 13
  condition 6, already stubbed in `cpu.c`).
- Hooks for hold detection (memory pipe stall, IFU parking).

---

## 4. BaseBoard interface devices

The BaseBoard talks to the Dorado via slow I/O on a tight set of
addresses. **This is the immediate Phase A.7 prerequisite** — Boot0
spends most of its time poking these registers. Sources:
`chm/dorado/expanded/doradobaserom.dm!12_/doradoboot.masm` (BB side)
and the IRTable jam patterns we already extracted.

### MIR (microinstruction register) injection

When the BB writes to MCPBus with `Run/SetSS` set, it deposits 5 bytes
into MIR0..MIR3 + ParityByte — those become the next microinstruction
the engine executes. The Dorado side must:

- Recognize the MCPBus write strobe.
- Decode 5 bytes into a logical `dorado_uinstr` matching the
  iw0/iw1/iw2 reconstruction in `dorado/CLAUDE.md`.
- Stuff that uinstr into the engine's "next instruction" slot,
  bypassing IM fetch.
- Honor `SetSS` (single-step) vs. `Run` (free-run from IM until
  `Freeze`).

### CPReg (control panel register)

The bidirectional debug port. From the microcode side:

- `B←RWCPReg` (FF FA=1 FB=7 FC=6) — reads the BB's current write into
  CPReg + simultaneously copies B into Link as a side effect.

From the BB side: a register pair on RIOT #3 (PA||PB) that the BB can
read (Dorado's last write) and write (Dorado will see on next
B←RWCPReg). Already wired in `src/baseboard.c`.

The Boot0 loader does:
```
loop: Cnt←255; B←RWCPReg; Write IM (decode RSTK[2:3] slice);
      branch loop until done.
```
So slow IO doesn't actually carry the boot bytes — CPReg does. Boot0
*does* read slow IO addresses to populate ALUFM/RM tables before the
loader loop. **Those reads are the Phase A.7 blocker.**

### CPRegH ("CPReg high")

Status register the BB drives. Bit 7 = AMSync (BB→Dorado sync ack);
bit 6 = MASync (Dorado→BB request); other bits TBD. Already
investigated; wiring is correct in current `src/baseboard.c`.

### What we need to build (BaseBoard interface)

For Phase A.7:

1. **Identify the actual TIOA addresses Boot0 reads** — disassemble
   IM[0o7700..0o7777] and grep for `TIOA←` + `Pd←Input` pairs.
2. **Stub each as a BB-side register** in `src/baseboard.c` returning
   plausible values (likely zeros for "no error" + a few magic
   probe-response constants).
3. Add a slow-IO routing entry in `src/io.c` (when written) that maps
   those TIOA addresses to BB-stubbed reads.

Once Boot0 passes its init phase, the BB will use CPReg streaming to
deposit Boot1+Initial — that path is already wired and tested.

### Open question

Does Boot0 read disk registers (TIOA 10₈–14₈)? If so, we'd need a
*minimal* disk-controller stub responding to DiskMuff/DiskTag for
"drive not ready." Likely **not** — Boot0's job is to load Boot1
from CPReg, not to touch disk — but the disasm will tell.

---

## 5. Disk controller (HM §9)

Postponed until Phase 6, but the design is locked. Includes here so
we know what we're committing to.

### Position in the system

- **Task**: 14₈ (DSK).
- **TIOA range**: 10₈–14₈ (5 addresses on this task).
- **Bus**: slow I/O for control, fast I/O (Fin/Fout) for data
  transport.
- **Drive**: up to 4 Century Data Trident SMD drives — T-80 (82 MB)
  or T-300 (300 MB). Drives are bussed daisy-chain plus per-drive
  radial cables.
- **Co-located with Ethernet** on the same logic board (sharing 3
  TIOA slots that the disk doesn't use; HM §9 page 92).

### TIOA register map (HM p. 92)

| TIOA | Name        | Direction                                              |
|-----:|-------------|--------------------------------------------------------|
| 10₈  | DiskControl | `Output←B` to control register                          |
| 11₈  | DiskMuff    | `Output←B` muffler control; `Pd←Input` to read muffler  |
| 12₈  | DiskData    | `Pd←Input` to read FIFO; `Output←B` to write FIFO       |
| 13₈  | DiskRam     | `Output←B` to format RAM (auto-increments)              |
| 14₈  | DiskTag    | `Output←B` to tag register                              |

> *Other tasks must not select these TIOA addresses at any time;
> doing so may cause the disk controller to malfunction.* (HM §9)

### DiskControl bit layout (HM p. 97)

| Bits   | Meaning                                                    |
|-------:|------------------------------------------------------------|
| B[5]   | ClearEnableRun                                             |
| B[6]   | SetDebugMode                                               |
| B[7]   | SetBlockTillIndex                                          |
| B[8:9] | Op for 1st block of sector (0=Done, 1=Write, 2=Read+Check, 3=Read) |
| B[10:11] | Op for 2nd block                                         |
| B[12:13] | Op for 3rd block                                         |
| B[14:15] | Op for 4th block                                         |

`EnableRun` is initially clear; only set by completing the format-RAM
load (HM p. 97). `DebugMode` enables fake bit-clocks for diagnostics
when no drive is present. `BlockTillIndex` masks sector/index task
wakeups until the next index pulse — used after seeks.

### Format RAM (HM p. 97–98)

16-word × 12-bit RAM holding word counts and tag commands. Loaded via
TIOA `DiskRam` writes (RAM Address register zeros on DiskControl
write, then auto-increments). Loading the **last** word turns on
`EnableRun`.

| Addr | Description                          | Example (Alto Diablo) |
|-----:|--------------------------------------|----------------------:|
| 00   | Word count of 1st block              | 0001                  |
| 01   | Word count of 2nd block              | 0007                  |
| 02   | Word count of 3rd block              | 0377                  |
| 03   | Word count of 4th block              | 0000                  |
| 04   | Control tag for read op              | 0104                  |
| 05   | Control tag for write op             | 0204                  |
| 06   | Control tag set Head Select          | 0004                  |
| 07   | Control tag zero tag bus             | 0000                  |
| 08   | Words to write 0s before 1st block   | 0033                  |
| 09   | Words to write 0s before successors  | 0006                  |
| 10   | Words to wait before reading 1st     | 0011                  |
| 11   | Words to wait before reading succ.   | 0002                  |
| 12   | ECC words plus one                   | 0002                  |
| 13   | Word count of 2 (ECC fixed)          | 0001                  |
| 14   | Word count of 1 (minimum count)      | 0000                  |
| 15   | Not used                             | 0000                  |

Word counts are 1 less than the desired count. The 12-bit RAM has 6
entries that depend on drive (9–14) and 6 on sector layout (0–8).

### Sequence PROMs

Two PROMs (one read, one write) drive the tag register from format
RAM during a transfer. Indexed by their own program counter
initialized to zero per sector. Clocked by `WordClock` (derived from
disk bit clock derived from pre-recorded timing on the pack — we'll
need to model this when we build a disk-pack image format).

### FIFO

16-word data FIFO between the controller and the disk. Disk task is
woken according to FIFO depth and reads/writes via `DiskData`.

### Fire Code ECC

`P(X) = X³² + X²³ + X²¹ + X¹¹ + X² + 1`. Detects errors of any
length, corrects bursts ≤ 11 bits. Two ECC words appended to each
block. We'll implement the polynomial in the C model and skip the
hardware-level shift register.

### Sector layouts

The controller is fully programmable. HM p. 94 calls out:

- **Alto Diablo**: 28 × 256-word sectors (4 subsectors/sector)
- **Pilot**: 16 × 512-word sectors (7 subsectors/sector)
- **Alto Trident emulation**: 9 × 1024-word sectors (13
  subsectors/sector) — compatible with BCPL Trident/IFS
- The 16-sector Alto Trident format used by Juniper is **incompatible**
  with the 16 × 512 Pilot format (also 16 sectors but different
  layout).

T-80 specs (HM Table 23): 5 surfaces × 815 cylinders, 9.67 Mb/s,
3 mode/cyl, 30 ms avg seek, 3600 RPM, 117 subsectors/rev.

### What we need to build (disk)

**Status (Phase 1+2: DONE).** `dorado/include/disk.h` + `src/disk.c`,
ported from ContrAlto2's TridentDrive/TridentController/DiskPack.cs.

Phase 1:
- **Pack image format**: dorado_disk_pack with Bitsavers/ContrAlto
  layout (dummy 2B + header 2w + label 10w + data 1024w per sector,
  stored CHS order, little-endian). T-80 = 815×5×9 (76 MB),
  T-300 = 815×19×9 (289 MB). ✓
- **Drive**: per-drive online/ready/RO/select state, current head
  position, seek-in-progress and index-pulse latches. ✓
- **Controller**: dorado_disk_controller registered on task 14₈
  (DSK), TIOA 10₈-14₈. DiskControl bit decode, Format RAM
  auto-increment + EnableRun, DiskData FIFO push/pop, DiskTag
  capture, DiskMuff status readout. ✓

Phase 2:
- **Tag command decoder** (HM pages 99–101): Drive Select
  (Tag[0:3]=0), Head Tag (=1), Cylinder Tag (=2), Control Tag
  (=3 with ReZero/HeadAdvance/Read/Write bits). Cylinder Tag
  updates `cur_cyl` (synthetic instant seek), Head Tag updates
  `cur_head`, Read kicks off sector load — drains FIFO and fills
  with header+label+(data prefix) from current (cyl,head,sec). ✓
- **Sector-pulse helper**: `dorado_disk_controller_advance_sector()`
  increments sector counter (wraps at 9 for T-80), sets
  `sector_tw`, and reloads FIFO with the new sector if active. ✓
- **Read-stream refill**: the FIFO now streams the whole
  header+label+data record instead of only the first 16 words. ✓

Phase 3 (later):
- Sequence PROM execution: read PROM and write PROM advance per
  WordClock, drive tag register from format RAM, generate wakeups.
- Fast-I/O FIFO transport — wire the FIFO into IOFetch←/IOStore←
  munches with main storage (sectors → memory and back).
- Fire Code ECC: P(X) = X³² + X²³ + X²¹ + X¹¹ + X² + 1.
- Real sector-pulse timing: 117 subsector + 1 index pulses per
  16.67 ms revolution drives DSK task wakeups.
- Block transitions per `Format RAM[8:11]` 1st/successive-block
  delays.

The MiSTer port will reuse drive-pack handling but redo the
controller in RTL.

---

## 6. Display controller (HM §11)

Two chips on two boards: **DispM** (mono, optional) and **DispY**
(7-wire interface for the Star/Dorado terminal). The DDC (Display
Data Channel) on the MemD board pulls words via Fast I/O.

### Tasks

- **DHT** (3) — *Display horizontal task*, low priority. Awakened
  once per scan line at end of HWindow. Calculates next scan line's
  parameters, loads NLCB.
- **DWT** (13₈) — *Display word task*, **very high priority**. Runs on
  behalf of either channel (subtask 0 = A, subtask 2 = B). Issues
  `IOFetch←` to fetch munches into the display FIFO.
- **AHT** (4) and **AWT** (11₈) — DispM terminal-interface variants.

DHT and DWT cooperate via per-channel RM registers (`αNextAddr`,
`αNextCount`, `αCurrentWCBFlag`, `αNextWCBFlag`). DHT sets up,
DWT munches.

> **Neither DWT nor DHT drives the IOAtten branch condition.** (HM
> p. 119)

### NLCB / CLCB (HM p. 113)

DDC has CLCB (Current Line Control Block, "what the hardware is
showing right now") and NLCB (Next Line Control Block, "what to
prepare"). NLCB is a 16-word × 12-bit RAM loaded by DHT via slow I/O.
At horizontal blanking start, CLCB ← NLCB.

NLCB fields per channel α (i.e., A and B both have these):

- **αPolarity** — 1 bit, inverts black/white (binary monitors only).
- **αResolution** — 2 bits: 0/2/3 = ¼/½/full pixel-clock rate.
- **αItemSize** — 4 bits unary: bits/pixel = 1, 2, 4, or 8 (only one
  bit set).
- **αLeftMargin** — 12 bits. Pixel clocks to wait after HWindow before
  turning channel on. For 7-wire interface, value = `-(B + L - 68)`
  (HM p. 114, accounts for 5-pixel video signal delay).
- **αWidth** — 12 bits. Counter, runs at pixel clock from when channel
  turns on. To run W pixel clocks, load `-(W + 255)`.
- **αFifoAddr** — 8 bits. Munch+word pointer for first FIFO read of
  the next scan line. Even (doubleword fetched).
- **MixerModes** — not channel-specific; mixer control bits.
- **VCW** — Vertical Control Word: VBlank, VSync, OddField bits.
- **CursorX** — 12-bit cursor x-position counter load value.

### Other RAMs

- **HRam** — 1024 × 3-bit RAM controlling horizontal sync/blank/half-
  line waveforms. Indexed by 12-bit pixel-clock counter
  (HRamAddr[0:11]). Loaded via slow I/O during init.
- **MiniMixer** — single output instruction loads address+data;
  produces 7-wire interface output from MinMixer[0].
- **Mixer / BMap / CMap** — RAMs loaded during init. Not often
  rewritten while displaying.
- **PixelClk** — programmable pixel clock register on DispM.
  Frequency = `(312.5 × (241 - M)) / (16 - D)` KHz where
  M = PixelClk[4:11], D = PixelClk[12:15]. Stabilizes ~½ s after
  load.

### DWT loop (HM p. 118 — verbatim)

```
DWTStart:    ACount←(ACount) - T,  Branch[DWTCheck, R<0];
             AAddress←(IOFetch← AAddress) + (Output←T), Block,
                                                    Branch[DWTStart];
DWTCheck:    AAddress←(AAddress) AND (1C), Branch[DWTAdjustWCBFlags, R even];
DWTRefill:   ACount←ANextCount;            *from DHT, # munches to fetch -1 in 0:11
             BrLo←ANextAddrLo;             *first munch address
             BrHi←ANextAddrHi;
DWTAdjustWCBFlags:
             AAddress←(AAddress) - 1, Output←AAddress, Block, Branch[DWTStart];
```

This is the canonical fast-output microcode. Note `IOFetch←` paired
with `Output←T` in a single instruction — the fast bus pulls the
data; the slow bus tells the DDC "another munch is coming." After
fetching the last munch for a scan line, DWT clears `αCurrentWCBFlag`
and blocks unless `αNextWCBFlag` is true.

### Slow IO Interface (HM p. 119)

DDC manages all control via slow I/O. Output sequence:

- t₁: TIOA latched into a register; output decoded as one of the DDC
  commands.
- t₃→t₅: IOB clocked; one of the "TIOA command" pulses fires.
- IOB stays in DDC's RIOB buffer until the next output command —
  useful for multi-cycle muffler readout.

RAM-load handshake: HRam, Mixer, BMap, CMap have **two address
sources** — slow I/O (load) and video system (display). A `Keep'`
flipflop on each RAM gates this — set false (RAM held by Dorado),
or true (RAM driving video). `LoadAddress` and `Write'` bits in the
output command control: load address from IOB[4:15], write data, or
auto-increment.

### Muffler

Diagnostic readout — DDC internal signals selected by `MufAddr[5:11]`
in the Statics register. Lets diagnostics observe FIFO state, channel
on/off, sizes, monitor type (`MufAddr=106` = MonitorType: 0 = Alto,
1 = LF "large format" / Star).

### Back channel (HM p. 116)

Keyboard and mouse arrive via the 7-wire interface's back channel.
- 32-bit messages, one bit per scan line. Start bit + 3 unused +
  4-bit type + 16-bit body + 8-bit trailer (200₈).
- Sampled by DHT during scan line after HWindow.
- Message types in Table 24: 01₈–04₈ = keyboard words 0–3; 05₈ =
  mouse buttons + keyset; 06₈ = mouse Δx/Δy (excess-200₈); 17₈ = boot
  button.

### What we need to build (display)

**Status (Phase 1+2: DONE).** `dorado/include/display.h` + `src/display.c`.

Phase 1:
- 808×606 mono framebuffer, MSB-leftmost packing. ✓
- DDC catch-all slow-I/O handler registered on tasks DHT/AHT/AWT/DWT.
  Records (task, TIOA, data) into a buffered RIOB; preserves data
  until the next output command per HM page 119. ✓
- Per-channel state buckets: NLCB/CLCB (16×12-bit, channels A and
  B), HRam (1024×3-bit), Mixer (1024×24-bit), PixelClk, Statics. ✓
- Per-channel FIFO (256 words) for IOFetch← munch delivery. ✓
- PGM snapshot helper (`dorado_display_snapshot_pgm`). ✓

Phase 2:
- `dorado_display_render_fifo()` drains the per-channel FIFO and
  lays 16 pixels per word into the framebuffer (MSB=leftmost,
  1-bit-per-pixel = Alto monitor convention). ✓

Phase 3 (later):
- Decode (task, TIOA) → specific DDC command (NLCB load, HRam load,
  Mixer load, etc.) once we trace what microcode emits.
- Pixel clock generation, real HSync/VSync timing.
- 7-wire interface back-channel for keyboard/mouse.
- 24Bit color (AMap+BMap+CMap), Mixer modes (A8B2/BBypass).
- αItemSize handling (2/4/8 bits per pixel for grayscale).

---

## 7. Ethernet (HM §10)

3 Mb/s Alto-style Ethernet. Two tasks:
- **EOT** (6) — output (transmit).
- **EIT** (7) — input (receive).

Co-located with disk on the same logic board. Ethernet uses 2 of the
3 TIOA slots in the disk's range that the disk doesn't claim (HM
§9 page 92 says "uses two of the other three"). Specific TIOA
assignments not captured in this doc — read HM §10 when implementing.

### What we need to build (ether)

Defer until disk + display are working. We do not need it for Mesa
boot. Useful eventually for IFS (file server) connectivity.

---

## Implementation order (revised)

Layered on `dorado/CLAUDE.md` "What's next":

| # | Layer                                          | Status                         |
|--:|------------------------------------------------|--------------------------------|
| 1 | Slow-I/O routing layer (`src/io.c`)            | **DONE**                       |
| 1b| Per-task TIOA storage                          | TBD (single TIOA today)        |
| 2 | BB-side stubs for whichever TIOA Boot0 probes  | N/A — Boot0 doesn't use slow-IO|
| 2b| ALUFM bit-mapping fix (Pd←ALUFMRW)             | **DONE** — unblocked Bootstrap |
| 3 | Task scheduler Wakeup/Block/Next protocol      | Partially done; "Next Lies" TBD|
| 4 | `IOFetch←`/`IOStore←` Fin/Fout transport       | **DONE** (synchronous variant) |
| 5 | Display: framebuffer + DDC catch-all (Phase 1) | **DONE**                       |
| 5b| Display: DDC command decoder + waveform gen    | TBD                            |
| 6 | Disk: pack/drive/controller stub (Phase 1)     | **DONE**                       |
| 6b| Disk: sequence PROM + Fire Code ECC + Fast-IO  | TBD                            |
| 7 | Ethernet                                       | TBD (optional)                 |

Boot0's actual blocker turned out to be ALUFM init, not slow-I/O —
see "Important Boot0 finding" in §1. (3)–(5) are Phase 5–6 of the
master plan. (6) gates Mesa boot. (7) is post-Mesa.

## Cross-references

- `docs/memory-architecture.md` — companion doc on cache/Map/Pipe/BR.
- `docs/boot-bringup-plan.md` — current state of Phase A.7 and the
  Boot0 trail observation that motivated this doc.
- `dorado/CLAUDE.md` — what's actually built today.
- HM §4.1–4.2 — task scheduler details we glossed over.
- HM §7 — slow I/O canon (read this fully before writing code).
- HM Figure 13 (disk), Figures 14/15 (display) — schematics referenced
  by the controller chapters.
