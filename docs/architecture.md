# Dorado architecture — distilled

A compact summary distilled from the September 1981 Hardware Manual.
Section numbers refer to that manual unless otherwise noted. This is a
working document — flesh it out as the emulator forces us to learn more.

---

## 1. Quick stats

| Property | Value | Source |
|---|---|---|
| Word size | 16 bits (data and main memory) | HM §1 |
| Microinstruction width | 34 bits + 2 parity | HM Table 1 |
| Microinstruction cycle | 60 ns nominal | HM §1, §14.1 |
| Virtual address space | up to 2²² 16-bit words (8 MB) | HM §1 |
| Main storage | 1 to 4 modules of 256 K × 16 (max 1 M words) | HM Table 1 |
| Cache | 4096 words, 64 rows × 4 columns × 16 words/line (4-way set-assoc) | HM Table 1 |
| Microcode store (IM) | 4096 × 34 bits + 2 parity | HM Table 1 |
| Hardware tasks | 16 priority levels (0=emulator, 15=fault task) | HM §4.1 |
| Instruction sets | up to 4, each 256 opcodes, 1/2/3 bytes per opcode | HM §1, §6.1 |
| Logic family | MECL-10K ECL (CPU); MOS + Schottky-TTL (storage) | HM §1 |

## 2. Sections (boards)

```
                 ┌───────────────┐
                 │  BaseBoard    │  ← 6502 + EPROMs, power, boot
                 └───────┬───────┘
                         │ CPReg, reset, hold
   ┌─────────┐   ┌───────▼───────┐   ┌─────────────────┐
   │   IFU   │◀─▶│   Control     │◀─▶│   Processor     │
   │ (IFU.pdf)   │ (Control_A/B) │   │ (ProcH/ProcL)   │
   └────┬────┘   └───────┬───────┘   └────────┬────────┘
        │                │                    │
        │       JCN/Link/TPC/tasking          │ A bus, B/Bmux, Mar, Pd
        │                │                    │
        └────────────────▼────────────────────▼
                  ┌───────────────────────┐
                  │  Memory (MEMC/D/X)    │
                  │  cache, Map, Pipe, BR │
                  └─────────┬─────────────┘
                            │ Sin/Sout (ECC)
                  ┌─────────▼─────────────┐
                  │  Main storage (MSA)   │
                  └───────────────────────┘
                            │
                  ┌─────────▼─────────────┐
                  │  I/O: Disk + Ethernet │  ← DskEth.pdf
                  │      Display          │  ← DispY/DispM
                  └───────────────────────┘
```

## 3. Memories (HM Table 1)

| Name | Size | Purpose |
|---|---|---|
| **IM**   | 4096 × 34b (+2p) | Microinstruction store. Written via Link+B (16 bits at a time + 1 parity bit from RSTK). |
| **ALUFM**| 16 × 6b          | ALU control RAM, indexed by 4-bit ALUF field. Microcoder pre-loads the 16 ALU ops it needs. |
| **RM**   | 256 × 16b (+2p)  | General register file. Address = `RBase[0:3] ‖ RSTK[0:3]`. Read onto A or B; loaded from Pd or Md. |
| **STK**  | 256 × 16b (+2p)  | Emulator-only evaluation stack. Active when BLOCK=1; address = StkP modified −4..+3 by RSTK. |
| **IFUM** | 1024 × 24b (+3p) | Opcode-decoding RAM: 256 entries × 4 instruction sets. Set selected by InsSetOrEvent←. |
| **MAIN** | up to 1 M × 16b  | Main storage (DRAM). Reached through the cache. |
| **BR**   | 32 × 28b         | Base registers. Indexed by MemBase. Virtual addr = `BR[MemBase] + Mar`. |
| **Pipe** | 16 × 6 words     | Trace info per memory reference (vaddr, map bits, fault, task, subtask). Read via Pipe0..Pipe5'. |
| **Map**  | 16 K or 64 K × 19b (+p) | Virtual→physical translation. Indexed by VA[10:23]. |

## 4. Registers (HM Table 2)

`*` = duplicated per task.

| Reg | Width | Notes |
|---|---|---|
| **T\*** | 16b (+p) | Working register. Sources A/B/Md; loaded from Pd or Md. |
| **RBase** | 4b | High nibble of RM address. Loaded from FF[4:7] or B[12:15]. |
| **StkP** | 8b | Stack pointer for STK. RSTK provides −4..+3 adjustment. |
| **Q** | 16b | Shift register for Multiply/Divide. Functions Q lsh 1, Q rsh 1. |
| **Cnt** | 16b | Decrement-and-test counter. Loaded from FF[4:7] (1..16) or from B. |
| **TIOA\*** | 8b | I/O address register (Slow IO). |
| **ShC** | 16b | Shifter-masker control. |
| **MemBase\*** | 5b | Index into BR. IFU loads MemBase[0..3] / 34..37 from IFUM at opcode start. |
| **MemBX** | 2b | Stack-pointer-like extension to MemBase (HM §6.4). |
| **Link\*** | 16b | Subroutine return / dispatch result / IM addr / TPC data. |
| **PC** | 16b | Byte displacement of next opcode within BR[31] (the code base). Maintained by IFU. |
| **TPC\*** | 16b | Per-task next-instruction pointer. Read/written via Link under JCN control. |
| **Mcr** | — | Memory control register; disables parts of memory for init/test. |

