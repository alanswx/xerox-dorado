# A demo worth showing — what to make work, in what order

Written 2026-08-01. The question behind it: given that some files are missing
from the archive, **what can we actually set up, make work, and document so
that a visitor finds this machine interesting?**

Grounded in the file-level audit in `docs/cedar-archive-gaps.md` and the
package inventory in `docs/cedarchest-packages.md`. Effort estimates assume
the existing tooling: `tools/fetch_cedarchest_app.py` to mirror, the
`CommandTool.BootCommands` profile mechanism to install at boot (recipe in
`docs/sil-schematics-handoff.md` §4), and ~25 min per checkpoint bake.

---

## 1. The headline: far less is missing than it felt like

Of CedarChest 6.1's 316 packages:

| | count |
|---|---:|
| complete, and no dependency gap | **244** |
| missing code of their own | 33 |
| complete but blocked by a gapped dependency | 39 |

So **244 packages are usable today** and were never the problem. Two more
(`Kipper`, `RegularExpression`) are "gapped" only in that a `.tioga`
documentation file is absent — harmless, and they are counted as usable
above.

And the blockage is concentrated. Seven packages account for 34 of the 39
blocked ones:

| gap | missing | blocks |
|---|---:|---|
| `AlpineUser` | 55 | DBIcons DBTools FileStreamTest Finger LoganBerry WalnutKernel WalnutRescue |
| `BiScrollers` | 34 | **Gargoyle** Histograms IdleHacks MickeyMouse PolyHack PreView **Sil** |
| `Cypress` | 7 | Nut Remember **Walnut** WalnutQuery WalnutSort |
| `Args` | 5 | Contours Dunn Geometry3dApplied Tube |
| `ArchivistBTree` | 7 | FileCmds ListArchives Misp StatementInterpreter |
| `CardTable` | 8 | Finch InterVoice LoganBerryTools VoiceUtils |
| `BigCardinals` | 4 | Combiner Fig |

**Every one of those seven has a CedarChest 6.0 payload directory.** Filling
them from 6.0 is worth trying — with eyes open, because that is exactly what
produced `VersionMismatch[BiScrollers]`. The difference: for `BigCardinals`
(4 files) or `Args` (5) we would be substituting a whole small package, not
mixing one 6.0 file into an otherwise-6.1 one. Try `BigCardinals` first: it
is 4 files and it unlocks **Gargoyle**, the illustrator.

---

## 2. Cedar — the visitor tour

The desktop already boots to Viewers with a CommandTool, a clock, working
`ls`/`OpenR` over the emulated file server, and the `DoradoWelcome` menu of
one-word commands. That is the frame; these are the acts.

### Act 1 — already working, just needs staging (0 effort)

| what | command | why a visitor cares |
|---|---|---|
| The machine draws its own board | `Schematic.cm` | a 1979 ProcH sheet, halftoned live by the 1985 Imager |
| The moon | `Moon.cm` | a 1978 Lick Observatory photograph, real halftone work |
| Ed Taft's 1980 booting memo | `Memo.cm` | period document in Tioga, the editor it was written in |
| Play chess | ChessHack icon | plays, offline, in the Chess40 font |
| Browse the release | `List [Cedar]<Cedar6.1>Tioga>*` | 306 files with true 1986 dates, served over emulated Pup |
| Read the source | `OpenR TiogaDoc` | opens Tioga's own manual, in Tioga |

### Act 2 — cheap wins, high visitor value (a few hours each)

Ranked by (works today) × (visible) ÷ (effort). All are **complete and
unblocked**; the work is mirroring + a profile line + a bake.

1. **IconEditor** (`df!4`) — edit the machine's own icons, live. Nothing
   says "this is a real workstation" faster.
2. **FontEdit** (`df!3`) — same for fonts. Pairs with the font-attach story
   we already understand well (`cedar-font-install-attach`).
3. **TopButtons** (`df!18`) + **DeskTops** (`df!3`) — visible desktop
   furniture; makes the screen look inhabited rather than empty.
4. **Magnifier** (`DF!1`), **NewCalc**, **NewClock** (`df!6`) — tiny,
   instant, obviously alive.
5. **Games**: `Football` `PigsInSpace` `ThreeCasabaFour` `OneCasaba`
   `Celtics` `VBounce` `Cookie` — all clean. Caveat: several of the flashier
   ones (`ColorTrix`, `ColorDisplay`) want a colour display the Dorado model
   does not have; stick to the monochrome ones.
