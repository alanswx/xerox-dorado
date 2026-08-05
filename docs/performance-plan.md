# Performance plan — closing the 2.2x gap to real hardware

Written 2026-08-04, after the clock audit.

## Where we ended up (2026-08-05)

| path | native (`make pgo`) | wasm / browser |
|---|---|---|
| Alto (Galaxian) | **1.33x** | 0.74x |
| Cedar desktop | **1.23-1.26x** | 0.80x |

**Both native paths now beat the real machine**, from 0.46x and 0.39x when
this plan was written. The browser roughly doubled. The one big
non-interpreter item left is Cedar's STP connection scan at 14.3% —
designed in `docs/stp-scan-design.md`, not yet implemented.

How to measure any of this without fooling yourself, and the traps that
produced two confidently wrong answers along the way:
`docs/performance-methodology.md`.

## Where we started

Measured in the machine's own unit (`dorado` prints it at the end of every
run; see `dorado_machine_uinstructions`):

| path | microinstr/s | vs real 16.67 MIPS Dorado |
|---|---|---|
| Alto (Galaxian) | 7.6 M | **0.46x** |
| Cedar desktop | 6.4 M | **0.39x** |

The older "1.75x / 1.51x faster than the hardware" claim was `bb.cycles`
per second — the BaseBoard 6502 counter, 3.70 per Dorado microinstruction —
read as microinstructions. We are slower than the machine by a bit over 2x,
which is what a PARC veteran meant by "it feels slow".

**Target: 16.7 M microinstructions/s on the Alto path**, i.e. 2.2x from
here. That is real-time parity, not a stretch goal — the Dorado's
microcycle is 60 ns and matching it is the point of the exercise.

## Where the time goes

Grouped by what the work is *for*, from `sample` on a Galaxian run
(2026-08-04, M4 Max). Percentages are of ~11k visible samples, so treat
them as ±2 points, not exact.

| group | share | components |
|---|---|---|
| Interpreter proper | ~23% | `execute_uinstr` plus its semantics helpers |
| **Per-cycle I/O polling** | **~35-40%** | `run_until` self 15%, `machine_store_va` 13%, `ethernet_wakeup_mask` 3%, `display_dwt_wakeup` 1.3%, BaseBoard ~4% |
| Task scheduling | ~5% | `task_schedule`, recomputed every microinstruction |
| Memory translation | ~7% | `map_index`, `memory_ref_task`, `visible_word_at_va` |

The middle row is the opportunity. Nothing in it changes at 60 ns
granularity: display fields are 16.7 ms, keyboard and mouse are human-scale,
the BaseBoard 6502 legitimately runs ~62x slower than we step it, and
Ethernet packets are microseconds at best. We ask, 7.6 million times a
second, questions whose answers change a few hundred times a second.

## The phases, cheapest and safest first

### Phase 0 — compiler flags (no behavioural risk)

`-O3`, LTO, `-mcpu=apple-m4`, then PGO. Interpreter dispatch loops
typically gain 10-25% from these alone. Costs an afternoon, cannot change
emulator semantics, and may move the target before a line of emulator code
is written. **Do this first so later phases are measured against the right
baseline.**

### Phase 1 — cache the task scheduler (~5%)

`task_schedule` recomputes best-next-task every microinstruction, but the
wakeup set changes rarely. Cache the result, invalidate on
wakeup/block/unblock. Behaviour-neutral by construction: the cached value is
a pure function of state already tracked.

### Phase 2 — predecode the microinstruction (~10-15%)

The IM is decoded into fields already, but each execution re-derives FF
field meaning, LC routing, ALUFM indexing and branch-target computation
through branchy dispatch. Precompute a per-IM-word recipe — a flags word
saying which post-effects apply — keyed by IM address.

**Constraint:** keep this strictly an emulator-side cache. `CLAUDE.md`'s
Phase-2 rule is to avoid constructs that will not map to RTL (function
pointers per opcode, implementation-defined bit-field layout) in code
intended to become Verilog. A lookup table keyed by IM address does not
change the model's structure; a dispatch table of function pointers
replacing the field decode would. Stay on the first side of that line.

### Phase 3 — VA→physical translation cache (~5%)

Consecutive references overwhelmingly hit the same page. Even a one-entry
cache usually pays.

### Phase 4 — deadline scheduling instead of per-cycle polling (~1.5x, the big one)

