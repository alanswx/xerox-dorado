# Clean Interlisp-D distributions, one per release

2026-08-11. Rebuilt from the original Xerox 1108/1186 distribution
floppies. Built by `tools/build_lisp_distributions.py` on top of
`tools/pilot_floppy.py`; media inventory in `docs/xerox-media-catalog.md`.

```
python3 tools/build_lisp_distributions.py MEDIAROOT dorado/build/lisp-dist
```

Layout per release: the reassembled sysout(s) at top level, then
`library/`, `lispusers/`, `other/`, and `MANIFEST.txt` naming the source
disk of every file.

## 1. What came out

| release | disks | sysout (bytes) | chunks | library | lispusers | other |
|---|---:|---:|---:|---:|---:|---:|
| carol-1983 | 8 | 2,688,000 | 3 | 0 | 0 | 54 |
| fugue-1983 | 13 | 2,647,552 | 3 | 136 | 148 | 112 |
| harmony | 10 | 2,786,816 | 3 | 98 | 156 | 11 |
| intermezzo-1985 | 37 | 3,151,360 / 3,962,880 / 4,109,312 | 3-4 | 106 | 148 | 2,603 |
| koto | 21 | 3,251,200 | 3 | 167 | 0 | 2,246 |
| lyric | 21 | **4,824,064** | 5 | 128 | 0 | 2,570 |
| medley-1.0 | 57 | 5,000,704 and 5,022,208 | 5 | 416 | 307 | 128 |
| medley-1.1 | 10 | 5,072,896 | 5 | 94 | 0 | 15 |
| medley-copies | 32 | 5,072,896 | 5 | 262 | 614 | 22 |
| loops | 19 | 4,792,832 and 5,223,936 | 5 | 14 | 0 | 118 |
| xde-5.0 | 26 | (Copilot/Tajo boot files) | 2 | 0 | 0 | 1,200 |

Two known-incomplete, both reported rather than silently written:
`intermezzo-1985/lisp-1978880b.sysout` is missing its first chunk (pages
0-2249: that disk is absent from the set), and Medley's `TajoDove.boot`
is 64 KB short.

## 2. Why the reassembly can be trusted

A sysout too big for one floppy is written one chunk per disk, each
leader page carrying `HUGEPAGESTART`, `HUGEPAGELENGTH` and `HUGELENGTH`
(IRM section 24.10). The medium states the order, so input order does not
matter, and a gap or a short total is detected instead of producing a
plausible-looking bad image.

**Oracle 1 — exact.** The Lyric sysout rebuilt from its five floppies is
**byte-identical** to `chm/archiveorg/_chm-parc_interlisp-lyric/LISP.SYSOUT!1`,
the image the emulator already boots to the Lyric desktop.

**Oracle 2 — informative.** Medley 1.0 rebuilds to exactly 5,000,704
bytes, the size of the CHM copy, but 0.77% of bytes differ. The
differences are confined to **114 pages, all below page 241**, with the
remaining ~9,500 pages identical. That is the signature of a different
*save* of the same release (interface page, initial process state, dates,
file map), not a decode error -- a decode error does not leave 9,500
consecutive pages intact. The floppies are the customer distribution; the
CHM file is PARC's copy.

## 3. Matched microcode

Booting a release needs the Dorado Lisp microcode of the same era.
Collected into `chm/lisp/ucode-by-release/`:

| file | era | md5 distinct? |
|---|---|---|
| `chorus-DORADOLISPMC.EB` | Jan-1983 | yes |
| `fugue4-DORADOLISPMC.EB` | Jul-1983 | yes |
| (already held) `chm/lisp/DORADOLISPMC.EB!1` | Mar-1984 (Fugue.5/6, Carol, Harmony, current) | yes |
| `intermezzo-DORADOLISPMC.EB` | Mar-1985 | yes |
| `koto-DORADOLISPMC.EB` = `jcai-...` | Aug-1985 | identical to each other |
| `lispcore-gc-DORADOLISPMC.EB` | Feb-1986 | yes |
| (already held) `chm/lisp/lyric/DORADOLISPMC.EB!1` | Jan-1987 | yes -- the newest that exists |

