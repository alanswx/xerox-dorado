# Dorado frame mailbox v1

Implementation status: the C/RTL endpoints, optional Dorado.sv ownership
mux, daemon control/mapping and Main grant/lifecycle manager are implemented.
Native46 passes all-device timing; pinned Main and daemon ARM builds pass.
Actual-service/guarded-memory tests pass on Mac/Linux/ARM, including Main's
patched lifecycle prologues. **Native46 staging failed its stopped indication; no daemon physical
mapping occurred. Native47 drained-state correction is compiling.**
Combined production wrapper/UIO/wire/C-endpoint simulation passes with
independent serial payload/CRC and broken mux/pause controls. Physical
coherence, packet bytes and full Cedar boot/delayed-Main delivery remain
unqualified. SPI/socket stays the default.

The frame-only pattern follows the reviewed local Mac/Sun implementations.
It preserves the original DskEth/controller and Manchester/CRC machinery;
neither endpoint supplies receiver status, a fabricated CRC, a modern NIC
register model or guest instruction completion.

Reserve ARM physical `0x1FF00000..0x1FF0FFFF` (64KiB). MiSTer14 reports
System RAM ending at `0x1FEFFFFF`; its boot arguments reserve the upper513MiB.
This window is below Main's boot mailbox at `0x1FFFF000`, the framework
scaler at `0x20000000`, Main framebuffer at `0x22000000`, and Dorado's
optional raw-DDR backing at `0x30000000..0x30FFFFFF`. The mapping layer
rechecks `/proc/iomem` before opening `/dev/mem` and rejects overlapping RAM,
other driver claims and hidden/missing maps. No physical DDR is accessed by
the current unit tests. This first wrapper uses SDRAM for raw guest storage,
freeing the framework DDR port for frames without a new arbiter.

The header uses8-byte spacing, with32-bit little-endian producer/consumer
words and reserved high halves. Exactly one actor owns each runtime index.

| Offset | Field | Writer |
| --- | --- | --- |
| 0x00 |64-bit magic `0x31474E49524F4444`, bytes `DDORING1` | FPGA, published last |
| 0x08 |64-bit Main-issued epoch, nonzero | FPGA at initialization |
| 0x10 |32-bit TX producer | FPGA |
| 0x18 |32-bit TX consumer | daemon |
| 0x20 |32-bit RX producer | daemon |
| 0x28 |32-bit RX consumer | FPGA |
| 0x30 |32-bit status:0 stopped,1 running,2 fault | FPGA |
| 0x38 |64-bit ABI `0x0001080000100008`: version1,2048 bytes,16RX/8TX | FPGA |

TX descriptors start at0x100 (8 entries); RX descriptors at0x200 (16).
Each descriptor is16 bytes:32-bit length,32-bit sequence,64-bit epoch.
TX data starts at0x1000, RX data at0x5000. Each slot holds the full2048-byte
payload with no inline header. The used region ends at0xD000. Payload byte0
is byte lane0 of its first64-bit DDR beat; it remains the first network
byte. The RTL swaps bytes within its16-bit port words to preserve that order.
Lengths must be even and4..2048, matching the qualified raw-frame boundary.
No Manchester preamble, hardware CRC or receiver status crosses the mailbox.

Indices are free-running unsigned32-bit counts. Occupancy is producer minus
consumer modulo2^32 and may not exceed the ring depth. Slot is count modulo
depth. A descriptor's sequence must equal the consumer count and its epoch
must match the grant. Payload and descriptor precede producer publication;
consumption precedes consumer publication. Full/empty leave ownership intact.
Malformed metadata or impossible indices stop the endpoint instead of
wrapping into another slot or silently consuming an invalid frame.

The daemon C library uses aligned32-bit volatile MMIO, `dmb sy` ordering on
ARM and `dsb sy` completion before a stop acknowledgement. The proposed map
must use the reviewed `/dev/mem`, O_SYNC, MAP_SHARED uncached/device pattern;
ordinary cached memory is not a supported hardware mapping. Tests use a
guarded software memory oracle, not proof of physical coherence.

The implemented lifecycle must pass its remaining hardware gates:

1. Main revokes the current daemon grant; the daemon stops in its sole
   service thread, completes MMIO and acknowledges STOP before core reload.
2. Main deasserts the FPGA engine's enable and waits for `stopped`. An
   already-started frame completes its payload and publication/ACK or
   commit/consume tail; an accepted DDR read drains. Magic is cleared and
   stopped status accepted before `stopped` asserts. A timeout refuses the
   reload; it must not turn into permission to continue programming.
