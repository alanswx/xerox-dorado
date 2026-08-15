# The Smalltalk boot file: DoradoInitialSmalltalk.eb reconstructed

2026-08-11. How the Dorado's Smalltalk boot files are built, why the
"missing bootstrap handoff" was never missing, and what `mb2eb -s` now does.

Companion to `docs/new-os-pairs-tasks.md` Task B (the Smalltalk-80 image
question, still open) and `docs/CONTINUE-HERE.md`.

## 1. The claim that was wrong

A bring-up attempt concluded that "the exact `DoradoInitialSmalltalk.eb`
composition listed in the release directory is unavailable" and that the
next step was "reconstructing the exact 1984 InitialSelect + DSemu
bootstrap." Both halves of that are already in the tree.

`chm/dorado/dsemu-src/DSemuRelease.cm` -- Xerox's own release script,
13-Jul-1983 -- ends with the two commands that build the boot files:

```
LoadMB/e DoradoSmalltalk.eb/o        DSemu.mb 1076/s
LoadMB/e DoradoInitialSmalltalk.eb/o InitialSelect.mb 406/s DSemu.mb 1076/s
```

`docs/CONTINUE-HERE.md:1456` has quoted those two lines all along.

## 2. What LoadMB's switches mean

From LoadMB's own source, `chm/doradomicrocode/loadmb/LoadMB.mesa!7.txt`:

| token | meaning |
|---|---|
| `/e` | output format = etherBootable (`.eb`) |
| `/o` | the preceding filename is the OUTPUT file |
| `/v` | verify |
| a bare number | **octal**; sets `startAddr` of the **preceding** source file |

`B/E/O/V` are the only letters in the switch `SELECT`; the number does
the real work (`lastSrcFile.startAddr _ Convert.CardFromRope[this, 8]`).
A file with no number gets `defaultStartAddress _ 1076B`.

**Output layout.** `WriteEBHeader` once, then for each source file in
order: that file's items followed by **its own End item**, carrying
**its own start address** and its own balancing checksum --
`AppendEndItem[nextItemAddr, srcF.startAddr]` inside the `FOR srcF`
loop, with `ReadMBFile` rebuilding the item array from its base each
time. So:

```
[EB header] [InitialSelect items] [End start=0406] [DSemu items] [End start=01076]
```

Two independently-terminated microcode loads in one boot file. The
"missing handoff" was a file structure, not a lost artifact.

One quirk not worth reproducing: the flush writes the *running*
`totalItems` from the buffer base, so a real multi-file `.eb` carries a
tail of zero items after the last End item -- inert padding from buffer
reuse. `mb2eb -s` does not emit it.

## 3. What 0406 is

`chm/doradosource/BootstrapSources.dm!12_/InitialSelectMain.mc` (Taft,
19-Jun-1982) is the top-level module for "the various flavors of
microcode that can be installed on the Initial area of the disk". Its
entry-point table:

```
Set[InitialEtherMesaEntryLoc, 400];       T <- 110C
Set[InitialEtherSmalltalkEntryLoc, 401];  T <- 111C
Set[InitialEtherLispEntryLoc, 402];       T <- 112C
Set[InitialEtherCedarEntryLoc, 403];      T <- 113C
Set[InitialEtherTestEntryLoc, 404];       T <- 114C
Set[InitialDiskEntryLoc, 405];            T <- 177400C
Set[InitialOverlayEntryLoc, 406];         T <- 177401C
```

`0406` is **InitialOverlayEntry**, documented in the same header as:
"loads the next microcode overlay from the same `.eb` file that the
currently-running microcode came from." The overlay IS segment 2.

(The same table independently confirms the boot-file numbers our
Ethernet server already uses: `111B` = Smalltalk. See
`docs/ethernet-architecture.md`.)

## 4. The handoff, and why it needs no re-fetch

The overlay is **already in memory**. `ISelCommon` captures the pointer
on entry, before anything can clobber it:

```
ISelCommon:
	RBase<- RBase[AEmRegs];
	DummyRef<- LRItem, B<- MD;	* Capture start adr of next overlay, if any
	Q<- VALo;
...
LoadFromOverlay:
	T<- Q, Branch[CallLoadRam];
```

with Taft's comment at `LoadFromOverlay`: "Q = address of first Item in
that overlay. Note that the entire enclosing .eb file has already been
checksummed by the guy who loaded us."

So the sequence is:

1. The boot path loads the **whole** `.eb` into VM at `1400B`.
2. `CheckChecksumAndLoad` sums the entire loaded region and requires
   zero. Two segments that each sum to zero total zero -- which is why
   per-segment checksums are safe.
3. `LoadRam` consumes items until the first End item, then jumps to its
   start address, `0406`, leaving `LRItem` pointing just past it.
