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

- **salto oracle: BUILDS + RUNS CONTINUOUSLY, injection WIP.**
  `AltoInfo/salto/src/altodiff_salto.c` -> `AltoInfo/salto/bin/altodiff-salto`.
  Links salto's core, `alto_init("roms")` + the non-SDL device inits,
  `alto_reset()`. KEY FIX FOUND: the emulator task stalls after one fetch
  because an I/O task (task_ksec = task 4, the disk sector task) wakes
  perpetually and starves it. Setting `cpu.task_wakeup = (1 << task_emu)`
  after reset (and NOT calling drive_init/disk_init) makes the emulator
  run continuously -- a 200000-cycle slice now does 15816 fetches (was 1).
  `cpu.task_wakeup = 0` fatals ("no tasks requesting service"); the
  emulator needs its own wakeup bit.

  TWO REMAINING ISSUES for clean per-opcode injection (both vectors still
  print [NO-DISPATCH]):
  1. run_one uses 1-cycle `alto_execute(CPU_MICROCYCLE_TIME)` slices,
     which (unlike the big warmup slice) do not advance emu_ir_count to 2.
     FIX: run one larger slice and break exactly at the target IR count --
     add `int emu_ir_break_at;` and in emu.c f2_load_ir set
     `alto_leave = 1` when `emu_ir_count >= emu_ir_break_at`; re-set
     `cpu.task_wakeup = (1<<task_emu)` before each slice.
  2. The emulator runs real boot/ROM code (warmup ended at pc=2, NOT
     looping JMP 0 at 0), so "plant opcode at word 0 + set PC" does not
     land. Need to inject at a clean inter-opcode boundary: break right
     after an IR load, then OVERWRITE the just-fetched-but-not-executed
     state is wrong (in-flight opcode clobbers). Better: get the emulator
     into a known idle loop (e.g. fill memory with a self-loop branch to
     a fixed address, let it settle there, then plant the test opcode at
     that address and set ACs). Trace emu.c f2_load_ir + the PC (cpu.r[6])
     handling to pick the injection point.

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
