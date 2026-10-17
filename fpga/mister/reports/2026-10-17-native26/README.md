# Native26 experimental memory geometry

Four64K-chip modules provide8MB physical RAM with64K Map entries.
Extra row/column address bits are wired from MSA MemAd.1 and MemX RTMapAd.1
through the explicit density straps; all21 Map planes select64K arrays.
Raw DDR grows to16MB. Native25 receiver setup correction is retained.

Parameter defaults preserve16K cells and byte-identical generated default
MemX/MSA boards. Independent cell, full MemX geometry and full MSA-vs-DDR
comparisons pass, including all9,437,184 stored bits. Default2,359,296-bit
MSA comparison passes too. Full generated wrapper elaborates.

Cold500M prototype finished: original Initial/Cedar image checks pass,
IFU8540094/8543534, all six errors enabled/clean486611635 samples. Source
sampling has8880 words with zero mismatch, below existing8900 minimum;
this gate fails coverage and must not be called a pass. See final reports.
The prototype predates Native25 receive-full correction and uses fixture
Pendulum/Refresh, so it does not qualify full BaseBoard startup.

Fit/review complete:29 checks,min+0.074ns, no illegal/unconstrained clocks;
exact same-cell write-address hold review included. RBF586dc7f8... is now
on MiSTer14 as the separate memory-geometry hardware control with a fresh
corrected-label packf3bce2a8.... Original BaseBoard/Initial firmware emitted
BFN113 at22:46 UTC and C released; actual OS routing requests follow.
Actual login and Guest accepted. All33 BCD loading lines through End.bcd
report ok. Installer keyboard response No+Return advanced personal-profile
question into BootEssentials BringOver. Desktop/input remain incomplete;
pack_fault0. See fpga-all-loadees-1080p.png and fpga-installer-1080p.png.

Video investigation:720p downscales source1072x862 interlace to900x431,
retaining only one field; HDMI video alternates missing text rows. Native
1080p internal capture retains both fields and renders clear text. External
HDMI stream went black during live mode change; not yet recovered. The
1080p30 diagnostic used the qualified74.25MHz HDMI clock. Main video_init
restored original720p preset/VIC4 without FPGA reload; HDMI still black
after transmitter power cycle and capture14 reconnect/format test. Full
capture configuration restored; other streams never restarted.1080p60 was an
out-of-qualification diagnostic and is not the qualified build setting.
No INI change/reload. Original display's2:1 interlace is explicitly specified
by HMp115. HDMI must still be presented progressively without shimmer.

Host Main extended-mode parsing stalled on zero Fpix23:12–23:14, recovered
by returning only its host video_mode_cmd frame with exact binary symbols,
cancelling queued host display commands. No guest-state write or reload;
normal network/disk transport resumed with pack_fault0. Host-only HDMI
transmitter reinit at input_poll boundary did not restore external capture.
See current handoff for the exact intervention and remaining video TODOs.
