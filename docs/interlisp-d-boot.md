# Interlisp-D on Dorado — boot model and files

Source: first-hand account from Nick (Xerox, used a Dorado extensively for
Interlisp-D; modified the BCPL VM-swap startup code to span multiple disk
partitions), June 2026, plus files pulled from the CHM Phylum archive and
validated against our emulator.

## The boot model (what Nick described, confirmed by the `.cm` recipes)

Interlisp-D on a Dorado is **not** a standalone microcode world. It boots
in layers:

1. **Net-boot a regular Alto Executive** for the Dorado (the Alto NetExec).
   Nick: "you net booted a regular Alto exec for the Dorado." This is the
   same Stage-2 Alto-software-boot path we are already pursuing. (Michele
   Denber was recently booting the Alto NetExec on a Dolphin/D0; Al/Josh
   have a Dolphin version that may be identical to the Dorado one.)
2. **Run `Lisp.run`** — a BCPL Alto program. It is the loader: it loads
   `DoradoLispMC.EB` (the Interlisp-D microcode) into IM and brings up the
   Lisp virtual machine, backed by a disk swap file.
3. **The VM lives on disk.** `Lisp.run` swaps the Interlisp-D virtual
   memory against a large contiguous Alto file `LISP.VIRTUALMEM`
   (15002 pages on a T80 user disk, 20000 on a big disk — see the `.cm`
   files). Nick personally modified this BCPL VM-access code so the swap
   file could span more than one partition.

### Disk format (important, and tractable for us)

- Drives were **Trident T80 (5 surfaces)** or **T300 (19 surfaces)**.
- Alto-lineage software used **each recording surface as a separate
  partition**, each formatted **like the Diablo Model 44 Alto disk**.
- **Interlisp-D kept the multi-partition Alto disk format** (unlike IFS and
  Cedar, which had their own filesystem formats).
- In the Dorado AEmu disk microcode, one selected Trident surface contains
  two emulated Diablo drives. The mapping verified from
  `AltoDiabloDisk.mc` is:
  `TridentCylinder = 406 * drive + DiabloCylinder + 3`,
  `TridentSector = 14 * effectiveHead + DiabloSector + 1`, and
  `TridentHead = partition - 1`.

Consequence: a bootable Interlisp-D Dorado pack is just standard Alto/
Diablo-format partitions (one per surface) — exactly what ContrAlto's
Trident/Diablo code (in `AltoInfo/`) already emulates. So unlike Pilot/
Cedar (proprietary, no surviving installed volume), an Interlisp-D disk is
**reconstructable** from the files below: install the Alto OS on a
partition, `CreateFile LISP.VIRTUALMEM`, copy in `Lisp.run` + `Lisp.syms` +
`DoradoLispMC.EB` + `AltoD1MC.eb` + `INIT.LCOM`, then run `Lisp.run`.

### The install recipe (verbatim intent of `NewUserDisk.cm`)

```
delete DMT.boot
InstallSwat                                  # Alto debugger
CreateFile LISP.VIRTUALMEM 15002D            # the VM swap file (Nick's code)
FTP <Lisp>Lyric>Basics>: Lisp.run Lisp.syms DoradoLispMC.EB AltoD1MC.eb
FTP <Lisp>Current>:      LYRIC-PARC-INIT.LCOM -> INIT.LCOM
FTP <Lisp>Lyric>CM>:     UpdateLisp.cm
```

`NewUserBigDisk.cm` is identical but with a 20000-page VMEM.

## Files fetched and where they live

From `xeroxparcarchive.computerhistory.org/phylum/lisp/lyric/` (Nick's
preferred "lyric" release; "koto" is the prior release). Stored locally
under `chm/lisp/`:

