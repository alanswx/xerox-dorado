# Native visible-R timing closure follow-up

The explicit-port visible-R correction passes original-image component tests,
but the first complete fit (v3) fails global hold timing by 0.031 ns. It is
rejected for assembly and deployment. At the slow 1100 mV/100 C corner, raw
storage `u_storage_5|old_data[5]` to `read_pipe[5]` has 0.683 ns clock skew
against 0.652 ns data delay. The receiver uses the gated machine clock;
the producer uses the ungated host clock. Both derive from the unchanged
20 ns core PLL clock.

`rejected-v3/` preserves all 836 closed source/report/log members. This includes
the original failed supplemental reporter, which expected sixteen physical R
registers and stopped when it found 27. The fitter retained sixteen original
R bits and made eleven named `~DUPLICATE` copies. A separate corrected reporter
accounts for every original and copy at all four corners and covers them in
the R feedback and R-to-MapBuf reports. `classified-r-reports/` preserves those
164 members; all 160 additional paths pass with minimum slack +0.163 ns. That
subset does not override the failed global storage hold check.

A read-only experiment on the same v3 placed netlist adds 0.100 ns of hold
uncertainty: the storage path changes from -0.031 to -0.131 ns. The command
therefore tightens the target. Its log records tool success and its process
is absent; the experiment did not record a driver exit code. Both before/after
reports and the script/log are in the rejected receipt.

The next native build, v4, retains identical RTL/ROM and original constraints.
After normalizing the project path, 511 existing files are unchanged; only
the QSF append and a new additive hold-margin SDC differ. The SDC verifies the
same 20 ns clock and adds 100 ps with `-add -enable_same_physical_edge`.
It does not waive the failed path. `logic-and-constraint-identity.json`
records that comparison. The full staged v4 source set is frozen in this
parent manifest, with the terminal V27 original-image component and its inputs.
That component uses the exact same CPU/evaluator RTL, verifies 3,600 IM words,
256 IFUM entries and entry 238, and matches the native control's final RM/map.
It is not a BaseBoard/germ/Cedar desktop boot.

At the recorded follow-up observation, builder 2852373, fitter 2880461 and
serialized observer 2852399 are live on misterubuntu in
`/home/alans/mister/Dorado_RFrontRNative_v4_20261021`; routing is active. Observe
these jobs rather than restarting them. V4 timing/physical-memory review,
fifteen damaged-evidence controls and assembly remain pending.

The loader is prepared for a separately reviewed canonical-R image. Its pure
metadata/MGL guard check passes and rejects twelve invalid cases, including
missing R timing, wrong ownership/source, stale RBF, changed controls and
ambiguous launcher names. These use synthetic metadata only and qualify no
real FPGA image. The loader preserves the old Boot2 pack/trace, uses a fresh
original seed and Main's normal load/barrier path, keeps Main the sole SPI
owner, and retains the same Main/daemon binaries. No hardware load occurred.

The parent archive has 982 members/525 blobs (26,593,653 raw bytes, 3,775,858
stored). The rejected fit shares 474 parent blobs; the independently generated
classified reports share none. Verify all three lossless receipts:

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-visible-hold-closure
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-visible-hold-closure/rejected-v3
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-visible-hold-closure/classified-r-reports
python3 tools/test_r_visible_loader_guards.py
```

MiSTer14 remains on unqualified RBF `57a552af`, Main 16808/server 16807.
Last qualified Cedar login speed remains 1,947,686.01 issues/s. No acceleration,
new hardware desktop, rollback or final acceptance is claimed. The active goal
retains the original 60 ns cadence, native Cedar/input, authentic rollback,
persistence, repeated boots and sustained operation. Complete FPGA snapshots
remain deferred.
