# Raw SDRAM power-on diagnostic: component evidence

The hardware sequencer writes/reads low-address patterns forward, writes high-address patterns and reads backward, then writes/verifies zeros. Only a complete pass releases native storage; a mismatch retains its first address, phase and values. It tests the 12 consumed raw bits and all 23 raw address bits before guest execution.

The bounded chip-pin test passes 98,304 transactions with request/response stalls; injected return corruption, low/high address aliasing and bad zero readback are detected. Removing the comparator is rejected at runtime. The full-address run passes 50,331,648 requests/completions across 8,388,608 raw cells through the unchanged BL8 backend and independent SDRAM chip model. The first full-address attempt hit the smaller bench deadline and is preserved as failed; the second changes only the deadline and passes.

The 501-source native stage reconstructs the qualified MC1668 parent plus the reviewed BL8/observer changes and the explicit pre-guest test. Actual synthesis uses the top, wrapper, backend and self-test. A verified live fitter prefix is included; full timing, assembly, FPGA memory integrity, Cedar boot and speedup are pending.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-storage-selftest/component`