Replace "do this every cycle" with "next event at cycle N": one compare per
cycle against the nearest deadline. Collapses input seeding, the ethernet
poll, the display wakeup check and the BaseBoard step onto a single branch.

The Lisp path already demonstrates the principle — it gates its seeding to
one per 16384 cycles — while the Alto path re-seeds ~10 cells *per
microinstruction*, each with a VA translate and a cache invalidation.

### Phase 5 — decouple the master clock from the 6502 (~4% plus fidelity)

`bb.cycles` is derived from 6502 instruction lengths, which is why every
cycle number in the tree is in 6502 units and why the 3.70 factor exists.
Making the time base microinstructions lets the BaseBoard run at its real
rate (one 6502 instruction per ~62 microinstructions rather than per 1).

Compounding phases 1-4: 1.5 x 1.25 x 1.05 ≈ **2.0x**, before Phase 0.

---

## Caveats — read before starting any phase

**1. The estimates are one profile, one path, one machine.** Galaxian on an
M4 Max. Cedar spends real time in the PDI and STP paths Galaxian never
touches, and its 0.39x may have a different composition entirely. Profile
Cedar separately before assuming this ranking transfers. Do not quote these
percentages as measurements of anything but the Alto path.

**2. Byte-identical framebuffers are the gate, not pixel counts.** `cmp`
two `.pgm` files. A pixel count cannot tell a correct screen from a
differently-wrong one, and this project has shipped at least three
regressions that a count waved through.

**3. Phase 4 is the dangerous one, and it is dangerous in a specific way.**
Changing *when* I/O is observed is exactly the class of change that has
desynced boots here before (memory: cycle-accurate timing; several worlds
are sensitive to I/O cadence, and the EFTP RxOn-clear is gated to the Cedar
path precisely because ungating it stalls the Alto boot). A deadline
scheduler must reproduce the same *observable* cadence, not merely a
similar one. Budget for the possibility that some polls cannot be moved.

**4. "Byte-identical on Galaxian" is not sufficient for Phase 4.** The
worlds exercise different I/O. Minimum gate set before believing a polling
change: Galaxian (Alto/ethernet), `verify-cedar-desktop` (Cedar/STP/disk),
`verify-lisp` (Interlisp/Leaf), `verify-smalltalk`, `verify-alto-disk`.
Several are slow; that is the price of touching this.

**5. Do not tune against a run with any `DORADO_*` variable set.** Setting
any of them disables the trace-flag fast path, which is itself a
double-digit effect. Gate runs set `DORADO_PDI_IGNORE_LABEL_FLAGS` and
`DORADO_FAKE_TIME`, so a "before" measured under a gate and an "after"
measured bare will lie to you.

**6. Snapshot ABI.** Any new `dorado_machine`/`dorado_cpu` member breaks
every baked checkpoint. Prefer file-scope statics for caches, as the
existing code does for the keyboard queue and display-active memo, and run
`make verify-snapshot-abi` after anything structural.

**7. `cpu->cycles` is snapshotted.** A restored checkpoint carries its
bake's microinstruction count. Measure the delta from run start or a
restored Cedar desktop reports "14.7x real hardware", as it did before this
was fixed.

**8. Speedups can be real and still not felt.** Interactive "feel" is
frame pacing as much as throughput. If the SDL/browser frontends pace to
wall time, raising throughput past real-time changes nothing a user
notices. Check the frontends' pacing before concluding a phase failed —
and, symmetrically, before claiming one succeeded.

**9. Stop when the gate says stop.** If a phase cannot be made
byte-identical across the gate set, it does not ship, however good the
number is. Record what it was worth and move on.

---

## Progress

| phase | status | measured |
|---|---|---|
| 0 — compiler flags | **DONE** | **1.95x — 0.52x → 1.02x real hardware** |
| 4a — diagnostic preamble hoist | **DONE** | +1.4%, byte-identical |
| 4b — germ I/O bridge cadence | **DONE** | Cedar 0.93x → **1.26x** native, 0.62x → **0.80x** wasm |
| 5 — BaseBoard idle suppression | **DONE** | **+19.7%** — Alto 1.05x → **1.29x**, Cedar 0.73x → **0.92x** |
| 1 — task-schedule cache | superseded — see below | — |
| 2 — predecode | not started | — |
| 3 — translation cache | not started | — |
| 4b — deadline scheduling (rest) | not started | — |

