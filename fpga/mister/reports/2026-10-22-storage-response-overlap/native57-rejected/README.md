# Native response overlap rejected: physical setup and hold

The complete first native fit and all supplementary reports are terminal.
All nine builder stages and five read-only follow-ups return zero, but the
actual31-check all-device summary fails setup at −0.363ns and hold at −0.041ns.
A successful Quartus command is not a timing pass. No assembly or hardware load.

The setup paths run from arbiter next_group and original group address state
through the live request selection and buffer lookup to rsp_data. The worst
hold path is storage_10 on msa2Odd: old_data[7] to read_pipe[7], a separate
storage-group path. Additional reports cover the real backend/group/arbiter
families across all four conditions, plus unchanged CPU, ready-gate, cache,
SDRAM pin, half-cycle capture and passive-counter checks.

The collector verifies all503 compiled sources and records the absence of
the builder, reporter and every child. Its1008-member archive is copied from
misterubuntu, independently decompressed and hash-verified. This1012-member
receipt preserves those complete sources/reports, the collector manifest and
verification helpers. Resources are33857ALMs,56013registers,332M10Ks and35DSPs.
These are inventories, not functional or timing qualification.

The serial candidate and all prior component results remain intact. A distinct
parallel per-line word-lookup revision is component-tested and fitting; see
[parallel-lookup-stage](../parallel-lookup-stage/README.md). Its new setup and
hold results remain pending. No timing exception, clock relaxation, fabricated
completion or deployed-core change is used to clear this failure.

Verify:
`python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-response-overlap/native57-rejected`.
