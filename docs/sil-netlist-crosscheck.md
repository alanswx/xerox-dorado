# Checking the emulator against the Dorado's own design data

**2026-08-08. Six boards: ProcH, ProcL, IFU, MemC, MemD, MemX.**

Everything in this project has been checked against the Hardware Manual, the
schematics-as-PDFs, and PARC's microcode. This is the first check against the
**design input** — the files the machine was actually built from — which is
what Tim recommended when he pointed at the Sil tools.

`tools/fetch_dorado_sil.py` fetched them (see `docs/chm-archive.md`);
`tools/sil_netlist_report.py` reads them.

## What the files are

`chm/sil/<Board>-Rev-Xx.dm!N_/` holds, per board revision:

| file | what |
|---|---|
| `<Board>-Rev-Xx-C.nl`, `-E.nl` | the board's **backplane interface**, `Signal: #s05-C.12` |
| `<Board>-Rev-Xx.lc` | the **component list**: part number, pins, package, grid positions |
| `<Board>-Rev-Xx.wl` | the wire list (118 KB for ProcH) |
| `<Board>NN.sil` | the numbered drawing sheets, Sil's binary format |
| `Build.cm`, `Print<Board>.cm` | PARC's own build scripts |

The `.nl` files are plain text and use **the machine's own signal names** —
`IOB`, `RSTK`, `MemBase`, `Overflow'` — which are the same names the C
emulator uses. That is what makes a direct comparison possible.

```
$ tools/sil_netlist_report.py ProcH ProcL IFU MemC MemD MemX
== ProcH  (ProcH-Rev-Ce.dm!1_)  175 nets,  68 signals
== ProcL  (ProcL-Rev-Ci.dm!1_)  176 nets,  71 signals
== IFU    (IFU-Rev-Ch.dm!1_)    137 nets,  47 signals
== MemC   (MemC-Rev-Be.dm!1_)   168 nets,  95 signals
== MemD   (MemD-Rev-Ca.dm!1_)   174 nets,  43 signals
== MemX   (MemX-Rev-Ch.dm!1_)   184 nets, 129 signals
```

## READ THIS FIRST: how to not get a false gap

The first pass of this document reported four gaps. **Two of them were
wrong**, and both failed the same way: I grepped for the Xerox signal name
and concluded that no hit meant no model.

- `SubTask.0/1` — reported "never touched". In fact subtask is modelled
  throughout: `cpu->task_subtask[16]`, `dorado_cpu_set_subtask()`,
  `dorado_io_read_subtask()/write_subtask()`, a `subtask` field on every Pipe
  entry, and MemBase[3:4] OR'd with subtask[0:1] for I/O tasks. Our code
  spells it `subtask`.
- `StkError` — reported as "only a comment in `cpu.c`". In fact
  `stk_error_check()` implements HM page 11's formula exactly, keeps StkUnd
  and StkOvf as sticky-recomputed flags, and wakes task 15. The comment I
  found was the header *of the implementation*.

**A case-sensitive grep for a hardware signal name is not a test for whether
we model the thing.** This project's convention is to keep the manual's
terminology in C form — `SubTask` becomes `subtask`, `StkError` becomes
`stk_error_check`. Grep case-insensitively, grep for the concept, and read
the hit before calling anything missing.

The corrected findings are below.

## Structural facts the design settles

- **The 16-bit datapath is split across the two processor boards, with a
  parity bit each.** Every major bus appears as `.00-.07` plus `.16` on ProcH
  and `.08-.15` plus `.17` on ProcL: `IOB`, `dMD`, `BMux`. Bit 16 is
  high-byte parity, bit 17 low-byte parity. The memory boards see the whole
  thing at once: `BMux[0-17]`, `dMD[0-17]`, `Fin[0-17]`, `Fout[0-17]`.
- **The ALU is four MC10181 slices**, two per board (`.lc`: `MC10181/24/E6W
  x2` on each). The MC10181 is the ECL 4-bit ALU, and its control is exactly
  the `aluF0..aluF3`, `aluM`, `aluC0` and `aluCout` nets — which is why an
  ALUFM entry is 6 bits (4 function + mode + carry-in) rather than an opcode.
- **The scale**: 419 packages on ProcH, 420 on ProcL, ~50 part types each.

## What the design CONFIRMS about the emulator

Every field and register width the emulator implements appears in the
hardware interface at the same width:

