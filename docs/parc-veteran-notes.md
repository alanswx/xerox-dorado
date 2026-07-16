# Notes from a PARC veteran (July 2026)

A former Xerox PARC person who worked on this hardware reviewed the project
and sent a collection of pointers and history (relayed 2026-07-15). This
file preserves everything they shared, cross-referenced against what we
already mirror locally. Items marked LOCAL are already in the repo; NEW
means we have only the URL so far.

## Cedar operations (directly actionable)

### Name lookup / "file not found" — the prefix-server table

Cedar 6.x ran at multiple sites (primarily Palo Alto and Pasadena); each
site kept its own copy of the release on its local IFS servers, and each
workstation mapped logical prefixes to physical servers with `PSAdd`:

```
PSAdd  Cedar Cyan Cyan
PSAdd  User  Ivy  Ivy
PSAdd  Fonts $    Cyan
```

Documented in `CommandToolCommands.tioga` — LOCAL:
`chm/cedar/cedar6.1-docs/CommandToolCommands.tioga!1` (fetched from
`cyan/cedar6.1/documentation/`). At PARC the fonts nominally lived on
Cyan; in the CHM archive they survive on **indigo** — so any PSAdd-style
mapping we honor should treat Indigo as the font host. (Our in-process
STP server currently answers for `[Cedar]`, `[Fonts]`, `[User]` name
lookups directly — see `dorado/src/ethernet.c` NetDir handling — which is
why the desktop install works; the PSAdd model is the authentic mechanism
if we ever want per-server routing.)

### Disk boot was the norm

In CSL the Dorados were **always booted from the local disk** — netboot
was avoided to keep network traffic down with so many machines. This
matches where we ended up (the `--pilot-disk` path); the netboot chain we
built first was the bring-up scaffold, not the historical common case.

Installing Cedar 6.1 from scratch is documented in `ReleaseMessage.tioga`
— LOCAL: `chm/cedar/cedar6.1-docs/ReleaseMessage.tioga!9`. This is the
reference for the real Othello-based volume install (handoff gap: replace
rusty-backup with an authentic install).

### The consistent boot-file set

`cyan/cedar6.1/top/` carries the coherent release set of `.boot`, `.pb`
(pilot boot), `.germ`, `.db` (diagnostic boot) and `.eb` files. The
microcode versions consistent with the release are enumerated in
`MicrocodeDorado.df` — LOCAL:
`chm/cedar/cedar6.1-docs/MicrocodeDorado.df!3`.

Notably: **`CedarSnapshotDorado.boot`** (built per `CedarSnapshot.df`) was
"the most common boot file used that was installed on most new Cedar
Dorados" — a checkpoint-style boot image. A native snapshot-boot path
would be the authentic analogue of our save/restore checkpoints.

## Cedar build-system genealogy (indigo/cedar/top)

`https://xeroxparcarchive.computerhistory.org/indigo/cedar/top/.index.html`
is the version-control root; the `.df` files enumerate every file of every
package. Boot-file construction (NEW, URLs only):

| File | Role |
|------|------|
| `CedarMaker.df` | how `CedarDorado.boot` is built |
| `CedarSnapshot.df` | how `CedarSnapshotDorado.boot` is built |
| `DoradoMicrocode.df` / `MicrocodeDorado.df` | exports all the microcode `.eb` files people installed on their Dorado disks, and everything required to build them |
| `Germ.df` / `GermDorado.df` | how the `.germ` files are built (Dorado-specific parts in the latter) |
| `PublicDoradoRelease.cm` | the command list for a release |
| `MakeDoradoDisks5.cm` | commands to build a Cedar 5 Dorado disk |
| `MakeBoot.df` | describes MakeBoot.bcd (see `MakeBoot/MBMake.cm`) |

Early Cedar 5.0 material in the same tree gives background on how Cedar
evolved toward 6.1/7.0.

## Hardware history (Phase 2 / Verilog background)

### Board technologies

Three generations of board wiring:

1. **Stitch-weld** — an X-Y plotter with the pen replaced by a wire
   feeder, welding tip and cutter, welding wire point-to-point on blank
   socket boards. Robust but slow per board.
2. **Multi-Wire** — a commercial process laying fine insulated wire into
   epoxy resin with Manhattan routing, running wires across not-yet-drilled
   via/component holes; drilled, plated, stuffed, wave-soldered.
   Unreliable: twisting a board even slightly broke connections at the
   vias.
3. **Multi-layer PC boards** — the investment made for the final builds
   because of Multi-Wire's unreliability.

Board source files are named by revision AND technology (prefix `mw` =
Multi-Wire, plus PC and stitch-weld variants). **For an FPGA start from
the PC versions.**

### Design automation: SIL

Schematics were drawn in **SIL** (SimpleIllustrator) on Altos — an
illustration tool extended for schematics (IC placement, signal
assignment, net-list extraction); its net-lists drove the stitch-weld
machine. Sources:

- Alto SIL source: `xeroxalto.computerhistory.org/_cd6_/sil/` (NEW)
- `indigo/altosource/RSILSOURCES.DM!2_` (NEW)
- The DA tool chain (`.run` files) used to build all three board
  technologies: `io/doradologic/DoradoBuildFiles.dm!1_` — LOCAL:
  `chm/io-doradologic/DoradoBuildFiles.dm!1`
- Board SIL sources: `io/doradologic/` — indexed locally
  (`chm/io-doradologic/`, survey in docs/chm-archive.md §5)
- `.press` renderings of the SIL drawings: `io/doradodrawings/` — LOCAL
  as PDFs under `DoradoDocs/doradodrawings/`

(The veteran suggests a SIL re-implementation in a modern language would
be worthwhile.)

### Bring-up: Midas and the manifold

Dorado hardware was brought up from an Alto running **Midas**, through a
dedicated access path on every board called the **manifold** — think
early JTAG but with much more functionality. Artifacts:

- `indigo/dorado/UnBug.bfs!1` — an Alto Midas disk image — LOCAL:
  `chm/dorado/UnBug.bfs!1_/` (117 files incl. DoradoLisp.MB)
- `GarageMidasManual.dm!1_` (indigo/doradosource) — Frank Vest's (lead
  tech at "The Garage" manufacturing site) summary of all Midas debugging
  support, incl. diagnostics — referenced in docs/chm-archive.md (Press
  renderings B5/B11); source archive NEW.

### ECL PROMs

Custom combinatorial logic lived in ECL PROMs; the BCPL generators are in
`indigo/doradosource/DoradoProms.dm!14_` — LOCAL (partial):
`chm/doradosource/DoradoProms.{bcpl,defs,help}`, `IFUProms.bcpl`,
`ProcProms.bcpl`, plus `chm/io-doradologic/DoradoProms.run!3`.

### Original microcode source archives

| Emulator | Archive | Local |
|----------|---------|-------|
| Alto (AEmu) | `indigo/doradosource/AemuSources.dm!82_` | LOCAL: `chm/doradosource/AemuSources.dm!82_` |
| Pilot/Mesa | `indigo/doradosource/trinity/` | NEW (we have the built TriEmu/TriMesa/Mesa .mb) |
| Cedar/Pilot | `indigo/doradosource/DMesaSources.dm!85_` | partial LOCAL: `chm/doradomicrocode/doradomicrocodesources/DMesa*.mc` (the singles used in bring-up debugging); the full `.dm` archive is NEW |
| Smalltalk | `indigo/doradosource/DSemuSources.dm!17_` | NEW (we have built DSemu.mb) |
