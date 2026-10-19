# Fresh Native51 repeat: idle and slower loader reproduce

Normal load06:39:18UTC19Oct on MiSTer14. Exact51 RBF3d253187, Main12945/
21e67a3f, server12944/c5996249, new originalf3bce2a8 seed checked unchanged
before final caching. Same50MHz/SYSPER4, SDRAM/no raw cache/direct DDR,
arbitration/interleaving ON, deferred reads OFF. ContB exact-advance adapters
with ALL snapshot administration inactive. No physical save/load.

Actual Main SPI74 flags0x16, fault/control-errors0, fresh epoche4fc88f50e0d587c;
daemon maps only64KiB1FF00000 and Main has no ring mapping or probe DSO.
Two known Main GDB controls precede measurements. Original CoolBoot/BFN113,
Pilot and Cedar login pass. C/cool-boot helpers are terminal. Three adjacent
10s windows average1,947,720.553 issues/s, +0.000504% versus first51: no
material difference. This is one new boot, not final3boot qualification, and
issues are not drained retirement. No diagnostic pauses during the loader.

After visual login verification and a separate ready read-only observer,
run-measured-loader.py collects33 log samples using the pinned measurement
helper and immediately sends Guest plus two Returns at0.2s key intervals.
This fixes measurement-to-input cadence. The earlier manual login gate still
varied, so do not call it a fully pinned final benchmark. Input timing is
recorded; Mac monotonic input timestamps and MiSTer observer timestamps are
different clock domains and must not be subtracted from one another.

All34 transfers/33BCD/1,345,843bytes and the actual final guest ACK pass.
Last-file coverage, exact ACK peer/probe and damaged-evidence controls are
checked by the same analyser used for49b/51. Elapsed333.175095..333.425737s
repeats first51's333.685688..333.915342s; both exceed49b311.339931..311.569520s.
Whole-loader issue brackets551.858..555.260M match the first51 scale and
exceed49b510.929..514.372M. Interior issue rate1.66192..1.66212M/s and host
clock progress bracket50MHz. No idle-throughput regression claim can clear
this observed end-to-end slowdown.

This run has93 replay events in DIFFERENT transfers:56 Imager,37 CommandTool;
the first51 had37 Tioga/56 CommandTool/35 Watch. Thus extra protocol work
repeats, but the affected files and event counts vary. The source of loss,
possible RTL effect and run-condition effects remain unproven. A fresh49
control with the same measurement-to-input recipe is next before promoting
any performance conclusion. Native49 remains the selected baseline. All34
served source hashes match after timing; physical payload hashes remain open.

Observer13211/Mac73914 is terminal after verified final ACK/stop file, and is
independently absent. Do not restart it. Cedar timed out to profile Yes;
real Return accepted the default name, the guest reported it missing, and
explicit No+Return was sent on the retry. Full51b desktop/input is pending;
the previous51 desktop/input/warm marker already passed. No dataset is
substituted for three final release boots or the two-hour sustained gate.

Old51 normal guest Boot visibly reached germ812. Actual normal Main network/
disk barriers and temporary Main exit preserved its63578624-byte pack at
Cedar51_20261019, SHA08623dd70ca076d2a773fac141c6574781f01c955bd0daa8bbdb102ba01b88ba.
Old Main11259/server11258/staging12929 are independently absent. The retired
disk trace ends barrier_end without errors/drop. Its rtl51persist.txt marker
has warm readback only: no cold marker readback, clean OS shutdown or complete
snapshot is claimed. Exact evidence is in deployment/retired51 records.

Read-only collection ran AFTER timed transfers and observer retirement. Its
live logs are file prefixes, not a disk barrier or atomic state snapshot.
field-collection.json validates the archived raw bytes; gzip files are exact.
Live Main:/tmp/dorado51b-main.log, disk:/tmp/dorado51b-disk.jsonl,
volume/server:/media/fat/games/DoradoRTL/Cedar51b_20261019. Working helpers:
build/rtl-executive/cedar51b-launch. Complete snapshots, original60ns clock
migration, fresh repeated comparisons/desktop boots and sustained stress
remain required.