3. Main programs/identifies the core and validates a fresh SPI capability.
   It issues a fresh nonzero epoch with enable initially low, then starts
   the engine and waits for completed initialization. Main grants that
   exact mapped window/epoch to the daemon only afterward.
4. The daemon binds only under that explicit exclusive grant and validates
   magic/ABI/status/epoch. Stale magic after FPGA reload cannot authorize
   memory access. Header checks alone cannot close a race with an
   unsynchronised core replacement. Neither endpoint automatically resumes
   an old grant; the RTL rejects reusing its last epoch after a drained stop.

The RTL engine runs from the ungated host clock. Its reset input requests a
drained stop; it does not discard an accepted DDR response. An enable-low
rearm and new epoch are required after reset/fault/generation change. The
framework's whole-FPGA reconfiguration remains a separate lifecycle boundary.
`stopped` means DDR transport is quiescent; it is **not** a complete guest or
wire-device snapshot barrier. Snapshot milestone4a must also save/drain the
wire, queues and server session state.

## Main control and capability negotiation

Command70 flag bit3 advertises the optional DDR interface. Command74 returns
magic D0D4 followed by17 words: version1; physical base low/high; size low/high;
flags (stopped, initialized, enable, fault, owned in bits0..4); fault low/high;
epoch in four low-first words; max bytes2048; TX slots8; RX slots16; control
errors low/high. Runtime fields are captured together at command selection.
Command75 is exactly six words including the command: mode0 STOP/1 START,
then four low-first epoch words. It commits only on deselection. Partial,
overlong, invalid-mode and reused/zero START epochs cannot change ownership.
START requires the previous engine to be stopped with enable low. Ownership
stays with DDR until its physical accesses and publication tail have drained;
commands71..73 cannot consume or submit frames while DDR owns the port.

The daemon's optional same-UID,0600 Unix-stream control socket defaults to
`DORADO_NET_SOCKET + ".control"`; `DORADO_NET_CONTROL` overrides both ends.
`DORADO_NET_DDR=1` enables negotiation in Main and the control listener in
the daemon. Daemon startup alone never maps memory. Each32-byte record is
little-endian: magic64 (`DDCTRL1\0`), command32, sequence32, epoch64, base32,
size32. Commands are STOP1, RESUME2, STATUS3. Replies carry command|0x80000000,
the exact sequence, current epoch, errno32 and active32. Duplicate identical
requests replay their ACK without repeating mapping/reset; stale or altered
sequences disconnect/quiesce. A live second controller cannot replace its
owner. Old-owner EOF is processed before accepting a replacement connection.

Main runs a bounded nonblocking STOP/drain/fresh-epoch/initialize/RESUME
state machine. It verifies ongoing FPGA and daemon ownership; faults hold
packet service rather than silently handing frames to SPI. Programming,
reexec, reboot and initialization require a daemon STOP ACK followed by an
actual FPGA stopped observation. Late/failed ACKs, cleanup errors or a drain
timeout refuse the action and retain retry state. SIGTERM/SIGINT use Main's
poll-thread disk and network barriers; no SPI occurs in a signal handler.
An inherited enabled FPGA lease is stopped even if the new Main asks for
SPI. An older core without bit3 retains the existing packet transport.

Tests additionally include:

```
make -C fpga/mister/server ddr-control-test ddr-map-test
python3 fpga/mister/main/tests/test_ddr_transport.py --main-source /path/to/patched/Main
```

The second command compiles the actual loader/reboot/reexec/init prologues,
replacing their first hardware action with a counter. The real manager,
client and C daemon service must prevent that action on late STOP, undrained
FPGA and disk errors. Guarded RAM substitutes only for physical mapping;
SPI is an independent ordering oracle. Dynamic ARM runs use Main's production
runtime. A static ARM test reached cleanup then threw in test-thread joining;
that configuration is retained as a failed test, not qualified. Evidence:
`reports/2026-10-18-ddr-network-control/` and `2026-10-18-native46-ddr-network/`.

Tests:

```
make -C fpga/mister/server ddr-ring-test
python3 fpga/mister/server/test_ddr_ring_rtl.py --mutations
```

The first test passes on Mac, Linux and actual MiSTer ARM, with sanitizers
on Mac. The second links the real C endpoint to Verilated RTL with independent
DDR/registered-wire-buffer oracles, random wait/read latency, zero-latency
responses, both full rings, starting-at-zero and count-wrap cases,2048-byte
frames, pending-read/stalled-write and mid-frame stop, reset/new grants and
invalid descriptors. Broken byte order, epoch validation and stop completion
controls fail. Evidence: `reports/2026-10-18-ddr-network-endpoints/`.
