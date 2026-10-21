# Original visible-R latches restore the complete Initial map

V22 finishes 14 million host samples, passes the existing original Initial
checks and matches the native V21 baseline exactly: all 16,384 map entries
(including flag planes) and all 256 RM words agree. All 6,000 sampled R, B,
BMux, MapBuf and sampled-write-data records agree, and all 23 actual map
writes are correct. The central candidate had 8,191 differing map entries
and 22 incorrect writes in this window.

Only the original ProcH/ProcL g06/g07 visible-R latch packages are restored:
16 physical R bits, original clock/select/data connections, byte-identical
MC10173 helper. The source audit verifies every executed input and proves
exactly those four package replacements; the observer is identical after
output-path normalization. This confirms the failing R-to-B-selector capture
epoch identified in the [producer comparison](../producer-epoch/README.md).

The logical V22 member `restored-r-map-comparison.json` records the full
comparison against native control V21, whose inputs/logs are preserved in
the producer receipt. `visible-r-source-audit.json` records the changes.
All original guest instructions and map writes execute normally.

V23 applies the same diagnostic change to the full 55-million-sample original
image readback/handoff gate. It was verified live at collection; the log here
is a running prefix, not a checksum, handoff or desktop result. The central
predicted R remains an internal duplicate, so neither test establishes a
reviewed production state representation or its complete parity/fault contract.

The extension preserves 907 members in 435 blobs: 23,101,234 raw bytes,
3,530,064 stored bytes. Verify all archived bytes:

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-front-map-capture-diagnostic/visible-r-map-closed
```

No production RTL, FPGA, Main, daemon or volume changed. Original-image
qualification, native Cedar recovery, speed improvement, rollback and final
release acceptance remain open. Complete FPGA snapshots remain deferred.
