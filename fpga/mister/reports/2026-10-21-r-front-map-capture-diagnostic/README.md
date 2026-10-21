# Original Initial map writes expose a CPU data capture regression

The matched native CPU comparison now identifies a failed map-data capture.
Both V19 runs execute original Initial through 14 million host samples and
pass its existing ROM/task/error-enable checks. Across the 6,000-sample map
window, all map control, clock, address and Hold records match. The baseline
writes real pages 0 through 22; the candidate writes zero for all 23 entries.

The first nonzero page makes the boundary visible:

| Host sample | Baseline B / MapBuf data | Candidate B / MapBuf data | MapBuf gate |
| --- | --- | --- | --- |
| 9,393,606 | 1 / 1 | 1 / 1 | Open |
| 9,393,607 | 1 / 1 | 0 / 1 | Open |
| 9,393,608 | 1 / 1 | 1 / 0 | Closed |
| 9,393,644 | Map RAM samples page 1 | Map RAM samples page 0 | Closed |

The raw 18-bit BMux/MapBuf values include two check bits: page 1 is `00006`,
page 0 is `00003`. The bad value is already held in MapBuf before the actual
DRAM write. This establishes the component's CPU-to-MapBuf failure; it does
not yet establish the complete cause of the FPGA's later Mesa trap loop.
The producer responsible for the brief B-bus change remains under investigation.

V18 independently compares the actual register/map arrays at sample 13,900,000:
all 256 RM words agree, but 8,191 map rows differ. All differing real-page
fields are zero in the candidate; 256 other-plane fields also differ. These
are raw DRAM row/column addresses, not linear virtual page numbers. In the
4,000-sample storage window, VA, Hold, RAS/CAS and write strobes match, while
both complete DRAM latched-address vectors differ in every sample. Shared
deployed I/O, memory and IFU sources and original external inputs are verified.

The full 50-million-sample Q3 shadow probe passes all 91 packages, with at
least 2,107,861 settled comparisons per package. This open-loop result does
not cover all transient board-to-board captures, as the map trace demonstrates.
V17's original-image control passes; its candidate retains 3,856 mismatches
and no Cedar entry. Earlier receipt prefixes remain unchanged.

V20's expanded operand observer failed compilation before simulation: an
overlapping token replacement generated an invalid hierarchical net. Both
failed preparations and logs are preserved. V21 corrects the observer, compiles,
and adds R/dR/dRm, bypass, R read/visible clocks, B-mux clocks/selects and root
clock. Its collected logs are **verified live prefixes**, not terminal results.
Inspect the exact existing process and run-state before taking further action.
No production RTL, microcode, PLL, Main or daemon change resulted.

The archive preserves 4,831 logical members in 494 deduplicated blobs:
58,331,547 raw bytes, 5,938,596 stored bytes. It includes terminal Q3/V17/V18/V19,
failed V20, live V21 prefixes, original EB/replies, exact executed sources,
helpers, comparisons and a read-only MiSTer14 process identity observation.
It excludes FPGA images, executables, databases and disk packs.
Earlier analyzer iterations are superseded by V18's `map-comparison-final.json`
and V19's `map-capture-comparison.json`.

Verify every archived byte:

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-r-front-map-capture-diagnostic
```

Replay both analyses using only extracted frozen receipt files:

```
python3 fpga/mister/reports/2026-10-21-r-front-map-capture-diagnostic/replay_analysis.py
```

The replay checks the full resulting analyses and every consumed evidence hash.
It does not rerun Verilator or qualify native hardware.

MiSTer14 remains RBF `57a552af` at 50 MHz/SYSPER4. The ordinary-instruction
ceiling is 6.25 million/s; the target is 16.667 million/s before waits.
The last qualified Cedar login measurement remains 1,947,686.01 issues/s.
Same-clock Cedar recovery, acceleration, authentic Checkpoint/Rollback,
persistence, three fresh boots and sustained operation remain open.
Complete FPGA snapshots remain deferred; the goal is active.

Follow-up: the [closed producer comparison](producer-epoch/README.md) identifies
R becoming visible before the retained B-selector capture. The
[original-R diagnostic](visible-r-map-closed/README.md) restores the original
four visible-R latch packages and now matches every map/RM word and every
sampled R/B/MapBuf record. Its longer original-image test remains in progress;
no production fix or hardware qualification is claimed.
