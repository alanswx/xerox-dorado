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

**Stage 2 — Ethernet software boot — works: NetExec runs interactively.**
The running emulator microcode loads a real OS over the net (Alto-style
Ethernet software boot: Mayday Pup `244B` + EFTP `30B`/`31B`, served by
the in-process boot server). The **BCPL Net Executive** transfers over
EFTP, starts, installs its display list, and renders its banner — and you
can **type at its command line** (e.g. `help`) in the windowed frontend.

A long-standing page-zero corruption that crashed the booted world on the
first divide (the "types a little, then stops" failure) was traced to a
microarchitecture bug — an instruction's `RBase← FF` change wrongly
redirecting that same instruction's RM write — and fixed; the world is now
stable for hundreds of millions of cycles. See the root-cause writeup at
the top of `docs/CONTINUE-HERE.md`.

The disk route remains blocked on *content* (no preserved Pilot/Alto
Dorado pack exists; see below) and the disk controller's data-transfer
path, so Ethernet is the boot path. Bringing up a richer world
(Pilot/Cedar) is the next frontier.

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

#### Boot paths (same binary, different `--eb` / `--eftp`)

The two payloads are the **emulator microcode** (`--eb`, loaded by Initial
over the Dorado microcode boot) and the **Stage-2 boot file** (`--eftp`,
fetched over EFTP by the loaded world). Pick a path:

```sh
# Alto / BCPL Net Executive  — WORKS, interactive (type `help`, etc.)
./build/dorado-sdl --eb worlds/aemu.eb \
                   --eftp '../chm/bootfiles/NETEXEC.BOOT!8'

# Cedar (Mesa VM)  — IN BRING-UP (see docs/cedar-boot-plan.md, Phase 0+)
./build/dorado-sdl --eb '../chm/dorado/CedarDorado.eb!6' \
                   --eftp '../chm/bootfiles/CedarNetExec.boot!4'
```

Booting takes a little while (the real BaseBoard → Bootstrap → Initial →
Ethernet-microcode chain, then the EFTP transfer of the boot file); for
the Alto path the banner and `>` prompt appear once it is up, after which
typing works. The Cedar microcode loads today but does not yet reach its
prompt — that bring-up is tracked in `docs/cedar-boot-plan.md`.

Flags: `--eb PATH` (emulator-microcode world), `--eftp PATH` (Stage-2 boot
file), `--boot-file-number OCTAL` (boot file number, default `110`),
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