### Cedar: half its deficit was the trace-flag cliff, not the emulation

Setting ANY `DORADO_*` variable disables the trace-flag fast path (and the
preamble hoist above). Every Cedar and Lisp recipe sets
`DORADO_PDI_IGNORE_LABEL_FLAGS` and `DORADO_FAKE_TIME`, so every one of
them paid the slow path for its whole run:

| Cedar desktop, PGO build | |
|---|---|
| with the recipe's env vars (before) | 0.48x |
| **with them, after the config allowlist** | **0.73x** |

Byte-identical framebuffer. Those two variables are CONFIGURATION — a
label-compare rule and a fixed clock — not traces, and
`dorado_trace_init()` now skips a short allowlist of such names when
deciding whether tracing might be on.

**The trap for anyone extending that list:** a name on it must be read
with a direct cached `getenv`, never `dorado_trace_flag()`. A name looked
up through the memo would silently answer 0 whenever it is the only
`DORADO_*` set — a behaviour change, not a speedup.
`DORADO_PDI_IGNORE_LABEL_FLAGS` was read through the memo and had to be
converted first.

PGO now also trains on the Cedar path and merges both profiles; that took
the Alto path to 1.05x as a side effect, and costs nothing.

### What is left between Cedar and real time

Cedar sits at 0.73x, so it needs ~1.37x more. Its profile is genuinely
different from the Alto path's — these are Cedar-specific and do not show
up in Galaxian at all:

| | share | what |
|---|---|---|
| `dorado_visible_word_at_va` | ~11% | guest-memory reads, largely the Cedar I/O bridge polling |
| `eth_ftp_pick_busy_conn` | ~9% | scans every STP connection slot **every cycle**, finding nothing while idle |
| germ/Cedar I/O bridge | ~4.5% | `machine_germ_complete_ethernet_tx`, `..._seed_ethernet_header_page`, `..._complete_disk_iocb`, `set_eftp_rx_armed` |

`eth_ftp_pick_busy_conn` is the clearest single target and it is a leaf
function, so unlike the `run_until` attribution above this share is
trustworthy. **It was not attempted here**: skipping the scan needs a
"nothing is pending" hint maintained wherever the connection fields
change, that state cannot become a `dorado_ethernet` member without
breaking every checkpoint (snapshot ABI), and this is the subsystem where
the EFTP RxOn-clear had to be gated to the Cedar path because ungating it
stalled the Alto boot. It deserves its own session and the full gate set,
not the tail of one.

### WebAssembly (2026-08-04)

Measured on the node harness, which is the same wasm the browser runs.
Galaxian, 1 B cycles, every step byte-identical:

| build | speed |
|---|---|
| `-O2` (the old default) | 0.54x real hardware |
| `-O3` | 0.54x — **no gain, omitted** |
| `-O3 -flto` | 0.57x |
| `-O3 -flto` + the **native** PGO profile | **0.68x** |

The wasm build already had Phase 5, the preamble hoist and the config
allowlist, because those are in the shared C sources — only the compiler
flags were missing. `-O3 -flto` is now the default and the profile is used
when present, so the browser gains **27%**.

**Emscripten cannot COLLECT a PGO profile but can USE one.** An
instrumented wasm build runs and writes no `.profraw` — the LLVM profile
runtime is not there. But profiles are keyed by function name, so the one
`make pgo` collects from the NATIVE binary applies to the wasm build of the
same sources. That is where the 0.57x → 0.68x comes from.

The flag is conditional (`WEB_PGO`): with no `build/pgo/dorado.profdata`
the flag is omitted rather than passed and failing, so `make web` still
works in a clean tree. **Run `make pgo` before `make web`** for the fast
browser build.

**The deployed page does NOT get this yet, and that is deliberate.**
`.github/workflows/deploy-pages.yml` runs only `make web`. Adding
`make pgo` before it would work locally-shaped but carries a real risk:
the profile would be collected by CI's clang and consumed by emsdk's
clang, and a profile-format version mismatch is an ERROR, not a skip —
it would break the deploy rather than silently fall back. Worth doing,
but it needs a CI run to prove the two clangs agree, not an assumption.

