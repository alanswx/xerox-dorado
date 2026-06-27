# New OS pairs landed — bring-up task list

**Status: research/acquisition done (2026-06-20). Code work not started.**

A survey of CHM (`xeroxalto` + `xeroxparcarchive`), bitsavers, and
archive.org pulled the **missing OS-image halves** for the two Dorado OSes
where we previously had only the microcode. Both pairs are now complete on
disk, plus bonus worlds, boot stages, and microcode source. This file is
the punch list for wiring them into the emulator.

Cross-refs: `docs/interlisp-d-boot.md` (Lisp boot model — already written),
`docs/dorado-software.md` (boot chain), `docs/running-the-emulator.md`
(how to run), `docs/handoff.md` (gap IDs), `docs/CONTINUE-HERE.md`
(live state). Nothing here changes the working Alto (Path A) or Cedar
(Path B) paths.

---

## What "OS pair" means

A runnable Dorado OS = **microcode world** (`.eb`/`.mb`/`.pb`, loaded into
IM) **+ the OS image it executes** (virtual-memory image / sysout / disk
volume). We already had the microcode for every OS; the gap was the image
half for Smalltalk-80 and Interlisp-D. Both are now local.

| OS | Microcode (had) | Image half (NEW) | Pair status |
|---|---|---|---|
| Alto/BCPL | `worlds/aemu.eb`, `AEmu.mb` | `.boot` files | ✅ runs (Path A) |
| Mesa/Pilot | `Mesa.mb`, `DoradoMesa.eb!4` | `MesaNetExec.boot` | ✅ have |
| Cedar | `CedarDorado.eb!6` | PDI disk + `CedarDorado.boot!59` | ✅ to login (Path B) |
| **Smalltalk-80** | `SmalltalkDorado.eb`, `DSemu.mb` | **ST-80 v2 `VirtualImage`** | ⬜ image now local — Task B |
| **Interlisp-D** | `DORADOLISPMC.EB` (Lyric) | **Lyric `LISP.SYSOUT`** | ⬜ image now local — Task A |

---

## Where the new files live

### Interlisp-D (Lisp)
- **Microcode (had):** `chm/lisp/DORADOLISPMC.EB!1` (Lyric, 36664 B).
  Identical copies also at `chm/lisp/harmony/basics/`,
  `chm/lisp/fugue.6/basics/`, `chm/archiveorg/_chm-parc_interlisp-lyric/`.
- **Sysout images (NEW — these were "deferred" in `interlisp-d-boot.md`):**
  - `chm/archiveorg/_chm-parc_interlisp-lyric/LISP.SYSOUT!1` (4.82 MB,
    27-Apr-1987) — **the period-correct match** to our microcode's manifest
    (`SOURCES-VERSIONS` says Lyric). Use this one first.
  - `chm/archiveorg/_chm-parc_interlisp-medley1.0/LISP.SYSOUT!1` (5.0 MB,
    Nov-1988) — newest, for forward testing.
  - `chm/lisp/harmony/basics/LISP.SYSOUT!15` + `FULL.SYSOUT!17`;
    `chm/lisp/fugue.6/basics/Lisp.sysout!1` + `FULL.SYSOUT!1` +
    `SMALL.SYSOUT!1` — earlier releases, alternates.
  - Sysout magic bytes: `00 0b 00 fe 01 00` (verified on all).
- **Loader / install recipe:** `chm/lisp/Lisp.run!6`, `Lisp.syms!4`,
  `chm/lisp/cm/NewUserDisk.cm!5` / `NewUserBigDisk.cm!6` / `UpdateLisp.CM!8`
  (the `CreateFile LISP.VIRTUALMEM` recipe — see `interlisp-d-boot.md`).
- **Microcode SOURCE for debugging:** `chm/lisp/fugue.6/ucode/` — full
  Micro source (`Lisp.mc`, `LispCons.mc`, `LispGc.mc`, `NewLisp.mc`,
  `NewLispArith.mc`, `StartLisp.mc`, `InitLisp.mc`, `MoreLisp.mc`,
  `Xufn.mc`, `nLispUfn.mc`, `CJump.mc`, `GcRef.mc`, `NewBLT.mc`,
  `IOPMain.mc`, `Lisp.midas`, `*.df`, `FILES`). Also
  `chm/lisp/harmony/ucode/` (`LISPDMC.DM`, `ALTOMC.DM`, build tooling).