## 5. Microinstruction fields (HM Table 5)

The 34-bit microinstruction is split into the following fields. Counts
and exact bit positions are in Table 5 of the manual; verify before
writing the decoder.

| Field | Approx width | Purpose |
|---|---|---|
| **JCN** | 8b   | Jump / call / dispatch / branch + condition selector. Heavily encoded; HM Table 13 lists conditions. |
| **RSTK** | 4b  | Adjusts RM address (low nibble), or selects STK adjust −4..+3 when BLOCK=1, or supplies parity bits when writing IM. |
| **ASEL** | 3b  | A-bus source selector. HM Table 8. |
| **BSEL** | 3b  | B-bus source selector. HM Table 7. |
| **ALUF** | 4b  | Index into ALUFM. HM Table 9. |
| **LC** | 2–3b   | Load control: where Pd / Md is written (T, RM, STK, …). HM Table 10. |
| **FF** | 8b    | Special function — large multiplexed field that picks among constants, register loads, IO ops, Map ops, etc. HM Table 11. |
| **BLOCK** | 1b  | When set, RSTK addresses STK (not RM); also enables stack-pointer adjustments. |

ALU operations come from ALUFM[ALUF]. ALUFM is loaded once at startup
(or whenever the emulator wants a different op set). HM Table 12 lists
the special "ALUF shift" decodes that combine ALU + shift.

## 6. Data paths (HM Table 3)

```
   +--------+      +-----+        +-----+
   |  T*    |--+---|     |        |     |
   |  RM    |--+---| A   |---+    |     |
   |  STK   |--+---|bus  |   |    |     |     +-----+
   |  Q     |--+   +-----+   v    |     |     |     |
   |  Cnt   |--+              +--+|     |     |     |
   |  Md    |--+              |Alu|----->Pd---|  T  |
   |  Id    |--+              |   |     |     | RM  |
   |  Shftr |--+              +--+|     |     | STK |
   +--------+   +-----+        ^   |     |     +-----+
                |  B  |--------+   |     |
                | bus |            |     |
                +-----+    -> Mar -+     |     +-----+
                              -> IOB ----+---->| IO  |
                              <- IOA <---+     +-----+
                               <- Md <-- cache <- main store
                               <- Fout, -> Fin (fast IO bypass)
                               <- Sin,  -> Sout (storage ECC)
```

- **A bus** ("alua"): high-true. 16 bits. Sources: T, RM, STK, Q, Md, Id,
  shifter output, small constant 0..0o17. Confined to Processor section.
- **B bus** ("alub" inside, "Bmux" on backplane): 16 bits. Drives the
  backplane to IFU/Control/Memory. Can carry a literal byte (in left or
  right half of the word) under FF/BSEL control.
- **Mar**: 16-bit memory-address-register / displacement to memory. Driven
  by Processor or by IFU. `BR[MemBase] + Mar` = virtual address.
- **Pd**: 8-input mux on the way back to T/RM/STK. Sources: ALU, shifted
  ALU, IO input, infrequently-read processor registers.
- **Id**: IFU → processor data path (operand bytes from prefetch).
- **Md**: cache → processor data path (memory reads).
- **IOA / IOB**: Slow-IO address (8b) and bus (16b).
- **Fin / Fout**: 16-bit fast-IO buses bypassing the processor.
- **Sin / Sout**: storage buses (between EDAC and the DRAM cards).

## 7. Tasking (HM §4.1–4.2)

- 16 priority levels, fixed. Task 0 = emulator, task 15 = fault task.
- I/O devices raise wakeup latches; the Control section selects the
  highest-priority awake task at each microcycle.
- Per-task replicated state: T, TPC, MemBase, Link (and IO-task-specific
  registers in some cases).
- Task switches are invisible to most microcode because the replicated
  registers swap with the task.
- BLOCK in microinstruction prevents task switch on the next cycle (used
  for atomic stack ops).
- The fault task wakes on machine-check conditions (parity, ECC,
  IFU/processor faults). HM §13.

## 8. IFU (HM §6)

- Up to 4 instruction sets. Select via `InsSetOrEvent←`.
- IFUM[set, opcode] = 24 bits (Table 18) defining: dispatch address into
  IM, MemBase override, operand layout, length, etc.
