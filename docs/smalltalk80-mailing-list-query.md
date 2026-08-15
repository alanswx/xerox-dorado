# Looking for Dorado Smalltalk-80 microcode (VERS build 6)

A post to send to the Interlisp / Alto / CHM-archive community. Everything
below has been verified against the artifacts named; the searches described
were actually run.

> **Read this first (2026-08-15).** A substantial body of Smalltalk material
> was prepared for the CHM PARC archive and is **not currently published**,
> because the intellectual-property position for Smalltalk -- including
> pre-ParcPlace material -- has not been cleared for public release. Nothing
> is lost and no one is at fault; the rights question is simply still open.
>
> That reframes this query. The premise below -- "does it survive anywhere?"
> -- may have the answer **yes, and it is already sitting with the archive**.
> So before sending anything, consider whether the more useful move is to ask
> CHM whether the clearance could be revisited, now that there is a working
> emulator for the material to run on. A concrete, documented use is a better
> argument than an abstract preservation request.
>
> If the query is still sent, ask openly whether the file exists rather than
> implying it is lost, and do not ask anyone to route around the rights
> question.

---

## Subject: Does Dorado Smalltalk-80 microcode survive anywhere?

I'm building a software emulator of the Xerox Dorado. It currently boots
five original worlds from the CHM archives: the Alto emulator (games,
NetExec), Alto/Mesa, Cedar 6.1 to its Viewers desktop, Interlisp-D Lyric
and Medley 1.0, and **Smalltalk-76** via `SmalltalkDorado.eb!1` / `DSemu`.

I would like to run **Smalltalk-80** on it, and I believe the one thing
standing in the way is a microcode file that I cannot find in either CHM
archive. Before I go further I would like to know whether it survives in
anyone's collection.

### What I am looking for

**A Dorado Smalltalk-80 emulator microcode: the `.mb` (MicroD output) or
a bootable `.eb`, for the emulator that reports VERS build 6.**

Failing the binary, the Micro sources would be just as good — the
Smalltalk-76 equivalent is `DSemuSources.dm!17` (`DSmallint.mc`,
`DSmallops.mc`, `DSmallsubrs.mc`, `SMTRaps.mc`, ...), so I am looking for
the analogous set for the -80 interpreter.

Names it might carry, by analogy with the Smalltalk-76 build: something
like `DS80emu.mb`, `Dorado Smalltalk80.eb`, `DoradoSmalltalk80.eb`, or a
later `DSemu` whose `VERS` returns `53000C` rather than `52400C`.

### Why I think it existed

1. **The microcode's own version table allocates it.** The `VERS`
   instruction in `chm/dorado/aemu-src/ATraps.mc` (Taft, 26-Feb-1982)
   documents the build number as naming the emulator included:

   ```
   *	0	Alto emulator only
   *	1	Alto + Alto Mesa
   *	2	Alto + PrincOps Mesa
   *	3	Alto + Cedar Mesa
   *	4	Alto + Lisp
   *	5	Alto + Smalltalk 76
   *	6	Alto + Smalltalk 80
   ```

   Build 5 is what I have: `SMTRaps.mc` line 49 is
   `Stack_ 52400C, IFUJump[0], At[SD400, 14]; * Smalltalk 76` (engineering
   5 = Dorado, build 5). I have found nothing that reports build 6.

   Curiously the DSemu copy of the same comment block (`SMTRaps.mc`,
   Jun-1982) calls build 6 **"Alto + Smalltalk 78"** where ATraps.mc calls
   it "Smalltalk 80". If anyone knows which it actually was — or whether
   both existed — I would like to know that too.

2. **It is in the published literature.** L. Peter Deutsch, "The Dorado
   Smalltalk-80 Implementation: Hardware Architecture's Impact on Software
   Architecture," in Glenn Krasner (ed.), *Smalltalk-80: Bits of History,
   Words of Advice*, Addison-Wesley, 1983. The Dorado was the machine
   Smalltalk-80 was fast on; that implementation had custom microcode.

3. **The sibling machine's version survives.** The Dolphin (Xerox 1100)
   Smalltalk-80 customer release of 15-Nov-1982 is on bitsavers complete
   with its microcode, `mcdolphinaltost80.eb` (25,216 bytes), plus
   `1100st80-82-11-15-customer.run`, the image and the sources. So a
   D-machine Smalltalk-80 microcode was preserved for the Dolphin. Only
   the Dorado's appears to be gone.

### Where I have already looked (so nobody repeats it)

