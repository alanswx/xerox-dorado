# Native59 physical startup and Native58 preservation

Native59 was normally loaded on MiSTer14 after its full-core qualification
was committed and pushed as b9d7b45a. Original firmware boot is in progress;
this report does not yet qualify Cedar login, desktop/input or speed.

Live Main29511/server29510 use the unchanged verified21e6/c599 binaries.
RBF SHA256 is db6883e495bacd51c188f0b22c459937071d4860516ae102bb84c4c7bf441cad.
Actual Main-owned SPI74 reports a new epoch aa846a314ec45d23, flags0x16,
no fault/errors,2048-byte frames/8slots/16word alignment. Daemon alone maps
the reserved64KiB DDR region; Main has no ring mapping/input-probe library.

Native58's live desktop and normal keyboard Boot to actual812 were captured.
Normal Main loader disk/network barriers and temporary Main exit completed;
retired Main27199/server27195/temp29487 were independently absent. Native58
pack is preserved at63,578,624bytes/SHA256
8b5777e6d31bf82057e4f039c593b2fc88be87bdec16890b20d9206455946ab4.
Its closed trace ends at barrier_end with no recorded disk error/trace loss.
Warm marker had passed; this is not a clean OS shutdown/cold marker readback.

Fresh59seed f3bce2a8 was checked unchanged before final Main/cache activation.
The normal core display/input settings were selected through actual Main at
input_poll. A new C watcher was armed before three physical BaseBoard boot
pulses. It is now terminal PASS: a new original Initial TX13/uCodeBoot264/
BFN113 was captured and C automatically released. Do not restart it. Early
issue1/state43 ended without retry, and the issue count is now advancing;
CRC/RX/abort are zero. Cedar login/desktop qualification remains pending.

The early uninitialized display capture is retained as boot evidence, not
a claimed Cedar screen or a diagnosed video regression. No CPU/guest-memory/
IOCB patches, C-state import, host completion or machine snapshot was used.
Bounded collectors streamed prefixes/spooled on SD, one parsed record at a
time; the active pack was not copied. See current handoff for later progress.