6. **Grep**, **Compare**, **Lister** — dull alone, but they make the
   CommandTool feel like a real shell for anyone who pokes at it.

### Act 3 — the headline demos (days, with real risk)

1. **Sil + the Dorado's own schematics** — the best story this project has:
   the machine displaying its own logic drawings in the original editable
   vector format, with the 1980 symbol libraries. 32 ProcH sheets are
   already on the volume. Blocked on `BiScrollers`; the untried retry is in
   `docs/sil-schematics-handoff.md` (serve the 6.0 build under its real name,
   renaming nothing).
2. **Gargoyle** — the Cedar illustrator, 256 files, complete. Blocked only
   by `BigCardinals` (4 files, present in 6.0). Probably the single highest
   value-per-hour item on this page.
3. **JaM** (`df!4`) — the stack language PARC built before PostScript, by
   Warnock and Geschke. Complete and unblocked. Even a `JaM` prompt drawing
   one shape is a strong exhibit: *this is what PostScript came from*.
4. **A "browse the sources" tour** — `Documentation.df!27` is complete;
   with `OpenR` already working this turns the desktop into a browsable
   1986 manual set. Nearly free, and it shows off `ls`/`OpenR`/Tioga
   together.

### What to give up on

`Walnut`/`Peanut` (mail) need Grapevine, which we deliberately answer
"AllDown"; `Alpine*` wants a second machine; `TeX` has nowhere to print.
`Chat` could in principle talk to our own Pup shim — interesting, but a
research project, not a demo.

---

## 2.5 Sil — DONE (2026-08-01)

`Sil ProcH01.sil`, or the Sil button in the CommandTool menu line, opens the
Dorado's own processor-board drawing. In the shipped native checkpoint.
Screenshot `docs/images/cedar-sil-proch01-2026-08-01.png`; recipe and the
four wrong turns in `docs/sil-schematics-handoff.md` §3.2. The answer was a
**pure CedarChest 6.0 chain, real names, nothing mixed** — 6.0 BiScrollers
imports nothing, so the layer 6.1 adds disappears.

**Gargoyle is not reachable the same way.** `Gargoyle.load` needs
`BiScrollersButtonned` and CedarChest 6.0 has no gargoyle directory, so no
consistent set exists. The BigCardinals idea in §1 was wrong: that analysis
read DF `Imports` and missed that `.load` files name dependencies the DF does
not. Any future "X unlocks Y" claim should be checked against the `.load`.

## 3. Smalltalk-76 — the software is already inside the image

`make run-smalltalk` reaches Top View, the Classes browser with its four
panes, and a UserView workspace (screenshot
`docs/images/smalltalk76-desktop-2026-07-28.png`). Unlike Cedar, the
interesting software is not files on a server — it is the class library
*in* the image, and it is all there.

**The gap is interaction, not content** — and it is narrower than
`CLAUDE.md`'s "no click/keystroke driven into the desktop yet" suggests.
Measured 2026-08-01, after the input fixes that followed that note:

| | state |
|---|---|
| Mouse **position** | **works.** `--mouse 300,200 --type-at N --click 300,200` renders the cursor at exactly that point on the Smalltalk desktop (126,636 px vs a 124,945 px baseline — the cursor is the difference). |
| The destructive click | **gone.** Clicking no longer turns the desktop into noise; that was the BR-relative seeding fixed by `host-input-seeding-write-only-where-guest-reads`. |
| Mouse **button** | **not yet.** Clicking a class category in the browser (`110,142`, on "Kernel Classes") leaves the screen at 124,956 px — the cursor moved, nothing selected. |

So the remaining unlock is narrow and well-defined: **the button press is
not reaching Smalltalk's selection logic**, though the position is. The
click is already held for `key_hold` cycles (3 M in the test, ~180 ms
emulated), so a too-brief press is probably not the cause; the next thing to
check is what Smalltalk-76 actually polls for buttons and whether our UTILIN
seeding matches it in the bank the display task uses (recall Smalltalk's
bitmap lives in bank 1 while its DCBs stay in bank 0).

After that: a scripted tour — open a class category, show a method's source,
`doIt` an expression. Smalltalk-76 browsing itself is the demo.

`chm/archiveorg/smalltalk-80/` holds a Smalltalk-80 image and sources — a
different, later system. Interesting as a comparison but a separate
bring-up; do not confuse it with the 76 world that works.

