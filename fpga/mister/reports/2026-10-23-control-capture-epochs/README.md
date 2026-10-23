# Control capture epochs and complete clock coverage — 2026-10-23

The task capture now uses a consistent **old MIR + old ContB selector** view.
The full corrected component passes the established delay-profile comparisons,
but its first combined timing experiment **fails the15ns budget by8.512ns**.
No FPGA core was assembled or loaded; no speed gain is claimed.

## The counterexample and correction

The earlier component omitted the old late-tasking/LOW-NoDispatch stimulus
while its raw ASEL/BSEL boundaries were converted to real ContB sources.
Restoring that stimulus exposes a failure at check6680 in both simulators.
During a LOW-root CP rMIR reset, `capture_local` combines old Control-A MIR
with new, reset ContB ASEL/BSEL values. Its StopTasks data therefore belongs
to a different instruction epoch.

The diagnostic preserves rejection while allowing the independent reference
to run. The joint controller's complete844-Q result matches that reference;
the child's redundant Switch copies disagree at CA bits40/41 (child0, joint1).
The normal solver invariant correctly rejects this disagreement. The diagnostic
is not a passing gate, and no comparison or invariant was removed.

Candidate `d8c8f7443887d57739e9abc0896ba283ac07b3d86bfea9bf5d849de35024d7d5`
changes capture data to:

```verilog
local_decode(cq, conta_pins(producer_pins(joint_q, pins_after)))
```

`joint_q` is the pre-event Control-A/ContB/MemC state. New selector values
remain available to the actual clock and settled-output views. The reviewer
inversely checks this one expression/comment change against original candidate
`a20b9e52…`: no new Q, reset, RAM copy or architectural cycle.

## Coverage and fixture restoration

Two balanced comparisons retain the previous stimulus exactly:10,121 checks,
242 writes,4,166 reads,391 port epochs. The expanded test adds real late
TaskingOn and LOW NoDispatch maintenance, actual FF072 multiply/Q.14 capture,
all64 values for each of three ContB maintenance codes, and real memory-run
clock release. Across six established profiles and both simulators, the
expanded comparisons pass11,300 checks,238 writes,4,368 reads,392 port epochs.
All844 Qs,9,216 RAM bits,100 clock channels and functional ports remain checked.

The first expanded fixture left h03 rMIR asserted after sweeping maintenance
data63. Its two corrected runs pass state comparisons but miss downstream
I/O clocks53/54, so they are retained as insufficient coverage. The final
fixture restores all three maintenance registers through their own decoder;
no state planting, forced clocks or masked coverage assertion.

The source audit maps all100 channels. In the expanded test:

| Channel | Original source | Observed positive edges |
|---|---|---:|
|43|ProcL l11.B, common Clock0'Bd OR FFeqMul'|32|
|91|ContB ManClk.0' / c04|65|
|92|ContB ManClk.7' / g03|65|
|93|ContB ManClk.1' / h03|65|
|95|MemC l02.B, common MemClkEnable'a|1|

**No clock channel is waived as unused.** MemC l02.B has unwired per-half
clock pin11 but live common clock pin9. Its B data is grounded; Hia (both B
outputs wired together) holds reset. The test releases SetRunRfsh and SetRun
through the real controller stages and observes the common-clock edge while
the unused B Q stays zero. Original models and wire-list identities are frozen.

## Combined15ns timing experiment

Private misterubuntu project:
`/home/alans/mister/r-front-control844-old-capture-15ns-v1-20261023`.
Builder2609365 and map/fit/STA/four-corner reporting all terminate successfully.
This is a **rejected timing result**, not a build success that can be deployed.

The shell keeps the established15.000ns period and2ns/0ns virtual I/O delays,
with no false/multicycle exceptions or register retiming. Launch/result staging
is1,650/1,400 bits before constant/alias optimization. Every state/event/repeat,
invariant, functional port and control flag stays registered; only the existing
`source_pins`/`fa_n` test observations remain unregistered.

- Setup minimum **−8.512ns**, hold minimum **+0.105ns**, pulse-width minimum
  **+6.116ns**, four conditions, zero unconstrained paths.
- 3,923 ALMs,3,261 registers,9,216 MLAB bits, zero M10K bits in this isolated
  component. These are not full-native resource figures.
- Worst launch878 is original state bit72, ContA f14 Switcha. Endpoint
  result339 is functional `faults[0]`. Data delay23.170ns through34 logic levels.