| File | Bytes | What it is |
|---|---|---|
| `DORADOLISPMC.EB!1` | 36664 | **Interlisp-D microcode** for Dorado (net/Lisp.run-loadable). |
| `altod1mc.eb!1`     | 34760 | Alto-exec ("D1") microcode — same Mesa IM as AltoMesaDorado.eb. |
| `Lisp.run!6`        | 90266 | BCPL loader that starts Lisp (loads the .EB + maps VMEM). |
| `Lisp.syms!4`       | 17094 | Symbols for `Lisp.run`. |
| `SOURCES-VERSIONS!1`| 10975 | Manifest of the release. |
| `cm/NewUserDisk.cm!5`, `cm/NewUserBigDisk.cm!6`, `cm/UpdateLisp.CM!8` | <1 KB each | Install/update command files (the recipe above). |

Deferred (large VM images, download when we can run them) — same dir:
`Full.sysout!6` (7.75 MB), `Released-Full.sysout!2` (6.02 MB),
`LISP.SYSOUT!1` (4.82 MB), plus `LispDove.db`/`LispDlion.db` debugger
images (Dove=Daybreak, Dlion=Dandelion — no Dorado `.db`, the `.EB` is the
Dorado-specific piece). The directory mixes Dolphin/Dorado/Dandelion/
Daybreak files; only the `.EB` microcode is processor-specific.

## What we verified in the emulator

### 2026-06-26: real Lisp microcode boot offset works

The Dorado booting memo assigns microcode boot offsets `0110..0114` as
Mesa/Alto, Smalltalk, **Lisp**, Cedar, Test. The emulator's fake microcode
boot server was missing offset `0112`; a proper Lisp request therefore
stalled with one request and no reply. That is fixed: `0112` now defaults to
`chm/lisp/DORADOLISPMC.EB!1`, and `--boot-file-number 112 --eb
../chm/lisp/DORADOLISPMC.EB!1` loads the Lisp microcode through the real
Initial->LoadRam path at cycle ~32M.

This is only a microcode-load validation. Directly loaded Lisp still shows
no display and spins in the Lisp microcode because the Alto `Lisp.run`
loader, the sysout, and the `LISP.VIRTUALMEM` Alto-format Trident file are
not present in the running world.

### 2026-06-26: reconstructed a large AEmu-compatible Alto disk layout

The `NewUserDisk.cm` recipe asks for `CREATEFILE.run LISP.VIRTUALMEM
15002D`. A plain Diablo-31 image is too small, and a single 406-cylinder
Model-44-style emulated drive is also too small. The AEmu mapping provides
two emulated Diablo drives on one Trident surface, however:

```
2 drives * 406 cylinders * 2 heads * 14 sectors = 22736 Alto pages
```

That is enough for a 15002-page VMEM file plus the Alto OS, `Lisp.run`,
symbols, microcode, init files, and update command file. `altofs` now creates
the two emulated Diablo-drive halves with this geometry, and `dsk2trident`
accepts `--diablo-cylinders 406 --diablo-sectors 14 --drive1 ...` to convert
them into the existing Diablo-on-Trident pack layout.

Tracked recipe:

```
make -C dorado lisp-disk-image
```

This builds `dorado/build/run-disks/lisp-diablo-trident.pack` from two
temporary Alto disk halves, inserts a 15002-page zero-filled
`LISP.VIRTUALMEM.`, and copies the tracked `Lisp.run`, `Lisp.syms`,
`DORADOLISPMC.EB`, and `AltoD1MC.eb` files. Verification output shows a
checker-clean two-disk Alto filesystem with 7328 pages free after those
inserts, and `dsk2trident` places 113680 sectors into the 60 MB pack.
`palo`'s filesystem checker requires the trailing dot in stored Alto
filenames; confirm with the real Alto Executive whether `Lisp.run`'s
`LISP.VIRTUALMEM` lookup normalizes to that directory name.

This is not yet a bootable Lisp disk. It proves the required geometry and
VMEM allocation, and includes the core Lisp loader/microcode files, but the
pack still needs a bootable Alto OS and `INIT.LCOM`.

### 2026-06-27: sysout capacity and pack-building status

