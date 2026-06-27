# ContrAlto as an oracle: where it is valid, and where it is NOT

**Date: 2026-06-23.** A correctness-criterion clarification that reshapes the
timing project. Grounded in the AEmu ethernet source
(`chm/doradosource/AEmuSources-cedar6.0.dm!1_/{AltoEtherEmu,AltoEther,EtherDefs}.mc`,
Taft 1980-81) and HM §10 (ethernet).

## The setup

- **ContrAlto** emulates a **real Alto** running the game, with the Alto's own
  native I/O hardware + microcode.
- **Ours** runs the **AEmu** (Alto emulator) on the **Dorado**. The game is the
  same Alto binary, but the I/O is the **Dorado's** hardware, with the AEmu
  translating the Alto programming interface onto it.

## The Dorado ethernet is different hardware (source-confirmed)

`AltoEther.mc` / `EtherDefs.mc` ("Dorado Ethernet"):
- **Two independent tasks** EIT (input) + EOT (output); the Alto had one
  half-duplex ethernet task ("the emulated Alto interface is half-duplex").
- Hardware the Alto lacks: 16-word FIFO, `RxCollision` ("which the Alto doesn't
  have"), loop-back, single-step, countdown clock, separate Tx/Rx cmd-enable.
- **Its own collision backoff in microcode**: `Random` RNG, `EWait`
  retransmission countdown, `MinPktSpc` ~500 us minimum inter-packet spacing.
  Transmit-completion timing is governed by THIS, not by Alto wire timing.

## The AEmu is a translation layer (source-confirmed)

`AltoEtherEmu.mc` = "Emulator-level microcode for operating Dorado Ethernet".
It emulates the Alto SIO instruction (`ESIO`), presents the Alto's fixed page-1
control block (`ECBR=600`: EPLoc=600 post, EBLoc=601 mask, EICLoc=604 in-count,
EOPLoc=607 out-ptr, EHLoc=610 host) and Alto-compatible post codes
(InDone=377, OutDone=777), and drives the Dorado EIT/EOT tasks underneath.
`EPost` stores EPLoc, ORs the bit into NWW, and `Reschedule`s the emulator.

## The conclusion (the correctness criterion)

**ContrAlto is a valid oracle for ARCHITECTURAL correctness, NOT for I/O
timing.**
- Valid: the game's computation -- ACs, memory, control flow that does not
  depend on *when* an I/O completion posts. (Why ours matches ContrAlto for the
  first 2091 opcodes on Invaders.)
- NOT valid: anything that depends on I/O completion timing. A **real
  Dorado-AEmu would also diverge from a real Alto** at such points, because its
  ethernet completes on Dorado timing (FIFO + random backoff), not Alto timing.

Therefore:
1. **The Invaders divergence at opcode 2091 (the EPLOC/OutDone wait) is EXPECTED,
   not a bug.** Making ours' OutDone land at ContrAlto's opcode ~2130 was chasing
   the *Alto's* wire timing -- the wrong target. Ours' OFF behavior (OutDone
   posts fast) is arguably closer to a fast Dorado than ContrAlto's slow Alto.
2. **The lockstep-with-ContrAlto metric is only meaningful UP TO the first
   timing-sensitive I/O event.** Past it, ours follows the Dorado-AEmu path and
   ContrAlto follows the Alto path -- both legitimate, not opcode-comparable.
3. **The blank-screen bug is NOT established to be at the ethernet.** It is
   downstream, in our Dorado-AEmu execution, where ContrAlto cannot diagnose it.
4. **The right reference for I/O is the Dorado spec + the AEmu microcode
   contract** (does our C ethernet satisfy what `AltoEther.mc` expects of the
   hardware?), not ContrAlto. Note our C ethernet is a packet-level FAKE that
   models none of the Dorado FIFO/backoff/collision/inter-packet spacing, so its
   timing matches neither a real Alto nor a real Dorado.

## What this changes about the plan

- **Stop** trying to make the ethernet OutDone match ContrAlto's opcode timing
  (the EOT-microcode rework in the timing scope is chasing the wrong oracle).
- **Re-validate the lockstep metric**: treat the first I/O-timing divergence as
  the *end* of ContrAlto's usefulness for a given game, not as the bug.
- **For the render bug**, find a Dorado-grounded diagnostic: validate the AEmu's
  Alto-memory/display state against what the AEmu microcode should produce, or
  obtain a real-Dorado-AEmu reference, rather than ContrAlto opcode-lockstep.
- The `DORADO_ETH_WIRE` model and the EOT investigation remain valid *findings*
  but are deprioritized: matching the Alto wire time is not the goal.