- **Both CHM servers, exhaustively.** I parsed the full cross-reference of
  each — 205,797 indexed entries on `xeroxparcarchive` and 14,680 on
  `xeroxalto`, 220,477 in total — and matched every filename against
  `st80|smalltalk80|smalltalk78|st78|dsmall|dsemu|doradosmalltalk|smalltalkdorado`.
  Every Dorado-Smalltalk hit is DSemu, i.e. build 5: `DSemu.mb!1`,
  `DSemu.dm!17` and its expanded sources under `<DoradoSource>`,
  `UnBug.bfs!1_>dsemu.mb`, and `[Io]<Murray>SmalltalkDorado.eb!1`. The only
  other Smalltalk-80-shaped entries anywhere are
  `[Indigo]<DLion>bj>ST80MesaDaybreak8K.db!1` (Daybreak, not Dorado) and
  the Alto files described below.
- `[Indigo]<Dorado>DoradoSmalltalk.eb!1` and `DoradoInitialSmalltalk.eb!1`
  are listed in `Archive.Directory` but 404 on both servers. Both are
  build-5 artifacts anyway — I have rebuilt them from `DSemu.mb!1` +
  `InitialSelect.mb!1` using the recipe in `DSemuRelease.cm`, and the
  single-segment one boots Smalltalk-76 to a screen byte-identical to
  `SmalltalkDorado.eb!1`'s.
- General web search for the file names and for anyone running Dorado
  Smalltalk-80: nothing.

### The Alto route, and why it does not substitute

`[Indigo]<BasicDisks>Smalltalk14.bfs!1` holds a complete December 1980
Alto Smalltalk-80 V1: `st80dec16.im` (498,604 bytes), `st80sources.v00`,
`st80dec16.changes`, and the Alto-side loader `d80load.run`. Since our
Dorado runs the Alto emulator, this looked like the way in.

It cannot work, for a reason that is not a missing file. Alto Smalltalk-80
runs on Alto **RAM microcode** — `d80load.syms` has `loadMicrocode` and
`D80RamImage`, and Ken Shirriff notes that "Smalltalk-80 required 3K of
control RAM, while Smalltalk-76 uses only 1K". The Dorado's Alto emulator
implements no writable control store at all, by design: in `ATraps.mc`,
`WRTRAM` is `IFUJump[0]` (accepted and discarded), `RDRAM` always returns
0, `JMPRAM` traps, and `EmIFUTrap[160, Trap17, 01]` traps opcodes
`70000-70377` — the first entry of the RAM-opcode trap table. So
`loadMicrocode` appears to succeed and the first dispatch into the RAM
range traps at 70000, which is exactly what we observe. On a Dorado the
"RAM microcode" *is* the emulator, so this is architectural.

(Two related questions I would also welcome answers to: **has anyone run
that December-1980 Alto Smalltalk-80 under ContrAlto?** It emulates an
Alto II XM with 3K CRAM, which is the configuration it wants. Note that
the widely circulated `st80.dsk` is reportedly Smalltalk-76 despite its
name. And **is `st80dec16.im` the only surviving Alto ST-80 image?**)

### If nothing survives

The fallback is to write a converter from the standard Smalltalk-80
interchange image to the Dorado's own object memory and drive the
Smalltalk-76 interpreter with it. The Dorado format is quite different —
`DSmallDefs.mc` defines a **Rot** (Reference Object Table) with a
reprobe-count hash and refcounts, SmallIntegers as a high OOP range
(`Oop00 = 176000B`), and no image header at all; the microcode expects a
fully laid-out memory image. That is a substantial piece of work with no
reference implementation to validate against, so I would rather find the
real microcode first.

### Contact / context

The emulator is a personal project. Happy to share details, the boot
recipes, or the analysis above with anyone interested. If you have a
Dorado disk pack image, an `Indigo`/`Nebula` boot-server snapshot, or a
personal archive from that era, the file would most likely be sitting
next to `DoradoMesa.eb` / `DoradoLisp.eb` / `DoradoCedar.eb` as boot file
number **111B** (`3111`), which is the number InitialSelect uses for
Smalltalk (`InitialSelectMain.mc`:
`Set[InitialEtherSmalltalkEntryLoc, 401]; T← 111C`).

---

## Notes for us (not part of the post)

- Sources for every claim above: `docs/smalltalk80-bootstrap.md` §7b for
  the trap analysis; `chm/dorado/aemu-src/ATraps.mc` and
  `chm/dorado/dsemu-src/SMTRaps.mc` for VERS and the trap tables;
  `chm/dorado/dsemu-src/DSemuRelease.cm` for the build recipe;
  `chm/basicdisks/Smalltalk14.bfs!1_/` for the Alto ST-80 kit.
- Shirriff's control-RAM figure:
  https://www.righto.com/2016/10/simulating-xerox-alto-with-contralto.html
- If the answer comes back "it never left PARC", the Rot converter is the
  path, and `docs/new-os-pairs-tasks.md` Task B has the format notes.
