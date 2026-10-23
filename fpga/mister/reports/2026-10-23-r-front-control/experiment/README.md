# Processor/control/Hold composition

Status: **INCOMPLETE_PROCESSOR_CONTROL_HOLD_COMPOSITION**. This joins the
processor/R-front slice and actual Control-A, Control-B and MemC Hold sources.
It is not a native core, a physical timing qualification or a speed result.

The original 798 processor/control Qs retain their map. Forty ContB Qs append
at837:798 and six MemC Qs at843:838. All844 Qs,9216 RM/STK bits,100 clock
channels,94 control flags and existing functional ports compare against
independently delayed original circuits after each test event. This remains
a processor slice: ALU/IFU/cache/Map and other native families are not supplied
by this composition.

The new reference includes one original BaseBd j02/l01/k01 fanout. Its l01
outputs supply CA/CB/ProcH/ProcL; k01 supplies memory. The memory's distinct
Hold/CBHold/PRhold outputs feed their respective consumers. The processor
PRhold -> f19.13 -> f19.14 Hold' -> e19.10 -> e19.13 Holda path is original
on both halves. The old ProcH boundary omitted e19; it is now present with
all unused package-half inputs explicit. Three additional boundaries are
MemBase.2', MemBase.3' and alu.00. No architectural Q or RAM copy is added.

Actual ContB ASEL copies, BSEL and LC replace raw processor inputs. Shared CP
address/strobe/data, wired-OR reset, memory enables/MemSH, StartCycle, BNT,
UseDMD and StopMIRClk stay coupled through the joint control evaluator.
The explicit RunRefresh arrival contract still requires a native producer.
Closing processor captures use pre-edge fields; settled/LOW views use new
fields. An unused child shadow of memory Q14/Q15 is excluded from the
redundant shared-processor check: actual memory Q14/Q15 come from the joint
evaluator and remain included in the full844-Q comparison.

## Terminal evidence

| Case | Icarus | Verilator | Scope |
|---|---|---|---|
| Initial pilot | pass | pass | 2112 events, zero writes: insufficient write coverage |
| Balanced, actual LC instructions | pass | pass | 4168 events,242 writes,1542 reads,74 old/new port epochs |
| New instruction fields at closing capture | reject131 | reject131 | Physical Q38/Q54 differ |
| Required early phase | fail1456 | fail1456 | RBase word1/Q644..647 differs |
| Early phase, arrival trace | — | same failure | Exact write/address order below |
| Change only phase FF1250->2000ps | — | digital pass | Causal diagnostic, no physical timing qualification |

Balanced clocks/counts agree between engines. Five channels (43,91,92,93,95)
remain unexercised; do not claim complete clock coverage. The new LC cases
use actual ContB instruction inputs and produce156 writes/282 reads before
random instruction/Hold traffic and a shared CP sweep. The old directed
RBase/StkP/tasking tests drove raw ASEL/BSEL/LC and are not qualified by this
pilot; they must be converted to actual-source stimulus.

## RBase write-window discrepancy

At event1456 the original ProcL a19 MC10158 changes LastNext address0->1 at
1020108.250ns. i18 SE10212 closes RBaseWrite' at1020108.750ns. Data is F,
and the zero-delay level-written F10145A oracle writes word1 during that
0.5ns overlap. The candidate closes the window without that additional write.
All other840 physical bits agree at the failure.

Changing only the phase-register delay1250->2000ps closes Write Enable at
1020108.750ns, then changes the address at1020109.000ns. The extra digital
write disappears and the full pilot compares. This does not waive the early
failure or establish physical correctness of the later profile.

The original [1979 Fairchild Bipolar Memory book](https://ftpmirror.your.org/pub/misc/bitsavers/pdf/fairchild/_dataBooks/1979_Fairchild_Bipolar_Memory.pdf)
contains the F10145A specification at printed7-20..7-24 (PDF87..91). Its
AC table/diagram list a4ns minimum write pulse,3.5ns address setup before
write assertion and1ns address hold after deassertion at the specified limit.
The digital oracle does not enforce those limits. The observed overlap and
the later0.25ns margin do not qualify physical RAM operation; neither trace
proves that an actual chip writes an extra task entry. The downloaded PDF's
hash and precise source pages are recorded in `summary.json`.

Next: derive the native write-close and address-switch subphases with real
setup/hold margin. Bind them to HM §2.3 and Table4 (RBase data/control at t2,
per-task file written at t3), retaining30ns clocks/60ns ordinary starts and
overlapping execution. Check room for RBase/RM reads before assigning a
15ns fabric phase; do not simply delay every phase FF or add a guest cycle.
Then expand actual-source directed/missing-clock coverage, close remaining
IM/parity/MCR/hold-data/DMD/B-bus producers and run full native timing/ROM gates.

## Reproduction and preservation

```
python3 tools/generate_r_front_control_reference.py --output /tmp/dorado-joint-reference
python3 tools/test_r_front_control.py \
  --base build/rtl-executive/r-front-ready-qualified-v1-20261020 \
  --reference /tmp/dorado-joint-reference \
  --output build/rtl-executive/dorado-joint-fresh --engine verilator
```

Use `--profile early_refresh --trace-check 1456` to
reproduce the failed stress trace, or `--mutation new_fields` for the wrong
capture epoch. The review command is `tools/review_r_front_control.py --output
/tmp/dorado-joint-review`. The lossless receipt contains2885 members/358 blobs/
1642282 compressed blob bytes. Both zero-write predecessors, both required
failures, both wrong-field variants and both causal traces are retained with
frozen compiled inputs/adapters/cone/timing/testbench/logs. Maintained RTL,
constraints, hardware, Main/daemon and disk volumes are unchanged.
