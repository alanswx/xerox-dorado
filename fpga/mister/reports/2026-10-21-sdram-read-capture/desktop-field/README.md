# Corrected SDRAM core: original Cedar desktop and input

One fresh-seed original-firmware boot reaches Viewers. This stage retains the qualified MC1668 CPU/cache/map/ECC; it does not integrate or qualify the separate experimental Ready/front CPU replacement. Actual captures qualify mouse movement/click, exact short and alphabet/digit echoes, and normal warm file create/close/type readback. This is not cold persistence, repeated boot, original 60ns clocks, rollback or sustained-operation qualification. No C-state import or host completion of guest instructions/IOCBs is used. Main remains sole SPI owner; original cache/map/ECC and board/peripheral clocks are preserved.

The 34 pinned base transfers and actual final guest ACK pass in 305.897..306.043 seconds (37 replay events). Interior issue rate is about 2.228M/s. This loader is slower than the prior 286-second observation despite the corrected core's 2.593M/s idle-login result; pre-login dwell differed, so neither a general boot speedup nor a single RTL cause is established.

Separate installer windows show 58.42% storage-only pauses, zero disk-only/simultaneous pauses, and 3.92% held instruction cycles. These are distinct intervals, not an atomic partition. Exact Main selector scripts detach before timed observations and restore selectors afterward. Current fitted address-to-ready/gate paths have +0.785/+0.790ns setup margin over a 10ns half-cycle relationship. This identifies a memory readiness target for the next speed component; no faster whole-device clock has been qualified.

CRC/RX/abort counters remain zero at reviewed captures, but TX drops are nonzero and retained. Read-only regular-file prefixes do not include all buffered trace records and are not a disk barrier or coherent machine snapshot. Bitstreams and the active pack are excluded. Exact screen/input/protocol/counter evidence and collectors are preserved.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-sdram-read-capture/desktop-field`
