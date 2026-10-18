# Native32:50MHz/SYSPER4 with128-line raw DDR buffer

Frozen build `misterubuntu:/home/alans/mister/Dorado_Native32_20261018`.
RBF SHA256 `5747eb2698ea0d8ebbed933b714da6ad29db79730f9eecd16595724aa9c040ea`.
Only the backing adapter and its explicit cache parameters differ from
Native31. The generated wrapper comparison asserts this. Original CPU,
DRAM/ECC/cache/serial I/O and peripheral clocks are unchanged.

Fit14m23:35,047/41,910ALMs(84%),307M10Ks,35DSPs. Data/tag buffers use
128x64/128x21 RAMs. Standard29 STA checks pass, slow minimum+0.156ns,
core setup+1.214ns. Additional complete global fast/slow setup, hold,
recovery, removal and minimum-pulse-width checks all pass: overall minimum
+0.084ns(fast hold). No new exceptions; inherited Native31 SDC retained.
Core clock20.000ns/50MHz; zero illegal/unconstrained clocks. Framework
external ports remain unconstrained and are documented. Detailed reports
and exact SDC are compressed. Qualification covers the74.25MHz HDMI mode,
not full148.5MHz1080p60.

Loaded onto MiSTer14 around04:19UTC via normal Main, fresh corrected Cedar
seed. Main22751/server22742 are unchanged immutable1fb2.../1d5... binaries.
Per-core1080p30 request; sole SPI owner Main. Original firmware/microcode
loading underway; BFN113/Cedar/login/Viewers/exclusive input pending.
No guest state import or fabricated disk/network completion.

The previous Native31 control passed firmware/BFN113, login, fast Guest,
all34 Basic.Loadees and real BootEssentials installation. It was replaced
with user authorization before Viewers to prioritize the memory experiment.
Its firmware26.4617%/loader16.9255% enabled-edge measurements are controls,
not retired MIPS. They count total global pause time, not a separate DDR/HPS
cause breakdown. The new cache's actual hardware benefit is still pending.
Unit/full-storage equivalence and failure controls are saved in
../2026-10-18-ddr-linebuffer/. Default cache option remains OFF until hardware
qualification; this frozen candidate enables128lines explicitly.

First firmware-only sample:25.0156% enabled,50.0087MHz host, versus the
uncached control's26.4617%. The buffer has not improved this workload;
Cedar loader/desktop performance remains unmeasured. Do not claim a hardware
speedup from fixture read-count reductions. CPU is stopped for the firmware
load in both cases. Existing counters measure combined global pauses and
do not split DDR versus HPS disk duration.

HARDWARE QUALIFICATION FAILED before Viewers (first observed04:36UTC).
Original firmware/BFN113/login/fast Guest work; real27th retrieve is
ReadEvalPrintImpl.bcd. First disk fault807c0200/000049cd is reason8(HPS
transfer timeout), cylinder124/head0/LBA18893. Packet CRC/RXbad remain0.
The live core is intentionally stopped by this fault; do not promote it.

The old watchdog is133,333,332 host cycles,4s at33.333MHz but only2.6667s
at50MHz. This is a proved clock-scaling bug; whether this actual failure
was a slow host or missed ACK was not observed. The old record lacks host
phase. Main uses synchronous image writes after returning SPI data ACK;
a later request can wait while a previous file write completes. That is
a possible mechanism, not a measured cause. Main file descriptor24 shows
O_SYNC and position9689088; no new kernel SD error was found.

Host remains sole-owner immutable Main1fb2..., no guest repairs or forced
completion. Whole log snapshot main-at-first-fault.log and active volume
remain remotely in Cedar32_20261018 for forensics. Saved context includes
only this session after its last Cedar32.mgl marker (older faults are in
the inherited Main log). Passive tools/mister_watch_main.py was validated
against this preserved fault: no SPI or ptrace, observations in RAM.

Matching Imager-phase sample20.0104% enabled versus Native31's16.9255%
is encouraging, not a controlled whole-machine speed result. Early sample
18.5936% is another phase. Firmware25.0156% vs26.4617% did not improve.
Fresh HDMI timing30.000012Hz/74.25003MHz; actual HDMI needed no workaround.
Native34 is building with frequency-derived4s budget and passive ACK-phase
first-fault details. All standalone CRC, persistence and watchdog gates pass;
actual hardware still requires qualification. Native33 lacks that correction.
