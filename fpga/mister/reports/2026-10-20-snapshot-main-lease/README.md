# Main preserves the daemon lease while the network parser is held

Main's real network poll now routes an active snapshot lease directly to the
existing control stream, before any packet/capability SPI access. PAUSE waits
behind an in-flight STATUS; paused health checks use socket STATUS only. CONTINUE
also waits for pending records and releases the lease only after the same-epoch,
active/unpaused ACK. It never substitutes STOP/RESUME, remaps DDR or resets the
connection/cursors. Ordinary active status now rejects an unsolicited paused flag.

Core-load, reboot, restart and initialization guards refuse a held or failed
lease before touching SPI or hardware. Failed leases stay pending; generic reset
and fallback cannot discard them. A genuine coordinated recovery remains work
for the complete snapshot controller. These APIs acknowledge only the daemon
lease: they do not grant FPGA state access or acknowledge whole-machine pause.

The actual Main poll/manager/client and C control service pass seven normal/fault
cases on Mac, Linux and MiSTer14's ARM: delayed records/continuation, stale ring,
EOF, core change, lost pause, timeout and unexpected pause. The test protects the
actual anonymous ring with PROT_NONE while paused and enforces Main-only SPI.
Queued bytes, cursors, epoch and the same socket connection survive continuation.
Pinned Main prologues execute up to their first hardware mutation, replaced by
a counter: all four entrypoints remain blocked during the lease.

All six compiled unsafe variants reject. Mac/Linux/ARM-build use identical 21
executed feature inputs. Both hosts also pass all seven existing server/Main
regression families with identical 94 inputs, including byte-exact EFTP/STP,
control/clock/ring/map, lifecycle, legacy transport and pause mutations. Maximum
normal poll call is 0.003 ms Mac /0.005 ms Linux /0.030 ms MiSTer14 ARM. The ARM
binary SHA256 is c0839c472f26367db4a189a44c21d87de0bec122f80bfa5692d33ebaf7e012d3.
The seven ARM runs use temporary sockets, anonymous memory and SPI spies only;
production Main, daemon and FPGA are unchanged.

Prior attempts remain raw evidence, not passes: sandbox socket restrictions;
a test mutant's unused variable and over-broad mutation match; lifecycle fixture
preparation that overlooked the baseline's existing network patch; and macOS's
conflicting reboot declaration. The final fixture applies only the remaining
disk/DDR patches. A test-only reboot symbol alias preserves the real guard body.
Final production implementation bytes match qualification; two added header
comment lines are checked separately as documentation-only.

All 1,104 raw members (7,735,034 bytes;2,713,731 stored) read back losslessly.
Extracted receipts verify all inputs, logs, controls, original/patched Main
sources, cross-platform identity and ARM binary identity. A fresh extracted-source
rebuild repeats all 13 candidate/control cases with the same 21 inputs. Raw source/commands/
logs are retained; executables and objects are excluded.

Still required: the Main coordinator and matching Core/daemon admission/version
checks; real FPGA disk/input/video acknowledgments; completion of pending FPGA
disk requests before the ARM file lease; physical DDR write visibility; native
state export/import, paired disk/server state and complete Cedar restore.
No deployed feature, faster clock, new FPGA rate or complete snapshot is claimed.
