# Frame-preserving DDR engine pause and serial-wire continuation

The real RTL engine drains its accepted DDR/frame work, then retains IDLE
state, epoch, queue indices and timer phase. ACK/commit pulses must finish
before snapshot_idle. Reset/disable/changed epochs retain drained STOP and
ownership revocation. The feature is disabled by default; production ties the
new request low. Normal RTL inverses byte-for-byte to terminal Serial69.

Two counter origins (zero/wrap) pass the actual C ring peer and independent
DDR/wire-buffer oracle, including full8TX/16RX queues plus a pending ninth TX,
accepted reads, stalled2048-byte frame tails, inactive state and lifecycle loss.
All 677 original register/task candidates are observed, conservatively including
the64-bit packing temporary. Four compiled early-idle/timer/epoch/pulse controls
reject. The original ring and production wire/CRC gates and their controls pass.

The enabled production-mux/UIO/engine/serial-wire fixture also holds34,338 bits,
including both packet RAMs (32,768 bits total), then continues partial
serial RX and guest TX capture with exact payload/CRC. Arbitrary nonzero initial
RAM patterns exercise unused words too. The ungated-guest control rejects.
Engine/wire rebuilds from frozen source repeat; extracted archive rebuilds do too.

This precursor requires idle SPI. Reviewing integration exposed shared-bus
snapshot traffic mutating the network parser. The subsequent
2026-10-20-snapshot-network-uio-pause report adds a real parser hold and replaces
that restriction for the observed network subsystem.

All 277 raw members (4,311,340 bytes;1,299,765 stored) read back losslessly. V1
preserves an earlier pass with a width warning; V2 uses an explicit integer
predicate and adds full-queue/pulse-tail coverage. No executable/object files.

No Main coordination, physical DDR-fabric write-visibility barrier, native
integration/capacity/timing, state import, whole-machine bundle or restored
Cedar desktop is claimed. The test's two wire-settling clocks still need to be
part of the real service coordinator. No hardware deployment or speed change.