Lyric's is the last Dorado Lisp microcode ever built; Medley 1.0 runs on
it (`dorado/Makefile:2665`), which is why Medley boots here at all.

## 4. On "some of them aren't loading because of vmem"

A VP-range scan of the rebuilt sysouts reported Carol, Fugue and Harmony
as having ~730 pages above 2^14 and a max VP of 48841 -- the signature
that killed `Full.sysout!6`. **That reading is wrong and should not be
acted on.** `tools/interlisp-sysout/inspect_sysout.py` is written for
Lyric-era images, and these older ones are laid out differently:
`IFPFPTOVPStart` is **320** for Carol and Harmony against **1023** for
Koto, Lyric, Medley 1.0 and Medley 1.1. A 1983 sysout cannot legitimately
address VP 48841 -- the period `VMem.decl` fixes `LastVirtualPage` at
16383 and notes "assumes 22-bit addresses!" -- and ExtendedVmem did not
exist until 1988.

So the honest position: **Koto, Intermezzo, Lyric, Medley 1.0/1.1 and
LOOPS all parse clean at max VP 16383 and fit a stock Dorado.** For
Carol, Fugue and Harmony the analyser must first be taught the pre-Koto
FPTOVP layout before any claim about their VMEM is worth making.

## 4a. Boot status, per release

| release | sysout | boots? | evidence |
|---|---|---|---|
| **Lyric** | 4,824,064 | **yes** | `verify-lisp` 208,966 px; full-library `Released-Full.sysout!2` 209,188 px; lispusers/colour/XCL checkpoints in `build/good-packs/` |
| **Koto** | 3,251,200 | **yes** | `make run-lisp-koto` -> `run-lisp-koto-color-sdl`, `lisp-koto-color-*.pack` baked, own colour ftp-root |
| **Medley 1.0** | 5,000,704 | **yes** | `make run-lisp-medley`, `lisp-medley-desktop.snap` |
| **Medley 1.1** | 5,072,896 | **yes (new)** | 219,235 px boot; 225,009 px with its 430-file library; `make verify-medley11-library` green |
| medley-copies | 5,072,896 | = Medley 1.1 | sysout byte-identical to 1.1 |
| Carol 1983 | 2,688,000 | no | "Sysout too old for this microcode" with Mar-1984 ucode (section 4d) |
| Fugue 1983 | 2,647,552 | no | same |
| Harmony | 2,786,816 | **loads, then faults** | whole sysout transfers with Mar-1984 ucode; Raid `Invalid address {26,154074}` (section 4d) |
| **Intermezzo 1985** | 4,109,312 | **yes (new)** | 200,912 px desktop with `lisp-4109312b.sysout` + Mar-1985 ucode (section 4d); the recipe had been picking the short rebuild |
| LOOPS | 4,792,832 / 5,223,936 | **not tried** | demo sysouts, not a base release |
| XDE 5.0 | (Copilot/Tajo `.boot`) | n/a | not Interlisp |

Separately, `chm/lisp/current` has a `run-lisp-current-sdl` recipe from
earlier work; not re-verified in this pass.

## 4c. The parallel sweep, and where the old releases actually stop

Ran all five untried releases at once (`make run-lisp-harmony` etc.):

| release | px | outcome |
|---|---:|---|
| **LOOPS** | **205,746** | **boots** -- Nov-1987, i.e. Lyric era, so Lyric's loader IS its match |
| Harmony | 3,155 | Alto Executive banner; sysout never loads |
| Carol 1983 | 3,155 | same |
| Fugue 1983 | 3,155 | same |
| Intermezzo 1985 | 3,055 | same |

Three identical counts was the clue: 3,155 px is the **XEROX Alto
Executive banner**, read off the framebuffer. They do not fail
differently; they never load.

