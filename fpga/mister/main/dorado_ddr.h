#pragma once
#include <cstdint>
// Main-thread control only. Packet bytes and physical DDR access stay in the
// daemon/FPGA; Main owns every SPI transaction and lifecycle decision.
bool dorado_ddr_poll(uint16_t packet_flags);
bool dorado_ddr_barrier(const char *reason, unsigned timeout_ms=1000);
bool dorado_ddr_reset();
bool dorado_ddr_needs_barrier();

enum class DoradoDDRSnapshotResult { Done, Wait, Invalid, Failed };
// Daemon lease only: does not acknowledge FPGA/device quiescence or authorize
// state access. Capture the controlled epoch, pause the daemon, then coordinate
// the separate guest/device barriers. Call Continue only AFTER FPGA state and
// DDR visibility have been validated and its network parser has been released.
// Calls and polling are bounded Main-thread operations; no packet/MMIO access.
// A failed lease stays pending and blocks generic lifecycle/reset. It requires
// coordinated recovery; these APIs never reconnect, STOP or discard it silently.
uint64_t dorado_ddr_snapshot_epoch();
DoradoDDRSnapshotResult dorado_ddr_snapshot_pause(uint64_t expected_epoch);
DoradoDDRSnapshotResult dorado_ddr_snapshot_continue(uint64_t expected_epoch);
bool dorado_ddr_snapshot_pending();
void dorado_ddr_snapshot_poll();
