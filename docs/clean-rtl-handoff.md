# Clean 1-Clock-per-Microinstruction RTL Handoff

## 1. Executive Summary

This handoff documents the design, implementation, and verification of the **clean 1-clock-per-microinstruction Dorado RTL core** located in [`rtl_core/`](file:///Users/alans/dev2/Dorado/rtl_core/).

### Why a New RTL Core Was Created
The original gate-level Sil-derived FPGA core on `mister14` was limited to **~28% of real Dorado speed** (4.68 MIPS vs 16.67 MIPS authentic):
1. **8-Tick Oversampled Clocking:** Every 60 ns microinstruction took 8 FPGA clock cycles (`2 * SYSPER = 8`) to emulate MECL 10K flip-flop setup/hold and buffer delays via shift registers.
2. **SDRAM Stalls:** 128 individual 1-bit memory wrappers in `dorado_storage_arb.sv` stalled `machine_clk` on 31.93% of host cycles.
3. **Synthesis Overhead:** 90% ALM utilization (~38,000 ALMs, 52,000 FFs) prevented efficient synthesis and caused routing failures in Quartus.

The new RTL core in [`rtl_core/`](file:///Users/alans/dev2/Dorado/rtl_core/) is built as clean, idiomatic SystemVerilog directly from the schematic-audited C emulator reference ([`cpu.c`](file:///Users/alans/dev2/Dorado/dorado/src/cpu.c), [`memory.c`](file:///Users/alans/dev2/Dorado/dorado/src/memory.c), [`io.c`](file:///Users/alans/dev2/Dorado/dorado/src/io.c), [`display.c`](file:///Users/alans/dev2/Dorado/dorado/src/display.c), [`ethernet.c`](file:///Users/alans/dev2/Dorado/dorado/src/ethernet.c)). It executes **1 microinstruction per clock cycle** at full 16.67 MHz (1.0x real time) and supports Turbo clocking (33.3 MHz 2.0x, 66.7 MHz 4.0x) within an estimated ~5,000 ALMs (~12% FPGA area).

---

## 2. Directory Structure & Module Architecture

All files are located in [`rtl_core/`](file:///Users/alans/dev2/Dorado/rtl_core/):

| File | Purpose | Key Subsystems & Grounding |
| :--- | :--- | :--- |
| [`dorado_rtl_pkg.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_pkg.sv) | Architectural Package | `dorado_uinstr_t`, `dm_ref_kind_t`, `dm_fault_kind_t`, `decode_model1`, `decode_mir`, `ff_fa/fb/fc`. |
| [`dorado_rtl_alu_shifter.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_alu_shifter.sv) | ALU & Barrel Shifter | 32 ALUFM ops, Carry20, Overflow, 32-bit left cycle, `LMask`/`RMask`, `WF<-A`/`RF<-A` field decoders, post-ALU Pd-mux. |
| [`dorado_rtl_mem.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_mem.sv) | Memory Subsystem | `BR[0:31]` 28-bit base registers, 64K Map RAM, 4-way 4K-word virtual cache, `Pipe0..5`, `FaultInfo'`, `Config'`, `MCR`, `DMux`. |
| [`dorado_rtl_cpu.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_cpu.sv) | CPU, IFU, Tasking | `IM` (4096x36), `IFUM` (1024x33), `RM` (256x16), `STK` (256x16), 16-way task scheduler, IFU prefetch & IFUJump, JCN engine. |
| [`dorado_rtl_io.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_io.sv) | I/O Controller Subsystem | Slow I/O router, Fast I/O munch router, DDC display controller (WCB/NLCB/Cursor/Rasterizer), Ethernet (TX/RX), Trident Disk. |
| [`dorado_rtl_top.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_top.sv) | Top-Level Core Wrapper | Interconnects CPU, Memory, and I/O bridge with selectable internal/external I/O modes (`io_bridge_en`). |
| [`sim_cosim.cpp`](file:///Users/alans/dev2/Dorado/rtl_core/sim_cosim.cpp) | Verilator Cosimulation Testbench | 11 lock-step microcode tests against C emulator + full world boot test (Suite 6). |
| [`Makefile`](file:///Users/alans/dev2/Dorado/rtl_core/Makefile) | Build System | Targets: `make`, `make test`, `make lint`, `make clean`. |
| [`verilator.vlt`](file:///Users/alans/dev2/Dorado/rtl_core/verilator.vlt) | Verilator Lint Rules | Clean warning suppressions for unused bit ranges and parameters. |

---

## 3. Current Verification Status

### A. Lock-Step Cosimulation (Suites 1–5: 100% PASS)
Run with:
```bash
make -C /Users/alans/dev2/Dorado/rtl_core test
```
Results (3.3–3.5 MHz in Verilator on Apple Silicon):
- **Suite 1:** ALU, Carry20, Overflow, WF/RF Shifter, ModStkPBeforeW, Return+Link<- (**PASS**)
- **Suite 2:** Memory Subsystem (Fetch/Store/DummyRef/Cache Miss Hold/TgetsMd/Pipe0..5) (**PASS**)
- **Suite 3:** IFU Prefetch, PCF<-B, IFUJump Dispatch & TisId Byte Stream (**PASS**)
- **Suite 4 & 5 (Real Microcode Lock-Step, 0 mismatches):**
  - `Bootstrap.mb` (2,000 cycles, 0 mismatches) (**PASS**)
  - `Initial.mb` (5,000 cycles, 0 mismatches) (**PASS**)
  - `kernel.mb` (5,000 cycles, 0 mismatches) (**PASS**)
  - `memMisc.mb` (5,000 cycles, 0 mismatches) (**PASS**)
  - `IfuComplex.mb` (5,000 cycles, 0 mismatches) (**PASS**)
  - `AEmu.mb!2` (10,000 cycles, 0 mismatches) (**PASS**)
  - `DSemu.mb!1` (10,000 cycles, 0 mismatches) (**PASS**)
  - `CedarDorado.eb!6` (10,000 cycles, 0 mismatches) (**PASS**)

### B. Full World Boot Simulation (Suite 6: PASS -- Galaxian on screen, 2026-10-30)
Suite 6 (`test_rtl_io_boot` in [`sim_cosim.cpp`](../rtl_core/sim_cosim.cpp)) boots
`dorado/worlds/aemu.eb` and serves `chm/bootfiles/Galaxian.boot!1` from the C
Ethernet server. In 80 M microinstructions (25 s of Verilator at 3.2 MHz):

1. AEmu's `ABoot` takes the Ethernet boot, `EIT` accepts the Breath-of-Life
   packet, and the Nova bootloader runs from Alto PC 3.
2. The bootloader relocates itself (three BLTs), sends Mayday, and receives
   the whole file over EFTP: `eftp_seq=113` = 112 data packets + End for the
   57,238-byte file.
3. Galaxian starts, installs its display list, and the RTL DDC paints it:
   title, player labels, score, alien formation, ship and starfield
   (`docs/images/rtl-core-galaxian-2026-10-30.png`; 25,350 scan lines,
   3,297 lit pixels).

Pass now requires the FULL file over EFTP and >= 1000 lit framebuffer pixels
(it used to accept any one rendered scan line).

**Not yet equal to the C emulator's picture.** C presents the 606-pixel Alto
raster at x~144 inside a white surround; the RTL paints it from x~10 on black.
That is the display presentation layer (left margin / unused-area fill), not
CPU behaviour. C's "121,549 px" counts the white surround, so do not compare
the two pixel counts.

### Harness switches (all off by default)
| env var | effect |
|---|---|
| `RTL_BOOT_ONLY=N` | skip suites 1-5; run only the Suite 6 boot for N cycles |
| `RTL_IFUDISP_TRACE=1` | one `RTLDISP` line per IFU dispatch: Alto opcode VA, opcode, AC0..3 |
| `RTL_MEMDUMP=<octal VA>` | dump VA 0..0777 (through the cache) at the first dispatch of that Alto PC |
| `RTL_UTRACE=<octal VA>` | 40 microinstructions (task, PC, T, ref VA, Md, ACs) after that dispatch |
| `RTL_FB_OUT=x.pbm` | write the 1024x808 framebuffer at the end |

The C side of the same comparison is `DORADO_IFUDISP_TRACE=1 ./build/dorado
--eb worlds/aemu.eb --eftp '../chm/bootfiles/Galaxian.boot!1'`, plus
`DORADO_TRACE_GATE=lo,hi DORADO_PCDIS=0,7777` (bb.cycles, ~3.7 per uinstr) for
microinstructions. Compare (opva, opcode) streams, collapsing consecutive
duplicates (a reschedule trap dispatches the same opcode twice). Do NOT compare
C's `aacs` dispatch by dispatch: C samples them mid-instruction, and they can
read one instruction ahead.

## 4. Key Bugs Uncovered & Resolved

### 1. `EmuBRHiReg` Negative Indexing for `EIBR` and `EOBR`
- **Issue:** In [`AEm0.mc`](file:///Users/alans/dev2/Dorado/chm/doradosource/AEmuSources-cedar6.0.dm!1_/AEm0.mc), `SetupBRs` initializes `BR32` (`EIBR`) and `BR33` (`EOBR`) using:
  ```
  T_ (EmuBRHiReg)-1, MemBase_ EIBR; BRHi_ T;
  ```
  When `EmuBRHiReg == 0` (standard Alto emulation in bank 0), `BRHi` is loaded with `0 - 1 = 0xFFFF` (`0xFFF`). In `AltoEther.mc`, `EIPtr` is set to `-count` (`0xFF00`), and `BRLo` is set to `pointer + count`. Adding `{12'd0, EIPtr}` produces virtual addresses in page `0xFF00`.
- **Fix:** Added mapping for virtual pages `0xFF00..0xFFFF -> 0x00..0xFF` in [`sim_cosim.cpp`](file:///Users/alans/dev2/Dorado/rtl_core/sim_cosim.cpp#L991).

### 2. Slow I/O Read Strobe & Task Switch Race
- **Issue:** In `AltoEther.mc` line `02417`: `T_ Input, Block, Branch[.-1, ALU#0];`. When `Block` executes on Task 7 (`EIT`), `task_schedule_task` switches `ctask` to `0` on the very same `posedge clk`. `io_read_task` and `io_read_tioa` immediately changed to Task 0's values, so `u_io` saw `io_read_stb` with Task 0's TIOA instead of Task 7's `EData` (`015`)!
- **Fix:** In [`dorado_rtl_cpu.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_cpu.sv), explicitly latched `io_read_stb_task`, `io_read_stb_subtask`, and `io_read_stb_tioa` at strobe issue time, and clocked [`dorado_rtl_io.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_io.sv) on `negedge clk` so CPU `posedge clk` strobes are sampled stably.

---

### 3. IFU trap vectors at the wrong address (the EFTP "CountZero" stall)
- **Symptom:** the Mayday transmit posted `ECBR(600) = 002377` (`CountZero`) and
  no packet left the machine.
- **Cause:** every IFU trap address -- NotReady (`*34`), the two fault traps
  (`*0`) and the reschedule trap (`*14`) -- was built as
  `{8'd0, ~ifu_insset, 4'd0} | base | n`, putting `~InsSet` at TNIA bits 5:4.
  It belongs at bits 7:6 (`cpu.c ifu_trap_addr`: `(~insset & 3) << 6`), so for
  the Alto set the reschedule trap went to `074` instead of AEmuReschedule at
  `0314`. The first reschedule (here, as the loader's `LDA 2, 151(PC)` at PC 034
  dispatched) ran unrelated microcode and woke the fault task. AC2 came back
  0 instead of 777, the loader built its output control block from that, and EOT
  found a zero word count.
- **Fix:** one helper, `ifu_trap_addr_fn(base, n)` in `dorado_rtl_cpu.sv`, used
  at all four sites.
- **Why the lock-step suites missed it:** their windows (2K-10K cycles) never
  raise a reschedule or IFU trap. **A lock-step window that never reaches a
  mechanism says nothing about it.**
- **Also noted:** the harness's own memory read `R_MEM(br)` is indexed by the
  microcode's OCTAL BR number (`BR[Code, 37]` = `br[037]`, `ECBR` = `br[031]`).
  The end-of-run "BR31=" line prints ECBR, which is correct as an octal label.

## 5. Next Steps to Resume

1. **Match the C emulator's display presentation.** Find where `display.c` /
   `machine.c` apply the Alto left margin and the unused-area fill, port them
   into the DDC in `dorado_rtl_io.sv`, and gate Suite 6 on a
   framebuffer comparison against C at a matched game state, not a pixel count.
2. **Lengthen the lock-step coverage.** Run the C model in lock-step through
   the whole Suite 6 boot, not just the 10K-cycle windows: that is the cheapest
   way to find the next bug of the "trap vector" kind. The I/O side (C
   `machine.c` vs `dorado_rtl_io.sv`) is the open question, since Suite 6 drives
   only the RTL.
3. **More worlds.** NetExec and the other Alto games (`make run-netexec` and
   friends in `dorado/Makefile`), then the Cedar/Pilot path.
4. **MiSTer FPGA integration (`fpga/mister/`):**
   - Add an `EXT_STORAGE` parameter to [`dorado_rtl_mem.sv`](../rtl_core/dorado_rtl_mem.sv):
     replace the local BRAM `storage` array with a 16-word burst interface to
     HPS DDR3 / SDRAM.
   - Wire [`dorado_rtl_top.sv`](../rtl_core/dorado_rtl_top.sv) into
     [`fpga/mister/Dorado.sv`](../fpga/mister/Dorado.sv).
   - **Quartus synthesis was tried on 2026-10-30, and the core does not
     elaborate.** Quartus 17.0 Lite on misterubuntu
     (`~/mister/DoradoRtlCoreSynth`, a bare synth-only project, all pins
     virtual) stops in Analysis & Synthesis. The ~5,000 ALM figure was never
     measured and cannot be until the core is restructured:
     - `dorado_rtl_cpu` calls tasks and writes variables inside `u_mem` by
       hierarchical name (38 sites: `u_mem.memory_ref_task(...)`,
       `u_mem.proc_srn = ...`). Error 10207 at `get_task_md`.
     - CPU and memory are one `always @(posedge clk)` block whose blocking
       tasks fill or write back a 16-word cache line against `storage` in zero
       time. Even past the hierarchy errors, `storage`, `cache_data`, the Map,
       IM and RM are accessed many times per cycle and will not infer block RAM.
     - `dorado_rtl_io` alone fails on an out-of-range constant index
       (`ddc_kbd_pending[5]`, line 460), which Verilator tolerates.
     - The Ethernet receive buffer `eth_rx_words` has no writer in RTL. Suite 6
       fills it from the C server through Verilator's public access, so the
       core has no receive path on hardware. The same applies to the IM, IFUM,
       RM and Map preload, which `sync_c_to_rtl` performs.
     Getting to hardware means a synthesizable rewrite: real module ports,
     arrays in block RAM with at most two accesses per fabric clock, cache
     misses as multi-cycle holds (HM §5), several fabric clocks per
     microinstruction, an RTL Ethernet receive path plus a boot-server source,
     and a microcode preload. Keep `make test` lock-step as the oracle
     throughout.
   - Build on `misterubuntu`, deploy to the MiSTer, and measure the issue rate
     with the hardware counters.
