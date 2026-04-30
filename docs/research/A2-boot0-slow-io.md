---
gap: A2
status: research (re-diagnosed; original framing was wrong)
---

# A2 — Boot0 stalls at $7744: re-diagnosed

`probe_full_boot` halts at `PC=0o7744`. The original handoff
diagnosis was that Boot0 polls slow-IO TIOAs we don't model
(`Pd←Input` returning 0 → ALUFM corrupted → conditional branches
go wrong). Disassembly + reading the commented source disproves
this.

## Source citations

- **`chm/dorado/expanded/BootstrapSources.dm/BootstrapMain.mc`** —
  the Dorado-side Bootstrap microcode source, fully commented (229
  lines once CR-line-endings are converted). Bootstrap is exactly
  the program the BB jams into IM at $0o7700–$0o7777 (= "Boot0").
- **`bbdis --hunks FE00 16 7700 chm/dorado/doradobaserom.mb!13`**
  — decodes the 16 packed hunks at $FE00 in the BB ROM into 64
  Dorado microinstructions placed at IM[$0o7700–$0o7777].
- **`doradoboot.masm SendIMBlockToDorado` / `SendViaMIR`** — confirms
  Boot0 is delivered "via MIR" with `ViaCP=0`, i.e., one MIR-injected
  Write IM per microinstruction (NOT via the CPReg streaming path
  that Bootstrap later uses for Boot1).

## Spec / what Boot0 actually does

Boot0 = Bootstrap. From `BootstrapMain.mc`:

1. **TaskingOff** at `$7740` (`BootstrapLoc`).
2. **Initialize ALUFM** entries 10, 12, 16 at runtime
   (`A XOR B`, `A+1`, `NOT A`). The BaseBoard pre-loads ALUFM[0]
   with `B` (= `0o25`) — the manual quote from line 22:
   *"Additionally, the BaseBoard writes NOT B before giving control
   to Bootstrap."* Confusing wording: actually the BB loads ALUFM[0]
   with `B`, not "NOT B".
3. **Read 4 CPReg values** (4 ReadBB calls) for the 16-bit start
   address (`Loc`) and 16-bit hunk count (`Cnt`).
4. **Main loop `BootByteL`** — for each half-microinstruction:
   - Read 2 CPReg values via `Call[ReadBB]` (= 2 bytes).
   - Reconstruct the 16-bit IM half from the two bytes via
     `LSH[T, 10] / LDF[T, 10, 0] / XOR`.
   - Decode the 3-bit dispatch in the second byte (positions 10–12)
     via `LDF[T, 3, 10]` and `BigBDispatch_ BTemp`.
   - `Link_ Loc; Branch[Write000]` — the dispatch lands in one of 8
     `Write000`..`Write111` slots that issue the appropriate Write IM
     LH/RH variant.
5. After `Cnt = 0&-1`, **`BranchExternal[InitialLoc]`** transfers
   to the loaded Initial microcode.

## Slow-IO survey of Boot0

I scanned all 64 microinstructions for `Pd←Input` (FF=`0o023`),
`Pd←InputNoPE` (FF=`0o024`), and `Output←B` (FF=`0o026`) — the FF
codes for slow-IO read and write — gated on BSEL ∈ {0..3} and
ASEL ∈ {4..7} (the rules under which FF is interpreted as a Table-11
function).

**Result: zero slow-IO operations in Boot0.** The only `FF=0o023`
match (at `IM[$7763]`) has `BSEL=4` (the `0,,FF` constant form),
which means FF is *not* a function — it's just supplying the byte
constant `0o23` (= `19`) on the B bus. No TIOA reads, no TIOA writes.

The three apparent `FF=0o262` references at `IM[$7771/$7773/$7776]`
are **`Pd←ALUFMRW`** (FA=2 FB=6 FC=2), NOT `TIOA←B[0:7]` (FA=1 FB=5
FC=2 → FF=`0o152`). They're the runtime ALUFM init writes from the
source: `ALUFMRW_ T, ALUF[10], A XOR B;` etc.

