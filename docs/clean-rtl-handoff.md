# Clean 1-Clock-per-Microinstruction RTL Handoff

## 1. Executive Summary

This handoff documents the design, implementation, and verification of the **clean 1-clock-per-microinstruction Dorado RTL core** located in [`rtl_core/`](file:///Users/alans/dev2/Dorado/rtl_core/).

### Why a New RTL Core Was Created
The original gate-level Sil-derived FPGA core on `mister14` was limited to **~28% of real Dorado speed** (4.68 MIPS vs 16.67 MIPS authentic):
1. **8-Tick Oversampled Clocking:** Every 60 ns microinstruction took 8 FPGA clock cycles (`2 * SYSPER = 8`) to emulate MECL 10K flip-flop setup/hold and buffer delays via shift registers.
2. **SDRAM Stalls:** 128 individual 1-bit memory wrappers in `dorado_storage_arb.sv` stalled `machine_clk` on 31.93% of host cycles.
3. **Synthesis Overhead:** 90% ALM utilization (~38,000 ALMs, 52,000 FFs) prevented efficient synthesis and caused routing failures in Quartus.

The new RTL core in [`rtl_core/`](file:///Users/alans/dev2/Dorado/rtl_core/) is built as clean SystemVerilog directly from the schematic-audited C emulator reference ([`cpu.c`](file:///Users/alans/dev2/Dorado/dorado/src/cpu.c), [`memory.c`](file:///Users/alans/dev2/Dorado/dorado/src/memory.c), [`display.c`](file:///Users/alans/dev2/Dorado/dorado/src/display.c), [`ethernet.c`](file:///Users/alans/dev2/Dorado/dorado/src/ethernet.c)).

**Correction (2026-10-30).** This section used to say the core runs one microinstruction per clock at 16.67 MHz in ~5,000 ALMs. That was never measured, and the first Quartus run showed the one-clock form could not be built at all (hierarchical task calls into the memory module; cache-line moves of 16 words in zero time against a 64K-word array). The core is now **sequenced**: a microinstruction is a short series of fabric clocks (fetch, decode, execute, optional memory/IFU states, finish, schedule), about **10.6 clocks per microinstruction** on the Galaxian boot. The first fit is **10,624 ALMs (25%)** with Fmax **50.8 MHz**, i.e. about 0.3x a real Dorado at the 40 MHz the MiSTer build uses. See section 6.

---

## 2. Directory Structure & Module Architecture

All files are located in [`rtl_core/`](file:///Users/alans/dev2/Dorado/rtl_core/):

| File | Purpose | Key Subsystems & Grounding |
| :--- | :--- | :--- |
| [`dorado_rtl_pkg.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_pkg.sv) | Architectural Package | `dorado_uinstr_t`, `dm_ref_kind_t`, `dm_fault_kind_t`, FF decoders. |
| [`dorado_rtl_alu_shifter.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_alu_shifter.sv) | ALU & Barrel Shifter | ALUFM ops, Carry20, Overflow, shifter masks, Pd-mux. |
| [`dorado_rtl_cpu.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_cpu.sv) | CPU, IFU, Tasking, **Memory** | Sequencer (`C_*` states), memory engine (`M_*` states, merged from the former `dorado_rtl_mem.sv`), BR/Map/cache/Pipe/MCR (HM section 5). Block RAMs: IM, IFUM, RM, STK, Map, cache data, storage. Loader-assist port (`ap_*`). |
| [`dorado_rtl_io.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_io.sv) | I/O Controllers | Ticked once per microinstruction by `step_done`. DDC display (framebuffer render engine, line buffer + munch-copy engine), terminal, Ethernet (receive buffer with registered read, WaitForBOP skip engine), Trident disk (`ifndef DORADO_NO_DISK`). |
| [`dorado_rtl_bootsrv.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_bootsrv.sv) | In-FPGA boot server | Breath of Life, Mayday -> EFTP of the boot file in ROM, Ack-paced (ethernet.c's lock-step EFTP). |
| [`dorado_rtl_assist.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_assist.sv) | Loader assists | Cold-Alto init at EBoot, keyboard words (BS held until EFTP), display gate from DASTART -- what the Suite 6 harness does by poking arrays. |
| [`dorado_rtl_top.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_top.sv) | Top-Level Core | CPU + I/O + boot server + assist; `bootsrv_en`/`assist_en` select the FPGA configuration. |
| [`sim_cosim.cpp`](file:///Users/alans/dev2/Dorado/rtl_core/sim_cosim.cpp) | Cosimulation harness | Lock-step suites against the C emulator + Suite 6 boot; `RTL_DUMP_INIT`, `RTL_RX_LOG`. Steps the RTL by `step_done`. |
| [`sim_cedar.cpp`](file:///Users/alans/dev2/Dorado/rtl_core/sim_cedar.cpp) | Cedar checkpoint harness | The whole C machine (host services included) beside the RTL from the Cedar desktop checkpoint, 4M words of storage (`make cedar`; section 8.1). |
| [`dorado_rtl_mailbox.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_mailbox.sv) | HPS mailbox | The ARM's way in on the MiSTer (`HPS_MAILBOX`): command and result rings, status, receive fill and transmit slots in DDR3 through the storage engine's aux port (section 8.3). |
| [`hps/`](file:///Users/alans/dev2/Dorado/rtl_core/hps) | dorado-hps | The C host services over the mailbox: `hps_host.cpp` (shared with the rehearsal), `hps_main.cpp` (the ARM program), `build_arm.sh` (static cross build on misterubuntu). |
| [`sim_hps.cpp`](file:///Users/alans/dev2/Dorado/rtl_core/sim_hps.cpp) | MiSTer rehearsal | The Cedar checkpoint core (DDR storage, mailbox, `init_cedar/`) against the DDR model (`sim_ddr_model.h`) with dorado-hps in-process (`make hps-sim`). |
| `init_cedar/` | Cedar checkpoint | The RTL state the Cedar core powers up in, from `sim_cedar.cpp` `CEDAR_DUMP_INIT` (generated; regenerate after any change to the state list). |
| [`cosim_sync.h`](file:///Users/alans/dev2/Dorado/rtl_core/cosim_sync.h) | Shared cosim code | C-to-RTL state copy (`sync_c_to_rtl`), `rtl_step`, `compare_c_rtl`; used by both harnesses. |
| [`sim_fpga.cpp`](file:///Users/alans/dev2/Dorado/rtl_core/sim_fpga.cpp) | FPGA-configuration testbench | No C model, no pokes: power-up images, RTL boot server, RTL assist (`make fpga-sim`). |
| [`tools/make_boot_roms.py`](file:///Users/alans/dev2/Dorado/rtl_core/tools/make_boot_roms.py) | ROM generator | `roms/bol.hex` from ethernet.c's `eth_bol_loader`, `roms/bootfile.hex` from the boot file. |
| [`Makefile`](file:///Users/alans/dev2/Dorado/rtl_core/Makefile) | Build | `make`, `make test`, `make init`, `make fpga-sim`, `make fpga-sim-ddr`, `make cedar`, `make lint`. |
| [`dorado_rtl_storage.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_storage.sv) | Storage (block RAM) | The munch port: one aligned 16-word munch per request, word mask on writes (HM section 5). |
| [`dorado_rtl_storage_ddr.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_storage_ddr.sv) | Storage (DDR3) | The same port on the MiSTer DDRAM interface (`EXT_STORAGE`): single-beat reads (four pipelined) and writes, region cleared after reset. |
| [`../fpga/mister_rtl/`](file:///Users/alans/dev2/Dorado/fpga/mister_rtl/) | MiSTer wrapper | `Dorado.sv` (emu: 50 MHz, 1024x808 scan-out, PS/2 keyboard and mouse, storage in DDR3) and `build.sh` (assembles the project from Template_MiSTer; `CLK_MHZ`). |

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

Keyboard, the first speed pass and storage in DDR3 are done (section 7).
What is next is Cedar (section 8). Smaller items still open:

1. **Match C's display presentation** (left margin, white surround) and gate
   on a framebuffer comparison rather than a pixel count.
2. **Lock-step the whole boot**, not only 10K-cycle windows.
3. **Reset does not restore the preloaded memories** (IM, IFUM, RM, Map take
   their contents only at configuration). Reload the core to restart.
4. **More speed.** 6.1 fabric clocks per microinstruction at 50 MHz is 0.49x
   a real Dorado. What is left per microinstruction (section 7): FETCH, EXEC,
   NEXT and FINISH (4 clocks), memory references (~0.85), the IFUJump
   sequence (~0.64). Overlapping C_FETCH with the previous C_FINISH for
   good, and an opcode-byte buffer in the IFU, are the next two.
5. **Galaxian's S key** reads the high-score file over the network; the
   in-FPGA boot server cannot serve it, so the game waits for ever.

## 6. The FPGA build (2026-10-30: Galaxian on MiSTer14)

![Galaxian on the FPGA](images/rtl-core-mister14-galaxian-2026-10-30.png)

HDMI capture from MiSTer14 (via MiSTerKVMS), about 40 s after loading the
core: AEmu preloaded, Ethernet boot from the in-FPGA server, Galaxian
animating (two captures differ in ~10K pixels).

**How a microinstruction runs.** `C_FETCH` (IM read; waits while `io_busy`,
the boot server's `stall` or the assist's `ap_hold`) -> `C_DEC` (RM/STK read)
-> `C_EXEC` (buses, ALU, FF) -> memory engine if ASEL makes a reference
(`M_ROW` cache row to registers, `M_MAPA`/`M_MAPV` Map reads, `M_BOOK` all the
bookkeeping in one clock, then queued data moves `M_WB`/`M_FILL`/`M_RD`/`M_WR`/
`M_IOC`/`M_IOS`/`M_IOW`) -> `C_PREF` -> `C_NEXTRD` (RM read for the branch
condition) -> `C_NEXT` (next PC; IFUJump runs the byte fetches through the
memory engine in `C_IFU_*`) -> `C_FINISH` (LC writeback, flags) -> `C_SCHED`
(task switch) and `step_done`. The I/O controllers tick once on `step_done`.
None of this changes what the microcode sees: every lock-step suite passes at
0 mismatches with cache data and storage compared too.

**What the FPGA has that the harness used to do.**
- `dorado_rtl_bootsrv`: Breath of Life, Mayday, Ack-paced EFTP of
  `Galaxian.boot!1` from ROM. The packet log (`RTL_RX_LOG`) of the C server
  has exactly 1 BoL + 113 EFTP + 1 dally End, and the RTL server reproduces
  it.
- `dorado_rtl_assist`: cold-Alto init at EBoot, keyboard words with BS held
  until EFTP starts, display gate from DASTART. (The Suite 6 harness read
  DASTART from Map page 0; VA 420 is on page 1. The RTL reads it correctly.)
- Power-up images in `rtl_core/init/` from `make -C rtl_core init`
  (`RTL_DUMP_INIT`): IM, IFUM, RM, ALUFM, Map. Reset PC 01076.

**Reproduce.**
```sh
make -C rtl_core init fpga-sim          # pure-RTL boot: PASS, 113 EFTP, fb_px 3259
# on misterubuntu, from a copy of rtl_core/ and fpga/mister_rtl/:
sh fpga/mister_rtl/build.sh ~/mister/Template_MiSTer ~/mister/DoradoRtlCore
cd ~/mister/DoradoRtlCore && quartus_sh --flow compile Dorado
scp output_files/Dorado.rbf mister14:/media/fat/_Development/DoradoRtlCore_20261030.rbf
ssh mister14 'echo load_core /media/fat/_Development/DoradoRtlCore_20261030.rbf > /dev/MiSTer_cmd'
```
(`misterkvms-cli load-core` connects as root without the MiSTer key here;
the `mister14` SSH alias has it.)

**Fit (Quartus 17.0 Lite, 5CSEBA6U23I7, with the MiSTer framework).**

| | ALMs | RAM blocks | timing |
|---|---|---|---|
| bare core, one-clock form | 55,344 (132%) | -- | did not fit |
| bare core, sequenced + reductions | 10,624 (25%) | 522 | Fmax 50.8 MHz |
| MiSTer build | 17,348 (41%) | 541/553 (98%) | core 40 MHz, slack +4.48 ns |

Lessons from the fit: Quartus silently turns an array into registers when it
does not like the access pattern (RM, STK, the Ethernet buffers, the line
buffer -- 18K ALMs of registers until `ramstyle = "M10K"` was added); a
framebuffer with a read-modify-write port and a scan-out port was built as two
full copies (the fix was one bit per address, write-only from the renderer);
`$countones` is not synthesizable in Quartus 17.

## 7. Keyboard, speed, storage in DDR3 (2026-10-31)

**Keyboard and mouse.** `dorado_rtl_assist` carries live input once the
cold-Alto init has run and EFTP has started, the way `machine.c`'s
`machine_seed_alto_live_io` does (the DDC back-channel that would carry it
is gap E2): keyboard words 177034-177037, UTILIN 177030-177033 (~buttons:
Red/left bit 2, Blue/right bit 1, Yellow/middle bit 0) and, once a mouse
has moved, MOUSELOC 424/425. They are written while the IFU is idle (17% of
microinstructions after boot; the longest busy run is ~10K), every 256
microinstructions and at the first idle moment after a change. `Dorado.sv`
maps `ps2_key` through `verilog/cells/dorado_keyboard.v` and keeps an
absolute mouse position. Galaxian's keys, from a C sweep of every key: **A**
moves the ship, **Return** fires, **1**/**Q** start a game, **S** reads the
high-score file over the network (the in-FPGA server cannot serve it; the
game then waits). On MiSTer14 holding A moves the ship and Return fires.

**Speed: 10.6 -> 6.1 fabric clocks per microinstruction, 40 -> 50 MHz.**
Measured with `SIM_HIST=1 ./obj_fpga/Vdorado_rtl_top` (per-state clocks
per microinstruction after the Galaxian boot):

| change | clocks/step |
|---|---|
| start | 10.61 |
| `io_busy` waited on in C_DEC/C_EXEC, not C_FETCH (the I/O tick overlaps the IM read) | 9.6 |
| C_NEXTRD only for branch conditions 4/5 (R<0, R odd, HM Table 13) and the LoadRam End exit | 8.82 |
| M_ROW folded into M_MAPA; M_BOOK skipped when the victim's Map entry is not needed; hit word read speculatively, M_RD one clock | 8.05 |
| C_SCHED merged into C_FINISH | 7.05 |
| IM read in C_FINISH at the chosen next PC; on a hit C_FETCH reads RM and goes to C_EXEC | 6.06 |

The decoded instruction is registered (`ex_u`) before C_EXEC, which took the
IM read data off the critical path (IM -> decode -> ALUFM -> ALU -> `x_pd`,
14 levels). The MiSTer build meets **50 MHz** (slack +1.69 ns): 8.25M
microinstructions/s, **0.49x a real Dorado** (was 0.22x). Every lock-step
suite still passes at 0 mismatches and Suite 6 is unchanged.

**Storage on a munch port, in DDR3 on the MiSTer.** The CPU now reaches
storage only through one port that moves an aligned 16-word munch (word mask
on writes): cache fill and writeback, fast-I/O munches, and the assist's
single words. `dorado_rtl_storage` serves it from block RAM (the cosim and
the default); with `EXT_STORAGE=1` `dorado_rtl_storage_ddr` serves it from
the MiSTer DDRAM port at byte 0x3000_0000, 16-bit words little-endian, so
the HPS can read and write storage directly (`devmem 0x30000000 64`).
`make -C rtl_core fpga-sim-ddr` runs the FPGA configuration against a model
of that port (random BUSY, 12-clock read latency); its framebuffer is
byte-identical to the block-RAM run. Block RAM falls from 541 to 413 of 553.

**Galaxian runs on MiSTer14 with storage in DDR3** (A moves, Return fires;
~7.4M microinstructions/s, the DDR reads cost a little against block RAM).
Every access is a single beat, as the Sorgelig cores' DDRAM clients do: a
munch read is four pipelined one-beat reads, a write one beat per 64-bit
group that has a word to write; reads carry BE = 0xFF. The sim model refuses
anything else. How that was arrived at is worth knowing:

- The first build used 4-beat bursts. A burst write came out **shifted two
  beats** (beat k at beat k-2, read back with `devmem 0x3001FC28 64`).
- After that, **reads were accepted and never answered** -- three one-beat
  reads taken, then BUSY high for ever, zero beats back -- through several
  rebuilds and core reloads, while single-beat writes still landed. The
  burst had left the HPS SDRAM controller's FPGA port stuck, and **reloading
  a core does not reset it; rebooting the MiSTer does.** After a reboot the
  same core ran. Bursts have not been retried since; if a DDR core ever
  hangs on its first read, reboot before believing anything.
- What found it: a debug overlay (OSD "Debug overlay", off by default) that
  draws `dbg_vec` -- the DDR engine's state, BUSY/RD/WE/DOUT_READY and its
  read-command, read-beat and write-beat counts, the CPU state, PC and cycle
  count -- as bit blocks at x 640-895, y 8-39 of the picture, decoded from
  an HDMI capture. Each hardware question costs a 25-minute build; reading
  the machine's own state off the screen answered three of them at once.

## 8. Cedar: the plan

Cedar on the C emulator leans on host-side services far more than the Alto
world does, and every one of them reads or writes guest memory from the
host:

- the germ/Pilot disk-IOCB bridge (`machine_germ_complete_disk_iocb`: PDI
  pages copied into guest memory), which stays live after boot -- Cedar's
  own Checkpoint writes went through it;
- the Ethernet transmit/receive bridges and the in-process STP/Leaf/Pup
  servers (Cedar 6.1 streams its 34 loadees from the file server on every
  boot);
- a host-injected 60 Hz Pilot timer (`machine_pilot_timer_channel`);
- the Cedar keyboard and display vertical-field naked notify
  (`machine_cedar_io`).

And Cedar's storage is 4M words (8 MB; the desktop checkpoint is 9.5 MB).

So the plan is the same split the old core used, with the RTL doing the
machine and the HPS doing the host:

1. **Storage in DDR3** -- done (section 7). Next: `STORAGE_WORDS` = 4M for
   a Cedar build (the clear is 1M beats, ~40 ms).
2. **A host port.** Every host write in `machine.c` goes through
   `machine_store_va` or its sibling in `memory.c`: translate through the
   Map, write storage, update any resident cache line with the same tag.
   That is exactly the assist port's `A_DO`. Host reads
   (`dorado_visible_word_at_va`) are the coherent read the port also does.
   Widen the assist port (raw-VA write for a vacant Map entry; CPU-state
   peeks such as `pre_pc`) and put a mailbox on it the HPS can drive.
3. **The services on the HPS.** Build the host half of `machine.c` (germ
   bridges, Pilot timer, Cedar I/O, STP server) for ARM against that
   mailbox, with the PDI and the served tree on the SD card.
4. **Verify in Verilator first:** restore the C emulator's Cedar desktop
   checkpoint into both models, run them in lock-step with the C machine's
   host writes replayed into the RTL through the same port (needs a
   one-microinstruction `dorado_machine_step` and a host-write hook in
   `machine.c`), and compare.
5. **Then a checkpoint on the FPGA:** storage image loaded into DDR by the
   HPS, CPU/Map/cache state through the host port, the desktop on HDMI.

### 8.1 Step 4 done: the RTL runs the Cedar desktop in lock-step (2026-10-31)

`make -C rtl_core cedar` (or `obj_cedar/Vdorado_rtl_top STEPS [snap] [pdi]`)
restores `dorado/build/good-packs/cedar-desktop.snap` into a complete C
`dorado_machine`, copies its state into the RTL (`sync_c_to_rtl`, now in
`cosim_sync.h` and shared with `sim_cosim.cpp`), and runs the two one
microinstruction at a time. The RTL is built with 4M words of storage, the
machine's whole storage, and is compared against all of it at the end.

The split is the FPGA one: the RTL is the Dorado and the C machine supplies
everything outside it.

- **Slow I/O.** The harness wraps every device in the C I/O registry and
  records what each answered this microinstruction (Input data and parity,
  IOAtten). It hands those answers to the RTL CPU on its external I/O port
  (`io_bridge_en = 0`, `io_attached = 1`), with `io_has_write` from the
  registry.
- **Host writes into memory** reach the RTL through `dorado_host_write_hook`
  (`memory.h`), which is called by `machine_store_va`,
  `dorado_storage_store_at_va`, `dorado_poke_va` and
  `dorado_memory_host_io_write`. The harness copies the munch's storage and
  its cache row.
- **Host register pokes.** `dorado_machine_step_hook` (`machine.h`) runs just
  before and just after `dorado_cpu_step`. Before the step, any CPU register
  that differs from the RTL can only have been changed by the host since the
  last compare. The harness copies it and counts it.

Both hooks are NULL in every frontend: one test per microinstruction. `make
test` (C) and `make -C rtl_core test` still pass.

Results, all with zero mismatches:

| run | microinstructions | what happened |
|---|---|---|
| idle desktop | 20,000,000 | only the 60 Hz timer and field notify |
| `CEDAR_LS=1`, to cycle 37.0B | 270,702,933 | click the CommandTool, type `List [Cedar]<Cedar6.1>VersionMap>*`, NetDir lookup and STP enumerate over the in-process server (83 trace lines, the same as the C machine alone), listing drawn |

At the end, the second run's screen (`CEDAR_PGM`) is byte-identical to the
C machine run alone (`CEDAR_C_ONLY=1`), and so is storage, all 4M words.
Harness speed is 217K microinstructions/s, at 7.9 fabric clocks each.

**What the host did, which is what the FPGA host port must carry.** Over the
listing run:

- 8,615 words written over 987 steps, all coherent stores at a VA (storage
  plus any cache line holding it).
- 1,985 ORs into RM[0] (NWW) and 987 settings of `reschedule_pending`. This is
  the "naked notify" (`RM[0] |= mask; reschedule_pending = 1`) that every
  host service uses to interrupt Pilot: the 60 Hz timer
  (`machine_pilot_timer_channel`), the Cedar field notify
  (`machine_cedar_io`), and the germ disk and Ethernet bridges (machine.c
  1290, 1368, 2028, 2075).
- 2,989 task wakeups, the Ethernet tasks woken by the C device model.

Nothing else: no STK, Map, BR or other register was touched. So the host port
is four operations, applied between microinstructions:

1. a coherent store at a VA;
2. a coherent read at a VA;
3. a notify (`RM[0] |= mask`, `reschedule_pending = 1`);
4. task wakeups.

Plus the slow-I/O device answers, for whichever devices stay on the HPS. The
assist port's `A_DO` is already (1).

**Cedar's display is not drawn by the Dorado's display microcode** in the C
emulator: `dorado_machine_render_display_list` walks the CSB at VA 420B, its
DCB chain and the long bitmap in VM from the host. Cedar's display tasks
never run here, and `wakeup_pending` was never poked for them. On the FPGA
that means either:

- a scanner that walks the same structures in DDR (the Map translates the
  bitmap's pages; dirty cache lines are the coherence question); or
- getting Cedar's own display tasks running against the RTL's DDC, which is
  the authentic route (dorado/CLAUDE.md "Live display + interrupts for
  Cedar").

Next, in order:

1. The host port in the RTL: the four operations above on a mailbox the HPS
   can drive.
2. The C host services built for ARM against it: `machine.c`'s host half
   (germ bridges, timer, Cedar I/O, Ethernet/STP).
3. The Cedar display.
4. Then the checkpoint on MiSTer.

Gotcha: script the input exactly as `dorado.c` does. That means 2M-cycle
frames, one typing-queue pump per frame, and the click run inline. A first
version pumped every 400K cycles; Cedar never issued the command, and that
looked like an RTL bug. It was not.


### 8.2 Step 1 done: the host port, driven by the C machine (2026-10-31)

The CPU's assist port now takes an operation code, `ap_op`. The codes are
`AP_*` in `dorado_rtl_pkg.sv`, and each one mirrors a C routine. The port is
served between microinstructions, while the hold input keeps the CPU in
`C_FETCH`:

| `ap_op` | operation | C routine |
|---|---|---|
| `AP_ASSIST` (0) | the loader assist's own storage/STK access (section 6) | -- |
| `AP_STORE` (1) | storage at the mapped VA, or `storage[va]` if the Map entry is vacant, plus any resident cache line (`ap_cache=1`) | `machine_store_va`, `dorado_poke_va` |
| `AP_STORE_INV` (2) | storage at the mapped VA; the resident line is dropped (valid and vacant cleared) | `dorado_storage_store_at_va` |
| `AP_RM_OR` (4) | `RM[va[7:0]] |= wd`: the RM is read in `C_FETCH` and written in `A_WAIT`, and `ap_rd` returns the old value | the naked notify's NWW half |
| `AP_WAKEUP_OR` (5) | `wakeup_pending |= wd` | `dorado_cpu_wakeup` |
| `AP_RESCHED` (6) | `reschedule_pending = wd[7:0]` | the naked notify's other half |

A read is `AP_ASSIST` with `ap_we=0, ap_cache=1`: the resident line if there
is one, otherwise storage. `dorado_memory_host_io_write`, the Lisp
device-window fill, has no port operation. Cedar never uses it.

`dorado_rtl_top` brings the port out as `hp_hold/hp_req/hp_op/hp_we/hp_cache/
hp_va/hp_wd -> hp_done/hp_rd`, plus `hp_between`. When the loader assist is
enabled (`assist_en && io_bridge_en`, the Galaxian configuration), the assist
owns the port. The MiSTer wrapper ties `hp_*` off for now.

**Verification.** `sim_cedar.cpp` drives every host change through `hp_*`;
`CEDAR_BACKDOOR=1` restores the section 8.1 array copies. A host write is
classified by what the C machine did to the cache. A line that was resident
in the RTL but is gone from the C cache means `AP_STORE_INV`; anything else
means `AP_STORE`. Each VA is written once, with the final value. The harness
counts any change the port cannot express (a bit cleared in RM or in
`wakeup_pending`, a line the C filled, STK/`ready`/junk) and backdoor-copies
it.

`CEDAR_LS=1` to cycle 37.0B gave 270,702,933 microinstructions with zero
mismatches. The screen is the same 168,631 px as before, and all 4M words of
storage match the C machine. The port carried 14,576 operations:

- 8,615 `AP_STORE` and 0 `AP_STORE_INV`;
- 1,985 `AP_RM_OR`;
- 2,989 `AP_WAKEUP_OR`;
- 987 `AP_RESCHED`.

Nothing was inexpressible. They cost 215,691 fabric clocks in all (about 15
per operation, 0.01% of the run). `make -C rtl_core test` and `fpga-sim` still
pass.

Bug on the way: the RM read address for `AP_RM_OR` was first gated on
`ap_done_q`, which the combinational read logic sees one clock late. So
back-to-back operations read the wrong register.

**Next: the transport.** The four operations above are all the host needs
for memory. The open question is how the HPS drives them, and where the
slow-I/O answers come from:

- **Ethernet registers.** These answer in the microinstruction that reads
  them, so they cannot cross the HPS link. On the FPGA they come from
  `dorado_rtl_io`'s Ethernet controller (`io_bridge_en=1`), which the
  Galaxian boot server already feeds. The STP/Pup server then runs on the
  HPS and exchanges packets with it, not register answers.
- **Wakeups.** `AP_WAKEUP_OR` was the C device model waking the Ethernet
  tasks. With the RTL controller, those wakeups are its own.
- **Host-port traffic.** It is tiny: about 15K operations per 4.5 s of guest
  time. It fits a command ring in DDR3 that the FPGA polls, or the
  framework's UIO channel. Germ disk IOCBs and the Cedar field notify need
  no microinstruction-level latency.

### 8.3 The MiSTer arrangement: split, mailbox, checkpoint core, dorado-hps (2026-10-31)

The user chose the route: run the Cedar host services on the ARM, boot the
checkpoint on the MiSTer, and do the screen properly afterwards (Cedar's own
display tasks, not a host-side scanner). Four pieces, each run in simulation
first.

**1. Split mode (`sim_cedar.cpp`, `CEDAR_SPLIT=1`).** The RTL is the whole
Dorado here, with its own slow-I/O devices (`io_bridge_en=1`). The C machine
never steps its CPU: its step hook returns `DORADO_STEP_HOOK_SKIP`
(machine.h), which accounts the step's time through `dorado_cpu_skip_step`.
That routine is the BaseBoard timekeeping factored out of `dorado_cpu_step`,
idle-6502 suppression included. So `dorado_machine_run_until` runs only the
host services and the Pup/STP server. They reach the RTL through two paths
only:

- **The host port.** Host reads go through `dorado_host_read_hook`
  (memory.h), which answers `dorado_visible_word_at_va`. Host writes go
  through `dorado_host_write_hook`, which now carries the value and the kind
  of store (`DORADO_HOST_WRITE_UPDATE/INVALIDATE/FILL`), because the C side's
  memory is stale in this mode. Changes to RM and `reschedule_pending` go as
  diffs.
- **The Ethernet receive buffer** (`hx_rx_*` on `dorado_rtl_top`). Each
  packet the server queues is copied in. `dorado_ethernet_rx_generation`
  (ethernet.c, file-scope) tells the harness a new packet was queued.

The slow-I/O census (sim_cedar now prints one) shows that the **Ethernet is
the only slow-I/O device the Cedar desktop touches**: 1,486 data reads by
task 7, IOAtten tests on 015 and 016, and 24 control writes. So the
checkpoint needs only the Ethernet device's state beyond the CPU's.

Bug found by split mode: when the guest reads a packet's last word, the C
model sets a 60,000-tick hold, so the input task's WaitForBOP tail runs
before the next BSP segment appears. That lived in `eth_read`, which never
runs when the RTL is the receiver. It is now `dorado_ethernet_rx_consumed()`.
`eth_read` calls it, and so do both bridges when the RTL reports the packet
drained. Without it, Cedar received the first NetDir reply and stalled.

**2. The DDR mailbox (`dorado_rtl_mailbox.sv`).** It sits on a new
single-beat aux port of `dorado_rtl_storage_ddr`; storage requests go first.
It is enabled with `HPS_MAILBOX=1` on `dorado_rtl_top`. Then the DDR clear at
reset is skipped (`CLEAR=0`), and the core is held until the ARM writes
`"DORA"` to the control beat. Layout (from byte 0x3080_0000, just past 8 MB
of storage):

- control;
- status: microinstructions, keyboard words, mouse, receiver state, and the
  receive-load count that makes `rx_drained` trustworthy;
- a 256-entry command ring with per-lap sequence bytes (the ARM writes the
  half holding the seq last);
- a result ring;
- the receive words and attention bitmap.

Op 7 (RXLOAD) copies a packet into the receive buffer.

**3. The checkpoint core.** `CEDAR_SPLIT=1 CEDAR_DUMP_INIT=../rtl_core/init_cedar
obj_cedar/Vdorado_rtl_top 0` writes the restored state:

- `$readmemh` images for IM, IFUM, ALUFM, RM, STK, the Map, cache data and the
  receive buffer;
- `checkpoint_cpu.svh` (2,375 register assignments, generated from
  `sync_c_to_rtl`'s own field list);
- `checkpoint_io.svh` (the Ethernet device).

`reset_memory` and the I/O reset include these files under
`DORADO_CHECKPOINT`, so a reset restores the checkpoint. The ARM loads
storage itself, from the same snapshot.

**4. dorado-hps (`rtl_core/hps/`).** `hps_host.cpp` is split mode with the
mailbox as transport. It paces itself from the FPGA's microinstruction count
(3.70 BaseBoard cycles each), in steps of `-q` microinstructions, with the
germ poll every `-p` BaseBoard cycles; a read is a round trip there.
`hps_main.cpp` maps DDR through `/dev/mem`. `hps/build_arm.sh` cross-builds a
static binary on misterubuntu. The same host code runs in the Verilator
rehearsal, `make -C rtl_core hps-sim` (`sim_hps.cpp`, through the DDR model
now in `sim_ddr_model.h`).

The mailbox also forwards packets out. Cedar's own Ethernet output
microcode transmits through the RTL controller once the germ transmit bridge
polls slowly, as the ARM's does. On each transmit strobe the mailbox holds
the CPU, copies the packet into one of 8 DDR slots (`B_TX`, lengths at
`B_TXLEN`), and bumps the status block's tx count; the ARM feeds the slots to
the server in order. The mailbox clears the control beat after reset,
because DDR keeps its contents across a core reload and a stale go started a
reloaded core on the previous run's storage.

**The 64-bit/32-bit snapshot question.** The C emulator's snapshots are raw
structs, so the 64-bit native `cedar-desktop.snap` does not restore on the
32-bit ARM. The browser build's `web-assets/cedar-desktop.snap` (wasm32)
does: ILP32 with 8-byte-aligned 64-bit fields is the same layout as ARM
EABI. It is the same machine. Storage hashes identically (FNV-1a
`8d6a253a2a84bee1`, printed by dorado-hps at start) at the same CPU cycle,
9,726,617,172. So the FPGA image comes from the native snapshot, and the ARM
restores the wasm32 one.

**Host-only mode** (`dorado_machine_host_only`). The C loop's Alto
display-active probe walks a DCB chain every scan line, to wake display
tasks on a CPU that is not running. Through the mailbox it was 215,000 reads
a second and held the FPGA to 0.30x; skipping it gives about 5,000 reads a
second and 0.38x.

**Results.**

| run | result |
|---|---|
| split mode, `CEDAR_LS=1` to 37.0B | 270.7M microinstructions, 23 packets in, 83 FTP trace lines (same as the C machine), 168,631 px on screen (same) |
| `make hps-sim` rehearsal, `CEDAR_LS=1`, quantum 16, germ poll 4096 | 275M microinstructions, 29 packets in through the mailbox, 18 out through the RTL transmitter, listing on screen, 169,142 px (the native emulator's figure) |
| **MiSTer14**, `dorado-hps -c 700,733 -t 'List [Cedar]<Cedar6.1>VersionMap>*\n'` | **the listing on screen** at 6.37M microinstructions/s = **0.38x a Dorado**: 33 packets in, 23 out, `docs/images/rtl-core-mister14-cedar-ls-2026-10-31.png` |

Cedar core fit: 19,559 ALMs (47%), 356 RAM blocks (64%), +1.39 ns at 50 MHz.

The screen is not yet on the MiSTer's video output. dorado-hps renders it
from the DCB chain to `/tmp/dorado.pgm`, as the C emulator does. **Next is
the screen, the authentic way**: Cedar's own display microcode tasks driving
the RTL's DDC, so the core's video shows it (dorado/CLAUDE.md "Live display +
interrupts for Cedar"). After that: speed (0.38x; the mailbox holds the CPU
for each command), and live keyboard and mouse from the core
(`dorado-hps` without `-c/-t` takes them from the status block, untested on
hardware).

MiSTer build: `CORE=cedar sh fpga/mister_rtl/build.sh ~/mister/Template_MiSTer
~/mister/DoradoRtlCedar`, then Quartus **Lite**,
`~/intelFPGA_lite/17.0/quartus/bin` (the `/opt/altera/17.0` install on
misterubuntu is an expired evaluation). On MiSTer14 the runtime files live
under `/media/fat/dorado/` in the repo's layout. The STP tree ships as
`stp-root.tar.gz` and is unpacked into `/tmp/dorado` (tmpfs): its 4,948 small
files would take about 600 MB of the SD card's 128 KB FAT clusters.

```sh
ssh mister14 'echo load_core /media/fat/_Development/DoradoRtlCedar.rbf > /dev/MiSTer_cmd'
ssh mister14 'mkdir -p /tmp/dorado && tar -C /tmp/dorado -xzf /media/fat/dorado/stp-root.tar.gz;
              /media/fat/dorado/dorado-hps -f /tmp/dorado/stp-root'
# /media/fat/dorado/dorado/build/good-packs/cedar-desktop.snap must be the
# wasm32 copy (web-assets/), the .pdi the native one
# screen every 10 s in /tmp/dorado.pgm (and on SIGUSR1)
```

### 8.4 The screen and the keyboard, Cedar's own way (2026-10-31)

On MiSTer14 Cedar now draws its own screen on HDMI and takes the MiSTer's
keyboard and mouse, both through Cedar's own display microcode on the RTL.
The host renders and pokes nothing.
`docs/images/rtl-core-mister14-cedar-hdmi-typing-2026-10-31.png` is an HDMI
capture after typing at the CommandTool (`echo Hello from The MisTer`,
answered) through MiSTerKVMS.

**The screen.** In the C emulator Cedar's display tasks never ran, because
the wake gate never opened for a germ world and the C machine drew the DCB
chain itself. Forcing the wakeups in C (`DORADO_FORCE_DISPLAY_WAKE`) showed
Cedar's display microcode drawing the whole desktop through the DDC model,
162,058 px. So the RTL Cedar core sets `display_wake_gate`. The checkpoint
now also carries the C display controller's state (HRam, NLCB, terminal task,
raster control, line sequencer: `sync_display` in sim_cedar.cpp, 603
assignments in `checkpoint_io.svh`). Without it nothing woke. One offset: the
NLCB line count includes the top border, and Cedar's first visible line
lands on frame line 22, measured as the exact row match against the host
render over 600 rows. `display_top_lines` (22 for Cedar, 0 for the Alto)
drops those lines.

**The keyboard and mouse.** Cedar's `ReadTerminal` (DisplayAux.mc) reads
the terminal's serial bit from **DispY Status (TIOA 370B) bit 0**, not
TStatus (360B): "Input from Status register returns: Bit 0 = keyboard data
bit ... Bit 15 = muffler data bit". Both models returned only the muffler
there, so no keyboard message ever arrived. The RTL now serves the
serializer on that register for the terminal task (DHT/AHT). The C model
serves it only on request (`dorado_display_terminal_on_status` /
`DORADO_TERMINAL_ON_STATUS=1`), because every C world's display task reads
that register. The Alto ones take their keyboard from the host, and the
serializer's in-flight message lives in statics a snapshot does not carry,
so `test_snapshot` diverged. In the Alto RTL core the terminal now delivers
the same words the loader assist writes, and Galaxian still draws 3,259 px.

Three more pieces of the same path:

- The terminal reports changed words **in the order they changed**
  (`display_kbd_order` / `ddc_kbd_order`, in both models). Lowest-word-first
  could deliver a letter before its Shift.
- Terminal word 4 (mouse buttons) carries `~mouse_buttons` in the RTL, as
  `dorado_machine_set_mouse` does in C.
- Mouse motion is message 06B, signed deltas. The wrapper sends PS/2 motion
  as `mouse_dx/dy/mouse_delta_stb` (Cedar build only; the Alto uses the
  absolute position). `dorado_rtl_top` sums it until the I/O tick takes it,
  because a one-clock report is otherwise missed. Cedar's pointer starts at
  (0,0).

`dorado_machine_display_microcode` (machine.h) tells `machine_cedar_io` to do
nothing: the display microcode posts the field interrupt (DisplayMain.mc,
the mask at 421B) and stores the keyboard and mouse. dorado-hps sets it
unless `-c`/`-t` script the input the host's way.

Bugs on the way:

- dorado-hps decoded the status block's mouse fields from bits 27/26:24.
  They are 25/24:22.
- dorado-hps must be stopped before its binary is replaced ("text file busy"
  makes `scp` fail and the old one keeps running).

**Known: capitals typed through MiSTerKVMS sometimes lose their Shift**,
consistently for letters in keyboard words 0-2 and never in word 3, where
Right Shift sits. MiSTerKVMS sends the modifier and the key together, so
both words change in the same tick and go out in index order. A person
presses Shift first; not yet confirmed with a real keyboard.

**Speed, measured** (rehearsal, HPS_HIST=1): 9.1 fabric clocks per
microinstruction:

- **5.0** for the basic sequence (FETCH, EXEC, NEXT, FINISH, and DEC, which
  is actually waiting);
- **1.9** for memory references (MAPA/MAPV/BOOK/RD);
- **1.4** for the IFU states;
- **1.06 clocks per microinstruction are the CPU held (`io_busy`) while the
  DDC line renderer writes the framebuffer one pixel per clock.** At Cedar's
  1024-pixel width that is 12% of all clocks; the pixel writes need not hold
  the CPU.

The speculative IM read does work (FETCH->EXEC 0.98 per microinstruction).
On MiSTer14: 5.6M microinstructions/s = 0.34x with the display running.

**First speed step: the line renderer no longer holds the CPU.** The DDC
render engine is now two engines.

- The **copy engine** holds the CPU (`io_busy`). It copies munches into the
  line buffer, and for a render job snapshots the line's words, at most 64,
  into a private `r_line` and latches the job.
- The **pixel engine** writes the framebuffer from that snapshot without
  holding the CPU. A render job waits for it only if the previous line is
  still being drawn, with any munch copy queued behind it in order.

Results:

- Rehearsal: 9.09 -> **8.15** clocks per microinstruction; the RTL
  framebuffer is unchanged (167,459 px).
- Galaxian unchanged (3,259 px).
- MiSTer14: 0.34x -> **0.38x** (6.3M microinstructions/s).

The clock: 55 MHz fails timing by 0.40 ns. Every one of the 12 worst paths
runs `ifu_idcnt` -> `x_pd` (the IFU operand `A<-Id` through the ALU into Pd
in one clock). Registering that operand ahead of C_EXEC is the next timing
fix; the core stays at 50 MHz (+1.7 ns).

### 8.5 Toward 1x: the sequencer (2026-11-01)

Timing-only sequencer changes (commit 6c325d125). Each was checked with
`make -C rtl_core test` and the Cedar lock-step; the full 270.7M listing run
passes.

| | clocks per microinstruction |
|---|---|
| start (after the background renderer) | 8.15 |
| Fetch hit returns at once, Md from `cache_q` (no M_RD) | 7.61 |
| IFU fetches do M_MAPA's work in the launching state | 7.17 |
| IFU dispatch: OPW reads IFUM, AW does BETA | 6.64 |
| C_PREF folded into the return | 6.6 |
| read-ahead: C_FETCH executes (fexec) | 5.83 |
| ...also after an IFUJump | 5.57 |
| branch-condition RM read in the fexec clock (no C_NEXTRD) | 5.47 |
| fetch hits skip M_BOOK; the dispatch-ending state does C_IFU_END | **4.98** |

Galaxian: 6.38 -> 4.30. **MiSTer14: 0.38x -> 0.59x (9.85M/s)** at 48 MHz
(+0.87 ns). At 50 MHz it misses by 0.016 ns on a Map RAM -> `m_wbway` path.
Area 58% (was 47%).

**The read-ahead, which is the subtle part.** C_NEXT (or the state that ends
an IFU dispatch) reads IM at `pred_np`. That is a pure, combinational
prediction of the next PC (`cond_pure_fn` is the branch test without the Cnt
side effect). C_FINISH, if that read is fresh, latches `ex_u` from it and
reads its RM/STK operand at `fin_rbase`/`fin_stkp`, the RBase and StkP
C_FINISH leaves. `rms_byp` gives that read write-first behaviour beside the
RAM; written inside the RAM, it stopped being block RAM, +8K ALMs.
`rm_pre_ok` holds only if:

- the prediction was the PC chosen;
- the task is the same;
- RBase and StkP are what the read used.

C_FETCH then executes in the same clock (`fexec`), unless a host-port
operation came first, I/O is busy, or the instruction is Read IM / Write IM.
Those two need C_EXEC's IM read at Link: a Write IM executed in FETCH
merged its half with the wrong word and corrupted IM, which is how the
Initial.mb lock-step caught it.

Three implementation rules learned the hard way:

- **Call a big task from one place.** `exec_task` and `mem_row_task` each
  inlined once per call site; two call sites took the core to 79% and 75%.
  They run after the `case` now, under `do_exec` / `x_rowload`.
- **Don't register a value at the end of the clocked block.** Its input is
  then the whole clock's logic (`id_r1` at -1.77 ns). `id_r0/id_r1` are
  computed at the top, from the start-of-clock IFU state, one clock behind,
  which is right because the clock before any execute never changes the IFU.
- **Quartus rejects `fn(...)[11:0]`.** Verilator accepts it.

**Where the 4.98 goes.**

| item | clocks per microinstruction |
|---|---|
| FETCH (with fexec) | 1.07 |
| NEXT | 1.0 |
| FINISH | 1.0 |
| IFU OPW / DEC / AW | 0.69 |
| MAPV, one per reference | 0.59 |
| MAPA (CPU references) | 0.16 |
| fallbacks: DEC / EXEC / NEXTRD | 0.27 |

**Next, in order of payoff:**

1. **Merge NEXT into FINISH** (about -0.74). The read-ahead would move to
   the execute clock, predicted from `ex_u`. The risk is timing: the merged
   clock chains the next-PC logic into the scheduler.
2. **IFU prefetch.** Fetch the next opcode while the current one's
   microcode runs, so OPW/DEC mostly vanish.
3. **The 50 MHz path, then more clock.**

1x is 16.67M/s: about 3 clocks per microinstruction at 50 MHz, or 4 at
67 MHz. Items 1 and 2 reach about 4 clocks, so 1x also needs the clock
near 67 MHz.

### 8.6 Where it stands (2026-11-05)

Section 8.5's list is done and overtaken. The detail, from 2026-11-01 on, is
in `docs/rtl-core-speed-plan.md` (Phases 1-3):

- the memory section and the IFU prefetch became their own modules;
- NEXT and FINISH merged (C_NF);
- the pipelined core (C_PIPE, `PIPELINE=1`) runs at **17.15 M
  microinstructions/s = 1.03x a Dorado** on MiSTer14 at 44 MHz.

What a new session needs to know:

- **The board build.** `/media/fat/Scripts/DoradoCedar.sh` loads
  `DoradoRtlCedarPipe_20261103c_44.rbf` (the C_NF fix, the stall report
  and the rx-load accounting; +1.388 ns at 44 MHz) and starts `/media/fat/dorado/dorado-hps`, built from
  ac33c0c47 with `rtl_core/hps/build_arm.sh`. The host and the core must
  come from matching commits: the status beat layout changed in d9926af49
  (stall report, beats 14-15) and ac33c0c47 (`rx_loads`, beat 11). MiSTer14
  is the development board; MiSTer11 belongs to the user.
- **Fixed.** The Schematic.cm / Moon.cm stalls had two causes: a C_NF that
  latched a stray IM word (9b57b5034), then a race in the PDI disk bridge
  between the ARM and the running guest (9d0239812). Both pictures paint.
- **Fixed: the mailbox Ethernet-TX-strobe deadlock (2026-11-07).** The
  intermittent freeze (~33% of longer runs) was NOT a DDR wait. UART
  sideband capture at a hang showed `dorado_rtl_storage_ddr` idle and the
  mailbox looping its TX states with `tx_pend` stuck high while the CPU sat
  held in C_FETCH (pc frozen). `eth_tx_packet_stb` is a core-clock level
  (`dorado_rtl_io`) cleared only under `io_tick` (a step tick); when a hold
  stops the CPU stepping, the strobe never clears, and the free-running
  mailbox re-armed `tx_pend` from that stuck level every clock -- so
  `hp_hold` never dropped and the CPU could never step to clear it, a
  self-sustaining deadlock. Intermittent because the hold must latch in the
  narrow window after the same TXEOP microinstruction sets the strobe. The
  mailbox now arms `tx_pend` on the RISING EDGE of `tx_packet_stb` only (a
  level is not an event): `rtl_core/dorado_rtl_mailbox.sv`. Confirmed on
  MiSTer14 -- 20/20 runs clean where the baseline wedged ~1 in 3 (p < 1e-3
  by chance); lock-step still PASSES (13 suites, 0 mismatches). The removal
  is total, not probabilistic: there is no re-arm from a level any more.
  (The same build also turned on `TIMEQUEST_MULTICORNER_ANALYSIS` +
  `OPTIMIZE_MULTI_CORNER_TIMING` -- the worst corner had never been analysed
  -- which lifted worst-corner setup slack and fixed the HDMI path at no
  speed cost. Strictly better, but not what fixed the wedge, and not yet
  propagated into `fpga/mister_rtl/build.sh`.)
- **Instruments.** The ARM/core accounting line, `DORADO_HPS_RXCHECK`,
  `DORADO_DISK_IOCB_TRACE`, `SIGUSR2` PDA dumps, `/tmp/hps-poke` and
  `/tmp/hps-dump-va`, all described in the speed plan. Tracing Mesa opcodes
  in the rehearsal: `HPS_OPTRACE` / `CEDAR_OPTRACE`.
- **Lock-step passing does not clear a board bug.** Both board-only bugs
  depended on DDR timing or on the ARM running beside the guest, which no
  simulation reproduces.

## 9. Smalltalk-76 on the clean RTL core (2026-11-08)

Smalltalk-76 (`SmalltalkDorado.eb!1` = DSemu, the XM Smalltalk pack) is the
second world on the clean core and the first that needs the Trident disk:
its object memory lives on the pack and it pages through the disk task. Cedar
reaches its disk through the host's PDI bridge; Smalltalk drives the Dorado's
own disk controller. The route was chosen by measurement first: the live
desktop's slow I/O is the Trident disk and the display, and **no Ethernet**
(Lisp needs a Leaf/STP file server as well, so it comes second).

### 9.1 The CPU runs Smalltalk (lock-step)

`sim_cedar.cpp` takes `SIM_WORLD=smalltalk` (checkpoint
`build/good-packs/smalltalk76.snap`, argument 3 the pack). The pipelined and
the sequential core each run **80,000,000 Smalltalk microinstructions** in
lock-step with the C emulator, 0 mismatches, through a click on the browser's
'Kernel Classes' (`CEDAR_CLICK=110,142 CEDAR_CLICK_HOLD=40000000`).
Smalltalk takes 2.42 fabric clocks per microinstruction on the pipelined
core (Cedar 2.57).

### 9.2 The RTL Trident controller: never run before, now checked against disk.c

Every MiSTer build had `DORADO_NO_DISK`, so the controller in
`dorado_rtl_io.sv` had never executed. `SIM_DISK_RTL=1` makes it the disk:

- `disk_bridge_en` (top) gives the CPU the RTL disk's answers for task 014 /
  TIOA 010-014, its DMux bits, its IOStore munch and its wakeup, while the C
  devices answer everything else; the C disk's wakeup is not poked into the
  RTL, so a timing difference is a divergence.
- `disk_time` / `disk_time_ext_en` rotate it on the C disk's clock. **C ticks
  the disk with bb.cycles**, so its Trident turns ~3.7x faster than a real
  3600-RPM drive (`DORADO_DISK_CYCLES_PER_REV` = 277,778 *bb* cycles =
  75,075 microinstructions) -- the same bb.cycles class as the old Cedar
  field-rate error. Left alone: C is the oracle and Smalltalk is proven on it.
  The board's disk turns at the real rate (277,778 microinstructions).
- After every step the two controllers are compared field by field
  (`compare_disk`), and the two packs at the end.

The checkpoint at the Alto Executive (`smalltalk76-exec.snap`, before
`Bootfrom xmsmall.boot`) covers the whole image load. Three defects, each
named by the compare at the step it happened:

1. **READ and WRITE swapped.** disk.h numbers the block ops DONE 0, WRITE 1,
   RDCHK 2, READ 3; the RTL had READ 1 and WRITE 3 (and the muffler's
   Read/Write bits with them). Now named `DISK_OP_*`.
2. **A read stream ran on into a write block.** When a read block ends and
   the next is a write, disk.c's refill stops (`if (!read_stream_active)
   break`); the RTL `continue`d and filled the write's FIFO with sector words.
3. **The sector buffer was loaded too late.** The tick raised
   `disk_sector_read_req` and started the read in the same clock, so the
   stream read the previous sector. Replaced by the engine (9.3).

Result (behavioural data path, all three fixed): **440,000,000
microinstructions in lock-step from the Executive to the desktop** (124,945
px), 286 transfers, 4 sector writes, every step's disk state equal to
disk.c's, the packs equal sector for sector.

### 9.3 The sector data engine (synthesizable)

The first form did a FIFO refill inside the tick: a 32-iteration loop reading
a 1,036-word sector array combinationally. Now the tick only asks
(`disk_job_load/commit/refill`); an engine does the work between ticks, one
step a clock, holding `io_busy` so the CPU waits in C_FETCH -- atomic
between two microinstructions, as in disk.c:

- the sector in four 260 x 16 lane RAMs (one M10K each; a lane-enabled
  64-bit RAM was not inferred and cost 16,640 registers);
- load/commit over a 64-bit beat port: DDR on the MiSTer (shared with the
  mailbox on the storage engine's aux port by an arbiter in the top), a DPI
  model over a private pack copy in lock-step;
- the pack in DDR at byte **0x3100_0000**: a record per sector in the pack's
  flat CHS order, header | label | data, ceil(words/4) beats (Smalltalk: 67);
- record address one multiply a clock; the rotation's divider and modulo
  replaced by tables (`disk_cps_rom`, `disk_spr_fn`); loops bounded for
  Quartus.

The harness drains the engine before each compare (`rtl_drain_disk`). The
engine passes the same image load in lock-step: 440M microinstructions,
19,162 beats read (286 loads x 67) and 1,064 words written (4 x 266), packs
equal.

**Out of the tick, for timing.** The first synthesis of the disk (it had never
been through Quartus) failed by **44 ns** at 44 MHz: the disk's share of the
single-clock I/O tick (strobes, block sequencing, the 16-iteration fast-I/O
loops, the sector-pulse timer, the wakeup) on 32-bit `int` counters. The tick
now latches the disk's strobes (`dk_*`) and a **disk stage** does the work in
the clocks after it -- stage 1 the strobes, stage 2 the timer -- under
`io_busy`; it runs only when the tick had a disk strobe or a sector pulse is
due, and the DSK wakeup stands in the tick from the TW registers. Counters are
narrowed, the fast-I/O FIFO moves are parallel. The sector buffer is four
lane RAMs (a lane-enabled 64-bit array was not inferred: 16,640 registers).
Result on the pipelined core: **32,715 ALMs (78%), +1.121 ns** core, +0.185
HDMI; the stage costs 2.70 vs 2.67 clocks per microinstruction.

### 9.4 The display: pixel-exact, no work needed

`SIM_DISPLAY_SHADOW=1` syncs the RTL DDC from the C display and lets it draw
from the display tasks' Outputs and munches. On the Smalltalk desktop it
matches C on **477,888 of 477,888 pixels** once aligned: the RTL's 22-line
offset is `display_top_lines`, and it shows 608 of the bitmap's 640 columns
because that is the line width the display microcode programs into the DDC
(C's rasterizer paints the whole bitmap). The XM bank-1 bitmap needs nothing
special: the microcode supplies the bank in its fetch addresses.

### 9.5 The board configuration (`CORE=smalltalk`)

The Cedar checkpoint core with the disk in: `build.sh CORE=smalltalk`
(`DORADO_SMALLTALK`, `DORADO_CHECKPOINT`, no `DORADO_NO_DISK`), init from
`rtl_core/init_smalltalk` (`SIM_WORLD=smalltalk CEDAR_SPLIT=1 CEDAR_DISPLAY=1
CEDAR_DUMP_INIT=../rtl_core/init_smalltalk obj_cedar/Vdorado_rtl_top 0`; the
dump carries the disk, with the rotation timer unarmed so the board arms it
at the real rate). `dorado-hps -w smalltalk` restores the snapshot, loads
storage and the pack into DDR, and says go. Mouse: absolute (the Alto way),
through the status block to the C machine, which writes MOUSELOC/UTILIN as
the emulator does.

Rehearsed in `sim_hps` (`make PIPE=0 HPS_ST_OBJ=obj_hps_st0
obj_hps_st0/Vdorado_rtl_top`, run with `SIM_WORLD=smalltalk
HPS_INIT_PARENT=obj_hps_st0`): the desktop draws (RTL DDC 112,145 lit, host
render 124,945 px), and a click on 'Kernel Classes' **opens it in the
browser** (128,535 px; C's `verify-smalltalk-input` is 128,547). Three things
found on the way:

- **The ARM's snapshot must be the wasm32 bake of the same run, with
  `DORADO_FAKE_TIME`.** Without it the boot asks the Pup server for the date
  and two bakes differ in storage at the same cycle (`ffc89879...` vs
  `dfdba7db...`). Pinned, native and wasm32 hash alike (`00056671553d2315`).
- **Host-only input cadence.** The emulator re-presents the Alto input cells
  every microinstruction with a read-back; through the mailbox that was ~17
  round trips per host step and held the core to 15.25 clocks per
  microinstruction. Host-only now presents them every 2 ms of guest time --
  the terminal's own message rate -- (2.42 clocks; 531,271 -> 1,041
  operations per 2M microinstructions).
- **Host-only input gate.** The seeding was gated on the C CPU's
  `ifu_active`, frozen at the checkpoint's value in host-only mode; the time-
  pinned checkpoint has it set, so no input reached the guest.

**On MiSTer14 (2026-11-08):** `/media/fat/Scripts/DoradoSmalltalk.sh` (core
`_Development/DoradoRtlSmalltalk_44.rbf`, host `dorado/dorado-hps-st -w
smalltalk`). The core runs **16.5M microinstructions/s = 0.99x a Dorado**
(2.67 clocks at 44 MHz), the desktop draws (124,945 px rendered from FPGA
memory), and a scripted pointer move reaches Smalltalk's own cursor cells
(426/427) and brings the Classes browser forward. A *scripted* button press
(`dorado-hps -c`) does not select: the core's terminal model reports the PS/2
mouse's buttons (up) to the terminal microcode, which rewrites UTILIN every
~160,000 cycles over the host's press -- two writers of one cell. A real
mouse feeds both alike (the rehearsal, which drives the core's mouse inputs,
selects 'Kernel Classes').

**OPEN: the C_PIPE overlap breaks Smalltalk under DDR timing.** With the
pipelined core free-running against DDR (`sim_hps`), the screen is noise
within 2M microinstructions. The sequential core, the pipelined core with
`RTL_OV_HOLD=1`, and split mode (block-RAM storage) are all correct, and the
pipelined core passes 80M steps in lock-step (where the harness suppresses
the overlap whenever a device event lands). So it is the execute overlap,
under DDR latency, in something the Alto-family display or disk microcode
does and Cedar does not. The Smalltalk board build holds the overlap off
(`Dorado.sv` ties `ov_hold` for the Trident worlds) until it is found. Reproduce:
`make PIPE=1 obj_hps_st/Vdorado_rtl_top`, `SIM_WORLD=smalltalk
HPS_INIT_PARENT=obj_hps_st RTL_FB_OUT=/tmp/fb.pbm obj_hps_st/Vdorado_rtl_top
2000000`.

### 9.6 Interlisp-D Lyric (2026-11-08, in progress)

Lyric (`worlds/aemu.eb` on its Trident pack, the full-library sysout) needs
the same disk and, it turned out, almost nothing else:

- **CPU:** lock-step (`SIM_WORLD=lisp`, with `DORADO_DISPM_PRESENT=1
  DORADO_FAKE_TIME=1783285880`) passes; 3.3 clocks per microinstruction.
- **Display:** Lyric needs the DispM board present (without it,
  `DisplayInitConfig` stops in Swat) and drives its desktop through DispM's
  Alto-terminal interface -- tasks AHT (4) / AWT (011), TIOA 0360-0367 -- which
  the RTL DDC already decodes. One defect: the DDC woke **DWT** for every word
  request; the word task pairs with the terminal task (AHT -> AWT), as
  machine.c's display wake has it. Fixed; split mode then draws the Lisp
  desktop **pixel-identical to C (209,165)**.
- **Input:** the board rehearsal (`obj_hps_lisp`, `HPS_ST_INIT=init_lisp`)
  types through the core's keyboard inputs; `(PLUS 1 2)` reaches the XCL Exec
  as typed and Lisp answers (`Undefined car of form PLUS` -- the XCL name is
  `IL:PLUS`). A typist presses Shift before the key; a script that changes
  both in one terminal message gets the key unshifted.
- **Storage image.** The ARM restores a wasm32 bake; for Lisp the August
  native and wasm32 bakes hash differently at the same cycle (`e323eed7...`
  vs `79f27ddf...`) though both pin the time. So the checkpoint dump now also
  writes `storage.bin` (the native run's exact storage) and dorado-hps loads
  DDR from `build/good-packs/<world>.storage` when present: the core's init
  and its storage always come from the same run.
- **Board:** `build.sh CORE=lisp`, `dorado-hps -w lisp` (Leaf/STP from
  `chm/lisp/ftp-root`, 426 files). The Lisp files are staged on MiSTer14.

### 9.7 To do: present each world at its own raster (2026-11-11)

On MiSTer14 Smalltalk-76 shows a wide white band at the right: `Dorado.sv`
scans out a fixed 1024x808 frame (Cedar's), Smalltalk's bitmap is 640x800,
and the undrawn columns 640-1023 (and lines 800-807) show as white. The C
emulator's frontends present each world at its native raster. Fix: for
`DORADO_SMALLTALK`, limit DE to 640x800 and set `VIDEO_ARX/ARY` to 640:800 so
the scaler centres it with black borders; Lisp and Cedar are 1024x808 already.
Checked with a real keyboard and mouse the same day: Smalltalk's browser
selects and its pop-up menus work.

### 9.8 Lisp on the board with a real keyboard and mouse (2026-11-12)

- **Cursor X.** The HDMI cursor stayed at one X while the guest's cursor
  moved, so clicks landed away from the arrow. Lyric's display microcode
  (`LispDisplayMain.mc` EndBootCheck) sends `CursorX_ CursorXConst - cursorX`,
  and under `aemu.eb` the value arrives on TNLCB tagged register 03
  (APointer), not 15 (CursorX) -- the same ReadIM select question as Cedar's
  13. `dorado_rtl_io.sv` takes a TNLCB write of reg 03 >= 04000 from the AHT
  as CursorX (measured in C: x = 4030 - val, rebased onto the draw's 3771).
  Verified in sim (`HPS_CLICK=300,400`: cursor drawn at x 300) and on
  MiSTer14.
- **A one-button mouse.** `Dorado.sv` maps Option (Alt) + click to Yellow
  (middle) and Command (GUI) + click to Blue (right), the Mac X11 convention;
  neither key is in the Dorado keyboard matrix. The modifier is latched at the
  press. Verified on MiSTer14: Blue gives the background and window menus,
  Yellow selects words in TEdit.
- **Busy cursors are Interlisp's own**: the whole cursor inverted during GC,
  horizontal bars inverted for swap read/write and stack operations (Koto
  `ChapTERMINAL` 30.4). Not a display bug.
- **Timing.** With seed 2 every clock closes at every corner (core +1.543,
  HDMI +0.136 ns at slow -40C). Seed 3 missed HDMI by 0.179 ns and seed 4 by
  0.268 ns on the same RTL -- the HDMI path is placement noise, so re-check
  it on every Lisp build.
- `(IL:FILESLOAD HELPSYS)` + `(IL:LOAD '{DORADO}<IRM>IRMDEMO)` (Guest/Guest)
  opens the IRM on the board, over Leaf from the ARM.

### 9.9 Smalltalk at its own raster; the C_PIPE overlap bug narrowed (2026-11-12)

- **Raster.** `Dorado.sv` now presents Smalltalk-76 at 640x800 (DE and
  `VIDEO_ARX/ARY` from `DISP_W/DISP_H`; Lisp and Cedar stay 1024x808), so the
  scaler centres it with black borders instead of a white band (9.7). Timing
  closes at every corner (core +1.890, HDMI +0.162 ns). On MiSTer14.
- **Speed: overlap is the only route to 1x for Lisp.** Sequential, Lisp is
  3.27-3.30 clocks per microinstruction (0.81x at 44 MHz) and the core has
  only ~7% clock margin. With the C_PIPE overlap it is 2.34-2.39 clocks
  (~1.1x) -- but it is wrong. All numbers below are `sim_hps`, PIPE=1,
  DDR model, deterministic, and need no board:
  `make PIPE=1 HPS_ST_OBJ=obj_hps_lisp_pipe HPS_ST_INIT=init_lisp
  obj_hps_lisp_pipe/Vdorado_rtl_top`, then `SIM_WORLD=lisp
  HPS_INIT_PARENT=obj_hps_lisp_pipe DORADO_DISPM_PRESENT=1
  DORADO_FAKE_TIME=1783285880 RTL_FB_OUT=fb.pbm ...Vdorado_rtl_top 30000000
  [+ovmask=N]` (`RTL_OV_HOLD=1` is the reference: 209,165 lit at 30M).
  Pass `HPS_ST_INIT` explicitly: the Makefile default carries trailing
  blanks and a comment, which splits the dependency path.
- **Bug 1, fixed (`fin_fault`).** A processor reference completing WITH A
  FAULT in the clock C_PIPE executes ex_u beside x_u's finish:
  `mem_pref_task` sets `wakeup_pending[15]` before the finish schedules, but
  `sw_maybe` was computed from the registers, so the finish switched to the
  fault task while ex_u ran as task 0 (Lisp printed `[C_PIPE] finish chose
  ... task 15` and `the forwarded view differs`). `x_ready` and `pent` now
  exclude it. Suite passes.
- **Bug 2, Smalltalk: IFUJump in the overlap.** Smalltalk is noise in 2M
  with overlap (233,918 lit vs 112,145). Taking a dispatching ex_u out of the
  pipeline (`+ovmask=32`) -- or a dispatching x_u (`+ovmask=256`) -- fixes
  the 2M screen, at 2.21 clocks. NOT yet a win: over 30M the same knob gives
  3.87 clocks against 2.70 sequential (drain/re-entry thrash, unexplained).
- **Bug 3, Lisp: open.** With bugs 1+2 excluded Lisp still Raids by 30M
  ("Fault on resident page" {0,177777}; without the IFUJump knob, "Invalid
  address" {1,66400}). No single exclusion fixes it: ex_u memory refs (1),
  Md reads (2), x_u refs (4), FF by FA group (1024/2048/4096/8192), LC (512)
  all still fail; only knobs that remove nearly all overlap pass (8, 64,
  128), at no gain. The self-checks print nothing. Next tool: a lock-step
  that does NOT suppress overlap at device events, or a per-task retired-
  instruction diff of overlap vs `RTL_OV_HOLD` up to the first divergence.
- `+ovmask` knobs (sim only, `dorado_rtl_cpu.sv`, `ov_mask`): 0 ex_u refs,
  1 ex_u reads Md, 2 x_u refs, 3 nothing beside a finish, 5 ex_u IFUJump,
  6 ex_u FF FA!=0, 7 ex_u any FF, 8 x_u IFUJump, 9 ex_u LC!=0, 10-12 FF
  function FA=1/2/3, 13 FF as constant. Off by default; synthesis sees 0.

### 9.10 The C_PIPE overlap bug: found and fixed (2026-11-12)

Bugs 2 and 3 of 9.9 were ONE defect, and it was not in the overlap itself.

- **The defect: a C_NF that waits loses its branch-condition register.** An
  instruction whose branch tests its own RM/STK register (R odd/even, R<0)
  has that register in `rm_q`/`stk_q` for exactly one clock after the
  execute. C_NF evaluates the condition from it. But while C_NF is in
  flight the RM port has already moved on to the NEXT instruction's operand
  (`x_pred_fresh`), so if C_NF waits a clock (`ifu_bk_busy`, `io_busy_tick`)
  the condition is evaluated from some other register. The fexec path
  already guarded the `x_pref_pending` wait for this reason (its comment
  says so); the other two waits were open. First seen at Smalltalk's
  `I.Vars: T _ Id, B _ Mode, DblBranch[Push, Pull, R Even]`, the first
  instruction after a dispatch (so `ifu_bk_busy` holds C_NF): it read RM[0]
  where it wanted Mode. That is why every knob involving IFUJump "fixed" it.
- **The fix** (`cw_valid`/`cw_rv`, `dorado_rtl_cpu.sv`): a waiting C_NF
  latches the condition register in its first clock and `rm_q`/`stk_q`
  present the latch until C_NF proceeds. It is not PIPELINE-specific: the
  sequential core's fexec -> C_NF path had the same hole, only rarer.
- **Result** (`sim_hps`, PIPE=1, 30M microinstructions, framebuffers
  BYTE-IDENTICAL to `RTL_OV_HOLD`): Smalltalk 2.19 clocks (hold 2.70), Lisp
  2.88 (hold 3.30). At 44 MHz that is 1.20x and 0.92x. `make test` and
  `make PIPE=1 OBJ_DIR=obj_dir_pipe test` pass. `Dorado.sv` now ties
  `ov_hold` to 0 for every world.
- **The tool that found it**: `HPS_RETIRE=file[,from,to]` (sim_hps) writes
  the processor after every retire (snap_* beside an overlap), and
  `rtl_core/tools/retire_diff.py A B [task]` compares one task's retired
  instructions between an overlap run and an `RTL_OV_HOLD` run. Compare per
  TASK: wakeups land at different instructions in the two runs, so a
  whole-machine diff diverges at the first task switch. The first task-0
  difference after the fix (Smalltalk, instruction 43,511) is an IFU
  dispatch taking an interrupt trap one opcode apart -- timing, not a bug.
  (zsh trap: `env $E prog` does not split `$E`; the run silently became the
  default Cedar world.)
- **Lisp to 1x: what is left.** The census puts 0.64 clocks per instruction
  in `finish waits: reference` -- Lisp makes 34 references per 100
  instructions and each finish waits ~2 clocks for `mem_fin`. Needs 2.64
  clocks at 44 MHz; either shorten the hit path in `dorado_rtl_mem.sv` or
  let the finish retire before a non-faulting hit completes. Also 0.076
  clocks per instruction waiting on the I/O engines.
- **On MiSTer14** (both cores rebuilt with `ov_hold` = 0, 44 MHz, timing
  closed at every corner): Smalltalk 20.39 M/s = **1.22x** (2.16 clocks,
  124,945 px, the gate figure); Lisp 15.53 M/s = **0.93x** (2.83 clocks,
  209,165 px, Exec prompt, no Raid). The `cw_*` mux on `rm_q` cost core
  setup slack: Smalltalk +1.890 -> +0.127 ns, so there is no clock headroom
  left to buy Lisp's last 7%; registering the latch select differently, or
  the reference-wait work above, is the way. The previous non-overlap
  bitstreams are kept on the board as `/media/fat/DoradoRtl*_44.noov.rbf`.

### 9.11 Board fixes found by use (2026-11-13)

**The host's inputs died after 18 hours.** The status beat carries
`cycles[39:0]`; at 2^40 microinstructions the host's count stepped back and
`machine_host_input_due` waited for a cycle a day away, so the absolute
mouse and UTILIN were no longer seeded (Lisp, Smalltalk; the keyboard goes
through the terminal microcode and kept working). `read_status` accumulates
the 40-bit difference now. Seen at 64,614 s of a Lisp session: the host's
read rate fell to a third and no store was issued again.

**Smalltalk's right 32 columns were white.** The Alto display microcode
clips each line to `MaxWidthWordsAlto` = 38 words on an Alto monitor
(`DisplayMain.mc`), and Smalltalk-76's bitmap is 40. `dorado-hps -w
smalltalk` presents an LF monitor in full-screen mode instead, as Alto
opcode 61032B (`SetDisplayConfigA`, AC0 = -1) does: `DisplayConfig` (RM
166B) OR 3, `MaxWidthWords` (RM 156B) OR 177500B (the 377 in the left half
makes DHT reinitialise at the end of the field). The ORs are queued in the
mailbox BEFORE the go (`DORADO_HPS_RM_OR`, also usable by hand): issued
after it, a few fields were drawn the old way first and left two stale rows
at the top that nothing redraws (the bitmap's rows 0-1 carry no DCB line).
The LF mode moves two constants, both in `Dorado.sv` for
`DORADO_SMALLTALK`: `display_top_lines` 26 (`TopBorderCntLF` 14 against
12) and the new `display_cursor_dx` 286 (the MargConst table's CursorX 611
against 325). On mister14 the HDMI frame equals the host's render of the
same memory, bit for bit apart from the cursor. `+ddcdbg` (sim only)
traces the DDC's line starts.

**A core loaded from the MiSTer menu shows a white screen**: it waits for
`dorado-hps`. Use the Scripts entries (DoradoSmalltalk.sh, DoradoLisp.sh).

### 9.12 Lisp to 1x: the reference no longer stalls the pipeline (2026-11-14)

Where the clocks went (`sim_hps`, Lisp, PIPE=1): 43 references per 100
instructions, and EVERY cache hit cost C_PIPE one stalled clock, because
the finish of the instruction that made the reference waited for the
section's registered result (`mem_fin`), and one more when the next
instruction read Md. The Dorado does neither (HM 5.4, "Memory Timing and
Hold": "barring io activity and cache misses, the processor can make a
fetch or store reference every cycle and never be held"; only "a fetch,
store, or <-Md is held after a preceding fetch or store by the same task
has missed", and an immediate <-Md in the cycle after a fetch).

Three changes, all in C_PIPE and off under `ov_hold` (the per-step
lock-step books a reference at its own instruction, as the C model does):

- **The Md of a fetch completing this clock is forwarded** (`cur_task_md`
  takes `md_r`, the section's register, when `mem_fin`/`d_fin` say a fetch
  delivers now). The execute beside that finish used to bubble a clock for
  the Md file to be written. 2.87 -> 2.72 clocks.
- **The early retire** (`early_c`): the reference's instruction retires
  while the result is outstanding when the next instruction neither refers
  nor uses Md (`follow_safe_fn`). The reference becomes the DETACHED one
  (`d_pending`, `d_kind`, `d_fcnt_before`); it is booked
  (`mem_pref_task`) when its result arrives, and every later finish waits
  for that (`d_wait` in `p_stall`, `finish_task`, C_NF). A fault then wakes
  the fault task one instruction later than the C model, which is the
  Dorado's own order: the instruction after a faulting fetch is not held
  unless it refers or uses Md.
- **The fast hit** (`early_f`): the same, beside an instruction that itself
  makes a Fetch/IFetch/LongFetch/Store or loads T or RM/STK from Md --
  allowed only when the first reference is KNOWN to be a hit that cannot
  fault. `dorado_rtl_mem` registers that in the read-ahead clock (`sp_hit`,
  `sp_wp`: the read-ahead address against the row as the row registers will
  hold it), so the decision costs the processor two registers and no hit
  compare in the execute clock; `x_spec_mine` says the read-ahead of the
  clock before was this reference's. A deferred Md (`x_md_late`) is taken
  when the detached result arrives (`x_md_eff` for the forwards in the same
  clock). If the first reference misses, nothing is known until it is over
  and the second instruction waits: the manual's hold.

Why not let any reference follow any reference: the second would be booked
(the emulator's Pipe entry overwritten) before the fault task had seen the
first's fault, and its execute cannot be undone -- `exec_task` writes Q,
Link, the dispatch register and Id at the execute.

Result (`sim_hps`, 30M microinstructions, framebuffers BYTE-IDENTICAL to
`RTL_OV_HOLD`): Lisp 2.58 clocks (was 2.87), Smalltalk 2.09 (was 2.19). At
44 MHz that is 1.02x and 1.26x. A sim assertion checks every fast hit
against the section (`[C_PIPE] fast_hit, but the section did not
complete`): silent over both runs.

What still stalls (Lisp, per 100 instructions): 21 clocks where the next
instruction refers but the first was not a known hit -- mostly because a
Store that writes keeps the words' port, so the next read-ahead is not
taken (`spec_take`'s `!c_store_write`); 6 for deferred Md, 3 each for an
immediate Md (the Dorado holds that too) and the shifter. C_NF still waits
for a reference as before (20 clocks per 100, 3 of them with a safe next
instruction).

Sim-only census added (`+refhist`): `[refhist]` (clocks a reference is
outstanding, by kind), `[refstall]` (why a finish waited), `[c_nf]`;
`sim_hps` now calls `top->final()`.

**On MiSTer14** (Lisp core rebuilt, 44 MHz): 17.36 M/s = **1.04x a
Dorado** (2.53 clocks; it was 0.93x), 209,165 px, Exec live, mouse and
keyboard work. Timing: the first fit (SEED 2) missed by 0.39 ns on the
prefetch-to-dispatch path (`pf_pcf` -> `nx_fast` -> `pred_np` -> `ex_u`,
the worst path before this work too); SEED 3 closes at every corner (core
+0.249 ns, HDMI +0.124 ns, hold +0.107 ns, no combinational loops). The
seed is line 82 of `Dorado.qsf` in `~/mister/DoradoRtlLispPipe44` on the
build host, not in the repo. The previous bitstream is on the board as
`/media/fat/DoradoRtlLisp_44.pre-early.rbf`.

**The doubled keys: two writers of the keyboard words (fixed, host
only).** MiSTerKVMS typing 200 characters (`type --delay 150 --hold 60`)
into the Lisp Exec doubled about 5 of them on the previous core and 7 on
this one ("11234", "55", "ss"), and lost Shifts ("(" typed as "9");
`sim_hps` with `HPS_TYPE` did neither. The terminal microcode on the FPGA
stores the keyboard words (177034B-177037B) itself, and
`machine_seed_alto_live_io` on the ARM ALSO stored them, from the copy of
the keys it reads out of the status block. That copy lags, so the host put
a stale word over the fresh one: down, up, down. In `sim_hps` the host's
copy is the script's own, never stale. With
`dorado_machine_display_microcode` set (dorado-hps with live input) the
host now leaves those four words alone; it still seeds the mouse cells.
After: 100 characters at the same setting, no doubles; `(IPLUS 1234 4321)
"Hello, World"` at 50/30 ms arrives exactly. What is left is upstream of
the Dorado: of 100 key presses the core's own keyboard words
(`DORADO_HPS_KEYTRACE`) showed 98-99, and Lisp printed every one the core
saw. Deployed as `/media/fat/dorado/dorado-hps-st` (previous:
`dorado-hps-st.pre-kbd`); Cedar's `dorado-hps` is not replaced.

**All three cores rebuilt with it (2026-11-14), on mister14:**

| world | before | now | seed that closes | previous bitstream on the board |
|---|---|---|---|---|
| Lisp | 0.93x | **1.04x** (17.36 M/s) | 3 | `/media/fat/DoradoRtlLisp_44.pre-early.rbf` |
| Smalltalk | 1.26x | **1.29x** (21.55 M/s) | 7 | `/media/fat/DoradoRtlSmalltalk_44.pre-early.rbf` |
| Cedar | 1.02x | **1.05x** (17.52 M/s) | 5 | `_Development/DoradoRtlCedarPipe_20261107fix_44.rbf` |

Cedar's other seeds: 1 missed by 0.27 ns, 3 by 0.04, 7 by 0.12; seed 5
closes with +0.013 ns to spare, so this core has no margin left at 44 MHz.
The seeds live in each build directory's `Dorado.qsf` on the build host.
Cedar now runs the same host program as the other two
(`dorado-hps-st`, which has the 40-bit count fix); `DoradoCedar.sh` on the
board points at the new core and host (`DoradoCedar.sh.bak-20261114` is the
old one). `sim_hps` Cedar, 30M microinstructions: framebuffer byte-identical
with and without the overlap. On the board the Schematic button paints
ProcH BitSlice 07 (89,414 px).

**RETRACTED the same day: "about half of Cedar's starts are deaf to keys
and buttons".** That was the test, not the machine. `misterkvms-cli mouse
down left` followed by a separate `mouse up left` does not hold the button:
the press ends when the first command exits, so the core saw a press about
a millisecond long, which the status beat sometimes caught and Cedar's
60 Hz sampling caught about half the time (a button held this way for 24 s
never showed in `input:` at all). With `mouse click left`: five starts of
the new core and host, five live (the Schematic button paints, 89,414 px),
and text typed after a focus click arrives with no character doubled.
**Use `mouse click`; never judge input from a down/up pair.**

**MiSTerKVMS and Shift:** it presses Shift and the key in the same
millisecond (one status sample shows both). Smalltalk then sometimes sees
the key first ("(3+4)" arrives as "93+40"), and Cedar always does for keys
in words 0-2 ("Echo (x)" arrives as "echo 9x0"; section 8.4 saw the same):
Cedar samples KeyBits once a field and takes the changes of one sample in
word order, so a Shift inside the same 17 ms as its key comes second. Lisp
got every shifted character right. A typist's Shift leads by far more.

### 9.13 The checkpoint out of the bitstream: one core restores any world, and saves the running one (2026-11-14/15)

Why there are three cores: each has its world's saved state compiled in --
the RAM contents as `$readmemh` files and about 3,100 register values as
reset constants (`init_*/checkpoint_{cpu,mem,io}.svh`). The aim is one core
that either boots or restores a checkpoint file, and can save one. Step 1
moves the REGISTERS out, behind `DORADO_SCAN` (`make SCAN=1`); every build
without it is unchanged (`make test`, `PIPE=1 test` pass; the compiled-in
Lisp sim is cycle-identical).

**The mechanism.** `tools/gen_scan.py` reads the checkpoint files' register
names and writes `scan_{cpu,mem,io}.svh`: one statement per chunk of 32
registers, `{sc[k], a, b, ...} = {a, b, ..., sc[k+1]}`, so the widths are
the compiler's and the chain is in the files' order. Each module's main
clocked block gets `else if (scan_en)` ahead of its normal work. The chain
runs mailbox -> io -> memory section -> cpu, with one register between
modules (`scan_out`; `pad` entries in the map). `dorado_rtl_mailbox` reads
the bits from the scan region (beat 16384 on) when the control word's high
half gives a count, holds the core out of reset but idle (`scan_ph`, clock
enables off) while it shifts, and goes at the last bit. A core with the
registers compiled in stays in reset through that and ignores it.

The host (`hps_host.cpp load_scan_regs`) packs `<snapshot>.regs` (the
three checkpoint files as `name hex` lines, `gen_scan.py --regs INIT_DIR
OUT`) in the order of `hps/scan_map.h`, which `gen_scan.py --map` makes
from a `+scanmap` run of the sim (each register's `$bits`). The map is
part of the host binary: **regenerate it whenever a checkpoint register
changes width or the lists change, or the restore is silently misaligned**
(the first run was two bits out and started at cycles/4).

**What it costs on the DE10-Nano, measured (Lisp core, seed 3):**

| chain | bits | ALMs | registers | result |
|---|---|---|---|---|
| none (compiled in) | 0 | 33,744 (81%) | 31,092 | timing met |
| every checkpoint register | 19,134 | 47,527 (113%) | 35,690 | does not fit |
| ...shift as one `else if (scan_en)` | 19,134 | 45,237 (108%) | 35,690 | does not fit |
| without the cache rows, HRam, task_md_ready | 7,614 | 38,477 (92%) | 34,890 | fits; setup -0.889 ns core, -0.336 HDMI |

A scanned bit costs about a logic cell: Quartus does not put the shift on
the registers' synchronous-load input for the big arrays, and constants and
unused bits it used to remove must now exist. The single condition helped
only the cpu (-5,000 cells). The two big arrays were the cache's row
arrays (6,400 bits) and `ddc_hram` (4,096).

**So the restored machine starts with an empty cache.** The cache rows are
out of the chain (reset leaves every line invalid), and the host writes the
snapshot's dirty lines to storage before the go (49 lines for Lisp). `sim_hps`
Lisp, 30M microinstructions: framebuffer byte-identical to the compiled-in
core.

**Step 2, the memories by load record (2026-11-14).** Nothing of the world
is compiled into a scan core now. Before the shift the mailbox reads load
records from DDR (`B_LOAD`, mailbox beat 32768; one 64-bit record a beat,
kind in bits 63:60, ended by kind 15; the kinds are listed in
`dorado_rtl_mailbox.sv`) and puts each on `ld_we`/`ld_d`:

| memory | how it is written |
|---|---|
| IM | a second (write) address: the record's, else `link_at_issue` -- the read address is untouched |
| IFUM | through the `x_ifum_*` registers of the IFUMRH/IFUMLH write, one record a half |
| RM, STK | a third case of the write port's address/data |
| ALUFM | assigned in the main sequencer's load branch |
| Map | a second (write) address: the record's, else the reference's or the victim's entry |
| Ethernet receive buffer | the mailbox's own `rx_wr` port |

The cache words are not loaded (the cache starts empty) and `ddc_hram` is
not either: the RTL display does not read it. `gen_scan.py --rams INIT_DIR
OUT` writes `<snapshot>.rams` (76,305 records for Lisp) from the directory's
`.hex` files; the host copies it to the load region, setting Ref and Dirty on
the Map entries of the pages whose dirty lines it wrote back, as the memory
section's own writeback would. Two simulation assertions guard the second
addresses (a Write IM is at Link as issued; a Map write is at the
reference's or the victim's entry). `sim_hps` Lisp, 30M microinstructions:
framebuffer byte-identical; `make test` and the PIPE suite pass.

**On the board (mister14, 2026-11-14).** The registers-by-scan bitstream
(memories still compiled in; the one that fit) with the new host: Lisp comes
up at 17.55 M/s (1.05x), 209,146 pixels, and takes typed input. The host's
DDR mapping had to grow (`hps_main.cpp DDR_LEN`, 0x810000 -> 0x1000000): it
stopped before the scan region and the host died at the first scan beat.
Files on the board: `_Development/DoradoRtlLispScan_44.rbf`,
`dorado/dorado-hps-scan`, `Scripts/DoradoLispScan.sh`, and `.regs`/`.rams`
beside the snapshot.

**The fit is the open problem.** With the load ports the core is 38,566
ALMs (92%, +89 over step 1) and seed 3 fails in ROUTING (congestion), where
step 1 routed at 38,477. By module, against the compiled-in core: cpu
+1,820 ALMs, memory section +1,133, io +1,667, mailbox +160. The chain's
bits: cpu 3,534 (BR 896, the per-task files 1,700), io 2,368, memory
section 1,710 (the Pipe 1,400).

**One core, three worlds, in simulation (2026-11-14).** The scan sim built
from the Lisp tree (`obj_hps_lisp_scan`, Trident controller in) runs each
world from its own `.regs`/`.rams`, 30M microinstructions, framebuffer
byte-identical to that world's dedicated core: Lisp 209,165, Smalltalk
112,145 (`SIM_WORLD=smalltalk`), Cedar 167,656. The three register lists are
the same except that Cedar's dump (a `DORADO_NO_DISK` core) has no `disk_*`;
the host loads those as 0. What the builds still differed in was the
wrapper's presentation constants, which a scan core now takes from the
host: mailbox control beat 1 (`world_cfg`: raster 640 x 800 or 1024 x 808,
mouse as terminal motion or absolute, the DDC's first line, the cursor's X
offset; `hps_main.cpp` sets it from `-w`, `Dorado.sv` uses it under
`DORADO_SCAN`). The wrapper change has not been through Quartus yet.

**Register files by load record, and a shorter chain (2026-11-14/15).**
Files of 16 or more entries (BR, the per-task files, the Pipe, the DDC's
NLCB, the disk's format RAM and FIFO; 29 files, 4,880 bits) left the chain
and are written by load records of kind 8/9/10 (`scan_<m>_ld.svh`,
`hps/scan_ld.h`): 7,614 -> 2,734 bits. Then six registers nothing reads
(dispatch and frame counts, the last-reference records) were dropped and
fourteen ints holding a small index or -1 are carried as their low bits and
sign-extended (`gen_scan.py` `SKIP`, `NARROW`; `--regs` refuses a value
that does not fit): 2,162 bits. All three worlds 30M byte-identical in
`sim_hps`; both lock-step suites pass.

**The fit (Lisp tree, seed 3, 44 MHz).**

| core | ALMs | result |
|---|---|---|
| compiled-in checkpoint | 33,744 (81%) | fits |
| registers by scan, 7,614-bit chain | 38,477 (92%) | fits, -0.889 ns |
| + memory load ports | 38,566 (92%) | routing fails (congestion) |
| register files by record, 2,734-bit chain | 38,193 (91%) | routing fails |
| ... MiSTer ALSA, YC, adaptive scaler off | 37,370 (89%) | routing fails |
| ... 2,162-bit chain | **36,900 (88%)** | **fits, -0.995 ns** (TNS -33 ns; HDMI PLL -0.191) |

The cost over the compiled-in core is combinational (about +5,000 ALUTs at
the same register count): the per-bit shift and load multiplexers. Failure
was a local hot spot (peak interconnect 74% around X11-21/Y35-45), not total
area. `Dorado.qsf` in the scan build has `MISTER_DISABLE_ALSA`,
`MISTER_DISABLE_YC` and `MISTER_DISABLE_ADAPTIVE` set (a Dorado has no
sound).

**ONE BITSTREAM RUNS ALL THREE WORLDS ON THE BOARD (mister14, 2026-11-15).**
`_Development/DoradoRtlOne_44.rbf` has no world compiled in; `Scripts/
DoradoOne.sh lisp|smalltalk|cedar` loads it and starts `dorado/dorado-hps-one`,
which loads storage, the `.rams` records and the `.regs` chain and sends the
world's presentation word. Log `/tmp/hps-one.log`.

| world | speed | screen |
|---|---|---|
| Lisp | 17.47 M/s, 1.05x | 209,165 px; typed form evaluated |
| Smalltalk | 21.56 M/s, 1.29x | 124,945 px at 640 x 800 |
| Cedar | 17.53 M/s, 1.05x | 167,656 px; mouse to the Schematic button, click, the sheet fetched over STP and painted (`docs/images/rtl-one-core-cedar-schematic-2026-11-15.png`) |

Each was started over the one before without reloading anything but the
core. The negative slack has not shown itself in these runs; it is one seed.

#### Saving (2026-11-15): the running machine to a snapshot the same core restores

`touch /tmp/dorado-save` on the board (or `DORADO_HPS_SAVE=<microinstructions>,<path>`
in simulation) writes `<snap>`, `<snap>.regs`, `<snap>.rams`;
`DoradoOne.sh <world> <snap>` (`dorado-hps-one -S`, sim `HPS_SNAPSHOT`)
restores it. `hps_save` (`hps/hps_host.cpp`) does, with the CPU frozen
between microinstructions (mailbox command bit 54, `frozen` into `hp_hold`):

| what | how |
|---|---|
| the chain's registers | command bit 55: the chain round once, each bit out of `scan_out` captured and fed back in (`M_SV*`) |
| the register files | the load record that rotates a file one entry, with entry 0 (`fr_q`) as its own value, once per entry (`M_X*`) |
| RM, STK | the host port, `AP_RM_OR` at 0..511 with nothing to OR |
| the Map, the cache's tags | `AP_PEEK` (no storage request) |
| storage | copied from DDR; each valid dirty line read by address and put at its real page, the Map entry marked Ref and Dirty |
| the host's machine | `dorado_machine_snapshot`, with the core's storage |

Not read back: IM, IFUM, ALUFM and the Ethernet receive buffer (their
records are copied from the file the run was loaded from, so a world that
has rewritten its microcode since is not saved correctly), and the Trident
pack in DDR (a restore loads the original pack).

Two faults only the board showed, both fixed in `59551f02a`:
- **STK is its own RAM.** The widened `AP_RM_OR` took STK's value from the
  RM RAM's output and wrote it back, so each save copied RM over STK.
  Smalltalk broke about four saves in ten ("Message not understood"); Lisp
  and Cedar did not show it, and neither did simulation, where the
  restored run was compared with the saving run -- both damaged alike.
  The check now also compares the saving run with a run that does not save.
- **The terminal's message serializer was not in the chain.**
  `ddc_term_msg_*` exists only in the RTL, no checkpoint file names it,
  and a restore resumed it at bit 0 with half a message (HM Table 24) in
  the microcode: a restored Lisp or Smalltalk showed two or three typed
  characters. `gen_scan.py` `EXTRA` adds it (chain 2,194 bits). Other
  RTL-only registers of `dorado_rtl_io` (`ddc_scanline_cd`, the pending
  keyboard and mouse) restart from reset; none has shown a difference.

Simulation, each world: the saving run's framebuffer equals a run with no
save, and the save restored into a fresh core and run on equals both
(Lisp 209,165, Smalltalk 112,145, Cedar 167,380 at 5 M microinstructions).

mister14, the save core (`~/mister/DoradoRtlOneB` on misterubuntu: the
seed-3 tree with `FITTER_AGGRESSIVE_ROUTABILITY_OPTIMIZATION ALWAYS`,
without which the register-save build failed routing at 37,011 ALMs):
37,665 ALMs (90%), worst setup slack -0.902 ns at 44 MHz (the same tree
ranged -0.231 to -1.032 over four builds).

| world | test | result |
|---|---|---|
| Lisp | `(SETQ SAVED 4242)`, save, core reloaded, restore, `(+ SAVED 1)` | 4243 (`docs/images/rtl-one-core-lisp-save-restore-2026-11-15.png`) |
| Smalltalk | five saves of the running desktop, then restore of the last | 124,945 px each time |
| Cedar | save, restore | 167,657 px both |

A save takes about 13 s on the board (131,000 host-port operations for
the Map).

#### Another world, or a save, over the running one: no reload of the core (2026-11-15)

`DoradoOne.sh lisp|smalltalk|cedar [SAVED.snap]` (now in the repo,
`rtl_core/hps/DoradoOne.sh`) no longer loads the bitstream when the core is
there: about 6 s from the command to the new world running, against the
reload's 8 s wait and the load after it. `-r` loads the core anyway.

- **The request.** `hps_start` writes `MB_RESTART` ("RSET") in the control
  beat before it writes anything else -- the old world is still using
  storage -- and waits for the beat to read 0. The mailbox reads the beat
  after each status (`M_RSRD`); on the request it holds the CPU, waits for
  `cpu_quiet` (or 2^24 clocks, for a machine that never comes to rest),
  8,192 clocks more for a disk-engine beat in flight, then takes its own
  reset branch (`restart`). `go` falls, so `core_rst` holds the cpu, the
  memory section and io; the DDR engine is not reset. `M_CLR` clears the
  beat, and the load is the one a fresh core gets. A mailbox still waiting
  for its first go answers the same way.
- **What reset does not clear.** Everything the load does not write is
  either reset (the cache's valid bits, the registers out of the chain) or
  not read before it is written, with one exception found in simulation:
  the framebuffer RAM. A world that draws less than the last one showed the
  last one's pixels there (Smalltalk's 608 columns with Lisp's desktop to
  the right). io now clears it while in reset (`fb_clr`, a free-running
  address; `DORADO_SCAN` builds only) and the mailbox keeps the core in
  reset 2^20 clocks at least (`settle`).
- **The host.** On the board every start is a new process, so nothing of
  the last world's host is left. `dorado-hps-one` exits 3, having written
  only the request, when no core answers in 3 s (`need_core`); the script
  then loads the core and runs it again. The simulation rehearses it in
  one process: `SIM_RESTART=<microinstructions>,<world>[,<snapshot>]`
  (`hps_restart`: the C machine destroyed, the host's counters and hooks
  cleared).

Simulation (3 M microinstructions after the switch, framebuffer against a
fresh start of the same world): Smalltalk -> Lisp, Cedar -> Lisp,
Lisp -> Smalltalk, Lisp -> Cedar all byte-identical; and Lisp saved at 1 M,
that save restored in place at 1.5 M and run 2 M equals an uninterrupted
3 M. Both lock-step suites pass.

mister14 (`DoradoRtlOneB` build f10: 37,889 ALMs, 90%; worst setup slack
-0.805 ns at 44 MHz): Lisp (core loaded) -> Smalltalk -> Lisp -> Cedar ->
Smalltalk -> Cedar, each without a reload: 209,165 / 124,945 / 209,165 /
167,657 / 124,945 / 167,657 px at 1.05x / 1.29x / 1.05x. Cedar's picture
after Smalltalk is whole. `lisp-save.snap` restored over Cedar:
`(+ SAVED 1)` -> 4243. From the MiSTer menu core the script falls back to
loading the core. The bitstream and host before this are on the board as
`DoradoRtlOne_44.save-f8.rbf.bak` and `dorado-hps-one.save-f8`.

**Seeds (f10 RTL, 44 MHz, worst setup slack; all fit, 90% ALMs):** seed 3
-0.805 ns, seed 5 -0.812, seed 7 -1.354, seed 1 -0.829, **seed 2 -0.089**
(37,846 ALMs). Seed 2 is now the build default in `Dorado.qsf` (both SEED
lines) and the bitstream on mister14 (`Dorado_one_final.rbf` in the build
dir; the seed 3 one is on the board as `DoradoRtlOne_44.f10-s3.rbf.bak`).
Checked on the board with it: Lisp (core loaded) 209,165 px at 1.05x,
Smalltalk 124,945 at 1.29x and Cedar 167,653 at 1.05x in place;
`lisp-disk-save.snap` restored over Cedar reads `{DSK}SAVETEST` (4244); a
new save of that, Smalltalk over it, the save restored: reads it again
(4245).

A trap in the script, found on the board: the start runs from a file sh
reads as it goes, so a second start that rewrote the same file made the
first one's shell run the old world again. Each start has its own file now.

#### A save takes the disk with it, and does not stop the machine for it (2026-11-15)

- **The disk.** A save now writes `<snap>.pack` (Lisp, Smalltalk: the
  Trident pack copied out of DDR, where the core's controller writes it) or
  `<snap>.pdi` (Cedar: the Pilot volume the host's germ bridge holds), and
  `hps_start` takes that file in the world's own disk's place when it is
  beside the snapshot. Before this a restore put the saved memory over the
  ORIGINAL pack: a file written since boot was in the world's directory
  cache and not on its disk.
- **Not with a sector in flight.** The Trident controller's sector buffer
  and engine are RTL-only and not saved. After the chain is out the host
  looks at `disk_read_stream_active` / `disk_write_stream_active` in it
  (`scan_reg`); while either is set it lets the machine run 5 ms and
  starts the save again (up to 400 times, then says so and saves anyway).
- **The files are written by a child.** 63 MB of pack takes about 90 s on
  the MiSTer's SD card. `hps_save` copies everything into its own memory
  with the CPU held, forks, and lets the machine go; the child writes the
  files and removes `<snap>.writing` when the last is whole. It ignores
  SIGTERM (a new start's `killall`), and `hps_start` waits for the mark to
  go. The machine is held under 3 s. The simulation writes in line.

Simulation, each world: saved at 1 M, the save restored in place at 1.5 M
and run 2 M, framebuffer equal to an uninterrupted 3 M; the saved
Smalltalk pack and Cedar PDI are byte-identical to the originals (nothing
wrote them). mister14, Lisp: `(WITH-OPEN-FILE (S "{DSK}SAVETEST" :DIRECTION
:OUTPUT) (PRINT 4242 S))`, save (pack differs from the stock one in 87
bytes), Smalltalk started in place, the save restored in place,
`(WITH-OPEN-FILE (S "{DSK}SAVETEST") (+ 1 (READ S)))` -> 4243.

#### Cold boot on the one core: each world boots itself (2026-11-15)

`DoradoOne.sh lisp|smalltalk|cedar cold` starts the world from the moment
its microcode is loaded, not from a saved desktop, and the core does the
rest as the machine did:

| world | the start | then, at the keyboard | to the desktop |
|---|---|---|---|
| Smalltalk-76 | the Alto Executive, off the Trident pack | `Bootfrom xmsmall.boot` | about 30 s; 124,945 px |
| Interlisp-D | the Alto Executive | `lisp.run/M {DORADO}LISP.SYSOUT` | about 5 min: the 4.8 MB sysout over the Ethernet bridge (9,447 packets); 209,122 px |
| Cedar 6.1 | the login (`Please login ... Name:`) | `Guest`, Return, Return | about 10 min at 1.05x: 34 loadees and the installer's BringOvers over STP (3,472 packets in) |

All three were run on mister14. A cold start is an ordinary restore: the
files are `<world>-cold.snap` with its `.regs`, `.rams` and `.pack`/`.pdi`
in `/media/fat/dorado/`, made by `rtl_core/hps/make_cold.sh` from the C
emulator stopped at 33 M BaseBoard cycles (the LoadRam handoff is at
about 31 M; Cedar at 70 M, after the germ is planted at 67.3 M). The
BaseBoard, Initial and its netboot of the `.eb` are still the emulator's;
so is Cedar's germ plant, which hangs on the C CPU's program counter.

What a restore at that point needed, each found by running it:

- **The display tasks' wakeups, held (`ddc_wake_held`, a chain register:
  298 registers, 2,195 bits).** At the handoff Initial has driven the
  terminal task and the loaded world has not initialised its display
  tasks' TPCs; the one core's wake gate is always open, so task 3 ran a
  stale TPC and starved the emulator task (Smalltalk: 0 px). The emulator
  has the same hazard and a predicate for it (`machine_ddc_display_active`
  against its LoadRam baseline). The register, set only by `gen_scan.py
  --regs --cold`, holds the scan-line and word-task wakeups until the
  loaded world's terminal task writes the controller, or a channel has a
  control block, or the raster interrupt is on. A `.regs` file without it
  loads as 0. The C emulator's own resume from so early a snapshot does
  NOT work for Smalltalk (its baseline is a file-scope static, zero after
  a restore; it resumes from 68 M on, when DSemu's display code has run):
  the core is not checked against it there, but against the straight run.
- **The DispM board's presence cells (`dispm_present`, `world_cfg` bit 2,
  Lisp).** With `DORADO_DISPM_PRESENT` the emulator answers the emulator
  task at 360B (all ones) and 361B (type 17B), and the Alto world's
  DisplayInitConfig takes the terminal through AHT/AWT. The core answered
  0, the world initialised DHT/DWT, and the controller still had AHT as
  its terminal task: no picture, and TPC[4] = 177777. A restored desktop
  never asks.
- **Cedar's display task is started by the host.** The emulator never
  wakes Cedar's DHT during the boot to the login -- it paints the login
  from storage -- and on the board the picture is the display microcode's.
  For a germ world with the hold set, `hps_poll` looks a field apart for
  Cedar's CSB at LONG[420B] naming a DCB with the long-bitmap seal
  (177423B), or the emulator's Alto display-list predicate
  (`dorado_machine_alto_display_active`, new in machine.c), and wakes the
  terminal task with `AP_WAKEUP_OR`; its first output ends the hold.
  **Only for a germ world**: the same wake in the Lisp world, before the
  presence cells were in, woke task 4 at TPC 177777 and hung the core.
- **The receiver's buffer limit.** `eth_world_rx_words` is the Alto
  world's posted input buffer size (EICLOC), which the emulator refreshes
  from storage and the core cannot: restored as 0 it refused every
  packet (`No such host: DORADO`). A cold `.regs` has 177777 and the host
  holds a packet that would not fit (`ethernet_bridge`, from the
  emulator's own `world_rx_words`).
- **A save reads IM, IFUM and ALUFM (`AP_PEEK` with `ap_wd[3]`).** A save
  used to copy them from the loaded `.rams`. `lisp.run` loads the Lisp
  microcode after the start, so a save of a cold-started Lisp came back
  with the Alto emulator's control store alone: storage whole (the host
  drew the desktop from it), a white screen. The memories have a read
  port of their own at `ap_va` -- nothing is added to the fetch's or the
  IFU's addresses -- and the processor answers in `C_APW`. In simulation a
  save of the Lisp desktop has the 4,096 + 1,024 + 1,024 + 16 records of
  the loaded file, and a Smalltalk cold start saved 5 M microinstructions
  in and restored goes on to the Executive. On mister14: Lisp cold-booted,
  `(SETQ SAVED 4242)`, saved, Smalltalk started over it, the save
  restored: `(+ SAVED 1)` -> 4243 at 1.05x, the picture whole.

**The fit with all of this (build f13, 44 MHz):** seed 2 -1.588 ns (37,864
ALMs), seed 3 **-0.351 ns** (37,894 ALMs, 90%; 403 of 553 RAM blocks -- the
read ports are a second copy of IM and IFUM). The seed is not a property
of the design: seed 2 was the best of five for f10 (-0.089) and is the
worst here. **At -1.588 the HDMI picture has colour fringes** (107,991
pixels of a Lisp screenshot not grey; 0 with seed 3) while the machine
itself ran correctly -- the first build in this series whose negative
slack showed. Seed 3 is on mister14 (`Dorado_one_f13_s3.rbf`); the ones
before are beside it as `DoradoRtlOne_44.f12.rbf.bak` (no control-store
read-back) and `.f10-s2.rbf.bak` (no cold boot), with their hosts
`dorado-hps-one.f12` and `.f10`. Checked with it: the cold-booted Lisp's
save 209,618 px, the stock Lisp 209,165, Smalltalk 124,945, Cedar
167,657, Smalltalk cold to the Executive 2,697, each in place.

`lisp.run` is served the root's `LISP.SYSOUT` (Lyric, 27-Apr-87), not the
full-library `Released-Full.sysout!2` the stock desktop was made from.
Cedar's cold boot ran the served user profile and stopped in it at
`Run BiScrollersImpl` with an UnboundProcedure signal in an event viewer
beside a live CommandTool; whether the emulator does the same with this
tree was not checked.

#### The screen where nothing is drawn, and the MiSTer menu (2026-11-16)

- **Stale pixels.** Found on the board: the Alto Executive, then `netexec`
  -- the Net Executive's two header lines and prompt came up with the
  first Executive's text still between them (2,252 px on HDMI against
  1,484 in the host's rendering of the display list). The framebuffer
  stage follows the C DDC model, "pixels at or beyond width are left as
  they were", and a line no control block covers is never written; a
  monitor shows background in both places. Under `DORADO_SCAN`,
  `dorado_rtl_io` keeps a table `ln_t` of each line -- {blank, the field
  it was last drawn in (3 bits), how far (w)}. A line's job clears from
  its width to the old w (`P_TAIL`); a walk of the table while the pixel
  engine is idle clears whole (`P_BLANK`) a line not drawn for four
  fields. A cursor row counts as drawing, to the cursor's end. Nothing is
  cleared that a job will not redraw, so the three worlds' reference
  pictures are byte-identical in simulation and the clock counts are the
  same to the clock (no new holds of the CPU). On mister14 the same
  `netexec` now gives 1,476 px on HDMI and 1,476 from the host.
- **The menu.** With `DORADO_SCAN` the core's OSD has World (Lisp,
  Smalltalk, Cedar), Start from (Saved desktop, Cold boot), "Start that
  world" and "Save this machine". The two actions are toggles in the
  status block (beat 10 bits 31:26: start, save, cold, world[1:0]);
  `dorado-hps-one` sees a toggle turn and runs `DoradoOne.sh WORLD
  [cold]`, or a save as `/tmp/dorado-save` asks for one. Tried on the
  board: Cedar started from the menu over the Net Executive (167,657 px),
  and a save from the menu wrote `dorado-save.snap` with its `.pdi`.

**The fit (f14):** seeds 3, 2, 5, 1: -0.528, -0.499, -0.551, -0.569 ns,
38,049 ALMs (91%) with seed 3, which is on mister14
(`Dorado_one_f14_s3.rbf`; the f13 pair is beside it as
`DoradoRtlOne_44.f13-s3.rbf.bak` / `dorado-hps-one.f13`). Lisp 209,165 px
at 1.05x, Smalltalk 124,945 at 1.29x, Cedar 167,657 at 1.05x, Smalltalk
cold 2,697, no coloured pixels.

**2026-11-17 (f16 seed 3 on mister14, -0.687 ns, 38,052 ALMs; f14 kept as
`DoradoRtlOne_44.f14-s3.rbf.bak` / `dorado-hps-one.f14`):**
- **A Cedar cold boot stuck about one start in two** (0 pixels, no packet,
  disk requests stopping after about 13,000 host operations; 2 of 6 and 2
  of 4 in two series). The germ's polled IOCB arm (`machine.c`, "PDI germ
  polled IOCB") stored `next` into the CSB head from a read made when the
  completion began; the germ, running beside the host, chained the next
  IOCB in between and waited for ever. The same race as 9d0239812 in the
  other arm, the same cure: read `iocb.next` again after the store. 36 of
  36 cold starts since, one of them saved by the re-read
  (`DORADO_DISK_IOCB_TRACE=1` prints "chained ... during its completion").
- **The menu resumes its own save.** Start from is `O[8:7]`: Saved desktop,
  Cold boot, My save (osd bit 0 = `status[8]`, bit 3 = `status[7]`). "Save
  this machine" writes `/media/fat/dorado/<world>-user.snap` (+ `.regs
  .rams .pack|.pdi`, about a minute for the pack on the card), and My save
  starts `DoradoOne.sh <world> <that file>`; with no such file it logs so
  and starts nothing. Tried: Lisp, `(SETQ MYSAVE 4242)`, save, My save,
  Start: `(+ MYSAVE 1)` -> 4243. `touch /tmp/dorado-save` still writes
  `dorado-save.snap`. On f16: Lisp 209,165 px 1.05x, Smalltalk 124,945
  1.29x, Cedar 167,657 1.05x, no coloured pixel on HDMI.
- **The served tree is looked up without regard to case.** A Cedar cold
  boot on the board stopped in its profile at `Run BiScrollersImpl`
  (UnboundProcedure, after "Failed to find load file for PopUpButtons");
  the emulator, same files, ran the profile to its end. The log had it:
  `STP_MISSING CedarChest6.1/Top/PopUpButtons.df` -- the file is
  `PopUpButtons.DF`, found on the Mac's caseless file system and not on
  the MiSTer's Linux. `ethernet.c eth_ftp_fold_case`: when the path as
  asked is not there, each component is matched caselessly. With it the
  cold boot, logged in as Guest, runs the whole profile to the `%` prompt
  with the Schematic/Moon/Memo buttons, as the emulator does (167,107 px
  against the emulator's 166,684; the only `STP_MISSING` left are two
  fonts the tree does not hold, Helvetica6BI/7BI). (The stop was recorded
  on 2026-11-16 as open.)
- **The "Guet" stall (closed 2026-11-17, on f18):** a Cedar cold boot logged
  in as "Guet" (no profile of its own) stopped for ever in the middle of a
  file, 2 of 4 starts (`InterpreterTool.bcd`, `IOPrintImpl.bcd`). The log:
  the server had sent the whole reply (`tx_next` past the completion mark),
  the guest's last acknowledgement was eleven Pups back, and the guest sent
  an empty AData probe every 40 s, each answered with an Ack. A Pup of the
  reply's tail was lost at the guest's receiver; the tail's fingers are
  plain Data, so the guest was never asked to acknowledge them, no
  duplicate Ack came, and the retained-history replay never started.
  `ethernet.c eth_ftp_handle_packet`: an empty AData arriving while the
  server is idle with unacknowledged stream replays from the last Ack. 6 of
  6 cold starts since run to the `%` prompt (183 files, 12-13 minutes,
  about 167,100 px); `make test` and `verify-cedar-desktop` pass. The
  recovery waits for the guest's probe, so a lost tail costs up to 40 s.
  The emulator, same login, shows no stall (it does not lose the Pup).

**The host no longer holds the Trident pack (2026-11-18, on mister14).**
On a Lisp or Smalltalk start the host used 252 MB of the board's 491 MB,
with about 30 MB left available; the first host that made its own scan
chain was killed for it. The cause was the emulator library's pack
structure, which holds every sector at the largest size any geometry has:
2,074 bytes for a 532-byte Diablo-on-Trident sector, 245 MB for a 63 MB
pack. The pack is the core's (its controller reads and writes the sectors
in DDR), so `hps_host.cpp` now reads the file into DDR a sector at a time
(`pack_open`, and the load in `hps_start`) and gives the C controller a
pack with the right geometry and no sectors, which is all
`sync_disk_body.inc` reads. A save copies the beats out of DDR while the
machine is stopped (63 MB, freed once the writing child has its copy) and
`pack_write` puts them in the file layout.

- Measured on Lisp: host resident 252 MB -> 14 MB; board memory available
  31 MB -> 287 MB.
- Simulation: Lisp and Smalltalk cold starts make the same `.rams` and the
  same registers as before.
- mister14: Lisp 209,165 px, Smalltalk 124,945, Cedar 167,653, Smalltalk
  cold 2,697; a save of the running Lisp (`touch /tmp/dorado-save`) wrote a
  63,105,450-byte `.pack` and the machine restarts from that save.
- The host before this is `dorado-hps-one.f18c` on the board.

**The host makes the scan chain itself (2026-11-17, on mister14).** A
snapshot with no `.regs`/`.rams` beside it (the three stock desktops, the
three cold starts) gets both from the host: `hps_host.cpp derive_regs` runs
the copy the simulation's checkpoint dump uses over the restored C machine
into a name table, and packs it as `load_scan_regs` packed the files. The
copy is one text for both: `rtl_core/sync_cpu_body.inc`,
`sync_display_body.inc`, `sync_disk_body.inc` (included by `cosim_sync.h` /
`sim_cedar.cpp` with `R_CPU` etc. naming the Verilated registers, and by the
host with them naming table entries), `uinstr_pack.h`. `-C` (`cfg.cold`;
`DoradoOne.sh WORLD cold` passes it) makes it a cold start: `ddc_wake_held`
1, `eth_world_rx_words` 177777. A save keeps its own files (the core's
registers read back). `cfg.derive` is on in `hps_main.cpp`; in `sim_hps`,
`HPS_DERIVE=1` (`HPS_COLD=1`); `DORADO_HPS_WRITE_DERIVED=1` writes
`<snapshot>.regs.made` / `.rams.made` to compare.
- Checked: for the lisp, smalltalk and cedar cold snapshots the made load
  records are byte-identical to `gen_scan.py --rams`' and every register of
  the chain and the register files equals the `--regs` file's in the chain's
  bits; `sim_cedar`'s dump is byte-identical to before; `make test` and
  `PIPE=1 test` pass. On mister14 with the offline files moved to
  `dorado/retired/made-offline/`: Lisp 209,165 px, Smalltalk 124,945, Cedar
  167,657; cold Lisp at the Executive (2,114), Smalltalk (2,697), Cedar at
  the login (28,477); `lisp-user.snap` (its own files) 209,722.
- `hps/make_cold.sh` now only bakes the snapshots and copies the disks: no
  native bake, no simulation run, no `gen_scan.py`. The stock desktop's
  storage is the snapshot's own (`lisp-lyric-full.storage` is not read).
- **The board is short of memory with a pack world.** The first build was
  killed by the kernel (out of memory) right after the pack load, Lisp and
  Smalltalk both: the host stands at 254 MB of the board's 491 with 29 MB
  left available, and the name table (maps, a node an array entry) went over.
  The table is vectors now and the memories do not go through the name map.
  The 254 MB is there without it (the pack in memory); anything else that
  grows in the host on a Lisp or Smalltalk start meets the same wall.
  `/tmp` is memory too (the Cedar tree is 68 MB there).

**Timing closed (2026-11-17, build f18, on mister14):** worst setup slack
0.000 ns at every corner, 43 MHz, 37,955 ALMs (91%), seed 3. Lisp 1.02x,
Smalltalk 1.26x, Cedar 1.03x (1.14x during a cold boot); Cedar cold-boots to
its login. The file on the board is still named `DoradoRtlOne_44.rbf` (the
start script names it); f16 is beside it as `.f16-s3.rbf.bak`.
- f16's report (`paths16.tcl` in the build directory; the reports the entry
  below quotes were of the 42.5 MHz database) had 70 failing endpoints in
  three chains. Three cuts, none of which changes a clock count in
  `sim_hps` (Lisp 79,326,240 clocks for 30 M microinstructions before and
  after; Lisp, Smalltalk and Cedar framebuffers byte-identical to
  `RTL_OV_HOLD`; `make test` and `PIPE=1 test` pass):
  - **the prefetch buffer's snoop** (52 of the 70: `m_va` -> the section's
    hit test -> `snoop_wr` -> `pf_v1` -> `pf_st` -> the peek issue,
    `mem_req2_va`). The snoop now drops the words after the buffer's
    sequencing, which acts on the drop the next clock; a dropped word0 takes
    its IFUM entry with it.
  - **`x_md_eff`** read the Md file through the task select; the detached
    reference is the emulator's, so it reads task 0's (and when the task has
    changed, the word at issue stands, as the clocked copy already did).
  - **`pipe_push_ok`** compared the chosen Pipe entry (asrn or proc_srn,
    chosen by the hit test); it compares the two registers and takes the
    choice last.
- With them at 44 MHz (f17): -0.455 ns, 6 failing endpoints (was -0.687,
  70), HDMI clean. What is left at 44 MHz is three deep chains, each the
  whole of a clock: a forwarded T -> shifter -> A -> the address adder -> the
  read-ahead's row select (the tags are 4,608 registers behind a 64:1
  select, ~6 ns) -> tag compare -> `sp_hit`/`sp_wp`; `pf_pcf` -> `nx_fast` ->
  the next address -> `p_fwd`; `mem_req_va` -> the hit test -> the Pipe
  entry -> `pipe_pipe4_errors`. 43 MHz (0.53 ns more) closes all three. To
  go back to 44 MHz the first needs the cache tags in a RAM, not registers.

**Not done:**
- (Superseded by the block above.) Timing: about -0.5 ns at 44 MHz whatever the seed (f14). Not closed; a
  build at -1.6 showed on the screen. **A lower clock does not close it by
  itself:** the same RTL at 42.5 MHz (0.80 ns more a cycle, seed 3) fit at
  -0.308 ns -- the fitter gave back most of the room -- so the build stays
  at 44 MHz. The worst paths are one chain in the processor: `res_valid`
  (Md arriving this clock) -> `x_md_eff` -> T -> the shifter -> ALU A ->
  the address adder -> the memory section's request select -> `pf_v0` ->
  `mem_req2_va`; then `mem_req_va` -> `pipe_pipe4_errors`. Closing it
  means taking a clock where Md is forwarded into a reference's address
  (`~/mister/DoradoRtlOneB/paths.tcl` writes the report).
- The start of a cold boot is the emulator's: the BaseBoard, Initial and
  the EFTP of the `.eb`, and Cedar's germ plant, are in the snapshot.
  **Tried (2026-11-16), Smalltalk, snapshots at 12 M, 20 M and 28 M
  BaseBoard cycles (Initial substituted at 11.3 M, identity-map shim at
  24.3 M, checksum-and-load at 31.0 M, world at 31.3 M):** the core runs
  Initial for 90 M microinstructions without a fault and stays in it --
  one packet out, one in, TPCs still in 6000-7700. The C emulator resumes
  from all three, and the RTL is right in lock-step from 28 M through the
  load (`obj_cedar/Vdorado_rtl_top 1500000 SNAP PACK`, PASS) -- with the C
  machine's devices. What host-only lacks: the emulator stages the whole
  boot file as ONE receive buffer of 16,744 words (the dump prints
  `rx 16744/16744`), and a receive fill of the core is 4,096 words; and
  the C machine pokes RM 31-35 on the way. So Initial's net load on the
  core needs a receive fill that continues (the buffer refilled as the
  input task drains it, with no end of packet between) and those RM
  words through the mailbox -- or the real EFTP exchange, packet by
  packet, which `dorado_rtl_bootsrv` does for the Galaxian configuration.
  **And more than that (read 2026-11-17, `machine.c` about 3540-3620):**
  the C machine carries Initial through the load with assists keyed on
  the PC of every instruction fetch, none of which a host that only polls
  can give: the boot parameter planted in STK[1..3] (file number,
  BootParameterSeal 056623, checksum) while the PC is in 6170-6217 or
  6406-6443; RM 32-35 held at 177777 (the boot keys "up": the terminal's
  back-channel is not modelled) in 6407-6431; the identity map of
  installed storage at 6222; at the hand-over the standard ALUFM put
  back, the junk timer quieted and pending wakeups dropped. On the core
  each would be a compare on the PC with a write of STK, RM, the map or
  ALUFM behind it -- the emulator's stand-ins moved into the FPGA, not
  the Dorado's own power-on. Not worth building as it stands: the cold
  start from just after the load already runs everything the world does
  itself. It becomes worth it when the stand-ins go (the terminal's boot
  keys and parameter through `dorado_terminal`, PresetMap run for real).
- Read-back of the Ethernet receive buffer.
- A disk write on the board has only been tried in Lisp. Cedar and Smalltalk saves (with their `.pdi` / `.pack`) were restored in place on mister14 and took input -- Cedar: a click in the CommandTool set the caret and typed text went in there; Smalltalk: the pointer and a click in the Classes browser filled its first pane, 124,945 -> 126,636 px -- but nothing had written their disks.
- (Done 2026-11-17, see "The host makes the scan chain itself".) The host
  made the stock `.rams`/`.regs` offline from a simulation dump.
- A save file is tied to the host that wrote it (the ARM's 32-bit snapshot
  does not restore in the 64-bit simulation).
- (Done 2026-11-17.) The per-world bitstreams are retired on mister14:
  the six cores (`DoradoRtlCedarPipe_*`, `DoradoRtlLisp_44`,
  `DoradoRtlLispScan_44`, `DoradoRtlSmalltalk_44`), their start scripts
  (`DoradoCedar.sh`, `DoradoLisp.sh`, `DoradoLispScan.sh`,
  `DoradoSmalltalk.sh`), their hosts (`dorado-hps`, `-st`, `-scan` and the
  kept variants) and the loop/UART scripts that drove them are moved, not
  deleted, to `/media/fat/dorado/retired/{cores,scripts,hosts}` with a
  README saying where each goes back. `Scripts/DoradoOne.sh` and the menu
  of the one core are the only way in now. After the move, from a fresh
  core load: Lisp 209,165 px, Smalltalk 124,945, Cedar 167,657. The
  per-world build paths (`fpga/mister_rtl/build.sh CORE=...`, the
  `DORADO_CEDAR/SMALLTALK/LISP` macros, `sim_cedar`) stay in the tree:
  the lock-step and the record dumps (`make_cold.sh`) use them.
