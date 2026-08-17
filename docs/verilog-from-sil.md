# Phase 2: what the Sil files give the Verilog, and how to test it

**2026-08-08.** Written while cross-checking the C emulator against the board
netlists (`docs/sil-netlist-crosscheck.md`). That pass answered "does our
model have the right shape". This one records what the same files mean for
the RTL, because the answer turned out to be much more than expected and it
should not have to be rediscovered.

**The headline: we do not have to infer the Dorado's logic from the manual or
the schematic PDFs. We have the gate-level netlist of every board, with pin
directions.**

## What is actually in the archive

`chm/sil/<Board>-Rev-Xx.dm!N_/`, sixteen boards:

| file | what it gives Phase 2 |
|---|---|
| `<Board>-Rev-Xx.wl` | **the wire list — the whole board, net by net** |
| `<Board>-Rev-Xx.lc` | package → part number (the cell library to build) |
| `<Board>-Rev-Xx-C.nl`, `-E.nl` | backplane interface = **the module's port list** |
| `<Board>NN.sil` | the drawing sheets, Sil's binary format (need Sil/ANALYZE) |
| `<Board>-Rev-Xx.bp` | **the same port list, bare pins** (`ALUCarry: E179`) |
| `<Board>-Rev-Xx.ad` | addendum: every sheet's revision and `MARKED BUILT` flag |
| `Build.cm`, `Print<Board>.cm` | PARC's own build scripts |

### The wire list is the design

`ProcH-Rev-Ce.wl` is 118 KB and 3,379 lines. After a header naming every
`.sil` sheet with its revision date and a `MARKED BUILT` flag, it lists every
net:

```
ALUCarry: <367> (381)
    f49.5i {205,370}      f17.15o {217,364}     g17.12i {268,364}     g18.10i {260,388}
    E179 {457,525}
```

Read that as: net `ALUCarry` connects pin 5 of package `f49` **as an input**,
pin 15 of `f17` **as an output**, pins on `g17` and `g18` as inputs, and
backplane connector **E pin 179**. `{x,y}` is the physical position on the
board.

So each net comes with:

- its **name**, and the names are the machine's own (`ALUCarry`, `aluCout`,
  `ALUF.0`, `aluC0`) — the same vocabulary the C emulator and the Hardware
  Manual use;
- every **package and pin** it touches;
- the **direction of each pin** (`o` = driver, `i` = receiver), which is what
  makes the graph directed and therefore synthesisable;
- backplane pins where the net leaves the board.

Paired with `.lc`, which maps every package to a part number
(`f17` → `MC10176/16/E`), that is a **complete structural netlist**. Writing
the RTL becomes: model ~50 ECL part types, then emit one module instance per
package and one wire per net.

### The cell library is small and completely documented

Across all sixteen boards: **5,563 packages, 127 part types.** Strip
terminators (`Term100`, 1,641 of them), spare sockets and passives:

- **3,771 packages, 125 types**
- of which **7 types / 745 packages are memory or PROM** — these become
  inferred RAMs, not gate models
- leaving **3,026 packages of pure logic in 118 types**, and **50 types cover
  90% of them**

The parts are the Motorola MECL 10K family and its Fairchild equivalents —
`MC10176` (hex D flip-flop, 307 packages), `MC10231` (dual master-slave D),
`SE10210`, `MC10158`, `MC10173`, `MC10102`, `MU10164`, `MC10181` (the 4-bit
ALU) — all 1970s parts with published truth tables. The memories:

| part | packages | what |
|---|---|---|
| `F10415A` | 174 | 1K × 1 ECL RAM |
| `MosRam` | 165 | main storage array |
| `MK4096P-6` | 144 | 4K × 1 MOS DRAM |
| `F10145A` | 105 | 16 × 4 ECL register file |
| `F10470` | 72 | 4K × 1 ECL RAM |
| `MB7071H` | 70 | PROM (sequence PROMs, decode tables) |
| `MCM10149` | 15 | 256 × 4 ECL RAM |

