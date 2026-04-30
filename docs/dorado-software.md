# Software for the Dorado

What's available to run on our Dorado emulator: microcode worlds,
disk pack images, and how each fits into the boot chain. This is a
status snapshot of what we already have locally and what CHM PARC
archives offer.

## TL;DR

We already have everything needed to boot. Two viable paths:

1. **Alto-on-Dorado** (recommended for first boot): load
   `chm/microcode/AltoMesaDorado.eb!1` as the emulator microcode and
   mount `AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk300` as the
   Trident T-300 disk pack. Our T-300 disk driver is compatible.
2. **Cedar-on-Dorado**: load `chm/microcode/BasicCedarDorado.pb!1`
   directly into memory — it's a self-contained Pilot world (microcode
   + Cedar OS as a memory image), no disk needed.

## What "boot disk" means for the Dorado

The Dorado boot chain has several layers, and "disk" only enters at
the end:

```
EPROM Boot0  ──►  Bootstrap  ──►  Initial  ──►  emulator microcode  ──►  OS
(BB ROM)         (in IM 7700-7777)  (loads emu)   (Mesa/Cedar/Alto)       (Pilot/Trident/...)
```

The first three stages (Boot0, Bootstrap, Initial) come from the
BaseBoard ROM. Then **Initial loads emulator microcode** — typically
either over Ethernet (`.eb` Ether-Bootable format), from disk, or from
a Pilot Backup memory image (`.pb`). Once the emulator microcode is
running, the OS reads from disk in its own format.

So "boot disk" can mean either:
- A **Trident pack** holding files the emulator-OS reads (Alto Spruce
  pack, Mesa world disk, etc.) — this is the disk hardware presents
  to the emulator.
- A **Pilot world** memory image — the OS state captured as a single
  memory snapshot, loaded directly into RAM without any disk involved.

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
contain Initial's private hard-microcode boot file at page 4, so the
normal Initial disk boot may legitimately fall through to Ethernet.

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

## What CHM does NOT have as raw disk packs

Searched `chm/cross-reference.html` (45 MB, 568K lines indexing the
xeroxparcarchive mirror) for Trident `.disk` / `.image` pack files:
**none exist as raw disk packs**. The PARC IFS archives store
individual files (`.bcd`, `.mesa`, `.config`, etc.), not raw disk pack
images. Dorado-era Mesa/Cedar worlds were always distributed as
`.eb` or `.pb` files, never as disk packs.

So if you specifically want a Mesa-on-Dorado **disk pack**, that's
not a thing the public archives have. The closest is the Spruce pack
(Alto, T-300) we already have.

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

### Recipe 1: Alto-on-Dorado with Spruce pack

The minimum-viable first-boot:

1. Microcode: `chm/microcode/AltoMesaDorado.eb!1` — provides Alto
   emulator, Mesa, IFU dispatch, etc.
2. Disk: mount `AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk300`
   as Drive 0.
3. Boot path: BB → Bootstrap → Initial → load `AltoMesaDorado` →
   Alto-emulator runs Spruce.

This is the path we should target first because:
- We have every byte needed locally.
- Our T-300 disk driver matches the pack format.
- ContrAlto2 (`AltoInfo/Contralto2-2.0-Beta/Contralto/`) runs the same
  combination, so it's a working oracle for cross-validation.

### Recipe 2: Cedar Pilot world

If you want to skip the disk entirely:

1. Microcode + OS: `chm/microcode/BasicCedarDorado.pb!1`.
2. Boot path: BB → Bootstrap → Initial → load Pilot world directly
   into memory → run.

Needs a `.pb` loader (not yet implemented). Path of less code, but
more upfront work.

### Recipe 3: Native Dorado Mesa from `.MB` sources

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

We have everything. No archive download needed.

- Microcode worlds: 17 `.eb`/`.pb`/`.mb` files in `chm/microcode/`
  and `chm/dorado/`.
- Disk packs: T-300 Alto pack (276 MB) ready to mount.
- Cross-validation: ContrAlto2 source + binaries.

When the emulator is ready, start with Recipe 1 (Alto-on-Dorado
with Spruce pack) for the first boot demonstration.