| emulator | netlist | board |
|---|---|---|
| `RSTK` 4 bits | `RSTK[0-3]` | ProcH |
| `ALUF` 4 bits | `ALUF[0-3]` | ProcL |
| `BSEL` 3, `ASEL` 3, `LC` 3, `FF` 8 | `BSEL'[0-2]`, `ASEL'[0-2]`, `LC[0-2]`, `FF[0-7]` | ProcH/L |
| `MemBase←B[3:7]` (5 bits) | `MemBase[0-4]` | ProcH, MemC |
| `RBase←B[12:15]` (4) | `RbAdr[0-3]` | ProcH |
| `StkP←B[8:15]` (8) | `StkAdra[0-7]` | ProcH |
| `TIOA←B[0:7]` (8) | `TIOA[0-7]` | ProcH |
| `ifum[1024]` | `IfuAddr'[4-13]` (10 bits) | IFU |
| 8-bit opcode byte | `IfuData[0-7]` | IFU, MemC |
| **16-entry Pipe** | `dPipe02Ad[0-3]`, `dPipe34Ad[0-3]` | MemC, MemX |
| 8-bit ECC syndrome (`pipe4_syndrome`) | `EcOut[0-6]` + `EcOut'[7]` | MemD |
| 9-bit Map address | `MapAd[0-8]` | MemC, MemX |

**Every branch condition the emulator implements is a real pin** —
`ResEqZero'`, `ResLtZero'`, `ALUCarry`/`aluCout`, `Cnt=Zero'`, `RmLtZero'`,
`RmOdd'`, `Overflow'`. `Overflow'` is worth its own line: the Overflow branch
condition was one of the six microengine bugs the bring-up fixed, and here it
is as a dedicated signal.

Several things the emulator got right the hard way show up as backplane
signals, which is the strongest form of confirmation available short of RTL:

- **`UseAsrn`** and **`ProcSrn_'`** (MemC, MemX). The ASRN-vs-ProcSRN
  distinction is exactly the 2026-07-26 Smalltalk fix — IFU opcode fetches
  select ASRN, not `pipe[ProcSRN]`. The hardware has a wire for the choice.
- **`FF[2-7]` plus `FA=1'`** (IFU *and* MemC). Both boards take only FF bits
  2..7 and a single decoded "FA equals 1" line — the physical form of the
  `FA = FF[0:1]`, `FB = FF[2:4]`, `FC = FF[5:7]` split in `ff_override_b()`.
  Each board is wired to receive only the part of FF it can act on.
- **`SignIfuData`** (IFU) — our `ifu_sign`, which sign-extends the operand
  byte when the IFUM entry's Sign bit is set (`cpu.c:3885`).
- **`JunkTW`** and **`Pendulum`** (IFU) — the junk-task wakeup timer, which
  `junk_timer_tick()` models and whose 32 us cadence was measured correct.
- **`ChipsAre64K`, `ChipsAre256/16K`, `M0`..`M3`, `Mod0SinEn'`..`Mod3SinEn'`**
  (MemX) — the storage straps and the four-module select that
  `dorado_memory_config_word()` reports through `B←Config'`.
- **`FinTask[0-3]` + `FinSubtask[0-1]`**, same for Fout (MemX). Fast I/O is
  addressed by task *and* subtask, which is what `dorado_io_read_subtask()`
  takes and what `fastio.c` uses to pick display FIFO A or B.
- **`TWReq15`** (MemX) — task 15 is the fault task, and it is wakeable from
  the memory board. `stk_signal_error()` and the Map-fault path both raise
  exactly that bit.
- **`DisHold`** (MemC, MemX) — `mcr.disHold` is a real backplane line, not a
  microcode convention. That is why boot-stage microcode can turn Hold off
  machine-wide.
- **`EventA`..`EventE`, `CrryEvCntA`, `GenIn[0-15]`, `GenOut[0-15]`,
  `GLd'`, `GDv'`** (IFU, MemD) — the §4.11 event counters and the generator
  our `gen_io_mode` models, which is what makes the `eventCounters`
  diagnostic pass.

## What the design shows the emulator does NOT model

### 1. A byte-parity network across the whole machine — a real gap

`IOB.16/.17`, `dMD.16/.17`, `BMux.16/.17` carry parity, with dedicated error
and control lines on every board: **`IOPE`**, **`MdPE`**, **`RamPE`**,
**`CkMdParity'`** (ProcH/L), **`dHitPerr`**, **`dSTPerr`** (MemC, MemX),
**`MemPE`** (MemX).

`include/cpu.h:203` says it outright: *"We do not yet model parity at the bit
level."* `src/memory.c` contains the string "parity" zero times. What exists
is `io_bad_parity`, a single device-flagged bit on the slow-I/O input path.
So a microprogram that induces or checks a bus or register-file parity fault
sees nothing happen. Given that the Hardware Manual lists faults as visible
to microcode, this is a real gap rather than a detail.

### 2. Hold is a multi-source request network — we model one source

This is the finding that changes the shape of the problem. Hold is not one
signal; it is a **request/collect/broadcast network** spanning the machine:

| requester | signal |
|---|---|
| processor | `PrHoldReq`, `PrHold`, `PrBlock'` (ProcH/L, MemC) |
| cache | `CHoldReq`, `CBHold` (MemC) |
| IFU | `IfuHold`, `WantIfuHold'` (IFU, MemC) |
| I/O | `IOHold` (MemC) |
| storage | `MXHold` / `MxHold`, `LargeHold` (MemC, MemX) |
| map buffer | `HoldMapBuf` / `HoldMapbuf` (MemC, MemX) |
| external | `ExtHoldReq` (MemC) |
| collected | **`Hold`** (MemC), gated by **`DisHold`** |

We assert the Hold *signal* from exactly one source — Md consumed before its
fetch latency elapsed — plus the §3.12 HOLDSIM debug register, and we gate it
on `mcr.disHold` correctly. But the engine's *response* stays behind the
`DORADO_HOLD` flag because our fixed 3/16/24-cycle ready-time approximation
models only a fraction of the Memory Section's autonomous timing
(`cpu.c:5412`). The netlist says what the rest of that model needs: seven
independent requesters, not a better constant.

### 3. `CountMiss` — cache misses are not counted

A dedicated line on both IFU and MemX, feeding the event counters. Our event
counters work but have no miss input. Zero hits in the source.

### 4. DRAM refresh does not exist in our model

`MemRASa/b`, `MemCASa/b`, `MemWEa/b`, `MemRfsh`, `RfshPeriod` (MemX) and
`MapRfsh'` (MemC). Main storage is a flat `uint16_t` array with no refresh
cycle, so on real hardware refresh steals memory bandwidth we give away
free. A timing-fidelity item, and one that only matters once Hold is real.

### 5. ECC is read back but never generated or checked

We keep an 8-bit `pipe4_syndrome` per Pipe slot so microcode can read a
fault, and `memory.h:222` notes plainly that main storage does not model ECC.
The hardware generates and checks it in the Ec pipeline: `StartEcGen'`,
`StartEcChk'`, `EcIn[0-1]`, `EcOut[0-6]`, `ECFault`, `ErrorsFromEc2`,
`MemError`.

## The bonus: MemC and MemX are a blueprint for Phase 2

The cache's internal pipeline is named on the backplane, stage by stage —
which is unusual and very useful, because it means the RTL does not have to
be reverse-engineered from the sheets:

- **Stage A** (address): `At=Curt'`, `AwantsDifHit'`, `AcanhaveMap'`,
  `AfreeOrEc'b`, `EcWantsA`, `EcKeepsAbusy`, `ReadInA'`, `Store_InA'`,
  `IoFetchInA'`, `IoStoreInA`, `DirtyIoFetchInA'`, `ProcTagInA`
- **Stage Ec1**: `CacheRefInEc1`, `TagInEc1`, `WPinEc1`, `MapTroubleInEc1`,
  `Store_InEc1'`, `IfuRefInEc1`
- **Stage Ec2**: `IfuFaultInEc2`, `ErrorsFromEc2`
- **victim selection**: `Hita`, `VicIfMiss'`, `VicInPair'`, `VicOrFS1C`,
  `PairFull'`, `STfree'`
- **transport**: `Transport'`, `MakeD_CD`, `MakeD_Dbuf`, `MakeF_D`,
  `MakeMD_D'`, `MakeMDM_D'`, `MakeFout_D`, `MakeSout_D`, `FastD_Dbuf`

That is the four-way cache, its victim policy and its data path spelled out
in the designers' own names.

## How to repeat this

```sh
tools/sil_netlist_report.py ProcH ProcL       # interfaces, grouped by bus
tools/sil_netlist_report.py --chips ProcH     # the parts and where they sit
tools/sil_netlist_report.py --grep Hold MemC  # find a signal family
```

Ten boards remain: ContA, ContB, DispY, DispM, DskEth, BaseBd, msa, PCMSA,
IOTest, Music. **ContA/ContB are the obvious next targets** — the Control
section is where tasking and the microinstruction sequencer live, so its
interface should say whether our 16-way scheduler owes the rest of the
machine anything we are not providing. DispY (monochrome) and DskEth follow,
and would settle the colour question in `docs/color-graphics-todo.md`
directly from DispM.

## What this is not

It is a **coverage** cross-check, not an equivalence proof. The `.nl` files
give each board's interface, not its logic; proving our datapath computes
what ProcH's 419 packages compute needs the `.wl` wire list and the sheets,
which is Phase 2's job. What this pass establishes is that the emulator's
*shape* — field widths, register widths, branch conditions, the Pipe's depth,
the ASRN/ProcSRN split, the FA/FB/FC decode — matches the hardware exactly,
and it names five specific things the hardware has that we do not.
