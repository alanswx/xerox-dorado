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
| `<Board>-Rev-Xx.ad`, `.bp` | addendum and backplane data |
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
the right size for a Verilog module. The port lists are already written:

| module | ports | notes |
|---|---|---|
| ProcH / ProcL | 68 / 71 signals | datapath, split high byte / low byte |
| ContA / ContB | 64 / 44 | sequencer, tasking, parity collection |
| IFU | 47 | |
| MemC / MemD / MemX | 95 / 43 / 129 | cache control / data / storage |
| DispY / DispM | 57 / 51 | monochrome / colour |
| DskEth | 74 | both I/O controllers |

`tools/sil_netlist_report.py` prints them grouped into buses with bit runs,
which is close to a port declaration already.

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
