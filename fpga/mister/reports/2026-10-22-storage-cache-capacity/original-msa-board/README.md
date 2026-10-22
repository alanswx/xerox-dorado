# Buffer capacity: complete original MSA board

Exact qualified native9e storage group, host address sampling, ready reduction, address preview, arbiter, BL1/BL8 sources and cells are hash-bound to the reviewed source tree. The original one-board144-chip oracle runs at57.142857 MHz,16 groups,64K addresses per group, prefetch/interleaving enabled. Unwritten physical cells are explicitly zero in this deterministic fixture, not a hardware assumption.

Both4- and16-line backing buffers pass12012 complete-board cycles,1111 output transitions and all9437184 stored bits identical. Both use750 physical reads,7872 writes and113899 host cycles. The complete raw transaction traces are byte-identical, so this workload shows no speed improvement. The separate synthetic group-sweep improvement cannot be generalized to actual instruction speed.

No production source or running FPGA changed. The full128-group native firmware, all-device physical fit/timing and pinned hardware comparison remain required before activation. Reproduce with tools/probe_native_storage_cache_board.py using the frozen qualified stage and preassembly qualification and a new output directory. Exact commands, all source inputs, raw traces and logs are preserved here. Verify with tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-cache-capacity/original-msa-board.
