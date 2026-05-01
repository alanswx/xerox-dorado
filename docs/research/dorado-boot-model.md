---
gap: H1, A3 (re-frames both)
status: synthesis
---

# What "booting a Dorado" actually means

Synthesis of three primary sources, read 2026-05-01:

1. **`Dorado_Booting_Operation_and_Mechanisms_Apr80.pdf`** — Ed Taft's
   April 21, 1980 memo (`[Ivy]<DoradoDocs>DoradoBooting.press`).
   The canonical original-design document.
2. **`DoradoUserOps.memo!3`** at
   `_cd8_/doradodocs/.DoradoUserOps.memo!3.html` — user-facing
   operations memo (1979).
3. **`DoradoBootingImpl.bravo!1`** at
   `_cd8_/doradosource/.DoradoBootingImpl.bravo!1.html` — the
   1983 implementation memo.

Plus the in-tree microcode: `InitialMain.mc`, `InitialDisk.mc`,
`InitialEther.mc`, `BootstrapMain.mc`, `PilotDisk.mc`,
`PilotDiskDefs.mc`, `DiskSubrs.mc`, `DiskBootTransfer.mc`.

This document re-frames gaps **A3** (Trident pack with hard
microcode) and **H1** (Ethernet boot) — A3 is no longer the
right first blocker to chase.

## Two distinct boot stages

Booting a Dorado is **two separate things**:

1. **Microcode boot** — load an emulator (Mesa / Cedar / Lisp /
   Smalltalk) into the Dorado's control store (IM + IFUM).
2. **Software boot** — once the emulator runs, it does an
   **Alto-style software bootstrap** (load Alto OS / NetExec /
   Pilot) from disk partition or Ethernet.

Quoting the 1980 memo verbatim:

> "When the emulator microcode is started … it initiates an
> Alto-style software boot from either disk or Ethernet,
> according to the keys that are pressed down. … The Alto-style
> software boot is described in Alto documentation: the Alto
> Hardware manual, the BuildBoot documentation, and the Alto
> Boot protocol specification."

So the disk pack the running emulator sees is **a standard Alto
disk in Model-31 / Diablo-30 format**, just embedded in a
Trident. A T-80 Trident holds **up to 5** Alto-format
partitions, numbered 1–5; T-300 holds up to 19. The emulator
runs the same Alto Boot protocol an Alto would.

## The 1980 original design has NO disk microcode boot

The 1980 memo says explicitly:

> **Microcode boot from disk** — "Eventually it will be possible
> to bootstrap emulator microcode from the disk as well as from
> the Ethernet, as is done for the D0. This will require some
> disk format changes and some other changes that are entangled
> with bringing up Pilot on the Dorado. **Therefore, implementing
> microcode booting from the disk has been deferred.**"

In the **original** (1980) design, the disk contains *no*
emulator microcode at all. Microcode is loaded **exclusively
over Ethernet** via the Pup `EtherBoot` protocol:

> "Initial makes contact with an Ethernet boot server (located in
> a Gateway or IFS system) and requests it to supply the selected
> emulator microcode. That microcode is transferred over the
> Ethernet and stored in the Dorado's main memory. (This step
> employs the **Pup Microcode Boot protocol** … documented in
> `[Maxc]<Pup>EtherBoot.press`.)"

The Initial-microcode-from-disk path that `InitialDisk.mc`
implements (`firstHardUCodePage = 4`,
`hardUCodeSerial = 0o64732`) is a **June 1982 addition** — it
postdates the original by two years and is the "Pilot / Othello"
path that gap A3 was chasing.

## Boot file numbers (Ethernet)

```
3100 DoradoMesa.eb
3101 DoradoSmalltalk.eb
3102 DoradoLisp.eb
3103 DoradoCedar.eb
```

The Dorado's Initial sends a small request (file numbers
100/101/102/103, the boot server adds 3000); the gateway streams
the `.eb` file back.

We **already have all four `.eb` files locally** in
`chm/microcode/` and `chm/dorado/`:

```
chm/microcode/AltoMesaDorado.eb!1     (35 KB)
chm/microcode/Initial.eb!1
chm/microcode/SmalltalkDorado.eb!1
chm/dorado/CedarDorado.eb!6
chm/dorado/AltoMesaDorado.eb!2
chm/dorado/InitialAltoMesaDorado.eb!3
chm/dorado/InitialEther{AltoMesa,Cedar,Test}Dorado.eb!3
```

## EB file format (also from the 1980 memo)

The `.eb` file consists of an overhead page followed by an Item
array. Each Item is 4 words:

```mesa
Item: TYPE = MACHINE DEPENDENT RECORD [
    extralM:  [0..17B],   -- LHParityBad, RStk[0], RHParityBad, Block
    unused:   [0..777B],
    type:     [0..7B],    -- {IM, IFUM, End}
    addr:     WORD,
    word0:    WORD,
    word1:    WORD
]
```

For an `IM` item, `addr` is the absolute IM address, `word0`/
`word1` are LH/RH halves. For `IFUM`, `addr` includes InsSet in
the high 2 bits. The `End` item has `word0` = checksum,
`word1` = microcode starting address. The two's-complement sum
of all words across all items must be zero.

`LoadRam` is the microcode routine that consumes this Item array
and pushes it into IM + IFUM. It lives at the same fixed
location in Initial AND every emulator (so it can replace the
microcode without overwriting itself). The 1980 memo describes
the Mesa instruction `LoadRam[itemArray, flag]` (MISC 3) and the
Alto opcode `61036B`.

## What this means for our boot probe

The simplest and most historically-faithful first-light path is
the **1980 Ether-only path**:

1. BaseBoard does its thing → loads Bootstrap → loads Initial.
   **Already works in our model.**
2. Initial does Pup `EtherBoot` request → gateway delivers
   `DoradoMesa.eb` (or AltoMesa). **This is gap H1
   (Ethernet) — currently unimplemented.**
3. Initial calls `LoadRam` to push the `.eb` content into IM/IFUM
   and jump to the emulator's `InitMap` (octal 1076). **`LoadRam`
   is well-spec'd in the memo; documented item-array format with
   End-item checksum.**
4. The emulator microcode runs.
5. Emulator does **Alto-style software boot** — either from disk
   partition (need a T-80 pack with an Alto-format partition 1)
   or from Ethernet (Pup `AltoBoot` for OS / NetExec).

For first-light **without any Trident pack at all**:
- Steps 1–4 via Ethernet microcode boot.
- Step 5: emulator does **Pup AltoBoot** — also Ethernet —
  fetching NetExec or an Alto OS image.

This is the same protocol Alto / ContrAlto already speaks; we
have the Pup spec to pull, and the source for the Alto-side
handler is in `AltoInfo/ContrAlto2-beta/`.

## Revised Phase 4 framing

**A3 (Trident pack) is no longer the right blocker to chase
first.** The original Dorado boots without one. The right path
is:

- **H1 (Ethernet boot)** ★ — implement the Pup `EtherBoot`
  protocol enough to deliver a `.eb` file. This unblocks
  first-light. We already have the `.eb` files. Spec is in
  `[Maxc]<Pup>EtherBoot.press` (need to pull) and the Pilot /
  Iago / NetExec sources in our local mirror.
- **(Optional, for OS boot from disk)** Build a Trident pack
  with Alto-format partitions populated with an Alto OS disk
  image (we have multiple Alto OS disks in
  `AltoInfo/ContrAlto2-beta/Disks/` — `nonprog.dsk`,
  `bcpl.dsk`, etc.). Standard Diablo-30 layout, no Pilot /
  Othello hard-microcode file required.
- **A3 stays "later"**: chasing the 1982+ disk-microcode-boot
  path is only useful if we want to run Pilot / Cedar
  specifically. For Mesa-on-Alto-OS or Cedar-stand-alone, the
  Ether path suffices.

## Boot button semantics

From the 1980 memo (operation section):