Fifty hand-written cell models is a bounded, mechanical job. It is far less
work — and far less guessing — than deriving the same logic from Figure 17
and thirty sheets of scanned schematic.

### The netlists are the module boundaries, already drawn

The `.nl` files are each board's backplane interface, and a board is exactly
the right size for a Verilog module. The port list is already written --
**three times**, in three different files:

| where | form |
|---|---|
| `<Board>.bp` | `ALUCarry: E179` |
| `<Board>.wl` | a bare `E179` among the net's pins (see the example above) |
| `-C.nl` / `-E.nl` | slot-qualified: `StartCycle'a: #s05-C.5` |

Across all sixteen boards they agree on **2,052 of 2,054 pins**; `.bp` and
`.nl` agree exactly, and the five differences against the `.wl` are ground
nets it numbers individually (`GND-26`) where `.bp` collapses them (`GND`).
Three independent statements of the same interface is about as much
confirmation as an archive can give. Measured with `tools/sil_backplane.py`:

| module | backplane pins | notes |
|---|---|---|
| ProcH / ProcL | 175 / 176 | datapath, split high byte / low byte |
| ContA / ContB | 170 / 130 | sequencer, tasking, parity collection |
| IFU | 137 | |
| MemC / MemD / MemX | 168 / 174 / 184 | cache control / data / storage |
| DispY / DispM | 122 / 117 | monochrome / colour |
| DskEth | 119 | both I/O controllers |
| BaseBd | 125 | mostly its own ACP bus; 45 reach the machine |

(An earlier version of this table gave 68 for ProcH. That is what
`sil_netlist_report.py` calls *signals* — bus groups after collapsing bit
runs, so `BMux [0-7,16]` counts once. A port list needs the pins.)

`tools/sil_netlist_report.py` prints them grouped into buses with bit runs,
which is close to a port declaration already.

**The port lists come from `.bp` (done 2026-08-16).** They used to be inferred
from the wire list, by treating a net whose only consumers were `Term100` pins
as leaving the board — wrong in both directions, and wrong at the root, since
`Term100` is a 100-ohm *terminating resistor* network, not a connector. That
inference missed 703 backplane nets (emitted as internal wires, so they could
never have reached another board) and invented 833 ports. All sixteen boards
now emit exactly the ports PARC states, and `tools/sil_backplane.py --ports`
is the gate:

```
TOTAL        1922     1920   1920        2         0
          (stated) (emitted) (agree) (missing) (spurious)
```

The two are DskEth's `GND`/`GND-`, which the wire list numbers individually
(`GND-26`), so there is no net of that name to make a port of.

Direction comes from the wire list, and a third case appears that a
two-direction port list cannot express: **a net the board both drives and
senses is `inout`**. 512 of the 2,052 ports are. Declaring those `output`
would let a board read back only its own contribution to a bus instead of the
bus — see the wired-OR note below.

## What this changes about the plan

`CLAUDE.md` says Phase 2 should map from the C emulator's structure. That is
still true for the *testbench* and for anything behavioural, but for the
datapath itself there is now a better source. Concretely:

1. **Do not hand-write the datapath from the manual.** The manual is the
   reference for *intent*; the wire list is the design. Where they disagree,
   the wire list is what was built — the `.wl` header even records which
   sheets were `MARKED BUILT` at that revision.
2. **A netlist-to-Verilog generator is the high-leverage tool**, not a
   nice-to-have. Input: `.wl` + `.lc` + a cell library. Output: one module
   per board with the `.nl` port list, one instance per package, one wire per
   net. That tool plus 50 cell models is most of the structural RTL.
3. **Revisions are tracked.** `chm/sil/` holds one revision per board, but
   `DoradoDocs/doradodrawings/` has ~50 PDFs covering every revision of every
   board. Tim's point about the Sil files tracking bug fixes matters here:
   pick the revision that matches the microcode we run.

