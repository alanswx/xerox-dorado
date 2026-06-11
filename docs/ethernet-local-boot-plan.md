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

## Acquired Stage-2 payloads (2026-06-08)

The Stage-2 boot payloads are now local in `chm/bootfiles/`, pulled from
the CHM PARC/Alto archives; every file's size matches the index exactly.
Two boot-file layouts are present: the small standalone tools lead with
`01 05 ...`, the larger OS/NetExec images lead with `00 e5 ... 01 03`.

Small standalone tools (self-contained Alto memory images, no OS/disk):

| File | Size | Role |
|---|---|---|
| `CRTTEST.BOOT!1` | 7080 | Smallest standalone boot; draws on the CRT. Best first "do we get pixels" test. |
| `DMT.BOOT!22` | 15430 | Display Memory Test; second small visible payload. |
| `NETEXEC.BOOT!8` | 43640 | Canonical Alto Network Executive - the documented BS+Quote netboot target. |

Full OS / environment net-boot images:

| File | Size | Role |
|---|---|---|
| `NEWOS.BOOT!21` | 130560 | Full Alto Operating System (Exec desktop), net-bootable. |
| `MesaNetExec.boot!1` | 130048 | Mesa environment network executive. |
| `AlphaMesaMesaNetExec.boot!1` | 130048 | AlphaMesa/Mesa network executive. |
| `CedarNetExec.boot!4` | 130560 | Cedar environment network executive (on-ramp to Cedar software). |
| `MazeWar.boot!1` | 130048 | Demo; unmistakable visual "it really booted" payload. |

The Mayday/EFTP server in Phase 6B can stream any of these directly. Start
with `CRTTEST` (smallest, visible), then `NETEXEC`, then `NEWOS` for a full
desktop. Full Alto *disk* images for the disk route + ContrAlto cross-
validation already exist under `AltoInfo/ContrAlto2-beta/Disks/`
(`games.dsk`, `nonprog.dsk`, `diag.dsk`, etc.). Protocol references local:
`chm/pup/{ALTOBOOT,ETHERBOOT,EFTPSPEC}.BRAVO`,
`chm/altodocs/{ETHERBOOT,NETEXEC}.TTY`.

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

### Client-side bring-up status (2026-06-08)

Server side is done and unit-tested (see `ethernet-architecture.md`). The
client side does not yet drive it. Findings from the forced-Ethernet probe
(`DORADO_ALTO_BOOT_ETHERNET=1 DORADO_ALTO_BOOT_QUOTE=1 DORADO_ALTO_BOOT_SHIM=0
DORADO_ETH_EFTP_BOOT=.../CRTTEST.BOOT!1 DORADO_BOOT_BUDGET=160000000`):

- **Stage 1 still works**: microcode boot `requests=1 bfn=0110 replies=69`,
  EB checksum matches, `LoadRam` runs, loaded world starts.
- **No Mayday is sent**: `eftp_requests=0`, `last_tx_pup=0264`. The loaded
  world never enters the Alto Ethernet software-boot path; it runs the
  Alto-disk loop instead (`ENDALTOLOOP`, `READ1MUFF`, `AEMU_WAITFORSECTOR`).
- **The boot-mode steering does not land.** `AEm0.mc` `ABoot` reads the
  keyboard at VM `177034` (`T_ (R400) XOR 177434C`) and does
  `Branch[EBoot, R even]` - EBoot only if the BS-key LSB of word `177034`
  is 0 (down). `ResumeEmulator` first stores `-1` (all keys up) to
  `177034..177037`, then `ABoot` waits ~100 ms for the terminal to update
  them. We have no DDC keyboard back-channel (gap E2), so nothing updates
  them, and the probe's seed is not reliably present at read time.
- **Two concrete bugs to fix in the probe diagnostics / seeding:**
  1. The Alto emulator reads its keyboard via `MemBase=MDS` (`BR[MDS,36]`,
     and in Alto mode `IOBR == MDS`), **not** the Mesa `IOBR=BR31`. The
     probe's `boot_keyboard_base`/`boot_keyboard_word` use BR31. A new
     `boot_keyboard_word_at(mem, BR36, ...)` report was added; both bases
     read `0000` at exit, so BS-down is not being held at `MDS+177034`.
  2. **Open question that gates everything here:** the offset-`0110` world
     ("AltoMesaDorado.eb") **fingerprints as Mesa** and runs Alto-disk code
     at `0o5xxx` (`ALTOLOOP=0o5434`), whereas standalone `AEmu.mb!2` puts
     `ABoot`/`AEMU_ALTOLOOP` at `0o44xx`. So the `AEm0.mc` `ABoot` decision
     traced above may belong to a different world than the one actually
     loaded. Before more keyboard-seed work, determine which emulator
     offset `0110` loads and what *its* disk-vs-Ethernet software-boot
     decision is (Mesa/Pilot boots Pilot from disk or via NetExec, a
     different path than the Alto EtherBoot Mayday/EFTP).

**Finding (2) RESOLVED (2026-06-08).** `AltoMesa.cm`
(`chm/doradomicrocode/doradomicrocodesources/AltoMesa.cm!1`) shows offset
`0110` ("AltoMesaDorado") is built `MicroD -nl Mesa` from the **Mesa
emulator** modules (`AltoMesaArith/Faults/Jumps/LS/Process/RW/Xfer/...`)
plus the Alto emulator as a *guest* instruction set (`AltoEmu`,
`AltoDiabloDisk`, `AltoEther/AltoEtherEmu`) and I/O. It is therefore a
**Mesa world that boots Pilot**, not a standalone Alto. Confirmed by the
local boot sources for this world: `InitMem.mc`, `PilotInitMem.dib`,
`DiskBootSoft.mc`, `DiskBootTransfer.mc` (Pilot disk boot) - and by the
absence of `AEm0.mc` (the standalone Alto `ABoot`/`EBoot`/`DiskBoot`
logic) from the module list. So this world never issues an Alto Mayday;
the `AEm0.mc ABoot` analysis above applies to the *standalone* Alto
emulator (`AEmu.mb`), which the standard netboot offsets do not load
(3110=Mesa, 3111=Smalltalk, 3112=Lisp, 3113=Cedar, 3114=Test - no
pure-Alto offset).

**Consequence - the two Stage-2 routes are now clearly separate:**

- **Route A (matches the loaded world): Mesa/Pilot disk boot.** The
  offset-`0110` world wants `DiskBootSoft`/`DiskBootTransfer` to read a
  Pilot physical volume + germ from disk. Blocked on: no Pilot Dorado
  pack exists (established earlier) and the disk read-transfer path
  (F1-F5). The EFTP/Mayday server does not apply here.
- **Route B (what the EFTP/Mayday server + `chm/bootfiles/` payloads
  serve): standalone Alto EtherBoot.** Needs a standalone Alto emulator
  world loaded (e.g. `AEmu.mb` via the LoadMB/Path-B mechanism, like
  `probe_aemu`), whose `AEm0.mc ABoot` reads the keyboard and branches to
  `EBoot` -> Mayday. The server already answers this; the work is loading
  that world and steering BS-down at `MDS+177034`.

**Next step (decision required - see chat):** pick Route A or B. Route B
is the one the new server unlocks and the one with content in hand
(NetExec/CRTTest), but it requires bringing up a standalone-AEmu boot
rather than the Mesa netboot world. Route A is the world that actually
loads today but is blocked on Pilot disk content that does not exist.

### EFTP server validated by simulation (2026-06-08)

`test_ethernet.c::test_eftp_boot_full_transfer` simulates the booting
Alto's `ReceiveEFTPPacket` loop against the server and reassembles the
stream byte-for-byte (`CRTTEST.BOOT` 3540 words, `NETEXEC.BOOT` 21820
words). So the EFTP transfer is proven correct independently of any
on-machine bring-up; a future client failure is attributable to the
client, not the wire protocol.

