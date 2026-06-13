# Alto-opcode differential harness (altodiff)

Goal: run the same Alto (Nova-family) opcode on our Dorado-AEmu emulator
and on the salto reference Alto emulator, and diff per-opcode state
(AC0-3, Cry, PC, memory writes) to localize Dorado opcode-emulation bugs
at the instruction level instead of by 90M-cycle boot forensics. The
suspect classes (per docs/CONTINUE-HERE.md SESSION-10b) are indirect/
indexed addressing, skip edge cases, and the dsz/isz path implicated in
the ctx!1 corruption.

## Status

- **Dorado side: DONE** (committed). `dorado/src/altodiff_dorado.c` ->
  `build/altodiff-dorado`. Runs one opcode through the real AEmu
  microcode (Initial+kernel+memMisc+IfuComplex+AEmu), primes the IFU via
  AEmu's `Start:` cold-entry, steps until `ifu_dispatch_count==2`. Covers
  ALC (ADD/SUB/AND/MOV) and memory-reference (STA/LDA/ISZ/DSZ), all
  validated against hand-computed Nova results. `altodiff-dorado ifum`
  dumps IFUM; `ALTODIFF_TRACE=1` traces steps.

- **salto oracle: BUILDS + RUNS, injection WIP.**
  `AltoInfo/salto/src/altodiff_salto.c` -> `AltoInfo/salto/bin/altodiff-salto`.
  It links salto's core, calls `alto_init("roms")`, and runs the Alto
  microengine. REMAINING ISSUE: after reset + setting `cpu.r[rsel_pc]`,
  the emulator does not fetch/execute the injected opcode (every vector
  prints `[NO-DISPATCH]`: `emu_ir_count` never reaches 2). This is a
  PC/fetch injection-timing problem (when does the Alto emulator read
  cpu.r[6] for the fetch; soft_reset vs full reset semantics; the
  cleared-memory JMP-0 self-loop at pc=0). Next: trace one fetch in
  salto (emu.c f2_load_ir) and find the exact moment to set PC so the
  first fetch lands on the injected opcode.

## salto build recipe (AltoInfo/ is gitignored - keep this)

salto needs SDL 1.2 for its full build. Installed via
`brew install sdl12-compat` (provides `sdl-config` + `SDL.h`).

Local salto patches (gitignored, re-apply if lost):
1. `include/alto.h`: add `#include <stddef.h>` after `#include "altoint.h"`
   (chargen_t uses size_t; modern clang errors otherwise).
2. `Makefile` line ~30: append
   `-Wno-implicit-int -Wno-implicit-function-declaration -Wno-int-conversion`
   to CFLAGS (K&R-era source vs modern clang).
3. `src/emu.c`: add global `int emu_ir_count = 0;` (after `emu_t emu;`)
   and `emu_ir_count++;` in `f2_load_ir_1` right after `emu.ir = cpu.bus;`
   -- the one-opcode boundary counter.

Then `make` (builds bin/salto + all obj/*.o). The oracle driver
`src/altodiff_salto.c` is compiled and linked against salto's object set
MINUS `salto.o` (it has main), with stubs for the salto.c SDL/UI symbols
the other objects reference (border_printf, border_putch, debug_view,
sdl_*, screenmng_*, fatal). Build commands are in the driver's header.

State map (salto): ACs in `cpu.r[rsel_ac0..3]`, carry `emu.cy` (0/1), PC
`cpu.r[rsel_pc]`, memory via `debug_read_mem`/`debug_write_mem`. Run from
the `salto/` dir so `alto_init("roms")` finds the ROMs.

## Line format (both halves emit this; diff it)

```
OP=<o> in:AC=<o,o,o,o> CRY=<d> -> AC=<o,o,o,o> CRY=<d> PC=<o> WR=<va:val,...>
```

## Phase 3 (after oracle injection works)

Sweep the ALC matrix (8 funcs x shift x carry x skip x operand patterns)
+ memory-ref (page-zero/PC-rel/AC-rel, direct + indirect) on both, diff
to the first divergence. ALC already validates correct on the Dorado
side, so expect divergences in indirect/indexed/skip and carry edges.
