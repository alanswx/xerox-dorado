# Original run/refresh controller contract

**Component qualification only; no native integration or hardware speedup.**

The candidate describes 22 existing Qs: i03's two three-bit single-step/run
pipelines and j04/k01/k02/l02/l03/l04/l05/l06. It propagates the controller's
derived positive clock edges and asynchronous set/reset effects to a stable
event boundary with a bounded combinational evaluator. It adds no state or
reset. The original pre/main clock events remain separately observable.

The oracle is extracted from the original ContA wire list: 22 packages and
181 pin connections. It uses the actual combinational pin-function models and
independent event-driven MC10176/MC10231/SE10231 models. Tied ECL outputs use
explicit ORs. The manifest records every boundary pin binding; unused outputs'
data inputs are supplied documented constants at this component boundary.

Both Icarus and Verilator reproduce exactly:

- 66,632 binary ordered pin events, all 22 Qs and seven clock-event channels.
- Processor/memory enable outputs and wired stop/memory-run decodes.
- Directed run startup, CPU halt with memory enabled by RunRefresh, single-step
  stop/retention and rStop recovery.
- 128 seeded initial states and 32,768 mixed transitions, each split into data
  setup and clock events. Asynchronous reset cases: 33,030; observed CPU-halted,
  refresh-enabled states: 18,477; running states: 30,440.
- Ten deliberately broken memory-enable, refresh priority, memory-stop clearing,
  feedback-clock, pipeline, reset, polarity, settling and stop-OR variants fail
  by behavioral mismatch after compiling.

**Setup is part of the contract.** Binary data/control pins settle before the
root clock changes. The first provisional fixture changed them simultaneously,
exposing an original D/clock path boundary. Its failure log is retained. The
corrected fixture preserves setup; this is not a proof for arbitrary coincident
data/clock changes, X/Z clock values or analog delays. The eventual phase
scheduler must preserve pre-edge values and physical fanout ordering.

The original graph, simulator results and all ten failure controls reproduce
from the extracted archive with fresh compiler outputs and no working RTL
inputs. Proof is `proof/replay.json.gz`. Every raw member is read back and
checked against `archive-manifest.json`; all 66 initial members are retained.
Simulation binaries and compiler object directories are excluded.
The regenerated graph RTL is byte-identical. Only package-dictionary key order
differs in provenance JSON; the replay proof normalizes that ordering and
checks every value and the ordering of all other fields. Both raw copies remain.

Reproduce from a checkout with a fresh output directory:

```
make -C verilog run-control-test RUN_CONTROL_OUT=build/run-control-new
```

From an extracted receipt, run Python on
`qualified/executed-inputs/tools/test_run_control.py --output /absolute/fresh/path --mutations`.
Icarus, Verilator and the C++ build tools must be installed.

Remaining: couple the instruction phases and run controller, qualify Hold/Freeze
and T/task captures, verify downstream memory refresh, then native timing and
original-firmware FPGA qualification. No production Core/Main/daemon changed.
