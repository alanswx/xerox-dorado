# Complete actual ContA with serial FF state access

Simulation, fresh physical-state classification, and matched isolated area/
timing PASS. The corrected candidate saves 211 ALMs: 1,472 -> 1,261, including
the common shell. Both use the same asynchronous register-file helper and
identical 50 MHz/opposite-edge constraints. This is not a whole-core saving,
a hardware boot, a global barrier or a complete-machine snapshot.

The 75 unchanged FF helpers hold all 681 physical FF bits. Their existing raw
state outputs and load inputs form a one-bit chain with no shadow bank. A read
rotates the entire chain and returns it to its original orientation before
acknowledgment. A write replaces only the selected compact 18-bit word during
the rotation. FF words0..37 are compact, with15bits in the last word. RAM/window
packages75..83 and phase package84 retain their addresses. There are192valid
state words covering all1,353physical bits; the old per-package FF word layout
is incompatible. The inherited ff[] package metadata describes physical fields;
its legacy word numbers are not the new scan access map. Consumers must use the
explicit scan layout and a new schema/ABI, not treat this as an old checkpoint.

Positive simulation v7 passes 1,657,868 host edges, 1,617,855 holds, 13,334 late
changes to enables and 9,499 administrative requests. The unchanged original
and independent physical probes check all1,353bits/all138outputs, all-bit
writes, all4,096indices, destructive continuation, restore, the first resumed
edge and20,000further cycles. All nine ordinary RAM writers are active
(784/784/784/784/511/841/841/841/841). The derived oracle changes only the access
map, bounded completion wait and added fault/reset tests. The original C++ and
all hierarchical raw-state probes remain preserved. Changing requests/payloads
while a scan is busy cannot steal ownership or write RAM/window state.

Losing the external hold after a partial rotation sets a sticky fault. FFs,
RAM/window helpers and phase state remain locally held despite attempted guest
advances and writes. Only SYSTEM reset can clear that quarantine. Administrative
writes are rejected during reset, the frozen acknowledgment clears, and a new
freeze entry is required afterward. Resetting only the transport does not
recover partially shifted guest state. The caller still owns whole-machine
reset and must hold every foreign consumer; those mechanisms are not installed.
Six broken boards/controllers reject: lost chain bit, unshifted FF package,
early completion, stolen busy ownership, lost local hold and reset-time access.

Fresh candidate XML is checked against the qualified original board schema:
all1,353physical plus206private transport bits, with no sequential exclusions.
The exact compact layout accounts for every FF field, setup/edge history, RAM
word, pending-write window and phase bit. Eight damaged inventories/layouts
reject. Original cells come from the frozen Native59 tree; the MC10170 workspace
comment difference is checked token-identical, and actual frozen bytes compile.

The final matched fit is SnapshotContAFFScanReset_20261019 on misterubuntu,
local session13410, terminal. Both sides retain all681FF bits in75helpers and
18physically placed MLAB read replicas. Every administrative replica retains
all64RAMbits; ordinary widths match the connected board pins. Total logical RAM
is576bits, replicated storage1,136bits. Read inputs/outputs remain unregistered;
RAM geometry, mode and locations are checked. Shell registers increase from
1,373to1,494 while ALMs fall by211. Four conditions pass setup/hold/pulse-width
checks, minimum+0.131ns, with no extra timing cuts or unconstrained paths.
Six damaged source/constraint/policy/retention/hold/RAM-latency controls reject.
The final offset uses an explicit size cast and adds no new truncation warning.
An extracted-archive fit audit and classification both pass again.

The first matched fit, SnapshotContAFFScanV2_20261019, saves207ALMs with the same
+0.131nsminimum. It predates reset-entry gating and explicit offset sizing. Its
single new truncation warning is range-proved (accepted FF indices0..37 produce
offsets0..666, fitting10bits), with no timing waiver. Exact earlier sources,
classification, positive v5 gate and fit audit remain preserved separately.
The independent six-size controller regression v3 also passes438transactions
and four damaged controllers after the size-cast change.

Earlier bring-up failures remain as evidence. V1 accidentally compiled the
unmodified one-edge C++ driver; V2 used pointer equality for a Verilator wide
bus, corrected to compare each word. V3/V4 correctly reject damaged RTL but
stop on overly specific expected diagnostic text; V5 completes all controls.
V6 fails a driver-source guard matching both20Kloops before compilation; V7
pins the first loop and adds the reset tests. No guest RTL repair was hidden in
those harness corrections. The first remote launch stopped on an unavailable
source-generation Python dependency before Quartus; lazy preparation imports
make subsequent runners standalone.

All1,324raw files pass lossless SHA256 readback. Raw HDL/C++/headers/reports/logs
and JSON over100KiB use gzip unchanged. The manifest records every raw hash;
objects, compiler databases and simulation executables are excluded. Additional
extracted-audit JSON files are verification metadata.

No production/core builder or live FPGA changes. Integration still requires:
versioned compact schemas; extension of the current16-edge reply timeout for
681-edge scans; global hold/fault/reset ownership across every board, SDRAM,
input, video, networking and ARM services; and matched disk/server snapshot
coherence. More boards and a complete-core fit/timing/boot remain required.
The last full-core experiment still needs45,228ALMs,3,318over capacity. Do not
subtract an isolated-board saving from that result or multiply it to claim a
whole-core gain. Original60ns clocks and final release gates remain open.
