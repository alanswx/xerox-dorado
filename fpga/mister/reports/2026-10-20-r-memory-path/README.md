# Real R address/storage handoff

PASS for an isolated636-bit processor plus9,216-bit RM/STK storage component.
The independent references retain the original final address/select/control
stages, twelve SRAMs and both byte-parity generators. Arrays stay behind four
functional256×9 ports; no production scan/init or flattened RAM-state interface.

At g16 capture the physical port retains its OLD address/select epoch while
writes are closed. Other events use the resolved address. A separate clock/
address preview uses the pre-event task source and qualified64-bit processor/
controller cone. The complete task/address evaluator remains the CPU-state
source; runtime checks compare resolved addresses/selects and parity events.

The primary gate passes16,448 events/profile in Icarus and Verilator across six
profiles, including9.9ns R-SRAM read delay. Every636 processor bit and9,216 RAM
bit is compared after every event.1,284 write/4,094 read observations;321 original
write windows per half have zero address/select changes while open.1,017 address
handoffs preserve the old epoch. All45 clock channels,256 addresses/bank and all
LC values pass. Seven broken variants reject. Advance suppression, actual j05
write gates, old parity sampling and unchanged sampled state after RAM commit
are checked. g16 has8,240 setup observations/profile; correlated PARC min/max
controls enforce1.7ns/1.1ns and observe702ns setup/2ns hold. These generous fixture
intervals are not independent-skew or60ns board qualification.

A separate directed gate passes1,680 events/profile in both simulators across
the same six profiles. Eight rounds exercise reset/Freeze, CurrentTask
maintenance, StopTasks, separate PRhold/Hold and resume with the inherited
masked-high producer setup contract. Full CPU/RAM comparison remains active.
These control runs have no writes; write/held-write coverage comes from the
primary gate. The first auxiliary driver fell through past `$finish` in Verilator
into unrelated workload coverage; its corrected initial block ends there.
RTL and fitted sources were unchanged by that test-harness correction.

The first functionally passing fit is REJECTED: full arbiter iteration in the
preview synthesized49 logic levels/41.018ns and setup-26.346ns. The separated
preview now passes all four15ns operating conditions: setup/evaluator+3.241ns,
hold+0.136ns, pulse width+6.111ns.1,992ALMs/2,507registers/9,216MLAB bits;
zero M10K/DSP/PLL.1,043 launch/847 result bits. All prepared input hashes and
all five absent process IDs are checked. Real muxes require UNREGISTERED read
addresses/outputs; the earlier external-address fixture retains its separate
OUTCLOCK read-address contract. Four banks, geometry, None initialization and
collision mode are checked. Sixteen damaged/rehashed reader controls reject.
Both preceding storage/address reviews still reproduce byte-identically.

2,315 selected raw source/report members are losslessly archived/read back.
Extraction and archived readers reproduce the positive AND rejected timing
reviews byte-identically. A second full simulator replay after extraction is
not claimed. Compiler objects, executables/databases and FPGA images are omitted.
`contract.md.gz` freezes the design boundaries at archive creation; current
qualification status is recorded here and in the repository handoff.

```
python3 tools/test_r_memory.py --output QUALIFIED --mutations
python3 tools/test_r_memory_control.py --qualified QUALIFIED --output CONTROLS
python3 tools/fit_r_memory.py --prepare FIT
# On misterubuntu only: python3 fit_r_memory.py --run .
python3 tools/audit_control_clock_fit.py FIT --qualified QUALIFIED --output review.json
python3 tools/test_control_clock_fit_reader.py FIT --qualified QUALIFIED --output READER
```

Accepted primary `r-memory-qualified-v3-20261020`; auxiliary
`r-memory-control-v2-20261020`; timing `r-memory-fit-v2-terminal-20261020`.
Remote `/home/alans/mister/r-memory-fit-v2-20261020` is terminal: builder4125726,
map4125729,fit4126059,sta4127796,corners4128093 are absent. Do not restart.
The rejected first fit and all our simulations are also terminal.

No native FPGA/Main/server change or speed gain. MiSTer14 remains on the
qualified MC1668 core15730f44, last measured1,947,686.01 instruction issues/s.
Remaining front RBase/stack/saved-address and complete bypass comparisons,
IFU/Ready/MIR/Hold and board timing precede native clock integration.60ns
cadence/timer calibration, authentic Cedar Checkpoint/Rollback, cold persistence,
three fresh boots and sustained stress remain open. Goal ACTIVE; full FPGA
snapshots deferred. `next/` contains original-driver inventories for resumption.