The original handoff diagnosis ("Boot0 reads slow-IO inputs to
populate ALUFM[1..15]") was wrong — Boot0 *writes* ALUFM (3 entries)
but never *reads* slow-IO. The "40 NOPs to clear cobwebs" the BB
runs before launching Boot0 (per `doradoboot.masm
PrepareProcessor`) is unrelated to ALUFM contents.

## Cross-check / deltas

- BootstrapMain.mc layout matches the symbol table in
  `chm/dorado/expanded/bootstrap.dm!20_/Bootstrap.mb`. The
  BB-ROM-embedded version (decoded by `bbdis --hunks`) is a
  later build but uses the same control flow.
- The 64-microinstruction IM block is consistent with
  `Boot0HunkCount = 16` from the BB ROM's `BOOTBLOCKTABLE`.

## Implementation impact

- **A2 needs no slow-IO devices.** The B2/H2 work (Hold/TaskSim,
  MCR-as-IO, Reset register) is unrelated to Boot0 stalling.
- **A2 is folded into A1.** The actual cause of `probe_full_boot`
  halting at `$7744` is the same root cause as A1: incorrect
  Bootstrap streaming. Specifically:
  - The trap-reservation slots at `$7744`, `$7754`, `$7760`, `$7764`,
    `$7770`, `$7774` are reached when Bootstrap branches to the
    wrong IM address. From the disassembly these slots have
    `BSEL=Md ASEL=Store←RM/STK FF=000 JCN=000(long)` — an
    unconditional long branch that lands in low IM (the trap
    recovery vectors that real Initial/Bootstrap install).
  - Bootstrap's main loop assembles the IM data word from CPReg
    bytes via shifter ops (`LSH[T,10]/LDF[T,10,0]/XOR`). If the
    CPReg byte sequence is mis-aligned with what Bootstrap expects,
    `T` and `BigBDispatch_BTemp` are wrong, and execution falls
    into a trap reservation.
- **The next concrete diagnostic step**: trace one full
  `BootByteL` iteration cycle-by-cycle in `probe_full_boot`,
  comparing each `T←RWCPReg` value with what `BootstrapMain.mc`
  expects after each `LSH/LDF/XOR`.

## What the source clarifies about A1

`BootstrapMain.mc` lines 56–83 define the canonical CPReg byte
format. The **"4 bytes per microinstruction"** in the comment is
the count from the Dorado side: 2 ReadBB calls × 2 halves = 4
ReadBB results. Each ReadBB returns one **16-bit CPReg word**, but
only one byte's worth of data is meaningful per call:

```
Byte 0:  bit 15 = 0 (sync = Tag), bits 0..7 = LH byte
Byte 1:  bit 15 = 1 (sync flips), bits 5..7 = dispatch, 0..7 = RH byte
                       (skipped: bits 8..12 unused)
```

The 17-byte hunk packing (`doradoboot.masm SendAHunk` comment) is
the **BB's internal ROM layout** — how Boot1Data is laid out in
$C016–$D7FF. `SendAHunk` unpacks it on the fly into the per-half
2-byte CPReg sequence. So our A1 research note's section §1 was
correct about the ROM layout but conflated it with the over-CPReg
stream — the over-CPReg stream is what `BootstrapMain.mc` sees.

## Verification

- `bbdis --hunks FE00 16 7700 …mb!13` produces 64 microinstructions
  matching `BootstrapMain.mc` line-for-line (with re-alignment of
  the placement directives).
- The 4 `Write000`/`Write001`/`Write010`/`Write011` blocks in the
  source (lines 181–192) correspond to the 4 IRTable entries
  `IMLHRSTK.0Is0#`/`IMLHRSTK.0Is1#`/`IMRHBLOCKIs0#`/`IMRHBLOCKIs1#`
  in `doradoboot.masm` — the BB's MIR-injected single-step Write IM
  helpers used during the *Boot0 jam* phase. After Boot0 starts
  free-running, those helpers are no longer used; Boot0's own
  Write000–Write111 do the IM writes from CPReg bytes.

## Follow-on

A2 is closed as a re-diagnosis. The actual fix landing point is
A1 — and the corrected understanding of the byte stream (from
`BootstrapMain.mc`) clarifies what to trace. The
`BootstrapMain.mc` source is an oracle: any divergence between our
microengine's behavior and the source is a real bug we want to
identify.
