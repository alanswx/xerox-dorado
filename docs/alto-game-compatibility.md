# Alto game compatibility (Alto-on-Dorado / AEmu world)

Status of the `io/murray` Alto games served over the fake EFTP boot server
(`make run-<game>` / the web build). Updated 2026-06-18.

## Matrix

| Game | Behavior | Notes |
|------|----------|-------|
| Galaxian | Works | attract animates, renders ~121553 px (the gate value) |
| Reversi | Works | playable end-to-end |
| Boggs | Works | eventually draws the Boggs photo (slow) |
| PinBall | Partial | instructions screen renders; **gameplay (post-start) does not** |
| MissileCommand | Broken | playfield clears to black, then frozen; no content |
| Pool | Broken | installs 6 DCBs but renders 0 px (all white), frozen |
| StarWars / Trek | Unverified | in the "no display in budget" bucket |

## What is and isn't the problem (evidence)

The renderer is **not** the gap. `dorado_machine_render_display_list` re-walks
the Alto DCB chain from memory every frame, so anything a game writes to its
bitmap is re-rendered.

Two diagnostics, both env-gated and committed:

1. `DORADO_DCB_TRACE` (machine.c) -- dumps the DCB chain + a per-render bitmap
   hash. Result: the broken games have a **frozen bitmap hash** (their
   per-field draw loop stops updating the bitmap), while Galaxian's hash
   changes every frame. So a display list IS installed and rendered; the game
   just isn't drawing into it.
   - MissileCommand: 1 DCB, mostly-black, hash frozen.
   - Pool: 6 DCBs, all-white (0 px), hash frozen.
   - PinBall instructions: 46 DCBs, 89372 px, correctly static.

2. `DORADO_MACHINE_PCHIST` (machine.c, needs `--progress` on `build/dorado`)
   -- hottest task-0 (AEmu) microcode PCs. Result: the distributions are
   **nearly identical** across MissileCommand, Galaxian, and Pool (same hot
   PCs 0o4000/0o334/0o622/0o4147/0o736/0o755/0o31/0o1245, same magnitudes;
   the round 2^22 / 2^20 counts are deterministic infrastructure loops, not
   game logic). The Dorado microengine is healthy and runs all three games
   the same way.

Interactive check (web build): driving mouse + clicks + keys into the frozen
MissileCommand delivers input to the machine (our XOR mouse-arrow tracks the
cursor) but the game never reacts -- so it is **stuck, not idle-waiting for
input**.

## Conclusion + next lead

The stall is at the **Alto-program level**: the games are being emulated, but
MissileCommand/Pool spin in a loop that never advances the display -- they are
waiting on an Alto timing/I-O feature that is not advancing.

Prime suspect: the Alto **vertical-field / real-time-clock (RTC)** path. AEmu
emulates it (symbols `SETDISPLAYFIELDRATE`, `EVENFIELD`/`ENDOFFIELD`,
`RTCCARRY`, `STARTCOUNTERS`, `THTNEWFIELD`, `DOCURSOR`), and the Dorado
machine has no explicit Alto-level field/RTC interrupt wiring -- it only wakes
the DHT/AHT/DWT display tasks (machine.c ~1101). If the field-rate / RTC
counters the games poll for timing never tick (or tick wrong), animation
loops freeze while static screens still render. Games that work (Galaxian,
Boggs) likely use a different timing source or self-clock.

### Next step
Instrument at the Alto-program level: capture the emulated **Alto PC** spin
during the MissileCommand stall and the **VA it polls** each iteration. If it
is reading an RTC / field-count / interrupt word that our emulator leaves
static, drive that path (verify EVENFIELD/ENDOFFIELD/RTCCARRY advance at the
right cadence). Confirm against Galaxian, which does advance.

This is separate from the Cedar germ bring-up tracked in `CONTINUE-HERE.md`.
