# Faithful Dorado Ethernet Receiver — pick-up notes

> **2026-06-23 (S1/S2 start) — the AEmu EOT does NOT model transmit wire time;
> deferring tx-completion cannot reproduce ContrAlto's OutDone delay.** Built the
> S1 lockstep harness (`tools/nova-trace-diff/lockstep.sh`) and confirmed every
> simple game (Invaders, AstroRoids) diverges at the SAME point (depth 2091, the
> EPLOC/OutDone wait) — so the ethernet completion is the forced *first* cadence.
> Traced the EOT (Dorado task 6) microcode (`DORADO_EOT_PC_TRACE`):
> - OFF: EOT does SendEOP, spins a short poll loop (real 0o2566/0o2533) until the
>   (instant) completion, then runs EPOST and posts OutDone — but too early
>   (537 cyc), so ours exits the spin ahead of CA.
> - WIRE (hold tx_eop, complete after wire time): holding tx_eop **diverts the
>   EOT down the EXINIT/idle path** (real 0o1253) instead of the
>   SendEOP->poll->EPOST path; the EOT then does **EOStop (tx_on->0) before** the
>   deferred completion can re-wake it, so the EOT wakeup (`tx_on && !tx_eop`)
>   never fires again and OutDone is never posted (game hangs).
> So the AEmu EOT expects the Dorado transmit to complete **synchronously** (the
> Dorado ethernet is fast); it has no block-and-wait-for-TxGone that survives a
> multi-us deferral. Matching ContrAlto's ~13-iteration OutDone delay therefore
> requires changing the EOT's tx-completion **decision tree** (make it
> block/poll across the deferral before EOStop), not just delaying the C-side
> completion. That is microcode-level work — the hard cadence, and the forced
> first one. The DORADO_ETH_WIRE rx-pacing + fixed-tx model stays as gated
> scaffolding; the tx half does not help until the EOT path is addressed.

> **2026-06-23 (render-path investigation) — confirms: the render path is fine;
> broken games diverge in EXECUTION before they build a display list.**
> - The C rasterizer works: Galaxian (renders) has a valid DCB
>   (`nwrds=20 slc=195` -> 390 lines x 20 words, 121602 px). Invaders (blank,
>   163 px) has a degenerate list (`nwrds=0`, 6 DCBs all pointing at one bitmap)
>   -- it never set up a real display because it diverged early.
> - Correction to the OutDone-delay number: ContrAlto exits the EPLOC spin at
>   ~opcode **2130 (~13 iterations** after it starts at 2089), NOT 733/4.1ms --
>   that earlier figure counted a LATER frame's reuse of the same 723/724/725
>   loop. So CA's OutDone arrives ~13 spin iterations after the transmit.
> - With the wire model ON, ours now spins in **lockstep with ContrAlto through
>   all ~13 iterations** (match 2091->2130) -- the timing approach is right. But
>   then ours **drops the game's OutDone entirely** (it posts during boot, last
>   at cyc 55M; the ~76M game transmit gets none), so ours hangs in the spin.
>   Root: EOT/EIT task contention. OFF completes the tx synchronously inside the
>   EOT's own task invocation (posts OutDone, but too early); the wire model
>   defers completion, the EIT (receive) takes over in the gap, and when the
>   wire timer finally clears tx_eop the EOT never gets its "sent" wakeup to run
>   EPOST. So neither OFF (too early) nor ON (dropped) matches CA's "post at
>   ~2130", and BOTH leave Invaders blank -- consistent with the px probe.
> - Net: matching CA's OutDone needs modeling the AEmu EOT's Block/resume across
>   the delayed TxGone wakeup so EPOST survives concurrent EIT activity. That is
>   AEmu-microcode-level timing work -- and the px probe already showed it has no
>   proven rendering payoff. Treat the wire model as scaffolding, not a fix.

Status: **paused again (2026-06-23, later)** — a gated wire model landed and an
A/B probe settled the priority question: **finishing the ethernet does NOT fix
game rendering.** With `DORADO_ETH_WIRE` ON vs OFF, every broken game renders
byte-identical px (Invaders 163/163, MissileCommand 491264/491264, AstroRoids
163/163, Scavenger 603/603, Reversi 831/831). The EPLOC/OutDone divergence is a
*symptom* (opcode-diff breaking at the first async wait), not the render blocker
— re-confirming `mc-bug-is-emulator-not-ethernet`. Remaining ethernet work
(matching CA's 4.1ms OutDone via tx/rx task mutual-exclusion + the real receiver
FIFO, ~1-2 weeks) is a fidelity item with **zero proven compatibility payoff**;
do NOT prioritize it over the render-path investigation. The gated wire model
stays as boot-safe scaffolding (Galaxian ON = canonical 121553).

