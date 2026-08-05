# How to measure this emulator without fooling yourself

Written 2026-08-05, out of two days in which the *measurement* was wrong
more often than the code was. Every trap below produced a confidently
stated number that was false, and several nearly produced a "fix" for a
thing that was not broken. Read this before optimizing anything here.

The short version: **the unit is microinstructions, the gate is
byte-identical framebuffers, and the only trustworthy comparison is a
timed A/B where exactly one thing differs.**

---

## 1. The unit. `--cycles` does not count microcycles

`m->bb.cycles` — behind `--cycles`, `--type-at`, every gate budget and
`DORADO_TRACE_GATE` — is the **BaseBoard 6502** cycle count.
`baseboard_run()` executes a whole 6502 *instruction* and adds all of its
cycles, so it advances **3.70 per Dorado microinstruction**.

    guest seconds = cycles / 3.70 / 16.666e6

10 M cycles is ~162 ms of guest time, not 600 ms.

**What this cost:** a published claim of "29.1 M microinstructions/s =
1.75x the real Dorado" that was really 0.46x — off by exactly 3.70. A PARC
veteran said it felt slow; he was right and the documentation was wrong for
weeks. It also made the guest's real-time clock look 3.70x slow and nearly
produced a fix for a clock that was correct.

**The defence:** every run now prints emulated-Dorado-seconds per
CPU-second, computed from `cpu->cycles`. Quote that line. If you find
yourself dividing a cycle count by a wall time, stop.

## 2. `cpu->cycles` is snapshotted

A restored checkpoint arrives carrying every microinstruction its *bake*
executed. Charging those to this run reported a restored Cedar desktop at
**"14.7x real hardware"**. Measure the delta from run start.

## 3. Any `DORADO_*` variable disables the trace fast path

Setting even one costs double digits. Two ways this lies to you:

- **A gate run and a bare run are not comparable.** Gates set
  `DORADO_PDI_IGNORE_LABEL_FLAGS` and `DORADO_FAKE_TIME`, so a "before"
  under a gate and an "after" bare will show a speedup that is entirely the
  cliff.
- **Your experiment's own flag can distort the control.** Measuring the
  BaseBoard change against `DORADO_BB_ALWAYS_STEP=1` made the *control* pay
  the cliff and a real 20% read as 35%.

Config variables that are not traces belong on the allowlist in
`dorado_trace_init()`. **A name on that list must be read with a cached
`getenv`, never `dorado_trace_flag()`** — through the memo it answers 0
whenever it is the only `DORADO_*` set, which is a behaviour change wearing
a speedup's clothes.

## 4. Do not A/B against a stale PGO profile

Adding one branch to the hot loop and re-measuring under a profile trained
on the *unmodified* binary showed the **control** regressing 15.8 s →
17.0 s, and made a real 7% win read as a 40% loss.

Either retrain per variant, or A/B on `-O3 -flto` without PGO and apply PGO
once at the end. **The tell is the control moving** — always measure the
control in the same session as the experiment, and be suspicious when it
does not match yesterday's.

## 5. Profile attribution lies under LTO

`-flto` inlines callees into callers, so a caller's "self time" is mostly
its inlined callees. `dorado_machine_run_until` showed 31% and I read it as
polling overhead; the change that inference predicted at ~15% measured
**1.4%**.

**Profile two builds:**

- the shipped `-O3 -flto` + PGO build — what actually runs;
- a plain `-O2 -g` build — for attribution, because functions stay separate.

Use the plain build for *ranking* and the shipped one for *reality*. Leaf
functions (`eth_ftp_pick_busy_conn`, the 6502 opcodes) are trustworthy in
both; anything that inlines is not.

## 6. Percentages move when the denominator shrinks

After the germ-bridge fix every other Cedar entry "rose" — `execute_uinstr`
34.3% → 40.3%. Nothing got slower; 18% left the total. Compare absolute
times, or say explicitly that you are comparing shares.

## 7. Byte-identical framebuffers, not pixel counts

`cmp` two `.pgm` files. A count cannot tell a correct screen from a
differently-wrong one, and this project has shipped regressions a count
waved through.

**One world is not enough** for anything touching I/O or timing. The three
that exercise genuinely different paths:

| world | exercises |
|---|---|
| Galaxian | Alto, ethernet, the display |
| Cedar desktop | PDI disk, STP, the germ bridge |
| Lyric | Interlisp, Leaf, `DORADO_DISPM_PRESENT` |

`make test`, `verify-lisp`, `verify-cedar-desktop` and
`verify-snapshot-abi` on top. `rundiag` matters for anything touching the
BaseBoard — note IfuComplex fails *pre-existing*, so compare its step count
and PC rather than pass/fail.

**Byte-identity can surprise you in both directions.** The germ-bridge
cadence was predicted not to be bit-exact and was; Phase 5's synthetic
clock was expected to be and shifted `verify-cedar-desktop` by 41 px
because a fixed cycle budget covered 0.03% more microinstructions.

## 8. Snapshot ABI, and static caches that shadow it

A new `dorado_machine`/`dorado_cpu`/`dorado_ethernet` member changes the
snapshot ABI and **every baked checkpoint fails to restore**. Caches go in
file-scope statics, reset in `dorado_machine_create` — as the keyboard
queue, display-active memo and germ poll cursor do.

But a static that summarises *snapshotted* state is stale after a restore,
and the failure mode is silent. `make verify-snapshot-abi` catches the
first problem in a second; nothing catches the second but care.

## 9. A speedup can be real and invisible

Both frontends paced a fixed number of cycles per vsync frame, which *is*
the emulated speed: 400,000 × 60 Hz = 0.39x. Doubling core throughput
changed nothing a user could see until that cap moved. Check the frontends
before concluding a change failed — and before claiming one succeeded.

## 10. `make clean` used to destroy the checkpoints

`clean` was `rm -rf build`, and `build/good-packs` — where `CLAUDE.md`
tells you to keep expensive artifacts *because it persists* — lived inside
it. A flag sweep that cleaned between builds destroyed every local
checkpoint. `clean` now preserves it; `clean-objs` is what a build loop
should use, and `clean-everything` is the explicit opt-in.

Mixing compilers is the related trap: a failed `make pgo` left clang LTO
objects that the next gcc link could not read, which took the Pages deploy
down. Never let one compiler inherit another's objects.

---

## The loop that works

1. Profile two builds. Rank from the plain one.
2. Pick one change. Predict what it should be worth.
3. A/B it, three runs each, same session, one variable.
4. `cmp` the framebuffers on all three worlds.
5. If the number disagrees with the prediction, **find out why before
   believing either** — that is where most of the findings above came from.
6. Write down what it was worth, including the ones worth nothing.

Things that measured far less than the profile implied: the diagnostic
preamble hoist (predicted 15%, got 1.4%), the input-seeding skip (2%).
Things that measured far more: the trace-global hoist at the top of
`execute_uinstr` (7.3% from eight stores), the germ-bridge cadence (18%
of profile, 35% of wall time, because its reads were also the largest
memory-path entry).
