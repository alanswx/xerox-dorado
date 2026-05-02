# Ethernet Bootstrap, Local OS Boot Plan

Goal: make the normal Dorado boot path use Ethernet for the Initial-stage
emulator microcode load, then choose one of two second-stage software boot
routes: local Alto-emulated storage, or Alto Ethernet software boot. This keeps
the first half hardware-like without requiring a full IFS, Gateway, or external
Pup network before the display/disk path is stable.

## Target Path

```text
BaseBoard ROM
  -> Bootstrap
  -> Initial
  -> fake Ethernet MicrocodeBoot server
  -> AltoMesaDorado.eb emulator microcode loaded by Initial/LoadRam
  -> Alto emulator starts
  -> second-stage software boot
       A: local Alto-emulated Trident partition
       B: AltoBoot/EFTP server for NetExec or a small boot file
```

First payloads:

| Stage | Local artifact | Why |
|---|---|---|
| Initial Ethernet microcode payload | `chm/microcode/AltoMesaDorado.eb!1` first; compare with `chm/dorado/AltoMesaDorado.eb!2` | Small, directly matches the documented Dorado `3110B`/`110B` Mesa/Alto-emulator boot-file path. |
| Local OS storage | `AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk300` | Known Trident T-300 pack image, already usable as a disk-controller validation target. It is not yet proven to contain a bootable Alto-emulated Dorado partition. |
| Alto software netboot docs | `chm/pup/ALTOBOOT.BRAVO!1.html`, `chm/pup/ETHERBOOT.BRAVO!1.html`, `chm/pup/EFTPSPEC.BRAVO!1.html`, `chm/altodocs/NETEXEC.TTY!2.html` | Primary references for the second-stage Alto Ethernet path after AEmu is running. |

Fallback payloads:

| Payload | Use |
|---|---|
| `chm/microcode/TestDorado.eb!1` or `chm/dorado/test/TestDorado.eb!5` | Smaller diagnostic payload if AEmu reaches too much unimplemented hardware. |
| `chm/dorado/CedarDorado.eb!6` | Later Cedar emulator microcode path; likely needs Pilot storage/germ work after LoadRam. |
| `chm/cedar/basiccedar/BasicCedarDorado.boot!14` + `chm/cedar/germ/Dorado.germ!4` | Cedar/Pilot disk-boot references, not payloads for Initial's microcode boot request. |
| `chm/microcode/BasicCedarDorado.pb!1` | Not for this path. It is a Pilot Backup memory image, useful for a future direct `.pb` loader. |

## Definition of Done

1. Initial reaches `EtherMicrocodeBoot`.
2. EOT emits a valid `MicrocodeBootRequest` packet.
3. The fake server maps request offset `110B` to `AltoMesaDorado.eb!1`.
4. EIT accepts all `MicrocodeBootReply` packets, copies the EB payload to
   `BootDataPtr`, receives the zero-length terminator, and sets `SeqNo = -1`.
5. Initial returns success from `EtherMicrocodeBoot` and calls `LoadRam`.
6. Loaded emulator microcode starts without using the direct probe loader.
7. Alto emulator reaches the second-stage software boot decision.
8. Either local disk boot reaches Alto disk reads, or Alto Ethernet software
   boot emits the Mayday/AltoBoot request.
9. We produce a display snapshot or equivalent trace showing post-emulator OS
   boot progress.

## Phase 1: Instrument the Current Initial Path

Purpose: prove exactly where Initial chooses disk versus Ethernet and capture
the I/O values it emits.

Tasks:

1. Add symbolic trace tags around `DiskHardMicrocodeBoot`, disk failure return,
   `EtherMicrocodeBoot`, `SetupEtherBootRequest`, EOT transmit, EIT receive,
   `LoadRam`, and first emulator PC after LoadRam.
2. Add a temporary trace for `Output<-B` to task 6/7 TIOA `016B` so the numeric
   `EControl` command words can be mapped to the source names:
   `TurnOnRx`, `TurnOffRx`, `TurnOnTx`, `TurnOffTx`, `WaitForBOP`, `SendEOP`,
   and `CountDown`.
3. Confirm the boot-key/default path can intentionally fall through to Ethernet
   after local hard-microcode disk boot fails. If not, force the selection in
   the probe only; do not bake that into the emulator core.

Expected result: a short trace showing request construction at VM `177400B`,
the command words for Ethernet control, and whether disk fallback reaches
Ethernet cleanly.

## Phase 2: Implement the Minimal Ethernet Controller Shell

Purpose: make Initial's EIT/EOT tasks see plausible hardware.

