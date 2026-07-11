# Running the emulator — software combinations and how to load them

A practical runbook: the software we have, which combinations work, and the
exact commands to load each. For the *survey* of what exists in the CHM
archive see `docs/dorado-software.md`; for the live bring-up state see
`docs/CONTINUE-HERE.md`.

All commands are run from the `dorado/` directory after `make` (headless
binary `build/dorado`) or `make sdl` (interactive SDL binary
`build/dorado-sdl`). Paths beginning `../chm/...` are relative to `dorado/`.

---

## The two boot paths at a glance

The boot chain is the same up to the point where emulator microcode is
running; then it forks:

```
BaseBoard ROM ─► Bootstrap ─► Initial ─► [microcode world] ─► [OS / app]
 (6502)          (IM 7700+)   (loads     ┌─ AEmu world ──► Alto sw boot (EFTP)   ◄── WORKS
                              the world)  └─ Cedar world ─► Pilot germ ─► OS boot  ◄── in bring-up
```

| Path | Microcode world | Second stage | Status |
|---|---|---|---|
| **A. Alto-on-Dorado** | `worlds/aemu.eb` (AEmu) | Alto boot file over EFTP | **Working** — Galaxian 121553 px, NetExec menu renders |
| **B. Cedar/Pilot germ** | `CedarDorado.eb` | `Dorado.germ` → Pilot disk | **Boots Cedar 6.1 to the SimpleTerminal login prompt; keyboard input works** |
| **C. Alto/Mesa** | `AltoMesaDorado.eb!2` | `MesaNetExec.boot` over EFTP | **Boots the Mesa Network Executive to an interactive `>` prompt** — herald + live clock + cursor; typed keys echo (the prompt appears ~155M cycles in) (`./build/dorado-sdl --eb '../chm/dorado/AltoMesaDorado.eb!2' --eftp '../chm/bootfiles/MesaNetExec.boot!1'`) |

Both stages share an in-process fake Pup/EFTP boot server (`src/ethernet.c`)
so no real network is needed: `--eftp PATH` registers the boot file the
server streams when the running software asks for it.

---

## Software inventory (what we have locally)

### Microcode worlds (`--eb`)
| File | What it is | Use |
|---|---|---|
| `dorado/worlds/aemu.eb` | The **Alto emulator** on Dorado (built locally from `chm/dorado/AEmu.mb!2` by `make worlds`) | Path A — runs Alto software |
| `chm/dorado/AltoMesaDorado.eb!2` | Alto/Mesa world (1984) | Path A variant; a regression-gate world |
| `chm/dorado/CedarDorado.eb!6` | **Cedar/Mesa microcode** (17-May-1984; the build shipped with Cedar 5.3/6.0/6.1) | Path B — runs the Pilot germ |
| `chm/dorado-mc-alt/CedarDorado.eb-1986.3` | Cedar microcode, May-1986 | Path B (newer; behaves identically on the germ so far) |
| `chm/dorado-mc-alt/CedarDorado.eb-1987.4` | Cedar microcode, Feb-1987 | Path B (newer) |
| `chm/dorado/Initial*.eb` | Bootstrap (Initial) microcode stages | Used internally by the full-boot chain |

### Pilot germs (`--germ`, Path B only)
The germ is the Pilot bootstrap that the Cedar microcode loads into VM. It
then loads the actual OS boot file. **Germ and microcode versions must
match.**

| File | Version | Status |
|---|---|---|
| `chm/cedar/germ/Dorado.germ!4` | Dec 1983 (Cedar ~5) | **Mismatched** with the 1984 microcode → faults early (`0o27132`) |
| `chm/cedar/germ-alt/Dorado.germ-6.1.6` | **Cedar 6.1, 1986** | **Current debug target** — the matched pair with `CedarDorado.eb!6`; gets furthest |
| `chm/cedar/germ-alt/Dorado.germ-6.0.6` | Cedar 6.0, 1985 | Behaves like !4 (same early fault) |
| `chm/cedar/germ-alt/Dorado10.germ-6.1.5` | Cedar 6.1 (Dorado10) | Same early-fault family |
| `chm/cedar/germ-alt/Dorado.germ-5.3.1` | Cedar 5.3, 1984 | Same early-fault family |