4. InitialSelect at `0406` recovers that pointer with the
   `DummyRef` + `VALo` readback, takes the `R odd` branch on `177401C`
   to `LoadFromOverlay`, and calls `LoadRam` on segment 2.
5. `LoadRam` consumes segment 2 and jumps to `01076` -- DSemu.

Note step 1: the loader must deliver the file **to EOF**, not stop at
the first End item. Ours does -- `eth_queue_boot_replies`
(`dorado/src/ethernet.c`) checks word 0 == 1, seeks past the 512-byte
header, and reads to EOF.

Note also that `LoadRam` lives at a fixed page and reserves it
(`IMReserve[LoadRamPage, 0, 100]`, `BranchExternal[LShift[LoadRamPage,
6]]`) -- which is why `mb2eb` skips items in `07600..07677`.

## 5. `mb2eb -s`

`dorado/src/mb2eb.c` now has three modes:

```
mb2eb in.mb out.eb [start_addr_octal=1076]
mb2eb -l out.eb start_octal layer1.mb [layer2.mb ...]
mb2eb -s out.eb in1.mb start1_octal [in2.mb start2_octal ...]
```

`-l` **merges** layers into one image with one start (what a complete
Alto-emulator world needs). `-s` **concatenates** independent loads,
one per `.mb`, each with its own start and End item (what a multi-file
LoadMB `.eb` is). They are not interchangeable. Starts are octal in
`-s`, matching LoadMB's `NNN/s`.

Both boot files are built by `make worlds`:

```
worlds/dorado-smalltalk.eb          31,832 B  1 segment,  start 0o1076
worlds/dorado-initial-smalltalk.eb  36,512 B  2 segments, starts 0o406 + 0o1076
```

## 5a. Two real mb2eb bugs, found with a Xerox oracle

`.eb` files we build can be checked against Xerox's own, because we hold
a **matched pair**: `chm/dorado/Cedar.mb!6` (17-May-1984 15:49:08) and
`chm/dorado/CedarDorado.eb!6` (15:49:19) -- eleven seconds apart, the
same LoadMB run. Feeding the `.mb` to `mb2eb` must reproduce the `.eb`.

It did not: **233 of 3857 common items differed.** Two causes, both
"we emitted zero where LoadMB emits a computed value":

- **IM bad-parity flags dropped.** The CSItem control nibble is
  `pe020, rstk0, pe2131, blk` (MSB..LSB) and LoadMB copies `pe020` /
  `pe2131` straight out of the `.MB` (`IMWord` fields `2:2` and `2:3`,
  which the model-1 unshuffle puts at `iw2` bits 13 and 12 = our
  `brk_p`). We wrote 0 for both, with the comment "parity bits 0".
  Those bits mark a microinstruction stored with **deliberately bad
  parity** -- how a trap slot is armed. Emitting 0 silently disarms it.
  Xerox's `IM[0o1614]` control word is `0o120000` (both set); ours was
  `0o0`.
- **IFUM parity bits never computed.** LoadMB recomputes `p0/p1/p2`
  into IFUM word1 before emitting each item, over masked subsets of
  word0/word1 (`ifumW0P0` = `01400`, `ifumW1P0` = `0317`, `ifumW0P1` =
  `0377`, `ifumW1P1` = `0`, `ifumW0P2` = `02000`, `ifumW1P2` =
  `0107460`), using `NotOddParity` and this comment: *"Despite what the
  manual says, the hardware really wants to see even parity - hence the
  'not' in the following statements."* We passed the `.MB`'s word
  through untouched, losing bits 12-14.

Both are fixed in `mb2eb.c` (`not_odd_parity`, `ifum_word1_with_parity`,
and the control nibble). **Result: 0 of 3857 items differ** -- our
output now matches Xerox's LoadMB exactly. The only remaining
difference is the 64 `LoadRamPage` IM items, which LoadRam ignores by
design and we skip deliberately.

## 6. Validation

Parsed back out of the generated files:

| file | segment | items | start | checksum sum |
|---|---|---|---|---|
| `dorado-initial-smalltalk.eb` | 1 (`InitialSelect.mb!1`) | 585 = 584 IM + End | `0o406` | **0** |
| | 2 (`DSemu.mb!1`) | 3915 = 3402 IM + 512 IFUM + End | `0o1076` | **0** |
| `dorado-smalltalk.eb` | 1 | 3915 | `0o1076` | **0** |

The format reading is confirmed against a genuine Xerox artifact, not
just self-consistent: the shipped `chm/microcode/SmalltalkDorado.eb!1`
parses with the same reader as one segment, start `0o1076`, checksum
summing to exactly 0 -- 3962 items against our 3915, the expected skew
between the 12-Jul-1983 drop and `DSemu.mb!1` of 16-Apr-1984.

Gates, after both the segment mode and the parity fix:

