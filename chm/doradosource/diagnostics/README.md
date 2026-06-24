# Dorado hardware-diagnostic sources

Original PARC microcode **source** (`.mc`) and build/place command files
(`.cm`) for the Dorado hardware diagnostics. These are the ground truth for
what each diagnostic actually checks — far more reliable than working from the
compiled `.mb` alone or from second-hand summaries.

Mirrored from the CHM Xerox PARC archive (verbatim raw bytes; files are
classic-Mac **CR-terminated** Bravo/Tioga text, matching the existing
`chm/doradomicrocode/kernelsources/` convention — convert with `tr '\r' '\n'`
to read). Server: `xeroxparcarchive.computerhistory.org`, volume
`[_CD8_]<DoradoSource>`. Fetched 2026-06-23.

## Layout

| Dir | Source archive (`[_CD8_]<DoradoSource>`) | What it is |
|-----|------------------------------------------|------------|
| `TriconD-Diagnostics/` | `TriconD-Diagnostics.dm!8_` | **TriconD** = the Trident **disk-controller** diagnostic (Roger Bates, 1979; last touched by Frank Vest, Dec 1985). `tricond.mc` is the program; `tricond-defs.mc` the definitions; `*.cm` the assemble/place/save recipes. |
| `memAll-Source/` | `memMiscSource.dm!9_` | The **memAll** memory-diagnostic source tree. Builds into BOTH `memA.mb` and `memMisc.mb` (different `.cm` place files). Subtests: `memMemA.mc` (storage), `memProcA.mc` (processor/shifter), `memMapA.mc` (Map), `memAfio.mc` (fast-IO), `memPipeAndFaultA.mc` (Pipe + faults), `memDesperateA.mc`. `memMisc.mc` is the top-level control file that calls each subtest in order. |
| `diagnosticSubrs/` | `diagnosticsubrs.dm!11_` | Shared subroutines + framework used by the mem* diagnostics: `preamble.mc`/`postamble.mc` (the BEGIN/DONE/ERR pass-fail harness), `memDefs.mc`, `kernelAlu.mc`, and the per-section `memSubrs{A,C,D,S,X,Chaos}.mc` helper libraries. |

The **kernel** diagnostic sources already live at
`chm/doradomicrocode/kernelsources/` (Preamble/Kernel/Kernel1..5/KernelALU/
Postamble). The compiled `.mb` images + Midas run-recipes for all of these are
under `chm/dorado/expanded/<name>.dm!NN_/`.

## How they're run

`dorado/build/rundiag <diag.mb> BEGIN DONE ERR [maxsteps]` loads the compiled
`.mb`, applies the Midas recipe (MCR=1, task 0), jumps to `BEGIN`, and reports
PASS (`DONE`) / FAIL (`ERR`) / HALT / TIMEOUT. See `docs/running-diagnostics.md`.

## Corrected diagnosis note (2026-06-23)

Reading these primary sources corrected an earlier mis-diagnosis that claimed
TriconD tests the "Hold & Task Simulator". It does **not** — TriconD is the disk
controller diagnostic; its first failure (`State.Errs`) is a comparison of the
disk-controller **state muffler** (`KSTATE`) against an expected pattern. The
"control for hold, task simulator" line in `TriconD.midas` is boilerplate copied
from `kernel.midas` (the recipe header says so) and does not reflect the test.

`memA`, by contrast, **does** exercise the §3.12 Hold&TaskSim register
(`SETHOLD`, FF=0o154) and displays a `HOLD` register in its Midas recipe.
`memMisc` shares the same memAll source but its `.mb` hangs inside `aMapTest`
(`memMapA.mc`). See `docs/running-diagnostics.md` for the current per-diagnostic
status grounded in these sources.
