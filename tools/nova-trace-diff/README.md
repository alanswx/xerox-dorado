# Nova trace-diff harness

Cross-validates our Dorado AEmu Alto-emulation against ContrAlto (the
reference Alto emulator) to find where a stuck Alto program (e.g.
MissileCommand) first diverges. Built for the MC no-render bug — see
`memory/mc-bug-is-emulator-not-ethernet.md` and
`docs/alto-game-compatibility.md`.

## Approach

Full instruction-stream alignment is defeated by interrupt-timing / RTC-clock
desync between the two emulators. Instead we diff the **ordered sequence of
values written to a single Alto memory address**: both emulators run the
identical MC program, so the value-sequence to an address matches until the
bug. Sampling per-address sidesteps global alignment.

- **Ours:** `DORADO_STORE_TRACE_VA="<addr>,<addr>"` logs every store to the
  address with the Alto PC (pcx) and code base (br31). We keep only writes by
  MC's loaded code (br31 != 0; boot-time writes have br31=0).
- **ContrAlto:** env `CA_TRACEW=<octal addr>` (added to
  `ContraltoLib/Memory/Memory.cs`) logs every Emulator-task write as
  `CATRACEW <global-seq> <octal value>`. The global-seq is a monotonic
  emulator-write counter (a timeline, lets us order divergences across
  addresses — lower seq = earlier = closer to the root).

`tracediff.sh <octal-addr>` runs both, aligns the value-sequences, and prints
the first divergent write (ours value + PC vs CA value + global seq).
`novadis.py` is a minimal Nova/Alto disassembler to decode the writer.

`tracepcdiff.sh [max-instructions]` runs the same binary in both emulators and
compares the executed Alto/Nova PC transition stream. Dorado emits IFU byte
opcodes (`DORADO_TRACEPC=<max>`); ContrAlto emits full Nova words
(`CA_TRACEPC=<max>`), so the script compares PCs and prints opcodes only as
context.

## Usage

    ./tracediff.sh 3016          # diff writes to Alto word 0o3016
    ./tracepcdiff.sh 20000       # find first divergent executed PC

Requires: built `dorado/build/dorado`; ContrAlto headless harness at
`AltoInfo/contralto-headless` (gitignored) with the `CA_TRACEW` patch and
`DOTNET_ROLL_FORWARD=Major`; MC boot file in `chm/bootfiles/`.

## Instruction-PC diff result (2026-06-19)

`./tracepcdiff.sh 20000` finds the first PC-transition divergence at the
polling branch around Alto `0o723`:

```text
723: 020655  LDA 0,.-123      ; reads M[0o600]
724: 101015  MOV# 0,0,SNR
725: 000776  JMP .-2
```

ContrAlto stays in the `723,724,725` loop because the load leaves AC0 zero.
Dorado exits through `0o726`, so its AC0 is nonzero at the same branch. This
pins the first control-flow divergence to the `M[0o600]` poll, before the
later no-render cascade.

## Findings so far (2026-06-19)

Consistent signature: **ours writes nonzero where ContrAlto writes 0**
(M[0o1104] ours 521 vs 0; M[0o1100] ours 420 vs 0; M[0o2625] ours 31 vs 0).
Divergences cascade — e.g. the 0o2625 writer is a subroutine at Alto 0o2564
saving its caller's accumulators (`STA 0,.+41` etc.); ours received AC0=31
where CA had 0, so the caller already diverged. Iterate upstream (lowest CA
global-seq) to the root. The "nonzero vs zero" pattern + the separately-found
layout-sensitive uninitialized-read both point at our emulator reading/keeping
state the real Alto leaves zero.

## ContrAlto-side patch (not in this repo; AltoInfo is gitignored)

In `AltoInfo/Contralto2-2.0-Beta/ContraltoLib/Memory/Memory.cs`, in the memory
`Store` path: a global `_tracewSeq` counts Emulator-task writes; when
`CA_TRACEW` matches the address, emit `CATRACEW <seq> <octal value>`.