(Section 4d supersedes the diagnosis below: the sweep was run with the
wrong microcode in slot 0112, and Harmony loads once that is fixed.)

The wire is specific. Harmony logs **101 FTP events** against LOOPS'
**39,630**: the retrieve starts, streams ~20 512-byte packets, then
`ack=0` with `in_flight=21/16` -- the client stops acknowledging and the
window fills.

**First hypothesis, and it was not enough.** A release's `Lisp.run` and
its sysout are a matched pair (the warning already at the Koto recipe),
so the 1983-85 sysouts were being read by Lyric's loader. Fetched the
era-matched basics set into `chm/lisp/run-by-release/` (LISP.RUN,
LISP.SYMS, ALTOD1MC.EB for Harmony, Carol, Fugue.6, Intermezzo), added
`make lisp-dist-pack REL=<rel>` modelled on `lisp-koto-color-pack`, baked
the Harmony pack, and booted it with Harmony's own loader:

**3,176 px, still 101 FTP events.** The loader swap changed 21 pixels --
two short lines of new text on the Executive screen -- and nothing else.
So the version pairing is necessary-looking but not sufficient, and the
stall is somewhere else.

## 4d. The stall was the guest refusing the image (2026-08-11)

**Everything above about "the wire is specific" was reading the symptom.
The guest says why, on screen.** `tools/pgm_text.py` on the Carol and
Fugue framebuffers:

```
// Retrieve of sysout {DORADO}<...>(Server:filename LISP.SYSOUT) failed
// Sysout too old for this microcode
```

The client stops acking because it has decided not to take the file. The
21/16 window is what a rejected retrieve looks like from the server side;
there is no protocol bug to find. **Read the screen before the wire.**

The variable that matters is the **microcode in slot 0112**, not the
loader. `chm/lisp/DORADOLISPMC.EB!1` is **Lyric's Jan-1987 build** despite
its generic path, and it is also the emulator's built-in default for that
slot, so every one of these runs was feeding a 1983-85 sysout the newest
microcode that exists. `--eb` can only aim one path and 0110 must stay
aemu, so a repeatable `--boot-file BFN=PATH` was added to place a second
world in a second slot.

With the Mar-1984 build in 0112 (`--boot-file
'112=../chm/lisp/ucode-by-release/harmony-DORADOLISPMC.EB'`), **Harmony
loads**: the whole sysout streams (`file=2786816/2786816`, 5,651 BSP
segments) and the connection closes with the Abort that is the 1983
loader's success path. Lisp starts -- `DORADO_ALTO_OPHIST` shows ~700K
instruction-set-1 dispatches. Then it stops, at 3,009 px, with

```
Raid: Invalid address {26,154074}
```

**That message settles the display question**: Raid is drawn by the guest
through the display path we already render, so a world that "does not
draw" is not what is wrong here. (The `Fugue DCSB` probes in `machine.c`
and `DORADO_DCB_PROBE` were added chasing that theory; the probes are
harmless but the theory is dead.)

**What the message means**, from Xerox's own source
(`chm/lisp/fugue.6/bcpl/lispbcplsources.dm!1_/VMemB.bcpl`): `PageFault` ->
`LookupPage(vp)` returns 0 -> `InvalidAddr` -> `RAIDCode("Invalid
address")`. Lisp touched a virtual page with no entry in the vmem page
map. Decoding it against the rebuilt image:

```
$ tools/interlisp-sysout/inspect_sysout.py build/lisp-dist/harmony/lisp.sysout --va 26:154074
VA {26,154074} -> VP 0o13330 (5848), word 0o74
  *** VP 0o13330 IS ABSENT from FPTOVP -- this address is not in the sysout at all. ***
  nearest mapped VP below: 0o13253    above: 0o13477
```

The image is self-consistent (`IFPNActivePages` 5,443 = its 5,443 pages),
so this is not a truncated reassembly: **Lisp dereferenced an address
that is not in its own image** -- a wild pointer, on microcode we have
never run before. Same shape as the Lyric-era `TgetsMd`/Md-bypass saga.