## How to test it — this is the part we are unusually well set up for

The C emulator is not just a reference implementation; it is a **differential
test oracle that already boots five operating systems.** That is a stronger
verification position than most RTL projects start from.

### Tier 1 — cell library, in isolation

Truth tables from the MECL data sheets. Cheap, mechanical, and it is where
sign/polarity errors get caught. Note that ECL is frequently active-low and
the netlist names say so (`Overflow'`, `Cnt=Zero'`, `BSEL'`); a cell model
that quietly normalises polarity will produce a machine that almost works,
which is the worst outcome.

### Tier 2 — per-board, against the C emulator's section boundaries

The C emulator is already split along the same lines the boards are:
`cpu.c` ↔ ProcH/ProcL/ContA/ContB, `memory.c` ↔ MemC/MemD/MemX, `display.c`
↔ DispY/DispM, `disk.c` + `ethernet.c` ↔ DskEth. So each board module can be
driven with the same stimulus as its C counterpart and compared.

The one caveat found during the cross-check: **the C model is not
cycle-accurate everywhere it is functionally correct.** Hold is the clear
case — the hardware has seven independent requesters and we assert Hold from
one source, with the engine's response behind `DORADO_HOLD`. Comparing
per-cycle timing there will diverge legitimately. Compare *architectural
state at instruction boundaries* first, and treat timing as its own tier.

### Tier 3 — run the real microcode, diff against the C emulator

`make test` already runs 11 test binaries against ~25,000 microinstructions
of genuine Xerox microcode. `tools/tracepcdiff` exists. The escalation is:

1. `ftest.MB` — MicroD's own worked example
2. the six PARC hardware diagnostics that `build/rundiag` already passes
   (`docs/running-diagnostics.md`) — these were written to test *the
   hardware*, which is precisely what the RTL is
3. Bootstrap → Initial → LoadRam, the full boot chain
4. a real world: Alto/Galaxian is the deterministic one

The six diagnostics deserve emphasis. `eventCounters`, `TestTW` and the rest
were written by the people who debugged the actual boards, they exercise the
event counters, HOLDSIM/TASKSIM and the tasking hardware, and they already
pass against the C emulator. They are the natural first real workload for the
RTL and they came with the machine.

### Tier 4 — the frame buffer, byte for byte

The project's existing discipline already applies: gate on **byte-identical
framebuffers** (`cmp` two `.pgm` files) across Galaxian, the Cedar desktop
and Lyric — never on pixel counts. An RTL Dorado that produces the same
121,553-pixel Galaxian frame as the C emulator, byte for byte, is correct in
a way no unit test demonstrates.

### Tier 5 — the DMux, if we want the period debugger

`DMuxClk`/`DMuxData` reach ten of the eleven boards in a working machine (all
but DispM). That serial scan chain is how **Midas**, PARC's own debugger,
read machine state. Implementing it in RTL would let Midas — an independent,
period-correct checker nobody on this project wrote — inspect the design.
The C emulator answers exactly two DMux addresses today, so this would need
building on both sides; worth it only if the RTL reaches the stage where an
independent oracle is the bottleneck.

## What is still missing

- **No `.sil` sheet renderer.** The sheets are Sil's binary format. `ANALYZE`
  and Sil survive in `_cd6_/sil` with BCPL source (see
  `docs/parc-feedback-todo.md` F.2) and `DoradoDocs/doradodrawings/` has PDF
  renderings, but nothing here reads `.sil` directly. **This is less
  important than it looks** — the `.wl` is derived *from* the sheets and is
  plain text, so the sheets are for human reading, not for extraction.
- **No timing data.** The netlist gives connectivity, not propagation delay.
  The 60 ns microcycle and the t₀..t₃ phase structure come from the Hardware
  Manual.
