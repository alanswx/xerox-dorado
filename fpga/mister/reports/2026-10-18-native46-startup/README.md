# Native46 startup refused; complete board recovery

Normal loader programmed the qualified Native46 RBF and reexeced Main21e6
as22443. Main observed command74 flags0x10: owned/not-stopped with enable,
initialized, fault and epoch all zero. It correctly retained ownership and
refused termination. The raw capability was read through matching Main at
input_poll; no second SPI owner or guest state modifications were introduced.

An isolated real daemon control listener22607 returned STOP ACK, but the
hardware stopped indication remained false and the drain timed out. No
RESUME occurred. /proc maps confirmed neither old daemon20495 nor the new
STOP-only daemon mapped /dev/mem. The staging disk was the unchanged f3bce2a8
original seed. Native42 previously completed a normal guest Boot (germ812),
and its real loader trace ended in successful barriers with no errors/drops.
Its63578624-byte physical pack0c5ab6f5 remains in its original folder.

Actual staging Main's disk barrier returned true immediately before recovery;
both daemons exited normally, storage synced and an OS whole-board reboot
returned MiSTer14 to stock Main876/MENU. This is recovery, not a successful
DDR STOP/hot-reload gate. No network ACK or guest completion was fabricated.

Native47 replaces the redundant initialized-high stopped flag with decoding
of the existing drained FSM STOPPED state. Standalone and combined packet
traffic/stop tests remain identical, and early-stop/byte-order/epoch/mux/pause
failure controls fail. The compiled46 FF metadata is recorded, but this is
not proof of a specific Quartus initialization cause.47 routed timing and
hardware must independently validate the correction. Main's new regression
requires refusing the exact initial disabled/not-stopped hardware observation.
