# Checking the emulator against the Dorado's own design data

**2026-08-08. Eight boards: ProcH, ProcL, ContA, ContB, IFU, MemC, MemD,
MemX** — the whole CPU.

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
$ tools/sil_netlist_report.py ProcH ProcL ContA ContB IFU MemC MemD MemX
== ProcH  (ProcH-Rev-Ce.dm!1_)  175 nets,  68 signals
== ProcL  (ProcL-Rev-Ci.dm!1_)  176 nets,  71 signals
== ContA  (ContA-Rev-Cd.dm!1_)  170 nets,  64 signals
== ContB  (ContB-Rev-Cd.dm!1_)  132 nets,  44 signals
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

A third near-miss on the Control pass shows the technique that actually
works. `QBit'` arrives at ContA and is not one of Table 13's eight branch
conditions — it looked like a ninth condition we lack. Asking the netlist
*which boards carry it* answered it instead: `QBit'` exists on exactly two
boards, **ProcL and ContA**. Q's manual bit 14 is C-LSB bit 1, i.e. the low
half of the word, which is why it leaves from ProcL; TNIA is formed on ContA,
which is why it arrives there. That is HM §4.4 p.32's Multiply verbatim —
"OR's Q[14] into TNIA[14] … captured in a flipflop at t2 … OR'ed into
TNIA[14] during the next instruction for the same task" — and `cpu.c:2048`
implements it.

**So: trace the net across boards first, then read the code.** A signal's
endpoints say what it is far more reliably than its name does.

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
- **`QBit'`** (ProcL → ContA) — the Multiply dispatch. `cpu.c:2048` ORs
  `Q & 0x0002` into the task's dispatch, and the reason it does is that with
  it stubbed every BCPL multiply returned garbage, NetExec's keyword table
  overran the first coroutine's CTX, and startup crashed into Swat. The wire
  that carries it runs from the board holding Q's bit 14 to the board that
  forms TNIA.
- **`EventA`..`EventE`, `CrryEvCntA`, `GenIn[0-15]`, `GenOut[0-15]`,
  `GLd'`, `GDv'`** (IFU, MemD) — the §4.11 event counters and the generator
  our `gen_io_mode` models, which is what makes the `eventCounters`
  diagnostic pass.

## The Control section: the tasking model, confirmed pin by pin

ContA and ContB are where the microinstruction sequencer and the 16-way
scheduler live, and their interface confirms the emulator's tasking model
more directly than anything else so far.

**`TWReq[1-15]` — fifteen wakeup request lines, numbered 1 to 15. There is no
line for task 0.** Our scheduler reads:

```c
static int task_bnt(uint16_t avail)
{
    avail |= 0x0001;   /* task 0 always ready */
    for (int i = 15; i > 0; i--) ...
```

That comment is a statement about physical wiring. Task 0 is the emulator and
the unconditional fallback, so the hardware never needs to request it — and
correspondingly our loop stops at 1.

**`BNTGtCT'a` / `BNTGtCT'b`** — "Best Next Task Greater Than Current Task",
the priority comparator that decides whether to switch. Our variable is
already called `bnt`, from the manual. The signal also reaches MemX and ContB,
which is how the memory section knows a task switch is coming.

**`FFok'a` / `FFok'b`** — the gate for "FF is interpreted as a function iff
BSEL is not a constant and JCN is not long". That rule is `ff_full_function_ok()`
in `cpu.c:1902`, and it is a pair of real signals distributed to the boards
that act on FF.

**All eight Table 13 branch conditions arrive at ContA as pins**:
`ResEqZero'`, `ResLtZero'`, `ALUCarry`, `Cnt=Zero'`, `RmLtZero'`, `RmOdd'`,
`IOatt`, `Overflow'` — plus `QBit'` for Multiply, discussed above.

**`TNIA[2-15]` and `BNPC[2-15]`** — 14 bits wired. `docs/jcn-encoding.md`
already records that TNIA is nominally 16 bits and that "bits 4:15 of TNIA
are the 12-bit IM address actually used". The netlist agrees and adds that
bits 0 and 1 are not wired at all.

**The control-processor interface is exactly the one our BaseBoard drives**:
`CPAddr'[0-2]`, `CPOut[0-8]`, `CPIn[0-3]`, `CPStrb'`, `SetSS'`, `SetRun`,
`Freeze`, `StopMIRClk`, `CRamClock`, `rMIRa`. The LoadDoradoCode handshake in
`baseboard.c` — jam a microinstruction, strobe MIR, `SetSS`, finally `SetRun`
to free-run — is that pin list in the right order.

