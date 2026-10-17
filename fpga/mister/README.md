# Xerox Dorado for MiSTer

The Dorado boards, generated from PARC's Sil wire lists, inside the MiSTer
framework. **The Alto Executive accepts keyboard commands on MiSTer14**
(October 16, 2026). The full Verilator simulation uses an assisted Initial/AEmu
boot, a file-backed serial Trident medium, and a read-only renderer of the
guest's display list. The test types `foo` + Return, gets the expected
`There is no subsystem named foo.` response, and returns to `>`.
[Screenshot](../../docs/images/rtl-alto-executive-input-2026-10-16.png).
[Hardware screenshot](../../docs/images/rtl-mister14-executive-input-2026-10-16.png).
The FPGA continues a saved state from that RTL boot, drives the actual DispY
video pins, and uses an HPS-backed serial Trident disk. On hardware, `foo` +
Return prints the expected error and another prompt. This is a checkpoint
start; the separate BaseBoard cold-boot path remains incomplete.
Current evidence is in [the Verilog handoff](../../docs/verilog-handoff.md).

```sh
sh fpga/mister/build.sh [TEMPLATE_DIR] [OUT_DIR]
cd <OUT_DIR> && quartus_sh --flow compile Dorado
```

The build script assembles the project using the template's `sys/`, PLL and
DE10-Nano pin assignments. It copies the Dorado wrapper into the output directory and references the
source RTL in the checkout.

## Build the Executive checkpoint configuration

Keep the ready checkpoint and its matching `obj_executive` build together.
Export does not rebuild that model, because that would invalidate the saved
state fingerprint. If either is missing, first run `make -C
verilog/verilator executive-checkpoint` to recreate the assisted RTL boot.

```sh
make -C verilog/verilator executive-mister-export
DORADO_QUARTUS_JOBS=4 sh fpga/mister/build.sh \
  ~/mister/Template_MiSTer ~/mister/DoradoExecutive \
  build/rtl-executive/fpga_checkpoint_hps
cd ~/mister/DoradoExecutive
export PATH="$HOME/intelFPGA_lite/17.0/quartus/bin:$PATH"
quartus_map Dorado
quartus_fit Dorado
quartus_sta Dorado
quartus_asm Dorado
```

Run export from the repository root (the make target does this). For a
remote build, copy `fpga/mister`, `verilog/generated`, `verilog/cells`,
`verilog/vendor`, `verilog/proms`, and the generated checkpoint directory,
keeping their paths relative to the checkout root. `build.sh` references
those sources; the assembled project is not a standalone source archive.
It supports both `Template.*` and `mycore.*` framework layouts.

