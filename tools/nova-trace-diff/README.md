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

`tracepcdiff.sh [max-opcodes] [boot-file] [dorado-cycles]` runs the same
binary in both emulators and compares the executed Alto opcode stream — both
**PC and the four ACs** — finding the first PC mismatch (control-flow
divergence) or, while PCs still agree, the first AC mismatch (a precise
behavioral fault at a matching instruction). Plumbing:

- **Ours:** `DORADO_IFUDISP_TRACE=1` emits one `IFUDISP …` line per opcode
  dispatch (`cpu.c`). `pcf=` is a byte cursor → word PC = `pcf>>1`; `acs=` is
  `STK[StkP+0..3]` = AC0..3 (Start.mc keeps AC0-3 in Stack[1..4]). The script
  streams stderr through `grep -m MAX` so Dorado is SIGPIPE'd once it has MAX
  opcodes (AEmu's first opcode dispatches ~100M+ cycles in, so the default
  cycle ceiling is high).
- **ContrAlto:** `CA_TRACEPC=<max>` emits `CATRACEPC <seq> <pc-oct> <bus-oct>
  cyc=… acs=r3,r2,r1,r0` (`EmulatorTask.cs`). Field 2 is the word PC; the ACs
  are reversed to AC0..3 for comparison.

The two emulators boot the image differently, so their streams start
misaligned (the "harmless phase slip"). The script **auto-aligns** by sliding
ours 0..127 to best-match CA's PC stream over the first 50 opcodes (override
with `SKIP_OURS=`/`SKIP_CA=`). `AC_PERM` retunes the Stack↔AC mapping or
`AC_PERM=skip` diffs PCs only.

## Usage

    ./tracediff.sh 3016          # diff writes to Alto word 0o3016
    ./tracepcdiff.sh 5000        # diff first 5000 executed opcodes (PC + ACs)
    ./tracepcdiff.sh 5000 ../../chm/bootfiles/Invaders.boot!1   # another world

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
