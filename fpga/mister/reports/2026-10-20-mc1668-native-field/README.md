# Corrected MC1668 native firmware/login regression

PASS: original BaseBoard/Initial/Cedar BFN113/Pilot, actual Cedar login, real
Guest input, all 34 base transfers (33 BCDs / 1,345,843 bytes), and the exact
final guest ACK. Viewers, interactive applications and persistence are pending;
the same boot is still attaching font DFs. Do not restart it.

Main3486/server3485 use unchanged Main21e6/serverc599 and core15730f44. Actual
SPI74 capability has fresh epoch c27c53f36e1a3ad8, flags22, fault/errors0.
Main remains sole SPI owner. Fresh original f3bce2a8 seed was checked before
activation; no guest state import or host instruction/IOCB completion.

Idle issues/s: 1,947,589.69 / 1,947,879.69 / 1,947,588.65; mean1,947,686.01.
This matches Native59 within 0.002%: no speed gain. These are issue strobes,
not drained retirements. The 50MHz/SYSPER4 clock and storage/network/disk policies
are unchanged. Three adjacent windows in one boot are not three fresh boots.

Base-loader visibility bounds286.376846345..286.481087635s; interior instruction
progress1.614350M/s.57 protocol replays, nonzero queue/drop counter, CRC/RX/abort0.
Timing is from regular-file log visibility, not wire timestamps or received
payload hashes. This one observed boot does not establish causal performance.
The analyser and its missing-ACK/data/peer controls pass. The bounded observer
stopped normally AFTER its final guest ACK and its PID was independently absent.

The first deployment stopped at its all-record guard because the old disk trace
contains a historical dropped-record event. The targeted continuation separately
checks the exact interrupted boundary and fresh seed; it preserves the closed
Native59 volume/hash and the entire incomplete trace without weakening that
guard. No clean guest shutdown or old filesystem-integrity proof is claimed.
Only closed tmpfs log duplicates were later removed, after matching complete SD
hashes, unchanged-file checks and no-open-descriptor checks.23,472,978bytes freed.

The login collector spools fixed-size live log prefixes on SD and verifies54,761
complete disk records without an error or trace loss. All collected prefix bytes
match their recorded hashes after extraction. It never copies the active pack.
60 selected evidence members were losslessly archived/read back. Exact executed
deployment/recovery source readback also matches the checked-in tools.

Live paths and next steps are at the top of `docs/verilog-handoff.md`. Full
native desktop/input regression, faster clocks, authentic Cedar Checkpoint/
Rollback, repeated fresh boots, cold persistence and sustained stress remain open.