### OS / application boot files (`--eftp`)
Streamed by the EFTP server as the "next stage." Two families:

**Alto boot files (Path A — these WORK):** `chm/bootfiles/*.boot` and
`*.BOOT` — `NETEXEC.BOOT!8` (the network executive menu), games
(`Galaxian.boot!1`, `Invaders.boot!1`, `MissileCommand.boot!1`,
`PinBall.boot!1`, `AstroRoids.boot!1`, `Boggs.boot!1`, `Reversi.boot!1`, …),
and tools (`CRTTEST.BOOT!1`, `DMT.BOOT!22`, `KeyTest.boot!1`,
`MadTest.boot!1`, `FTP.boot!1`, `EtherWatch.boot!1`, …).

Three caveats, all reproducing real-Dorado behavior (June 2026 findings):

- **Load time.** Boot + the lock-step EFTP transfer cost ~1M cycles per
  512-byte packet, so a game's own code only starts after ~80M cycles
  (small file) to ~190M (Trek-sized). A headless `--cycles 100000000` run
  of a big boot file snapshots the *loader*, not the program.
- **Blank start screens.** `AstroRoids` and `Invaders` come up waiting for
  a keystroke on an essentially blank screen (their FullBootInit display
  stream is empty); type any key and the playfield paints. `Reversi` is a
  text-prompt UI.
- **Games that need Alto RAM microcode never run:** `Pool.boot!1`
  (`JMPRAM`), `StarWars.boot!1` and `Trek.boot!1` (custom `70000B`-family
  opcodes). They `WRTRAM` a custom Alto microcode image and jump into it.
  The Dorado has no Alto control RAM and AEmu makes no pretense of one
  (`chm/doradosource/AEmuSources-cedar6.0.dm!1_/ATraps.mc`: `WRTRAM`
  no-ops, `RDRAM` returns 0, `JMPRAM` and all of `70000B..77777B` branch
  to the trap microcode), so they trap into Swat and park in TeleSwat's
  receive loop — identical to real hardware. `MissileCommand.boot!1`
  paints its attract screen first, then dies the same way on its first
  `70000B` opcode. Diagnostic: the machine-debug line `TRAPPC=` (Alto
  `M[527B]`) is nonzero and the world idles on Pup socket `60B`
  (TeleSwat), acking the fake server's probes with type-`204B` Pups.

**Cedar/Pilot boot files (Path B — would be loaded by the germ once it
reaches `DoInLoad`; not yet reached):** `chm/bootfiles/CedarNetExec.boot!4`,
`MesaNetExec.boot!1`, `NEWOS.BOOT!21`, and `chm/cedar/germ/OthelloDorado.boot!8`
(Othello, the Pilot disk/volume utility).

---

## How to load each combination

### Path A — Alto software on Dorado (working)

Headless (runs N cycles, writes a `dorado-screen.pgm` snapshot):
```
cd dorado
./build/dorado --eb worlds/aemu.eb --eftp '../chm/bootfiles/Galaxian.boot!1' --cycles 160000000
```
Interactive (SDL window, keyboard/mouse) — the Makefile has a `run-<name>`
target per boot file:
```
make run-galaxian      # Galaxian
make run-netexec       # the NetExec boot menu
make run-invaders      # Space Invaders (type a key once loaded)
make run-missilecommand / run-pinball / run-astroids / run-reversi
make run-boggs / run-crttest / run-dmt / run-keytest / ...
# run-pool / run-starwars / run-trek load but trap into Swat: they need
# Alto RAM microcode, which no Dorado has (see the caveats above).
```
Any Alto boot file works by hand:
```
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/<NAME>.boot!N'
```
Booting the NetExec menu (`make run-netexec`) lets you pick from the served
directory interactively; pass `--boot-dir` / `--boot-dir-all` to control
what the directory server advertises.

### Path B — Cedar/Pilot germ (in bring-up)

Use the **matched** pair (Cedar 6.1 germ + the Cedar microcode) with the
Pilot/Cedar PDI mounted as drive 0:
```
cd dorado
./build/dorado --boot-reason disk \
               --no-alto-boot \
               --eb '../chm/dorado/CedarDorado.eb!6' \
               --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
               --pilot-disk '../CedarDisk/CedarDorado-boot.pdi' \
               --cycles 1200000000
```
This boots the BaseBoard→Bootstrap→Initial chain, Initial netboots the Cedar
world, the germ is planted into VM, the Pilot disk path runs, and Cedar boots
to its **SimpleTerminal login prompt** ("Please login ... / Name:"). The
headless run writes `dorado-screen.pgm` at exit.

