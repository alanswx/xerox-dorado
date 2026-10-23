# Physical operand windows and the current SRAM port

The latest component passes both full balanced original-reference comparisons
and reduces the 15 ns setup deficit to **1.122 ns**. It still fails timing.
No FPGA image was assembled or loaded; hardware speed remains unchanged.

[Reviewed results](windows/summary.json) separate completed profile matrices,
balanced proofs, negative controls, pending runs and all three physical fits.
The content-addressed receipt `windows/manifest.json.gz` preserves executed
sources, independent references, testbenches, commands and terminal readback.
Lossless readback verified 5,176 members / 409 unique blobs / 2,312,507
compressed bytes.
Recreate the review with `tools/review_r_front_windows.py`.

## Structural changes

**One physical task-clock event.** The original ContA f14 tap is joint channel
12 and processor channel 22. Its two Qs are CA bits 40/41, mapped to processor
bits 72/73. The joint controller already computes that actual capture event;
forward it, including repeat detection, through the processor hierarchy. Keep
the old task-data epoch. Remove the duplicate clock reconstruction from newly
captured Switchb/StopTasks. Channel 22 remains checked against the independent
original f14 counter; it is not tied off. Omitting the forwarded event makes
both engines reject event 17 through the retained shared-state/clock checks.

**T uses the same physical latch model as R.** T's 32 input and 16 visible
MC10173 bits retain closed values and expose D combinationally while open.
At closure they capture actual pre-close data. The original embedded T RAM
and parity arrays remain single owners; their writes use physical k01/k02 Q,
rather than a new-control-epoch prediction. Alongside the preceding R-input
work, 80 existing latch bits use closed encoding. Total physical state remains
844 bits, with no new reset, state bit or guest instruction.

T parity/fault exports describe **current physical T Q**. The full functional
assertion is retained after owner commit, and capture/window controls are also
checked before commit. This interface is not an atomic prediction of next state
and that next state's future combinational outputs. All 844 physical Qs, all
9,216 RM/STK bits, all 100 clocks and every functional observation remain
checked. Omitting T closing capture or hiding its open visible Q makes both
engines reject event 7733 on actual T bits, including bit 402.

**SRAM has one current address epoch.** The existing RM/STK banks now use their
current physical address/selection for both reads and writes, with current dRm
Q as write data. Delete the next-address preview and its replica comparisons;
the actual controller and independent state/clock/address/memory assertions
remain. No RAM copy or extra read port is introduced. Corrupting one write
address makes both engines reject event 203 through full RAM comparison.

Every balanced positive comparison reports **11,300 events, 238 writes,
4,368 reads and 392 capture epochs**, with all 100 clock channels exercised.
The preceding read-window matrix and the shared-f14 matrix both completed all
12 cases across six profiles and both simulators. The T-window matrix also
completed all 12. The current-memory matrix remains a separate pending gate;
revalidate its current result before claiming broader qualification.

## Physical measurements

| Experiment | Worst setup slack | ALMs | Shell registers |
| --- | ---: | ---: | ---: |
| Previous R-input windows | −3.557 ns | 3,999 | 3,363 |
| Shared f14 event alone | −4.116 ns | 4,107 | 3,363 |
| Plus T physical windows | −2.414 ns | 4,195 | 3,386 |
| Plus current SRAM write port | **−1.122 ns** | **4,022** | **3,341** |

The shared event alone was a timing regression and was not selected as a
faster native build. The later changes were measured independently. All fits
retain the 15.000 ns constraints, registered shell and 1,400 result fields.
The T status sampling semantics are explicit above; the result vector is not
a simultaneous future-state snapshot. Every fit retains 9,216 MLAB bits and
zero M10K bits, with four operating conditions and zero unconstrained paths.
Exact input readback permits only Quartus's version annotation, after the
builder and all four stage PIDs have exited.

The latest minimum hold is +0.136 ns and pulse width +6.118 ns. Its worst path
is `launch[804]` (after-clock) to `result[291]` (`forwarded_bypass[0]`):
18 logic levels / 15.785 ns through control, MIR, NextMacro, RBase and the
address comparison. The preceding T fit's global worst path instead ended
at an RM MLAB write enable; its restricted evaluator report did not include
that endpoint. The reviewer records global and evaluator paths separately.

## Remaining work

Next inspect the forwarded-bypass/source observation versus the real capture
and consumer epochs. Keep all functional comparisons and actual consumer
timing; do not drop the endpoint or relax the clock. Finish the current-memory
profile matrix at `build/rtl-executive/r-front-current-memory-matrix-v1-20261023`
(driver 74505, tool session 76089). Its tools/imports stay frozen until terminal.

Native integration must distinguish current live D/gates from historical
`r_front_before` and upcoming capture inputs, export physical Q rather than
closed encoding, and retain a single visible-R owner. The packed-view dependency
warning, original early RBase overlap, actual native producers/ROM, 60 ns
phase partition, timers, physical 66.7 MHz SDRAM and full A–F gates remain.
Complete FPGA snapshots remain deferred. Hardware 5c65/Native59, Main/daemon,
volumes and protected Executive artifacts are unchanged. The last pinned
hardware result remains 4.025473 million StartCycle issues/s, not retirements.
