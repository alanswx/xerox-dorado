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
 (6502)          (IM 7700+)   (loads     ┌─ AEmu world ──► Alto sw boot (EFTP)      ◄── WORKS
                              the world)  └─ Cedar world ─► Pilot germ ─► OS boot   ◄── WORKS
```

| Path | Microcode world | Second stage | Status |
|---|---|---|---|
| **A. Alto-on-Dorado** | `worlds/aemu.eb` (AEmu) | Alto boot file over EFTP | **Working** — Galaxian 121553 px, NetExec menu renders |
| **B. Cedar/Pilot germ** | `CedarDorado.eb` | `Dorado.germ` → Pilot disk | **Boots Cedar 6.1 all the way to the Viewers desktop** (2026-07-15): Guest login → LoaderDriver install over the in-process STP server → live Clock + CommandTool |
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
| `chm/cedar/germ-alt/Dorado.germ-6.1.6` | **Cedar 6.1, 1986** | **The working germ** — the matched pair with `CedarDorado.eb!6`; boots Cedar to the desktop |
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

**Cedar/Pilot boot files (Path B net-boot alternative):**
`chm/bootfiles/CedarNetExec.boot!4`, `MesaNetExec.boot!1`, `NEWOS.BOOT!21`,
and `chm/cedar/germ/OthelloDorado.boot!8` (Othello, the Pilot disk/volume
utility). The EFTP/Mayday server serves these byte-exact; they are what the
germ would request via `DoInLoad` if driven to net-boot. The working Cedar
path boots from the PDI disk instead.

### Pilot disk images (`--pilot-disk`, Path B)

Gzipped in `CedarDisk/` and rehydrated by make on demand:

| File | What it is |
|---|---|
| `CedarDisk/CedarDorado-boot.pdi` | The minimal boot volume — reaches the SimpleTerminal login prompt (`make run-cedar`) |
| `CedarDisk/CedarDorado-work.pdi` | The working volume the **desktop install** runs on — log in as Guest and LoaderDriver installs Basic.Loadees/BootEssentials/fonts from `--ftp-root` (`make run-cedar-work`) |
| `CedarDisk/CedarDorado-bestof.pdi` | Corrected kitchen-sink volume with 2,240 recovered files (`make run-cedar-bestof`) |

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

### Path B — Cedar/Pilot germ (boots to the Viewers desktop)

Fastest first:

```sh
make run-cedar-desktop-sdl   # restore the saved Viewers-desktop checkpoint
make run-cedar-work          # full cold boot + install; log in as "Guest"
make run-cedar               # boot volume only, to the login prompt
```

`run-cedar-work` boots `CedarDorado-work.pdi` with
`--ftp-root ../chm/cedar/stp-root` (the in-process STP file server) and
`DORADO_PDI_IGNORE_LABEL_FLAGS=1` (masks the label DA-hint words that
synthetic PDI media store as 0). After the Guest login, LoaderDriver
installs the loadee set and the desktop comes up. `make
cedar-desktop-snapshot` regenerates the checkpoint headlessly (it replays
the whole ~21 G-cycle boot + install and saves the snapshot **together with
the mutated PDI** — a matched pair, preserved gzipped in
`dorado/snapshot-assets/`); `make cedar-desktop-web-snapshot` produces the
separate wasm32-ABI pair for the browser build.

Under the hood, use the **matched** pair (Cedar 6.1 germ + the Cedar
microcode) with the Pilot/Cedar PDI mounted as drive 0:
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

For the corrected software-filled Cedar volume, use:

```sh
make run-cedar-bestof                 # cold boot
make cedar-bestof-snapshot            # regenerate the 1.2B-cycle login checkpoint
make run-cedar-bestof-snapshot-sdl    # immediate interactive restore
```

`CedarDorado-bestof.pdi` was rebuilt with Cedar-compatible two-page file
headers and a 17-page VAM. It currently carries 2,240 files recovered from the
first historical kitchen-sink image, with named launchers including ChessHack,
Celtics, Football, Draw2d, Clock, ColorTool, Chat, FTP, FontEdit, IconEditor,
Interpress, Animation3D, and Clip3d. The compressed PDI is the reproducible
source for the gitignored raw image; the compressed native snapshot survives
`make clean`.

---

## Installing CedarChest applications onto the desktop

The desktop world installs software exactly the way PARC did: `Bringover`
reads a package's `.df` from the in-process STP server and fetches its
files, then `Run` starts it. The served tree is `chm/cedar/stp-root/`;
mirror any CedarChest6.1 package into it with

```sh
python3 tools/fetch_cedarchest_app.py 'ChessHack.df!3'   # name!version
```

(the version number comes from
`https://xeroxparcarchive.computerhistory.org/cyan/cedarchest6.1/top/.index.html`;
if a payload directory is missing on cyan, try the CedarChest6.0 directory —
CardTable's files survive only there, with identical creation dates).

