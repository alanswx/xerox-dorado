# Interlisp-D in the CHM archive — what exists, per release

Surveyed 2026-08-01 against `xeroxparcarchive.computerhistory.org`, to answer
three questions: what does each Interlisp-D release actually contain, what
should we ask CHM to add, and does getting Lisp software working require
building Harmony.

**Short answer to the third question: no.** The Lyric-era library exists and
loads today. Details in §4.

---

## 1. Correcting the previous finding

`docs/demo-plan.md` §4 said the archive holds no Lyric-era library and that
every Interlisp package is a 1982-84 Harmony build. That was wrong, and the
mistake is worth naming because it is easy to repeat:

- **The compiled-file extension changes with the release.** Interlisp-D
  compiles to `.DCOM` through Koto (1986) and to **`.LCOM`** from Lyric
  (1986-87); the Common Lisp compiler in the same era emits **`.DFASL`**.
  Searching for `.DCOM` finds only the pre-Lyric world by construction.
- **Directory roots can be empty while the content sits one level down.**
  `phylum/lispusers/` lists zero files and two subdirectories, `koto` and
  `lyric`. A scan that only counts file rows in the root reports "nothing
  here" for a tree holding 871 files.

Both errors were mine, in the same session, and both produced a confident
"it is not in the archive" about material that is in the archive.

---

## 2. Release-by-release totals

Counting unique `name!version` under each release tree, two levels deep.

| release | era | files | size | dominant kinds |
|---|---|---:|---:|---|
| Fugue | 1982-83 | 530 | 40.3 MB | 295 no-ext, 28 PRESS, 23 MESA, 22 MC, 13 EB |
| Carol | 1983 | 684 | 35.1 MB | 254 no-ext, **232 DCOM**, 37 PRESS, 28 TTY |
| Chorus | 1984 | 20 | 2.3 MB | 9 EB, 2 RUN — a boot-set fragment, not a release |
| Harmony | 1983-84 | 1226 | 75.4 MB | **389 DCOM**, 291 no-ext, 89 MC, 48 TEDIT |
| Intermezzo | 1985 | 1014 | 51.5 MB | 388 no-ext, **381 DCOM**, 78 TEDIT, 42 PRESS |
| Koto | 1985-86 | 4833 | 109.4 MB | 1498 DISPLAYFONT, 1385 WD, **626 DCOM** |
| **Lyric** | **1986-87** | **855** | **64.4 MB** | 282 no-ext, **217 LCOM**, 164 TEDIT, **86 DFASL** |

"no-ext" is normal: Interlisp source files carry no extension (`LLBASIC`,
`DTDECLARE`, `PROC`). `WD` in Koto is window-dump images; `DISPLAYFONT` is
screen fonts.

### Where each release lives

| release | path |
|---|---|
| Fugue 2/4/5/6 | `eris/lisp/fugue.N`, also `erinyes/lisp/fugue.N` |
| Carol, Chorus, Harmony, Intermezzo | `eris/lisp/<name>` |
| Koto | `eris/lisp/koto` + `phylum/lisp/koto` |
| Lyric | `phylum/lisp/lyric` |

Lyric is the one release that is **not** under `eris/lisp/`. That alone
explains why it was missed.

### Sub-tree breakdown

| release | sources | library | lispusers | ucode | patches | doc |
|---|---:|---:|---:|---:|---:|---:|
| Fugue.6 | 11 | 0 (1 subdir) | 0 | 32 | 9 | 46 |
| Carol | 283 | 252 | 39 | 0 | 5 | 31 |
| Harmony | 309 | 385 | 8 | 12 | 59 | 59 |
| Intermezzo | 307 | 458 | 28 | 0 | 60 | 31 |
| Koto | 293 | 410 | 690 | 0 | 23 | 0 (5 subdirs) |
| Lyric | 2 | 227 | 408 | 0 | 53 | — no doc dir |

The Lyric `sources` count of 2 is **not** a gap: the Lyric-era system sources
are on the development server as `eris/lispcore/sources` — 1100 files,
52.5 MB, 1057 of them dated 1986. `chm/lisp/lispcore/sources` already holds a
sample (`LLBASIC!62`, `LLDATATYPE!75`, `AINTERRUPT!33`).

---

## 3. Every release can boot on a Dorado

Checked `basics/` for each release. The Dorado needs four things: the Dorado
Lisp microcode, a sysout, the Alto-side loader `Lisp.run`, and a symbol file.