Tasks:

1. Add an Ethernet device registered on EOT/EIT task contexts with:
   - TIOA `015B` as `EData`,
   - TIOA `016B` as `EControl`.
2. Return a stable host address from `EControl` input. Use a nonzero local host,
   for example `42B`, in the high byte so `RSH[Input, 10]` gives Initial the
   host number.
3. Maintain controller state:
   - receiver enabled/disabled,
   - transmitter enabled/disabled,
   - transmit packet capture buffer,
   - receive packet queue,
   - current receive word index,
   - transmit abort flags, initially always clear.
4. Implement task wakeups at the behavioral level:
   - EOT wakes while transmitter is enabled and can accept another output word,
   - EIT wakes when receiver is enabled and a receive packet's first word is
     available,
   - `IOAtten` on EIT input is true only for the receiver status word,
   - `IOAtten` on EOT is false for the first path.

Expected result: Initial can turn Ethernet on/off and EOT can send a 15-word
request without collisions or data-late failures.

## Phase 3: Add the In-Process Microcode Boot Server

Purpose: serve local `.eb` files using the exact protocol Initial expects.

Tasks:

1. On `SendEOP`, parse the captured transmit packet:
   - word 1 must be `1000B`,
   - word 3 must be `264B`,
   - word 4 should be `1B`,
   - word 5 is the requested boot-file offset.
2. Map offsets:
   - `110B` -> `chm/microcode/AltoMesaDorado.eb!1`,
   - `111B` -> `chm/microcode/SmalltalkDorado.eb!1`,
   - `113B` -> `chm/dorado/CedarDorado.eb!6`,
   - `114B` -> `chm/microcode/TestDorado.eb!1`.
3. Parse the selected `.eb` as 16-bit words.
4. Validate word 0 is microcode version `1`; reject or trace otherwise.
5. Skip the first 256 words.
6. Queue `MicrocodeBootReply` packets with the server chunk pattern from
   `PupMicrocodeBooter.mesa`:
   - 255 payload words,
   - 255 payload words,
   - 258 payload words,
   - repeat until empty,
   - final zero-length reply.
7. For each reply:
   - Ethernet dest/source word = `LHost << 8 | RHost`,
   - Ethernet type = `1000B`,
   - Pup type = `265B`,
   - Pup ID high = `1B`,
   - Pup ID low = sequence number,
   - Pup length = `26B + 2 * payload_word_count`,
   - append payload at word 15,
   - append a dummy CRC word,
   - append receiver status word with low byte zero and `IOAtten`.

Expected result: EIT copies every chunk into main memory, advances `SeqNo`, and
finishes on the zero-length reply.

## Current Bring-Up Status (2026-05-01)

Implemented:

1. `dorado/include/ethernet.h` + `dorado/src/ethernet.c` provide an
   in-process Ethernet controller registered on EOT/EIT `EData`/`EControl`.
2. `DORADO_BOOT_ETHERNET=1` is now the default for the full bootstrap probe,
   so Initial falls through from failed hard-disk microcode boot to
   `EtherMicrocodeBoot`.
3. EOT reaches the `EData` transmit path once `IOAtten` uses real device
   attention instead of the old wakeup-bit proxy.
4. The fake server queues `AltoMesaDorado.eb!1` replies for boot-file offset
   `110B`; EIT consumes the queued reply stream.
5. `tests/test_ethernet.c` covers the packet-level fake server and reply
   status-word attention path.

Open blockers:

1. Initial's `ELoad_ A0` should clear the EOT retry/load mask, but the full
   probe leaves `RM[0x19]` set. `DORADO_ETH_FORCE_ELOAD_ZERO=1` is a
   temporary probe-side guard so EOT can reach the transmit path.
2. EOT currently fetches zeros from the transmit packet buffer instead of the
   15-word request Initial stored at VM `177400B`. The fake server accepts this
   all-zero, 13-word bring-up shape as offset `110B` only to exercise EIT.
3. EIT reads all reply words, but Initial never reaches
   `CheckChecksumAndLoad`; likely the same non-emulator task memory
   fetch/store path prevents the received packet buffer/filter logic from
   seeing the reply header and sequence words correctly.

Next fix: debug non-emulator task memory references with `MemBase=IOBR` in
EOT/EIT, specifically EOT `Fetch_ EOPtr` from VM `177400B` and EIT
`Store_ EIPtr`/subsequent `Fetch_ EIPtr` from page zero.

## Phase 4: Validate the Loaded EB Image Before LoadRam

