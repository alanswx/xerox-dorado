# Xerox Dorado Emulator

An emulator for the **Xerox Dorado** — the high-performance microcoded
research workstation built at Xerox PARC (Computer Science Lab) in
1978–1985. The Dorado was the follow-on to the [Alto][alto] and the
research-line predecessor of the productized [Star/8010][star]. It
hosted the Mesa, Cedar, Interlisp-D, and Smalltalk-80 environments by
loading different microcode emulators into its 4 K-word
microinstruction store.

To our knowledge no public Dorado emulator existed before this project.

The plan is two-phase:

1. **Phase 1 — C emulator.** Correct, observable, single-binary
   interpreter of Dorado 34-bit microinstructions, running real PARC
   microcode (Mesa, Cedar, AEmu, Smalltalk, Bootstrap, Initial). In
   progress.
2. **Phase 2 — Verilog.** A synthesizable RTL implementation that
   matches the C emulator's cycle-accurate behavior. Target: FPGA.

[alto]: https://en.wikipedia.org/wiki/Xerox_Alto
[star]: https://en.wikipedia.org/wiki/Xerox_Star

## Status

The microengine is real and runs original PARC microcode. Working
subsystems (all test-covered): the 34-bit microinstruction decoder, the
ALU/ALUFM datapath, full JCN branching, the shifter, 16-way hardware
tasking, the IFU, the memory subsystem (cache + Map + Pipe + BR), slow-
and fast-I/O routing, and a real BaseBoard (6502 + RIOTs) that cold-boots
the machine. Display, disk, and Ethernet have working device models.

**Boot progress.** The full BaseBoard → Bootstrap → Initial chain runs.
**Stage 1, Ethernet microcode boot, works end to end:** Initial falls
through to `EtherMicrocodeBoot`, an in-process fake Pup boot server
delivers `AltoMesaDorado.eb` (Pup types `264B`/`265B`), Initial verifies
the EB checksum, calls `LoadRam`, and the loaded Alto/Mesa emulator
microcode world starts (~61 M cycles in; run with a 140 M-cycle budget).

**Stage 2 — Ethernet software boot — the BCPL Net Executive runs, with a
working Pup network stack.** The loaded Alto/Mesa microcode boots NetExec
over Alto-style Ethernet (Mayday Pup `244B` + EFTP `30B`/`31B`), which
starts, installs its display list, and renders its banner — and you can
**type at its command line** in the windowed frontend. NetExec's full Pup
stack now functions against the in-process fake server: it **learns its
network number** (gateway-info `200B`/`201B`), **sets its clock** to the
host's real wall-clock time (Alto time `206B`/`207B`), and **discovers the
boot-file directory** (`257B`/`260B`). All replies are spec-correct
(verified against the Alto Pup driver source and the Living Computer
Museum IFS server), including the Pup checksum and the hardware-CRC framing
the AEmu receive microcode requires.

**The documented Cedar-over-Ethernet chain works through the load.** Per
the PARC booting memo, Cedar is reached by booting NetExec, then typing the
name of the next boot file. With `--boot-dir` registering `CedarNetExec`,
NetExec lists it under `?`, and selecting it Mayday-requests its boot-file
number; the server serves `CedarNetExec.boot` and the EtherBoot loader
pulls it in. CedarNetExec itself does not yet reach its prompt (it stalls
in early startup) — that bring-up is the current frontier; see
`docs/CONTINUE-HERE.md`.

