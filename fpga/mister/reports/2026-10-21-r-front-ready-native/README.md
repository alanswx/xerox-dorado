# Native Ready/front integration: actual Initial-ROM component PASS

The unchanged50MHz/SYSPER4 experimental native source replaces91 small
storage packages/752 physical bits and12 RM/STK packages/9216 bits, retaining
surrounding original gates and every other board/device. Source generation
verifies the MC1668/Native59 baseline and the previously qualified component.
All512 staged inputs read back; after project-path relocation, only three
native wrappers, their backplane and the QSF change, plus14 added RTL files.
No Main/daemon/FPGA/volume deployment occurred; no speed gain is claimed.

Actual native full-backplane power-up diagnostic v2 passes2M host cycles,
499911 root rising edges,359 paused samples,1998976 alias checks and zero
unsettled states. Its independent physical-array backend honors storage
ready; it does not complete guest instructions or IOCBs. Nine original
BaseBoard CP strobes but zero instruction issues in that short interval:
this is not an Initial/Cedar firmware boot. V1 correctly failed the existing
storage assertion because the harness advanced before storage was ready;
that assertion remains intact in v2.

Initial component v4 AND stronger v5 compile the exact three changed native
wrappers and native central storage wiring. The original960-word payload
reads back with zero wrong, executes325 distinct IM addresses, initializes
all15 tasks, then keeps all six error sources enabled for2611683 samples
without error. V5 additionally asserts live input aliases, unsettled state
and repeated events after the first1024 samples, with no failure. Both runs
are terminal/exit0; all434 frozen inputs per run verify. The original F43
debug entry/CPU-manifold fixture and generic remaining boards are retained:
this is not a full native BaseBoard/Cedar or FPGA boot qualification.
Three removed oversampling history fields are explicitly unavailable in
optional diagnostic displays; physical storage accesses map to actual Qs/RAM.

Initial preparation v1 stopped on the generated top's second module; v2
stopped on a Python local-name collision. Neither started a simulator. V3's
compile failed on an obsolete parity-RAM diagnostic address reference; its
frozen inputs/log are retained. V4 corrects that address to actual physical
pins; v5 adds the live invariants. These development failures are not passes.

Full native Quartus project is still running on misterubuntu:
`/home/alans/mister/Dorado_RFrontReady_v1_20261021`.
At2026-10-21T01:57:08UTC builder138712 and fit146109 are confirmed LIVE;
map138713 is ABSENT/exit0. All512 remote inputs match the staged candidate.
The sequential driver stops before assembly. Timing/memory/constraint review,
the prepared explicit central-Q crossing audit, full original-firmware Cedar,
new hardware speed measurements and rollback/final gates remain pending.
Do not run another Quartus stage against this database until the builder
is terminal. An observation timeout is not a terminal job.

Raw archive:2684 members/70699900 bytes, stored as15012654 bytes. Lossless
readback verifies every member; separate archived-input readback verifies
all512 candidate, all421 startup inputs and434 inputs in each accepted
Initial run, plus native wrapper identities. See the JSON for exact counts.
No second complete simulation from extracted evidence is claimed.
The archive excludes objects, executables, FPGA images and Quartus DBs.

Prior component qualification and native inventory are dependencies preserved
in `2026-10-20-r-front-ready-path`; the MC1668/Native59 immutable receipts
provide the original baseline. This archive preserves the produced candidate
and actual executed tests, not a new full baseline/component replay.

Current scripts:

- `tools/stage_r_front_ready_native.py`: isolated normal-operation source.
- `tools/test_r_front_ready_native.py`: actual native power-up boundaries.
- `tools/test_r_front_ready_initial.py`: actual native-board Initial component.
- `tools/build_r_front_ready_native.py`: sequential fit/reporting, no assembly.
- `tools/audit_r_front_ready_native.tcl`: prepared central-Q timing report;
  not executed at this milestone.

State administration remains INACTIVE. Obsolete selected history/scan slots
are unavailable; no snapshot ABI or full FPGA snapshot is advertised.
The last physical speed remains1947686.01 instruction issues/second.
