# Ready coupled to RBase, stack, processor and functional RM/STK

The combined 752-Q / 9,216-RAM component passes original-circuit comparison
in both simulators across six profiles, then the isolated 15 ns FPGA budget.
Ready/IsNext and all three SetReady drivers are internal. No FPGA, Main,
daemon or guest volume was changed; no hardware speed gain is claimed.

## Functional comparison

One extracted 274-package / 2,884-pin circuit compares every processor and RAM
bit after each of 13,173 events/profile. Both Icarus and Verilator pass balanced,
fast-gates, later-control, correlated PARC parity min/max, and 9.9 ns SRAM-read
profiles. All 59 clock channels are exercised; all 100 R-front and 16 Ready bits
change. Every task 1..15 also wakes through Notify/Ready with direct wake wires
zero. The original RBase-slot and all 256 expected StkP-load checks remain.

All reference write-window checks pass: 266 RBase windows across all 16 addresses,
155 RM/STK windows per half, zero address/select movement. The runs observe
620 writes, 4,322 reads and 810 old/new parity-source handoffs. All RAM bits
agree after each commit, advance suppression holds RAM, and committing the
write cannot change already sampled processor state. PARC profiles enforce
the original targeted g16 setup/hold requirements and observe 702 / 2 ns.
Other profiles use illustrative arrivals; none proves whole-board 60 ns timing.

All nine damaged RTL variants reject: two Ready wiring faults and seven
inherited RBase/stack/address/decode faults. Stored state, Ready/IsNext levels,
resolved source pins, functional outputs, clocks, RAM contents and arrival /
write-window summaries agree across both engines. Combinational wiring and
cell functions remain shared with the reference; delayed sequential/storage
models are independent. The inherited pure cone is byte-identical, so its
prior decoder/representation proofs apply without a new simulation claim.

Smoke v1 stopped at the pin-mapper identity check because its text extractor
expected a different indentation. Smoke v2 exposed a fixture connection that
dropped the added high Notify input bits. Smoke v3 corrected that connection
and passed 5,043 events with reduced random work. Final qualification additionally
seeds and asynchronously clears all 16 Ready bits. These development records
remain preserved separately from the six-profile gate.

## Four-condition implementation budget

The fit is **PASS_COMPONENT_TIMING** at 15 ns: setup **+0.622 ns**, hold
**+0.171 ns**, pulse width **+6.117 ns**. Resources: 2,944 ALMs / 2,809 registers,
four 256 by 9 MLAB banks (9,216 bits), no M10K/DSP/PLL. Read addresses and outputs
remain unregistered; there is no initialization. The checked DONT_CARE collision
contract retains original read/write nonoverlap and source epochs.

All 1,247 launch / 1,021 functional result interface bits remain in the fully
constrained registered fixture, including state, all events/repeats, Ready
levels and unsettled checks. Only the same 210 post-state `source_pins` / `fa_n`
test observations remain unregistered. No timing exception, added architectural
cycle or RAM copy. No inferred latch warnings.

Twenty damaged/rehashed evidence controls reject with the strict reader.
They cover every RAM parameter, generated cone, both additional child RTL
dependencies and a top that replaces registered state with zeros while keeping
the same dimensions. The canonical top's identity is explicitly checked.
The original 19-case reader receipt and development comment-only control remain
preserved; `strict-reader/` contains the final tools and meaningful 20 controls.
The strict review is byte-identical to the original passing review. Earlier
R-front and task/Ready reviews remain byte-identical with the extended reader.

All stages exited zero. Builder110834/map110842/fit111318/sta114102/corners114597
were absent at collection. All 24 prepared files read back unchanged except
the permitted Quartus version annotation. The unrelated SharpMZ2500 job was
left untouched; this fit used its own project/database. All local jobs are
terminal; no job needs restarting.

## Concrete native integration inventory

The 752 physical Qs map bijectively to 91 storage packages in the preserved
MC1668/Native59 source: ContA 64, ProcH 280, ProcL 408. Native source, package
type, exact pin connections, parameters, state-word metadata and global Q-bit
ordering are recorded. The 12 physical RM/STK data/parity packages map to the
four component banks without an extra copy. All 498 native source identities
were verified before inspection. The 370 additional history/setup/window bits
are **source metadata**, not a fitted-register saving or speed measurement.

The frozen selected native sources/metadata and Q mapping are reproducible
from extraction. A fresh check of all 498 files requires the separately
preserved native baseline; that full-source replay is not claimed here.
The twelve unused Control-A package-half bindings remain dead in the actual
752-Q selection by the expanded pin-function proof. Original logic outside
the replacement slice must remain connected.

## Raw receipts and replay

The root archive preserves 2,492 raw members / 128,608,618 bytes losslessly as
11,197,162 bytes. `strict-reader/` adds 1,446 raw members / 114,597,047 bytes
stored as 6,832,770 bytes for the final reader/evidence controls.
Non-JSON text and JSON over 100 KiB use gzip. Databases, objects and executables
are excluded. Readback/extraction verifies every raw member and all 96 primary
inputs. Extracted tools reproduce the passing timing review byte-identically,
the 752-Q mapping and the dead-stub proof. This is not a full simulation rerun
after extraction. Supplemental replay JSON and this README are outside the
raw manifests.

```
python3 fpga/mister/snapshot/archive_receipts.py --readback --output fpga/mister/reports/2026-10-20-r-front-ready-path
python3 fpga/mister/snapshot/archive_receipts.py --readback --output fpga/mister/reports/2026-10-20-r-front-ready-path/strict-reader
```

Extract both manifests to their raw names under the same directory `$R`, with
the second under `$R/strict-reader`; verify recorded lengths/SHA-256:

```
python3 "$R/strict-reader/review-tools/audit_control_clock_fit.py" "$R/r-front-ready-fit-v1-terminal-20261020" --qualified "$R/r-front-ready-qualified-v1-20261020" --output review.json
```

## Next action and remaining acceptance

Stage the native replacement from the preserved normal-operation source at
the same 50 MHz / SYSPER4 first. Use the exact Q/RAM/package inventory; preserve
BaseBoard, memory/cache/map/ECC, IFU, SDRAM, disk and direct-DDR services. Close
live boundary wiring and producer arrivals, retain the original source epochs,
then pass all-device timing and an original-ROM gate that actually instantiates
the modified native boards. The old generic-board Initial fixture alone cannot
qualify a change confined to native wrappers.

Only then run a fresh original-firmware Cedar/input boot and compare issues /
holds at the same workload. Clock acceleration and timer calibration follow
that qualified integration. Authentic Cedar Checkpoint/Rollback, cold disk
persistence, three fresh boots and two-hour combined stress remain open.
Complete FPGA snapshots remain deferred. Live speed remains the last measured
1,947,686.01 issues/s; the ordinary unstalled target is 60 ns / 16.67 million/s.
