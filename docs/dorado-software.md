# Software for the Dorado

What's available to run on our Dorado emulator: microcode worlds,
disk pack images, and how each fits into the boot chain. This is a
status snapshot of what we already have locally and what CHM PARC
archives offer.

## TL;DR

There are two hardware-like boot approaches worth pursuing. They share
the same first half of the boot chain, but diverge after emulator
microcode is running.

1. **Alto-emulator software boot** (recommended first): Initial loads
   `AltoMesaDorado.eb` via the Dorado microcode boot protocol, then
   the loaded Alto/Mesa emulator performs a normal Alto software boot
   from either an emulated Alto disk partition or the Alto Ethernet
   boot protocol. This is the path that can use Alto disk images and
   the Spruce T-300 pack as validation fixtures.
2. **Cedar/Pilot disk boot**: Initial loads Cedar microcode, then
   Cedar boots a `Dorado.germ` and physical volume boot file from a
   Pilot/Cedar disk volume. The CHM archive has the Dorado germ,
   `BasicCedarDorado.boot`, and `BasicCedarDorado.pb`; this path is
   more native to Cedar, but requires more Pilot volume knowledge.

## What "boot disk" means for the Dorado

The Dorado boot chain has several layers, and "disk" only enters at
the end:

```
EPROM Boot0  ──►  Bootstrap  ──►  Initial  ──►  emulator microcode  ──►  OS
(BB ROM)         (in IM 7700-7777)  (loads emu)   (Mesa/Cedar/Alto)       (Pilot/Trident/...)
```

The first three stages (Boot0, Bootstrap, Initial) come from the
BaseBoard ROM. Then **Initial loads emulator microcode** — typically
either over Ethernet (`.eb` Ether-Bootable format), from the disk
Initial region, or via an InitialSelect overlay. Once the emulator
microcode is running, it performs a **second-stage software boot**.

So "boot disk" can mean either:
- A **Dorado Initial region** containing emulator microcode for the
  first-stage microcode boot.
- An **Alto-emulated disk partition** on a Dorado Trident pack. The
  Dorado user-ops memo says the Trident could hold many complete Alto
  disks; Alto-emulator microcode selects one of those partitions and
  boots it using Alto conventions.
- A **Cedar/Pilot volume** containing a germ and physical volume boot
  file.
- A **Pilot Backup** memory image (`.pb`) that captures a complete
  Pilot world and can bypass disk if we implement a loader for it.

## Boot documents mirrored locally

Primary Dorado/Alto/Pup/Pilot references now saved in the tree:

| Area | Local files |
|---|---|
| Dorado boot operation/implementation | `chm/doradodocs/DoradoUserOps.memo!3.html`, `chm/doradosource/DoradoBooting.tioga!2.txt`, `chm/doradosource/DoradoBootingImpl.bravo!1.html`, `chm/doradodocs/DoradoBooting*.pdf` |
| Dorado Initial/LoadRam/TriDisk sources | `chm/doradosource/BootstrapSources.dm!12_/`, `chm/doradosource/LoadMB.dm!6_/`, `chm/doradosource/TriDiskSources.dm!8_/`, `chm/doradomicrocode/loadmb/LoadMB.mesa!7.txt` |
| Alto software boot and disk utilities | `chm/altodocs/ALTOHARDWARE.PRESS!2.pdf`, `chm/altodocs/ETHERBOOT.TTY!2.html`, `chm/altodocs/NETEXEC.TTY!2.html`, `chm/altodocs/COPYDISK.TTY!2.html`, `chm/altodocs/BFS.TTY!2.html` |
| Pup/Alto boot protocols | `chm/pup/ALTOBOOT.BRAVO!1.html`, `chm/pup/ETHERBOOT.BRAVO!1.html`, `chm/pup/EFTPSPEC.BRAVO!1.html`, `chm/pup/PUPDEF.MAC!1.html`, `chm/pup/PUPSPEC.PRESS!1.pdf` |
| Cedar/Pilot boot artifacts | `chm/cedar/basiccedar/BasicCedarDorado.boot!14`, `chm/cedar/basiccedar/BasicCedarDorado.loadmap!69.txt`, `chm/cedar/germ/Dorado.germ!4`, `chm/cedar/germ/Dorado.loadmap!1.txt`, `chm/cedar/germ/BootChannelDisk.mesa!2.txt`, `chm/cedar/germ/BootChannelEther.mesa!3.txt`, `chm/cedar/germ/MiniEthernet*.txt`, `chm/cedar/germ/OthelloDorado.boot!8` |