- **Galaxian byte-identical.** `worlds/aemu.eb` changes (it now carries
  the parity bits), so it was A/B'd: same 121,602 px and a
  byte-identical framebuffer against the pre-fix `.eb` on the same
  command.
- **11/11 test suites pass.**
- **Smalltalk 124,945 px**, framebuffer byte-identical to the shipped
  `.eb`'s.

## 7. Acquired

`chm/doradomicrocode/initial/InitialSelect.mb!1` -- 11,024 bytes, from
`_cd8_/doradomicrocode/initial/`. We previously had only the source
(`chm/doradosource/BootstrapSources.dm!12_/InitialSelectMain.mc.html`).

## 7a. Does it boot? Yes -- and the two-segment file is not what boots

Both files were run through `make verify-smalltalk`'s exact command
(same pack, same `Bootfrom xmsmall.boot`), substituted for the shipped
`chm/microcode/SmalltalkDorado.eb!1`:

| served `.eb` | result |
|---|---|
| `worlds/dorado-smalltalk.eb` (1 segment, `01076`) | **124,945 px** -- the Smalltalk-76 desktop, and the framebuffer is **byte-identical** to the shipped `.eb`'s |
| `worlds/dorado-initial-smalltalk.eb` (2 segments) | 0 px |

Two things follow.

**Our `mb2eb` build of `DSemu.mb!1` boots.** Two independent microcode
builds -- Xerox's `.eb` of 12-Jul-1983 and ours from the `.mb` of
16-Apr-1984 -- converge on a byte-identical screen. `CLAUDE.md`'s
"our `mb2eb` build of `DSemu.mb!1` render[s] 0 px" (2026-07-25) is
stale: it predates the 2026-07-26/28 Smalltalk fixes and was never
re-tested against them.

**The two-segment file was never the ether-boot artifact.** The release
script says so in its last three lines:

```
Ftp Indigo conn/c Dorado store/c DoradoSmalltalk.eb DoradoInitialSmalltalk.eb
Ftp Nebula store/c DoradoSmalltalk.eb
```

Both go to the release directory on Indigo; **only `DoradoSmalltalk.eb`
is stored on Nebula**. `DoradoInitialSmalltalk.eb` is for installation
on the **Initial area of the disk** -- InitialSelectMain's own opening
sentence -- where InitialSelect is entered at `0406` and pulls its
overlay from the copy already in memory. Feeding it to the Ethernet
microcode-boot path is not a configuration that ever existed, so 0 px
there is not evidence of a defect. Diagnosing it further is only worth
doing if we implement disk-installed Initial.

## 7b. Smalltalk-80: the trap at 70000 explained, and the route closed

The Dec-1980 ST-80 V1 kit on `Smalltalk14.bfs!1` (`st80dec16.im` +
`d80load.run`) looked like the most promising ST-80 route, because
`d80load.run` is an **Alto** program and we boot Alto worlds. A bring-up
attempt got the pack converted, the Alto Executive up, `d80load` to the
image prompt -- and then "traps at 70000" on loading `st80dec16.im`.

That trap is not a bug. It is four facts meeting.

**(1) Alto ST-80 V1 runs on Alto RAM microcode, and needs 3K of control
store.** The loader's symbol table (`d80load.syms`) has `loadMicrocode`
and `D80RamImage`. Ken Shirriff, who ran Smalltalk on the restored YC
Alto, states the requirement independently: *"Smalltalk-80 required 3K
of control RAM, while Smalltalk-76 uses only 1K."*

**(2) The microcode is almost certainly NOT missing -- it is inside
`d80load.run`.** An earlier draft of this section claimed `d80ram.BR`
was a missing microcode file. That was wrong. `d80load.syms` lists the
**BCPL object modules linked into the program**: `d80aux.BR`,
`d80check.BR`, `d80debug.BR`, `d80disk.BR`, `d80ether.BR`,
`d80float.BR`, `d80init.BR`, `d80load.BR`, `d80prim.BR`, `d80ram.BR`,
`d80term.BR`, plus the library modules `gp.BR`, `mdi.BR`,
`template.BR`. That is the ordinary Alto `.syms` convention -- our own
`chm/lisp/Lisp.syms!4` lists 76 such entries (`AltoQueue.BR`,
`AltoTimer.BR`, `BFSInit.BR`, ...), and `gp.br` / `mdi.br` /
`template.br` are present in the xeroxalto archive as the shared
library modules they are. So `d80ram` is the loader's own
RAM-handling module, already linked in, and `D80RamImage` is a symbol
inside it -- the microcode image as embedded data. Nothing to hunt for.

**(3) `70000` is the Alto RAM-microcode trap space, and the Dorado's
Alto emulator traps it by design.** `chm/dorado/aemu-src/ATraps.mc`:

