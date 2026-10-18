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