**And the browser has the same pacing cap as SDL.**
`WEB_CYCLES_INTERACTIVE` is 400,000 cycles per frame, which at 60 Hz is
0.39x — now *below* what the wasm core can sustain (0.68x). Boot uses
`WEB_CYCLES_BOOT` (4,000,000) and is already core-limited. Raising the
interactive value to ~1,028,000 would let the core run flat out; left
unchanged for the same reason as SDL's, that it changes how every world
and demo feels, and that is a human's call.

### The profile after Phase 5, and what 1.0x in wasm would take

Re-profiled with the BaseBoard suppressed (Galaxian, PGO build):

| | share |
|---|---|
| `execute_uinstr` | ~44% |
| `dorado_machine_run_until` | ~40% |
| `next_pc` | ~5.7% |
| `dorado_memory_ref_task` | ~3.7% |
| `display_output_b` | ~2.8% |
| `ff_override_b` | ~2.3% |

The 6502 is gone entirely — Phase 5 worked. What remains is **84% pure
interpreter**, and under LTO those top two inline into each other, so
treat them as one number rather than two.

One more raw `getenv` was hiding in there: `display_dispm_present()` said
in its own comment that it was "read only during the display-status probe,
not on every microcycle", but the Alto microcode POLLS that status
register, so it sat under `display_output_b` at ~0.7%. Now cached and
invalidated by `dorado_display_init()` — it cannot be cached
process-wide, because the browser setenv/unsetenvs it and creates a new
machine per world without reloading the module. Worth ~1%, byte-identical.
That is the second instance of this exact trap in `display.c` alone; grep
for raw `getenv` before assuming there is not a third.

**Two more found by looking INSIDE the interpreter (2026-08-05).** Before
starting on predecode, reading the top of `execute_uinstr` turned up the
same pattern as the `run_until` preamble, in the hotter place:

- **Eight global stores per microinstruction, purely for tracing**, at the
  very first line of `execute_uinstr` — including a `dorado_br_get()` call
  into the memory subsystem — preparing `dorado_mem_trace_*` context for
  output that is off. Every reader sits behind a `dorado_trace_flag()`
  check. Gated: **7.3%**, byte-identical. That one change is worth more
  than the entire diagnostic-preamble hoist was.
- **The Alto input seeding re-writes 18 cells per microinstruction**
  (keyboard at three bases = 12, UTILIN 4, mouse 2) with values that change
  only when a human touches the input. `machine_store_va` was ~11% of
  runtime. Storing only when the cell does not already hold the value is
  exactly equivalent and cheaper — a store costs a VA translate plus a
  cache-line invalidation, a read is a cache lookup. **2%**,
  byte-identical.

  It is a READ-BACK, deliberately, not a memo of what we last wrote: if the
  guest consumes or clears one of those cells we must re-present it. The
  "skip if our value is unchanged" shortcut would stop re-presenting a held
  key.

Net: wasm 0.68 → **0.72x**, native Alto steady at 1.30x (PGO had already
been hiding much of the trace-store cost, so the native gain is small and
the wasm gain is not).

**Getting wasm to 1.0x is now an interpreter project, not a flags one.**
wasm sits at 0.68-0.70x and needs 1.45x. With 84% of the time in the
interpreter, that means cutting the interpreter itself by roughly 40% —
there is no longer any peripheral work of that size to remove. The
candidates are Phase 2 (predecode: precompute per-IM-word what to do,
instead of re-deriving FF/LC/ALUFM routing and branch targets every
execution) and dispatch restructuring. Both are real work with real
regression risk, and neither is a flag.

The compiler-flag avenue is exhausted: `-O3 -flto` plus the native PGO
profile is everything emscripten will give. `-O3` alone was worth nothing,
`-mcpu` does not apply, and emscripten cannot collect its own profile.

### Phase 5 DONE: the idle BaseBoard (2026-08-04)

The 6502 ran ONE INSTRUCTION PER DORADO MICROINSTRUCTION — about 62x
faster than the real ~1 MHz part relative to a 16.67 MHz Dorado — for the
whole life of every run, spinning in its idle loop long after boot. It is
now stepped no further once boot is well behind, with the master clock
advanced synthetically at the same 3.70 cycles per microinstruction real
stepping averages, so every cycle constant and gate budget keeps its
meaning.

| | before | after |
|---|---|---|
| Alto (Galaxian), PGO | 1.05x | **1.29x** |
| Cedar desktop, PGO | 0.73x | **0.92x** |