(Historical re-prioritization note from earlier 2026-06-23, kept for the trail:
"now has a concrete game-blocker behind it (Invaders), so the transmitter side
is the lead" — superseded by the A/B probe above.)

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

> **2026-06-23 — first gated tx-wire piece landed (`DORADO_ETH_WIRE`, default
> OFF).** A faithful transmit model: when EOT sets TxEOP we hold `tx_eop` set
> (suppressing the EOT "sent" wakeup) and finish the packet in
> `dorado_ethernet_wakeup_mask()` only after **carrier-sense deferral** (no tx
> while a receive is in progress: `rx_on && rx_pos < rx_count`) plus per-word
> wire time (`DORADO_ETH_TX_TICKS_PER_WORD`, 170/word == the rx drain rate).
> A/B vs ContrAlto on Invaders: OutDone now posts **9112 cycles** after the game
> arms the EPLOC wait (was 537 instant), pushing the opcode match **2091 -> 2130**.
> Boot-safe and gate-clean: default OFF is byte-identical (Galaxian 121602,
> NetExec 1484, `make test` 12/12); ON keeps both booting (Galaxian 121600,
> NetExec 1491). **Still ~7x short of CA's ~68000-cycle (4.1 ms) delay** because
> the *receiver* drains the fake rx queue in ~7000 cycles where the real wire
> takes ~60000+. Closing the rest is exactly the receiver wire model below: the
> carrier must stay busy for the incoming packet's true wire time, not until the
> fake FIFO is read. So the tx scaffold is in place; the receiver is the
> remaining work.

> **2026-06-23 — how ContrAlto models the wire (read this before copying it).**
> ContrAlto (`ContraltoLib/IO/EthernetController.cs` + `Scheduler.cs`) drives all
> device timing from a **discrete-event scheduler**: a priority queue of `Event`s
> keyed by absolute nsec; `Schedule(e)` sets `e.TimestampNsec += now` (callers
> pass a relative delay), and `Clock()` (per emulated cycle) fires every event
> whose deadline has passed. The two ethernet delays are scheduled events:
> - **Transmit** -- `EndTransmission()` schedules `OutputFifoCallback`
>   `_fifoTransmitDuration = 87075 nsec` ahead (~87 us to clock 16 words at
>   3 Mb/s). On firing it clears OBUSY and wakes the Ethernet task ("OUTGONE
>   post wakeup"); the microcode then posts OutDone. **No carrier sense / no
>   collision** -- the code comments say so; tx fires its fixed 87 us regardless
>   of the receiver.
> - **Receive** -- `InitializeReceiver()` schedules `InputHandler` every
>   `_inputPollPeriod = 5400 nsec` (~5.4 us = one word at 3 Mb/s); each firing
>   stuffs ONE word into the 16-deep FIFO, wakes the task at >=2 buffered words,
>   and re-schedules itself until the packet drains. A whole-packet host thread
>   enqueues packets (cap 32); the scheduler-paced `InputHandler` is the only
>   thing that fills the FIFO, so a packet occupies words*5.4 us of timeline.
>
> **Our port (2026-06-23, `DORADO_ETH_WIRE`, default OFF), now faithful:**
> - tx: fixed wire time (`tx_count * 170` ticks, ~5.4 us/word == ContrAlto's
>   87 us/16 words), NOT carrier-sense (the earlier carrier-sense version was
>   removed -- it isn't what ContrAlto does and it deadlocked against the paced
>   receiver: the carrier never freed during a long receive and the microcode
>   timed out and cleared TxOn, dropping the packet).
> - rx: pace EIT wakeups at one delivered word per 170 ticks (`rx_wire_timer`),
>   so the world can't drain the prequeued reply faster than ~3 Mb/s -- our fake
>   used to hand the whole queue over in a few us.
>
> A/B vs ContrAlto: gate-clean (default OFF byte-identical: Galaxian 121602,
> `make test` 12/12) and boot-safe ON (**Galaxian ON = 121553, exactly the
> documented gate value**; NetExec in band). Invaders' opcode match extends
> **2091 -> 2130**. It does NOT yet reach ContrAlto's ~733-iteration (4.1 ms)
> EPLOC wait, and we found why: in ContrAlto the OUTGONE post is delayed not by
> the 87 us tx time but by **tx/rx task contention** -- the single Ethernet task
> is busy in the receive loop, so it doesn't run the OutGone handler (which posts
> OutDone) until it yields from the in-progress receive. Reproducing that needs
> modeling the ethernet task's tx-vs-rx mutual exclusion, not just per-word wire
> time. That is the remaining receiver work.

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
