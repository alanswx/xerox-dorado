# Correct the line observer; preserve its stopped output

The V16 observer's xxxxE10 mask also matched idle I/O VAfe1d. It emitted repeated
irrelevant records during Initial's boot wait. The exact live test processes
7072/7220 were verified by command and stopped deliberately with SIGTERM;
both are terminal-15. This is an instrumentation failure, not a guest failure
or response to a polling timeout. Q capture V3 continued unchanged.

Full observed output remains locally as `test.log.gz` in each V16 directory.
`compress_closed.py` checked terminal status, streamed each raw log to gzip,
verified decompressed SHA/length and unchanged source mtime, then removed only
the redundant raw file. Receipts bind1,471,393,267 and528,458,759 raw bytes to
32,244,785 and11,354,955 gzip bytes. No records were discarded; the large logs
are preserved locally rather than added to git. The parent archive's earlier
bounded prefixes and frozen sources remain unchanged.

V17 restricts accesses to the boot image's four lines at0e10/1e10/2e10/3e10 and
retains the32-sample tail. An explicit200,000-row limit stops the diagnostic
with a named observer-limit error rather than silently producing excessive
logs. Full original Initial and loaded IM/IFUM checks remain enabled. No guest
state, RTL, hardware clock, Main or daemon changes.

Both V17 arms compile and were verified live as9099/9203. `corrected-inputs.json`
binds their exact commands and provenance. `capture_corrected_inputs.py`
verifies every frozen file and proves only the bench and observer helper differ
from archived V16:419 control/434 candidate inputs remain identical. Changed
inputs plus complete provenance/compile logs and running-state prefixes are
preserved here with lossless readback. These are not terminal results.

Resume the V17 pair and independent Q3 through actual process/run-state
inspection. Do not restart on an observation timeout. The CPU handoff regression
and prior passing native baseline/RAM checks remain as documented in the parent
receipt; Cedar recovery and speed qualification remain open.
