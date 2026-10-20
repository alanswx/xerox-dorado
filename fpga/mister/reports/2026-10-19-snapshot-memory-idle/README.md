# Passive idle acknowledgment on the real memory adapters

The actual arbiter, SDRAM and DDR adapters each gain one snapshot_idle output
and zero state. Removing only the explicit output/assignment restores the exact
immutable Serial65 source. No normal request/response or physical clock/timing
process changes. Signals remain unconnected in the native wrapper/Serial68.

The arbiter rejects idle with queued producers or a latched transaction. The
SDRAM backend rejects idle with queued/active/unconsumed guest work but remains
idle during autonomous refresh; physical clock/refresh must never be frozen.
The DDR output means per-adapter idle, not DDR-fabric write completion.

External-handshake monitors retain original independent scoreboards. Both
128/16-group prefetch/interleaved cases pass; the 128-way case has 8,384
transactions, 80,197 idle checks, request/response stalls and three reset phases.
The actual 50MHz XSDS chip model passes 309 transactions (156 reads/153 writes),
both ranks/banks/rows/lanes, backpressure refresh and reset drain. Its idle gate
checks 20,682 observations including 18 quiet refresh commands. The DDR byte-lane,
request/read stalls, response backpressure and reset tests pass. Four deliberately
broken queued-request/unconsumed-response/refresh predicates compile and reject
at runtime. The first attempt's ambiguous mutation anchor is preserved as a
failed qualification harness, not a passing gate.

All 77 raw members read back losslessly. Exact input hashes and extracted inverse
proof recheck; archived actual arbiter/SDRAM/DDR candidates rebuild and run with
no working HDL. No native service barrier, SDRAM bank closure/controller state
export, physical DDR write drain, complete FPGA snapshot or speed gain is claimed.
