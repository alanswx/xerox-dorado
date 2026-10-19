# Complete native MemD state adapter qualification

The exact frozen MemD-Rev-Ca board (679660eb) now has complete modeled
state access for the actual native STORAGE_INPUT_DELAY=0 configuration.
Nonzero delay explicitly fails elaborated simulation; it is not silently
accepted with unexported pipeline state. No running FPGA was changed here.

296,554 mutable bits: 968 FF/history bits, ten phase bits, 294,912 cache
RAM bits, 72 raw cache read-pipeline bits, 512 small-file RAM bits and 80
small-file write-window bits. Cache package grouping retains the reviewed
14 canonical groups and original connected CE/WE/output gating. Clock
histories, original phase initialization and cell parameters are preserved.
The state index is 20 bits (7 package/13 row), with a five-bit response
selector covering all 22 cache/small-file responses.

Accepted simulation is snapshot-memd-full-v3-20261019: 2,379,084 host edges,
2,179,091 held edges, 2,169,058 requests, all 296,554 state bits and the full
1,048,576-index space. All fourteen cache and eight small-file original
writers are exercised through normal board pins. Independent C++ old-data
RAM accounting and fixture-only physical probes check every cache lane/row;
probes bypass the state decoder to detect shared read/write aliases.
Destructive full-image writes, restore, recapture before guest advance,
the first resumed edge and 20,000 subsequent cycles pass against the
untouched reference. Ten broken RTLs and an unsupported delay configuration
fail for the expected reasons. Fresh original XML classifies every mutable
bit; fourteen damaged inventories/schemas reject.

Matched isolated 50 MHz fits use identical negative-edge external input
staging and positive-edge launch/capture, preserving the 20 ns embedded
budget and the original SDC. Both variants pass all four timing conditions,
no illegal/unconstrained clocks/I/O, minimum snapshot slack +0.164 ns.
Cache RAM is retained at 294,912 bits: baseline 38 M10Ks, adapter 40 M10Ks.
All fourteen adapter macros and actual physical banks retain OLD_DATA.
Baseline -> adapter: 604 -> 2,267 ALMs; 1,353 -> 2,340 registers. These shell/
admin/pruning counts are not an enabled whole-core snapshot resource budget.
Nine damaged fit/source/timing/physical-RAM controls reject.

The initial isolated baseline failed parsing because the frozen original
uses a SystemVerilog loop-variable declaration in its optional delay branch.
It is preserved under fit/SnapshotMemD_20261019. The accepted fresh project
SnapshotMemDSV_20261019 parses the byte-identical original as SystemVerilog,
as the native project does; cell RTL and timing constraints were not relaxed.
The exact initial builder and the accepted preparation's older test-driver
dependency are preserved separately with their recorded hashes.

Earlier simulation/classification attempts are retained as historical
records; accepted final sources and artifacts are under accepted/ and
executed-sources/. All raw text is gzip-compressed unchanged. No complete
machine barrier, device/ARM coherence, actual FPGA restore, original 60 ns
clock migration or speed increase is claimed. Full-core integration is next.