Already mirrored: **AIS**, **AISViewer**, **ChessHack**, **CardTable**,
plus Clock and BootTool. In the running desktop (click the CommandTool
first for type-in focus):

```
Bringover -p [Cedar]<CedarChest6.1>Top>ChessHack
Run ChessHack
```

### The schematics viewer (AISViewer) — the PROVEN recipe (2026-07-18)

AISViewer is a library (a viewer class, no Commander command); images are
displayed through the CommandTool's `Eval` interpreter escape. The current
desktop checkpoint's profile already runs `AISImpl`/`AISViewerImpl` at
boot, so in a fresh desktop (`make run-cedar-desktop-sdl`, click the
CommandTool prompt line first for type-in focus) the whole recipe is:

```
Bringover [Cedar]<CedarChest6.1>Top>AISImages
Delete AISViewer.bcd AIS.bcd ViewerOps.bcd ViewerClasses.bcd TIPUser.bcd TIPTables.bcd Real.bcd AISFormat.bcd
Eval AISViewer.DisplayAIS[AISViewer.CreateAISViewer[], "ProcH-BitSlice07.ais", NIL, FALSE, NIL, NIL, NIL, TRUE]
Eval ViewerOps.OpenIcon[ViewerOps.FindViewer["AIS Viewer"], FALSE, TRUE, TRUE]
```

Line by line, because every deviation here cost us a debugging session:

- `Bringover` (no `-p`) attaches the image DF's files; content is
  demand-fetched over STP when the viewer first reads a file.
- `Delete` removes ATTACHED interface bcds. Attached-but-not-loaded
  interface bcds from the mixed-vintage mirror shadow the loadstate and
  poison every `Eval` through those interfaces ("NoSymbols"/version
  faults); after `Delete`, symbols resolve from the RUNNING modules. Do
  NOT delete `AISImpl.bcd`/`AISViewerImpl.bcd` — runtime-loaded modules
  are their own symbols and must stay attached. (On the CURRENT desktop
  checkpoint every name reports `-- not found!`: the profile's selective
  `-o` Bringovers never create the poisoning attachments, so the line is
  a harmless no-op — kept for worlds that did full Bringovers.)