Purpose: separate Ethernet transport bugs from LoadRam/microcode-start bugs.

Tasks:

1. Add a debug option to dump the memory region starting at `BootDataPtr` before
   `LoadRam`.
2. Compare the dumped words with the selected `.eb` file after skipping the
   256-word header.
3. Trace the final `BootDataPtr` and total words copied.
4. If the EB contains a checksum item that Initial verifies, log the computed
   value and the expected value from the end item.

Expected result: byte-for-byte or word-for-word match between served EB payload
and Initial's memory buffer.

## Phase 5: Cross LoadRam and Start AEmu

Purpose: prove the emulator microcode was loaded by the real Initial path.

Tasks:

1. Trace entry to `LoadRam` and the first branch out of it.
2. Confirm IM/IFUM/RM writes come from the Ethernet-loaded memory image, not
   the direct probe loader.
3. Confirm the first stable emulator execution point. Expected AEmu path should
   resemble the current direct-load probe:
   `STARTEMULATOR -> RESUMEEMULATOR -> SETUPBRS -> IFU dispatch`.
4. Preserve the direct `probe_aemu` path as a comparison oracle but keep it out
   of this boot path.

Expected result: AEmu starts from Initial-loaded `AltoMesaDorado.eb!1`.

## Phase 6A: Boot Alto Software from Local Disk

Purpose: continue from emulator microcode into local storage using the
Alto-emulator disk conventions described in the Dorado booting docs.

Tasks:

1. Mount `AltoInfo/ContrAlto2-beta/Disks/spruce-server.dsk300` as drive 0.
2. Ensure the boot mode handed to the Alto emulator selects local disk boot,
   not Ethernet software boot. If keyboard/back-channel is still incomplete,
   force this in the probe only.
3. Model the Alto-emulated disk command surface, not just raw Trident sectors.
   Primary references:
   - `chm/doradosource/TriDiskSources.dm!8_/TriDisk.mc.html` for the Alto
     Trident KBLK/KCB shape,
   - `chm/doradosource/AemuSources.dm!82_/AEm0.mc` and `ADefs.mc` for AEmu's
     legacy Alto boot command block,
   - `chm/altodocs/ALTOHARDWARE.PRESS!2.pdf` section 3.3 and `BFS.TTY!2.html`
     for Alto disk/software expectations.
4. Reuse existing disk traces:
   - DiskControl commands,
   - Format RAM setup,
   - seek/status path,
   - DiskData FIFO reads/writes.
5. Produce periodic display snapshots after LoadRam:
   - at frame 1,
   - frame 10,
   - frame 100,
   - and on first non-gray/nonblank framebuffer change.
6. Compare disk behavior with ContrAlto2 when the same `spruce-server.dsk300`
   pack is mounted.

Expected result: visible post-emulator boot progress on the framebuffer, or a
precise disk/display blocker with a trace that starts after successful Ethernet
microcode load.

## Phase 6B: Boot Alto Software from a Fake AltoBoot/EFTP Server

Purpose: continue from emulator microcode into the Alto software Ethernet boot
path if local Alto disk contents remain uncertain.

Tasks:

1. Preserve the Initial MicrocodeBoot fake server as-is. It serves Dorado EB
   files with Pup types `264B`/`265B`; it is not the software boot server.
2. Add tracing after LoadRam for the Alto Ethernet surface exposed by AEmu.
   The expected first request is the Alto software boot "Mayday" path, not a
   Dorado microcode boot request.
3. Use these local references:
   - `chm/pup/ALTOBOOT.BRAVO!1.html`,
   - `chm/pup/ETHERBOOT.BRAVO!1.html`,
   - `chm/pup/EFTPSPEC.BRAVO!1.html`,
   - `chm/altodocs/ETHERBOOT.TTY!2.html`,
   - `chm/altodocs/NETEXEC.TTY!2.html`.
4. Serve a deliberately small Alto boot file first. Once packet sequencing and
   memory deposit are stable, move to NetExec and then copydisk/scavenger.
5. Keep this server in-process and deterministic, just like the Initial fake
   server, until the emulator can boot headless reproducibly.

Expected result: AEmu emits an Alto software boot request and accepts an EFTP
boot file, giving us a post-LoadRam progress path even without a proven local
Alto partition.

## Phase 6C: Cedar/Pilot Local Disk Route

Purpose: document the native Cedar route so we do not force Cedar through Alto
software netboot semantics.

Tasks:

1. Load Cedar microcode via Initial offset `113B` (`CedarDorado.eb`), or start
   from an InitialDisk/InitialEther Cedar variant once Initial selection is
   robust.
