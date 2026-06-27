# Restart Handoff: Alto Games No-Render

Date: 2026-06-20

## Goal

Get the no-render Alto games (MissileCommand, Pool, PinBall-gameplay) to draw
under the AEmu-on-Dorado world, as Galaxian/Reversi/Boggs already do.

## STATUS (2026-06-20): bottomed out -- no quick fix

A focused, multi-angle attempt this session ruled out every tractable
mechanism by experiment (not speculation). MC's no-render is a **deep,
multi-layer, cumulative-timing/state-divergence** failure, not a single
value/knob/cell. Read the dated sections below for the evidence; the
short version:

- **NOT a single ethernet value.** Our AEmu posts CmdAbort `2771` (vs real
  Alto `2777`) repeatedly but it never *settles* -- EPLOC oscillates
  `377/777/2771 <-> 0`. Forcing the value (prior CmdAbort patch) does not fix
  it.
- **NOT the completion-delay timing.** Swept `DORADO_ETH_TX_DELAY_*` /
  `RX_WORD_HOLD` across the full render -- all identical frozen-black. The
  first PC divergence (a +/-1-iteration phase slip in the `0o723` EPLOC poll)
  is tunable but a **red herring** for the outcome.
- **NOT the display field/RTC rate.** The AEmu field handlers run at ~the same
  rate for MC and the WORKING Galaxian (ENDOFFIELD 96 vs 88, RTCCARRY 34/34).
  Galaxian renders at that rate; MC does not. (This *refutes* the earlier
  "~9x too slow field is the lead".)
- **NOT a time budget.** MC at **300M cycles is still frozen-black** (199M
  cycles after boot completes ~101M).
- **NOT the boot loader.** MC's boot DATA loads at Galaxian speed; the "2.5x
  slow boot" is one 15M-cycle attract-init **network-I/O timeout** (MC pokes
  our boot-only fake with an unhandled Pup `0o204` + EFTP it cannot service),
  which self-resolves and is downstream-irrelevant.
