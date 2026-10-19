# Native47 active DDR stop and normal reload

After the qualified original-firmware Cedar Viewers/input run, normal guest
`boot` visibly reached germ812 (`guest-boot-germ812.png`). The actual pinned
Main1509 was stopped at `input_poll` only for the diagnostic. It called its
real `dorado_net_barrier`; no daemon ACK, hardware flag or epoch was patched.

The actual SPI74 response changed from flags0x16 (initialized/enabled/owned)
to1 (stopped, unowned, disabled), with zero fault/control errors and the
same old epochfdfe0a77fc6c87b5. The real barrier returned true. A separate
process/maps check proved daemon1506 had no `/dev/mem` mapping afterward.
Main's Quiesced state prevents automatic RESUME. `stop.log` and `unmap.json`
preserve the complete response and independent mapping observation.

The normal `load_core` command then programmed the same timing-qualified
RBF3df4a799 through the real Main network/disk barriers. Old Main1509 and
daemon1506 retired normally. Temporary Main3263 also exited normally; no
forced process termination, full-board reboot or FPGA programming bypass
was used. Its inherited O_EXCL trace path is discarded by starting final
Main3277 with fresh paths. New daemon3276 is the same c5996249 binary; Main
remains21e67a3f. The final Main has no qualification DSO loaded.

The original f3bce2a8 disk seed was checked unchanged after temporary Main
exit and before the final image cache started. Full reload/cache-ready/
fresh physical DDR grant succeed. Actual Main's new SPI74 record is0x16,
fault/errors0, epoch67595ea973a3f023, different from the retired grant.
The daemon maps64KiB at physical1FF00000. Only Main owns SPI.

The old volume remains at `Cedar47_20261018/cedar-work.pack`,63578624 bytes,
SHA25641c26b7bbb6ea6b0fd7bfe5ea90f420d05c8e94f2224f45a4caf015fe2f985a5.
The full old disk trace ends in `barrier_end`, with no file/barrier error or
trace loss. Its known debugger pauses are not host-latency qualification.
This is physical disk preservation, not a full FPGA snapshot.

Fresh47b paths: `Cedar47b_20261019`, `_Computer/DoradoRTL_Cedar47b_20261019/
Cedar47b.mgl`, `/tmp/dorado47b-main.log`, `/tmp/dorado47b-disk.jsonl`.
One setup GDB pause selects core video/input0, counter view1 and checks the
new epoch. The new C watcher is armed and three real CoolBoot pushes issued;
fresh original-firmware boot is underway. Never rerun this deployment script
blindly; it asserts process identities, untouched seed and absent new paths.

This passes one actual active-lease STOP/unmap/normal-load/new-grant lifecycle.
It is not physical DDR payload hashing, hardware stop-under-forced-bus-stall,
complete machine save/load, repeated final boots or sustained qualification.
The independent stalled/mid-frame drain fixtures remain separate evidence.
