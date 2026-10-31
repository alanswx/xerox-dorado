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
| [`sim_fpga.cpp`](file:///Users/alans/dev2/Dorado/rtl_core/sim_fpga.cpp) | FPGA-configuration testbench | No C model, no pokes: power-up images, RTL boot server, RTL assist (`make fpga-sim`). |
| [`tools/make_boot_roms.py`](file:///Users/alans/dev2/Dorado/rtl_core/tools/make_boot_roms.py) | ROM generator | `roms/bol.hex` from ethernet.c's `eth_bol_loader`, `roms/bootfile.hex` from the boot file. |
| [`Makefile`](file:///Users/alans/dev2/Dorado/rtl_core/Makefile) | Build | `make`, `make test`, `make init`, `make fpga-sim`, `make fpga-sim-ddr`, `make lint`. |
| [`dorado_rtl_storage.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_storage.sv) | Storage (block RAM) | The munch port: one aligned 16-word munch per request, word mask on writes (HM section 5). |
| [`dorado_rtl_storage_ddr.sv`](file:///Users/alans/dev2/Dorado/rtl_core/dorado_rtl_storage_ddr.sv) | Storage (DDR3) | The same port on the MiSTer DDRAM interface (`EXT_STORAGE`): 4-beat burst reads, single-beat writes, region cleared after reset. |
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
