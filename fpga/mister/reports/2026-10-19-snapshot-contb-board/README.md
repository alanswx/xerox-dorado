# Actual ContB IM access: board comparison and isolated routed fit PASS

`generate_contb.py` creates a separate experimental board from the unchanged
generated ContB. Only the144 F10415A instances are replaced, using the eight
audited18-bit half-banks. All other board body text, original data/parity,
address/CE/WE cones and private wired-output contributions are retained.
Production generated sources, QSF and the live FPGA are unchanged.

The14-bit local state index selects a half-bank with bits13:11 and a word
with bits10:0. Words0..1023 address RAM,1024 addresses its raw read pipeline,
and1025..2047 return error without mutation. The shared response-bank register
supports bank changes on every host cycle. The caller must first hold every
guest clock/state and supply the actual falling-edge-latched clock enable;
the local frozen handshake does not establish a machine-wide barrier.

The unchanged original board is the reference, with independently resolved
backplane self-contributions. A separately implemented array oracle checks
each original read-before-write. Test-only initialization seeds both copies;
normal cycles use actual board address/write/parity logic without forcing
internal pins.97,418 host edges,67,357 held edges,13,334 deliberately late
enable-request changes and57,392 state accesses pass. All71 board outputs
and all144 raw pipeline bits match. Real-strobe write counts per half are
716/421/4264/2567/1099/961/7772/6924. Every RAM/pipeline bit is exported,
inverted and restored through the candidate state port while both boards'
other FFs are held. All invalid rows, cross-bank responses, backend holds,
recapture and first actual resumed edges pass. Four damaged controls fail:
wrong clock enable, output bit, row bit and response bank.

Matched Quartus17.0.2 fits use the5CSEBA6U23I7 device,50MHz, a real global
clock gate with falling-edge enable capture, and registered I/O shells.
External virtual-pin budgets remain2ns max/0ns min. Opposite-edge I/O launch/
capture gives10ns internal shell budgets. Gate/host-related paths are timed;
there are no timing exceptions. Both variants retain147,456 RAM bits and
16M10Ks. The candidate infers eight1024x18 simple-dual-port memories with
OLD_DATA. All12 setup/hold/pulse checks per variant across four operating
conditions pass; candidate minimum slack+0.168ns. Recovery/removal have no
asynchronous paths. Illegal/unconstrained clocks and I/O paths are zero.
Eight damaged project/source/clock/constraint/geometry/mode/timing/coverage
controls reject for their intended reasons.

The matched fixture measures287->600ALMs and325->584registers: +313ALMs,
+259registers, no RAM growth. This includes shell/admin-interface pruning
differences and is not a full-core timing or area projection.

Reproduce on the Mac:

```
python3 fpga/mister/snapshot/test_contb_snapshot.py --mutations --output NEW_DIRECTORY
python3 fpga/mister/snapshot/fit_contb_snapshot.py --prepare NEW_FIT_DIRECTORY
```

Copy the prepared fit directory to misterubuntu and execute its `run-fits.py`.
Collect the resulting directory, then run:

```
python3 fpga/mister/snapshot/audit_contb_snapshot.py COLLECTED_DIRECTORY --negative-controls
```

Remote project `/home/alans/mister/Dorado_SnapshotContB_20261019` and Mac
handle97952 are COMPLETED. The evidence archive retains fitted sources,
project assignments, logs and all-corner timing reports. Generated simulation
boards/fixtures are gzip-compressed byte-for-byte, retaining original whitespace.
The audit checks
current tool/source provenance, exact generated board/fixture/project/SDC and
memory geometry/modes. Quartus's appended version marker is the only ignored
project line. No other project assignments are waived.

Next: the separately elaborated original ContB inventory finds99 non-IM
sequential candidates totaling209 bits: eleven MC10176 packages (13 bits
each: q, d_pre and ck_d), and eleven MC10231 packages (6 bits each: both q,
pre-capture and clock-history bits). Inventory XML and raw results are
archived, but their remaining combinational/initial-only candidates still
need classification. Export/import those registers and prove actual whole-
board continue/restore before wider integration. This test restores IM only;
it does not continue and restore the other FFs. Global barriers, full machine
state, SDRAM, ARM disk/server state, atomic bundles, FPGA restore and restore
speed remain unimplemented/unqualified.