The CHM/archive.org Lisp material does include real sysout images, but the
Lyric `LISP.SYSOUT!1` is 4,824,064 bytes, or 9,422 Alto pages. That does not
fit beside the 15,002-page `LISP.VIRTUALMEM` file on the same two-drive AEmu
partition: the main partition has only about 7,328 pages free after VMEM plus
the small loader files. This matches the Xerox install recipe, which copies
`Lisp.run`, symbols, microcode, and init/update command files, but does not
copy a sysout into that same partition.

`make -C dorado lisp-disk-image-full` builds an exploratory local fixture
when the optional CHM/archive.org sysout files are present:

- partition 5 (Trident head 4): a minimal Alto OS/Executive seed extracted
  from `AltoInfo/ContrAlto2-beta/Disks/bcpl.dsk` (`Sys.Boot.`,
  `Executive.Run.`, `Swat.`, `Swatee.`, `SysFont.Al.`, `SYS.ERRORS.`,
  `Com.Cm.`, `User.Cm.`, `SCAVENGER.RUN.`), plus `LISP.VIRTUALMEM.`,
  `LISP.RUN.`, `LISP.SYMS.`, `DORADOLISPMC.EB.`, `ALTOD1MC.EB.`,
  `INIT.LCOM.`, and `UPDATELISP.CM.`
- partition 4 (Trident head 3): `LISP.SYSOUT.`

This uses `dsk2trident --base` to overlay the second partition without
clearing the first. The full target installs `Sys.Boot.` as the Alto boot
file; the earlier experiment of installing `LISP.RUN.` directly was wrong,
because `Lisp.run` is a BCPL program to run under the Alto Executive, not a
standalone disk boot image.

This is **not** a known-good Lisp pack. It is a probe that keeps the geometry,
capacity, and multi-partition overlay steps reproducible while we finish the
writable disk path. The useful current facts are:

- The low-level disk gates pass: `make -C dorado test` and
  `make -C dorado run-tricond-pack` are green on 2026-06-27.
- Disk-mode AEmu input now gets live Alto keyboard/mouse state, so typing at
  a disk-booted Executive prompt is no longer blocked by the frontend path.
- `altofs` installs a `SysDir.` DShape, and Scavenger now reads it correctly:
  a headless `Scavenger.boot!1` run against
  `build/run-disks/lisp-diablo-trident.pack` reaches the first prompt as
  `Partition: 5, Disks: 2, Cylinders: 406, Heads: 2, Sectors: 14`.
  The blocker was not the DShape bytes; it was the controller feeding RDCHK in
  the opposite order from `AltoDiabloDisk.mc`'s descending `DskMAddr` compare
  loop. RDCHK now consumes the same high-to-low block stream as READ/write.
- The host-built full fixture can reach the Alto OS/display path. On
  2026-06-28, native Scavenger accepted the regenerated full pack and returned
  to the Alto Executive (`6311 Pages`) after a full no-alter scan.
- The false `Would have Salvaged SysDir>LISP.VIRTUALMEM.` report was a
  `dsk2trident` cross-drive label-link bug, not a bad VMEM chain. The converter
  was forcing drive-1 bits onto label links that pointed back to drive 0; the
  boundary was VDA 11367/11368 (`prev` was `156256B`, should be `156254B`).
  `dsk2trident` now preserves the drive bit from each linked RDA.
- Running old BCPL utilities from a repacked filesystem still exposes absolute
  real-disk-address assumptions. After typing `lisp.run`, Swat/Swatee reports a
  disk-label mismatch at absolute RDA `030374B`; the fresh pack really has
  serial `0150B` there, so this is not runtime write corruption.
- `dsk2trident --remap-vda` is deliberately experimental. A 12-sector
  `bcpl.dsk` contains a mix of metadata: some checks want the 14-sector AEmu
  VDA interpretation, while other utilities check absolute physical RDAs from
  the original 12-sector pack. A blanket host-side remap fixes one label check
  and breaks another.

One bounded probe with the older direct-`LISP.RUN.` boot-file experiment:

