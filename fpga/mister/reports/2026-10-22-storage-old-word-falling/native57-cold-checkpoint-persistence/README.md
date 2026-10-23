# 5c65 cold files/input PASS; HDMI remains open

The original Checkpoint and explicit Rollback were separately qualified on5c65.
Its changed volume then closed through two normal Main/fsync barriers, with
no open pack/trace and daemon DDR mapping revoked:63,578,624 bytes, SHA256
288af3e962945ca6bbe72333074cef56826d65b9b5b5decd29179629af5370ca.
A local streaming copy matches. Historical three diagnostic gaps/25,201 lost
records remain explicit. Closure is not a guest clean shutdown or a complete
historical trace. Old236b/a19a/288a sources remain preserved.

A literal288a copy cold-loads the SAME reviewed5c65 RBF under fresh Main13244,
daemon13243, disk cache and full FPGA storage initialization. Main21e6/receiver
0845,57.142857MHz/SYSPER4/140ns nominal and requested1080p30 are unchanged.
Actual original113/C-release, visible prefilled Guest.pa/two Returns, germ812
and the original personal-profile prompt/No restore the saved full processor
schematic, CommandTool, Clock and icons. No repeated load/login, C-state import
or host guest-instruction/IOCB completion.

Fresh normal mouse focus and keyboard execute `Echo early57 cold` exactly.
`Type ///early57persist.txt` returns the pre-save marker;
`Type ///early57after.txt` returns the file created AFTER Checkpoint.
Full alphabet/digits echo exactly. Thus both pre/post-checkpoint filesystem
changes survive authentic Rollback AND fresh FPGA/Main/cache remount. Native58
shows the complete result. The bounded observer finishes60 samples; all senders
finish and release controls. This is one cold persistence test, not three
fresh-seed combined-core boots or a two-hour disk/network/input stress test.

Fresh bounded trace:509 read and42write request/ACK pairs,10pwrite/3fsync
completions, no recorded error or diagnostic gap. Latest good55 with CRC/RX/
abort/drop0, pack-fault clear. Identity and immutable236b/a19a checks pass.
The current cold volume is open; do not copy it without a normal close barrier.
Clock's pinned1998 date is not a timer-rate measurement.

Material limitation: HDMI/capture is BLACK on this cold run while native
screenshots and input work. Settled6s sees182frames/one unchanged black image;
normal F12/6s181frames/Escape also remains black, with no selected option,
video setting or capture-service change. The later native post-OSD screenshot
is clear (retained in the firmware-window followup receipt). The capture CLI
discovers the stream on misterubuntu, but read-only V4L2 timing queries are
unsupported and privileged device mapping is unavailable. The cause is not
proved. This reproduces the older cold690f limitation; it does not qualify
HDMI or whole-goal acceptance.

Next qualify actual firmware memory gains, faster SDRAM/native timing,60ns
phases/all-device rates and the final boot/stress gates. Goal stays active;
complete FPGA snapshots deferred. No hardware speedup is claimed here.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-old-word-falling/native57-cold-checkpoint-persistence`.