**Keyboard works.** Type at the `Name:` prompt in `make run-cedar`; the live
keys are delivered to Cedar's KeyBits at absolute `LONG[177033B]` and a
display vertical-field naked-notify wakes its keyboard watcher (see
`machine_cedar_io` in `src/machine.c`). Headless self-test (echoes `abc` at
the prompt; the login appears around 640M cycles):

```
./build/dorado --boot-reason disk --no-alto-boot \
  --eb '../chm/dorado/CedarDorado.eb!6' \
  --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
  --pilot-disk '../CedarDisk/CedarDorado-boot.pdi' \
  --type "abc" --type-at 655000000 --key-hold 4000000 \
  --cycles 700000000 --out /tmp/cedar-type.pgm
```

The emulator runs ~23.9M cycles/sec (~1.4x the real 16.67 MIPS Dorado) after
the per-step `getenv()` caching fix, so the login prompt appears in ~27s.

SDL command for the same path:
```
make run-cedar
```
The SDL window is blank until roughly 650M emulated cycles; the Makefile uses
`--speed 4000000` so pixels usually appear around frame 160. For a headless
SDL proof image:
```
make run-cedar-screenshot
```
which writes `/tmp/dorado-sdl-cedar-300.pgm`.

For an immediate native launch at the verified, keyboard-responsive login
prompt, use:

```sh
make cedar-login-snapshot       # one-time 650M-cycle producer
make run-cedar-snapshot-sdl     # subsequent launches restore immediately
```

The native checkpoint is also preserved in compressed form under
`dorado/snapshot-assets/`, so `make clean` can rehydrate it without repeating
the Cedar boot. The browser has a separate wasm32-native checkpoint under
`dorado/web-assets/`; selecting Cedar restores the login prompt instead of
replaying the boot.

The older `Dorado.germ!4` is the historical default but is version-mismatched
with the 1984 microcode; prefer `Dorado.germ-6.1.6`.

---

## Useful flags and trace env vars

CLI flags (`./build/dorado --help`): `--cycles N`, `--eb PATH`, `--germ PATH`,
`--eftp PATH`, `--out PATH` (final snapshot PGM), `--shot-prefix PATH`,
`--shot-every N` (periodic headless snapshots), `--snapshot-in PATH`,
`--snapshot-out PATH`, `--quote` / `--boot-keys` (DDC boot keys),
`--no-alto-boot`, `--progress`, `--boot-file-number`, `--boot-dir[-all]`,
`--type`, `--key-hold`.

Disk (`docs/disk-subsystem-plan.md`): `--disk-real` boots Cedar through the
**real disk controller** read path (FIFO + framing) instead of the IOCB shim
(`make run-cedar-real` for the interactive SDL version); `--disk SLOT=PATH`
mounts a real Trident pack (T-80/T-300, R/W) on drive SLOT (0..3), creating a
blank T-80 if the file is missing and flushing writes back on exit. Disk traces:
`DORADO_DISK_SEQ` (controller events), `DORADO_DSK_PC_TRACE`,
`DORADO_DISK_MUFF_TRACE`. Map a runtime IM address to a source label with
`./build/mbdis -r 0oADDR file.MB`.

Tracing (env vars, mostly gated by `DORADO_TRACE_GATE="lo,hi"` cycle window):
- `DORADO_IFUDISP_TRACE=1` — one line per dispatched bytecode (pc/pcf/op/br31/stkp/stack).
- `DORADO_XFER_TRACE=1` — per-microinstruction pc/T/Q/md/last-VA (the workhorse).
- `DORADO_STORE_TRACE_VA="lo,hi"` — stores whose VA is in the octal range.
- `DORADO_VMDUMP="lo,hi,cyc"` — one-shot VM word dump at a cycle.
- `DORADO_BR_TRACE`, `DORADO_MAP_TRACE`+`DORADO_MAP_TRACE_INDEX`, `DORADO_FAULT_TRACE=all`,
  `DORADO_ETH_TX_TRACE`, `DORADO_PIPEVA_TRACE` — base registers / map / faults / ether / pipe.
