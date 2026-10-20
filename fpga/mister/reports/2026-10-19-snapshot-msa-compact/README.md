# Compact MSA bridge: component qualified, native integration pending

Matched complete-MSA fit: **2,213 baseline -> 1,721 compact ALMs**, saving 492.
Both use the same 50 MHz registered shell, 991 virtual pins,2ns max/0ns min IO,
CycloneV device and four area-policy options. No shared gated clock is used.
All four device conditions pass setup/hold/pulse width, minimum +0.119 ns, zero
unconstrained clocks/IO; recovery/removal have no synchronous-fixture paths.
Five damaged fit/source/constraint cases reject. No assembly or deployment.

The bridge removes five actual implementation registers/group: saved_write1,
cache_addr16, saved_addr16, saved_data16, old_data16. They are redundant because
ready gating holds address/write_data throughout accepted raw transactions.
The new cache valid bit describes the current address; a combinational output
of the original row/column helper predicts actual changes on a guest edge.
Read-before-write is retained by delaying cache-data commit until the guest
samples the old word, after the write has completed. All remaining actual
registers are exported. A new snapshot schema is required; old snapshots are
not compatible. Original 866 FF/history bits, all original helper equations,
board pins/parameters and guest DRAM pipeline remain.

Physical per-MSA state becomes 1,650 bits/378 fields (866 FF/history + 784 bridge),
versus 2,690/458. Scan 92 words, last 12 bits, unit stride 256 integration pending.
Fresh XML covers all 1,650 candidate bits, all 2,690 unmodified reference bits
and 81private bits. None are sequentially excluded. Fourteen inventory/exact-alias
controls and nine independent source controls reject, including cross-group
binding with its damaged metadata rehashed. Guest process and original helper
bodies are checked against qualified committed receipts, not generated copies.

Frontend-v2 uses untouched original RTL and original DRAM-cell arrays:
62,316host /1,802,415checks /5,010guest /10,953requests /929writes /
14,704scan /9snapshots; pending reads/writes/write-done/reset and full1,310,720bit
raw-array comparison pass. Eight actual bad RTL cases reject. Complete MSA-v2:
996,995host /14,097,552checks /40,002guest /55,928requests /8,696writes /
558state operations /910,807scan edges. All new physical fields and original
outputs/raw traffic agree through full inversion/restore, first resumed edge,
20,000 additional advances, invalid bounds, abort and sticky quarantine.
Seven actual bad RTL cases reject. These are known reachable state comparisons;
no arbitrary old-schema or four-state representation equivalence is claimed.

Dedicated address tests seed 12,000 independently varied physical helper states
at ROW_BITS7/8 and compare predictions with actual original address-register
updates; equal-address captures are included. Four damaged equations reject.
This supplements board traffic that does not necessarily distinguish every
address group under the pinned random workload.

History: frontend-v1's first lost_guest_commit control actually changed the
reset commit; v2 names it correctly and adds the real guest-commit control.
MSA-v1's cross-group0/1 mutant survived; no qualification is claimed for that
run. MSA-v2 rejects an inverted-group mutant, while the independent source audit
rejects the exact original cross-group alias. First local fit preparation
attempts asserted on the shell's multiline port shape before making an output
directory. Prepared-v1 was never run (top-level generator imports would have
required remote dependencies); corrected-v2 puts preparation imports inside
prepare. The remote matched run is terminal; 40 non-QSF inputs remain unchanged.

All 666 raw members pass lossless readback. Extracted generation and fixture are
byte-identical; both original-RTL/DRAM tests rebuild and pass. Extracted source,
XML/alias and fit controls pass; address units replay. tools holds the Python
dependency closure and original cells. Replay also uses committed immutable
msa-ff/msa-board/msa-transport and msa-clocked-rejected receipt archives, named
in archive-replay.json. Objects/databases/executables are excluded; sources and
reports are lossless gzip; JSON over 100 KiB is gzip.

This saving does not prove complete-core capacity. The new eight-member schema,
SPI transport, native builder, real external-service barriers and disk-paired
complete FPGA snapshot remain pending. No measured speed improvement yet.
