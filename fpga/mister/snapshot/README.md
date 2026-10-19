# Complete FPGA snapshots: board state access experiments

Complete machine save/restore is required milestone4a in
[the delivery plan](../../../docs/verilog-clock-plan.md). It is not implemented
in the running MiSTer core. This directory now qualifies bounded RAM access
and complete modeled ContB state restore in isolated RTL/routed experiments.
Machine-wide state access, coherent barriers and physical restore remain open.

Update2026-10-19: the qualified ContB host-clock/exact-advance adapters are
now in the full Native51 core on MiSTer14. Four-condition whole-core/crossing
timing, original firmware, Cedar login and all34 base transfers pass. ALL
snapshot admin inputs remain inactive; the running core cannot save/load
state.51 full Viewers/mouse/exact keyboard and warm marker now pass. The
slower-loader/replay investigation remains open;49 is the performance baseline. Evidence:
reports/2026-10-19-native51-contb and native51-field. Historical isolated
experiments below did not themselves change production RTL.

`f10415_bank.sv` models an IM-like1024x34 bank from34 original F10415A slices.
It exports1024 RAM words plus a separate34-bit read-pipeline word. The RAM
read always clocks. During scanning, a saved register supplies the guest's
previous read output. On resume, the first ordinary edge performs the normal
read/write; no guest warm-up edge is inserted. The caller must hold every
other guest clock/state at the same boundary. That system barrier is not
provided by this prototype. Do not wire this into production as a complete
pause implementation.

The requester asserts `freeze` after holding guest state, waits for `frozen`,
then issues requests. Each accepted request produces `done` and its read data
after one clock; write-response data is unspecified. Word0..1023 accesses RAM,
word1024 accesses the saved pipeline, and every larger11-bit index returns
`error` without writing. Requests during entry or resume are not accepted.
Transfers use complete34-bit words, keeping write enables shared so Quartus
can pack the slices. A future byte/DDR serializer must assemble a complete
word before committing it. This is a local bank interface, not a snapshot
file format or the final host protocol.

Run `python3 fpga/mister/snapshot/test_f10415.py --mutations` on the Mac.
The test uses34 unchanged original cells with a separately controlled reference
clock, plus an independent array oracle. It checks every exported/restored
bit, a saved read output deliberately different from its current RAM word,
every invalid index, destructive continuation and first-edge resume, and
30000 random running steps. Four broken variants must fail: lost pipeline,
scan output leakage, running access and invalid-address aliasing.

`fit_f10415.py --output NEW_DIRECTORY` runs isolated matched projects on
misterubuntu with Quartus17.0.2, the MiSTer5CSEBA6U23I7 device,50MHz and
virtual data pins with2ns input/output budgets. It does not program a board.
`audit_f10415.py DIRECTORY` checks source hashes, memory geometry/packing,
OLD_DATA mixed-port collision configuration, all four operating conditions
and unconstrained-path reports. Recovery/removal have no applicable paths
in this synchronous bank.

The qualified whole-word fit retains34816 RAM bits/fourM10Ks in both variants.
Baseline101 ALMs becomes163, with43 additional registers. All12 setup/hold/
pulse-width checks per variant pass across four conditions; snapshot minimum
slack is+0.096ns. These are isolated virtual-I/O measurements, not a full-board
resource or timing budget. Two rejected experiments show why inference must
be checked: separate conditional write addresses expanded the bank into
logic, and independent per-bit restore enables used34 M10Ks. The production
original cell is unchanged; the prototype additionally requests defined
old-data collision behavior instead of its `no_rw_check` attribute.

Next: prove grouping/access on an actual board with its real strobes and
clock-history registers, then extend the inventory and barrier to external
SDRAM, all devices, input and ARM disk/local network-server state. A complete
bundle must pair a matching disk generation, validate schema/build/hashes
before changing live state, and pass actual Cedar desktop restore tests.
No physical snapshot, fast-boot timing or cross-build compatibility is claimed.

Actual ContB grouping is now checked by `audit_contb_im.py` and
`test_contb_im.py`. The physical IM is4096x36:34 instruction bits and2 stored
parity bits,144 original RAM packages and144 registered read bits. Following
actual buffered address/WE/CE expressions proves eight18-bit groups, two
independently written halves per bank. This is the grouping for the next
board-access experiment; the qualified34-bit bank above remains an isolated
prototype. Six damaged-board controls reject; a common inversion of all
address copies is explicitly outside a grouping proof's coverage. Evidence:
reports/2026-10-19-snapshot-contb. Neither production RAM nor clocks change.

The next18-bit prototype is `f10415_enable_bank.sv`. It uses a single host
clock with the actual guest advance as a read/write enable. This preserves
ordinary backend holds while permitting frozen scans; the saved read output
survives release until the first real guest edge. Original-cell/array tests
and five broken variants pass. Matched registered-shell fits retain twoM10Ks
and OLD_DATA, with all four-condition checks passing, minimum+0.255ns.
Fixture counts are53->102ALMs/48->138registers and include launch/capture
registers; do not extrapolate that delta to a complete core. The two-clock
prototype also in this directory is REJECTED by Quartus inference. Earlier
direct-virtual-input timing failures are preserved. Evidence and full scope:
reports/2026-10-19-snapshot-enable. No physical save/load or complete snapshot
is implemented by either prototype.