2. Model a Pilot/Cedar disk volume containing:
   - `chm/cedar/germ/Dorado.germ!4`,
   - a physical volume boot file such as
     `chm/cedar/basiccedar/BasicCedarDorado.boot!14`.
3. Use symbol/source references:
   - `chm/cedar/germ/Dorado.loadmap!1.txt`,
   - `chm/cedar/basiccedar/BasicCedarDorado.loadmap!69.txt`,
   - `chm/cedar/germ/BootChannelDisk.mesa!2.txt`,
   - `chm/cedar/germ/BootChannelEther.mesa!3.txt`,
   - `chm/cedar/germ/MiniEthernetDriver.mesa!2.txt`.
4. Do not assume Cedar supports the same direct Ethernet software boot as
   Alto-emulator microcode. The Dorado booting memo says Cedar software
   Ethernet boot goes through Alto/Mesa NetExec and CedarNetExec.

Expected result: a native Pilot/Cedar boot path from local disk, separate from
the Alto-emulator disk and AltoBoot/EFTP paths.

## Phase 7: Harden the Fake Server

Purpose: keep headless and GUI future paths deterministic.

Tasks:

1. Make the boot-file mapping configurable by CLI/env:
   - default `110B=chm/microcode/AltoMesaDorado.eb!1`,
   - allow overriding to Test/Cedar/Smalltalk.
2. Add a headless assertion mode:
   - run until `EtherMicrocodeBoot` success,
   - run until `LoadRam` success,
   - run N frames after emulator start,
   - optionally dump screenshot at frame N.
3. Add tests for:
   - request parsing,
   - EB chunk sequence lengths,
   - reply packet headers,
   - zero-length terminator,
   - EIT status `IOAtten`,
   - word-order round-trip.
4. Keep wire-level features disabled by default:
   - no collisions,
   - no real CRC,
   - no external network dependency.

Expected result: repeatable CI-friendly boot milestones that still exercise the
real Initial Ethernet tasks.

## Non-Goals for the First Pass

- Full Pup routing.
- IFS or Gateway file service.
- External Ethernet/TAP interface.
- Wire-level phase encoding, real CRC, jamming, or exponential backoff.
- Pure network OS boot was a non-goal for the first pass, but it is now a
  plausible fallback if the local pack is not bootable. It is a separate
  second-stage AltoBoot/EFTP path, not the Initial microcode boot server.

## Main Risks

| Risk | Mitigation |
|---|---|
| Unknown `EControl` command encodings | Trace values emitted by Initial before writing behavior; bind traced values to the source-level command names. |
| EB word order wrong | Compare Initial memory buffer against the EB payload after the 256-word header. |
| Initial does not naturally fall through to Ethernet | Force the probe's boot selection temporarily; keep the core hardware model neutral. |
| LoadRam rejects the transferred image | Verify the exact served payload and checksum before debugging LoadRam. |
| AEmu starts but cannot boot Spruce disk | Treat as a separate post-LoadRam disk/display blocker and compare with ContrAlto2. |
| Display remains ambiguous | Use frame-counted headless snapshots and trace first framebuffer writes. |

## Immediate Next Coding Pass

1. Ethernet microcode boot is now past the first-pass milestone:
   Initial requests `0110`, the fake Ethernet device serves
   `AltoMesaDorado.eb!1`, the EB End item matches memory, and LoadRam
   jumps into Mesa/AEmu.
2. The documentation splits the next stage into Alto-style software boot
   from disk or Ethernet. Disk boot is still useful for Alto-pack testing, but
   network boot needs a different protocol: BS/Quote selects the Alto
   NetExec path, the loader sends a Mayday Pup (`244B`), and the server sends
   EFTP Data/End packets (`30B`/`31B`).
3. Keep `DORADO_ALTO_BOOT_ETHERNET=1` as a probe for the second-stage path.
   The current focused run shows the final AEmu keyboard words as
   `FFFE FFF7 FFFF FFFF`, but still records no post-LoadRam Dorado EOT/EIT
   packets and still reaches the disk path. Trace the AEmu keyboard read and
   `EBoot` branch next.
4. Once AEmu demonstrably reaches the Alto Ethernet loader, implement the Alto
   Ethernet/SIO surface exposed to the simulated Alto code, then add a minimal
   Mayday/EFTP boot server that can serve a known boot file or NetExec image.
5. Continue producing `/tmp/dorado_boot_display.pgm` as the headless visual
   artifact while either disk or network software boot advances toward
   display-list construction.
