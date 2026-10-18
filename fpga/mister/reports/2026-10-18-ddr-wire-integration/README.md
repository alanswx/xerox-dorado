# Combined production packet wiring, DDR and serial wire qualification

Production passes10,038,206 host clocks with1129 DDR reads,2166 writes,
810 held requests and63 immediate read responses. The wrapper's packet
instantiations, mux and falling-edge enable are extracted verbatim from
Dorado.sv; only CPU diagnostics/unrelated devices are outside this fixture.
The real C ring endpoint exchanges bytes with the actual RTL adapters.

An actual DskEth-captured FileSystem request and its literal7c50 CRC enter
through the physical serial boundary. Tests reject corrupted CRC, transfer
full2048-byte packets, preserve an initially SPI-pending packet on DDR grant,
isolate legacy packet commands, stage host frames while guest time pauses,
retain mid-frame serial phase, drain STOP without losing a committed wire
frame, restore SPI readiness and restart with a new epoch. The emitted
payload and CRC are checked by an independent midpoint sampler/CRC oracle,
not only by looping the adapter back into its own decoder.

A reversed RX-write ownership mux fails the payload/CRC oracle. Making the
wire always advance fails the paused-wire check. Neither mutation changes
the unit-test oracle. The fixture contains no CPU or DskEth board; existing
original-board receive/CRC and clock tests remain separate gates. The DDR
memory is an independent randomized-latency/stall model, not physical DDR.
Native46 production sources are unchanged, so its frozen timing applies.

Reproduce: python3 fpga/mister/server/test_ddr_wire.py --mutations
