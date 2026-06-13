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

  Implemented and WORKING: the `emu_ir_break_at` hook (emu.c f2_load_ir
  sets `alto_leave=1` at the target IR count) so a big slice stops after
  N opcodes; and the emulator-only wakeup. Those are correct.

  THE WALL (injection): salto's emulator BOOT owns memory. After reset
  the boot microcode loads its own Nova program into low memory
  (overwriting any fill) and runs ~15.8k instructions, then WAITS in
  microcode (mpc ~0o612) polling the absent disk -- so it stops fetching
  and never reaches an OS-level steady fetch loop. Concretely: filling
  ALL memory with `JMP LIDLE` before the warmup does NOT take -- the
  warmup still ends at pc=2 ir=1 (the boot's own code), and the
  per-vector settle then does 0 fetches (emulator parked in the disk
  wait). So debug_write_mem injection cannot land a clean single opcode.

  THREE WAYS FORWARD (pick one):
  (a) Boot-bypass: force the emulator microcode to its fetch-loop entry
      directly -- set cpu.task_mpc[task_emu] to the Alto emulator's
      "next instruction" microaddress and cpu.r[rsel_pc] to the opcode
      address, bypassing boot. Needs the emulator START microaddress
      (capture cpu.mpc at f2_load_ir during the boot burst, or find the
      Alto-microcode START label). Risk: forcing mid-pipeline mpc.
  (b) Real disk boot: attach a minimal bootable Alto disk so the boot
      completes to an OS with a steady fetch loop, then inject. Heavier;
      re-enables the disk (and the ksec-starvation, which the wakeup
      mask handles).
  (c) Pivot the ORACLE to a spec-derived Nova reference (the option not
      taken at the build fork). ~150 lines of unambiguous Nova ALC+MRI
      semantics from the published ISA, independent of the Dorado
      microcode; spot-check against the already-validated Dorado ALC
      results. Fastest path to a working Phase-3 sweep; keep salto as a
      later gold-standard cross-check once (a)/(b) is solved.

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
