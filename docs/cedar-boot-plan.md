# Plan: bring up Cedar (or a Mesa/Pilot world) on the emulator

## UPDATE (2026-06-13c): the chain works through the load

The NetExec -> CedarNetExec chain this plan describes is now FUNCTIONAL
through the load. NetExec's full Pup stack works (it learns its net via
gateway-info 200B/201B, sets the real clock via 206B/207B, and discovers
the boot directory via 257B/260B -- all unblocked by fixing a dropped-
reply bug: socket replies were short the trailing hardware-CRC word the
AEmu receive microcode subtracts). With `--boot-dir` registering
`CedarNetExec.boot`, NetExec lists it under `?`, and selecting it
Mayday-requests its boot-file number; the server serves it and the
EtherBoot loader pulls it in. The OPEN frontier is now one level deeper:
`CedarNetExec.boot` itself transfers in full but stalls in early startup
(no display, no network I/O, PC spinning in page 0/1). See
`docs/CONTINUE-HERE.md` (top) for the full state. The original plan and its
correction follow.

Status: PLAN (2026-06-13). Written after the Alto/NetExec Ethernet boot
landed end to end (NetExec boots and is interactive) and the page-zero /
RBase-timing keystone bug was fixed. The next frontier is a Mesa-level
world: **Cedar 6.1**, via the same Ethernet boot chain one VM level up.

## CORRECTION (2026-06-13): how Cedar actually boots over Ethernet

The Dorado Booting memo (`chm/doradosource/DoradoBooting.tioga!2.txt`,
section 1.3) is explicit: **Cedar microcode boots its software from the
DISK** (germ -> physical volume boot file). **"Ethernet booting of
software is possible only with Alto emulator-based microprograms, NOT with
Cedar. To boot Cedar software from the Ethernet, you must first boot the
NetExec using Alto/Mesa, then use the NetExec to call the CedarNetExec,
then use the CedarNetExec to call the desired program."**

So loading `CedarDorado.eb` directly (Phase 0 below) is the DISK path --
the Cedar microcode starts and waits for a disk germ that we do not have,
which is exactly why it loops forever in InitMem. That is expected, not a
bug, and NOT the route to take over Ethernet.

The real Ethernet route chains through the NetExec we already boot:

```
NetExec (Alto/Mesa microcode + NETEXEC.BOOT -- WORKS, interactive)
  --> CedarNetExec   (NetExec boots CedarNetExec.boot over EFTP)
  --> the Cedar program / world
```

This stays on the **Alto/Mesa microcode** and reuses the exact EFTP
software-boot path that already serves `NETEXEC.BOOT`; we just hand it the
next boot file. No direct Cedar-microcode load, no disk.

## Strategy in one sentence (revised)

From the working Alto/Mesa NetExec, chain-boot `CedarNetExec.boot` (and
then the Cedar world) over the same EFTP path that already serves
`NETEXEC.BOOT` -- staying on the Alto/Mesa microcode per the booting memo,
rather than loading the Cedar microcode (which is the disk-only path).

## (superseded) earlier strategy

Reuse the *entire* working Alto-boot transport (Initial microcode boot +
the in-process EFTP/Mayday boot server) and swap the two payloads for
their Cedar equivalents; the only genuinely new engineering is making the
**Mesa virtual machine microcode run**. -- This is the DISK path and is
superseded by the correction above for the Ethernet route. (Direct Cedar
microcode + Mesa VM bring-up is still the path if/when we pursue a built
disk volume.)

## The boot chain — what changes, what is reused

```
WORKING (Alto):
  Boot0 -> Bootstrap -> Initial -> EtherMicrocodeBoot
        -> [serve] AltoMesaDorado.eb      (Nova/Alto-emulator microcode)
        -> ABoot/EBoot -> EFTP/Mayday -> [serve] NETEXEC.BOOT
        -> NetExec runs, interactive

TARGET (Cedar):
  Boot0 -> Bootstrap -> Initial -> EtherMicrocodeBoot
        -> [serve] CedarDorado.eb         (Mesa/Cedar microcode)   <-- swap
        -> germ (Dorado.germ) -> BootChannelEther -> EFTP/Mayday
        -> [serve] CedarNetExec.boot, then BasicCedarDorado.boot   <-- swap
        -> Cedar runs
```

Reused unchanged (already working, test-covered):
- Stage 1: Initial -> EtherMicrocodeBoot -> CheckChecksumAndLoad -> LoadRam.
  We already serve `AltoMesaDorado.eb` this way; serving `CedarDorado.eb!6`
  is the same code path with a different payload.