+19.7% on a fair non-PGO A/B (27.94 s → 23.35 s). Byte-identical
framebuffers on all three worlds that exercise different I/O: Galaxian
(Alto/ethernet), the Cedar desktop (PDI/STP) and Lyric.

**Two things make it safe, and both must stay true:**

- **Nothing presses the boot button after boot.** The only presses are the
  scripted three in `machine.c`, all below 3.4 M cycles, and no frontend
  exposes one. A suppressed BaseBoard stops its RIOT timers, so a
  post-boot press would otherwise be silently ignored.
- **A CPReg touch wakes it.** That is the Dorado's only channel to the
  BaseBoard, and `rundiag` drives BaseBoard diagnostics through it, so
  "nobody talks to it after boot" is true of the shipped worlds but must
  not be assumed of every binary in the tree. Any access resets a
  16 M-microinstruction wake window.

`DORADO_BB_ALWAYS_STEP=1` restores unconditional stepping for bisecting,
and is on the config allowlist so using it does not itself distort a
measurement.

**Caveat — this is not byte-identical everywhere.** The synthetic clock
advances at 3.70/microinstruction where real stepping averages *almost*
exactly that, so a fixed `--cycles` budget now covers ~0.03% more
microinstructions (270,396,790 → 270,475,713 on Galaxian). The three
world restores above are unaffected, but `verify-cedar-desktop`, which
drives input and paints the moon, moved 245,677 → 245,594 px: the same
screen, fractionally further along. Anything comparing screens across
this change must expect that, and a gate wanting bit-exactness should
compare at a fixed microinstruction count rather than a fixed cycle
budget.

Diagnostics: kernel, eventCounters, memMisc, IfuSimple and TriconD all
PASS. IfuComplex FAILS — identically, at the same step count and PC, with
`DORADO_BB_ALWAYS_STEP=1`, so it is the pre-existing discrepancy the
handoff already records, not a regression.

### DONE: the germ I/O bridge now polls on a cadence (2026-08-05)

The block that completes the germ's disk and ethernet IOCBs ran on EVERY
microinstruction for the whole life of a Cedar run — five VA reads and
three bridge calls, asking "has the germ posted an IOCB yet?" about
something that changes at device rates. It now polls every
`GERM_POLL_INTERVAL` (64) master cycles.

| Cedar desktop | before | after |
|---|---|---|
| native (PGO) | 0.93x | **1.26x** |
| wasm | 0.62x | **0.80x** |

**Cedar is now faster than the real machine natively**, and 35% / 29%
better respectively — from an 18% profile item, because the reads were
also feeding `dorado_visible_word_at_va`.

64 cycles is ~4 µs of guest latency against a real SA4000's milliseconds;
the germ just spins a little longer in its wait loop. Set
`GERM_POLL_INTERVAL` to 1 for the old behaviour.

**It came out byte-identical**, which I did not expect and had explicitly
warned would not happen. The plain Cedar desktop restore is bit-for-bit
unchanged; `verify-cedar-desktop`, which drives input and paints the moon,
moved 245,594 → 245,635 px. So the timing shift is real but far smaller
than predicted. Do not read the byte-identity as a guarantee for the
COLD-BOOT path, which no gate covers — that remains the risk, and it is
the same gate hole recorded elsewhere in this document.

**Alto, Lisp and Smalltalk are untouched by construction**: the block is
inside `if (m->germ_word_count)`, which is 0 for those worlds, so it never
ran for them. Both verified byte-identical anyway.

### Measured profiles with percentages (2026-08-05)

Two builds each, because on the shipped build the percentages mislead:
`-flto` inlines callees into callers, so `run_until`'s share is mostly the
emulation itself. Use the plain build for RANKING and always confirm with
a timed A/B.

Current speeds: Alto **1.33x** native / **0.74x** wasm; Cedar **0.93x**
native / **0.62x** wasm.

**ALTO (Galaxian) — plain `-O2 -g`, no LTO**

| | share |
|---|---|
| `execute_uinstr` | 27.4% |
| `dorado_machine_run_until` | 16.0% |
| `dorado_visible_word_at_va` | 7.7% |
| `task_schedule` | 6.0% |
| `next_pc` | 5.7% |
| `b_bus` | 3.9% |
| `ifu_id_at` | 3.9% |
| `dorado_ethernet_wakeup_mask` | 3.6% |
| `ff_override_b` / `apply_lc` / `lc_write_address` | 2.4 / 2.2 / 2.2% |
| tail (`stk_apply_post`, `ddc_render_line`, `disk_controller_tick`, `machine_seed_keyboard`, `junk_timer_tick`, `cache_lookup`…) | ~14% |

