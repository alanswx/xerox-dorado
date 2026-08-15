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

## 2a. Validating the conversion against the originals (2026-08-12)

**The archive keeps per-release sysouts with size and CRC32**, so a
rebuild does not have to be trusted -- it can be diffed against the real
thing. `tools/verify_lisp_distributions.py` does this for every release
and exits nonzero on a decode defect.

| rebuild | verdict |
|---|---|
| harmony | **EXACT** = `eris/lisp/harmony/basics/LISP.SYSOUT!15` |
| koto | **EXACT** = `eris/lisp/koto/basics/LISP.SYSOUT!15` |
| lyric | **EXACT** = `phylum/lisp/lyric/basics/LISP.SYSOUT!1` |
| **carol** | same size, **112 bytes wrong** vs `current/LISP.SYSOUT!2` |
| **fugue** | same size, **112 bytes wrong** vs `fugue.6/Lisp.sysout!1` |
| medley-1.0 | different SAVE, not a defect (see below) |

**The carol/fugue defect has an unmistakable signature.** In BOTH
releases: exactly 112 bytes, **every one** ours=`0x00` where the original
is nonzero (zero exceptions, no byte differing the other way), **every
one** at an EVEN file offset -- the high byte of a big-endian word -- and
39 of the first 40 sit exactly at a `<len><name>` boundary in the atom
table. Values run 0x07..0x20, i.e. plausible name lengths.

Compare Medley 1.0, which section 2 calls a different save and which the
measurement confirms: 38,590 bytes differ, **two-directional** (2,763
ours-zero against 2,761 original-zero), even/odd offsets balanced, all
inside pages 36-240. That is what an honest different save looks like;
one-directional zeros is not.

**Ours is provably the corrupt one, no media needed.** For 108 of the 112
bytes, the ORIGINAL's value is exactly the length of the printable atom
name that follows it; ours is zero in all 112. A self-consistent length
table on one side and zeros on the other is not ambiguous.

**The correlation is with the MEDIUM.** harmony/koto/lyric come from
`.IMD`; carol/fugue from `.dmk`. Medley is also `.dmk` but its original
is a different save, so it cannot discriminate -- the defect is evidenced
on two releases, not three, and `dmk_to_image` in `tools/pilot_floppy.py`
is the suspect rather than the convicted.

**Not yet fixed, and deliberately so:** the carol/fugue `.dmk` images are
not on this machine (`chm/lisp/release-floppies/` holds only the
HARMONY-era `.IMD` set), so a fix could not be validated by rebuilding.
The reproduction is: obtain the media, rebuild, and run
`tools/verify_lisp_distributions.py` until carol and fugue read EXACT.

**And this defect is NOT what stops Carol.** Booting the pristine archive
sysout (`current/LISP.SYSOUT!2`, byte-perfect) on the Carol pack reaches
the SAME Raid stop at the same 928 px, with the same opcode counts.
So the conversion bug is real and worth fixing, but the Raid stop is
ours -- see section 4g.

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
| Carol (Jun-84) | 2,688,000 | **loads, runs Lisp, then Raid** | needs its OWN pack; same Raid stop on BOTH `ee7a2f70` and its own rebuilt microcode (sections 4e/4f) |
| Fugue (Apr-84) | 2,647,552 | **loads, then Raid** | same, `make lisp-pack-fugue` |
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

## 4e. Carol and Fugue: how the version check behaves

**RETRACTED HEADLINE (2026-08-12).** This section originally concluded
that the microcode pairing for Carol and Fugue "does not survive in the
archive". **That conclusion tested the wrong variable and is withdrawn.**
Every run in the table below used the LYRIC boot pack, and `LISP.RUN`
lives ON THE PACK, not behind a flag -- so all fourteen ran Lyric's 1987
loader and varied only the ether microcode slot. The decisive control,
run afterwards:

```
harmony sysout + harmony-DORADOLISPMC + LYRIC pack    -> Sysout too old for this microcode
harmony sysout + harmony-DORADOLISPMC + HARMONY pack  -> LOADS (3,009 px)
```