```
EmIFUTrap[160, Trap17, 01];	* 70000-70377
```

with `JMPRAM: Branch[NPTrap]`, `RDRAM: * always return 0`, and
`WRTRAM: IFUJump[0]` -- WriteRAM is accepted and silently discarded,
ReadRAM answers 0, JumpRAM traps. So `loadMicrocode` appears to succeed
and the first dispatch into the RAM range traps. `70000` is the very
first entry of that table.

**(4) DSemu re-uses that same space for Smalltalk-76.** In
`chm/dorado/dsemu-src/SMTRaps.mc` the corresponding lines are commented
out -- `*EmIFUTrap[160...]` `*[161]` `*[162]` `*[164]` `*[165]` -- so
`70000-71377` and `72000-72777` are claimed by the **ST-76** instruction
set. Under DSemu an ST-80 image's microcode call is not trapped; it is
dispatched into Smalltalk-76 microcode, which is worse than a trap.

**And DSemu is Smalltalk-76, definitively.** `SMTRaps.mc` line 49:

```
	Stack_ 52400C, IFUJump[0], At[SD400, 14]; * Smalltalk 76
```

`52400C` = engineering 5 (Dorado), build 5. The VERS build enumeration
in the same comment block allocates a **separate build 6** for the other
Smalltalk -- and the two copies disagree on which one it is:
`ATraps.mc` (26-Feb-1982) says `6 Alto + Smalltalk 80`, `SMTRaps.mc`
(Jun-1982) says `6 Alto + Smalltalk 78`. Either way build 6 is not
DSemu, and no build-6 `.mb` or `.eb` is in either PUBLISHED archive -- which
is not the same as saying none exists; see "Availability" below.

**Conclusion.** The Alto route is blocked **architecturally, not by a
missing file**: Alto ST-80 wants 3K of writable control store, and the
Dorado's Alto emulator has none -- by design, since on a Dorado the
"RAM microcode" IS the emulator. No image and no loader can get past
that. The Dorado-native route needs the **build-6** emulator named in
ATraps.mc's own VERS enumeration, which is not in either published archive
(see "Availability" below -- unpublished is not the same as unavailable).

Two consequences worth separating:

- **`st80dec16.im` + `d80load.run` should run on ContrAlto today.**
  ContrAlto emulates an Alto II XM with 1K ROM + **3K CRAM**, which is
  exactly the configuration ST-80 wants. If it runs there, we have a
  working ST-80 V1 and a reference to diff against -- and note that the
  widely circulated `st80.dsk` is *not* it (a commenter on Shirriff's
  ContrAlto post: "st80.dsk actually contains a version of
  Smalltalk-76, despite its label").
- **Only a build-6 Dorado microcode makes ST-80 native.** That is the
  thing to ask for; see `docs/smalltalk80-mailing-list-query.md`.

### Availability: "not in the archive" is not the same as "does not exist"

**A substantial body of Smalltalk material was prepared for the CHM PARC
archive and is not currently published**, because the intellectual-property
position for Smalltalk -- including pre-ParcPlace material -- has not been
cleared for public release. Nothing is lost and no one is at fault; the files
exist, the rights question is simply still open.

That changes the shape of this problem in a way worth stating plainly:

- Everywhere these notes say a Smalltalk artifact is "in neither archive",
  read it as **"not in the PUBLISHED archive"**. It is a statement about what
  we can currently download, not about what survives.
- So the productive route may not be reconstruction or a community hunt at
  all. It may be that a working, well-documented emulator makes the case for
  revisiting the clearance worth making -- rights holders are generally more
  receptive when there is something concrete for the material to run on.
- Until then, keep treating the published archive as the boundary of what we
  can act on, and keep the reconstruction notes accurate, because they are
  what makes the case.

**Do not treat any of this as a licence to source the files another way.**
The point of recording it is that the artifact may be recoverable through the
front door later, not that we should work around the question now.

## 8. Open

- **Smalltalk-80 itself is a separate problem.** None of the above is
  about ST-80: DSemu is the **Smalltalk-76** interpreter (`DSmallDefs.mc`:
  "Dorado Model 1, XM version", Deutsch, July 1979). The ST-80 blockers
  are unchanged -- see `docs/new-os-pairs-tasks.md` Task B (the standard
  interchange images need conversion to the Dorado's Rot object memory)
  and the Dec-1980 ST-80 V1 kit now local in
  `chm/basicdisks/Smalltalk14.bfs!1_/` (`st80dec16.im`, `d80load.run`).
- **The `70000` trap** reported from an earlier attempt should be
  re-measured after the overlay path works, and checked against
  **DSemu's** trap table rather than AEmu's -- conflating the two
  produced a retracted conclusion once already (memory
  `aemu-cannot-host-smalltalk`).