No single item above 8%. The interpreter core — `execute_uinstr`,
`next_pc`, `b_bus`, `apply_lc`, `lc_write_address`, `ff_override_b` — is
~44% together, and that is what predecode would attack.

**CEDAR desktop — shipped build (the two Cedar-specific items are leaves,
so their shares are trustworthy here)**

| | share |
|---|---|
| `execute_uinstr` | 34.3% |
| `dorado_machine_run_until` | 20.1% |
| **`dorado_visible_word_at_va`** | **13.7%** |
| **`eth_ftp_pick_busy_conn`** | **11.3%** |
| `next_pc` | 6.1% |
| `dorado_memory_ref_task` | 6.0% |
| `machine_germ_complete_ethernet_tx` | 1.8% |
| `machine_germ_complete_disk_iocb` | 1.3% |
| `machine_germ_seed_ethernet_header_page` | 1.2% |

**This confirms the research below, and revises it upward.** The germ
bridge — `visible_word_at_va` 13.7% plus the three germ functions 4.3% —
is **18%**, and the STP connection scan is **11.3%**. Together **~29%**,
where the research estimated 24.5%. Removing all of it would be
1/(1−0.29) = **1.41x**, taking Cedar to ~1.30x native and ~0.87x wasm.

Cedar's plain-build profile also shows `dorado_trace_flag` at 2.3% and
`eth_ftp_maybe_deliver` at 7.1% (the scan's caller), i.e. the same two
items seen from the other side.

### Research: getting Cedar from 0.73x to 1.0x (2026-08-04, no code written)

Cedar needs **1.37x**. Reading the call sites rather than guessing, its
deficit is three concrete things, all of them *polling guest memory at
16 MHz for events that happen at millisecond rates*.

#### 1. The germ I/O bridge polls the guest every microinstruction (~15.5%)

`dorado_machine_run_until` runs this on EVERY microinstruction whenever
`m->germ_word_count` is set — i.e. for the entire life of any Cedar run:

```
machine_germ_complete_disk_iocb(m);
machine_germ_seed_ethernet_header_page(m);
machine_germ_complete_ethernet_tx(m);
```

plus **8 `dorado_visible_word_at_va` calls** inline in the same block. Each
of those is a VA translation. The bridge functions add more before they can
bail: `complete_disk_iocb` reads the IOCB pointer and then its seal word;
`complete_ethernet_tx` reads an IOCB VA and a completion word. So the Cedar
path performs **roughly a dozen guest-memory translations per
microinstruction** — about 145 million per wall-clock second at today's
speed — to ask "has the germ posted an IOCB yet?".

That is the `dorado_visible_word_at_va` 11% and the bridge functions' 4.5%,
and the two are the same phenomenon.

What it is asking about happens at **device** rates. A disk IOCB completes
in milliseconds; the CSB changes when the germ writes it. Three ways to fix
it, cheapest-risk first:

- **(C) Cache the translation, keep the cadence.** The CSB sits at a fixed
  VA (`GERM_ETH_CSB_VA`), so its VA→physical mapping is stable for long
  stretches. Caching the physical pointer leaves the polling *rate*,
  *order* and *behaviour* exactly as they are and only makes each read
  cheaper. Lowest behavioural risk of the three — the machine still looks
  at the same words at the same moments. Needs invalidation when the map
  changes (Pilot does remap), which is the whole difficulty.
- **(A) Poll on a cadence.** Run the bridge every N microinstructions
  instead of every one. **There is in-tree precedent**: `machine_cedar_io`
  already does exactly this with `CEDAR_FIELD_INTERVAL_CYCLES` (277,778)
  and `next_cedar_field_cycle`, and the Lisp input seeding gates itself to
  one pass per 16,384 cycles. Even N=256 removes ~99.6% of the work while
  adding at most 256 cycles — about 15 µs of guest time — of completion
  latency, which is three orders of magnitude below a real disk. The risk
  is not the average case but the boot: the germ's polled IOCB path is
  timing-sensitive, and this is the code that took a long bring-up to get
  right.
- **(B) Trigger on guest writes.** Exact and event-driven: only run the
  bridge when the guest stores into the CSB/IOCB pages. Most invasive, and
  needs a write hook in the memory subsystem.

Recommendation: **A, gated to post-boot, with C as the follow-up.** A is
small, has precedent in this file, and its risk is confined to a window
(boot) that can simply be excluded by keeping the per-cycle rate until
`germ_data_done`.

#### 2. The STP connection scan, every cycle (~9%)

`dorado_ethernet_wakeup_mask` is called per microinstruction from
`run_until`, and reaches `eth_ftp_maybe_deliver` → `eth_ftp_pick_busy_conn`,
which walks **all 16 `ftp_ctx` slots** (`DORADO_FTP_MAX_CONN`) looking for
one with work. Its early-out only fires when a connection is already open
AND busy, so the idle case — the common one — is a full 16-slot scan.

This is a leaf function, so unlike the `run_until` attribution below its 9%
is trustworthy.

The obvious fix is a "some connection has work" hint so the scan is skipped
when nothing is pending. **Two obstacles, both real:**

- The hint cannot become a `dorado_ethernet` member without changing the
  snapshot ABI and invalidating every baked checkpoint (see
  `make verify-snapshot-abi`). It has to be a file-scope static, like the
  keyboard queue and the display-active memo, and reconstructed after a
  restore.
- It must be updated at every site that sets `pending_ack`, `tx_mode`,
  `used` or `open`, and a missed site means a connection that silently
  never gets serviced. This is the subsystem where the EFTP RxOn-clear had
  to be gated to the Cedar path because ungating it stalled the Alto boot
  mid-stream — mistakes here do not show up as crashes, they show up as a
  world that hangs three minutes later.

#### 3. The BaseBoard, as already measured (~7%)

Phase 5 above. Applies to both paths, not just Cedar.

#### Does it add up to 1.0x?

| item | share |
|---|---|
| germ I/O bridge + its VA reads | ~15.5% |
| STP connection scan | ~9% |
| BaseBoard 6502 | ~7% |
| **total** | **~31.5%** |

Removing all of it would give 1/(1−0.315) = **1.46x → 1.07x**. Removing a
more plausible 80% of each gives 1.34x → **0.98x**. So real time is
reachable from these three, with nothing left over — there is no fourth
item of this size waiting behind them.

**Caveats on that arithmetic.** The shares come from ONE profile of a
*restored, largely idle* Cedar desktop; a Cedar that is compiling or
repainting has a different mix, and the germ bridge's share in particular
should fall when the machine is doing real work. The three items are not
independent — cutting the per-cycle poll rate reduces both #1 and the
`visible_word_at_va` cost inside it, so they cannot simply be summed
twice. And every one of these percentages is top-of-stack attribution, which
this session already demonstrated can mislead badly for non-leaf functions;
only #2 and the BaseBoard are leaves. **Treat the table as a ranking, not a
budget, and re-measure by timed A/B after each change.**

### Correction: the profile lied, and here is why

The phase ordering above was derived from `sample` attributing 31% to
`dorado_machine_run_until` and reading that as per-cycle polling overhead.
**That inference was wrong.** Under `-flto` (and more so under PGO)
`dorado_cpu_step` and much of `execute_uinstr` inline *into* `run_until`,
so its self-time is mostly the emulation itself. Hoisting the entire
per-cycle diagnostic preamble out — the change that inference predicted
would be worth ~15% — measured **1.4%**.

**Only A/B measurement counts here.** Profile attribution on an LTO/PGO
build cannot distinguish a caller's own work from its inlined callees.
Every number in this document from Phase 0 onward is a timed A/B with a
`cmp` on the framebuffers; treat any percentage that is not, including the
original table above, as a hypothesis.

A second measurement trap, learned the same way: **do not A/B a code change
against a PGO profile trained on the unmodified binary.** Adding one branch
to the hot loop and re-measuring under the old profile showed the *control*
regressing 15.8 s → 17.0 s, and made a real 7% win look like a 40% loss.
Either retrain per variant, or A/B on `-O3 -flto` without PGO (what the
numbers below do) and apply PGO once at the end.

### Phase 5 is the biggest remaining item, and it is not the smallest job

Running the BaseBoard 6502 at a realistic rate after boot measures **+7.2%**
(9.75 → 10.45 M uinstr/s on `-O3 -flto`, byte-identical framebuffer). It
is the largest single measured item left, not the ~4% the original ranking
guessed — PGO speeds the interpreter far more than it speeds the 6502, so
the BaseBoard's share grew.

The obstacle is real and is the reason this is its own phase: `bb.cycles`
IS the 6502's cycle count, and it is also the master clock every cadence,
budget and gate constant is denominated in. Running the 6502 less often
makes the master clock advance slower, which changes the meaning of every
`--cycles` number in the tree. The shape of the fix:

- a master counter advancing at today's rate (3.70 per microinstruction),
  so every existing constant keeps its meaning bit-for-bit;
- the BaseBoard keeping its own counter, stepped at its real ratio
  (~1 6502 instruction per 62 microinstructions at 1 MHz vs 16.67 MHz);
- the BB still running normally through boot, so the BB↔Dorado CPReg
  handshake is untouched.

**Caveat before anyone starts:** the measurement above *pauses* the BB
after boot rather than slowing it. Those are not the same. A paused BB
stops its RIOT timers, so anything post-boot that depends on them —
notably a boot-button press from the SDL frontend — would silently stop
working, and no current gate would catch it. Slowing rather than pausing
avoids that, but has not been measured.

### Phase 0 result (2026-08-04)

Phase 0 alone hit the target. The Alto path crossed real-time parity with
no emulator code changed, every step byte-identical:

| build | Galaxian 1 B cycles | vs real Dorado |
|---|---|---|
| `-O2 -g` (old default) | 30.9 s | 0.52x |
| `-O3 -g` | 30.1 s | 0.54x |
| `-O3 -g -mcpu=apple-m4` | 30.4 s | 0.53x |
| `-O3 -flto` (**new default**) | 27.1 s | 0.60x |
| `-O3 -flto` + PGO (`make pgo`) | **15.8 s** | **1.02x** |

Findings worth keeping:

- **PGO is the whole story** — 1.7x on top of LTO. An interpreter dispatch
  loop is close to the ideal case: the branch pattern is stable and
  completely opaque to static heuristics.
- **`-mcpu=apple-m4` does not help** (marginally worse than plain `-O3`)
  and would make the build host-specific. Deliberately not used.
- **LTO is worth 11.9%** on its own and costs nothing, so it is the
  unconditional default. PGO is `make pgo` rather than the default: it
  needs a two-stage build and leaves a `.profdata` artifact, and CI does
  not need it.
- The profile is trained on Galaxian. The Cedar path still gained
  (0.39x → 0.50x), so it is not pathologically overfit — but retrain via
  `PGO_TRAIN` if you are tuning for Cedar or Lisp.

Gates: Galaxian byte-identical against the `-O2` baseline at every step,
`make test` 193 assertions, `verify-snapshot-abi` 15/15,
`verify-lisp` 209,145 px.

**Phases 1-5 are consequently not urgent.** The remaining work buys
headroom (Cedar is still at 0.50x, and the browser build gets none of
this), not parity on the Alto path. Re-derive the profile before starting
Phase 1 — the ranking above was measured on an `-O2` build and PGO will
have reshuffled it.

### A cost incurred getting here

`make clean` is `rm -rf build`, and `build/good-packs/` lives inside it —
the directory `dorado/CLAUDE.md` tells you to put expensive artifacts in
*because it persists*. A flag sweep that cleaned between builds destroyed
every local checkpoint. Recovered: everything tracked in
`snapshot-assets/` and `web-assets/` (all shipped state was safe), plus
the Lyric pack, which is disk media and therefore ABI-independent. Lost
and rebaked: the native Lyric snapshot. Not recovered: the several
`cedar-sil-*`, `cedar-clean-*`, `cedar-schematic-*` and `cedar-iago-*`
working images, which were experiment scratch.

`clean` now preserves `good-packs`; `clean-everything` is the explicit
opt-in, and `clean-objs` is what a build loop should use. The instruction
and the tool contradicted each other and the tool was wrong.

Baseline for all comparisons (2026-08-04, M4 Max, `-O2 -g`, clean
environment):

```
./build/dorado --eb worlds/aemu.eb --eftp '../chm/bootfiles/Galaxian.boot!1' \
    --cycles 1000000000 --out /tmp/perf.pgm
270,396,790 microinstructions = 16.22 s of Dorado time in ~35.4 s CPU = 0.46x
```
