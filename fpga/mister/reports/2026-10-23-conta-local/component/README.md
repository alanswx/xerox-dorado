# Original Control-A tasking and branch producers

`dorado_conta_local_step.sv` extends the98-Q Control-A component with six
original d17 MC10176 Qs. StopTasks and RWTPCorRWIM now come from the original
tasking/FF/branch circuit instead of external pins65/1. The component retains
104 physical Qs,23 separate clock channels and51 observable flags. No native
instantiation, reset, initialization, added pipeline cycle or timing fit.
Candidate SHA256:
`e109ac28840e67e0415a04e1f121384e980ad0e8aa4bbbc9de875e6a5212f934`.

The reference adds original a13/a16/a17/a19/b18/b21/e21/d21 plus d17 to the
existing Control-A wire-list graph. It retains all wired-OR drivers and both
physical JCN.2 copies. d21.2 uses d23.B's complemented output, distinct from
d24's normal-data copy. The candidate derives the same branch qualifier from
the physical MIR state. StopTasks feeds back through d17's TaskingIsOff' Q;
TaskingOff/On and MidasOn use the actual FA/FB/FC decoder.

Final primary v12 passes both Icarus and Verilator in all three illustrative
delay profiles:

* 23512 events/profile; all104 Qs,23 clock channels and51 flags compared.
* 518 CP captures,2724 MIR captures,19 task changes, all16 tasks seen.
* Repeated actual MIR TaskingOff/On transitions persist after MIR changes.
  Midas captures/releases; all six d17 Qs change, including retained unclocked
  values of otherwise unobserved package bits;3985 d17 clock captures.
* Actual JCN encodings produce199 ordinary two-clock intervals and66 extended
  six-clock intervals. The obsolete external stretch pin is deliberately driven
  oppositely. This counts original component clocks, not FPGA speed or retirements.
* Eleven damaged RTL variants reject in BOTH engines: the seven retained
  parent controls plus external branch stretch, external StopTasks clock,
  wrong physical JCN copy and missing d17 capture. All56 actions are terminal.

Twelve inherited functions are source-compared with the parent. The two clock/
control transfer functions are byte-identical; the others are identical after
only widening the containing state/pin types. The changed logic is the local
producer decode, its data/clock connections and d17's real capture.

The targeted LOW-root CP probe v13/v14/v15 passes both engines in all three
profiles:1496 events,257 CP captures,165 MIR captures and173 d17 captures. It
clears a just-loaded TaskingOn before the next d17 edge, with a pending higher
task request. The new StopTasks creates a task-clock edge while the flip-flops
capture the OLD task data: root0,stored tasking-off,Switch1,StopTasks1. Then both
CP banks are swept through all128 values with root LOW.

Changing only the task data input to the newly resolved StopTasks value fails
at check215 in BOTH engines/ALL three profiles, while predicting the same clock
events. This is a concrete integration requirement: retain separate pre-event
data and resolved-clock views. Sending the final producer value to every
consumer is a known failing implementation. These deterministic delay profiles
are digital-order checks, not manufacturer-independent skew or full60ns proof.

Thirteen remaining package-half inputs are exact-dependency audited as unused
in this selected graph. Three modified cell functions that connect SWm,
Call13 or WantIfuHold into live preemption/branch/tasking logic reject. The two
other d17 data inputs remain explicit boundaries, not stubs; IM/IFU/memory-Hold
and other unselected producers still require their actual native connections.

Earlier attempts are retained. The first two probes reached the end with
matching state/flags but failed a bench counter check: its new clock22 had not
been included in the counter loop. That was corrected. An initial fixture also
aliased two spare gate inputs onto the new d17 input slots. The final fixture
gives every external producer its own pin and asserts uniqueness; the13-boundary
audit confirms those two spare inputs have no selected consumer. v12 reruns the
full matrix and controls after the correction. A reporting cleanup records
actual negative cases by engine/profile; corrected LOW probes run their epoch
control in every profile. None of these corrections changed candidate RTL.

Reproduce:

```
python3 tools/test_conta_local.py --output NEW-UNUSED-DIRECTORY --mutations
python3 tools/test_conta_local_low_cp.py --output ANOTHER-UNUSED-DIRECTORY \
  --profile balanced --mutations
python3 tools/audit_conta_local_boundaries.py --output THIRD-UNUSED-DIRECTORY
python3 tools/archive_r_front_ready_followup.py --verify \
  fpga/mister/reports/2026-10-23-conta-local/component
```

Next integrate the104-Q producer into the792-Q R-front/processor cone with
explicit data/clock epochs, then close actual MIR/IM/Hold arrivals and shorten
the measured functional stack-RAM/parity path. The previous combined15ns fit
remains **REJECTED** (-2.961ns); there was no refit or speed gain here.

The two existing native firmware runs were extended together from1.5B to5B
host ticks without restart, source/input/epoch changes or guest-state plants.
The original reset source permits two20s disk waits;1.5B ticks at57.142857MHz
is only26.25s of HOST time, with less guest progress under transport holds.
This justifies a larger observation budget, not an attribution of the current
ROM routine or SkipDiskWait setting. The change record, raw ROM source and
64-bit/runtime-refresh limit reader are preserved. Current113 observers still
select epoch1 after actual113; the archived watcher is a running prefix only.
All full-goal native/timing/60ns/66.7MHz/firmware/persistence/stress gates remain.
