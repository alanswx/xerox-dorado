# Clean Cycle-Accurate Dorado RTL Architecture Plan

## 1. Executive Summary & Root-Cause Diagnosis

The existing gate-level FPGA core on `mister14` (`Dorado_StoragePacking57_v2_20261026.rbf`) boots Cedar to the Viewers desktop using the modified MiSTer `Main` process (for 64 MiB RAM-cached Trident disk tracks and Unix-socket Ethernet transport) and the local `dorado-server` C daemon (for Pup/EFTP/STP network services). However, hardware counters across the pinned 50,000-iteration `Time CedarDhrystone` benchmark show it runs at **4,677,385 microinstructions/s = 28.06% of a real 16.67 MHz (60 ns) Dorado**.

### Why the Gate-Level Core Is Stuck at ~28% Speed

1. **8-Tick Oversampled Clocking (`2 * SYSPER = 8` FPGA Clocks per Microinstruction):**
   - In `verilog/cells/cell_MC10176.v`, `cell_MC10231.v`, and `tools/sil_to_verilog.py`, every MECL 10K flip-flop is modeled by oversampling its clock input (`ck_en = p9 & ~ck_d`) and capturing a pre-edge shadow copy of its data inputs (`d_pre`), while nanosecond ECL buffer delays between `PreClock1'` and `Clock1'` are simulated by inserting discrete `sys_clk` shift-register stages (`__lag1`, `__lag2`).
   - Because each 30 ns half-cycle takes `SYSPER = 4` fabric clocks and each 60 ns microinstruction requires two 30 ns half-cycles, every microinstruction consumes **8 `clk_sys` ticks**.
   - At `clk_sys = 57.142857 MHz`, the zero-stall ceiling is $57.142857 / 8 = 7.143\text{ MIPS}$ (**42.86%** of real time).
   - Every 6-bit ECL flip-flop synthesizes to 13 FPGA flip-flops (`6 q + 6 d_pre + 1 ck_d`), inflating register usage to **~52,000 FFs** and logic utilization to **~88–92% of the Cyclone V's 41,910 ALMs**.

2. **Global Clock Stalls on Fragmented 128-Group SDRAM Backing (`31.93%` of Host Clocks Lost):**
   - The `MSA` main storage board is synthesized as 144 individual 1-bit `MK4096` DRAM chip wrappers grouped into 128 arbitrated storage groups (`dorado_storage_arb.sv`).
   - Whenever an `MSA` access misses the per-group BL8 word cache, `!storage_ready` gates off `machine_clk` for the entire FPGA. Across the `CedarDhrystone` window, **31.93% of all host clock edges** are spent frozen on `!storage_ready`, reducing the 42.86% ceiling to **29.17%** (and **28.06%** after 3.87% architectural `RepeatCur` holds).

3. **Transistor-Level 6502 BaseBoard Overhead:**
   - Synthesizing the 1,725-node relaxation 6502 and five 6532 RIOTs consumes ~4,000 ALUTs and ~6,300 registers, accounting for 90% of Quartus hold-time repair congestion while doing nothing after bootstrap except watchdog ticks.

---

## 2. Clean C-Derived RTL Architecture (`rtl_core/`)

Instead of adding runtime shadow-certificate predictors onto an 8x-oversampled gate netlist, we build a clean, idiomatic, synchronous SystemVerilog implementation in `rtl_core/` directly from the schematic-audited C emulator (`dorado/src/cpu.c` and `dorado/src/memory.c`), while keeping the original Sil gate-level core intact as a separate reference target.

### Architectural Properties of `rtl_core/`

| Property | Gate-Level Sil Core (`verilog/`) | Clean C-Derived RTL Core (`rtl_core/`) |
| :--- | :--- | :--- |
| **Execution Cadence** | 8 `clk_sys` ticks per microinstruction | **1 `clk_sys` tick per microinstruction** |
| **Estimated Area (Cyclone V)** | ~38,000 ALMs (90%), ~52,000 FFs | **~4,500–6,000 ALMs (~12–15%)**, ~3,500 FFs |
| **Available M10K BRAM** | ~218 free (of 553) | **~300+ free** (enables large on-chip L1/L2 cache) |
| **Target Clock (`clk_sys`)** | 57.14 MHz (routing-limited) | **66.67 MHz – 100 MHz** |
| **1.0x Real-Time Mode** | Not reachable at `SYSPER=4` | **Exact 16.667 MIPS** (via 1-in-4 clock enable at 66.67 MHz) |
| **Speed Boost / Turbo Mode** | None | **2.0x (33.3 MIPS), 4.0x (66.7 MIPS), up to 5x–6x** |

---

## 3. Module Hierarchy in `rtl_core/`

1. **`rtl_core/dorado_rtl_pkg.sv`**
   - Architectural constants (`IM_SIZE=4096`, `IFUM_SIZE=1024`, `RM_SIZE=256`, `STK_SIZE=256`, `ALUFM_SIZE=16`).
   - Packed structs for decoded microinstructions (`dorado_uinstr_t`: `rstk`, `aluf`, `bsel`, `lc`, `asel`, `block`, `ff`, `jcn`, `iw0..2`) and memory references (`dm_ref_kind_t`, `dm_fault_kind_t`).
   - Pure helper functions for model-1 `.mb` unshuffling (`decode_model1`), `MIR` 5-byte decoding (`decode_mir`), and `FF` sub-field extraction (`ff_fa`, `ff_fb`, `ff_fc`, `ff_decode_ok`, `ff_full_function_ok`).