Identical microcode in slot 0112; only the pack differs. **The pack is a
discriminator the matrix below held fixed at the wrong value.** The
loader's own four-way message set says as much -- it distinguishes "too
old for this *microcode*" from "too old for this *Lisp.Run*" -- which
only makes sense if both are independently versioned. Carol and Fugue
have never been tried with their own loaders; `make lisp-pack-carol` and
`make lisp-pack-fugue` build those packs and had not been run.

(Intermezzo is unaffected: it boots on the Lyric pack once slot 0112
carries its own microcode, so the slot genuinely matters too. Both the
loader and the microcode participate.)

**With their own packs, Carol and Fugue LOAD** (`make lisp-pack-carol`,
`make lisp-pack-fugue`, then `run-lisp-dist ... LISP_DIST_SEED=<pack>`):

| release | on the LYRIC pack | on its OWN pack |
|---|---|---|
| Carol (Jun-84) | refused, ~100 FTP events | **loads, 5,429 BSP segments** -> Raid |
| Fugue (Apr-84) | refused, ~100 FTP events | **loads, 5,353 BSP segments** -> Raid |
| Harmony (Dec-84) | refused | loads, 5,651 -> Raid `Invalid address {26,154074}` |

Both stop at 928 px with

```
Raid: Error in uninterruptable system code -- ^N to continue into error handler
```

which is a DIFFERENT stop from Harmony's, and Raid is offering to
continue. **`^N` works** -- `--type $'\016'` reaches it (typetext.c
synthesises Ctrl-<letter> for the 1..032 range), Raid accepts it and
names its caller:

```
Raid: Error in uninterruptable system code -- ^N to continue into error handler
     -1
@^N - Return NIL
Raid: Called from uCode 4726Q
@^N - Return NIL
Raid: Called from uCode 5204Q
@
```