### Route B on-machine status (2026-06-08) - root cause found

Ran `probe_aemu` (standalone AEmu: Initial+kernel+memMisc+IfuComplex+AEmu
direct-loaded, run from START/STARTEMULATOR) against the current engine
and traced it to ground. Findings (probe now prints boot symbols, tasking
/junk state, the RTC value, and a spin disassembly):

- AEmu runs **much further than the old 203-cycle `JNKINITPC` halt**:
  `STARTEMULATOR -> RESUMEEMULATOR -> SETUPBRS -> DOBRS -> InitTasks ->
  ABoot`. Boot symbols (this load): `INITTASKS=0o1123 ABOOT=0o724
  EBOOT=0o2006 DISKBOOT=0o2005 AEMUNEXT=0o745`.
- It **stalls inside `ABoot` at real `0o736`** (10 instructions past
  `ABOOT=0o724`) - the `T_(R400)+(30C); ... Fetch_ T, Branch[.-1, ALU>=0]`
  **100 ms RTC wait loop**. `T=0x118 = 0o430` confirms it is polling the
  Alto RTC at VM 430.
- Tasking is on, the junk timer is enabled, and the **JNK task actually
  runs (1261x)** - yet `VM430 = 0x0000` and never advances, so the wait
  never exits.
- **Root cause: `MDS = 0x00000`.** The bypass never establishes the
  emulator-handoff state Initial provides (`EmuBRHiReg/RM[0x18]` should be
  `0x0D24` -> `MDS=0xD240000`; map entries; RTClock init). With MDS and
  the clock machinery uninitialized, the JNK task's MemBase-relative
  clock store does not land at the VM 430 ABoot polls. So the blocker is
  **not** a missing single emulator feature - it is that the standalone
  -AEmu *bypass* skips Initial's setup.

**Implication for "implement correctly":** the right vehicle is real
`Initial -> emulator handoff`, which sets MDS/BRs/map/RTClock before
`STARTEMULATOR`. Two ways to get there for *standalone* Alto:
  1. **Serve a standalone-Alto `.eb` at netboot offset `0110`** so the
     real Initial loads AEmu (not AltoMesa/Mesa) and hands off with full
     setup, then AEmu reaches `ABoot`, EBoots, and drives the EFTP server.
     Needs an AEmu `.eb` (we only have `AEmu.mb`; would require MicroD/
     LoadMB-style `.eb` generation, or finding one in the archive).
  2. **Faithfully reproduce Initial's emulator-handoff state in the
     bypass** (plant `EmuBRHiReg`, map the MDS region, init RTClock) -
     bounded but requires the JNK-task clock source to verify, which is
     not in the local AEmu files.

Until one of those lands, the validated EFTP server has no correctly
-initialized client to answer on-machine. The EFTP transfer itself
remains proven by `test_eftp_boot_full_transfer`.

#### Update: ABoot's RTC wait analyzed (2026-06-08, cont'd)

Re-examined the `0o736` stall with the Junk-task clock source
(`chm/doradomicrocode/doradomicrocodesources/Junk.mc!1`). The RTClock is a
DDA: each JNK wakeup adds ~0.84 to a 22-bit fraction; the Alto clock is
VM 430 (high 16 bits) + `RTClock[0:9]` (low 10). VM 430 ticks only every
~1000+ JNK wakeups. The JNK task runs with `MemBase=IOBR(=MDS)`, so even
with `MDS=0` its VM 430 store lands at physical `0x430`, exactly where
ABoot reads. So the mechanism is correct - it was just **starved of
cycles**: the probe's 200k-cycle budget gave only ~1300 JNK wakeups
(VM 430 = 1). `probe_aemu`'s budget is now `DORADO_AEMU_CYCLES`
(default 200k). At 8M cycles JNK runs ~19k times and **VM 430 reaches 6**
(past ABoot's +3 target) - yet ABoot **still loops at `0o736<->0o755`
with `T=0o430`** (still reading VM 430). So passing the numeric target is
not sufficient: the loop's compare is not seeing the advanced value.

#### RESOLVED (2026-06-08, cont'd): it was a small-constant ALU bug

A per-cycle trace of the loop disproved the Md-timing theory: it is pure
task 0 (no interleave) and `Md` is read correctly. The real bug was in the
**Dorado small constant `nS`**. ABoot's `ETemp0_ (3S)+MD` (real `0o751`,
`ASEL=A<-RM/STK`, FA=0 FB=0, FF[4:7]=3) must compute `3 + Md`, but the
emulator computed `(oldETemp0 | 3) + Md` because the FA=0 FB=0 A-override
only overlaid the low nibble onto RM/STK instead of forcing
`A = {0,,FF[4:7]}`. So the RTC target came out `0o423` instead of `3` and
the wait could never end.

Fix in `src/cpu.c` (A-bus override): for the full-function form
(`ASEL > 3`, FA=0 FB<=1) A is now the small constant with `A[0:11]=0`; the
memory-reference Mar tweak (`ASEL 0/1`) still keeps the high bits. Proof
the zeroing is correct: `0o751` reads and writes the same register, so
keeping the high bits would corrupt every `nS` constant and Taft's
canonical microcode could not work. `test_a_low_ff_override` was corrected
to the real-hardware semantics (it had encoded the wrong keep-high
expectation). Full suite green. This is a broad fix - small constants on
the A bus are pervasive, so it likely also helps the Mesa boot path.

**Result:** ABoot now computes `ETemp0=3`, exits the RTC wait once VM 430
passes 3 (~5M cycles), and standalone AEmu advances into the keyboard /
disk-vs-Ethernet boot decision.

#### probe_aemu environment built out (2026-06-08, cont'd)

`probe_aemu` now attaches the slow-IO bus + the in-process Ethernet/EFTP
controller, maps a 512-page MDS region, and (when `DORADO_AEMU_EBOOT=1`,
default) holds BS down at `MDS+177034` to steer ABoot to Ethernet boot.
With these the bring-up advances much further:

1. Full MDS map fixed the `NOTEMUFAULT` page-fault on ABoot's keyboard
   fetch (VM 177034). Without BS forced, AEmu now reaches `KWAIT` -
   DiskBoot waiting on a (nonexistent) disk.
2. With BS down it takes **EBoot** (`0o2006`) - the Alto Ethernet
   software-boot path - and runs a few instructions before faulting at
   `0o2021` (a `MemBase<-FF` store) back to `NOTEMUFAULT`.

So AEmu reaches EBoot but cannot complete it: EBoot needs more of Initial's
emulator-handoff than the bypass provides. Planting `EmuBRHiReg=1`
(`DORADO_AEMU_EMUBRHI`) so EIBR/EOBR/MDS are coherent was necessary but
not sufficient - EBoot also wants the Ethernet command blocks in memory
and the EtherBoot bootloader copy. **Conclusion:** driving the validated
EFTP server on-machine requires the *real* `Initial -> emulator` handoff,
not the bypass; the bypass got us to EBoot for diagnosis but is the wrong
vehicle to finish it.

**Headline fix:** the Dorado **small-constant `nS`** A-bus bug
(`src/cpu.c`) - A is now `{0,,FF[4:7]}` for the full-function form, not a
low-nibble overlay on RM/STK. Broad (small constants are everywhere) and
unblocked ABoot's RTC wait. `test_a_low_ff_override` corrected; suite green.

#### Did it correctly: real Initial -> AEmu via an .eb (2026-06-08)

