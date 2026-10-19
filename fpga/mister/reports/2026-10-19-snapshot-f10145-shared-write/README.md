# F10145 shared normal/restore write port

Primitive original/model/restore simulation, fresh field classification,
matched routed timing and all eight affected board restore gates PASS.
This remains experimental; no live FPGA change is claimed.

The two mutually exclusive RAM writers share one address/data/write-enable
path. Both read ports, ordinary asynchronous output, captured WE/address/data/
CE window, old-value administrative write response and 17-word state ABI remain
unchanged. All 74 physical bits remain present; no state, latency, RAM-style
change or additional clock is introduced. The qualified reference helper is
byte-identical to Native59's compiled source (SHA256 e128278c...).

The unchanged original/independent write-window oracle passes 35,407 host edges,
7,987 holds, 326 state requests, every physical bit and address, a saved pending
write, destructive continuation, restore, first-edge commit and 20,000 further
cycles. Eight broken variants reject: restore alias, current-address commit,
missing advance/CE gates, running/entry/invalid access and lost window restore.
Fresh old/new XML retains the same 74 physical + 13 private interface bits;
seven damaged inventories reject. Both synthesized cell hierarchies retain all
87 registers. Matched enabled-state fixtures keep identical 50MHz/opposite-edge
launch/capture, 10ns embedded paths, virtual IO and SDC. All four conditions
pass, minimum +0.172ns, no added cuts/unconstrained clocks or paths. Shell
totals: 117 -> 96 ALMs, 142 -> 139 registers, zero block RAM. Seven damaged
source/policy/timing/retention controls reject. Counts include the shell and
must not be multiplied into a claimed complete-core saving.

All eight affected boards pass separate complete positive restore gates:
ContA, ProcH, ProcL, MemC, MemD, MemX, DskEth and DispY, totaling 1,727,916
modeled state bits. The accepted cohorts are full-board-v2 for the first six
and full-board-v3 for the last two. Their original physical-state, output,
writer and first-edge/continuation checks remain unchanged. This does not
claim another run of their earlier broken-board suites or global coherence.
The generated board wiring is byte-identical to each Native59 adapter; only
the F10145 helper and its manifest hash change. Original cell bytes come from
the actual frozen source trees.

Earlier v1 rejects the newer workspace MC10170 explanatory comments. V2
verifies token identity and compiles frozen bytes instead, then passes the
first six boards before the DskEth generator rejects the already-adapted
Native59 backplane. V3 supplies the actual unadapted Native58 DskEth and
Native57 display parent trees and passes both remaining boards, while still
comparing generated wiring against Native59. The parent backplanes and actual
external HDL/C++/PROM inputs are retained. No frozen input file is patched.

The opt-in --shared-files builder replaces exactly the old absolute and
relative F10145 QSF assignments with one new source plus Quartus's optional
matching alias. All other project policies and baseline source bytes remain
unchanged. Local and exact remote audits pass 413 baseline receipt files,
2,137 original board pins and seventeen damaged controls, including stale/
wrong replacement paths. The legacy eighteen-source experiment also passes
its fifteen controls under the updated auditor. The first audit extension
shadowed its mutation-input dictionary with source-replacement metadata; its
existing wrong-memory-delay control caught that error before qualification.
The corrected verifier keeps those dictionaries distinct.

The combined all-eleven-board build at
`misterubuntu:/home/alans/mister/Dorado_Enabled60SharedFiles_20261019`
is terminal/rejected:48,353ALMs /41,910available,6,443over capacity. It saves
6,050ALMs versus the first all-enabled experiment; see
../2026-10-19-snapshot-shared-files-capacity. Timing remains unqualified.
All 1,944,744 modeled board bits remain accessible. No assembler/deployment
runs. Whole-machine support-state/global barriers, disk-paired atomic bundles,
actual Cedar restore, original60ns clocks and final release gates remain.

All 664 raw files pass lossless SHA256 readback; sources/reports/logs and JSON
over100KiB use gzip without changing bytes. Generated objects/databases are
excluded. The independent source/state/timing auditor can be rerun after
extracting fit/; that extracted audit also passes. The rejected strict MLAB placement experiment is separately
retained at ../2026-10-19-snapshot-f10145-mlab-rejected.