```
cd dorado
./build/dorado --eb worlds/aemu.eb \
  --disk 0=build/run-disks/lisp-full-diablo-trident.pack \
  --boot-reason disk --cycles 180000000 \
  --out /tmp/dorado-lisp-full-disk.pgm --progress
```

reaches "Alto/Mesa world loaded" at cycle ~32M but produces 0 display-list
pixels. In strict disk-only mode (`--no-alto-boot`), the generated `Sys.Boot.`
pack reached the normal Alto OS/display path in one 2026-06-26 run:

```
cd dorado
./build/dorado --eb worlds/aemu.eb \
  --disk 0=build/run-disks/lisp-full-diablo-trident.pack \
  --boot-reason disk --no-alto-boot --cycles 300000000 \
  --out /tmp/dorado-lisp-full-os-diskonly-300m.pgm
```

Result on 2026-06-26: `28694 display-list pixels`. That result proves the
boot-file selection and basic disk read path can work; it does not prove the
fixture is filesystem-correct enough for Lisp.

An older exploratory run typing `lisp.run` at the Executive prompt:

```
./build/dorado --eb worlds/aemu.eb \
  --disk 0=build/run-disks/lisp-full-diablo-trident.pack \
  --boot-reason disk --no-alto-boot --type $'lisp.run\n' \
  --type-at 330000000 --key-hold 4000000 --cycles 700000000 \
  --out /tmp/dorado-lisp-run-typed-700m.pgm
```

typed all nine keys and ended with `0 display-list pixels`. That is consistent
with `Lisp.run` clearing the display before switching into the Lisp loader, but
it is not proof that the Lisp microcode/sysout path is complete. Next debugging
should trace `Lisp.run`'s LoadMicrocode / VMEM / sysout path, especially
whether it expects `LISP.SYSOUT.` on the auxiliary partition or only via the
original network installer flow.

### 2026-06-28: preserved BcplProg layout reaches the VMEM gate

The CHM `basicdisks/BcplProg.BFS!13` file is not a Palo/ContrAlto AAR sector
image. It is a Palo-readable BFS transfer stream: `par -1` rejects it as a
short AAR image, while `par -ibfs -1 chm/basicdisks/BcplProg.BFS!13 -d SysDir.`
loads it and reports a coherent BCPL Programmer's Disk (`OS Version 20/16`,
2450 free pages). This matters because the BFS stream preserves the installed
Alto OS layout, including Swat/Swatee real disk addresses and serials.

`make -C dorado lisp-bcplprog-loader-image` now builds a reproducible probe by
copying that BFS stream into `build/run-disks/`, converting it through Palo's
`-ibfs` load on the first insert, adding only the Lisp loader files, and then
mapping the resulting AAR image into a Diablo-on-Trident pack. The smoke target:

```
make -C dorado run-lisp-bcplprog-loader-smoke
```

boots AEmu from the generated pack, types `lisp.run`, and reaches the Alto
Executive message:

```
Cant find LISP.VIRTUALMEM.
```

That is the best current Lisp signal. It proves the disk-boot path, installed
Alto OS layout, keyboard injection, `LISP.RUN.` directory lookup, and BCPL
loader startup are all working together. The remaining blocker is no longer
Swat/Swatee label corruption; it is providing `LISP.VIRTUALMEM` with enough
capacity without disturbing the installed OS layout.

After the 2026-06-28 cross-drive label fix, the same style of probe no longer
blanked. It reached Swat with:

```
Open error: a file that appears to be mentioned in your directory cannot be opened.
The disk routines have encountered an unrecoverable disk error.
CallSubSys error: Are you sure this is a ".run" file?
The length of the file does not agree with BLDR's layout vector.
```

