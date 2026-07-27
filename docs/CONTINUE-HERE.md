# Continuation handoff — Alto-on-Dorado boot bring-up

## 2026-07-26: SMALLTALK BOOTS AND RUNS ITS INTERPRETER. Two real emulation
## bugs found: IFU fetches used the wrong Pipe ring, and the display-wake
## predicate is a content heuristic that derails non-Alto worlds.

`SmalltalkDorado.eb!1` (equivalently a hand-built `.eb` from `DSemu.mb!1`)
now loads, completes the whole of `InitMem`, and enters the Smalltalk
bytecode interpreter. Previously it hung forever inside the first of
InitMem's three enumerations. Two independent defects were in the way.

**Bug 1 (FIXED, committed): IFU opcode fetches used ProcSRN, not ASRN.**
`InitMem.mc!1`'s `NextMapEntry` does

    T_ LShift[1, logWordsPerPage]C;
    DummyRef_ T, T_ MD;      * "let the memory system do the add"
    ITemp17_ VALo;  T_ VAHi;

i.e. it issues a `DummyRef` and reads the resulting VA back out of
Pipe1/Pipe0. A contiguous `DORADO_PIPEVA_TRACE` showed the walk's VA
advancing correctly (+256/step, 17,405 times) and then snapping back to
exactly `0x000001` — 110 times in a 2 M-cycle window. Tagging each pipe
slot with its owner identified the clobber: the resetting entries were
`kind=9` (`DM_REF_IFETCH`) where the loop's own were `kind=6`
(`DM_REF_DUMMYREF`).

The IFU prefetches autonomously against BR[31] (HM §5, "the IFU is using
the AS for an opcode-fetch reference"), asynchronously with respect to the
microcode. Landing those in `pipe[ProcSRN]` lets a prefetch overwrite the
slot between a reference and the microcode's readback. An autonomous
requestor belongs in the automatically-allocated ring, so `DM_REF_IFETCH`
now selects ASRN alongside IOFetch/IOStore. Measured on DSemu over the same
400 M cycles:

| | before | after |
|---|---|---|
| NEXTMAP1 region | 12,908,745 | **65,539** (= exactly one 65,536-entry pass) |
| `WaitForMapBuf` spin (`0o6247`) | 127,792,449 | **648,740** |
| `CacheFlush` reached | no | yes |

**Bug 2 (FIXED): the display-wake predicate tested controller activity
against zero instead of against the LoadRam baseline.** Display tasks were
woken 3 cycles after Smalltalk's world loaded, at TPCs the world had not
initialized; outranking the emulator task, they starved it:

| | default wake | wake suppressed |
|---|---|---|
| task 0 | 93,353,210 — **93,350,889 of them pinned at `0o334`** | 98,549,913, InitMem completes |
| task 3 | 6,257,534 (BootTransfer wait loop) | 2,446 (real display code at `0o4257`+) |
| other tasks | none | tasks 1, 2, 6, 7, 14, 17 all live |

With the wake suppressed, all four InitMem milestones are reached —
`CacheFlushLoop`, `MapInitLoop`, `EndOfStorage`, **`InitMemDone`** — and
task 0 settles into the interpreter: `0o421` = `LOADX` (a Smalltalk
opcode) at 3,197,445 executions, with `0o4615` = `JUNKTASKLOOP` idling in
task 2. That is genuine Smalltalk bytecode execution.

**The trigger was NOT the Alto DCB heuristic**, as first supposed, but
`machine_ddc_display_active()`'s `output_task_count[terminal_task] != 0`.
Initial drives the terminal task while it runs, so that counter is already
386 at the LoadRam handoff — nonzero for every world the instant it loads.
Instrumenting the predicate over time (`DORADO_DISPLAY_WAKE_DEBUG`) shows
the two worlds are identical at the moment of the wake and diverge
completely afterwards:

| | t=+3 cyc | t=+100M | t=+400M |
|---|---|---|---|
| Alto | `dcb=0 ddc=1 out[3]=386` | `dcb=1 out[3]=221,664` | `out[3]=1,093,495` |
| Smalltalk | `dcb=0 ddc=1 out[3]=386` | `dcb=0 out[3]=387` | `out[3]=387` |

A real Alto world drives the controller past a million outputs; DSemu never
touches it. So the question the predicate wants to ask is **"has the LOADED
world driven the controller?"**, which needs a baseline captured at
LoadRam — exactly the stale-history hazard the function's own comment
already warned about, and which Cedar and Interlisp each worked around with
a bespoke guard. On real hardware DHT/DWT wakeups come from the display
controller, which is off until microcode starts it over slow I/O.

`machine_disp_out_baseline[]` is snapshotted when `ether_loaded_world_cycle`
is set and compared with `>` rather than `!= 0`. It is a **file-scope
static, not a `dorado_machine` member** — adding a member changes the
snapshot ABI and every baked checkpoint fails to restore. After a restore it
stays all-zero, degrading the test to the historical `!= 0`, which keeps
restored checkpoints bit-identical.

With that in place Smalltalk completes all four InitMem milestones with
**default settings and no knobs**. `DORADO_NO_DISPLAY_WAKE=1` remains as a
diagnostic counterpart to `DORADO_FORCE_DISPLAY_WAKE`.

Gates: 11/11 test binaries plus snapshot fidelity pass; the Alto Galaxian
framebuffer is **byte-identical** to baseline (121,602 px, verified by
stash-and-diff); `make verify-cedar-desktop` passes unchanged (246,086 px).
Cedar cold boot is untouched by construction — the germ path never
evaluates the DDC branch.

**Found while gating, NOT diagnosed: the Cedar cold-boot login path is
non-deterministic run to run** — 28,490 vs 28,494 px from the *identical*
binary, so it cannot serve as a byte-exact gate (an earlier note in this
file treating 28,580 px as a fixed baseline is therefore unreliable). Ruled
out so far: no wall-clock/RNG calls anywhere in `src/*.c`; the PDI is not
mutated by the run (`git status` clean afterwards); `dorado_machine` is
`calloc`'d; and `dorado_trace_flag`'s pointer-keyed memo verifies
`keys[i] == name` before returning, so ASLR-varying literal addresses
cannot cross-contaminate flags. Remaining suspects are the `malloc`'d
(un-zeroed) ethernet buffers in `machine.c` (`eftp_words`, `rx_words`,
`rx_attention`) and any other uninitialized read. The Alto path IS
deterministic, so this is specific to the germ/PDI path.

### How the Smalltalk environment is actually set up (period sources)

Settled from the archive, so nobody re-derives it:

- **`chm/dorado/dsemu-src/Smalltalk.midas!17`** is the period setup script,
  and it is short:

      L X Reset ;   smalltalk.midas.  February 25, 1980  11:07 AM...Taft
      L X Do-it  L X SetClk  L X 32  L X Ld DSEMUL
      A19 Val 0
      L X TimeOut 7777777777;  as close to infinite timeout as we can manage.
      L X Go INITMAP;  this is the standard emulator entrypoint.

  Load the `DSEMUL` microcode, then `Go INITMAP` — the *Alto emulator*
  entrypoint, which is exactly what our `.eb` path already does.
- **`DSemu.cm`** shows why: DSemu is the **Alto emulator plus Smalltalk
  microcode**, built from `AltoEmuDefs ^AltoEmu AltoVarious AltoBitBlt
  SMTraps BCPLRuntime ^AltoEther AltoEtherEmu AltoDiabloDisk AltoDiskSubrs
  ^AltoDisplayMain ... ^DSmallint DSmallops ...`. That is why `DSemu.mb`
  carries `AEMU*` labels.
- **`DSmallDefs.mc`** header: "Dorado Model 1, **XM version**", and notes
  registers "known to the **Nova code**" — Smalltalk-76's Alto-side code.

So there is **no standalone virtual-image file**. Smalltalk comes up the way
Alto software does: off a Diablo pack. The ST-80 `VirtualImage` in
`chm/archiveorg/smalltalk-80/` is a red herring for this world.

**We have the packs.** `AltoInfo/ContrAlto2-beta/Disks/xmsmall.dsk` ("XM
Smalltalk", matching DSemu's XM version) and `maststlk.dsk` ("Master
Smalltalk"). Recipe:

    ./build/dsk2trident --all-heads ../AltoInfo/ContrAlto2-beta/Disks/xmsmall.dsk /tmp/xmsmall-trident.pack
    ./build/dorado --cycles 300000000 --eb WORLD --disk 0=/tmp/xmsmall-trident.pack \
        --boot-reason disk --no-alto-boot --out /tmp/out.pgm

Remember `--no-alto-boot`, or the run silently falls back to Ethernet and a
"renders from pack" result is not what it looks like.

### Where Smalltalk stops now: a page-377 write-protect fault storm

| world | result on `xmsmall` pack |
|---|---|
| `worlds/aemu.eb` | **2,709 px** — "XEROX Alto Executive" at a `>` prompt |
| `SmalltalkDorado.eb!1` | 0 px |

Both worlds now *run* (that was today's fix); they diverge on the disk:

| | AEmu | DSemu |
|---|---|---|
| disk counts | `ctrl=476 xfer=469 rs=469 fr=126482` | **`ctrl=8 xfer=1 rs=1`** |
| task-13 (DWT) | 719,269 | **0** |
| task-17 (fault) | **2** | **8,832,783** |

DSemu does exactly ONE sector read, then the fault task storms. The
`memref` dump names it:

    last=STORE va=0177400 fault=WRITE_PROTECT fault_va=0177400
    fault_real=0o2035 fault_task=0.0 fault_mb=36

`0177400` is **page 377**, and `InitMem.mc` write-protects it on purpose —
this is the XM Alto emulation's I/O-page trap:

    * Then, if XM Alto emulation is desired, write-protect page 377 of
    * the 64K space for Alto emulation (MDS).
        T_ 177400C;
        ITemp0_ A0, BRLo_ T;
        Cnt_ 17S;
        ITemp0_ (Flush_ ITemp0), Carry20, Branch[., Cnt#0&-1];
        ITemp0_ TIOAwProtect, Call[IWriteMapFlags];

`GetEmulatorMapParams` returns "1 to write-protect page 377, for XM Alto
emulation; 0 to emulate a non-XM Alto" — so AEmu (non-XM) never arms it and
DSemu (XM) always does. Page 377 is the Alto's memory-mapped I/O page, so
the trap IS the mechanism by which the emulator intercepts Alto I/O stores.

**The fault storm is therefore expected; failing to RESOLVE it is the bug.**
The emulator task spins in a 2-instruction loop at `0o2035`/`0o2066`
(8,754,528 each, in the `BLKLP`/`BLKNPF` BitBlt region) while the fault task
runs a 6-instruction handler at `0o6032`-`0o6046` 519,576 times. Handler
runs, returns, store re-faults, forever.

### The trap handler, and why it never terminates (leading hypothesis)

The AEmu microcode sources were **not** in the local tree; they are now
mirrored to `chm/dorado/aemu-src/` from
`https://xeroxparcarchive.computerhistory.org/_cd8_/doradosource/AemuSources.dm!82_/`.
Note the URL case: the volume is lowercase (`_cd8_/doradosource`) but the
archive name is display case (`AemuSources.dm!82_`) — the all-lowercase form
from `cross-reference.html`'s href 404s.

`XMFaultTask.mc` (Taft, 3-Sep-1980) is the handler, and its header states
the contract:

> This version does a partial emulation of an Extended Memory Alto.
> Specifically, it supports changing the emulator task's alternate bank
> register (for XMLDA, XMSTA, and BITBLT) and the display task's normal
> bank register. **If a fault occurs as a result of storing into
> write-protected page 377**, and the emulator's or Alto display task's
> bank register is the word addressed, allow the store to proceed and also
> update the base register. **If any other word in page 377 was addressed,
> ignore the store.**

The fault task redirects the *emulator* task into `Fault0` by writing its
TPC:

    EmuFault:  FaultVal_ DBuf;
               RdTPC_ EMU;
               FltEmuPC_ NOT (Link);   * TPC data is complemented
               Call[GetEmuFaultPC];
               LdTPC_ T;               * <-- redirect EMU to Fault0
               Block, Branch[FaultTask];

**All of that machinery works in our emulator.** Measured (real addresses
from `mbdis`: `FAULT0`=`0o6061`, `MAPFAULT`=`0o326`, `IGNORESTORE`=`0o346`,
`XMBSTOREONLY`=`0o352`, `TASK0XM`=`0o367`, `DISPXM`=`0o353`):

| label | hits |
|---|---|
| `FAULT0` | reached |
| `IGNORESTORE` | reached |
| `MAPFAULT` breakpoint | **0** |
| `XMBSTOREONLY` / `TASK0XM` / `DISPXM` | **0** |

So `RdTPC`/`LdTPC` redirect correctly and the handler decodes without
hitting its "can't handle" breakpoints. `DORADO_PIPEVA_TRACE` shows the
handler's `T_ VAHi` / `FltPipe1_ VALo` (real `0o6045`/`0o6056`, 519,575
reads each — one per fault) always returning **`va=0x000FF00` = `0177400`**,
the first word of page 377, never the bank register at `0177740`. The
microcode is therefore right to take `IgnoreStore`.

**The problem is that the VA never changes.** 519,575 faults, all on
`0177400`: the same Alto instruction re-executing forever, never advancing.
`IgnoreStore` ends with `Branch[AEmuReschedule]`, and the source says why
that is supposed to be safe:

    * Restart the current instruction -- the instruction following the
    * one that caused the fault.  All instructions must wait for faults
    * before changing any permanent state, and instructions that can cause
    * faults (presumably only STA) must do so as their last
    * microinstruction, so it is guaranteed that a new instruction will
    * have begun by the time the fault occurs.

**Leading hypothesis (NOT yet proven): we report the map fault too early.**
The design depends on the fault arriving late enough that the emulator has
already begun the *next* Alto instruction; `AEmuReschedule` then resumes
that one. If our fault is delivered synchronously with the store, the same
STA restarts and loops forever — precisely the observed behaviour, and it
explains why the VA is frozen.

Partial confirmation: `DORADO_FINAL_DEBUG` gives

    ifu={active=1 op=142 alpha=005 len=2 pcx=000006 pcf=000010 dispatch=519727}

`dispatch=519,727` is ~one IFU dispatch per fault (519,575), and **`pcx` is
frozen at `000006`** — the same Alto instruction re-dispatched half a
million times, matching the `fault_pc=000006` in the `memref` dump. So the
Alto PC provably never advances. (`pcf=000010` is ahead of `pcx`, so the
byte cursor did move; it is the instruction restart that repeats.)

### RESOLVED to a single instruction: BLT restarts instead of resuming

Diffing the Alto instruction streams of the two worlds on the same pack
(`DORADO_IFUDISP_TRACE`) isolates it exactly. They are identical for seven
dispatches and then:

| # | AEmu (works) | DSemu (hangs) |
|---|---|---|
| 6 | `opva=0000350 op=142` | `opva=0000350 op=142` |
| 7 | `opva=0000350 op=142` | `opva=0000350 op=142` |
| 8 | `opva=0000351 op=134` | `opva=0000350 op=142` (forever) |

`op=142 alpha=005` is Alto instruction `0o61005`, which `S-Group.mc` names:

    * BLT (61005)
    * Accepts AC0: first source -1, AC1: last destination, AC3: -count.
    * If interrupted, returns with ACs prepared for remainder of transfer.
    BLT:    Stack&+3_ (Stack&+3)+1, At[SD400, 5]
    BLTlp:  Stack_ (Fetch_ Stack)+1, T_ MD
            ETemp_ (Store_ ETemp)+1, DBuf_ T, Branch[BLTlp, Cnt#0&-1]
    BLTlpx: T_ ETemp3, Cnt_ 20S, Branch[BLTint, Reschedule]
    BLTint: Stack&+3_ (Stack&+3)-(2C), Branch[BLKint]  * AC3_ -count remaining

`BLTlp` is a two-microinstruction loop — matching our two spin PCs
`0o2035`/`0o2066` at 8,754,528 each, which is ~16.8 per fault, i.e. one
16-word munch per fault. (`0o61004` is SIO; BLT is 61005. Not an STA, which
matters below.)

**The ACs are identical in both worlds at that dispatch**, so there is no
upstream divergence:

    AEMU:  stkp=004 acs=000000,000000,006126,006373
    DSEMU: stkp=004 acs=000000,000000,006126,006373

BLT computes `ETemp = AC1 + AC3 + 1 = 0o6374` and sweeps upward over ~62,208
words, so the block legitimately runs off the top of memory and **through
page 377**. AEmu (non-XM) writes straight through: `DORADO_LOAD_TRACE_VA=177400,177400`
records **exactly one** reference, after which it continues. DSemu (XM)
write-protects that page, so the same store traps.

> **Correction.** An earlier revision of this section claimed AEmu "never
> references `0177400`, zero hits". That was a trace-gating artifact -- the
> BLT runs at cycle ~20.9 M, before the `DORADO_TRACE_GATE=32100000` that
> was in force, so the store was filtered out. Ungated, the reference is
> there. Same trap as [[gate-pc-traces-after-world-load]]; the gate that is
> mandatory for PC histograms silently lies about anything that happens
> before the world load.

**The bug is the restart, not the trap.** `XMFaultTask` states its own
precondition:

    * Restart the current instruction -- the instruction following the
    * one that caused the fault. ... instructions that can cause faults
    * (presumably only STA) must do so as their last microinstruction, so
    * it is guaranteed that a new instruction will have begun by the time
    * the fault occurs.

So `AEmuReschedule` is meant to consume the opcode the IFU has **already
prefetched** — on real hardware the IFU has fetched `0o351` while BLT is
still running, and after `IgnoreStore` execution continues there, abandoning
the rest of the BLT. Our IFU instead re-dispatches the *same* opcode: every
re-dispatch shows `pcf=0o6` with `pc_after=0o10`, i.e. the byte cursor is
reset to 6 each time rather than advancing to the prefetched 8.

**Everything on our side that this path touches has now been checked and
matches the manual and the microcode:**

- `RdTPC`/`LdTPC` are implemented and the redirect into `Fault0` works.
- `Fault0` decodes correctly and takes `IgnoreStore`, never its
  "can't handle" breakpoints.
- **`PCX` is right.** `AEmuReschedule` -> `RestartIFU` resumes via
  `T_ NOT (PCX')` -> `StartIFU: PCF_ T`, and HM Section 6.2 says
  "`B<-PCX'` reads the PC of the **current** opcode, and `PCF<-B` does not
  change `PCX`". Our `ifu_pcx = ifu_pcf` latch at dispatch matches that, so
  restarting at PCX re-executing BLT is the specified behaviour, not a bug.
  (An earlier revision of this file proposed "make the post-fault dispatch
  use the prefetched next opcode" -- that would contradict HM Section 6.2
  and is withdrawn.)
- **A fault does not assert `Reschedule`.** That was the one mechanism that
  would let BLT exit through its own `BLTlpx` checkpoint and save
  `AC3 = -count remaining`, making progress across restarts. Per HM Table 20
  the Reschedule branch condition is set only by the microcode `Reschedule`
  function and cleared by `NoReschedule`; `src/cpu.c` does exactly that.
  Ruled out.
- `StkP` is **not** corrupted. It varies per instruction in AEmu too
  (001, 002, 004, 003 ...), so the differing `stkp` values across DSemu's
  restarts are normal, not damage.

**So as modelled, XMFaultTask + a BLT sweeping through page 377 cannot
terminate**: the store is ignored by design, the instruction restarts by
design, and nothing advances the ACs. Two readings remain:

1. **The Alto-level state is wrong**, i.e. `AC3 = 006373` at Alto PC `0o350`
   is not what a real machine would have, and the enormous 62,208-word BLT
   is itself the symptom. Note both our worlds share our IFU/ALU/STK
   emulation, so their agreeing on the ACs proves only self-consistency --
   **an internal comparison cannot catch a bug they both inherit.**
2. **This boot path is not the one DSemu was used with**, and the period
   setup drove Smalltalk differently (`Smalltalk.midas!17` only loads
   DSEMUL and does `Go INITMAP`, so how the pack got booted is not
   documented there).

### ORACLE RESULT: our Alto-level state is CORRECT. Reading 1 is dead.

Cross-validated against ContrAlto, and **our ACs match exactly**:

| | AC0 | AC1 | AC2 | AC3 |
|---|---|---|---|---|
| ContrAlto at PC `0o350` | `177777` | `176777` | `0` | `0` |
| Dorado `aacs` at PC `0o350` | `177777` | `176777` | `0` | `0` |

> **Correction, and a trap to avoid.** The previous analysis here claimed
> `AC1=0, AC3=006373` driving a 62,208-word BLT. That read the **wrong
> field** of our own `DORADO_IFUDISP_TRACE` line, which prints both:
>
>     stkp=004 acs=000000,000000,006126,006373 aacs=177777,176777,000000,000000
>
> `acs=` is **StkP-relative** and `src/cpu.c` already comments that it "is
> wrong for comparison once StkP moves off 1"; `aacs=` is the fixed Alto AC
> window `STK[1..4]` per `Start.mc`. Use `aacs`. Everything derived from
> `acs` in earlier revisions of this section was wrong.

Corrected reading of the instruction: `AC3 = 0`, so `-count = 0` and BLT
sweeps the **whole 64 K**, with `ETemp = AC1 + AC3 + 1 = 0o177000` -- which
enters page 377 just 256 words later. The sweep is authentic, not an
artifact, and both non-XM emulations (our AEmu, ContrAlto) run straight
through it.

**So the Dorado emulator is validated at the point of divergence.** The
hang is not a bad AC, not PCX, not Reschedule, not StkP, and not the trap
machinery -- all checked. What remains is a genuine incompatibility:
`XMFaultTask` requires that "all instructions must wait for faults before
changing any permanent state, and instructions that can cause faults
(presumably only STA) must do so as their last microinstruction". **BLT
violates that requirement**, so a BLT sweeping through write-protected
page 377 cannot make progress under this fault handler -- on real hardware
either.

### How every Dorado world was boot-selected (period source, reusable)

`DSemuRelease.cm` shows the release builds **two** files:

    LoadMB/e DoradoSmalltalk.eb/o DSemu.mb 1076/s
    LoadMB/e DoradoInitialSmalltalk.eb/o InitialSelect.mb 40

The second is a bootstrap, and `InitialSelectMain.mc` (Taft, 19-Jun-1982,
local copy at `chm/doradosource/BootstrapSources.dm!12_/`) documents the
whole scheme -- "T[0]=0 => Ether-boot microcode whose BFN is given in
T[8:15]":

| entry | T | meaning |
|---|---|---|
| `InitialEtherMesaEntry` | `110C` | Mesa (this is our `DEF_ETH_110`) |
| `InitialEtherSmalltalkEntry` | **`111C`** | **Smalltalk** |
| `InitialEtherLispEntry` | `112C` | Lisp |
| `InitialEtherCedarEntry` | `113C` | Cedar |
| `InitialEtherTestEntry` | `114C` | Test |
| `InitialDiskEntry` | `177400C` | installed Pilot "soft" microcode |
| `InitialOverlayEntry` | `177401C` | next overlay from the same `.eb` |

So Smalltalk's microcode is ether-booted as **BFN `0o111`**, which is the
path we already use via `--eb`. `InitialDiskEntry` loads "the one pointed to
by the softMicrocode entry of the physical volume root page, installed by
Othello's Pilot microcode fetch command" -- worth remembering for the Iago
track too. Holding **`P`** at boot forces the disk entry.

### It is NOT Smalltalk-specific: DSemu cannot boot ANY Alto pack

| pack | AEmu | DSemu |
|---|---|---|
| `xmsmall.dsk` | 2,709 px | 0 px |
| `maststlk.dsk` | 2,285 px | 0 px |
| **`games.dsk`** | **2,104 px** | **0 px** |

`games.dsk` under DSemu fails with **instruction counts identical to the
digit** to the Smalltalk packs (`0o2035`=8,754,528, task-17=8,832,783, same
`WRITE_PROTECT va=0177400`), so the hang does not depend on pack contents at
all -- it is in the Alto boot path every pack shares.

**That reframes the conclusion of the previous section.** DSemu was a
shipped, used microcode; it obviously booted Alto packs on real hardware. So
"this boot path is not how DSemu was driven" is no longer tenable, and the
defect is much more likely ours after all, somewhere in the XM page-377
path.

Ruled out along the way: the `task=14` (disk task) fetch page faults on
vacant bank-1 pages are **normal** -- AEmu logs exactly the same 1,245 of
them over the same window and boots fine. Not a lead.

### Smalltalk-76 boots and runs; the screen is the open item

**How to launch it** (settled by asking the Executive itself, via `?`):
Smalltalk on `xmsmall` is NOT a `.RUN`. The directory holds
`Smalltalk.Sources.5.5k`, `smalltalk.syms` and **`xmsmall.boot`**, so it is
started with the Executive's `Bootfrom` command. (`Smalltalk.Sources.5.5k`
dates it **Smalltalk-76 v5.5k**. A `SMALLTALK.RUN` found by scanning the raw
image is file CONTENT, not a SysDir entry -- don't chase it.)

    ./build/dorado --cycles 2000000000 --eb '../chm/microcode/SmalltalkDorado.eb!1' \
      --disk 0=/tmp/xmsmall-trident.pack --boot-reason disk --no-alto-boot \
      --key-hold 3000000 --type-at 350000000 --type 'Bootfrom xmsmall.boot
    '

**`--key-hold 3000000` matters.** At the 600,000-cycle default, `Bootfrom`
arrives as `Botfrom` -- the doubled `o` collapses into one press because the
Alto path applies keys directly with no pacing (the paced queue is
Cedar-only). Worth fixing properly.

**Oracle reference.** ContrAlto boots the same pack with the same command
straight into Smalltalk-76 -- "Top View", the "Classes" browser with its four
panes, and a "UserView workspace". So the pack and command are right, and any
difference is ours. Reproduce with
`DOTNET_ROLL_FORWARD=Major dotnet run --no-build -- ../ContrAlto2-beta/Disks/xmsmall.dsk /tmp/ca_st.pgm 12 "Bootfrom xmsmall.boot" 25`.

**Where we are.** The boot runs for real: 262.5 M task-0 instructions, 2,664
disk sector reads, and a broad Alto opcode mix. The display list is correct
and stable -- `DORADO_DCB_TRACE` reports a 3-entry chain from `MDS+0420`,
`nwrds=40` (32 words/line), `sa=076400`, 800 lines, in **bank 0** (so this is
not an extended-memory display; that hypothesis is dead). We render exactly
what is at `0o76400`, and it is noise, because **nothing ever draws there**:

- `BITBLTA` (`0o13124`), `BITBLTSUB` (`0o16260`) and `BITBLTDONE` (`0o13203`)
  are executed **zero** times, and Smalltalk-76 draws everything via BitBlt.
- Every dispatched opcode is **`ins0`** -- the Alto instruction set. Dorado
  Smalltalk is a *second* instruction set (the `DSmall*` modules; `SMTRaps.mc`
  declares `InsSet[AEmuInsSet, 1]`), and we never switch to it.

So `xmsmall.boot` loads and runs as Alto code but the machine never enters the
Smalltalk instruction set. **That is the next thing to chase**: find what
selects the Smalltalk InsSet (`InsSetOrEvent←B`, B[0]=1 loads InsSet from
B[6:7]) and why it never fires. Note the ether-booted DSemu run *does* reach
Smalltalk microcode (`0o421` = `LOADX`, 3.2 M executions), so the second
instruction set works -- it is the disk-boot path that never gets there.

### FIXED: a WP fault must come from a store that MISSES (HM p.46)

**`DSemu` now boots every Alto pack, with pixel counts identical to AEmu:**

| pack | AEmu | DSemu before | DSemu after |
|---|---|---|---|
| `games.dsk` | 2104 | 0 | **2104** |
| `xmsmall.dsk` | 2709 | 0 | **2709** |
| `maststlk.dsk` | 2285 | 0 | **2285** |

HM page 46 "Faults" enumerates the WP-fault sources exactly:

> **WP fault** — `Store←` that **misses**, `IOStore←`, or dirty-victim write
> with WP true.

A `Store←` that **hits** is not one of them. `src/memory.c` faulted on any
store whose cache line carried WP, so a write-protected page faulted on
*every* store rather than only the first. The `memref` dump said so all
along -- `miss=0` beside `fault=WRITE_PROTECT`.

The correct behaviour on a hit is to **drop the store silently**: no fault,
no data change, and critically **no dirty bit**. Dirtying it manufactures
the dirty-victim WP fault that page 46 says "should not occur", which is
the same point page 47 makes: "Before turning on WP, a flush prevents dirty
cache entries from being written into the now write-protected page."
(I hit exactly that when I first fixed only the fault and let the store
through: the fault task then spun on its `NotMapFault` breakpoint.)

`XMFaultTask.mc` independently confirms the semantics -- its `XMBStoreOnly`
path brackets the real store with `Flush_ FltPipe1` *precisely* to force the
next store to miss and fault again, which would be pointless if hits faulted
too.

So the sequence works as designed: InitMem flushes page 377 and sets WP; the
Alto boot BLT sweeps 64 K into it; the first store misses and faults;
`XMFaultTask` ignores it; the reference proceeds so the line is now cached
clean; and every subsequent store hits and is dropped. BLT runs to
completion.

Gates: 11/11 test binaries plus snapshot fidelity; Alto Galaxian
**byte-identical** (121,602 px); `verify-cedar-desktop` unchanged (246,086
px); 5/6 PARC hardware diagnostics pass. `test_map_write_protect` was
updated -- it had encoded the imprecise reading and its own leading `Fetch←`
filled the line, so its store was a *hit*; it now asserts both cases
(hit → dropped, then `Flush←` → miss → faults).

> **Note, pre-existing and NOT from this change:** `IfuComplex` fails at
> `IMISCERR5B` after 8,873,980 steps -- **identically before and after**,
> verified by stash-and-compare. Memory records all six diagnostics passing
> on 2026-06-26, so this regressed somewhere between then and now and wants
> its own bisect.

### Superseded lead (kept for the trail)

Dropping non-bank-register stores in page 377
is harmless in itself (the Alto's top page is largely memory-mapped I/O, and
a 64 K BLT sweeping through it does not care). The damage is that the
instruction is *restarted*. So the question is narrow: what does real
hardware do to a long, multi-store instruction when one of its stores is
write-protected? HM Section 6.2 fixes `PCX` = current opcode, and
`XMFaultTask` assumes only STA faults and only on its last microinstruction
-- an assumption BLT violates on every boot. Something must break that
cycle, and it is not PCX, Reschedule, StkP, the ACs, or the trap machinery
(all verified). Look next at the fault *abort* semantics: whether a
write-protected store should suppress the fault-task wake entirely once
`IgnoreStore` has run, or whether the Dorado retries/aborts differently than
we model. HM Memory Section pp. 46-48 is canon here per memory
[[memory-fixes-validate-against-hw-manual]].

**Oracle harness note.** `AltoInfo/` is gitignored, so the probe added to
`AltoInfo/contralto-headless/Program.cs` is recorded here to be
recreatable. Insert before `sys.Controller.StartExecution(AlternateBootType.Disk)`:

    string probe = Environment.GetEnvironmentVariable("PROBE_PC");
    if (!string.IsNullOrEmpty(probe)) {
        int want = Convert.ToInt32(probe, 8);
        sys.Controller.Reset(AlternateBootType.Disk);
        int hits = 0; ushort prev = 0xffff;
        for (long i = 0; i < maxSteps && hits < wantHits; i++) {
            sys.SingleStep();
            ushort pc = sys.CPU.R[6];
            if (pc == want && prev != want) {
                hits++;
                // Alto ACs are backwards in the R file (altoIIcode3.mu:
                // "$AC0 $R3; AC'S ARE BACKWARDS"); PC is R[6].
                Console.Error.WriteLine($"PROBE step={i} " +
                    $"AC0={Convert.ToString(sys.CPU.R[3],8)} " +
                    $"AC1={Convert.ToString(sys.CPU.R[2],8)} " +
                    $"AC2={Convert.ToString(sys.CPU.R[1],8)} " +
                    $"AC3={Convert.ToString(sys.CPU.R[0],8)}");
            }
            prev = pc;
        }
        return 0;
    }

Run with `DOTNET_ROLL_FORWARD=Major PROBE_PC=350 dotnet run --no-build --
<pack.dsk> /tmp/out.pgm` (the project targets .NET 8; only 10 is installed
here, hence the roll-forward). Note `SystemType.OneKRom` is already
"Alto II **XM**", so no config change is needed for the XM packs.

**Do not use that harness's pixel count as a boot indicator** -- it reports
~489 K ink pixels (~99.5 %) for every pack including `games.dsk`, so it is
saturated and says nothing about whether the pack booted.

Caveat from memory [[contralto-oracle-only-architectural]]: ContrAlto is a
valid oracle for architectural state but NOT for I/O timing.

### Superseded analysis (kept for the reasoning trail)

Two candidate explanations were open before the instruction-stream diff
above resolved it; hypothesis 2 is now ruled out (the ACs match, so nothing
diverges upstream):

1. **Fault delivered too early** (above): the emulator restarts the same
   STA instead of the next instruction. Check our fault-wake timing in
   `src/cpu.c` (`wakeup_pending |= 1u << 15`) against the Hardware Manual's
   fault-reporting latency.
2. **The store address is wrong upstream.** The faulting VA `0177400` is
   the first word of page 377, and `XMFaultTask` only ever expects the bank
   registers (`0177740`, `0177751`) there. If the Alto code at PC 6 should
   not be storing to `0177400` at all, then the fault is a *symptom* of a
   bad address computation earlier and the timing is a red herring. Note
   the handler's own caveat — "instructions that can cause faults
   (presumably only STA) must do so as their last microinstruction" — and
   that `0o2035` is in the BitBlt `BLKLP` region, which is NOT an STA.

Discriminate by disassembling what the pack's boot sector puts at Alto
location 6 and what `op=142` is, then comparing against ContrAlto running
the same pack (`AltoInfo/Contralto2-2.0-Beta/`), which is a working oracle
for both the Alto instruction stream and what the XM bank-register trap
must ultimately accomplish.

**Method note.** All three of the earlier wrong calls on this world traced
to one mistake: counting PCs without gating the trace to after the world
load at cycle ~32 M. Always use
`DORADO_TRACE_GATE=32100000,<end>` — ungated counts are dominated by
Initial's microcode sitting at the same IM addresses and are meaningless.

## 2026-07-25 (late): A CEDAR VOLUME BUILT FROM A BLANK DISK, WITH SOFTWARE.

Three make targets take 60 MB of zeros to a Cedar volume that boots from
disk to the full Viewers desktop and carries a package collection:

```
make cedar-diskvolume           # Iago builds the volume on RD1
make cedar-diskvolume-desktop   # boot it; Cedar installs itself (167,732 px)
make cedar-diskvolume-software  # one Bringover per CedarChest package
```

158 files / 19.94 MB on the volume; `List ///*.df` shows 75 DFs / 862 KB,
including ten CedarChest packages with real 1985-86 dates (Abutters, AIS,
AISImages, AISViewer, CardTable, ChessHack, Clock, DoradoArchive,
PopUpButtons, ViewRec) plus the Cedar runtime DFs their closures pulled.

**The blocker that had to be found first.** The Iago-built volume booted,
logged in, then died with `FS.Error: Local volume's permanent data
structures are inconsistent`. Running Iago ON the booted volume
(`--boot-switches l`) and using `Describe Logical Volumes` printed the
discriminator -- "Don't understand the VAM" / "VAM fp: [id:1B,da:1B] ...
the file has been deleted" -- so it was `vamStatus`, not `rootStatus`.
Label dumps across build stages showed physical page 4 (the VAM header,
logical page 1) correct after Erase and clobbered after the Installs, with
pages 4..14 -- ELEVEN of them -- overwritten. `InitialDiskDorado.eb` is
5192 bytes = exactly 11 pages: **Install Initial Microcode** writes it into
the hard-microcode region at low physical pages, and `IagoOps.ReservePages`
reserves that region only when the drive ordinal is 0. Building on RD1 left
those pages inside the logical volume. Skipping that one step (the
emulator supplies microcode with `--eb`) makes the volume work.

**Period documentation, which corrected how we were driving it.**
`ReleaseMessage.tioga!9` net boots Cedar first so RD0 is free for `Create
User World` -- hence that macro's RD0-only target, and hence our RD1 path
being the document's own "steps by hand". `CedarChestDoc.tioga` gives the
one-DF-per-package convention; `CommandToolCommands.tioga` records that
Bringover is `FS.Copy` with `attach: TRUE` ("Case 4 ... is used by
BringOver").

**Two numbers worth keeping.** Bringover cost is dominated by the IMPORT
CLOSURE, not payload -- Interpress is 47 files but pulls most of the Cedar
runtime and had not finished in 15 B cycles; the default package set is the
self-contained ones at 6 B each. And Cedar 6.1 still streams its base
system: `Basic.Loadees` names all 34 loadees by unversioned remote path, so
the server is consulted every boot. Both escapes are closed (a fuller
`CedarDorado.boot` exists only as 1983 builds; FS attachments cannot
redirect a remote name).

Also new: `--storage-modules N`, `MAX_TYPE_EVENTS` 16 -> 64 (driving Iago
needs 22 answers for the volume build alone), and
`tools/pdi_fix_root_checksums.py` (Cedar zeroes a root checksum while the
volume is mounted and rewrites it on a clean dismount, which the emulator
never performs).

## 2026-07-25: IAGO RUNS. Cedar creates its own physical and logical volumes.

**The Othello/Iago dead end is open.** `--boot-switches l` (new) reaches
Iago's "Do you want to use Iago, the Cedar disk utility program?", and
answering `y` gives its live `>` command interpreter. From there Cedar's
own installer drives our emulated disk: it reads the working volume
correctly and it **creates a physical and a logical volume on a blank
disk**. Screenshots: `docs/images/cedar-iago-prompt-2026-07-25.png`,
`cedar-iago-describe-volumes-2026-07-25.png`.

**The mechanism, from period sources.** `GermSwap.Mesa` defines
`Switch: TYPE = MACHINE DEPENDENT {zero..nine, a..z}` and
`Switches: TYPE = PACKED ARRAY Switch OF BOOL`, and `GermSwap.InLoad`
plants them at `PrincOps.SD[sBootSwitches]`; `PrincOps.mesa!1` gives
`SD = 1100B`, `sBootSwitches = 142B`, so the location is germ MDS +
`1242B` and `l` is ordinal 21 = word 1, mask `0400B`. `GermSwapImpl`
reads it back, `BootingImpl` copies it to `Booting.switches`, and
`IagoMainImpl` gates on `Booting.switches[l]`.

**Three traps, worst first:**

1. **`IagoMainImpl.DoIt` calls `IagoCommands.Login` ITSELF, before the
   switch test.** The Cedar login prompt we have always seen IS Iago
   running. The first runs looked like the switch did nothing because
   they stopped at login, several steps short of the test. Type `Guest`
   + 2 Returns first.
2. **A single plant is wiped.** GERMREMAP relocates the germ over that
   MDS a few million cycles after the germ's data pass — measured zero
   again by cycle 70 M. The emulator re-plants whenever the location
   reads all-zero, sampled every 100 K cycles, and stops after 8 stable
   samples (converges 68.9 M).
3. **`dorado_machine_create` copies config field by field.** A new
   `dorado_machine_config` member is dropped unless added there too. The
   symptom is an option that parses and does nothing.

**Also landed: the disk bridge stopped assuming a link convention.** The
germ's polled IOCB path read `PhysicalRoot.bootingInfo.firstLink` as a
flat PDI page number; Pilot (and anything Iago installs) writes CHS. It
now decodes both ways at mount and keeps the reading whose target page
LABEL matches that entry's fileID and firstPage — the medium decides.
All shipped volumes still read flat and boot bit-identically.
`tools/pdi_boot_links_to_chs.py` makes the CHS form for testing; it
yields germ link `(3,20)`, the value the period notes record.

**Verified through our disk path:** `Describe Machine` (Dorado, 16339
real pages, microcode 12-May-1984), `Describe Drives` (RD0..RD3, 114100
pages each = 815 x 5 x 28), `Describe Physical Volumes` (CedarWork on
RD0, 48564 free, sub-volume at physical page 84), `Create Physical
Volume` on RD1 (writes `PhysicalRoot` seal `121212B` at page 0), and
`Create Logical Volume` (114096 pages, `LogicalRoot` seal `131313B` at
page 3). Blank targets from `tools/pdi_create_blank.py`.

**Checkpoints:** `dorado/build/good-packs/cedar-iago.{snap,pdi}` restores
to Iago's `>` prompt; `cedar-iago-vol.{snap,pdi}` + `cedar-iago-target.pdi`
restores with both new volumes already created.

**Scripting Iago:** `IagoOps.GetCommand` auto-completes on a SPACE, so
typing a command's FULL name leaves the tail as input for the next prompt.
Type only through the token that makes it unique — `Describe P` + CR.

**Next:** Erase Logical Volume (the first whole-volume write — 114 K page
labels plus the VAM), then Install Boot File / Germ / Cedar Microcode,
Create VM Backing File, and the Cedar install onto the new volume.
`Create User World` is the macro that does the whole sequence and prints
its plan first.

## 2026-07-21: PLAYABLE CHESS OFFLINE, the font mechanism cracked, and the Othello/Iago track mapped.

**A real chess program runs in Cedar with zero network.** ChessHack's board
paints with all pieces in the period Chess40 font, and the whole thing
restores and runs offline (0 STP fetches). New web dropdown option **"Cedar
6.1 — apps demo (Chess, Clock, offline)"** at alanswx.github.io/xerox-dorado
(commits `d5f9daa`, `ebb2cdd`, `23888c1`, deployed green). Board screenshot:
`docs/images/cedar-chesshack-board-2026-07-21.png`.

**Cedar has NO font catalog — the 6-attempt font saga, resolved.**
`ImagerFont.Find[name]` does `FS.EnumerateForNames["///Fonts/" + name +
".*!h"]` over the volume's FS name B-tree; `ImagerTypefaceImpl` ERRORs
`$fontNotFound` unless the stored name carries the `Xerox` component
(`///Fonts/Xerox/TiogaFonts/Chess40.*`). Fonts get that name ONLY from the
installer's cold-boot attach of the font DFs. **Fix: one CR-terminated line
in `chm/cedar/stp-root/CedarFonts/Top/TiogaFonts.df`** (`Chess40.ks!1
18-Mar-86 06:18:12 PST`, size 2624) **+ a cold-boot rebake** (`make
cedar-desktop-snapshot`) so the installer attaches it like the other 187.
Two shortcuts that BOTH crash the world to the blank "Type Key" herald (81
px): a runtime `Bringover TiogaFonts` (pulls the imager import closure,
poisons the loadstate) and rusty-backup file INJECTION into the volume
(Cedar's live FS can't digest the modified client directory — slows ~4x then
crashes). Full detail: memory `cedar-font-install-attach`.

**The offline demo model = Path B (pre-load into the checkpoint's memory).**
Because on-disk injection crashes Cedar, the offline apps demo loads
ChessHack + Clock into memory + pre-caches the Chess40 font, then snapshots.
It restores to a clean desktop with app ICONS; clicking ChessHack opens the
board from cache, no network. `make cedar-demo-snapshot` (native) /
`cedar-demo-web-snapshot` (wasm) / `run-cedar-demo-sdl`;
`cedar-demo-native.{snap,pdi}.gz` + `web-assets/cedar-demo.{snap,pdi}.gz`.
The corpus browser checkpoint (1109 CRC-recovered names) + desktop were also
rebaked and redeployed this session (the corpus web assets never existed, so
`make web` and the last two Pages deploys had been failing).

**Othello / clean system-volume track — dead-end killed, Iago is the path.**
Standalone `OthelloDorado.boot!8` cannot boot: its header is `062400`
(Cedar-6.0), the germ's `DoInLoad` (`BootSwapGerm.mesa`) requires
`currentVersion`=`063000` (germ-6.1.6) or the `150` outload seal. Patching
the one-byte version CLEARS `germBadBootFile` but the load stalls — the
`062400` header layout physically differs (an extra word). The `062400`-era
germs (`!4`/`6.0.6`/`5.3.1`) fault in `TrapsImpl` before `DoInLoad`. **The
authentic path is Iago inside the working Cedar 6.1** (present locally,
`063000`-compatible, writes via normal Pilot `channel.DoIO` IOCBs our write
path models). Iago is a BOOT-TIME program gated by `Booting.switches[l]`
(fires "Do you want to use Iago?"; `Booting.switches ← GermSwap.switches`).
RULED OUT this session: the interpreter route (`Eval Booting.switches['l] ←
TRUE` is syntax-rejected; `Run Iago` says "previously loaded" so `DoIt`
won't re-fire) and the herald `Boot` button (only SELECTS on --click).
Plain `Bringover [Cedar]<Cedar6.1>Top>Iago` DOES fetch Iago + its 69-file
closure onto the volume. **NEXT: boot-time injection of
`Booting.switches[l]=TRUE`** — patch the loaded boot file's
`StartListHeader.switches` (`BootStartList.Header` word 34, 64-bit
`[a,b,c,d]`; find the `l` bit) in VM after load, or extend the germ-request
seeding (`machine.c` ~245). Then drive Iago's `Create Physical/Logical
Volume` + `Format` (the first HEAVY disk-write workload). Full plan: memory
`othello-dead-end-iago-is-the-path` + the a93dbb research report.

**Input reliability: a host-side keyboard buffer.** Cedar samples the key
matrix once per display field, so a keystroke whose down+up fell in one
field was dropped (fast typing lost 4+/36 chars). `dorado_machine_set_key`
now enqueues into a file-scope FIFO drained one transition per 3 fields
(`machine.c`); lossless 36/36. Kept a static (not a struct member) so the
snapshot ABI is unchanged.

## 2026-07-18: THE MACHINE DISPLAYS ITSELF. Friendly desktop, 2x faster,
## clipboard paste, and the ethernet bug behind every big-transfer wedge.

**The Dorado draws its own schematic.** `ProcH-BitSlice07.ais` -- the 1979
processor-board sheet, converted from the PDFs in `DoradoDocs/schematics/`
-- paints in an open AIS Viewer on the Cedar 6.1 desktop
(`docs/images/cedar-ais-proch-bitslice07-2026-07-18.png`). The moon photo
`uscmoon.ais` came first and proved the pipeline.

**Root cause of the >100 KB transfer wedge (the real find).** Full
`DORADO_FTP_TRACE` forensics showed the client's acks freezing at one byte
ID and repeating forever, with an `FTP_QUEUE` that had no matching
`FTP_RX_CONSUMED`: a Pup queued while the guest receiver re-armed was lost
(the wire is a single-packet buffer, exactly like a real dropped frame),
and BSP clients discard everything after a gap. **Retransmission is the
SENDER's job in BSP** and our STP server had no retransmit ring. Fix
(`src/ethernet.c`, `PUP_TYPE_BSP_ACK`): on a *duplicate* acknowledgement,
rewind the transmit cursor to the acked byte -- during a retrieve the file
itself is the ring (rewind `ftp_file_pos`), and past EOF the fixed 20-byte
completion tail re-enters `FTP_TX_DONE` at the lost step. The 324 KB
fetch now completes after 3 rewinds. This same drop class was wedging
cold-boot BringOvers of the complete tree.

**A friendly front door.** Tim Diebert (AISViewer's author) pointed at
Xerox's own sample configurations, now mirrored in
`chm/cedar/cedar6.1-docs/`. They document the software-loading layer we
had been doing by hand: `Install <pkg>`, `.cm` command files (run by
typing their name; `Source` passes `$1`), `Alias`, `CreateButton`,
`.load` manifests, and the profile hooks (`BootCommands`, `NewUser`,
`PerLogin`, `PerCommandTool`). We adopted them:
`chm/cedar/stp-root/CedarChest6.1/DoradoWelcome/` serves eight `.cm`
files, and the profile fetches them, creates five CommandTool buttons,
and prints a menu. A visitor now types `Moon.cm` -- verified end to end
on the shipped checkpoint -- instead of a 100-character `Eval`.

**Three traps when authoring guest files**, all failing identically and
silently as "1 files acted upon" (the `.df` alone):
1. Cedar text files are **CR-terminated**; LF makes the file one line.
2. **`Bringover -p` is public-files-only** -- data files need plain
   `Bringover`. (This bit us twice: first on `DoradoWelcome.df`, then
   again in our own `Memo.cm`/`Chess.cm`, which is why their buttons
   errored until 2026-07-18 late.)
3. A **semicolon is a command separator** -- it cannot appear in `Echo`
   text.
Also corrected: quotes do NOT break a profile value (Xerox's own profiles
are full of `\"`); our earlier corruption was a formatting error.

**2x faster, by profiling instead of guessing.** `sample` on a live Cedar
run showed **~40% of all runtime in trace-enable checks**:
`dorado_trace_flag`'s memo is keyed by call-site string-literal pointer
and held 128 entries for ~250 sites, so overflowed sites paid a full scan
plus a real `getenv()` every call. Now an open-addressed 1024-slot hash.
Second find: `dorado_ethernet_wakeup_mask` called raw
`getenv("DORADO_ETH_WAKE_TRACE")` on every wakeup poll (~17%). Both
fixed, byte-identical output: **Alto 11.6 -> 20.4 M cycles/s, Cedar
~12 -> ~27 M** (1.2x / 1.6x the real 16.67 MIPS Dorado). Next spots if
needed: `dorado_visible_word_at_va` VA re-translation (~20%), the
interpreter (~15%).

**Clipboard paste** in SDL (`Cmd/Ctrl+V`) and the browser (native paste
event -> `dorado_web_paste`), plus headless `--paste-at CYCLES --paste
TEXT`, all through one paced queue in the new `src/typetext.c` -- which
also unifies the ASCII->Alto key map that `dorado.c` and `dorado_sdl.c`
had drifting copies of.

**Web:** the browser build now runs the in-process STP server against a
pruned tree preloaded at `/stp` (CedarChest6.1 + fonts; Cedar6.1 stays
out to keep `index.data` under GitHub's 100 MB file limit), so Bringover
and demand-fetch work there too. The page gained a "What can I do here?"
panel with a type-it-for-me button per command.

Open next: mount the kitchensink volumes as secondary drives (read-only
access to 2,240 recovered files) before attempting an installed-volume
disk boot; XC1-2-2/CedarPS fonts and printing; Diebert's book software.

## 2026-07-16: CedarChest applications install into the live desktop;
## AIS raster pipeline; the Dorado's own schematics as .ais files.

Software installation now works the way PARC did it, end to end: a
scripted desktop session BringOvers **AIS, AISViewer, ChessHack, and
CardTable** from `[Cedar]<CedarChest6.1>Top>` (34 files, every creation
date matched) and `Run AISImpl` loads and starts straight from the
remote release directory (`Ran: [Cedar]<CedarChest6.1>AIS>AISImpl.bcd!1`).
New tooling and content, all committed:

- `tools/fetch_cedarchest_app.py NAME.df!N` mirrors any CedarChest6.1
  package into `chm/cedar/stp-root/` (CardTable's payload survives only
  under CedarChest6.0 — same dates, shifted versions; the tool prints
  what's missing so you can fetch by hand).
- `tools/pbm2ais.py` converts PBM (from `pdftoppm -mono` over our
  schematic PDFs) into AIS rasters per AISFormat.mesa — header layout
  verified byte-for-byte against period `[Cyan]<AIS>` files (password
  -31574, 1024-word attribute page, uca part, scanDir=3).
  `stp-root/CedarChest6.1/AISImages/` now serves ProcH-BitSlice07 /
  ProcH-Title / IFU-Sheet02 (the machine's own schematics!) plus period
  images uscmoon, reducedparc, AlbumMusician.
- Display route (AISViewer is a library, not a command): CommandTool
  `Eval` escape —
  `Eval AISViewer.DisplayAIS[AISViewer.CreateAISViewer[], "[Cedar]<CedarChest6.1>AISImages>ProcH-BitSlice07.ais"]`.
  OPEN: `Run AISViewer` (the packaging CONFIG) hangs the CommandTool
  (>2.5B cycles, no "Ran:"); plain modules (`Run AISImpl`,
  `Run AISViewerImpl`) are the workaround under test — root-causing the
  config load is the next debugging target. See handoff.md.

Also today: kitchensink volumes re-tested with LV boot records + stamped
free labels on a copy — still blank at 2B cycles (0 px). The
installed-system-volume boot path (vs our login/work volumes' simple
path) remains the real blocker for those images; media fixes alone don't
move it. `--pilot-disk SLOT=PATH` mounts up to 4 PDIs (drives 0-3) but
nothing yet proves Cedar's Dorado disk head enumerates a second SA4000.

## ===> MILESTONE (2026-07-15): CEDAR 6.1 BOOTS TO ITS VIEWERS DESKTOP.
## Screenshot: docs/images/cedar-desktop-first-boot-2026-07-15.png

A full 30B-cycle cold boot from committed artifacts reaches the live
Cedar Viewers desktop: "Ready." + Cmd/Open/New/Idle menu, a CommandTool
viewer at a `%` prompt (WD ///Users/Guest.pa/), WatchTool and EditTool
desktop icons painted with demand-fetched Tioga fonts, and the User
Profile's boot commands executed (with graceful errors only for optional
content not in the served tree: BootTool.df, Clock).

**The fix that unlocked it** (c25240b, cpu.c): on a Subroutine Return,
`Link <- CIA+1` must NOT be reloaded when the same instruction loads
Link explicitly -- the co-routine contract DMesaFloat.mc's RoundLong
states verbatim ("Note Link_ overrides Return's normal action of
loading Link with .+1"). `RoundI` of 0.0 runs FixZero -> CoReturn ->
`FTemp2_ T, Link_ FTemp2, Return` -> FixRoundRet -> Return-to-caller;
with the unconditional reload, that last Return jumped into @JNE2's
microcode, whose stack pops underflowed StkP, and the uncaught
StackError (via StkError HOLD -> fault task ChkStkErr -> EmuFault ->
TrapsImpl -> no catcher -> DebugNub display blank) killed every boot at
InterpreterPackage START -- the "fonts crash" of 2026-07-13.

**Font demand-fetches work end to end**: attached names (BringOver
enter) -> ImagerFont.Find -> FS.EnumerateForNames -> FS.Open -> STP
Retrieve; the run serves Fonts/TiogaFonts/{Tioga10B,Helvetica10,
TimesRoman10,...}.ks live from chm/cedar/stp-root.

**Gates after the fix**: all unit tests pass (test_ethernet NetDir =
documented pre-existing), Galaxian @160M = 121,549 px (historical
band), diagnostics kernel/eventCounters/IfuSimple/memMisc/TriconD PASS
(IfuComplex fails identically pre/post fix -- the known discrepancy).

Repro: `make run-cedar-work` (SDL, log in as Guest) or the headless
recipe below. Next frontier candidates: serve the optional
BootTool/Clock/EditorComforts content; keyboard/mouse interaction with
the desktop; snapshot+web checkpoints of the desktop state.

**Herald boot buttons (2026-07-15): root-caused, media-fixed, and
RUNTIME-VERIFIED — clicking `CedarWork` reboots the volume.** With
BootTool served and run, left-clicking the herald's `Boot` button makes
BootTool's `MakeSimpleButtons` paint the volume buttons
(`Alto | Basic | CedarWork`); the buttons are NOT painted at START, so
the saved desktop checkpoint shows them only after that click
(BootTool.mesa, fetched to `chm/cedar/boottool-src/`). Clicking
`CedarWork` originally raised an uncaught `File.Error` from FileImpl.bcd
(Event `11601B`, `NoSymbols[FileImpl.bcd]`) into the debugger viewer.
Root cause, verified against the media: the work PDI's single logical
volume (`CedarWork`, 65,450 pages) had **all six LV `bootingInfo` slots
empty** — rusty-backup's installer only wrote the PHYSICAL volume root's
bootingInfo (all the cold-boot microcode/germ chain reads), but Pilot's
soft boot (BootTool buttons, `Booting.Boot`, RollBack) resolves the
LOGICAL root's `bootingInfo` (37B) + `rootFile` (125B) instead — a real
Othello install writes both via `File.SetRoot`/`RecordRootFile`
(FileImpl.mesa). Fixed both ways: `pilot::install_boot_file` now writes
the LV records on fresh builds, and `tools/pdi_install_lv_bootfiles.py`
retrofits existing images (committed `CedarDorado-work.pdi.gz` carries
germ@104 + bootFile@139 in both roots; 997dbb2). **Runtime verification
(2026-07-15, headless, from the desktop checkpoint):** restore the
snapshot, `--type-at 21050000000 --click 830,8` (herald `Boot`) then
`--type-at 21300000000 --click 780,8` (`CedarWork`) — 700M cycles later
the screen shows the fresh `Cedar 6.1.0 ... CedarWork on Dorado` banner,
"This is the Basic boot file.", and by +3.2B cycles all 34 Basic.Loadees
have loaded and STARTed through the Guest-profile prompts. So
Booting.Boot → `Boot.Request{inLoad, DiskFileID}` at germ-MDS `1360B` →
germ re-entry after 21B cycles in VM → polled-disk inload all work.
Scripted-desktop-interaction notes: keyboard input needs the input
focus first (click into the CommandTool viewer, e.g. `--click 700,467`);
runs of repeated characters (`///`) drop keys at the default cadence —
use `--key-hold 1600000`. `RollBack` stays struck out until Cedar's
`Checkpoint` command is run (writes `bootingInfo[checkpoint]`; would be
the first heavy disk-write workout). Cedar user-level semantics
(BootTool, boot switches, checkpoint/rollback):
`DoradoDocs/manuals/Introduction_to_Cedar_7.0.md` (added 2026-07-15;
written for 7.0 but matches our 6.1 world).

**Herald soft-reboot wedge (2026-07-15 evening): root-caused and FIXED —
the PDI germ-stream cursor survived into the second boot.** Clicking
`CedarWork` in SDL or the browser wedged the world (cursor stops
XOR-ing and streaks; Mesa spins at a jump-to-self, IFU hot PCX
`005263` with br31 in germ MDS; disk counts show `ctrl=36 xfer=0`).
Root cause: the polled germ-boot IOCB bridge streams GERMDATA pages
sequentially through `pilot_pdi_stream_active`/`pilot_pdi_next_page`
(the germ's mid-stream requested addresses are stale flat links, so the
cursor must override them) — but the latch persisted for the process
lifetime AND through snapshot restore, so a SECOND germ boot (herald
button -> Booting.Boot -> germ re-entry) continued the old stream at
the wrong page (germ asks for the LV bootFile page 0o213, got 0o2257)
and the germ halt-spun. The CLI accidentally masked it: dorado.c
re-applies --pilot-disk after --snapshot-in, which resets the cursor —
which is why every scripted CLI/node replay booted while every
interactive SDL/web click wedged. Fix (machine.c): any non-GERMDATA
polled command (the 0o260 label read the germ issues to follow a
boot-file link) resets the stream, so the next GERMDATA re-seeds from
its own (correct) first address. Verified: the previously-wedging
condition (stale restored cursor, no CLI re-apply) now boots to the
Basic banner + loadees; cold boot to login, unit tests, and Galaxian
(121,549 px) unchanged. Debug leverage added along the way:
`dorado_web_debug()` (exported; run
`Module.ccall('dorado_web_debug')` in the browser console) dumps the
machine X-ray (hot PCs, IFU dispatches, disk/PDA state) — build with
DORADO_MACHINE_PCHIST=1 for the histograms.
Two related notes: (a) the browser main loop is rAF-driven, so a
hidden/backgrounded tab suspends the emulator — looks like a hang,
isn't one; (b) the herald `Basic` button posts "BasicCedarDorado.boot
not found" by design — BootTool's BasicCedar does a purely local
FS.Open and the volume has no file by that name (the CHM mirror has
BasicCedarDorado.boot!22 if we ever want to plant/attach it).

**WASM stack overflow (2026-07-15): found and fixed — the web builds
were silently corrupting static data on every display-list render.**
`dorado_machine_render_display_list` kept a framebuffer-sized scratch
copy (`ddc_fb`, 101 KB) on the stack; Emscripten's default wasm stack is
64 KB and overflow tramples the static-data segment with no guard page.
Observed symptom: `getenv()` went NULL mid-run in the node build, so
`DORADO_PDI_SAVE=1` was silently ignored and `make
cedar-desktop-web-snapshot` paired the desktop snapshot with the
PRISTINE work PDI (582 KB gz vs the real 1.6 MB gz) — a checkpoint that
would corrupt on first disk access. The browser build calls the same
renderer every frame and had been overflowing all along. Fix: `ddc_fb`
is now static (machine.c) and both wasm links get `-sSTACK_SIZE=1048576`
(Makefile). Native builds were never affected (8 MB stacks); Galaxian
gate re-verified at 121,549 px @160M.

## 2026-07-14 (late): the FULL INSTALL now runs from committed artifacts —
## 96 STP transfers including all three fonts DFs. The Imager crash remains,
## still with ZERO font-file demand-fetches; that is the frontier.

On top of the morning's fixes (next section), three more landed tonight:

1. **Free-page labels lacked the volume ID** (`a687874`). Cedar's allocator
   verifies candidates against `FileInternal.FreeLabel[volume]`, whose
   fileID is the logical volume's VolumeID (FilePagesImpl.mesa); rusty
   wrote fileID=0, so every VerifyLabels failed (33K label-check mismatches
   in one boot) and FS said "No more free pages on a local volume" with
   64,309 pages free. `tools/pdi_stamp_free_labels.py` stamps them; the
   stamped work volume is tracked as `CedarDisk/CedarDorado-work.pdi.gz`
   and `make run-cedar-work` boots it (638e5f0).

2. **Live BSP connections were being evicted** (`02a1203`). Six slots +
   an allocator that never preferred closed slots meant the install's
   connection churn could strand a live conversation (observed: a
   Grapevine enquiry retransmitting into FTP_UNSERVED forever, install
   hung at transfer #50). Now 16 slots, closed slots recycled first.

3. With those, a 30B cold boot completes the ENTIRE install: Guest login,
   34 loadees, the BootEssentials closure, and the three fonts DFs
   (TiogaFonts/PressFonts/FontMetrics transferred; XC1-2-2 correctly
   STP_MISSING), then the optional VersionMap lookups — 96 transfers.

**The remaining frontier, now precisely characterized** (the crash is at
cycle 12,939,649,700 with `DORADO_FAKE_TIME=1783285880`; bare `0` written
to the CSB head at `0420`, debugger-entry IWDC at 12,939,641,547):

- **The attaches are PERFECT.** The pre-crash terminal transcript shows
  every `local <-- remote` attach line and a clean `End BringOver of
  [Fonts]<CedarFonts>Top>FontMetrics.df`; the client B-tree on the saved
  PDI contains exactly the names the Imager wants, e.g.
  `Fonts>Xerox>TiogaFonts>Tioga10.ks` attached to
  `[Fonts]<Fonts>TiogaFonts>Tioga10.ks!3`.
- **The crash site is a loadee START after the installer** -- the
  transcript ends at `Starting ... InterpreterPackage.bcd ...` (it paints
  a viewer, needing the default font). The raise chain replays 2026-07-13
  exactly: raiser br31=0x120808 (ImagerPackage; ImagerTypefaceImpl's
  Create fall-through per the fetched source), 2454 SignalsImpl
  catcher-hunt dispatches, DebugNub CoreSwap, SetMP[cantWorldSwap],
  TerminalHead turnOff. Module IDs confirmed by VMDUMP-vs-BCD matching:
  126B08/128010 = ImagerPackage, F5300 = BasicPackages, 3A720/3B204 =
  boot-resident FS/BTree code.
- **The failing step is `FS.EnumerateForNames` returning nothing** with
  zero network traffic in the final 140M cycles (the demand-fetch never
  starts). The enumeration mechanism (FSMainImpl2.InnerEnumerate +
  FSDirImpl.EnumerateEntries over the client B-tree) STOPS at the first
  key that `Match` classifies as `clash`, so one out-of-scan-order key
  between the start position (`Fonts>Xerox>TiogaFonts>Tioga10.`) and the
  target makes the wildcard find nothing while exact-name lookups (tree
  descent) still work -- and NOTHING else in the whole boot uses wildcard
  enumeration, so a broken scan order would be invisible until now.
- Verified NOT the cause: rusty's initial B-tree free-list conventions
  (chain through minPage, freePageMarker in freeWords) match
  BTreeWrite.mesa's allocator exactly, and AllocatePage would raise
  Bug[pageNotFree] loudly on a bad chain.
- Offline tree-walk is blocked by design: the on-disk TreeState says
  rootPage=29/greatestPage=33/depth=3 while the client file holds only 24
  data pages (6 tree pages) -- the live tree is mostly in FS's VM cache
  (laundry lag), so only in-guest observation can see what the
  enumeration sees.

**LATE-NIGHT REFRAME — the crash may not be about fonts at all.** A
full-VA `DORADO_LOAD_TRACE_VA` sweep over the final 1.5M cycles before
the raise (125K fetches) contains ZERO font-name text: no
`FetchTypeface` SymTab hash, no `Rope.Cat` pattern text, no B-tree key
characters. `ImagerFont.Find` cannot execute without reading the name's
characters, so **no font lookup precedes the raise**. The 2026-07-13
"it is the missing fonts" conclusion rested only on the raiser being
ImagerPackage.bcd; the actual failing operation is something
InterpreterPackage's START asks of ImagerPackage that fails with little
compute (raise stint: ~90 dispatches at pcf 0o1004..0o1315, br31
0x120808, same code region as 2026-07-13). Suspects: an ImagerPackage
init/binding failure, or (per project history — see the
germ-blockers-tend-to-be-emulator-offset-bugs memory) a microengine bug
in freshly loaded code.

**THE ERROR IS NAMED: uncaught `StackError`** (RuntimeError.StackError,
raised by TrapsImpl's StackErrorTrap -- the µcode-invoked Mesa
stack-consistency trap). Decode, fully verified:

- At DebugNub.Catcher entry (`UCSProc [msg: WORD, signal: SIGNAL, frame]`,
  IFUDISP cpu-cyc 3496221834) the stack is [msg=0o177777, signal=0o1721,
  frame=0o3664].
- Signal value encoding `[gfi | sei<<1 | 1]`: gfi = 0o1700>>6 = 15, sei=8.
- `BasicCedarDorado.loadmap!69` bracket numbers are gfi<<6; counting:
  HeadsDorado = gfi 1..10, FrameImpl 11, InstructionsImpl 12, ProcessImpl
  13, SignalsImpl 14, **TrapsImpl [1700] = gfi 15**.
- TrapsImpl.mesa (`chm/cedar/cedar6.1/mesaruntime/`, fetched) `= CODE`
  declaration order: BoundsFault 0, ControlFault 1, DivideCheck 2,
  HardwareError 3, LinkageFault 4, PointerFault 5, PortFault 6,
  StartFault 7, **StackError 8**, UnboundProcedure 9, ...
- msg=0o177777 fits (StackError has no parameters); the two 80-dispatch
  br31=34100 stints before the SignalsImpl hunt are StackErrorTrap.

**The victim instruction** (last dispatch before the trap): opcode
`0o364` (ESC/MISC family) `alpha=0o34` at pcf=0o1010, br31=0x120808
(inside ImagerPackage, code at BCD file offset ~0x661x), executed during
InterpreterPackage's START. The 2026-07-13 "missing fonts" theory is
fully retired: this is the microengine mis-maintaining the Mesa
evaluation stack (StkP) around that instruction -- the same emulator-bug
family as the Md-bypass and IFU-operand sagas (see the
germ-blockers-tend-to-be-emulator-offset-bugs memory).

**The victim opcodes are the FLOATING-POINT escapes, and the failure is
in the punt/state-save path.** Findings from the µtrace (all real
addresses from Cedar.mb!6 symbols via mbdis; µtrace now prints
tsk=/stkp=):

- MISC alphas 0o20..0o35 are the FP family in this build (MiscTable0
  slot = 4*alpha+1 at real 0o4000+): FADD 0o4101, FSUB 0o4105, FMUL
  0o4111, FDIV 0o4115, **FCOMP 0o4121 (=alpha 0o24)**, FIX 0o4125,
  FLOAT 0o4131, FIXI 0o4135, FIXC 0o4141, FSTICKY 0o4145, ROUND 0o4155,
  **ROUNDI 0o4161 (=alpha 0o34)**, ROUNDC 0o4165. ImagerPackage init
  does Real arithmetic; the crash sequence is FCOMP ... then ROUNDI.
- Both ops immediately branch into a common punt path (the build runs
  with FP-microcode-absent semantics; FLOATINGPOINTPRESENT at 0o4400):
  0o0300/0o0400-region code builds an opcode-trap state for the
  software FP (RealImpl).
- **The kill shot**: the punt/resume validation reads a saved
  [bank,,stkp] word of 0o103 (= STK bank 1, index 3), tests it with
  AND 0o300 at µPC 0o1355 (task 0; the fault task 0o17 runs the same
  test at 0o1627), branches 0o1356 -> 0o1366 -> long jump to
  **0o2035 = STACKERROR**. A valid Mesa saved stkp must be < 0o100;
  the spurious 0o100 (bank-1) bit is the corruption. The word is
  computed at µPC 0o1530 as A-B(+2): 0o346 - 0o245 -> 0o103.
- The µtrace also shows the same FCOMP state (stkp=004, T=000120)
  re-entering the 0o410 MISC entry repeatedly WITHOUT new IFUDISP
  dispatches -- a punt-retry loop, consistent with the punt-resume
  never validating.
- Hardware doubt to resolve FIRST: our per-instruction StkP (now in the
  µtrace) stays in bank 0 (values 1..5) through these windows, so
  either the save-time StkP transiently carries 0o100 (StackSelect /
  task-switch exposure our model mishandles), or the save µcode's
  subtraction operands (0o346/0o245) are themselves off -- compare
  against PilotMesaProcess.mc's state-save/stack-save code
  (chm/cedar/refs/PilotMesaProcess.mc!1, local) and HM Table 6
  (StkP[0:1] = stack region, StkP[2:7] = offset).

**Repro one-liners** (from dorado/, ~30 s each): µtrace the punt window
with `DORADO_UCODE_TRACE=1 DORADO_TRACE_GATE=12939585000,12939615000`
from the snapshot pair; the ROUNDI entry is at trace PC=4161, the
StackError raise at PC=2035. IFUDISP gate 12939580000,12939645000 shows
the two MISC dispatches (alpha 0o24 at cpu-cyc 3496208505, alpha 0o34 at
3496208548). Clock mapping: machine ~= 12939500000 + (cpu_cyc -
3496183757) * 3.7047; IFUDISP lines print `cyc=` (cpu clock).

**Diagnostics staged (in /private/tmp, regenerate if lost — recipes here):**
a pre-crash framebuffer at 12.90B (`cedar-precrash.pgm`), and a
fast-iteration snapshot+PDI pair at 12.8B (`cedar-prefonts.{snap,pdi}`,
built with `DORADO_PDI_SAVE=1` on a copy; restore with `--snapshot-in ...
--pilot-disk <copy of cedar-prefonts.pdi> --ftp-root ../chm/cedar/stp-root`
and NO `--type`; the window replays cycle-exact, ~30 s per iteration).
The crash window for gated traces is
`DORADO_TRACE_GATE=12939500000,12939645000`.

Gateway-info noise: the guest sends GatewayInfoRequests (type 0o200)
throughout the install; they print as FTP_UNSERVED but ARE answered by the
handler further down the dispatch — singletons are benign; only a
repeating identical packet indicates a stranded connection.

## 2026-07-14: cold-boot regression fixed (germ polled disk path); fonts now
## served end to end. Full detail: docs/handoff.md top section.

Two things happened this session:

1. **The committed PDIs cold-boot again.** Commit 2e8018b had hard-coded CHS
   decoding of every IOCB DiskAddress in the PDI bridge, but the committed
   images (`CedarDisk/CedarDorado-boot.pdi`, `CedarDorado-work.pdi`) store
   flat page numbers in their boot-chain links, so the germ's first
   boot-file read decoded link 119 as CHS page 3332 and hung forever. That
   is why the 2026-07-13 session needed the ad-hoc `chs-contig10` image
   (lost in a `/private/tmp` wipe, recipe never committed). The fix
   (`machine.c machine_germ_complete_disk_iocb`): polled IOCBs — CSB
   interrupt mask 0, and only the germ polls — get the pre-2e8018b bridge
   semantics verbatim (flat addressing, raw label copyback, GERMDATA
   sequential streaming, unconditional success); Pilot's interrupt-driven
   traffic keeps the per-action CHS-faithful path. Both committed PDIs now
   reach the SimpleTerminal login at ~646M cycles, byte-consistent with a
   pre-regression reference build.

2. **The fonts are now served.** `[Indigo]<Fonts>Top>FontMetrics.df!2` and
   `PressFonts.df!1` fetched into `stp-root/CedarFonts/Top/`
   (XC1-2-2-Fonts.df does not survive anywhere in the archive); the font
   files now live at their full export paths (`Fonts/TiogaFonts/*.ks`,
   `Fonts/FontMetrics/*.tfm`, `PressFonts/*.sd`) matching the STP
   resolver's Directory/Name-Body mapping — the old flat `Fonts/` layout
   could never have served a demand-fetch; and the resolver strips IFS
   `!<version>` suffixes (a demand-fetch names the DF-pinned version).
   `tools/fetch_cedar_fonts.py` handles all three DFs, full export paths,
   and the archive's case-sensitive URLs.

   **Why zero font files were requested (2026-07-13 mystery, resolved):**
   `BringOver[action: enter]` with autoConfirm does `FS.Copy[remoteCheck:
   FALSE, attach: TRUE]` — a pure local attach, no network. The crash is
   `VFontsImpl.CreateDefaultFont` (ViewersPackage START):
   `ImagerFont.Find["Xerox/TiogaFonts/Tioga10"]` fails, and its fallback
   `EstablishFont["Tioga", 10]` has **no catch**, so the second
   `Imager.Error[$fontNotFound]` propagates uncaught (raised by
   ImagerTypefaceImpl inside ImagerPackage.bcd — matching the 2026-07-13
   VM-dump identification). The find is `FS.EnumerateForNames` over local
   attached names, then `FS.Open` demand-fetches `[Fonts]<Fonts>TiogaFonts>
   Tioga10.ks!1` via STP. `Tioga10.ks` is now present in the served tree.

**Media lives in the repo now, not /tmp.** The work volume is tracked as
`CedarDisk/CedarDorado-work.pdi.gz` (free-page labels already stamped with
the volume ID -- see `tools/pdi_stamp_free_labels.py` and the 2026-07-14
handoff section); `make run-cedar-work` rehydrates it and boots the full
install path interactively (log in as `Guest` at the prompt). Headless
repro, from `dorado/`:

```sh
DORADO_FAKE_TIME=1783285880 DORADO_PDI_IGNORE_LABEL_FLAGS=1 \
DORADO_FTP_TRACE=1 ./build/dorado \
  --boot-reason disk --no-alto-boot \
  --eb "../chm/dorado/CedarDorado.eb!6" \
  --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
  --pilot-disk ../CedarDisk/CedarDorado-work.pdi \
  --ftp-root ../chm/cedar/stp-root \
  --type-at 760000000 --type 'Guest\n\n' \
  --cycles 30000000000 --out /private/tmp/cedar.pgm 2>/private/tmp/cedar.log
```

Runs do not write the PDI back unless `DORADO_PDI_SAVE=1`, so running the
repo copy in place is safe; only scratch logs/screens belong in /tmp.

## 2026-07-13: Cedar loads + installs everything; FONTS are the last blocker.
## Full detail: docs/handoff.md top section.

Cedar 6.1 now boots as Guest, transfers **and STARTs** all 34 `Basic.Loadees`
packages, runs `InstallerImpl`, and BringOvers the entire `BootEssentials.df`
closure (93 STP transfers: TIP tables, `.icons`, JaM programs, styles,
`User.Profile`). It then dies on an **uncaught signal from
`ImagerPackage.bcd` — no font ever reaches the guest** — and Cedar's crash
path (DebugNub -> `SetMP[cantWorldSwap]` -> DeviceCleanup `turnOff`) blanks
the display, which is why the screen goes white. **The blank screen is a
symptom, not a missing display turn-on; do not chase the display.**

Get fonts into the guest and the Viewers desktop should follow. Nothing else
is known to be broken.

Four emulator bugs were fixed to get here: host stores bypassing the Map (the
one that corrupted the germ's credentials and caused the old WDC=0333B
death), a single-connection STP server (Cedar runs several concurrent BSP
connections), synthetic creation dates (BringOver skips any file whose date
does not match the DF, so the Installer was installing nothing), and the same
Map bug in `dorado_poke_va()`.

**Two traps that cost hours:** pin `DORADO_FAKE_TIME=1783285880` on every run
(the guest reads host wall-clock time, so cycle numbers drift between
identical runs and gated traces silently miss), and the release DFs are
**CR-delimited** (`fgets()` swallows one as a single line).

**Snapshots are stale** — `dorado_ethernet` grew, so the committed Cedar
login / Lisp desktop / wasm web snapshot assets must be regenerated.

## 2026-07-12 (superseded): the WDC=0333B loadee death — root cause turned out
## to be the Map bug above; the "wild monitor-lock pointer" was its symptom

## 2026-07-12: Cedar graphical-OS path — earlier handoff (STP stage;
## superseded by the sections above where they conflict)

**Objective.** Continue past the graphical SimpleTerminal login into Cedar's
Viewer-based graphical environment.  The emulator, disk-germ boot, display,
keyboard, and Guest login all work.  The present stop is the Cedar remote-file
cache confirmation that precedes LoaderDriver's `Basic.Loadees` transfer.

**Reproducible work volume.** Rusty Backup now builds a fresh 65K-page,
matched-germ Cedar volume with:

```sh
tools/rusty-backup/scripts/build-cedar-work-volume.sh \
  CedarDisk/CedarDorado-work.pdi CedarWork
```

It installs `Dorado.germ-6.1.6`, `BasicCedarDorado.boot!22`, and a writable
Cedar `client` B-tree; `pilot_probe verify` and `dorado/build/pdidump` pass.
A cold 720M-cycle boot reaches the graphical login.  Guest login must be
typed at approximately 760M cycles:

```sh
cd dorado
./build/dorado --boot-reason disk --no-alto-boot \
  --eb ../chm/dorado/CedarDorado.eb!6 \
  --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
  --pilot-disk ../CedarDisk/CedarDorado-work.pdi \
  --ftp-root ../chm/cedar/stp-root \
  --type-at 760000000 --type 'Guest\n\n' --cycles 1800000000 \
  --out /tmp/cedar-current.pgm
```

**Release content is present.** `chm/cedar/stp-root/Cedar6.1/` now contains
`Basic.Loadees` plus all 33 Cedar 6.1 BCDs it names.  Recreate that tree with
`python3 tools/fetch_cedar_loadees.py`; it reads the local CHM cross-reference
and downloads the archive revisions, including `ViewersPackage.bcd` and
`End.bcd`.

**Exact observed STP boundary.** With `DORADO_FTP_TRACE=1`, Cedar:

1. authenticates through the Grapevine-down fallback;
2. opens the STP socket and sends `iAmVersion` then `Retrieve` for
   `[User]<Guest>6.1>Basic.Loadees`;
3. receives `hereIsPList` with the source-faithful requested properties;
4. acknowledges the BSP EOC, but never sends STP `yes`.

The original `STPServerMainImpl.Mesa!1` confirms that the server must wait for
that `yes` before it sends `hereIsFile`; `FSRemoteFileImpl.mesa!2` confirms
that Cedar sends `yes` only after its local confirmation callback creates a
cache stream.  Therefore the active gate is the guest's local cache/name-file
creation, not missing BCDs or an unavailable STP server.

**Important tested facts.**

- The STP plist now follows Xerox `SendPropList`: only requested properties,
  original `Directory`/`Name-Body` decomposition, `Version 1`, and
  `Creation-Date ... GST`.
- Xerox `PupBSPStreams.bcpl` distinguishes ordinary `Mark` from acknowledged
  `AMark`; both EOC variants were tested.  The acknowledged EOC is the only
  variant that lets Cedar complete receipt of the plist, but it still does not
  reach `yes`.
- A forced server-side `hereIsFile` stream without the client `yes` is ignored
  by Cedar, as the original protocol predicts.
- A no-client-root control PDI reaches Guest login but does **not** enter
  LoaderDriver at all.  The client B-tree is required; deleting it is not a
  solution.
- The authoritative Cedar 6.1 B-tree sources are now local:
  `chm/cedar/cedar6.1/btree/{BTreeInternal,BTreeWrite}.mesa!1`.  They confirm
  the free-page marker and `firstFreePage` chain fields in the Rusty writer,
  but the complete guest-writable client/cache volume semantics still need
  comparison against a period installed volume.

**Fast debug snapshot.** A Guest/Loader checkpoint can be made at 1.35B
cycles and restored with the PDI and FTP root supplied again; details are in
`tools/rusty-backup/docs/dorado_cedar_work_volume.md`.  Do not use the old
ABI-incompatible Cedar snapshots for this work.

## 2026-07-10: clean recovery and web snapshot audit

- `make clean` removes the entire `dorado/build` tree, including experimental
  `build/good-packs`. The supported Lyric base pack now has a real Make
  dependency that rehydrates it from `web-assets/lisp-lyric-xcl.pack.gz`.
  Older Current/Harmony/Medley packs are not supported dependencies;
  representative descendants remain under `/private/tmp`.
- Cedar's blank regression is fixed and its verified login is now checkpointed
  for native SDL and wasm32. `make cedar-login-snapshot` captures the native
  650M-cycle state; `make run-cedar-snapshot-sdl` restores it immediately and
  accepts keyboard input. `make cedar-login-web-snapshot` creates the separate
  wasm32 ABI asset used by the browser's Cedar option. Mesa NetExec remains the
  next useful web checkpoint after its capture frame is revalidated.
- Web world switches now clear Lyric's `DORADO_DISPM_PRESENT` setting for
  Alto/Mesa/Cedar. `display_dispm_present` no longer caches that setting across
  machines, since the browser changes worlds without reloading the module.

## 2026-07-11: Cedar LoaderDriver / STP bring-up — in progress

**Objective.** Make the regenerated Cedar best-of PDI reach the real Cedar
desktop, not merely the verified SimpleTerminal login.  The PDI itself is a
valid Pilot volume, but Cedar's `LoaderDriver` obtains its desktop BCD set over
STP; it does not load that release tree from the Pilot disk.

**Source material is in the tree, never `/tmp`.** The original client/server
sources are cached under `chm/cedar/stp/{client-6.1,client-6.0,server-6.1,server-6.0}/`.
Related LoaderDriver, FS remote-file, Grapevine, and Pup sources are under
`chm/cedar/cedar6.1/{loaderdriver,fs,grapevineuser,pup,usercredentials}/`.
The initial shared release command file is
`chm/cedar/stp-root/Cedar6.1/Top/Basic.Loadees`.

**Confirmed Cedar path.** Starting from
`build/good-packs/cedar-bestof-login.snap`, typing `Guest` plus two Returns
does all of the following:

- calls the Grapevine registry (`GrapevineRServer`, Pup WKS 050/052);
- receives the source-defined `AllDown` result and visibly prints
  `Grapevine down, proceeding anyway`;
- enters LoaderDriver and prints `This is the Basic boot file`;
- opens the real STP endpoint on Pup socket 3 and issues an STP `iAmVersion`
  followed by `Retrieve`.

The first request is the expected user override:

```
((User-Name Guest.pa)(Directory Guest)(Name-Body 6.1>Basic.Loadees)...)
```

The read-only server recognizes Cedar's decomposed `Directory`/`Name-Body`
form and maps the standalone archive's initial lookup to
`Cedar6.1/Top/Basic.Loadees`.  `--ftp-root ../chm/cedar/stp-root` is reapplied
after `--snapshot-in`, since snapshots intentionally restore the Ethernet
controller state too.

**Current implementation.** `dorado/src/ethernet.c` has a deliberately small
in-process STP/Grapevine shim behind `--ftp-root`:

- NetDir replies advertise a local RServer and the STP endpoint.
- `echoMe`/`iAmEcho` and a minimal RServer reply permit Cedar's documented
  Grapevine-down fallback; this is not an authentication implementation.
- socket 3 accepts RTP/BSP and handles `iAmVersion`, `Retrieve`, plist,
  `Yes`, file chunks, and `No fileNotFound` replies from a safe rooted tree.
- `dorado_machine_set_ftp_source` reapplies explicit host-side sources after
  snapshot restore without changing the snapshot ABI.

**Exact blocker.** Cedar accepts the RTP/BSP transport and sends its Retrieve
command, but sends RTP Abort text `Unwinding...` immediately after the server
returns `HereIsPList` plus EOC.  This reproduces even when the plist is the
syntactically valid empty `()`, so it is not a filename, BCD, or plist-property
spelling issue.  The server's normal plist currently mirrors
`STPServerMainImpl.SendPropList` property names:
`Directory`, `Name-Body`, `Version`, `Creation-Date`, and `Size`.

**2026-07-11 late — guest-visible root cause.** A native SDL capture of the
same login snapshot shows that STP has reached Cedar: after `Guest` signs in,
LoaderDriver prints `This is the Basic boot file`, then both its user and
shared `Basic.Loadees` lookups fail with `FS.Error: No more free pages on a
local volume`.  The abort is the remote-file cache's unwind, not rejection of
the plist or BSP stream.  `CedarDorado-bestof.pdi` is full enough to trigger
this; the separately authored kitchen-sink disk 3 has ~32,996 free pages but
does not by itself reproduce the working login boot.  The next media task is
therefore to derive a bootable PDI which preserves the working installed boot
state while retaining enough free Pilot pages for the STP cache.  The
standalone `CedarDorado-boot.pdi` has room but its saved login checkpoint is
currently ABI-incompatible; regenerating that 650M-cycle snapshot needs to be
made reliable before it can be used as the writable base.

**Important protocol result.** Do **not** group a BSP mark and the following
data in one Pup.  `PupBSPStreams.bcpl:BSPPutMark` calls `BSPForceOutput` after
writing the mark, so the real exchange is a standalone `typeMark`, then a
`typeData`, then the EOC `typeAMark`.  A trial that grouped Version/PList
payload into the mark caused Cedar to fail earlier, before Retrieve; it was
reverted.  The remaining work is a packet-level comparison of the post-plist
Data/EOC/ACK state against a real PupStream server, especially BSP allocation
and acknowledgement timing.

**Useful repro.** Run from `dorado/`:

```sh
DORADO_FTP_TRACE=1 ./build/dorado \
  --boot-reason disk --no-alto-boot --eb worlds/cedar.eb \
  --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
  --pilot-disk ../CedarDisk/CedarDorado-bestof.pdi \
  --ftp-root ../chm/cedar/stp-root \
  --snapshot-in build/good-packs/cedar-bestof-login.snap \
  --type-at 1201000000 --type 'Guest\n\n' --cycles 1500000000 \
  --out /tmp/cedar-stp.pgm
```

`make test` passed after this work; `make build/dorado build/dorado-sdl` is
also clean.  Do not bulk-download the 33 BCDs listed by `Basic.Loadees` until
the plist-to-file transition works: the server has not yet received Cedar's
`Yes` or transferred the command file.

## ===> MILESTONE (2026-07-04): INTERLISP-D BOOTS TO ITS DESKTOP.
## The Md-bypass fix (c9aa818 + 9bd7f76 + 29c7ad8) brings Lyric all the way
## up: gray desktop + "Prompt Window" + "Exec (XCL)" window (191,993 px).

## ===> MILESTONE (2026-07-10): LYRIC REACHES ITS DESKTOP WITHOUT STARTUP RAID.
## `tools/interlisp-sysout/discard_stale_process.py` discards the one
## unavailable saved RTP process context; an 8.5B-cycle boot reaches a clean
## Prompt Window + Exec desktop (206,668 display-list pixels).

**Fast restore + input finding (2026-07-10):** SDL now accepts machine
`--snapshot-in/--snapshot-out`. `make lisp-lyric-desktop-snapshot` captured a
clean 8.8B-cycle XCL desktop and its matched writable pack;
`make run-lisp-snapshot-sdl` restores it in about 0.3 seconds, with a
byte-identical framebuffer after forward execution. The web build now ships a
separate wasm32-native checkpoint (raw snapshots are ABI-specific) plus its
pack as 5.6 MB of gzip assets and offers **Interlisp-D Lyric — saved Exec
(XCL) desktop** in the boot menu.

The input investigation now reaches beyond SDL/web and the keyboard matrix.
A held `a` is visible at LLKEY's Dorado low-core address as active-low
`175777`; the display-field reschedule enters the keyboard context; and a
baseline/key snapshot diff finds exactly one new ASCII `0141` byte in Lisp VM,
proving `\\DECODETRANSITION` and `\\PUTSYSBUF` ran. Mouse motion and the
active-low left-button transition reach LLKEY too. Nevertheless, clicking the
Exec window and typing leaves the framebuffer byte-identical, and the blank
Prompt/Exec windows never print their initial prompt. A live atom scan also
finds non-NIL `\\TTY.PROCESS`, `\\RUNNING.PROCESS`, and `\\PROCESSES`.
The active blocker is therefore the saved guest's process/TTY reader, which
does not drain SYSBUFFER, not host focus or SDL key mapping.

**Process root cause (2026-07-10):** `\RUNNING.PROCESS={75,6000}` is
`ERIS#LEAF`, while `\TTY.PROCESS={75,6300}` is Exec. Leaf loops in saved
cleanup with `PROCOWNEDLOCKS={75,1730}` and never yields, so XCL cannot drain
SYSBUFFER. A cycle trace shows opcode `CREATECELL` legitimately reusing that
monitor cell during startup because the saved owned-lock pointer is not a
live GC root; this is not a torn memory store or SDL focus problem. Restoring
the cell long enough for Leaf to continue exposes a second archived
inconsistency: RAID reports `DELREF on PTR with 0 refcnt` for `{57,73730}`,
the saved `\PROCESSES` list root. Skipping Leaf, clearing process roots for a
cold `PROCESSWORLD`, and forcing a late reschedule were tested and rejected
(RAID or Swat); none is wired into Make targets or snapshots. The next safe
route is a sysout saved with processes off, or a guest-aware repair that also
updates Interlisp reference counts. Do not patch queue links or top values
directly.

`DORADO_LISP_FIND_ATOM=NAME[,NAME...]` is a read-only live-sysout probe added
for this work. The earlier Dandelion IOPage cache helper remains for the
pre-IFU AEmu path and has an RP0-alias regression test, but post-load Dorado
Lisp now refreshes only the absolute low-core words selected by LLKEY. This
also avoids repeatedly clearing unrelated dirty state in the IOPage cache
line. Do not describe the saved XCL checkpoint as keyboard-usable until a
typed form is visibly consumed and evaluated.

**Added Cedar media (2026-07-10):** the three images now under `CedarDisk/`
`CedarDorado-kitchensink*.pdi` images are structurally valid PDI v1 / Pilot
physical volumes. `pdidump` verifies PRSeal `0121212`, version 6, the
`CedarKitchenSink` label, root checksum `0x3F79`, one 65,450-page subvolume,
and coherent page labels. They contain the same 33-page germ and 1,060-page
boot file but distinct populated data; disk 3 has about 32,996 free pages,
versus 299 and 42 on disks 1 and 2. Extracted FileID 3 has the identical
SHA-256 `c1778650...b0e435` on the small boot PDI and all three kitchen-sink
volumes, so adding boot files or mounting a second disk is not the missing
step. Each kitchen-sink image independently maps logical pages 0..65449 rather
than forming three fragments of one logical volume. All three enter the
installed-system-volume path but remain blank at 700M; disk 1 is still blank at
1.2B after completing many post-germ IOCB reads. Catalog/software testing now
depends on finishing that installed-volume disk bridge.

**Cedar regression root and checkpoints (2026-07-11):** a historical build at
`0993f81` booted the current PDI (15,535 pixels), and a deterministic revision
bisect found the first regression at `4b54d4d`, where the incomplete fixed
3/16/24-cycle Md-ready Hold approximation became unconditional. It is opt-in
again with `DORADO_HOLD`; Hold signal/event-counter accounting and HOLDSIM stay
active. A second regression was the Interlisp LC=5 same-instruction Md bypass:
the Hardware Manual's real bypass is previous-instruction pipeline behavior,
while the collapsed-pipeline compatibility substitution needed by DoradoLispMc
corrupts planted-germ Pilot state. `compat_same_instr_md_bypass` therefore
remains enabled for the validated Lisp path and is disabled for `germ_path`
worlds until the overlapping processor pipeline is modeled. Cedar again
renders 28,539 pixels at 700M and visibly echoes `abc`; restoring the checkpoint
and pressing Return advances to `Name: abc.pa  password:`. The clean native
capture at 650M has 28,467 pixels. Its compressed native copy survives
`make clean`, and the independently generated wasm32 checkpoint restores and
accepts `abc` as well.

**SDL launch fix (2026-07-10):** the first version of
`run-lisp-current-sdl` omitted `DORADO_DISPM_PRESENT=1`, although the
validated headless command and the older Lisp targets all set it.  The
interactive shortcut consequently reported no DispM board during
`DisplayInitConfig` and remained in Swat (`Trap instruction 77400 at 0`).
The target now reports DispM present and pins `DORADO_FAKE_TIME` to the
same deterministic value used by the successful 8.5B-cycle validation.

**Boot-scar root and repair (2026-07-10):** the `{1,101700}` value is the
sleeping-stack pointer in a real `PROCESS` record, not an emulator-generated
bad pointer.  Static FPTOVP decoding maps the archived Lyric bytes to a
64-word `PROCESS` at `{74,101600}`.  `PROC!37` defines its first two words as
`PROCFX0` and `PROCFX`; they contain `{74,101700}`.  The record also has
`PROCSTATUS=PSTAT.DELETED`, the `PROCDELETED` flag, and a non-NIL
`PROCRESETVARSLST`.  Hard-reset `PROCESSWORLD` therefore preserves it for
unwinding, then `\RELEASE.PROCESS` dereferences `PROCFX` before clearing it.
Startup rebases the saved segment 74 stack pointer to live `STACKHI=1`, whose
page is absent from the archived sysout, and RAID follows.

The repair clears only `PROCESS.PROCFX`, which is the state
`\RELEASE.PROCESS` itself establishes after releasing a valid context.  The
tool requires the record VA explicitly and validates the PROCESS layout,
deleted state, surviving reset state, and absent rebased stack page before it
writes a derived sysout.  It never changes the archive.  Full Lyric validation
with `DORADO_FAKE_TIME=1783285880` reached a scar-free desktop at 8.0B and
finished at 8.5B with 206,668 pixels.  The analogous Medley record is
`{74,075500}` with `PROCFX={74,075600}`; the sanitizer validates it and
produces the expected one-word change, but a full Medley desktop run remains
to be repeated.  `make run-lisp-current-sdl` now creates and serves the
sanitized Lyric derivative automatically.  A second 8.6B run delivered all
11 characters of `(PLUS 1 2)` after the clean desktop appeared; it produced
  no visible echo. A later focus-aware probe exposed the virtually cached
  IOPage bug above; after fixing it, guest-visible key state is correct but
  automated XCL echo remains the last interaction gate.

**The complete bypass semantics (commit 29c7ad8; six-witness triangulation
in its message):** with LC=5 ("T←Pd, RM/STK←Md") and BSEL=RM/STK, the ALU B
input sees the incoming Md — EXCEPT when BLOCK is set in the emulator task
(StackSelect): STK addressing gives the read and the write different cells,
so B keeps the old value (DoradoLispMc's resident Nova/BCPL emulator, pc
0o3321, depends on it — bypassing there Swats lisp.run's BCPL side).
All other B consumers (LongFetch address bits, dbuf, Q←B, R conditions)
always sample the raw register. `DORADO_BYPASS_TRACE` logs value-changing
firings.

**Verification:** resuming /tmp/pre-overflow.snap past the old
\FREESTACKBLOCK failure point reaches the full desktop by 5.7B cycles
(/tmp/lisp-blockfix.pgm). A fresh end-to-end boot validation is the
standard recipe (June-30 pack lineage + lisp.run/M; see the 2026-06-30
section). Gates all green: make test 12/12, kernel dm!38 + UnBug,
IfuSimple, Alto disk boot 2126 px @700M, Galaxian 121549.

**The `Raid: "Invalid address" {1,101700}` — 2026-07-04 findings:**
- NOT caused by the \FREESTACKBLOCK scan (that episode was re-traced: the
  wrapped scan legitimately found a 6734-word free chain at 0o14120 —
  internally consistent FSBs — split it, returned, execution continued in
  the new block; ESP=0o21054 afterward ✓).
- The faulting reference: a stale stack pointer 0o101700 read from
  heap-resident data and passed to an LLSTK type-checker (fnheader
  0o5053404, caller 0o5331240 does GETBASEN(x,1) → 0o101700 → FN1). The
  page {1,101700} is **provably absent from the sysout's FPTOVP** (offline
  table check: no file page maps vpage 0x183) — the saving machine's stack
  once extended there and the page was unmapped before saving. The fresh
  ALL-fixed-engine boot reproduces the identical message, so it is not a
  poisoned-snapshot fossil.
- RAID is fully interactive over the emulated keyboard (see the --type
  \xNN/CTRL commit): `S/E/T/A/...` echo and execute; **^N = continue
  (retries the fault — loops)**; **^D + Return = "Return to top level"**
  (Confirm() wants a bare Return) — after which the next layer surfaces:
  `"Bad array block reclaimed--continue with ↑N but save state ASAP"`,
  which also loops on ^N (same block or successive blocks — TBD).
- Raid sources mirrored: Raid.bcpl + RaidPrint/RaidProcs/RaidStack (+
  KbdIoSubrs, Raid.decl) under chm/lisp/fugue.6/bcpl/lispbcplsources.dm!1_/.

**RAID-marching findings (2026-07-04 late):**
- The ^D reset + ^N-continue sequence PROGRESSES: after ~40 ^Ns the errors
  moved from `Invalid address {1,101700}` through the bad-array blocks to
  `Invalid address {36,103626}` — each a reference into a vpage PROVABLY
  absent from the sysout's FPTOVP (vp 7815 sits in the hole 7743→12144).
  Working theory: \RAIDEXITFN's reset sweeps dead stack/heap objects and
  errors once per stale reference — possibly finite. A 200-^N mega-barrage
  is the test (in flight).
- FULL.SYSOUT!2 rejected early as predicted (IFPAGE key 032366 ≠ Lyric's
  044712 — needs its matched LISP.RUN!3 + DORADOLISPMC.EB from
  chm/lisp/current/ inserted into the pack; a future fixture variant).
- `--click X,Y` added (headless mouse; cursor visibly tracks) — but RAID's
  keyboard grab is BCPL-level TTY-global; clicking can't bypass an active
  Raid.
- The 200-^N mega-barrage FIXED-POINTS at `Invalid address {36,103626}` —
  every ^N re-errors identically (the earlier progression through the
  bad-array phase was real, but this one loops).
- **{36,103626} provenance (traced end-to-end this session):** the vpage
  (7815) is genuinely absent from the sysout FPTOVP (hole 7743→12144), BUT
  the value itself is RUNTIME-COMPUTED (the sysout holds zeros at the
  stack cells that carry it): it is the result of an IDIFFERENCE inside a
  signed-offset pointer-add helper (fnheader 0o4661534, atom 0o601;
  computes `x + y − 65536` with hi-word borrow — \ADDBASE-style negative
  displacement), i.e. wild INPUTS from its caller. Call chain at the fault:
  fn@0o4660000 → fn@0o4741130 (does GETBITS on the result at its pcx 0o57;
  faulting micro pc 0o1451) → 0o4661534. NEXT SESSION: identify
  fn@0o4660000/0o4741130 (fnheader word5 atoms; disassemble with
  /tmp/lispops.json + the established offline tooling) and trace the (base,
  offset) args one more hop — either an authentic stale heap object or an
  engine misread will fall out. Fault reproducer: resume
  /tmp/pre-overflow.snap + `--type '\x04\r' --type-at 5800000000 --type
  <20×\x0e> --type-at 6300000000`; first {36,103626} fault at cyc
  ~6,369,061,154 (FAULT_TRACE=all shows it; timings shift slightly).
- Alternative strategy (may sidestep the damaged sysout entirely): build a
  pack with the MATCHED `chm/lisp/current/` set (LISP.RUN!3 +
  DORADOLISPMC.EB + FULL.SYSOUT!2, IFPAGE key 032366) — FULL.SYSOUT was
  rejected by the Lyric-era lisp.run on the current pack (key mismatch),
  not proven bad.
- **2026-07-04: the matched `current/` set WORKS through netload + world
  start, and exposed + fixed a real HM-grounded engine bug (519f30d).**
  Chain: `make run-lisp-lyric-remote-long` with LISP_RUN_FILE/LISP_SYMS_FILE/
  LISP_MC_FILE/LISP_INIT_FILE/LISP_SYSOUT_FILE overridden to
  `../chm/lisp/current/*` builds the pack, netloads FULL.SYSOUT (the FTP
  close-with-Abort is the 1983 loader's SUCCESS path — RemoteVmemInit1.bcpl
  deliberately aborts the BSP after the last useful page), LoadRams the
  1983 DoradoLispMc.eb and starts the Lisp world. It then froze forever:
  the fault task parked at its ManyFaults breakpoint (`Branch[.],
  Breakpoint`, LFAULTS.mc) because our engine incremented FaultCnt on
  IFU-fetch map faults. HM Memory Section p.54: IFU-fetch map faults never
  wake the fault task and must not advance FaultCnt — the LispIFUMapFault
  trap re-fetches from the processor precisely so the fault task sees
  exactly ONE fault. Fix: `kind != DM_REF_IFETCH` guards in memory.c
  (FaultCnt/FirstFaultSRN/EmulatorFault) and cpu.c (task-15 wakeup).
  Verified: make test, Lyric desktop byte-identical (191,993 px incl. the
  {1,101700} scar — sysout data, unaffected as expected), kernel/IfuSimple/
  memMisc diagnostics PASS (eventCounters dm!5 = known pre-existing fail).
- **CLOSED (2026-07-06 evening): the FULL.SYSOUT panic is NOT fixed by a
  right-sized VMEM — the archived image references VM content that is not
  in the file. The `current/` path is unbootable as archived.** Three
  decisive results on top of the morning's VMEM-size work:
  1. **The morning's "19000-page" pack was silently truncated.** The
     CreateFile phase ran under `LISP_LYRIC_CREATEFILE_CYCLES=5.8B`, which
     is only enough for ~17.5K pages; the saved pack's `LISP.VIRTUALMEM.`
     label chain ends at page 17,564 (altofs: "discontiguous file_pgnum at
     VDA 1454: expecting 17565 but got 0"). 17,564 < 18,028, so vp 8206's
     backing page was STILL unreadable — the morning "still panics" result
     proved nothing about the size fix. `lisp-current-good-pack` now passes
     9B cycles; ALWAYS verify a rebuilt pack with `dsk2trident --extract` +
     `altofs --extract LISP.VIRTUALMEM.` (exact byte length, no chain
     warning).
  2. **A verified-complete 19,000-page VMEM still panics identically.**
     Rebuilt with 9B cycles (verified 19,000 pages, chain clean), booted
     with the reproducer time: the vp-8206 fault sequence still shows 73
     word-faults (that alone is not failure — under `noFaultFlg` the BCPL
     `PageFault` serves each vacant-page reference through the SwapBuf
     redirect without mapping the page, so a 73-word scan always makes 73
     faults), and the screen shows the same `Raid: Error in uninterruptable
     system code` panic by 4.0B, now with page 18028 readable (as zeros —
     the sequential install only writes sysout pages 0..6207).
  3. **The panic is time-INDEPENDENT.** On the same pack, the previously
     "known-good" `DORADO_FAKE_TIME=1783247912` produces the identical
     panic screen by 5B (and at 8.5B, 3759 px). The 2026-07-05
     time-dependence only ever gated which page got scanned first on the
     truncated fixtures; it does not gate the outcome.
  Mechanism recap (all authentic code paths, no engine bug found): sysout
  page 252 (real archive data, served byte-exact) contains the pointer
  `{0o40,0o7064}` into vp 8206; both era installers (`LocalVMemInit.bcpl`
  and `RemoteVmemInit1.bcpl`) copy the sysout SEQUENTIALLY into the VMEM
  ("the entries in the page maps on the file have the correct vmem address
  already"), so any PAGEMAP entry pointing at a file page >= NActivePages
  (6208) reads whatever the fresh VMEM holds — zeros. FULL.SYSOUT!2 (and
  the fugue/harmony `basics` images generally) carry many such
  beyond-NActive mappings; Lyric carries almost none, which is why Lyric
  boots. Static PMT/PAGEMAP decoding caveat for future sessions: only the
  FIRST pagemap page is at `IFPfilePnPMP0`; later pagemap pages are located
  through their own pagemap entries (see `LockSpecialPageVp`), so flat
  file-offset decodes beyond page 0 are unreliable — trust runtime traces.
  IFPAGE is sysout file page 1 (byte 512); `IFPValidKey=0o12743` for ALL
  eras (fugue through Lyric), so the fugue.6 BCPL sources' semantics govern
  every image we have.
- **Harmony pairing tried for the first time (2026-07-06 evening): same
  disease, different page.** Built a scavenged + verified-complete
  19,000-page-VMEM pack from the complete matched harmony set
  (`chm/lisp/harmony/basics/` LISP.RUN!1 + Lisp.syms!1 + DORADOLISPMC.EB!1
  + AltoD1MC.eb!1 + INIT.NONET!3, sysout LISP.SYSOUT!15 over fake FTP).
  The version gate passes, the world starts, and by 4B it dies with
  `Raid: Invalid address {26,154074}` — an UNMAPPED page in system space
  0o26 (vp 0o13330), i.e. the harmony image also references VM state it
  does not carry. Packs preserved: `build/good-packs/lisp-harmony-*.pack`
  (scavenged = pre-CreateFile base), run pack `/tmp/lisp-harmony-boot.pack`.
  Conclusion: only the Lyric-era images are self-contained enough to boot;
  the pre-Lyric `basics` sysouts on the archive all reference beyond-file
  VM content. The usable Interlisp-D remains the Lyric desktop
  (`make -C dorado run-lisp-current-sdl`).
- **NEW WORLD (2026-07-06 night): Interlisp-D Medley 1.0 BOOTS TO ITS
  DESKTOP.** `chm/archiveorg/_chm-parc_interlisp-medley1.0/LISP.SYSOUT!1`
  (LispV=0o115000) declares the same MinRVersion (0o13062) and
  MinBcplVersion (0o25400) as Lyric, so the existing Lyric pack + Lyric
  DoradoLispMc + Lisp.run!6 accept it unchanged: serving it with
  `--ftp-sysout` over the stored `lisp-lyric-desktop.pack` boots to the
  gray desktop + Prompt Window + Exec (XCL), 190,099 px at 8B
  (DORADO_FAKE_TIME=1783285880). Booted pack preserved as
  `build/good-packs/lisp-medley-desktop.pack`.
  Its Prompt Window scar is `Raid: "Invalid address" {1,75600}` (vp 379)
  vs Lyric's `{1,101700}` (vp 387) — BOTH bootable worlds show the same
  CLASS of scar: a stale pointer into the vp-380..390 stack region,
  unmapped in the respective sysout. Two successive releases carrying the
  same boot scar weakens the "authentic sysout damage" reading — an
  emulator-side early-init divergence that both worlds hit (something that
  should re-establish/ignore saved stack-region state before it is
  dereferenced) is now equally plausible, and there are two reproducers to
  compare. That comparison is the top lead for a scar-free desktop.
- **SCAR MECHANICS + THE REAL BLOCKER (2026-07-06 late night).** Probes on
  the two desktop worlds established:
  - The scar is date-INDEPENDENT (period-1988 vs 1998 dates both scar
    identically; DORADO_FAKE_TIME=1451606400 vs 1783285880).
  - The scar fault is real and captured: task 0, pc=0o1451 (the
    GETBITS-family opcode helper from the July-4 {36,103626} analysis),
    va 0o301700 (Lyric; 3 faults at cyc ~7.653B). The Lyric desktop+scar
    appears between 7B and 8B on a fresh boot.
  - **The scar RAID actively owns the TTY at the desktop.** Typing into
    the booted world is interpreted as RAID debugger commands — chars echo
    and execute (`T`/`E`/`S`... menus). Keyboard path is fully functional;
    `char_to_key` in dorado.c/dorado_sdl.c now maps `( ) * < >` (Alto
    digit-row shifts) so full Lisp forms can be typed.
  - `^D` + Return exits the scar RAID; the NEXT layer in Medley is
    `Raid: "DELREF on PTR with 0 refcnt" {74,120600}` — a reference-count
    UNDERFLOW detected at an htOflo overflow-table address. With Lyric's
    "Bad array block reclaimed" and FULL.SYSOUT's reclaim panic, THREE
    worlds show GC refcount corruption → suspect ONE emulator divergence
    in LGC.mc's GCLOOKUP path (harmony/ucode/lispdmc.dm!1_/LGC.mc):
    the `rcy[T,LTEMP1,11]` double-word shifter cycle, the `carry'`
    count-overflow test, the "Cancel BDispatch" DispTable[1,3,3] punt
    path, or the deferred overflow-table appends (.htpuntloop). The scar
    itself is plausibly the FIRST dangling-pointer casualty of that
    corruption.
  - The July-4 Md-bypass is EXONERATED for the Lisp world:
    DORADO_BYPASS_TRACE over a full Lyric boot shows zero substitutions
    after LoadRam (all firings are the known AEMU display-task case), and
    the desktop is byte-identical (191,993 px).
  - `make test` note: the CompareErr write-inhibit fix requires tests to
    clear CompareErr (DISKMUFF) between a RDCHK and WRITE phase like
    ACmmdCheck does — test_disk.c's mixed-write test was updated; suite
    is green.
  NEXT: instrument htMain/htOflo refcount stores (underflow detector =
  catch the first bad count in vivo), verify GCLOOKUP's dispatch-cancel
  and rcy/ldf semantics against the loaded control store (DORADO_IM_DUMP),
  with three reproducing worlds to cross-check any fix.
- **GC MACHINERY AUDITED — IT IS MECHANICALLY SOUND; the trail leads back
  to the boot scar (2026-07-07 morning).** Tools + findings:
  - `DORADO_HT_TRACE=1` (memory.c): logs htMain count-field UNDERFLOW
    stores and ALL htOflo stores. Real table bases from the final-debug
    BR dump: **htMainBR(BR 0o16)=va 0o4000000, htOfloBR(BR 0o17)=va
    0o4100000** (NOT the fugue-era spaces 0o73/0o74 — those are ordinary
    data spaces in the Lyric/Medley world). 128 pages = vps 0o10000+.
  - Loaded GCLOOKUP mapped in IM: `.htprobe` DispTable = 0o5210/11/12
    (add/sub/or with FF,,0 constants 0o2000/0o2000/0o1000 = ht1cnt/
    htstkbit), `.htpunt`=0o5213 (address ends ...11b — the
    "Cancel BDispatch" alignment trick verified in the loaded image),
    `.htstore`=0o5215 (AND 0o177000=htStkCnt), `BDispatch_ Case`=0o5205
    (FF=0o071, FA0/FB7/FC1 — matches our cpu.c dispatch group).
  - Audits passed: dispatch OR applied in all 5 next-address forms;
    Md-hold early-returns BEFORE dispatch consumption (pending dispatch
    survives holds); BDispatch's Link clobber matches the microcode's
    explicit LTEMP2 save/restore. Md-bypass fires ZERO times post-LoadRam.
  - Monitor results (full Medley boot): ZERO count underflow stores;
    punt records healthy ([Case<<8|hi, lo] pairs, Case∈{0,1}); the
    \GC.HANDLEOVERFLOW consumer zeroes processed records; oflo traffic
    only begins at desktop/RAID time (~7.8B).
  - The Medley `DELREF on PTR with 0 refcnt {74,120600}` decoded end-to-
    end: the object's htMain slot (lo>>1=0o50300 → va 0o4050300) held
    ANOTHER object's saved entry (0o010146: hi-bits 0o63, biased count 4)
    — hash collision; the delref punts; HANDLEOVERFLOW restructures the
    slot into a collision chain (linkp entry 0o1145 stored by pc 0o761 at
    cyc 8.3206B — the ONLY store to that slot in 10B cycles); the delref
    then finds an explicit count 0 → a DOUBLE-DELREF within the ^D
    RAID-exit sweep operating on the boot-scar's anomalous state.
    KEY SEMANTIC (opGCREF comment): "Null entry means a refcnt of 1" —
    the table only stores exceptions; "0 refcnt" = explicit zero entry.
  - The delref-time pcs (0o1451/0o1425, GETBITS-family) are the SAME pcs
    as the boot-scar fault — this is the error-reporting machinery
    walking objects. Everything funnels back to the BOOT SCAR as the one
    anomaly; the GC refcount engine executes correctly.
  NEXT (the one target left): why does boot-time code walk a pointer into
  the never-saved stack region (vp 379/387)? Candidate framing: the RAID/
  error machinery may be examining/printing an error context object and
  faulting on it — i.e., the displayed Invalid-address may be the
  REPORTER's fault, with a smaller first error underneath. Trace the
  FIRST entry into RAIDCode/\MP error paths before the scar fault at
  ~7.65B (Lyric) / ~7.7B (Medley), and find what error was being
  reported and who raised it.
- **SCAR ROOT NARROWED: it is a CONTROL-FLOW TRANSFER into the dead stack
  page, not a data dereference (2026-07-07 morning).** DORADO_FAULT_TRACE
  over the whole 260M-cycle pre-scar window (7.4B..7.66B, Lyric) shows
  exactly THREE faults, all on the scar page va 0x183C0 (0o301700,
  vp 387): two `kind=ifetch` (the IFU fetching INSTRUCTIONS from the
  stack page!) then one `kind=fetch` (the LispIFUMapFault trap's
  processor re-fetch, per the 519f30d semantics) → unresolvable →
  RAID "Invalid address". There is NO smaller earlier error — the scar
  is the first fault, and it is the Lisp PC/code-base landing inside the
  never-saved stack region: a RETURN or FUNCALL through a stale saved
  frame/code pointer from the resumed image. The earlier "GETBITS at
  pc=0o1451 dereferences a pointer" framing is RETIRED — pc 0o1451 is
  the trap re-fetch machinery, common to every such fault.
  NEXT CONCRETE MOVE: find who loads the code base/PC with the stack
  address — DORADO_BR_TRACE=1 DORADO_BR_TRACE_MB=31 gated
  7640000000,7653000000 shows the BrHi/BrLo loads of BR[31] (code base)
  into the vp-387 region with the loading pcs; from there identify the
  FUNCALL/RETURN site and the saved object it read (both worlds have the
  same shape: Lyric vp 387, Medley vp 379). If the frame walk turns out
  correct-per-image, the scar is authentic resumed-state damage; if the
  saved frame is fine and the walk mis-reads it, it is our engine bug in
  the return path (LCALLRET.mc / LSTACK.mc territory).
- **ROOT LOCALIZED (2026-07-05): the fault is a TIME-DEPENDENT missing map
  entry for page 200E (vp 8206).** Map-trace comparison of the SAME boot at a
  faulting vs non-faulting DORADO_FAKE_TIME:
    - Non-faulting boot: the loader churns (Map<- / vacate / remap) the page
      set {0000,0001,0052,0056,00FF,0200,0201,02FE,02FF,**200E**} — vp 8206 IS
      mapped, so DoradoLispMc's later scan of a pointer into it resolves.
    - Faulting boot (DORADO_FAKE_TIME=1783285880): the loader churns the SAME
      set MINUS 200E — vp 8206 is NEVER mapped, so the scan (pc=0o4130,
      pointer {0o40,0o7064}=0o10007064 read off STK[1]/STK[2]) faults.
    So whether page 200E lands in the loader's mapped-page set depends on the
    sole time() input. The 9 always-present pages (0000..02FF) plus the
    conditional 200E look like a fixed working-set the loader flushes; 200E's
    presence is data-driven and time-sensitive.
  IMPORTANT NUANCE (don't over-read the mapping angle): in the NON-faulting
  boot page 200E is mapped early but then FREED at ~2.735B (loader world) and
  never re-mapped, so it is ALSO unmapped by the 3.716B fault point — yet that
  boot does not fault. So the fault is NOT simply "200E unmapped"; the real
  divergence is whether DoradoLispMc SCANS a pointer into 200E at all. In the
  faulting boot it does (pointer {0o40,0o7064} sits on the Lisp stack); in the
  non-faulting boot it does not. The loader's differing 200E map activity and
  DoradoLispMc's differing stack pointer are correlated effects of the one
  time() input, not a simple cause->effect. So the real next step is to trace
  WHO writes {0o40,0o7064} onto the DoradoLispMc stack (STK[1]/STK[2]) in the
  faulting boot and what time-derived value produces the high word 0o40.
  OLDER (partial) framing — OPEN (the actual fix): why is page 200E conditionally in that set? Either
  (a) the emulator mishandles the date/time value so a memory word that feeds
  the page list differs (an engine bug — most likely, since time() is the only
  variable and it should NOT change which VM pages exist), or (b) legit
  Lisp/loader behavior. Next: with the reproducer, trace WHO builds the
  page-churn list (the Map<- at the loader pc doing these ops) and what memory
  word decides 200E — compare that word's value between faulting/non-faulting
  boots; it should reveal a date/time value leaking into a page number.
- **LEAD: the fault frame looks like STALE pre-LoadRam stack (2026-07-05).**
  DORADO_STK_WRITE_WATCH (new tool) at the reproducer time shows the fault
  frame's distinctive word 0o14673 is written to the STK ONLY in the AEMU
  era (cyc 78M-681M, before the ~3B LoadRam), and only to STK[01] — never in
  the DoradoLispMc era and never to STK[04] where the fault frame holds it.
  So DoradoLispMc's pc=0o4130 scan appears to read STACK CELLS IT NEVER
  WROTE (stale values left over from the AEMU/loader world) and treat
  {0o40,0o7064} as a pointer -> the out-of-bounds fetch. This points at an
  emulator STK-state issue across the LoadRam world switch (or a
  non-rm_stk_write push path the trace misses). NEXT: trace STK[04]/STK[03]
  writes too (DORADO_STK_WRITE_WATCH=0o56217), and check whether real HW
  clears/初始izes the STK at LoadRam; confirm via the DoradoLispMc source
  what pc=0o4130's routine expects on the stack.
- **CAVEAT on the stale-stack lead (unresolved inconsistency):** rm_stk_write
  (cpu.c) is the ONLY STK write path, and the value-watch caught 0o14673
  written ONLY to STK[01] (pre-LoadRam) — yet the fault frame reads
  STK[04]=0o14673. So either the frame dump / StkP interpretation is off, or
  the value reached STK[04] by a route not captured. RESOLVE THIS before
  trusting "stale stack": re-dump with absolute STK indices + StkP at the
  fault, and value-watch 0o56217 (STK[03]) and the pointer words too. The
  solid, trustworthy facts remain: deterministic reproducer
  DORADO_FAKE_TIME=1783285880; captured frame pointer {0o40,0o7064}=
  0o10007064 at StkP-relative [+0]/[+1]; time-dependent; no emulation-code
  regression.
- **FAULT REPRODUCED + STACK FRAME CAPTURED (2026-07-05).** Deterministic
  reproducer: `DORADO_FAKE_TIME=1783285880` (+ DORADO_DISPM_PRESENT=1, run to
  >=3.717B) makes the vp-8206 fault occur every boot (73 page-8206 faults;
  verified via FAULT_TRACE and now the STK probe). IMPORTANT LESSON: the
  fault was reproducing in earlier sweeps all along — my DETECTION was broken
  (a) DORADO_STK_ON_FAULT strtol("0o..",0)=0 bug (probe never fired; fixed),
  and (b) a pixel-count heuristic read mid-fault-storm (2907 px at 3.717B is
  NOT "no fault" — run to 3.72B to see 3768). Emulation code is byte-identical
  to 519f30d (no regression). The fault is time-dependent only via the sole
  `time(NULL)` input (DORADO_FAKE_TIME).
  Captured frame (`DORADO_STK_ON_FAULT=0o20016`):
  `pc=0o4130 va=0o10007064 StkP=1 Q=000040: STK[+0]=000040 STK[+1]=007064
  STK[+2]=056217 STK[+3]=014673` (all STK below StkP = 0). So the scan reads
  the 2-word pointer {hi=0o40, lo=0o7064}=0o10007064 straight off the Lisp
  stack (STK[1]/STK[2]); the LOW word increments with the scan, the HIGH word
  0o40 (which aims it at the 2M region past the 6208-page sysout) is fixed.
  STK[3]=0o56217, STK[4]=0o14673 are the adjacent frame words. NEXT (now
  cheap — deterministic reproducer): trace who WRITES STK[1]=0o40 (the bad
  high word) — a targeted STK-write trace around the fault, OR identify
  pc=0o4130's routine via the DoradoLispMc symbol map — to decide legit
  dynamic pointer vs corruption.
- **REPRODUCTION RECIPE + the `seq` gotcha that wasted hours (2026-07-05).**
  To hunt for a DORADO_FAKE_TIME value that reproduces the vp-8206 fault:
  loop `DORADO_FAKE_TIME=$t DORADO_STK_ON_FAULT=0o20016 DORADO_DISPM_PRESENT=1
  ./build/dorado ... --cycles 3717000000` over integer `t`, grep the log for
  `STKDUMP` (= hit). **CRITICAL: do NOT generate `t` with macOS `seq` for
  values ~1.78e9 — it emits SCIENTIFIC NOTATION ("1.78325e+09"), which
  strtoll parses as 1, so every iteration silently tests DORADO_FAKE_TIME=1
  (garbage). Use a bash C-style `for ((t=...; t<=...; t++))` loop.** This
  bug invalidated two full sweeps this session (all "no fault" results were
  meaningless). Confirmed lisp-flt (the one definite fault, start
  1783247910) read time() early; its faulting value is ~1783247910 + boot
  delay, but the delay is load-dependent and lisp-flt's exact value wasn't
  logged (ETHTIME_TRACE added after). Valid values tried so far (all
  no-fault): 1e9, 1783247912, 1783247914, 1783251520, and 1783247905..912.
  Each boot is ~3 min IDLE but ~7 min when the host is busy (Chrome/RustDesk/
  video etc. — watch `uptime` load; a full 50-value sweep needs an IDLE
  machine). Next if sweeping stays fruitless: add cycle-logging to the
  ETHTIME print to learn the exact cycle time() is read, or (better) find
  what Lisp computation consumes the clock and why it steers vp 8206 onto
  the stack — that's the real root, and needs the DoradoLispMc symbol map.
- **ROOT OF THE NONDETERMINISM FOUND + FIXED (2026-07-05): the sole
  wall-clock input is `time(NULL)` in ethernet.c (the NetExec date/time
  response); no rand/srand anywhere.** It is called ONCE, early in boot
  (before ~700M cycles), and its value flows into Lisp init. Pinning it with
  **DORADO_FAKE_TIME=<unix-seconds>** makes the WHOLE boot deterministic —
  PROVEN: two boots at the same pinned time gave byte-identical results
  (2907 px). So the vp-8206 fault is a deterministic FUNCTION of the
  time() value: some values fault, others (incl. every one tried midday:
  1e9, and ~1783247912..1783247930) give a clean 2907-px no-fault boot.
  The morning runs that faulted (e.g. lisp-flt @06:38, which showed 76
  page-8206 faults) got a faulting time() value that was NOT logged (the
  DORADO_ETHTIME_TRACE logging was added after). Verified my session's
  edits are behavior-neutral (git diff 519f30d..HEAD on cpu/memory/ethernet/
  machine = only env-gated additions), so the fault IS reproducible at the
  right value — it's a search problem, not a regression.
  - **To reproduce for debugging:** sweep DORADO_FAKE_TIME values (the boot
    is ~3 min each but now DETERMINISTIC, so each value is a definitive
    test) with DORADO_STK_ON_FAULT=0o20016; a STKDUMP line = a hit, and that
    time value is then a PERMANENT reproducer. DORADO_ETHTIME_TRACE prints
    the value time() returns. Faulting-time window unknown; morning wall-clock
    faulted but the exact second/value is lost. A coarse-then-bisect sweep
    over a wide range is the way to find one.
  - **Bigger picture:** DORADO_FAKE_TIME is a general testability win —
    it removes the only source of run-to-run boot variance, useful for ALL
    regression/debugging work, not just this fault.
- **(superseded observation) The vp-8206 fault looked NONDETERMINISTIC.** A
  clean boot with DORADO_STK_ON_FAULT=0o20016 (the committed reliable
  fault-triggered STK-dump probe) ran to 3.717B and produced ZERO vp-8206
  faults — final display = 2907 px vs 3419/3768 px in the runs that DO panic.
  So the boot diverges run-to-run and the fault does not always occur. This
  reframes the hypothesis: the bad pointer to vp 8206 likely depends on
  TIMING-SENSITIVE state (a race / timing divergence), not a purely
  deterministic data error — consistent with the whole fidelity-timing
  theme. Practical consequence: catching the fault for a probe needs either
  several attempts until it reproduces, or snapshotting AT a run where it
  fired then resuming (both expensive at ~3 min/run). Probe recipe when it
  does fire: `DORADO_STK_ON_FAULT=0o20016` (no FAULT_TRACE needed) dumps
  STK[StkP-8..+3] + pc at the fault.
- **Gotcha that cost this session hours: stderr is BLOCK-BUFFERED to files.**
  A redirected run shows a 0-byte log for most of its life and only flushes
  at exit (or when the buffer fills) — it is NOT stuck. Do NOT kill a
  "silent" run; wait for it to exit (or add periodic `--shot-every` output,
  or fflush). Several runs this session were killed prematurely on this
  false signal.
- **Iteration/tooling note for whoever resumes (learned the hard way).**
  Live tracing of the storm window (~3.716B cycles) is impractical by full
  replay: each run to 3.7B is ~3 min, and DORADO_PCDIS over a cycle-gated
  window repeatedly produced zero/garbage output or ran pathologically slow.
  Two concrete gotchas: (1) the Bash cwd silently drifts to the repo root
  between calls, so ALWAYS `cd .../dorado` inside the command or ./build/dorado
  isn't found (many "empty" traces were just this); (2) DORADO_IM_DUMP only
  fires with DORADO_FINAL_DEBUG=1, and dumps at END of run, so to see the
  DoradoLispMc world you must run PAST the ~3B LoadRam. RECOMMENDED next-time
  setup BEFORE probing: build a snapshot near the storm (`--snapshot-out` at
  ~3.7155B) and do SHORT resume-traces (~150K cycles, PCDIS with NO gate) —
  that's fast and reliable. Note the snapshot omits disk state, so pair it
  with the exact pack copy used to make it. (I could not get a snapshot to
  finish within this session's time budget — the make-the-snapshot run
  itself is the same ~3-min replay.)
- **Concrete anchor + accurate single-world picture (post-LoadRam
  DoradoLispMc disassembly, DORADO_IM_DUMP at ~4B).** In the FAULTING world:
    - pc=0o4110 = `BrHi<-A` (FF=0o124 = FA1/FB2/FC4), `A<-RM/STK` under BLOCK
    - pc=0o4114 = `BrLo<-A` (FF=0o123 = FA1/FB2/FC3), `A<-RM/STK` under BLOCK
    - pc=0o4130 = `Fetch<-RM/STK` (FF=0o300, JCN=0o107) — matches fault iw
  So the routine reads a VIRTUAL PAGE NUMBER off the STACK (BLOCK=
  StackSelect), builds `LScratchBR = vp<<8` from it, and fetches the page.
  This is the FlushVP / per-page-processing pattern (`BrHi_ T; BrLo_ ETEMP3`
  then a fetch/flush loop) in LISP0.mc. The vp it processes is 8206, which
  is beyond the loader's active-page map (IFPNActivePages=6208) → fault.
  So the CLEAN single-world question is: **why is vp 8206 on the Lisp stack
  here?** (a page-processing loop iterating vps, or a pointer/vp pushed by an
  earlier op). The AEMU-world free at 2.735B is a red herring — different
  world. NEXT: with a symbol map, name this routine and find who pushes vp
  8206; or trace the stack slot that pc=0o4110 reads back to its writer.
- **CORRECTION (2026-07-05, later): the "use-after-free / GC reference-count
  divergence" reframe below (commits f68eca1, 2f518c4) OVER-REACHED and is
  retracted.** It conflated TWO different worlds. The machine runs
  `lisp.run` as a BCPL program UNDER the AEMU (Alto-emulator) microcode; that
  world builds the Lisp VMEM map (createfile identity-map, then the sysout
  netload) and FREES vpage 8206 at cyc 2.735B — legitimately, since 8206 is
  beyond `IFPNActivePages`=6208. Only AFTER the sysout completes (~3B) does
  `lisp.run` `LoadRam` the DoradoLispMc microcode, which REWRITES the control
  store (verified: real pc=0o4130 disassembles to a DIFFERENT instruction at
  900M vs at the 3.716B fault — see DORADO_IM_DUMP below) and then runs Lisp
  doing ZERO further Map<- ops (the last map op in the whole boot is the
  2.735B free). So the free (AEMU/lisp.run world, pre-LoadRam) and the fault
  (DoradoLispMc world, post-LoadRam) are DIFFERENT microcode worlds — the
  pc=0o3333 "SetFlags" match was against the WRONG (Lisp) source. What is
  actually true and solid: DoradoLispMc faults following a pointer to vpage
  8206, which is beyond the loader's active-page map, so the page is
  legitimately not resident. The open question reverts to the ORIGINAL one:
  is that pointer legit sysout data (⇒ our VMEM/active-page setup is wrong)
  or miscomputed (⇒ an engine arithmetic/pointer bug). Resolving it needs
  WORLD-AWARE tracing (know when LoadRam fires) + a DoradoLispMc symbol map.
  New tool added this session: **DORADO_IM_DUMP="lo,hi"** (octal) +
  DORADO_FINAL_DEBUG=1 disassembles the LOADED control store (m->mc.im) so
  you can read what any real pc does in the running world — but note it
  dumps at END of run, so run PAST the ~3B LoadRam to see DoradoLispMc.
  The map-history / empty-page / BR-trace facts below are individually
  correct but their "GC free" INTERPRETATION is retracted per the above.

- **SUPERSEDED by the 2026-07-06 VMEM-size fix above. Historical trace
  details follow.** The `vp 8206` pointer and disk reads below are real, but
  the conclusion was wrong: `vp 8206` is backed by `FULL.SYSOUT!2`'s page map
  at VMEM file page `0o43154`, beyond a 15002-page fixture and within a
  20000-page fixture.
- **New 1983-world frontier — diagnosed 2026-07-05.** FULL.SYSOUT boots
  into Lisp and reaches a live RAID prompt: `Raid: Error in uninterruptable
  system code -- ^N to continue into error handler / -1 / @` at ~4.5B
  cycles, then `Raid: Called from uCode 4754Q / 5204Q` on each `^N`. This
  is Interlisp machine-panic `\MP.UNINTERRUPTABLE` (22Q, LLPARAMS): a page
  fault taken inside interrupt-disabled (tasking-off) system code. Root
  cause traced (NB: interpretation partly retracted — see CORRECTION above):
  - A microcode scan at real IM pc=0o4130 (`Fetch<-RM/STK`, MemBase=
    LScratchBR) reads consecutive VM words that are all VACANT — the fault
    map index is CONSTANT (idx=0o020016) across the whole storm, i.e. the
    same unbacked page re-faulting word-by-word (76+ times). Tasking is ON
    during the storm (task 0 plus io tasks 2/4/14 and the fault task 17
    all run) and the fault task services each fault, but it CANNOT make the
    page resident because it is beyond the sysout backing (next bullet), so
    every word reads vacant, the scan advances on garbage, and Lisp later
    panics `\MP.UNINTERRUPTABLE`. (An earlier draft of this note wrongly
    attributed the word-by-word re-fault to tasking being off — corrected.)
  - The scan reaches **vpage 8206** (va 0o10007xxx) but FULL.SYSOUT's
    `IFPNActivePages` (IFPAGE word 0o24) = **6208** = exactly its file
    content (6208 512-byte pages). So the scan runs ~2000 pages PAST the
    backed image. vpage 8206 sits in the VM upper half (past the 2^21-word
    / 0o10000000 boundary), within the 15002-page createfile'd VMEM file
    but past the 6208 sysout-written pages — i.e. an in-file-but-unwritten
    slot.
  - **How the bad address is built (DORADO_BR_TRACE, MB=34).** The loop
    (pc 0o4110→0o4114→0o4130) reads a 2-word pointer and loads LScratchBR
    from it: at cyc 3716543301 `BrHi<-A` with A=Q=**0o40**, then
    3716543303 `BrLo<-A` with A=**0o7064** → BR[34]=0o40<<16|0o7064 =
    **0o10007064** = the fault VA (mar=0, so va=BR[34] exactly; that's why
    the "same page re-faults" — BR[34] IS the incrementing scan pointer,
    rebuilt each iteration, no separate offset). The high word 0o40 (the
    thing that flings the pointer into the 2M region) comes from **Q**; the
    prior iteration had BrHi A=0 (low VM) — so this iteration's pointer
    changed to point high. Whether Q=0o40 is a legit stored pointer
    high-word (⇒ the 2M page SHOULD be backed and our VMEM/pager isn't
    backing in-file-but-unwritten pages — an engine/setup gap) or upstream
    corruption is THE open question. Provenance of Q=0o40 not yet nailed
    (DESC_TRACE at cpu.c:5607 didn't fire for these PCs — its gate/path
    needs a look; or add a targeted Q-source trace).
  - So the scan's address register (BR[34]/LScratchBR) is loaded with a
    pointer to vpage 8206 from a data structure it is walking.
  - **REFINED ROOT — it is a USE-AFTER-FREE, not an out-of-bounds address**
    (DORADO_MAP_TRACE, full boot). vpage 8206 is NOT invalid space: it is a
    real, dynamically-allocated page (real page 0x200E, identity-mapped)
    that Lisp's storage manager mapped and used, then **legitimately FREED**.
    Its complete map history: mapped present cyc 18.0M (pc=0o6366) → ref
    churn cyc 32M → vacated/remapped 39.7M/59.1M (pc=0o4114, normal
    SetFlags churn) → used (goes dirty) → **VACATED at cyc 2,735,595,951
    (pc=0o3333, Map<- with tioa=0o300 ⇒ wp=1,dirty=1 = Vacant)**. That is
    Lisp's `SetFlags[v,Vacant]` returning the page to the storage manager.
    Then ~1B cycles later the pc=0o4130 scan follows a still-live pointer to
    the freed page → page fault → the fault task cannot re-back a
    deliberately-freed page → re-faults word-by-word → `\MP.UNINTERRUPTABLE`.
    NO map op touches vpage 8206 between the free (2.735B) and the fault
    (3.716B) — so at fault time the page is legitimately vacant. The
    "past IFPNActivePages / out-of-bounds" framing above is superseded:
    8206 is a valid dynamic page, the bug is the DANGLING REFERENCE to it.
  - This reframes the root as a **GC / reference-count divergence**:
    real Interlisp-D (reference-counted, htMainBR/htOfloBR tables) would
    not free a page still reachable from a live pointer, nor keep a live
    pointer to a freed page. An upstream engine divergence made either the
    free premature (a refcount hit 0 early) or the scanning structure stale.
    Next probe: (1) identify what pc=0o3333 is (the SetFlags/ReturnPage
    caller) and what made vpage 8206's refcount drop; and/or (2) find who
    holds the pointer {0o40,0o7064} the scan walks and whether that
    structure should still reference 8206. The GC reference-count opcodes
    (opGCREF/GCADDREF/GCDELREF in LGC.mc, htMainBR/htOfloBR) are the prime
    suspects for the upstream divergence.
  - **The free was NOT a premature-free-of-a-live-object.** DORADO_VM_DUMP
    of vpage 8206 at cyc 2.0B (while it was still resident, pre-free) reads
    ALL ZEROS — it is an empty scratch page, not a dense live structure. So
    the free (of an empty page) looks legitimate; the anomaly is the
    DANGLING POINTER to it. That narrows the root to either (a) a
    reference-count UNDERCOUNT — a reference to the object existed but was
    never counted (a missed GCADDREF), so the storage manager freed a page
    still pointed-to; or (b) a MISCOMPUTED scan pointer — Q=0o40 (the high
    word that aims the pointer at the freed 2M page) is wrong. Deciding (a)
    vs (b) still needs Q=0o40's provenance (unresolved: DESC_TRACE at
    cpu.c:5607 doesn't fire for these PCs; narrow-gate PCDIS also produced
    nothing — needs a working targeted trace of Q's source before pc=0o4110).
  - **Suspected shared root with the Lyric world.** Lyric's post-RAID
    errors are "Bad array block reclaimed--continue with ^N" — literally a
    GC/reclaim error. If the same reference-count divergence underlies both,
    fixing it could unblock BOTH the FULL.SYSOUT panic and Lyric's reclaim
    errors. Worth checking whether Lyric also exhibits a premature
    free / dangling ref (DORADO_MAP_TRACE on the pages its reclaim errors
    touch) as a second test case.
  - **Blocker to going deeper: no symbol map for the loaded DoradoLispMc.**
    pc=0o3333/0o4130/0o3314 can't be resolved to source labels — the
    running DoradoLispMc.eb is a LoadRam image, the UnBug DoradoLisp.MB
    symbols don't match this build, and no .DLS listing exists. Pinning the
    exact refcount site would need either regenerating the listing (run
    Micro+MicroD on chm/lisp/harmony/ucode/lispdmc.dm — a real toolchain
    task) or a dedicated htMain/htOflo refcount-trace instrument.
  - **Lyric never does this.** A full-boot fault census of the clean Lyric
    world: 1662 faults, ALL demand-paging at pc=0o6654 (1286) / 0o6343
    (371) that resolve; ZERO at pc=0o4130, ZERO at va 0o10007xxx. So the
    use-after-free is specific to the FULL.SYSOUT world's execution.
  - Reproducer: `make run-lisp-lyric-remote-long` with LISP_RUN_FILE/
    LISP_SYMS_FILE/LISP_MC_FILE/LISP_INIT_FILE/LISP_SYSOUT_FILE set to
    `../chm/lisp/current/*`; storm at cyc ~3.716-3.718B, panic on screen
    ~4.5B. FTP server now prints `FTP_ABORT code=... text=...` under
    DORADO_FTP_TRACE.
- **Usable-Lisp status:** the Lyric world remains the closest thing to a
  usable Lisp (boots to desktop: Prompt Window + Exec (XCL), 191,993 px),
  but its Prompt Window shows the authentic `{1,101700}` sysout scar and
  RAID's TTY-global keyboard grab can hold input. FULL.SYSOUT was pursued
  as a clean-sysout alternative; it now boots to RAID but panics in init
  per the above. Neither path yet yields a fully interactive Exec.
- The games' unrelated blocker remains ethernet completion timing.

**LAYERING CORRECTION (9bd7f76, supersedes the c9aa818 shape):** the bypass
must substitute Md ONLY at the **ALU B input** (alu_op call site). The
b_bus-level version corrupted every direct-B consumer — fatally the
**LongFetch address high bits B[4:15]** used by the AEmu display DCB walker
(`LongFetch←T, T_ Pd, Rx_ Md`, ALUFM="A+1" so Pd ignores B), which killed
the whole Alto disk boot (white screen, FTP never started). Static grep of
AEmu.mb!2/Initial.mb/Bootstrap.mb shows zero LC5/BSEL1 — but the RUNNING
AltoMesaDorado.eb display microcode uses it (found by live bypass-firing
trace with ALUFM logged). Gates all green: make test 12/12, kernel dm!38 +
UnBug, IfuSimple, Alto disk boot Exec-paints @700M (2126 px), and no
\DOSTACKOVERFLOW entry through the old Lyric failure window.

Also learned this stretch: the docs' eventCounters dm!5 "PASS @5,355,594"
is NOT clean-reproducible even at its own commit (c6397d7) — clean builds
fail at EVENTAHOLDERRLO @4,046,387 there and on main (a git-bisect first
pointed at e482936 but was invalidated by clean-build verification; beware
incremental-build staleness across checkouts when bisecting).

**NEW FRONTIER (post-fix 9B boot, /tmp/lisp-boot2-*):** with the corrected
bypass, the fresh Lyric boot reaches a NEW endpoint — between the 3.5B and
4B screenshots (the Write IM / Lisp-start era) the screen becomes the Alto
**Swat: "CallSwat from 56605 / Memory map confused / Try triple booting to
reinitialize map"** (11546 px, stable to 9B; snapshot at 9B =
/tmp/lisp-boot2-9b.snap). Every AEmu-world bypass firing site has a
B-independent ALUFM op ("A+1"/"A−1"/"0"), so the fix cannot have perturbed
the Alto phase — the likely story is that Lisp now runs FURTHER (working
LLSH1), exits/gives up somewhere new, and the returning Alto OS finds its
map clobbered. NEXT: rerun the boot to ~3.6B with --snapshot-out, then
forensically walk the Lisp phase to its giveup (same loop as the
\FREESTACKBLOCK investigation: LISPFN watches on the failing region,
IFUDISP/PCDIS gated windows, VM dumps against the sysout). Boot runs have
small cycle variance run-to-run (±10K observed June 30) — expect the
transition cycle to shift slightly.

**The root cause of the \FREESTACKBLOCK episode (fixed):** HM p.35/p.77 —
when one microinstruction reads RM/STK onto B AND loads the same slot from
Md (LC=5), hardware bypasses the B read to the incoming Md. Micro's
multi-assign idiom `LTEMP2_ T_ Md` (= .UNBOX1's first instruction) assembles
as LC=5 + BSEL=RM/STK + ALUFM="B" and needs the bypass to put Md into T. Our
engine returned the stale register, so every opLLSH1 on a valid SMALLP
mis-unboxed → non-FIXP → CallUFN → the UFN handler re-ran LLSH1 → ~11
recursion rounds ate the user context's 586 free words → STKOVPUNT →
\DOSTACKOVERFLOW → \MOVEFRAME → \FREESTACKBLOCK(0o36, 0o2674, NIL) → the
scan merge-spun on degenerate saved-stack data. Fix: cpu.c b_bus BSEL=1,
LC==5 → B=task_md(). Two sharp edges pinned by PARC-microcode oracles (see
the commit message): LC=4 must NOT bypass (eventCounters `rscr_ Md`), and
B←T must keep OLD T under LC=2/3 (PUSHTMD pushes old T; bypassing traps
Lyric into Swat).

**State after the fix:** resuming the pre-overflow snapshot
(`/tmp/pre-overflow.snap`, cycle 5.296B, pack copy of `/tmp/lisp-preov.pack`)
past the old failure point: the LLSH1 UFN recursion and its stack overflow
NO LONGER OCCUR. Downstream (5.5B–6.5B) the run still degrades — observed
variously in a later \FREESTACKBLOCK spin or parked in a small insset-0
handler (base 0o15534/0o44026) — AND long resumes are **non-deterministic
run-to-run** (disk/PDI state is not in snapshots; documented omission in
machine.c), so cycle-precise claims are only reliable near the resume point.

**Follow-on leads (in order):**
1. The next Lyric blocker: from a fresh boot with the fix, find where init
   stalls now (fresh-boot repro: copy `/tmp/dorado-lisp-lyric-m.pack`, run
   the June-30 command in CONTINUE-HERE's 2026-06-30 section; the fixture
   chain `make run-lisp-lyric-remote-long` currently FAILS earlier —
   lisp.run/M drops to the Exec by 1B — a separate fixture regression).
2. \FREESTACKBLOCK's scan fragility stands (any overflow episode can still
   spin on the {0o177777, size=0} junk at stack offset 0o6203) — decide
   whether real HW tolerates this via data (needs a working reference) or
   whether our remaining divergence (why an overflow still happens at all
   post-fix) is the true question.
3. Diagnostics-table staleness on main: eventCounters dm!5 FAILs @4046387
   (EVENTAHOLDERRLO), IfuComplex FAILs @10516941, full MEMA.DM TIMEOUTs —
   all identical on pristine main; either a post-June-26 regression or the
   documented runs used env setups not recorded in running-diagnostics.md.
4. Instrumentation kept (env-gated): DORADO_LISP_FN_TRACE=<octal br31 list>
   logs IFU dispatches by code base (64/base cap) — note it misses one
   dispatch path (a post-fix run sat in 5054130 with zero hits; find the
   second dispatch site if reusing).

## ===> PREVIOUS (2026-07-03): poke past the \FREESTACKBLOCK spin
## reaches "Raid: Called from uCode NIL" with a live @ prompt — display
## pipeline PROVEN end-to-end; hang chain fully mapped.

**The poke experiment (validates the whole diagnosis):** un-sticking the
merge-spin by poking the stuck size word —
`DORADO_POKE="206204,40173,7002100000"` on the 7B-snapshot resume — lets Lisp
RUN ON: it clears the screen to white and renders, via the real DDC display
pipeline, the text

```
Raid: Called from uCode NIL
@
```

(357 px; readable in /tmp/lisp-poke-9b.pgm; ASCII/PNG recipe in the git log).
That is Interlisp-D's RAID debugger with an interactive prompt. The
`uCodeCheck` punt is expected — the poked 0o40173-word "free block" overlaps
the guards/upper frames, and the microcode caught it — so the poke is a
diagnosis validator, NOT a fix. What it proves:
- The \FREESTACKBLOCK merge-spin WAS the (only) post-banner hang.
- Screen-clear, font rendering, and the DDC path all work.
- The Lisp error machinery runs and waits for keyboard at `@`.

**The refined causal chain (2026-07-03 session):**
- The hang context is a *stack overflow during init*: caller chain decoded
  from the live FX frames = `\DOSTACKOVERFLOW` (fnheader VA 0o5052744) →
  `\MOVEFRAME` (0o5053020, callsite pcx 0o227, FN3) →
  `\FREESTACKBLOCK(SIZE=0o36, START=0o2674, NIL)` (0o5054130). START=0o2674 =
  the user context's frame (IFPAGE CurrentFXP); the handler runs in its own
  context whose frames live at stack offsets 0o11002/0o11026/0o11070.
- Live Lisp ucode registers (RM map from LISPDEFS.mc: ESP=RM0, NARGS=RM4,
  TSP=RM7, IVAR=RM10, PVAR=RM12): ESP=0o12400, TSP=0o11130 → 680 words free
  in the handler context; WANTEDSIZE = 0o36+0o1440(800) = 0o1476 = 830 words;
  the biggest mid-stack FSB is 0o1360 = 752 → nothing fits, so the scan
  legitimately walks the whole stack space and derails as documented below
  (crosses the deepest frame's stale NEXTBLOCK=0o11124 into live temps →
  garbage NEXTBLOCK=1 → odd-lattice climb → merge-spin at 0o6203 {177777,0}).
- pvar slots confirmed live: PVAR=0o11102: slot0=WANTED(0o1476),
  slot2=EASP(0o46376, = IFPAGE[7]), slot4=FREEPTR(0o6203),
  slot6=FREESIZE(0), slot10=SCANPTR(0o6203).

**Open questions (the true root, in order):**
1. WHY did the user context (frames at 0o2674 area) stack-overflow during
   early init? Candidate: a spurious punt from opFN's room check
   (`ESP - stkmin - TSP` carry test in LCALLRET.mc .ATOMICFN) — verify our
   subtract-carry semantics on that path; or genuine stack pressure from an
   earlier emulator divergence.
2. How does a REAL machine's \FREESTACKBLOCK scan survive crossing the
   active handler frames (deepest frame's NEXTBLOCK is stale + temps are
   live on any machine)? The Lyric bytecode has extra field checks
   (GETBITS 0,0o140 / 7,0o207 at pcx 0o166-0o215) vs the Intermezzo source —
   decode that path before assuming the scan derails identically on real HW.
3. Next concrete probe: fresh boot with traces gated around the FIRST
   stack-overflow punt (STKOVPUNT / ADDSTK in LCALLRET.mc) — capture
   ESP/TSP/stkmin at punt time to decide spurious vs real. Also possible: at
   the RAID `@` prompt, RAID commands are keyboard-driven (`LU` prints the
   stack) — the poked machine is interactively debuggable!

## ===> PREVIOUS (2026-07-02): Lisp post-banner hang ROOT-CAUSED —
## \FREESTACKBLOCK merge-spin (suspect was "under-allocated frame"; superseded
## by the stack-overflow-context finding above).

The 2026-06-30 "post-banner display loop" is fully diagnosed (all read-only
probing from the 7B snapshot; no model changes yet). The complete causal chain,
each link verified:

1. **The display side is a red herring.** The terminal DCB chain at VA 0o110000
   is a legitimate *blank bootstrap chain* (every scanline points at the same
   6-word zero buffer at 0o155070). The DDC render path works; Lisp simply
   never installs the real display because init never finishes.
2. **Task 0 spins in an interpreted Lisp loop, not BitBlt.** The
   `SRCGRAYDST*`/`STORELASTSRCDST` PC names were wrong-build symbol noise
   (DoradoLisp.MB ≠ the running DoradoLispMc.eb). The loop is Lisp bytecode at
   fnheader VA 0o5054130, IFU insset 1, pcx 0o122..0o277.
3. **The function is `\FREESTACKBLOCK` (LLSTK)** — identified by disassembling
   its bytecode from the sysout (opcode tables extracted from the Dorado Lisp
   microcode source, now mirrored at
   `chm/lisp/harmony/ucode/lispdmc.dm!1_/` — LISP0/LISPDEFS/LCALLRET/LSTACK/
   LBITBLT/LVARCONST/LMEM/LJUMP/…) and matching it against the Intermezzo
   `LLSTK` Lisp source (fetched to /tmp; on eris `<Lisp>Intermezzo>SOURCES>`).
   Stack-block tags: flags = top 3 bits; FSB=5, GUARD=7, FX=6, BF=4, NOTFLAG=0;
   FX.NEXTBLOCK = word +4; pvar slots confirmed
   (pvar1=endofstack=0o46376 from IFPAGE[7], FREEPTR=0o6203, FREESIZE=0).
4. **The spin:** FREE-merge loop with FREEPTR=0o6203 whose "FSB" is
   `{0o177777, size=0}` → `FREESIZE += 0` forever. Engine execution of the
   loop is micro-verified correct (GETBITS field=7, GETBASEN=0, live-traced).
5. **The walk that got there (replayed offline, reproduces exactly):** scan
   from StackBase 0o1400 walks ~20 well-formed frames/FSBs → crosses the
   scanner's own frame **FX@0o11070 (NEXTBLOCK=0o11124)** → climbs through the
   *live interpreter temporaries* at 0o11124+ (the loop's own pushes) →
   misparses a stale tag-6 word at 0o11174 with NEXTBLOCK=000001 → BF-climbs
   the ODD lattice from 1 → first odd tag-7 word = 0o6203 → merge-spin.
   Replayer + full stack dump diff: only the active frame page (0o211xxx) and
   0o201400-0o202777 differ from the sysout; ALL walked block data is
   byte-identical to the sysout (VMEM load is correct; vpage 268 ↔ sysout file
   page 778, contiguous through the stack space).
6. **The suspected root:** the scanner's frame block is 0o34 (28) words
   (0o11070..0o11124) but its FNHEADER (VA 0o5054130) says stkmin=0o76 (62)
   words — the frame is under-allocated, so its temps overflow past NEXTBLOCK
   and the walk-through-own-frame invariant breaks. Suspect the **microcoded
   frame allocator** (`opFN`/`\MAKEFRAME` path in LCALLRET.mc) mis-executing on
   our engine (FNHEADER stkmin/na/pv fetches use the same IFetch/←Id operand
   machinery where we've had offset bugs; a shift/halving error would also fit
   0o34-vs-0o76).

**Next steps (concrete):**
1. Read LCALLRET.mc's opFN/frame-allocation size computation (mirrored source);
   derive the expected NEXTBLOCK for the \FREESTACKBLOCK call.
2. Fresh boot with a cycle-gated micro-trace (DORADO_PCDIS) on the opFN handler
   around the first post-sysout frame allocations; compare each store (BF/FX
   fields, FSB split) against the source. The mis-computed fetch/store is the
   engine bug.
3. Note: frames at 0o11002/0o11026 (sizes 0o20/0o32) may be equally
   under-sized — check their callees' fnheaders too; if ALL frames are ~half
   size, hunt a shift; if only interpreted-call frames, hunt the FNHEADER read.

Probe tooling used (all still valid): `DORADO_VM_DUMP` (octal va:count,...),
`DORADO_VM_FIND_PAIR`, `DORADO_STORAGE_DUMP`, `DORADO_LOAD_TRACE_VA` +
`DORADO_TRACE_GATE`, `DORADO_IFUDISP_TRACE`, `DORADO_PCDIS=lo,hi` +
`DORADO_PCDIS_LIMIT`, snapshot resume via
`--snapshot-in /tmp/dorado-lisp-lyric-m-7b.snap` (+ a COPY of
`/tmp/dorado-lisp-lyric-m-pchist.pack`; don't mutate the original).

## RESOLVED (2026-07-06): "some games show nothing" — RAM-microcode games
## trap into TeleSwat; others just need a keystroke or more load cycles

Investigated Pool/StarWars/Trek (0 display-list pixels at any cycle count)
plus the "minimal display" games. No emulator bug: every behavior matches
the real Dorado. Three independent causes:

1. **EFTP load time dominates.** Boot + the lock-step EFTP stream cost
   ~1M cycles per 512-byte packet, so game code starts at ~77M cycles for
   a small file and ~190M for Trek (156 packets). A 100M-cycle snapshot of
   a big boot file is a picture of the loader. (This alone explained the
   stale "Boggs shows nothing" note — Boggs paints its portrait fine.)
2. **AstroRoids/Invaders wait for a keystroke on an empty FullBootInit
   display stream** (zero-width DCB chain at `DASTART`, `nwords=0`). Type
   any key once loaded and they paint (verified: 437k / 71k px). Reversi
   is a text-prompt UI (~1.4k px is its real screen).
3. **Pool, StarWars, Trek load custom Alto RAM microcode and die,
   authentically.** Pool executes `JMPRAM` (61010B); StarWars and Trek
   execute custom `70000B`-family opcodes; MissileCommand paints attract
   first, then does the same. Per the real AEmu source
   (`chm/doradosource/AEmuSources-cedar6.0.dm!1_/ATraps.mc`): `WRTRAM`
   no-ops, `RDRAM` returns 0 always, `JMPRAM` -> `NPTrap`, and op bytes
   160B-177B (`70000B..77777B`) all dispatch to trap microcode. The games
   never probe (WRTRAM appears to succeed), jump in, trap, and the
   FullBootBase trap path (all vectors `530B..567B` = `JMP @176B`) ends
   in Swat.

**Identity of Sessions 4-6's "raw ether exchange service": it is
TeleSwat** (`chm/altosource/buildboot.dm!2_/TeleSwat.asm`), the remote
debugger in every BuildBoot FullBootBase resident. Protocol on Pup socket
60B (`socTeleSwat`): `200B`=Store, `201B`=Fetch, `202B`=Swap (arms the
~5.4s `dally` deadline against RTC 430B), `203B`=SwapReply (restore +
resume), `204B`=ptSwatAck (the reply). The mystery 204B transmissions
with body `[401B, 1000B]` are a Swat-ed program *acking the fake
server's periodic 201B probes* as Fetch requests ("M[401B]=1000B"), and
the solid-block/inverting cursor is TeleSwat's `sa2` cursor-invert on
every ack. A trapped world therefore looks "alive" on the wire while its
coroutine ring is frozen. Do NOT stop the 201B broadcasts — Session 4
showed they are what starts NetExec's contexts.

Diagnostics added: machine-debug now prints `TRAPPC=` (Alto `M[527B]`,
nonzero = the world took an S-group trap), and `DORADO_FINAL_MEMDUMP=1`
dumps the full 64K Alto space as `MD va val` lines whenever the machine
debug runs (pairs with `DORADO_FINAL_DEBUG=1`; complements cpu.c's
`DORADO_MEMDUMP_AT`, which only arms on the disk-boot loader entry).
Makefile game-target comments and `docs/running-the-emulator.md`
reclassified accordingly.

**Same day, web frontend: the deployed Pages build was dead in current
Chrome, and rebuilding with emsdk 6.0.0 exposed two SDL2-port breaks —
the web build is now SDL-free.** The old deployed build crashed at
`callMain` (`TextDecoder ... ArrayBuffer must not be resizable` — old
emscripten vs new Chrome). Rebuilding with emsdk 6.0.0 fixed startup but
(a) the SDL renderer never presented a frame (SDL_CreateRenderer +
UpdateTexture + RenderPresent left the canvas black; a raw gl.clear from
JS painted and even *persisted* across SDL "presents"), and (b) SDL
keyboard events never reached SDL_PollEvent (mouse-motion did; the
keydown listener was registered on window but keys never queued).
Emulation speed was never the problem: the identical core built to wasm
runs 100M cycles in ~7.7s under node (~native/1.5). Fix: dorado_web.c no
longer links SDL at all — frames go to the canvas via an EM_JS
`js_present()` (2d context + putImageData; pixels are COPIED out of the
heap first because Chrome rejects ImageData views over the resizable
ALLOW_MEMORY_GROWTH buffer), and web_shell.html feeds input through
exported `dorado_web_key()`/`dorado_web_mouse()` (unshifted-ASCII + a
small WEB_KEY_* table — keep the C and JS maps in sync). Verified in
Chrome: NetExec menu ~20s after load, typing `galaxian` + Return at the
prompt boots and renders the attract screen. Console diagnostics that
remain on purpose: a ~256-frame heartbeat (`cyc/px/chunk`) and a
`slow frame`>1s logger in the shell. Beware while testing: Chrome fully
suspends rAF for occluded/unfocused tabs, which looks exactly like a
frozen emulator.

## ===> PREVIOUS (2026-06-30): Lisp `/M` loads DoradoLispMc and
## reaches the post-sysout banner loop.

The current Lisp frontier is **not** disk labels, VMEM creation, fake FTP/BSP,
or `LoadRam`. The physical-preserving BcplProg pack plus native
Scavenger/CreateFile flow can retrieve Lyric `LISP.SYSOUT` over fake Pup
FTP/BSP, write it into `LISP.VIRTUALMEM.`, load `DoradoLispMc.eb`, and execute
the loaded Lisp microcode. The active bug is after the sysout banner: Lisp keeps
executing and the display task keeps running, but the screen does not advance
past:

```
{DORADO}LISP.SYSOUT!1, 31-Dec-83 19:00:00...
{DORADO}LISP.SYSOUT!1, 31-Dec-83 19:00:00...
```

Latest trace facts:

- The earlier low-core-link hypothesis was over-broad. In failure cases that
  return to the Alto Executive, the OS intentionally reloads `Executive.Run`'s
  page-zero image. Original `OsMain.bcpl` does this in `SystemMain`:

  ```
  ReadBlock(subsys, 16b, 16b)
  ReadBlock(subsys, 16b, 300b-16b)
  ```

  With `subsys == Executive.Run`, this copies `0o262` words to
  `0o016..0277`. `SYS.SYMS` resolves the trace to `ReadBlock+0142` called
  from `SystemMain+077`; the stream FP is `Executive.Run`
  (`serial 0o155`, leader VDA `0o034`). That overwrite is expected after
  `GiveUp`/`LISPFINISH`, not evidence of a BLT or disk bug.
- The small `SMALL.SYSOUT!1` smoke case hits exactly that expected path after
  `CheckIPage` rejects the sysout as too old. It is no longer a valid proxy for
  the compatible Lyric post-transfer failure.
- Plain `lisp.run {DORADO}LISP.SYSOUT` is the wrong Lyric launch path for the
  current AEmu setup: it completes FTP/BSP (`file=4824064/4824064`, `open=0`,
  `wait_ack=0`) but never reaches `Write IM`, then idles in the Alto Executive.
  `lisp.run/M` forces `DoradoLispMc.eb` loading.
- Full Lyric remote `/M` run:

  ```
  DORADO_DISPM_PRESENT=1 DORADO_WRITEIM_TRACE=1 \
    ./build/dorado --eb worlds/aemu.eb \
    --disk 0=/tmp/dorado-lisp-lyric-m.pack --boot-reason disk --no-alto-boot \
    --ftp-sysout ../chm/archiveorg/_chm-parc_interlisp-lyric/LISP.SYSOUT\!1 \
    --type 'lisp.run/M {DORADO}LISP.SYSOUT\n' --type-at 330000000 \
    --key-hold 4000000 --cycles 7000000000
  ```

  `Write IM` starts at cycle `3770919107` (repeat run: `3770909938`), then the
  loaded Lisp microcode continues executing. FTP/BSP is closed cleanly at the
  end (`file=4824064/4824064`, `open=0`, `wait_ack=0`).
- A 7B-cycle run with `DORADO_MACHINE_PCHIST=1` saved a reusable resume point:
  `/tmp/dorado-lisp-lyric-m-7b.snap`. Continuing that snapshot to 10B cycles
  leaves the same banner-only screen, but display counters advance
  substantially (`outputs` from ~18.9M to ~23.9M; terminal messages from ~217K
  to ~311K), so this is not a hard CPU halt.
- A post-banner Return key probe from the 7B snapshot has no visible effect by
  8B cycles. Treat this as a post-`MainInit`/`InitLisp` display or early Lisp
  initialization loop, not a simple prompt waiting for input.
- A short `DORADO_PCDIS` slice from the 7B snapshot shows task 0 repeatedly
  executing Lisp interpreter/BitBlt paths (`SRCGRAYDST*`, `STORELASTSRCDST`,
  IFU PCX around `000122..000141` and `000260..000277`) with display task slices
  interleaved. That is the next concrete debugging surface.
- The junk task path was audited against `Junk.mc!1`. `AckJunkTW` is executed
  by task 2 at `pc=4257` with `b=141217`, the expected odd `RTCDeltaLo`, so the
  timer stays enabled. A real emulator bug was found in the hold path: memory
  `Md`-hold cycles incremented `cpu->cycles` but did not tick the external
  junk-task pendulum. `cpu.c` now ticks the junk timer during held cycles before
  scheduling, matching the hardware's external 32 us source. `make -C dorado
  test` passes after this change.
- The timer fix is necessary but not sufficient. A resume to `7100000000` cycles
  with `DORADO_LISP_FORCE_KEY_MASK=1` still shows the same two sysout banner
  lines. The display task remains active (`TDCB=110040`, many terminal/DDC
  counters advance), but the DCB bitmap samples are still zero.

Current useful artifacts:

- `/tmp/dorado-lisp-lyric-m-7b.snap` — machine snapshot after `/M`, FTP,
  `Write IM`, and the banner.
- `/tmp/dorado-lisp-lyric-m-pchist.pack` — matching mutable pack for that
  snapshot.
- `/tmp/dorado-lisp-lyric-m-pchist.pgm` / `/tmp/dorado-lisp-lyric-m-10b.pgm`
  — banner-only screens at 7B and 10B.
- `/tmp/dorado-lisp-return-probe.pgm` — Return-key probe, still banner-only.
- `make -C dorado run-lisp-lyric-resume-debug` resumes from
  `LISP_LYRIC_SNAPSHOT_IN`/`LISP_LYRIC_RESUME_PACK` for short post-banner
  probes.

Next steps:

1. Trace `InitLisp`/display handoff state from the 7B snapshot: interface-page
   display words, `DASTART=110000`, DCSB/NLCB state, and the Dandelion display
   task path. The current display IOFetch samples are all zero even though the
   terminal/line counters are active.
2. Resolve the low IFU PCX loop (`000122..000141`, `000260..000277`) against
   `Lisp.syms!4`/sysout symbols to identify which Lisp code is running after
   the banner.
3. Keep using `/M` for Lyric probes. Plain `lisp.run {DORADO}LISP.SYSOUT` is
   useful only as a negative control for the skipped-`LoadRam` bug.

## ===> ACTIVE TASK (2026-06-26, latest): diagnostics are green with the
## diagnostic-specific harness commands.
## Map: [`docs/running-diagnostics.md`](running-diagnostics.md);
## handoff: [`docs/HANDOFF-hold-and-diagnostics.md`](HANDOFF-hold-and-diagnostics.md).

PARC's original **Dorado hardware diagnostics** run on our microengine
(`build/rundiag`) and now provide real regression gates. Current verified passes:
kernel, eventCounters, memMisc, IfuSimple, IfuComplex, TriconD no-pack, and the
memA D/X/S slices listed in [`docs/running-diagnostics.md`](running-diagnostics.md).

The old "implement Hold to make every diagnostic pass" framing is obsolete. The
diagnostics needed separate fixes in tasking/timing, memory/fault/Pipe status,
IFU/event-counter behavior, and Trident disk-controller mufflers. The remaining
caveat is harness scope: memA's complete S-board/chaos burn-in is not a quick
gate, and TriconD's no-pack success point is `TESTOK-WITHOUT-DISK`, not `DONE`.

`make` builds `rundiag`; header deps are tracked. The ethernet/render
investigation below is separate from the hardware-diagnostic gates.

---

## ===> 2026-06-23 (earlier): tooling built + the crash seeds REFRAMED

Picking up the "most games crash" investigation, this session built the Phase 0
tooling and used it to characterize the crashes — which **changed the diagnosis**.

**What's now true (all on branch `fidelity-timing`, all gates green):**

1. **Phase 0 tooling is DONE.**
   - **Machine snapshot/restore** (`dorado_machine_snapshot`/`_restore`, `machine.c`):
     boot a game once, snapshot the running game, restore into a fresh machine —
     so timing experiments skip the fragile boot. Bit-identical validated by
     `tests/test_snapshot.c` (in `make test`). Plus `dorado_machine_state_digest()`.
   - **`tools/nova-trace-diff/tracepcdiff.sh` repaired** — diffs the executed Alto
     opcode stream (PC + ACs) vs ContrAlto, auto-aligning the boot-phase slip.
     Run: `tracepcdiff.sh 5000 ../../chm/bootfiles/Invaders.boot!1` (or `AC_PERM=skip`
     for a clean PC-only diff).
   - Two latent bugs fixed en route: the vendored 6502's register file lived in
     file-scope globals (now mirrored into `bb->cpu6502` with an owner-cache);
     `baseboard_active` wasn't repointed per run.

2. **First divergence found and FIXED (cold-Alto init on the ether path).** The
   ether games inherited the AEmu's leftover Stack ACs where ContrAlto cold-boots
   clean 0. Extended the salto-verified cold-AC/IO-page init from DiskBoot (0o2005)
   to EBoot (0o2006). Grounded, regression-safe (Galaxian still 121553).

3. **THE REFRAME — the remaining crash seeds are concrete bugs, not cadence.**
   The prior "most games crash from cumulative *timing* divergence" is only partly
   right. Measuring the seed per game:
   - **MissileCommand's M[3016] oscillation is network-specific** — Invaders writes
     M[3016] once (=0), matching ContrAlto. So M[3016] = the **ethernet spurious
     InDone/OutDone completions**, not display/scheduler cadence. Neither the
     scanline cadence nor a one-field delay on the first field interrupt moved it.
   - **Invaders' seed is an early per-opcode AC divergence** — a clean PC-only
     `tracepcdiff` shows the ACs diverge by **~opcode #2** (ours loads `6126/6373`
     into AC2/AC3 where ContrAlto has `0/1`), past what the trace's one-opcode AC
     lag explains. A per-opcode emulation or early Alto-memory-state bug.
   - Cadence work (scanline cadence cuts MC's M[3016] oscillation ~20%, knob
     `DORADO_SCANLINE_CYCLES`) is real but **secondary**.

**Read first:** [`docs/cycle-accurate-timing-plan.md`](cycle-accurate-timing-plan.md)
— the "Phase 0/1/2 status" + "Phase 2 REFRAME" sections at the top carry the full
detail and the per-game next targets. The original holistic-cadence plan + the
two ruled-out experiments are still in [`docs/fidelity-audit.md`](fidelity-audit.md).

## ===> UPDATE (2026-06-23, latest): Invaders "double-dispatch" was a 3rd tool
artifact -- Invaders is now IR-IDENTICAL to ContrAlto for 2091 opcodes

Chased the "Invaders double-dispatches an early opcode" lead. It is **NOT an
emulation bug** -- it is the AEmu **Reschedule trap** (`AEmuReschedule`), a
Dorado mechanism a plain Alto (ContrAlto) has no equivalent of, and the
`tracepcdiff` tool was **over-counting the trapped IFUJump** as an executed
opcode.

What actually happens at boot: `Reschedule` (FF function, microcode 0o1770)
sets `reschedule_pending=2`. The trap fires on the 2nd successful IFUJump,
which **diverts that dispatch to the reschedule vector (0o314) instead of the
opcode's handler** -- so the "held-back" opcode does NOT execute there; it is
re-dispatched (and executed exactly once) on the next IFUJump after
`AEmuReschedule` restores PCF via `T<-NOT(PCX')`. The IFUDISP trace fired on
the trapped IFUJump too, so the tool saw IR `0o100000` "twice". Skipping the
trapped record, ours' IR stream is `0o22574, 0o100000, 0o40437, ...` -- exactly
ContrAlto's.

**Fixes (this session, gates green):**
- `cpu.c` IFUDISP trace now prints `rtrap=%d` (1 = this IFUJump traps to
  AEmuReschedule; the dispatched opcode is re-run next, not executed here).
- `tracepcdiff.sh` skips `rtrap=1` records (they are not executed opcodes).
- `tracepcdiff.sh` AC bug fixed: ContrAlto emits `acs=R[3],R[2],R[1],R[0]`, and
  the Alto maps ACs to R in reverse (AC0=R[3]..AC3=R[0]), so the printed list is
  ALREADY AC0..3 -- the tool's old `[::-1]` reversal compared ours and CA in
  opposite AC orders. Removed. AC check is now lag-tolerant and **advisory**
  (default `AC_PERM=skip`, IR-only); exact AC equality is unreliable because (a)
  ours' aacs snapshot lags CA's by ~1 opcode and (b) ACs holding PC-relative
  addresses differ by the boot-phase namespace slip. The **IR stream is the
  trustworthy divergence signal**.

**Result:** `tracepcdiff.sh 5000 chm/bootfiles/Invaders.boot!1` now reports the
IR streams match for **2091 contiguous opcodes**, then diverge at #2091:

```
2089: ours/CA  IR=0o20655   (LDA 0,@255,2  -- load AC0 from a pointer)
2090: ours/CA  IR=0o101015  (MOV# 0,0,SNR  -- skip if AC0 != 0)
2091: ours IR=0o106415  |  CA IR=0o776 (JMP .-2)  <-- ours exits the loop, CA spins
```

This is a **3-instruction spin-wait loop** (`723: LDA 0,0o255(PC)` / `MOV#0,0,SNR`
/ `JMP .-2`) polling Alto location **`0o600` = EPLOC** (the standard Alto Ethernet
**Post Location**). The game/loader transmitted a Pup and is waiting for the
Ethernet to post a completion. ours' EOT task (task 6, microcode `EPOST`) posts
**OutDone (`0o777`)** to EPLOC ~32us after the game arms the wait; ContrAlto
spins **~4.1ms** (733 iterations) before OutDone. So **the divergence is
Ethernet, not "non-network"** -- the prior framing was wrong.

**Root cause (chased this session, NOT yet fixed):** ours **completes transmits
instantly**. `eth_tx_packet_done` (`src/ethernet.c`) fires synchronously the
moment EOT sets TxEOP -- no wire time and, crucially, **no transmitter deferral
while the receiver is busy**. At the transmit point `rx_on=1` and a **9896-word
receive is in progress** (`ETH_WAKE rx=.../9896`); a real 3 Mb/s controller
cannot transmit until the wire is free, so OutDone is delayed milliseconds.
Per-word wire time alone (~70us for the 13-word packet at 5.4us/word) is far
short of CA's 4.1ms -- the gap is the **tx-defer-while-receiving** wire
interaction. This is the same root as MissileCommand (see
[`mc-bug-is-emulator-not-ethernet`] and [`docs/ethernet-faithful-receiver.md`],
which Invaders now confirms is a game-blocker, not just a fidelity nicety).

**Why no patch shipped:** a simple per-word tx wire-time delay (~70us) does NOT
match CA (~4.1ms) and would not release the loop at the right time; the correct
fix is transmitter deferral during active receive + wire timing, which lives in
the faithful-receiver/wire-model work and risks desyncing the EFTP boot (the
loader alternates tx/rx and relies on the current instant-completion). That is
an architectural change to make deliberately, gated hard on Galaxian + boot, not
a quick incremental patch. **Next:** build the faithful tx/rx wire model
(`docs/ethernet-faithful-receiver.md`) so EOT's OutDone post is gated on the
wire being free; validate Invaders' spin then matches CA's ~733 iterations.

---

## ROUTE B (2026-06-20): germ-netboot/DoInLoad WORKS — full boot-file transfer + loaded world runs

The old "frontier is EFTP seq 1 / the CompactVM allocator" notes below are
**superseded**. With the matched payload `chm/cedar/cedar6.1/BasicCedarDorado.boot!22`
the germ now netboots end-to-end at the mechanism level:

- The **entire** boot file transfers over the in-process EFTP server: all 1062
  packets, `pos=271616/271616`, EFTP state advances to completion. The germ
  posts a `RecvPacket` for every sequence (the "no second QueueInput" stall is
  gone -- enabled by the IFUJump-StkP / Carry20 / dirty-victim / disk fixes
  since 2026-06-18).
- `DoInLoad` accepts the header (`germInLoad` 001452/4/6, no `germBadBootFile`),
  copies the pages, and **starts the loaded world**: task 0 then runs Mesa
  bytecodes (XFEREXIT / XFEREXITDISPATCH / MESAIFUNOTREADY dispatch machinery,
  thousands of dispatches/window) through 1.4B cycles.

Run it:
```
./build/dorado --no-alto-boot --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --eftp '../chm/cedar/cedar6.1/BasicCedarDorado.boot!22' --germ-netboot-bfn 0
```

**New frontier = boot *context*, not the payload.** Decisive test: the PDI's own
`bootFile` (fileID=3, `pdidump --extract 3`) is **byte-identical** to
`BasicCedarDorado.boot!22` (header `063000 141530...`, 271360 words). That exact
boot file **renders the Cedar login when booted from the PDI (disk path, 28k px)
but not when netbooted** -- netbooted it runs Mesa bytecodes yet installs no
display list (`DASTART=0`). So the render gap is the **physical-volume boot
context** the disk path establishes (volume/MDS state Cedar needs to paint, even
under "No System Volume"), which the Ethernet `DoInLoad` path doesn't set up --
not a payload problem. (`OthelloDorado.boot!8` -> `germBadBootFile`,
`CedarDorado.boot!59` -> not served: those are separately version-mismatched,
header `062400` vs `063000`.) The netboot transport + DoInLoad are solid; the
remaining work is reconciling the net-boot path's post-load VM/volume context
with the disk path's so the loaded Cedar paints.



## 2026-06-20 (latest): Mesa Net Executive reaches an interactive prompt — NOT hung

Investigated "Mesa Net Executive never gets to a prompt." It is **not hung**:
it reaches a working interactive `>` prompt and echoes typed keystrokes. A 400M
headless run renders the full herald

```
--- XEROX Mesa Net Executive 8.0 -------------- Saturday 20-Jun-98  1:24:58 pm ---
------------------------------ Dorado [Dorado] 1#42# ------------------------------
>
```

with a **live clock** (advances 1:24 -> 1:27 across the run) and a blinking
cursor. `--type "abc" --type-at 160000000` echoes `> abc` at the prompt (3 keys,
1526 -> 1572 px), so keyboard input reaches it via the normal Alto keyboard
words (177034). The pc=0o2167 "spin" found earlier (busy-reading M[164747]=6) is
the **normal keyboard-input idle loop** at the prompt, not a deadlock — the
prompt simply appears late (~155M cycles), so in the browser it takes a while to
show up and the small `>` in the lower-left is easy to miss. No code change
needed; Path C is interactive. Repro:

```
./build/dorado --eb '../chm/dorado/AltoMesaDorado.eb!2' \
  --eftp '../chm/bootfiles/MesaNetExec.boot!1' \
  --type "abc" --type-at 160000000 --key-hold 4000000 \
  --cycles 200000000 --out /tmp/mne-type.pgm
```

## 2026-06-20 (later): Mesa text "solid block" font bug FIXED (shifter A-bus)

The Mesa Network Executive (and any Mesa BitBlt/scan-converter text) rendered
glyphs as solid filled rectangles. Root cause: a real microengine bug in
`ff_a_override()` (cpu.c). On a barrel shift, ASEL=7 puts ~(cycled word) on the
A bus, but `ff_a_override` also ran and, for a ShC-controlled shift whose FF
decoded as FA=0 FB=2 FC=3 (`FF=023`), returned "A<-Q", clobbering the cycled
glyph word with Q(=0). The ALU then did `(NOT 0) OR B = 0177777` and the
shifter edge-mask turned that into a solid `width`-bit run -> every glyph a
filled box. Fix: `ff_a_override` returns 0 for ASEL=7 (the shifter owns the A
bus). Herald now reads "XEROX Mesa Net Executive 8.0 ..." (1536 px vs ~4060
solid); Galaxian 121553, Cedar login+keyboard, 11/11 tests unchanged. Found by
tracing the herald draw to a tight microcode loop (a glyph bit 0100000 ->
0177400 at the store; shift alu=0177777 with a=0) and cross-checking
DMesaMiscOps.mc / BitBlt.mc (CHM) + PrincOps BitBltTable. This likely helps any
Mesa world's text (the Mesa games Chat/Fly/PPong/... are worth re-checking).

## 2026-06-20: more boot files cached + a second OS pair (Mesa NetExec) boots

Pulled the rest of the CHM `Io/Murray/` `.boot` collection into
`chm/bootfiles/` and the WebAssembly bundle. Of the new files, four are Alto
B-format (run on `worlds/aemu.eb`): **Neptune** (the Alto disk tool — renders a
real "Neptune Initialization Parameters" screen, ~14k px), **What** (~301 px),
and **Johnsson** / **Kal** (load at 32M but paint nothing by 160M, like
several existing games that need keyboard input). Six are Mesa-format
(`0o345`): Chat, Fly, Murray, PPong, Pupwatch, TriEx — they need a Mesa world,
not AEmu, so they are skipped by the Alto NetExec auto-menu. (The DLion/Star
`.boot` files in Murray were left out — different machine.)

**Non-Cedar OS pairs tried:**
- **`AltoMesaDorado.eb!2` + `MesaNetExec.boot!1` WORKS** — the Mesa Network
  Executive (a Mesa/Pilot environment, Cedar's sibling) boots over EFTP,
  installs an Alto-style display list (`DASTART` nonzero) and renders its
  herald + cursor (~3990 px). Now a web boot option (`dorado_web_boot_mesa`,
  "Mesa Network Executive" in the dropdown).
- `AltoMesaDorado.eb!2` + `NEWOS.BOOT` (a Pilot OS): 0 px — like Cedar it wants
  a germ / physical-volume boot, not an EFTP outload.
- `SmalltalkDorado.eb!1`: 0 px — needs a Smalltalk image (none in the archive).
- `AltoMesaDorado.eb!2` + Mesa games (e.g. Chat): load but paint nothing by
  200M — probably need input / to be launched from the Mesa NetExec.

So there are now **three** boot stories: Alto (AEmu + games, working), Mesa
(AltoMesaDorado + MesaNetExec, herald), and Cedar (CedarDorado + germ + PDI,
login prompt). The WASM bundle grew to ~4 MB. Next: make the Mesa/Cedar worlds
interactive (verify keyboard reaches MesaNetExec; get Cedar past login), and
the Pilot-OS pairs (NEWOS) need the disk/germ path, not EFTP.

## ROUTE B (2026-06-19 LATEST): Cedar 6.1 boots to the login prompt and accepts keyboard input; emulator sped up 2.7x

**Cedar 6.1.0 now boots all the way to its SimpleTerminal login prompt and is
interactive.** The headless Cedar run (the known-good command below, ~640M
cycles to the prompt) renders the SimpleTerminal herald + login:

```
Cedar 6.1.0 of December 3, 1986 ...     Friday, June 19, ...    No System Volume on Do[rado]
Please login ...
Name:
```

Typing now works end to end: `--type "Guest\n"` echoes `Guest` (uppercase via
shift), Return submits, and Cedar advances to `Name: Guest.pa  password:`.

### What was wrong with the keyboard, and the fix

Two independent bugs (NOT a key-layout bug — the layout is correct, see below):

1. **Key state never reached VM.** The keyboard delivery code
   (`machine_seed_keyboard` -> `177034`) is gated on `m->alto_ether_boot`, so
   for the native Cedar path (`--no-alto-boot`) nothing ever wrote the key
   cells. Holding a key and dumping `M[0o177033..0o177041]` showed it stuck at
   all-up `0o177777`. So there was no transition for the OS to see -> no
   characters at all (exactly: not *wrong* characters, *no* characters).

2. **The keyboard watcher never polled.** Cedar's
   `SimpleTerminalImpl.ProcessKeyboard` blocks on the display vertical-field
   retrace naked-notify before each `GetKeys`. On real hardware the field
   interrupt ORs `CSB.wakeupMask` (`LONG[421B]`) into NWW. Our display is
   rasterized in C (not by the display microcode tasks), so that notify never
   fired.

Fix (all in `dorado/src/machine.c`, new `machine_cedar_io()` called per step):
- Once per display field (~60 Hz, `CEDAR_FIELD_INTERVAL_CYCLES`), seed the
  live keyboard/mouse into the Cedar KeyBits at absolute `LONG[177033B]`
  (`machine_seed_cedar_keyboard`), then post the field naked-notify by ORing
  `CSB.wakeupMask` (`M[0o421]`) into NWW (`RM[0]`) and setting
  `reschedule_pending` -- the same idiom the modeled Ethernet/disk completions
  and `machine_pilot_timer_channel` use.
- This mirrors how `machine_seed_keyboard` already shortcuts the (unmodeled)
  7-wire keyboard back channel for the Alto-on-Dorado world at `177034`.

### Keyboard layout is PROVEN correct (TerminalDefs.KeyName, Cedar 6.1)

The Dorado used Alto keyboards. Pulled `TerminalDefs.mesa!1` from
`[Cyan]<Cedar6.1>Heads>`: `KeyBits = PACKED ARRAY KeyName OF DownUp`
(down=0, up=1 -> all-up = `0177777`), and `KeyName` VAL 16..79 are *exactly*
the four Alto keyboard words in the same bit order as our Alto matrix
(`Five,Four,Six,E,Seven,D,...`). So Cedar's KeyBits words land at
`177034..177037` taking our active-low `keyboard_words[0..3]` verbatim; word
`177033` (VAL 0..15) is Pen/Keyset + mouse buttons (Red=left=bit2,
Blue=right=bit1, Yellow=middle=bit0). Hardware basis: HM p.116 Table 24
(back-channel message types `01..04` = keyboard words 0-3) and
`TerminalHeadDorado.mesa` (`keyboard _ LOOPHOLE[LONG[177033B]]`,
`csbPtr = LOOPHOLE[LONG[420B]]`, `csbPtr.wakeupMask |= wakeVF`).

The SDL frontend already feeds `dorado_machine_set_key`, so interactive
`make run-cedar` typing now works too.

### Emulator is 2.7x faster (per-step getenv was the bottleneck)

`getenv()` rescans the whole process environment on every call; the hot path
ran several boolean trace-flag `getenv()`s per microinstruction. Under a
populated shell that was ~2.4-2.7x of total run time (empty-env A/B: 9.6s vs
22.8s for 200M cycles). Added a cached `dorado_trace_flag()` (keyed by the
string-literal pointer; `src/cpu.c`, declared in `include/cpu.h`) and routed
all boolean `DORADO_*_TRACE` / flag checks in `cpu.c`, `machine.c`, `memory.c`
through it (value-returning `getenv()` calls -- VMDUMP, POKE, TRACE_GATE
window, etc. -- left direct). Result: 200M-cycle Cedar run 22.8s -> 8.4s
(**~23.9M cycles/sec, ~1.43x real-time**; the real Dorado is 16.67 MIPS).
Cedar boot-to-login now ~27s instead of ~73s. All unit tests still pass.

### Alto Path A regression -- FIXED (was: 0px)

Path A (Alto-on-Dorado: Galaxian/NetExec/games) had regressed to 0 display-
list pixels in the dirty tree. Bisected against the last clean commit
(`3154d9c`) via a worktree: with HEAD's cpu/memory the Alto rendered fine, so
the microengine was clean; adding HEAD's `ethernet.c` reproduced the 0px.
Root cause: `eth_write` (RxOn control) was changed to call `eth_clear_rx()`
whenever the world clears RxOn (modeling "HM §11 clearing RxOn resets the
receiver"). The Alto EtherBoot loader toggles RxOn off/on between EFTP
packets while the in-process fake server holds the next lock-step packet on
the wire; discarding it there dropped the packet the Alto was about to read,
stalling the boot at ~seq 7 of the EFTP stream (`DASTART=0`, 0px). The base
never cleared rx on RxOn-off. Fix: gate that `eth_clear_rx()` to the
Cedar/Pilot germ path (`eftp_wait_for_rx_arm`), whose IOCB-gated delivery
re-arms per input buffer and needs it; the Alto path keeps the held packet,
matching pre-regression behavior. Galaxian back to 121553 px, NetExec ~1463,
Cedar unaffected. The fix is one line in `dorado/src/ethernet.c`.

### Verify

```sh
cd dorado
make build/dorado && make test           # 11/11 suites pass
# Cedar login + typing self-test (writes /tmp/cedar-type.pgm with "abc" echoed):
./build/dorado --boot-reason disk --no-alto-boot \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --pilot-disk '../CedarDisk/CedarDorado-boot.pdi' \
  --type "abc" --type-at 655000000 --key-hold 4000000 \
  --cycles 700000000 --out /tmp/cedar-type.pgm
# Interactive: make run-cedar  (type at the Name: prompt)
```

`dorado.c` gained `--type-at CYCLES` (default 110M for the Alto self-test;
pass ~655M for the Cedar login prompt) and now triggers `--type` on
`booted` rather than `interactive`.

---

## ROUTE B (2026-06-19 earlier): direct Pilot disk boot reaches `germFinished`; next blocker was post-handoff Pilot/Cedar init

The active boot path is now the real Pilot physical-volume path using the
local PDI image:

```sh
cd dorado
./build/dorado --cycles 800000000 \
  --boot-reason disk \
  --no-alto-boot \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --pilot-disk '../CedarDisk/CedarDorado-boot.pdi'
```

Current result:

- The Cedar/Pilot PDI mounts as disk drive 0 via `--pilot-disk`.
- `DiskHeadDorado` sees the PDI drive as an online/read-only T-80-style
  SA4000 volume through DMux `02000..02037`.
- The germ reads the physical-volume root, finds `bootingInfo[pilot]`
  (`FileID=3`, first link `0o167`), streams the Pilot boot file from the PDI,
  and reaches `germFinished`.
- The direct PDI bridge now updates the caller's label buffer on every page
  in a multi-page transfer, so after a read it contains the final sector
  label as the real controller would leave it. This is required for
  label-chain/terminator consumers, though it does not change the current
  post-handoff blocker because this boot file is one contiguous run.
- 800M-cycle confirmation run:

  ```text
  PDI disk IOCB stream reaches nextPage=0o2233
  SETMP cyc=362949559 code=001456  germFinished
  SETMP cyc=362957125 code=000017  post-handoff Pilot/Cedar code
  SETMP cyc=515355406 code=000000
  SETMP cyc=582723665 code=000377  current error/status point
  booted=1 cyc=800000004 pc=0o3445 tk=0
  MDS=0o400000
  DASTART=000000
  ```

The current blocker is therefore **after** the germ has loaded the Pilot boot
file and transferred into the loaded world. There is still no display list by
800M cycles. At the repeatable error/status point, detailed `SetMP` tracing
shows:

```text
SETMP_DISP cyc=582723665 pc=0o4654 pcf=0o3243 br31=3C614 stkp=001 code=000377
SETMP_DETAIL cyc=582723665 MDS=0o0 MemBase=36 RBase=00 ... BR30=0o400000 BR36=0o0
```

The final micro-PC is `0o3445`, which disassembles in `Cedar.mb!6` as the
Mesa fault/trap path (`MESAFAULT`). The next blocker is to identify why the
post-handoff world calls `SetMP[0o377]` and enters that trap path before
installing a display list. Avoid broad `DORADO_BR_TRACE`; it floods output.
Use gated, targeted traces (`DORADO_TRACE_GATE=582720000,582725000`) or add
more focused diagnostics around `SetMP[0o377]`, MDS/BR36, and the Mesa return
frame.

Implemented in this pass:

- `dorado/src/cpu.c` now implements `Pd <- ALU rsh 1` as the logical shift
  documented by the hardware manual. It was incorrectly preserving the sign
  bit; MesaInterrupt's `WDC _ (WDC) RSH 1` scan then got stuck at `177777`
  instead of walking `100000, 040000, ... 000001, 000000`.
- `dorado/src/memory.c` now lets device code handle diagnostic DMux reads.
  `dorado/src/disk.c` wires the disk controller's muffler manifold at
  DMux `02000..02037`, which is what `DiskHeadDorado.RWMufMan` uses during
  controller/drive discovery. Before this, those reads always returned zero.
- `dorado/src/disk.c` now has a PDI-backed media path for Pilot pages
  (`header + 10-word label + 256-word data`) and reports the right DMux
  bits for the PDI drive.
- `dorado/src/machine.c` mounts `--pilot-disk` and completes PDI-backed
  SA4000 IOCBs posted through DiskHeadDorado's CSB at `LONG[177520B]`.
  This is a narrow bridge over the still-incomplete disk sequence-PROM path,
  not a replacement for a full hardware disk controller.
- Diagnostic fault and `SetMP` traces now honor `DORADO_TRACE_GATE` better;
  `DORADO_SETMP_TRACE_DETAIL=1` adds a targeted stack/base-register dump.

Focused tests:

```sh
cd dorado
make build/dorado build/test_disk build/test_pdi
./build/test_disk
./build/test_pdi
```

## ROUTE B (2026-06-19 previous): BasicCedar Ethernet shortcut loads and runs; blocker was a `pilotOutLoad` request

Current result after both fixes:

- EFTP delivers the full `BasicCedarDorado.boot!22` payload
  (`seq=1061`, `pos=271616/271616`, `eftp_q=1062`).
- The germ reaches `germFinished` (`SETMP code=001456`, decimal 814) and
  transfers through `pMon.CrossMDSCall` into BasicCedar.
- BasicCedar now gets past the earlier FilePackage-only idle point and reaches
  additional OS code ranges, then re-enters the germ:

  ```text
  cyc=175599964 br31=3E1E10 code=001456  germFinished
  cyc=175608427 br31=34C14  pcf=0o105   DiskImpl-ish BasicCedar code
  cyc=340826276 br31=38020  pcf=0o6665  VMImpl code range
  cyc=414017270 br31=3C614  pcf=0o3243  CedarRuntime code range
  cyc=414513138 br31=44D1C  pcf=0o225   FilePackage code range
  cyc=414672442 br31=32500  pcf=0o70    additional BasicCedar module
  cyc=414681902 br31=3470C  pcf=0o2413  additional BasicCedar module
  cyc=414691318 br31=3E1E10 code=001452  germStarting
  cyc=414757935 br31=3E0F38 code=000001
  ```

- Final 500M-cycle state from the confirmation run:

  ```text
  booted=1 cyc=500000000 pc=0o1716 tk=0
  eth: rx=1 tx=0 req=1 repl=63 eftp_r=1 eftp_q=1062 seq=1061
  DASTART=000000
  MDS=0o17400000
  ```

The live blocker is no longer BootChannelEther or EFTP delivery. A final
`pRequest` dump after the OS-side return shows action `3`. In the 6.1 germ
source, `BootSwapGerm.mesa` handles `outLoad, pilotOutLoad` together and
`teledebug` separately, so action `3` is the Cedar/Pilot `pilotOutLoad` path,
not the old 1980 `Boot.teledebug` value from `chm/cedar/pilot/Boot.mesa!1`.
The loaded BasicCedar world is asking the germ to write an outload/checkpoint,
with no installed Pilot/Cedar disk volume available.

Runtime store trace pins the writer:

```sh
DORADO_TRACE_GATE=414500000,414760000 \
DORADO_STORE_TRACE_VA=017401360,017401400 DORADO_SETMP_TRACE=1 \
./build/dorado --cycles 414760000 \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --eftp '../chm/cedar/cedar6.1/BasicCedarDorado.boot!22' \
  --germ-netboot-bfn 0
```

The decisive line is:

```text
STORE_VA cyc=414689598 task=0 pc=0o626 va=17401360 data=000003 pcx=0o2440 br31=0o643414 op=0o177400
```

`BasicCedarDorado.loadmap!69.txt` maps `0o643000..0o645377` to the
`Tentacles` code pages, whose modules include `GermSwapImpl`. The local CHM
tree does not appear to include the BasicCedar `GermSwapImpl` Mesa source, so
the next pass should continue from runtime traces around this writer rather
than source-grep alone.

Next step: find why BasicCedar requests `pilotOutLoad` instead of reaching the
terminal display path. Likely areas are disk/physical-volume discovery and the
absence of a bootable local volume. The code path to inspect is OS-side, not
the germ EFTP reader: `BasicCedarDorado.loadmap!69.txt`, `DiskImpl`,
`FilePackage`, and callers that set `GermSwap.pRequest.action` to
`pilotOutLoad`.

Focused tests pass:

  ```sh
  cd dorado
  make build/dorado build/test_cpu build/test_disk
  ./build/test_cpu
  ./build/test_disk
  ```

Important caveat: `machine_germ_seed_ethernet_header_page()` is still a
diagnostic shim for the first BootFile header page. After that header, the
real BootChannelEther/EFTP path transfers the remaining pages.

## ROUTE B (2026-06-19 late): direct germ boot reaches BootChannelEther; current blocker is the first real `EFTPGetClump` frame allocation

This supersedes the "post-header CompactVM" wording below. `germInLoad`
proves `DoInLoad` was entered; it does **not** by itself prove the BootFile
header was consumed. The current failure happens before the next Ethernet
receive is posted, inside the first `Transfer[channel, pageBuffer, 1]` /
`BootChannelEther.EFTPGetClump` path.

What is now known:

- First allocator failure is at cycle `98393647`, not only in the later 180M
  loop. The tight loop is:

  ```text
  IFUDISP pc=0o234 pcf=0o4601 ... op=165 ... stkp=003 acs=000000,000000,000003,177200
  IFUDISP pc=0o354 pcf=0o4602 ... op=162 ... stkp=003 acs=000000,000000,000003,177200
  ALLOC_TRACE cyc=98393647 pc=0o3302 md=0 mar=0o200000 va=0o400000 mb=0o3 br=0o200000 br36=0o17400000
  ```

- `BR[36]=0o17400000` is the germ MDS. The failing allocation reads
  `BR[3]+MAR = 0o200000+0o200000 = 0o400000`, the incoming
  BasicCedar MDS base (`BasicCedarDorado.loadmap`: MDS page `0o1000`).
  `DORADO_VMDUMP` confirms `0o400000..0o400057` and `0o402000..0o402037`
  are all zero at the failure.
- This is not yet evidence that `CompactVM` is running. The immediate source
  path is `BootChannelEther.mesa!3`: `EFTPGetClump` first computes
  `where _ PrincOpsUtils.AddressForPageNumber[page]`, then calls
  `MiniEthernetDefs.RecvPacket[...]`. Either call can need a local frame
  before a seq-1 IOCB is queued.
- Use the 6.1 germ loadmap, `chm/cedar/os-src/Dorado.loadmap!6.txt`, for the
  matched `Dorado.germ-6.1.6` global frame offsets. The older
  `chm/cedar/germ/Dorado.loadmap!1.txt` has different frame addresses.
  Current live frames at the failure include:

  ```text
  BootChannelDisk G=0o6160: codebase low/high = 0o10544/0o76
  BootChannelEther G=0o6234: codebase low/high = 0o11410/0o76
  BootChannelEther state: mySocket=0o7176/0o13, buffer=0o5504/0o76,
    iocb=0o177220 or 0o177200-family, receiveSeqNumber has advanced after ACK 0
  ```

- `BasicCedarDorado.boot!22` header entries begin at VM pages
  `0o400..0o416`, then `0o1001..`; page `0o1000` (VA `0o400000`) is not in
  that first header entry run. The zero AV page is therefore expected until
  the incoming world is actually initialized/loaded; the real question is why
  this pre-transfer call is allocating from the incoming MDS.

Repro for the current blocker:

```sh
cd dorado
DORADO_TRACE_GATE=98392800,98393680 \
DORADO_IFUDISP_TRACE=1 DORADO_ALLOC_TRACE=1 DORADO_ALLOC_TRACE_LIMIT=24 \
DORADO_SETMP_TRACE=1 DORADO_CSB_TRACE=1 \
./build/dorado --cycles 98393700 \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --eftp '../chm/cedar/cedar6.1/BasicCedarDorado.boot!22' \
  --germ-netboot-bfn 0
```

Useful dumps:

```sh
DORADO_VMDUMP="0400000,0400060,98393600" ./build/dorado ...
DORADO_VMDUMP="0402000,0402040,98393600" ./build/dorado ...
DORADO_VMDUMP="017405400,017406340,98393600" ./build/dorado ...
```

Next step: identify exactly which control link at byte PC `0o4601/0o4602`
is being called and why its allocation uses incoming-MDS AV (`BR[3] =
0o200000`) instead of a germ-local frame list. The likely suspects are the
first `EFTPGetClump` calls to `PrincOpsUtils.AddressForPageNumber` or
`MiniEthernetDefs.RecvPacket`, not the later `CompactVM` loop.

## ROUTE B (2026-06-19): direct germ boot now gets past the BootFile header; current frontier is allocator/map state after `CompactVM`

Use this as the current Route B state, superseding the CedarNetExec framing in
the older entries below.

Current best direct-germ target:

```sh
cd dorado
DORADO_GERM_NETBOOT_TRACE=1 DORADO_SETMP_TRACE=1 DORADO_EFTP_TRACE=1 \
./build/dorado --cycles 220000000 \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --eftp '../chm/cedar/cedar6.1/BasicCedarDorado.boot!22' \
  --germ-netboot-bfn 0
```

Key corrections:

- `CedarNetExec.boot!4` is the wrong payload for direct `BootSwapGerm.DoInLoad`.
  It is a raw Mesa outload (`word0=0o345`) for the Alto/Mesa NetExec chain, not
  a Pilot `BootFile.Header`. Direct germ boot correctly reaches
  `germInLoad` (`SETMP code=001454`) and then reports `germBadBootFile`
  (`001467`) when served `CedarNetExec.boot!4`.
- `BasicCedarDorado.boot!22` is the current direct BootFile target. Its first
  header words are `000146 054303 120636 174400 ...`; with the matched
  `CedarDorado.eb!6` + `Dorado.germ-6.1.6` pair, the germ accepts the header.
  In a 220M-cycle run there is no `germBadBootFile`.
- The seq-1 Ethernet delivery to IOCB `0o177777` was an emulator shim bug.
  `EthernetOneHeadDorado` allocates IOCBs from the germ's
  `dFirst64KStorage = [0o177200..0o177400)`, so the CSB direct-completion shim
  now only arms/completes plausible IOCBs in `0o177200..0o177377`.

Observed good path with `BasicCedarDorado.boot!22`:

```text
EFTP_DEFER state=1 seq=0 pos=0/271616 armed=0
EFTP_DELIVER type=0o30 state=1 seq=0 data_words=256 packet_words=271 pos=0/271616 armed=1
EFTP_ACK_IN ack=0 state=1 seq=0 pos=0/271616
EFTP_ACK_ADV state=1 seq=1 pos=256/271616
EFTP_DEFER state=1 seq=1 pos=256/271616 armed=0
[machine] germ seeded Ethernet header page @0o17422400 word0=0o146 word1=0o54303
SETMP_DISP ... code=001454
```

Important caveat: `machine_germ_seed_ethernet_header_page()` is still a
diagnostic shim. The real `Transfer[channel, pageBuffer, 1]` / `EFTPGetClump`
path has not yet been made to fill `pageBuffer`; the shim copies the first real
BootFile page into `pageBuffer` after `StartRecving` discards and ACKs seq 0.
This is useful because it proves the payload choice and moves the germ into the
post-header path, but it is not the final Ethernet transfer model.

New frontier:

- After header acceptance, `DoInLoad` takes the `load` branch and enters the
  `CompactVM[]` / map work in `BootSwapGerm.mesa`.
- No valid second `QueueInput` for EFTP seq 1 is posted through 220M cycles.
  That is no longer evidence of an Ethernet packet-format problem; the germ is
  busy in post-header VM/map/allocator code before it asks for more pages.
- Around 180M cycles task 0 is in a tight Mesa allocator microcode loop:

  ```text
  pc=0o3302 ALLOCREPEAT
  pc=0o1071
  pc=0o1072
  pc=0o3301 ALLOCSUB
  lva=0o200000 mb=0o3 T=0 Q=0 MD=0
  ```

  `mb=0o3` means MDS bank 3, so `lva=0o200000` is a bank-3 access at offset 0.
  The repeated `ALLOCSUB` / `ALLOCREPEAT` sequence is now the live blocker.

Focused late-loop trace:

```sh
cd dorado
DORADO_TRACE_GATE=179990000,180002000 \
DORADO_PCDIS=01070,01073 \
DORADO_FAULTREG_TRACE=1 DORADO_FAULT_TRACE=1 DORADO_TASK_TRACE=1 \
DORADO_XFER_TRACE=1 \
./build/dorado --cycles 180002000 \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --eftp '../chm/cedar/cedar6.1/BasicCedarDorado.boot!22' \
  --germ-netboot-bfn 0
```

Disassemble the loop PCs with:

```sh
cd dorado
./build/mbdis -d ../chm/dorado/Cedar.mb!6 |
  rg -C 4 '^  (1070|1071|1072|1073|3300|3301|3302|3303):'
```

Next step: determine why the `CompactVM` / allocator path is repeatedly trying
to allocate/fetch at bank-3 offset zero with zero MD. Start with map/fault state
around the first transition into `ALLOCSUB` rather than enabling broad
`DORADO_MAP_TRACE` over the full run, which is too noisy.

## ROUTE B (2026-06-18): BootChannelEther receives and ACKs EFTP seq 0; current frontier is return/control flow into `DoInLoad`

Current Cedar/Pilot pair remains:

```sh
./build/dorado \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --eftp '../chm/bootfiles/CedarNetExec.boot!4' \
  --germ-netboot-bfn 0
```

Latest correction:

- BootChannelDisk -> BootChannelEther is no longer the frontier.
- BootChannelEther's odd codebase swap/start path now runs and the module
  reaches real Ethernet traffic through `MiniEthernetDriver` and
  `EthernetOneHeadDorado`.
- The germ sends its Mayday (`244B`) from socket `[0o7176,0o13]`; the
  in-process server loads `CedarNetExec.boot!4` as 65,280 words and defers
  delivery until the germ posts an input IOCB at `LONG[177600B]`.
- The first EFTP Data packet lands in the germ's EthernetOne buffer and is
  ACKed. The server then correctly advances to Data seq 1
  (`pos=256/65280`) and waits for the next receive arm.
- The active frontier is now **after `BootChannelEther.StartRecving` ACKs the
  discarded seq-0 packet**. Per `chm/cedar/germ/BootChannelEther.mesa!3.txt`,
  `StartRecving` should return `handle _ EFTPGetClump`; then
  `BootSwapGerm.DoInLoad` should call `Transfer[channel, pageBuffer, 1]`, and
  `EFTPGetClump` should post another `RecvPacket` expecting seq 1. In the
  emulator no second `QueueInput` store to the IOCB appears through 200M
  cycles, so the next bug is control flow / return / Mesa state after
  `BootChannelEther.Create`, not EFTP packet sequencing.

Key evidence:

```text
[machine] germ netboot request seeded @cyc=67984795:
  pRequest=0o17401360 action=inLoad device=ethernet bfn=0o0

TX n=13: 000042 001000 000026 000244 ... 007176 000013 ...
EFTP_DEFER state=1 seq=0 pos=0/65280 armed=0
EFTP_DELIVER type=0o30 state=1 seq=0 data_words=256 packet_words=271 pos=0/65280 armed=1
[machine] germ direct RX candidate: ... iocb=0o177200 completion=0o0 length=0o454 buffer=0o17405446 direction=0o0
[machine] germ EthernetOne direct RX complete: iocb=0o177200 buffer=0o17405446 used=0o417 status=0o400
TX n=13: 021042 001000 000026 000031 ... 007176 000013 ...
EFTP_ACK_IN ack=0 state=1 seq=0 pos=0/65280
EFTP_ACK_ADV state=1 seq=1 pos=256/65280
EFTP_DEFER state=1 seq=1 pos=256/65280 armed=0
```

Long IOCB-store trace:

```text
STORE_VA cyc=68051790..68051824  ; QueueInput for seq-0 discard, length=0o454
STORE_VA cyc=68061735..68061773  ; QueueOutput for ACK 0, length=0o15
STORE_VA cyc=68062497 task=6 va=0177201 data=000400  ; EOT completes ACK output
STORE_VA cyc=68062501 task=6 va=0177202 data=000015
; no further stores to 0177200..0177210 through 200M cycles
```

Implemented support now in the emulator:

- `dorado/src/ethernet.c` gates germ EFTP delivery until the germ posts an
  EthernetOne input IOCB, uses the Mayday source socket as the EFTP destination
  socket, and traces EFTP state with `DORADO_EFTP_TRACE`.
- `dorado/src/machine.c` has a germ-only EthernetOne direct receive completion
  shim. It copies the packet into the posted IOCB buffer, writes
  `completion=0o400`, writes `used`, and pops `iCSB.next` to model the input
  microcode consuming the queue head. This is a controlled CSB-level model for
  the fake in-process boot server, not a replacement for raw EIT timing.
- `./build/test_ethernet` passes and confirms `CedarNetExec.boot!4` is served
  byte-exact as 65,280 words.

Latest useful traces:

```sh
env DORADO_ETH_TX_TRACE=1 DORADO_CSB_TRACE=1 DORADO_EFTP_TRACE=1 \
  ./build/dorado --eb ../chm/dorado/CedarDorado.eb!6 \
    --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
    --eftp ../chm/bootfiles/CedarNetExec.boot!4 \
    --germ-netboot-bfn 0 --cycles 82000000

env DORADO_STORE_TRACE_VA=0177200,0177210 \
  DORADO_EFTP_TRACE=1 DORADO_ETH_TX_TRACE=1 \
  ./build/dorado --eb ../chm/dorado/CedarDorado.eb!6 \
    --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
    --eftp ../chm/bootfiles/CedarNetExec.boot!4 \
    --germ-netboot-bfn 0 --cycles 200000000

env DORADO_TRACE_GATE=68062480,68065000 \
  DORADO_IFUDISP_TRACE=1 DORADO_RCLK_TRACE=1 DORADO_COND_TRACE=1 \
  ./build/dorado --eb ../chm/dorado/CedarDorado.eb!6 \
    --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
    --eftp ../chm/bootfiles/CedarNetExec.boot!4 \
    --germ-netboot-bfn 0 --cycles 68065050
```

Next step:

1. Trace the return from `BootChannelEther.StartRecving` / `Create` after
   ACK 0 and verify that `BootSwapGerm.DoInLoad` receives `handle =
   EFTPGetClump`.
2. Decode the `BootSwapGerm.DoInLoad` bytecode around the first
   `Transfer[channel, pageBuffer, 1]` call. The expected next observable event
   is another `QueueInput` store to IOCB `0o177200` with `length=0o400` or
   `0o454`, followed by EFTP Data seq 1.
3. Keep the separate timeout/compare suspicion in mind. Earlier traces showed
   `TimeoutHasExpired` / `DSUB` / `DCOMP` / `JGEB` can report expired even
   when the current clock appears less than the target. That is still a real
   CPU/Mesa arithmetic candidate, but the current post-ACK stall must first be
   localized in the Create -> DoInLoad return path.

## ROUTE B (2026-06-18): IFUJump saved-StkP timing fixed; BootChannelDisk.Create no longer drops the fifth argument

What changed:

- Fixed IFUJump's saved-stack-pointer timing in `src/cpu.c`. A successful
  IFUJump now defers `ifu_saved_stkp` capture until after that IFUJump
  instruction's own post-cycle RSTK update. `RestoreStkP` must restore the
  stack state handed to the dispatched opcode, not the pre-RSTK stack state.
- Added `ifu_save_stkp_pending` in `include/cpu.h`.
- Added `test_ifujump_saves_post_rstk_stkp` in `tests/test_cpu.c`.
- Implemented HM Table 11a `Carry20` (`FA=0 FB=2 FC=6`) in `src/cpu.c`.
  It forces the internal carry into the octal-`020` ALU bit for arithmetic
  ops. This is exactly what `NewSetBRAndFlushPage` uses in:
  `RTemp0_ Flush_ RTemp0, Carry20, Branch[., Cnt#0&-1]`.
- Added `test_carry20_ff_function` in `tests/test_cpu.c`.
- Fixed the dirty-victim fault path so a writeback fault records FaultInfo /
  Pipe4 MapTrouble and wakes the fault task; Pipe4 raw `notMapTrouble` polarity
  is now compatible with `DMesaFaults.mc`.
- Fetched the matching 1985/1986 `BootChannelDisk` source from the CHM mirror:
  `chm/cedar/germ/BootChannelDisk.mesa!1.cyan61`. Use
  `python3 tools/tioga2txt/tioga2txt.py` to read it. The local
  `BootChannelDisk.mesa!2.txt` is the older 1983 source and still imports
  `MicrocodeVersion`; it is not the right source for the 6.1 germ.

Root cause of the latest false frontier:

- BootSwapGerm calls BootChannelDisk.Create through EFC1 at byte PC `0o220`.
  The IFUJump instruction that commits the EFC1 dispatch has `RSTK=01` and
  changes `StkP` from `004` to `005`; the fifth active argument is
  `dFirst64KStorage.size = 0o200`.
- The old emulator saved `ifu_saved_stkp` inside `next_pc()`, before the
  IFUJump instruction's post-cycle RSTK update. Later `RestoreStkP` restored
  `004`, so `STATE` / `SaveState` wrote a malformed StateVector with
  `stkptr=4`.
- `LSTF` then faithfully restored only four active arguments. The fifth word
  (`0o200`) stayed above TOS, and BootChannelDisk.Create's prologue eventually
  underflowed at `SL0`. This made the chain-dispatch path look broken even
  though the caller state was already wrong.
- The focused trace now shows `RESTORE_STKP ... saved=005`, `LoadStack` loading
  the two extra words, then `Stack&-2` landing at `StkP=005`. BootChannelDisk
  consumes the Create arguments without the prior underflow.

Carry20 / dirty-victim note:

- `Map[0x3FFC]` was previously made vacant at cyc `67269620`, but the preceding
  flush loop repeatedly flushed only `17776000` because `Carry20` was a no-op.
  That left a dirty cached line at page offset `0o40`; later eviction produced a
  stale dirty-victim MapTrouble at cyc `67857790`.
- After `Carry20`, the focused trace shows the page flush walking all munches:
  `17776000`, `17776020`, `17776040`, ... `17776360` before the map write. The
  old dirty-victim `FAULT_TRACE cyc=67857790 ... victim_va=17776040` is gone.

Validation:

```sh
make && ./build/test_cpu
```

passed, including the new IFUJump saved-StkP regression. Earlier
`./build/test_memory` also passed after the dirty-victim/Pipe4 fix.

Focused Cedar traces:

```sh
env DORADO_TRACE_GATE=68029620,68029760 \
  DORADO_IFUDISP_TRACE=1 DORADO_STACK_TRACE=1 DORADO_STKP_TRACE=1 \
  ./build/dorado --eb ../chm/dorado/CedarDorado.eb!6 \
    --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
    --eftp ../chm/bootfiles/CedarNetExec.boot!4 \
    --germ-netboot-bfn 0 --cycles 68029800 2>&1 |
  rg "IFUDISP|STACK_TRACE|STKP_POST|SAVE_STKP|RESTORE_STKP"

env DORADO_TRACE_GATE=68032920,68033500 \
  DORADO_IFUDISP_TRACE=1 DORADO_STACK_TRACE=1 DORADO_STKP_TRACE=1 \
  ./build/dorado --eb ../chm/dorado/CedarDorado.eb!6 \
    --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
    --eftp ../chm/bootfiles/CedarNetExec.boot!4 \
    --germ-netboot-bfn 0 --cycles 68033500 2>&1 |
  rg "IFUDISP|STACK_TRACE|STKP_POST|SAVE_STKP|RESTORE_STKP|STK_ERROR_HOLD"
```

The first trace shows `RESTORE_STKP ... saved=005`. The second shows the final
`LSTF` restoring five active words and BootChannelDisk.Create no longer hitting
the old `STK_ERROR_HOLD`.

New live frontier:

- The germ gets past the previous stack/fault noise and seeds the configured
  request:

  ```text
  [machine] germ netboot request seeded @cyc=67984795:
    pRequest=0o17401360 action=inLoad device=ethernet bfn=0o0
  ```

- The earlier "parked at IM `0o150`" read was misleading. `0o150` is a common
  IFU vector/helper; the final steady state is still BootSwapGerm's `JB 0`
  germERROR loop at `pcf=0o5334`, not a meaningful wait at IM `0o150`.
- The BootChannelDisk `pcf=0o42..0o54` sequence is also no longer suspicious.
  It is the module main-body initializer from the matching source:

  ```mesa
  pulsesPerMilli: CARDINAL =
      Basics.LongDiv[100000, ProcessorFace.microsecondsPerHundredPulses];
  ```

  The observed `LLKB 0o11; R0; LDIV; SGB 0o50; RET` reads
  `ProcessorHeadDorado.microsecondsPerHundredPulses = 0o6200` (`3200`
  decimal), divides `100000` by it, stores `0o37`, and returns normally.
- The first real BootChannelDisk.Create window starts immediately after that.
  A trace through `68033400..68033500` now shows:
  - BootChannelDisk dispatches at `pcf=0o56`, `0o60`, `0o62`, ...
  - The argument stack is correct through the prologue; the old dropped
    `0o200` word is fixed.
  - `DORADO_LOAD_TRACE_VA=017401360,017401370` proves the request record is
    intact when consumed: BootChannelDisk reads
    `pLocation.deviceType` at `0o17401361` as `000005` (ethernet).
- Extending `DORADO_XFER_TRACE` through `68033480..68035000` shows the chain
  link itself is now good:
  - BootChannelDisk's zLLKB path reads codeLink `0o1221` at `0o17410541`.
  - XFER resolves it through the GFT to BootChannelEther's global frame
    `G=0o6234`.
  - `LoadGC` reads BootChannelEther codebase low/high as
    `0o11411,0o76`, i.e. codebase `0o17411411`.
  - The low codebase word is odd, so `LoadGC` takes
    `CSegSwappedOut -> TrapParamDLink` with `sSwapTrap = 0o10`, then enters
    `SavePCAndTrap` at real PC `0o2000`.
  - `DORADO_STORE_TRACE_VA=017406235,017406235` over
    `68033720..68035000` shows no write to BootChannelEther `G[1]` during the
    trap window. The only writes to `M[0o17406235]` are the early zero and the
    germ-load/plant value `0o11411`.
  - No sustained `br31=3E1309` BootChannelEther bytecode dispatch occurs
    before control falls into TrapsImpl.
  - No Stage-2 Mayday/EFTP transmit occurs within a 120M-cycle run:
    `0 display-list pixels; wrote dorado-screen.pgm`.
- Therefore the current blocker is narrower than the older notes claimed:
  not `Carry20`, not the dirty-victim fault, not `IM 0o150`, and not the
  `pulsesPerMilli` initializer. It is also not the
  BootChannelDisk->BootChannelEther chain link anymore. The current frontier is
  the BootChannelEther **code swap trap** path: why `sSwapTrap` for
  BootChannelEther's odd codebase does not clear/start/swap the code and resume
  into BootChannelEther.Create.

Follow-up trace on that frontier:

- `PrincOps.mesa!1` confirms `GlobalFrame` layout:
  - `G+0`: gfi + frame flags (`copied, alloced, shared, started, trapxfers,
    codelinks`)
  - `G+1/G+2`: `FrameCodeBase`
  - `G+3`: first `global[0]` word
- At the BootChannelEther trap, `G=0o6234` and the live frame is:

  ```text
  M[0o17406234] = 0o001211   ; header: gfi 1200 + flags
  M[0o17406235] = 0o011411   ; codebase low, odd => code.out TRUE
  M[0o17406236] = 0o000076   ; codebase high
  M[0o17406237] = 0o000000   ; global[0]
  ```

  Correction: earlier shorthand called `M[G]` "global[0]"; that was wrong.
  `global[0]` is `M[G+3]`.
- `CodeTrap` resolves the faulting destination to BootChannelEther, then calls
  `Start[[frame[frame]]]` exactly as the source says. The trace shows it does
  **not** store to `M[0o17406235]`, so `frame.code.out <- FALSE` is never
  reached.
- The path after `Start` is not a clean return to `CodeTrap`. It re-enters the
  TrapsImpl runtime/signal path and eventually loops back through the same
  Start/CodeTrap machinery. The focused trace that captures the first loop is:

  ```sh
  env DORADO_TRACE_GATE=68034120,68034220 \
    DORADO_IFUDISP_TRACE=1 DORADO_STACK_TRACE=1 DORADO_STKP_TRACE=1 \
    DORADO_STORE_TRACE_VA=017402650,017402720 \
    ./build/dorado --eb ../chm/dorado/CedarDorado.eb!6 \
      --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
      --eftp ../chm/bootfiles/CedarNetExec.boot!4 \
      --germ-netboot-bfn 0 --cycles 68034230
  ```

  Key dispatches:
  - `pcf=0o613..0o622`, `br31=3E0D58` (TrapsImpl): the call into
    `Start[[BootChannelEther]]`.
  - `pcf=0o617` reads `M[G+0]=0o1211`.
  - `pcf=0o621/0o622` proceeds into `LFC2`/runtime signal handling rather than
    returning to CodeTrap and clearing `G+1`.
- The second loop around `68034520..68034850` shows the same family as the
  older "malformed code pointer" notes:
  - XFER reads TrapsImpl's codebase correctly from `M[0o17404765]=0o6530`,
    `M[0o17404766]=0o76`.
  - It then walks a frame/state path through `MDS+0o2264/0o2300`, matching the
    previous StartWithState / StateVector / frame-size investigations.
- Do not go back to the BootChannelDisk `zLLKB` rabbit hole without new
  evidence. With the IFUJump saved-StkP fix, the chain reaches
  BootChannelEther's real global frame and the remaining failure is downstream
  in TrapsImpl start/swap/state handling.

Next step:

1. Decode the TrapsImpl `Start`/`StartCM` bytecode around `pcf=0o613..0o623`
   and `pcf=0o137..0o145` against the source. The open question is why this
   `Start[[frame[frame]]]` path raises/falls into runtime signal handling before
   `CodeTrap` reaches `frame.code.out <- FALSE`.
2. Reuse the older session-19/20 StateVector work instead of redoing it from
   scratch. The current trace again walks through the same `StartWithState` /
   frame-state area (`MDS+0o2264/0o2300`) after the StartFault/signal path.
3. Determine whether the nonzero saved `state.stkptr` / frame allocation is an
   emulator bug in `DST`/`STATE`/XFER frame setup, or whether BootChannelEther
   should have been marked started/resident earlier by the boot-channel init
   call.
4. Keep the fixed BootChannelDisk window command handy:

   ```sh
   env DORADO_TRACE_GATE=68033480,68035000 \
     DORADO_IFUDISP_TRACE=1 DORADO_XFER_TRACE=1 DORADO_XLINK_TRACE=1 \
     ./build/dorado --eb ../chm/dorado/CedarDorado.eb!6 \
       --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
       --eftp ../chm/bootfiles/CedarNetExec.boot!4 \
       --germ-netboot-bfn 0 --cycles 68035020 2>&1 |
     rg "IFUDISP|XFER|XLINK"
   ```

## ROUTE B (2026-06-17, session 23): XFER StkP off-by-one DISPROVEN; StkError-HOLD is a correct-but-symptomatic catch; real blocker is a REPEATING 16-entry binding scan in module 3E15DC

### FOLLOW-UP 10 (corrects follow-ups 5/7/8/9: PrincOps procedure-link gfi is `link >> 7`, not `link >> 6`; `0o1221` is NOT BootChannelEther)
Re-ran the tight BootChannelDisk fallback window with:

`DORADO_XFER_TRACE=1 DORADO_XLINK_TRACE=1 DORADO_IFUDISP_TRACE=1 DORADO_STORE_TRACE_VA=017403356,017403356 DORADO_TRACE_GATE=68032660,68033120`

Key correction: the earlier decode of procedure links used the wrong shift.
`DMesaXfer.mc` explicitly says:

`XferTagOdd: RTemp2_ RSH[T, IfE[AltoMode, 0, 6, 7]]`

For **PrincOps** this is `RSH[...,7]`, and `XferDisp01` uses
`T_ (RTemp2)+(GFT)` (not `2*gfi+GFT`). Therefore the old table decode
`gfi = link >> 6` was wrong for Cedar/Pilot. In particular,
**`0o1221` is not a BootChannelEther procedure link**. The force-patch
experiments that wrote `0o1221` were based on a bad decoder and should be
ignored as evidence about the real link chain.

What the current trace actually shows:

- The BootChannelDisk frame is allocated by `XferProc` at cyc `68032775..68032794`.
  `STORE_VA` proves `XferProc pc=0o4034` stores its return link:
  `M[0o17403356] <- 0o2704` (`L+2`, source link/SLink).
- The BootChannelDisk bytecode sequence at pcf `0o42..0o54` is:
  `LIW`, `LI1`, `LLKB 0o11`, `R0`, `LDIV`, `SGB 0o50`, `RET`.
  This is not a direct external call; it computes/stores state and then
  returns through the frame's `returnlink`.
- `RET` at pcf `0o54` reads that frame return link. `XferMD` sets DLink from
  the fetched MD, so the actual XFER destination is **`0o2704`**, not the
  stale pre-RET T value `0o6200`.
- XFER then treats `0o2704` as a frame link and loads
  `M[0o17402704] = 0o4764`, i.e. TrapsImpl's global frame. That is why
  control lands in TrapsImpl. This is consistent with the frame having been
  entered from a trap/startup context, not with a direct
  BootSwapGerm->BootChannelDisk->BootChannelEther procedure call.

New frontier:

1. Stop chasing `0o1221` and the `gfi>>6` codeLink table. They are wrong for
   PrincOps.
2. Decode the actual procedure-link extraction with `RSH[...,7]` and
   `LDF[...,5,1]`, then identify what procedure link should represent
   `RemainingChannels.Create` in BootChannelDisk's import machinery.
3. The next suspicious low-level spot is the XFER procedure-link decode path
   (`pc=0o1700..0o1707`): the `DORADO_XLINK_TRACE` line for `pc=0o1700`
   shows `T=0o11` but bus values `a=b=0o1101` while executing the
   `RTemp2_ RSH[T,7]` instruction. Verify whether that is the correct MicroD
   encoding of `RSH[T,7]` or a shifter/FF-subdecode bug in the emulator.
4. If the `RSH[T,7]` path is correct, the remaining issue is higher-level:
   BootChannelDisk's module-start/fallback path is returning to TrapsImpl
   because it was invoked from the trap/start chain, and BootChannelEther
   startup/binding has not been reached through the expected chain.

### FOLLOW-UP (corrects this entry's framing -- the germ gets MUCH further than "head-startup")
Decoded the maintenance-panel (MP) trail via the SETMP opcode (zMISC=op
`0o364`, alpha `0o10` = aSETMP; the code is on TOS=acs[0]). The SETMP codes in
the failure window are both `0o1452` = **810 = germStarting** (NOT germERROR),
and the "16-entry scan in 3E15DC" is just **ProcessorHeadDorado.InitSetMPTrap**
rendering the MP code into the cursor bitmap (`cursorPtr[0..15]`, then 3 digits
via the `BITBLT` opcode `0o365`) -- pure DISPLAY aftermath, run twice
(= 2 SetMP calls). So the 3E15DC "grind" is NOT the bug; head-startup
COMPLETES. (Module 3E15DC = ProcessorHeadDorado: codebase `0o17412734` at
global frame g=`0o3400`, confirmed by VM dump M[`0o17403401`]=`0o12734`,
M[`0o17403402`]=`0o76`.)

WHAT ACTUALLY HAPPENS (current tree = committed + uncommitted StkError, with
`--germ-netboot-bfn 0`): the germ completes head-startup, enters `Run[]`, shows
**germStarting**, takes the seeded **inLoad / ethernet** path (the
`--germ-netboot-bfn` "BS key" hack WORKS), and dies AFTER germStarting but
BEFORE **germInLoad** (812) -- i.e. inside the Ethernet boot-channel bring-up,
with germERROR (821 = unnamed ERROR/uncaught trap), NOT the clean germNoServer
(828) / germDeviceError (825) a timeout would give.

LOCALIZED to **`MiniEthernetDriver.ActivateDriver`** (or the path into it):
`BootChannelEther.Create` (deviceType=ethernet) -> `StartRecving` ->
`MiniEthernetDefs.ActivateDriver` -> `EthernetOneFace` device procs
(`GetNextDevice`/`GetEthernet1Address`/`AddCleanup`/`TurnOn`, implemented by
**EthernetOneHeadDorado**, gfi 300). DECISIVE: a `DORADO_ETH_TX_TRACE` run
shows the ONLY transmit is the Stage-1 MicrocodeBoot request (Pup type
`0o264`, bfn `0o110`); the germ's `SendPacket[bootFileSend]` NEVER fires -- so
it traps in `ActivateDriver` (the first thing `StartRecving` does), BEFORE the
SendPacket retry loop. The StkError underflow / germERROR is the trap-handling
tail of that earlier trap (TrapsImpl reads the faulting codebase `0o174067xx`).

So our in-process EFTP/Mayday server never even gets a `bootFileSend` to
answer. The bug is the germ's Pilot Ethernet driver (MiniEthernetDriver +
EthernetOneHeadDorado / `EthernetOneFace`) hitting an operation our
emulator's Ethernet model -- built for Initial's Stage-1 MicrocodeBoot --
does not support, causing a trap.

NEXT STEP (revised): read `chm/cedar/os-src/EthernetOneHeadDorado.mesa!1.txt`
(now readable via the new `tools/tioga2txt`) -- the `GetNextDevice` /
`GetEthernet1Address` / `TurnOn` / `QueueOutput` / `QueueInput` / `GetStatus`
`EthernetOneFace` procedures -- and trace where in the `ActivateDriver` chain
the germ traps (gate a `DORADO_XFER_TRACE` + `DORADO_FAULT_TRACE` to the window
after germStarting, ~cyc 68007000+). Then decide whether to extend the
emulator's Ethernet device to support the germ's driver (IOCB/control-block
ops) so `ActivateDriver` succeeds and the germ reaches `SendPacket[bootFileSend]`
-> our EFTP server. Repro:
`DORADO_ETH_TX_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6'
--eftp '../chm/bootfiles/CedarNetExec.boot!4' --germ-netboot-bfn 0
--cycles 120000000` (only TX is the `0o264` Stage-1 request; no `bootFileSend`).

### FOLLOW-UP 2 (REFINES follow-up 1: the germ dies at the BootChannelDisk->BootChannelEther chain link, BEFORE ActivateDriver)
Mapped every running module by codebase (codebase = cb_hi*65536 + cb_lo, read
from each global frame g+1/g+2; VM dump at cyc 68007740):
- 3E15DC=ProcessorHeadDorado(g=3400) 3E18A8=DiskHeadDorado(4524)
  3E1A6D=EthernetOneHeadDorado(4634) 3E1E10=BootSwapGerm(4720)
  3E0D00=GermSwapImpl(4740) 3E0D58=TrapsImpl(4764) 3E0F39=TeledebugImpl(4770)
  3E1435=MiniEthernetOneDriver(5430) 3E1164=BootChannelDisk(6160)
  3E1309=BootChannelEther(6234).

The germ DOES take the seeded ethernet path: pRequest reads
`[action=0(inLoad), deviceType=5, ord=0, bfn=0]` (VM dump 0o17401361=5). It
runs BootSwapGerm.Run -> BC.Create. The chain (GermDorado.config) is
BootChannelDisk(BC0) -> BootChannelEther(BC1) -> BCnull(backstop). The germ
runs **BootChannelDisk.Create** (3E1164, ~10 dispatches: reads
pRequest.location at `0o1361`, deviceType 5 != sa1000/sa4000 -> falls to
`RETURN[RemainingChannels.Create[...]]`). That `RemainingChannels.Create` call
**XFERs to TrapsImpl (G=`0o4764`), NOT BootChannelEther** -- with
`acs=006160(BootChannelDisk G),177774(UnboundLink),...`. **BootChannelEther
(3E1309) NEVER runs** (0 dispatches out to cyc 68.2M); TrapsImpl runs ~31
dispatches, then the germ stops dispatching (JB-0 germERROR spin).

KEY NUANCES (narrow the root):
- BootChannelEther IS loaded as a module: its global frame G=`0o6234` has a
  valid codebase (`0o11411,,0o76`). So only the inter-module CALL fails, not
  the module's presence.
- There is **NO `SavePCAndTrap` (pc=`0o2000`)** when TrapsImpl is entered here
  (the pc-`0o2000` trap trace is empty for cyc 68032000+). So either the XFER
  goes straight to TrapsImpl via the XFER microcode's own unbound/odd branch
  (no SavePCAndTrap), or the link literally points at TrapsImpl.
- `0o177774` occurs only **2x** in the whole germ MDS (0o17400000-0o17420000)
  -- so it is NOT a pervasive UnboundLink splatter (unlike germ!4's `0o27132`
  or the `0o26411` of session 19). It is specific.
- BC0 (BootSwapGerm->BootChannelDisk) WORKS (BootChannelDisk runs real code),
  but BC1 (BootChannelDisk->BootChannelEther) does NOT -- so the chain-link
  binding/relocation is SELECTIVELY wrong for BC1.

OPEN ROOT (decisive next test): trace the BootChannelDisk `RemainingChannels.
Create` opcode (the EFC/SFC at BootChannelDisk pcf ~`0o54`/`0o62`) and capture
the exact control-link value it loads + the trap index. Distinguish:
(A) the BC1 link is genuinely UnboundLink `0o177774` (our germ relocation/GFT
setup failed to bind the BootChannelDisk->BootChannelEther EFC link -- compare
the germ FILE bytes for that link vs VM); vs
(B) BootChannelEther's code is swapped-out and the call's CodeTrap->Start
lazy-start fails (the start-chain bug, same family as the 6 sSwapTraps seen
earlier). BootChannelDisk's code-segment link area
(VA ~`0o17410530..0o17410543`) holds links `0o4631,0o4550,0o3424,0o4632,...`
(near head-module G's) but NO `0o62xx` link to BootChannelEther(G=`0o6234`) --
which supports (A): the BootChannelEther.Create link is missing/unbound.
Repro: same `--germ-netboot-bfn 0` run; gate `DORADO_XFER_TRACE` to
`68032000,68033000` and watch BootChannelDisk (br31=3E1164) -> XFER to
G=`0o4764`.

VERIFY ALSO: confirm `ethernet` really = Device.Type ordinal **5** (the seed's
`GERM_DTYPE_ETHERNET`); `sa4000`=3 is runtime-confirmed but `ethernet`=5 was
author-derived. If 5 is wrong, BootChannelEther.Create wouldn't match it even
once reached -- but the BC1-link failure is the immediate blocker regardless.

### FOLLOW-UP 3 (EXACT values: the failing call is zLLKB #9, and its codeLink is wrong)
The call opcodes (PrincOps.mesa): `op=0o344`=**zLLKB** (Load LinK Byte),
`op=0o343`=zRET. At BootChannelDisk pcf `0o46` the germ runs **`zLLKB alpha=0o11`**
to load external link #9 (= the `RemainingChannels.Create` link to
BootChannelEther.Create). zLLKB microcode (DMesaXfer): `IF G.codeLinks THEN
Push[Fetch[C - alpha - 1]] ELSE ...` -- codeLinks mode reads the link from the
code segment, just before the codebase C.

DIRECT TRACE (DORADO_XFER_TRACE md=data, lva=addr; cyc ~68032854):
- zLLKB reads G's GFI word `M[0o17406160]=0o1115` (codeLinks bit set), computes
  T=`0o177766` (= -(alpha+1) = -0o12), and reads the codeLink at
  **VA `0o17410532` -> `0o3424`**.
- `0o3424` is then used indirectly: `Fetch[0o17403424] -> 0o6200`. But
  `0o3424` (= ProcessorHeadDorado G `0o3400` + `0o24`) points INTO
  ProcessorHeadDorado's frame, NOT BootChannelEther (G=`0o6234`).
- The resolution ends on control link `0o103240` -> gfi = `0o103240>>6` =
  `0o1032` (538) -- OUT OF the 128-entry GFT -> unbound -> XFER's sUnbound
  branch -> SD[sUnbound] -> TrapsImpl -> germERROR. (No SavePCAndTrap; the
  XFER's own unbound branch dispatches it.)

So the concrete defect is **BootChannelDisk's codeLink #9 at VA `0o17410532`
holds `0o3424` (wrong) instead of a valid link to BootChannelEther.Create
(gfi `0o1200`=640, i.e. a link ~`0o120000`)**. The codeLink region for
BootChannelDisk is at VA `0o174105xx` just below codebase `0o17410545`:
`...0o3424(@532),0o4632(@533),...` -- none of which encode gfi 640.

DECISIVE TEST DONE (file vs emulator) -- RESULT: NOT a relocation bug.
Read the germ file directly (`Dorado.germ-6.1.6`, big-endian words; the germ
loads word0->VM `0o1000` then GERMREMAP adds `0o17400000`, so VA `0o17410532` =
file word `0o7532`). File word `0o7532` = **`0o3424`** -- IDENTICAL to VM, and
the whole neighbor region matches byte-for-byte
(file `0o7530..0o7536` = `0o4631,0o4550,0o3424,0o4632,0o217,0o211,0o207` =
the VM `0o174105xx` dump). word0=`0o3354` confirms the file<->VM mapping. So
**the codeLink `0o3424` is GENUINE germ data; GERMREMAP did not corrupt it.**

REVISED ROOT: this is an **emulator link-RESOLUTION** problem, not a
relocation/binding-data problem. The germ legitimately stores `0o3424` as
BootChannelDisk's codeLink #9, and `RETURN[RemainingChannels.Create[...]]`
is a TAIL CALL: zLLKB #9 loads `0o3424` as the source/return link, then
**zRET (op `0o343`, pcf `0o54`) XFERs through it**. Our emulator resolves
`0o3424` to an out-of-range gfi (`0o1032`) -> unbound. So either (a) our XFER /
codeLink resolution unpacks `0o3424` wrong (it must resolve to
BootChannelEther.Create, gfi `0o1200`), or (b) a GFT/PrincOps binding table the
resolution indexes is mis-set. The trace shows the resolution doing
`Fetch[0o17403424]->0o6200` (treating `0o3424` as a pointer into
ProcessorHeadDorado's frame G=`0o3400`) -- that intermediate looks wrong; the
correct codeLink resolution must NOT land in ProcessorHeadDorado.

NEXT: read the PrincOps zRET + codeLink resolution semantics (DMesaXfer.mc
XferProc/XferTagOdd/XferTagEven + the LLKB GetLinkID subroutine), and verify
our emulator's XFER applies them to `0o3424` the same way real hardware does.
Cross-check GFT setup at MDS+`0o1400` (gfi-indexed). Repro:
`DORADO_XFER_TRACE=1 DORADO_IFUDISP_TRACE=1 DORADO_TRACE_GATE=68032800,68033000`
-- BootChannelDisk pcf `0o46` zLLKB reads `0o3424` from `lva=0o17410532`, then
pcf `0o54` zRET XFERs through it to the sUnbound handler.

### FOLLOW-UP 4 (ruled out GFT binding too; root is the codeLink RESOLUTION of 0o3424)
Checked the remaining bind tables; both are CORRECT, so the defect is narrowed
to how the codeLink `0o3424` is resolved:
- **GFT is correct.** GFT base = MDS+`0o1400`, indexed by `gfi>>6`:
  `GFT[9]`=`M[0o17401411]`=`0o6160` (BootChannelDisk, gfi `0o1100`),
  `GFT[10]`=`M[0o17401412]`=`0o6234` (BootChannelEther, gfi `0o1200`),
  `GFT[1]`=`0o3400` (ProcessorHeadDorado), `GFT[6]`=`0o4764` (TrapsImpl). So a
  link carrying gfi `0o1200` WOULD reach BootChannelEther correctly.
- **CodeTrap doesn't bind links.** germopsimpl's swap-trap handler
  (`ReadCodebase`/`WriteCodebase` clears the out-bit, then
  `ControlModuleFormat.Call[MainBody[gfi]]`) only makes code resident + runs
  MainBody; it does not fix up links. So codeLinks are pre-bound (consistent
  with file==VM).

So GFT + relocation + module presence are all correct. The lone defect is that
**BootChannelDisk's codeLink #9 = `0o3424` does NOT resolve via the GFT to
gfi `0o1200`** -- our XFER treats it as a frame link (`Fetch[0o17403424]->0o6200`,
an invalid G) and ends unbound. None of BootChannelDisk's codeLinks
(`0o4631,0o4550,0o3424,0o4632,0o217,...`) look like a proc link with gfi
`0o1200` in the high bits, so the codeLinks use a Cedar-specific encoding
(frame/indirect/GFT-relative) that our `zLLKB`+`zRET`+XFER path resolves
incorrectly. NOTE the open sub-question: confirm our `zLLKB` reads the RIGHT
codeLink slot (alpha `0o11` -> VA `0o17410532`; my C-offset had a +/-1
ambiguity, so an off-by-one slot read is possible -- if the correct
BootChannelEther link sits in an adjacent slot, the bug is the zLLKB offset).

REMAINING WORK (the real fix, microcode-vs-emulator): step the FULL
`zLLKB`(pcf `0o46`)+`zRET`(pcf `0o54`)+XFER resolution of `0o3424`
microinstruction-by-microinstruction against `DMesaXfer.mc` (XferProc /
XferTagOdd / XferTagEven / GetLinkID) and find where our microengine diverges
from the Cedar codeLink semantics -- or where the zLLKB codeLink offset is
off by one. Everything upstream (germ planting, GERMREMAP, GFT, module
presence) is verified correct.

### FOLLOW-UP 5 (the EXACT anomaly: zLLKB #9 lands on a frame link; the real BootChannelEther link is at index #2)
Disassembled GetLink/GetLinkID (DMesaXfer.mc): `GetLink: T_ NOT(T);
LongFetch_ T` with MemBase=Code -> fetch at **Code - alpha - 1** (downward
from the code base). For BootChannelDisk: Code base BR[`0o37`] = br31 =
`0o17410544` (= codebase `0o10545,,0o76` minus 1), alpha=`0o11`(9) ->
Code-10 = VA `0o17410532` = **`0o3424`** (what our zLLKB loaded). alpha=`0o11`
is GENUINE bytecode: the code word at `0o17410567` = `0o162011` =
`[opcode 0o344=zLLKB, alpha 0o11]` (IFU decoded it correctly).

Decoded BootChannelDisk's whole codeLink table as procedure links
`[gfi=w>>6, ep=(w>>1)&037, tag=w&1]` (downward index = Code-VA-1):
```
  idx#0 0o411 gfi4    idx#1 0o221 gfi2    idx#2 0o1221 gfi10 <== BootChannelEther (GFT[10]=0o6234)!
  idx#3 0o213 gfi2    idx#4 0o203 gfi2    idx#5 0o207 gfi2
  idx#6 0o211 gfi2    idx#7 0o217 gfi2    idx#8 0o4632 tag0(frame)
  idx#9 0o3424 tag0(frame) <== WHAT WE LOADED (garbage gfi 28)
  idx#10 0o4550 tag0   idx#11 0o4631 tag1 gfi38
```
So the ONE valid BootChannelEther link (`0o1221`, gfi 10, tag=1 procedure) is
at **index #2**, but the germ's zLLKB uses **alpha #9** and lands on the frame
link `0o3424`. A 7-slot gap. tag-1 (procedure) links cluster at #0-#7,#11;
tag-0 (frame) links at #8-#10 -- and #9 is a frame link, semantically wrong
for `RETURN[RemainingChannels.Create[...]]` (a procedure call).

THREE candidate roots (decisive work for next session):
1. **LoadGC sets the Code base wrong.** Our BR[`0o37`]=`0o17410544`. If real
   hardware's links live at a different base (so index #9 reaches `0o1221` at
   `0o17410541`, the base would need to be `0o17410553` = codebase+6), our
   LoadGC / code-base setup is off. Trace LoadGC for BootChannelDisk and check
   the Code base vs codebase, and whether codeLinks are at a header offset, not
   immediately below the codebase.
2. **Our zLLKB link-index arithmetic is off.** The microcode does
   `Code - alpha - 1`; verify our microengine executes `T_ NOT(T)` +
   `LongFetch_ T` to the same VA hardware would (re-check the +/-1 and the
   MemBase=Code base register).
3. **codeLink table ordering** -- but file==VM is verified, so this would have
   to be a layout/interpretation difference, not corruption.
Note the strong hint: a valid, correctly-encoded BootChannelEther link
(`0o1221`->GFT[10]) EXISTS in the table, so the germ image is fine; the defect
is in how our emulator indexes/loads/bases the codeLink for alpha #9. Repro:
`DORADO_XFER_TRACE=1 DORADO_TRACE_GATE=68032850,68032880` -- watch the
LongFetch at MemBase Code (`mb=0o37`) read `lva=0o17410532` (`0o3424`); the
target slot for `0o1221` is `0o17410541`.

### FOLLOW-UP 6 (full resolution trace: zRET resolves the codeLink to TrapsImpl, not BootChannelEther -- this is the frontier)
Traced the zRET (pcf `0o54`) resolution end to end (DORADO_XFER_TRACE,
cyc 68033054+). The zLLKB-loaded `0o3424` is processed and the zRET transfers
through link **`0o6200`** (T=`0o6200` at Xfer entry pc `0o1700`). The XFER then:
`0o6200` -> (pc `0o1706`) T=`0o2704` -> reads frame `0o2704`'s globallink
`M[0o17402704]=0o4764` (= TrapsImpl G) -> LoadGC G=`0o4764` (reads TrapsImpl
codebase `0o6530,,0o76`) -> XferExitDispatch -> enters **TrapsImpl at
pcf `0o623`** (br31=3E0D58). So the codeLink resolves to a TrapsImpl frame,
NOT BootChannelEther, and TrapsImpl then drives germERROR. (No SavePCAndTrap;
this is a normal-looking XFER that lands on the wrong module.)

So the chain is: zLLKB #9 -> `0o3424` -> (processed) -> `0o6200` -> frame
`0o2704` (a TrapsImpl frame) -> TrapsImpl. Whether the defect is the LOAD
(`0o3424` is the wrong slot; the valid BootChannelEther link `0o1221` is at
index #2 -- follow-up 5) or the RESOLUTION (`0o3424`/`0o6200` is right but our
XferProc/XferTagEven mis-resolves it) is the remaining question, and it now
requires disassembling + stepping the actual XferProc / XferTagEven /
XferTagOdd / LoadGC microcode (`mbdis --disasm 'chm/dorado/Cedar.mb!6'`, real
pc `0o1700`/`0o1706`/`0o2003`/`0o3600`/`0o3611`) against our microengine's
execution, microinstruction by microinstruction, to find the divergence.

THIS IS THE FRONTIER. Everything upstream is verified correct (germ planting,
GERMREMAP, GFT binding gfi 1100->`0o6160` / 1200->`0o6234`, module presence,
IFU operand decode alpha=`0o11`, the codeLink table is genuine germ data with a
valid BootChannelEther link `0o1221`@#2). The bug is in the
zLLKB-load-slot-or-XFER-resolution of the BootChannelDisk->BootChannelEther
codeLink. Repro: `DORADO_XFER_TRACE=1 DORADO_TRACE_GATE=68033050,68033190`
(zRET at BootChannelDisk pcf `0o54` resolves link `0o6200` to TrapsImpl
pcf `0o623`). Next session: side-by-side microcode disasm vs emulator step.

### FOLLOW-UP 7 (force-and-observe: NEGATIVE -- forcing the codeLink to 0o1221 does NOT reach BootChannelEther)
Added a gated diagnostic (`DORADO_FORCE_BCLINK`, machine.c, UNCOMMITTED) that
overwrites BootChannelDisk's codeLink #9 slot (VA `0o17410532`) from `0o3424`
to `0o1221` (the "valid BootChannelEther link" decoded at index #2 in
follow-up 5). RESULT: the force fires (`0o3424 -> 0o1221 @cyc 67929860`) and
CHANGES behavior -- BootSwapGerm now runs 42603 dispatches (was 168) -- but
**BootChannelEther (3E1309) STILL never runs**, no `bootFileSend`, and the germ
STILL germERRORs (BootSwapGerm spins 42591x at pcf `0o5334` = the JB-0
germERROR spin; 0 display pixels).

So the simple "load the right link value" theory is DISPROVEN. Two
implications for the next session:
1. **`0o1221` may NOT be BootChannelEther.Create.** My follow-up-5 decode
   (`gfi=w>>6`, so `0o1221>>6=10`=GFT[10]=BootChannelEther) may be wrong --
   the ControlLink bit layout / gfi-extraction needs to be re-derived from
   PrincOps (`procedure => [gfi: GFTIndex, ep: EPIndex, tag: BOOL]`) and the
   actual XFER microcode, not assumed. Treat the "valid link at index #2"
   claim as UNCONFIRMED.
2. **The codeLink RESOLUTION (XferProc/XferTagEven) may be broken independent
   of the value** -- forcing a plausibly-valid value still didn't resolve to
   BootChannelEther. So the bug is more likely in how our microengine resolves
   these Cedar codeLinks than in the loaded value alone.

NET: the force experiment redirects the next session AWAY from "patch the link
value" and TOWARD "step the XFER codeLink resolution microcode vs our
microengine" (the follow-up-6 frontier). The exact ControlLink encoding +
XferProc/XferTagEven/LoadGC resolution must be disassembled and compared
microinstruction-by-microinstruction. Uncommitted debug aids in the tree:
`DORADO_FORCE_BCLINK` (machine.c), `DORADO_DMUX_TRACE` (memory.c), the StkError
HOLD (cpu.c) -- none committed.

### FOLLOW-UP 8 (CORRECTED encoding + the codeLink is INDIRECT -- deref goes through ProcessorHead's global frame)
Re-derived the exact ControlLink encoding from PrincOps.mesa (the
`rep => [fill:[0..37777B], indirect:BOOL, proc:BOOL]` overlay gives the
discriminator bits): in C-word terms, **`W&1`=proc bit, `W&2`=indirect bit**.
So `W&1==1` -> procedure `[gfi=(W>>6)&01777, ep=(W>>1)&037]`; `W&3==0` -> frame
link (frame=W); `W&3==2` -> indirect (W&~3 = POINTER TO ControlLink/Port).
With this: `0o3424` (`W&3=0`) = frame link; `0o1221` (`W&1=1`) =
procedure[gfi=10,ep=8] -> GFT[10]=`0o6234` = BootChannelEther (so the
follow-up-5 decode of `0o1221` WAS right).

But the force-and-observe (follow-up 7) is now EXPLAINED and the picture
CORRECTED: with `DORADO_FORCE_BCLINK` the zLLKB really does read `0o1221`
(traced: `md=0o1221 lva=0o17410532`, `0o1221` on stack), yet the bytecode
immediately **DEREFERENCES it** (`Fetch[0o17401221]=0`) -- i.e. codeLink #9 is
accessed as a POINTER (indirect import), not a direct link. So forcing a
DIRECT procedure link (`0o1221`) was the wrong KIND of value; `*0o1221`=0 ->
trap -> germERROR. The negative force result is a value-type mismatch, not a
disproof of the link theory.

WHAT THE INDIRECT DEREF ACTUALLY DOES (unforced, link=`0o3424`):
`*0o3424` = `M[0o17403424]` = **`0o6200`**, and `0o17403424` is
ProcessorHeadDorado's global frame region (`G=0o3400` + `0o24` = global var
`0o24`). So `RemainingChannels.Create` resolves: codeLink #9 `0o3424` (a
pointer) -> ProcessorHead.global[`0o24`] = `0o6200` -> (frame link) -> ... ->
TrapsImpl. It should reach BootChannelEther (`0o6234`), so the indirect chain
lands on the wrong target.

OPEN (the real fix, refined): why does codeLink #9 `0o3424` deref to
ProcessorHead's global `0o24` (`0o6200`) instead of a BootChannelEther link?
Either (a) `0o3424` (genuine germ data) is the correct indirect pointer but it
should point somewhere holding a BCEther link and our emulator computes the
deref ADDRESS wrong, or (b) ProcessorHead.global[`0o24`] is SUPPOSED to hold a
BCEther pointer and our ProcessorHead init never set it (a module-init / fixup
our emulator skips), or (c) `0o6200` IS the right indirect link and our XFER
mis-resolves it. NEXT: identify the post-zLLKB opcodes (op `0o100`/`0o255`/
`0o055` at BootChannelDisk pcf `0o50`/`0o51`/`0o52`) that do the deref, decode
what address they compute, and check whether ProcessorHead.global[`0o24`]
(`M[0o17403424]`) should hold `0o6200` or a BCEther pointer. To force-test
correctly now: patch `M[0o17403424]` (the indirect target) to a BCEther
procedure link (`0o1221`), NOT the codeLink slot. Repro:
`DORADO_FORCE_BCLINK=1 DORADO_XFER_TRACE=1 DORADO_TRACE_GATE=68032850,68033010`.
(Uncommitted debug aids in tree: DORADO_FORCE_BCLINK/DMUX_TRACE/StkError.)

### FOLLOW-UP 9 (corrected force ALSO negative; force-patching is exhausted -- next must be a microcode walk or reference impl)
Patched the indirect TARGET `M[0o17403424]` (`0o6200 -> 0o1221`, the BCEther
proc link) instead of the codeLink slot. Fired @cyc 68000608, but the germ
STILL never reaches BootChannelEther (3E1309) and germERRORs (BootSwapGerm
spins at pcf `0o5334`). So neither force works. Confounders that make
force-and-observe unreliable here: (1) the resolution is a multi-step indirect
chain (codeLink ptr -> ProcessorHead.global[`0o24`] -> `0o6200` -> further
processing by the post-zLLKB opcodes `0o100`/`0o255`/`0o055` -> zRET -> XFER),
(2) the indirect target `M[0o17403424]` is a RUNTIME global that may be
re-written between the patch (68000608) and the zLLKB deref (~68032880), (3)
value-type sensitivity (the chain expects specific link kinds at each step).

ASSESSMENT: inline tracing + force-patching is EXHAUSTED for this bug. The
defect is somewhere in the multi-step indirect codeLink resolution
(`zLLKB` -> deref -> `0o100`/`0o255`/`0o055` -> `zRET` -> `XferProc`), and
pinning it needs a fundamentally different method:
  (a) DISASSEMBLE + STEP the exact opcodes (decode op `0o100`/`0o255`/`0o055`
      from PrincOps -- they do the indirect deref + link massaging) and the
      XferProc/XferTagEven/XferTagOdd microcode, comparing our microengine's
      execution to the canonical microcode microinstruction-by-microinstruction;
      AND/OR
  (b) get a REFERENCE: what does this exact codeLink chain resolve to on real
      hardware / another PrincOps impl? The germ's RemainingChannels (an
      indirect BootChannel import) -> BootChannelEther.Create. Decode op
      `0o100`/`0o255`/`0o055` first (PrincOps.mesa opcode table) -- they ARE
      the indirect-link massage and likely where our emulator diverges.
VERIFIED-CORRECT upstream (do NOT re-chase): germ planting, GERMREMAP
(file==VM), GFT (gfi 1100->`0o6160`, 1200->`0o6234`), module presence
(BCEther loaded), IFU operand decode (alpha `0o11`), ControlLink encoding
(`W&1`=proc/`W&3==2`=indirect), and that `0o1221`=BCEther proc link. The bug
is isolated to the indirect-codeLink resolution chain for
BootChannelDisk->BootChannelEther.

### (original session-23 findings below -- the StkError/XFER analysis stands;
### only the "blocker = 3E15DC grind / head-startup" framing is superseded above)

Investigated the uncommitted `cpu.c` work (StkError HOLD + fault-task wake)
against the documented `xf.push`/XFEREXIT root cause. Three findings, all
instrumented on the current working tree (committed + uncommitted StkError),
germ-6.1.6 + CedarDorado.eb!6, `--germ-netboot-bfn 0`:

1. **The `xf.push`/XFER StkP off-by-one theory does NOT hold on the current
   tree.** Per-PC StkP accounting over the head-startup window
   (`DORADO_STKP_TRACE`, gate 68000000,68040000) shows the XFER
   anticipation/cancellation balances EXACTLY: XferProc `0o4030` (+26) +
   XferDisp00 `0o2004` (+31) = **57 anticipation +1 = 57 XferExit `0o2026`
   -1**. At the DiskHead `@SFC` (cyc 68007728) the handler pops a REAL link
   at stkp=1->0, NOT the empty-stack NullControl session 22 described (the
   committed RestoreStkP + IFUReset fixes already changed that). So do NOT
   re-chase the XFER `+1`/`-1` pairing or `xf.push`.

2. **StkError-HOLD (uncommitted) is correct hardware behavior but only a
   symptomatic catch.** HM §3.1 confirms verbatim: StkError HOLDs the
   instruction (StkP uncommitted), latches StkUnd/StkOvf, and wakes the
   fault task (task 15); Cedar has the real task-15 handler
   (`DMesaFaults.mc`: FaultTask -> ChkStkErr -> EmuFault -> StackError,
   `RestoreStkP` + clamp + `sStackError`). The uncommitted diff's ONLY
   functional change is this HOLD intercept (the widened PC windows at
   cpu.c:3344/3348 are just `DORADO_STACK_TRACE` gates). It fires EXACTLY
   ONCE in a full run -- at cyc 68033697, pc=0o224, stkp=000, und=1 -- and
   the germ still germERRORs (0 display pixels). It catches the underflow at
   the bottom of the grind; it does not prevent it. KEEP it (it's HW-correct
   and pairs with RestoreStkP), but run the Galaxian/NetExec PIXEL gate
   (`make run-galaxian`/`run-netexec`, NOT just `make test`) before trusting
   it -- the intercept runs on every emulator BLOCK=1 instruction and the
   Alto worlds hammer the stack.

3. **The real head-startup blocker is a REPEATING 16-entry binding scan in
   module `br31=3E15DC`, not a StkP/XFER bug.** `DORADO_IFUDISP_TRACE`
   shows pcf 0o2240..0o2257 each dispatched exactly 32x, all in module
   3E15DC (br31 NEVER advances). The loop-top (pcf 0o2240) `acs[1]` is a
   counter that climbs 0->0o17 (16 entries) then RESETS to 0 and repeats --
   a 16-entry counted scan run >=2x with IDENTICAL state (so it is a spin,
   NOT forward progress through different OS-resident frames). The scanning
   opcode (`pc=0o224`) reads a run of codebase long-pointers
   (`lva`=0o17406760,6762,6764,6766,... +2 each) and encounters
   **`T=0o177774`** -- the same UnboundLink-class value sessions 20/22 hit.
   Only 4 traps in the whole grind, all sSwapTrap (T=0o10); no ControlFault.
   The repeated scan slowly drains the stack until the `0o224` pop
   underflows -> StkError -> germ still ends in germERROR.

**NEXT STEP (the decisive open question):** identify the 16-entry table at
codebase ~`0o174067xx` that module 3E15DC re-scans, and determine whether
the unbound entries (`0o177774`) are (a) genuinely OS-bound imports a
germ-only run cannot supply (-> OS boundary; strategy must change), or
(b) entries the germ's own relocation/binding pass should have filled but
our emulator computed wrong (-> fixable emulator/germ-state bug). This
converges with session 20's `0o177774` ("a stack word of a well-formed
StateVector") and session 22's start-chain reading. Repro:
`DORADO_IFUDISP_TRACE=1 DORADO_XFER_TRACE=1
DORADO_TRACE_GATE=68007000,68034000 ./build/dorado --eb
'../chm/dorado/CedarDorado.eb!6' --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6'
--eftp '../chm/bootfiles/CedarNetExec.boot!4' --cycles 120000000`
(grep IFUDISP pcf=0o2240 for the counter; grep `pc=0o224` for the
long-pointer scan + `T=0o177774`).

NOTE on the `--germ-netboot-bfn` "BS key" hack: it FIRES correctly
(`pRequest=0o17401360 action=inLoad device=ethernet`) but is downstream of
this blocker -- the germ germERRORs in head-startup (the 3E15DC scan)
BEFORE `Run[]` ever reads `pRequest`, so the netboot request never takes
effect yet.

## ROUTE B (2026-06-17, session 22): `IFUReset` current-opcode state FIXED -- Cedar map restart now matches HM/Cedar microcode; remaining blocker is still germERROR before Stage-2 EFTP

### THE FIX (landed locally, `src/cpu.c`)
`IFUReset` was over-clearing the IFU state. It stopped future fetches, but
also zeroed the current X-level opcode context (`PCX`, decoded operands,
`Length`, `IdCnt`, etc.). Cedar's map-restart path explicitly relies on
that state surviving:

```text
MapOpExit:
    T_ ID, IFUReset, StkP+2, Branch[NewMapExit]; * ID = instruction length
NewMapExit:
    T_ T - (PCX') -1, branch[SetPCAndJump0];
SetPCAndJump0:
    Global, PCF_ T; ... IFUNext0;
```

This matches the Hardware Manual:
- HM §6.2: `B<-PCX'` reads the PC of the current opcode, and `PCF<-B` does
  not change `PCX`.
- HM §6.2: after operands are consumed, `<-Id` returns `Length`; the manual
  gives the same `T <- (Id) - (PCX') - 1; PCF<-T; ... IFUJump[0]` restart
  sequence.
- HM §6.6 says `IFUReset` clears IFU errors, prevents further IFU memory
  references, and clears Brk/Test state; it does not say to destroy the
  current opcode's `PCX`/`IdCnt` context. HM §6.10 also names `IdCnt` as
  part of the current IFU state.

Implementation: `IFUReset` now still makes the IFU passive and clears
Brk/Test state, but preserves the current opcode context. The focused CPU
test now checks that `PCX`, opcode, `IdCnt`, `alpha`, `beta`, `Length`, `N`,
`PackedAlpha`, and `Sign` survive reset.

### Verification
- `make -C dorado` passed.
- `./build/test_cpu` passed.
- Cedar repro with `DORADO_IFUDISP_TRACE=1
  DORADO_TRACE_GATE=68020500,68021200` now shows MISC `0o364` at
  `pcf=0o5727` restarting at `pcf=0o5731` instead of redispatching itself.

### Current status / next blocker
This fixes the artificial IFU restart regression. It does **not** yet reach
Stage-2 EFTP: a 100M-cycle Cedar run still has `eftp_r=0` and settles into
the already-documented Mesa `JB 0` germERROR loop at `pcf=0o5334`,
`op=0o210`, with MP/top-stack value `0o1465`. So the next investigation is
still the session-21 germ/head-startup state problem, not Ethernet packet
serving.

## ROUTE B (2026-06-17, session 21): `RestoreStkP` FIXED -- Cedar starts Stage-1 Ethernet netboot again; Stage-2 EFTP still not reached

### THE FIX (landed, `src/cpu.c` / `include/cpu.h`)
The head-startup SFC failure was an emulator bug, but NOT the deeper
`xf.push`/XFEREXIT theory below. The concrete missing hardware behavior was
**`RestoreStkP`**: the emulator treated `FF=0o145` (FA=1 FB=4 FC=5) as a
stub. Per HM §3.1, StkP is saved at t2 of an IFU-dispatched instruction and
`RestoreStkP` reloads that saved value so restart-style traps can re-enter an
opcode after partial stack motion.

Implementation:
- Added `cpu->ifu_saved_stkp`.
- On successful IFU dispatch, save the pre-RSTK `StkP` before the
  IFUJump instruction's post-cycle stack adjustment.
- Implemented `RestoreStkP` for `FF=0o145` by reloading that saved value.
- Added `test_restore_stkp_ff`.

Why this matters: Mesa `SFC` (op `0o342`) pops its destination link. If the
opcode traps on swapped code, `SavePCAndTrap` uses `RestoreStkP` before the
opcode retry. Without it, the retry sees the stack one entry too low and the
head-startup call path degenerates into NullControl / ControlFault. With the
fix, the SFC retry preserves the head link.

### Verification
- `make test` passed before commit: all 10 emulator suites green.
- After rebasing on `origin/main`, rebuilt and ran `dorado/build/test_cpu`;
  all CPU tests passed, including `test_restore_stkp_ff`.
- Long Cedar run starts the Ethernet microcode boot path:
  `uCodeBoot(264)`, `req=1`, `repl=63`; Cedar world loads and the germ is
  planted.

### Current status / next blocker
The emulator now starts **Stage-1 network boot** for Route B again, but the
germ still does **not** reach Stage-2 EFTP / `DoInLoad`: `eftp_r=0` and the
run spins at `pc=0o150` / `JB 0` after the planted germ run. So the previous
SFC empty-stack symptom is fixed; the remaining work is the germ-state /
load-boundary that prevents the germ from requesting `CedarNetExec.boot`.

Repro for the current state:

```sh
DORADO_ETH_TX_TRACE=1 DORADO_BOOTDIR_DEBUG=1 \
  ./build/dorado \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --eftp '../chm/bootfiles/CedarNetExec.boot!4' \
  --cycles 120000000 --progress
```

Expected current result: Stage-1 Ethernet boot traffic appears and the Cedar
world/germ load completes, but no Stage-2 EFTP request is made yet.

## ROUTE B (2026-06-17, session 20): the +11 dest is FIXED at the root -- `RisID` never substituted the IFU Id (alpha) onto the **Mar** of a memory-reference, so `LADRB`/`GADRB`'s `DummyRef_ StackNoUfl&+1, RisID` computed `@s = L + StackNoUfl` (stale stack value 18) instead of `L + alpha` (=7). The germ now advances **188 -> 1187 real IFU dispatches** into a NEW germ module (`br31=3E15DC`, codebase `0o17412734`, 767 dispatches there) before raising a LATER germERROR. That LATER germERROR is now traced to a GENUINE OS boundary (2 handled sSwapTraps then a ControlFault on an OS-resident link `M[0o340]`=0), NOT another emulator bug -- so the RisID Mar fix was the real, last emulator bug on this path. Gate ALL GREEN. Details in the NEXT BLOCKER section below.

### THE FIX (landed, `src/cpu.c`) -- HM p.24 RisID + microcode-grounded
`LADRB` = `Push[L+alpha]` is implemented as `DummyRef_ StackNoUfl&+1,
T_ MD, RisID` then `StackT_ VALo` (DMesaXfer.mc!1). The DummyRef forms
VA = BR[L] + Mar, where the **Mar (A bus) is the RM/STK source** and
`RisID` must overrule it with the IFU Id (= the operand byte alpha). Our
`RisID` substitution only replaced the bus for the explicit register
forms (`A<-RM/STK` asel=4, `B<-RM/STK` bsel=1); a code NOTE explicitly
punted the memory-reference Mar case "until a concrete case proves the
Mar should be replaced." `LADRB`/`GADRB` ARE that case -- the ONLY
memory-ref users of `RisID` in the whole microcode (verified: the three
`DummyRef_ Stack..., RisID` lines in DMesaXfer.mc are the only ones).
Fix: capture whether `ff_a_override` left the A source as the RM/STK
default (`a_is_rmstk_default`, asel 0/1 with no FF A-override), and in
the RisID branch set `a = Id` for that case too. Same IFU-operand-byte
family as the session-12/15/19 fixes (notLength, TisId/alpha, codebase
IFetch-Id).

### Proof (store-traced, decisive)
- Ground truth: dumped StartWithState's Mesa code bytes -- the BLT
  (op `0o352`) is at codebase+`0o455` (`M[0o17406756]` low byte), and the
  instruction right before it is `[0o072, 0o007]` = **`LADRB alpha=0o7`**.
  So `@s` belongs at frame offset 7 (= `0o2713`); the germ genuinely
  places `s` at offset 7 and the fsi-5 (27-word) frame is CORRECT. (This
  RESOLVES the session-19 follow-up 5 vs 6 tension: the frame size was
  never wrong; the dest was misread by +11 because the Mar took the stale
  StackNoUfl value 18 instead of alpha 7.)
- With the fix, `DORADO_STORE_TRACE_VA="017402742,017402742"` shows the
  BLT (pc `0o3537`) NO LONGER stores to retFrame.returnlink `0o2742` --
  the old clobber (`0o26411` UnboundLink at cyc 67990672) is GONE. The
  germ continues into the StartChain.

### GATE -- ALL GREEN
1. `make test` = 10/10 suites.
2. Galaxian @160M: **121553** px (exact).
3. AEmu NETEXEC @200M: **1481** px (band 1476-1505).
4. germ-6.1: 188 -> **1187 real dispatches**; NO hardware fault (flt=0);
   modules reached: 3E0D58 (TrapsImpl, 342 disp), **3E15DC (new, 767)**,
   3E1E10 (SignalHandler, 78). (Gate-safe: the RisID Mar sub only fires on
   `asel 0/1` memory refs whose A source is the RM/STK default; the Alto
   worlds never run `LADRB`/`GADRB`, hence unchanged pixel counts.)

### SUPERSEDED NEXT BLOCKER (session 21 fixed the concrete StkP bug) -- the SFC that starts DiskHeadDorado pops an empty stack; XFEREXIT StkP-1 ate the link
**CORRECTION (supersedes the "genuine OS boundary" reading below).** The
germ source (`bootswapgerm-indigo.mesa` / germ-6.1) shows the boot order
is, inside the germ's `Initialize`: set SD trap handlers ->
`ProcessorFace.Start[]` -> **`Heads.Start[]`** (start the device heads:
ProcessorHeadDorado, DiskHeadDorado, EthernetOneHeadDorado) -> `RETURN
WITH state` to `Run[]` -> `DoInLoad[]` (the boot-file request over EFTP).
So **head-startup happens BEFORE DoInLoad**, and we fault DURING it -- which
is precisely why the germ never requests the OS. The faulting module is
**ProcessorHeadDorado** (G=`0o3400`, the loadmap's first module = our
`br31=3E15DC`). Since the germ is DESIGNED to start the heads with NO OS
present (germ source: "We can't set up handlers to catch Frame, Page, and
Write Faults because they require that the ProcessDataArea be already
initialized, and it's not until Pilot comes to life"), a ControlFault
during head-startup is an EMULATOR BUG, not an OS dependency.

THE FAULT (VERIFIED via symbolic microcode + per-instruction StkP trace --
this SUPERSEDES the earlier "indirect link 0o342 -> M[0o340]" reading,
which was a misread: `md=0o342` was stale latched data and there is NO
page-0 fetch). The head-startup runs `Call[<link>]` to start each head
(germ-6.1 StartCM does `IF frame ~= cm.frame THEN Call[MainBody[cm.frame]]`,
where `Call` = `zSFC`). At the **SFC** (op `0o342` -> `@SFC`, real
pc `0o1054`) the eval stack SHOULD hold the head's proc link. Trace at the
SFC opcode dispatch (cyc 68007693): **stkp=1, STK top = `0o201`** = a VALID
proc link (tag 1, **gfi 2 = DiskHeadDorado**). But the `@SFC` handler
(`DLink_ Stack&-1`) runs with **stkp=0** and pops 0 -> NullControl -> XFER
(`0o1700`) -> **ZERODEST** (`0o1705`) -> TrapParamSLink -> ControlFault
(T=`0o7`) -> germ SignalHandler -> germERROR.

WHERE THE LINK IS LOST (per-instruction StkP, decisive): the op before the
SFC is op `0o372` (a KFCB-family kernel call, 149 cycles, did an XFER that
RESUMES a context whose PC = pcf `0o437` = where the SFC is). That XFER
returns through **XFEREXIT**. `XferExitDispatch` increments StkP "in
anticipation of pushing DLink and SLink"; the no-push case `XferExit:` does
**`StkP-1`** to undo it AND `IFUNext0` (dispatches the SFC). The trace
shows StkP go 1->0 across XFEREXIT (real pc `0o2010` XferExitTable ->
`0o2026` -> XferExit) -- popping the head link `0o201` that the resumed
context's SFC needs. (NOTE: SwapTrap #2 is at cyc 68007090, BEFORE op
`0o372` at 68007544 -- so the SwapTrap is not mid-op-0o372; the whole
2-SwapTrap recursion led here, but this specific XFER is op `0o372`'s
kernel-call resume.) So the StkP anticipation/undo accounting is off by one
relative to the RESUMED context's stack -- the XFER restored the context
(stack + StkP from the saved state via LoadStack), and the XferExit
`StkP-1` then over-decrements, eating the SFC's link.

DEEPER TRACE (StkP, decisive -- the root recedes to xf.push/XFEREXIT):
- op `0o372` is **@LST** (Load State, real `0o11114`); alpha=`0o6` (germ
  code `M[0o17407044]` low byte), so it loads the state at L+`0o6`=`0o2656`.
  That state has **stkP=0** (M[`0o2674`]) but the DiskHead link **`0o201`
  at stack[0]** (M[`0o2656`]) -- so the link sits ABOVE TOS (scratch) and
  the resumed SFC (stkP=0) can't reach it.
- The state was written by **SaveState** (pc `0o2474`/`0o2430`, cyc
  68007287-94): it saved `0o201` to stack[0] AND stkP=0. So when SaveState
  ran, StkP was already 0 with `0o201` in the scratch slot -- the push that
  should have made StkP=1 did not stick.
- StkP trace upstream (cyc 68007150-68007295): **XferProc does `StkP+1`
  (anticipation, pc `0o4077` cyc 68007221), then XFEREXIT does `StkP-1`
  (pc `0o2026` cyc 68007261)** -> net 0. The link's `+1` is undone by the
  no-push XFEREXIT path. The XferExitTable dispatch picks no-push (`StkP-1`)
  vs push (XferPush, keeps DLink/SLink) based on **XferFlags `xf.push`**
  (set in XferDisp10 indirect: `XferFlags_ (XferFlags) OR (xf.push)`).

So the ROOT is the **`xf.push` / XFEREXIT push-vs-pop decision**: the
germ's recursive StartCM (which suspends/resumes via SaveState/LST around
the `Call[MainBody]`) expects the link to survive on the stack for the SFC,
but our XFEREXIT takes the no-push `StkP-1` path and the SaveState then
records stkP=0, so the LST resume + SFC pop hits an empty stack -> null ->
ControlFault. NOT YET FIXED -- this is a deep, subtle StkP/XferFlags
interaction across the XFER-anticipation, SaveState, and LST resume; a
wrong change risks the green gate, so it needs careful `xf.push` semantics
analysis (when should XFEREXIT push DLink/SLink vs StkP-1, and does our
XferFlags get `xf.push` set for the germ's StartCM call path?) before any
code change. Repro: `DORADO_XFER_TRACE=1
DORADO_TRACE_GATE="68007150,68007300"` shows XferProc `StkP+1` (pc `0o4077`)
then XFEREXIT `StkP-1` (pc `0o2026`); `DORADO_STORE_TRACE_VA=
"017402674,017402674"` shows SaveState (pc `0o2430`) save stkP=0; @LST real
`0o11114`, SaveState real `0o2440`, XferExitTable real `0o2010` (via
`mbdis --disasm 'chm/dorado/Cedar.mb!6'`). Repro: `DORADO_IFUDISP_TRACE=1
DORADO_TRACE_GATE="68007400,68007742"` shows op `0o372` (stkp=0) then op
`0o342` SFC dispatch (stkp=1, acs[0]=`0o201`); a per-instruction StkP trace
across cyc 68007688..68007696 shows the XFEREXIT `StkP-1` (pc `0o2026`)
drop stkp 1->0 before `@SFC` (pc `0o1054`). Module map:
`chm/cedar/germ/Dorado.loadmap!1.txt` (gfi 2 = DiskHeadDorado g=`0o4524`).
Symbolic microcode: `mbdis --disasm 'chm/dorado/Cedar.mb!6'` (real `0o1054`
@SFC, `0o1700` XFER, `0o1705` ZERODEST, `0o2010` XFEREXITTABLE).

### (SUPERSEDED) earlier reading: "genuine OS boundary" -- 2 handled sSwapTraps then a ControlFault on an OS-resident link
After 1187 clean dispatches (no hardware fault) the germ's
**module-startup chain (StartCM/Start)** reaches a module whose code is
marked **swapped-out** and resumes a saved StateVector whose control link
is **unbound** -> germERROR. Traced end-to-end:
1. The last real opcode is op `0o371` (2-byte, operand `0o7`) at pcf
   `0o2153` in module `br31=3E15DC` (codebase `0o17412734`). It XFERs to a
   target whose global frame (MDS+`0o3400`) has **code-base-low =
   `0o12735` (ODD)**. `LoadGC` tests "code base odd => swapped out"
   (`Branch[CSegSwappedOut, R odd]`, DMesaXfer.mc) and raises
   **`sSwapTrap` (T=`0o10`)** at `SavePCAndTrap` pc `0o2000` cyc 67995189.
   This is GENUINE germ data: the odd `0o12735` is written by the germ's
   own relocation BLT (pc `0o2761`, cyc 67969321) from the germ file --
   the germ deliberately marks module code swapped-out and clears it
   lazily on first call (no disk needed; the code is already resident).
2. `pSD[sSwapTrap] = CodeTrap` (TrapsImpl): CodeTrap correctly clears the
   out bit (stores `0o12734` EVEN at pc `0o343`, cyc 67995745) and calls
   `Start[[frame]]` -> StartCM -> StartWithState, allocating frames
   (AV[5], AV[6]) and recursing. ALL of this runs cleanly.
3. Inside that recursion, a `StartWithState` `s _ state^` BLT (pc `0o3561`,
   pcx `0o455`) copies the static StateVector template into `s`; one
   copied slot holds **`0o177774`** (genuine template data, like the old
   `0o26411`). `LoadState`/`Xfer` (pc `0o2372`/`0o1700`) then resumes `s`
   with DLink/SLink = `0o177774`; gfi = `0o177774>>6` = `0o1777` (far past
   the 128-entry GFT) -> unbound -> trap -> germ `SignalHandler`
   (`3E1E10`) -> germERROR (MP `0o1465`=821) -> `JB 0` spin.

The SwapTrap is genuine and CORRECTLY handled. The germERROR root is
**NOT YET PINNED** -- it lives in the post-SwapTrap start-chain/trap
recursion, NOT in the state resume. TWO specific root-cause leads in
earlier drafts of this entry were investigated with register-level
instrumentation and **BOTH DISPROVEN** (recorded here so the next session
does not re-chase them):
- WRONG LEAD #1: "`0o177774` is a genuine unbound control link / Stage-2
  OS boundary (sessions 14-19 class)." DISPROVEN: `0o177774` is just a
  STACK word (`stack[1]`) of a well-formed StateVector, legitimately
  loaded onto the eval stack by `XferNoBreak` ("Leave 2 words above TOS").
- WRONG LEAD #2: "the trap/`RETURN WITH state` resume reads the state at a
  base `-sizeStack` too low (`0o2641` vs `0o2657`), so DLink is garbage."
  DISPROVEN by direct A-bus instrumentation: at `LoadState` (pc `0o2362`)
  and `LoadStack` (pc `0o7040`) **RTemp0 = `0o7`** (the offset of `s` in
  frame L=`0o2650`, MemBase=L), and the fetches land on the CORRECT
  well-formed state -- SLink `0o2677`=`0o2314`, **DLink `0o2676`=`0o101`**
  (valid), stkP `0o2675`=`0`. The reads at `0o2657`/`0o2660` are the
  `XferNoBreak` 2-words-beyond-TOS stack load, not a wrong base. So the
  resume is CORRECT and the Xfer goes through the valid DLink `0o101`.

### WHAT IS ACTUALLY TRUE (instrumented, forward-traced)
- `StartWithState` builds a well-formed StateVector at `@s=0o2657`
  (DLink=`0o101`, SLink=`0o2314`, stkP=0) and `RETURN WITH s` resumes it
  correctly, XFERing through **DLink=`0o101` = gfi 1** (proc link).
- gfi 1 IS module `3E15DC` itself (codebase `0o17412734`); its global
  frame G=`0o3400` had code-base-low `0o12735` (odd=swapped-out), which
  CodeTrap cleared to `0o12734` (even) -- that store sticks (no re-set).
- The XFER to gfi 1 then re-enters `XferProc`/`StartCM`, allocates a frame
  (AV[0]=`0o3344`), and grinds the start chain: in the window after op
  `0o371` there are **3 SavePCAndTraps (all sSwapTrap T=`0o10`)**, **~50
  AV[0] allocations**, and the module codebase is read odd (`0o12735`) 4x
  / even (`0o12734`) 7x -- i.e. SEVERAL global frames share codebase
  `0o17412734` and each takes its own SwapTrap. After this bounded grind
  the germ germERRORs **without ever dispatching a new IFU opcode** (stuck
  at 1187 real dispatches through 90M cycles), so the start chain FAILS to
  reach any module body.

### RESOLVED: the germERROR is a genuine ControlFault (OS-dependency), NOT an emulator bug
The 3 traps in the recursion are, by their `SavePCAndTrap` T index
(`DORADO_XFER_TRACE` pc `0o2000`): **#1 sSwapTrap (T=`0o10`, cyc 67995189),
#2 sSwapTrap (T=`0o10`, cyc 68007090), #3 sControlFault (T=`0o7`, cyc
68007741)**. The first two SwapTraps are handled cleanly (CodeTrap clears
the out bit, Start recurses). The THIRD trap -- the one that becomes
germERROR -- is a **ControlFault (ZeroDest)**: an XFER through a control
link whose dest fetches 0.
- The faulting link is the germ **indirect control link `0o342`** (tag 2),
  read from TrapsImpl. `0o342` is GENUINE germ-file data: written by the
  germ's own relocation BLT (pc `0o2761`, cyc 67943017) into
  `M[0o17406747]`. Resolving it indirectly fetches `M[MDS+0o340/0o342]`
  which is **0** -> ZeroDest -> ControlFault -> `SIGNAL ControlFault`
  (trapsimpl-6.1.mesa) -> uncaught -> germ SignalHandler classifies it to
  germERROR (MP `0o1465`) -> `JB 0`.
- DECISIVE: `M[0o17400340]` (the indirect-link target) is **NEVER written
  by the germ** in the entire run (only the cyc-66M boot memory-clear
  zeroes it). It is a low-MDS system slot the germ expects to be live but
  that only the OS/Pilot initializes. So this is the SAME genuine
  control-link-to-resident-but-zero-memory wall session 17 validated --
  now reached MUCH deeper (the RisID fix carried the germ from the shallow
  `0o27132`/`0o26411` wall, which was MY BLT-overrun bug, through the full
  StartWithState + 2 SwapTraps to this genuine one).

So **the RisID Mar fix was the real (last) emulator bug on this path**; the
residual germERROR is the genuine **Stage-2 / OS-load boundary** the
project targets -- the germ's module-startup chain needs OS-resident
bindings (`M[0o340]` and the swapped-out code segments) that a germ-only
run cannot supply. The forward lever is unchanged: get the germ to
`DoInLoad` so it requests a boot file over EFTP (the Stage-2 server is
ready), per `docs/ethernet-local-boot-plan.md`.

CONFIDENCE NOTE: the trap type (ControlFault), the link's genuine
provenance, and `M[0o340]`=0-never-written are all solid + cross-checked.
Two EARLIER intermediate hypotheses this session (an unbound link
`0o177774`; a `-sizeStack` resume base) were DISPROVEN by instrumentation
-- recorded above so they are not re-chased. The one residual thread if
someone wants to be thorough: confirm the germ does not, in a correct run,
populate `M[0o340]` via some MDS-init step we skip (cross-check
`germopsimpl.mesa` low-MDS setup) -- but the evidence strongly favors
OS-resident init. Repro: `DORADO_XFER_TRACE=1 DORADO_TRACE_GATE=
"67995185,68050000"` (3 traps: 2x T=`0o10`, then T=`0o7` at cyc 68007741);
`DORADO_STORE_TRACE_VA="017400340,017400340"` (M[0o340] never written);
`DORADO_VMDUMP="017400340,017400346,68007740"` (target = 0).

## ROUTE B (2026-06-17, session 19 follow-up 6): the clobber is the Mesa BLT for `s _ state^` writing its DEST 11 words too high (0o2726 vs @s=0o2713). Source confirmed correct (state=0o7724, the static StateVector template). Root = the BLT dest (@s) computation is off by +11; exact cause NOT isolated. [SUPERSEDED by session 20 above: the +11 was the RisID Mar-substitution bug; the frame size (fsi 5) was correct and `s` is genuinely at offset 7.]

### Disassembly (added a DORADO_PCDIS microcode-disasm hook, cpu.c)
- The copy is the Mesa **BLT opcode (op 0o352)** at StartWithState pcf 0o455
  (DMesaRW.mc: `DoBLT` pops dest,count,source; copies `Fetch[base+source]^
  -> StoreMDS[dest]^`). Microcode loop pc 0o3524 (Fetch src) <-> 0o3537
  (Store dst) using FlipMemBase (FF=337) to toggle the src/dst base regs.
- **Source is CORRECT**: state = `0o7724` = the static StateVector template
  (legitimately UnboundLink-filled at the src-link offset).
- **Dest is WRONG**: the BLT writes `0o2726..0o2746` (starts at 0o2726, an
  offset-bug not a count-bug -- it does NOT write 0o2713..0o2725). `s`
  belongs at `0o2713` (LSTF alpha=7, L=0o2704; a 17-word StateVector fits in
  the 27-word fsi-5 frame at offset 7). Dest is off by exactly **+11
  (0o13)**: written at frame offset 18 instead of 7. This overruns into
  retFrame (0o2740), clobbering its return link.
- At the BLT dispatch (pcf 0o455) acs=[`0o7724`,0,0,`0o6126`], stkp=3 -- the
  dest `@s`/count are NOT plainly the simple stack values, so the dest is
  computed in a way I could not fully trace.

### Where it stands (honest)
The germERROR is now traced end-to-end to "the BLT for `s _ state^` puts its
dest 11 words too high, clobbering the caller frame's return link with
UnboundLink." But the EXACT cause of the +11 (a mis-read BLT dest operand /
local-address @s, a BR/MemBase base-reg off-by-11 in the FlipMemBase BLT, or
a frame/offset interaction) is NOT isolated -- store/XFER/BR tracing did not
converge, and the on-hand loadmap is germ!4 (not germ-6.1), so frame
numbers are approximate. The clean way forward is a DIFFERENT technique:
compare the BLT dest computation against `DMesaRW.mc DoBLT` step-by-step in
the emulator (the new DORADO_PCDIS hook disassembles microcode at a pc
range), or cross-check the StartWithState bytecode against a reference Mesa
emulator's frame layout. Repro: DORADO_PCDIS="03520,03545"
DORADO_TRACE_GATE="67990440,67990540"; the BLT is at pcf 0o455
(br31=3E0D58); dest stores at pc 0o3537 (STORE_TRACE_VA 017402726..017402746).

## ROUTE B (2026-06-16, session 19 follow-up 5): PRECISE narrowing -- StartWithState's frame is allocated fsi 5 (27 words) but its StateVector local needs fsi 7 (39 words). The frame-size index read (entry vector) or the proc-entry resolution gives the wrong (too-small) fsi. NOTE: the loadmap on hand is germ!4, not germ-6.1, so per-proc frame numbers are approximate. Fix not yet landed.

### Allocation trace (germ-6.1, decisive)
- AV free-list heads (MDS+`0o1000`+, cyc 67990300): AV[0]=`0o3344`,
  AV[1]=`0o2314`, AV[2]=`0o2430`, AV[3]=`0o2544`, AV[4]=`0o2620`,
  AV[5]=`0o2704`, AV[6]=`0o3104`, **AV[7]=`0o42` (EMPTY -- fsi-7 free list is
  a bare terminator)**, AV[9]=`0o3144`.
- The call into StartWithState reads its frame-size word at
  codebase+`0o11` = M[`0o17406541`] = `0o1005` = **AV[5] -> fsi 5** (XFER cyc
  67990314 T=`0o5`, 67990320 reads AV[5]=`0o2704`). So our emulator allocates
  StartWithState a **27-word (fsi 5) frame at `0o2704`**.
- The StateVector copy `s _ state^` (pc `0o3537`) writes `s` ending at
  `0o2746` -- past the 27-word frame (`0o2704..0o2737`) into retFrame
  (`0o2740`), clobbering retFrame.returnlink at `0o2742`. A StateVector
  local (~17-28 words, at a frame offset) needs a 35-39 word frame = fsi 7.
- StateVector def (AltoMesaProcess.mc): PrincOps `sizeStack = 0o16 = 14`
  stack words + stkp/dst/src control words.

### Candidate roots (for the fix -- one of these)
1. **Frame-size-index read off** (most concrete): XferProc reads the entry
   vector frame-size word (`CP+2n+3` lower byte = fsi). We read entry n=3 ->
   fsi 5. If the entry index `n` (from the proc link's epi) or the
   `CP+2n+3` address is off, we get the wrong (too-small) fsi. Check the
   proc-link epi extraction + the entry-vector indexing in the XferProc/
   LFC microcode path against `DMesaXfer.mc`.
2. **`@state` source pointer wrong**: the copy reads from `0o77xx` (source
   `state` = `0o7724`), but `@Start.state` should be in Start's frame
   (`~0o2744`, retFrame=`0o2740`). If `@state` (LADRB/param) is mis-computed
   the copy reads a static template (with UnboundLink) AND lands wrong.
3. **StateVector copy length / SIZE[StateVector]** too large (28 vs 17),
   overrunning a correctly-sized frame.

### NEXT PASS
Get/derive the germ-6.1 frame layout (the !1 loadmap is germ!4 -- find a
germ-6.1 loadmap or read StartWithState's entry-vector word directly from
the running VM at codebase `0o17406530`). Then: verify StartWithState's
CORRECT fsi vs our fsi 5, and trace the entry-index (epi) the LFC/SFC into
StartWithState resolved. If our epi/entry-vector read is off-by-one (a
sibling of the codebase G+0/G+1 and notLength bugs), that is the fixable
emulator bug. Repro: alloc at cyc 67990314 (fsi=`0o5`), copy at pc `0o3537`
cyc 67990602+, clobber M[`0o2742`] at cyc 67990672.

## ROUTE B (2026-06-16, session 19 follow-up 4): ROOT = StartWithState's frame is TOO SMALL for its StateVector local, so the `s := state^` copy overruns into the adjacent live frame (retFrame) and clobbers its return link. This is an fsi/frame-size bug (AV/AllocSub family, sessions 14/16). Fix not yet landed.

### The definitive mechanism
- `StartWithState` (TrapsImpl) has a local `s: StateVector`. Its first stmt
  `s _ state^` is a block copy (microcode pc `0o3537`/`0o3524`) of the whole
  StateVector (~17-28 words) into `s`.
- `s` lives in StartWithState's frame; `retFrame` (Start's frame, the caller)
  is the ADJACENT frame at MDS+`0o2740`. The copy's dest runs from ~`0o2726`
  up THROUGH `0o2740`, so it overwrites retFrame's L[2] (return link) at
  `0o2742`: the real `0o3234` becomes `0o26411` (UnboundLink, copied from the
  static template region `0o77xx`).
- StartWithState then reads retFrame.returnlink (now UnboundLink) into
  `s.source` -> resumed module RETs through UnboundLink -> sUnbound trap ->
  germERROR. (See follow-up 3 below for the store-by-store proof.)
- **fsi -> frame size (loadmap):** fsi 0=7, 1=11, 2=15, 3=19, 4=23, 5=27,
  6=31, 7=39 words. A proc with a StateVector local (~17-28w + 4 overhead)
  needs a LARGE frame (fsi >= 5/6/7). If our emulator allocates a smaller
  fsi for StartWithState, its StateVector local overruns the frame.

### THE FIX (next pass -- narrowed to fsi/frame-size)
Determine StartWithState's CORRECT frame-size index and compare to what our
emulator allocates:
- (likely) our XferProc/AllocSub reads StartWithState's fsi WRONG (too
  small) from the code-segment frame-size word. XferProc does
  `T_ DPF[T,10,10,MD]` (right byte = fsi); if the field extraction / the
  entry-vector frame-size word is read off-by-one or the wrong byte, the
  frame is undersized. Cross-check the DPF/fsi read in cpu.c against the
  StartWithState entry (TrapsImpl gfi `0o6`, codebase `0o17406530`, frame-
  size word at CP+2n+3 lower byte for entry n).
- OR the AV free list / AllocSub returns an overlapping frame (the
  AV[0]/free-list family from session 16). Dump AV (MDS+`0o1000`+) and the
  two frames to confirm sizes/placement.
Repro: the call into StartWithState is ~cyc 67990347 (AV read pc `0o4030`,
XferProc pc `0o4014`/`0o4034`); the overrunning copy is pc `0o3537` cyc
67990602+; the clobber of M[`0o2742`] is cyc 67990672 (see follow-up 3).

## ROUTE B (2026-06-16, session 19 follow-up 3): BUG LOCATION FOUND -- a block copy at microcode pc 0o3537 clobbers retFrame.returnlink (the real 0o3234) with UnboundLink (0o26411) BEFORE StartWithState reads it, so the resumed module's SLink becomes UnboundLink -> RET -> trap -> germERROR. This is a concrete clobber, traced store-by-store.

### The causal chain (store-traced, decisive)
1. retFrame = `0o2740` (= GetReturnLink[].frame, read from M[`0o2710`]); its
   return link is at L[2] = M[`0o2742`].
2. A call (XferProc, microcode pc `0o4034`, cyc 67989736) sets
   **M[`0o2742`] = `0o3234`** -- the REAL return link into the start chain.
3. **microcode pc `0o3537` (a block copy) OVERWRITES M[`0o2742`] with
   `0o26411` (UnboundLink) at cyc 67990672**, clobbering `0o3234`. The copy
   runs dest `0o2726..0o2746`, source `0o77xx` (= dest + `0o4776`, the germ's
   static template region where the relocation BLT splattered UnboundLink);
   it writes UnboundLink to L[2] of the live frame at `0o2740`.
4. `StartWithState` then reads retFrame.returnlink (now the clobbered
   `0o26411`, fetched at pc `0o600` cyc 67990779 from M[`0o2742`]) and stores
   it as `s.source` (pc `0o224`, cyc 67990784, to the StateVector SLink slot
   M[`0o2733`]).
5. LSTF resumes the module with SLink = UnboundLink; module RETs -> sUnbound
   trap (SD[`0o13`]) -> germERROR.

### What is pc 0o3537 / why does it overrun?
pc `0o3537`/`0o3524` is a block-copy loop (TrapsImpl bytecode ~pcf `0o455`,
br31 `0o17406530`) copying a static template from `0o77xx` into the frame/
StateVector region `0o27xx`. Its dest range (`0o2726..0o2746`, ~17 words)
RUNS INTO the live frame at `0o2740` (retFrame), overwriting L[2]. So either:
- (emulator) the block copy's count/dest is wrong -- it copies too many
  words or to the wrong base, overrunning retFrame (the same FSI/frame-size
  family as sessions 14/16: a frame allocated too small, or a copy length
  off); or
- (germ-state/overlap) retFrame and the copy's dest legitimately abut and
  the static template genuinely has UnboundLink there, in which case the
  germ expects this and our frame placement (AllocSub/AV free list) put
  retFrame where the template lands.

NEXT PASS (the fix): identify the pc-`0o3537` opcode at TrapsImpl pcf ~`0o455`
(disassemble / read the code bytes) and its intended dest+count; compare to
what our emulator's block-move executes. Check StartWithState's frame size
(fsi) and retFrame's placement -- is the gap (`0o2740`-copy_base) >= the
copy length on real hardware? If our frame is too small or the copy too
long, that overrun is the fixable emulator bug. Repro:
`DORADO_STORE_TRACE_VA="017402742,017402742"` shows the XferProc set to
`0o3234` (cyc 67989736) then the pc-`0o3537` clobber to `0o26411` (cyc
67990672); `DORADO_XFER_TRACE` gated `67990600,67990790` shows the copy loop
and the retFrame.returnlink fetch.

## ROUTE B (2026-06-16, session 19 follow-up 2): the StateVector is WELL-FORMED; SLink 0o26411 = UnboundLink. The germ resumes TrapsImpl with an UNBOUND source/return link, so finishing it traps -> germERROR. Two earlier claims CORRECTED.

### Corrected facts (dumped the StateVector at MDS+0o2713)
RTemp0 = `0o2713`; layout stack[0o2713..0o2730], brk,,stkP @ `0o2731`,
DLink @ `0o2732`, SLink @ `0o2733`:
- **stkP = `M[0o2731]` & 377 = `0o033407` & 377 = 7 -- VALID** (CORRECTION:
  the earlier "bogus StkP ~50" was germ!4's `0o7562`, wrongly carried over).
- **DLink = `0o601` -> gfi `0o6` = TrapsImpl -- VALID** (GFT[`0o6`]=`0o4764`).
- **SLink = `0o26411`** -- the only anomalous field.

### SLink 0o26411 = UnboundLink (confirmed)
- The germ relocation BLT (pc `0o2761`) writes `0o26411` to MANY slots
  (`0o17412646`,`0o17412463`,`0o17411114`,`0o17410140`,`0o17407740`,...).
  One value splattered across many control-link slots = the signature of
  **UnboundLink** (the loader fills every unbound-import slot with it).
- gfi = `0o26411` >> 6 = 180, deliberately PAST the 128-entry GFT so any
  call/return through it traps sUnbound -- exactly how UnboundLink is built.
- The loadmap "Unbound Imports" are real germ imports (ProcessorHeadDorado:
  DeviceCleanup, SoftwareTextBlt; DiskHead/EthernetHead: DeviceCleanup) --
  OS procedures the germ leaves unbound, each slot = UnboundLink = `0o26411`.

### So the actual question (narrowed, NOT yet an identified bug)
The germ resumes TrapsImpl (DLink valid) with **source link = UnboundLink**.
Per `DMesaXfer.mc` StartWithState (`s.source _ retFrame.returnlink`), the
resumed module returns to retFrame.returnlink when its body finishes; here
that is UnboundLink, so TrapsImpl's RET -> sUnbound -> TrapsImpl's sUnbound
handler (SD[`0o13`]=`0o631`) -> uncaught signal -> germERROR. For the germ
to boot, a module started in the StartCM/StartChain chain should return INTO
the chain (eventually to Initialize -> Run -> DoInLoad), NOT to UnboundLink.
So either:
- (emulator) our call/return or StartWithState path computes retFrame /
  retFrame.returnlink wrong, losing the chain back to Initialize; or
- (germ-state) the germ legitimately resumes a top-level process whose
  source is UnboundLink and TrapsImpl's unbound handler is SUPPOSED to
  transfer to the next phase (not germERROR) -- which would need a catch /
  state our germ-only setup doesn't provide.

NEXT PASS: read `germopsimpl.mesa` StartChain/StartCM + `trapsimpl-6.1.mesa`
sUnbound (UnboundProcedure) handler to learn the intended behavior of a
module-start returning to UnboundLink; and trace retFrame (the frame whose
returnlink is read as SLink) -- is its returnlink genuinely UnboundLink in
VM (germ-state) or did our call chain drop the real return link (emulator)?
Repro: dump the StateVector `DORADO_VMDUMP="017402713,017402734,67990800"`;
LoadState/LoadStack at pc `0o2362`/`0o7040` (cyc 67990859/67990869).

## ROUTE B (2026-06-16, session 19 follow-up): "first trap" investigation -- NO spurious early trap; the unbound link 0o26411 has an OUT-OF-RANGE gfi (180 vs GFT=128) + the resumed StateVector has a bogus StkP, so the state is ILL-FORMED. This REOPENS the emulator-bug possibility (LSTF/LoadState reads the state wrong) -- the earlier "genuine OS dependency / Stage-2 boundary" conclusion was PREMATURE.

### What the first-trap trace showed
- The germ takes only ONE real trap (`SavePCandTrap`, pc `0o2000`) in the
  whole run: the sUnbound (T=`0o13`) at cyc 67991921 when it XFERs through
  `0o26411`. There is NO spurious earlier trap. (An earlier `MTrap` at cyc
  67988524, T=`0o276`, resolves to SD[`0o276`]=`0o401` -> gfi `0o4`, a BOUND
  germ module -- handled, not a failure.) So the "a spurious trap cascades
  into germERROR" hypothesis is REFUTED.
- The germ runs cleanly until it RETs/XFERs through `0o26411`.

### The sharper finding: 0o26411 is an INVALID link (state is ill-formed)
- `Dorado.loadmap!1` says **GFT length = 128** (valid gfi 0..127). The Xfer
  extracts gfi = `0o26411` >> 6 = `0o264` = **180**, which is OUT OF RANGE.
  GFT[180] reads past the 128-entry table (MDS+`0o1664`, in the gfi
  `0o200`+ region) = 0 -> unbound. (The extraction is correct: the bound
  MTrap link `0o401` >> 6 = `0o4` indexes GFT correctly.)
- The resumed StateVector also has **brk,,stkP = `0o7562`** -> a loaded StkP
  ~`0o62` (50), far above the ~14 Mesa eval-stack max (session 19 anomaly).
- Two malformed fields (out-of-range SLink gfi + impossible StkP) in the
  SAME StateVector => the StateVector the germ resumes via **LSTF/LoadState**
  is ILL-FORMED. `0o26411` is genuine germ-file data (word `0o6740`), but it
  is being USED as a control link from a garbage state -- so either our
  LoadState/LSTF reads the StateVector from the wrong place/offsets (a
  fixable emulator bug, like the codebase off-by-one), or the germ saved a
  bad state earlier (DST/SaveState), or it should not be resuming this state.

### REVISED diagnosis + next step
The earlier conclusion ("0o26411 is a genuine unbound OS import; Stage-2
boundary") was PREMATURE -- an out-of-range gfi is not a normal forward
reference, it is garbage. The likely culprit is the **LSTF/LoadState
StateVector handling** (the same family as the WF/codebase emulator bugs).
NEXT PASS: trace LSTF/LoadState/LoadStack -- WHERE the StateVector is read
from (VM address), its full contents (stack, brk/stkP, DLink, SLink), and
whether our LoadState reads the right offsets for Cedar 6.1's StateVector
format (loadmap: "Statevector length = 65535", "Statevector counts" by FSI).
Cross-check `DMesaXfer.mc!1` LoadState/LoadStack (real pc `0o2362`/`0o7040`)
against the bytes. If the StateVector is read wrong, fixing it is the next
real emulator bug; if the state is genuinely garbage in VM, trace the
DST/SaveState that produced it. Repro: `DORADO_XFER_TRACE=1
DORADO_TRACE_GATE="67990600,67992000"` (the LoadStack at pc `0o7040`/`0o7034`
reads the state block; SLink `0o26411` lands in L[2] via XferProc pc
`0o4034`).

## ROUTE B (2026-06-16, session 19): codebase page-fault FIXED at the root -- a bare `IFetch_` was missing the HM-p.38 `BR[24:31]<-Id` replacement, so the Mesa read-double's field offset (alpha) was dropped and TrapsImpl read its codebase from G+0 instead of G+1. With the fix the germ-6.1 codebase read works and ALL page faults vanish; the germ advances **155 -> 188 real IFU dispatches** (TrapsImpl's startup now completes). Gate ALL GREEN. New blocker = an **sUnbound trap (SD[0o13]) on control link `0o26411`** -- a GENUINE germ control link (germ file word 0o6740), the same XferProc/SLink control-link-to-unbound-module wall as germ!4's 0o27132. NOT an emulator bug (an earlier draft wrongly suspected a relocation bug -- that was a hex miscalc; 0o26411 = 0x2D09 IS in the germ file).

CORRECTION/HONESTY NOTE: an earlier draft of this entry (and commit 2bbe17c)
said "155 -> ~35,000,000 dispatches." That 35M is the **JB-0 halt spin**
after germERROR, NOT real work. The accurate progress is **155 -> 188 real
dispatches** (count excluding the spin at pcf 0o5334). The fix is still a
real, correct, gate-safe microengine bug fix that eliminates the codebase
page fault; it just UNMASKS the next (signal) blocker rather than booting.

### THE FIX (landed, HM page 38)
`src/cpu.c`: an `IFetch_` is "a fetch for which BR[24:31] are replaced by
Id from the IFU." That replacement happens for **EVERY** IFetch; an
accompanying `TisId`/`RisId` is only needed to ADVANCE the ←Id cursor, NOT
to make the replacement happen. The code gated the `BR[24:31]<-Id` mix-in
(`ifetch_id_valid`) on a TisId/RisId function being present, so a **bare**
`IFetch_ X` got no Id mix-in and read at offset 0. Fix: peek the current Id
unconditionally and apply it for any `DM_REF_IFETCH` (the TisId/RisId block
still does its separate bus substitution + advance). One line of intent:
`uint16_t ifetch_id = ifu_peek_id(cpu); int ifetch_id_valid = 1;`.

Why this is the codebase off-by-one: the Mesa read-double `RDB`
(`DMesaRW.mc!1`: `T_ (IFetch_ Stack)+1`) reads `{[p+alpha], [p+alpha+1]}`
where the field offset **alpha rides in via the Id** (no TisId). For
`frame.code`, p = the global frame G and alpha = 1 (codebase at G+1). With
the Id dropped, BR[MDS] low byte stayed 0, so the fetch landed at
BR[MDS]+G = G+0 (the GFI word) instead of (BR[MDS]|1)+G = G+1
(codebase-low). The fix puts alpha=1 back into BR[24:31], so the read
lands at G+1 and the codebase resolves to VA 0o17406530 = 0x3E0D58 = the
TrapsImpl code base (br31).

GATE-SAFE: the Alto worlds run real AEmu microcode through the same IFetch
path; with the fix Galaxian stays 121553 (exact), NETEXEC 1478,
AltoMesaDorado 1479 (both in band), tests 10/10, sdl compiles. (Per HM the
replacement is always correct, so any Alto IFetch already had a 0 Id where
it relied on no mix-in; none regressed.)

### Original root-cause writeup (now resolved by the fix above)

### The bug, fully grounded
TrapsImpl (running, br31=`0o17406530`=0x3E0D58) reads its own 2-word
**codebase LONG POINTER** (`frame.code`, via `GetCodeBytes[frame][frame.pc]`
to test a trap instruction). It reads the pair from global-frame offset
**G+0** when it must read **G+1**. Per PrincOps (DMesaDefs.mc!2 line 203,
"Global frame configuration"):
```
  G      GFI,,code links(1 bit)
  G+1    code base low      <-- the codebase LONG POINTER is here
  G+2    code base high
  G+3    global 0
```
VM dump at TrapsImpl's global frame (G=`0o4764`, VA `0o17404764`) confirms
the 3-word `{GFI, cb-lo, cb-hi}` record exactly:
```
  M[0o4764]=0o000611 (GFI)   M[0o4765]=0o006530 (cb-lo)   M[0o4766]=0o000076 (cb-hi)
  M[0o4770]=0o000711 (GFI)   M[0o4771]=0o007471 (cb-lo)   M[0o4772]=0o000076 (cb-hi)
```
The germ read the codebase pair at `{M[G], M[G+1]}` = `{0o615, 0o6530}` =
`{GFI(patched), cb-lo}` and built BR[`0o34`] = bank `0o6530` -> VA
`0xD580687`, a VACANT page -> page fault (155-dispatch hang). The correct
codebase pair is `{M[G+1], M[G+2]}` = `{0o6530, 0o76}` = VA `0o17406530` =
0x3E0D58 = **br31 itself** (the code base already in use). G itself is
computed CORRECTLY (microcode pc `0o4014` stores G=`0o4764`); only the
+1 to skip the GFI word is missing.

### Proof + payoff (the landed IFetch-Id fix)
With the fix (`ifetch_id` peeked + applied for every IFetch):
- **ALL page faults vanish** (`DORADO_FAULT_TRACE=all` shows only the 2
  known early-bootstrap va=0 faults). The codebase read now lands at G+1,
  so the "germ references unmapped memory" blocker is GONE.
- germ advances **155 -> 188 real IFU dispatches**: TrapsImpl's startup
  body now runs to completion (~123 dispatches, ops incl. the codebase
  read) and returns via FREE (op 0o347) to its caller (br31=3E1E10).
- That caller is the germ's SignalHandler / error classifier (the
  JGEB/JGB/JLEB range-check dispatch, dispatches 177-188), which maps the
  raised condition to the generic **germERROR (MP 821 = `0o1465`)**, calls
  SetMP, and does `JB 0`. The 35M total dispatch count is this halt spin
  (pcf 0o5334), NOT progress.

### How it was traced (for the record)
The codebase-read opcode is the Mesa read-double `RDB` (op `0o104`,
`DMesaRW.mc!1`: `T_ (IFetch_ Stack)+1`), in the pcf `0o1056..0o1062` region
(TrapsImpl code VA ~`0o17407157`). It reads `{[p+alpha], [p+alpha+1]}` with
the field offset `alpha` carried by the IFU Id. The code-byte dump showed
the operand **alpha=`0o001`** right after the opcode (`M[0o17407160]` low
byte) -- exactly the +1 the codebase needs. Our emulator dropped it because
the `BR[24:31]<-Id` mix-in was gated on a TisId/RisId being present; `RDB`
has none. The bad read was at microcode pc `0o521/0o522`, cyc 67992448 (VA
`0o17404764` = G+0; should be G+1 = `0o17404765`). Same IFU operand-byte
class as the `notLength` (session 12) and `TisId`/`alpha` (session 15)
fixes.

### NEXT PASS: the downstream software germERROR (MP 821) at ~35M dispatches
After the fix the germ runs 188 real dispatches then raises a generic
germERROR (MP 821 = `cGermERROR`) and `JB 0` halts. TRACED TO THE ACTUAL
FAULT (an earlier "StartFault" guess in this doc was WRONG): it is an
**sUnbound trap (SD[`0o13`])** -- at cyc 67991921 `SavePCandTrap` (pc
`0o2000`) fires with trap index **T=`0o13`** (= sUnbound; SD[`0o13`]=`0o631`
= the TrapsImpl unbound handler). The germ XFERs through control link
**`0o26411`** (a procedure link, tag=1) which is unbound -> trap ->
TrapsImpl (reads the faulting instr via the now-fixed codebase read) ->
raises an uncaught signal -> the germ's SignalHandler (br31=3E1E10, the
JGEB/JGB/JLEB classify) maps it to the generic germERROR.

Chain of `0o26411` (the unbound link) -- CORRECTED:
1. `0o26411` is **GENUINE static germ-file data** -- germ file word `0o6740`
   reads big-endian `0o026411` (= 0x2D09). An earlier draft of this entry
   wrongly said "NOT in the germ file / relocation-produced / emulator bug"
   -- that was a hex miscalculation (`0o26411` was mistakenly treated as
   0x2B09 instead of 0x2D09; the grep searched the wrong value). The germ's
   pc-`0o2761` BLT is a **plain copy** (reads `md=0o26411`, writes it
   unchanged) that relocates the germ image from its initial low-bank load
   (BR[`0o31`], bank-0 `0o7740`) up to MDS (BR[`0o34`]). No relocation
   arithmetic touches the value. So this is the SAME class as germ!4's
   `0o27132`: a genuine germ control link, NOT an emulator-computed value.
2. LSTF/`LoadStack` (pc `0o7040`/`0o7034`) loads a saved **state vector**
   whose SLink = `0o26411`.
3. `XferProc` (pc `0o4034`, the same microcode that stored germ!4's
   `0o27132`) stores SLink=`0o26411` into the new frame's L[2]
   (`M[MDS+0o3346]`). The frame: L[0]=`0o4764` (= TrapsImpl's global frame
   G, VALID), L[1]=`0o107`, **L[2]=`0o26411`** (the unbound SLink).
4. RET/XFER through L[2] -> Xfer tag=1 (procedure, gfi field `0o26411>>2`)
   -> the gfi's global frame is unbound in our germ-only setup -> trap.

So the codebase IFetch-Id fix removed a REAL emulator bug (the page-fault
symptom) and advanced the germ to the SAME genuine germ-state wall as
sessions 14-19: the germ resumes a saved boot-process / start-list whose
control links (`0o27132` for germ!4, `0o26411` for germ-6.1) reference
modules our germ-only environment never binds/starts. This is NOT a
microengine bug -- both links are genuine germ data.

RESOLVED (session 19, follow-up): decoded the `0o26411` proc link fully.
The Xfer microcode (DMesaXfer.mc!1 XferTagOdd/XferDisp01) extracts
**gfi = link >> 6 = `0o26411` >> 6 = `0o264`** (=180) and indexes the GFT
at base **MDS+`0o1400`** (1-word frame-pointer entries): GFT[gfi] =
M[`0o1400`+gfi]. GFT[`0o264`] = M[MDS+`0o1664`] = **0** -> unbound -> trap.
The germ's GFT binds **only 10 modules, gfi `0o1`..`0o12`** (all germ
modules; TrapsImpl = gfi `0o6`, frame `0o4764`); GFT[`0o264`] is null IN
THE GERM FILE ITSELF (file word `0o664` = 0). So gfi `0o264` is an
**OS/Pilot module, not a germ module** -- the germ never binds it; Pilot
binds it only after the OS loads. `0o26411` is therefore a genuine
**unbound import** to the not-yet-loaded OS: a real Stage-2/OS-dependency
boundary, NOT a fixable StartCM gap and NOT a microengine bug.

OPEN QUESTION (the only remaining emulator angle): WHY does the germ's
188-dispatch startup path reach a CALL to this unbound OS import before
`DoInLoad`? Either (a) the germ legitimately calls it and expects it bound
(genuine OS dependency -> the path forward is Stage-2: get the germ to
`DoInLoad` so the OS supplies gfi `0o264`), or (b) an emulator control-flow
mis-step earlier in the 188 dispatches branched the germ into this call
when it should have skipped it. Distinguishing (a)/(b) needs decoding the
188-dispatch path against `germopsimpl.mesa`/`bootswapgerm` -- a deep germ-
state trace. The germ does NOT yet reach `DoInLoad` (only the Stage-1 ether
TX; no germ `0244` Mayday). Repro: `DORADO_XFER_TRACE=1
DORADO_TRACE_GATE="67990600,67992000"` (LoadStack reads SLink `0o26411`;
XferProc pc `0o4034` stores it to L[2]; Xfer resolves gfi `0o264` ->
GFT MDS+`0o1664`=0 -> SavePCandTrap pc `0o2000` T=`0o13` at cyc 67991921).
GFT dump: `DORADO_VMDUMP="017401400,017401414,67991000"` (gfi 1..12 bound).

### HARD REGRESSION GATE -- ALL GREEN
1. `make test` = 10/10 suites.
2. Galaxian @160M: 121553 px (exact).
3. AEmu NETEXEC: 1478 px (band); AltoMesaDorado.eb!2: 1479 px (band).
4. germ-6.1: 155 -> 188 real dispatches; page faults eliminated; then the
   germERROR/JB-0 halt spin (the 35M total is the spin, not progress).
5. `make sdl` compiles.


## ROUTE B (2026-06-16, session 16): bug 3 (`AV[0]=0o2`) FIXED at the root -- the `Q<-B` side-effect was missing for the Pipe external-B sources, so the Mesa `Q_ VALo` idiom never loaded Q. With it fixed the germ runs its real boot prologue (163 bytecodes -- LFC/EFC calls, DST state-dumps, no hardware faults), announces `germStarting` (MP 810), then -- ROOT FOUND (session 17) -- hits the SAME `0o27132` ControlFault as session 14, now cleanly trapped (the germ installed SD handlers first), which raises an uncaught `SIGNAL ControlFault` -> germERROR (821) -> deliberate `JB 0` halt. So the WF+Q<-B fixes converted the old infinite trap-loop into a clean halt but the `0o27132` SLink-to-unbound-memory bug is unchanged and is THE blocker. Gate ALL GREEN. (See the session-17 NEXT-blocker section below; germ Mesa sources in `chm/cedar/germ-src/`.)

### Root cause (HM Table 7 asterisk, one missing case)

`AV[0]` was seeded `0o2` because the XFER FREE path (`XferFreeQ`,
`DMesaXfer.mc`) does `Q_ VALo` (= `Q_ B<-Pipe1`, the VA-lo of a preceding
`DummyRef` to `frame-1`), then `RTemp0_ (Fetch_ Q)+1` and `AV[fsi]_
RTemp0`. The pipe held the correct VA (`0x3F06E3` = `0o17603343`, traced
via `DORADO_PIPEVA_TRACE`), and `B<-Pipe1` delivered VaLo=`0o3343` on the
bus -- but **Q stayed `0o1`**, so `RTemp0 = Q+1 = 0o2` (the bad value) was
stored as the freed frame instead of `0o3344`.

`src/cpu.c ff_override_b`: HM Table 7 asterisk -- "when an external B
source is in play AND BSEL=3, the external value also lands in Q" -- was
applied ONLY in the FB=7 block (Link/CPReg, line ~794), NOT in the FB=6
block (the Pipe/Config/FaultInfo sources). So `Q_ B<-Pipe1` (and any
`Q_ B<-Pipe*`) delivered the value on B but never loaded Q. Fix: add the
same `if (u->bsel == 3) cpu->Q = *b;` at the FB=6 block's return. (This is
the same Q<-B mechanism Bootstrap's `Q_ Link` snapshot relies on -- it
was just never wired for the pipe sources.)

### Result (verified)

- The free now stores the real freed frame `0o3344` to `AV[0]` (was `0o2`);
  the fsi-0 free list is intact. The germ no longer self-loops in
  `AllocSub`.
- Germ: **21,942,131 IFU dispatches** (was 110) -- it runs the real
  BootSwapGerm/Pilot boot code (RET, EFC/LFC calls, jumps, ALU/field ops)
  and settles into a HEALTHY busy-wait: op `0o210` (JB) at pc `0o150`,
  br31 `3E1D0C` (BootSwapGerm), `flt=0`, NO trap PCs. Stack at the wait:
  `acs=0,0o1465,0o617,0`.
- No germ-originated outbound Pup yet (only the Stage-1 n=13 boot request
  at cyc<31M); 0 display pixels.

### NEXT blocker (session 17) -- the germ raises germERROR (821) and deliberately halts

DECODED (the JB self-loop is NOT an interrupt-wait -- it is a deliberate
halt after an error). Sequence, fully traced + cross-referenced against
the germ Mesa source (now downloaded to `chm/cedar/germ-src/`):

1. The germ runs its real boot prologue -- **163 bytecodes**, br31
   `3E1D0C` (BootSwapGerm) and `3E0D58`: LFC/EFC calls, RET, **DST (Dump
   State, op `0o370`)**, comparisons. NO hardware faults anywhere
   (`flt=0` throughout).
2. Early it calls **`SetMaintPanel`** (MISC `alpha=0o10`) with MP code
   **`0o1452`=810 = `germStarting`** ("germ entered") -- traced live at
   the `MaintPanel_ T` instruction (real pc `0o4041`, cyc 67975655). So
   the germ announces a healthy start.
3. It then hits an error via a **`KFCB` (Kernel Function Call, op `0o347`,
   pcf `0o735`)** -> the germ's error/trap path (DST + the classify
   dispatch at pcf `0o3720..0o3756`, conditional `JGEB`/`JLEB` on an error
   code), which loads **`LIW 0o1465`=821 = `germERROR`** ("unnamed
   ERROR") and calls `SetMaintPanel` again (cyc 67980386, T=`0o1465`).
4. Then **`JB 0`** (op `0o210`, disp 0 = jump-to-self) at pcf `0o3756` --
   the germ's deliberate halt-after-error. The ~21.9M dispatch count is
   almost entirely this JB spin.

So the germ gets a long way (state-dumps, kernel calls) then raises an
**uncaught Mesa ERROR**. It set the GENERIC `germERROR` (821), NOT a
specific boot code (`germNoPhysicalBootFile`=824 / `germNoServer`=828 /
`germBadBootFile`=823), which leans toward an UNEXPECTED condition (an
emulator mis-model surfacing as an uncaught signal) rather than the clean
"no OS volume" path -- though a legitimate boot-channel failure is not
ruled out.

ROOT FOUND (session 17): the germERROR is the **same `0o27132` ControlFault
as session 14**, now cleanly trapped instead of looping. Full chain:
1. The germ runs its boot prologue and **installs its SD trap handlers**
   (writes SD[7]=sControlFault, SD[8], SD[`0o13`]=sUnbound, ... at cyc
   67978776-67979040; SD[7] <- `0o633` = TrapsImpl.ControlFaultTrap).
2. TrapsImpl's `XferProc` stores **SLink=`0o27132`** into a new frame's
   L[2] (return link) at MDS+`0o2346` (cyc 67978520, pc `0o4034`,
   br31 `3E0D58`=TrapsImpl). `0o27132` is GENUINE germ data (session 14:
   it lives in the germ file at MDS+`0o7652`, BLT-copied around).
3. That frame RETs: pc `0o1034`(RET) -> reads L[2]=`0o27132` (cyc
   67979221, lva `0o17403346`) -> `0o1026`(XferMD) DLink=`0o27132` ->
   `0o1700`(Xfer) tag=2 indirect -> fetch MDS+`0o27132` (VA `0o17427132`)
   = **0** -> ZeroDest -> **ControlFault (T=`0o7`)** (cyc 67979268).
4. Now that SD[7] is bound, the ControlFault XFERs to
   `TrapsImpl.ControlFaultTrap` -> `SIGNAL ControlFault[source]` (a
   resumable PUBLIC SIGNAL, `trapsimpl-6.1.mesa` line 20/209). Uncaught ->
   the germ's `SignalHandler` (`bootswapgerm-indigo.mesa` line ~550:
   `SetMP[code]; Halt[]`) classifies it (JGEB/JLEB range checks) to the
   generic **germERROR (821)** and does the `JB 0` halt.

So my WF + Q<-B fixes advanced the germ PAST the point where it installs
handlers, converting the old infinite ControlFault trap-loop (sessions
8-14) into a clean germERROR halt -- but the **underlying `0o27132`
SLink-to-unbound-memory bug is UNCHANGED** and is now THE blocker again.

VALIDATION EXHAUSTED -- `0o27132` is NOT a microengine bug (session 17,
follow-up 3). Every emulator path around this fault has been checked and
is CORRECT: (1) the germ file loads flat 1:1 (verified vs GERM FILE MAP,
0o40 pages); (2) `0o27132` is genuinely in the germ file at MDS+`0o7652`;
(3) the BLT that copies the static process-descriptor record reads the
intended source MDS+`0o7642` (germ-computed, stack value `0o7640` confirms
intent), not a stray pointer; (4) the LSTF/LoadState reads SLink from the
correct state-vector offset (block+`0o20`, validated vs LOADSTATE/LOADSTACK
microcode); (5) no SD/IFU fault precedes it. So `0o27132` is a GENUINE germ
control-link constant that the germ deliberately installs as a resumed
process's SLink/return-link, and it references MDS+`0o27132` -- a
resident-but-zero page PAST the entire loaded germ (data+code end at
MDS+`0o21000`) that nothing ever initializes.

CONCLUSION: this is a germ-LOAD / process-init / OS-dependency issue, not a
microengine defect. The germ is starting modules / setting up initial
Pilot processes from a static descriptor table (MDS+`0o7642`), and one
descriptor's control link points to MDS+`0o27132` which is never loaded.
Most likely it is (a) an OS-resident link that should only be live AFTER
DoInLoad loads the OS (our germ never reaches DoInLoad -- it faults during
its own module init), or (b) the germ relies on an MDS region / unbound
import that Pilot fills, which our germ-only setup never provides. Getting
PAST this is therefore Stage-2 territory (loading a real OS boot file via
the boot channel), or requires identifying a germ MDS-init step that
populates `0o27132` -- NOT a microcode/field/pipe fix. The 3 emulator bugs
fixed this session (WF/RF, TisId/RisId+IFetch, Q<-B) were the real
microengine defects on the path here; the germ now runs its full boot
prologue + module-startup chain and stops at this genuine germ/OS-load
boundary. NEXT real lever: bring up the Stage-2 OS load
(`docs/ethernet-local-boot-plan.md`) so DoInLoad has an OS to inLoad, OR
decode the static process-descriptor table (MDS+`0o7642`) against the germ
process-init source to learn what `0o27132` should resolve to and whether
the germ is meant to defer that process until after the OS loads.

MECHANISM FULLY DECODED (session 17, follow-up 2) -- source-grounded via
`chm/cedar/germ-src/trapsimpl-6.1.mesa` + `germopsimpl.mesa`. The germ is
running its **module-startup chain**: GermOpsImpl drives a **StartList**
(from the boot-file header, `header.pStartListHeader`) -> `HeadStartChain.
Start` -> `TrapsImpl.Start` -> `StartCM` (recursive over module deps,
explaining the deep StkP ~`0o62`) -> **`TrapsImpl.StartWithState`**:
```
StartWithState: PROC [frame, state] = {
  s: StateVector _ state^;
  retFrame _ GetReturnLink[].frame;
  s.dest _ MainBody[frame];                 -- DLink = module main body (=0o601, TrapsImpl)
  SetReturnLink[s.source _ retFrame.returnlink];  -- SLink = CALLER frame's return link
  RETURN WITH s };                           -- resume module (microcode LSTF/LoadState)
```
So **SLink=`0o27132` is `retFrame.returnlink`** -- the return link of the
frame that called StartWithState (the StartCM/StartChain call chain). The
resumed module (TrapsImpl main body) runs, then its RET XFERs through
SLink=`0o27132` (tag-2 indirect) -> fetch unloaded MDS+`0o27132`=0 ->
ControlFault -> germERROR. `0o27132` traces back to a control link in the
germ's STATIC start-list data (germ file MDS+`0o7652`), copied via stores
into frame L[2]/return-link slots, then propagated as the SLink.

So the ControlFault is the germ's startup chain having a **return link
(`0o27132`) into MDS that is never loaded/initialized** (resident-but-zero,
past the `0o40`-page germ). NEXT PASS: decode the StartList format in
`germopsimpl.mesa` (StartChain / pStartListHeader / `StartList.Base`) and
whether `0o27132` is (a) a link into a non-resident germ/OS region the
StartList shouldn't be starting yet, (b) a return link our emulator's
call/return (SFC/LFC) mis-stored somewhere up the chain, or (c) a
start-list our germ-plant set up wrong. Trace `retFrame` (the StartCM
frame) and where its `returnlink` got `0o27132`: `DORADO_STORE_TRACE_VA`
on the retFrame's L[2], and the StartList header from the boot-file header
our germ-plant fakes (`src/machine.c` germ pass1/3). All germ sources are
in `chm/cedar/germ-src/` (germopsimpl, trapsimpl, bootswapgerm, mpcodes,
pilotmp).

----
SLink OFFSET VALIDATED (session 17, follow-up) -- the SLink read is
CORRECT; `0o27132` is genuinely the SLink. NOT an emulator addressing bug.
The LSTF `LoadState` (real `0o2362`) does `T_ RTemp0 + Add[sizeStack,2]C`
with FF=`020` => sizeStack+2 = `0o20`, and `LoadStack` (real `0o7040`)
fetches `RTemp0 + sizeStack+1` = `RTemp0+0o17`. Traced live: that fetch is
at lva `0o2733`, so **RTemp0 = `0o2713`**, giving the state-vector control
words: brk,,stkP = M[`0o2731`]=`0o7562`, DLink = M[`0o2732`]=`0o601`
(a VALID TrapsImpl proc, tag=1 gfi=6 -- the resumed proc DID run), and
**SLink = M[`0o2733`] = `0o27132`** (= block base+`0o20`, the correct
source-link offset). So our emulator reads SLink from the right slot.

The germ DELIBERATELY puts `0o27132` there: it BLTs a template
(M[`0o2733`]<-`0o11420` at cyc 67977403) then OVERWRITES the DLink/SLink
slots (M[`0o2732`]<-`0o601`, M[`0o2733`]<-`0o27132` at cyc 67977597,
pc `0o224`, br31 TrapsImpl). `0o27132` is a COMPILE-TIME CONSTANT in the
germ's static data (germ file MDS+`0o7652`). The target VA `0o17427132`
is RESIDENT (map idx 3E53, rp 3E53, identity-mapped) but ZERO -- valid
memory with nothing loaded there (it is past the 8192-word loaded germ).

ANOMALY to chase: brk,,stkP=`0o7562` yields a loaded StkP ~`0o62` (50),
far above the ~14 Mesa eval-stack max (the resumed proc shows stkp=`0o63`).
A bogus StkP hints the whole resumed StateVector may be ill-formed.

So the real question is now squarely a GERM-STATE / LOAD question, not an
emulator microcode bug: WHY does the germ resume a process whose return
link (`0o27132`) and StkP point at memory that is zero in our load?
Hypotheses for NEXT PASS: (a) the germ image/MDS should be larger or
extended at runtime (the boot file is 8192 words; `0o27132`=11866 is 3674
words past it) -- check whether the germ allocates/inits MDS past
`0o21000` (ResidentMemory.AllocateMDS in `bootswapgerm-indigo.mesa`, or a
GERMREMAP step); (b) the static link `0o27132` needs RELOCATION at load
that we skip (germ loaded flat 1:1 at MDS+`0o1000`); (c) this is a
genuinely uninitialized/OS-resident link and the germ should not be
resuming this context yet (trace the DST/SaveState at dispatches 56/144/145
that built the saved state -- decode against `GermOpsImpl.mesa`, not yet
downloaded). Repro: `DORADO_XFER_TRACE=1 DORADO_TRACE_GATE="67977660,
67977700"` shows LoadState/LoadStack reading the block; the bad SLink
store is `DORADO_STORE_TRACE_VA="017402733,017402733"` (data=`027132` at
67977597); ControlFault at cyc 67979268. Sources in `chm/cedar/germ-src/`:
`trapsimpl-6.1.mesa`, `bootswapgerm-indigo.mesa`.

### HARD REGRESSION GATE -- ALL GREEN
1. `make test` = 10/10 suites.
2. AEmu NETEXEC @200M: **1477** px (band 1476-1505). PASS.
3. Galaxian @160M: **121553** px (=121552 +/-1). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1473** px. PASS.
5. `make sdl` compiles.

## ROUTE B (2026-06-16, session 15): the 113-dispatch ControlFault is FIXED -- root-caused to TWO grounded emulator bugs in the Mesa field-opcode path and both fixed in `src/cpu.c`. (1) `WF<-A`/`RF<-A` were UNIMPLEMENTED STUBS; (2) `TisId`/`RisId` never substituted the IFU operand byte onto the bus (and `IFetch<-` never did the `BR[24:31]<-Id` replacement). With both fixed, the Mesa `WF` opcode `T_(IFetch_Stack&-1)+T, TisId` now computes the field pointer as `Stack&-1 + alpha` (was `Stack&-1 + staleT`), so it no longer corrupts `TrapsImpl`'s code base (`g[1]` stays `0o6530`, was clobbered to `0o11602`). The germ NO LONGER ControlFaults at the LSTF resume. NEW blocker found and PROVEN: a separate pre-existing bug seeds `AV[0]=0o2` (a self-looping AllocSub terminator) where the germ file has `0o6` (indirect->fsi 1); forcing `AV[0]=0o6` lets the germ run **~22 MILLION dispatches** of real boot code and settle into a HEALTHY wait loop (op `0o210` JB at pc `0o150`, NO trap PCs). Gate ALL GREEN.

### The two fixed bugs (HM + schematic grounded)

**Bug 1 -- `WF<-A`/`RF<-A` (load ShC from a Mesa field descriptor).**
`src/cpu.c ff_apply_post` FF 5/5 and 5/6 were `/* shifter ctrl TBD */`
no-ops, so the Mesa Read/Write-Field opcodes ran `ShMdBothMasks`/
`ShiftLMask` against a STALE `ShC`. Implemented `field_desc_to_shc()`.
The exact transform is NOT in any manual (both say only "transform the
bits appropriately") -- it is the **"Shift Register Control" hardware on
ProcL sheet 18** (`DoradoDocs/doradodrawings/ProcL-Rev-Ci.press!1.pdf`
p.19, `ProcL18.sil`), which tabulates, for descriptor `A[8:15]=(P<<4)|S`
(P=position `A[8:11]`, S=size=width-1 `A[12:15]`; `ShC[2:3]<-A[2:3]`):
```
        Shift Count    Right Mask     Left Mask
   RF   P + S + 1      (don't care)   16 - S - 1
   WF   16 - P - S - 1 16 - P - S - 1  P
```
(The PROM grab confirms the LMASK/RMASK PROMs are just count->mask
tables; the field->count transform is hardwired combinational logic, not
a PROM. Sources saved in `chm/doradosource/{ProcProms,IFUProms,
DoradoProms}.bcpl`.)

**Bug 2 -- `TisId`/`RisId` bus substitution + `IFetch<-` BR replacement.**
HM p.24: "RFfoA and WFfoA ... also load ShC[2:3] from A[2:3]"; p.38:
"IFetch<- -- A fetch for which BR[24:31] are replaced by Id from the IFU
... the IFU does not advance ... so an accompanying TisId or RisId
function is needed to advance." Our code consumed the Id post-ALU
(advancing) but never put it on the bus, so the WF's `T_(IFetch_Stack&-1)
+T, TisId` used the STALE T on the B bus instead of `alpha`. Live trace
(corrupting WF, cyc 67976936): `a=Stack&-1=0o4700, alpha=0, oldT=1` ->
we computed `p=0o4700+1=0o4701` (= `g[1]` codebase) and wrote the field
there; correct is `p=0o4700+alpha=0o4700` (= `g[0]`). Fix: a pre-ALU
block peeks the current Id and substitutes it for the matching bus source
(TisId: B<-T / A<-T; RisId: B<-RM / A<-RM explicit form), and the mem-ref
VA does `BR=(BR&~0xFF)|Id` for `DM_REF_IFETCH`. The cursor still advances
exactly once (the existing post-ALU `ff_apply_post` consume); the pre-ALU
peek does NOT advance, so both see the same byte. Added `ifu_id_at()` +
`ifu_peek_id()` (refactor of `ifu_consume_id`).

GATE-SAFE / verified: the AEmu and AltoMesaDorado gate worlds never run
`WF<-A`; the TisId/RisId substitution is HM-correct and the full gate
stays green (NETEXEC 1481, Galaxian 121553, AltoMesaDorado 1489, 10/10
tests, sdl compiles). The earlier transient NETEXEC 1472 was banner
host-time render variance, not a regression (see memory
`aemu-pixel-gate-not-ground-truth`).

### NEW blocker (session 16) -- bug 3: `AV[0]` seeded with `0o2` not `0o6`

After the WF fix the germ reaches the LSTF process-resume cleanly, which
allocates an fsi-0 frame via `AllocSub` reading `AV[0]` (MDS+`0o1000`).
But `AV[0]` has been overwritten with `0o2` -- a tag-2 indirect that
`AllocSub` resolves to `RSH[0o4000+0o2,2]=AV[0]` (SELF -> infinite loop).
The germ FILE's fsi-0 free list (head `0o3354`) terminates with `0o6`
(tag-2 indirect -> fsi 1: `RSH[0o4000+0o6,2]=AV[1]`), the correct
"fsi-0 exhausted, fall through to fsi 1". So `0o2` is wrong; `next_fsi`
came out 0 instead of 1 (lost bit 2 = the `0o4`).

PROOF this is THE next wall: env-forcing the `0o2`->`0o6` store made the
germ jump from **110 -> ~21.9M dispatches** and run real boot code (RET,
EFC/LFC calls, jumps, ALU/field ops -- ops 350/010/111/070/126/164/057/
343/245/244/072/033/370/364) before settling into a healthy busy-wait
(op `0o210` JB at pc `0o150`, br31 `3E1D0C` = BootSwapGerm; the XFER tail
at 74.9M is ALL pc `0o150`, NO trap PCs). The germ is now WAITING (likely
for the next boot phase / OS load / a device), not faulting.

WHERE the bad `0o2` is written: a Store at **pc `0o271`** (the generic
`0o263`->`0o271`->`0o272` fetch/modify/store helper) at cyc **67975458**,
running inside **BootSwapGerm** (br31 `0o17416414`), stores `RM/STK=0o2`
to `AV[0]`. This is PRE-EXISTING (A/B identical with the WF fix gated on/
off), so it is a SEPARATE bug -- some BootSwapGerm AV-init/relocation
arithmetic computes `0o2` where `0o6` is intended. NEXT PASS: trace the
RM/STK source feeding the pc-`0o271` store back to where `next_fsi`/the
terminator is computed (is it a field/shift dropping bit 2, or an
off-by-one in the fsi-chain build?). Repro:
`DORADO_IFUDISP_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 80000000` -> 110
dispatches; `DORADO_STORE_TRACE_VA="017401000,017401000"` shows the
`data=0o2` store at cyc 67975458 pc `0o271`. To watch the unblocked run,
temporarily force the store (`if va==017401000 && b==2 then b=6` in
`src/memory.c DM_REF_STORE`) -> ~22M dispatches. Germ file fsi-0 chain +
terminator `0o6` are in `chm/cedar/germ/Dorado.germ!4` (head AV[0]=`0o3354`);
loadmap FSI/next-FSI table in `chm/cedar/germ/Dorado.loadmap!1.txt`.

### HARD REGRESSION GATE -- ALL GREEN
1. `make test` = 10/10 suites.
2. AEmu NETEXEC @200M: **1481** px (band 1476-1505). PASS.
3. Galaxian @160M: **121553** px (=121552 +/-1). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1489** px. PASS.
5. `make sdl` compiles.

## ROUTE B (2026-06-15, session 14): the prior `BLTC`/`LSTF`-`alpha` hypothesis is REFUTED with hard evidence -- alpha is read CORRECTLY; the 113th-dispatch ControlFault is the germ resuming its saved boot-process via `LSTF`/`LoadState`, whose loaded DLink/SLink and the resumed proc's fsi are GENUINE germ data that reference MDS locations past the loaded 8192-word germ image. No emulator IFU/operand bug found; NO code change made (tree clean). Next blocker is upstream germ process-resume state.

### What was tested and REFUTED (the session-13 lead)

The session-13 handoff guessed the corruption was a `BLTC` block-transfer
whose `dest` overlapped an `LSTF` state vector because `LSTF`'s `alpha`
operand byte was mis-read (a sibling of the `notLength` IFU bug). Every
part of that is wrong:

1. **`BLTC` (op `0o354`) never dispatches.** The only block transfer in
   the germ run is **`BLT` (op `0o352`)** at `br31=3E0D58` `pcf=0o455`
   (opcode bytes verified by VMDUMP: `M[0o17406756]=0o003752`, low byte
   `0o352`). `BLT`/`BLTC` are length-1 and take dest/count/source off the
   Mesa eval stack -- they have NO `alpha` operand, so an alpha mis-read
   cannot affect them. (Opcode map from `DMesaDefs.mc!2`: `BLT=0o352`,
   `BLTC=0o354`, `LST=0o371`, `LSTF=0o372`, `RET=0o343`.)

2. **`LSTF`'s `alpha` is read CORRECTLY = `0o007`.** `LSTF` (op `0o372`)
   dispatches at `pcf=0o477`; VMDUMP `M[0o17406767]=0o167772` -> low byte
   `0o372` (= LSTF), and `M[0o17406770]=0o003777` -> high byte (pcf=`0o500`,
   even = high byte for insset<2) = `0o007`. The engine's `<-Id` delivers
   N=1 (`xf.free`) on the first read and `alpha=0o007` on the second
   (`ifu_consume_id`, `src/cpu.c`), exactly matching the code byte. The
   state vector legitimately sits at `L+alpha = L+7`. `<-Id`/operand fetch
   is FINE. (The germ ran 113 dispatches incl. many EFC/LFC proc calls
   through the same `<-Id` path, all correct.)

3. **`0o27132` is GENUINE germ data, faithfully loaded.** It lives in the
   germ FILE at `MDS+0o7652` (big-endian `0o027132`, confirmed by reading
   `Dorado.germ!4` directly), inside a static ~7-word-record array at
   `MDS+0o7642..` (repeating `0o177660`/`0o55000,0o55001`/`0o3564`
   pattern). `BLT` faithfully copies it; nothing corrupts it.

### The real failure chain (re-diagnosed, traced live ~cyc 67.97-67.99M)

The 113th dispatch is **`RET` (op `0o343`, vec `0o1034`)** at `br31=3E1382`,
running inside a context the germ just **resumed via `LSTF`**:

- `LSTF` (the only `LST*` in the whole run) runs `LoadState`/`LoadStack`
  (`DMesaXfer.mc!1` line 980-1036): loads StkP/DLink/SLink/Stack from the
  state block at `L+alpha`, then `MemBase_ MDS, Branch[Xfer]`.
- The loaded **DLink = `0o173`** -> `Xfer` tag dispatch -> `XferDisp01`
  (proc descriptor) -> `XferProc` (`DMesaXfer.mc!1` line 729): allocate a
  frame, patch links, start the IFU.
- `XferProc` reads the proc's **fsi** from its code-segment frame-size
  word (`T_ DPF[T,10,10,MD]`, MD=`0o2323` -> right byte **fsi=`0o323`**).
  `AllocSub` (`DMesaXfer.mc!1` line 539) fetches `AV[0o323]` (`AV=0o1000`,
  so `MDS+0o1323`) = **0**, `BDispatch 0` -> tag-0 "good frame" -> returns
  **frame = 0**. So the resumed proc runs in **L = MDS+0** (low core).
  The proc runs 3 bytecodes (`067`, `210`=JB, `343`=RET).
- `XferProc` stored the new frame's `L[2] = SLink` (= the LSTF-loaded
  **source link `0o27132`**). With L=0, that store landed at `MDS+2`.
- `RET` (`IFUP[RET,1,L,N[2]]`) fetches `L[2]` = `MDS+2` = `0o27132`,
  XferMD sets DLink=`0o27132`, `Xfer` sees tag = `0o27132 & 3 = 2`
  (INDIRECT) -> `XferDisp10`: `Fetch_ T` (MemBase=MDS) at **`MDS+0o27132`
  = VA `0o17427132`**, which is **past the 8192-word germ** (germ occupies
  `MDS+0o1000..0o21000`) -> **md=0** -> ZeroDest -> `ControlFault` (T=`0o7`)
  -> `SD[7]`=0 -> the steady `0o1700/0o1026/0o1041/0o1600/0o2000` trap loop.

NOTE: even with a proper (nonzero) frame, `RET` would still read
`L[2]=SLink=0o27132`, so the **fatal** issue is the indirect SLink past
the germ; the `fsi=0o323`/`frame=0` is a SEPARATE anomaly (the proc still
runs). Both derive from the LSTF-loaded state block.

### Verdict

NOT an IFU `<-Id`/operand/`alpha` mis-model (refuted above), NOT a `BLTC`
bug (no BLTC), NOT a load/BLT corruption (`0o27132` is genuine). The
blocker is in the germ's **boot-process resume**: the state block that
`LSTF` loads yields a DLink (`0o173`)/SLink (`0o27132`) and an fsi
(`0o323`) that reference MDS locations past the loaded germ. Either (a)
the germ is supposed to build/relocate more MDS state (frame heap / the
target of indirect link `0o27132`, the global frame for DLink `0o173`'s
gfi, and the `AV[0o323]` slot) BEFORE this resume, and an upstream
emulator mis-model dropped that setup; or (b) the state block being
resumed is sourced/relocated wrong. The AV free lists ARE initialized
(verified vs the germ file: `AV[1]/AV[5]` show normal partial
consumption, `AV[0]` consumed to a tag-2 indirect terminator `0o2`; germ
chains terminate with tag-2 indirect markers `0o16/0o12/0o6`, never 0)
-- so it is NOT a wholesale heap-init failure; it is specific to the
`fsi=0o323` size class and the `0o27132`/`0o173` links.

### NEXT PASS (session 15) -- where to dig

This is genuinely the FIRST `LSTF` in the run and the first germ
process-resume, so the bug (if emulator) is exercised only here and is
gate-safe to chase (Alto worlds never run `LST*`). Concrete steps:

1. **Read the `BLT` (op `0o352`, `pcf=0o455`) operands.** Source ran
   `MDS+0o7642`, dest `MDS+0o2730` (from XFER trace). Confirm dest/count/
   source popped off the stack are what the germ intends, i.e. that the
   state block the LSTF later loads is sourced from the right place.
   (Augment the `IFUDISP` trace to print `STK[stkp-1..stkp-3]`, the
   popped args, since the current trace only prints `STK[stkp..+3]`.)
2. **Decode the resumed proc.** DLink=`0o173` -> gfi -> `GFT[gfi]` global
   frame (traced fetch `MDS+0o1406` -> `md=0o4700`) -> entry vector ->
   code. Is `fsi=0o323` genuine for that proc, or is `GFT[gfi]`/the code
   base (`LoadGC`) resolved wrong (the MemBase/BR family of sessions
   9-13)? `DPF[T,10,10,MD]` right-byte extraction worked for 100+ earlier
   proc calls, so suspect the proc identity (DLink/GFT), not DPF.
3. **Is `MDS+0o27132` ever supposed to be bound?** Check whether the germ
   writes a valid control link there during a setup step we mis-emulate
   (gate a `DORADO_STORE_TRACE_VA` on `0o17427132` across the whole run).
   If nothing ever writes it, the resume is premature/the state block is
   wrong; if a store was dropped/faulted, that drop is the bug.

Repro: `DORADO_IFUDISP_TRACE=1 ./build/dorado --eb
'../chm/dorado/CedarDorado.eb!6' --germ '../chm/cedar/germ/Dorado.germ!4'
--cycles 68100000` -> 113 dispatches; `RET` at cyc 67978584. Gated
`DORADO_XFER_TRACE` with `DORADO_TRACE_GATE="67976000,67979000"` shows the
LSTF LoadState, the `XferProc` alloc (frame=0), and the failing indirect
`Xfer` at `MDS+0o27132`. Opcode/alpha bytes via
`DORADO_VMDUMP="017406750,017406772,67978000"`.

### REGRESSION GATE -- NOT RUN (no code change)

Session 14 made **no emulator code change** (the prior hypothesis was
refuted; any fix to the shared XFER/alloc/`<-Id` path would be
speculative and risk the gate). `git status --short` clean except this
doc. The 5-item gate is therefore unaffected from session-13's all-green
state.

## ROUTE B (2026-06-15, session 13): `LongFetch` ignored the high address bits from B -- fixed the VA computation in `src/cpu.c`; the EFC4 (op `0o304`) external-link read now lands at the right code-segment word; germ jumps from **53 to 113 IFU dispatches**, running through **3 code modules** of Pilot/germ startup; new blocker = another ControlFault on an indirect control link that resolves to 0

### The stall: op `0o304` = EFC4 (External Function Call 4)

Op `0o304` is row `30x` col 4 of `DMesaDefs.mc!2` `MesaOps[...]` = **EFC4**.
Its handler (`DMesaXfer.mc!1` `IFUP[EFC4,1,G,N[4]]` -> `EFCM1`) does
`Fetch_ 0S, Call[GetLinkID]` then XFERs through the fetched external
control link. `GetLinkID` reads the external link at code-segment offset
`~ID` (= `-(ID+1)` = -5 for EFC4) via **`LongFetch_ T, B_ RTemp0`** with
`MemBase=Code`, RTemp0=-1. The link lives at the negative offset C-5
(VA `CP-5` = `0o17416407`, which holds the valid link `0o000605`).

### Root cause (emulator mis-model, HM-grounded)

`src/cpu.c` computed every memory-reference VA as `va = br + mar`, where
`mar` is only the **16-bit** A bus. For `LongFetch` that drops the high
address bits. Per HM "Processor Memory References" (LongFetch entry):
*"A fetch for which the complete 28-bit VA is `(B[4:15]^Mar[0:15]) +
BR[MemBase]`"* and "LongFetch takes the low 16 bits of address from
RM/STK and **high 8 bits from B**." With B=RTemp0=`0o177777`, the
displacement is a full 28-bit `-5`, so VA = CP-5. Our model used only
Mar=`0o177773` (-5 as unsigned 16 bits), so the bank-carry pushed VA up
one 64K bank to `0o17616407` (verified: that page is zero), read md=0 ->
DLink=0 -> ZeroDest -> ControlFault(7) -> SD[7]=0 -> infinite trap loop.

### The fix (committed) -- HM-grounded

`src/cpu.c` memory-ref VA: for `DM_REF_LONGFETCH`, form the displacement
as `((B & 0o7777) << 16) | (Mar & 0xFFFF)` (B[4:15] supplies VA[4:15]);
all other reference kinds keep the plain 16-bit Mar. Confirmed by VMDUMP:
intended CP-5 (`0o17416407`) holds `0o000605`; the old wrapped address
(`0o17616407`) holds 0. **Alto worlds stay correct** because their
LongFetch high bits come out the same (only the high-B contribution is
new, and microcode written for real HW always loads B for a LongFetch);
all pixel gates unchanged within band.

### Result (verified)

- Germ: **113 IFU dispatches** (was 53). After EFC4 it XFERs into a new
  module (br31 `3E1D0C` -> `3E0D58`, 57 dispatches), then a third
  (`3E1382`, 3 dispatches), running real Pilot/germ startup bytecodes.
- **New blocker:** after the 113th dispatch (op `0o343` RET, vec `0o1034`,
  cyc 67978584) the returned control link `0o27132` is an **indirect**
  link (tag=10); Xfer fetches the real link at VA `0o17427132` (MDS+`0o27132`,
  *past* the 8192-word germ extent `<=0o17421000`) and gets **md=0** ->
  ZeroDest -> ControlFault(7) at pc `0o1711` -> `SD[7]`=0 -> the same
  `0o1700/0o1041/0o1026/0o2000/0o1600` trap loop, now much further along.
- Still **no own outbound Pup** (`DORADO_ETH_TX_TRACE` shows only the
  Stage-1 n=13 boot request at cyc<31M); 0 display pixels.

### HARD REGRESSION GATE -- ALL GREEN

1. `make test` = **10/10** suites.
2. AEmu NETEXEC @200M: **1482** px (band 1476-1505). PASS.
3. Galaxian @160M: **121553** px (=121552 +/-1). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1480** px (band 1466-1505). PASS.
5. `make sdl` compiles.

### NEXT PASS (session 14)

Decode the RET (op `0o343`, the 113th dispatch) and the indirect control
link `0o27132` it returns into. Is VA `0o17427132` (beyond the loaded germ)
a location the germ binds during later startup (missing state we should
seed), or is the returned link itself corrupt (another addressing/BR
mis-model in the RET / frame-return path, the family of sessions 9-13)?
The RET path: pc `0o150`(RET) -> `0o1034` -> `0o1053` -> `0o1026`(XferMD,
DLink=md=`0o27132`) -> `0o1700`(Xfer) -> tag=10 indirect -> fetch
`0o17427132` md=0 -> ControlFault. Repro:
`DORADO_IFUDISP_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 120000000` -> 113
dispatches; gated `DORADO_XFER_TRACE` at `67978584,67978720` shows the
failing indirect XFER; `DORADO_VMDUMP="017427100,017427140,67978620"`
dumps the (zero) indirect-link region.

## ROUTE B (2026-06-15, session 12): IFUM Length field is `notLength` (`~length`), not `length-1` -- fixed in `ifu_decode_lh`; germ jumps from 10 to **53 IFU dispatches** (past the op-`002` / SD[7] ControlFault); new blocker = germ stops dispatching after the 53rd bytecode and spins, having issued NO outbound Pup (does not yet reach its own Ethernet volume fetch)

### The fix (committed `57b6d18`) -- microcode-grounded (`chm/microd/mdfields.d` TIFUM)

`src/cpu.c ifu_decode_lh` decoded the 2-bit IFUM Length field as
`ifu_length = lpr + 1`. Per `mdfields.d`'s `TIFUM` struct the field is
**notLength** = `~length & 3`, so `length = ~lpr & 3` (field 2 -> len 1,
1 -> len 2, 0 -> len 3). The old `lpr+1` form is correct ONLY for the
length-2 case (field 1), which is why it survived every Alto gate (all
Alto/Nova opcodes are 2 bytes). It mis-sized every length-1 and length-3
opcode, corrupting the saved Mesa PC in `SavePCInFrame` for the
PrincOps/Mesa instruction set (e.g. 1-byte `LFC4`/`NOOP`).

Three synthetic IFU tests in `tests/test_cpu.c` (`test_ifu_conditional_
dispatch`, `test_ifu_conditional_cond_true`, `test_ifu_dispatch_
synthetic`) encoded their length-1 INC/HALT opcodes with the old
`length_p = 0`; updated to `2` (notLength of length 1) so they assert the
same intended opcode length under the corrected decode. Length-2 cases
unchanged.

### Result (verified)

- Germ: **53 IFU dispatches** (was 10). It now executes a long run of real
  Cedar/Mesa bytecodes (ops `263 170 071 256 343 060 056 264 163 070 100
  050 374 364 331 020 022 350 111 164 344 304` ...).
- **New blocker:** after the 53rd dispatch -- op `0o304` at real `pc=0o761`,
  IFUM vec `0o764` -- the germ stops issuing opcodes and spins in microcode
  for the rest of the 120M-cycle budget (0 further IFU dispatches).
- **Does NOT yet reach its own Ethernet volume fetch.** The only outbound
  TX (`DORADO_ETH_TX_TRACE`, n=13) fires at cyc < 31M -- that is Stage-1
  `EtherMicrocodeBoot`'s request, before the world loads. The germ runs at
  cyc 67M+ (pass3 data load @67.28M) and issues **zero** Pups; no outbound
  `0244` Mayday. The 2 fetch faults (`pc=0o6023/0o6024`, va=0) are early
  bootstrap-phase (cyc ~11.3M), unrelated to the germ.
- 0 display pixels.

Next: disassemble Cedar op `0o304` (insset=1) and trace the microcode at
IFUM vec `0o764` / real `pc=0o761` to find why the germ halts dispatch
there (waiting on a device/memory cell? a fault that self-loops? needs the
keyboard/DDC back-channel to pick a boot mode?). Repro:
`DORADO_IFUDISP_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 120000000`.

### HARD REGRESSION GATE -- ALL GREEN

1. `make test` = **10/10** suites (with the 3 test-encoding updates).
2. AEmu NETEXEC @200M: **1481** px (band 1476-1505). PASS.
3. Galaxian: **121553** px @160M (=121552 +/-1); 121641 @122M, 124239 @118M
   -- animation-frame variance, NOT a regression (all Alto opcodes are
   length-2, so the decode is bit-identical old vs new). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC: **1475/1484/1488** px @200/195/205M
   (band 1466-1505; banner renders host time so it varies run-to-run;
   menu renders). PASS.
5. `make sdl` compiles.

## ROUTE B (2026-06-15, session 11): the boot XFER's `XferProc` now stores BootSwapGerm's saved-G (frame[0]) correctly (`0o4634`); `LoadGC` loads G correctly; the swap-trap loop is GONE; the germ now runs 10 bytecodes (was 6) -- 4 more after the prologue RET; new blocker = a later XFER (after the 10th bytecode, op `002`) hits a ControlFault into the still-empty SD trap table

### Root cause of the session-10 blocker (XferProc stored frame[0] = `0o10210` instead of `~0o4634`)

ONE emulator mis-model, microcode-grounded. `DMesaXfer.mc!1` `XferProc`
(PrincOps) does, after allocating the frame:

```
	RTemp0_ T, MemBase_ G;          * real 0o3360, FF=0o251 = MemBaseX G (BRX[G,1])
	RTemp1_ (RTemp1)+(RTemp1);
	DummyRef_ 0S, T_ MD, StkP+1;    * real 0o4030: Get VA of G[0]
	...
	T_ VALo;                        * T_ global frame pointer = lo(VA of G[0])
	T_ (Store_ RTemp0)+1, DBuf_ T;  * L[0] _ G
```

`DummyRef_ 0S` references **BR[G] + 0**, so `VALo` = lo(BR[G]) = the global
frame pointer (`0o4634`, loaded by the boot LoadGC). The compiled form is
`asel=1 (Fetch<-RM/STK), lc=3 (T<-Md), FF=000` -- the displacement (A bus)
is the small constant **0** carried by FF=000.

`src/cpu.c` had a blanket DummyRef special case:
`if (kind==DM_REF_DUMMYREF && asel==1 && lc==3) mar = cpu->T;`. It was added
for InitMem.mc `NextMapEntry`'s `DummyRef_ T, T_ MD` (real `0o7000`), but
that construct is `asel=1, lc=3, FF=021` (FA=0 FB=2 FC=1 = the memory-
subdecode **A<-T** override). XferProc's DummyRef has the **same asel/lc**
but `FF=000` (A<-0), so the blanket test caught BOTH and forced Mar=T for
XferProc too: VA = BR[G] + T = `0o4634 + 0o3354` (T held the just-allocated
local frame `0o3354`) = **`0o10210`**, the exact corrupt frame[0]. The
frame-return LoadGC then read a garbage global frame -> swap/MTRAP loop.

### Fix (committed) -- microcode-grounded (InitMem.mc!1 / DMesaXfer.mc!1) + HM Table 8a DummyRef

`src/cpu.c`: **delete the `mar = cpu->T` blanket special case**; DummyRef Mar
is just the A bus (`mar = a`), exactly like any processor ref. The
displacement is already carried correctly by the FF memory-subdecode A-source
override (`ff_a_override`, applied to `a` at the `a = ovr` line before the
ref): InitMem's FF=021 -> `a = T`; XferProc's FF=000 -> `a = 0`. So both
constructs now compute the right Mar without a per-construct hack. This is the
**MemBaseX/BR-resolution family** the last two passes worked (the `MemBase_L`
fix was BRX[L,0]; here the path runs through `MemBase_G` = BRX[G,1]), but the
actual bug was the DummyRef displacement, not the MemBase decode (which
correctly resolves G=1).

**Why the Alto worlds stay correct:** the deleted hack was redundant for
InitMem (ff_a_override already yields `a = T` for FF=021), and the Alto worlds
do not run DMesaXfer's `DummyRef_ 0S`. The shared Mesa-VM XFER path is
exercised by AltoMesaDorado, which stays green.

### Result (verified)

- `XferProc`'s DummyRef now references VA `0x3E099C` (lo `0o4634`, was
  `0x3E1088`/`0o10210`); `VALo`/frame[0] = `0o4634` = BootSwapGerm's G.
- The frame-return `LoadGC` loads G correctly; **the swap-trap loop is GONE**.
- The germ runs **10 IFU dispatches** (was 6): the prologue
  (LFC4/LI1/`165`/`263`/`361`/RET) returns cleanly into BootSwapGerm, which
  then dispatches **4 more** bytecodes: op `100`, `000`, `000`, `002`.

### HARD REGRESSION GATE -- ALL GREEN

1. `make test` = **10/10** suites.
2. AEmu NETEXEC @200M: **1493** px (band 1476-1505). PASS.
3. Galaxian @160M: **121553** px (=121552 +/-1). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1490** px (band 1477-1502;
   NetExec menu renders). PASS.
5. `make sdl` compiles (no test_cpu.c:7709 warning reproduces; the line is a
   plain printf).

### NEW blocker (session 12): a later XFER (after the 10th bytecode, op `002`) ControlFaults into the empty SD trap table

After op `002` (10th dispatch, pc `0o6`, vec `0o470`), control enters a Mesa
XFER/trap loop: `XFER`(0o1700) / `XFERMD`(0o1026) / `SAVEPCANDTRAP`(0o2000) /
`SAVEPCINFRAME`(0o1600) / `MTRAP`(0o1041), steady with **T=`0o7`**
(= `sControlFault`). The XFER reads `SD[7]` and gets **md=0** (lva `0o17401107`)
-> ZeroDest -> ControlFault(7) -> re-fault forever -- BootSwapGerm has not yet
populated the SD trap handlers. So either op `002`'s XFER targets a legitimately
unbound control link (germ/boot state not yet set up by these first 10
bytecodes) or another emulator XFER mis-model surfaces only on this 11th
transfer. NEXT PASS: decode op `002` (insset=1) and the control link it XFERs
through (`DORADO_XFER_TRACE` gated ~67975700..67976000; the trap is T=`0o7`,
md=0 at SD[7]). Repro:
`DORADO_IFUDISP_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 80000000` -> 10 dispatches
then the ControlFault loop; the first XFER is at cyc 67974929, the 10th
dispatch ~67975700.

## ROUTE B (2026-06-15, session 9): the prologue RET's ZeroDest is FIXED (two grounded bugs); the RET now reads the correct return link and returns cleanly into BootSwapGerm; new blocker = the frame-return LoadGC mis-loads G because the boot XFER stored BootSwapGerm's saved-G (frame[0]) wrong

### Root cause of the session-8 blocker (RET reads a zero return link -> ZeroDest)

TWO independent bugs, both in the shared Mesa-VM XFER path, masked each other:

1. **`MemBase_ L` mis-decoded (emulator, HM Table 11d).** `MemBase_ L`
   (L = base register 0) compiles to the **MemBaseX form** FF=`0o250`
   (FA=2 FB=5 FC=0). Our `ff_apply_post` handler for "MemBaseX <- FF[6:7]"
   reused the OLD MemBase bits: `(MemBase&0o30)|((MemBase>>1)&3)|((ff&3)<<3)`,
   so with MemBase=MDS(`0o36`) it produced `0o33`, NOT 0 (=L). HM Table 11d
   (FA=2): "MemBase[0]<-0, MemBase[1:2]<-MemBX[0:1], MemBase[3:4]<-FF[6:7]"
   => `MemBase = ((MemBX&3)<<2)|(FF[6:7])` (the SAME construction the IFU
   uses at the `MemB[0]==0` case). Fix in `src/cpu.c`. Effect: the local
   frame base register L was never loaded with the allocated frame -> L=0 ->
   SavePCInFrameIL computed SLink=0 -> the LFC4-called proc's returnLink
   (`L[2]`) = 0 -> RET ZeroDest.

2. **pipe_push fault-preservation guard blocked the emulator's OWN store
   (emulator, HM page 51).** With #1 fixed, the RET then read return link
   `0o20056` (still wrong). `SavePCInFrameIL` does `Store<-1S; SLink<- T AND
   VALo` where `VALo` = `B<-Pipe1` (the VA of its own just-issued store, read
   back from `pipe[ProcSRN]`). The session-7 guard that preserves the
   first-faulting pipe entry was skipping **every** push to `pipe[ProcSRN]`
   while `fault_count>0` (stuck at 15 in the Cedar path) -- including the
   emulator's own `Store<-1S` -- so `VALo` read a stale IFU code VA
   (`0o20057`) instead of the store's VA. HM page 51 ("IFU References"): the
   hardware "disables IFU references when the processor is either making a
   reference or doing ... B<-Pipei", so only the **asynchronous IFU
   prefetch** can clobber the private slot, never the emulator's own ref.
   Fix in `src/memory.c`: restrict the guard to `DM_REF_IFETCH`/
   `DM_REF_PREFETCH`. Effect: `VALo` now reads the store's VA, SLink =
   `0o3354` (BootSwapGerm's frame), RET reads `L[2]=0o3354` and returns.

### Result (verified)

- The prologue RET (op `343`) now reads return link `0o3354` and returns
  cleanly into BootSwapGerm. **ZeroDest fault is GONE** (0 hits around the
  RET). Control runs on into the frame-return path (XferDisp00 -> LoadGC).

### HARD REGRESSION GATE

1. `make test` = **10/10**.
2. AEmu NETEXEC @200M: **1491** px (band 1476-1505). PASS.
3. Galaxian @160M: **121553** px (=121552 +/-1). PASS.
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1499-1502** px (3 runs).
   NOTE: this shifted UP ~12 px from the HEAD baseline (1485-1495). Isolated:
   the shift is ENTIRELY from the (correct, HM Table 11d) `MemBaseX<-FF[6:7]`
   fix -- AltoMesaDorado also uses that FF form, and the old code gave a
   buggy result there too. The pipe-guard fix alone reproduces the baseline
   (1489-1496). The world still boots and renders the NetExec menu correctly;
   the +12 px is benign NetExec host-time render-timing from a correct fix.
   The AltoMesaDorado band should be recalibrated to ~1485-1502.
5. `make sdl` compiles (links `build/dorado-sdl`; no `test_cpu.c:7709`
   warning reproduces).

### NEW blocker (session 10): frame-return LoadGC mis-loads G; BootSwapGerm's saved-G (frame[0]) was stored wrong at the boot XFER

After the RET returns to BootSwapGerm (frame `0o3354`), `XferDisp00` calls
`LoadGC` to restore G/Code from the frame's word 0 (the saved access
link/G). But **frame[0] = `0o10210`**, not BootSwapGerm's G (~`0o4634`).
LoadGC therefore reads a garbage global frame -> unbound/swap trap (T=`0o10`)
-> MTRAP -> `SD[0o10]`=0 -> ZeroDest loop. **Still 6 IFU dispatches.**

Where frame[0] got corrupted: at the **boot XFER's `XferProc`** (DMesaXfer.mc
`L[0]_G`, line 766 `T_ (Store_ RTemp0)+1, DBuf_ T`). The stored G comes from
line 765 `T_ VALo`, which reads the VA of the preceding line-763
`DummyRef_ 0S` at G[0]. Traced (gated): that DummyRef referenced VA
`0x3E1088` (lo `0o10210`) -- which matches NEITHER `BR[G].lo`=`0o4634` NOR
`BR[Code].lo`=`0o16414` as loaded by the boot LoadGC (BRLO_FF trace:
`0o3612` mb=01 a=`0o4634`; `0o3604` mb=37 a=`0o16414`; `0o3641` mb=37 hi=`0o76`).
So the base register **selected/resolved at the line-763 DummyRef (real PC
`0o4030`, MemBase shown =`0o1`=G)** is wrong -- the same MemBase/BR-resolution
family as bug #1, but in the XFER setup. NEXT PASS: gate-trace the **at-issue
MemBase index AND the resolved BR value** at PC `0o4030` (~cyc 67975051) plus
every BR[1]/BR[037] load between the boot LoadGC and that DummyRef; decide
MemBase-load timing vs a stray BR reload. (`0o4030` is not listed by
`mbdis --disasm` -- likely a placement detail; find it via image index.)

Repro: `DORADO_IFUDISP_TRACE=1 ./build/dorado --eb
'../chm/dorado/CedarDorado.eb!6' --germ '../chm/cedar/germ/Dorado.germ!4'
--cycles 80000000` -> 6 dispatches (LFC4/LI1/165/263/361/RET) then the
LoadGC-trap loop; first XFER at cyc 67974929, RET at ~67975296. Useful gated
traces: `DORADO_XFER_TRACE`, `DORADO_BR_TRACE`, `DORADO_PIPEVA_TRACE`
(all need `DORADO_TRACE_GATE="lo,hi"`).

## ROUTE B (2026-06-15, session 8): germ FILE was loaded LITTLE-endian but Dorado memory is BIG-endian; fixing the byte order makes the first XFER complete and the germ now DISPATCHES Mesa bytecode (BootSwapGerm prologue runs); new blocker = the prologue's RET reads a zero return link

### Root cause of the post-MGo XFER/MTRAP loop (the session-7 blocker)

`MGo` (PrincOps, real `0o3740`) deliberately ends with `T_ sBoot, Branch[MTrap]`
= the *designed* boot entry `Xfer[dst: Fetch[@SD[sBoot]]^, src: 0]`
(`DMesaXfer.mc` MGo + MTrap + XferMD). So the XFER/MTRAP "loop" is the boot
mechanism, not inherently a fault. The live first XFER (traced, gated):

- `SD[sBoot]` (sBoot=`0o276`, SDLoc=`0o1100`, so VA MDS+`0o1376`=`0o17401376`)
  read **correctly** = `0o401` = the BootSwapGerm proc descriptor
  (tag=01, gfi=`RSH[0o401,6]`=4, ep=0). Germ data + fetch path were fine.
- Xfer tag=01 -> `XferDisp01`: `T_ gfi+GFT` (GFT=`0o1400`) -> fetch **GFT[4]**
  at VA `0o17401404`. It read `0o116011` -> global frame `0o116010`, which is
  **past the 8192-word germ** -> `LoadGC`/`XferProc` read 0 for the frame/code
  base -> **PC=0 -> ProcUnbound -> sUnbound (`0o13`)** trap. That trap XFERs
  through `SD[13]`=0 -> ControlFault (`sControlFault=7`) -> `SD[7]`=0 ->
  re-fault forever (the observed `XFER`/`XFERMD`/`SAVEPCANDTRAP`/`MTRAP` loop;
  the germ never populates the SD trap handlers -- BootSwapGerm does that only
  AFTER it runs).

The germ's GFT[gfi] should have given BootSwapGerm's frame g=`0o4634`
(`Dorado.loadmap`: ProcessorHead gfi1->`0o3400`, BootSwapGerm gfi4->`0o4634`,
...). **Byte-swapping every GFT entry yields EXACTLY those loadmap frames**
(GFT[1]`0o7`->`0o3400`, GFT[4]`0o116011`->`0o4634`, GFT[8]`0o30014`->`0o6060`,
...), and BootSwapGerm's code-base-high word byte-swaps `0o37000`->`0o76`
(= `pilotMDSHi`). So the germ FILE holds **big-endian** 16-bit words; our
`--germ` plant read them little-endian (`b0|(b1<<8)`), corrupting every
internal pointer.

### Fix (committed) -- microcode/loadmap-grounded, one line

`src/machine.c` germ load: `(b0<<8)|b1` (big-endian) instead of `b0|(b1<<8)`.
Gated entirely on `--germ` (the `germ_words[]` buffer is used only by the germ
plant), so the Alto worlds are byte-identical. Also added a gated
`DORADO_XFER_TRACE` diagnostic in `src/cpu.c` (inert without the env + cycle
gate) that prints pc/T/Q/md/last-ref-VA -- this is how the SD/GFT/frame fetches
above were read; keep it for the next pass.

### Result (verified)

- First XFER now completes: GFT[4] reads `0o4634`, `LoadGC` loads G/Code, and
  control reaches the IFU. **IFU dispatch count > 0** -- the germ runs its
  BootSwapGerm prologue: 6 Mesa bytecodes dispatch (op `324`=LFC4, `057`=LI1,
  `165`, `263`, `361`, `343`=**RET**) with br31=`3E1D0C` (germ code region),
  `flt=0`.

### HARD REGRESSION GATE -- ALL GREEN

1. `make test` = 10/10 suites.
2. AEmu NETEXEC @200M: **1476** px (band 1476-1505).
3. Galaxian @160M: **121553** px (=121552 +/-1).
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1478** px (band 1477-1497).
5. `make sdl` compiles (only pre-existing `vendor/6502/fake6502.h` warnings;
   no `test_cpu.c:7709` misleading-indentation warning reproduces).

### NEW blocker (session 9): BootSwapGerm prologue's RET reads a zero return link

After the 6-bytecode prologue, the trailing **RET** (op `343`) fetches its
local frame's return link (`L[2]`) and gets **0** -> `XferMD` DLink=0 ->
ZeroDest ControlFault (T=`0o7`) -> back into the `SD[7]`=0 trap loop (traced:
cyc ~67975302 `XferMD` md=0, lva=`0o2`; then the steady `0o1700`/`0o1041`/
`0o1026` loop resumes). So the germ now genuinely RUNS bytecode but the
frame-link chain for the boot context is wrong: either (a) an emulator XFER
frame-store mis-model (the boot `XferProc` does `Store_ T, DBuf_ SLink` =
`L[2]_ SLink`; for the boot XFER SLink=0, and the LFC4 that calls the inner
proc must store the caller frame into the inner frame's `L[2]` -- verify
`XferProc`'s store + the LFC `RTemp4`/Alloc path), or (b) germ/boot state we
still set up wrong (the local frame BootSwapGerm runs in is allocated by
`XferProc` via `AllocSub` off the AV free list at MDS `0o1000`; AV head germ
word 0 is now `0o3354` (BE) -- check AllocSub returns a valid frame). NEXT
PASS: trace the LFC4 frame allocation + the inner proc's `L[2]` store (gated
`DORADO_XFER_TRACE` around cyc 67974929..67975310) and the AV/Alloc path;
decide emulator-XFER vs germ-state. Repro:
`DORADO_IFUDISP_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 80000000` -> 6 dispatches
then the RET trap loop; the first XFER is at cyc 67974929.

## ROUTE B (2026-06-15, session 7): germ relocation aliasing FIXED at the root (`Pipe4'`/`Errors'` flag-encoding bug); GermRemap now assigns DISTINCT real pages, control reaches MGo (Mesa emulator, insset=1); the de70f5e MapBitsBR hack is REMOVED (faithful Map now keeps the `0o7030` fault gone); new blocker = post-MGo XFER/MTRAP loop, 0 IFU dispatches

### Root cause found (the real bug, not the Map population)

The session-6 diagnosis ("Initial maps the whole 64K aliased; need to leave
high VM Vacant") was a red herring. Traced the live steal: our InitMem map IS
faithful (page N -> rp N 1-to-1, exactly matching real HW + the
`TranslateMapEntry` "crock" for rp > 7777B that compensates for the boot's
"no end test" real-memory scan -- see `DMesaMiscOps.mc`). The actual bug was in
**`dorado_pipe4_at` (`src/memory.c`)**, the `Errors'`/`B<-Pipe4'` readback that
`PilotBoot.FindEndMappedVM` -> `TranslateMapEntry` uses to classify a map entry
Vacant vs resident:

- The **ref bit (b0)** was hardcoded to `kind != DM_REF_NONE` instead of the
  snapshotted map entry's actual Ref flag (`map_flags_pre & 4`). EMemDefs.mc:
  `m1pipe4.wpdref = b0,b2,b3` (ref, wProtect, dirty are the map flags).
- The **wProtect bit (b2)** had the wrong polarity: `dirty` (b3) was already
  read back complemented (`Errors'` = active-low, "Previous flags
  (complemented)" per `DMesaMiscOps.mc`) via baseline bit12=1, but wProtect was
  NOT (baseline bit13=0). Fixed by setting baseline `0o150361 -> 0o170361`
  (bit13=1) so wProtect' = NOT(wProtect) like dirty' and ref'.

Effect of the bug: `TranslateMapEntry` classified **resident page 0** (wp=0,
dirty=1, ref=1) as MapVacant and **genuinely-vacant page 0xFFFF** (wp=1,
dirty=1) as MapNotVacant -- exactly inverted. So `FindEndMappedVM` stopped at
boundary 0 and `GermRemapLp` stole pages from the TOP of VM (0xFFFF down),
which the crock returns as rp=0 -> every MDS-76 germ page collapsed onto rp 0.

(HM grounding: the `0o150361` baseline was never a literal HM quote --
`pdftotext` finds no such constant; it is derived from EMemDefs.mc's Pipe4
bit layout + the microcode's complemented-flag reading. Documented as such.)

### Result (verified)

- `FindEndMappedVM` now ascends page 0 -> ~4096 (boundary = the crock's first
  rp>7777B page), `GermRemapLp` steals distinct pages 4095..4064, and the
  MDS-76 germ-dest entries get DISTINCT real pages (idx 0x3E10 -> rp 0x0FEE).
  No rp=0 collapse.
- The single `0o7030` MapBitsBR fault is GONE **without** any intercept: with
  the faithful classification the MapBitsBR bank (VA 0xFFF000, page 0xFFF0)
  stays resident (rp 0xFFF0, not scanned/stolen by FindEndMappedVM/GermRemapLp,
  which only touch pages <=4096), so MapDirtyBit's Fetch reads real-backed
  storage and never faults. **The de70f5e `mapbits_buf` bypass hack is REMOVED**
  (`mapbits_intercept`/`mapbits_lo`/`mapbits_hi`/`mapbits_buf`/`DM_MAPBITS_WORDS`
  and `machine_enable_mapbits_intercept` all deleted). A/B test confirmed: with
  the hack gated off the germ-dest rp and the post-relocation flow are
  byte-identical, so the hack was redundant once the flags were fixed.
- After GermRemapDone the BLT runs and control reaches **MGo (real 0o3740)**
  with **insset=1** -- the Mesa emulator is now executing the germ's resident
  code.

### HARD REGRESSION GATE -- ALL GREEN

1. `make test` = 10/10 (updated `test_pipe4_error_encoding` and
   `test_cpu_pipe4_no_error_baseline` to the corrected `0o170361` baseline +
   actual-ref semantics).
2. AEmu NETEXEC @200M: 1483 px (band 1476-1505).
3. Galaxian @160M: 121553 px (=121552 +/-1).
4. AltoMesaDorado.eb!2 + NETEXEC @200M: 1485 px (band 1477-1497).
5. `make sdl` compiles.

### NEW blocker (session 8): post-MGo XFER/MTRAP loop, 0 IFU dispatches

After MGo the Mesa emulator settles into a steady loop (real PCs, traced
115M-120M): `XFER`(0o1700) / `XFERMD`(0o1026) / `SAVEPCANDTRAP`(0o2000) /
`SAVEPCINFRAME`(0o1600) / `CHECKSTKP`(0o2020) / `MTRAP`(0o1041) -- a Mesa
control-transfer that traps repeatedly, **0 IFU dispatches** (the germ never
runs a bytecode via IFUJump). So GermRemap + the microcode handoff now work,
but the germ's first XFER (likely starting the `BootSwapGerm` process, then
its own Ethernet volume fetch -- plan Step 4) traps. Next pass: decode the
XFER target / trap cause (is it `MTRAP` for an unimplemented/absent control
link, a stack fault at `CHECKSTKP`, or the germ awaiting its ether boot
channel?). Repro: `./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' --germ
'../chm/cedar/germ/Dorado.germ!4' --cycles 120000000`; the germ-dest distinct
rp is observable via `DORADO_MAP_TRACE=1 DORADO_MAP_TRACE_INDEX=0x3E10`.

## ROUTE B (2026-06-15, session 6): MapBitsBR `0o7030` fault ELIMINATED via a reserved-buffer intercept; germ now reaches GERMREMAP's relocation BLT; new blocker = the relocation aliases every MDS-76 germ page onto real page 0 (still 0 IFU dispatches)

### What was fixed (committed)

The session-5 residual `0o7030` fault was `MapDirtyBit` (`DMesaRastMiscOps.mc`):
`MemBase_ MapBitsBR; Fetch_ RTemp6` -- the Fetch of the per-real-page
extra-dirty-bit array referenced by base register **MapBitsBR (=25**,
`DMesaDefs.mc` `BR[MapBitsBR, 25]`) at VA **0xFFF000** (the top of the 16 MW
VM). Confirmed: base reg 25 is *deliberately* set to 0xFFF000 by the Cedar
microcode (BRHI=0xFF @real `0o3756`, BRLO=0xF000 @real `0o3775`); this is the
canonical MapBits location, not garbage. `MapDirtyBit` is reached during
`PilotBoot.GermRemapLp` via `WriteMapPage`/`TranslateMapEntry`.

Per `DMesaRastMiscOps.mc` the array is "real memory ... [holding] one bit for
each page of real memory"; on real Dorado it is a reserved real-backed region
whose Fetch never faults. In our model that VM page maps to an arbitrary real
page that GermRemap vacates (faulting the Fetch). **Only the PrincOps/Cedar
map subroutines use MapBitsBR** -- the Alto worlds use `DMesaMiscOps.mc`'s
`WriteMapPage`, which keeps the duplicate dirty bit in the real-page *sign
bit* and never touches MapBitsBR -- so the fix is naturally Cedar-only and the
Alto pixel gates are structurally untouched.

Fix: a **dedicated reserved-buffer intercept** in the memory model.
- `include/memory.h`: `DM_MAPBITS_WORDS` (4096 = one bit per 64K real pages),
  plus `mapbits_intercept`, `mapbits_lo/hi`, `mapbits_buf[]` in
  `dorado_memory`.
- `src/memory.c` `dorado_memory_ref_task`: at entry, references in
  `[mapbits_lo, mapbits_hi)` route to `mapbits_buf` (Fetch->Md, Store->buf,
  PreFetch no-op), bypassing Map + cache, **never faulting** and surviving any
  Map mutation -- exactly the real-hardware invariant.
- `src/machine.c` `machine_enable_mapbits_intercept`: armed at germ pass 1
  (Cedar PC `0o7012`, where base reg 25 already holds 0xFFF000), reads the
  live MapBitsBR base reg and arms the intercept. Inert without `--germ`
  (`mapbits_intercept` stays 0), so Alto behavior is byte-identical.

### Result

- The `0o7030` MapBits fault is **GONE**. GermRemap now runs its full
  `GermRemapLp` page-relocation loop and reaches `GermRemapDone`'s IOBR->LPtr
  **BLT copy** into MDS 76.
- New single fault at real **`0o2761`** (the BLT `Store_ T, DBuf_ MD`),
  VA `0x3E16FF` (MDS 76, germ dest), absorbed. Still **0 IFU dispatches**.

### Hard regression gate -- ALL GREEN

1. `make test` = **10/10**.
2. AEmu NETEXEC (`worlds/aemu.eb` + NETEXEC) @200M: **1476** px (band
   1480-1505; 1476 is the known animation/cursor frame at exactly 200M).
3. Galaxian @160M: **121553** px (= 121552 ±1).
4. AltoMesaDorado.eb!2 + NETEXEC: **1483/1480/1477** @195/200/205M (band
   1466-1497; the intercept is provably inert without `--germ`, so this is
   pure NetExec host-time render variance).
5. `make sdl` compiles.

(No `test_cpu.c:7709` misleading-indentation warning reproduces under Apple
clang 17 with the project's flags + `-Wmisleading-indentation`; the build is
warning-clean except for pre-existing `vendor/6502/fake6502.h` ones. The
code around 7709 is correctly indented.)

### NEW blocker (session 7): GERMREMAP relocation aliases all MDS-76 germ pages onto real page 0 -> germ image destroyed -> REQUEUE spin

The germ does NOT dispatch because the relocation produces a corrupt germ
image. Diagnosis (precise):

- `PilotBoot.GermRemapLp` "steals pages from the end of mapped VM" and remaps
  them into MDS 76, then BLT-copies the germ. It reads each stolen page's
  entry via `TranslateMapEntry` and writes it into the MDS-76 slot.
- In our model **every MDS-76 germ entry ends up rp=0** (traced idx
  `0x3E02..0x3E21`: all `rp=0000`, mostly `wp=0 dirty=1`, two `wp=1` ->
  the `0o2761` BLT-store write-protect/page fault). All 32 germ pages
  collapse onto real page 0, so the BLT overwrites itself and the germ image
  in MDS 76 is garbage.
- Root cause = the **inherited Map from Initial** maps the whole 64K-page VM
  resident with aliasing: high VM pages map to low/`rp=0` real pages (traced
  `0xFFF0 -> rp 0`), and some map to rp > 7777B which
  `TranslateMapEntry`'s "crock" (`real page > 7777B => vacant`) returns as
  vacant (rp 0 in Alto mode / wProtect&dirty in PrincOps). `FindEndMappedVM`
  finds its boundary at `0xFFF1` and the stolen pages all yield rp 0.
- On real Dorado real memory is far smaller than the 16 MW VM, so the boot's
  mapped region is a small 1-to-1 working set (distinct valid rp <= 7777B) and
  the stolen pages are distinct real pages. The MapBits island sits ABOVE the
  vacant gap and is never stolen.

**What was tried and did NOT work this session (reverted):**
- Capping reported real-memory modules (`config_modules_max`): no effect --
  the resident extent is set by the enumeration, not `RealPages`, and the
  Cedar world warm-starts off Initial's map.
- Forcing `RealPages=4096` via chip-size: no effect (warm start).
- Manually vacating a `[0x1000, 0xFFEF]` "real-memory gap" to move the
  `FindEndMappedVM` boundary: the boundary did not move to 0x1000 as modeled
  (the steal geometry / inherited map state is not what a naive
  first-vacant-from-0 scan predicts), so it only changed the failure (MDS-76
  vacant instead of rp 0) without dispatching. The exact steal mechanics need
  to be traced live before forcing a gap.

**NEXT PASS:** fix the relocation so the stolen pages have distinct valid
real pages. The likely correct lever is **how Initial (the bootstrap)
populates the Map** -- it should map only the actual real-memory working set
1-to-1 (distinct rp <= 7777B), leaving high VM vacant, instead of aliasing
all 64K. Pages 0..0xFFF already map 1-to-1 (`rp=vp`, confirmed). Trace
`GermRemapLp`'s live steal source (`RTemp2` boundary, the per-iteration
`make-vacant` page, and the `RTemp4` entry written to each MDS-76 slot) at
cyc > 66.7M to learn the true boundary before forcing the geometry. Repro:
`DORADO_FAULT_TRACE=all ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 90000000` -> intercept
armed, 3 germ passes, single `0o2761` BLT fault, 0 dispatches.
`DORADO_MAP_TRACE=1 DORADO_MAP_TRACE_INDEX=0x3E10` shows the MDS-76 rp=0
collapse.

---

## ROUTE B (2026-06-15, session 5): RMap<- now READ-ONLY (was conflated with Map<- write); the `0o7030` MESAFAULT loop is GONE; GERMREMAP completes and control runs on into the Mesa-emulator microcode; new blocker = germ doesn't dispatch (0 IFU dispatches)

### Root cause of the session-4 blocker (the `0o7030` page fault -> infinite MESAFAULT)

The faulting op at real `0o7030` is **not** FindEndMappedVM's RMap; it is
`MapDirtyBit` (`DMesaRastMiscOps.mc`): `MemBase_ MapBitsBR; Fetch_ RTemp6`,
the Fetch of the per-real-page extra-dirty-bit array that lives in the
**highest VM bank** (`MapBitsBR` = base register **025**, VA `0xFFF000`).
`MapDirtyBit` is called by `WriteMapPage`, which `PilotBoot.GermRemapLp`
calls for every page it steals from the end of mapped VM. The Fetch hit a
**vacant** page `0xFFF0` and trapped.

But the *reason* page `0xFFF0` was vacant was an upstream emulator bug:
**we conflated `RMap<-` (read map) with `Map<-` (write map).** HM page 46-47
+ Table 8a: both are the ASEL=0 / FF[0:1]=1 map reference, but
- `Map<- R, MapBuf<- T` (write): FF=`0o100` (FF[2:7]=0). Writes B/TIOA into
  the entry; returns previous contents in the pipe.
- `RMap<- R` (read): FF=`0o131` — carries the **ReadMap function** (FA
  forced 0, FB=3, FC=1 = FF[2:7]=`0o31`). Returns previous contents in the
  pipe and **must not modify the entry, never faults** (verified against
  Cedar.mb: `NewReadMapPage` real `0o4161` and the FindEndMappedVM scan
  real `0o6506` both FF=`0o131`; InitMem map write `IWRITEMAP` real `0o7013`
  FF=`0o100`).

Our `DM_REF_MAP` always *wrote* the entry. So `FindEndMappedVM`'s
`RMap_ RTemp0` (RTemp0=0) scan overwrote every page it touched with
rp=0/wp=0/dirty=0 (and the `MapBitsBR` bank too), misplacing the "end of
mapped VM" boundary; `GermRemapLp` then stole the wrong pages — including
the `MapBitsBR` page `0xFFF0` — making it vacant, which broke
`MapDirtyBit`'s Fetch. (`DORADO_MAP_TRACE_INDEX=0xFFF0` showed idx FFF0
toggling mapped<->vacant, including the tell-tale rp=0/wp=0/dirty=0 write
from the RMap-with-RTemp0=0.)

### Fix (committed)

Added a distinct `DM_REF_RMAP` read-only map reference:
- `include/memory.h`: new `DM_REF_RMAP` enum (appended, so existing kind
  numbers — e.g. FETCH=10 in traces — are unchanged).
- `src/cpu.c` `decode_ref_kind`: a non-io ASEL=0/FF[0:1]=1 map reference is
  `DM_REF_RMAP` when it carries the ReadMap function (`((ff>>3)&7)==3 &&
  (ff&7)==1`), else `DM_REF_MAP` (write). Map<- writes stay writes.
- `src/memory.c`: `DM_REF_RMAP` case snapshots previous contents to the pipe
  (already done by `pipe_push`) and does **nothing else** — no entry write,
  no MapBufBusy, no fault. Also excluded from the DVAVIC path and named in
  the trace tables.
- `tests/test_cpu.c`: ref-kind name switch updated (kept -Wswitch clean).

The Alto/InitMem write path is unchanged (those are `Map<-`, FF=`0o100`),
so the working worlds still set up their maps exactly as before.

### Result

- `DORADO_FAULT_TRACE=all` over the whole CedarDorado germ run: only the
  pre-existing two faults at `0o6023/0o6024` (~11M, present in every world)
  plus a **single** `0o7030` fault (~66.68M). The session-4 ~60K-iteration
  `MESAFAULT`/`REQUEUE` loop at `0o3306` is **gone**.
- That single `0o7030` fault is now absorbed by the memory fault task
  (task `0o17`, short handler real `0o1622..0o1613`) which **returns to
  task 0**; `GermRemapLp`/`GermRemapDone` then run their IOBR->LPtr BLT
  copy loop (real `0o1264`) to completion and control continues.

### Hard regression gate — ALL GREEN

1. `make test` = **10/10**.
2. AEmu NETEXEC (`worlds/aemu.eb` + NETEXEC.BOOT): 1487/1493/1495 px at
   190M/195M/205M (1476 at exactly 200M — animation/cursor variance; the
   1480-1505 band absorbs it).
3. Galaxian (`worlds/aemu.eb` + Galaxian.boot) @160M: **121553** px (=121552 ±1).
4. AltoMesaDorado.eb!2 + NETEXEC @200M: **1481** px (band 1466-1497).
5. `make sdl` compiles.

### NEW blocker (session 6)

After the germ remap, control settles into a tight Mesa-emulator microcode
loop — real `0o3301 -> 0o3302 -> 0o1071 -> 0o1072` (mb=`03` = MDS,
insset=1) with occasional excursions to `0o6606..0o6635` — and stays there
for >13M cyc with **0 IFU dispatches** through 80M. So the germ does NOT
yet execute Mesa opcodes; it is NOT (yet) the expected "germ does its own
ether fetch" (plan Step 4). Likely candidates to chase next:
- Whether the single `0o7030` fault leaves `Md` garbage so `MapDirtyBit`'s
  dirty-bit word is wrong and `WriteMapPage` mis-relocates a germ page (on
  real Dorado the `MapBitsBR` bank is real-backed and this Fetch never
  faults — consider mapping/zero-backing the `MapBitsBR` highest bank so
  the first-touch Fetch returns 0 instead of faulting).
- Whether `BootSwapGerm` (g=`004634`) actually XFERs / installs a runnable
  process; the `0o3301/3302/1071/1072` loop looks like the Mesa emulator
  idling with nothing schedulable (it Fetches from MDS and spins).
Repro: `./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' --germ
'../chm/cedar/germ/Dorado.germ!4' --cycles 80000000`; gate the PC/IFU
traces to cyc>66.68M.

---

## ROUTE B (2026-06-15, session 4): Map widened 16K -> 64K (MapIs64K / VirtualBanks=400C); GERMREMAP's relocation loop now RUNS; new blocker is FindEndMappedVM's high-VA Fetch fault (16K aliasing was load-bearing for the IOBR map scan)

This session **widened the modeled Map from 16K to 64K entries** so the germ
relocation no longer collapses MDS 76 (germ VA `0o17401000`, page `0o174010`
= 64008) onto a low aliased index. Cited to `InitMem.mc` `GetMemConfig` and
the DMux/muffler (`Various.mc` `SetDMuxAddress`). Hard regression gate GREEN.

### What changed (committed)

- `include/memory.h`: `DM_MAP_ENTRIES` 16384 -> **65536** (64K map, 256-word
  pages, VM = 2^24 = 16 MW). Comments updated. `dorado_memory.map[]` grows
  from 128 KB to 512 KB inside the calloc'd machine struct (heap, safe).
- `src/memory.c` `dorado_memory_dmux_read`: DMux **`0o1511` (MapIs64K) now
  reports sign-SET (0x8000)**; `0o1512` (MapIs256K) stays sign-CLEAR. Per
  `InitMem.mc` `GetMemConfig`: `0o1512` sign-set => 256K (`VirtualBanks=2000C`);
  `0o1511` sign-set => **64K (`VirtualBanks=400C` = 256 banks)**; neither =>
  16K. So `GetMemConfig` now selects `VirtualBanks=400C`, and `400C*256 =
  65536 == DM_MAP_ENTRIES` (the loop-bound == map-size invariant that keeps
  the cold `Map1to1Loop`/`MapInitLoop` from running off the end -- the same
  invariant the 16K config preserved; a mismatch is what spun Mesa forever).

### Hard regression gate (all GREEN)

1. `make test` = **10/10** suites.
2. AEmu NETEXEC (`worlds/aemu.eb`) -> **1482 px** (band 1480-1505), world
   loads at cyc 32M (unchanged), budget **200M cyc** to reach paint.
3. Galaxian -> **121552 px** (exact) at budget **150M-180M cyc** (the stable
   attract screen; 120M=121641 / 200M=121554 are animation-frame variance).
4. AltoMesaDorado.eb!2 + NETEXEC -> **1489 px** (band 1466-1497), budget 200M.
5. `make sdl` compiles.

The 4x-longer cold InitMem loop did NOT push world-load later (still 32M for
the ether-loaded worlds; the InitMem 4x is inside the *loaded* world and
still terminates and paints within 200M). 200M is a safe budget for all.

### CedarDorado germ: how far it gets now

`./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' --germ
'../chm/cedar/germ/Dorado.germ!4'`. The cold InitMem 4x pushes the 3 germ
disk-read passes from ~53M to **~66.7M cyc** (descriptor/label/data all
still land: seal `0o121212` v6, 8192/8192 germ words at VM `0o1000+`).

**Progress vs the session-3 16K state:** the old blocker is GONE -- the
`GermRemapDone` BLT store (`0o2761`) no longer faults and the fault task
`0o17` no longer takes over. **GERMREMAP's relocation machinery now actually
RUNS**: control reaches the DMesa map subroutines `SetBRForPage`/
`TranslateMapEntry`/`MapDirtyBit` (real `0o7000`/`0o7020`/`0o7030`) called
from `PilotBoot.FindEndMappedVM`(`0o6724`) / `GermRemapLp`(`0o2763`).

**NEW blocker (the next pass):** task-0 takes a **page fault at real
`0o7030`** -- a Fetch (`ASEL=0`, kind=10) via **MemBase=IOBR** to VA
**`0x0FFF000`** (page `0xFFF0` = 65520) -- then diverts into the Mesa
`MESAFAULT`->`REQUEUE` loop (real `0o3306`, mb=03, fetching VA 0; ~60K
hits to end-of-run, **0 IFU dispatches**, no XFER to `BootSwapGerm`). Root
cause: `FindEndMappedVM` scans UP through VAs (via IOBR) looking for the
first VACANT page = the end of mapped real VM (`PilotBoot.mc`:
`FindEndMappedVM: T_ RTemp2, Call[SetBRForPage]; RMap_ RTemp0,
SCall[TranslateMapEntry]; Branch[GermRemapLpE]; ... Branch[FindEndMappedVM]`).
With the **16K** map this scan ALIASED onto resident low pages and
terminated early (illusory -- the per-page `WriteMapPage`s and the BLT then
operated on aliased entries, which is why session-3's BLT faulted). With the
**64K** map the scan walks the *real* VA space and reaches a genuinely
vacant high page (`0xFFF0`, near the top of the 16 MW map / the IO region);
our model **page-faults the Fetch** instead of letting `TranslateMapEntry`
read the vacant map flags cleanly. On real Dorado `TranslateMapEntry` reads
the entry via `RMap` (ReadMap, Mar-addressed) and the microcode TESTS the
vacant flag -- it does not fault. So the next blocker is in the
map-entry-read path, not the map size.

### NEXT PASS

Make `FindEndMappedVM`/`TranslateMapEntry`'s map-scan reference (real
`0o7030`, `SetBRForPage`+`TranslateMapEntry` in `DMesaRastMiscOps.mc` /
`PilotBoot.mc`) read the vacant high-VA map entry WITHOUT a CPU page fault,
so the scan returns "found first vacant page" (the `+1` exit to
`GermRemapLpE`) instead of trapping into `MESAFAULT`/`REQUEUE`. Then
`GermRemapLp` relocates pages `RTemp3` down to the germ extent, `GermRemapDone`
runs the IOBR->LPtr BLT into VM `0o17401000+`, and control XFERs into
`BootSwapGerm` (g=`004634`) with `insset=1` (plan Step 4 = the running germ
doing its OWN ether fetch of the volume). Repro:
`DORADO_FAULT_TRACE=all ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ/Dorado.germ!4' --cycles 90000000 2>&1 |
awk '/germ pass3/{p=1} p&&/FAULT_CPU/{print $4,$3,$6}' | sort | uniq -c`
-> 1x `pc=0o7030 task=0 mb=25`, then ~60K x `pc=0o3306 task=0 mb=03`.

## ROUTE B (2026-06-15, session 3): PV descriptor fabricated + all 3 disk-read passes fed; the REAL GERMREMAP now runs; blocked on the 16K-map aliasing of MDS 76

This session **fabricated a faithful PV root-page Descriptor and fed all
three of DiskBootSoft's disk-read passes**, so the real microcode validated
the descriptor (no MESAFAULT), read the germ, and ran the real
`PilotBoot.GERMREMAP` relocation loop to completion. The germ does **not run
yet**: GERMREMAP's relocation of the germ into **MDS 76** can't land because
our **16K Map (MapIs16K)** aliases MDS 76 with low VAs. Regression gate green:
`make test` 10/10; AEmu NETEXEC **1495**; Galaxian **121552** (exact);
AltoMesaDorado NETEXEC **1490**; `make sdl` compiles.

### What landed (committed)

The disk-read interception lives in `src/machine.c` (gated on `--germ` +
Cedar-only PC `0o7012`, so the Alto pixel gates are structurally
untouched). At each `BootTransferLp` seal-fetch (real PC `0o7012`) with
`IOCB.seal == IOCBSealValue`, read `IOCB.command` (VM `0o435`) and dispatch
by pass, then KCmmdDone-complete (seal=0, pageCount=0, labelStatus=0 ->
BootTransfer +2):
1. **Descriptor (cmd `0o274`):** deposit `page0[0]=0o121212` (Desc.seal),
   `page0[1]=6` (Desc.currentVersion); zero the germ DFID region
   `0o32..0o43` (Desc.bi.germ; fID/firstPage/da all 0 -- the microcode
   copies them into the IOCB but our fake completion bypasses the real
   read). Source: `PilotBootDefs.mc`, `PhysicalVolumeFormat.mesa`.
2. **Label (cmd `0o260`):** just complete (DiskBootSoft never inspects the
   label contents).
3. **Germ data (cmd `0o100254`):** deposit germ file word W -> VM
   `IOCB.dataPtr+W` (dataPtr = BootDataPtr = baseGerm = `0o1000`, read
   live), then advance `IOCB.dataPtr` by `0o400`/page (8192 words = `0o40`
   pages -> dataPtr `0o1000`->`0o21000`) as the disk microcode would, so
   GermBoot's `BootDataPtr_ MD` and GERMREMAP relocate exactly the loaded
   extent.

**Two real emulator fixes** were required and are the load-bearing part:
- **`src/memory.c` `dorado_storage_store_at_va`** now clears stale *valid+
  vacant* cache lines for the target VA (the old code used
  `dorado_cache_lookup`, which *skips* vacant lines). The Mesa world leaves
  VM page 0 vacant (null-trap); a stale valid+vacant line there satisfied
  the seal-check fetch with 0 instead of our deposited `0o121212`. General
  correctness fix; gate stayed green.
- **`src/machine.c`** makes VM page 0 resident before depositing the
  descriptor (the real descriptor DMA write would map-fault page 0 and the
  fault handler makes it resident). It allocates a **genuinely free real
  page** via the new `machine_find_free_rp()` -- page 0's existing `rp` is
  aliased to a live high-VA page, and reusing it let that page's dirty
  writeback clobber the descriptor (`storage[0]` went 0o121212 -> 0).

### The precise NEW blocker (where it stops now)

Flow after pass 3 (verified, task-0 real PCs): GermBoot ->
`FINDENDMAPPEDVM`(`0o6724`) -> `GERMREMAPLP`(`0o2763`) loop (calls
`SetBRForPage`/`TranslateMapEntry`/`FlushPage`/`WriteMapPage` from
`DMesaMiscOps.mc`) -> `GERMREMAPDONE`(`0o2725`) -> the BLT copy loop
(`Fetch_ RTemp5`@`0o2762` from MDS0, `Store_ T,DBuf_ MD`@`0o2761` to MDS76).
**Task-0 stalls at the BLT Store `0o2761`** and the fault task (`0o17`)
takes over (TPC `0o1606`) and never resolves. insset=1 (Cedar world is
running), **no IFU dispatch / no XFER yet**.

Root cause: the germ's resident pages (VA `0o17401000+`, MDS 76) are still
**VACANT** at the stall (`rp=0 wp=1 dirty=1`). GERMREMAP's `WriteMapPage`
*did* run for every page, but the Map index it writes, `(0o17401000>>8) &
(DM_MAP_ENTRIES-1)` = `0o37002`, **ALIASES** low VAs (e.g. `0x3C0200`,
`0x7C0200`, ...) because the Map is only **16384 entries** (`MapIs16K`,
`DM_MAP_ENTRIES`) while the Mesa world's VA space (MDS 76 ~ VA 4M) far
exceeds 16K pages. So the MDS-76 map writes collide with / are overwritten
by other-VA entries, the germ destination stays vacant, and the BLT store
faults.

### NEXT PASS

The descriptor + disk-read feed is DONE and faithful; the remaining work is
the **Map size**. Two routes:
(a) **Widen the Map** so MDS 76 doesn't alias: make `GetMemConfig`/the DMux
    muffler report `MapIs64K` (or `256K`) and grow `DM_MAP_ENTRIES` to
    `0o200000`+ so `0o17401000>>8 = 0o174010` is a *distinct* index. This is
    the muffler/`SetDMuxAddress` path fixed in the 2026-06-14 DMux session
    (currently pinned to 16K); raising it touches the whole memory model +
    the cold InitMem loop bound, so revalidate the Alto gates carefully.
(b) Audit `WriteMapPage`'s Map-write Hold handshake (gap B1/C1) -- but the
    aliasing above is the dominant cause; (a) is the real fix.
Repro: `./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' --germ
'../chm/cedar/germ/Dorado.germ!4' --cycles 90000000` -> the three
`[machine] germ passN` lines print, then it relocates and stalls at the BLT.

## ROUTE B (2026-06-15, session 2): disk-completion mechanism PROVEN; germ-run blocked on the (remote) PV-root descriptor format

This session **proved the IOCB completion mechanism empirically** and
**reverse-engineered the descriptor seal/version constants from the
Cedar.mb disasm**, then pinned the precise remaining blocker: fabricating
a *minimal* descriptor (seal+version only) is not enough — the real
PilotBoot microcode then dereferences the PV-root descriptor's
`bootingInfo`/`DFID` structure, which we cannot fabricate because that
Pilot layout is not in any local source. No code committed this session
beyond this doc (the experiment was kept under `DORADO_GERM_EXP` and
reverted); regression gate verified: `make test` 10/10, `make sdl`
compiles, tree byte-identical to green HEAD (`9d8ab2f`) so the
AEmu/Galaxian/AltoMesaDorado pixel checks are structurally unaffected.

### What is now KNOWN (new this session)

1. **Completion mechanism PROVEN (the linchpin).** At the `BOOTTRANSFERLP`
   spin, writing **IOCB.seal=0 (VM 0o432), IOCB.pageCount=0 (0o434),
   IOCB.labelStatus=0 (0o453)** makes `BootTransfer` return **+2 success**:
   the hot PCs `0o7012`/`0o7003` are LEFT (observed flow
   `7012→7003→7013→7051→7052→7053→7033→7017`(`BootTransferDone`)`→6713`
   back in `DiskBootSoft`). This matches `PilotDisk.mc` `KCmmdDone`
   (line 336-338: "Zero IOCB.seal"). NOTE: my static JCN branch-polarity
   read was inverted; trust this empirical result. (Do NOT confuse with
   the `0o7012` branch to `BootTransferTimeout` real `0o7002` — the
   conditional R-bit polarity makes seal-cleared advance to the
   pageCount/labelStatus success check, not to the timeout.)
2. **MemBase=25 (IOBR), base 0** during the whole disk boot, so absolute
   VA == the microcode's offsets (the committed plant's absolute stores
   are correct). IOCB base **VM 0o431**; fields (PilotDiskDefs offsets):
   seal `0o432`, drive `0o433`, pageCount `0o434`, command `0o435`,
   diskAddress `0o436-437`, headerPtr `0o442`(=`@0o440`),
   labelPtr `0o447`(=`@0o461`), labelStatus `0o453`,
   dataPtr `0o454-455`, diskLabel `0o461+`. `IOCBSealValue=0o125377`.
3. **Pass-1 (descriptor) IOCB confirmed live:** command `0o274`
   (`[check,read,read]`), pageCount `1`, dataPtr `0` → the PV root page is
   read into **page 0 (VA 0)**; `Desc.seal` is at **VA 0**.
4. **Descriptor constants reverse-engineered** (Cedar.mb `DiskBootSoft`
   images 6547-6553, the seal/version check `BTemp0_ HighByte[DescSeal]`,
   `OR LowByte`, `XOR seal`, `XOR DescCurrentVersionValue`):
   **`DescSealValue = 0o121212`** (HighByte = byte `0o242` → `0o121000`,
   LowByte = `0o212`), **`DescCurrentVersionValue = 0o6`**. A descriptor
   that passes the check needs `page0[0]=0o121212`, `page0[1]=0o6`.
5. **GERMREMAP located in the disasm** (Cedar.mb, module PilotBoot):
   `FINDENDMAPPEDVM` img 6505 / real `0o6724`, `GERMREMAPLP` img 6511 /
   real `0o2763`, `GERMREMAPLPE` img 6523 / `0o2723`, `GERMREMAPDONE`
   img 6524 / `0o2725`. It uses global calls (`JCN=370`/`311` = map RAM
   subroutines) to relocate pages and set up the Mesa map/MDS, then the
   tail XFERs into `BootSwapGerm`. It only runs AFTER `DiskBootSoft`
   returns +2, i.e. it is gated behind the descriptor blocker below.

### The PRECISE remaining blocker

Depositing seal+version only and KCmmdDone-completing each pass advances
the real microcode past `BOOTTRANSFERLP`, but then it **XFERs/faults into
the Mesa fault path and loops forever** in `MESAFAULT`(real `0o3210`) /
`REQUEUE`(`0o1300`) / `CHECKSTKP`(`0o2020`) / `RESTOREALUFM`(`0o6740`),
with **insset=0 (NO IFU dispatch — the germ never runs)**. Root cause:
after the seal/version check, GERMBOOT/`DiskBootSoft` dereference the
descriptor's **`bootingInfo` array of `DFID`s** (BTemp2-relative; the
booted file's `DFID.da` (2-word DiskAddress), `DFID.fID` (File.ID),
`DFID.firstPage`) which we left zero. The Pilot **PhysicalVolume root-page
descriptor format** (the `bootingInfo`/`DFID`/`BootFileType` layout +
the `BTemp2` offset GERMBOOT computes) is **not in any local source** —
only `DiskBootSoft.mc` *references* those symbols; the defs live in the
unavailable `PilotBoot.mc` / Pilot `PhysicalVolume.mesa`, and inside
`GermDorado.bcd` (remote binary, per `chm/cross-reference.html`). So a
faithful descriptor cannot be fabricated yet, and a blind steer past the
descriptor leaves the Mesa map/MDS unmounted → the same fault loop.

**WARNING for the next pass:** do NOT make the committed plant
auto-complete the IOCB without a real descriptor — it derails into the
`MESAFAULT` loop, which is *worse* than the current benign spin.

### NEXT PASS — two concrete routes to finish

(a) **Recover the descriptor layout.** Get the Pilot PhysicalVolume
   root-page format: `bootingInfo: ARRAY BootFileType OF DFID`,
   `DFID = [da: DiskAddress(2w), fID: File.ID, firstPage]`, and the
   `Desc.seal`/version offsets — from `GermDorado.bcd`, a Pilot
   `PhysicalVolume.mesa`, OR by reverse-engineering **GERMBOOT** (real
   `0o6737+`, PilotBoot, in the Cedar.mb disasm) to learn the `BTemp2`
   (bootingInfo offset) it computes and which descriptor cells it reads
   before/after `DiskBootSoft`. Then fabricate a COMPLETE descriptor so
   the 3 passes run and `GERMREMAP`+XFER fire. The faithful IOCB-level
   disk hook (deposit per-pass data at the `0o7012` spin: descriptor for
   cmd `0o274`, label for cmd `0o260`, the germ at `IOCB.dataPtr` for the
   `0o1xx254` incrementDataPtr pass, then KCmmdDone-complete) is the right
   vehicle — it is PROVEN to advance the real microcode past the spin.
(b) **Decode GERMREMAP fully** (Cedar.mb images 6505-6531) + the
   post-`DiskBootSoft` GERMBOOT tail to learn the exact map/MDS state it
   needs, plant the germ at `BootDataPtr` (read live from `IOCB.dataPtr`
   VM `0o454`, not the final `0o17401000`), and steer into the XFER.

Repro of the proven completion (re-add the gated experiment from git
history of this session, or):
`DORADO_GERM_EXP=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
 --germ '../chm/cedar/germ/Dorado.germ!4' --cycles 120000000`.

## ROUTE B (2026-06-15): CedarDorado reaches the disk GermBoot; germ-plant scaffolding landed; germ-run blocked on PilotBoot source

Route B = Cedar/Mesa germ net-boot. This session **characterized the disk
germ-read fully** and **landed the regression-safe germ-plant scaffolding**
(`--germ`, the VM deposit, the gated plant). The germ does **not run yet**:
that needs the `PilotBoot` GERMREMAP / Mesa-XFER handoff microcode, whose
source is not in our tree. Details below; regression gate stayed green.

### STEP 1 — the disk germ-read, characterized (CedarDorado.eb!6)

Flow (real PCs via `build/mbdis --disasm ../chm/dorado/Cedar.mb!6`, 4th col):
`GERMBOOT`(0o6737, image 6463, module **PilotBoot**) → `DISKBOOTSOFT`(0o6700)
→ `BOOTTRANSFER`(0o6160) → spins in `BootTransferLp`. Microcode SOURCE for
the transfer is local: `chm/doradomicrocode/doradomicrocodesources/`
`DiskBootSoft.mc!1` + `DiskBootTransfer.mc!1` (GERMBOOT/GERMREMAP itself is
in `PilotBoot`, NOT local — see CedarMesa10MB.cm module list).

- **(a) Disk command issued.** `DiskBootSoft` builds an IOCB at VM 0o431
  (drive 0, pageCount, command, diskAddress, dataPtr, diskLabel) and posts
  it to `CSB.next`; the PilotDisk microcode then drives the real slow-IO
  sequence (confirmed empirically via the new `DORADO_DISK_TRACE`):
  TIOA 0o14 DiskTag = DriveSelect drive 0 + cyl/head seek tags
  (e.g. 0o104360); TIOA 0o13 DiskRam = load the read-sequencer Format RAM
  (words 0,1,2,4,6,11,...,377; the last word sets EnableRun); TIOA 0o10
  DiskControl = op-load (command `[check,read,read]`=0o274 etc.); TIOA 0o11
  DiskMuff = TW-clears + EnableRun. Three IOCB passes: read the PV root page
  (descriptor, seal/version checked) into page 0, then the first page's
  label, then the whole boot file into memory at `BootDataPtr` page-by-page
  following `bootChainLink`s (a `[-1,-1]` link = EOF).
- **(b) Load target.** The germ’s resident VM is **0o17401000+** (loadmap,
  see STEP 2). `DiskBootSoft` first stages it into a low-64K buffer at
  `BootDataPtr` (IOCB.dataPtr); `PilotBoot.GERMREMAP` then relocates pages
  to the resident addresses / sets up the Mesa map+MDS. In our run it never
  gets past the FIRST pass (descriptor) because no pack responds.
- **(c) The polled status / spin.** `BootTransfer` posts the IOCB and waits
  for `iocb.seal` to be cleared by the disk microcode, with a ~2 s RTC430
  timeout. Hot PCs: **0o7012** = the `Fetch_ EventTemp1, PD_ MD,
  Branch[BootTransferTimeout, ALU>=0]` seal-fetch; **0o7003** = the
  `PD_ (RTC430)-T-1, Branch[.-1, ALU#0]` timeout compare. It spins forever
  because the junk timer / RTC430 is quiesced at the LoadRam handoff
  (`machine.c`, `cpu->junk_tw_enabled = 0`), so the timeout never fires AND
  no disk ever clears the seal. Confirmed: at 150 M cyc task-0 hot PCs are
  `0o7012`/`0o7003` (~13 M hits each), `booted=1`, 0 dispatches, insset=0.
- **(d) Post-read entry.** On success `BootTransfer` returns +2 →
  `DiskBootSoft` returns +2 → `PilotBoot` runs `GERMREMAP`
  (`GERMREMAPLP`/`GERMREMAPDONE`, images ~6511-6524) then XFERs into the
  germ wart **`BootSwapGerm`** (loadmap global frame g=004634) as Mesa code.

### STEP 2 — germ format + placement (Dorado.germ!4 + Dorado.loadmap!1.txt)

- **Loadmap page numbers are OCTAL.** "Germ file pages: 40" = 32 decimal =
  the 16384-byte / 8192-word file. The GERM FILE MAP maps **contiguously**:
  **file word W → VM word 0o17401000 + W**, W in 0..8191. (Check: file page
  13₈=11₁₀ → 0o17401000+11·0o400 = 0o17406400 ✓; page 33₈=27₁₀ →
  0o17416400 ✓.) **No leader page** — file page 0 is real germ data.
  word0 = 0o166006 (little-endian, matching disk.c’s pack word order).
- **BootFile.Location** is a germ-INTERNAL Mesa global read by the germ’s
  OWN `BootChannelEther.Create` (`chm/cedar/germ/BootChannelEther.mesa!3`):
  it keys on `pLocation.deviceType = ethernet` + `pLocation.bootFileNumber`,
  then runs exactly our Mayday/EFTP protocol (`bootFileSend`=244B + eData/
  eAck/eEnd) to fetch the next stage. So it is consumed AFTER the germ is
  running, when the germ fetches the volume over Ethernet — it is NOT part
  of the microcode handoff. Its exact word offset needs the GermDorado.bcd
  source (not local); it lives in the germ’s global-frame area (writable
  once the germ is planted). Germ frames: BootChannelEther g=006150,
  BootChannelDisk g=006100, MiniEthernetDriver g=006060, BootSwapGerm
  g=004634.

### STEP 3 — germ-plant: SCAFFOLDING landed, germ-run BLOCKED

Implemented (gated, regression-safe):
- `src/memory.c` `dorado_storage_store_at_va()` — VM deposit primitive
  (translate + write storage + invalidate the stale cache line).
- `dorado_machine_config.germ_path` + `--germ PATH` (`src/dorado.c`); the
  machine loads the germ image at create (little-endian words).
- `src/machine.c` gated plant: when task-0 first reaches the Cedar
  germ-boot spin (real PC **0o7012**, which exists ONLY in the Cedar world
  — the Alto worlds never reach it) and a germ is loaded, deposit
  file word W → VM 0o17401000+W. Verified: `8192/8192 words at VM
  0o17401000+ (readback word0=0o166006)` at cyc 53.2 M.
- `src/disk.c` `DORADO_DISK_TRACE` diagnostic (the STEP-1 evidence above).

**BLOCKER (why the germ doesn’t run yet).** Depositing the resident image
is necessary but not sufficient. The germ runs as Mesa code (insset≠0) and
needs the Mesa VM map+MDS set up so its logical addresses (MDS base
0o3400000, code base 0o3401000) resolve to the loaded pages — that is what
`PilotBoot.GERMREMAP` does, followed by the Mesa XFER into `BootSwapGerm`.
**`PilotBoot.mc` is not in our local sources** (only DiskBootSoft/
DiskBootTransfer are), so the remap+handoff cannot be reconstructed
faithfully yet, and a blind PC-steer past the spin would derail (the map
isn’t mounted for 0o17401000 in the Mesa sense). The alternative —
satisfying the disk read so the REAL GERMREMAP runs — needs a synthesized
PV root-page descriptor (seal/version + bootingInfo, layout in unavailable
PilotDisk defs) plus the multi-run bootChainLink transfer = the incomplete
disk data path the project deliberately avoided.

**NEXT PASS:** either (1) obtain/reverse `PilotBoot` GERMREMAP from the
Cedar.mb disasm (images ~6463-6532) to learn the map/MDS setup + the XFER
that gives `BootSwapGerm` control with insset≠0, then steer there after the
plant; or (2) reverse the PV root-page descriptor + DFID layout and emulate
the BootTransfer disk read so the real microcode does the handoff. Once the
germ runs, set BootFile.Location = {ethernet, bfn} so it fetches the volume
over our EFTP/Mayday server (the step after this milestone).
Repro: `DORADO_MACHINE_TRACE=1 ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' --germ '../chm/cedar/germ/Dorado.germ!4' --cycles 120000000`.

Regression gate this session: `make test` 10/10; Galaxian 121552 (exact);
NETEXEC/AltoMesaDorado within their wall-clock variance band (NetExec
renders host time, so its px count varies run-to-run ~1466-1505 — verified
inherent: the same binary gives 1488/1492/1497).

## STATUS (2026-06-14): canonical Mesa world boots NETEXEC; Mesa VM reached, stops at first XFER

End-of-session state, on top of the muffler/DMux fix below.

**IFUM half-word order fix (`a03c4c5`) -- the keystone.** `src/cpu.c`'s `IFUMLH<-B`/`IFUMRH<-B`
FF handlers were inverted (wrote the wrong IFUM half) and `src/mb2eb.c` pre-swapped IFUM
word0/word1 to compensate -- two bugs that cancelled for our self-built `worlds/aemu.eb` but
loaded a *canonical* archive world (`AltoMesaDorado.eb`) IFUM-transposed. Opcode `045`
(`LDAipc`) then dispatched to garbage vec `0o4470` (vs the real `0o1370`); the bogus fields
word set `TPause'` (halting the IFU) and `RBase=0`, and it spun forever. Fixed atomically:
swapped the four IFUMLH/IFUMRH read+write handlers to match `LoadRam.mc` (IFUMLH=address=
`ifum_lo`, IFUMRH=fields=`ifum_hi`), emit canonical item order in `mb2eb.c`, corrected
`test_cpu.c`'s `test_ifum_load_read` (it had encoded the bug), and REGENERATED `worlds/aemu.eb`.
Now `AltoMesaDorado.eb!2` + NETEXEC paints (~1466 px); AEmu still green (NETEXEC 1482, Galaxian
121552, suite 10/10). Foundational: every canonical Xerox world had been loading
IFUM-transposed, masked only by the cancelling `mb2eb` pre-swap.

**Mesa VM reached and works; op 362 is `@STOP` (designed), not an XFER trap [CORRECTED].**
Serving a Mesa-format (`0o345`) program (MazeWar) to `AltoMesaDorado.eb!2`: downloads fully,
runs the Nova relocation bootstrap, switches to `insset=2` (the Mesa VM), and runs a 6-bytecode
boot prologue at VM PC `0x5108` (`IWDC,RIL0,LL2,R0,WB,STOP`). The 6th, op `362`, is **`@STOP`** --
the *designed* Mesa->Nova handoff, executed faithfully (`DMesaXfer.mc`: "save state and exit to
Nova world", loads `T=MesaStopLoc=0o26`; no fault; the `insset>=2` byte order at `cpu.c:2159` is
confirmed correct). So the Mesa VM genuinely works -- "stops at first XFER" was the wrong framing.
The 13 `DMesa*` Mesa-VM modules (named `AltoMesa*` in `AltoMesa.cm`, `DMesa*` on disk) are local in
`chm/doradomicrocode/doradomicrocodesources/`.

**Real Mesa blocker: two separate, larger items (neither a gated one-liner).** After `@STOP` the
Nova world parks polling an I/O-completion control block; device confirmed = Ethernet (not
RTC/keyboard). It splits:
- **MazeWar / `0o345` worlds -- the Alto interrupt fabric is COMPLETE; the WORLD never arms it
  [PINNED].** The interrupt machinery is already correct in our microengine: NWW (an RM register,
  not memory `0o452`), the `AEmuReschedule` trap (`AEmuTrapBase+14`), the `Reschedule` FF, and the
  `PCLOC`/`INTVEC` (`0o500`/`0o501`) vector path. The receive-completion microcode (`AltoEther.mc`
  `EPost`) runs faithfully on the EIT (posts EPLOC `0o377`). But the world never set up the
  interrupt CHANNEL: traced over a full MazeWar run, `EBLOC` (`0o601`), `ACTIVE` (`0o453`) and
  `INTVEC` (`0o501`, the `EtherInterruptEntry` handler addr) are PERMANENTLY 0 and Nova `EIR` is
  never reached for the ether channel. So `EPost`'s `NWW <- NWW OR EBLOC` ORs 0 -> no interrupt
  ever fires. Forcing NWW from C would CRASH: `AEmuReschedule` vectors through `INTVEC(0o501)=0`
  -> jump to page 0 (the documented runaway). So there is NO faithful C fix. The bare `0o345`
  Pilot outload reaches its poll loop at VM `0x0FD2F` WITHOUT running the Pup interrupt-subsystem
  init (channel-allocate + `EIR` + install the handler at `0o501`) -- work the **Pilot germ/nucleus**
  is supposed to do. So this confirms the architectural conclusion: the `0o345` family needs the
  germ/Pilot boot (Route B below), not any Alto-side interrupt change.
- **CedarNetExec:** a different loop polling EPLOC directly (no interrupt); Rx works but it awaits
  Cedar-germ *server content*, so its path is the **Cedar germ/Pilot net-boot**
  (`InitialEtherCedarDorado.eb!3` + `CedarDorado.eb!6` + germ), per the FINDING below -- not the
  Alto receive interrupt.

(The `2026-06-13f` interrupt-cell framing was retracted by `13g`; don't rely on it.)

**Also landed.** NetExec game menu (`87834cd`): `--boot-dir-all` (default) advertises all 29
Alto B-format games via the `257B`/`260B` boot directory; boot NETEXEC and type a game name at
the `>` prompt to boot it. 26 CHM-Murray Alto programs + games committed (`7f07cce`), each with
a `make run-*` target. The Alto path stayed green at every step.

## FIX (2026-06-14): muffler/DMux model unblocks AltoMesaDorado.eb past InitMem -> STARTEMULATOR

**Root cause.** `AltoMesaDorado.eb` (= placed `chm/dorado/Mesa.mb!3`) entered `InitMap`
(real PC `0o1076`) and hung forever in the cold `InitMem` map/storage-init loop
(`MAP1TO1LOOP`/`IWRITEMAP`/`WAITFORMAPBUF`/`NEXTMAPENTRY`), never reaching `INITMEMDONE`
`0o5706` or `STARTEMULATOR` `0o5021`; 0 opcode dispatches, 0 pixels. `worlds/aemu.eb` ran the
byte-identical loop fine. The loop bound is `VirtualBanks` (entries enumerated =
`VirtualBanks * 256`), set by `GetMemConfig` (`InitMem.mc`) from two diagnostic-multiplexer
("muffler") reads issued by `SetDMuxAddress` (`Various.mc`): DMux `0o1512` sign-set ->
`MapIs256K` (`VirtualBanks=2000C`), DMux `0o1511` sign-set -> `MapIs64K` (`400C`), neither ->
`MapIs16K` (`100C` = 64 banks * 256 = 16384). The muffler bit arrives as the SIGN of
`SetDMuxAddress`'s trailing `T_ XTemp17_ ALUFMem, Return`. We modeled `ALUFMem` as the raw
ALUFM register, so the sign was world-dependent garbage: AEmu happened to land sign-CLEAR
(16K, matches our `DM_MAP_ENTRIES=16384` map, loop terminates) while Mesa landed sign-SET
(64K/256K, enumerates a space far larger than our map, never finishes). Both worlds were
faithful; AEmu booted by luck of its ALUFM contents.

**Fix.** Added a DMux/muffler model (`dmux_addr`/`dmux_pending` in `dorado_memory`,
`dorado_memory_dmux_strobe/_use_dmd/_pending/_read` in `src/memory.c`). `MidasStrobe<-B`
captures the DMux address (T[4:15]) on the leading strobe and arms a pending read/write;
`UseDMD` consumes it as a manifold WRITE (Initial `WriteManifold`); a `Pd<-ALUFMem`/`ALUFMRW`
read while pending returns the muffler word with the selected bit in the SIGN position. For
our 16K-map config both `0o1512` and `0o1511` read sign-CLEAR, so `GetMemConfig` falls through
to `MapIs16K` and the cold loop terminates. The `dmux_pending` gate (set only by a preceding
`SetDMuxAddress` shift, cleared by the muffler read or by `UseDMD`) keeps normal
`Pd<-ALUFMRW` ALUFM access (Bootstrap/Initial/emulator startup) untouched. `src/cpu.c` wires
the `MidasStrobe`/`UseDMD`/`ALUFMem` handlers.

**Result.** `AltoMesaDorado.eb!2` + NETEXEC now runs `InitMap` -> `TasksOff` ->
`GetMemConfig`/`SetDMuxAddress` -> past `InitMemDone`/`StartEmulator`; task-0 hot PCs are now
`GETPC`/`ABOOT`/`AMIDSTBOOT`/`READTERMINAL` (Alto-emulator routines only reachable via
`STARTEMULATOR`), the InitMem map-loop PCs are gone from the hot list. Next blocker: the
post-StartEmulator boot-selection wait (`ABOOT`/`READTERMINAL` reads the keyboard for the
boot choice; the DDC keyboard back-channel / NETEXEC pull-through, gap E2, is not modeled), so
still 1 IFU dispatch (`insset=0`) and 0 pixels. Regression gate green: `make test` 10/10;
AEmu NETEXEC 1501 display-list pixels; Galaxian 121552 pixels.

## FINDING (2026-06-13): Cedar boots from disk; the Mesa VM is in AltoMesaDorado.eb, not worlds/aemu.eb

Established this session from the Dorado Booting memo
(`chm/doradosource/DoradoBooting.tioga!2.txt`, sections 1.1–1.4). This overturns the prior
assumption that serving `CedarNetExec.boot` directly to `CedarDorado.eb` over EFTP is the
right Cedar bring-up route.

**Cedar microcode (`CedarDorado.eb`) boots software from disk, not Ethernet.** Memo §1.3:
"Ethernet booting of software is possible only with Alto emulator-based microprograms, not
with Cedar. To boot Cedar software from the Ethernet, you must first boot the NetExec using
Alto/Mesa, then use the NetExec to call the CedarNetExec, then use the CedarNetExec to call
the desired program." So serving `CedarNetExec.boot` over our EFTP/Mayday server directly
to `CedarDorado.eb` was a category error: Cedar never listens for an Ethernet software boot.
Observed: the Cedar microcode loads and enters at `pc=0o1070` (BOOTORSTART), then hangs in
`InitMem`'s cold map-write loop (`IWRITEMAP`/`WAITFORMAPBUF`, hot PC `0o7116`), expecting a
disk germ, zero opcode dispatches, never reaches GERMBOOT.

**The Mesa VM lives in `AltoMesaDorado.eb`, not `worlds/aemu.eb`.** `worlds/aemu.eb` is
Alto/Nova ONLY (no Mesa VM). That is why Mesa-format (`0o345`) boot files mis-run on it:
the Alto interpreter decodes Mesa bytecodes as Nova opcodes (`insset=0`), nothing paints,
nothing executes. The supported Ethernet route to Mesa or Cedar software is via
`AltoMesaDorado.eb` (which carries the full Mesa VM) chained through NetExec.

**Two routes going forward, both work-in-progress:**

- **Route A (recommended):** `AltoMesaDorado.eb` -> NetExec -> CedarNetExec -> target.
  Reuses the existing Ethernet transport. `AltoMesaDorado.eb!2` enters correctly at
  `pc=0o1076` cold-bootstrap (unlike `CedarDorado.eb`'s stuck `0o1070`). Blocked on boot
  orchestration in `src/machine.c` and `src/ethernet.c` being tuned for `worlds/aemu.eb`;
  served a Mesa-format file it completes only 1 dispatch. Route A needs that orchestration
  adapted — first validate with `AltoMesaDorado.eb + NETEXEC` (an Alto B-format file), then
  fix the leader-page prepend and boot-select for a Mesa-format second stage. Not a zero-code
  swap.

- **Route B:** fix the Cedar microcode path directly. Requires (a) fixing the `InitMem`
  map-write Hold handshake in `src/memory.c` (`IWRITEMAP`/`WAITFORMAPBUF` blocker; see
  `dorado/CLAUDE.md` gaps B1/C1), and (b) a germ-Ethernet or disk boot channel. The Pilot
  germ is present locally: `chm/cedar/germ/Dorado.germ!4`; the physical-volume boot file is
  `chm/cedar/OthelloDorado.boot!8`. The ether-germ route avoids the incomplete disk write
  path (`BootChannelEther.mesa` / `BootChannelDisk.mesa` sources are in `chm/cedar/`).

See the finding immediately below for the two-format boot-file split that accompanied this
session.

## FINDING (2026-06-13): Boot files come in TWO formats needing TWO different emulator microcodes; we had been using the wrong one for Mesa/Pilot files

Established this session, with evidence. Dorado boot files split into two
incompatible classes, distinguished by the first big-endian word of the
file (`od -An -to2 -N2 FILE`, which prints the two bytes swapped):

- **Alto B-format** — big-endian word0 = `0o405`; `od` shows `0o002401`.
  These run on **AEmu**, the Alto emulator microcode at
  `dorado/worlds/aemu.eb`. They WORK today. Files in `chm/bootfiles/`:
  `NETEXEC.BOOT!8`, `CRTTEST.BOOT!1`, `DMT.BOOT!22`.
- **Mesa/Pilot outload** — big-endian word0 = `0o345`; `od` shows
  `0o162400`. These CANNOT run on AEmu — they need the **Cedar/Mesa
  emulator microcode** at `../chm/dorado/CedarDorado.eb!6`. Files:
  `CedarNetExec.boot!4`, `MesaNetExec.boot!1`, `AlphaMesaMesaNetExec.boot!1`,
  `MazeWar.boot!1`, `NEWOS.BOOT!21`.

Why this matters: we had been trying to boot `CedarNetExec` on AEmu, which
is architecturally impossible. CedarNetExec is a Mesa/Pilot program — its
own source `NXControl.mesa` hard-codes
`microcodeFiles[Dorado] = "CedarDorado.eb"`. On AEmu the Ethernet EFTP
download completes fine, but the Alto interpreter just mis-runs the Mesa
bytecodes (the IFU instruction set stays `insset=0`), so nothing paints and
nothing executes. (This corrects the earlier 2026-06-13g..e sessions below,
which were chasing an EPLOC race / interrupt-cell hang while booting a
Mesa-format file on the wrong microcode.)

Switching `--eb` to `../chm/dorado/CedarDorado.eb!6` makes the Cedar
microcode LOAD, but it does not yet run: it enters at `pc=0o1070`
(RestartEmulator) with zero opcode dispatches. A real Cedar bring-up
additionally needs:

- the Cedar Initial image `../chm/dorado/InitialEtherCedarDorado.eb!3`,
- the Pilot germ, and
- a Stage-2 rework from the Alto BootViaNet / leader-page path to the
  Pilot/germ net-boot.

So **all Mesa-format boots (including MazeWar) are EXPERIMENTAL /
not-yet-running**, pending that Cedar bring-up.

Also fixed this session (both EFTP-transfer gating): commit `62a4fb9` added
an EICLOC rx-delivery gate, and `858857b` corrected it to
`rx_count > EICLOC + 2`. The off-by-two had been silently blocking the
entire EFTP boot transfer; with it fixed the transfer now completes
(1 boot Mayday + 254 Acks).

## CORRECTED (2026-06-13g): CedarNetExec derails on an EPLOC race; the broadcast injector floods ~250x too fast

Supersedes 2026-06-13f below (the "interrupt-driven cell" framing was WRONG
-- those page-0 cells 0o103/0o112/0o130/0o164 are constant JSR-coroutine
vectors, reached AFTER divergence, not a data-dependent wait). Real root
cause: CedarNetExec's FIRST Ethernet init after the EFTP transfer (~141M,
VM 0o634-0o777) does SIO TurnOnRx (waits for an input completion at
EPLOC=VM 0o600) then SIO TurnOnTx (waits for OutDone=0o777 at the SAME
EPLOC). Our periodic broadcast injector (dorado_ethernet_time_broadcast,
machine.c ~670-677) feeds NETEXEC-tuned input packets into that same EPLOC;
an input completion (or InBufOverflow 0o1377 from an over-large packet)
landing during the Tx poll clobbers OutDone -> CedarNetExec takes a reset
path (SIO=CmdAbort) -> JMP @0 into a page-0 coroutine dead loop. With
broadcast OFF it hangs EARLIER on the Rx poll, so it needs an early packet
but NOT during the Tx poll. NETEXEC's init tolerates the flood; CedarNetExec
does not (it never reaches interrupt/display config: M[0o453]=0 vs NETEXEC's
0o010403).

THE CORRECT PROTOCOL (from IFS, quoting the Pup spec): gateway-info should
broadcast ~ONE EVERY 30 SECONDS (IFS GatewayInformationProtocol.cs:135-149,
Thread.Sleep(30000); the comment notes MazeWar "expects periodic updates");
breath-of-life every few seconds. Our injector fires every 2M cycles
(~120ms) then 50M (~3s) -- ~250x too fast. FIX DIRECTION: deliver the early
packet CedarNetExec's Rx init needs, then back off to the real ~30s cadence
so nothing lands during its Tx poll; also size injected packets to its
input buffer (EICLOC=VM 0o604) so they post InDone 0o377 not InBufOverflow.
A naive slow-down starves the Rx init; a naive tx_on gate is insufficient
(the packet is already queued and read during Tx -- tried, did not help).
Repro: `DORADO_STORE_TRACE_VA="0600,0600"` (the EPLOC race);
`DORADO_VMDUMP="0450,0460,200000000"` (interrupt cells stay 0). Baseline is
clean: 10/10, NETEXEC unaffected.

## FRONTIER (2026-06-13f): CedarNetExec runs but busy-waits on an interrupt-driven page-0 cell that never updates

After the pipe-fault fix (below) CedarNetExec RUNS: it dispatches Alto
opcodes (IFUDISP insset=0 -- it is an Alto program, NOT a Mesa image),
relocates, advances, then parks in a tight ~6-PC poll loop. The mouse
cursor works and tracks (M[0o426/0o427] cursor cells set; M[0o420] DASTART
=0o31426), but it never paints its menu -- the display bitmap stays
uninitialized (page-0 garbage = the "noise"). User confirmed: typing,
clicking, mouse-move all leave the cursor fine but draw nothing else.

KEY FINDING (this session): the page-0 OS cells the loop reads
(0o103/0o112/0o130/0o164, in the 0o100..0o170 block) are written EXACTLY
ONCE -- at cyc ~63.79M by the loader (pc=0o1622) populating the boot
image -- and NEVER again during the poll. So CedarNetExec is busy-waiting
for an INTERRUPT HANDLER to update one of those cells, and that event never
fires in our model. (Store trace confirms; note DORADO_STORE_TRACE_VA is
NOT gated by DORADO_TRACE_GATE -- it shows all writes in the VA range.)
Since the cursor tracks the mouse, mouse handling works; the missing event
is most likely the RTC/interval-timer, keyboard, or display-vertical
interrupt. NETEXEC (same Alto OS) gets past this and paints, so the lever
is the delta: which interrupt/cell NETEXEC has that CedarNetExec waits on.

NEXT (where the prior agent stalled -- it needs a Nova disassembler, not a
microcode trace): decode the ~6-PC loop. VM PC = br31(hex) + pcf/2 from
IFUDISP. Sampled loop br31 values: 05088, 05801, 00001, 00B49, 0717.
CedarNetExec.boot loads 1:1 (file word N = VM word N), so disassemble it
(Alto/Nova: LDA/STA/JMP/JSR/ISZ/DSZ/SIO+ALC) at those VM addrs to find the
exact cell tested and branch taken; then identify which interrupt should
write it (RTC at the page-1 timer cells? keyboard? DVI?) and whether our
engine posts that interrupt to the running Alto world. Repro:
  DORADO_IFUDISP_TRACE=1 DORADO_TRACE_GATE="146000000,146010000" \
  ./build/dorado --eb worlds/aemu.eb \
    --eftp '../chm/bootfiles/CedarNetExec.boot!4' --cycles 146050000
  DORADO_STORE_TRACE_VA="0100,0170" ... (writes to the polled cells)

## FIXED (2026-06-13e): CedarNetExec's post-load hang was a Pipe-VA clobber on a page-377 map fault

Forensic trace of the CedarNetExec hang (after the leader-page fix it loaded
but its display was noise and it did NO further work). Root cause found and
FIXED (pipe_push in memory.c):

- After load, CedarNetExec's Nova relocation BLT stores into MDS bank-0
  page 377 (e.g. VA 0o177653 / 0xFFAB). AEmu intentionally write-protects
  page 377 (XMFaultTask.mc), so the store map-faults. The handler's design
  (Fault0) is: bank-register store (0o177740..) -> handle; any OTHER page-377
  store -> IgnoreStore (drop it, restart). CedarNetExec's BLT hits "other".
- Our engine instead ran Fault0 to `MapFault: Branch[.]` -- a self-loop --
  and task 0 spun there forever (hot uPC real 0o4046), dispatching ZERO Alto
  opcodes. The display "pixels" were stale residue; the VM was frozen.
- Why: Fault0 reads the faulting VA from pipe[ProcSRN] (VAHi/VALo). The
  faulting store pushed VA=0xFFAB into pipe[ProcSRN], but the very next IFU
  prefetch (also task 0, same fixed ProcSRN slot) overwrote it with the next
  opcode's VA (0x5034) BEFORE the fault task read it. So Fault0's page-377
  test failed -> MapFault. On real Dorado consecutive refs get distinct SRNs
  (16-entry ring), so the faulting entry survives at SRNFirstFault.
- Fix: pipe_push now refuses to overwrite the first-faulting ProcSRN slot
  while a fault is pending (released on dorado_fault_clear). ASRN ring
  unaffected. `make test` 10/10; NETEXEC still boots (1490 px, no regression).

RESULT: CedarNetExec now RUNS -- it dispatches Alto opcodes continuously,
the Alto VM PC advances through real code, and it settles into an input/poll
loop (br31 in {05033,05088,05801} + page-0 cells; it animates a counter at
display word 0x2D61 and re-stores 0o177653 each pass, all IgnoreStore'd).

NEW FRONTIER: it does not paint a clean menu. The display is still dense
noise and rx=0/tx=0 (no network). DASTART(M[0o420])=0o31426 but the chain
there does not parse as a clean Alto DCB (w2/SA=0o22 is page 0), so either
CedarNetExec lays out its display differently than NETEXEC's BCPL DCBs, or it
is parked waiting (keyboard/gap E2, or a clock/network event) before drawing.
Next: identify the poll's exit condition (what page-0 cell it waits on and
who should update it -- keyboard interrupt? RTC?), and whether the rasterizer
must follow a different display structure. Trace hooks added: DORADO_PIPEVA_TRACE
(prints pc/psrn/va at each B<-Pipe0/Pipe1 read) and a cycle stamp on FAULT_CPU.

## DIAGNOSED (2026-06-13d): CedarNetExec stall is a boot-loader handoff CRASH (fix in progress)

Forensic trace nailed the CedarNetExec stall (and all four other 000345
Mesa-format boot files -- MesaNetExec, AlphaMesaMesaNetExec, MazeWar,
NEWOS -- which all stall identically). It is NOT a device poll-wait: it is
a CRASH. After the verbatim EtherBoot loader (`eth_bol_loader[254]`,
ethernet.c:535; final `JMP@0` = the `02000` at ethernet.c:567) restores the
boot file's saved page-0 (file words 0o400..0o777 -> M[0..0o377]) and does
`JMP@0`, M[0] = file[0o400]:
- NETEXEC.BOOT (works): file[0o400]=0o001311, a real low-memory code entry.
- CedarNetExec.boot (crashes): file[0o400]=0o165054, a DATA pointer into
  its boot-menu/file-descriptor table (records + strings at 0o165000+).
  M[0o165054]=0, so JMP@0 runs a zero word -> trap -> the Alto PC runs away
  through the page-0 pointer table forever (the "page-0 spin").
CedarNetExec's header is 0o345 0o354 0o403 (NETEXEC: 0o405 0 0); file[0]=
0o345 points at a relocation bootstrap at file[0o345] (`LDA1 0o110; COM 1,3;
BLT; STA3 @0o111; JMP 3`) that is NEVER reached. Load is faithful (live VM
== file; full transfer lands). So: not corruption, not truncation, not a
device poll.

FIXED (commit a125bd4): it was neither (a) nor (b) -- it was a SERVER-side
boot-file format issue. The standard Alto Ethernet loader loads only
B-format files; a real boot server prepends a 256-word leader page to the
non-B-format (000345 Mesa/Pilot) files before serving (ETHERBOOT.BRAVO).
Our server (and the reference IFS server) sent them verbatim, so the image
loaded shifted one page and M[0] got file page 1 (garbage 0o165054).
`eth_queue_eftp_boot` now prepends a synthesized leader page (word0=0o405,
word1=0) for any file whose word0==0o000345; B-format files are untouched
(no regression). All five 000345 worlds now LOAD AND RUN into real code:
CedarNetExec builds a ~50K-pixel display (was 0); MesaNetExec/MazeWar/NEWOS
advance into real code with no page-0 runaway.

NEW FRONTIER: the Mesa-format UI. They run now but do not reach a clean
screen. CedarNetExec: DASTART != 0, ~50K px but the display is garbage/
noise (display buffer not yet filled / blocked on display-list install +
the keyboard back-channel, gap E2). MazeWar: DASTART != 0 but 0 px and PC
wandering page 0 -- LESS far than CedarNetExec. So CedarNetExec is the best
vehicle (furthest along, highest value). Trace hooks in machine.c:
DORADO_TRACE_GATE, DORADO_VMDUMP, DORADO_IFUDISP_TRACE, DORADO_STORE_TRACE_VA.

## RESOLVED + NEW FRONTIER (2026-06-13c): the whole NetExec chain works; CedarNetExec stalls on startup

The interrupt-driven Pup-receive blocker below (2026-06-13b) is FIXED. Root
cause: our socket-reply builders omitted the trailing hardware-CRC word
that the AEmu receive microcode (AltoEther.mc EIEnd) unconditionally
subtracts, so packetLength came out one short and EtherPupFilter rejected
every reply. Adding the dummy CRC word (matching append_reply /
append_eftp_packet) made it land. CONFIRMED end to end:
- NetExec learns its net (banner [0#42#] -> [1#42#]) and sets its clock
  (now the real host wall-clock time, derived from the host TZ/DST).
- `?` lists CedarNetExec; typing it Mayday-requests bfn 111B; the server
  serves CedarNetExec.boot; the EtherBoot loader takes over (screen clears).
So the documented Cedar-over-Ethernet path (DoradoBooting 1.3: NetExec ->
CedarNetExec) is functional through the load.

NEW FRONTIER: CedarNetExec.boot ("XEROX Cedar Net Executive 6.0c", an
Alto-format program -- readable Alto strings, references CedarDorado.eb /
.eg germ / Othello / PhysicalVolumeBoot) transfers in full (65280 words,
255 EFTP packets, eftp_q=257 seq=256) but does NOT come up: DASTART=0 (no
display list), rx=0/tx=0 (no network I/O), PC wandering in page 0/1
(0o101..0o665). It stalls BEFORE any network activity, so the receive fix
does not affect it -- this is a fresh early-startup bring-up problem, like
NetExec's page-zero bug was. Repro WITHOUT needing to type (serves it via
the breath-of-life path, same loader + image):
  cd dorado && ./build/dorado --eb worlds/aemu.eb \
    --eftp '../chm/bootfiles/CedarNetExec.boot!4' --cycles 350000000 \
    --progress 2>&1 | grep -iE 'pixels|DASTART|booted'
Next: trace what CedarNetExec is doing in its page-0/1 spin loop (is it
waiting on a device/memory cell that never changes, or did it crash into a
loop?). Same forensic approach that cracked the receive bug. NETEXEC.BOOT
disassembles 1:1 (file word = VM word); CedarNetExec.boot likewise.

## NEXT FRONTIER (2026-06-13b): NetExec's interrupt-driven Pup receive does not deliver socket replies (FIXED -- see above)

Goal: boot Cedar the documented way (DoradoBooting.tioga 1.3) -- get into
the Alto NetExec, type `CedarNetExec<CR>`, which net-boots CedarNetExec
(then Othello / the desired Cedar program). This needs NetExec's network
stack to actually RECEIVE Pup socket replies. It currently does not.

WHAT IS DONE AND COMMITTED (all spec-grounded, suites green, NETEXEC still
boots to its prompt and typing works):
- **Boot-directory protocol.** `--boot-dir NAME=BFN=PATH` (repeatable) ->
  the fake server answers NetExec's BootDirReq (257B) with a BootDirReply
  (260B) listing {bfn, date, BCPL name}, and serves the matching file by
  boot file number on the follow-up Mayday (244B). The 260B packet is
  byte-perfect vs the IFS boot server (PUP/MiscServicesProtocol.cs
  SendBootDirectory + BCPLString.ToArray): BFD blocks at the first Pup
  content word, name `[len][chars]` padded to even.
- **Real Pup checksum.** `pup_checksum()` in ethernet.c (ones-complement
  sum, end-around carry, left rotate, neg-zero normalize) ported from IFS
  PUP.CalculateChecksum, stamped on the bootdir/gateway/time replies. The
  old 0177777 "skip" sentinel is gone.
- **Documented routing/time replies** replacing the dead 0204 handler:
  GatewayInformationRequest 200B -> GatewayInformationResponse 201B whose
  destination net teaches NetExec its net (PupRoute.bcpl
  ProcessRouteInfoReply); AltoTimeRequest 206B -> AltoTimeResponse 207B
  (was wrongly 205B, which InstallTime rejects).
- `--boot-file-number` CLI flag; the STK boot-parameter block is now
  configurable (machine.c) instead of hardcoded.

THE DIAGNOSTIC THAT ISOLATES THE BLOCKER: with EVERY reply now byte- and
type-correct (routing, time, bootdir), NONE land -- the banner still shows
`[0#42#]` (net 0, UNLEARNED), "Date and time unknown", and `?` does not
list CedarNetExec. So it is NOT a packet-format problem. The replies are
READ (the DORADO_BOOTDIR_DEBUG "260b reply CONSUMED" line fires, which
requires the receiver armed) but never delivered to NetExec's contexts
(GatewayListener / GetTime / GetDir).

WHY EFTP WORKS BUT THIS DOES NOT: the EFTP boot loader (Taft EtherBoot)
POLLS ePLoc directly. NetExec's full driver is INTERRUPT-DRIVEN:
`EtherInterrupt` in chm/altosource/pupsources.dm!4_/PupAlEthb.bcpl. So the
EFTP transfer succeeding never exercised the interrupt-driven receive path
NetExec uses.

THE TWO CONCRETE SUSPECTS (both checkable by tracing the running NetExec):
1. `EtherInterrupt` never fires -- AEmu posts the received-packet status
   (the loader polls and sees it) but does not raise the Alto Ethernet
   interrupt, so NetExec's handler never runs. The received packet sits in
   the buffer unprocessed.
2. `EtherInterrupt` fires but rejects -- PupAlEthb.bcpl accepts a packet
   only when status `lastEPLoc eq 377b`; if AEmu posts any other value it
   is "bad packet, throw it away." Then it runs the predicate filters
   (EtherPupFilter: `(pup.length+5) rshift 1 eq packetLength &
   EtherPBI.type eq typePup`) and enqueues to pbiIQ; the PupLevel1 context
   (Pup1Init.bcpl) demuxes pbiIQ to the dest socket.

KEY SOURCE (now in the tree): chm/altosource/pupsources.dm!4_/ --
  PupAlEthb.bcpl (EtherInterrupt, FeedEther, EtherPupFilter),
  PupRoute.bcpl (GatewayListener, ProcessRouteInfoReply, the routing
  table), Pup1Init.bcpl (InitPupLevel1, the PupLevel1 demux context,
  pbiIQ/socketQ). IFS reference clone (gitignored) at repo-root /IFS:
  PUP/PUPProtocolDispatcher.cs (demux), PUP/Transport/Ethernet.cs,
  PUP/Gateway/GatewayInformationProtocol.cs, PUP/MiscServicesProtocol.cs,
  PUP/PUP.cs (PupType enum + CalculateChecksum).

NEXT STEP: trace whether NetExec's EtherInterrupt runs when our reply
arrives and what ePLoc status it sees. Find EtherInterrupt's VM address
(disassemble NETEXEC.BOOT 1:1, or trace from the InitAltoEther interrupt
setup), gate a STORE/IFUDISP trace around a 200B/206B/257B reply delivery,
and confirm (a) the interrupt is taken, (b) the status word presented is
0o377, (c) the packet reaches pbiIQ, (d) PupLevel1 demuxes it to the
socket iQ. The first link that fails is the bug. Reproduce with:
  cd dorado && DORADO_BOOTDIR_DEBUG=1 ./build/dorado --eb worlds/aemu.eb \
    --eftp '../chm/bootfiles/NETEXEC.BOOT!8' \
    --boot-dir 'CedarNetExec.boot=111=../chm/bootfiles/CedarNetExec.boot!4' \
    --cycles 150000000 2>/tmp/bootdir.log
(SDL: build/dorado-sdl, same flags; type `?` to list boot files, expect
CedarNetExec once the receive path delivers.)

## ROOT CAUSE FOUND (2026-06-13): the page-zero / divide-vector corruption is FIXED

The long-standing M[0o344] divide-vector clobber / page-zero BitBlt spray
(chased across sessions 8-10b, papered over by the divide-vector guard,
and the cause of the user's "types a little bit then stops responding")
was a single microarchitecture bug, now fixed (commit cf129c1):

  The RM/STK write address is latched at instruction issue, but the
  emulator applied a same-instruction `RBase<-FF` change BEFORE the write.
  AEmu BitBlt `BBNormal: BBTemp_ (BBTemp)+MD, RBase_ RBase[AEmRegs]`
  computes the destination base (= displacement + DBCA) and stores it to
  BBTemp in the BBRegs region; with the FF-changed RBase the result landed
  in AEmRegs instead, so BBTemp kept displacement-only and DBCA was
  DROPPED. The BitBlt destination then missed the bitmap and the
  bottom-to-top gray fill sprayed page zero. Fix: the LC write uses the
  issue-time RBase (cpu.c execute_uinstr).

  Found via a forensic BBT dump (DORADO_BBT_TRACE at real 0o3124): the
  BitBlt table was SANE (DBCA=0o122330, DBMR=46, DTY=0, DH=14) yet the
  destination dropped DBCA -- isolating the RBase-in-same-instruction
  write rather than garbage inputs. The single-opcode differential harness
  (altodiff-dorado sweep, 20768 vectors) had first proven the steady-state
  opcode emulation correct, narrowing the search to exactly this kind of
  intra-instruction timing.

VERIFIED: all 10 suites pass; with NO divide-vector guard
(DORADO_NO_DIVPROT=1) the world is stable to 200M -- M[0o344] stays
0o4155 (never 0o4154), Swat OutLdRet stays 0, typing "Probe" survives,
the banner renders. The old ~190M FillWithDash Swat is also gone (same
root). The divide-vector guard is now UNNECESSARY (being retired).

# (original handoff below) Alto-on-Dorado boot bring-up (2026-06-11, session 2)

Read this first if you are a fresh Claude session resuming the Dorado
Alto-emulator boot work. Project-wide context is in `/CLAUDE.md` and
`dorado/CLAUDE.md`. This file supersedes the earlier 2026-06-11 version:
every issue in it is FIXED and committed. The Stage-2 Alto Ethernet
software boot now completes end to end and NetExec runs; the open
frontier is NetExec's runtime behavior (display list, heavy fault-task
activity).

## One-line status

The full chain works: BaseBoard -> Initial -> Ethernet microcode boot ->
AEmu world -> InitMap -> ABoot -> EBoot -> breath-of-life -> Taft
EtherBoot loader -> Mayday -> **the complete NETEXEC.BOOT transfers over
EFTP in a single attempt** (one Mayday, 87 data packets + End + dally,
`pos=21820/21820`) -> the loader jumps into NetExec -> **NetExec runs**
(installs a DASTART display list, takes vertical field interrupts, does
EWait ether activity, manipulates its DCB over time). The framebuffer is
still blank: NetExec never builds a non-zero-width text DCB, and in the
latest run the fault task (17) consumes 25 M cycles and NetExec clears
DASTART — that is the open question.

## One-command repro

```
cd dorado && make build/test_cpu build/mb2eb && \
./build/mb2eb '../chm/dorado/AEmu.mb!2' /tmp/aemu_only.eb 01076 && \
DORADO_ONLY_FULLBOOT=1 DORADO_NO_DISK=1 \
DORADO_STORAGE_MODULES=1 DORADO_ETH_BOOT_110=/tmp/aemu_only.eb \
DORADO_ALTO_BOOT_ETHERNET=1 \
DORADO_ETH_EFTP_BOOT='../chm/bootfiles/NETEXEC.BOOT!8' \
DORADO_BOOT_BUDGET=400000000 ./build/test_cpu 2>&1 | \
grep -E 'Stage-2|task cycles|Alto display|Alto DCB'
```

Expected: `eftp_requests=1 ... state=0 seq=87 max_seq=87 pos=21820/21820`
(the whole boot file transferred), `bol=1`, an `Alto display:` line
showing DASTART/WW/ACTIVE and a DCB chain walk. Note
`DORADO_NO_DISPLAY_WAKE` is NO LONGER set — the display tasks run and
the boot still completes.

## What was fixed this session (all committed + pushed)

1. **Per-task BDispatch (THE big one, commit 10d6d0a).** The B-dispatch
   latch was a single global; HM §4.4 p32: the B dispatches load Link
   from B and OR Link bits into TNIA "during the next instruction for
   the task. Since Link is task-specific, this works correctly across
   task switching." With the global latch, the interleaved EIT consumed
   the emulator's dispatch, so AEmu's Nova skip tables (BigBDispatch ->
   LSKP/LNSK exits) broke whenever the EIT was busy: the EtherBoot
   loader's `sz 0 0` / `se 3 0` never skipped, looping
   boot3->boot4->boot6->boot3 forever, wiping every InDone post via
   boot3's re-arm. Fix: `task_dispatch[16]` in dorado_cpu, the dispatch
   functions also load Link per the manual, and LdTPC<- clears the
   target task's pending dispatch (HM p32 note).
2. **EFTP sender retransmission (same commit).** The lock-step fake
   server resends the unacked packet after 3 M wakeup ticks (EFTPSPEC
   ~1 s). The loader's protocol depends on sender resend to recover
   lost posts. 1 M ticks is TOO SHORT — it lands mid-Ack and breaks
   the exchange; keep >= 2x the loader's ~650 K-cycle process+Ack
   window.
3. **Boot shims released after use (a1a369a).** The forced BS(+Quote)
   keyboard chord is released (all-up) once EBoot (AEmu real 0o2006) is
   entered; breath-of-life rebroadcast stops once `eftp_max_seq > 0`.
   NetExec now sees a clean keyboard and a quiet wire.
4. **DispM false-positive (31fa85f).** The boot-button serial-bit jam
   applied to ANY TIOA TStatus read, so DisplayAux.mc's
   DisplayInitConfig ("Nonzero => DispM board installed") saw a phantom
   DispM and assigned the terminal task to AHT (task 4), which never
   gets scanline wakeups. The jam now applies only to DHT/AHT
   terminal-task reads. The THT now runs on task 3: per-field
   DASTART/DCB fetches, cursor bitmap reads, vertical field interrupts
   (NWW OR mask + Reschedule), ~940 K NLCB writes per boot.
5. **IFUReset enables the junk timer (9af147d).** HM p67: IFUReset ==
   IFUTest<-0, and junk wakeups occur when IFUTest.15 = 0. We had it
   disabling the timer, so ABoot's IFUReset silenced the RTClock
   forever (VM 430 ticked 4 times in 385 M cycles; NetExec sat in
   timed startup waits). Also: tick period corrected 1000 -> 533
   cycles (32 us at the 60 ns microcycle; Junk.mc's RTCDelta math
   assumes exactly 32 us per wakeup).

## FIRST PIXELS (2026-06-11, session 3)

The virtual screen renders: the DispY NLCB cursor path is implemented
in src/display.c (register select in the top 4 bits of each NLCB
output; CursorLo = the per-scanline marker, CursorHi closes a cursor
line and draws the 16-bit row; the per-field CursorX rides an
IM-derived constant whose tag decodes as register 13B, screen x =
2063 - value, calibrated against the EtherBoot loader's cursorX of
531B). /tmp/dorado_boot_display.pgm now shows the boot loader's
cursor sprite (the Ack-packet-as-bitmap PARC trick) at x=345 plus
NetExec-era cursor rows. Vertical placement is approximate (the
line counter tracks CursorLo writes since VSync; TopBorder biasing
not yet calibrated). The probe prints "Display cursor:
rows_drawn/last_x_raw/line/field"; DORADO_NLCB_TRACE=1 dumps decoded
NLCB writes.

## SESSION 4: THE PUP STACK IS ALIVE (2026-06-11 evening)

The fake server now speaks enough Pup to wake NetExec's whole network
stack. Sequence of unblocks, each verified by the TX trace:

1. **The raw routing probe accepts a directed packet.** NetExec's
   pre-context park (the VM 0o3205 raw listen) wants a DIRECTED Pup
   (nonzero dest host), dest socket 60B, type in 200B..203B (the
   GatewayInfo family - NOT time), and a length-consistent packet.
   dorado_ethernet_time_broadcast() (misnamed now) sends a
   GatewayInfoReply (201B) shaped for it, alternating dest socket
   60B / psRouteInfo(2). Once accepted: THE CONTEXTS START, the
   cursor changes from the boot-loader sprite to a solid 16x16 block
   (fb_nonzero 53 -> 74), and NetExec begins SOCKET-LEVEL
   TRANSMISSION - type 204B routing-info requests to net1/host1
   psRouteInfo, from its socket 60B, retried continuously.
2. **eth_tx_packet_done answers type-204B requests** by dest socket:
   socket 2 -> GatewayInfoReply 201B with one routing tuple
   <net 1, gw net 1, gw host 1 (server), hops 0>; socket 4 -> Alto
   time reply 205B with an NTime body. Replies are directed to the
   requestor's source port with dnet=0 ("this net") to dodge the
   localNet bootstrap deadlock (a net-1-addressed reply gets
   FORWARDED, not delivered, until localNet is learned - which the
   reply itself teaches).
3. NetExec LEARNED ITS NET: its requests changed from dnet=0/dhost=1
   to net1/host1 after the socket-2 routing broadcasts.

SESSION-5 STRUCTURAL DISCOVERY: the module occupying VM ~0o1664-
0o3667 with statics at M[0o1010..0o1113] is the FullBootBase
RESIDENT (BuildNetExec.cm links "FullBootBase/J ... DiskBoot.Run/B
300/N FullBootBase.xc/S 1411/V"): it provides MoveBlock
(static M[0o1013] = 0o3513 - verified), the interrupt-window sub
(0o2014), the channel handlers, the RCLK timer pump (M[0o1113] =
0o2330), and the raw ether exchange service (M[0o1012] = 0o3203,
entry chain 0o3203 -> JSRII @M[0o1017]=0o3530 -> the SIO+poll at
0o3205-0o3232). The eternal loop never calls the Context package's
Block (0o47701) - all its JSRII statics resolve inside the module -
so the coroutine ring stays frozen (ring resume PCs identical across
every run) while the bootbase service spins. NetExec's socket-level
204B requests are emitted from THIS machinery (pre-context), not
from the GatewayListener context. The received replies are stored at
VM 0o3531+ (verified EIT stores) - into what appears to be the
service's own buffer/return area; the per-exchange pointer cells
(M[0o3144..0o3150] = 600B..610B) are also REUSED as scratch at
runtime, so static disassembly of the @-operands is unreliable -
trace live.

SESSION-6: THE RAW SERVICE'S STATE MACHINE IS DECODED. Live dispatch
tracing through one full round (gate 92222590+) proves our reply
passes EVERY filter check and dispatches by Pup type:
- 201B -> handler at @M[0o3355]: inverts the 16-word cursor bitmap
  (the observed solid-block cursor!), TURNS THE PACKET AROUND
  (word0 byte-swap, dPort/sPort exchange, type <- M[0o3505]) and
  transmits it back - the "verbatim echo" fully explained, it is the
  service ANSWERING a ping.
- 202B -> handler at @M[0o3407]: arms the deadline cell M[0o3315] =
  RTC430 + M[0o3313](=207B ticks ~ 5.4 s).
- 203B -> handler at @M[0o3325]: if armed, restores the saved ether
  block (two MoveBlocks from the 0o3613/0o3646 save areas) and
  RETURNS TO THE CALLER - the clean exit. (Sending 203B as FIRST
  contact derails: nothing saved yet. A 203B mid-stream after 201B
  pings showed no visible exit - sequencing not yet right; possibly
  202B must arm first, or data must arrive between.)
- The machine's own 204B requests to psRouteInfo share the same
  socket 60B - they are THIS module's requests; body [0o401, 0o1000]
  (= net1-host1 port + 0o1000?). WORKING HYPOTHESIS: FullBootBase
  fetches something over the net at startup (page/image fetch or a
  server handshake): request 204B, responses typed 200B..203B with
  202B=start(arm timeout), data, 203B=done(restore+return). Next:
  decode handlers @M[0o3352] (type 200B) and the 202B-then-data path
  (where does a post-202B packet's BODY get copied?), and try the
  sequence 201B, 202B, <payload?>, 203B. Identify the protocol by
  finding FullBootBase's source (BuildBoot /B 300/N switches;
  search AltoSource for FullBootBase.asm/bcpl or BuildBoot docs).

SESSION-7 BREAKTHROUGH: THE MACHINE IS SITTING IN SWAT. The raw
module is **TeleSwat** (TeleSwat.asm fetched into
chm/altosource/buildboot.dm!2_/ along with FullBootInit/BootBase/
SaveState): socket 60B, ptSwatStore=200B (poke), ptSwatFetch=201B
(peek - the "echo" was a memory-read reply!), ptSwatSwap=202B,
ptSwatSwapReply=203B; the solid-block cursor is the SWAT CURSOR
(userCursor/swatCursor exchange); the eternal loop is TeleSwat's
il1/il2 receive loop; OutLd at VM 0o3317, OutLdRet at VM 0o3323.

THE CRASH: NetExec called CallSwat during startup. The swat-saved
state (page 0o700 area: AC1=0o142476 = the first context,
AC2=0o142502 = its CTX.StackMin VALUE, saved-PC-ish 0o47720 = inside
CallContextList's runCtx `jsr @1 2`) plus the ring dump
(ctx[0].Stack == 0) give the mechanism: **the first context's
CTX.Stack word (VM 0o142477) is 0/corrupt at first resume**, so
`jsr @(AC2=0)+1` jumps through M[1] (boot-loader residue) into
garbage -> trap -> CallSwat -> TeleSwat. The OTHER contexts'
CTX.Stack values are CORRECT (ctx[1] = pupLevel1Ctx has
stack = ctx+0o253 = region+175-4 exactly per InitializeContext), so
InitializeContext works in general - something specifically
corrupts ctx[0] (FeedEther, the 40-word eFSS context, the FIRST
allocated+enqueued) between its init and the first resume.

SESSION-10 (2026-06-12): FOUR more microarchitecture fixes, each
verified and committed (d40d6aa, 30cbf0e):
1. Multiply consumed the LATCHED Carry' instead of the live ALU
   carry-out (MulSub's A0 entry steps are logical ZERO ops) - every
   product gained a phantom 100000B partial term.
2. BLOCK now clears the task's wakeup-request FF unconditionally
   (HM p27). EOT's `Output_ TurnOffTx, Block` used to re-wake and
   retransmit every packet ~13x; once the BCPL driver cleared
   eOCLoc, the microcode posted CountZero (2377B) and EtherInterrupt
   SysErr'd (the 92.3M-cycle crash). EFTP now sends exactly one
   copy per Ack (88 replies for an 88-packet file, was 484).
3. Divide/CDivide implemented (were no-ops): Pd<-ALU[1:15],,Q[0];
   Q<-Q[1:15],,ALUcarry (CDivide: carry'). Stub made lenPup
   unhalved -> 25 PBIs of 564 words -> sysZone exhausted -> Alloc
   error 1801 at Title's 461-word display-line Allocate.
4. ALU=0/ALU<0 branch flags latch from the ALU output, NOT from the
   FF-transformed Pd (HM p30) - DivSub's FinalAdd remainder fixup
   was being skipped.
Probes added to the full-boot summary: BCPL stack walk from the
swatted AC2, runtime code windows, sysZone boundary-tag census
(zone base 123240B, min/max in hdr words 8/9, signed length word
per block, negative = allocated). Forensic recipe that worked,
repeatedly: STORE_TRACE_VA="700,707" -> swat cycle; then
TRACE_GATE + IFUDISP_TRACE -> br31(hex)=Nova word PC of each
dispatched opcode; disassemble NETEXEC.BOOT (1:1 file-word = VM
word) at those addresses; JSRII (64400B) = call through M[M[.]];
M[1303B]=lvSysErr; CallSwat=16251B; M[2..7]=77400B trap words;
Sys.Errors (in OSSOURCES.DM!2, fetched local) decodes error codes
(1801=zone full, 4267B=ecBadEtherStatus...).
STATE at end of session: 400M-cycle boot has healthy sysZone
(15858 alloc/6268 free), BOTH Title text bands in the DCB chain
(42-line strip + 2x 38-word x 6-scanline bands at 142544B/140606B
+ tab bands), eftp_replies=88, fb still cursor-only (DWT renderer
never paints the DCB text - separate gap).
DIVIDE-VECTOR GUARD keeps NetExec alive past the BitBlt crash
(2026-06-12): the periodic Title BitBlt that sprays the OS low core
in page zero corrupts the M[0o344] divide transfer vector, which is
what crashed the booted world into Swat ~124-190M (the user saw
"types a little bit then stops responding"). Mitigation: a
physical-cell write guard in memory.c (protect_phys/protect_val,
forced in the DM_REF_STORE path) armed by the machine once NetExec's
OS init has set M[0o344] to a plausible OS-resident code address
(0o2000..0o7777, captured ~63M). Verified: with the guard,
M[0o344] stays 0o4155 and Swat OutLdRet=0 at 160M (not crashed);
typing "Probe" at 110M echoes at the ">" prompt in a 200M snapshot.
This is a BRING-UP WORKAROUND. (SESSION-11 UPDATE: the BitBlt
destination-BR arithmetic is NOT the bug -- it is correct and
faithful to hardware; BR[0o22] wraps to page zero because the BBT
the caller passed already points there. The real fix is the
upstream BBT/context corruption. See SESSION-11 below.) It guards
only the divide vector; other
sprayed cells (0o412/0o460 MASKTAB/0o526/0o574/0o642) are not yet
protected but do not appear to crash the world.

LIVE SDL WINDOW + KEYBOARD + MOUSE (2026-06-12): the emulator now
has a windowed frontend. `make sdl` builds build/dorado-sdl (SDL2,
pure C; the core stays C99). It boots the machine and presents the
808x606 Dorado display in a window, rasterizing the Alto display
list from memory each frame; host keyboard is translated to the
Alto key matrix and host mouse to the Alto mouse cells, both
delivered to the running world once it is interactive. Run:
  make sdl && ./build/dorado-sdl --eb /tmp/aemu_only.eb
Flags: --eb --eftp --quote --no-alto-boot --scale N --speed CYCLES.
F1 pauses; Cmd/Ctrl+Q quits. Machine API added (machine.h):
dorado_machine_set_key (dorado_display_key), dorado_machine_set_mouse
(x,y,buttons; MOUSEX 0o424 / MOUSEY 0o425, buttons UTILIN
0o177030-0o177033 active-low, Left=4/Middle=1/Right=2 per ContrAlto),
dorado_machine_interactive. The machine forces the boot keys
(BS-down) until the EFTP transfer starts (eftp_max_seq>0 = boot
selection done), then releases to live host input -- so typing
reaches NetExec's command line. Mouse cells (MOUSEX/Y) and the
ReadTerminal 7-wire message map were taken from AEmu DisplayAux.mc
(msg types: 1-4 kbd 177034-177037, 5 buttons 177033/177030, 6 mouse
deltaX/Y, MouseXLoc=424) and salto/ContrAlto. Headless `dorado`
still renders the banner (2070 px) and all 10 suites pass after the
seed-block change. NOTE: NetExec is keyboard-driven and may not move
the cursor from the mouse; the mouse plumbing is correct for OSes
that read MOUSEX/Y. Next: a windowed live display naturally wants
the cycle-accurate DWT path (still iofetch=0) for an authentic
self-refreshing screen rather than the per-frame display-list
rasterizer; and an optional ImGui debug target.

STANDALONE BINARY BOOTS THE BANNER (2026-06-12): SOLVED. The
missing piece was the Mesa boot-parameter seed: the harness seeds
STK[1]=0110 (boot file number), STK[2]=056623 (BootParameterSeal),
STK[3]=0121045 during Initial (pre_pc 06170-06217 / 06406-06443,
force_ether_mesa_boot defaults ON) so the loaded world selects the
normal Mesa boot instead of falling into the cold/NoStorage path.
Ported into src/machine.c. The standalone `dorado` binary now boots
NetExec over Ethernet end-to-end: rx=1, bol=1, EFTP transfer
complete (88 packets, seq 87), DASTART=176762, and the
"-- XEROX BCPL Net Executive" banner rasterizes (2070 pixels,
identical to the harness). Run:
  ./build/dorado --eb /tmp/aemu_only.eb --cycles 130000000 \
                 --out screen.pgm
The diagnostic env knobs (DORADO_MACHINE_TRACE / _PCHIST /
_INITMEM / _NO_ALUFM_RESTORE, config_word in --progress) remain
for future bring-up. Decisive technique that found it: PC_COUNT +
PCWATCH side-by-side between harness (works) and machine (NoStorage
loop) - both hit InitMap 0o1011 with identical T=000025 but the
harness arrived via the warm path (STK boot parameter present) and
the machine via the cold path, diverging at the 0o1011 branch.

STANDALONE EMULATOR (2026-06-12): the first runnable binary outside
the test harness now exists. include/machine.h + src/machine.c lift
the boot orchestration out of test_cpu.c's probe_full_boot_with_
bootstrap into a reusable library (dorado_machine_create/run_until/
render_display_list/booted/debug); src/dorado.c is the CLI frontend
(build/dorado). Build: `make build/dorado`; run:
  ./build/dorado --eb /tmp/aemu_only.eb --cycles 124000000 \
                 --out screen.pgm --progress
Status: boots the full hard chain through STAGE 1 (BaseBoard cold
boot, 3 boot-button presses, Bootstrap swap @0o7740, canonical
Initial substitution @0o7717, PresetMap identity-map shim @0o6222,
CheckChecksumAndLoad @0o6460, LoadRam handoff -> world loaded @~32M,
booted=1, eth requests=1 replies=63). Library sizes storage to one
4MW module directly (no DORADO_STORAGE_MODULES dependency) and
seeds boot keys / parks I/O tasks in-code.
STAGE 2 remaining gap (precisely localized): at the LoadRam handoff
the machine now parks tasks 1..15 (invalid TPC, junk off) and gates
display wakeups on DASTART, so the EMULATOR task (0) runs in the
loaded world instead of being starved by stale-TPC I/O tasks (was
task 3 then task 2 spinning at 0o7003/0o234). Task 0 advances
0o5400 -> 0o7116 -> 0o7012 then STALLS at ~0o7012 before turning on
the Ethernet receiver (rx stays 0) or setting DASTART, so
breath-of-life is never accepted (bol=0) and the banner never
draws from the standalone path. FINDING-3 (ROOT LOCALIZED to InitMem NoStorage): a side-by-side
PC_COUNT comparison nails it. The harness and the standalone
machine BOTH hit AEmu InitMap (0o1076) exactly once at cycle ~31M,
but the harness then hits the loop addresses (0o7140/0o7167/0o5400/
0o7012/0o7116) ZERO times while the machine loops there millions of
times. The machine's init sequence shows it reaches 0o1011
(GOTMAPCONFIG) and branches to 0o1017 (NOSTORAGE) - InitMem.mc
decides NO storage modules are present and falls into the NoStorage
breakpoint loop. InitMem.mc GotMapConfig does:
  ModMask_ NOT(Config'); ... ModMask_ LSH[ModMask,10];
  ModMask_ T_ (ModMask) AND (170000C);
  ITemp1_ T, Branch[NoStorage, ALU=0];
RULED OUT (each measured): Config' itself is CORRECT - the machine
prints config_word=0o214 (= module-0 present + 64Kx1 chip), so
NOT(Config')=0o214 and after LSH/AND it should be nonzero (storage
present). Storage is correctly 1 module (4194304 words). Skipping
restore_standard_alufm (DORADO_NO_ALUFM_RESTORE=1) does NOT change
it. Fault-task park, identity-map shim: irrelevant. So config_word
is right but the GotMapConfig computation still yields ALU=0 ->
NoStorage. The remaining divergence is engine REGISTER STATE at the
~6 GotMapConfig instructions (ModMask/ShC/T/ALU flags) - the
machine and harness must differ in one register feeding the
LSH/AND/ALU=0 test, OR in the SetDMuxAddress muffler reads
(0o7000-0o7076) that run just before GotMapConfig (these set
VirtualBanks and clobber T/XTemp17 via MidasStrobe; if the
machine's muffler model returns different data the path differs).
NEXT (decisive): instrument BOTH at GotMapConfig - dump ModMask,
ShC, T, ALU=0 right before the Branch[NoStorage] in the harness
(works) vs the machine (NoStorage) and diff the first differing
register. Suspect the muffler/DMux read (SetDMuxAddress) since
that is the least-tested path and runs immediately before.
Tooling added: config_word in --progress debug;
DORADO_NO_ALUFM_RESTORE=1 to A/B the ALUFM restore.
OLD-FINDING-2 (DORADO_MACHINE_PCHIST init sequence):
the world starts at AEmu InitMap (1070->1076), runs a long init
clearing loop (4426 4436 4436 4437 x N), then DELEGATES to a shared
InitMem routine in INITIAL's address range and gets stuck in a
repeating loop: 7140 7167(x9) 7166 7172 7175 ... 7065 5400 5430
5434 5450 5454 5470 5474 7066 7064 7160 7171 (these addresses are
Initial/kernel leftovers; AEmu.mb has no code there, and
aemu_only.eb is AEmu-ONLY so it relies on the substituted
init_mc Initial/kernel/memMisc routines - the SAME setup the
harness uses to reach the banner). Confirmed NOT the cause:
storage is correctly 1 module (stwords=4194304); removing the
fault-task park did not change it; the identity-map shim ran.
So the loop is a shared InitMem/InitMap routine that terminates
in the harness but not here. The remaining divergence is in the
post-LoadRam world execution and needs a SIDE-BY-SIDE task-0 PC
comparison: run the harness (test_cpu) with the identical env
(DORADO_ALTO_BOOT_ETHERNET=1 DORADO_NO_DISK=1
DORADO_ETH_BOOT_110=/tmp/aemu_only.eb DORADO_STORAGE_MODULES=1)
and a focused PC trace over 5400/7140/7167, and diff against the
machine's init sequence to find the first instruction where they
diverge (likely a register/Cnt/ALUFM value feeding the loop's
exit test). Tooling: DORADO_MACHINE_PCHIST=1 --progress dumps the
machine's task-0 hot PCs + first-600 init sequence + storage size.
OLD-FINDING: the AEmu world DOES start (task 0 reaches AEmu
real 0o1070, which carries the Multiply FF), but it then LOOPS in
KERNEL-level microcode at 0o7003-0o7167 (these addresses are
present in kernel.mb/Initial.mb, ABSENT in AEmu.mb) instead of
completing its init and reaching EBoot. Hottest task-0 PC 0o7116
(BLOCK=1 emulator op). So the world boots and faults/waits into a
kernel handler rather than progressing - a different failure mode
from the harness, which reaches the banner. Tooling added:
DORADO_MACHINE_PCHIST=1 dumps the top task-0 PCs of the loaded
world; DORADO_MACHINE_TRACE=1 logs boot-state transitions.
NEXT: side-by-side compare the loaded-world execution against the
harness (same env, DORADO_PC_COUNT) to find where they diverge -
prime suspects: (a) my world-loaded detection fires mid-LoadRam
(32M; CLAUDE.md notes the ether boot costs ~61M) so the
task-park / ALUFM-restore happens too early and corrupts the
world's init; (b) the world's emulator loop dispatches to an I/O
task I parked (junk/fault) and waits in kernel for it; (c) an
unhandled fault traps to the kernel handler at 0o7116. Try: defer
the world-loaded handoff (task-park + ALUFM restore) until task 0
actually reaches AEmu's start region for a sustained run, rather
than the first non-Initial PC. OLD-NEXT: trace task 0 from world-load
to the 0o7012 stall (DORADO_MACHINE_TRACE + a PC histogram) and
compare against the harness's loaded-world init - the harness
NATURAL path did NOT reset TPCs yet still reached the banner, so
either (a) my world-loaded detection fires earlier than the
harness's (restoring ALUFM / parking tasks mid-init), or (b) the
loaded world needs an I/O task I parked (junk timer? fault task?)
to make progress past 0o7012, or (c) the keyboard BS-down seed
isn't seen at 0o7012's boot-decision read (MDS bank). The banner
ITSELF renders correctly from the test harness (commit 6bbdf7d);
this is purely about reproducing it from the standalone binary.

MILESTONE (2026-06-12): FULL SCREEN DRAW ACHIEVED. The NetExec
banner "-- XEROX BCPL Net Executive" + machine/date line now
rasterizes to the framebuffer. Mechanism: a direct display-list
rasterizer in probe_full_boot (tests/test_cpu.c, just before the
snapshot) walks DASTART(VM 420)->DCB chain straight from MDS memory
- Alto DCB format w0=next, w1=ctl(b15 res,b14 inv,b8:13 HTAB words,
b0:7 NWRDS), w2=SA bitmap word addr, w3=SLC (scanlines, rendered
x2 vertically) - and paints each DCB band with dorado_display_set_
pixel (MSB=leftmost, 1=black). At a 124M-cycle budget (BEFORE the
page-zero/divide crash at 124,025,617) NetExec is healthy and the
display list holds two 38-word x 6-SLC text bands (the title +
machine/date lines, DCB[1]@142540 DCB[2]@140602) over a 42-SLC
blank strip. dl_pixels=2070. Snapshot: /tmp/dorado_dcb.pgm; ASCII
reference checked in at docs/netexec-banner.txt.
This is a SNAPSHOT rasterizer (reads the display list NetExec
built); it is NOT the cycle-accurate DWT path. The real DWT word
task still barely runs (iofetch=0, dwt_wakeups=1) because the DHT
(task 3) does not repeatedly set the WCB/raster trigger flags that
dorado_display_dwt_wakeup() needs - that remains the open item for
a hardware-accurate live display. Two remaining gaps for a
self-sustaining boot: (a) DHT->DWT wakeup chain (display.c
next_wcb_flag/raster_next_wt_flag are set from DHTFlag/RAST_TASKCMD
writes but the DHT isn't issuing them per scanline), and (b) the
BitBlt page-zero destination crash at 124M (BBT with a page-zero
DBCA; see SESSION-10d).

SESSION-11 (2026-06-12): TWO RESULTS -- (a) implemented the stubbed
XorCarry/XorSavedCarry FF functions (cpu.c); (b) PROVED the BitBlt
destination-BR arithmetic is CORRECT, redirecting the page-zero hunt
upstream. Do NOT keep chasing the "BitBlt destination-BR math" -- it
is faithful to hardware. Evidence:
- XorCarry/XorSavedCarry (HM Table 11a, FA=0 FB=2 FC=4/5) were
  no-op stubs. Now implemented: they XOR the carry-in (bit 5 of the
  ALUFM entry) before the ALU runs -- XorSavedCarry with the SAVED
  (previous-instruction) carry, XorCarry unconditionally. The BitBlt
  bottom-to-top base math (T_ T-1, XorSavedCarry; BBNormal
  T_ T+EmuBRHiReg, XorSavedCarry) is the canonical user. All 10
  suites pass; banner pixel count unchanged (1478 px at 124M, pre ==
  post), so no regression.
- Forensics on the M[0o344] corruptor (DORADO_STORE_TRACE_PHYS=344,
  new phys-keyed store trace in memory.c): the legit OS write sets
  M[0o344]=0o4155 (pc=0o1622, mb=36/MDS, br=0, mar=0o344) at ~64M;
  the corruptor at cyc 122,993,593 is pc=0o1201 (GRAYLOOP), mb=22,
  br=0o1777700642 (=0xFFF81A2: br_hi=0xFFF, br_lo=0x81A2), mar=0x7F42.
  (br+mar) & 0x0FFFFFFF wraps at 2^28 to 0o344 -> writes 0o4154.
- The XSC trace (PC range 0o1405-0o1426) shows the math is correct:
  pc=0o1424 BBTemp-0x8000 BORROWS (savedC=0), so pc=0o1423
  T_ T-1, XorSavedCarry correctly yields T=0o177777 (the intended
  bottom-to-top bank decrement). The 12-bit br_hi + 28-bit BR-adder
  wrap then brings the bank back to 0 exactly as real hardware does.
  So va=0o344 is the FAITHFUL result of the given register inputs --
  the destination base the BBT specified is itself in page zero.
- CONCLUSION (confirms SESSION-10b/10d): the root is an upstream
  malformed BBT whose destination is page zero, traced to a corrupt
  BCPL context stack pointer (ctx!1) running with its frame in page
  zero. The divide-vector guard stays as the bring-up workaround
  (now also gated by DORADO_NO_DIVPROT for A/B testing); it is NOT a
  microcode arithmetic bug. NEXT: resume SESSION-10b's hunt for who
  writes the bad ctx!1 (STORE_TRACE_VA on the context block), i.e.
  the allocator/context-overlap corruption, not the BitBlt.

SESSION-10d: REF_W (symbol-independent) CONFIRMS it. Per-ref dump
(DORADO_STORE_WINDOW) at cyc 124,025,621: STORE (kind=5) at uPC
1201B, MemBase=22B (Alto MDS bank, correct), BBDst=0xE4=344B,
data=4154B; paired StoreLastDst at 361B writes 345B. Destination
writes land at SCATTERED page-zero cells (413B,344B,345B) across
scanlines, so the BBT's DBCA/DBMR are garbage. The store IS BitBlt
(BITBLT=61024, BitBltA SD400+24); the UpdateSector symbol mismatch
was external-symbol-file straddling. So the bug is upstream: a
malformed BBT with a page-zero destination base. NEXT: dump the
BBT words at BitBlt entry (Fetch 2S=DBCA,3S=DBMR off the BBT
pointer) and trace who built that base.

SESSION-10c: THE PAGE-ZERO CLOBBER IS A BAD BitBlt DESTINATION.
The store that flattens M[344B] is BitBlt microcode: uPC 1201B=
GRAYLOOP / 361B=STORELASTDST (AltoBitBlt.mc), i.e. a Nova BitBlt
gray-fill writing word after word starting at BBDst~344B. BBDst is
computed in BitBlt setup as DstY*DBMR (scan-line * raster, via
Call[MulSub]) + DBCA(base), MemBase=BBDstBR. So the destination
landed in page zero because EITHER (a) the DstY*DBMR offset or the
base-add wrapped, or (b) NetExec passed a BBT whose DBCA/DBMR were
already garbage. Recurs at EXACTLY 124,025,617 and 187,450,772 (a
periodic Title-banner redraw via EraseBits->BitBlt); earlier
redraws built the DCB bands correctly, so it is DATA-DEPENDENT - an
arithmetic edge case, not a blanket BitBlt failure. BitBlt setup
uses the just-fixed MulSub AND a `Branch[.+2, ALU=0]` skip-multiply
test - both in recently-touched paths; re-audit in BitBlt context.
NEXT STEP (concrete): gate to the failing BitBlt (cyc ~124,025,000),
capture the BBT it reads. Setup fetches BBT words via Fetch_ 2S/3S/
4S/5S/7S (DBCA/DBMR/DLX/DTY/DH) off MemBase=BBT pointer; print
T/Q/DstY/DRast at GRAYLOOP entry (uPC ~1175-1202) to recover DstY,
DBMR, DBCA and the final BBDst. If BBDst != DBCA+DstY*DBMR by hand,
the bug is our address arithmetic; if it equals but DBCA~0, walk
back to who built that BBT (CmdScanDisplay EraseBits / a display-
stream bitmap base) and whether that base came from an earlier
mis-multiply/divide. Identify the BitBlt caller via IFUDISP br31
just before uPC enters the BitBlt trap / 1140B SETUPBRS.

SESSION-10b: THE FillWithDash SWAT IS FULLY TRACED - it is page-zero
corruption, not a divide bug:
1. M[344B] is the OS page-zero DIV transfer vector (jsr @344,z from
   compiled BCPL x/y). FullBoot init sets it to 4155B (the divide
   routine; word 4154B before it is a `jmp @1,2` thunk).
2. At cycle 124,025,617 (and again 187,450,772) Nova code running
   with its BCPL FRAME IN PAGE ZERO (AC2 ~ 270B-324B!) executes a
   normal-looking store sequence (function at 12672B..13105B,
   GetFrame size 42B, vec buffers IN FRAME at frame+const) whose
   in-frame "region" writes land on 344B/345B, leaving M[344B]=4154B.
3. Every divide thereafter goes jsr @344 -> jmp @1,2 -> resumes at
   the previous call's nargs word -> executes 000002 = jmp 2,z ->
   M[2]=77400B trap -> Swat at 190M. (The AC-save 2/135B/7 is just
   the divide args/remainder mid-veneer; MulSub/DivSub/DIVx are all
   CORRECT - test_mulsub_aemu, test_divsub_aemu, test_divx_aemu.)
4. WHY is a frame in page zero - CORRECTED after tracing: the
   "IST with Stack=0" reading was WRONG. InitAltoEther
   (PupAlEthInit.bcpl) redirects interruptVector!i to
   `lv ndb>>EtherNDB.asmProlog` - M[502B]=160224B is the Ethernet
   NDB's asm prologue, and the "Stack/InitPC" words I read were NDB
   queue fields (STORE_TRACE on 160233-160240 shows them written
   constantly by Enqueue/Dequeue at br31=50033B/50050B and a
   counter at 50142B - normal). The REAL IST is at
   ndb+lenEtherNDB, not yet inspected.
   The corrected suspect: routine 4551B is the Context package's
   Block/yield (AC2 <- ctx arg, ctx!1 <- suspended frame). The
   bad slice ran inside a context resumed at cyc 124,016,575 whose
   StkMin (M[335B]) was set to a HEALTHY 155303B, yet the BCPL
   frame was ~270B: i.e. **the context's CTX.Stack field (ctx!1)
   held ~270B garbage while CTX.StackMin (ctx!2) stayed intact**.
   Context resume trusts ctx!1 with no bounds check, so execution
   continued at a page-zero frame and its in-frame vec stores
   flattened M[344B].
   NEXT STEP: the corrupt context block is at ~155277B (StackMin
   155303B = ctx+3+extra). Run STORE_TRACE_VA="155277,155304" to
   catch who writes the bad ctx!1 (one 400M boot, watch values
   ~270B), then trace that writer. Candidate mechanisms: a Block()
   while already running on a clobbered frame (cascading), an
   interrupt-path save storing a bad frame, or one of our
   remaining microarchitecture gaps corrupting AC2 mid-slice.
   Also identify which context 155277B is (walk the ctx ring at
   probe end and print each ctx's Next/Stack/StackMin/proc).
5. Probe additions this session: IntVec/IST dump, sysZone census,
   BCPL stack window, code windows. ETHC control-write trace env:
   DORADO_ETHC_TRACE=1 (+TRACE_GATE).

UPDATE (same session, post-commit): with the Divide fixes the boot
now runs to cycle ~189.97M before the SAME FillWithDash swat
(STORE_TRACE_VA="700,707" -> AC-save at 189968878-189970790;
AC0=2 AC1=135B AC2=7 AC3=157167B PC=21071B). That is ~97M cycles
(11+ emulated seconds) of healthy NetExec running - many Title
banner redraws succeed before one divide dies, which all but
proves an ASYNC interference (a task interleaving mid-DivSub).
Junk.mc's `Q_ T` is in an emulator-only subroutine (ReadCounters),
NOT the junk task body; DisplayMain.mc and AltoEther.mc contain no
Q_/Cnt_ at all - so the obvious Q/Cnt clobber suspects are
exonerated at the source level. NEXT: capture the microinstruction
interleaving in the ~2000 cycles before the swat (gate
189966000,189971000; need a per-cycle task+uPC trace - add one if
DORADO_PCWATCH doesn't fit) and watch DivSub's Cnt/Q/latched-Carry
across the interleaved task slices; verify our task-switch
save/restore of the latched ALU branch flags vs HM, and whether
Return/SCall skew interacts with a task switch at the Return
boundary. The swat cycle moved 92.9M -> 190M with identical
signature across the last two binaries, so it reproduces
deterministically per build.

OPEN: one remaining swat, PC=21071B = return from `jsr @344,z`
(the BCPL divide veneer) in FillWithDash's dash-count divide
(NetExec.bcpl Title path, code at 21050B-21105B): AC-save shows
AC0=2 (correct remainder of 93/7), AC1=135B (=93, quotient NEVER
stored), AC2=7, AC3=157167B. BUT test_divx_aemu drives the exact
same S-Group DIVx wrapper (STK-resident ACs, 93/7) and PASSES.
So the in-vivo failure involves state the unit test lacks - prime
suspects: (a) a mid-DivSub task switch clobbering shared Q or Cnt
(EOT/EIT/display tasks interleave in vivo; check whether their
microcode touches Q/Cnt and whether Cnt must be per-task), (b) the
runtime veneer at M[344B] doing something beyond Nova DIV, (c) an
IFU/Reschedule interaction at DIVx's IFUJump[0] tail. NEXT STEP:
STORE_TRACE_VA="700,707" for the new swat cycle, then gated
IFUDISP+ETHC trace around it; also dump runtime M[344B] veneer
code via the code-window probe. After that swat falls: check
whether Title's PutTemplate writes glyphs into the band bitmaps
and make the DWT/display pipeline render the DCB chain (probe
"Display render" still shows A_pixels=0).

SESSION-9 ROOT CAUSE + FIX: **the Dorado Multiply FF function was
a TBD no-op** (cpu.c FA=1 FB=7 FC=2). AEmu's Nova MUL microcode
(Various.mc MulSub) steps on it; HM p23 gives the full semantics,
now implemented: Result <- ALUcarry,,ALU/2; Q <- ALU[15],,Q/2; next
branch address OR 2 if pre-shift Q[14] (per-task, via the
task_dispatch plumbing). Every BCPL multiply was garbage before -
CreateKeywordTable allocated 15 words instead of ~82 (verified via
the SB length word at 0o142450: -16), LoadKT's second entry then
overran into the first coroutine's CTX. Post-fix the boot moves
STRICTLY FORWARD: the KT-overrun crash is gone, contexts run
further, CallContextList returns to its caller (CtxRunning =
dummyCtx at probe end), and a NEW later CallSwat happens with crash
site ~VM 0o16277-0o16306 (the BCPL call veneer region; saved frame
~0o172142). fb_nonzero=59 (a third cursor state). NEXT: same
forensic loop on the new crash - dispatch-gate around the new
OutLd call (find its cycle via STORE_TRACE on 0o3323/OutLdRet or
the swat AC-save writes), identify the SysErr, fix the next
emulation gap. Each iteration of this loop has removed exactly one
fundamental microarchitecture bug (BDispatch, RBase, Map Mar, junk
timer, Multiply...).

SESSION-8 FORENSICS (led to the above) (the corruption, three layers down): the
4-word Zero at VM 0o142474 is `Zero(ptr,4)` (BCPL veneer at VM
0o4536) called from the keyword/boot-directory entry machinery at VM
0o31675-0o32100 (timeline = AfterJunta's CreateKeywordTable/LoadKT,
~92.09 M). The getter at 0o31675 walks a keyed chain at q!5
(node = [next@0, key@1, ...]; on key match `resultis node+1`) and
MATCHED key 0o100000 against STALE memory: node = 0o142473 points
into a just-recycled allocation. The legitimate history of that
memory: a 4-word object (data 0o142471-74, sb 0o142470, length -5 -
all CORRECT zone arithmetic, verified by store traces at cyc
92060802+) was built with packed-field stores (field merges verified
CORRECT - dorado/CLAUDE.md's "ShMd stubbed" note is STALE, the
implementation merges real Md), then released at cyc 92078882+
(-1 writes at 0o142470/71 from br31=0o50764 ~ AltoTimer region, then
Enqueue(q, 0o142470) at br31=0o50033). The mystery: the lookup chain
head q!5 contained 0o142473 (not the enqueued 0o142470) - find who
wrote 0o142473 into the chain head. NEXT: identify q (frame!4 of the
0o31675 call - dump via a PCWATCH at 0o31701 reading AC3, or trace
the Enqueue at 92080914's queue argument), then STORE_TRACE the
chain-head cell. Every software step verified so far is legal -
expect ONE wrong emulated instruction upstream (the remaining
unverified exotic: `dsz/isz @indirect`, the BCPL veneer's numargs
walk, or a carry case in the 0o31707 sub# compare chain).

PREVIOUS (superseded) NOTE: THE CORRUPTOR IS CAUGHT (the one-command probe ran): the CTX life
in stores: InitializeContext writes Next=0, StackMin=ctx+4,
**Stack=0o142542 CORRECTLY** (cyc 92053271, br31=0o47764), ctx!3=ndb,
Enqueue sets Next - and then at cyc 92095284 a Zero()/BLKS issued
from caller code at ~VM 0o4536 wipes ctx+0 AND ctx+1 (CTX.Stack!).
Later enqueues restore Next but Stack stays 0 -> the first resume
crashes into Swat. The Zero belongs to a LATER Allocate (timing
matches InitPupLevel1's `pupRT = Allocate(zone, n); Zero(pupRT, n)`,
the 20+1-entry routing table) - **the zone allocator (Alloc package)
returns a block overlapping the earlier FeedEther-context
allocation on our emulator**. NEXT: REF_W window 92095200-92095400
to get the exact zeroed range; reconstruct the Allocate sequence;
find the emulation defect in the allocator's arithmetic (the Alloc
package's compare/carry chain - the same instruction-semantics class
as the BDispatch/RBase/Map bugs). Fix it, and the contexts run:
Title paints the banner.

ALSO AVAILABLE NOW: TeleSwat gives a REMOTE DEBUG CHANNEL into the
parked machine (Fetch/Store/Swap) - the fake server can peek/poke
arbitrary VM and even resume; and CallSwat's message argument (a
BCPL string) is reachable from the swat frame for an exact error
identification.

STILL OPEN (the current frontier):
- The 204B routing requests never STOP - the replies reach the
  machine (InDone + EIT consumption verified) but the requesting
  context never consumes them. Removing the dummy-CRC trailer word
  from replies reduced the retry rate substantially (EtherPupFilter's
  length equality is sensitive to the trailer shape) but did not
  satisfy it. Calibrate the exact on-wire length convention the
  PBI/EtherPupFilter math expects: packetLength must equal
  (pup.length+5) rshift 1; derive the eELoc/stored-words accounting
  from AltoEther.mc's receive loop and match the trailer exactly.
- ANOMALY worth chasing: NetExec re-transmits our replies VERBATIM
  (ether word0 preserved with src=1, impossible for a normal send) -
  every reply bounces once. Either our model's EOT path leaks rx
  into tx, or the driver's eOB/eOPLoc gets pointed at the input PBI.
  Find who builds that transmission (trace the EOT data-write era
  after a delivery).
- The banner DCB still never appears (Title context still parked
  behind the unfinished routing exchange).

## Open questions (the active work)

0a. **NetExec auto-runs its EtherBoot command.** NetExec1.bcpl's
   MyEtherBoot ends with DisableInterrupts(); StartIO(3); copy the
   boot loader to VM 1; goto 6 - and the observed window-opening loop
   at VM 0o1747 (poll cell, SIO, jsr 0o2014) plus the EFTP machinery
   (OpenEFTPSoc/ReceiveEFTPBlock with 1 s/10 s timeouts at NetExec1
   lines 268-273) are boot-fetch machinery. NetExec is likely
   auto-dispatching a boot (keys/BFN-derived) rather than sitting at
   a prompt; the wire is silent because its sends stall on the dead
   interrupt path below.

0a2. **The parked context is FeedEther; the full causal chain.**
   (Session-3 final state.) The context ring is live (CtxRunning
   valid, 8 contexts walked by the probe) and the RUNNING context has
   CTXStack=0 = first resume, never Blocked: that is FeedEther
   (PupAlEthb.bcpl), the first context on ftpCtxQ. Its body:
   CauseInterrupt to kick input arming, the tx-timeout branch
   (DisableInterrupts; StartIO(reset); @ePLoc = 3 lshift 8;
   EnableInterrupts), then Dismiss(4) repeat. The last EIR (92198761)
   followed by the last DIR (92198965) is an Enable-of-pass-N then
   Disable-of-pass-N+1 whose matching Enable never came - the flow
   died inside that bracket or in Dismiss. Dismiss's expiry rides on
   AltoTimer time, which advances off the 60 Hz vertical interrupt -
   circular once interrupts latch disabled. Title/Command never get
   their first resume, hence no banner DCBs.

   EXPERIMENT (committed, env-gated): DORADO_FORCE_EIR_AT=<cycle>
   clears NWW bit 0 (RM[023]) once at the given cycle. Result at
   93 M: interrupt fabric comes alive - 1496 deliveries (was 2),
   EIR/DIR cycling (1183/1181), cursor repainted continuously
   (rows_drawn 657 -> 3712) - but BRI executes 1.87 M times
   (delivery/dismiss storm; PCLOC re-delivery loop suspected) and the
   banner still does not appear. Next probes: (1) why the BRI count
   is 1000x the delivery count (check our BRI/PCLOC semantics:
   BRI fetches WW then PCLOC with the T_ MD, Fetch_ T at-issue
   pattern - verify the second fetch's Md routing); (2) catch the
   exact death between FeedEther's Disable/Enable bracket with
   IFUDISP gates at 92198965-92200000; (3) the Interrupt package's
   IntEnt saves/restores the Nova CARRY through MOVL/MOVZR around
   handlers - verify our A-Group carry for those forms (a corrupted
   carry in the interrupted mainline would derail comparisons).

0a3. **Final session-3 narrowing: the ether output never starts.**
   Hard facts from store traces: M[0o452] (WW) is NEVER stored
   post-LoadRam - so Interrupt.asm's CauseInterrupt (dir; WW |= mask;
   eir) NEVER RUNS, hence SendEtherPacket's kick (`if eOB eq 0 then
   CauseInterrupt(mask)`, PupAlEthb.bcpl line 70) is never reached or
   its guard fails; consequently no ether interrupt, no output start,
   no Mayday/routing-probe ever transmitted, and every higher layer
   (GetTime/GetDir/GetName, the boot fetch) waits forever. The
   dispatch trail shows SendEtherPacket's broadcast-loopback branch
   executing (MoveBlock x3 + filter walk) and then the flow entering
   an SIO + poll sequence at VM 0o3205-0o3232 whose shape matches
   PupAlEtha.asm's StartEther start2 path (zero @ePLoc, zero @eLLoc,
   lda outCmd, sio) followed by a deadline/event poll. The window
   sub at VM 0o2014-0o2022 (lda @X; sub; sta @Y; EIR; DIR; sta @Z;
   ret) runs to completion - the EIR;DIR pair is by-design and the
   sub returns normally; the system legitimately runs
   disabled-with-windows. 47 of 49 windows found nothing pending
   BECAUSE WW never got the ether bit. NEXT: single-step the FIRST
   SendEtherPacket call (the LocateNet routing probe during
   InitPupLevel1, before 92 M) - find why it never executes
   CauseInterrupt's `sta 1,@.Wakeups`: RESOLVED ONE MORE STEP: the
   probe now scans VM for the EtherNDB's ePLoc pointer table
   (consecutive 600B..610B) and dumps eIB/eOB/eState - ALL ZERO at
   probe end. eOB=0 means the guard would have passed; therefore the
   flow diverts BEFORE the `Enqueue(oQ); if eOB eq 0 CauseInterrupt`
   lines - inside the broadcast-loopback branch's filter walk
   ((pf>>PF.predicate)(ipbi) = EtherPupFilter in PupAlEtha.asm), or
   the MoveBlock cluster belongs to CompletePup's header/checksum
   construction and SendEtherPacket is never reached at all. Probe
   accordingly: trace the Alto PC from the MoveBlock cluster
   (~92.199 M) forward instruction by instruction and identify the
   divergent branch; suspect list: the Pup software checksum
   (add-and-left-cycle per word - carry/cycle semantics), and
   EtherPupFilter's hand-coded compares. The Alto-PC regions: SendEtherPacket around
   VM 0o315x-0o320x, CauseInterrupt would store @0o452.

0b. **NetExec stops opening interrupt windows at ~92.2 M cycles.**
   (Newest finding, post dispatch-RBase fix.) NetExec's idle loop
   polls cells (VM 0o1747/0o1755 poll loops, RCLK deadline compares
   at 0o2330, dispatcher at 0o3214) and opens interrupt-delivery
   windows via an `EIR; DIR` pair in a subroutine at VM 0o2014-0o2022.
   `DORADO_PC_COUNT="3100,3101,3102,3113,756,314,307"` shows DIR=49,
   EIR=51, BRI=2 executions ALL before 92.2 M, then never again -
   while AEmuReschedule traps keep firing per display field (1264
   traps, 1262 take RestartIFU because NWW has bit0 set = disabled).
   1262 vertical interrupts go undelivered; NetExec's display/queue
   machinery (which syncs DCB swaps to the vertical interrupt) never
   completes, so the text DCB never appears. The delivery mechanism
   itself is verified working (EIR's RescheduleNow trap delivered
   interrupts at 91.8 M, handler ran, vectors at INTVEC are
   installed). A 1.5 G-cycle run (23+ virtual seconds, RTC430
   reaching 0o1121) shows NO further progress: the post-92.2 M state
   is a quiescent polling dispatcher (VM 0o3214 loop + JSRII @M[1113]
   = 0o2330, the RCLK deadline-compare sub) with NOTHING queued -
   M[0o3315] = 0 (no timer armed; M[0o3316] = 0o430 is a stale
   deadline), @M[0o3344] = 0 (event queue empty), NWW = 0o110402
   (disabled + pending vertical bits). NetExec's keyboard/event
   inflow rides on the vertical-interrupt handler (delivered twice,
   at 91.8 M only), and its last critical section (DIR at 92198965,
   VM 0o2020 inside the EIR;DIR sample-and-update sub at 0o2014-22)
   never re-enables. RESOLVED TO A TIGHT TARGET via the
   BCPL sources (now in chm/altosource/netexec.dm!4_/ and
   contextsources.dm!1_/): NetExec.bcpl's AfterJunta builds exactly
   our observed spacer DCB (height=42), enqueues six coroutine
   contexts (Title, Command, GetTime, GetDir, GetName, WatchDog) on
   ftpCtxQ, and loops CallContextList. Title() paints the banner
   display streams on its FIRST resume after an initial Block() - and
   it never resumes. Context.asm (the OS-less Context package) shows
   the switch mechanics: resume = `jsr @1 2` (JSR indirect through
   CTXStack frame offset 1, with an inline data word after the jsr);
   Block() does `inc 3 3; sta 3 1 2` (save return+1 in the frame),
   saves the stack pointer in CTXStack, and chains to CTXNext. The
   observed "dispatcher" loop (VM 0o3214 + the RCLK compare at
   0o2330) is ONE context spinning in `Block() repeatuntil
   TimerHasExpired(...)` - its Block() is returning without cycling
   the other contexts. NEXT: unit-test our emulation of the exact
   instruction sequence in Context.asm (plain Nova `jsr @disp,2`
   indirect-indexed EA, `inc 3 3`, frame stores via sta n,2; check
   the JSR return-address convention against the inline-data word
   after `jsr @1 2`) and trace one Block() round at the microcode
   level. The statics CtxRunning/CtxCaller (.srel cells) likely map
   to the dumped cells around M[0o1113]. ContrAlto ground truth
   remains the fallback.


1. **Fault task storm + DASTART cleared.** With timing fixed, NetExec
   progresses further and the FLT task (17) burns 25 M cycles;
   DASTART reads 0 at probe end (earlier runs left a zero-width spacer
   DCB at 0o176762 whose SLC evolved 42 -> 1). DIAGNOSED ONE LEVEL
   DOWN: the FLT loops 1.48 M times at AEmu FAULTTASK/GETEMUFAULTPC
   (real 0o4151/0o4040/0o4133/0o4134/0o4141) with mar=0o177756 — the
   Alto-II XM bank-register page (bank reg for task N at 0o177740+N).
   NetExec probes extended memory through those registers; AEmu
   emulates them via the XM write-protect fault path (XMFaultTask.mc),
   and our fault/restart plumbing re-faults the same access forever
   instead of completing the emulated store/read. The emulation flow
   (XMFaultTask.mc): the FLT saves FaultVal from DBuf and the
   emulator's TPC, restarts the emulator at Fault0; Fault0 reads the
   faulting VA from the Pipe, then XMBStoreOnly clears the page-0o377
   write protect via `Map_ T, MapBuf_ FaultMapVal` + WaitForMapBuf,
   performs the store itself, restores WP, and resumes at the NEXT
   instruction via AEmuReschedule. A persistent loop therefore means
   the WP-clear or the emulated store is not taking effect in our Map
   model (the emulation's own Store_ re-faults), or the Pipe VA the
   FLT reads is wrong. First check: does our `Map<-`/`MapBuf<-` write
   path update the map entry's WP bit, and does `B<-Pipe0/Pipe1` at
   the FLT's ProcSRN return the faulting VA?
2. **No text DCB / blank framebuffer.** NetExec never created a
   non-zero-width DCB. May resolve once (1) is fixed. Independent
   quick win: the cursor bitmap at VM 0o431-0o446 IS populated and the
   THT feeds cursor X/Y + data words to the NLCB every field —
   modeling DDC cursor rendering into the framebuffer would give the
   first visible pixels.
3. **EOT duplicate transmissions.** Every Ack is transmitted ~13 times
   (OutDone storm; LoadOverflow 0o1777 posts seen historically). The
   transfer converges anyway (dup Acks just resend the current packet
   and the loader re-acks). Root cause is in the EOT output path / our
   always-empty bus-register wakeup model. Cosmetic for now.

## Debug tooling added this session

- `DORADO_STORE_TRACE_VA="lo,hi"` (octal) now prints the issuing
  microcode PC, IFU PCX, BR31, and prefetched opcode word per store.
- `DORADO_TRACE_GATE="lo,hi"` (cycles) gates cpu.c-level traces that
  lack a cycle counter (currently DORADO_IFUDISP_TRACE).
- `DORADO_ETH_TX_TRACE=1` dumps every completed transmit packet (and
  TX_ACK lines with the server's lock-step seq).
- `DORADO_JUNK_TRACE=1` logs junk-timer enable/disable transitions.
- `DORADO_PC_COUNT="a,b,..."` (octal, max 8) counts task-0 executions
  of specific real uPCs in the full-boot probe, printed at probe end
  with last-execution cycles. AEmu reference points: DIR=3100,
  EIR=3101, BRI=3102, DIRS=3113, interrupt delivery (PCLOC store)=756,
  AEmuReschedule=314, RestartIFU=307.
- `DORADO_RM_WATCH=<octal>` logs writes to one RM address with cycle,
  uPC, RBase, PCX (cpu.c rm_stk_write).
- `DORADO_RBASE_TRACE=1` + `DORADO_TRACE_GATE` logs task-0 RBase
  transitions per instruction.
- The probe summary prints `Ethernet Stage-2 ... state/seq/max_seq/pos`
  and an `Alto display:` block (DASTART, vmask, WW, ACTIVE, cursor,
  DCB chain walk).
- `/tmp/realmap` pattern: a tiny standalone C tool linking mb.c +
  microcode.c + disasm.c that prints real-address -> symbol for
  AEmu.mb, and a sibling that dumps decoded fields at real addresses.
  CALL `mb_init(&mb)` BEFORE `mb_load` or im_id/dmachine are garbage.

## Hard-won caveats (do not relearn these)

- REF_W (harness trace) misattributes refs in IFUJump-tailed
  instructions as the IFETCH — stores can vanish from REF_W entirely.
  The memory.c-level STORE_VA trace is authoritative.
- The EtherBoot loader executes 0o30 words below its listing addresses
  (the BoL payload lands at VM 1; the listing's load origin is 0o31).
  PC-relative code and literal cells make this self-consistent; when
  matching dispatch traces to the listing, add 0o30.
- IFUDISP br31 = the last IFU rebase target (word address), pcf = byte
  offset from it.
- Alto ether: ePLoc (VM 0o600) is the SHARED post cell for input and
  output. InDone=0o377, OutDone=0o777, SIO-reset abort = CmdAbort XOR
  ECmdBits = 0o2771. boot6 accepts only post == 0o377 exactly.
- AEmu sources are one-line Bravo files: read with
  `perl -pe 's/;/;\n/g; s/\r/\n/g'`. EBoot/EBLoop live in
  AltoEtherEmu.mc; the boot dispatch and ABoot in AEm0.mc; Nova skips
  (NoSkip/DoSkip, PCF <- PCX+4) in Start.mc; the RTClock math in
  Junk.mc ("Junk wakeup interval = 32 us").
- ABoot's 100 ms keyboard-settle wait polls VM 0o430 (RTClock high
  word, ticks every ~39 ms of virtual time) — a slow RTC stretches the
  entire boot timeline, and timed waits in booted software stall.
- Run the repro from `dorado/` — a stray `cd` to the repo root makes
  `./build/test_cpu` silently produce empty pipelines.

## ROUTE B (2026-06-16, session 18): germ VERSION matters -- Dorado.germ!6 (Cedar 6.1) avoids the 0o27132 bug and gets further (155 vs 110 dispatches); new blocker = a LongFetch via LPtr to an unmapped bank (high word 0o6530)

### Germ-version survey (all with CedarDorado.eb!6)
- `Dorado.germ!4` (Dec 1983, our old default): 0o27132 germERROR at 110 dispatches.
- `Dorado.germ!6` (Cedar **6.1**, 1986): does NOT hit 0o27132; runs 155 dispatches; faults on a long-pointer fetch. **Better debug target.**
- `Dorado.germ!6` (Cedar 6.0) / `Dorado10.germ!5` / `Dorado.germ!1`: same germERROR family as !4.
- Germ candidates staged in `chm/cedar/germ-alt/` (germ-plant buffer bumped to 16384 words, committed).

### Germ 6.1 blocker (cyc 67992494, mesa_pc=0x232, in TrapsImpl br31=3E0D58)
A `Fetch` via `LPtr` (BR[0o34]) page-faults: VA `0xD580687` (page idx 0x5806) is VACANT.
The long pointer is built at real pc 0o701/0o702: `BrHi[LPtr]<-0o6530`, `BrLo[LPtr]<-0o615`,
read from a 2-word long pointer in TrapsImpl's global frame (MDS+0o4764 = {low, high}).
- The HIGH word `0o6530` is GENUINE germ-file data (germ-6.1 file MDS+0o4765=0o6530), preserved
  through the germ's relocation BLT (pc 0o2761). The low word is patched 0o611->0o615 at runtime
  (pc 0o343 RET); the high word is NOT touched.
- `0o6530` is ANOMALOUS: every other BR-high load in the germ uses `0o76` (the MDS/code bank).
  `0o6530` is exactly the TrapsImpl CODEBASE offset (g[1]).
- The "code-relative" interpretation (MDS+0o6530+0o615 = VA 0o17407345) is MAPPED and full of
  germ code/data; the "absolute" interpretation (bank 0o6530 = 0xD580687) is vacant.
- BrHi<-A (cpu.c ff fb=2 fc=4, dorado_br_hi_load) loads A[4:15] directly per HM Table 11c -- so
  the emulator builds BR correctly FROM the value it is given; the value 0o6530 itself is the
  question. NOTE: the running microcode is CedarDorado.eb!6, which may DIFFER from Cedar.mb!6
  (the disasm source) -- disassemble the actual .eb image to confirm the RSTR/long-ptr handler.

### Common thread (both germs)
Both faults are inside TrapsImpl (the Mesa runtime trap module) referencing memory NOT mapped/
loaded in our germ-only environment: germ!4's SLink 0o27132 (MDS, past germ) and germ-6.1's
LPtr high 0o6530 (bank, vacant). TrapsImpl uses long pointers / process states that reference VM
regions Pilot would set up. Leading hypotheses: (a) a germ relocation step should rewrite these
high words to the runtime bank (0o76) and our load skips it; (b) an emulator long-pointer/XM
resolution detail; (c) genuine Pilot/OS dependency. The schematic-vs-emulator audit
(docs/schematic-audit.md, generated by a background subagent) may surface an underlying
microengine cause.

### Infrastructure status for Stage-2 (all READY)
- EFTP/Mayday server: PASSES all tests (serves NETEXEC/CedarNetExec byte-exact).
- OS boot files available: CedarNetExec.boot!4, MesaNetExec.boot!1, NEWOS.BOOT!21,
  OthelloDorado.boot!8 (+ ~30 apps).
- Germ has BootChannelEther (g=0o6150) + MiniEthernetDriver. The germ just needs to reach
  ProcessRequests->DoInLoad to send the Mayday request (it faults in heads.Start, upstream).

Repro (germ 6.1): `DORADO_FAULT_TRACE=all ./build/dorado --eb '../chm/dorado/CedarDorado.eb!6'
--germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' --cycles 68500000` -> page fault at cyc
67992494 pc 0o703 va=0xD580687. BR build: `DORADO_BR_TRACE=1 DORADO_TRACE_GATE="67992400,67992500"`.

## ROUTE B (2026-06-16, session 18b): schematic-vs-emulator audit (background subagent) found 2 real microengine bugs -- both fixed

A read-only subagent compared the high-res board drawings (DoradoDocs/doradodrawings, read as images) against the emulator and wrote `docs/schematic-audit.md`. It covered Processor (ProcL/ProcH) thoroughly and Control+Memory at the sheet+manual level; everything else checked was consistent. Two findings, both fixed and gate-clean:

1. **Overflow branch condition (Table 13 cond 7) was unimplemented** -- `eval_branch_condition` had only conds 0-6 (default 0), so Overflow (FF=067) always read false. The flag was already computed/latched; wired cond 7 to `cpu->alu_overflow`. (commit 03df978)
2. **Barrel-shifter masked BEFORE the ALU** instead of in the Pd mux. HM §3.11: the shifter puts the UNMASKED word on A; the LMask/RMask/Md merge is post-ALU, and "on a shift the ALU branch conditions apply to the unmasked ALU output." Fixed: `shifter_output` returns unmasked + hands back mask/fill; masking applied to Pd after the ALU. (commit dd78b63)

Net gate effect: Galaxian 121553 (exact), NETEXEC 1467->1479 (back IN the 1476-1505 band -- the two fixes together are MORE correct than HEAD), 10/10 tests.

NOTE: Neither audit fix unblocked the germs (germ!4 still germERRORs on 0o27132; germ 6.1 still faults at 155 dispatches on the bank-0o6530 long pointer). Both germ blockers remain "germ references memory we don't map/load" (validated NOT microengine bugs). So the remaining work is germ-state/load (relocation of high-bank pointers, or reaching DoInLoad to load the OS), not the microengine.

Highest-value remaining raster-audit targets (subagent's recommendation): IFU IFUM Length/MemB/RBaseB decode (`ifu_decode_lh`), ContA03 JCN fn-decode, MemX VA-adder sheet.

### Session 18 microengine bugs fixed (cumulative)
WF/RF field opcodes (b7e803d), TisId/RisId+IFetch (b7e803d), Q<-B for Pipe sources (b89f3d5), Overflow branch cond (03df978), shifter Pd-mux masking (dd78b63). All HM/schematic-grounded, all gate-green. The germ advanced from faulting at dispatch 113 to running its full module-startup chain.

## ROUTE B (2026-06-16, session 18c): memory-config / version-compatibility investigation

Q: are we running an untested/incompatible memory config or germ+microcode pairing?

FINDINGS:
- **Memory config SIZE is compatible.** Our emulator advertises VirtualBanks=256 (the 64K-map / 64Kx1-chip config; `dorado_memory_config_word`, `DM_MAP_ENTRIES=65536`). The audit (MemX sheet 15) confirmed the map index is config-dependent (14/16/18 bits for 16K/64K/256K chips) and we model the middle (64K) one. germ-6.1's faulting page (idx 0x5806) is WITHIN that addressable map -- it is just VACANT, not out-of-range. So map size is not the incompatibility.
- **Microcode version is NOT the issue for germ-6.1.** Our `CedarDorado.eb!6` is the 17-May-1984 build (checksum 8d9b44d4) -- the SAME file shipped in Cedar 5.3, 6.0, AND 6.1 (verified via cross-reference). Tested germ-6.1 against the 1984, 1986 (`CedarDorado.eb!3`) and 1987 (`!4`) microcode builds (downloaded to `chm/dorado-mc-alt/`): ALL THREE give the identical 155-dispatch fault. So the germ-6.1 blocker is version-independent.
- **germ VERSION matters, though:** germ!4 (Dec 1983) PREDATES the 1984 microcode -> mismatched -> 0o27132 fault. **germ-6.1 (1986) + CedarDorado.eb (1984) is the matched Cedar-6.1 release pair** and is the correct debug target (gets to 155 dispatches).

### germ-6.1 blocker, refined
germ-6.1 hangs (155 dispatches, cyc 67992494 -> 200M, nothing runs after) on a Fetch via LPtr to VA 0xD580687 (bank 0o6530), a VACANT page never mapped by Initial or the germ. The long pointer is `{low=0o611, high=0o6530}` read from TrapsImpl's global frame (MDS+0o4764). TrapsImpl reads code bytes via a `CodeBytesPtr: LONG POINTER TO RawBytes` (`GetCodeBytes[frame][frame.pc]`, to test the trap instruction for zLDIV). The value `{0o611, 0o6530}` is exactly `{TrapsImpl.g[0]=GFI, g[1]=codebase-low}` -- when a code pointer should be the codebase `{g[1]=0o6530, g[2]=0o76}` = VA `0o17407345` (which IS mapped, full of germ code). So the high word is `0o6530` (a codebase-offset value) where the runtime MDS bank `0o76` is expected -- an **OFF-BY-ONE-WORD / wrong-high-word signature** in a code/long pointer. The high word `0o6530` is genuine germ-FILE data (deposited at germ load); the low word is patched 0o611->0o615 at runtime.

Consistent across microcode versions => it is either (a) an emulator bug in how a frame's code-base long pointer is laid out/resolved, or (b) a germ relocation/fixup step (rewriting code-pointer high words to the runtime MDS bank 0o76) that our flat germ load skips. NEXT: test forcing the LPtr high word 0o6530->0o76 (env-gated) -- if germ-6.1 then proceeds, it confirms the relocation/high-word hypothesis and points at the load-time fixup or the frame.code field-offset. (Defer the cpu.c/memory.c edit until the background schematic-audit agent finishes reading those files.)

DEFAULT DEBUG TARGET going forward: germ-6.1 (`chm/cedar/germ-alt/Dorado.germ-6.1.6`) + CedarDorado.eb!6.

## ROUTE B (2026-06-16, session 18d): high-word experiment CONFIRMS the germ-6.1 code-pointer bug (155 -> 5.9M dispatches) but it is not yet a clean boot

EXPERIMENT (env-gated, since reverted): forced the single `BrHi<-A` load of
`0o6530` to the runtime MDS bank `0o76`. Result: germ-6.1 jumps from **155
to ~5.9 MILLION dispatches** -- it runs vastly more real boot code and
reaches a different module (br31 3E1E10). So the wrong-high-word diagnosis
is CONFIRMED: that LPtr must land in mapped MDS, not vacant bank `0o6530`.

BUT it is not a clean fix and not yet a boot:
- The substitution fires EXACTLY ONCE -> a single specific pointer, NOT a
  systematic frame-layout mismatch.
- Forcing only the high word makes the fetch read MDS+`0o615` -- the WRONG
  data (the real target is TrapsImpl code at MDS+`0o6530`+offset). The germ
  proceeds on bad state.
- It STILL germERRORs (MP 810 then 821) after the 5.9M dispatches and never
  reaches DoInLoad (only the Stage-1 ether TX; no germInLoad MP).

ROOT (refined): the long pointer var at MDS+`0o4764` (TrapsImpl's
`CodeBytesPtr`, read by `GetCodeBytes[frame][frame.pc]` to test the trap
instr for zLDIV) holds `{low=0o615, high=0o6530}` and SHOULD hold the
codebase `{low=0o6530, high=0o76}` = VA `0o17406530` (TrapsImpl code,
mapped). BOTH words are wrong: `0o6530` (codebase-offset) sits in the HIGH
slot, and `0o76` (MDS bank) is absent. The high word is STATIC germ-FILE
data (never written at runtime); only the low word is patched 0o611->0o615
(microcode pc 0o343). So this is a malformed code/long pointer the germ
builds/holds -- either a germ-file value that needs a load-time fixup our
flat germ-plant skips, or a `frame.code`/`PrincOpsUtils.Codebase`
resolution our emulator mis-executes for Cedar 6.1 (whose "MDS relief"
moved global frames out of MDS -- a candidate layout difference, though the
single-fire count argues against a systematic layout bug). NEXT: find where
the var is initialized -- trace the store at microcode pc 0o343 (the low
0o615 patch) and what computes it; decode `PrincOpsUtils.Codebase` for
Cedar 6.1; determine whether the high word should be set at runtime (a
dropped store = emulator bug) or fixed at load (relocation). A correct fix
must produce VA MDS+`0o6530`+offset, not MDS+`0o615`.

## ROUTE B (2026-06-17): gated germ Ethernet-inLoad seed proves request selection, next blocker is germ Ethernet driver startup

Added an opt-in shortcut to test the "skip physical volume, ask Ethernet
directly" hypothesis:

```
./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --germ-netboot-bfn 0 \
  --eftp '../chm/bootfiles/CedarNetExec.boot!4' \
  --cycles 90000000 --progress
```

Implementation is deliberately gated: `--germ-netboot-bfn OCTAL` waits until
PilotBoot's real GERMREMAP has copied the request into resident MDS 76, then
rewrites `GermSwap.pRequest` at `0o17401360` from the disk request to:

```
action = inLoad
deviceType = ethernet      # Device.Type.ethernet = 0o5 from PilotBoot.mc
deviceOrdinal = 0
bootFileNumber = BFN       # 0 means the fake server's default --eftp file
net = host = 0
```

Result: the seed fires and the memory dump confirms the request is correct:

```
[machine] germ netboot request seeded @cyc=67984995:
  pRequest=0o17401360 action=inLoad device=ethernet bfn=0o0
M[0o17401360] = 0o000000
M[0o17401361] = 0o000005
M[0o17401363] = 0o000000
```

But it is NOT booting yet. Ethernet stats remain `eftp_r=0`, and no
Stage-2 `Mayday(244)` packet is transmitted. A focused IFU/XFER trace with
the new diagnostic shows the first `pc=0o150` is only a Mesa microcode
dispatch/branch address, not proof of a final hardware wait. More important:
after the seed the germ briefly runs in the module-startup family
(`br31=3E1E10`), then falls back into the known TrapsImpl/codebase region
(`br31=3E0D58`) and loops through the same germ error/trap machinery already
seen in the code-pointer bring-up.

The Ethernet hardware is not being reached. An opt-in dump at the first
post-seed `pc=0o150` confirms:

```
[machine] pRequest @0o17401360:
  000000 000005 000000 000000 000000 000000 000000 000000
[machine] EthernetOne CSB @0o177600:
  000000 000000 000000 000000 000000 000000 000000 000000 ...
```

There are also no `EControl`/`EData` accesses after the seed. The
schematic-derived Ethernet notes (`docs/schematic-audit.md` and
`docs/ethernet-architecture.md`) say the modeled TIOA addresses and EIT/EOT
wakeup bits already match the DskEth sheets; the missing FIFO/CRC/status work
is real, but it is NOT the current blocker because `BootChannelEther.Create`
has not reached `MiniEthernetDefs.ActivateDriver`/`EthernetOneHeadDorado`.

Corrected interpretation: request selection is no longer the blocker, and the
next blocker is not raw Ethernet hardware. It is still the germ-state /
TrapsImpl code-pointer problem from the previous section: the germ reaches
startup/error machinery before it can invoke the Ethernet boot channel. Next
debug target: resolve the malformed code/long pointer at MDS+`0o4764` (or the
missing load-time fixup that should form `{low=0o6530, high=0o76}`) and only
then re-test `--germ-netboot-bfn`.

Logs:
- `/private/tmp/dorado_germ_netboot_bfn0_v2.log`
- `/private/tmp/dorado_germ_netboot_dump.log`
- `/private/tmp/dorado_germ_netboot_window.log`

## Schematic audit -- session-2 sweep (2nd background agent) findings
- **NEW (high conf):** cache replacement is TRUE-LRU but the hardware (MemC
  sheet 04 + HM Sec 5.7) is a 2-pointer Victim/NextVictim pseudo-LRU
  ("not quite LRU"). `memory.c:761`/`:661`; the comment at `:659` ("HM
  doesn't pin down LRU") is wrong. Benign for data correctness; affects
  `B<-Pipe5` victim readback and Flush ordering. Worth fixing for fidelity.
- Minor/benign: Link not smashed with CIA+1 after Write/Read IM / LdTPC and
  RdTPC parks data in Link (`cpu.c:2763+`); StkError doesn't wake fault
  task 15 (`cpu.c:442`, already noted).
- Confirmed consistent: ContA03 JCN fn-decode (Return/RdTPC/LdTPC/RdIM/
  WrIM), per-task Link/TPC save-restore, 28-bit BR/VA adder, fault-info SRN
  latch. Still un-rastered: MemC 15-19, MemX 05-10/12/13/16/17, all MemD
  (ECC/Pipe4), IFU bit-slices+PROM, DskEth/DispM/DispY.
