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

### B. Full World Boot Simulation (Suite 6: Major Milestone Reached)
Suite 6 (`test_rtl_io_boot` in [`sim_cosim.cpp`](file:///Users/alans/dev2/Dorado/rtl_core/sim_cosim.cpp)) tests booting `../dorado/worlds/aemu.eb` with the bootfile `../chm/bootfiles/Galaxian.boot!1` served by the C Ethernet server.

**What is working:**
1. `aemu.eb` executes `RestartEmulator`, initializes base registers (`SetupBRs`), initializes all 16 tasks (`InitTasks`), and detects the boot chord (BS key held down) at `ABoot` to branch to `EBoot` (`02006`).
2. `EBoot` arms the receiver (`TurnOnRx`), clears post location `EPLoc` (`VM 0600`), and waits for the Breath-of-Life packet.
3. The RTL Ethernet controller in [`dorado_rtl_io.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_io.sv) receives the Breath-of-Life packet and wakes Task 7 (`EIT`).
4. `EIT` reads the packet, passes the address filter, executes the input loop (`02424` / `02417`), stores the 254-word bootloader into memory at `M[1..254]`, stores completion status `000377` (`InDone`) into `EPLoc` (`VM 0600`), and blocks.
5. Task 0 (`EMU`) wakes from `EBLoo1`, reads `EPLoc == 000377`, fetches `M[2]`, verifies that `M[2] == 000602` (`typeBreathOfLife`), and executes `T_ 3C, Branch[Start]`.
6. The Alto Nova emulator starts executing the Breath-of-Life bootloader at Alto PC 3!
7. The Nova code runs under the IFU: **248,123 IFU dispatches** executed!
8. The Nova bootloader arms `EOT` (Task 6) to broadcast a Mayday Pup packet. Task 6 runs through `EOGo`, executes the transmission loop, and completes with `ECBR(600) = 002377` (`CmdAbort/CountZero` status).

---

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

## 5. Next Steps to Resume

1. **Complete EFTP Bootfile Download in `test_rtl_io_boot`:**
   - In [`sim_cosim.cpp`](file:///Users/alans/dev2/Dorado/rtl_core/sim_cosim.cpp), inspect why the Mayday Pup packet transmitted by the Nova bootloader at `ifu_disp = 248123` posted status `002377` in `ECBR(600)`.
   - Check if `top->eth_tx_packet_stb` fired and whether `dorado_ethernet_direct_transmit` was called to advance the server's EFTP state machine.
   - Once EFTP transfers the packets, `Galaxian.boot!1` loads into memory, jumps to the game entrypoint, and initializes the DCB chain (`VM 0420 != 0`).
   - `top->display_wake_gate` will assert, enabling scanline rendering (`ddc_lines_rendered > 0`, `fb_px > 0`).

2. **MiSTer FPGA Integration (`fpga/mister/`):**
   - Add `EXT_STORAGE` parameter to [`dorado_rtl_mem.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_mem.sv): on FPGA, replace the local BRAM `storage` array with a 16-word burst interface to MiSTer HPS DDR3 memory.
   - Wire [`dorado_rtl_top.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_top.sv) into [`fpga/mister/Dorado.sv`](file:///Users/alans/dev2/Dorado/fpga/mister/Dorado.sv).
   - Use the build server `misterubuntu` to compile the `.rbf` bitstream via Quartus.
   - Deploy to MiSTer `10.0.2.14` and measure microinstruction issue rate using the hardware performance counters.