And the whole microinstruction arrives at Control: `RSTK[0-3]`, `ALUF[0-3]`,
`BSEL'[0-2]`, `ASEL'[0-2]`, `LC[0-2]`, `dFF[0-7]`, `dJCN[0-7]`, `Block`.

## What the design shows the emulator does NOT model

### 1. A parity network spanning the machine, converging on ContB

`IOB.16/.17`, `dMD.16/.17`, `BMux.16/.17` carry parity, with dedicated error
and control lines on every board: **`IOPE`**, **`MdPE`**, **`RamPE`**,
**`CkMdParity'`** (ProcH/L), **`dHitPerr`**, **`dSTPerr`** (MemC, MemX),
**`MemPE`** (MemX). The Control pass finished the picture: **ContB is where
they all land** — `IOPE`, `MdPE`, `MemPE`, `RamPE` plus IM's own
`IMLHPE'`, `IMRHPE'`, `IMLHPEDly`, `IMRHPEDly` — and the collected result is
`Error'`, which ContA also carries.

`include/cpu.h:203` says it outright: *"We do not yet model parity at the bit
level."* `src/memory.c` contains the string "parity" zero times. What exists
is `io_bad_parity`, a single device-flagged bit on the slow-I/O input path.

**Including IM's own parity, which the first version of this document
claimed we model.** We *carry* those bits — `mb2eb.c` writes `LHpar` and
`RHpar` into every `.eb` because `LoadRam` expects them there — but nothing
ever checks them: `grep -i "lhpar\|rhpar"` over `cpu.c` and `microcode.c`
returns nothing. Carrying a parity bit through a loader is not modelling
parity. (`dorado/CLAUDE.md` never claimed otherwise; it documents the bits'
position in the `.MB` format and stops there, and `cpu.h:203` is explicit.
The overclaim was this document's.)

So a microprogram that induces or checks any parity fault sees nothing
happen. Given that the Hardware Manual lists faults as visible to microcode,
this is a real gap rather than a detail.

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

### 5. A machine-wide serial diagnostic bus — we answer two addresses of it

`DMuxClk` and `DMuxData` appear on **ten of the eleven boards checked** —
ProcH, ProcL, ContA, ContB, IFU, MemC, MemD, MemX, DispY, DskEth. The one
exception is **DispM, the colour board**, which is not on the chain at all.

This is the path Midas reads machine state through: a serial scan chain
reaching every register in the Dorado. We model it as far as exactly two
addresses — `MapIs64K` (0o1511) and `MapIs256K` (0o1512), because
`InitMem.mc`'s `SetDMuxAddress` probe needs them to compute VirtualBanks
(`memory.c:913-945`). Every other address returns nothing.

That is the right call for running software and the wrong one for running
Midas, which is the period debugger and would be the natural way to validate
Phase 2 RTL against the C emulator.

### 6. ECC is read back but never generated or checked

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

Tracing one signal across boards is the highest-value move, and the tool does
not do it directly — loop instead:

```sh
for b in ProcH ProcL ContA ContB IFU MemC MemD MemX; do
    printf "%-6s %s\n" "$b" \
      "$(tools/sil_netlist_report.py --grep QBit $b | grep -c QBit)"
done
```

The whole CPU is now covered. **Six boards remain: DispY, DispM, DskEth,
BaseBd, msa, PCMSA, IOTest, Music** (the last four are test and support
boards, not part of a working machine). **DispY and DispM are the obvious
next targets** — they would settle `docs/color-graphics-todo.md` from the
design data rather than from the manual, and one fact is already in hand:
DispM is the only board in the machine not on the DMux diagnostic chain.
DskEth follows, covering both remaining I/O controllers at once.

## What this is not

It is a **coverage** cross-check, not an equivalence proof. The `.nl` files
give each board's interface, not its logic; proving our datapath computes
what ProcH's 419 packages compute needs the `.wl` wire list and the sheets,
which is Phase 2's job.

What this pass establishes is that the emulator's *shape* matches the
hardware exactly — field widths, register widths, all eight branch
conditions, the Pipe's 16 entries, the ASRN/ProcSRN split, the FA/FB/FC
decode, the 15-line wakeup network with no request line for task 0, the
control-processor handshake — and it names six specific things the hardware
has that we do not: bus and IM parity, the seven-requester Hold network,
`CountMiss`, DRAM refresh, the DMux scan chain beyond two addresses, and ECC
generation/checking.

Worth saying plainly: across eight boards, **every disagreement found was a
gap, not a contradiction**. Nothing the emulator implements turned out to be
the wrong width, the wrong polarity or the wrong shape. For a model built
from a manual and a pile of microcode, that is the result you want.
