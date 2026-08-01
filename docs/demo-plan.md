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

## 3. Smalltalk-76 — the software is already inside the image

`make run-smalltalk` reaches Top View, the Classes browser with its four
panes, and a UserView workspace (screenshot
`docs/images/smalltalk76-desktop-2026-07-28.png`). Unlike Cedar, the
interesting software is not files on a server — it is the class library
*in* the image, and it is all there.

**The gap is interaction, not content.** The top-level `CLAUDE.md` records
it: "Still open: interaction (no click/keystroke driven into the desktop
yet)". A visitor cannot currently do anything.

Work, in order:

1. **Drive a mouse click into the Smalltalk desktop** and make the class
   browser respond. This is THE unlock — everything else follows. Note the
   memory `host-input-seeding-write-only-where-guest-reads`: a click once
   scribbled guest memory and produced noise, so this needs the per-world
   care that fix established.
2. Then a scripted tour: open a class category, show a method's source,
   `doIt` an expression. Smalltalk-76 browsing itself is the demo.
3. `chm/archiveorg/smalltalk-80/` holds a Smalltalk-80 image and sources —
   a different, later system. Interesting as a comparison but a separate
   bring-up; do not confuse it with the 76 world that works.

---

## 4. Interlisp-D — 295 packages nobody has tried

Lyric boots to its Exec (XCL) desktop (208,966 px gate). The archive has a
substantial user library we have never touched:

| tree | contents |
|---|---|
| `eris/lispusers/` | **295 compiled `.DCOM` packages** |
| `eris/lispcore/`, `eris/lisp/`, `eris/lispmanual/` | the release and its manuals |
| `phylum/lisplibrary/`, `phylum/lispfonts/` | library and fonts |
| `qv/lispcourse/` | 40 files — teaching material |

A sample of `lispusers`: `BROWSER EMACS EDITBITMAP EDITFONT CALENDAR BANNER
BLACKOUT DRAWFILE ACTIVEREGIONS CHUNK-MENU DEDITK EDITHIST EDITMACROS`.

`EMACS`, `BROWSER`, `EDITBITMAP` and `DRAWFILE` are obvious exhibits —
Interlisp-D's structure editor and a bitmap editor on a 1979 machine.

**Unknown to resolve first:** how a running Lyric loads a `.DCOM` from our
in-process server. Cedar uses STP over Pup socket 3, which we implement;
Interlisp uses its own file access, and we currently only serve it a sysout
over EFTP. Establishing `(LOAD 'BROWSER.DCOM)` from the emulated wire is the
one piece of plumbing that would open all 295 at once — the same shape of
win the STP Enumerate work was for Cedar.

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