- **The five remaining boards** — BaseBd, msa, PCMSA, IOTest, Music — are
  uncrosschecked. BaseBd is the interesting one: we already emulate its 6502
  from the real EPROM dumps in `firmware/`, so its netlist is a direct check
  on `baseboard.c`.
- **The gaps the cross-check found are RTL work items, not emulator bugs**:
  bus and IM parity (`IOPE`/`MdPE`/`RamPE`/`IMLHPE'`, all collected on ContB
  into `Error'`), the seven-requester Hold network, `CountMiss`, DRAM
  refresh, ECC generate/check. The RTL will have them whether we model them
  in C or not, because they are wired.

## Suggested order

1. `.wl` parser + `.lc` reader → an in-memory netlist graph. Small, and it
   makes everything after it mechanical.
2. Cell models for the top ~20 parts, verified against data-sheet truth
   tables.
3. Generate ProcH + ProcL, simulate against `cpu.c`'s ALU/register-file
   behaviour on the microcode we already run.
4. ContA/ContB next — the sequencer and tasking, checked with `TestTW` and
   `eventCounters`, which already pass in C.
5. Memory section, then IFU, then I/O.

Start with the processor boards because they are the ones whose behaviour the
C emulator models most confidently, and because `docs/sil-netlist-crosscheck.md`
already confirmed every field width, register width and branch condition on
them matches.

---

# Step 1 is DONE (2026-08-15): the netlist reader and generator

The `.wl` + `.lc` reader this document called "self-contained, testable on
its own, and makes everything after it mechanical" exists and runs. All
sixteen boards parse, generate Verilog, and **elaborate under Verilator**.

## The tools

| tool | what it does |
|---|---|
| `tools/sil_netlist.py` | reads `.wl` (nets, pins, directions) + `.lc` (parts). `--all chm/sil` summarises every board; `--net NAME` prints one net |
| `tools/sil_ecldict.py` | reads PARC's **own** part dictionary, `EclDict.Analyze` / `TtlDict.Analyze` -- pin numbers, gate grouping, and for complex parts the DATASHEET signal names |
| `tools/sil_emit_cells.py` | emits a Verilog cell skeleton per part type, ports generated from the dictionary + observed directions |
| `tools/sil_to_verilog.py` | emits one module per board, instantiating a cell per package and wiring it with the board's own net names |
| `tools/sil_gen_all.py` | all sixteen boards in one go |

`verilog/Makefile`: `make boards`, `make cells`, `make lint`.

## Measured, and it corroborates this document's own sizing

```
16 boards   12,841 nets   5,563 packages   52,865 pin references
            127 part types;  48 logic types cover 90% of logic packages
            67,960 lines of generated Verilog, 16/16 lint clean
```

The 5,563 packages and 127 part types match the figures derived
independently earlier in this document, which is a useful check that the
reader is seeing the whole design and not a subset.

## The find that removed the guesswork

**PARC's own part dictionary survives**, at
`chm/sil/msa-Rev-Bg.dm!1_/ecldict.analyze` (K. Pier, 12-Sep-1978), with
`ttldict.analyze` beside it -- the ECL file's own header points at the TTL
one ("REMOVE first semicolon of this line if you have TTL logic"). It is
what ANALYZE, Xerox's design-rule checker, used to know what each package
IS, so the cell library's pinouts do not have to be reconstructed:

```
MC10181:  2=H3 3=H2 4=Gg 5=COUT 6=H0 7=H1 8=Pg 9=E0 10=D0 11=E1
          13=F0 14=F3 15=F1 16=D1 17=F2 18=D2 19=E2 20=E3 21=D3 22=CIN 23=M
```

-- the 4-bit ALU with its datasheet pin names. Merging both dictionaries
took the parts with no definition from 57 down to 17, and those remaining
are memories and analog (`MosRam`, `LM3911+20K`, `CA3140`), not logic.

