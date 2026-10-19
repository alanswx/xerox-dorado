# Explicit RIOT row decoder: no area benefit

REJECTED_NO_AREA_BENEFIT. The qualified shared-port implementation and explicit
128-row write decoder both use 782 ALMs and 1,323 shell registers. Every
synthesized RIOT retains all 1,097 physical plus 21 private interface bits.
All four timing conditions pass, minimum +0.167 ns; constraints are identical.
The source/constraint/macro/hold/warning controls and both lost-bit controls
reject damaged evidence. There is no benefit to justify core integration.

The rewrite exposes static row enables while retaining the exact read port,
single-edge reset, reset priority and 133-word state layout. The unchanged
original/independent oracle passes 54,701 host edges, 4,448 held edges, 4,109
accepted requests, 2,593 writes, all 1,097 bits/all 256 indices/all 128 RAM
words/four prescalers, destructive restore, first-edge continuation and
20,000 further cycles. All six damaged variants reject. Fresh XML accounts
for all physical/interface bits with no sequential exclusions; seven damaged
inventories reject. This qualifies the actual init_7800=0 binding only.

The matched job SnapshotRIOTRowDecode_20261019 on misterubuntu and local
session23489 are terminal. No whole-BaseBoard rerun, full-core integration,
FPGA programming or speed change followed. The raw source/geometry/timing
qualification remains distinct from the decision to reject the optimization.

Classification selected generate_riot_row_decode in the unchanged
classify_riot_shared_port classifier at runtime. Fit auditing selected the
qualified shared-port generator as the baseline and the row-decoder generator
as candidate in audit_riot_shared_port, retaining all source, state-retention
and damaged-report checks. Exact generators, fixtures, original vendor bytes,
executed fit inputs, reports and audits are preserved. The archive manifest
and readback record cover 172 raw files; generated objects/databases are excluded.