2. **`rtl_core/dorado_rtl_alu_shifter.sv`**
   - **16-bit Dorado ALU:** Implements all 32 `ALUFM` operations (HM Table 9: 16 logical + 16 arithmetic with `carry_in`, `XorCarry`, `XorSavedCarry`, `Carry20` nibble-carry injection, and signed `Overflow`).
   - **32-bit Barrel Shifter & Pd Masker:** Implements HM §3.11 32-bit left-cycle on `{SHA, SHB}`, `LMask`/`RMask` generation from either `ShC` or `FF`, unmasked `~lo16` A-bus output for branch-condition accuracy, post-ALU `Pd` mask/fill merge (`0` or `Md`), and `WF<-A` / `RF<-A` Mesa field-descriptor decoding (`ProcL18`).

3. **`rtl_core/dorado_rtl_cpu.sv`**
   - **On-Chip Memories (M10K BRAM):**
     - `IM` (4096 × 36 bits: `iw0`, `iw1`, `iw2`, `present`, `breakpoint`) with runtime `Write IM` / `Read IM` support.
     - `IFUM` (1024 × 33 bits: `ifum_hi`, `ifum_lo`, `present`) with `IFUMLH<-B` / `IFUMRH<-B` and `B<-IFUMLH'` / `B<-IFUMRH'`.
     - `RM` (256 × 16 bits) and `STK` (256 × 16 bits) with single-cycle read (`rm_address`) and `lc_write_address` (`ModStkPBeforeW`, `FA=0 FB=4..5`, `FA=2 FB=2..3` overrides).
     - `ALUFM` (16 × 6 bits).
   - **16-Way Hardware Task Scheduler:**
     - Per-task register arrays (`task_t[16]`, `task_tpc[16]`, `task_link[16]`, `task_membase[16]`, `task_rbase[16]`, `task_tioa[16]`, `task_md[16]`, `task_subtask[16]`, `task_dispatch[16]`, and per-task ALU flags).
     - Priority encoder (`task_bnt`), 2-cycle wakeup pipeline (`wakeup_pipe[0:1]`), `TaskingOff`/`TaskingOn` resume delay, `BLOCK` handling, and `Junk` 32 us timer task (`533` cycles).
   - **JCN Next-Address & Branch Engine:**
     - Local Jump/Call, Global Call, Long Jump/Call, Conditional Jump/Call (`ALU=0`, `ALU<0`, `Carry'`, `Cnt=0&-1`, `R<0`, `R odd`, `IOAtten'`/`Reschedule`, `Overflow'`), `Return` (with `ff_loads_link` override), `RdTPC<-B`, `LdTPC<-B`, `Read IM`, `Write IM`, and `IFUJump`.

4. **`rtl_core/dorado_rtl_mem.sv`**
   - **Base Registers (`BR[0:31]`):** 32 × 28-bit registers with `BrLo<-A`, `BrHi<-A`, `IFetch` low-byte `Id` substitution, and `LongFetch` 28-bit displacement.
   - **Virtual-to-Physical Map:** 64K × 19-bit (`RP[15:0]`, `WP`, `Dirty`, `Ref`) in M10K BRAM / fast backing, with `Map<-`, `RMap<-`, and `ReadMap` (`FA=0 FB=3 FC=1`).
   - **Pipe & Fault Registers:** 16-slot `Pipe` indexed by `ProcSRN` and `ASRN` (`Pipe0..5`, `FaultInfo'`, `Config'`, `MapBufBusy` 9-cycle countdown, `DBuf`, `MCR`, `DMux`).
   - **Unified Storage / Cache Interface:** Single-cycle hit path into BRAM cache + clean burst interface to MiSTer SDRAM / 64-bit HPS DDR3.

5. **`rtl_core/dorado_rtl_top.sv` & Verilator Cosimulation (`rtl_core/sim_cosim.cpp`)**
   - Connects `dorado_rtl_cpu` and `dorado_rtl_mem` into a complete testable unit.
   - `sim_cosim.cpp` links the existing C emulator (`dorado/src/cpu.c`, `dorado/src/memory.c`, `dorado/src/mb.c`, `dorado/src/microcode.c`, `dorado/src/disasm.c`) alongside the Verilated SystemVerilog model (`Vdorado_rtl_top`), loads real Dorado `.mb` files, steps both in lock-step, and verifies cycle-by-cycle register, PC, and memory equivalence.

---

## 4. Real-Time (1.0x) and Speed-Boost (2x–6x) Clocking Design

On MiSTer (`Dorado.sv`), `clk_sys` will run at **66.667 MHz** (or **83.333 MHz**):