- Stage 2 transport: the germ's `BootChannelEther.mesa` uses the SAME Pup
  `bootFileSend` + EFTP clump protocol (`anyBootServer`, boot file number)
  that `src/ethernet.c` already implements for the Alto NetExec boot.
- The microengine, memory subsystem, IFU, 16-way tasking, the RBase-write
  fix, and the differential-harness-verified Nova opcode core.

New work (in rough risk order):
1. **The Mesa VM** (the bulk). Cedar runs the Mesa instruction set, a
   stack bytecode machine, dispatched by the IFU into `Cedar.mb`/
   `CedarDorado.eb` handlers -- a different microcode world than AEmu.
   It exercises the full virtual-memory machinery (Map faults, Pipe, BR,
   MDS) that the Alto emulator barely touched. Prior work
   (`docs/handoff.md`) got `Mesa.mb!3` LoadRam'd and reaching `SETDLP`
   (real 0o4654) before halting at `CPU_HALT_IFU_NOT_READY` -- but that
   predates this year's IFU/RBase/carry fixes, so it must be re-baselined.
2. **The germ** (`Dorado.germ!4`) -- Cedar's Mesa-level bootstrap loader.
   It runs as the first Mesa world, opens the Ethernet boot channel, and
   pulls the real boot file by boot file number.
3. **Boot-file plumbing** -- serve the Cedar boot files by BFN, and make
   the fake server's boot-decision/back-channel select the Cedar world.

## Local content we already have (no downloads needed)

| Piece | File |
|---|---|
| Cedar microcode (Mesa VM), ether-bootable | `chm/dorado/CedarDorado.eb!6`, `chm/microcode/BasicCedarDorado.pb!1` |
| Cedar microcode source | `chm/dorado/Cedar.mb!6`, `Mesa.mb!3` |
| Initial for Cedar ether boot | `chm/dorado/InitialEtherCedarDorado.eb!3` |
| The germ + its boot channels | `chm/cedar/germ/Dorado.germ!4`, `BootChannelEther.mesa!3.txt`, `BootChannelDisk.mesa!2.txt`, `MiniEthernet*.txt`, `Dorado.loadmap!1.txt` |
| Cedar boot files | `chm/cedar/cedar6.1/BasicCedarDorado.boot!22`, `chm/cedar/basiccedar/BasicCedarDorado.boot!14` |
| Mesa/Cedar NetExec (stepping stones) | `chm/bootfiles/CedarNetExec.boot!4`, `MesaNetExec.boot!1`, `AlphaMesaMesaNetExec.boot!1` |
| Disk installer (later, if we build a pack) | `chm/cedar/germ/OthelloDorado.boot!8` |
| Full Cedar world snapshot (alt path) | `chm/microcode/BasicCedarDorado.pb!1` (513 KB) |

## Phased plan

### Phase 0 -- RESULTS (2026-06-13, started)
Ran `CedarDorado.eb!6` through the existing Stage-1 path (same
`dorado --eb` mechanism as the Alto world). Findings:
- **Stage 1 works for Cedar**: the Cedar microcode is LoadRam'd exactly
  like the Alto world (`booted=1 at cyc 32M`, `config_word=0o214` =
  storage present, so NOT the NoStorage case).
- **Then it loops in the shared kernel InitMem/InitMap** (task-0 PCs
  `5400 5430 5434 5450 5454 5470 5474 7064 7066 7140 7160 7166 7167 7171
  7172 7175`, plus `7003/7012`), never reaching the Ethernet boot
  (`eftp_r=0 bol=0 DASTART=0`). Confirmed GENUINELY STUCK, not just slow:
  still in the same loop at 200 M cycles (vs the Alto world, which reaches
  NetExec by ~110 M).
- This is the SAME loop the standalone Alto boot sat in before the **Mesa
  boot-parameter STK seed** (`STK[1]=0110` boot-file number,
  `STK[2]=056623` BootParameterSeal, `STK[3]=0121045`) got it past -- but
  that seed is Alto-specific. So Cedar's loaded InitMap wants different
  boot parameters / a different boot-mode select than the Alto world.

UPDATE: this stall is EXPECTED, not a bug. Per the booting memo (see the
CORRECTION at the top), a started Cedar microcode boots its software from
DISK; with no disk germ it loops in InitMem forever. So this direct-Cedar-
microcode route is the disk path, not the Ethernet route. The Ethernet
route does NOT load `CedarDorado.eb` -- it chains NetExec -> CedarNetExec
on the Alto/Mesa microcode (next).

Revised next step (Ethernet route): from the working NetExec, boot
`CedarNetExec.boot!4` over the same EFTP path that serves `NETEXEC.BOOT`
(swap the `--eftp` file, or implement the NetExec "call CedarNetExec"
chain), and observe what CedarNetExec does. The boot-parameter seed
question is moot for this route -- we stay on the Alto/Mesa microcode.