- **Multi-layer.** Hold-poking `M[0o3016]=2616` post-boot (force the interrupt
  handler's normal-return dispatch) flips MC frozen-BLACK -> WHITE-blank:
  correct attract *background*, but **zero content** drawn. Each gate opened
  reveals the next.

**Only real paths to render MC:** (a) cycle-accurate emulation timing so MC's
cumulative state tracks ContrAlto (an architectural project), or (b)
gate-by-gate RE of a source-less 1980 binary (demonstrably low-yield). Neither
is quick. The faithful ethernet receiver (`docs/ethernet-faithful-receiver.md`)
is a real fidelity win but, per the post-boot poke, would not by itself render
MC. Recommendation: treat MC/Pool as known-hard unless rendering them is worth
the cycle-accurate-timing investment.

## Read First

- `tools/nova-trace-diff/README.md`
- `docs/alto-game-compatibility.md`
- `docs/ethernet-faithful-receiver.md`
- `docs/CONTINUE-HERE.md`

Note: `mc-bug-is-emulator-not-ethernet.md` is NOT in the repo `memory/` dir --
it lives in the Claude memory dir
(`~/.claude/projects/-Users-alans-Documents-development-Dorado/memory/`) and
is loaded into session context automatically. It holds the DEEPEST prior
analysis (the attract is interrupt-driven; the stuck ethernet wait is a
*symptom* of MC's interrupt handler deciding `M[0o3016]=1537` instead of
`2616` once interrupt-vs-mainline timing has drifted). Read it before
re-deriving the ethernet angle. CAUTION: that memory's "display field ~9x
too slow -> calibrate the cadence" lead was **refuted** here -- the field
rate is ~equal for MC and the working Galaxian, so it is not MC's
differentiator (see STATUS above).

## Current Code State

The only intended functional fix currently retained is in `dorado/src/ethernet.c`:

- `dorado_ethernet_init()` now defaults `eftp_turnaround_ticks` to `2400`, while preserving the `DORADO_EFTP_TURNAROUND_TICKS` override.
- `eth_eftp_deliver_current()` comments document that `2400` matches ContrAlto's same-binary poll iteration at `0o100`.
- `dorado_set_rx_on()` only clears receive state on RxOff for the EFTP wait-for-arm case:
  `if (!on && eth->eftp_wait_for_rx_arm)`.

Do not reintroduce the broader RxOff reset-clear experiment unless it is made much more precise. It removed one stale PinBall receive byte but did not improve rendering and regressed NetExec.

## Session 2026-06-20 (cont.): harness fix + MC root cause refined

### Fixed: `tracediff.sh` was silently broken (zero Dorado writes)

`tracediff.sh` converted the address to decimal (`DOR_ADDR=$((8#$ADDR))`)
before passing it to `DORADO_STORE_TRACE_VA`, but the parser in
`dorado/src/memory.c` (~line 1043/1094) uses `sscanf("%lo,%lo")` -- **octal**.
So `600` became `384`, `%lo` stopped at the non-octal `8` leaving `hi=0`, and
the trace was disabled. Result: **every recent Dorado-side `tracediff` produced
0 writes** (only the ContrAlto side had data). Fixed to pass `$ADDR` octal
verbatim. Confirmed working: `tracediff.sh 3016` now yields 45961 Dorado
writes. Any prior "ours" tracediff conclusion from this script is suspect.

### Same-binary cross-validation (ContrAlto boot-server vs ours), MC

Baseline re-verified green: tests pass; Galaxian 121549, NetExec 1499.
ContrAlto runs OUR exact `MissileCommand.boot` and renders (17268 px),
ending `EPLOC(0o600)=2777`. Ours is frozen-black, ending `EPLOC=0`.

New, sharper characterization (supersedes the older "AEmu never posts 2777"):

- Our AEmu **does** post CmdAbort -- value `2771` (`ECmdBits XOR CmdAbort`,
  vs the real Alto's clean `2777`) -- and posts it **repeatedly**, not never
  (seen at cyc 76.5M, 101.0M, ...). It just never *settles*.
- MC enters a **sustained ~92K-cycle loop** (from ~76.5M through past 101M).
  Each iteration: MC clears `EPLOC`/`EICLOC` (Emulator pc=0o42/0o53) -> our
  AEmu posts a completion to `EPLOC` (`777`=OutDone task6, `377`=InDone task7,
  or `2771`=CmdAbort) at EPOST pc=0o3461 -> MC clears it again. EPLOC
  oscillates `377/777/2771 <-> 0` forever and never holds CmdAbort.
- MC issues **no transmits during the stall** (all 103 TX are boot: 1 Mayday,
  1 uCodeBoot, 101 EFTP-Acks). The EFTP boot stream delivers once (seq 0..100)
  and stops -- it is **not** the steady-state feed. Yet InDone/OutDone keep
  posting, so our AEmu/fake is **spuriously completing ethernet ops** while
  MC waits (EOT is woken whenever `tx_on && !tx_eop && !tx_cntdwn`; the EIT/rx
  side re-feeds via leftover/queued state on MC's RxOff/RxOn re-arm).
- `tracediff.sh 3016`: ContrAlto writes `2616` and **holds it** (1970 writes);
  ours **oscillates 2616<->1537<->0 and spins 45961 writes** (pc=10 br=644).
  Same `br31=0o644` region as the EPLOC posts. This is MC re-running its
  ethernet bookkeeping every spurious completion instead of settling.

### Conclusion (consistent with prior multi-session finding, now mechanised)

The blocker is a **race + fidelity gap in the AEmu<->fake ethernet receive**,
not a single value or memory cell: our controller keeps generating spurious
InDone/OutDone completions during MC's post-init wait, which both (a) race
ahead of / overwrite the CmdAbort post and (b) keep MC re-processing, so EPLOC
never settles at the clean `2777` a real Alto reaches. Forcing the `2771->2777`
value alone does not fix it (already ruled out) because the race and the
spurious completions remain. A correct fix must make the post-SIO-reset
controller go genuinely quiet (no EOT/EIT completion when there is no real
packet) **without** regressing the boot path (Galaxian/NetExec gate), which is
why naive RxOff-clear / completion-suppression experiments regressed NetExec.

### Experiments run this session (all measured vs the 130M render)

Reference: ContrAlto renders MC = 17268 black px; our frozen-black blank =
368448 nonwhite.

- **OutDone-delay sweep is the lever for the FIRST PC divergence but a
  red herring for the outcome.** `DORADO_ETH_TX_DELAY_MUL/ADD` shifts the
  `0o723` EPLOC-poll phase: 27/3 -> ours exits 1 iter late (diverge @2127);
  8..20 -> ours exits 1 iter early (@2100..2118). It never *exactly* aligns
  (sub-instruction phase). Crucially, **every value renders identically
  frozen-black (368448)** at 130M (swept 13/18/20/40 x add, +rx_word_hold).
  So the first PC divergence is a harmless +/-1-iteration phase slip the diff
  flags but that does not change the result.
- **Field/RTC rate is NOT MC's differentiator** (confirms the memory's own
  data): the AEmu field handlers run at ~the same rate for MC and the
  WORKING Galaxian (ENDOFFIELD 96 vs 88, RTCCARRY 34 vs 34). Galaxian renders
  at that rate; MC does not. MC's differentiator is post-boot ethernet, which
  Galaxian never touches.
- **Forcing the handler decision is insufficient.** Hold-poking
  `M[0o3016]=2616` (so the interrupt handler always takes the normal-return
  dispatch instead of the stuck ethernet routine 0o1540) changes MC from
  frozen-BLACK to WHITE-blank (0 nonwhite) -- a *different* stuck state, not
  the attract. So the handler's stuck-ethernet dispatch is one gate, not the
  only one (matches the memory's multi-factor finding).

### Honest bottom line (this session)

The env-tunable levers (OutDone/InDone completion delay, rx hold, field/RTC
cadence) are **exhausted** -- none render MC. The PC-stream phase slip is
harmless. The real blocker is the AEmu ethernet **completion oscillation**
(EPLOC never settles: 377/777/2771 posts race MC's clears forever) plus at
least one further early-init gate. The only principled fix left is the
**faithful Dorado ethernet receiver** (`docs/ethernet-faithful-receiver.md`):
model the real Alto controller's FIFO + receive state machine so completions
post ONCE and settle (as ContrAlto's do) instead of oscillating. That is a
substantial rework, it is gate-risky (same machinery the boot path uses), and
it may still not fully render MC (the white-blank gate above). It is the next
real step if MC is to be cracked; knob-tuning will not do it.

### Step-1 instrumentation (faithful-receiver track) reframed the problem

Pursuing the steady-state completion feed surfaced two facts that change the
framing:

- **The EFTP boot LOAD is slow and overlaps the "stall."** EFTP delivery
  starts at cyc ~75.9M for every game (after Initial -> AEmu -> NetExec ->
  Mayday). MC's load is ~2.5x slower per packet than the working Galaxian
  (MC 251K cyc/pkt, done ~101M; Galaxian 99K cyc/pkt, done ~87M -- both 100+
  packets, both start 75.9M). So much of MC's 76M-101M "EPLOC oscillation"
  is the EtherBoot LOADER receiving boot packets, not a post-boot hang. The
  ~2.5x slowdown is itself a symptom of MC's worse ethernet-completion poll
  phase (more loader spin per packet), the same phase issue as the `0o723`
  divergence.
- **It is genuinely a post-boot hang, NOT a time budget.** MC at **300M
  cycles is still frozen-black** (199M cycles after boot completes ~101M).
  Confirms the memory's "400M does not fix it" on the current tree.
- **The hang is multi-layer.** Hold-poking `M[0o3016]=2616` *after* boot
  (from 105M) flips MC from frozen-BLACK to WHITE-blank: the handler now
  returns normally and MC paints the correct white attract *background*, but
  draws **no content** (0 black px vs ContrAlto's 17268). So fixing the
  handler's stuck-ethernet dispatch only exposes the NEXT gate (content
  draw). Opening each gate reveals another -- the documented low-EV pattern.

Conclusion after a focused, instrumented attempt: there is no single
tunable/pokeable fix. MC's attract is a chain of timing/state-dependent gates
that each diverge from ContrAlto due to cumulative micro-timing differences.
Cracking it needs either (a) cycle-accurate emulation timing so MC's
cumulative state tracks ContrAlto, or (b) gate-by-gate RE of MC's attract
init (many levels, each low-EV). Both are large; neither is a quick win. The
faithful receiver would settle the ethernet oscillation (a real fidelity win)
but, per the post-boot poke, would not by itself render MC.

### Boot-speed anomaly dig (2026-06-20) -- RESOLVED, not a loader bug

The "MC boots 2.5x slower than Galaxian" is explained and is NOT a separate
fixable loader/emulation bug:

- MC's boot DATA loads at the SAME speed as Galaxian. Inter-packet
  InDone-gap MEDIAN is identical (MC 92838, Galaxian 92837 cyc). Packets
  0..101 are all received by ~85.9M for both.
- MC's slowdown is ONE 15.2M-cycle stall (85.9M -> 101.1M) AFTER the boot
  data is loaded, with NO resends. It is MC's **attract-init network I/O**:
  MC issues post-load network requests our **boot-only fake does not
  service** -- an unhandled Pup type `0o204` and EFTP traffic with IDs
  climbing 136..144 (~8 retransmits over ~15M cyc ~= 2M/retransmit) -- so MC
  retransmits/limps ~0.9 s, times out, and proceeds. (Our fake replies to
  Mayday/EFTP-Ack/Gateway/AltoTime/AddressLookup/BootDir only; post-boot
  EFTP-Acks hit `eth_eftp_ack` with `state=0` and are dropped; `0o204` is
  unhandled.)
- It **self-resolves** (timeout) and is **downstream-irrelevant**: MC
  proceeds to the SAME multi-layer frozen-black hang regardless (300M proves
  it is not a time budget). Galaxian does no attract networking, hence no
  stall.

So the anomaly is a facet of the fundamental limitation -- our `ethernet.c`
is a scripted BOOT server, not a general network peer -- and a more-networked
game (MC) exercises post-boot networking it cannot service. The only concrete
unhandled item is Pup type `0o204`; there is no evidence that replying to it
would render MC (the render hang is a separate content-draw gate, proven by
the post-boot M[0o3016]=2616 poke -> white-no-content).

## Verification Already Run

Build and unit tests:

```sh
make -C dorado build/dorado build/test_cpu build/test_ethernet
cd dorado
./build/test_ethernet
./build/test_cpu
```

Both unit tests passed.

Image smoke results with the retained `2400` default:

- Galaxian: renders. Final image had `121549` nonwhite pixels.
- NetExec: renders. Final image had `1493` nonwhite pixels.
- MissileCommand: still blank/frozen by behavior, though it has many display-list pixels.
- PinBall: after keyboard input it gets past the text screen, but still does not draw the pinball-machine graphics.

PinBall command used:

```sh
cd dorado
./build/dorado --eb worlds/aemu.eb --eftp '../chm/bootfiles/PinBall.boot!1' \
  --type ' ' --type-at 140000000 --key-hold 1000000 \
  --cycles 220000000 --out /tmp/pin_noreset_2400_220.pgm
```

Result:

```text
unique 2 nonwhite 227136 nonblack 262512
```

## Trace Findings So Far

The old default `DORADO_EFTP_TURNAROUND_TICKS=60000` delivered the next EFTP packet too late. Dorado stayed in the loader poll loop at `br31=100` with AC0 zero while ContrAlto had already seen `0o377`.

Sweep of Dorado-only first `AC0=000377` at the poll loop:

- `DORADO_EFTP_TURNAROUND_TICKS=0`: first hit near trace entry `2175`, too early.
- `2000`: first hit near `2315`.
- `2400`: first hit near `2345`.
- `2600`: first hit near `2360`.
- `5000`: first hit near `2530`.
- `10000`: first hit near `2890`.
- `15000+`: no `377` before 3000 trace entries.

ContrAlto reference first sees `AC0=377` around trace entries `2344-2345`.

With the current default, Dorado aligns to the same poll iteration:

```text
DTRACEPC 2345 102 001 pcx=4 br31=100 insset=0 cyc=76597745 acs=000377,000000,001000,000777
```

Important: `tools/nova-trace-diff/tracepcdiff.sh 20000` still reports a divergence around this region because the streams are not fully equivalent. The fix only aligns the formerly too-late receive completion.

## Tried And Reverted

### Broad RxOff Reset-Clear

Experiment:

```c
if (!on && (eth->eftp_wait_for_rx_arm || (eth->eftp_max_seq > 0 && eth->eftp_state == 0))) {
    ...
}
```

Effect:

- Removed an immediate stale `task7 pc=3461 data=377` after PinBall rearmed receive around `143408M`.
- Did not change the final PinBall image.
- Regressed NetExec to effectively blank output.

Final code keeps the narrower EFTP wait-for-arm clear only.

### Trailer Marker / CRC As IOAtten

Marking CRC as IOAtten instead of status broke booting and produced no useful rendering. Reverted.

### CmdAbort Normalization Only

The `2771 -> 2777` CmdAbort normalization was documented as tried and ruled out as a standalone fix in `docs/alto-game-compatibility.md`.

## Next Step (only if rendering MC is worth a major investment)

The cheap/medium levers are exhausted (see STATUS). The trace-diff harness is
now reliable (both `tracediff.sh` and `tracepcdiff.sh` work; the former was
fixed this session) and was used to bottom this out -- but pinning *more*
divergences just walks the multi-layer cascade (each gate opened reveals
another, demonstrably low-yield).

The two real options, both large:

1. **Cycle-accurate emulation timing** -- the only path likely to actually
   render MC. Make the AEmu microcycle/IFU/RTC/ethernet-completion timing
   track real-Alto ratios so MC's cumulative interrupt-vs-mainline state
   matches ContrAlto. This is an architectural project; scope it on its own.
2. **Gate-by-gate RE** of `MissileCommand.boot` (no source): decode the
   content-draw path that stays blank even after the handler decision is
   forced (the white-no-content gate). Low expected value per level.

If touching the ethernet at all, the faithful receiver
(`docs/ethernet-faithful-receiver.md`) settles the EPLOC oscillation (a real
fidelity win) but the post-boot poke proves it will NOT by itself render MC.

Harness tips:
- `DORADO_STORE_TRACE_VA` expects OCTAL text, e.g. `600,605` (the parser is
  `sscanf "%lo,%lo"`). `tracediff.sh` now passes octal directly.
- `tools/nova-trace-diff/tracepcdiff.sh <N>` = first divergent executed PC vs
  ContrAlto (same binary via the BootServer). The first divergence is the
  harmless `0o723` EPLOC-poll phase slip -- look *past* it.
- Useful one-shots (all octal): `DORADO_POKE="va,val,cycle[,hold]"`,
  `DORADO_TRACE_GATE="lo,hi"` (cycle window for the gated ETHC/WAKE/STORE
  traces), `DORADO_EFTP_TRACE`, `DORADO_ETH_TX_TRACE`, `DORADO_ETHC_TRACE`.

## Useful Commands

MissileCommand trace with low-core store logging:

```sh
cd dorado
env DORADO_TRACEPC=3000 DORADO_STORE_TRACE_VA=600,605 \
  ./build/dorado --eb worlds/aemu.eb --eftp '../chm/bootfiles/MissileCommand.boot!1' \
  --cycles 130000000 --out /tmp/mc_final2400.pgm > /tmp/mc_final2400.log 2>&1
```

PinBall keyboard-progress run:

```sh
cd dorado
./build/dorado --eb worlds/aemu.eb --eftp '../chm/bootfiles/PinBall.boot!1' \
  --type ' ' --type-at 140000000 --key-hold 1000000 \
  --cycles 220000000 --out /tmp/pin_noreset_2400_220.pgm
```

Pixel stats:

```sh
python3 -c 'p="/tmp/file.pgm"; data=open(p,"rb").read(); pix=data.split(b"\n",3)[3]; print("unique",len(set(pix)),"nonwhite",sum(b!=255 for b in pix),"nonblack",sum(b!=0 for b in pix))'
```

Trace diff harness:

```sh
tools/nova-trace-diff/tracepcdiff.sh 20000
tools/nova-trace-diff/tracediff.sh 3016
```

## Worktree Caution

The tree was already dirty before this handoff. Known changed/untracked areas included:

- `dorado/include/ethernet.h`
- `dorado/src/cpu.c`
- `dorado/src/ethernet.c`
- `dorado/src/machine.c`
- `dorado/tests/test_ethernet.c`
- `docs/ethernet-faithful-receiver.md`
- `tools/nova-trace-diff/`

Do not revert unrelated changes. Work with the current tree unless a specific change is proven to be part of the bug.
