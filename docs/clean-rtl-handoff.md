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
