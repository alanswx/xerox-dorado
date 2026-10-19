# F10145 zero private write replies and asynchronous MLAB placement

Primitive logic, fresh state classification, all eight affected positive board
restore fixtures, and an isolated matched asynchronous-MLAB fit PASS. This does
not qualify whole-core timing, original-firmware boot or a physical snapshot.

The private administrative write response now returns zero; the public fabric
already returns zero. Administrative reads explicitly exclude writes. Ordinary
asynchronous output, captured WE/address/data/CE window, RAM contents and state
reads retain their behavior and complete 74-bit state layout. No guest-cycle
latency, clock period or SDC exception is added. The normal and restore writers
continue to share one mutually exclusive write port.

Simulation v2 passes 35,407 host edges, 7,987 held edges, 326 administrative
requests, every physical bit/address, pending-write save/restore, destructive
continuation, first-edge commit and 20,000 further cycles. Ten damaged RTLs
reject, including overlapping administrative reads/writes and a nonzero private
window-write reply. The original C++ oracle is retained unchanged beside the
derived copy adding the zero-write-response check. Earlier simulation v1 and
its exact executed inputs remain as history.

Fresh XML classification v2 used the qualified classifier with its candidate
module explicitly selected at runtime. V3 adds a dedicated reproducible command,
`classify_f10145_zero_write.py`. Both account for all 74 physical plus 13 private
interface bits, without sequential exclusions; seven damaged inventories reject.
Both generated source trees and the current wrapper provenance are preserved.

The eight unchanged positive board fixtures pass: ContA, ProcH, ProcL, MemC,
MemD (actual native delay 2), MemX, DskEth and DispY, covering 1,727,916 modeled
bits. Wiring is byte-identical to Native59; only this helper and its manifest
hash change. DskEth/DispY use their actual unadapted Native58/57 parent trees.
The workspace MC10170 comment difference is checked token-identical, and actual
frozen bytes are compiled. Earlier damaged-board suites are separate evidence,
not claimed rerun. External executed HDL/C++/PROM inputs and parent backplanes
are retained; generated objects/databases are excluded.

The first zero-reply strict MLAB trial is REJECTED_MAPPING_AND_AREA. Quartus
reports both RAMs uninferred because of unsupported read-during-write behavior,
zero MLAB memory bits, and 96 -> 107 ALMs (141 -> 206 shell registers). All
four timing conditions pass, but this is not successful RAM mapping. Exact
executed v1 generator/driver bytes and the rejected result are retained.

With the explicit read guard and the original asynchronous `MLAB, no_rw_check`
style, the second trial actually infers two 16 x 4 read replicas, 128 physical
replica bits. Both read ports have unregistered inputs and outputs. One logical
RAM and one shared write source feed both replicas. Synthesis records 23 owned
control/interface registers plus 8 inferred RAM-input registers, 31 total;
the logic reference retains 87. The logical state remains 64 RAM plus 10 window
bits. The matched shell changes from 97 to 69 ALMs and 147 to 87 registers.
All four timing conditions pass, minimum +0.346 ns, with unchanged 50 MHz,
opposite-edge launch/capture, virtual IO and embedded-path constraints.
Preprocessed functional RTL is identical except for the RAM implementation hint.
These figures include the isolated shell; do not multiply them into a core
saving or treat them as proof of ordinary same-edge system timing.

`audit_f10145_zero_async.py` checks exact generated sources, constraints, state
retention, both physical RAM geometries, read latency, legal locations and
all timing conditions. Six damaged source/constraint/hold/state/memory/latency
controls reject. Re-extracting `async-fit/` and rerunning this independent audit
also passes (`archive-extracted-fit-audit.json`). The earlier audit expecting
23 total registers was corrected to account explicitly for all 8 inferred RAM
input registers before acceptance; no RTL or timing constraints were relaxed.

The prepared full-core assets select `--shared-riot --shared-files
--zero-write-reply --files-async-mlab`. Both local and actual remote integration
audits pass 413 baseline receipt files, 2,137 original board pins and 17 damaged
integrations. All eleven boards and 1,944,744 modeled bits remain accessible;
baseline clocks, SDRAM/ECC and timing policies are unchanged. The full-core
experiment is `/home/alans/mister/Dorado_Enabled60ZeroAsync_20261019` on
misterubuntu. At this checkpoint synthesis passed and fitting was active;
whole-core capacity/timing remain unqualified. No assembly/deployment occurs.
Later terminal full-core results belong in a separate report.

All 819 raw archived files pass SHA256 readback. Raw HDL/reports/logs and JSON
over 100 KiB are gzip-compressed without byte changes. `archive-manifest.json.gz`
records original/stored names and raw hashes; `archive-readback.json` records
aggregate sizes. The extracted-fit audit is additional verification metadata.
Global RAM/device/input/ARM/server/disk coherence and physical Cedar restore,
original 60 ns cadence and final release gates remain open.
