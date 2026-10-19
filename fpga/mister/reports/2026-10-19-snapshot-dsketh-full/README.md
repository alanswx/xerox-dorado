# Complete modeled actual DskEth state PASS

All2,656mutable bits are covered:117FF packages,18x16x4files plus private write
windows, two signed70bit SN74123s, two19bit CRCs,33bit oscillator,34bit passive
receive monitor and nine board phases. All14immutable PROMs (12,032bits) are
source-hash pinned. Actual SYSPER4/timerTW400/oscillator23530-SYSPER8/CRC0+1
bindings and omitted chip enables are preserved. No production RTL changes.

Original board plus independent RAM oracle, all2,656physical bits/all8,192indices,
destructive export/restore/recapture/quiet release/first resumed edge/20,000more
pass:256,389host edges/50,293held/25,232requests/33,334late enable changes. Every
one of18normal RAM writers is exercised. Nine broken board/helper variants
reject. Per-word physical shadow checking also catches cross-word corruption.
Fresh actual XML classifies every candidate and nonaliased transport bit,
excludes only1,908checked simulator diagnostic bits and rejects16damaged schemas.
Passive monitor histories are saved, not reset as supposedly disposable debug.

Earlier tests are preserved: v1 passed state/restore but random pins did not
exercise TxFIFO writes; v2's fixture lacked a C++ forward declaration; v3 still
omitted the memory half-cycle waveform. Accepted v4 supplies stable I/O setup,
four-sample disk clocks AND matching memory-half-cycle input. All18writers now
execute; no chip/board logic was altered to satisfy coverage. Early classifier
attempts rejected tied backplane outputs; final review accepts only source-pinned
output-wire declarations with explicit literal0/1assignments. Unexported state
and mutable PROMs still reject. The compiled MC10170 comment predates the current
explanation; token streams are identical and its exact compiled bytes are kept.

Matched50MHz isolated fits pass all four operating conditions/min+0.147ns;
no illegal/unconstrained clocks or I/O or extra exceptions. Baseline1,054ALMs/
2,213regs/5M10Ks vs candidate3,480ALMs/3,699regs/0M10Ks. Every141state-helper
synthesis register count is checked, retaining all local mutable bits and
responses. Mutable files remain logic registers in BOTH variants. Baseline
packs nine logical immutable ROMs into5M10Ks/8,192implemented bits; the candidate
implements PROMs as LUT logic with all14source assets unchanged. Resource delta
includes shell/admin/pruning and is not an enabled whole-core snapshot budget.
Eleven bad source/monitor/macro/SDC/hold/PROM/placement/retention/corner/warning
controls reject. Exact compiled inputs reverified; raw evidence gzip unchanged.

Native59 builder prepares full-core integration from actual frozen Native58,
reusing11helpers byte-identically and adding8files. All snapshot administration
will remain inactive during normal-operation qualification. Whole-core timing,
original firmware/hardware boot and enabled global snapshots remain separate.
No complete physical save/load, timer calibration or speed gain is claimed.