---

## 4. Interlisp-D — the Lyric library, reachable and loading

> **DONE (2026-08-04) and beyond — see `docs/lisp-leaf-handoff.md`.** The
> shipped Lyric world carries 204 Lyric-vintage `.LCOM` packages + 14
> fonts on its pack, loads anything else over a full in-process **Leaf**
> server (the pack ceiling no longer binds), and opens the 1987
> Interlisp-D Reference Manual on screen (`(IL:FILESLOAD HELPSYS)` then
> `(IL:LOAD '{DORADO}<IRM>IRMDEMO)`, Guest/Guest at the login) — native
> (`make run-lisp-lispusers-sdl`) and on the deployed GitHub Pages build.
> The sections below are the planning trail that got there.

Lyric boots to its Exec (XCL) desktop (208,966 px gate). The archive has a
substantial user library we have never touched:

| tree | contents |
|---|---|
| `eris/lispusers/` | **131 compiled `.DCOM` packages, all 1982-84 (Harmony)** |
| `eris/lispcore/`, `eris/lisp/`, `eris/lispmanual/` | the release and its manuals |
| `phylum/lisplibrary/`, `phylum/lispfonts/` | library and fonts |
| `qv/lispcourse/` | 40 files — teaching material |

A sample of `lispusers`: `BROWSER EMACS EDITBITMAP EDITFONT CALENDAR BANNER
BLACKOUT DRAWFILE ACTIVEREGIONS CHUNK-MENU DEDITK EDITHIST EDITMACROS`.

`EMACS`, `BROWSER`, `EDITBITMAP` and `DRAWFILE` are obvious exhibits —
Interlisp-D's structure editor and a bitmap editor on a 1979 machine.

**The plumbing is closer than it looked** (measured 2026-08-01). Lyric does
not need a new protocol: it already fetches its sysout from us over **Pup
FTP on socket 3**, the same server Cedar's STP uses. Two things were in the
way, one now fixed:

1. **A single `--ftp-sysout FILE` short-circuits every request.**
   `eth_ftp_resolve_file` returns that one file for any name when the path
   is not a directory (`src/ethernet.c`, the `S_ISDIR` test). That is why
   Lyric can fetch its sysout and nothing else. Pointing Lisp at a served
   **root** instead removes the short-circuit — no code change.
2. **Interlisp names a DEVICE with braces** — `{DORADO}<>LISP.SYSOUT`, as
   the trace in `CONTINUE-HERE` shows — where Cedar names a host with
   brackets. The resolver only stripped `[...]`, so a brace name failed its
   character filter. **Fixed and unit-tested** (`test_ftp_brace_device_names`
   in `tests/test_ethernet.c`): a Retrieve for
   `{DORADO}<Cedar6.1>VersionMap>CedarSource.VersionMap` now answers
   HereIsPList with the right file instead of markNo.

So the remaining work is configuration and one experiment, not protocol:
build a Lisp FTP root holding `LISP.SYSOUT` plus a few `.DCOM`s, point the
Lisp targets at it instead of the bare sysout, and type
`(LOAD '{DORADO}<>BROWSER.DCOM)` at the Lyric Exec. If that lands, all 295
packages are reachable at once.

**A second route worth knowing:** Lyric boots from a Trident **pack**
(`lisp-lyric-xcl.pack`), and `dsk2trident` builds packs from files. Putting
`.DCOM`s on the pack and loading them from `{DSK}` avoids the wire
entirely, at the cost of a pack rebuild per change.

### RESULT (2026-08-01): the pack route works, and Lyric packages load

**Superseded in part -- read `docs/interlisp-archive-survey.md`.** The
paragraphs below correctly describe the pack machinery and the failure with
Harmony-era `.DCOM`s, but their conclusion ("no Lyric-era library exists")
was a search error: Interlisp compiles to **`.LCOM`** from Lyric onward, and
`phylum/lisp/lyric/` holds ~320 Lyric-vintage compiled files. `GREP.LCOM`
and `HRULE.LCOM` both load on the shipped Lyric world. Harmony is not
needed.

### The original finding: the pack route works; `.DCOM` packages are the wrong vintage

Both halves of the plumbing above were built and proved, and then the
library turned out not to fit. Recording the outcome in full because the
negative result is the useful part.

