# Native51: original-firmware Viewers/input and warm marker pass

Normal load completed 2026-10-19 05:39:59 UTC on MiSTer14. Main11259
is21e67a3f; daemon11258 isc5996249; RBF3d253187. Exact hashes and original
f3bce2a8 seed are in deployment.json. The new seed was checked unchanged
before the final disk cache. No imported C state or guest memory repair.
Same50MHz/SYSPER4, SDRAM/no raw cache, direct-DDR network, arbitration and
physical interleaving ON, deferred reads OFF. ContB alone uses the qualified
host-clock/exact-advance adapters. ALL snapshot administration is inactive;
this running core cannot save or restore the machine. Full-core source,
inference and52 detailed/four-condition timing checks are in native51-contb.

Actual Main SPI74 flags0x16, fault/control-errors0, fresh epoch64113e80774df597.
Daemon independently maps only64KiB at1FF00000; Main has no ring mapping or
qualification DSO. Two known GDB setup controls precede all measurements.
C watcher/cool-boot/idle/Guest helpers are completed. Original BFN113,
Pilot and Cedar login pass. Idle three adjacent10s windows in ONE boot:
1,947,710.746 issues/s, +0.000995% versus fresh49b, no material difference.
These are StartCycle issues, not drained retirements or full-speed execution.

The completed read-only observer brackets all34 base transfers (33BCD plus
Basic.Loadees,1,345,843bytes) and the actual final guest ACK, with exact peer,
probe and last-file data-range coverage. Elapsed333.685688..333.915342s is
longer than49b's311.339931..311.569520s using the same new analyser. The older
stored timing reports have slightly wider upper bounds because they waited
for the server's subsequent ACK log. Those historical reports are unchanged.
All34 remote source files match the49b hashes after measurement. These are
source hashes, not hashes of payloads received physically by the guest.

`tools/compare_fpga_loaders.py` shows552.031..555.317million issued starts
inside conservative whole-loader brackets, versus49b's510.929..514.372million
(7.32..8.69% more work). Interior throughput is1.66235..1.66256million/s,
versus49b's1.64249..1.64271million/s. Host-clock progress brackets50MHz.
There are128 FTP_REPLAY events versus zero in49b:37 in Tioga,56 in
CommandTool and35 in Watch. Those phases take about8.85,12.41 and6.31seconds
longer respectively; other phases partly offset that difference. This
localizes the observed delay to additional protocol work, but does NOT prove
why replay was needed or clear the RTL of a regression. Fixed-cadence fresh
repeats and packet-delivery investigation remain required before promotion.
Both analysers reject damaged evidence; missing ACK, missing data, wrong
peer, missing counters and reset-counter controls are preserved in JSON.

Observer11620 is terminal, reason stop file, and independently absent. Do
not restart it or the original deployment. Cedar timed out to profile Yes,
then waited for the profile name. A real keyboard Return accepted the
displayed default; the guest reported it missing and offered the question
again with default No. Explicit No+Return was accepted, BootEssentials and
subsequent installer transfers advanced. This first collection preceded
desktop qualification. No timed measurement
overlaps the read-only field collector's compression/file-hash work.

Old49b normal guest Boot visibly reached germ855 (not812). The normal Main
loader completed its actual network/disk barriers and normal temporary-Main
exit; old Main8321/server8318 and staging11239 are independently absent.
The preserved63578624-byte49b pack hash is670f3e54 (full value in deployment).
Its disk trace ends barrier_end without error/drop. The prior desktop/input
and warm-marker checks are archived; no49b cold marker readback, clean OS
shutdown or complete machine snapshot is claimed.

`collect-field.py` was executed read-only on MiSTer14. field-collection.json
contains hashes of the exact captured bytes; large logs are individually
gzip-compressed without editing. The51 logs were copied while live and may
end mid-line; they are not a disk barrier. The retired49b trace is complete.
Local working helpers: build/rtl-executive/cedar51-launch. Live Main log:
/tmp/dorado51-main.log; disk trace:/tmp/dorado51-disk.jsonl; run directory:
/media/fat/games/DoradoRTL/Cedar51_20261019. Snapshot4a, original60ns clock
migration, final three fresh desktop boots and sustained two-hour test remain
required. Native49 is still the selected performance baseline.

Follow-up: full Viewers Ready, WatchTool/EditTool/Clock and profile tool now
paint. Real mouse0,0->912,8/Cmd opens a second CommandTool; focus737,608.
Exact `rtl51 keyboard ok` and complete alphabet/digits echo with fresh prompts.
Normal guest `echo rtl51persist > ///rtl51persist.txt` followed by `type`
prints the exact marker and a new prompt. Captures/actions are preserved in
desktop-input.json/input-actions.json. No cold readback for this marker yet.
The first tool continues optional profile installs. This is ONE fresh51
desktop, not the final repeated/stability gate; no speed-regression clearance.
Additional live desktop-era log prefixes are separately collected after the
warm-marker readback. They are not a disk barrier or complete machine snapshot.
