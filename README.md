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

**Active focus: Stage 2 — software boot.** Once the emulator microcode is
running it must load an actual OS. The disk route is blocked on *content*
(no preserved Pilot/Alto Dorado pack exists; see below) and on the disk
controller's data-transfer path. The chosen route is therefore **Alto-
style Ethernet software boot** (Mayday Pup `244B` + EFTP `30B`/`31B`),
which needs the Alto-side Ethernet surface the emulator exposes plus a
small boot file / NetExec to serve. The framebuffer renders but stays
blank until a real display list is installed by booted software.

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
make            # builds mbdis + four test binaries
make test       # runs all four test suites
```

C99, no external dependencies. Tested on macOS (Apple clang).

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