Instead of the bypass, the standalone Alto emulator is now loaded the
*real* way: a new `mb2eb` tool (`src/mb2eb.c`, `build/mb2eb in.mb out.eb`)
converts a `.MB` into an Ethernet-boot `.eb` (LoadRam Item array + balanced
checksum + InitMap start address `0o1076`), implementing the
`LoadRam.mc!1` Item format (IM `word0=iw0`, `word1=FF,,JCN`, extraIM nibble
`LHpar,,RSTK[0],,RHpar,,BLOCK`; IFUM `word0/1=ifum_lo/hi`; End checksum so
the 16-bit sum of all payload words = 0).

`build/mb2eb ../chm/dorado/AEmu.mb!2 /tmp/AEmu.eb` then
`DORADO_ETH_BOOT_110=/tmp/AEmu.eb` in the full-boot probe: **real Initial
requests `0110`, receives AEmu (38 reply packets), passes
`CheckChecksumAndLoad`, runs `LoadRam`, and hands off to `InitMap`
(`0o1076`)** - no bypass, no planted state. AEmu's own InitMem then runs
(MDS=0x10000 set by real InitMap; ABoot's RTC target computes correctly as
`ETemp0=3`).

**New frontier:** AEmu's InitMem does **not terminate** its map
enumeration. At 300M cycles `NEXTMAP1` (`0o3262`) has run ~2.2M times -
far more than the 16384 VM pages (VirtualBanks=0x40 -> 64 banks x 256) it
should enumerate once - while `0o4147`/`0o4120 (WAITFORMAPBUF)` spin. `mar`
stays inside the 64K MDS region (`0x10000..0x1FFFF`), so this is the Alto
64K map setup, not the 16MW zeroing. So `NextMapEntry`'s end-of-VM test
`PD_ (BRHi_ T)-(VirtualBanks)` never fires - the bank counter never reaches
VirtualBanks. Config seen: `VirtualBanks=0x40`, `RealPages=0x0000`
(=64K pages/16MW, same as the Mesa boot), `faults=0`.

This is the same class as Initial's own earlier `NextMapEntry` bug ("saw
ALUF[4] as A+B+1 instead of subtract and overran enumeration"), now in the
loaded emulator's copy.

**FULLY LOCALIZED (2026-06-09, deep trace): two AEmu-InitMem map bugs.**
A windowed PC trace gated to the loaded world (`DORADO_NEXTMAP_TRACE`,
armed in bank 3, with `DORADO_STORAGE_MODULES=1`) shows the enumeration
**does progress** - the BR climbs bank-to-bank correctly across crossings
(`0x2FD50 -> 0x2FE50 -> 0x2FF50 -> 0x30050`). So it is NOT a VA-accumulate
bug. The two real bugs:
  1. **`IWriteMap` writes the map at VA = `BR + ITemp1`** (e.g.
     `0x2FE50 + 0x2E0 = 0x30130`), using the real-page number `ITemp1` as
     the address displacement. It should write the map entry for the BR's
     own VA (the enumerated VM page) with `ITemp1` as the *data* (real
     page). The map writes are FF-driven (IWRITEMAP1 `0o4113` FF=152,
     IWRITEMAP `0o4140` FF=177, ASEL=A<-RM/STK - not an ASEL ref), so our
     FF map-ref path is taking `Mar = A-bus = ITemp1` instead of Mar=0.
  2. **The per-module loop never advances.** Region-4 regs across the run:
     `ModMask` (RM[0x4B]) stays `0x8000` and `ITemp2` (RM[0x4E]) stays
     `0x0003` - neither updates. So `MapModule`/`FindModule` either are not
     reached or their `ITemp2_ (1S)-(PgsPerMod)` / `ModMask_ (ModMask)
     LSH 1` writes do not take, and the loop remaps "module 0" forever,
     never reaching `EndOfStorage`.
Both are in AEmu's `InitMem` map/config code (FF-driven map writes +
`MapModule`/`FindModule`), which our emulator has never validated because
Initial's own map setup uses different code (`WRITEMAP`/`WAITFORMAPBUF`,
confirmed working in the same run).

**RESOLVED 2026-06-10 - InitMem hang fixed; boot now reaches DiskBoot.**
The root cause of bug #2 was an **inter-task pipe-slot collision**, not a
VA-carry or per-module-loop bug. Full trace chain:
  * InitMem's `NextMapEntry` does `DummyRef_ T` then reads the resulting VA
    back via `ITemp17_ VALo; T_ VAHi` (Pipe1/Pipe0 of `pipe[ProcSRN]`), and
    rebuilds BR from it (`BrLo_ ITemp17; BrHi_ T`). Each pass advances BR by
    one page; the enumeration ends when `VAHi==VirtualBanks(0x40)`.
  * A ring-buffer trace caught the failure: between the DummyRef (0o3257,
    `va=0x30850`, bank 3) and the `T_ VAHi` read (0o3261), `last_ref_va`
    changed to `0x10150` with **no intervening task-0 instruction** - i.e. an
    I/O task preempted, did a reference, and that reference landed in
    `pipe[ProcSRN]`, clobbering the emulator's DummyRef VA. The readback then
    got `VAHi=1` instead of 3, so BR dropped from bank 3 to bank 1 and the
    bank counter cycled 1->2->3->1 forever, never reaching 0x40.
  * Root cause in `memory.c`: SRN selection routed only `IOFetch`/`IOStore`
    to ASRN; any **other** reference kind (Fetch/Store) from a non-emulator
    task used `ProcSRN`. Per HM p.51-52 the emulator (task 0) and fault task
    (task 15) use ProcSRN; **all I/O tasks (1..14) use ASRN**. Fix: select
    ASRN whenever `task != 0 && task != 017` (or the kind is IOFetch/IOStore
    or a prefetch miss). This keeps I/O-task references out of the emulator's
    pipe entry.
  * Result: VAHi now climbs monotonically to 0x40 (enumeration completes at
    exactly 16384 entries x3 passes); the AEmu world reaches `StartEmulator`,
    `ResumeEmulator`, and `DiskBoot`/`ABoot`, with all 16 tasks scheduling.
    Full suite stays green. The bug #1 map-write `Mar=0` fix (below) is also
    kept - it is independently correct.

**UPDATE 2026-06-10 (later): the RTC issue was an INCOMPLETE-WORLD
artifact; complete world boots much further.** The earlier RTC/junk-task
stall was caused by booting an **incomplete** world: `/tmp/AEmu.eb` was
`mb2eb` of `AEmu.mb!2` alone (2148 IM words = just the Alto-emulator layer)
loaded over Initial's base, so the junk/IFU/trap code was Initial's,
mismatched - producing a spurious tasking-off spin (0o7741-0o7747) and a
junk task that never re-ran. A complete world needs all five layers
(Initial+kernel+memMisc+IfuComplex+AEmu = ~14k IM words, like
AltoMesaDorado.eb).

Two concrete results:
  1. **Complete world via the bypass `probe_aemu`** (loads all 5 layers,
     enters at STARTEMULATOR), run with `DORADO_AEMU_CYCLES=25000000`: the
     **RTC advances (VM430=4)**, ABoot's 100 ms wait passes, and the boot
     becomes **fully multitasking** - disk task (`SEEKWAIT`/`ABADSECTOR`/
     `SENDDRIVETAG`, disk outs=2450/ins=740), display task, ethernet-input
     task (`EIRETN`) - reaching `NOTEMUFAULT` (0o4167) after disk activity.
     The disk read hits `ABADSECTOR` because the disk data path is
     incomplete (expected; we boot over Ethernet). So the SRN fix really did
     unblock the emulator: the complete world runs through ABoot.
  2. **New tool `mb2eb -l`** merges several `.mb` layers into one complete
     `.eb` (base via `dorado_microcode_load`, rest via
     `dorado_microcode_layer_load` - same as probe_aemu). Built
     `/tmp/AEmuFull.eb` (IM=3968 IFUM=256 RM=123, 35 KB) from
     Initial+kernel+memMisc+IfuComplex+AEmu.
     Command: `mb2eb -l out.eb 01076 Initial.mb kernel.mb memMisc.mb
     IfuComplex.mb AEmu.mb!2`.