Division of authority, which matters: **pin NAMES come from the dictionary,
pin DIRECTIONS from the wire lists.** The `.wl` marks every pin `i` or `o`
per instance, which is both authoritative and finer-grained than a part-level
rule -- a pin can drive on one board and be sensed on another.

## Three things the generator refuses to do silently

Each would produce plausible-looking, wrong RTL:

- **Wired-OR.** MECL 10K open emitters are legitimately tied together (91
  such nets on ProcH alone). Verilog forbids multiple continuous drivers, so
  those nets are emitted as an explicit OR of their drivers and every one is
  reported, rather than being quietly reduced to one driver.
- **Missing cell models.** A package whose part has no model becomes a named
  stub with its real ports, and is counted. Logic is never dropped.
- **Undriven nets.** A net with no driver on this board is a board INPUT
  arriving over the backplane; it becomes a module port, not a floating wire.

## Two bugs the elaboration caught, both worth knowing

- **Net-name collisions were silently merging signals.** `CTask.0` and
  `CTask=0` are DIFFERENT nets on ContA, and the first name mapping sent both
  to `CTask_0`. Verilog reported a duplicate declaration -- but only by luck:
  had one of them not been declared, the board would have elaborated with two
  signals shorted and no diagnostic at all. The mapping is injective now
  (`_` escaped first so no escape sequence is reachable another way) and
  verified collision-free across all sixteen boards.
- **Cell port directions are global, net classification is per-board.** A
  part pin that drives on MemC and is only sensed on IFU is `output` in the
  shared cell, so IFU would declare that net a module input and then connect
  an output to it (`%Error-ASSIGNIN`). The classifier reads the cell library
  back and treats a net touched by any cell output as internal.

## What is NOT done

**No cell has behaviour yet.** All 125 are skeletons with correct ports and a
`TODO` body, so the boards elaborate but do not compute. Filling them in is
the next work, and the order is given by usage: 48 logic types cover 90% of
logic packages. Each model should cite its part function when it lands.

The harness for running them is `verilog/verilator/` -- Verilator + Dear
ImGui, carried over from the Apple-IIgs MiSTer `vsim` framework with its
`sim/` support library intact (sim_video, sim_bus, sim_input, sim_clock,
sim_console, sim_audio, sim_blkdevice). The top module is MiSTer's `emu`,
deliberately: RTL that runs in the harness runs on MiSTer without a second
port.

---

# The PROMs survive too, as the PROGRAM that generates them

2026-08-15. The 70 `MB7071H` packages were the largest unmodelled part, and
the one that needs *data* rather than a model. That data exists.

`<DoradoSource>DoradoProms.dm!14_` (mirrored into `chm/doradoproms/`) holds
`DoradoProms.bcpl` plus one file per board section -- `ProcProms`,
`MemProms`, `IFUProms`, `DispProms`, `DiskProms`, `EtherProms` -- and a
`.help`. From the help file:

> "DoradoProms is a program for defining, blowing, and listing any and all
> proms used the the DORADO. ... DoradoProms.run is run to produce a
> microbinary format file (called DoradoProms.mb) which contains the actual
> PROM definitions"

and it emits a `Prom.cm` carrying, per PROM, "the 'name' of the prom, and a
label containg board name and location". So we get **contents and placement**,
and in a better form than binaries would be: the source says what each PROM
*means*.

**26 PROMs**, extracted from the sources rather than retyped
(`tools/dorado_proms.py --list`):

| PROM | size | where |
|---|---|---|
| LMASK, RMASK | 32 x 16 | PrH-b07/b08, PrL-b07/b08 |
| ST, EC-1, EC-2, Map-Mem, Map-Map, 4k-Mem, 16k-Mem | 32 x 8/16 | MemX |
| Data-Select, **Mouse-Motion**, **Keyboard-Map** | 32-256 | IFU-a06/i03/k05/l05 |
| DisPromA, DisPromB, LFProm-Low, AltoProm | 256 x 4 | Display |
| DiskWrite, DiskRead, DiskTag, DiskFifo, DiskUnits | 32-256 | Disk |
| EtherFifo x2, EtherPD, EtherRcvr, EtherXmtr | 256 x 4/12 | Ethernet |