## Trident T-300 disk packs (locally available)

Our `dorado/include/disk.h` driver implements **Trident T-80**
(815 × 5 × 9 × 2074-byte records ≈ 76 MB) and **Trident T-300**
(815 × 19 × 9 × 2074-byte records ≈ 289 MB). The preserved image
record is 2 dummy bytes + 2 header words + 10 label words + 1024
data words. Locally available pack files:

| File | Size | Format | Contents |
|---|---|---|---|
| `AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk300` | 276 MB | T-300 | Alto Spruce print server pack |
| `AltoInfo/ContrAlto2-beta/Disks/spruce-server-t300.zip` | (compressed) | T-300 | same, zipped |
| `AltoInfo/ContrAlto2-beta/Disks/tdisk4.dsk` | 2.5 MB | T-80? | small Trident sample |
| `AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk` | 2.5 MB | Diablo | Alto Diablo 30 (NOT Trident) |

`spruce-server.dsk300` is the right preserved Trident image format for
drive validation. It is an Alto Spruce print-server pack, not known to
contain a Dorado Initial microcode region or a Cedar physical volume, so
the normal Dorado hard-microcode disk boot may legitimately fall through
to Ethernet. Treat it as an Alto-emulator software-boot fixture, not as
a complete Dorado system pack.

The other `.dsk` files (`games.dsk`, `bcpl.dsk`, `bravox.dsk`,
`xmsmall.dsk`, etc.) are **Diablo 30 packs** (2.5 MB) used by ContrAlto.
These are NOT Trident, so the Dorado disk driver can't read them
directly.

## Microcode worlds (locally available)

In `chm/microcode/`:

| File | Size | What it is |
|---|---|---|
| `AltoMesaDorado.eb!1` | 34 KB | Alto-emulator + Mesa microcode for Dorado |
| `BasicCedarDorado.pb!1` | 513 KB | Cedar Pilot world for Dorado (microcode + Cedar OS) |
| `CedarDorado.eb!6` | (similar) | Cedar Ether-bootable microcode |
| `SmalltalkDorado.eb!1` | 31 KB | Smalltalk-80 emulator microcode |
| `TestDorado.eb!1` | 31 KB | Diagnostic / test microcode |
| `Initial.eb!1` | 7.8 KB | Initial loader (Ether-boot version) |
| `BootMeAgain.eb!1` | 43 KB | Secondary boot helper |
| `Fiala.eb!1` | 26 KB | Fiala microcode |
| `Murray.eb!1` | 22 KB | Murray microcode |

In `chm/dorado/`:

| File | What it is |
|---|---|
| `Mesa.mb!3` | Mesa emulator microcode (`.MB` source format) |
| `Cedar.mb!6` | Cedar emulator microcode |
| `AEmu.mb!2` | Alto emulator on Dorado (`.MB`) |
| `DSemu.mb!1` | Display-task emulator |
| `doradobaserom.mb!13` | BaseBoard 6502 ROM image |

The `.eb` (Ether-boot) and `.pb` (Pilot Backup) files are the
**runtime-loadable** forms — they're what Initial would normally pull
across Ethernet or load from disk. The `.mb` files in `chm/dorado/`
are the **MicroD-output** form (what we feed to our microcode loader
directly).

**Sibling-machine microcode** (Dolphin/D0 and Dandelion) — same .MB
format, useful as cross-references for understanding microcode
patterns:

| File | Machine |
|---|---|
| `AlphaMesaPilotD0.eb!1` | Mesa/Pilot for Dolphin |
| `AlphaMesaOthelloD0.pb!1` | Mesa Othello debugger for Dolphin |
| `PilotD0.eb!1` | Pilot OS for Dolphin |
| `AltoD0.eb!1` | Alto-emulator on Dolphin |
| `OthelloD0.pb!1` | Othello debugger for Dolphin |
| `BasicCedarD0.pb!1` | Cedar for Dolphin |
| `BasicCedarDLion.pb!1` | Cedar for Dandelion (Star/8010) |
| `OscarDicentra.pb!1` | Mesa world (Dicentra release) |

## What CHM does and does not have as raw disk packs

