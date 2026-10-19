# Actual Main worker delay: completed keyboard command and mouse

PASS on Native48/Main3781. The immutable55bc9a8c probe runs on the actual
Main worker, waits4200ms, then successfully fsyncs a duplicate of mounted
image source FD20. It writes no image bytes. This is delayed worker flush
work, not a4.2s kernel syscall. The DSO must remain resident until Main exits.
Main remains the sole SPI owner; the normal daemon retains direct DDR.

`echo worker48` was fully typed before arming; Return and mouse40,20 were
injected after actual worker begin. During8489411081881..8493611215385ns,
8 read/28 write ACKs complete, no poll gap above20ms overlaps, and four
strictly interior Main samples advance4,139,265 issues with CRC/RX/abort/
pack faults zero. Inner TX-drop counts remain15; this does not establish
zero drops over the entire boot or qualify an independent network exchange.

Capture1 completes at8492953743048ns, at least657,472,337ns before worker end.
Direct review against the pretyped image confirms a separate new worker48
output line, a following new prompt and the pointer moved lower/right.
Capture2 completes after the worker; its identical image is excluded.
`visual-review.json` pins the chosen image and pretyped hashes.

`qualify.py` checks actual timing, file completion, poll/error coverage,
instruction progress, read/write ACKs and the reviewed capture boundary.
`check-controls.py` accepts the real case and rejects five altered cases for
their intended reason: late capture, wrong reviewed capture, overlapping
poll gap, CRC fault and missing concurrent disk reads. To rerun, decompress
disk-prefix.jsonl.gz beside these scripts, then run both with python3.

Native48 idle and base-loader performance were measured before this DSO
was loaded. A fresh Main is required for subsequent clean comparisons.
This closes completed input during controlled worker delay; sustained
operation, complete snapshots and final repeated boot gates remain open.