Booting Harmony on **its own** baked pack (its own `LISP.RUN`, so no
mixed-era loader) gives a **byte-identical framebuffer**. The fault is
deterministic and the loader vintage is not the variable.

| release | ucode 0112 | result |
|---|---|---|
| Harmony | Mar-1984 | sysout loads in full; Raid `Invalid address {26,154074}` (3,009 px) |
| Harmony | Mar-1984, own pack | byte-identical to the above |
| Carol 1983 | Mar-1984, Jul-1983, Jan-1983 | "Sysout too old for this microcode" -- all three (3,155 px) |
| Fugue 1983 | Mar-1984, Jul-1983, Jan-1983 | same, all three |
| Intermezzo 1985 | Jan-1987 | "File not in sysout format" -- a DIFFERENT refusal, see below |
| **Intermezzo 1985** | Mar-1985, full sysout | **boots: 200,912 px desktop** |

Carol and Fugue are refused by **every** DORADOLISPMC we hold, including
the oldest (`chorus-`, Jan-1983), so for those two the microcode vintage
is not the remaining variable. The one combination still untried is their
own era's **Alto-side loader**: `LISP.RUN` lives ON the boot pack, not
behind a flag, so it needs `make lisp-pack-carol` / `lisp-pack-fugue`
first and then `run-lisp-dist ... LISP_DIST_SEED=<that pack>`. (The same
swap made no difference for Harmony -- byte-identical framebuffer -- but
Harmony is not the one being refused.)

**The identical-looking stalls were not identical refusals**, which is the
whole argument for reading the screen. Intermezzo's says *File not in
sysout format*, and the reason was in the recipe, not the machine:
`LISP_DIST_SYSOUT` takes `$(firstword $(wildcard ...*.sysout))`, and that
directory holds four, the alphabetically first being
`lisp-1978880b.sysout` -- **the one section 1 records as missing its first
2,250 pages**, header included. Intermezzo had therefore never actually
been tried.

**Pointed at the right one, INTERMEZZO BOOTS TO ITS DESKTOP -- 200,912
px.** Prompt Window, EXEC Window (`INTERLISP-D 5-Dec-85 ... Please enter
name of system init file`), the Interlisp-D splash with its package list,
and the Space Allocation window; screenshot
`docs/images/lisp-intermezzo-desktop-2026-08-11.png`. It is the fifth
Interlisp release running here, and the first from the rebuilt floppies
that is not Lyric-era.

```
make run-lisp-dist REL=intermezzo-1985 \
  LISP_DIST_SYSOUT=build/lisp-dist/intermezzo-1985/lisp-4109312b.sysout \
  LISP_DIST_UCODE='../chm/lisp/ucode-by-release/intermezzo-DORADOLISPMC.EB'
```

Its sysout streams in full (8,306 BSP segments) and closes on the same
success-path Abort. `LISP_DIST_SYSOUT`'s wildcard now picks the largest
complete image (`ls -S`), not the first alphabetically.

**Greeting it needed two more corrections, and a checkpoint.** That
200,912 px screen is the world waiting at its greet prompt, not an
initialised system:

- **The INIT must be era-matched.** Every seed pack here carries LYRIC's
  INIT, whose first form is `(DEFINE-FILE-INFO ...)` -- Xerox Common Lisp,
  two years newer than this system. Intermezzo breaks into the debugger on
  it: `DEFINE-FILE-INFO - UNDEFINED CAR OF FORM`. Its own distribution
  ships `library/INIT.NONET-1108` (29-Nov-84, `RPAQQ INITCOMS` syntax),
  which is accepted without complaint;
  `chm/lisp/inits/INIT.INTERMEZZO-DORADO` is that file with `{FLOPPY}` ->
  `{DORADO}<>` (77 CR line endings preserved). It is **served, not
  inserted into the pack** -- the prompt asks for
  `{server}<directory>INIT.extension`, so a remote init is what it expects
  and no pack bake is needed.