Carol and Fugue produce IDENTICAL screens here (2,068 px), so they stop
for the same reason. **The next step is a source question, not another
run:** what is at IM `0o4726` and `0o5204` in `ee7a2f70`, and at `0o3762`
(Harmony's faulting PC)?  The Lisp microcode source is in
`chm/lisp/harmony/ucode/`.

So the rule for the pre-Intermezzo releases is: era-matched pack AND
era-matched microcode. Intermezzo is the one that does not need the pack
-- Lyric's loader accepts it, and slot 0112 was enough.

What follows is still accurate as a description of the CHECK and of the
archive, just not as a conclusion about what survives.

**The check is the LOADER's, and it is an exact pairing, not a floor.**
`Lisp.run` carries a four-way matrix -- `Sysout too old for this
microcode` / `... for this Lisp.Run`, and `Microcode too old for this
sysout` / `Lisp.Run too old for this sysout` -- and the one we get is
always the first. It is NOT a "use something older" hint:

```
harmony sysout + ee7a2f70 (harmony/basics)   -> LOADS
harmony sysout + f2a18dd6 (lyric, newest)    -> Sysout too old for this microcode
harmony sysout + b9936842 (chorus, OLDEST)   -> Sysout too old for this microcode
```

(Those three came from `--boot-file` runs and are therefore MEANINGLESS
-- see immediately below; the same-message-from-both-ends reading that
once appeared here, "no monotone rule fits", was an artifact of the flag
doing nothing. The check IS monotone.)

## 4f. The .eb version stamp, and building a microcode that never shipped

**The version numbers are NOT in the `.MB`.** The real toolchain builds an
`.eb` in two steps -- LoadMB emits the item array, then a separate
`StampVersions` writes three words into the header. From
`StampD1UCode.cm`:

```
// StampVersions <filename> <RamVersion> <MinBcplForRam> <MinLispForRam>
StampVersions DoradoLispMc.eb 12004 21000 110400
```

They land at **header words 64, 65, 66**; words 5-14 hold the packed name
(`DoradoLispMc.EB`), 3-4 a creation date. `LISP.RUN` reads them BEFORE
loading the microcode -- which is the whole four-message matrix -- while
Initial's own LoadRam ignores the header, which is why every non-Lisp
world we build works without a stamp.

**Read the stamp instead of booting.** Every surviving microcode, by
header word:

| microcode | RamVersion | MinBcpl | MinLisp |
|---|---|---|---|
| chorus | 10402 | 21000 | 106400 |
| fugue.4 | 10410 | 21000 | 106400 |
| **harmony** | **12004** | 21000 | **110400** |
| intermezzo | 13024 | 21000 | 113000 |
| koto/jcai | 13032 | 21000 | 113000 |
| lyric | 13062 | 21000 | 113000 |
| lispcore-gc | 14000 | 21000 | 113400 |

`MinLispForRam` is the minimum SYSOUT version the microcode accepts, so
"Sysout too old for this microcode" means `sysout.LispVersion <
ucode.MinLispForRam`. That predicts the whole 4e matrix with no boots:
Carol's sysout is >= 110400 and < 113000, so harmony's build is the only
fit -- which is what `carol/basics` ships.

**`mb2eb --stamp RAM,BCPL,LISP --name NAME`** now writes it (78 of the
archive's first 80 header words reproduce exactly; the two that do not
are the creation date, deliberately not faked). Without it a rebuilt
microcode carries version 0 and Lisp.run says "Microcode too old for this
lisp.run" -- an artifact of the builder, not a real relationship.

**A release's own numbers are recoverable from its `.MB`.**
`InitLispRegs` pushes all three as compiled constants (`LISP0.mc`:
`T_ and[RamVersion, 177400]c` then `+ and[RamVersion, 377]c`), at image
**0o2032-0o2041**. Extracting harmony's gives 12004/21000/110400 --
exactly its shipped stamp, which validates the method. Carol's Feb-1984
`.MB` gives **12002**/21000/110400.

**So Carol's microcode, which was never built to an `.eb` that survives,
now runs** (`build/ucode-built/carol-stamped.eb`): it passes the version
gate, the full sysout transfers (5,429 BSP segments, closing on the
loader's success-path Abort) -- and then **Lisp never starts**. Zero
instruction-set-1 dispatches; the machine sits at microcode pc 0o234 with
no error printed. That is FURTHER than the harmony-microcode run in one
sense (no Raid stop) and less far in another (no Lisp at all).

**That first Carol build had the WRONG ENTRY POINT.** Insisting on a
byte-exact rebuild found three defects in our builder, of which the third
is the one that matters: no version stamp, item order (LoadMB walks the
`.MB` in IMAGE order emitting REAL addresses), and **the start address is
`01070` for the Lisp microcode, not the `01076` Dorado default**. A build
with `01076` passes the version gate, transfers the whole sysout, prints
no error, and never starts Lisp -- exactly what was observed.

With all three fixed, `mb2eb` reproduces `ee7a2f70` **byte-for-byte**
except the two creation-date words, so the build path is now a verified
reimplementation of LoadMB. Procedure and the three traps:
`docs/rebuilding-lisp-microcode.md`.

**And rebuilt correctly, Carol's own microcode converges on the same
failure.** With start `01070`, Lisp runs (~100K instruction-set-1
dispatches, against zero at `01076`) and stops at the identical 928 px
Raid -- the same place harmony's `ee7a2f70` stops. So the
microcode axis is now CLOSED by two independent routes: the version
bracket says `ee7a2f70` is the only shipped build that fits, and the
era-matched build, reconstructed byte-exactly, fails identically.

**The Raid stop is therefore not a pairing problem.** It is our emulator or
the rebuilt Carol/Fugue sysouts, and since both releases fail the same
way the cause is shared. The next probe is unchanged and now unambiguous:
dump the cons page the microcode reads at `.CNSOD1` and compare it
word-for-word against the same page in the sysout on disk.

**THE MICROCODE IS A FILE ON THE PACK, NOT A FLAG.** `--boot-file
112=PATH` aims the ETHER microcode-boot slot, which a `lisp.run/M` boot
never requests; `lisp-dist-pack` does `--insert '$(LISP_MC_FILE)'
DORADOLISPMC.EB.` and `LISP.RUN` loads it from the disk. An earlier
version of this section reported a 7x2 matrix in which everything was
refused -- every cell of it ran whatever microcode the pack happened to
carry, and identical results across seven files was the tell. **To vary
the microcode, re-bake the pack:**

```
make lisp-dist-pack REL=<rel> LISP_DISTPACK_UCODE=<path.eb> \
     LISP_DISTPACK_PREFIX=build/mx/<tag>
```

**Redone properly (2026-08-12), one pack bake per cell, no `--boot-file`
anywhere. The guest's version check is a TOTAL ORDER and it brackets the
answer from both sides:**

| microcode (crc32) | Carol (Jun-84) | Fugue (Apr-84) |
|---|---|---|
| chorus `b9936842` | Microcode too old for this sysout | Microcode too old for this sysout |
| fugue.4 `a4dca991` | Microcode too old for this sysout | Microcode too old for this sysout |
| *(our build of the Feb-84 `.MB`)* | Microcode too old for this **lisp.run** | -- |
| **harmony `ee7a2f70`** | **LOADS, runs, Raid stop** | **LOADS, runs, Raid stop** |
| intermezzo `7c5cf9ee` | Sysout too old for this microcode | Sysout too old for this microcode |
| koto/jcai `6210ad82` | Sysout too old for this microcode | Sysout too old for this microcode |
| lispcore-gc `39415154` | Sysout too old for this microcode | Sysout too old for this microcode |
| lyric `f2a18dd6` | Sysout too old for this microcode | Sysout too old for this microcode |

Older builds say *microcode* too old; newer builds say *sysout* too old;
exactly one build sits in the accepted window, and it is `ee7a2f70` --
which is precisely the `.EB` that `carol/basics` and `fugue.6/basics`
ship. **So the pairing is correct and unique, confirmed by the guest
itself, and the Raid stop is a real failure with the RIGHT microcode.** That
leaves our emulator or the rebuilt sysouts, and since Carol and Fugue
fail identically the cause is shared.

Note `koto` and `jcai` are byte-identical, `lyric` == the generically
named `chm/lisp/DORADOLISPMC.EB!1`, and `harmony` == `current/`: 7
distinct builds behind 10 paths. `lispcore-gc` and `koto` are the SAME
SIZE (36696) and different content -- match by CRC, never by size.

**Match by CRC, not by
directory name** -- `chm/cross-reference.html` gives size and CRC32 for
every copy, and the mapping is:

| CRC32 | size | the release directories holding it |
|---|---:|---|
| `b9936842` | 36528 | chorus/basics |
| `a4dca991` | 36776 | fugue.4/basics |
| `ee7a2f70` | 36720 | **carol, fugue.5/current, fugue.6, harmony, current, erinyes/fugue.6** |
| `7c5cf9ee` | 36272 | intermezzo/basics |
| `6210ad82` | 36696 | jcai, koto, phylum/koto, qv/idl |
| `39415154` | 36696 | lispcore/gc |
| `f2a18dd6` | 36664 | lyric/basics |

Six directories -- Carol's among them -- hold the SAME file, `ee7a2f70`.
That is worth knowing when reasoning about provenance, but it no longer
supports any claim about what survives: with the era-matched pack in
place, `ee7a2f70` is exactly the microcode that loads Harmony, and Carol
may well load with it too once Carol's own loader is on the disk.

Two things make this checkable without booting:

- **The sysout states its own version** at interface-page **word 49**,
  which orders strictly by release: Fugue 662, Carol 667, Harmony 674,
  Intermezzo 697, Koto 742, Lyric 786.
- **And its own date**, as a plain string: `INTERLISP-D 20-Jun-84` for
  Carol, `9-Apr-84` Fugue, `20-Dec-84` Harmony, `5-Dec-85` Intermezzo,
  `7-Feb-86` Koto. (Our directory names -- "carol-1983", "fugue-1983" --
  are a year off; they came from the tool, not the medium.)

**To calibrate an unknown microcode, probe it with a sysout of known
version.** That is how the mislabelling above was caught: Harmony loads
only with `ee7a2f70`, so any build that refuses Harmony is not the
Harmony-era build whatever directory it sits in.

The section-3 microcode table dates every build by the directory it came
from. Those dates are unverified assertions and at least one is
contradicted by behaviour -- treat the CRCs above as the identity.

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

**The faulting reference, caught (2026-08-12).** `DORADO_MAP_TRACE=1
DORADO_MAP_TRACE_INDEX=013330` on the ERA-MATCHED PACK (see 4e -- on the
Lyric pack the sysout is refused and any trace is of a run that never got
here):

```
cyc 2,285,077,426  MAP_TRACE  pc=0o3333 op=0o67402 mb=34  -> VACANT, never remapped
cyc 2,542,723,011  PAGEFAULT  pc=0o3762 pcf=0o47 op=0o2223 mb=10
                              br=5500000 mar=54074 va=5554074 idx=16D8 write=0
```

`va=0o5554074` is exactly `{26,154074}`, the address Raid prints, so this
IS the fault behind the message. The reference is a **read at microcode
PC 0o3762 while executing Lisp opcode 0o2223, through MemBase 10 (BR =
0o5500000) at offset 0o54074** -- 257 M cycles after the loader vacated
the page at pc 0o3333 (DoradoLispMc's resident Nova/BCPL emulator, which
is the loader tearing down the identity map as it builds Lisp's virtual
memory, and correct because the page is not in the sysout).

So the unmapping is right and the reference is the anomaly. Next: what is
at IM 0o3762 in `harmony-DORADOLISPMC.EB`, and what is opcode 0o2223 --
the Lisp microcode source is at `chm/lisp/harmony/ucode/`.

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

## 4g. The Raid stop is ours, not the artifact (2026-08-12)

The decisive control: boot Carol from the **archive** sysout
(`chm/lisp/sysout-by-release/current-LISP.SYSOUT!2`, byte-perfect, not
our rebuild) on the Carol pack. Result: **928 px, the same
`Raid: Error in uninterruptable system code`**, with
instruction-set-1 counts matching our rebuild's to within one dispatch
(`020=30495` against `30494`).

So every artifact explanation is now excluded:

| suspect | how it was excluded |
|---|---|
| microcode pairing | version bracket says `ee7a2f70` is the only fit; Carol's OWN rebuilt microcode fails identically (section 4f) |
| the loader / pack | Harmony's sysout loads fine ON Carol's pack |
| our floppy conversion | the byte-perfect archive sysout fails identically |

**CORRECTION (2026-08-12): the check is NOT `badcons` -- MEASURED.**
`DORADO_PCDIS=4725,4725` records **zero** executions of `.CNSOD1` in a run
that ends in the Raid stop, so that check never fires. The earlier
identification assumed
identification assumed `uCodeCheck` saves `CIA+1`, so that Raid's `4726Q`
pointed one past `.CNSOD1`. The macro is `SaveLink_ Link, Branch[
UCODECHECKPUNT]` (`LISPDEFS.mc`) -- it copies the **Link register**,
which holds the last CALL's return address. That is what "Called from
uCode" means, and under that reading the two numbers name the CALLERS:

| reported | real -> image | label |
|---|---|---|
| `4726Q` | 0o4072 | **`.RPLACDLOCAL+1`** |
| `5204Q` | 0o4645 | **`.NOPUSH+0`** |

Three facts support the caller reading: `.RPLACDLOCAL` (replace-cdr) is
exactly a routine that allocates cons cells; the `.CNSOD1` guard was
traced executing 13 times reading an EVEN value every time; and
`.CNSOD1` itself never executes at all. **Which uCodeCheck actually
fired is therefore unknown**; what is known is that the stop happens in
the cons/list machinery with `.RPLACDLOCAL` on the stack.

**What the microcode actually sees.** Tracing the guard instruction
(real `0o4762`, `Branch[.+2, R even], T_ LTEMP3_ (LTEMP3) - (400c)`) with
`DORADO_PCDIS=4762,4762` shows the cons free-list header counting down
normally and then reading ZERO:

```
rv=005560  rv=005176  rv=004420  rv=004150  rv=003540  rv=003144
rv=002636  rv=002244  rv=001622  rv=001220  rv=000640  rv=000000   <- fires
```

The free list is EXHAUSTED -- the count reaches zero after 13
allocations from the same page (`BrLo` is loaded with a constant
`T=0o3400` every time). Note that a zero read is EVEN, so it takes the
guard's branch and should reach `.consfail` ("trap out if no cells
left"), NOT `badcons`. Whether running out of cells here is itself the
bug, or a symptom of allocation never being replenished, is the open
question.

**Next:** that word is fetched by `FETCH_ 0s` against a base register the
microcode has just loaded (`LTEMP1_ BrLo_ T`). So dump the VA and compare
what our memory path returns against what is on the pack at that VA --
the question is now narrow: did the page get written to the vmem file
correctly, and does our fetch read back the same words.
