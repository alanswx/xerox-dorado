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
HDMI stream went black during live mode change, then recovered after a
host Linux framebuffer enable/disable (same guest boot). The
1080p30 diagnostic requested74.25MHz; see measured-clock correction below. Main video_init
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

Later BootEssentials progress: `fpga-bootessentials-1080p.png` is an
unmodified full-field internal screenshot showing Tioga styles/Viewers TIP
and icons. Current diagnostic modeline requests1080p30/74.25MHz.
Main parser-only patch/regression and separate ARM build pass; candidate
6358f310... not deployed. External HDMI has subsequently recovered.
Installer warned about PrintTV.df despite its presence on ARM and two
successful FileLookup replies; no retrieve observed, cause open.

Recovery/current boot: BootEssentials completed, Tioga and Press font names
attached, now FontMetrics.df attachments. `fpga-hdmi-recovered.png` and
`fpga-hdmi-recovered.mp4` are actual HDMI captures, complete text rows.
Original Main1fb2/PID12701 and native guest boot retained. Host framework
reset was tested while core Pause asserted and storage quiescent: CPU
3081c81c and all16MB raw DDR SHA256
0f4bd71a03cb3a8c1553085c091796a22c1040a812874131956fa7980201a9a1
unchanged before/after actual reset release; then resumed. A separate Linux
host framebuffer at0x22001000 was backed up/tested/restored; Linux redrew
the console, and switching back to native video restored HDMI. No guest
memory was written/imported. See handoff for exact interventions/limits.

CLOCK CORRECTION: requested Fpix is not a physical clock measurement.
Main OSD/cached hardware frame counter reports60.000024Hz (vtimeh1666666),
despite requested1920x1080/74.25MHz. Actual148.5MHz output or a stale
measurement remains to distinguish. Original STA qualifies74.25MHz only.
Additional hdmi148 audit replaces just the HDMI output divider6 with3,
148.536MHz conservatively above148.5, and retains all original path checks.
Slow100C scaler setup FAILS -2.382ns (o_h_lum_pix.g[7] -> o_poly_lum[0]);
fast setup and both models' hold/recovery/removal pass. Zero illegal and
unconstrained clocks. This is not qualification of1080p60. No SDC/RBF or
live board mode was changed by this audit. Script and representative full
reports retained here; all reports also in frozen build's core/hdmi148-*.

Fresh-read resolution: cached OSD refresh was stale. UIO_GET_VRES read via
Main's own SPI at input_poll gives HDMI period3333332/100MHz =30.000012Hz,
consistent with2200x1125 timing totals and74.25003MHz. No mode/guest writes.
See hdmi-fresh-timing.{json,log,gdb}. Current live1080p30 is within the
74.25MHz clock qualification; the148.5MHz audit failure remains a limit on
future1080p60 use. Do not rely on cached Main OSD refresh after live changes.

Viewers desktop reached00:02 UTC on the same original native boot: Ready,
WatchTool,EditTool, two CommandTools, BootTool and Clock. Actual mouse moves
+100,+80 in the internal raster and Cmd-button click opens a second tool
with a `%` prompt. Direct captures/logs are included here.

Typing tests were NOT isolated: the user was concurrently trying
`openr tiogadoc`, and subsequent field-paced typing interleaved with that
command. Screens show echo FPA/FPA and Echo GA/GA/new prompts; those are
real command round trips, not exact Echo FPGA proof. The preliminary
commentary claiming FPGA output was corrected. Do not use the overlapping
runs to quantify hardware input loss. User independently reports typing
works. All scripted input is stopped; keyboard/mouse left to the user.
A separately reproduced design flaw exists: the native decoder's machine
clock can stop while raw host key toggles arrive. New host key FIFO passes
its stopped-clock failure-control test; Native28 builds independently,
not deployed or hardware-qualified. Clock architecture audit is in the
2026-10-18-clock-audit directory and docs/verilog-from-sil.md.
