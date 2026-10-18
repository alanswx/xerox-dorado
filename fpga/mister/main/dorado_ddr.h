#pragma once
#include <cstdint>
// Main-thread control only. Packet bytes and physical DDR access stay in the
// daemon/FPGA; Main owns every SPI transaction and lifecycle decision.
bool dorado_ddr_poll(uint16_t packet_flags);
bool dorado_ddr_barrier(const char *reason, unsigned timeout_ms=1000);
bool dorado_ddr_reset();
bool dorado_ddr_needs_barrier();
