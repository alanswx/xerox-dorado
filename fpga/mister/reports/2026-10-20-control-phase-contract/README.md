# Original ContA instruction-phase contract

**Component qualification only. No native integration, faster clock or hardware
speedup.** Full FPGA snapshots are deferred; this is the original clock work.

`dorado_control_phase_next.sv` computes the next values of seven existing phase
flip-flops: pre-start (k03.B), StartCycle (k04.B), Phase0/1 (l07.B/A), Phase2/3
(k05.B/A), and Phase4 (j01.B). It keeps separate pre/main capture events and
original sPhase0 set/reset priority. It adds no pipeline stage or reset source.

The original-wire-list check covers 13 packages, 74 pins, 30 unwired/pulldown
inputs and unique drivers. Clock-pin Boolean levels reduce to
`CLK.ca' | Stop | Run'` through distinct fanout paths. This does not establish
physical arrival times or authorize collapsing those paths. Upstream Run/Stop,
Hold, Freeze, RunRefresh, SC&NoFreeze and CRamClock remain unqualified.

The reference uses the actual MC10117/MC10104 combinational cells and independent
positive-edge MC10231 flip-flops with original set/reset priority. A fixture-only
load seeds oracle state; the candidate has no administrative state-load port.

- Icarus: 546,739 comparisons, including 524,288 exhaustive four-state transitions.
- Verilator: 22,451 comparisons. Both include 2,048 known-state cases and 20,000
  mixed closed-loop clock/init/decode events.
- Manual cadence: 99 ordinary two-clock intervals and 32 IM/TPC six-clock
  intervals per simulator, independently measured from the running sequence.
- Eight bad extension, cascade, merged-clock, initialization, four-state,
  phase-toggle and hold variants fail by behavioral mismatch, after compiling.

All nine executed source/dependency files match the current implementation.
Archive extraction followed by a fresh rebuild reproduces the original wiring
checks, both simulator results and all eight expected failures. The replay used
the extracted source tree, not working HDL. Proof is `proof/replay.json.gz`.
Every raw member is losslessly compressed and indexed in `archive-manifest.json`;
all 47 initial members are retained, including provisional failure logs. No object
files, simulation binaries or compiler build directories are archived.

The first provisional test reached the cadence coverage check before its D
decode settled. Waiting for the combinational decode fixes that test query;
the final qualified fixture contains this delay. Earlier logs are history only.

From a repository checkout, choose a fresh output directory:

```
make -C verilog control-phases-test CONTROL_PHASE_OUT=build/control-phase-new
```

To replay an extracted receipt, run
`qualified/executed-inputs/tools/test_control_phases.py --output /absolute/fresh/path --mutations`
with Python 3. Icarus, Verilator and the C++ toolchain must be installed.

Next: qualify upstream Run/Hold/Freeze/RunRefresh and processor T-clock event
ordering before integrating an enable-based phase chain into Native59.
