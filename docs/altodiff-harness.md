# Alto-opcode differential harness (altodiff)

Goal: run the same Alto (Nova-family) opcode on our Dorado-AEmu emulator
and on the salto reference Alto emulator, and diff per-opcode state
(AC0-3, Cry, PC, memory writes) to localize Dorado opcode-emulation bugs
at the instruction level instead of by 90M-cycle boot forensics. The
suspect classes (per docs/CONTINUE-HERE.md SESSION-10b) are indirect/
indexed addressing, skip edge cases, and the dsz/isz path implicated in
the ctx!1 corruption.

## RESULT (2026-06-12): Dorado single-opcode Nova emulation is CORRECT

The harness now uses a self-contained spec-derived Nova reference
(`dorado/src/altoref.c`, from the Alto Hardware Manual May 1979 Sec 3),
not salto. `altodiff-dorado sweep` diffs the real AEmu microcode against
it over **2848 vectors, 0 mismatches**:
- ALC: 8 functions x 4 shifts x 4 carry modes x no-load x operand
  patterns x carry-in.
- Memory-reference (LDA/STA/ISZ/DSZ) across data values in ALL addressing
  modes: page-zero direct, page-zero indirect, AC2/AC3-indexed (incl.
  sign-extended negative displacement), and indexed-indirect.
The diff is fault-checked (breaking the reference's ADD -> 160 mismatches).

So the ctx1 corruption bug is NOT in the single-opcode emulation of these
classes -- including the handoff's prime suspects (dsz/isz @indirect,
jsr @disp,2 indirect-indexed). Remaining suspects, now narrowed:
- The S-group / augmented instructions (CYCLE, MUL/DIV [unit-tested],
  BitBlt [arithmetic verified correct this session], the BCPL byte/string
  veneers, JSRII) -- not yet swept.
- Skip control-flow (not yet swept; affects PC not data, less likely for
  a corrupted stack pointer).
- Async/tasking interactions (a mid-DivSub task switch clobbering shared
  Q/Cnt; NOT single-opcode-testable) -- the handoff's latest hypothesis.

## Async/tasking hypotheses checked against the Dorado HM (all exonerated)

After the single-opcode space came back clean, the remaining suspects
were async/tasking. Checked against the Dorado Hardware Manual (Sep 1981):
- **Shared Q/Cnt clobbered mid-DivSub**: already exonerated by the handoff
  (DisplayMain.mc / AltoEther.mc have no `Q_`/`Cnt_`; Junk's `Q_` is in an
  emulator-only sub). Cnt is confirmed not task-specific (HM line 1695),
  but no I/O task writes it.
- **Per-task ALU branch conditions / saved carry**: our emulator makes
  Carry'/Overflow/ALU=0/ALU<0 and the saved carry-out per-task
  (cpu.c task_save/task_load: task_alu_carry[16] etc.). VERIFIED CORRECT
  by the HM: "The carry-out (task-specific) changes whenever an arithmetic
  operation is performed" (line 1980) and the branch conditions "test the
  ALU output of the previous instruction executed by the task" (line 2106).
  So a task switch does NOT clobber the emulator's in-flight carry -- the
  multi-precision DivSub/XorSavedCarry path is safe across preemption, as
  on hardware. (The fault microcode's explicit save of ALU conditions,
  line 6021, is persisting them to memory across the trap/restart, not
  evidence of sharing.)

**Multi-cycle MUL/DIV wrappers - verified correct.** test_divmul_sweep_aemu
(tests/test_cpu.c) drives the full S-Group MULx (0o61020) and DIVx
(0o61021) wrappers over a broad operand sweep against a C reference:
6859 MUL + 3249 DIV combos, all pass. So MUL/DIV is clean across the
operand space, not just the handful in test_divx_aemu.

**BLT/BLKS analysis.** The handoff fingered a Zero()/BLKS wiping ctx+0/+1.
Reading the S-Group.mc BLT(0o61005)/BLKS(0o61006) microcode confirms its
own later conclusion: BLKS correctly fills exactly the block it is told to
(first dest = AC1-count+1, count = -AC3); the corruption was the zone
ALLOCATOR returning a block that OVERLAPS the context, then BLKS faithfully
zeroing it. The allocator's arithmetic (ALC compares + carry chain + MUL)
is precisely what the sweeps above verified correct.

Net: the single-opcode emulation, every memory-reference addressing mode,
the per-task state/carry model, and the multi-cycle MUL/DIV wrappers are
all verified correct (sweep + HM + 10108-combo MUL/DIV test). The ctx1
corruption is therefore in territory this harness cannot reach: an
async/interrupt-timing interaction (a multi-cycle op -- BLT/BLKS/BitBlt --
interrupted mid-transfer with a wrong saved-state restart, or the
interrupt-delivery / IFU-reschedule path), not a steady-state computation.
That needs a cycle-accurate in-vivo trace differential, a different
instrument than this single-opcode harness.

## Status (salto oracle - shelved, see below)

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
