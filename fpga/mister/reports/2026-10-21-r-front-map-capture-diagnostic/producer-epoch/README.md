# Visible R precedes the retained B-selector capture

Both V21 producer probes finish 14 million host samples, pass the existing
original Initial checks and emit all 6,000 phase records. Map writes and
control/clock/address records align without a sample offset. The candidate
still has 8,191 map differences and 22 incorrect writes in the sampled window.

At sample 9,393,607 both models have the same R-visible clock (`00`) and
B-selector clock (`11`), and both still select R (`BMuxL=001`). The baseline
retains R=1; the central candidate already exposes R=0. At 9,393,608 both
selectors change to T (`BMuxL=010`), making B=1 again, but MapBuf has already
captured zero. The candidate's dR also becomes zero one sample earlier.
The exact trace and preceding window are in the candidate's logical
`map-producer-comparison.json` member. These observations isolate a mixed
capture epoch between the collapsed R path and retained B-selector model.

V22 tests that boundary by restoring the four original visible-R latch
packages, ProcH/ProcL g06/g07, from the preserved MC1668 native wrappers.
They implement 16 physical R bits using the original clock, select and data
connections and byte-identical MC10173 helper. No bus or map data is forced.
The source audit proves that only those four package blocks change; its
observer is identical after output-path normalization. The central predicted
R state remains an internal diagnostic duplicate, so this is a hypothesis
test, not a reviewed production storage representation or clock fix.

V22 was verified live at collection; its log here is a running prefix.
The extension preserves 1,336 members in 447 blobs: 28,536,230 raw bytes,
3,936,484 stored bytes. Earlier receipt members remain immutable.

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-front-map-capture-diagnostic/producer-epoch
```

No production RTL, FPGA, Main, daemon or guest volume changed. Hardware
recovery, speed improvement, rollback and release acceptance remain open.