`generate_contb.py` now creates a separate actual ContB experiment with the
original144 RAM slices replaced by the eight enabled half-banks; all other
board logic stays unchanged. `test_contb_snapshot.py --mutations` compares
all71 outputs against the original board, actual write/CE/address/parity cones
and raw read pipelines, plus an independent array.97,418 host cycles and
57,392 state operations pass, including late clock-enable changes, every
RAM/pipeline bit, invalid rows, cross-bank responses and first-edge resume.
Four damaged RTL variants reject. The matching isolated board fit retains
16M10Ks/147456bits; all four-condition checks pass min+0.168ns, no cuts or
unconstrained paths. Shell counts287->600ALMs/325->584regs include admin
pruning differences; do not project to the core. Eight audit controls reject.
Evidence: reports/2026-10-19-snapshot-contb-board. Production is unchanged.

The subsequent `generate_contb_full.py` experiment adds all209 remaining
MC10176/MC10231 q, pre-capture and clock-history bits. The complete modeled
board now passes save/candidate-only continuation/restore/first-edge execution
against an untouched original board. All71 outputs and147809 modeled state
bits are covered; all209 FF bits are individually walked, all invalid15-bit
addresses are rejected and six broken RTL controls fail. Fresh original-board
XML classification accounts for every candidate; seven missing/overlapping/
aliased-state controls reject. External inputs and clock phase are outside
this board boundary and must be captured by the caller.

Matched complete-board isolated fits retain16M10Ks/147456bits/OLD_DATA and
pass all12 checks per variant/four conditions, minimum+0.165ns. No cuts or
unconstrained paths. Shell counts288->774ALMs/320->693regs (+486/+373) include
admin pruning differences, not whole-core overhead. Nine damaged-evidence
controls reject. Sources: generate_contb_full.py, test_contb_full.py/.cpp,
classify_contb_full.py, fit_contb_full.py and audit_contb_full.py. Evidence:
reports/2026-10-19-snapshot-contb-full. Production/live FPGA remain unchanged.
Next integrate access in the complete-machine experiment, expand/classify the
other boards and wrapper/device/input scope, establish the global services
barrier and qualify whole-core timing before physical Cedar restore.

The remaining delivery order is:

1. Classify the generated inventory and prove bounded access on an actual
   machine. Complete modeled ContB access/continue/restore and isolated timing
   now pass; other boards, wrapper/devices/inputs and whole-core timing remain
   required. Inventory/classify every remaining state path before release.
2. Implement one machine-wide pause boundary with ARM disk/network services.
   Preserve or drain pending operations explicitly; keep SDRAM refresh running.
3. Save all classified RTL, RAM, device, clock and input state together with
   matching disk contents/generation and local Pup/BSP/STP server state.
   Validate the complete versioned, checksummed bundle before restoring it.
4. Qualify same-build save/continue/restore, including pending I/O and rejected
   corrupt/mismatched bundles. Restore a real FPGA Cedar desktop, verify input,
   transfers and subsequent disk writes, and measure time against a cold boot.

The intended user operation is Save/Load machine state. It remains a required
part of release acceptance alongside fresh original-firmware cold boots.

The next primitive, `generate_f10145.py`, now preserves each16x4 file's64 RAM
bits and all10 pending-write-window bits. `test_f10145_snapshot.py --mutations`
checks the unchanged original and an independent oracle, including freeze
inside WE low, candidate-only continuation and exact first-edge commit.
All74 bits/invalid indices/backend holds pass; eight damaged RTL controls fail.
`classify_f10145.py` accounts for original XML candidates, explicitly excluding
106 synthesis-translate-off diagnostic bits with source guards and11 failure
controls. Raw native inventory counts remain review candidates, not physical
snapshot size. Matched isolated fits pass four conditions/min+0.146ns, no cuts
or unconstrained paths, zero block RAM.57->117ALMs/89->142registers include
registered shells/pruning; no core budget or hardware save/load claim.
Evidence: reports/2026-10-19-snapshot-f10145. Next integrate/classify remaining
ContA FF/latch families and six clock-history bits, then global machine state.

Full frozen49 ContA now passes modeled board restore in
`generate_conta_full.py`/`test_conta_full.py`: all1,353 non-diagnostic modeled
bits and138 outputs, including75 FF packages, nine register files with write
windows and six phase-history bits.59,787 host edges,13,334 late-enable changes,
all bits/invalid addresses and first-edge resume pass; seven damaged RTL
variants fail. Fresh XML classification excludes954 original simulation-only
diagnostic bits and rejects17 altered inventory/layout/source cases. The
modeled bit count is not a fitted baseline register count. Matched isolated
fits pass four conditions/min+0.163ns, no cuts/unconstrained paths, zero block
RAM.769->1974ALMs/1343->1951regs include shell/pruning. Evidence:
reports/2026-10-19-snapshot-conta-full. No production/live FPGA change yet.
Next whole-core ContA+ContB clock-enable integration and original boot, then
remaining board/device/ARM access and complete safe machine restore.
