# Actual ContA l17 feedback passes original-graph ordering

`dorado_r_front_l17_step.sv` closes the original ContA l17 -> d17 path in
the coupled processor model. The six added state bits belong to the physical
MC10176 package; this adds no architectural pipeline stage or reset. The
798-Q map retains every old bit and appends l17 at global Q797:792, corresponding
to Control-A Q109:104. Its original clk1'Da is appended as clock channel68;
the 68 previous clock indices retain their meaning.

The independent reference adds original l17, j19 clock fanout, g03 feedback
and e01 RBMux gates. `clk1'Da` follows f13/j19 and preFH', the complement of
physical l06.Q0 (Control-A Q20). Its Boolean mapping is
`base | q[6] | !q[7] | !q[20]`, distinct from Clock0's Q25 term. Original FF
UseDMD/B_Dbuf decoding, d17 recirculation and RBMux.04 data are retained.
TrueAC is the original g05.13 inverter output with both inputs unwired LOW,
not an invented non-rail constant. Old external slots297/299/300/308/312 cease
to be producers in this graph. The actual B-bus source BMux.04 remains a live
boundary at317; closing its processor producer remains future work.

Every run compares all798 Qs,9216 RAM bits,69 clocks and functional ports.
All six profiles pass in Icarus and Verilator: balanced, fast gates, later
control, parity minimum, parity maximum, and9.9ns RAM reads. Each run has
13932 events,328 write-active events,5800 read-active events and797 old-parity
epochs. All69 clocks fire and engine counts match. The directed four-command/
two-B-value sequence changes all six l17 bits. Existing RBase32/StkP256,
eight IM/TPC commands,120 two-/six-clock intervals,16 WriteIM phases, late
LOW task/branch/NoDispatch, RAM nonoverlap and old capture epochs remain.

Both deliberately broken variants reject after successful compilation in
both engines, at check17:

- Old external l17 feedback corrupts actual d17 Q3/globalQ789.
- Using Clock0 for l17 captures its original physical Qs at the wrong edge.

Two additional complete LOW-B comparisons pass14071 events each,328 write/
5864 read events and797 epochs. Both directions verify that B changing during
LOW clk1 leaves l17 stored, the next clk1 captures the current B value, and
the value then traverses the actual d17 feedback. These are separate additional
probes, not twelve additional profile runs. In total36 compile/test actions
are terminal:14 positive and4 expected failing graph executions.

The boundary audit follows16 unused inputs through all selected gate functions.
BMux.04 is separately rejected as unused at both its observation and actual
l17 input. Two malformed live dependencies and one newly exported B bus also
reject. Twenty clock/feedback pins, unwired LOW inputs and the original TrueAC
source are checked. The old command audit still passes12 inputs/five controls.
Unused package halves in this selected graph remain live elsewhere in the
full Dorado; nothing here permits tying them off globally.

The first aggregation is preserved as FAILED: the parent driver cleared the
mutation-name namespace after generating an actual mutant. The reused negative
run's compiled bytes and circuit failure were correct. A separate read-only
review validates exact compiled source hashes, all frozen input/timed-source
hashes, all32 original actions and complete profile/engine counts, yielding
12 passes/four expected rejects. It overwrites nothing and restarts no test.
The driver now retains mutation metadata, and future reused-run classification
checks actual executed hashes. An earlier audit-control guard attempt is also
preserved; its correction removes both observations only in that diagnostic.

The MIR-mask source transform was extracted into a pure generator. Both the
qualified good and frozen-bad SV bytes reproduce identically. The l17 recipe
now generates its maintained source entirely from maintained command RTL;
it no longer needs a generated parent under build/. Candidate SHA256:

```
665188a29d07ba22981061233a825f496a479d577f5a6794a6b81cfac8c418d2
```

This lossless receipt has4869 members in385 blobs,1,748,024 compressed bytes.
Verify with:

```
python3 tools/archive_r_front_ready_followup.py --verify \
  fpga/mister/reports/2026-10-23-r-front-l17/ordering
```

No new15ns fit or native instantiation/load occurred. The preceding792-Q
visible-R fit's0.382ns shortfall remains historical evidence for that circuit,
not a timing result for this798-Q version. Next close actual ContB IM/ASEL/
BSEL/StopMIRClk and MemC Hold/PRhold and the remaining B-bus producer, then
continue capture-stage/native original-ROM qualification. Original60ns clocks,
timers, physical66.7MHz SDRAM crossings/IO, Cedar, rollback/persistence, fresh
boots and sustained stress remain open. Hardware5c65/Main/daemon/volumes and
the native2284/2331 simulations remain unchanged; no hardware speedup is claimed.
