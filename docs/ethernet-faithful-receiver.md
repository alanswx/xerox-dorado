# Faithful Dorado Ethernet Receiver — pick-up notes

Status: **paused** (2026-06-19); **re-prioritized 2026-06-23** — now has a
concrete game-blocker behind it (Invaders), so the **transmitter** side of the
wire model (not just the receiver) is the lead.

> **2026-06-23 update — Invaders IS blocked by the ethernet tx timing.**
> `tracepcdiff` (after the AEmuReschedule tool fix) shows Invaders matches
> ContrAlto for 2091 opcodes, then diverges where the game polls **EPLOC
> (`0o600`)** for an Ethernet **OutDone** completion. ours' EOT task posts
> OutDone ~32us after the transmit; ContrAlto spins ~4.1ms (733 iterations).
> ours **completes transmits instantly** (`eth_tx_packet_done` is synchronous on
> TxEOP) with **no transmitter deferral while the receiver is busy** -- and at
> the transmit point a 9896-word receive is in progress, so a real controller
> would defer for milliseconds. Per-word wire time (~70us for 13 words) does NOT
> explain the 4.1ms gap; the missing piece is **tx-defer-while-receiving**. So
> the faithful wire model DOES unblock at least one game; the earlier "does not
> unblock the games" note (below) was based on MissileCommand alone. The risk
> remains real: the EFTP boot relies on instant tx-completion (loader alternates
> tx/rx), so any deferral must be gated hard on Galaxian + the boot.

The earlier framing (kept for history): we paused because fresh cross-validation
proved the no-render Alto games (MissileCommand etc.) are blocked by an
**emulator bug, not the ethernet** — see
`memory/mc-bug-is-emulator-not-ethernet.md` and
`docs/alto-game-compatibility.md`. This receiver is a real fidelity
improvement; per the 2026-06-23 update it now also has a confirmed game-blocker.

## Why build it

`dorado/src/ethernet.c` is an in-process Pup/EFTP **fake**: it pre-queues
reply packets as `rx_words[]` + `rx_attention[]` and hands them to the AEmu
input task. It has no real receiver FIFO, no end-of-packet (EOP) timing, no
CRC, and (until now) no status register. The Dorado has a complete,
schematic-specified Alto-compatible 3 Mb/s controller (David Boggs's
design). `docs/schematic-audit.md` gap **H1** lists exactly what's missing.

## The schematic (DoradoDocs/schematics/DskEth.pdf)

Ethernet controller is sheets 23–41 (pages ~23–41 of the PDF). Key sheets:

- **Sheet 23 (Ether00)** — block diagram. Receiver pipeline:
  Phase Decoder -> Receiver Control -> Receiver Data Paths (FIFO) ->
  Receiver Wakeups -> Dorado buses. Plus Next & IO Attention (Ether13),
  Clocks (Ether12), the transmitter mirror, Test Logic (Ether11).
- **Sheet 25 (Ether02, Receiver Control)** — receiver status state machine
  outputs: `RxState.0/1/2`, **`RxEOP`**, `RxCollision`, `RxSync'`,
  `RxIncTrans`, `RxCRCReset`, `RxCRCClk`, `RxData`, `RxSRCtrl.0/1`,
  `RxSRFull'`. Note on sheet: "slowest Dorado clock the receiver works at
  is 85 ns (T0 to T1)."
- **Sheet 26 (Ether03, Receiver Data Paths)** — the **16-deep Rx FIFO**
  (F145A chips j8–j12), shifting `RxSR.00-18` into `EthData.00-18`. Crucial:
  **`EthData.18` = "End of packet"** (EOP), latched from `RxEOP` through an
  MC231 flip-flop. The data words are `EthData.00-15`; `EthData.18` is the
  EOP/IOAtten bit the microcode branches on.

## The microcode contract (what the receiver must satisfy)

AEmu Alto-ethernet source:
`chm/doradosource/AEmuSources-cedar6.0.dm!1_/{EtherDefs,AltoEther,AltoEtherEmu}.mc`

