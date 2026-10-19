# Complete actual native MemD state qualification

Native55 actually compiles DORADO_STORAGE_INPUT_DELAY=2. The previous
zero-delay adapter's guard rejected the first Native56 synthesis before an
RBF existed. The earlier zero-delay qualification was real but its "actual
native" description was wrong; the prior report carries an explicit erratum.
The live FPGA was unchanged. The failed integration's source, QSF, original
Native55 QSF and terminal map error are preserved here.

The complete native2 model exports 296,591 mutable bits: the prior 296,554
plus both 18-bit storage-input words and their one-bit edge history. The
original two-stage delay is preserved, not overridden. All other delay values
explicitly reject. Clock-history and RAM/write-window state remain complete.

The original XML inventory omitted state owned by generated blocks. The
shared tool now traverses generated lexical scopes and resolves parent/local
names without aliasing. Its regression checks real generated/nested RAM and
same-name state in distinct scopes, alongside all previous cell/task/ROM/
array/destination tests. Fresh native2 MemD XML classifies all 296,591 bits.
Two source-pinned loop indices (64 simulator bits) are recomputed: each for
loop assigns its index before any read, with no use outside that loop.
The adapter generator pins the complete original branch, so these exclusions
cannot silently accommodate a changed loop. Sixteen damaged schemas reject.

Accepted simulation snapshot-memd-delay2-full-v3-20261019 passes 2,379,213
host edges, 2,179,153 held edges, 2,169,187 requests, all 296,591 state bits
and all 1,048,576 indices. All14cache/eight small-file writers and14,265
actual storage-input captures execute through original board pins. Independent
C++ oracles check cache old-data pipelines AND both storage-input words/
history. Fixture-only physical probes bypass state decode. Destructive
whole-image writes/restore/recapture, first actual advance and20,000more
cycles agree with the untouched reference. Twelve broken RTL controls plus
an unsupported delay setting reject. Earlier simulation's rejection expected
the wrong diagnostic for a history bit lost on restore; the actual failure
is preserved, and the final gate accepts either direct bit-readback or
physical-state mismatch. There was no RTL change for that diagnostic fix.

Matched50MHz isolated fits SnapshotMemDDelay2_20261019 pass all four timing
conditions with unchanged staged-input/positive-edge shell and20ns embedded
budgets; minimum snapshot slack +0.107ns, no illegal/unconstrained clocks/I/O.
294,912cache RAM bits remain:38 ->40M10Ks,620 ->2358ALMs,1389 ->2411registers.
All14actual RAM macros and physical banks preserve OLD_DATA. Nine damaged
fit/source/physical/timing controls reject. Counts include shell/admin/pruning,
not an enabled whole-core snapshot resource budget.

The exact older test-driver dependency read during fit preparation is saved
with its matching provenance hash; final executed simulation/classifier/tool
sources are saved separately. Raw reports/logs/source text are gzip-compressed
unchanged. Native56b full-core integration is a separate, still-pending gate.
No complete machine snapshot, new clock cadence or speed increase is claimed.
