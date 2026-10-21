# Native visible-R correction passes original image handoff

The corrected explicit-port candidate passes the original-ROM component gates
with the exact CPU/evaluator sources staged for the FPGA build. It retains
the original ProcH/ProcL g06/g07 visible-R latches, matching the timing of the
retained B-selector and MapBuf receivers. The evaluator reads those physical
Qs rather than storing another R prediction: 736 evaluator-owned Q bits plus
16 native R bits, still 752 total. Two explicit eight-bit board ports route
the latch outputs. Clocks remain 50 MHz/SYSPER4.

The [preceding capture investigation](../2026-10-21-r-front-map-capture-diagnostic/README.md)
shows why this is necessary: the central candidate exposed R=0 before the B
selector stopped selecting R, causing MapBuf to capture zero. The original
candidate had 8,191 wrong map entries and failed all loaded-image readback.

Closed functional results:

| Test | Result |
| --- | --- |
| V26, exact explicit-port sources | Original Initial: 960 words, 15 task initializations, all six error checks enabled for 611,683 samples; all 16,384 map entries and 256 RM words match native control |
| V26 phase window | All 6,000 R/B/BMux/MapBuf/write-data records and all 23 sampled map writes match |
| V27, exact explicit-port sources | 55 million host samples; all 3,600 original Cedar IM words and 256 IFUM entries match; Cedar entry 238 executes |
| V27 against native control V15 | First 640 CPU receive records match; final RM/map dump is byte-identical; post-entry task counts match |
| V24/V25, single-owner variant | Complete map/RM and full original-image gates pass |
| V23, diagnostic duplicate-R variant | Full original-image gate passes |
| Native v3 startup/pause | 2 million host samples, 499,911 roots, 359 pauses, 1,998,976 alias checks, zero unsettled states |

The startup test observes nine CP strobes but zero instruction issues at that
early point. The longer direct-ROM/raw-storage fixture ends in the same task12
wait as the passing native oracle: 2,153 emulator samples followed by 5,431,273
task12 samples. Neither is a native BaseBoard/germ/Cedar desktop qualification.
No C-emulator state or host completion of guest instructions/IOCBs is used.

`repaired-ports-handoff-comparison.json`, under the V27 logical members,
verifies source identities, original image readback and native-control state.
The staged v3 sources and exact Initial/source/owner review are included.
The latter rejects five damaged ownership/control variants. The independent
synthesis-memory review passes the intended 12-to-4 bank replacement, keeps
158 other RAM rows unchanged and rejects six damaged reports. Its underlying
map report remains in the build receipt to be collected after fitting ends.

An initial v2 synthesis attempt used hierarchical R reads accepted by Verilator
but rejected by Quartus. That build is terminal failed, all recorded processes
are absent, and its exact 527-member receipt is preserved under
`failed-hierarchical-v2/`. It shares 474 unchanged blobs with this parent;
14 additional blobs preserve the failed source/configuration/log differences.
V3 replaces the references with explicit ports; V26/V27 test those new sources.

At the recorded 12:25:32 UTC observation, v3 builder 2573751, fitter 2597431
and serial report observer 2573781 are live on misterubuntu. Placement has
finished and routing is active. Observe these existing jobs; never restart
because a poll expires. Whole-device fitted timing/physical memory review,
assembly, hardware Cedar recovery and acceleration remain pending. New reports
cover all sixteen native R registers plus R feedback and R-to-MapBuf setup/hold
at all four conditions. Assembly requires the exact-source image PASS and
fourteen rejected evidence controls in addition to the full timing/memory review.

The functional archive maps 4,052 members to 579 blobs: 54,765,449 raw bytes,
5,450,970 stored bytes. It includes the independent native control, terminal
tests, frozen inputs and complete staged v3 source set. It excludes executable
images, FPGA images, databases and disk packs. Verify both manifests:

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-visible-native-correction
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-visible-native-correction/failed-hierarchical-v2
```

MiSTer14 remains on the earlier unqualified RBF `57a552af`; Main/server and
the guest volume are unchanged. Last qualified Cedar login remains
1,947,686.01 instruction issues/s, with a 6.25 million/s nominal ceiling.
The goal remains the original 60 ns cadence, interactive native Cedar,
authentic Checkpoint/Rollback, persistence and final repeated-boot/stress gates.
Complete FPGA snapshots remain deferred.
