# Fresh Native49 repeat after rejecting deferred reads

Normal load03:46:47UTC19Oct; Main8321/21e67a3f, daemon8318/c5996249, same
Native49 RBF6e7866f3 and original f3bce2a8 seed. Temporary8310 exits normally;
seed checked unchanged before caching.50MHz/SYSPER4, SDRAM/no raw cache,
arbitration/interleaving ON, deferred reads OFF, direct DDR. Main-owned SPI74
flags0x16/fault+errors0, fresh epoche062b80b135256f6; daemon maps exactly64KiB
at1FF00000, Main does not map the ring and has no probe DSO.

Original firmware/BFN113/Pilot/Cedar login pass. C watcher9694/CoolBoot81386
COMPLETED. Two setup GDB controls precede timing. Three adjacent10s windows
mean1,947,691.366 issues/s versus first49's1,947,687.412 (+0.000203%). These
are two independent boots, not three final combined-release boots.

Guest31922 accepted. All33BCD+list transfers and exact final guest ACK pass
in311.33993..311.59041s; all34 actual remote source hashes match AFTER timing.
Observer8733/Mac38162 COMPLETED and absent independently; profile No34368
sent, full Viewers/input still pending. Manual pre-Guest idle delay differed,
so this is an observed repeat, not the final automated pinned benchmark gate.
50's311.63439..311.86355s lies inside the total observed49 timing range.

Native50 is rejected as a baseline: negligible idle gain, no demonstrated
loader benefit beyond observed variation, additional3323ALMs/2832regs.
Its physical63578624-byte pack remains at its original path, SHA
0b1572b76a2662f372c5a8b79eebf281f7976fc780369de3514f9c20189080f6.
It was retired at the completed loader/profile prompt with no user files;
no clean guest shutdown or complete snapshot claimed. Actual Main network/
disk barriers pass; retired trace ends barrier_end without errors/loss.

Main /tmp/dorado49b-main.log; disk /tmp/dorado49b-disk.jsonl; Cedar49b_20261019.
Helpers build/rtl-executive/cedar49b-launch. Never restart completed watchers.
