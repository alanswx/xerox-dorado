# MIR, CP and Ready coupled component

`dorado_mir_ready_step.sv` couples 50 original physical Qs: 16 Ready, 12 CP and
22 MIR. Actual i02 ClrReady and j02 rMIR feed their asynchronous consumers.
ContA a20/b16/f24 provide Notify; c22 provides preemption from Block', Hold and
SwitchUp; c19 provides the original MIR clock fanout. Both d23 JCN.2or3 drivers
and the independent BNT/BNT' storage inputs remain. There is no new reset,
initial state, RAM copy or Dorado pipeline stage. Native RTL is unchanged.

One extracted original Control-A graph is the reference. Its gates use the
existing cell functions; register storage is independently event-driven.
Three illustrative gate/FF profiles (1/1, 0.5/1 and 1/2 ns) agree in Icarus and
Verilator: 22,397 events per run, 15 Notify wakeups, 4,111 shared-clock old-MIR
epoch checks, 257 real CP captures and 15 held-MIR exercises. The workload also
covers all FF bytes/low JCN combinations, separate d23/d24 startup Qs and all
256 independent positive/negative BNT pairs. Six actual damaged candidates
reject in BOTH engines: new MIR epoch at Ready capture, merged JCN storage,
missing CP MIR reset, old CP clear, missing Hold in preemption and merged BNT.

The first test attempt matched the reference then failed a bench assertion:
after advancing its state the bench inspected a recomputed next-state output,
which proposed applying the same clock event a second time. The corrected bench
captures the compared flags BEFORE advancing. Candidate RTL did not change.
The failed source, compile and FATAL log are preserved beside the passing v2.

Eight extra boundary signals enter unrelated halves of selected physical gates.
An exact pin-function dependency audit proves every affected output has no
selected consumer or observed clock/flag. Two changed cell functions that make
SWm or Link decode feed live preemption/Notify reject. This is not permission to
remove those producers from the full machine; the original native logic stays.

A single isolated Quartus 17.0.2 fit on misterubuntu passes all four conditions
at 15.000 ns: minimum setup +5.521 ns, hold +0.149 ns, pulse width +6.548 ns.
The launch-to-result setup margin is at least +9.083 ns. Resources including
the registered shell: 247 ALMs / 395 registers; no RAM, DSP or PLL. All 232 nominal
launch bits and 109 nominal result bits are wired in the frozen shell; synthesis
can merge/prune duplicates. Input staging is measurement infrastructure, not
an added architectural cycle. Virtual I/O retains 2 ns max / 0 ns min delays;
no timing exceptions, clock relaxation or FPGA assembly. All five recorded
build processes are absent; only Quartus's version annotation changed.

The independent reader checks exact fixture wiring, every qualified RTL
dependency, all corners, targeted evaluator paths and unconstrained I/O. Eleven
damaged/rehashed controls reject, including each child and a zeroed state result.
The earlier CP/Ready timing review still passes unchanged (+5.640/+0.155 ns).
Complete prepared/collected sources, raw reports, controls and review tools are
retained. This measures a component budget, not a complete 60 ns Dorado.

The setup contract requires CP transactions/data to settle separately before
phase-clock edges. IM arrivals, complete controller/arbiter feedback, memory
Hold/IFU/processor producers, independent analog skew and native timing/firmware
integration remain OPEN. Next connect this producer slice to the actual
clock/task evaluator and retain its pre-event capture ordering. Do not select
a faster native core from this isolated result.

The watcher appendix is a live prefix from existing native simulations2284/
2331 and watcher7144. Both initialized; the queue run has reached its first
normal button interval, with no CPU/113 window yet. No limit, stimulus or run
was changed. Hardware remains5c65/Main13244/daemon13243: cold native persistence/
input PASS, HDMI OPEN, measured speed unchanged. The full goal remains active.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-23-mir-ready/component`.
