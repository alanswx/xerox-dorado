# Rejected66.7MHz ALU candidate: terminal timing

The original builder607315 and all nine Quartus stages return0; read-only reporter607316 and collector607317 finish without retries/refitting. All503 source hashes remain unchanged and all940 downloaded terminal members verify size/SHA; outer archive77d8e3f0b09dbc820dc0c79769e0f1d60858d38b75eb1cf48a34652a25b2f85e. A successful tool exit means report completion, not passed timing.

Summary CPU setup-1.994ns and host hold-0.010ns reject this candidate. All negative targeted categories and their worst rows are preserved in timing-rejection-v1.json. The slow100C worst setup has moved to MemC k02 q[2] through the machine diagnostic read network to BaseBoard RIOT d_out[0] (15ns requirement,16.130ns data delay, negative skew). ProcL k22's slow-40C path improves to-1.787ns but still fails, now from MemX h05 q[3]. Other failed CPU/diagnostic/hold paths remain; this experiment does not close the whole device. The binary-pin carry equivalence proof remains valid but is not a native speedup.

No exception, timing repair, assembly or hardware load followed. The qualified57.1MHz candidate uses the original MC10181 cell. Further work must address real clock/capture and diagnostic/data-path timing, not treat the diagnostic mux as halted-only (HM4.10 permits programmatic strobes while running).

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-board-ready/alu-native66-rejected`