Host-side parsing of `build/run-disks/lisp-drive{0,1}.dsk` shows `LISP.RUN.`
is directory-clean: leader VDA 1049, serial `156B`, 178 pages, 90266 content
bytes, byte-exact against `chm/lisp/Lisp.run!6`. The current hypothesis is
therefore not insertion corruption. Follow-up with `BcplProg.BFS!13` showed the
root cause more precisely: repacking the Alto OS files changes the labels and
serials that Swat/Swatee check by absolute real disk address.

The small `*Initial.db` files under `chm/lisp/*/basics/` are not Trident pack
images. They share the Lisp/sysout-style magic bytes but are only 8-10 KB, so
they are debugger/initialization payloads rather than bootable disk packs.

Follow-up debugging found that host-side pack reconstruction is the remaining
weak link. One failing Swatee read-check at Alto real disk address `30374B`
expects the original `bcpl.dsk` label:

```
040374 020374 000000 001000 000376 000001 000000 000161
```

The synthetic full pack had repacked `Swatee.` and other Alto system files into
a new filesystem, so that same real disk address held a different file label
(initially a `LISP.VIRTUALMEM.` page, later another synthesized file page).
That explains the Swatee "file smashed" / `047402` check-error screen: old Alto
system utilities are not safe to byte-copy into an arbitrary new filesystem
layout. `altofs --vmem-after-inserts` and the updated full target now at least
allocate `LISP.VIRTUALMEM.` after the OS seed files, matching the Xerox install
order more closely, but the remaining fix is to preserve or install a real Alto
OS disk layout rather than extracting and reinserting `Swat.`/`Swatee.` as raw
files.

A separate 12-sector-to-14-sector experiment showed why this should be solved
by running the original Alto tools instead of teaching `dsk2trident` every
filesystem invariant. RDA `011270B` maps to `DiskDescriptor.` page VDA 2437
under AEmu's 14-sector geometry, so VDA remapping fixed that check. Later,
RDA `050330B` expected a `Swatee.` page by absolute physical address from the
original 12-sector layout; the same VDA remap put the wrong page there. The
right long-term target is therefore:

1. Boot a known-good Alto Executive on AEmu.
2. Attach a fresh writable copied pack/partition.
3. Run the original Xerox flow from `NewUserDisk.cm`: `Install`, erase/clean,
   `Scavenger`/BFS, `CreateFile.run LISP.VIRTUALMEM 15002D`, then copy
   `Lisp.run`, symbols, microcode, init, and update files.
4. Treat "CreateFile succeeds and Scavenger is clean" as the writable
   Alto-format pack acceptance test.

### 2026-06-28: native VMEM/sysout path now reaches Lisp version checks

The preserved BcplProg image can be expanded in place to AEmu's full
dual-drive model-44 shape without moving the original physical sectors:
406 cylinders, 2 heads, 14 sectors, 2 emulated Diablo drives. Patching
`SysDir.`'s DShape property to that geometry makes native `Scavenger` accept
the pack as:

```
Partition: 5, Disks: 2, Cylinders: 406, Heads: 2, Sectors: 14
Ready to scavenge a dual model-44 14-sector file system
```

After the duplicate `bravo.scratchbin.` prompt is answered with a rename,
Scavenger exits back to the Executive with about 19,833 free pages. Native
`CreateFile.run` can then allocate `LISP.VIRTUALMEM.`:

```
Found a group of 41716b pages starting at vda 12402
```

Running plain `lisp.run` after that no longer says `Cant find
LISP.VIRTUALMEM.`. It reaches Swat with `Trap instruction 77400 at 0`, which
is expected for a blank VMEM resume attempt. The loader source confirms the
correct initialization path: `lisp.run <sysout>` or `lisp.run <sysout> ...`
causes `LocalInitVmem(SYSINid, VMEMid)` to copy a sysout into the VMEM file.

Two tooling fixes make that test repeatable:

- `dsk2trident --extract` reverses a Diablo-on-Trident partition back into
  editable 406x14 AAR drive images.
- `altofs --existing --force-existing --insert` can add files to the
  scavenged pair. Palo's allocator walks one VDA past the end when extending a
  large file that reaches the final sector, so `altofs` uses a local
  page-by-page insertion path for this wrapper.

