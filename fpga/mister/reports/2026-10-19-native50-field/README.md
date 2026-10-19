# Native50 deferred reads: fresh hardware boot

Normal active-lease Main loader completed03:25:31UTC19Oct. Old49 Main6414/
daemon6413 and temporary Main7647 exit normally. Actual network/disk barriers
precede programming; the new f3bce2a8 seed is unchanged before caching.
Native49's63578624-byte volume is preserved at its original path, SHA
f48ce873af077e0e2bb77bcd60f635441fe67010d0838722157b586f6ffb6375.
The normal guest Boot/germ812 followed verified desktop/input/marker close
and readback. Retired disk trace ends barrier_end without errors or loss.

Fresh Main7660 is21e67a3f; daemon7659 isc5996249. RBF70886931 runs50MHz/
SYSPER4, SDRAM/no raw cache/direct DDR, arbitration and physical interleaving
enabled. Only deferred unobserved reads differ from49. Original pipeline,
old-data and write cadence remain; all-device31/36/four-condition timing
passes, minimum+0.035ns. No speed gain is assumed from synthetic tests.

Actual Main-owned SPI74 flags0x16/fault+errors0, fresh epochc4d9e1289cdc99f5.
Daemon maps exactly64KiB1FF00000; Main does not map that ring or load a probe.
One matching-ELF GDB pause selects core input/video0 and issue view1.
Three original BaseBoard CoolBoot pulses97146 and C watcher80173 COMPLETED;
fresh BFN113 served and C immediately released. Do not restart completed
helpers or the boot on an observation timeout.

Pilot/Cedar login, measured speed and full Viewers/input remain pending.
Helpers: build/rtl-executive/cedar50-launch. Main /tmp/dorado50-main.log,
disk /tmp/dorado50-disk.jsonl, volume Cedar50_20261019. No complete snapshot
or final repeated/stability qualification is claimed.
