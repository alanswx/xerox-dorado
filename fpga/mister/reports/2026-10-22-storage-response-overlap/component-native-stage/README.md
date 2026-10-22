# Raw-memory response/request overlap: components pass, native fit pending

The opt-in transport change permits consuming the old raw-memory response and
accepting its successor on the same host edge. Old response ownership and data
remain unchanged, with at most one outstanding operation. Defaults remain off.
It removes a backing-memory handshake bubble; CPU, architectural cache, Map,
ECC and original chip/board timing are unchanged.

| Component check | Baseline | Overlap |
|---|---:|---:|
| Independent SDRAM pins, 64 hot reads | 128 host ticks | 65 host ticks |
| Pins, 96 mixed operations and response stalls | 566 host ticks | 444 host ticks |
| Original 144-chip MSA, 12,012 board cycles | 113,899 host cycles | 103,525 host cycles |
| Full128-group arbiter, each of16 request waves | 1,131 requests total | 1,131 requests total |

The original MSA's 9,437,184 stored bits, 1,111 output transitions, 750 physical
reads and 7,872 writes agree. Default-off reproduces the frozen full raw trace
byte for byte. Overlap preserves raw operation/address/data order, allowing
host timestamps to differ: 9.108069% fewer host cycles in this component only.
The common observational trace records an OLD completion before accepting its
successor; it changes no DUT, stimulus or oracle behavior.

The128-group independent scoreboard passes Icarus and Verilator with identical
results: 954 overlapping handoffs, request stalls51/162 and response stalls
502/970. It checks round-robin ownership, read/write payload, per-group replies,
stable requests under backpressure and single-outstanding occupancy. Physical
pin controls reject accepting through a held response and disabled overlap;
MSA and arbiter controls reject stale ownership, and the arbiter rejects
disabled overlap. Five native staging controls reject altered clocks, read
capture, qualified component input, omitted rejection and parent qualification.

Two failed scoreboard runs are retained. V1's constant-argument function hid
the changing global wave from Icarus continuous-assignment sensitivity; explicit
wave arguments repair stimulus. Both simulators passed V2, but its negative
control failed an earlier owner/fairness assertion than the runner expected.
V3 retains that exact rejection diagnostic. No RTL was changed for these fixes.

The frozen503-source native project changes only the generated arbiter,
backend, and wrapper (two overlap enables and16-to4 backing-buffer lines).
The four-line buffer follows the measured lack of idle improvement at16 lines.
Main, daemon, clocks, SDC and all architectural state behavior remain unchanged.
The new live selection-to-backend path requires actual all-device timing;
no new exception or loosened constraint is added.

One existing fit runs on misterubuntu at
`/home/alans/mister/Dorado_StorageResponseNative57_1_20261022`: builder4103958,
fitter4115679; read-only reporter4125729 and collector4125730 follow it.
These PIDs were verified live during this receipt, not assumed from journals.
Map passed; routing was still active. No assembler, RBF or hardware load.
This is not native timing, firmware or measured hardware speed qualification.
Nominal140ns/original60ns and the full delivery requirements remain unchanged.

750 members pass lossless readback, including all component inputs/results,
failed helpers, native sources and read-only report inputs. Verify with
`python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-response-overlap/component-native-stage`.