### Phase 0 -- method (re-baseline the Mesa microcode)
Serve `CedarDorado.eb` through the existing Stage-1 path and run it on the
*current* (fixed) emulator. Goal: a fresh map of how far the Mesa VM gets
and where it stalls now, since the prior `SETDLP`/`IFU_NOT_READY` blocker
predates the RBase/IFU/carry fixes.
- Build/serve: `DORADO_ETH_BOOT_110=<CedarDorado.eb>` (mb2eb not needed --
  it is already an `.eb`), like the AEmu probe.
- Instrument: the existing PC-histogram / store-trace / IFU traces; add a
  Mesa-symbol map (`Cedar.mb!6` real addresses) the way `altodiff-dorado`
  resolves AEmu symbols.
Deliverable: "Cedar microcode reaches PC X, stalls on Y" -- the punch list
for Phase 1.

### Phase 1 -- Make the Mesa VM run (the bulk of the work)
Drive the Cedar microcode to its steady Mesa-bytecode dispatch loop and
fix the gaps it exercises that Alto did not:
- **Mesa IFU startup** (the `SETDLP` / IFU-pause/PCF path that previously
  halted) -- the Mesa instruction set uses the IFU differently than Nova.
- **Virtual memory**: Map faults, the fault task, Pipe read-back, BR/MDS
  for Mesa's larger address space and frame/MDS layout.
- **Mesa opcode handlers**: extend the differential harness
  (`altodiff-dorado`) to the Mesa instruction set -- the same tool that
  found the RBase bug. NOTE: salto is an *Alto* oracle and cannot validate
  Mesa; the oracle here is the **Mesa Processor Principles of Operation**
  (Princ-Ops) doc plus targeted microcode tracing. Where a clean oracle is
  impossible, fall back to the boot-progresses-further loop the handoff
  used (each fix moves the world forward).
This is the high-risk, multi-iteration phase. It is also where this
session's foundation pays off: the verified-correct microengine, the
RBase/carry fixes, and the harness tooling all transfer.

### Phase 2 -- Germ + CedarNetExec (the stepping stone)
Once the Mesa VM runs, the Cedar microcode boots the **germ**, which opens
`BootChannelEther` and EFTP-fetches a boot file. Serve **`CedarNetExec.boot`**
first -- the small Mesa/Cedar network executive, the exact analogue of the
Alto `NETEXEC.BOOT` we already boot. Reaching its prompt proves the whole
Mesa-level chain (VM + germ + boot transport) end to end, on a small world
before the big one.
- Reuse `src/ethernet.c`'s EFTP server; wire the Cedar boot file number.
- Calibrate the germ's `bootFileSend`/clump handshake vs the Alto EFTP
  (likely identical; verify the socket/type constants in
  `BootChannelEther.mesa`).

### Phase 3 -- Cedar 6.1
Serve **`BasicCedarDorado.boot!22`** (Cedar 6.1). The germ loads the Cedar
nucleus; Cedar starts, installs its display, and runs. Success = the Cedar
herald/display renders and (stretch) is interactive.

## Alternative / fallback paths
- **`.pb` direct load** (`BasicCedarDorado.pb!1`): a Pilot Backup is a full
  Cedar memory image. A loader that maps the `.pb` into VM and sets the
  Mesa machine state could bypass the germ/boot-file dance entirely. Faster
  IF the Mesa state layout (frame, MDS, GFT, the boot-time registers) is
  implemented; a good fallback if the germ boot proves fiddly. Needs the
  `.pb` format + Pilot/Mesa state docs.
- **Disk boot** (`OthelloDorado.boot` to build a pack, then disk germ):
  deprioritized -- blocked on the disk read/write data path and on having
  a pack to boot. Ethernet avoids both. Only pursue if a persistent
  installed volume becomes a goal.

## Why Cedar over Pilot, and Ethernet over disk
- **Cedar** (not bare Pilot): we have a complete, ether-bootable Cedar
  world for Dorado (`CedarDorado.eb` + `BasicCedarDorado.boot` 6.1) and the
  germ; bare Pilot-on-Dorado artifacts here are mostly D0/DLion, not Dorado.
- **Ethernet** (not disk): the disk path is blocked on content and the
  data-transfer path; the Ethernet boot chain already works for the Alto
  world and the germ speaks the same EFTP protocol -- maximal reuse,
  minimal new transport code.

## First concrete action
Phase 0: serve `CedarDorado.eb!6` through the existing Initial Ethernet
microcode-boot probe and capture where the Mesa VM stalls on the current
emulator. Everything else keys off that punch list.
