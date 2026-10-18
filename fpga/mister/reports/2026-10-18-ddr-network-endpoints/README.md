# Direct DDR network endpoints: unit qualification

C ring endpoint and standalone RTL engine pass interoperability tests.
**Not integrated, fitted or deployed.** MiSTer14 continues using SPI/socket
for Native41d. These results do not complete milestone4 or prove a speedup.
Protocol: `fpga/mister/dorado-ddr-protocol.md`.

The proposed64KiB aperture at0x1FF00000 avoids the observed Linux RAM,
framework framebuffer/boot windows and Dorado raw DDR. The recorded memory
map is read-only; no physical DDR is mapped/written by these tests.

C endpoint:16,384 byte-oracle attempts pass on Mac, Linux and actual MiSTer
ARM. Mac ASan/UBSan passes. Full/empty/wrap,2048-byte frames, neighboring
bytes, descriptor sequence/epoch and malformed/foreign ownership fail
closed. Quiesced access to a PROT_NONE mailbox is not attempted. The pinned
GCC10.2.1 ARM binary contains `dmb sy` ordering and `dsb sy` stop completion;
disassembly and source/binary hashes are archived.

Actual RTL/C endpoint, independent DDR/registered-wire-buffer oracles:
128 frames each direction from counts0 and0xFFFFFFF0,514597 host clocks,
18739 reads/20449 writes,9735 held commands and1056 immediate responses.
Both rings become full;66097 host RX-full retries. Packet bytes/order/lengths,
2048-byte boundaries,32-bit wrap and index byte enables agree. Read latency
is0..17 clocks. Pending-read/stalled-write stops, reset and epoch changes
drain and invalidate magic; old grants cannot automatically restart.
Mid-frame2048-byte TX/RX stops finish payload and publication/ACK/commit/
consume tails. Malformed descriptors/counters cannot commit or ACK a wire
packet. All FPGA reads/writes are checked against the owner regions.

Broken TX byte order, missing RX epoch validation and premature stop controls
fail as expected. Strict lint passes with only the repository's intentional
power-up initialization warning disabled. Reproduce with protocol commands.

Two fixture corrections remain recorded: the initial completion check read
the RX consumer after wire commit but before DDR publication; it now waits
for actual publication. The epoch mutation was caught earlier by an
unsolicited commit than the initially expected diagnostic; the control now
requires that specific failure. Neither is claimed as a production defect.

Remaining: acknowledged Main/daemon lifecycle and exclusive grants; fresh
SPI capability negotiation; mapping validation/physical DDR coherence;
wrapper/fallback ownership; original wire/CRC and real server payload tests;
full-core fit/all-device timing; Cedar cold boots and delayed-Main packet/
sustained tests. Transport stopped is not a complete snapshot barrier.