- Operands of length 1/2 fetched as bytes from cache via BR[31] + PC.
- Prefetch runs in parallel with execution; emulator microcode usually
  ends with an `IFUJump` that consumes the prefetched dispatch.
- HM §6.5 describes traps (e.g. PageFault during operand fetch).
- HM §6.10 covers continuing from processor faults — important for
  emulator microcode that must be replayable.

## 9. Cache, Map, Pipe (HM §5)

- 4 KW cache, 4-way set-associative, 16-word lines, 64 sets.
- Cache references: `Fetch←`, `Store←`, `IFetch←`, `LongFetch←`,
  `PreFetch←`. IFU references the cache independently.
- Map memory translates VA[10:23] → physical page + flags
  (write-protect, dirty, referenced, single-error, double-error).
- Pipe: 16-entry × 6-word ring of trace records, written automatically on
  every memory reference. Microcode reads with Pipe0..Pipe5'. The Pipe
  is how microcode finds out *which* address faulted, what the map
  said, etc.
- ECC is on the storage path (Sin/Sout) via a syndrome generator;
  single-bit corrected, double-bit reported.

## 10. I/O

| Controller | Section | Buses |
|---|---|---|
| Disk (T-80 SMD, optionally T-300) | HM §9 | Slow IO + Fin/Fout for sector data |
| Ethernet (3 Mbit/s; later 10 Mbit/s) | HM §11 | Slow IO + Fin/Fout |
| Display (DispM monochrome / DispY color / OIS 7-wire) | HM §10 | Slow IO + DDC framebuffer DMA |
| "Junk task" / general IO | HM §12 | Slow IO |

Slow IO uses TIOA (8-bit address) on the IOA bus and IOB (16-bit data).
Fast IO bypasses the processor via Fin/Fout, used for bulk transfer.

## 11. BaseBoard (HM §1, Booting memo)

- Microcontroller + EPROM-resident code.
- Functions: power monitor, temperature monitor, boot button, status
  LED, microcode bootstrap loader.
- Communicates with the main Dorado via CPReg (a control/parameter
  register accessible to both sides).
- On 3-push or power-on boot, the BaseBoard:
  1. Halts the Dorado, issues an I/O reset.
  2. Loads a tiny ~50-instruction Bootstrap microprogram into IM.
  3. Lets Bootstrap copy the larger ~700-instruction Initial microprogram
     out of BaseBoard EPROM into IM (via CPReg, in 16-bit chunks).
  4. Hands off to Initial; Initial loads the chosen emulator microcode
     from disk or Ethernet into IM, then jumps to it.

## 12. Software bootstrap chain

```
power-on / boot button
    │
    ▼
BaseBoard 6502 code (B-08, B-10 EPROMs)
    │ writes ~50 µinstrs of Bootstrap into IM via CPReg
    ▼
Bootstrap microprogram
    │ calls back to BaseBoard for Initial in chunks
    │ writes Initial into a different region of IM
    ▼
Initial microprogram (~700 µinstrs)
    │ initializes Map/cache/storage, parity, registers
    │ enables Display, Ethernet, Junk tasks
    │ contacts Ethernet boot server (Gateway / IFS) — Pup EtherBoot proto
    │ receives chosen emulator MB-format microcode
    │ calls LoadRam to copy emulator into IM (replacing Bootstrap+Initial)
    ▼
Emulator microprogram (Mesa.mb / Cedar.mb / Lisp.mb / Smalltalk.mb / Alto.mb)
    │ finishes memory init, resets default disk partition
    │ initiates Alto-style software boot from disk or Ethernet
    ▼
Operating system (Alto OS, Pilot, Cedar, Interlisp-D, …)
```

The fastest path to a useful emulator: implement enough µengine to run
the **Alto emulator microprogram** against an Alto Diablo disk image.
That puts us in well-trodden territory — Alto emulators (ContrAlto,
Salto) exist and we can validate behavior against them.

## 13. Things the Hardware Manual emphasizes that often surprise people

- **Sources are high-true, sinks are low-true.** The Bmux on the
  backplane appears low-true; the inverters live at the section
  boundaries.
- **`B[i:j]` and `Pd[i:j]` use big-endian bit numbering** — bit 0 is the
  most significant bit. Octal indexing is common.
- **A literal in the microinstruction comes via FF + BSEL** specifying
  whether the constant lives in the left or right byte of the word.
- **ALUF doesn't pick an ALU op directly.** It indexes ALUFM, which the
  microcoder loads. So ALUFM[3] could mean different things in different
  emulators.
- **The B bus and the Pd path are not the same.** B is the source side,
  Pd is the destination side; many "register reads" use Pd, not B.
- **Most "instructions" the user runs are emulator-defined opcodes**,
  decoded by IFUM, dispatched to IM. The hardware is the microengine
  that runs the 34-bit IM words.
