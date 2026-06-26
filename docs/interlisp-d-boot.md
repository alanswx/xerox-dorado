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
symbols, microcode, init files, and update command file. `dsk2trident` now
accepts `--diablo-cylinders 406 --diablo-sectors 14 --drive1 ...`, so it can
convert the two emulated Diablo-drive halves into the existing
Diablo-on-Trident pack layout.

Verified manually with a local `palo` geometry patch:

```
truncate -s 7681024 /tmp/LISP.VIRTUALMEM
palo/par --cylinders 406 --sectors 14 -1 /tmp/lisp0.dsk -2 /tmp/lisp1.dsk -f -rw
palo/par --cylinders 406 --sectors 14 -1 /tmp/lisp0.dsk -2 /tmp/lisp1.dsk -rw -i /tmp/LISP.VIRTUALMEM LISP.VIRTUALMEM.
dorado/build/dsk2trident --all-heads --diablo-cylinders 406 --diablo-sectors 14 --drive1 /tmp/lisp1.dsk /tmp/lisp0.dsk /tmp/lisp.pack
```

That produced a checker-clean two-disk Alto filesystem with 7683 pages free
after inserting a 15002-page zero-filled `LISP.VIRTUALMEM.`, and
`dsk2trident` placed 113680 sectors into a 60 MB test pack. `palo`'s
filesystem checker requires the trailing dot in the stored Alto filename;
confirm with the real Alto Executive whether `Lisp.run`'s `LISP.VIRTUALMEM`
lookup normalizes to that directory name.

This is not yet a bootable Lisp disk. It proves the required geometry and
VMEM allocation, but the pack still needs a bootable Alto OS plus
`Lisp.run`, `Lisp.syms`, `DORADOLISPMC.EB`, `AltoD1MC.eb`, and `INIT.LCOM`.
A tracked Alto filesystem image builder, or a properly vendored `palo`
geometry wrapper, is the next tooling step before adding this to `make`.

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
  proven, but the Alto filesystem image builder still needs to become tracked
  repo tooling.
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
