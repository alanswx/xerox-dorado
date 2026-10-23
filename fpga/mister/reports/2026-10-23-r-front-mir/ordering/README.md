# R-front and original MIR/CP producer ordering

This is a bounded composition experiment, with no native instantiation or FPGA
speed claim. The candidate retains 786 original physical Qs and the existing
9216 RM/STK RAM bits. The reference combines original Control-A, ProcH and ProcL
wire-list graphs with independent sequential/storage models. All Qs, RAM bits,
67 clock channels and processor outputs are compared after every event.
ASEL.0'a and ASEL.0' remain distinct physical producer boundaries.

The primary test passes both Icarus and Verilator across three illustrative
delay profiles: 4417 events each, 182 write-active samples, 822 read-active samples and
267 old-parity capture-port changes. The reference is not an independent
implementation of every combinational cell function; the inherited cone/cell
functions are shared, while the timed sequential models establish ordering.

The first composition exposed a real interface error. Giving every consumer
the new MIR made H l05/L i15 capture new address data too early (physical Qs
597/607, check91). Giving every consumer the old MIR instead failed settled
source observations at check67. Both alternatives reject at the same checks
in both simulators. The original balanced trace shows the address clocks at
64504ns and MIR Q changing at64505ns. These are illustrative digital delays,
not measured board skew or an ECL setup/hold proof.

The corrected composition uses the old MIR for closing HIGH-root captures and
new MIR for LOW transparent windows. A separate combinational cone observes
settled new MIR/CP/Ready Qs without feeding them into the closing capture.
There is no additional architectural state, RAM copy or pipeline cycle.
Candidate SHA256:
`eb5a25a8b188c90963098d4b2372cad18aeb553a2f90b50483f81e815a06ba83`.

Additional balanced-profile tests pass in both engines:

* Actual CP bus sweep:1537 events,128 captures per CP bank, all16 Ready bits
  change. Primary clock totals were printed before its CP sweep; this extension
  prints totals afterward and requires both128-capture counts explicitly.
* MIR-directed commands:6066 events,32 RBase fills covering all16 entries at
  both extremes, all256 StkP values,2592 reads and320 parity-port epoch changes.
  91 of100 front bits change; this is not a claim that every front transition
  has been exercised in the combined graph.
* Additional CP releases while root LOW:2818 events,256 total captures per bank,
  1370 read-active samples and all16 Ready bits change.

Earlier failures remain in the lossless receipt. Initial declarations before
use failed Icarus elaboration; moving declarations corrected the generator and
trace bench. The first directed bench used CP bus value32 for NoDispatch, but
original j02 wiring maps CPOut.6/value32 to rCT and CPOut.4/value8 to NoDispatch.
That mistakenly overlapped asynchronous task set/reset and failed at check123.
Correcting the bench bus value fixed it without an RTL change. The original
reference manifest also inherited parent package count274 and generator hash;
corrected metadata says296 packages/current generator. Every executed reference
SV file is byte-identical to the corrected generation.

Reproduce a primary case:

```
python3 tools/test_r_front_mir.py \
  --base build/rtl-executive/r-front-ready-qualified-v1-20261020 \
  --output build/rtl-executive/NEW-UNUSED-DIRECTORY \
  --iterations 1024 --producer-random --cp --engine verilator
```

Use `--profile fast_gates`/`later_control`, `--engine icarus`,
`--front-directed`, `--low-cp`, or diagnostic `--post-mir 0`/`1` as appropriate.
Each run freezes sources and records actual compile/test return codes and hashes.
`tools/review_r_front_mir.py` checks the primary matrix, rejected alternatives,
CP extensions and selected directed tests before archiving. The generic receipt
verifier is `python3 tools/archive_r_front_ready_followup.py --verify PATH`.

Still required: broader producer/skew/late-read qualification, actual IM/IFU/
StopTasks/RWTPC/memory-Hold arrival closure, one combined timing fit and native
original-ROM integration. Full60ns/timers,66.7MHz SDRAM physical timing,
native storage firmware windows, three fresh boots and sustained stress remain
open. Qualified5c65/Native59 and the MiSTer session are unchanged.
