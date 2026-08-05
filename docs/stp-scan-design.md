# Design: the STP connection scan (`eth_ftp_pick_busy_conn`)

Written 2026-08-05. **Verdict: yes, it can be sped up, and by two
independent routes.** The simple one has a precedent that landed the same
day; the exact one is safe but needs a hook. Recommend the simple one
first.

## What it is

`dorado_ethernet_wakeup_mask()` runs on every microinstruction from
`dorado_machine_run_until`, and reaches
`eth_ftp_maybe_deliver` → `eth_ftp_pick_busy_conn`, which walks **all 16**
`ftp_ctx` slots (`DORADO_FTP_MAX_CONN`) looking for a connection with work:

```c
if (eth->ftp_open && (eth->ftp_pending_ack || eth->ftp_tx_mode != FTP_TX_NONE))
    return;                       /* the ACTIVE one is busy: nothing to pick */
for (int i = 0; i < DORADO_FTP_MAX_CONN; i++) {
    const struct dorado_ftp_ctx *c = &eth->ftp_ctx[i];
    if (eth->ftp_ctx_valid && eth->ftp_ctx_cur == (uint8_t)i) continue;
    if (!c->used || !c->open) continue;
    if (c->pending_ack || c->tx_mode != FTP_TX_NONE) { eth_ftp_ctx_activate(eth, i); return; }
}
```

That early-out only fires when a connection is *already open and busy*, so
the common case — idle — is a full 16-slot walk. `dorado_ftp_ctx` is a
large struct (it carries `cmd_data`), so 16 iterations touch 16 scattered
cache lines to read four bytes from each.

**Cost: 14.3% of Cedar's total runtime** (2026-08-05, shipped build, after
the germ-bridge fix). It is a leaf function, so unlike inlined callers that
share is trustworthy. Removing it is ~1/(1−0.143) = **1.17x**, taking Cedar
to roughly **1.44x native and 0.94x wasm** — the browser within a hair of
real time.

Alto, Lisp and Smalltalk barely touch it; this is a Cedar/STP item.

## Why it is safe to attack: there is exactly one writer

The whole design rests on this, and it was worth checking rather than
assuming:

- `used`, `open`, `pending_ack`, `tx_mode` are written in **one place**,
  `eth_ftp_ctx_store()` (ethernet.c ~2415), which saves the live working
  set into the current slot.
- `eth_ftp_ctx_activate()` stores the current slot, then loads another.
- `eth_ftp_ctx_alloc()` only *chooses* an index; it writes nothing. A
  recycled slot keeps its old flags until stored over — the existing scan
  reads those same stale flags, so any summary that mirrors the fields is
  equivalent to today by construction.
- `dorado_ethernet_init()` `memset`s the struct.
- A snapshot restore replaces the array wholesale.

Only the last two are outside `eth_ftp_ctx_store`, and both are single,
identifiable points.

---

## Option A (recommended): poll on a cadence

Run the scan every N master cycles instead of every one, exactly as the
germ I/O bridge now does.

```
static uint64_t eth_ftp_pick_last;      /* file-scope, reset in init */
if (cycles - eth_ftp_pick_last >= FTP_PICK_INTERVAL) { eth_ftp_pick_last = cycles; ... }
```

**Why this is the right first move**

- **No derived state, so no staleness class at all.** The failure mode that
  makes Option B delicate simply does not exist here.
- **The delay is not guest-visible latency.** This is the *server's* choice
  of which connection to service next. Deferring it by 64 cycles (~4 µs of
  guest time) is nothing against BSP round trips measured in milliseconds,
  and the client is not waiting on the switch itself.
- **The precedent is one day old and byte-identical.** The germ bridge got
  the same treatment (`GERM_POLL_INTERVAL 64`) and the Cedar desktop restore
  came out bit-for-bit unchanged.
- Five lines, one constant, trivially revertible by setting the interval
  to 1.

**Risks, honestly**

- It shifts *when* a connection switch happens. The germ change turned out
  byte-identical, but that is evidence, not proof, and this is the
  subsystem where the EFTP RxOn-clear had to be gated to Cedar because
  ungating it stalled the Alto boot mid-stream.
- **The cold-boot path is the real exposure**, and no gate covers it. A
  cold Cedar boot streams the whole release over STP with several
  concurrent connections — precisely the case where switching promptly
  might matter. Test a cold boot by hand before believing it.
- N is a tuning knob against a workload nobody has profiled under load: a
  busy multi-connection transfer may want a smaller N than an idle desktop.
  Start at 64, measure a Bringover, and do not assume the desktop number
  generalises.

## Option B: an exact busy-set summary

Keep a 16-bit mask, bit *i* = `used && open && (pending_ack || tx_mode != NONE)`.
`eth_ftp_pick_busy_conn` early-outs when `mask & ~(1u << cur)` is zero.

Maintenance is genuinely small, because of the single writer:

| point | action |
|---|---|
| `eth_ftp_ctx_store()` | recompute the bit for `ftp_ctx_cur` |
| `dorado_ethernet_init()` | mask = 0 |
| **after a snapshot restore** | **rebuild by scanning all 16 slots once** |

**The whole risk is that third row.** The mask must be a file-scope static
— a new `dorado_ethernet` member changes the snapshot ABI and every baked
checkpoint fails to restore — so it is derived state shadowing snapshotted
state. Miss the rebuild and a busy connection is never serviced: not a
crash, a world that hangs minutes later, which is the worst failure shape
this codebase produces.

Two things make that tolerable if Option A proves insufficient:

- **One explicit hook.** `dorado_machine_restore` is a single function and
  already performs post-restore fixups (it re-applies the FTP source).
  Rebuilding the mask there is one line at a known point.
- **A paranoid self-check.** Under a `DORADO_*` flag, recompute the mask
  from the array on every call and compare, aborting on mismatch. That
  turns "someone added a second writer in 2027" from a hang into a test
  failure. Run the gate set once with it on.

A third possibility — putting the mask *in* `dorado_ethernet` so the
snapshot carries it — removes the staleness class entirely and is the
cleanest engineering answer, but it breaks the snapshot ABI and forces a
rebake of every native and wasm checkpoint. Not worth it for 14%; worth
reconsidering if several such summaries ever accumulate.

## Recommendation

**Do Option A.** It is smaller, has no staleness class, and has a
same-day precedent that came out byte-identical. Gate it on Galaxian, the
Cedar desktop, Lyric, `verify-cedar-desktop` and `make test`, and **test a
cold Cedar boot by hand**, since no gate does.

If the cadence turns out to cost responsiveness under a real multi-connection
load (watch a `Bringover`, not the idle desktop), fall back to Option B with
the restore hook and the paranoid check.

Expected: Cedar ~1.44x native, ~0.94x wasm. That would leave the interpreter
core as the only thing between the browser and real time.
