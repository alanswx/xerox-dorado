# Complete snapshots: first generated state inventory

This begins milestone4a. **No online FPGA save/load is implemented.**
`tools/sil_state_inventory.py` walks the unflattened Verilator XML hierarchy
and records per-instance assignment destinations with bit widths, array
geometry and source locations. It uses logical paths, not C++ offsets.
The XML hash identifies this elaboration; it is not a cross-build schema.

The archived elaboration uses the Native45-style SDRAM generator fixture:
four64K-chip storage pairs,64K map, deferred reads, group interleave,
response-boundary arbitration, M10K F10415 and logic F10145. It is not an
attestation of the frozen45 FPGA build. Sources/macros/command are recorded.

4,158 native instances contain8,900 sequential candidates (2,109,812 bits),
4,405 initial-only candidates including ROMs (87,421 bits),13 subroutine
review candidates (335 bits), and24,204 combinational-review candidates
(43,807 bits). These conservative counts are **not snapshot size or FPGA
resource estimates**. Many reviewed candidates will be wires/constants;
external state is absent. Every candidate is UNCLASSIFIED until explicitly
designated saved, drained, immutable or reconstructible with evidence.

Important coverage details:

- Array indices are reads, not destination registers. Concatenated and
  selected destinations resolve to their base storage.
- Edge-sensitive blocking captures count, as do nonblocking assignments
  and transparent-latch candidates. Blocking combinational assignments stay
  in review so incomplete-assignment latches are not silently discarded.
- Runtime task/function writes are inventoried. The real SDRAM `rows`
  updates occur inside `activate`, so ignoring helper tasks misses state.
- `$readmemh`/`$readmemb` loads count even without an ordinary assignment.
- Original F10415 memory and `dout_r` both appear. Deferred raw frontend
  `old_data`, pipeline address/known flags and pending write state appear.
- Signed array bounds use the constant's signed dtype, including Verilator
  hexadecimal negative values whose printed spelling omits the `s`.
- Unsupported type/destination/scope forms fail rather than silently drop
  coverage. Port aliases, primitive state, call side effects, inferred latch
  behavior and actual compiled wrapper scope still require independent review.

`python3 tools/test_sil_state_inventory.py` elaborates a real fixture plus
the original F10415 cell. It checks two hierarchy instances, synchronous
read pipeline, blocking/edge/latch captures, helper-task state, ROM loads,
concatenated writes, array-index exclusion and multidimensional signed
geometry. Removing the original read-pipeline writer changes coverage;
an unresolved state destination is rejected. Test passes.

Outside this inventory:16MiB physical raw SDRAM, Dorado.sv's clocks/phase/
gates, key/network queues, video/scaler pipelines, Main disk/cache/worker
state, disk generation, and daemon Pup/BSP/STP sessions/history/timers.
Physical SDRAM command/refresh state must be safely drained/reconstructed
while refresh continues. Do not restore stale physical open-row state.
Complete snapshot acceptance requires inventorying/classifying those too,
a coherent host/guest barrier, bounded state access preserving BRAM read
pipelines, measured area/routed timing, checksummed compatible bundles,
pending/reset/error tests and an actual Cedar desktop restore. No restore
time or resource promise is made by this inventory.
