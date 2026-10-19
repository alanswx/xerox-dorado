# Complete modeled IFU state qualification

The frozen IFU adapter accounts for 28,653 mutable modeled bits: 974 FF bits,
27,648 IFUM bits, 27 raw read-pipeline bits and four board phase bits. Its
32x8 PROM remains immutable. Eleven independent address groups retain widths
1/5/1/1/4/1/5/2/1/4/2; all original CE omissions are preserved. This is
two-state model qualification, without a new zero tie or four-state proof.

Final simulation compares 79 inputs/83 outputs with the untouched original
and an independent RAM oracle: 498,875 host edges, 458,884 held edges,
448,849 requests, every state bit, all 262,144 address encodings, all original
RAM writers, destructive continuation/restore, first resume, recapture and
20,000 subsequent cycles. Seven faulty RTL variants fail. Fresh XML coverage
accounts for all modeled state and 256 immutable bits; 14 damaged inventories
or schemas reject.

Matched positive-edge 50 MHz launch/capture fits use identical 20 ns clock,
2 ns maximum/0 ns minimum virtual I/O, with no timing cuts. Both variants
pass all four operating conditions; snapshot minimum slack is +0.157 ns.
Physical IFUM bits remain 27,648; baseline packs into four M10Ks, while
independently writable snapshot groups require eleven. Resources change
466 -> 1,474 ALMs and 867 -> 1,832 registers, including shell, administration
and previously pruned state. All eleven snapshot macros and physical RAM
rows retain OLD_DATA. Nine damaged-fit controls reject. These are isolated
figures, not an enabled full-core resource budget.

Earlier attempts and their raw logs are preserved. The source-list fixture
first omitted parameterized SG10139, then needed the immutable PROM path.
The C++ fixture also needed to clear unused bits above its 79-bit stimulus
port: dirty padding leaked into scalar Verilator inputs. The correction
constrains the input container; it does not mask saved state or change RTL.
The final mutation expectation records invalid-address alias corruption
detected before the error response. The first fit auditor rejected a physical
table-label mismatch; the corrected auditor reads the same unchanged fit.

Raw text is gzip-compressed without content changes. Accepted simulation,
classification, exact executed sources, both fit variants and audit controls
are retained. Native54 full-core integration is separate and still pending.
No global barrier, whole-machine restore, 60 ns cadence or speed gain is claimed.
