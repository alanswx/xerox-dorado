# Original RBase, stack-pointer and bypass clock component

Functional comparison passes; the 15 ns implementation budget remains open.
No FPGA, Main, daemon or guest-volume change was made. The active goal remains
speed, original Cedar Checkpoint/Rollback and final hardware qualification;
complete FPGA snapshots remain deferred.

## Functional evidence

`primary/` preserves all 87 frozen executed inputs, generated references,
commands, logs, six staged timing profiles and the completed results record.
Both Icarus and Verilator pass 11,236 resolved events per profile, comparing
all 736 processor and 9,216 functional RM/STK bits after every event. The
reference contains 259 original packages and 2,712 connected pins.

All 57 clock channels are exercised and all 100 added bits change. Directed
original maintenance/FF inputs fill every RBase slot with both nibble extremes
and check all 256 expected stack-pointer loads. Balanced observes 266 RBase
write windows/all 16 addresses/zero address changes and 155 RM/STK windows per
half/zero address or select changes. There are 620 write observations, 3,362
reads and 810 old/new parity-address handoffs. All resolved non-memory source
pins agree. Seven deliberately damaged variants reject in balanced Icarus.

Both simulators also pass 4,352 decoder/adapter checks: all 4,096 raw ASEL/FF/
disable cases and 256 original-Q adapter fixtures. Correlated PARC min/max
profiles enforce g16 setup/hold limits of 1.7/1.1 ns and observe 702/2 ns.
The generously spaced fixtures do not establish a 60 ns board cycle or
independent skew tolerance. Canonical combinational gates and wiring are
shared; delayed storage/capture models are independent.

## Synthesis representation

The first three Quartus attempts are terminal map failures: implicit generate,
inline genvar, then reference-style hierarchical package observations. They
are preserved with exact input readback and absent-process checks. Local v4
preparation is a partial development directory, not a Quartus job.

The synthesis adapter exports ordinary front-data ports and resolves local
package observations to their exact existing pin connections and sampled
clock expressions. It also uses explicit generate/separate genvar syntax.
No state, gate function, capture equation or loop bound changes. The original
cone remains alongside the adapted source; the reader reconstructs the exact
adaptation. Both simulators pass the original 4,352 checks on the adapted
form. `r-front-lowering-check-20261020/` compares it with the qualified cone
for 16,384 arbitrary state/pin/root samples across all 1,085 consumed/sample-Q
bits (8,254 LOW and 8,130 HIGH roots), in both simulators.

## Timing results: both REJECTED

| Fixture | Launch/results | Setup | Hold | Pulse width | ALMs/registers |
| --- | --- | --- | --- | --- | --- |
| `rejected-observer-fit/` (v5) | 1,211 / 1,181 | -3.041 ns | +0.164 ns | +6.115 ns | 3,117 / 3,060 |
| `rejected-functional-fit/` (v6) | 1,211 / 971 | -2.617 ns | +0.156 ns | +6.117 ns | 2,979 / 2,785 |

Both complete all four operating conditions with unchanged 15 ns constraints,
no timing exceptions, and fully constrained staging/I/O. Each infers exactly
four 256 by 9 MLAB banks (9,216 bits), no initialization, unregistered read
addresses/outputs, and the checked DONT_CARE collision contract. No M10K, DSP
or PLL. Original read/write nonoverlap and sampled-source checks remain required.

V5 additionally registers the post-state `source_pins` and `fa_n` test exports;
its worst path computes the next task then reads RBase for `source_pins[156]`
(result164). These exported buses feed no state, RAM port or invariant.
V6 leaves only those 210 observation bits unregistered, retaining every state,
event/repeat, functional port and unsettled check. All observations remain
simulation-checked. `--capture-observers` preserves the old experiment.

V6 still fails: the actual 18-level critical path runs from task request/Ready
through priority/current-task feedback to `unsettled` (result120), with 17.003 ns
data delay at the slow -40C condition. The next implementation step is to shorten
that feedback computation using original capture epochs, retaining the check,
full state/ports and 15 ns constraint, then repeat circuit qualification and fit.
Remaining IFU/Ready/MIR/Hold producers and native integration follow. The older
R-memory and R-storage reviews remain byte-identical; 16 existing damaged/rehashed
reader controls reject. New front damaged-fit controls await a passing base fit.

## Raw receipts and replay

4,821 selected raw members: 187,144,831 bytes stored losslessly as 15,868,097
bytes. Every selected non-JSON file is gzip; JSON over 100 KiB is gzip too.
Compiler databases, objects and executables are excluded. The manifest records
each raw name, raw SHA-256, length and encoded member name. Readback verifies
all bytes. All members were extracted; all 87 frozen primary inputs rechecked.
Both timing reviews rerun with the extracted `review-tools/` reproduce the
original REJECTED reviews byte-for-byte. This is not a second full simulation
run after extraction. Supplemental replay/readback JSON and this README are
outside the raw-member manifest.

Readback:

```
python3 fpga/mister/snapshot/archive_receipts.py --readback --output fpga/mister/reports/2026-10-20-r-front-path
```

For extraction, decode `archive-manifest.json.gz`; write each member to its
`raw_name`, gunzipping when `stored` differs from `raw_name`, and verify its
length/SHA-256. Given an extracted directory `$R`, the portable review commands are:

```
python3 "$R/review-tools/audit_control_clock_fit.py" "$R/rejected-observer-fit" --qualified "$R/primary" --output observer-review.json
python3 "$R/review-tools/audit_control_clock_fit.py" "$R/rejected-functional-fit" --qualified "$R/primary" --output functional-review.json
```

The earlier reader regression data refer to the separately archived
[qualified R-memory receipt](../2026-10-20-r-memory-path/README.md).
Development/debug trees preserve failed attempts without upgrading their scope.

## Process and hardware state

All work jobs are terminal. Failed map PID pairs: 4193364/4193367, 2199/2206,
3993/4003. Reviewed v5 PIDs: 8154/8155/8682/11516/11880. Reviewed v6 PIDs:
14971/14978/15519/18117/18383. Every recorded PID was absent at collection;
all prepared inputs read back unchanged except the permitted Quartus version
annotation in successful fits. No job should be restarted.

MiSTer14 still runs Main3486/server3485 and RBF15730f44. Last measured login
issue rate is 1,947,686.01/s, unchanged; no speed gain is claimed. The 23:15:33 UTC
read-only sample records good3334/dropped33/CRC0/RX0/abort0. This idle health sample
is not the final two-hour combined stress gate. No fresh boot, cold persistence,
FPGA rollback or native clock qualification is claimed here.
