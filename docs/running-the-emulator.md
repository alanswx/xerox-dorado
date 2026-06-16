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
| **A. Alto-on-Dorado** | `worlds/aemu.eb` (AEmu) | Alto boot file over EFTP | **Working** — renders pixels |
| **B. Cedar/Pilot germ** | `CedarDorado.eb` | `Dorado.germ` → OS boot file | In bring-up — germ runs but doesn't boot an OS yet |

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
`PinBall.boot!1`, `AstroRoids.boot!1`, `StarWars.boot!1`, `Trek.boot!1`,
`Reversi.boot!1`, `Pool.boot!1`, …), and tools (`CRTTEST.BOOT!1`,
`DMT.BOOT!22`, `KeyTest.boot!1`, `MadTest.boot!1`, `FTP.boot!1`,
`EtherWatch.boot!1`, …).

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
make run-invaders      # Space Invaders
make run-missilecommand / run-pinball / run-astroids / run-reversi / run-pool
make run-starwars / run-trek / run-crttest / run-dmt / run-keytest / ...
```
Any Alto boot file works by hand:
```
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/<NAME>.boot!N'
```
Booting the NetExec menu (`make run-netexec`) lets you pick from the served
directory interactively; pass `--boot-dir` / `--boot-dir-all` to control
what the directory server advertises.

### Path B — Cedar/Pilot germ (in bring-up)

Use the **matched** pair (Cedar 6.1 germ + the Cedar microcode):
```
cd dorado
./build/dorado --eb '../chm/dorado/CedarDorado.eb!6' \
               --germ '../chm/cedar/germ-alt/Dorado.germ-6.1.6' \
               --cycles 90000000
```
This boots the BaseBoard→Bootstrap→Initial chain, Initial netboots the Cedar
world, and the germ is planted into VM and runs. It does **not** yet boot an
OS (see Status below). When it does reach `DoInLoad`, point `--eftp` at a
Cedar boot file, e.g. `--eftp '../chm/bootfiles/CedarNetExec.boot!4'`.

The older `Dorado.germ!4` is the historical default but is version-mismatched
with the 1984 microcode; prefer `Dorado.germ-6.1.6`.

---

## Useful flags and trace env vars

CLI flags (`./build/dorado --help`): `--cycles N`, `--eb PATH`, `--germ PATH`,
`--eftp PATH`, `--out PATH` (snapshot PGM), `--quote` / `--boot-keys` (DDC boot
keys), `--no-alto-boot`, `--progress`, `--boot-file-number`, `--boot-dir[-all]`,
`--type`, `--key-hold`.

Tracing (env vars, mostly gated by `DORADO_TRACE_GATE="lo,hi"` cycle window):
- `DORADO_IFUDISP_TRACE=1` — one line per dispatched bytecode (pc/pcf/op/br31/stkp/stack).
- `DORADO_XFER_TRACE=1` — per-microinstruction pc/T/Q/md/last-VA (the workhorse).
- `DORADO_STORE_TRACE_VA="lo,hi"` — stores whose VA is in the octal range.
- `DORADO_VMDUMP="lo,hi,cyc"` — one-shot VM word dump at a cycle.
- `DORADO_BR_TRACE`, `DORADO_MAP_TRACE`+`DORADO_MAP_TRACE_INDEX`, `DORADO_FAULT_TRACE=all`,
  `DORADO_ETH_TX_TRACE`, `DORADO_PIPEVA_TRACE` — base registers / map / faults / ether / pipe.
- `DORADO_STORAGE_MODULES=1..4` — model 4MW..16MW of real memory (affects the map config).

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