The exporter reads actual RTL register/RAM state, derives native C++ offsets
and Verilator hierarchy automatically, and generates per-instance power-up
initializers. It does not use C-emulator CPU state. The first hardware
configuration retains the Alto's 64K physical words; out-of-range storage
accesses are rejected and asserted in simulation. The main core runs at
33.333 MHz with four fabric samples per clock, or 4.167 M
microinstructions/s (one quarter of the Dorado's rate).

`dorado_pack_hps.sv` supplies the real serial Trident stream from a mounted
image. A 16 KB window caches a whole track, including neighboring bytes at
unaligned track boundaries. A seek flushes complete checked writes before
loading the next track; the CPU and drive pause during HPS transfers.
`make -C verilog/verilator hps-pack-test` compares 11,024 serial bits against
the qualified file-backed medium and checks a read/write/seek loopback.

**Always pair this checkpoint with its exported `executive.pack`.** The
exporter applies its saved disk-write overlay to a disposable copy of the
base pack. The installed restart script replaces the working image from an
unmounted seed before reloading the RBF. Reloading memory against a changed
working disk is not a clean restart. Framework reset pauses the continuation;
reloading the paired checkpoint restores the initial state.

Two FPGA memory details matter: asynchronous SRAM slices need the explicit
MLAB hint, and a RAM output latch initialized to one prevents Quartus 17
from inferring block RAM. The exporter preserves that first-cycle value
with an external valid mux while leaving the RAM output register zero.

## Recorded checkpoint/HPS fit

Quartus Prime Lite 17.0.2, 5CSEBA6U23I7, **2026-10-16**:

| Resource | Used / available |
| --- | --- |
| ALMs | 23,733 / 41,910 (57%) |
| Registers | 21,936 |
| RAM blocks | 445 / 553 (80%) |
| Block memory bits | 2,336,070 / 5,662,720 (41%) |
| DSP blocks | 35 / 112 (31%) |

The fit reported 48.06 MHz core Fmax against the 33.333 MHz clock. Reported
setup/hold checks have positive slack after the scoped hold-only exception
in `checkpoint_timing.sdc`; the framework also reports unconstrained I/O paths.
ContA a04 captures its RAM write address only while write is disabled, so
the same-edge address-to-write-port hold path is inactive. Setup remains
checked. The original report and failing paths are retained for review.
Reports and hardware-validation metadata are in
[`reports/2026-10-16-executive/`](reports/2026-10-16-executive/).

## Installed hardware test

On MiSTer14 (`10.0.2.14`), run `/media/fat/Scripts/DoradoExecutive.sh` from
the Scripts menu or SSH. It creates a fresh working pack and loads
`_Computer/DoradoRTL_20261016/Executive.mgl`. The local RBF is
`build/rtl-executive/DoradoExecutive_20261016.rbf` (SHA256
`3b86d7136d576c7c7a2b9c0b83082e2ed1646aa30b4e95d69ebdc5632dcc32cf`).

The display is interlaced. MiSTer's scaler discards one field when its output
height is below the full input height, making text incomplete. The installed
`[DoradoExecutive_20261016]` INI section uses `video_mode=8` (1080p) and
`vsync_adjust=0`; other cores retain their settings. The original INI is
preserved as `/media/fat/MiSTer.ini.before-dorado-20261016`.

## Connected hardware and remaining work

- **Video:** the checkpoint continuation drives the real DispY video pins,
  a phase-accumulator pixel clock, pixel enables and interlaced field IDs.
  Its Alto display uses a 3:4 portrait aspect ratio. Two clean starts passed
  hardware field-order and input checks; 2,330 text pixels match the reference
  exactly. DispM's separate colour screen is absent.
- **Keyboard:** PS/2 events feed the Alto matrix and terminal serializer.
  Character echo and command completion are validated on MiSTer14 through
  the remote virtual USB keyboard. Mouse and keyset inputs idle released.
- **Storage:** the checkpoint build uses 64K physical words in MSA block RAM,
  with the small asynchronous register files in MLABs. External SDRAM is
  unconnected; a larger Dorado configuration needs another storage solution.
- **Disk:** the checkpoint build has HPS reads and checked serial writes,
  actual unit selection, index and EndOfCyl status. The older BaseBoard
  configuration still has the blank-medium stub and is not a cold-boot gate.
- **Reset:** the checkpoint's OSD control pauses execution. A clean restart
  reloads its saved RTL state with a fresh matching working disk. The
  separate BaseBoard boot-button route remains incomplete.

## Run the Executive in Verilator

From the repository root, with Verilator, SDL2 and a C++20 compiler installed:

```sh
make -C verilog/verilator executive-checkpoint  # slow fresh RTL boot; save at 390M samples
make -C verilog/verilator run-executive-ready  # open the saved Executive in SDL
```

The current workspace already has the ready checkpoint at
`build/rtl-executive/alto-executive.rtl`. Key events follow the same PS/2
decoder and terminal serial return channel as the MiSTer wrapper. Typing is
slow in Verilator; events are paced in emulated time so the OS can scan them.
The checkpoint includes actual RTL register/RAM state and volatile serial
disk writes. It belongs to the exact generated model build and is not a
C-emulator snapshot. Recreate it after RTL changes.

The October bring-up repaired storage clock wiring, DRAM write capture,
cache configuration/refill, IFU capture, and serial disk writes. The final
boot blockers were a transient stale cache-hit write and an odd-bank word
counter taking the following address when a memory hold ended. Regressions
cover the failing handoffs at four and sixteen fabric samples per clock;
the complete fresh boot is qualified at four. The HPS bridge replaces the simulation's file-backed medium for the
checkpoint hardware configuration.

## Completion sequence

1. Qualify video, physical keyboard input, an Alto application launched from
   the pack, and sustained disk I/O on hardware.
2. Finish disk persistence: flush a final dirty track without requiring a
   seek, handle errors/remounts, and verify a guest-created file after restart.
3. Supply a local boot server on the MiSTer ARM and a buffered Ethernet
   interface to the RTL. Reuse the C emulator's Pup/EFTP server code without
   running its CPU emulator. A separate Linux service owns the server and
   files; Main supplies packet transport where necessary. No external
   server should be required.
4. Integrate BaseBoard boot/reset with the working machine and medium.
   Remove the testbench checksum feeder, preloaded world/table handoff, and
   saved processor state. Acceptance: power-up to Executive with the
   external network disconnected.
5. Add mouse input and external SDRAM-backed physical storage. Keep the
   Dorado cache, address map and memory-controller behavior around it.
6. Extend the local ARM service with the required file protocols, then
   qualify Mesa microcode, the matched germ, Pilot disk I/O and Cedar using
   the real controllers. The C emulator's Pilot bridge is reference behavior,
   not an implementation to put in Main.
7. Improve timing and reduce the chip models' sampling overhead, retaining
   boot/input/storage/display gates at each step. Full speed at the present
   eight fabric clocks per microinstruction requires 133.333 MHz, above the
   recorded 48.06 MHz core Fmax. A faster PLL alone cannot achieve it.

RTL owns instructions, task scheduling, registers, microcode memories,
cache/map/storage control, device controllers, video generation, terminal
protocol and BaseBoard firmware execution. Main owns menus, USB event
collection, core/image loading and backing-file block transfers. The local
ARM service supplies external boot/file-server behavior through packet
transport. Those host interfaces must not execute guest instructions or
complete guest I/O behind its hardware controllers. Reuse existing MiSTer
facilities where they provide the required interfaces.

The server needs both Initial's microcode-world exchange (Pup 0264/0265)
and the later Alto Mayday/EFTP exchange. Extract packet handling from
`dorado/src/ethernet.c`; its current receive queue also carries controller
status and a dummy wire CRC, which must not be passed through as successful
RTL reception. The FPGA link adapter must present properly framed wire data
to DskEth so its own receiver checks and status path are exercised.

## Clock rate

`SYSPER` is fabric samples per **30 ns clock**, with two clocks per
microinstruction (Hardware Manual section 2.3):

```
instructions/second = clk_sys / (2 * SYSPER)
```

The checkpoint wrapper uses a 33.333 MHz PLL and SYSPER=4: nominally
**4.167 M microinstructions/s, 0.25x the original Dorado**, with additional
pauses during host disk transfers. Short unit tests at a ratio do not
establish full boot at that ratio; the fresh-boot reference uses SYSPER=4,
with focused timing regressions also checked at 16. The external drive
counts samples per microinstruction, so it receives `2 * SYSPER`.
