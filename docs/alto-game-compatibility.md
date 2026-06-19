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

## Deep trace of the MissileCommand stall (session 2026-06-18/19)

Instrumented the Alto-program level (`DORADO_IFUDISP_TRACE` for the Alto PC,
`DORADO_FETCH_TRACE` for read VAs, `DORADO_MACHINE_PCHIST` extended to all
tasks + a field/RTC handler watch, `DORADO_STORE_TRACE_VA` for writes). The
stall is now fully characterized, and several hypotheses were tested and
**ruled out**:

- **Alto PC**: MC's emulated Alto pins in a 5-segment busy loop
  (`br31` 0x5F2/0x366/0x360/0x60C/0x35E) for 1.3+ display fields -- no other
  Alto code ever runs. Galaxian (works) runs 20+ segments continuously
  (busy-drawing), and passes *through* 0x5F2/0x60C without blocking, so those
  are a shared runtime routine MC gets stuck in.
- **Polled words**: the loop reads exactly 9 low-memory words
  (0o576,0o600,0o1153,0o1466,0o1537,0o1637,0o3016,0o3017,0o3020). All are
  **constant across 2.4M cycles (~4 fields)**.
- **Writers**: the 9 words ARE written during init -- boot clear (~24M),
  game-image load (`task=7 pc=0o6642`, ~30M), and further rewrites through
  ~49M+ (incl. a `pc=0o4000` clear that is infrastructure -- identical 2^22
  hits for working Galaxian, so not the bug). The point is that **during the
  stall (150M+) the values are stable** (verified by gated reads at 150.0M /
  151.2M / 152.4M): whatever should change one of them next never does.
  (Correction: an earlier draft said "nothing writes them after boot" -- that
  was a subset-range artifact; they are written through init, then freeze.)
- **RULED OUT - field/RTC**: the AEmu field/RTC handlers run for MC at the
  *same* rate as Galaxian (ENDOFFIELD 96 vs 88, EVENFIELD/THTNEWFIELD ~97 vs
  ~89, RTCCARRY 34 vs 34). The display field and RTC are processed fine.
- **RULED OUT - Alto interrupts**: `INITIATEINT`/`RESCHEDPENDING` run **0**
  times for *every* game incl. Galaxian -- no game uses Alto-level interrupts;
  they busy-poll.
- **RULED OUT - input polling**: MC reads **nothing** in the Alto I/O region
  (0o177000+) during the stall -- it is not waiting on keyboard/mouse.

### Conclusion
MissileCommand has **deadlocked on internal state**: it set up 9 words during
init, then entered a busy-wait for one of them to change, but the code that
would change it never runs (it is itself stuck in the wait). Nothing external
(field, RTC, interrupt, input) is involved. The most likely cause is that
**MC's initialization computed a wait-loop value wrong** -- an Alto-opcode /
AEmu emulation subtlety MC exercises that Galaxian does not -- leaving the
loop's exit condition permanently unsatisfiable. Pool is the same shape
(all-white, frozen); PinBall gameplay is presumably the same after start.

### Next step (a real harness, not a quick patch)
Two ways to find the exact wrong opcode/value:
1. **ContrAlto cross-validation** (the project's stated strategy): run the
   same boot file under ContrAlto2 and diff emulated-Alto architectural state
   until it diverges. Pinpoints the opcode but needs a boot-file load path
   into ContrAlto.
2. **Manual back-trace**: disassemble MC's 5-segment loop, identify the exact
   word + value its exit test wants, then back-trace (with the new traces) how
   that word got its stuck value during MC init -- the opcode that produced
   the wrong value is the bug.

### Cross-validation result (2026-06-19, option 1)
Used the in-tree differential harness `altodiff-dorado` (runs one Alto opcode
on the real AEmu microcode, diffs the architectural result -- AC0-3, carry,
PC, memory writes -- against the spec-derived `altoref.c`; this is the
in-tree equivalent of the ContrAlto opcode diff, and sidesteps ContrAlto's
need for a host boot server to run a `.boot` file). `altodiff-dorado sweep`
runs **20768 vectors with 0 mismatches** across:
- all 8 ALC functions x shift/cycle/no-load/skip/carry,
- STA/LDA/ISZ/DSZ page-zero, single-level indirect, and indexed
  (AC2/AC3-relative, direct + indirect).

**So MC's deadlock is NOT a per-opcode bug in the common Nova/Alto ISA.**
MC's loop reads 0o576..0o3020 (not the auto-increment range 0o20-0o37), so
auto-increment is not implicated either.

Remaining uncovered (and NOT modeled by `altoref`): the Alto-specific
graphics/emulator opcodes -- `BITBLT` (0o60000), `CONVERT` (0o63400-ish),
JSRII/JSRIS, the cycle/RCLK/SIO I-O group. These are the prime remaining
suspects for a graphics game that deadlocks while simpler games run. Next:
confirm whether MC's 5-segment loop executes any of these (decode the loop's
Alto instructions), and if so extend `altoref` + the sweep to cover them, or
disassemble MC's loop (salto/tools/adasm.c) to read the exit condition
directly.

This is separate from the Cedar germ bring-up tracked in `CONTINUE-HERE.md`.