- **The name must carry its directory.** Bare `{DORADO}INIT.DORADO` is
  silently re-prompted -- twice in a row, which reads exactly like a failed
  transfer and cost a run to tell apart from one. `{DORADO}<>INIT.DORADO`
  logs in and greets. The `<>` is the same empty-directory form the
  working Lyric INIT uses in `DIRECTORIES`.

With both, the Exec shows `{DORADO} Login: Guest (password) *****` then
**`Hi.`** and the `2<-` prompt. `make lisp-intermezzo-snapshot` bakes that
(201,795 px, `build/good-packs/lisp-intermezzo-desktop.{snap,pack}`) and
`make run-lisp-intermezzo-sdl` restores it in seconds; the restored
framebuffer is byte-identical to the bake. `make
run-lisp-intermezzo-cold-sdl` still does the five-minute cold boot.

Note the gate trap: pre-greet is 200,912 px and greeted is 201,795 --
0.4% apart. **Read the EXEC window, not the pixel count.**

**The map's own history of that page confirms the unmapping is
legitimate** (`DORADO_MAP_TRACE=1 DORADO_MAP_TRACE_INDEX=013330`; index
0x16D8 is written exactly five times in the whole run):

```
cyc      17,760,900  pc=0o6366              rp:0000 VACANT -> rp:16D8 occupied   (Initial's identity map)
cyc      31,636,476  pc=0o4114 op=0o177400  rewritten, still occupied
cyc      38,904,464  pc=0o4114 op=0o177400  -> VACANT
cyc      55,481,543  pc=0o4114 op=0o177400  -> rp:16D8 occupied
cyc   2,285,077,426  pc=0o3333 op=0o67402   -> VACANT, and never remapped
```

`pc=0o3333` is DoradoLispMc's resident Nova/BCPL emulator, i.e. the
Alto-side loader tearing down the identity map as it builds Lisp's
virtual memory -- correct, because the page is not in the sysout. So the
unmapping is right and **the later reference is the anomaly.**

**Next:** `DORADO_FAULT_TRACE` to catch the faulting reference itself --
its microcode PC and Lisp context -- which is what tells
wild-pointer-from-our-bug apart from wild-pointer-from-the-image.

## 4b. The loader question, answered

`docs/lisp-distributions.md` previously suggested each release's own
`Lisp.run` in its `other/` directory might be what the older sysouts
need. **There is no `Lisp.run` on any of these distributions.** What the
`other/` disks carry is Dandelion installation material -- `DLion.germ`,
`TajoDLion.boot`, `SAx000Lisp23Initial.db`, `lisp11SAx000Initial.db`,
`Lisp Installation.script`, `LispTool.bcd` -- because a 1108 boots Lisp
natively through Pilot/Mesa, not through an Alto-side loader.

So the floppies give us the machine-independent halves (sysout + library)
and nothing usable as a loader. Our Dorado path is right to keep using
the Alto `Lisp.run` we already have (`chm/lisp/Lisp.run!6`), which is
what booted Medley 1.1. The variables that remain for the untried
releases are the **microcode vintage** (now collected in
`chm/lisp/ucode-by-release/`) and whether that era's sysout accepts a
later loader.

## 5. Next

1. Boot each release with its matched microcode, oldest first, and record
   what happens -- the microcode goes in slot 0112 with `--boot-file`, and
   the guest's own rejection message ("Sysout too old for this microcode")
   is the signal that the pairing is wrong. Open: Harmony's wild pointer
   (section 4d), and Carol/Fugue against the Jul-1983 and Jan-1983 builds.
2. Teach `inspect_sysout.py` the pre-Koto layout (`IFPFPTOVPStart` 320)
   so Carol/Fugue/Harmony can be analysed at all.
3. The Interlisp `Lisp.run` loader is release-specific too --
   `chm/lisp/Lisp.run!6` is what our recipes use. Each distribution's
   `other/` holds its own installation utility and system files; those,
   not the Lyric loader, are what the older sysouts expect.