- Device addresses: `EData` = TIOA 15, `EControl` = TIOA 16.
- `EControl` STATUS read: left byte = host address; right byte =
  `RxOn=200 TxOn=100 LoopBack=40 TxCollision=20 NoWakeups=10 TxDataLate=4
  SingleStep=2 TxFifoPE=1`.
- Receiver status bits in the end-of-packet status word (`EData`):
  `RxCollision=200 RxDataLate=40 RxCRCError=10 EInCmd=4 EOutCmd=2
  RxIncTrans=1`. `EISMask` keeps collision|datalate|crc|inctrans.
- Post codes (`EPLOC`/EPost): `InDone=377 OutDone=777 InBufOverflow=1377
  CountZero=2377 CmdAbort=2777`.
- EIT input main loop (`AltoEther.mc`):
  ```
  EIPtr_ (Store_ EIPtr)+1, DBuf_ T, Branch[EIEnd, IOAtten];  [A] store prev, branch on EOP
  T_ Input, Block, Branch[.-1, ALU#0];                       [B] read next word
  ...
  EIEnd:  EIPtr_ (1S)-(EIPtr);   * finalize count
          T_ Input;              * read the trailing status word
          T_ T AND (EISMask); T_ T XOR (InDone); Call[EPost];
  ```
  So `IOAtten` (= `EthData.18` EOP) fires on the **CRC word** (the word in
  T at branch time — the comment says "the word in T is the CRC"); the loop
  stores it, then `EIEnd` reads ONE more word = the status. Delivery order
  the receiver must produce:
  `[data words, EOP=0] [CRC word, EOP=1] [status word]`.

## Known gap vs the current fake (the actionable diff)

In `dorado/src/ethernet.c`, `append_eftp_packet()` and the other rx-append
paths emit: `... payload + pup-cksum(att=0) + dummy-CRC(att=0) +
status(att=1)`. i.e. the attention/EOP flag sits on the **status** word,
one slot LATE vs the schematic (EOP belongs on the **CRC** word). This was
a deliberate tuning (comment at ~line 516) to post InDone instead of
InBufOverflow, but it inflates the delivered input count by one word — the
kind of off-by-one a count-sensitive client would notice. A faithful build
moves EOP to the CRC word and makes the trailing word the real status.

`dorado/include/ethernet.h` already has the `EthC` control bits and
EOT=06/EIT=07 wakeup gating, which `docs/schematic-audit.md` says already
match the schematic — so the control/command side is fine; only the
receive (Rx) side needs the faithful rebuild.

## Done so far

- **`EControl` status register read** (ethernet.c `eth_read`, TIOA 16): now
  returns `host<<8 | RxOn | TxOn | NoWakeups` instead of host-only. Gate
  re-verified: Galaxian 121552, NetExec ~1475, MC unchanged (still the
  black box — confirming status alone is not the MC issue). This change is
  faithful and safe to keep.

## Remaining work (when resumed)

1. Move EOP/IOAtten onto the CRC word in all rx-append paths; make the
   trailing word the real receiver status (0 for a clean receive ->
   masks+XOR to InDone). Re-verify the FULL gate (Galaxian, NetExec, plus
   the boot path — this changes the count for every received packet, so
   regression risk is real; back out if it breaks boot, per the
   source-control philosophy).
2. (Optional, deeper fidelity) model the actual 16-deep Rx FIFO + CRC + the
   receiver state machine so packets are delivered word-at-a-time with
   correct EOP timing instead of pre-queued. The clean architecture: feed
   the receiver real packets (the BootServer/breath-of-life already
   generates the netboot protocol — see
   `AltoInfo/Contralto2-2.0-Beta/ContraltoLib/IO/BootServer.cs` for the
   protocol mirror) rather than pre-queued fake replies.

## Validation harness

- Headless gate: `build/dorado --eb worlds/aemu.eb --eftp <bootfile>
  --cycles 130000000 --out X.pgm`; count black px with `/tmp/pgmcount.py`.
  Gate values: Galaxian **121552**, NetExec band **~1474–1505**.
- Cross-oracle: `AltoInfo/contralto-headless` net-boots the same .boot
  (`DOTNET_ROLL_FORWARD=Major CA_NETBOOT=1 CA_BOOTSERVER=<bootfile>
  dotnet run -- dummy.dsk out.pgm <secs>`).