1. **1.0x Authentic Speed Mode (16.667 MIPS):**
   - A fractional/modulo-4 cadence generator asserts `uop_step_en` once every 4 `clk_sys` ticks ($66.667\text{ MHz} / 4 = 16.667\text{ MHz} = 60\text{ ns}$).
   - If a memory access misses the on-chip BRAM cache and needs an external DDR3/SDRAM word, the 3-to-4 `clk_sys` tick memory read completes **within the same 4-tick (60 ns) microinstruction window**, or stretches `uop_step_en` only by the architectural cache-miss latency.
2. **2.0x / 4.0x / Max Turbo Mode (OSD Selectable):**
   - Asserts `uop_step_en` every 2 ticks (`33.33 MIPS = 2.0x`) or every tick (`66.67–83.33 MIPS = 4.0x–5.0x`).
   - **Wall-Clock Timer Decoupling:** The 32 us `Junk` task timer (`DORADO_JUNK_TICK_CYCLES`), 60 Hz display vertical blanking wakeup, and Ethernet/Disk service timers remain anchored to real wall-clock time (e.g., triggering every 32 us of real time rather than every 533 turbo microinstructions) when `Real-Time Clocks` is enabled, so cursor blink rates, UI double-clicks, and network timeouts behave naturally while compute and rendering run at 4x+ speed.

---

## 5. Phased Implementation & Verification Roadmap

- **Step 1 (Completed 2026-10-30 — `rtl_core/` Foundation & Verilator Lock-Step Harness):**
  - Created `rtl_core/dorado_rtl_pkg.sv`, `rtl_core/dorado_rtl_alu_shifter.sv`, `rtl_core/dorado_rtl_mem.sv`, `rtl_core/dorado_rtl_cpu.sv`, and `rtl_core/dorado_rtl_top.sv`.
  - Built `rtl_core/sim_cosim.cpp` + `rtl_core/Makefile` with 11 cosimulation suites.
  - Passed all 11 lock-step test suites against `dorado/src/cpu.c` and `dorado/src/memory.c` with **0 mismatches** at **3.37–3.54 MHz** in Verilator (Suites 1–3: ALU/Shifter, Memory, IFU; Suites 4–5: `Bootstrap.mb`, `Initial.mb`, `kernel.mb`, `memMisc.mb`, `IfuComplex.mb`, `AEmu.mb!2`, `DSemu.mb!1`, `CedarDorado.eb!6`).

- **Step 2 (Completed 2026-10-30 — I/O Bridge & Peripherals):**
  - Created `rtl_core/dorado_rtl_io.sv`:
    1. Slow I/O router (`Pd<-Input`, `Output<-B`, `IOAtten'`, `TIOA`).
    2. Fast I/O 16-word munch router (`IOFetch<-`, `IOStore<-`).
    3. Display Controller (DDC: NLCB, Cursor, HRam, WCB line buffers, scanline timer, rasterizer).
    4. Ethernet Controller (TIOA 015 Data, TIOA 016 Control, TX/RX packet buffers, EIT/EOT wakeups).
    5. Trident Disk Controller (TIOA 010..014, Format RAM, FIFO, DMux 02000..02037).
  - Clean linting across all 8 SystemVerilog modules (`make lint`: 0 warnings, 0 errors).

- **Step 3 (In Progress 2026-10-30 — World Boot Bring-up in Verilator):**
  - Integrated `top->io_bridge_en` into `sim_cosim.cpp` (Suite 6 `test_rtl_io_boot` with `aemu.eb` + `Galaxian.boot!1`).
  - **Milestone Reached:** EBoot successfully received, verified, and accepted the Ethernet Breath-of-Life packet! It jumped to location 3 (`Start`), entered the Nova emulator loop, executed **248,123 IFU dispatches**, and initiated the Mayday boot broadcast over `EOT` (Task 6).
  - Key bugs uncovered & resolved:
    - *Base Register Indexing:* When `EmuBRHiReg == 0`, `SetupBRs` initializes `EIBR` (`BR32`) and `EOBR` (`BR33`) with `BRHi = 0xFFF` for negative indexing; added virtual page mapping for `0xFF00..0xFFFF`.
    - *Slow I/O Read Latching:* In `T_ Input, Block`, `ctask` switches to 0 on the same posedge; latched `io_read_stb_task` / `io_read_stb_subtask` / `io_read_stb_tioa` in `dorado_rtl_cpu.sv` and clocked `dorado_rtl_io.sv` on `negedge clk`.

- **Step 4 (Next — Complete EFTP Download, Display Rendering & MiSTer Synthesis):**
  - Complete the EFTP Mayday -> Data handshake in `test_rtl_io_boot` to download `Galaxian.boot!1` into RAM and verify active framebuffer pixel rendering (`ddc_lines > 0`, `fb_px > 0`).
  - Add `EXT_STORAGE` burst interface to `dorado_rtl_mem.sv` for MiSTer HPS DDR3 / SDRAM backing.
  - Wire `dorado_rtl_top.sv` into `fpga/mister/Dorado.sv`, build the `.rbf` bitstream on `misterubuntu`, and deploy to MiSTer `10.0.2.14` with OSD speed switches (1.0x authentic 16.67 MHz, 2.0x, 4.0x Turbo).