With `chm/lisp/fugue.6/basics/SMALL.SYSOUT!1` inserted as `LISP.SYSOUT.`,
the pack boots, `Lisp.run` finds both `LISP.VIRTUALMEM.` and `LISP.SYSOUT.`,
and the loader rejects the image at the real Lisp interface-page version check:

```
Retrieve of sysout LISP.SYSOUT. failed
Sysout too old for this microcode
```

That proves the disk write path, VMEM lookup, local sysout lookup, and
sysout-to-VMEM initialization path are all being exercised. The small sysout is
simply too old for the available Dorado Lisp microcode.

### 2026-06-29: remote sysout retrieval over fake Pup FTP/BSP works

The preserved BcplProg path now has a shorter reproducible remote-sysout test:

```
make -C dorado run-lisp-remote-sysout-smoke
```

That target starts from a fresh BcplProg-derived loader image, inserts
`CreateFile.run`, boots the Alto Executive, creates a small contiguous
`LISP.VIRTUALMEM.` with native `CreateFile.run`, then runs:

```
lisp.run {DORADO}LISP.SYSOUT
```

with `--ftp-sysout ../chm/lisp/fugue.6/basics/SMALL.SYSOUT!1`. `DORADO_FTP_TRACE`
is on by default for this short smoke and shows the full client path: NetDir
lookup, RTP RFC to socket FTP, FTP `Version`, `Retrieve`, plist approval, and
BSP 512-byte data transfer. The final screen is:

```
Retrieve of sysout {DORADO}SMALL.SYSOUT!1 failed
Sysout too old for this microcode
```

So the fake Pup FTP/BSP path is now good enough to feed `RemoteInitVmem`; the
remaining blocker is finding or building a sysout/loader/VMEM combination that
can complete initialization. The relevant version words are in the sysout
interface page at byte offset 512 (`LispBcpl.params`: `IFPLVersion=#10`,
`IFPMinRVersion=#11`, `IFPMinBVersion=#12`, `IFPNActivePages=#24`):

| File | LispV | MinR | MinB | ActivePages | Result with current smoke |
|---|---:|---:|---:|---:|---|
| `fugue.6/basics/SMALL.SYSOUT!1` | `110400` | `012000` | `021400` | `010642` | too old for Lyric microcode |
| `fugue.6/basics/Lisp.sysout!1` | `110400` | `012000` | `021400` | `012063` | too old for Lyric microcode |
| `harmony/basics/LISP.SYSOUT!15` | `111000` | `012000` | `021400` | `012503` | too old for Lyric microcode |
| `archiveorg/_chm-parc_interlisp-lyric/LISP.SYSOUT!1` | `114000` | `013062` | `025400` | `022316` | plausible Lyric candidate; needs larger VMEM/pack |

The current top-level `DORADOLISPMC.EB!1` is the Lyric microcode (`RamVersion
= 013062`, `MinBcplForRam = 021000`, `MinLispForRam = 113000`) and is
byte-identical to `archiveorg/_chm-parc_interlisp-lyric/DORADOLISPMC.EB!1`.
The older Fugue/Harmony `DORADOLISPMC.EB!1` variants require only
`MinLispForRam = 111000`; those are a possible alternate pairing if we want to
boot Harmony instead of Lyric.

For large sysout probes, disable packet-by-packet FTP logging:

```
make -C dorado run-lisp-remote-sysout-smoke \
  LISP_REMOTE_FTP_TRACE= \
  LISP_REMOTE_SYSOUT_FILE=../chm/archiveorg/_chm-parc_interlisp-lyric/LISP.SYSOUT!1
```

That still needs a larger writable/scavenged pack before it can complete,
because the small BcplProg smoke pack only creates a 1500-page VMEM.

The current long Lyric probe uses a preserved BcplProg-derived pack expanded
to the full AEmu 406x14 dual-drive shape, creates `LISP.VIRTUALMEM.` with the
native `CreateFile.run`, and then serves the Lyric sysout over fake Pup
FTP/BSP:

```
make -C dorado run-lisp-lyric-remote-long
```

This is intentionally not a short regression test. VMEM creation alone defaults
to 5.8B cycles (`LISP_LYRIC_CREATEFILE_CYCLES=`), then the Lyric run defaults
to 5B cycles and writes periodic headless screenshots to
`/tmp/dorado-lisp-lyric-CYCLE.pgm` using `--shot-every 500000000`; adjust with
`LISP_LYRIC_CYCLES=` and `LISP_LYRIC_SHOT_EVERY=` while debugging.

Latest result: host-side VDA-preserving expansion plus native `CreateFile.run`
does build the large VMEM file. The screen returns to the Alto Executive and
extracting the pack shows `LISP.VIRTUALMEM.` has 15,004 linked pages including
the terminal page. But running `lisp.run {DORADO}LISP.SYSOUT` from that pack
still trips Swat/Swatee disk-label checks before reaching FTP. Interpretation:
VDA-preserving expansion is good for host/Palo directory consistency and native
CreateFile, but old Alto system utilities still care about physical RDA labels.
The next path should be the physical-preserving expansion plus native Scavenger
repair flow, not further changes to fake FTP.

### 2026-06-09: checksum/load validation

Pointed our **existing, working Stage-1** Initial->LoadRam netboot path
(`DORADO_ETH_BOOT_110=<file>`) at each `.eb`. All three load with **zero
new code**:

| `.eb` | payload words | LoadRam checksum | IM vs known .MB | Verdict |
|---|---|---|---|---|
| `AltoMesaDorado.eb` | 17124 | `sum=0` OK | `Mesa.mb=6/6` | known Mesa world (baseline) |
| `altod1mc.eb`       | 17124 | `sum=0` OK | `Mesa.mb=6/6`, IM identical to AltoMesa | Mesa-based Alto exec; start PC `1070` vs `1076` |
| `DORADOLISPMC.EB`   | 18076 | `sum=0` OK | `0/6` against ALL known .MB | **new world — Interlisp-D microcode** |

So:
- `DoradoLispMC.EB` is a brand-new, valid microcode world (distinct from
  Mesa/Alto/Cedar/Smalltalk/DSemu) — a fresh validation target for the
  decoder/loader and the only net-loadable Lisp microcode we have (we
  previously had only `DoradoLisp.MB` source in UnBug).
- `altod1mc.eb` confirms our Stage-1 target microcode is the right one
  (it IS the Alto-exec/Mesa world).

## Implications for the project plan

- **Validates the Ethernet/Alto-NetExec direction.** Nick independently
  describes exactly the Stage-2 path in `ethernet-local-boot-plan.md`.
- **Disk is mandatory for full Lisp** (the `LISP.VIRTUALMEM` swap). This
  raises the priority of finishing the Alto disk install path on top of the
  Trident read/write controller path. The pack geometry and VMEM file are now
  reproducible with `make -C dorado lisp-disk-image`.
- **A concrete, reconstructable Lisp bring-up exists** once disk works:
  build an Alto Trident partition + VMEM file, run `Lisp.run`, which loads
  `DoradoLispMC.EB`. This is more achievable than Pilot/Cedar.
- The BaseBoard 6502 likely does **not** need full emulation (Nick:
  "perhaps not") — consistent with our protocol-level stub.

## Open questions to take back to Nick (he is travelling until ~July 20)

1. Where does the Dorado Alto NetExec live? (He didn't know the backup
   path; Al/Josh's Dolphin NetExec may be identical.)
2. Does `Lisp.run` load `AltoD1MC.eb` itself, or rely on the already-booted
   Alto-exec microcode, and what triggers the `DoradoLispMC.EB` LoadRam?
3. Multi-partition swap: which partitions, and the on-disk layout of
   `LISP.VIRTUALMEM` across surfaces (his BCPL change).
