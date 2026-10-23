# Conservative native844 integration — 2026-10-23

**Native source integration and elaboration pass; original-ROM runs remain
live. This is not a boot, FPGA timing, 60 ns cadence or speed qualification.**

The 844-Q processor/control/Hold composition is now privately staged inside
the complete frozen 5c65 native wrapper. The current 57.142857 MHz host/SDRAM
clock and SYSPER4 stay unchanged. Original firmware, existing storage comparison
inputs/limits, MiSTer14, Main, daemon and disk files were not modified.

## Ownership and native capture contract

- All 530 frozen native inputs are checked. The selected storage inventory
  covers 123 packages / 844 original Qs, plus 12 RM/STK packages / 9,216 bits.
- Replace 119 Q packages and the 12 RAM packages. Keep the four native visible-R
  latch packages as the sole owners of Q572..587: 828 evaluator Qs + 16 native
  visible-R Qs. No predicted R copy, reset planting or added architectural cycle.
- The old selected packages contain 519 additional history/setup/window bits.
  This inventory is not a fitted resource or performance measurement.
- Exact frozen native l01/f13/l06/l03 wiring and cell models establish that the
  present oversampled wrapper captures old RunRefresh at l03 Set. Sixteen cases,
  including the release case, pass. The opposite arrival assumption fails at
  case 4 (`old_refresh=1,want=0,cpu=0,stop=0`, actual `11`). This binds descriptor
  zero to this existing wrapper only; original analog/future60ns order is open.

## Structural loop diagnosis

The initial full elaboration adds two UNOPTFLAT reports at RM/STK read outputs
`qh/ql`, alongside the baseline disk-ECC loop and dual-clock media buffer warning.
The trace crosses packed board exports, control resolution and address preview.

Both engines pass **10,121 events, 242 writes, 4,166 reads, 391 port epochs**
with candidate input slots127..144 zeroed. Those18 slots are replaced internally
by the owning memory evaluator's real data/parity before processor evaluation.
The independent original reference still receives its own actual RAM outputs.

Zeroing only the central interface does not remove the reports. Additional
consumer masks, bit-splitting annotations and an unoptimized diagnostic also do
not qualify the loop as resolved. The consumer masks separately pass the same
full comparison in both engines, but remain unpromoted diagnostics.

**Stage v7 cuts those unused slots at the board exports as well.** The reviewer
checks that the only ProcH/ProcL source differences from stage v2 are those18
export assignments. Physical ddR/parity signals and every physical consumer
remain byte-identical. All other boards are byte-identical. This version uses
neither the consumer-mask rewrite nor bit-splitting annotations.

With warnings visible, complete native v7 elaboration has **one UNOPTFLAT and
one MULTIDRIVEN**, matching the baseline categories/counts. The new `qh/ql`
reports are absent. Width warnings still exist; elaboration is not timing proof.
All initial stages and diagnostic logs/DOT graphs are preserved here.

## Original-ROM runs and limits

- Cold v1 terminated at 12M host ticks, before physical memory initialization;
  it is **insufficient coverage**, not a pass. The original 23-bit SDRAM
  initialization needs about 193M host ticks in this harness, including sweeps.
- Cold v2 (stage v1) is live at archive time, PID33395, its own 250M limit.
  It reuses the terminal v1 binary only after checking every compiled input,
  HDL list, define, artifact identity and binary SHA. No source change was reused.
- Cold v3 (corrected stage v7) is freshly compiled and live, PID37871, its own
  1B host limit. Tool session38920. It must establish startup progress before
  any native firmware claim. Its stage and binary differ from cold v2.
- Both use original ROM/button startup, physical SDRAM model and passive
  alias/resolver monitors. The pack opens `rb`; simulated writes stay in the
  host vector. No CPU state or guest IOCB completion is imported.
- The original 5B storage comparisons (PIDs2284/2331) and watcher94312 are
  untouched. No BFN113/Cedar measurement window was observed at this milestone.

Running-prefix receipts establish live process identity at their recorded time,
not completion. Re-poll these exact jobs; an observer timeout is not a restart
condition. Recheck current state before relying on the PIDs above.

## Reproduce and inspect

The reviewer verifies source hashes, ownership, component results, native refresh
positive/negative cases, stage differences and unsuppressed structural warnings:

```sh
python3 tools/review_r_front_control_native.py \
  --output build/rtl-executive/r-front-control-native-review-NEW \
  --archive fpga/mister/reports/2026-10-23-native844-integration/NEW
python3 tools/archive_r_front_ready_followup.py \
  --verify fpga/mister/reports/2026-10-23-native844-integration/conservative
```

Use a fresh destination. While runs are active, the reviewer verifies their
actual processes before preserving observations. After completion it reads the
terminal result; it never starts/restarts a simulation.

`manifest.json.gz` maps 6,718 named members to 888 content-addressed gzip blobs;
lossless readback verified 70,202,499 unique raw bytes / 6,256,859 stored bytes.
`summary.json` records assertions and scope. All stage sources, frozen simulation
inputs, tool versions, terminal logs and bounded live log prefixes are included;
build objects/binaries and disk packs are excluded. To recover a member, look up
its blob in the manifest and decompress that blob's `stored` file.

## Remaining acceptance

Finish native original-ROM/producer correctness, the five unexercised clock
channels43/91/92/93/95 and remaining tasking/LOW-NoDispatch cases. The previously
recorded early RBase overlap still fails; these changes do not filter its oracle
or establish a physical arrival contract. Implement accurate60ns capture phases
with the full15ns FPGA budget, actual IM/parity/MCR/hold-data/DMD/B-bus producers,
then all-device timing/timers and physical66.7MHz SDRAM qualification.

The full A–F goal remains active: measured storage gains, pinned speed/holds,
interactive cold Cedar, authentic Checkpoint/Rollback and cold file persistence,
three independent fresh-seed boots and two-hour disk/network/input stress.
No FPGA assembly/load or speed gain is authorized by this receipt. Full FPGA
snapshots remain deferred. Last qualified pinned measurement stays4,025,473.4492
StartCycle issues/s (24.15% nominal), not retired instructions or a new desktop
benchmark. Ordinary140ns floor and hardware speed are unchanged by this work.
