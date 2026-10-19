# Terminal all-enabled asynchronous register-file RAM fit

REJECTED_CAPACITY: 45,228 ALMs / 41,910 available, 3,318 over capacity;
4,570 LABs / 4,191. Synthesis succeeds, fitting fails with errors 170012 and
11802. Builder 1931650 and fitter 1933677 are terminal/absent. No STA,
assembly or hardware programming followed. Live Native59 is unchanged.

This saves 3,125 ALMs versus the shared-RIOT/shared-writer logic candidate
(48,353), and 9,175 versus the first all-enabled candidate (54,403). Every
board remains enabled. The synthesis/failed-fit hierarchy is diagnostic
attribution, not legal physical placement, routed timing or a prediction
of the next fit. The largest mutable child family remains the five RIOTs
(3,394.2 ALMs of failed-fit attribution); board-own decode/response logic
and other leaf families remain possible area targets.

The exact actual-source audit passes all 413 baseline receipt files,
2,137 board pins and 17 damaged integrations. Original clocks, SDRAM/ECC
and all timing policies remain unchanged. The zero-private-write-response
helper and its isolated physical qualification are in the separate report
../2026-10-19-snapshot-f10145-zero-async.

The independent synthesis mapping auditor derives 85 expected register files
from all eight board manifests. It finds exactly two inferred MLAB read
replicas per package. Every administrative replica retains all 16 x 4 bits:
5,440 logical RAM bits across all 85 files. Twelve ordinary-read replicas
have fewer output lanes because corresponding pins are absent from the
actual compiled board instantiations. The auditor checks each width against
those exact connected output pins, rather than accepting arbitrary pruning.
There are 170 inferred replicas totaling 10,480 bits, including those narrower
ordinary replicas. All memory depths/modes/types and absent initialization
files are checked. Seven damaged mappings reject. This does not classify
window/control register merging, establish physical placement or prove timing.

All 29 raw files pass lossless hash readback, including exact complete map/fit
reports, logs, summaries, source-integration and capacity audits, and the board
manifests/sources used by the mapping check. Sources/reports/logs and JSON over
100 KiB are gzip-compressed without byte changes. Extracting the archive and
rerunning the independent mapping audit also passes. Database/generated object
files are excluded. Continue area reduction without dropping state, then qualify
whole-core timing and remaining support/ARM/disk snapshot coherence.