| Pushes | Effect |
|---|---|
| 1 | "Alto-style" — currently-running microcode does its software bootstrap (= reboot Alto OS / NetExec). No BB action. |
| 2 | Force-restart current emulator. BB resets the engine; microcode in IM untouched. Zeros main memory. Reverts to default disk partition. |
| 3 | Full microcode bootstrap. BB loads Bootstrap → Initial → emulator microcode via Ether. Hold a key for non-Mesa: `C` / `L` / `M` / `S` (or `=` / `[` / `RETURN` / `SHIFT`). Hold ≥ 10s. |
| 4 | Shutdown (~30s). |
| Power-on | Equivalent to 3-push at +60s (waits for disk ready). |

Software-side keys (= what the emulator interprets, identical to
Alto): `BS` + `Quote` for NetExec, etc.

Our `probe_full_boot` corresponds to a 3-push (or power-on)
boot. The path we should drive end-to-end is **3-push → Ether
load of `AltoMesaDorado.eb` → Mesa emulator running**.

## Status-light protocol

The green light on the front panel reports BB-tracked state:

| Pattern | Meaning |
|---|---|
| 1 flash | Bootstrap sequence in progress (power-on / 3-push) |
| 2 flashes | Bootstrap sequence failed |
| 3 flashes | Transient power-supply problem since last boot |
| 4 flashes | Present power-supply problem |
| 5 flashes | Shutdown (4-push) |
| 6 flashes | Overheated, auto-shutdown |
| 7 flashes | Midas has taken control |
| Solid on | Boot succeeded, no problems |
| Solid off | Serious malfunction or AC off |

We model the green-light counter in `dorado_baseboard`. Worth
adding the patterns to `test_baseboard` once H1 is in.

## EProm layout (for reference)

The 1980 memo confirms exactly what we already had from the BB
ROM symbol table:

| EProm | Address | Contents |
|---|---|---|
| F000 | (board pos c61) | BB microcomputer code |
| F800 | (board pos b61) | BB microcomputer code (cont.) |
| C000 | (board pos f60) | Initial microcode + boot tables |
| C800 | (board pos e60) | Initial microcode (cont.) |

> "The microcomputer code and Bootstrap and Initial microprograms
> are placed independently so as to minimize the number of EProms
> that have to be reprogrammed when a change is made.
> Specifically, the microcomputer code and Bootstrap are
> contained in F000 and F800, while Initial and some tables that
> control loading of both Bootstrap and Initial are contained in
> C000 and C800."

This matches our `bbdis` survey (F000–FFFF = BB code + Boot0
data; C000–D7FF = Boot1Data and BootBlocks) and explains why the
1987-firmware rev had a 5th chip at D000–D7FF: Initial outgrew
its 2 KB once Pilot disk-microcode-boot was added.

## What to pull next

To implement H1 properly, fetch:

1. **`[Maxc]<Pup>EtherBoot.press`** — the Pup Microcode Boot
   protocol spec. Try
   `https://xeroxparcarchive.computerhistory.org/_cd8_/pup/`
   first; fallback to `xeroxalto/Indigo/Pup/`.
2. **`<Pup>AltoBoot.press`** — the Alto Boot protocol spec
   (used by step 5).
3. **`InitialEther.mc`** — already pulled into
   `chm/dorado/expanded/BootstrapSources.dm/`. Re-read with the
   1980 memo's Pup-protocol context.
4. **The gateway-side `EtherBoot` server source** — likely under
   `_cd8_/pupgateway/` or `_cd8_/alto/etherboot.dm!1_/`. Useful
   to confirm we satisfy the protocol from both sides.

## Recommended next step

Pivot from "build a Trident pack with hard microcode" (A3) to
**"implement Pup EtherBoot well enough to deliver
`AltoMesaDorado.eb` into Initial's main-memory buffer and let
LoadRam install it"** (H1). The `.eb` files are on hand; the
protocol is documented; this is the historically-correct first
boot path. The Trident pack work becomes optional follow-up for
OS-on-disk experiments.