- **Net boot stage (NEW):** `chm/cedar/top/DoradoInitialEtherLisp.eb!1`
  (4880 B) — the Initial loader that requests the Lisp microcode world.

### Smalltalk-80
- **Microcode (had):** `chm/microcode/SmalltalkDorado.eb!1` (=
  `DoradoSmalltalk.eb`), `chm/dorado/DSemu.mb!1`.
- **Image (NEW):** `chm/archiveorg/smalltalk-80/VirtualImage` (596128 B,
  Smalltalk-80 **version 2**, authentic Xerox tape via Wolczko) +
  `Smalltalk-80.sources` (1.4 MB) + `*.oops` symbol/trace files. Magic
  bytes `00 03 f3 40` (real ST-80 object memory). Backups (zipped, need
  extraction): `chm/bitsavers/bits/Xerox/1100/dolphin_smalltalk.zip`
  (`st80-82-11-15-customer.im`, 1982) and
  `chm/bitsavers/bits/Xerox/1186/1186_Smalltalk-80_DV6_Dec87.zip` (IMD).
- **Microcode SOURCE for debugging (NEW):** `chm/dorado/dsemu-src/` —
  `DSmallDefs.mc` (object-memory definitions; header says **"Model 1 XM"**),
  `DSmallint.mc` (interpreter), `DSmallops.mc`, `DSmallsubrs.mc`,
  `DSmalltrapped.mc`, `DSmallmsgs.mc`, `SMTRaps.mc`, `TJunk.mc`,
  `Smalltalk.midas`, `DSemu.cm` (build recipe).

### Bonus Dorado worlds / boot stages (cross-ref + test targets)
- `chm/microcode/Chuck.pb!1`, `FlukeServer.pb!1`, `SA4000Diagnostic.pb!1`
  — new Dorado packed worlds. `SA4000Diagnostic` = Shugart SA4000 fixed-disk
  diagnostic (good disk-controller exercise). `Brio.txt!1` is the
  BootServer catalog that names them.
- `chm/cedar/top/CedarDorado.boot!59` (1.5 MB full Cedar world),
  `CedarSnapshotDorado.boot!8`, and the boot-stage set `DoradoMesa.eb!4`,
  `DoradoCedar.eb!6`, `DoradoInitialMesa.eb!4`, `DoradoInitialDisk.eb!1`,
  `DoradoInitialEther{Cedar,Mesa,Test}.eb!1`.
- `chm/cedar/top/DoradoMicrocode.DF!13` — canonical release manifest
  (authoritative list of which `.eb` worlds ship together; names a
  `DoradoSmalltalk.eb!2` / `DoradoInitialEtherSmalltalk.eb!1` we have not
  yet located — see Open Questions).

---

## Task A — Interlisp-D bring-up

Background already written in `docs/interlisp-d-boot.md`. Key model: Lisp on
Dorado is **not** a standalone world — net-boot an Alto exec, run `Lisp.run`
(BCPL), which loads `DORADOLISPMC.EB` into IM and demand-pages the
Interlisp-D virtual memory against a large contiguous Alto-format disk file
`LISP.VIRTUALMEM`. The sysout is the *source* image that gets installed into
that VMEM swap file.

