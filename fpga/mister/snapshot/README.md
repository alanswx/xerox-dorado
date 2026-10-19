# Complete FPGA snapshots: RAM access prototype

Complete machine save/restore is required milestone4a in
[the delivery plan](../../../docs/verilog-clock-plan.md). It is not implemented
in the running MiSTer core. This directory qualifies one bounded RAM-bank
access method before extending it to the actual board hierarchy.

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

The remaining delivery order is:

1. Classify the generated inventory and prove bounded access on an actual
   board, including its real clock strobes and RAM read pipelines. Measure
   the added logic and routed timing before extending the access fabric.
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