Two microarchitecture/protocol root causes fixed along the way: a page-zero
corruption that crashed the booted world on the first divide (an
instruction's `RBase← FF` change wrongly redirecting that same
instruction's RM write), and socket replies being dropped because they
omitted the trailing hardware-CRC word the receive microcode subtracts.
Both are written up at the top of `docs/CONTINUE-HERE.md`.

The disk route remains blocked on *content* (no preserved Pilot/Alto
Dorado pack exists; see below) and the disk controller's data-transfer
path, so Ethernet is the boot path.

There is **no shortcut disk image**: the CHM PARC archive is an IFS
file-server dump, not a collection of bootable packs, so no installed
Pilot or Alto Dorado volume exists to mount. A persistent disk would have
to be *built* by running Othello inside the emulator — which itself
depends on the Ethernet boot path landing first.

See `docs/ethernet-architecture.md` and
`docs/ethernet-local-boot-plan.md` for the protocol and the phased plan,
`docs/handoff.md` for the running state, and `dorado/CLAUDE.md` for the
code-side guide and the punch list of remaining emulation gaps.


## Build & run

```sh
cd dorado
make            # builds the headless emulator + tools + test binaries
make test       # runs all test suites
```

The core emulator is C99 with no external dependencies. Tested on macOS
(Apple clang) and Linux.

### Windowed frontend (SDL) — boot NetExec and type at it

The `dorado-sdl` frontend opens an 808×606 window, rasterizes the Alto
display list each frame, and feeds your keyboard and mouse to the running
world. Once the **Net Executive** prompt appears you can type commands
(`help`, etc.) just as on a real Alto.

It needs **SDL2** (the core emulator does not):

```sh
# macOS:   brew install sdl2
# Debian/Ubuntu:  sudo apt install libsdl2-dev
```

Build the frontend and the bootable microcode worlds (all from the
`dorado/` directory). The worlds live in the tree — nothing in `/tmp`:

```sh
cd dorado
make sdl          # -> build/dorado-sdl
make worlds       # -> worlds/aemu.eb (the Alto-emulator world, from AEmu.mb)
```

The Cedar/Mesa worlds need no generation — they are ready `.eb` files
already checked into `../chm`.

#### Boot recipes by environment

A run has two payloads: the **emulator microcode** (`--eb`, the world our
substituted Initial delivers over the Dorado microcode boot) and the
**Stage-2 software** (`--eftp`, fetched over EFTP by that world). The real
Dorado picks its world by which microcode is installed on disk (booting
memo §1.5: `InitialEtherAltoMesaDorado.eb` for Alto/Mesa,
`InitialEtherSmalltalkDorado.eb` for Smalltalk, `CedarDorado.eb` for Cedar,
etc.); here you choose it directly with `--eb`. Every recipe runs windowed
(`build/dorado-sdl …`) or headless (`build/dorado … --cycles N --out
screen.pgm`).

**A boot file's format dictates which microcode (`--eb`) it needs.** Check
the first word of any boot file with `od -An -to2 -N2 FILE` (`od` prints the
two bytes byte-swapped): `0o002401` is an **Alto B-format** file (word0 =
`0o405`) and runs on the Alto emulator world `worlds/aemu.eb`; `0o162400`
is a **Mesa/Pilot outload** (word0 = `0o345`) and needs the Cedar/Mesa
emulator world `../chm/dorado/CedarDorado.eb!6`. A Mesa-format file cannot
run on AEmu — the Alto interpreter just mis-runs its Mesa bytecodes (the IFU
instruction set stays `insset=0`), so nothing paints and nothing executes.

##### Group A — Alto emulator (`worlds/aemu.eb`) — WORKING

The Alto-emulator world runs the BCPL Net Executive and any Alto B-format
(`0o002401`) boot file. These come up and are interactive today. Each has a
`make run-…` shortcut (run from `dorado/`).

```sh
# Net Executive — interactive: learns its net + the real time, lists the
# boot directory under `?`, accepts typed commands at the `>` prompt
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/NETEXEC.BOOT!8'     # make run-netexec

# CRTTEST — CRT alignment/test patterns; press keys to cycle the patterns
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/CRTTEST.BOOT!1'     # make run-crttest

# DMT — the diagnostic/idle program a diskless Alto boots
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/DMT.BOOT!22'        # make run-dmt
```

**NetExec game menu (`--boot-dir-all`, on by default).** Booting `NETEXEC.BOOT` with
`--boot-dir-all` (the default when no explicit `--boot-dir` is given) makes the Net
Executive advertise every Alto B-format boot file in `chm/bootfiles/` as a boot
directory over the `257B`/`260B` boot-directory protocol. At the `>` prompt, type `?`
to list the directory, then a game name (e.g. `Galaxian`) to boot it — NetExec sends a
Mayday for that file, the in-process EFTP server streams it, and it boots over the
emulated wire. Every asset is read-only (microcode + `.boot` files), so this "type a
game name" experience is the natural shape for a future in-browser (Emscripten) demo.
(The headless `--type` keystroke path can pre-fetch the directory; the full typed boot
is driven via the SDL frontend.)

Games and utilities downloaded from `https://xeroxalto.computerhistory.org/Io/Murray/`
and verified Alto B-format (`0o002401`). Tested headless at 100 M cycles; pixel counts
are snapshot display-list pixels from `dorado-screen.pgm`. All load at ~32 M cycles.

| Program | What it is | make target | Pixels at 100 M cyc |
|---|---|---|---|
| Galaxian.boot!1 | Space Invaders-style arcade (CHM Murray) | `run-galaxian` | 124,239 — paints |
| MissileCommand.boot!1 | Missile Command arcade (CHM Murray) | `run-missilecommand` | 368,448 — paints |
| PinBall.boot!1 | Pinball arcade (CHM Murray) | `run-pinball` | 85,117 — paints |
| AstroRoids.boot!1 | Asteroids-style game (CHM Murray) | `run-astroids` | 163 — minimal (needs input/more cycles) |
| Invaders.boot!1 | Space Invaders (CHM Murray) | `run-invaders` | 163 — minimal (needs input/more cycles) |
| Reversi.boot!1 | Reversi board game (CHM Murray) | `run-reversi` | 831 — boots, minimal display |
| Pool.boot!1 | Pool/billiards game (CHM Murray) | `run-pool` | 0 — boots, no display at 100 M cyc |
| StarWars.boot!1 | Star Wars game (CHM Murray) | `run-starwars` | 0 — boots, no display at 100 M cyc |
| Trek.boot!1 | Star Trek game (CHM Murray) | `run-trek` | 0 — boots, no display at 100 M cyc |
| Boggs.boot!1 | Alto program (CHM Murray) | `run-boggs` | 0 — boots, no display at 100 M cyc |
| EtherLoad.boot!1 | Ethernet boot loader utility (CHM Murray) | `run-etherload` | 365,073 — paints |
| EDP.boot!1 | Ethernet diagnostic program (CHM Murray) | `run-edp` | 37,985 — paints |
| KeyTest.boot!1 | Keyboard test utility (CHM Murray) | `run-keytest` | 25,424 — paints |
| Calculator.boot!1 | On-screen calculator (CHM Murray) | `run-calculator` | 12,431 — paints |
| MadTest.boot!1 | Memory/ALU diagnostic (CHM Murray) | `run-madtest` | 7,316 — paints |
| PupTest.boot!1 | Pup network test (CHM Murray) | `run-puptest` | 6,814 — paints |
| BFSTest.boot!1 | BFS filesystem test (CHM Murray) | `run-bfstest` | 6,802 — paints |
| Scavenger.boot!1 | Disk scavenger/repair (CHM Murray) | `run-scavenger` | 4,032 — paints text UI |
| DiEx.boot!1 | Disk exerciser (CHM Murray) | `run-diex` | 1,969 — minimal |
| EtherWatch.boot!1 | Ethernet packet monitor (CHM Murray) | `run-etherwatch` | 965 — minimal text |
| Messenger.boot!1 | Alto messaging client (CHM Murray) | `run-messenger` | 383 — minimal |
| FTP.boot!1 | File Transfer Protocol client (CHM Murray) | `run-ftp` | 0 — boots, no display |
| CopyDisk.boot!1 | Disk copy utility (CHM Murray) | `run-copydisk` | 0 — boots, no display |
| GateControl.boot!1 | Gateway control utility (CHM Murray) | `run-gatecontrol` | 0 — boots, no display |
| Clock.boot!1 | Alto clock display (CHM Murray) | `run-clock` | 0 — boots, no display |
| ShowAIS.boot!1 | Show AIS data utility (CHM Murray) | `run-showais` | 0 — boots, no display |

```sh
# headline games — all paint without any keyboard input
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Galaxian.boot!1'          # make run-galaxian
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/MissileCommand.boot!1'    # make run-missilecommand
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/PinBall.boot!1'           # make run-pinball

# other games (load and start; may need keyboard/more cycles to show game screen)
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/AstroRoids.boot!1'        # make run-astroids
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Invaders.boot!1'          # make run-invaders
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Reversi.boot!1'           # make run-reversi
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Pool.boot!1'              # make run-pool
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/StarWars.boot!1'          # make run-starwars
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Trek.boot!1'              # make run-trek

# utilities and diagnostics that paint (no network/disk needed for initial display)
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/EtherLoad.boot!1'         # make run-etherload
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/EDP.boot!1'               # make run-edp
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/KeyTest.boot!1'           # make run-keytest
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Calculator.boot!1'        # make run-calculator
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/MadTest.boot!1'           # make run-madtest
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/Scavenger.boot!1'         # make run-scavenger
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/EtherWatch.boot!1'        # make run-etherwatch
./build/dorado-sdl --eb worlds/aemu.eb --eftp '../chm/bootfiles/FTP.boot!1'               # make run-ftp
```

##### Group B — Cedar/Mesa route — NOT YET WORKING (see `docs/CONTINUE-HERE.md` for the two paths forward)

`CedarNetExec.boot`, `MesaNetExec.boot`, `AlphaMesaMesaNetExec.boot`,
`MazeWar.boot`, and `NEWOS.BOOT` are Mesa/Pilot outload (`0o162400`) files.
They cannot run on `worlds/aemu.eb` (Alto/Nova only; Mesa bytecodes are
decoded as Nova instructions, `insset=0`, nothing executes).

**Key finding from the Dorado Booting memo §1.3:** Cedar software boots from
*disk*, not Ethernet. Serving `CedarNetExec.boot` directly to `CedarDorado.eb`
over EFTP is a category error — the Cedar microcode loads, enters BOOTORSTART
(`pc=0o1070`), then hangs in a cold `InitMem` map-write loop (`IWRITEMAP`/
`WAITFORMAPBUF`, hot PC `0o7116`), expecting a disk germ, zero opcode
dispatches, never reaching any Ethernet software-boot handler.

The memo's documented Ethernet route to Cedar is a three-stage chain using
the **Alto/Mesa** microcode: NetExec -> CedarNetExec -> target.

Two paths forward (both incomplete; see `docs/CONTINUE-HERE.md` for detail):

- **Route A (recommended):** use `AltoMesaDorado.eb` (contains the full Mesa
  VM, unlike `worlds/aemu.eb` which is Alto/Nova only) and chain through
  NetExec. `AltoMesaDorado.eb!2` enters correctly at `pc=0o1076`; the boot
  orchestration in `src/machine.c` / `src/ethernet.c` needs adaptation before
  a Mesa-format second stage can run.
- **Route B:** fix the Cedar microcode path directly — requires the `InitMem`
  map-write Hold handshake in `src/memory.c` (see `dorado/CLAUDE.md` gaps
  B1/C1) and a germ-Ethernet or disk boot channel (germ available at
  `chm/cedar/germ/Dorado.germ!4`).

The make targets below exercise the EFTP load path but do not reach a UI:

```sh
# load only — no UI yet
./build/dorado-sdl --eb '../chm/dorado/CedarDorado.eb!6' --eftp '../chm/bootfiles/CedarNetExec.boot!4'           # make run-cedarnetexec
./build/dorado-sdl --eb '../chm/dorado/CedarDorado.eb!6' --eftp '../chm/bootfiles/MazeWar.boot!1'                # make run-mazewar
./build/dorado-sdl --eb '../chm/dorado/CedarDorado.eb!6' --eftp '../chm/bootfiles/MesaNetExec.boot!1'            # make run-mesanetexec
./build/dorado-sdl --eb '../chm/dorado/CedarDorado.eb!6' --eftp '../chm/bootfiles/NEWOS.BOOT!21'                 # make run-newos
./build/dorado-sdl --eb '../chm/dorado/CedarDorado.eb!6' --eftp '../chm/bootfiles/AlphaMesaMesaNetExec.boot!1'
```

##### Other worlds

**Smalltalk — experimental.** The Smalltalk emulator microcode loads
through the boot chain, but Smalltalk needs its own loaded image to come up
(none is wired yet):

```sh
./build/dorado-sdl --eb '../chm/microcode/SmalltalkDorado.eb!1' \
                   --eftp '../chm/bootfiles/NETEXEC.BOOT!8'
```

**Interlisp-D (Lisp).** Build `DoradoLisp` from
`chm/dorado/expanded/UnBug.bfs!1_/DoradoLisp.MB`; it loads and draws a
display, but the contents are garbage — it needs a sysout (not wired).

#### Booting a second-stage file *through* NetExec (`--boot-dir`)

The real NetExec is a network command processor: at its `>` prompt you
type the *name* of another boot file and it loads that next. NetExec
discovers which files exist by broadcasting a **boot-directory probe** (Pup
`257B`); boot servers answer (`260B`) with `{boot file number, date, name}`
tuples. Register the files our fake server should advertise with
`--boot-dir NAME=BFN=PATH` (repeatable; `BFN` octal, `NAME` must end in
`.boot`):

```sh
./build/dorado-sdl --eb worlds/aemu.eb \
    --eftp '../chm/bootfiles/NETEXEC.BOOT!8' \
    --boot-dir 'CRTTEST.boot=111=../chm/bootfiles/CRTTEST.BOOT!1'
```

NetExec then lists the registered name under `?`; typing it sends a Mayday
for that boot file number, which the server serves from the registered path.
(The breath-of-life that loads NetExec itself uses boot file 0 and the plain
`--eftp` file.) Note the file you chain to must be an Alto B-format file to run on
`worlds/aemu.eb`. A Mesa-format file (e.g. `CedarNetExec`) downloads but
will not execute there (see Group B above). The intended path for Mesa-format
second stages is Route A: boot `AltoMesaDorado.eb` (which carries the full
Mesa VM) rather than `worlds/aemu.eb`, then chain through NetExec — that
path is not yet wired up.

Booting takes a little while (the real BaseBoard → Bootstrap → Initial →
Ethernet-microcode chain, then the EFTP transfer of the boot file); the
Alto/Mesa banner and `>` prompt appear once it is up, after which typing
works. A second-stage file you select (e.g. `CedarNetExec`) is itself a
large transfer — give it a moment after the screen clears.

Flags: `--eb PATH` (emulator-microcode world), `--eftp PATH` (Stage-2 boot
file), `--boot-file-number OCTAL` (boot file number, default `110`),
`--boot-dir NAME=BFN=PATH` (advertise a boot file to NetExec; repeatable),
`--scale N` (window scale), `--speed CYCLES` (cycles/frame), `--quote`,
`--no-alto-boot`, `--screenshot F1,F2,...`. Controls: **F1**
pauses/resumes; **Cmd/Ctrl+Q** quits.

`--boot-file-number` sets the Mesa/Dorado **boot file number** — the value
a real machine derives from the boot button and keyboard and hands to the
booting microcode. It is the first word of the three-word boot-parameter
block (`STK[1..3]`: boot file number, `BootParameterSeal`, and a checksum
word that makes the three sum to 0 mod 2^16); the emulator plants the
block directly because the 7-wire terminal back-channel that would carry
it is not modeled. The same number selects the netboot file Initial
requests. The default `110` (octal) is the normal Mesa/Alto net boot and
is what every path above uses — you only need this flag to experiment with
a different boot file number.

A headless build (`build/dorado`, same flags plus `--cycles N`,
`--type STRING`, `--out screen.pgm`) renders to a PGM snapshot for
scripted/CI use.

## Repository layout

```
.
├── README.md                          this file
├── CLAUDE.md                          project mission, working norms
├── docs/                              architecture references
│   ├── INDEX.md                       map of every artifact
│   ├── architecture.md                distilled architecture (HM §1–4)
│   ├── memory-architecture.md         memory subsystem reference
│   ├── jcn-encoding.md                JCN field encoding (HM Figure 6)
│   ├── chm-archive.md                 CHM mirror inventory
│   ├── chm-urls.md                    URL convention + curl recipes
│   ├── altoinfo-survey.md             what's reusable from AltoInfo (gitignored)
│   └── glossary.md
├── dorado/                            ★ the C emulator
│   ├── CLAUDE.md                      in-directory guide
│   ├── Makefile, compile_commands.json
│   ├── include/{mb,disasm,microcode,cpu}.h
│   ├── src/{mb,disasm,microcode,cpu,mbdis}.c
│   └── tests/test_{mb,disasm,microcode,cpu}.c
├── chm/                               microcode pulled from CHM
│   ├── microcode/                     17 ready-to-load .eb/.pb worlds
│   ├── microd/                        full MicroD BCPL source
│   ├── microd-manual/                 MicroD user manual + Alto exec
│   ├── dorado/                        canonical _cd8_/dorado/ tree
│   │   ├── *.mb / *.midas             every Dorado emulator
│   │   └── expanded/                  unpacked .dm archives
│   ├── io-doradologic/                schematic-source archives (selected)
│   └── indigo-dorado/                 directory index for older versions
├── DoradoDocs/schematics/                 board PDFs + Hardware Manual + cross-ref index
├── DoradoDocs/                        full schematic revision history
└── firmware/              5 BaseBoard EPROM dumps + chip-label photo
```

## Documentation

The Hardware Manual (`DoradoDocs/manuals/Dorado_Hardware_Manual_Sep1981.pdf`)
is the canonical reference for everything. Read its TOC; bookmark
Tables 5–13 (microinstruction fields), §3 (processor), §4 (control),
§5 (memory), §6 (IFU), Figures 1, 6, and 8.

The April 1980 booting memo
(`DoradoDocs/manuals/Dorado_Booting_Operation_and_Mechanisms_Apr80.pdf`)
explains the Bootstrap → Initial → emulator microcode chain.

## Microcode and tools sourced from CHM

`chm/` was pulled from the **Xerox PARC Archive** at the Computer
History Museum:

- https://xeroxparcarchive.computerhistory.org/ — fuller PARC IFS dump,
  contains the canonical Dorado source tree at `_cd8_/dorado/`.
- https://xeroxalto.computerhistory.org/ — narrower Alto-era mirror,
  contains the `Io/Murray/` boot-file collection plus MicroD source.

URL convention and curl recipes for re-fetching are documented in
`docs/chm-urls.md`.

## License

The C emulator code in `dorado/` and the documentation in `docs/` and
this repo's root are © 2026 contributors and licensed under the MIT
License (see `LICENSE`).

The Xerox PARC documents in `DoradoDocs/schematics/`, `DoradoDocs/`,
`firmware/`, and `chm/` originated at Xerox Corporation
and are reproduced from the Computer History Museum's public archive
of PARC materials. They are included verbatim for reference and to
make this repository self-contained for testing; they are *not* part
of the original work and retain whatever rights Xerox/CHM holds.

## Acknowledgements

- E. R. Fiala for the 1981 Hardware Manual and decades of preservation
  effort.
- Ed Taft for the Booting memo and Pup/EtherBoot.
- Butler Lampson, Ken Pier, Chuck Thacker, and the rest of the PARC
  CSL Dorado team.
- Al Kossow, Bitsavers, and the Computer History Museum for hosting
  the IFS archives.
- Juergen Buchmueller (salto) and Josh Dersch / Living Computers Museum
  (ContrAlto) — their Alto-emulator source trees are invaluable
  cross-references; reusable findings are catalogued in
  `docs/altoinfo-survey.md`.