**Trident packs (T-80, T-300):** none obvious on either CHM mirror. The
only known public Trident pack image is the Spruce T-300 we already have
locally (`AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk300`).
Dorado-era Mesa/Cedar boot artifacts are present as `.eb`, `.boot`,
`.germ`, `.loadmap`, and `.pb` files rather than raw Dorado pack images.

**Alto Diablo-30 packs (`.bfs` / `.altodisk` / `.copydisk`):** yes —
9 of them in `_cd8_/basicdisks/` and `indigo/basicdisks/` on
`xeroxparcarchive`. Headline pack: `Mesa6-14.bfs!4` (4.9 MB, Mesa 6.14
development). Plus 4 D0/Dolphin diagnostic packs under `cyan/d0/*.disk!1_/`.
Our own `chm/dorado/UnBug.bfs!1` (2.3 MB) is also a BFS pack — the
Dorado developers' Alto debug environment.

These run under the **Alto-emulator microcode** on our Dorado
(`AltoMesaDorado.eb`/`AEmu.mb`), not under Mesa or Cedar microcode
directly. See `parc-archive-survey.md` for the full inventory and the
tools (`restore_alto_files`, `palo`, ContrAlto's AltoFS) that read
and write BFS packs.

## Pilot Backup (`.pb`) format

The Pilot Backup format captures a complete Mesa/Pilot world as a
memory image. It's similar to a "core dump" — the OS state, all
loaded modules, plus the microcode needed to run them, packaged into
a single file. To "boot" from a `.pb`:

1. Load the microcode portion into IM (replaces emulator microcode).
2. Load the OS portion into main memory.
3. Set PC to the world's entry point and run.

This bypasses the disk entirely. `BasicCedarDorado.pb!1` is the
Dorado example. We don't yet have a `.pb` loader in the emulator,
but the format is documented and straightforward to add when needed.

## Ether-Bootable (`.eb`) format

A subset of the Pilot Backup format, sized to fit in an Ethernet
boot transfer. Just the microcode and the minimal OS bootstrap;
expects to load further state from a server over Ethernet. Initial's
default boot path uses `.eb` files transferred via the Ethernet
controller. We have `AltoMesaDorado.eb`, `CedarDorado.eb`,
`SmalltalkDorado.eb`, `TestDorado.eb`, and the generic `Initial.eb`.

## Recommended boot recipes

### Recipe 1: Alto-emulator disk boot

This is still the best first display target:

1. Microcode: `chm/microcode/AltoMesaDorado.eb!1` — provides Alto
   emulator, Mesa, IFU dispatch, etc.
2. Microcode load path: Initial Ethernet MicrocodeBoot request offset
   `110B`, full boot-file number `3110B`, then LoadRam starts the
   emulator at the EB end-item start address.
3. Software boot path: once Alto/Mesa microcode is running, boot with
   BS up for disk, or BS plus boot-key combinations for Alto Ethernet
   software boot.
4. Disk target: an Alto-emulated disk partition on a Dorado Trident
   pack. `spruce-server.dsk300` is the current T-300 fixture, but it is
   not yet proven to contain the right bootable partition for our path.

This is the path we should target first because:
- We have every byte needed locally.
- Our T-300 disk driver matches the pack format.
- ContrAlto2 (`AltoInfo/Contralto2-2.0-Beta/Contralto/`) runs the same
  combination, so it's a working oracle for cross-validation.
- The Dorado docs say Alto-emulator-based microcode performs a standard
  Alto software boot. The local primary references are Alto Hardware
  section 3.3, `ALTOBOOT`, `ETHERBOOT`, `EFTPSPEC`, `NETEXEC`, and
  `COPYDISK`.

Current emulator status (2026-05-02): this remains the desired first
recipe, but it is not a proven local boot yet. The harness can load both
local `AltoMesaDorado` EB copies and keep the Alto keyboard words visible
as all-up in MDS, but the loaded world does not yet post the legacy Alto
disk command pointer at `VM 0521` and does not install a display DCB.
Spruce is still a useful Trident pack-format fixture; do not assume it
is sufficient by itself until the second-stage Alto/Mesa software boot
payload or disk command handoff is identified.

### Recipe 2: Alto-emulator Ethernet software boot

This is the alternative if local Alto disk boot remains blocked:

1. Load `AltoMesaDorado.eb` exactly as in Recipe 1.
2. Present Alto boot-key state for Ethernet software boot. BS+Quote is
   the NetExec path per Dorado boot docs.
3. Implement the Alto boot server side using `chm/pup/ALTOBOOT`,
   `chm/pup/ETHERBOOT`, and EFTP (`30B`/`31B` data/end packets) rather
   than the Dorado Initial MicrocodeBoot protocol.
4. Serve a small Alto boot file first, then NetExec or a disk utility
   such as copydisk/scavenger once packet mechanics are stable.

This does not require a local file-server volume at first, but it does
require the Alto Ethernet surface exposed by AEmu after LoadRam. It is a
different protocol from Initial's `264B`/`265B` microcode boot.

### Recipe 3: Cedar/Pilot disk boot

This is the more native Cedar path:

1. Load `CedarDorado.eb` directly via Initial Ethernet MicrocodeBoot
   offset `113B`, or use an InitialDisk/InitialEther Cedar variant.
2. Provide a Pilot/Cedar disk volume with `Dorado.germ!4` and the
   installed physical volume boot file.
3. Use `BasicCedarDorado.boot!14` and
   `BasicCedarDorado.loadmap!69.txt` as local references for the boot
   file and symbol map.
4. Use `BootChannelDisk.mesa`, `BootChannelEther.mesa`, and
   `MiniEthernetDriver.mesa` as the source references for germ-level
   disk and Ethernet channels.

The Dorado booting memo is explicit that Cedar microcode does not do the
Alto Ethernet software boot directly. To boot Cedar software over the
network on real hardware, users first booted Alto/Mesa NetExec, then
CedarNetExec, then the desired program. For our emulator, a local Cedar
disk/germ path is likely cleaner than trying to fake that entire chain.

### Recipe 4: Direct Pilot Backup world

If you want to skip the disk entirely:

1. Microcode + OS: `chm/microcode/BasicCedarDorado.pb!1`.
2. Boot path: BB → Bootstrap → Initial → load Pilot world directly
   into memory → run.

Needs a `.pb` loader (not yet implemented). This is probably not a
simple EB loader extension: local byte checks show `BasicCedarDorado.pb`
is not just a 256-word EB header followed by LoadRam items. Use the
newly mirrored `BasicCedarDorado.boot` and loadmap as format clues, but
expect to implement real Pilot Backup parsing.

### Recipe 5: Native Dorado Mesa from `.MB` sources

Load `chm/dorado/Mesa.mb!3` directly via our microcode loader (this
is what `probe_aemu` does for Alto microcode). No disk needed for
the microcode itself, but Mesa expects to read its boot file from
disk to find the OS image. Without that, Mesa just sits there.

This is the slowest path because it requires the full Mesa boot
sequence including Mesa's expected disk layout.

## Cross-validation with ContrAlto2

`AltoInfo/ContrAlto2-beta/` and `AltoInfo/Contralto2-2.0-Beta/` are
local snapshots of the ContrAlto Alto emulator. ContrAlto runs an
Alto, but it understands Trident T-300 disks and runs the same
microcode our Dorado would run when in Alto-emulator mode. So:

- **Same disk pack** can be mounted in ContrAlto and our emulator.
- **Same Alto OS** runs in both (BCPL, Mesa-on-Alto, etc.).
- **State diff** between them at instruction boundaries confirms
  microengine correctness.

That's the cross-validation strategy in `CLAUDE.md`. ContrAlto is
the ground-truth oracle for Alto-mode execution.

## Summary

- Microcode worlds: `.eb`/`.pb`/`.mb` files in `chm/microcode/`
  and `chm/dorado/`.
- Alto software boot docs: now mirrored under `chm/altodocs/` and
  `chm/pup/`.
- Cedar/Pilot boot artifacts: now mirrored under `chm/cedar/`.
- Disk packs: T-300 Alto pack (276 MB) ready as a drive fixture, but
  not yet proven as a complete Dorado boot pack.
- Cross-validation: ContrAlto2 source + binaries.

When the emulator is ready, start with Recipe 1 until either the
Alto-emulated disk path is proven or blocked by missing pack contents.
If blocked, Recipe 2 is the shortest network-first route to visible Alto
software. Recipe 3 is the more accurate Cedar/Pilot route, but needs a
Pilot volume model.
