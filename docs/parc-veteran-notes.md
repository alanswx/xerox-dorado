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


---

# Second veteran: Tim Diebert (July 2026)

A second former PARC/CSL engineer reached out — **Tim Diebert**, author
and maintainer of AISViewer itself (see AISViewerDoc.tioga: "Maintained
by: Tim Diebert <Diebert.pa>"), and with Nick [Briggs?] a 20+-year
veteran of the Cedar print-server lineage that became the Xerox
DocuPrint product. Highlights, all fetched into
`chm/cedar/cedar6.1-docs/`:

## The four /cyan/ Cedar directories, explained first-hand

For Cedar 6 and beyond, software was classified **core** vs **other
useful**: core packages went to `/Cedar/Cedar6.x/`, the rest to
`/Cedar/CedarChest6.x/`, each with its own `Top` of DFs. Two more
directories served printing: `/Cedar/CedarPrinter6.1/` (early print
server driving a Raven from a DLion) and `/Cedar/CedarPS/` (the
Cedar7.0-based multi-printer server for DLion/Dorado — the genesis of
DocuPrint on SPARC).

**The XC1-2-2 fonts live at `/Cedar/CedarPS/Top>XC1-2-2-CD-bits.df`**
(fetched; exports `.cd-bit300` printer bitmap fonts, Diebert's own DF,
Apr-1987) — resolving the installer's long-mysterious XC1-2-2 probe.

## Documentation set (all fetched)

Glossary.tioga (PARC server/jargon glossary), BriefingBlurb.tioga (the
new-CSL-member document), Introduction.tioga (An Introduction to
Cedar), OverviewDoc.tioga (Cedar Language Overview),
NoviceUserProfile.Tioga + StandardUser.Profile (canonical starting
profiles — compare ours!), StandardUser-LoginWork.cm,
CedarChestDoc.tioga (Cedar-vs-CedarChest in detail),
CedarExamplesDoc/CedarProgramStyle/CedarSyntaxDoc (the language docs),
and **EtherBoot.tioga — the net-booting protocol documentation** (goes
straight to the Stage-2/DoInLoad work).

## Tim on the colour monitor, and on how the book was actually made

First-hand, 2026-08-08. This is the use case the colour work exists for, and
it settles several things we had been inferring.

**Dorados came in exactly two configurations:** one with just a monochrome
display, and one with **both a monochrome display and a colour RGB monitor**.
Nothing else. That matches the Hardware Manual (doc p.110: "on a Dorado with
only a 7-wire terminal and no color monitor, only the DispY board is
present") and it is why `DORADO_DISPM_COLOR` is a binary install rather than
a family of options.

**"Because of the low resolution on the colour monitor, you needed a
monochrome display to do anything useful."** So the two screens were not
peers -- the mono screen was where you worked and the colour one was a
second, coarser surface beside it. That is exactly the arrangement
`ColorDisplay left | right` describes, and it justifies the emulator opening
a second window rather than trying to blend them.

**On the resolution his memory is uncertain** -- "I can't remember if the
monitor was 480x320 or 320x?" -- and the driver source is definite where
memory is not. `ColorDisplayHeadDorado.mesa`'s `SetDisplayType` offers
exactly two: `standard` = **640 x 480** and `highResolution` = **1024 x 768**.
A plausible reconciliation: the same file's margin-offset table names
**ConracRQB525**, and a 525-line monitor is the NTSC-ish one that gives
640x480. Treat 640x480 as the standard colour monitor and Tim's figure as a
recollection of roughly that.

### How the book was made -- the whole pipeline

> "For the book software, the color monitor was used with an antialiasing
> Imager context as a **soft proofing device**. This setup was good enough to
> be used for the actual page layout, ie page breaks, position of boxes for
> the photographs &c. Once that was done, the software made **Interpress or
> Press files** that were printed on a **Dover** for hard copy proofing
> before being printed at high resolution **directly on film**. (The high res
> printer only printed Interpress.) The film and monochromatic photographs
> were sent to the printing company. The company optically half-toned the
> photos and put them into the film page masters **where the holes were**.
> They then made the printing plates from the assembled film signature."
>
> "**All of the Artwork was created in Gargoyle using the same process.**"

Four things follow for this project:

1. **Gargoyle is not a nice-to-have, it is the tool.** Tim made the book's
   artwork in it, so "get Gargoyle running" is the requirement, not a
   demonstration of colour.
2. **The colour screen's job was soft proofing**, through an *antialiasing
   Imager context*. So the interesting output is not "colour on screen" for
   its own sake -- it is whether an Imager context renders a page proof
   there. Antialiasing means grey levels, which is what the 8-bit ATable path
   and `ColorDisplay gray | dither` are for.
3. **Interpress is on the critical path, not a side quest.** The layout
   software emitted Interpress or Press; the high-resolution filmsetter took
   Interpress only. That retroactively justifies the Interpress and
   InterpressTools packages in Gargoyle's import closure -- they are the
   output half of the workflow, not incidental weight.
4. **Photographs were holes.** The page masters carried boxes where images
   would go and the printer optically half-toned the photos into them. So a
   faithful reproduction of Tim's workflow does not need photographic images
   on screen at all -- it needs correct page geometry and correctly placed
   empty boxes.

## A living use case

Diebert wrote software at PARC that produced the actual page negatives
for his book (Southern Pacific Steam Locomotive Compendium, Diebert &
Strapac, started 1985, printed via PARC's high-resolution film
printer) — and would like to run that software again on this emulator
for the revised edition. A concrete, personal target for the
CedarPS/printing bring-up: the emulated Dorado producing book pages
again, four decades later.