1. **Cheap validation first (no new code):** confirm
   `chm/cedar/top/DoradoInitialEtherLisp.eb!1` and the Lyric
   `DORADOLISPMC.EB` decode/load via the existing Initial→LoadRam netboot
   path (the `DORADO_ETH_BOOT_110=<file>` harness already validated
   `DORADOLISPMC.EB` loads with `sum=0` — see `interlisp-d-boot.md` §"What
   we verified"). Re-run against the boot stage to confirm the world it
   requests.
2. **Decide the VM-load mechanism.** Two options to evaluate:
   (a) the hardware-accurate disk-swap path (`Lisp.run` + Alto-format
   Trident pack + `LISP.VIRTUALMEM`), which depends on the Trident
   read/**write** data path (gap F-series) and Alto/Diablo disk format —
   reconstructable per the `NewUserDisk.cm` recipe; or
   (b) a shortcut: map the sysout directly into the emulator's VM/swap
   backend. Read `chm/lisp/fugue.6/ucode/StartLisp.mc` + `InitLisp.mc` +
   `Lisp.run`/`Lisp.syms` to determine how `Lisp.run` lays the sysout into
   VMEM and what the microcode expects at startup. **Do not guess the
   format — it is in those sources.**
3. **Finish the Alto-format Lisp pack** (if pursuing 2a): the geometry and
   VMEM allocation are reproducible with `make -C dorado lisp-disk-image`
   (`2 * 406 * 2 * 14 = 22736` Alto pages, enough for `LISP.VIRTUALMEM
   15002D`). The small generated pack includes `Lisp.run`, `Lisp.syms`,
   `DORADOLISPMC.EB`, and `AltoD1MC.eb`.

   There is also a local full fixture:
   `make -C dorado lisp-disk-image-full`. It seeds a minimal Alto
   OS/Executive from `bcpl.dsk`, keeps the VMEM/loader files on partition 5,
   installs `Sys.Boot.` as the disk boot file, and overlays the Lyric
   `LISP.SYSOUT.` onto partition 4 with `dsk2trident --base`, because the
   sysout is 9,422 Alto pages and cannot fit beside `LISP.VIRTUALMEM.` on the
   main partition.

   Treat that target as a probe, not a correct finished pack. It can exercise
   boot-file selection and basic disk reads, but byte-copying old BCPL system
   files into a fresh host-generated filesystem changes labels and absolute
   real-disk-address relationships that utilities such as Swat/Swatee check.
   The failed 12-sector-to-14-sector remap experiment confirmed the ambiguity:
   one label check wants AEmu's 14-sector VDA interpretation, while a later
   Swatee check wants the original 12-sector physical RDA. Next preserve or
   install a real Alto OS layout by running the original Alto tools (`Install`,
   erase/clean, Scavenger/BFS, `CreateFile.run LISP.VIRTUALMEM 15002D`) in the
   emulator, then run `Lisp` and confirm whether it finds/uses the sysout on
   the auxiliary partition or expects the original network installer path.
   ContrAlto's Trident/Diablo code (`AltoInfo/`) remains the format reference.
4. **I/O + render:** Lisp keyboard/mouse/display delivery, analogous to
   `machine_cedar_io` (`dorado/src/machine.c:836`). Interlisp-D uses the
   standard Alto-lineage display; the framebuffer is already wired.
5. **Add `make run-lisp`** mirroring `run-cedar`, once a load path works.

## Task B — Smalltalk-80 bring-up

We now have both halves but have never run this OS. Smalltalk-80 ran on the
Dorado at PARC via the DSemu microcode; the ST-80 v2 `VirtualImage` is
largely interpreter-independent (the machine-specific part is the microcode
we have).

1. **Cheap validation first:** confirm `SmalltalkDorado.eb!1` / `DSemu.mb`
   decode/load through the existing loader (same `sum=0` check used for the
   Lisp `.eb`). This is a fresh decoder/loader validation target.
2. **Resolve the image-format question.** `chm/dorado/dsemu-src/DSmallDefs.mc`
   header says **"Model 1 XM"**. Determine whether the Dorado DSmall
   microcode expects the **ST-80 v2 interchange object-memory format** of
   our `VirtualImage` (magic `00 03 f3 40`), or an older/different layout.
   Read `DSmallDefs.mc` (object/region layout, oop format) + `DSmallint.mc`
   (interpreter entry, image header expectations). If formats differ, a
   one-time image converter may be needed (or use the 1982
   `st80-82-11-15-customer.im` from the bitsavers zip instead — try both).
3. **Determine the image-load path.** How does the DSemu world get its
   object memory? (disk? net? planted into VM by an Initial stage like the
   Cedar germ?) The release manifest names `DoradoInitialEtherSmalltalk.eb`
   / `DoradoInitialSmalltalk.eb` — locating those (Open Questions) likely
   answers this. Cross-check `DSemu.cm` + `Smalltalk.midas` for the load
   convention.
4. **Get the image into VM**, start the interpreter, **wire Smalltalk I/O**
   (keyboard/mouse/bitmap display — ST-80 is mouse-and-bitmap heavy), and
   render. Model on `machine_cedar_io`.
5. **Add `make run-smalltalk`.**

### VERDICT 2026-06-20 (cont.) — direct image plant is NOT possible; a converter is required

Deep-read the DSmall interpreter (`DSmallint.mc`, `DSmallops.mc`,
`DSmallDefs.mc`, `DSmalltrapped.mc`) and decoded our two standard images
(`VirtualImage` v2 header `00 03 f3 40 ...`; the bitsavers 1982
`st80-82-11-15-customer.im` header `00 03 c0 56 ...`). Both are the **standard
Smalltalk-80 interchange format**; the Dorado interpreter expects a **different,
Dorado-specific in-memory object world**. They are not interchangeable:

| Aspect | Standard ST-80 image (what we have) | Dorado DSmall expects |
|---|---|---|
| OOP/SmallInt | OT **index**; low-bit-set => SmallInteger | SmallInts are a high OOP range (`Oop00=176000`B, min `174000`B); `ImmBit=4` |
| Object table | standard OT (2-word v1 / 4-word v2 entries) | **Rot** (Reference Object Table): clean bit, reprobe-count hash (`RpcBit=400`B, `HkrMsk=174000`B), refcount (`RctMsk=360`B), `Rot0Em=360`B |
| Header | header(8w) + OT + object space | **none** -- microcode parses no header; assumes a fully laid-out memory image |
| Bytecodes | standard v1 / v2 | Dorado-custom (v1-aligned: has `min:/max:`; plus v2 `super` send) |
| Startup | n/a | `.Initialize` runs only after ~18 BRs are set; needs a valid Rot, class map (`PmBaseBr`), and an initial Arec via `Ctxt` (sender/self/`CodeF` method/`PcF`/`StackPF`), method bytecode, temp frame. Final trigger: `WriteReg #56` (RotBaseBr). |

**Consequence:** there is no quick "load image + jump." Running Dorado Smalltalk
requires writing the missing `DoradoInitialSmalltalk` functionality from scratch:
a converter that rebuilds a standard ST-80 object table + object space into the
Dorado **Rot** format with Dorado OOP/SmallInt encoding, lays object space into
the Dorado memory regions, constructs the initial Arec/Ctxt + base-register set,
and (likely) reconciles the bytecode dispatch. No reference converter and no
native Dorado-format image survive to copy or validate against, so correctness
(esp. the Rot reprobe-hash placement and the special-objects OOPs) must be
reverse-engineered and is the main risk. This is a multi-session subsystem, not
a wire-up. Building blocks if pursued: (1) a standard-image reader (header ->
OT -> object space, big-endian 16-bit, byte-swapped on load -- the easy part);
(2) the OT->Rot + OOP re-encoder (the hard part); (3) the planter + I/O wiring
(model `machine_cedar_io`). Reference for the standard format: Goldberg & Robson
Blue Book "Virtual Image"/"Object Memory"; for the target, `DSmallDefs.mc`.

### Interlisp-D update 2026-06-26 — Lisp microcode offset fixed, OS still gated on VMEM

The fake microcode boot server was missing the documented Dorado Lisp offset
`0112` (boot-file number 3112). That made a correct `--boot-file-number 112`
probe stall before LoadRam, even though the earlier `0110` override could
load the same file. The server now has a real `0112` slot defaulting to
`chm/lisp/DORADOLISPMC.EB!1`, and the corrected probe loads the Lisp
microcode at cycle ~32M.

That does **not** make Interlisp-D boot by itself. After LoadRam it still has
no display and no progress toward a Lisp session, consistent with the model
below: `Lisp.run` must create/use `LISP.VIRTUALMEM`, load the sysout, and
then switch into `DORADOLISPMC.EB`.

### Interlisp-D verdict 2026-06-20 — blocked on the Trident disk WRITE path

Confirmed (interlisp-d-boot.md + `StartLisp.mc`/`InitLisp.mc`/`Lisp.run`):
Interlisp-D is not a standalone world. It needs the Alto NetExec to run
`Lisp.run` (BCPL), which **creates and writes** the contiguous Alto-format
`LISP.VIRTUALMEM` swap file and demand-pages the sysout against it. That makes
the **Trident disk read+write data path (gap F-series) a hard prerequisite**;
the only alternative (Task A.2b: a custom swap backend feeding sysout pages to
the microcode's IOP page-fault path) is also a substantial subsystem. Neither is
a quick win; Lisp is gated on the disk path landing first.

### Update 2026-06-20 — plant stage verified absent + version-era caveat

Research follow-up (the other LLM found DSmall is a pure interpreter
expecting object memory pre-planted in VM by a `DoradoInitialSmalltalk.eb`
stage we don't have). Acquisition side checked exhaustively:

- **No plant-stage artifact to grab.** `DoradoInitialSmalltalk.eb` /
  `DoradoInitialEtherSmalltalk.eb` / `DoradoSmalltalk.eb!2` are named in
  `DoradoMicrocode.DF!13` but are **not** in the CHM mirror (absent from the
  `xeroxparcarchive` cross-reference and 404 on `indigo/cedar/top`,
  `indigo/dorado`, `phylum`, `indigo/precedar/top`). The full
  `DSemuSources.dm!17_` archive (= our `chm/dorado/dsemu-src/`, complete)
  contains **only** the interpreter (`DSmall*`, `SMTRaps`, `TJunk`) — **no
  image-loader / InitMem / boot module**. So the plant stage really must be
  reconstructed, not downloaded.
- **Red herrings ruled out** (don't chase these): `edsmallmem.MB/.mc` =
  a **D0 memory diagnostic** (`cyan/d0/edmicrodiagnostics…`);
  `LoadSmallFromDLinit.cm` = **Interlisp** intermezzo (`eris/lisp/intermezzo`);
  `ReadSmallCache.cm` = the **Dragon** project (`_cd*_/dragon7.0`). None is
  the Smalltalk image loader.
- **Version-era caveat (important for image choice).** The DSemu interpreter
  is "Dorado Smalltalk Microcode — **Model 1, XM version**" (origin Deutsch,
  Jul-1979) **last edited 20-Sep-1982 by Haugeland**; `SMTRaps.mc` Jun-1982,
  `TJunk.mc` Sep-1982. That 20-Sep-1982 date matches `DoradoSmalltalk.eb!2`
  in the manifest — so the world is the **Sept-1982** build. Our
  `VirtualImage` is Smalltalk-80 **v2** (the 1983 "Bluebook" release,
  file-dated 1985). A 1982 interpreter most likely targets ST-80 **v1**
  (1981) bytecodes/object format, so **the v2 image may not run as-is**.
  Before investing in the plant path: confirm the bytecode era by reading
  the dispatch in `dsemu-src/DSmallint.mc` + `DSmallops.mc` against the
  ST-80 v1-vs-v2 bytecode tables, and consider sourcing a **Smalltalk-80
  v1** image (or the bitsavers `st80-82-11-15-customer.im`, which is
  period-1982 and may match better than the v2 tape).
- **RE reference for the plant format:** the object-memory layout the
  interpreter expects is defined in `dsemu-src/DSmallDefs.mc` (oop/region
  format, `XM` extended-memory addressing). For "how a host plants an ST-80
  image into machine memory," the closest working model is the Alto
  Smalltalk-80 boot loader — see ContrAlto/salto in `AltoInfo/`.

## Task C — lower-priority wins

- **Decode-validate the bonus worlds.** Run `Chuck.pb`, `FlukeServer.pb`,
  `SA4000Diagnostic.pb`, `CedarDorado.boot!59`, `CedarSnapshotDorado.boot!8`
  through `mbdis`/the loader as additional decoder regression targets
  (these are formats/worlds the loader hasn't seen).
- **`SA4000Diagnostic.pb`** is a self-contained disk diagnostic — a good
  fixture for exercising the disk-controller data path once write works.
- **`CedarDorado.boot!59`** is a full Cedar world (alternative to the
  current `.eb` + PDI path) — candidate second Cedar boot route.

---

## Open questions to resolve before/while coding

1. **Smalltalk image load path + format.** Does DSmall (Model 1 XM) accept
   the ST-80 v2 `VirtualImage` as-is, or need conversion? (Read
   `dsemu-src/DSmallDefs.mc` + `DSmallint.mc`.)
2. **Missing Smalltalk boot/plant stage — RESOLVED as "not in the mirror"
   (2026-06-20).** `DoradoInitialSmalltalk.eb` / `DoradoInitialEtherSmalltalk.eb`
   / `DoradoSmalltalk.eb!2` are named in `DoradoMicrocode.DF!13` but were
   confirmed **absent** from CHM (cross-reference + 404 on `indigo/cedar/top`,
   `indigo/dorado`, `phylum`, `indigo/precedar/top`), and the DSemu source
   archive has no loader module. The plant stage must be reverse-engineered.
   We do have the interpreter microcode itself (`SmalltalkDorado.eb` =
   `DoradoSmalltalk`). Details + the version-era caveat are in
   **Task B → "Update 2026-06-20"** above.
3. **Lisp VM mechanism (disk-swap vs. direct-map)** — see Task A.2; the
   answer is in `StartLisp.mc`/`InitLisp.mc`/`Lisp.run`.
4. The three `interlisp-d-boot.md` questions for Nick (Alto NetExec
   location; what triggers the `DORADOLISPMC.EB` LoadRam; multi-partition
   `LISP.VIRTUALMEM` layout) are still open and overlap Task A.
