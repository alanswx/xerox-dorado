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
One matching-ELF GDB pause selects core input/video0 and issue view1; a second
control selects the same view before timing.
Three original BaseBoard CoolBoot pulses97146 and C watcher80173 COMPLETED;
fresh BFN113 served and C immediately released. Do not restart completed
helpers or the boot on an observation timeout.

Original Pilot/Cedar login PASS. Three adjacent10s windows mean1,948,049.932
issues/s (+0.01861% versus49). ONE fresh boot each; this demonstrates no
meaningful idle gain for the added3323ALMs/2832regs (73->81% ALMs). Do not
promote this policy based on the synthetic fixture or this tiny difference.

After idle timing, bounded read-only observer7959/Mac96827 uses the same
policy as48/49 and real Guest28107 is accepted. The full34-transfer sequence
and exact final guest ACK pass311.63439..311.86355s; all34 remote source
hashes match after timing. Every Load ok and profile prompt are visible.
Observer is stopped/retired, not running. Full Viewers/input are NOT tested.

Decision: reject this policy as the baseline. Loader difference from first49
is only0.22..0.40% in one run each; the second49 boot is311.33993..311.59041s,
so50 falls inside observed49 timing variation. This does not justify3323ALMs.
Retain optional RTL/tests/evidence but return to49 with deferred reads OFF.
Actual normal49b loader retires50 safely and preserves its physical disk;
see native49b-repeat. It was at the profile prompt, not a clean guest shutdown.
Helpers: build/rtl-executive/cedar50-launch. Main /tmp/dorado50-main.log,
disk /tmp/dorado50-disk.jsonl, volume Cedar50_20261019. No complete snapshot
or final repeated/stability qualification is claimed.
