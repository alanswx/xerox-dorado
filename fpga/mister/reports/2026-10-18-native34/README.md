# Native34: preserve host watchdog wall time at50MHz

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
all inputs/steps. Fit/STA/clock/all-corner audit and full original-firmware/
Cedar/Viewers/input qualification pending. Not deployed;14 still holds the
Native32 fault for diagnosis. Do not promote Native32/33 as working cores.

Fit completed13m52,34,840/41,910ALMs(83%),307M10Ks/35DSPs. All29 default
summary checks pass. Expanded sign-off enumerates all four supported device
conditions (fast/slow at-40/100C): all20 setup/hold/recovery/removal/pulse
categories pass, overall minimum+0.101ns. Every condition has zero illegal/
unconstrained clocks and actual50MHz core. No new exceptions. Quartus map
parameters confirm HOST_CLOCK_HZ50000000 and HOST_TIMEOUT_CLOCKS200000000.
RBF SHA256 `0ebe68f456ce46e09b813dd126b605c083b606176ce0bdc797c9f3194f27bbbd`.
Guarded deployment is now in progress. Hardware qualification still pending.