Note **Mouse-Motion** and **Keyboard-Map** on the IFU board -- the same
terminal path the A6 serialiser work went through. Those PROMs are the
authority on what the keyboard map actually is, against which
`display.c`'s 61-key matrix can be checked a fourth time.

## The generators port cleanly, and one is already cross-checked

They are small closed-form loops, so porting is transcription, not
reimplementation. `ProcProms.bcpl MakeLMask`:

```
Zero(buff,32); let Lbit = #100000; let Lmask = 0
for address = 1 to 31 do        //address 0 should be zero
    [ Lmask = Lmask % Lbit; Lbit = Lbit rshift 1; buff!address = Lmask ]
```

which yields `0000 8000 C000 E000 F000 ...` -- the shifter's LEFT edge mask.
The C emulator computes the same masks independently, from the Hardware
Manual (`shifter_output()`, ShiftLMask/ShiftRMask). `dorado_proms.py --check`
compares them and they agree, including the edge case the BCPL comment calls
out ("address 0 should be zero").

That is the cross-check worth repeating for each PROM as it is ported: the
PROM is what the hardware HOLDS, the emulator is what we THINK it holds, and
they were written 45 years apart from different sources.

One detail not to mistake for a bug: entries 16..31 saturate at `FFFF`,
because it is a 32-entry part holding a 16-bit mask.


---

# Session close, 2026-08-15

| piece | state |
|---|---|
| Boards generated + elaborating | **16 / 16** (67,960 lines, plus 4,599 of cells) |
| Cell models with behaviour | **68**, covering **93.4%** of 3,771 logic packages (94.7% of the eleven-board machine) |
| 6502 / 6532 | netlist-derived 6502 (Holme, via jotego); MiSTer 7800 RIOT (CC BY-NC, noted) |
| PROMs generated from PARC's BCPL | **26 / 26**, all property-checked |
| Harness | Verilator + Dear ImGui, builds, runs, `--headless` gate |
| Backplane | **stated per board in `.bp`/`.nl`** -- no schematic needed, and no inference either |

**Nothing computes as a machine yet.** Boards elaborate, most cells have
behaviour, but no board is instantiated in `sim.v` and nothing is wired
between boards. That is the next step and it is unblocked.

Continue from **`docs/verilog-handoff.md`**, which is written to be read
cold: Task A is the two remaining Ethernet PROMs (self-contained), Task B is
wiring the machine together and testing it against the C emulator in four
steps that can land separately.

## What made this go quickly, worth repeating

Three of PARC's own artifacts did the work that would otherwise have been
reconstruction:

1. **The `.wl` wire lists** give every net, every pin, and the DIRECTION of
   each pin -- so the RTL is placed and wired from the design input, not from
   a reading of the schematics.
2. **`EclDict.Analyze` / `TtlDict.Analyze`** give pin numbers and, for the
   complex parts, the DATASHEET SIGNAL NAMES. MC10181 came out as the 4-bit
   ALU with `D0-D3 E0-E3 F0-F3 CIN COUT M Gg Pg` without a datasheet in hand.
3. **`DoradoProms.bcpl`** is a PROGRAM that computes the PROM contents, so
   the PROMs arrive documented -- `EtherPD` is a Manchester phase decoder
   with its timer thresholds commented, and the disk PROM is a 32-step
   sequencer with every step explained.

And the discipline that caught the errors: **check transcriptions
structurally, not by eye.** Every PROM generator has a property test, and the
size check alone found two real errors in one batch -- including a bug in the
tool's own `Header()` parser that was silently dropping a PROM from the map.