| release | Dorado microcode | sysouts in basics | Lisp.run |
|---|---|---|---|
| Fugue.6 | `DORADOLISPMC.EB!1`, `DORADOLISP.MB!1` | 4 (2.3-4.1 MB) | !1 |
| Carol | `DORADOLISPMC.EB!1` + 3 make-disk `.CM`s | 4 (2.3-4.3 MB) | !1 |
| Chorus | `DoradoLispMc.EB!1` | 1 (1.8 MB) | !1 |
| Harmony | `DORADOLISPMC.EB!1`, `DoradoLisp.MB!1` | 8 (2.8-4.7 MB) | !1 |
| Intermezzo | `DORADOLISPMC.EB!3`, `DoradoLisp.MB!3` | 5 (3.1-5.2 MB) | !1-3 |
| Koto | `DORADOLISPMC.EB!1` | 5 (3.3-9.3 MB) | !1-5 |
| **Lyric** | `DORADOLISPMC.EB!1` | 5 (4.8-9.5 MB) | !1-6 |

Nothing is missing for booting any of them. `phylum/lisp/lyric/basics/
LISP.SYSOUT!1` is **4,824,064 bytes** — byte-for-byte the size of the sysout
we already boot (ours came from archive.org), and `Lisp.run!6` /
`DORADOLISPMC.EB!1` are the pair the Makefile already points at. **Our Lyric
setup is the authentic Lyric release set.**

Also worth knowing: Harmony, Intermezzo and Koto each ship `Full.sysout` and
`DEMO.SYSOUT` builds substantially larger than their `LISP.SYSOUT` — those
are the ones with the library preloaded, and are a shortcut to "Lisp with
software in it" that needs no pack editing at all.

---

## 4. Determination: Harmony is not needed

The blocker was never the archive. It was that we were loading Harmony-era
`.DCOM` files into a Lyric sysout.

**Verified end to end, 2026-08-01.** Six Lyric `.LCOM` packages were placed on
the Lyric pack (`make lisp-lispusers-pack`), a checkpoint was baked from a
cold boot of that pack, and Lisp loads them:

```
2<-(LOAD '{DSK}GREP.LCOM)          2<-(LOAD '{DSK}HRULE.LCOM)
Loading {DSK}GREP.LCOM             Loading {DSK}HRULE.LCOM
compiled on 17-Dec-86              compiled on 27-Nov-86
GREPCOMS                           HRULECOMS
                                   {DSK}HRULE.LCOM
```

against the previous result for the same machinery with a Harmony `.DCOM`:

```
Loading {DSK}BANNER.DCOM
compiled on 30-Nov-83
Bad compiled function
```

Gate: `make verify-lisp-lispusers`, which reads Lisp's reply off the
framebuffer and distinguishes loaded / not-found / wrong-vintage.

**So the recommendation is: stay on Lyric.** Building Harmony would work --
its boot set is complete and its 385-file library matches its own vintage --
but it buys nothing we do not already have, and it would cost a second world
bring-up plus a new checkpoint and gate.

**What is now reachable on Lyric, without asking anyone for anything:**

| source | files | notes |
|---|---:|---|
| `phylum/lisp/lyric/library` | 102 LCOM | the released library: SKETCH, TEDIT extras, chat, printing |
| `phylum/lisp/lyric/lispusers` | 83 LCOM + 47 DFASL | contributed: PACMAN, CALENDAR, PLOT, ANSICHAT, GREP |
| `phylum/lispusers/lyric` | 97 LCOM + 49 DFASL | later contributions, 1987-88, incl. a `pcl` subdir |
| `phylum/lisplibrary/lyric` | 25 LCOM | 1987-88 |
| `phylum/lisp/lyric/patches` | 15 LCOM + 8 DFASL | bug fixes, incl. `lyric-patch-1` |

That is **285 distinct Lyric-vintage `.LCOM` files** (6.0 MB), all now
mirrored to `chm/lisp/ftp-root/`.

**The demo, as built (2026-08-01).** 194 of them fit the pack -- the limit is
free pages, ~4800 once `LISP.VIRTUALMEM.` (15002 pages) and the Alto OS files
are counted, with each file costing a leader page plus `ceil(bytes/512)`. The
set is `dorado/lisp-lispusers-packages.txt`. Load-tested individually:

| package | result |
|---|---|
| `GREP.LCOM` | loads |
| `HRULE.LCOM` | loads, returns `{DSK}HRULE.LCOM` |
| `PACMAN.LCOM` | loads, `PACMANCOMS` |
| **`SKETCH.LCOM`** | **loads, and resolves its own dependency chain off `{DSK}`** -- SKETCHELEMENTS, SKETCHEDIT, SKETCHOBJ, SKETCHBMELT, SCALEBITMAP, each reporting its own 1986-87 compile date |

SKETCH is the interesting one: the first attempt died with `SIMPLE: ERROR /
in DOFILESLOAD: File MATMULT not found on {DSK}`, which is how we learned the
search path already includes `{DSK}` -- the file was simply absent. With
`MATMULT.LCOM` on the pack the whole chain resolves unattended.

**Two limits found, both honest.** A batch of seven more (MASTERSCOPE,
GRAPHER, DEDIT, CALENDAR, PLOT, SOLITAIRE, FILEBROWSER) produced four error
windows -- three `SIMPLE: ERROR`, one `INTERLISP: ERROR` -- on dependencies
that did not fit in the 194. **The import closure is the cost here, exactly
as it is for Cedar Bringover**, and the pack cannot hold all 285. And
`(IL:SKETCH)` echoed without opening a window: loading a package is proved,
*driving* one is not, and the entry points are documented in the Lyric
manuals that are the one real archive gap (SS5).

Note the Exec is **XCL (Xerox Common Lisp)**, not the Interlisp Exec:
`(FILESLOAD SKETCH)` answers `Undefined car of form: FILESLOAD`. `LOAD`
works unprefixed; Interlisp functions need `IL:`.

---

## 5. What to ask CHM for

Being deliberately conservative, because two "it is missing" claims in this
project have already turned out to be search errors. Each item below was
checked in more than one place.

**1. Lyric release documentation.** `phylum/lisp/lyric` has no `doc`
directory at all, and no release notes or users guide. Every other release
has one: Harmony 59 files, Intermezzo 31, Carol 31, Koto a
`doc/releasenotes` with 8. The manual sets in `eris/lispmanual` (377 files)
and `erinyes/lispmanual` (321) stop at 1985 — they document the Intermezzo
era. The only Lyric-specific prose in the archive is two beta cover letters
in `phylum/lisp/lyric/cover-letters`. **Ask: Lyric release notes, users
guide, and the "what's new in Lyric" documents.** This is the one clear,
well-evidenced gap.

**2. ~~Anything post-Lyric — Medley.~~ CORRECTED 2026-08-01: Medley 1.0 is
in the archive.** `phylum/medley/1.0` holds two bootable sysouts --
`LISP.SYSOUT!1` (5,000,704 bytes) and `Released-Full.sysout!1` (6,134,784),
both 09-Nov-1988 -- plus a `patches` subdirectory, and `phylum/medley` has a
1989 internal-library copylog. My original claim that Medley "does not appear
anywhere in this archive" was another root-count error: I never looked under
`phylum/` for a `medley` directory at all.

So there is nothing to ask for here, and there is something to TRY: Medley
1.0 is one release past Lyric and its sysout is a plausible future world.
Whether it runs on a Dorado is unknown -- by 1988 the D-machines were the
target -- and `phylum/medley/1.0` has no Dorado microcode of its own, so it
would have to run on Lyric's `DORADOLISPMC.EB!1` or not at all.

**3. A Dorado-installed Lisp disk pack, if one survives.** Everything we
have is the file-server view: sysouts, microcode and a loader that we must
assemble into a pack ourselves. An actual installed Dorado Lisp pack image
would be an independent check on our `dsk2trident`/`altofs` reconstruction.
Low expectation -- pack images rarely survive -- but cheap to ask.

**Explicitly NOT worth asking for**, having checked:

- Lyric sources — present as `eris/lispcore/sources` (1100 files, 1986).
- The Lyric library — present, ~320 compiled files across four trees.
- Dorado microcode for any release — every release has it.
- `eris/lispusers` completeness — 295 files, all present; it is simply a
  Harmony-era tree, not an incomplete Lyric one.

---

## 6. Reproducing this survey

`tools/fetch_lispusers.py --list` enumerates one directory. The survey above
walked `.index.html` two levels deep under each release root and parsed the
`name!version owner (Original) size date` rows. Three practical notes:

- Always enumerate **subdirectories** as well as file rows; an empty root is
  not an empty tree.
- Match on the **release's own extension** (`.DCOM` / `.LCOM` / `.DFASL`),
  not on a single one.
- The index is case-sensitive in places (`Lisp.run` vs `LISP.RUN`,
  `.dfasl` vs `.DFASL`); match case-insensitively.