- `DORADO_STORAGE_MODULES=1..4` — model 4MW..16MW of real memory (affects the map config).
- `DORADO_ETH_WIRE=1` — faithful transmit wire model (default OFF): defer EOT's
  OutDone post by carrier-sense (no tx while receiving) + per-word wire time,
  instead of completing transmits instantly. A/B scaffold for the faithful
  receiver (`docs/ethernet-faithful-receiver.md`); boot-safe but partial.

Snapshots serialize the running control store, CPU, memory, display,
BaseBoard, Ethernet, disk controller, fast-I/O router, and scalar machine
state. Pack media bytes are not embedded; restore with the same `--disk`
arguments used to create the snapshot. Writable dirty packs are flushed before
snapshot so the restored controller state and media image agree.

For Interlisp-D Lyric, create the XCL checkpoint pair once, then resume it:

```sh
cd dorado
make lisp-lyric-desktop-snapshot   # one full 8.8B-cycle boot
make run-lisp-snapshot-sdl         # subsequent launches are immediate
```

The SDL frontend accepts `--snapshot-in` and `--snapshot-out`. The generated
`lisp-lyric-xcl.snap` and `lisp-lyric-xcl.pack` are an inseparable pair because
pack media is not embedded in the snapshot. The web build has a separate
wasm32-ABI checkpoint under `web-assets/`; select **Interlisp-D / Lyric — saved
Exec (XCL) desktop**, or link directly with `?boot=lisp`. Native raw snapshots
cannot be used by wasm32 because the current format deliberately serializes C
structs and validates their ABI sizes.

The saved Lyric desktop currently restores and renders but is not yet an
interactive Lisp listener. Keyboard and mouse transitions reach LLKEY and
typed characters enter Lisp's SYSBUFFER, but the saved process/TTY state does
not consume them and the blank Exec window never prints its initial prompt.
This is a guest-process bring-up bug, not a need to click a different host
window.

`make clean` removes the native files under `build/good-packs`. The producer
automatically rehydrates its prerequisite Lyric pack from the preserved
compressed web checkpoint media before performing the full native boot.
Historical experimental Current/Harmony/Medley packs in `build/good-packs`
are not automatically regenerated and are not prerequisites of supported run
targets; keep anything valuable outside `build/` before cleaning.

For browser snapshots, prefer checkpoints that remove a long wait without
substantially increasing the initial download. Lyric is already checkpointed.
Cedar's login is now checkpointed after fixing its blank-display regression.
Mesa Network Executive is the next high-value target once its exact capture
frame is revalidated. The directly booted Alto games are fast enough
that a separate snapshot for every menu item is not currently worthwhile.

`make test` runs the unit suites; the regression "gate" the bring-up keeps
green is: `make test` (10/10) + AEmu NETEXEC ≈ 1476–1505 px + Galaxian 121553 px
+ AltoMesaDorado in band + `make sdl` compiles.

---

## Current status (June 2026)

- **Path A (Alto-on-Dorado): working.** AEmu boots Alto software over EFTP and
  renders — games and the NetExec menu come up. This is the validated path and
  the regression gate.
- **Path B (Cedar/Pilot germ): active bring-up.** The full
  BaseBoard→Bootstrap→Initial→Cedar-microcode→germ chain runs; the germ
  executes its boot prologue, installs its trap handlers, and drives its
  module-startup chain. It does not yet load an OS. The blocker is germ-state:
  with the matched Cedar-6.1 germ it stops on a single malformed code pointer
  (a long pointer whose high word is a codebase value `0o6530` instead of the
  MDS bank `0o76`); forcing it correct lets the germ run ~5.9M bytecodes, so
  it's a confirmed, specific bug. See `docs/CONTINUE-HERE.md` for the live
  detail and the next step.
- **Stage-2 server is ready:** the in-process EFTP/Mayday boot server serves
  Cedar boot files byte-exact (proven by `make test`); the germ just needs to
  reach `DoInLoad` to request one.
- **Microengine:** thoroughly schematic-audited (`docs/schematic-audit.md`);
  the bring-up fixed five real microengine bugs in the Mesa/field/shifter/
  branch paths. Remaining hardware gaps and their specs:
  `docs/hardware-specs.md`.
