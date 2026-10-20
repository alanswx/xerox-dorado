# T parity and R parity checks

Component qualification only. Native59 remains the hardware reference at
1,947,654.12 instruction issues/s. No production core, Main or daemon changed.

`dorado_t_parity_step.sv` extends the qualified T path to 540 original bits:
both 16x4 parity SRAMs, both complete g16 F10000 capture packages, T/R checkers
and the low-half fault path into g18. The independent wire-list graph covers
150 packages / 1,506 connected pins and includes all drivers of consumed nets.
R data, R-storage parity outputs, R bypass/IFU masks, unused g16 inputs and
the inherited T/task producer cuts remain boundaries. R storage is unqualified.

Clock1 captures parity from the old task-file address before the address
register changes. Writes generate odd byte parity. Bypass and IFU modes suppress
the appropriate fault requests; low-half faults reach the original g18 capture.
The g16 reference models only its physically used parallel mode; original
pins4/5/6/12 are unwired and pulled low.

Icarus and Verilator agree on 23,400 checks in EACH of five profiles, comparing
all 540 bits, 25 clock channels, write/read/bypass gates and parity/fault outputs.
Tests retain distinct per-task writes, rewrites, holds, IFU reads and randomized
ordinary/extended instructions. They inject each of 16 T-data bit faults, two
stored T-parity faults and 16 R-data faults; check 33 directed masks; and reject
12 compiled bad variants. Each run has 583 write windows per half and no SRAM
address movement inside a write window.

The first three profiles retain illustrative gate/FF/phase delays of 1/1/1ns,
0.5/1/0.5ns and 1/2/1ns. Two targeted correlated controls use PARC's original
`ecldict.analyze` timing annotations: F10000 clock-to-Q 1..5.5ns, MC10170 A 1..6.6ns
and SRAM address-to-read 2..9.9ns. The inherited clock graph remains illustrative.
The targeted controls enforce the g16 data setup/hold limits of 1.7/1.1ns; measured
minimum setup is 44ns and hold 3ns / 10.9ns respectively. Both simulators record
16,400 setup and 10,700 hold samples. Illustrative-profile hold minima are 1/1/2ns;
these profiles are not asserted against the source limits. This is not a full
board min/max, independent-skew, analog timing or 60ns operating qualification.
The inherited 20ns boundary settling, NoDispatch-low and high-window Hold/StopTasks
contract remains conditional.

The initial monitor counted constant initialized inputs differently between
simulators. The accepted monitor explicitly starts each observation epoch,
using its start as a conservative stable-time bound. No candidate RTL changed
for this correction. Earlier functional/monitor runs are retained as history.

One terminal Quartus17.0.2 fit on misterubuntu passes all four conditions at
15.000 ns: setup +4.470ns, hold +0.139ns, pulse width +6.548ns. Internal evaluator
setup is at least +4.470ns. The staged virtual-I/O shell uses 1,232 ALMs / 2,001
registers (727 ALMs contain virtual pins), no RAM/DSP/PLL. Targeted collections
cover 725 launch / 583 result registers. I/O remains2ns max / 0ns min, with no
exceptions, retiming, assembly or deployment. All five recorded job processes
were absent at collection. Only Quartus's version annotation changed the QSF.

The timing reader matches all three RTL dependencies to the executed inputs.
Ten damaged/rehashed evidence controls reject. The four previous component
profiles still pass. Raw sources/reports/logs are losslessly compressed; the
manifest records every raw byte count and SHA256. Objects, binaries and Quartus
databases are omitted. Verify and extract each manifest member before replay.

From a checkout, with fresh output:

```
make -C verilog t-parity-test T_PARITY_OUT=build/t-parity-new
```

After extraction, timing review and full simulation replay commands are:

```
python3 review-tools/tools/audit_control_clock_fit.py timing --qualified qualified --output /absolute/review.json
python3 review-tools/tools/test_processor_capture_fit.py timing --qualified qualified --component t_parity --output /absolute/fresh/reader-controls
python3 qualified/executed-inputs/tools/test_t_parity.py --output /absolute/fresh/simulation --mutations
```

The qualified run already executes from frozen inputs. Archive readback verifies
their identity; extracted timing review and reader controls are rerun separately.
No second full simulation run is claimed for the extracted archive.

Next: close the R read/IFU/bypass latches and functional RM/STK ports, then
Ready/MIR/hold producers and native integration. Keep bulk RM/STK behind RAM
ports. Original-ROM diagnostics, actual instruction progress, faster Cedar boot,
authentic rollback, final persistence/repeated boots and stress remain open.