- The path crosses task priority and repeated control-event resolution before
  the processor fault output. This establishes a structural serialization
  problem; a faster PLL or a relaxed timing constraint does not fix it.
- Compiled inputs match preparation; only Quartus's exact version annotation
  was appended to QSF. Assembly is disabled throughout.

A separate task-clock hypothesis holds the prior Switchb clock view during a
normal capture and still responds to asynchronous NoDispatch reset. It changes
no Q/state or settled flag equation. Both balanced full tests pass the same
11,300-check stimulus. Broader mask-specific reset/arrival proof remains open.
Its matched timing project is separate and terminal:
`/home/alans/mister/r-front-control844-tasking-mask-15ns-v1-20261023`, builder2680959.
It reduces the deficit from8.512ns to **4.184ns**:20 logic levels/18.863ns
data delay,4,041 ALMs/3,260 registers, unchanged9,216 MLAB bits. Hold minimum
is+0.154ns and pulse-width minimum+6.115ns, with no unconstrained paths and
only Quartus's version annotation changed. The new worst path is after-pin291
(`CPAddr.0'`) to `state_next[563]`, the high-byte dR input latch. This is still
a rejected15ns result. No speed or native-core Fmax is inferred from either fit.
Do not promote the masking hypothesis based only on balanced comparisons or a
fit: qualify additional actual-Hold/TaskingOff/NoDispatch reset cases and the
full profile matrix, then trace the CP decode/capture path from its real producer.

While the profile matrix was active, its five exact tool hashes were preserved.
The experimental mask CLI/generator text is frozen separately, and a small
runner executes it with the original source lookup locations. The running
matrix's original tools were restored byte-for-byte before subsequent cases.
No simulation was restarted to accommodate a source edit or observer timeout.

## Native integration and ROM scope

Private native stagev9 includes the selector-epoch correction,119 replaced Q
packages/12 RAM packages,828 evaluator Qs plus16 canonical native visible-R
bits. Full native elaboration retains baseline UNOPTFLAT1/MULTIDRIVEN1 with
warnings visible; no new register-file loop. Clocks stay57.142857MHz/SYSPER4.

Corrected original-ROM coldv4 uses stagev9, PID43350/tool92956, its own1B host
limit. It adds passive actual6502 PC/phase counters; a bus address is not an
instruction PC. Coldv3(stagev7) remains independent, PID37871/tool38920.
Recheck current processes/results before relying on these observations.

The older coldv2(stagev1) terminates at250M host ticks:37,059,195 enabled
samples,9,264,799 root edges,211 CP transitions, no alias/unsettled/repeated
monitor failure. This is a bounded original-ROM startup prefix, **not Cedar,
complete Initial, the corrected stagev9, or speed qualification**. Its source,
binary-reuse identity and terminal logs remain preserved. Original5B storage
comparisons and their C protocol/limits were not changed.

## Review and next gates

The immutable `epochs/` receipt preserves the original failures, diagnostic,
insufficient fixture runs, complete profile comparisons, original source audit,
native staging/elaboration, exact15ns fit sources/reports and experimental mask
inputs. `summary.json` states the qualified scope; its content-addressed gzip
manifest can be verified with `tools/archive_r_front_ready_followup.py --verify`.
Lossless readback verifies11,080 members/989 blobs,40,177,417 unique raw bytes
and4,805,584 stored bytes. Both rejected timing results are included.

Reproduce the independent review into fresh destinations:

```sh
python3 tools/review_r_front_control_extended.py \
  --fit build/rtl-executive/r-front-control-old-capture-fit-terminal-v1-20261023 \
  --output build/rtl-executive/control-capture-review-NEW \
  --archive fpga/mister/reports/2026-10-23-control-capture-epochs/NEW
```

Next remove the task/control event feedback with source-bound capture semantics,
prove mask/reset and all producer arrivals, and meet the same15ns budget. The
original early RBase overlap remains unqualified. Native original-ROM/Initial/
Cedar, accurate60ns ordinary cadence, IFU/cache/Map/peripheral clocks/timers,
physical66.7MHz SDRAM, pinned storage/speed gains and full A–F acceptance remain
required. Qualified5c65/Native59/Main/daemon/volumes are preserved. Full FPGA
snapshots remain deferred; the full goal stays active.
