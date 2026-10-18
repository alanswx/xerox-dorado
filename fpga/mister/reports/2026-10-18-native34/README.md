# Native34: preserve host watchdog wall time at50MHz

**Hardware result:** one original-firmware cold boot passes on MiSTer14.
All34 base packages, BootEssentials, Tioga/Press/metrics and Viewers completed.
Real mouse movement, Cmd-button creation of a second CommandTool and focus
work. At0.1s key down/up timing, both `echo rtl50ok` and
`echo abcdefghijklmnopqrstuvwxyz0123456789` received/echoed the exact argument
and returned to `%`. Native/HDMI images and `input-qualification.json` record
visual verification; OCR was inaccurate and is not a gate. No C checkpoint,
guest state import, host IOCB completion or forced ACK was used.

`hardware-observation.json` and saved Main/server/observer prefixes cover
nearly an hour:3435 heartbeats, maxgap1.1344s, no late probes or pack/packet
CRC/RX fault. Observer24210 remains running through further disk writes,
stopping at2h/first fault/Main exit. The earlier timeout has not recurred;
its specific host latency/ACK cause is still unproved.

`loader-counts.json`, `font-counts.json` and `desktop-counts.json` count fabric
edges, not retired instructions. The desktop sample includes automatic
startup Bringovers:19.8755% enabled at50.0134MHz, implying a ceiling1.2422M
ordinary instructions/s(~7.45% original), versus6.25M with no global pauses.
This is not an idle benchmark or a controlled A/B speedup. General defaults
remain33.333MHz/cacheOFF; next measure DDR/HPS pause durations. Original
phase/enable translation and MC12061 calibration are still required.
Some optional welcome app `.load.bcd` lookups fail; package cleanup is a
separate task from the verified base desktop and input.

Native32 stopped with host timeout reason8, cylinder124/head0/LBA18893.
The fixed133333332-cycle limit shrank4s ->2.6667s with the PLL change.
This scaling defect is proved; actual Main latency/ACK phase was not traced
at the fault, so it is not yet a proved explanation of that hardware stall.

pack_hps now defaults HOST_TIMEOUT_CLOCKS=4*HOST_CLOCK_HZ. build.sh derives
DORADO_HOST_CLOCK_HZ from the SAME validated PLL request and records sys_hz
and4s policy in dorado_clock_config.json. Default33.333MHz behavior is
unchanged. This is host WALL time, independent of SYSPER/guest pause time.
No acknowledgment/completion, CRC check or guest state is fabricated.

Host fault words keep reason/cylinder/head/wr_block/wr_state; low6bits now
encode {host_state[2:0],sd_rd,sd_wr,sd_ack}. Serial faults retain wr_bits.
The extra word remains the original requested/cached LBA. Diagnostics are
passive. States1/2 wait write ACK assert/release,3/4 read ACK assert/release.

New regression checks actual elapsed simulated time at333/400/500Hz scaled
fabric clocks. Three-second service succeeds at all rates. Missing ACK and
stuck-high ACK fault at4s, preserve the original phase/LBA, remain sticky
and never report ready/completion. The fixed-budget control fails at500Hz
before3s; existing damaged-Fire-code, Pilot persistence, neighboring-byte
and timeout regressions pass. Build fixture verifies PLL/QSF macro/metadata
consistency at33.333333/40/50MHz and rejects bad inputs before output.

Read-only tools/mister_watch_main.py timestamps normal Main heartbeats,
samples /proc kernel/syscall/I/O state when a heartbeat is late, and saves
the first fault in /tmp RAM. No SPI, ptrace, guest write, reset or I/O ACK.
It was validated on Native32's preserved fault. Run it on the next test to
distinguish long Linux I/O from an uncompleted hardware handshake.

Frozen candidate Dorado_Native34_20261018 on misterubuntu clones Native33's
no-bubble128-line cache at50MHz/SYSPER4, adds this pack_hps and explicit
DORADO_HOST_CLOCK_HZ=50000000 plus matching clock metadata. build.py records
all inputs/steps. Fit/STA/clock/all-corner audit has passed as recorded below.
Native34 is deployed on14; Native32 fault evidence and volume were preserved.
The full base Cedar/Viewers/input test has now passed as recorded above.
Do not promote the failed Native32 or undeployed Native33 as working cores.

Fit completed13m52,34,840/41,910ALMs(83%),307M10Ks/35DSPs. All29 default
summary checks pass. Expanded sign-off enumerates all four supported device
conditions (fast/slow at-40/100C): all20 setup/hold/recovery/removal/pulse
categories pass, overall minimum+0.101ns. Every condition has zero illegal/
unconstrained clocks and actual50MHz core. No new exceptions. Quartus map
parameters confirm HOST_CLOCK_HZ50000000 and HOST_TIMEOUT_CLOCKS200000000.
RBF SHA256 `0ebe68f456ce46e09b813dd126b605c083b606176ce0bdc797c9f3194f27bbbd`.
Guarded deployment completed05:09UTC. Original firmware/BFN113/login and
fast Guest input pass. All34 real packages loaded by05:29UTC, including
ReadEvalPrint where32 stopped. HDMI profile question observed; real keyboard
`n` accepted as No. BootEssentials/fonts completed, Viewers by06:02UTC,
exact input tests by06:05UTC, without pack/packet-CRC/RX fault.
Clock count mode is ON. Passive Main observer24210 runs through disk writes.
At idle login a normal Main host-framebuffer toggle restored initially black
HDMI; fresh physical1080p30/74.250007MHz is recorded separately. No debugger
is used during package loading.

Input drivers are preserved as `input-keyboard.py` and `input-mouse.py`,
hardcoded to the allocated MiSTer14 WebSocket endpoint. Only run them during
an exclusive input test with the intended viewer focused. The native captures
show baseline, motion, focus, short Echo and alphabet/digits Echo; the HDMI
capture independently shows the latter output and returned prompt. PNG chunk
CRCs/IEND and compressed raster data were checked after completed copies.
Saved log archives are prefixes; Main begins at the last Cedar34.mgl marker,
not inherited earlier runs. All3475 saved Main status samples lack pack fault.
