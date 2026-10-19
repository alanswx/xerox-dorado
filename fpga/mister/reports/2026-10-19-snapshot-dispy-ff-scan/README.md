# Complete display-board serial FF state access

Complete display simulation, fresh state classification and matched isolated
area/timing PASS. Serial access saves 501 ALMs: 2,814 -> 2,313. Both sides keep
the same RAM implementation, 50 MHz clock, staged inputs, positive-edge20ns
embedded timing and SDC. This is not a full-core saving, hardware boot, global
snapshot or clock/timer correction.

The existing140FF helpers hold all1,439FF state bits and form a one-bit chain
without a shadow bank. Compact FF words0..79 end with17valid bits. Every read
returns after a full rotation restores the bank; every write changes only the
selected word. RAM/window, raw read-pipeline, oscillator and phase addresses
stay unchanged. The compact-ff-v1 schema provides exact offsets/word widths;
old per-package FF addresses are marked legacy_parallel_word, with no ff.word
alias. A loader must reject incompatible layouts before modifying state.

The actual native field board and oscillator defaults remain SYSPER16 and
FREQ_KHZ50000; fabric is50MHz. The old source-generation guards preserve the
unadapted Native57 parent/backplane, omitted word-memory CE connections, PROM
assets, signed accumulator arithmetic and original cell bytes. No source root
is patched to fake an original binding. The necessary original tree is included
under generator-root for independent regeneration after archive extraction.

Positive simulation v3 passes7,874,288host edges,7,799,391holds,
1,101,581requests,33,334late-enable changes, all13,993state bits and524,288indices.
It retains the original full-board oracle, independent physical RAM arrays,
all-bit write/read/padding checks, destructive restore, quiet recapture, first
resumed edge and20,000further cycles. All15ordinary RAM writers are active:
46; eight at288; three small-file writers at7; three word-bank writers at23.
Only the administrative mapping/wait, helper freeze probes and fault/reset
checks are derived. Original and derived C++ plus every executed input remain
preserved. Word and RAM probe metadata are adjusted to the compact layout.

Changing requests/payloads while busy cannot steal ownership or write other
state. Loss of the external hold during a partial rotation quarantines local
FFs, RAM/window helpers, raw word pipelines, oscillator and phase state until
SYSTEM reset. All16memory/oscillator helpers acknowledge that hold, even with
attempted guest advances and writes. Reset-time requests are rejected and a
fresh freeze entry is required afterward. Foreign consumers still require a
global barrier; resetting only the transport cannot repair shifted guest state.
Seven damaged designs reject: lost chain bit, missing FF shift, early completion,
stolen busy owner, missing local hold, reset-time access and released word-bank
hold.

The last control exposed a test gap in v1: coincidentally unchanged pipeline
values allowed it to pass despite an escaped hold. V2 adds direct observations
of all16helper freeze acknowledgments; its strengthened check also catches the
lost-local-hold case earlier than the old expected diagnostic. V3 pins those
specific failure messages and completes all controls. No guest RTL change or
weakened assertion accompanies those harness corrections. Earlier sources,
outputs and the surviving-control result remain archived as history.

Fresh candidate XML accounts for13,993physical+232private transport bits, with
no sequential exclusions, plus2,048immutable PROM bits. The held word pipeline
uses the qualified helper's saved_q as the canonical original guest pipeline;
ram_q, pending and response state are private to its scan transport. This mapping
is valid at the acknowledged frozen boundary, with pending active; full raw
pipeline bit writes and the first resumed edge are tested. Every other FF,
setup/edge history, RAM word, write window, signed oscillator bit and phase bit
is explicit. Twelve damaged inventories/layouts reject, including lost pipelines,
oscillator width, mutable PROM and legacy address aliasing.

The matched fits retain all1,439FF bits in140synthesized helpers and18placed
physical RAM rows. Nine256x4MLAB FIFOs, six16x4register-file replicas and three
independent1024x1M10K word banks preserve all12,480logical RAM bits,
12,672replicated bits, asynchronous file/FIFO reads and word-bank OLD_DATA.
Each word macro's geometry/collision mode and physical packing are checked.
Immutable PROMs are byte-exact. Both fits use3,072M10K bits/three blocks;
shell registers increase2,234 ->2,382. All four conditions pass, minimum+0.124ns,
with no added timing cut, unconstrained paths or new warning category.
Eight damaged source/constraint/policy/FF-retention/hold/OLD_DATA/placement
controls reject at the intended audit stage.

SnapshotDispYFFScan_20261019 on misterubuntu and local session59099 are terminal;
simulation session98875 is also closed. All940raw files pass lossless SHA256
readback. Sources/C++/headers/PROMs/XML/reports/logs and JSON over100KiB use gzip
unchanged; large manifests are gzip too. Objects and compiler databases are
excluded. Extracted source regeneration, state classification and independent
fit audit pass again. Additional extracted-audit JSON is verification metadata.

No production builder or live FPGA changes. The actual complete-core result
remains45,228ALMs /41,910available,3,318over capacity. Do not subtract isolated
savings or multiply them into a whole-core claim. Remaining boards, combined
capacity/timing, versioned formatters, a transport timeout beyond1,439scan edges,
global hold/fault/SYSTEM-reset ownership and backend/input/ARM/server/disk-paired
snapshot state still need qualification. The existing16-edge fabric cannot
serve this adapter unchanged. Physical Cedar restore, original60ns clocks and
final release gates remain open.