**UPDATE 2026-06-10c (CORRECTION - the carry hypothesis below is RETRACTED).**
A direct hypothesis test disproved the carry story in UPDATE-b: forcing
`alu_carry=1` at `GetEmulatorMapParams`' 2nd entry (0o3240) did NOT change its
branch - call#2 still went to 0o4004 (a temporary `DORADO_FORCE_GEMP_CARRY`
diagnostic printed `FORCE carry=1 ... n=2` then `after-0o3240 next pc=0o4004`).
So 0o3240 is NOT a `Carry'`-conditional branch; the "fast" JCN there resolves
to the correct caller-continuation for each call (call#1 -> 0o3256 = MapInitLoop
continuation; call#2 -> 0o4004 = EndOfStorage continuation). The carry/zero
difference at entry was real but NOT what drives the divergence.

What remains VERIFIED:
  - `AEmu.mb!2` is the complete Alto-emulator boot vehicle (see below).
  - The boot derails before ABoot because every emulator BR is bank 0xD24
    (SetupBRs @0o1176 reads `EmuBRHiReg`=0xD24 and BrHi's it into all BRs).
  - `EmuBRHiReg`=0xD24 == Initial's STALE RM value, and the `.eb` loads no RM.
  - EndOfStorage's `EmuBRHiReg_ T` (InitMem.mc line 78) is supposed to
    overwrite it. The 2nd-call trace showed T=0x8000 at the EndOfStorage
    continuation (0o4004=line75 `ITemp0_T`), then `T_ T AND 7777C` -> T=0,
    which would set `EmuBRHiReg`=0 (correct!). Yet SetupBRs later reads 0xD24.
  - LEADING (unconfirmed) hypothesis now: the `EmuBRHiReg_ T` write at line 78
    is NOT actually reached on the boot path (the EndOfStorage continuation
    stalls/branches away - the 2nd-call trace ended spinning at 0o4104), so
    `EmuBRHiReg` keeps Initial's stale 0xD24. Next step: trace whether the
    instruction that writes `EmuBRHiReg`'s RM slot executes post-LoadRam, and
    if not, why the EndOfStorage path diverges before line 78.

The carry-specific text below is left for history but is superseded by this
correction.

**UPDATE 2026-06-10b (AEmu.mb!2 is the boot vehicle; MDS=0xD24 traced to a
carry-flag bug in InitMap). [SUPERSEDED by 2026-06-10c - carry part WRONG.]**

Switched the boot vehicle from `AltoMesaDorado.eb` (Mesa world, DMesaDefs
layout, no symbols) to **`AEmu.mb!2`** - which `mbdis` shows is the COMPLETE,
self-contained Alto-emulator world WITH symbols and the ADefs layout: it
defines `GetEmulatorMapParams` (@0o3240), `ABoot` (@0o724), `EBoot` (@0o2006),
`DiskBoot` (@0o2005), `InitMap`, `SetupBRs`, the junk/display/disk tasks, etc.
Build the boot `.eb` with `mb2eb AEmu.mb!2 /tmp/aemu_only.eb 01076`; serve via
`DORADO_ETH_BOOT_110=/tmp/aemu_only.eb` with `DORADO_ALTO_BOOT_ETHERNET=1
DORADO_ONLY_FULLBOOT=1 DORADO_NO_DISK=1 DORADO_STORAGE_MODULES=1`.

The boot now runs InitMap -> StartEmulator -> SetupBRs -> InitTasks, but
derails BEFORE ABoot (task 0 spins at the 0o4000 trap; every ref shows the
wild `mar=0xD24FE1F`). Full root-cause chain (all instruction-verified with
temporary traces; the AEmu.mb!2 symbols made this tractable):

1. The derail is because **all emulator base registers get bank 0xD24**.
   `SetupBRs`' DoBRs loop (@0o1176) does `BrHi_ EmuBRHiReg` into every BR,
   and `EmuBRHiReg` = 0xD24.
2. `EmuBRHiReg` is set ONLY by InitMem.mc's `EndOfStorage`:
   `Call[GetEmulatorMapParams]; T_ T AND 7777C; EmuBRHiReg_ T`. The `.eb`
   loads NO RM (RM=0), so if this didn't run, EmuBRHiReg would keep Initial's
   stale RM - but it DOES run.
3. `GetEmulatorMapParams` is called twice: call#1 (MapInitLoop) returns
   **T=0** (correct - InitMem.mc says it "should be 0 in all emulators except
   Lisp"); call#2 (EndOfStorage) returns **T=0xD24** (wrong).
4. The divergence is at `GetEmulatorMapParams`' first instruction (@0o3240):
   it does a LOGICAL op and a `Carry'`-conditioned "fast" branch. Both calls
   reach it with identical RBase (0o4) and identical A (0x0004) - so the
   branch is decided purely by the CARRIED carry flag. Call#1 enters with
   **carry=1** -> 0o3256 (returns 0); call#2 enters with **carry=0** -> 0o4004
   (returns 0xD24).
5. The carry entering call#2 is WRONG. A predecessor trace of the final
   map-loop iteration shows carry transitioning **1 -> 0 across NextMapEntry
   (0o3263 -> 0o3271)** and staying 0 through FindModule/EndOfStorage. On real
   hardware EndOfStorage must leave carry=1 (so GetEmulatorMapParams returns 0
   like call#1). The flip point is `NextMapEntry`'s 0o3263 (ALUF=4 ->
   ALUFM[4]=0o111000, an ARITHMETIC op, ASEL=A<-T): in the final map-loop
   iteration its carry-out is 0, but EndOfStorage needs carry=1.
   (`GetEmulatorMapParams`@0o3240 itself is ALUFM[0]=0o12400 = op `B`, a
   LOGICAL op that preserves caller carry and tests it - so the bug is the
   carry NextMapEntry leaves, not 0o3240.) Likely an ALUFM[4] carry-out
   computation error in cpu.c `alu_op`, or interaction with the long-known
   stubbed `Carry20`/`XorSavedCarry` (cpu.c ~779) - HM Table 9.

**Next step (focused):** disassemble `NextMapEntry`'s 0o3263-area ALU ops,
determine the correct `Carry'` behavior per HM Table 9, and fix the carry
update in cpu.c `alu_op`/the FF carry functions so EndOfStorage leaves
carry=1. That makes `GetEmulatorMapParams` call#2 return 0, `EmuBRHiReg`=0,
the BRs map to bank 0, and the boot should reach ABoot (then the BS-down
keyboard seed -> EBoot -> Mayday -> EFTP -> NetExec). The earlier
`DORADO_FORCE_MDS_BANK=1` diagnostic already confirms that with a sane MDS the
keyboard reads BS-down correctly.

**UPDATE 2026-06-10 (MDS corruption is a deep root; RTC works in isolation).**
Two findings, building on the DWT fix:
  1. **The multi-scenario probe was corrupting shared state.** Running the
     AltoMesaDorado boot in isolation (`DORADO_ONLY_FULLBOOT=1`, skips the
     earlier probe_* scenarios) makes the junk task run ~19295x and the boot
     reach `ACMMDEND` (an Alto disk-command handler) - PAST ABoot's RTC wait.
     So the RTC DDA advances and ABoot completes; the persistent RTC=0 was an
     artifact of the other probe scenarios (shared display/io/ethernet
     singletons + static counters), not an emulator bug.
  2. **MDS (BR[036]) gets corrupted to 0xD240000.** `GetEmulatorMapParams`
     (emulator-specific, only *referenced* in InitMem.mc - source NOT in our
     local trees) returns a wild MDS bank `0xD24`; InitMap stores it in
     `EmuBRHiReg` (RM[0x18]); `SetupBRs` (AEm0.mc DoBRs loop) then builds
     EVERY emulator BR (0o34-0o37 etc.) with that bad high half. So all
     MDS-relative refs - the keyboard (VM 177034), the RTC (VM 430), the
     display list (VM 420) - land at wild/unmapped addresses (the
     recurring `mar=D24FE1F`), the keyboard seed is lost, and the emulator
     eventually crashes and the BaseBoard restarts it into Initial's
     `DiskHardMicrocodeBoot`. The bypass (`probe_aemu`) only ever worked
     because it *plants* `EmuBRHiReg=1`.

**Diagnostic added (`DORADO_FORCE_MDS_BANK=n`):** forces `EmuBRHiReg`=n
during boot so SetupBRs builds correct BRs. With it, `BR36(MDS)=0x10000`
and the keyboard reads `177034=0xFFFE` (BS-down seed lands correctly) -
confirming the MDS-corruption diagnosis. But the boot STILL restarts
(DiskHardMicrocodeBoot), so MDS corruption is one root, not the only one -
there is at least one more crash after MDS is corrected.

**Next:** (a) obtain `GetEmulatorMapParams`'s source (it's the Alto-emulator
module that should return the MDS bank - 0 per InitMem.mc comment, or the
planted 1) to see why it yields 0xD24, OR rebuild the world via MicroD; (b)
find the post-MDS crash. The SRN + DWT fixes and the ONLY_FULLBOOT /
FORCE_MDS_BANK harness/diagnostic keep the suite green.

**UPDATE 2026-06-10 (RTC DDA confirmed WORKING; blocker is junk-task
starvation, not the DDA).** Disassembled the AltoMesaDorado junk task by
tracing it with field decodes (no symbols needed): it runs the canonical
`Junk.mc` loop (`JunkTaskLoop`/`JunkTaskCont`) at 0o3751-0o3776, with the
correct constants (`RTCDeltaLo=0xC28F=0o141217`, `RTCDeltaHi=0x35=0o65`).
Found the Events region at RM[0xB0] (RTClock), 0xB1 (RTCDeltaLo), 0xB2
(RTC430), 0xB3 (RTCFrac) - NOT 0xD0 (the AEmu/ADefs guess); AltoMesaDorado
uses a different RM layout. **RTClock accumulates correctly**: traced
0x0035, 0x006A, 0x009F, ... 0x2968 (+0x35/iter), `junkEn=1`. So the DDA is
fine; it just needs ~1260 iterations to overflow 16 bits and carry into
VM 430 (RTCCarry).

The blocker: the junk task runs only ~200-400 iterations then stops, before
RTClock overflows, so VM 430 never increments and ABoot's wait never
completes. It is NOT disabled (junkEn stays 1) - it is starved (a higher-
priority task becomes continuously ready). But pinning the exact starver is
hampered by the **multi-scenario probe**: the test driver runs several boot
scenarios (bootstrap_pure / bootstrap / aemu / initial / main) whose
`cpu->cycles` reset and whose stderr interleaves, so cycle-gated traces are
unreliable. Reliable RTC debugging needs a dedicated single-scenario harness
for just the AltoMesaDorado ether boot.

Separately found a real (latent) bug: **XorSavedCarry / XorCarry / Carry20
(FF FA=0 FB=2 FC=4/5/6) are stubbed no-ops** in cpu.c (~line 779). The junk
DDA and multi-precision routines (UpdateCounters etc.) use XorSavedCarry for
carry propagation; it is not the RTC blocker (RTClock accumulates without it)
but should be implemented for arithmetic correctness.

**Next:** (a) add a dedicated single-scenario AltoMesaDorado-boot harness so
RTC traces are clean, then find/fix the junk-task starver; (b) implement
XorSavedCarry/XorCarry/Carry20. The DWT fix below is the concrete win this
round; SRN + DWT fixes keep the suite green.

**UPDATE 2026-06-10 (DWT display-spin FIXED; RTC now isolated to the
AltoMesaDorado junk task).** Real fix landed (commit "Display: only request
DWT refill while the FIFO is actively draining"): the Display Word Task was
woken every cycle while CurrentWCB/raster-WT was set and the FIFO had room,
so at boot (empty FIFO, no display list) it spun forever and starved the junk
task. Now the refill wakeup is gated on `display_fifo_used()>0` (data
actually flowing); the DWT is still started for a real scan line by the
next->current edge. In the real `AltoMesaDorado.eb` cold boot the display
task now **idles instead of spinning** - measurable progress on the canonical
Alto-exec world. Full suite green (display test updated to the draining
contract).

After the DWT fix, the real `AltoMesaDorado.eb` boot still has RTC=0 because
its **junk task never accumulates RTClock / carries into VM 430** - but this
is now isolated and is the Mesa-vs-AEmu world difference: `AltoMesaDorado.eb`
is built with the PrincOps/Mesa register layout (DMesaDefs), so its Events
region (RTClock/RTCDeltaLo/RTC430/RTCFrac) and Junk-task code are at
different RM/IM addresses than the AEmu world (ADefs) whose symbols we have.
The Junk-task RTC logic itself is understood (chm/.../Junk.mc: DDA, AckJunkTW_
RTCDeltaLo with odd B[15]=enable, RTCCarry writes VM 430), and in the merged-
AEmu BYPASS it works (writes VM 430 at 0o1271, counting 1,2,3,4). Resolving
it in `AltoMesaDorado.eb` needs that world's symbol table / decompilation
(the .eb carries no symbols).

Confirmed the fork is fundamental:
  * `AltoMesaDorado.eb` (canonical, clean, no collisions): display now idles
    after the DWT fix; blocked only on the Mesa-layout junk-task RTC.
  * AEmu merges (`AEmu.mb!2` alone, `Initial+AEmu`, 5-layer `AEmuFull.eb`):
    the AEmu junk-task RTC works, but they wedge at the IFU-not-ready/
    fast-IO-SRN region (0o7747 / 0o6207) from layer collisions / resident-
    code mismatch. Not a clean world.
Long-term fix: obtain `AltoMesaDorado.eb`'s symbols (or rebuild the modern
complete Alto-emulator world via MicroD) to fix the junk-task RTC on the
canonical image. SRN fix + DWT fix are both real and keep the suite green.

**UPDATE 2026-06-10 (deep RTC dive - the two viable paths each have one
blocker; it is a FORK).** Exhaustively traced why the real-Initial
`AltoMesaDorado.eb` cold boot stalls at RTC=0. Findings (all experiments via
new gates `DORADO_NO_DISPLAY_WAKE`, `DORADO_NO_DISK`):
  * The RTC (VM 430) is written by the **junk task (task 2)**. In the
    WORKING bypass (`probe_aemu`, merged AEmu layers) the junk task writes
    VM 430 at PC **`0o1271`**, counting 1,2,3,4 (RTC ticks). In the
    real-Initial `AltoMesaDorado.eb` path the junk task runs DIFFERENT code
    (`0o3614`) and **never reaches `0o1271`** - nothing ever writes VM 430
    (confirmed: zero stores to offset 0o430). So ABoot's 100 ms RTC wait
    can never complete.
  * Suppressing the display wakeups (`DORADO_NO_DISPLAY_WAKE`) lets the junk
    task run far more (it was being starved by the DWT spin) but RTC is
    STILL 0. Forcing the junk timer permanently enabled is STILL 0. So the
    blocker is not (only) display starvation or the junk-timer-enable: the
    `AltoMesaDorado` junk task simply does not run the VM-430 RTC code.
  * Root: the bypass and the real-Initial path load **different microcode
    worlds** (merged AEmu layers vs the prebuilt Mesa-based
    `AltoMesaDorado.eb`); their junk tasks behave differently. The merged
    AEmu junk task maintains the Alto VM-430 RTC; `AltoMesaDorado`'s junk
    task (at `0o3614`) does not appear to (it is the PrincOps/Mesa junk
    task maintaining `RTClock` in RM, read via the RCLK opcode, not VM 430).

**This is a genuine fork:**
  * **Bypass (merged AEmu, planted clean state):** RTC ticks, full
    multitasking, reaches `NOTEMUFAULT`/disk activity - the FURTHEST any
    path gets - BUT cannot EBoot (faults at `0o2021`, lacking Initial's
    handoff: BRs, Ethernet command blocks, the EtherBoot bootloader copy).
  * **Real-Initial `AltoMesaDorado.eb`:** has the full Initial handoff (can
    EBoot), reaches ABoot/DiskBoot, BUT its junk task never ticks VM 430 so
    ABoot's RTC wait hangs.
  * **Real-Initial merged AEmu (`AEmuFull.eb`):** wrong composition
    (collisions) - derails. Abandoned.

**Open strategic question (may need Nick / deeper microcode work):** does
`AltoMesaDorado.eb` (= AltoD1MC.eb, the confirmed Alto-exec) actually
maintain the Alto VM-430 RTC under our boot, or does its ABoot use a
different clock? If it should tick VM 430, why does its junk task sit at
`0o3614` instead of the RTC code? Two candidate next directions:
  (a) Decompile/trace `AltoMesaDorado`'s junk task to find its VM-430 RTC
      code and why it is not reached (timer-wakeup routing / subtask).
  (b) Make the working bypass do EBoot by planting the Initial-handoff
      state it faults on (`0o2021` MemBase<-FF + BRs + Ether cmd blocks) -
      potentially the faster route to actually serving NetExec over EFTP.

**UPDATE 2026-06-10 (Nick reply - CORRECT IMAGE identified).** Nick (the
first-hand Interlisp-D Dorado user) confirmed by email: *"Lisp.run runs with
the AltoD1MC.eb microcode and loads the DoradoLispMC.EB from the current
partition itself."* So **`AltoD1MC.eb` is the Alto-exec microcode** - the
clean image we already validated (loads via real Initial with IFUM, no merge
collisions). Our earlier validation showed `altod1mc.eb` loads to the same IM
as `AltoMesaDorado.eb`, so **`AltoMesaDorado.eb` IS the Alto-exec world** (not
"just Mesa"). This RETIRES the whole `mb2eb -l` layer-merge approach (wrong
composition, collisions); the correct image is the prebuilt `AltoMesaDorado.eb`
/ `altod1mc.eb`.

Booting these via real Initial:
  * `altod1mc.eb` starts at `0o1070` (RestartEmulator = SOFT restart, ETemp4=-1,
    skips ABoot) - so it resumes a nonexistent program and spins at `StartIFU`
    (IFU never ready, nothing to fetch). Wrong start for a cold boot; expected.
  * `AltoMesaDorado.eb` starts at `0o1076` (InitMap = COLD boot) - this is the
    right one: it runs InitMap -> StartEmulator -> SetupBRs -> InitTasks ->
    ABoot, and reaches DiskBoot activity (`DODISKBLOCK`, `SECTORFOUND`). It is
    gated only on **RTC=0** (ABoot's 100 ms wait).

**The actual blocker (systemic): a high-priority I/O task spins and starves
the junk task, so RTC (VM 430) never ticks.** For `AltoMesaDorado.eb` cold
boot the junk timer is enabled at cyc=439, disabled at cyc=7M (pc=0o7623),
and NEVER re-enabled - because the junk task (task 2, low priority) can't run:
the **Display Word Task (DWT = task 0o13) spins at `0o6002`** (~1M hits),
starving it. Earlier the disk task did the same. The bypass (probe_aemu, clean
planted state - zeroed memory, 512-page map) does NOT have this; its I/O tasks
idle and the junk task ticks the RTC. So in the real-Initial post-boot state
the I/O tasks spin instead of idling. Likely cause: the I/O tasks read their
control structures from memory (display list VM 420, KBLK VM 521) that ABoot
has not yet zeroed (ABoot runs AFTER InitTasks wakes the I/O tasks), or our
display/disk wakeup model doesn't let the task Block/idle. **Next:** determine
why DWT does not Block at `0o6002` in the real-Initial path (display DWT
wakeup stuck on, vs garbage display list) - this is the gate to RTC ticking,
ABoot completing, and EBoot -> Mayday -> EFTP -> NetExec.

Nick also confirmed: (a) Al/Josh's Dolphin NetExec is probably the same binary
- worth trying as the served boot file; (b) a single `LISP.VIRTUALMEM` in the
Lisp.run partition suffices (multi-partition swap is optional, /X switch);
(c) decompiling `Lisp.run!6` + `Lisp.syms!4` could extract the disk/partition
layout - a future task.

**UPDATE 2026-06-10 (deepest, now partly SUPERSEDED by the Nick reply above -
the merge approach is abandoned): the I/O-task derail is a WRONG-LAYER-SET
microcode image, not missing hardware.** Traced the disk-task derail to its
root:
  * The disk task derails at **memMisc `0o6207`** (in the fast-I/O
    `APDecSRN`/`FIOSubTaskXit` region). `0o6207` is a Long Jump
    (`addr12 = (FF<<4)|(JCN&0xF) = (0o12<<4)|4 = 0o244`) to **`0o244`**,
    which is an **empty/unpopulated IM location** in our merged world -> it
    executes garbage and ends up wedged at the IFU not-ready trap `0o334`.
    No memory fault is involved (DORADO_FAULT_TRACE shows none on the I/O
    tasks); it is purely a jump to unloaded IM.
  * Root cause: **`mb2eb -l` merged the wrong set of layers.** I merged
    `Initial+kernel+memMisc+IfuComplex+AEmu` (what `probe_aemu` happens to
    load), but those collide heavily (e.g. Initial/kernel/memMisc each place
    a DIFFERENT instruction at `0o6207`; ~195 shared real addresses between
    Initial and memMisc alone) AND they omit layers the Alto-emulator world
    needs. The complete Alto-emulator world's real build (per
    `chm/dorado/expanded/bootEmu.dm!3_/bootemu.dls`) is
    `aemu + bitblt + atraps + eth + dsk + OISDDC + bootInitmem +
    bootstrap2` - so `0o244` lives in one of `eth`/`dsk`/`atraps`/`OISDDC`,
    which my merge never included. The merge "works" in the bypass only
    because the bypass's code paths avoid the corrupted/missing addresses;
    the real-Initial disk path hits `0o6207`.
  * **`bootemu.mb` is the clean complete build** (single MicroD pass, 2307
    instructions, NO collisions). Converting it (`mb2eb bootemu.mb`) and
    booting via real Initial gets further in places (reaches `SIOSTART`,
    `DISKBOOT`, `STARTIFU`) but it has **IFUM=0 in-image** (it rebuilds the
    IFU decode tables at runtime from memory tables) and is a 1979-vintage
    build; it currently spins elsewhere (task 6 at `NEWDCB`).

**Conclusion / next direction:** the standalone-Alto bring-up is gated on
having a *correct complete-world image*, not on a single missing-hardware
feature. The 5-layer `mb2eb -l` merge is the wrong composition and should be
abandoned for booting. Options: (a) use `bootemu.mb` and make the runtime
IFUM-table load work (understand how the Alto world builds IFUM at startup);
(b) obtain/build the modern complete Alto-emulator world from AemuSources via
MicroD with the correct DIB set (aemu+bitblt+atraps+eth+dsk+OISDDC+
bootInitmem+bootstrap2); (c) confirm whether `AltoMesaDorado.eb` (which loads
cleanly with IFUM) actually exercises ABoot/EBoot or is purely the PrincOps/
Mesa world. The SRN fix and everything below remain valid; this is purely
about which microcode image we hand to LoadRam.

---

**Earlier framing - real-Initial divergence is I/O tasks DERAILING
post-LoadRam, not the RTC.** Traced the AEMUNOTREADY spin in the
real-Initial complete-world run:
  * The dominant spinner is **the disk task (DSK = task 0o14)** stuck at
    `0o334` (AEmuNotReady, the IFU not-ready trap, which does `IFUJump[0]`
    and re-traps forever). Because DSK is high priority, it starves the
    junk task (so RTC stays 0) and the emulator.
  * DSK starts correctly (TPC=0o3001, runs real disk microcode at 0o3xxx)
    but then **derails out of disk code into the emulator's opcode/trap
    region**: a ring-buffer trace caught
    `...0o3601 0o3606 0o3620 ... 0o6225 0o6207 -> 0o244 -> 0o224 -> 0o100
    ... -> 0o334`. The disk source (`AltoDiabloDisk.mc`) contains NO
    `IFUJump`, so this is a wrong branch/Return into emulator code, after
    which it hits `DONTSKIP` (0o200) / opcode exits that `IFUJump` into the
    not-ready wedge. Derail point ~`0o6207`.
  * Disabling the disk (`DORADO_NO_DISK=1`, added to the probe) stops the
    disk wedge - but then **the display task (DHT = task 3) derails the
    same way**, spinning at `0o7606` (inside Initial's resident LoadRam
    page) and again starving everything (RTC still 0).
  * So it is **systemic**: in the real-Initial path the I/O tasks run their
    proper microcode briefly then derail to wrong addresses; the bypass
    (probe_aemu, same merged IM) does NOT - its disk/display tasks run
    normally and the RTC ticks. Same IM => the difference is runtime state
    the I/O tasks read (map/BRs/the control structures in memory that
    Initial's handoff leaves vs. the clean state probe_aemu plants) or a
    Link/Return corruption specific to the real path. **Next:** decode the
    `~0o6207` disk-derail branch (is it a Return on a bad Link, or a
    data-driven branch through a garbage KCB/display-list pointer?), and
    check whether the I/O tasks are waking and running before ABoot
    initializes their control blocks (display list at VM 420, KBLK at VM
    521). This - not the RTC - is the real-Initial blocker.

**Remaining divergence (earlier framing):** booting `/tmp/AEmuFull.eb`
via **real Initial** (`DORADO_ETH_BOOT_110=/tmp/AEmuFull.eb`) loads and
reaches StartEmulator -> ResumeEmulator -> SetupBRs/DoBRs -> InitTasks, but
ends at **AEMUNOTREADY** (the IFU not-ready trap, `IFUJump[0]`) with
**RTC=0** - i.e. the junk task does NOT tick the RTC on the real-Initial
path, unlike the bypass. So the same merged IM behaves differently by entry
path: the difference is the CPU/memory state Initial hands off vs the clean
state probe_aemu plants. Two things to chase next:
  (a) why the junk task ticks under probe_aemu but not under real Initial
      (one-shot `Wakeup[JNK]` delivery / `junk_tw_enabled` after AEmu's
      `IFUReset`; suspect the handoff state or an IFU-not-ready spin that
      runs with tasking effectively off);
  (b) AEMUNOTREADY itself - is the IFU spuriously stuck not-ready right
      after the handoff (CODE/RCODE base register or PCF setup)?
Once the real-Initial complete world ticks the RTC and runs opcodes, the
BS-down keyboard seed (`DORADO_ALTO_BOOT_ETHERNET=1`) should drive ABoot to
EBoot -> Mayday -> the byte-exact EFTP server (disk boot is expected to fail
on `ABADSECTOR`).

The SRN/pipe-slot fix and `mb2eb -l` are committed-quality and the full
suite stays green.

---

**Next blocker (new frontier, 2026-06-10): the Alto RTC (VM 430) never
advances.** AEmu now runs the emulator and stalls in `ABoot`/`DiskBoot`.
Root cause, pinned by the probe:
```
RTC: VM430 (MDS=0x10000) = 0x0000  ETemp0(target,RM[0x1A])=0x0003
     (ABoot exits when VM430 > target)
```
`ABoot` (AEm0.mc) reads VM 430 as the real-time clock and waits ~100 ms for
it to advance (`ETemp0_ (3S)+MD; ... Fetch_ T, Branch[.-1, ALU>=0]`); then
`DiskBoot`'s `KWait` likewise polls VM 430 for a ~1 s disk-status timeout
(which, on timeout, falls through to `EBoot` -> Mayday/EFTP). VM 430 is
maintained by the **Junk task (JNK)** via a DDA (region 13 `Events`:
`RTClock`/`RTCDeltaLo`/`RTC430`/`RTCFrac`; copies `RTC430` -> VM 430). In our
run the junk task (`JUNKTASKSTART/LOOP/CONT`) ran **exactly once** then never
again, so VM 430 is stuck at 0 and ABoot spins forever (final PC 0o7742;
`last_tx_pup=0o264`, `eftp_requests=0` - no Mayday issued yet).

Deeper trace (DORADO_JUNK_TRACE) localizes it further. Our model already has
a junk-timer-wakeup (`junk_timer_tick`, ~32 us, gated by `junk_tw_enabled`,
enabled via `AckJunkTW<-B` B[15]=1, disabled by `IFUReset` which loads
IFUTest=1 per HM Table 20). During the **Mesa** phase the junk task (task 2,
`JUNKTASKCONT` 0o3753) runs every ~1000 cycles and Acks with B[15]=1 -
correct. During the **AEmu** phase the junk timer is `enabled=0` (countdown
frozen) and the junk task never runs again to re-enable it, so VM 430 stays
0. The chain: AEmu `ResumeEmulator` does `IFUReset`, which both disables the
junk timer AND clears the pending junk wakeup bit; `InitTasks` then issues a
one-shot `Wakeup[JNK]`, which should make the junk task run once and re-enable
periodic ticks - but in our run that re-enable never happens.

So the remaining bug is a **wakeup-delivery/ordering issue around the AEmu
boot's one-shot `Wakeup[JNK]`** (note: the probe runs multiple phases and
`cpu->cycles` resets between them, which complicates tracing). Next steps:
(a) trace whether `InitTasks`' `Wakeup[JNK]` bit survives to a task switch and
the junk task actually runs once post-LoadRam; (b) confirm `IFUReset`'s
clearing of the junk wakeup bit matches HM (it may clear the *current* tick
but must not swallow a subsequently-issued explicit `Wakeup[JNK]`); (c) the
exact JNK wakeup/loop body is in the Junk/Events display-group source, NOT in
the `AemuSources.dm!82_` member we have (only ADefs/AEm0/AltoDiabloDisk/
EMemDefs/InitMem/RegisterDefs/S-Group/Start) - fetch it from CHM if needed.
Once the RTC advances, ABoot reads the keyboard and DiskBoot times out into
EBoot -> Mayday -> the (already byte-exact) EFTP server.

**UPDATE 2026-06-09 - bug #1 FIXED, bug #2 re-diagnosed.**
  * **Bug #1 (map-write VA) is fixed.** `cpu.c` now forces `mar = 0` for
    `DM_REF_MAP` refs (`Map_ 0S` addresses the BR's own VA, not A-bus =
    `ITemp1`). This is semantically correct per `InitMem.mc` (`Map_ 0S,
    MapBuf_ ITemp1`) and the full suite stays green - Initial's own map
    setup uses `DM_REF_STORE`, so it is unaffected. The AEmu hot-PC count
    at `0o4147` dropped from ~14M to ~7.7M, confirming the write path
    changed.
  * **Bug #2 is NOT the per-module loop - it is DummyRef/Pipe VA
    accumulation, and the map-write fix shifted its symptom.** A decisive
    trace at `NextMap1`'s subtract (`0o3263 PD_ (BRHi_ T)-(VirtualBanks)`,
    ALUF=04, A=T=VAHi, B=VirtualBanks=0x40) and the following
    `Return[ALU=0]` (`0o3271`) shows:
      - the subtract is **correct**: `A(VAHi)=0x0000, B=0x0040 ->
        alu_zero=0, alu_lt0=1`, so `Return[ALU=0]` correctly falls
        through (it is NOT wrongly taken). The Return / ITemp2++ /
        `FindModule` machinery is fine.
      - **`VAHi (T) = 0` on every iteration.** Before the map-write fix
        `VAHi` climbed 0..3; after it, `VAHi` is stuck at 0 (bank 0). The
        DummyRef VA's offset wraps at `0x1000` instead of carrying into
        the bank field at `0x10000`, so the enumeration never leaves bank
        0 and `VAHi` never approaches `VirtualBanks(0x40)`. `ModMask`/
        `ITemp2` looking "stuck" was a downstream consequence, not the
        cause.
    So the real remaining bug is the **DummyRef -> Pipe0/Pipe1 ->
    ITemp17/T -> BRHi/BRLo round-trip**: the VA offset is not carrying
    across the 16-bit boundary into the bank field. Likely a Pipe-slot
    (ProcSRN) collision between the DummyRef's pushed VA and `IWriteMap`'s
    map-ref push (now VA=BR after the fix), so `T_ VAHi`/`ITemp17_ VALo`
    read the wrong slot. **Next (fresh eyes):** per-cycle trace of one
    full `NextMapEntry` across a bank boundary - DummyRef VA -> which Pipe
    slot it lands in -> what `Pipe0`/`Pipe1` return at `0o3257`/`0o3261` ->
    BRHi/BRLo - against `InitMem.mc` and HM section 5.8 (Pipe). The
    map-write fix is correct and should be kept regardless.

**Earlier (partly superseded) note - "the spin is `Map1to1Loop`":** A register-window trace (region 4: VirtualBanks=0x40,
RealPages=0x4000, PgsPerMod=0x4000 - all correct) shows the loop counter
`ITemp1` climbing monotonically to 0xFFFF while the **enumeration bank
`VAHi` only hovers 0..3 and resets** - i.e. `Map1to1Loop`'s `NextMapEntry`
VA does not climb to `VirtualBanks(0x40)`; it wraps around ~256K (bank ~4).
`ModMask=0x8000` constant is a red herring (that loop does not touch it),
and `FindModule`'s `LSH 1` (FF=274 = FA=2 FB=7 FC=4) maps correctly to our
`Pd <- ALU lsh 1`. The map is 16K entries (wraps at 4MW, bank 0x40), so
not the map size either. The real bug is in the **DummyRef-based VA
accumulation**: it climbs correctly through a bank (PIPE_TRACE showed VaLo
0x7D00->0x8000->... in bank 0) but the BR round-trip loses bits near a
boundary so the bank stops advancing toward 0x40 - and the wrap point is
config-dependent (offset 0xF50->0x150 in the 4-module run vs bank-3/4 in
the 1-module run), which points at a register clobber or a mis-evaluated
condition in `NextMapEntry`/`Map1to1Loop`, not a fixed-bit mask. This needs
a careful per-cycle trace of one full `NextMapEntry` (DummyRef VA -> Pipe0/
Pipe1 -> ITemp17/T -> BRHi/BRLo) across a bank boundary, with fresh eyes.

Superseded earlier note (the FindModule theory):
**AEmu InitMem `FindModule` never reaches `EndOfStorage`.** The map enumeration is NOT a VA/VALo bug (a PIPE_TRACE
showed `VaLo` climbing past 0x1000 correctly: `0x7D00,0x8000,0x8100...`),
and it is NOT 16MW slowness (forcing one 4MW module via the new
`DORADO_STORAGE_MODULES=1` gives `RealPages=0x4000`, `VirtualBanks=0x40`,
`PgsPerMod=16384` - all correct - and it still loops). The real
non-termination is in AEmu's `InitMem.GetMemConfig`/`FindModule` module
loop (`InitMem.mc`), which our emulator has **never validated** because
Initial used different map code (`PRESETMAP`/`WRITEMAP`). `FindModule` does
`ModMask_ (ModMask) LSH 1, Branch[MapModule, R<0]`; `EndOfStorage` only
fires when `PgsPerMod==0` (never). With our `ModMask` (built by
`GetMemConfig` from the config word via `NOT(Config')`, `LSH 8`,
`AND 0170000`) plus our `LSH 1`/`R<0` handling, `FindModule` keeps finding
modules "present" and maps pages indefinitely (`NEXTMAP1` ran ~1.4M times
vs the 16384 expected for one module). **Next:** trace `ModMask` across
`FindModule` - confirm `(ModMask) LSH 1` actually shifts it toward 0 and
that `R<0` after the shift detects exactly the present modules, and align
the config-word module-bit positions (`module_bits` in
`memory.c::dorado_memory_config_word` vs what `GetMemConfig` expects after
`LSH 8 & 0170000`) so `EndOfStorage` is reached. This is config/shift-
semantics work to do with fresh eyes against the HM config-word format.

**Earlier per-cycle finding (superseded by the above):**
`NextMapEntry` walks VM by doing
`DummyRef_ T(=0o400)` ("let the memory system add a page"), reading the
new VA back via `VAHi`(Pipe0)/`VALo`(Pipe1), then `BRHi_ VAHi; BRLo_ VALo`
so the BR carries to the next page; it ends when `VAHi == VirtualBanks
(0x40)`. Per-cycle traces (env `DORADO_NEXTMAP_TRACE`, `DORADO_DUMMYREF_TRACE`
added to the probe/cpu) show the DummyRef VA climbs by `0x100`/page
correctly **within a 12-bit offset** - `...0x10E50, 0x10F50` - then the BR
**resets to `0x10150`** instead of carrying to `0x11050`. So the VA offset
wraps at `0x1000` (16 pages) instead of carrying into the bank at
`0x10000`; VAHi therefore never climbs to `0x40` and the loop never ends.
The pipe push/read round-trips the full 28-bit VA and `psrn==proc_srn`, so
the corruption is in the **BR offset between `NextMapEntry` and the next
DummyRef** - i.e. during the `IWriteMap` (RMap_/Map<-) map read/write the
loop does for each page, the enumeration BR's offset bits 9..11 get lost.
Next: check whether our `RMap_`/`Map<-`/map-ref path disturbs the current
MemBase BR (it must not), or whether `VALo`/`ITemp17`/`BRLo_` loses the
high offset bits, so the VA carries `0x0FF00 -> 0x10000` into VAHi.

Earlier (now-disproven) theory follows for the record:
Most likely the **memory-pipeline/Md timing** (the B1/C1 Hold/Pipe gap):
our model delivers a single global `Md` immediately, so when tasking
interleaves fetches around the `Fetch_ T (0o755) ... read Md (0o736)`
pair, the compare can read the wrong `Md`. The next step is a per-cycle
trace of the loop (`T`, `Md`, `ETemp0` target, ALU result, branch taken,
and whether a task switch lands between `0o755` and `0o736`) to confirm
whether it is an Md-clobber, a moving target, or an ALU/condition decode
issue. If it is the pipeline, implementing per-reference/per-task `Md`
delivery (Hold/Pipe semantics) is the correct fix - and it is the same
gap that makes AEmu's main dispatch loop unable to run, so it is the
highest-leverage emulator feature to build next.

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