- The `Eval` passes ALL EIGHT DisplayAIS arguments positionally: the
  interpreter cannot synthesize defaulted arguments ("can't hack default
  for argument 5") or build partial record constructors.
- The last `TRUE` displays into the (iconic) viewer; `OpenIcon` opens it
  on screen. Expect the halftone paint to take a while — the 500x644
  ProcH sheet is ~20 emulated seconds of honest 1985 Imager work.

Both frontends also accept **Cmd/Ctrl+V paste** (typed as paced
keystrokes), so these lines can be pasted instead of typed; headless
scripts can use `--paste-at CYCLES --paste TEXT` for the same path.

Other images to try in place of `ProcH-BitSlice07.ais`: `uscmoon.ais`
(the 1978 Lick Observatory moon photograph), `IFU-Sheet02.ais`,
`ProcH-Title.ais`, `reducedparc.ais`, `AlbumMusician.ais`. Images must be
8-bit AIS: `AISViewerImpl` hardcodes `GrayLinearColorModel[sWhite~255]`,
so 1-bit files render black (convert with `pdftoppm -gray` +
`tools/pbm2ais.py`). Big fetches are safe since the BSP
sender-retransmission fix (`FTP_REWIND` in the `DORADO_FTP_TRACE` log is
the server rescuing a dropped packet, not an error).

`stp-root/CedarChest6.1/AISImages/` serves period images from `[Cyan]<AIS>`
(uscmoon, reducedparc, AlbumMusician) and **the Dorado's own schematics**,
converted from `DoradoDocs/schematics/*.pdf` with

```sh
pdftoppm -mono -f 10 -l 10 -scale-to-x 1000 -scale-to-y -1 \
    DoradoDocs/schematics/ProcH.pdf /tmp/page
python3 tools/pbm2ais.py /tmp/page-10.pbm \
    chm/cedar/stp-root/CedarChest6.1/AISImages/ProcH-BitSlice07.ais
```

(`pbm2ais.py` implements AISFormat.mesa's uca raster layout; header fields
validated byte-for-byte against the period files.)

To make installs permanent, run with `DORADO_PDI_SAVE=1` so the volume
keeps them, or bake them into the checkpoint recipe
(`make cedar-desktop-snapshot` types its install commands itself).

### The "archive" checkpoint (2026-07-16)

`make run-cedar-archive-sdl` boots the desktop with everything above
already attached PLUS PressReader/ShowPress, Cedar SIL (+its
BiScrollers/ViewRec/Abutters/SirPress/ImagerPress/Interpress closure)
and the ORIGINAL Dorado artifacts on the volume:
`ProcH-Rev-Ce.press`, `IFU-Rev-Ch.press`, `DoradoLibraries.press`
(the 1979-81 drawings) and Ed Taft's `DoradoBooting.tioga` memo.

Verified working: `Open DoradoBooting.tioga` in the CommandTool
displays the memo fully formatted in Tioga. Notes from verification:

- Scripted focus clicks on this checkpoint must hit the PROMPT line at
  the bottom of the CommandTool (`--click 700,733`), not mid-transcript
  — clicking into the scrolled log turns keystrokes into text edits.
- `Run ShowPressPackage` reports unbound `[PressReader,*]` imports:
  run `Run PressReaderImpl` FIRST (the config does not include the
  reader impl). Displaying a page then needs the interpreter
  (`ShowPress.CreateShowViewer` / `Open`+`DrawPressPage`), which is
  gated on the interpreter open problem (handoff #1).
- `Run Sil` currently traps at startup into a debugger event viewer
  (`NoSymbols[TrapsImpl.bcd]` — the debugger wants symbols for
  BOOT-FILE modules to even decode the trap; BringOver MesaRuntime
  first to see the real event). Sil bring-up is open.

---

## Useful flags and trace env vars

CLI flags (`./build/dorado --help`): `--cycles N`, `--eb PATH`, `--germ PATH`,
`--eftp PATH`, `--pilot-disk PATH` + `--boot-reason disk` (Cedar/Pilot disk
boot), `--ftp-root DIR` (in-process STP file server), `--out PATH` (final
snapshot PGM), `--shot-prefix PATH`, `--shot-every N` (periodic headless
snapshots), `--snapshot-in PATH`, `--snapshot-out PATH`, `--quote` /
`--boot-keys` (DDC boot keys), `--no-alto-boot`, `--progress`,
`--boot-file-number`, `--boot-dir[-all]`, `--type`, `--type-at CYC`,
`--key-hold`. SDL adds `--scale N`, `--speed CYCLES/frame`,
`--screenshot F1,F2,…`. A running instance (headless or SDL) writes a
`<shot-prefix>-<cycle>.pgm` screenshot on **SIGUSR1**.

Controls in the SDL window: F1 pauses/resumes, Cmd/Ctrl+Q quits, and the
three-button Dorado mouse is mapped left/middle/right = Red/Yellow/Blue with
laptop modifiers Option/Alt+click = Yellow (middle) and Cmd- or Ctrl+click =
Blue (right) — details in the top-level `README.md` "Controls" section (the
browser build behaves identically).

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

`make test` runs the unit suites (11 binaries); the regression "gate" the
bring-up keeps green is: `make test` + AEmu NETEXEC ≈ 1476–1505 px +
Galaxian ≈ 121.5k px + AltoMesaDorado in band + `make sdl` compiles +
PARC's real hardware diagnostics via `build/rundiag`
(`docs/running-diagnostics.md`).

---

## Current status (July 2026)

- **Path A (Alto-on-Dorado): working.** AEmu boots Alto software over EFTP and
  renders — games and the NetExec menu come up. This is the validated path and
  the regression gate.
- **Path B (Cedar/Pilot germ): boots to the Viewers desktop (2026-07-15).**
  The full BaseBoard→Bootstrap→Initial→Cedar-microcode→germ→Pilot chain
  boots Cedar 6.1 from the PDI disk to the SimpleTerminal login; logging in
  as Guest on the work volume drives LoaderDriver's remote install over the
  in-process STP server, Tioga fonts are demand-fetched, and the Viewers
  desktop comes up live (Clock, CommandTool, menus). Screenshot:
  `docs/images/cedar-desktop-first-boot-2026-07-15.png`. Saved checkpoints
  restore it instantly (`make run-cedar-desktop-sdl`; browser dropdown
  entry). See `docs/CONTINUE-HERE.md` for the live detail and next steps.
- **Interlisp-D: boots to the Lyric desktop.** `make run-lisp-snapshot-sdl`
  restores the saved Exec (XCL) desktop; the full boot is
  `make run-lisp-good-sdl`.
- **Stage-2 net-boot server is ready:** the in-process EFTP/Mayday boot
  server serves Cedar boot files byte-exact (proven by `make test`); driving
  the germ to `DoInLoad` over the net is an unexercised alternative to the
  working disk path.
- **Microengine:** thoroughly schematic-audited (`docs/schematic-audit.md`);
  the bring-up fixed six real microengine bugs in the Mesa/field/shifter/
  branch/Link paths (the sixth — `Return` clobbering a same-instruction
  explicit `Link←` load — was the final blocker in front of the Cedar
  desktop). Remaining hardware gaps and their specs:
  `docs/hardware-specs.md`.