**What works.** `make lisp-lispusers-pack` inserts `.DCOM`s into the Alto
file system inside the Lyric pack, `make lisp-lispusers-snapshot` boots that
pack from cold and saves the world, and Lyric then finds the files:

```
2<-(LOAD '{DSK}BANNER.DCOM)
Loading {DSK}BANNER.DCOM
compiled on 30-Nov-83 ...
File created 30-Nov-83 ...
```

That date is not ours -- it is stored inside the DCOM and matches the
archive index for `BANNER.DCOM!9` exactly, so the file arrived intact
through insert -> pack -> Alto FS -> Interlisp's own file reader.

**What does not.** The next line is `Bad compiled function`. Same for
`BROWSER.DCOM!10`, the newest build in the directory. **Every one of the 131
`.DCOM`s in `eris/lispusers` was compiled in 1982-1984** (checked against the
index: 8 files from 1982, 39 from 1983, 78 from 1984, none later), which is
the **Harmony** era. The sysout we boot is **Lyric** (1986-87), and the
compiled-code format moved in between. There are only 4 `.LSP` sources in
the whole directory -- the other non-DCOM files are documentation
(`.TTY`, `.PRESS`, `.BRAVO`, `.TED`) -- so there is no load-the-source
workaround either. `eris/lispcore`, `eris/lisp`, `phylum/lisplibrary` and
`qv/lispcourse` hold no `.DCOM`s at all.

So the earlier framing of this section -- "295 packages nobody has tried" --
was wrong in the way that matters. They are reachable, and they are for a
different Interlisp release.

**What that leaves, in order of cost:**

1. **Boot a Harmony sysout instead.** We already have
   `chm/lisp/harmony/basics/LISP.SYSOUT!15`, and the whole library then
   matches by construction. This is the cheap path to a Lisp demo with
   software in it, at the price of bringing up a second world (the
   validated desktop today is Lyric).
2. **Compile inside Lyric.** Version-independent and slow, and needs
   sources we mostly do not have.
3. **Find a Lyric-era library.** Not in the obvious places in the archive.

The gate `make verify-lisp-lispusers` distinguishes all three outcomes --
file-not-found (a pack or checkpoint fault), `Bad compiled function` (this
vintage wall), and a real load -- by reading Lisp's own reply off the
framebuffer with `tools/pgm_text.py`. A pixel count cannot tell them apart:
the failing load and the succeeding one differ by 0.5%.

---

## 5. The 10-minute visitor script (what to aim at)

The goal is a page a visitor reads while the machine is in front of them,
each step one word, each showing something the others cannot.

1. `Welcome.cm` — the machine prints its own menu.
2. `Schematic.cm` — **it draws its own processor board.**
3. `Moon.cm` — 1978 photograph, halftoned by the 1985 Imager.
4. `List [Cedar]<Cedar6.1>Tioga>*` — 306 real files, 1986 dates, fetched
   over emulated Ethernet.
5. `OpenR TiogaDoc` — read the manual for the editor, in the editor.
6. Click the **ChessHack** icon — play a game.
7. *(target)* Click the **Sil** icon — open `ProcH01.sil` and zoom into the
   ALU the emulator is executing on.
8. *(target)* Switch worlds: the same hardware running **Smalltalk-76** and
   **Interlisp-D**, from the dropdown.

Steps 1–6 work today. Step 8 works but is view-only for Smalltalk. Step 7 is
the prize.

---

## 6. Recommended order of work

1. **`BigCardinals` from 6.0 → Gargoyle.** 4 files; unlocks a real
   illustrator. Best value per hour on this page, and a clean test of
   whether 6.0-fill works at all when the whole package is substituted.
2. **The Sil retry without renaming** (`sil-schematics-handoff.md` §3). ~10
   minutes on the existing `cedar-sil-base` checkpoint; may already work.
3. **Act 2 bundle in one bake** — IconEditor, FontEdit, TopButtons,
   Magnifier, NewCalc, and two or three games. One profile edit, one bake,
   one screenshot each for the docs.
4. **Smalltalk interaction** — one click into the class browser.
5. **Interlisp `.DCOM` loading over the wire** — the plumbing that opens 295
   packages.
6. **A `demo.cm`** that prints the tour above, the way `Welcome.cm` already
   does, so the machine explains itself.

Items 1–3 are low-risk and mostly mechanical. Items 4–5 are the ones that
change what the project can show, and both are bounded pieces of emulator
work rather than archaeology.
