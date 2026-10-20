# Shared-SPI parser hold completes the tested network pause boundary

Snapshot traffic uses the same EXT_BUS as network commands. Packet-engine
pause alone leaves the parser live: every other client's transaction changes
its command/count fields. The network UIO now latches hold only between fully
completed transactions, ignores all bus traffic while held, and releases only
with the bus deselected. This adds ONE required state bit. Defaults remain off;
production request inputs are tied low until the complete coordinator exists.
Reversing the added hooks recovers the immutable Serial69 normal UIO bytes.

The enabled fixture uses actual production mux/UIO/DDR engine/serial wire, the
real C ring endpoint and independent DDR/payload/CRC oracles. It observes all
34,339 sequential/task candidate bits and both packet RAMs (32,768 bits total),
with no sequential exclusions. The 16-bit automatic CRC function return is
reviewed scratch; the 64-bit packing temporary remains conservatively observed.
Nonzero test-only RAM seeds exercise unused words too.

Tests finish a live SPI transaction before acknowledging hold, send snapshot
transactions containing network opcodes while held, and release during an
active shared-bus packet without losing framing. They also continue accepted
DDR RX/TX tails, partial serial RX and partial guest TX capture with exact data
and CRC. Four compiled ungated-guest/missing-parser-hold/early-hold/early-release
variants reject. The unchanged production/default-off wire suite and its two
mutations pass. A fresh frozen-source rebuild repeats all five enabled cases.

All 125 raw members (2,731,835 bytes;780,078 stored) read back losslessly.
Extracted archived source rebuilds the candidate and all four mutations without
working HDL. The report retains actual source, commands, XML, state review and
raw build/test logs; executables/objects are excluded.

This is a network-subsystem continuation boundary, not complete snapshots.
Real Main/daemon ordering, two wire-settling clocks, physical DDR write
visibility, native state access and import, disk pairing and Cedar restore
remain open. No new core/Main/daemon is deployed and no speed claim is made.
