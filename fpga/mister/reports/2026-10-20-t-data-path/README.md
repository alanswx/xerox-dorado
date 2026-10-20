# Coupled T-data, task-file storage and visible T read path

Component contract only: no native integration, faster hardware boot, analog
setup/hold or 60ns operating claim. Native59 remains the hardware reference at
1,947,654.12 instruction issues/s. Complete FPGA snapshots remain deferred.

The candidate composes the unchanged 88-Q task/capture kernel with both g18
packages, k01/k02 transparent data latches, all four T-file SRAM slices,
j03/j04 file/IFU read latches and i03/i04 visible T latches. The independently
extracted original graph has 404 bits, 139 packages and 1,403 connected pins.
Pdata/dMD, IFU data/sign/control, unused g18 D pins and inherited Ready/MIR/
memory-Hold producers remain physical boundary inputs. Parity is outside this
slice. Every consumed mixed-driver net must include all original drivers;
ProcH g18 D5 requires both f20 (FF076) and h17 (Freeze).

Icarus and Verilator each pass 21,962 events in each of three illustrative
arrival profiles, comparing all 404 bits, 25 clock channels and write/latch/
bypass gates. Tests write/rewrite distinct values in all sixteen tasks through
Pd/Md/FF075, check sixteen held writes, revisit saved tasks including a switch
down to task zero, and read signed IFU data without modifying SRAM. All LC
values and ordinary/extended instructions are exercised. Each run records
1,698 transparent updates (462 Md / 1,236 Pd), 113 memory-change observations,
593 write windows per half, 3,612 closed-latch probes and 558 delayed-Hold
blocks. No SRAM address changes while its write window is open.

Seventeen compiled bad variants reject, including wrong transparency, mux,
Hold, address, write, read staging, bypass and IFU sign behavior. Two scoped
equivalence controls pass: old/settled T data during writes and the masked
high control in the low write gate. The data/write non-overlap invariant is
asserted. These results do not authorize dropping original FF/latch stages or
generalizing to startup/freeze/producer timing. The original paths remain.

Profiles are gate/ordinary-FF/SE-phase-FF delays of 1/1/1ns, 0.5/1/0.5ns and
1/2/1ns; mux latches use the ordinary-FF illustrative delay. They characterize
no vendor parts. Boundary observations allow 20ns settling, NoDispatch stays
low, memory Hold/StopTasks update in the root high window, and overlapping
CT set/reset is excluded. Actual SRAM pulse widths and board skew are unproven.

Historical runs include incomplete coverage, two initially misclassified
equivalence cases, wrapper diagnostics and a Verilator exit without a passing
receipt. The initial reference used nonblocking combinational SRAM writes;
Verilator warned of their conversion. The accepted reference uses explicit
physical-pin sensitivity and blocking level-sensitive SRAM updates. The mux
latch preserves its arrival delay. Both simulators now agree. Frozen inputs
and raw diagnostic reports are history, not qualification.

One terminal Quartus17.0.2 fit on misterubuntu targets 5CSEBA6U23I7 at15.000ns,
with opposite-edge input staging, 2ns max/0ns min virtual I/O, no exceptions,
no retiming, no assembly/deployment. All four conditions pass: setup +4.378ns,
hold +0.179ns, pulse width +6.548ns; evaluator setup at least +5.336ns. Resources
including the shell: 941 ALMs / 1,526 registers, no RAM/DSP/PLL (562 ALMs contain
virtual pins). Targeted collections cover 553 launch / 439 result registers.
This measures an FPGA combinational budget. All five recorded processes were
absent at collection. Only Quartus's version annotation changed the QSF.

The independent reader verifies both RTL dependencies against the executed
simulation inputs. Ten damaged/rehashed receipt cases reject, including a
changed child kernel. The prior 32/64/88-bit profiles still pass. Archive replay
repeats all simulation profiles, defective/equivalent controls and timing review.

Raw sources/reports/logs are losslessly gzip-compressed with raw byte counts
and SHA256 in archive-manifest.json.gz. Objects, binaries and Quartus working
databases are omitted. Decompress and verify each listed member to raw_name
under a fresh directory before replay.

From a checkout (fresh output):

```
make -C verilog t-file-test T_FILE_OUT=build/t-file-new
```

After archive extraction:

```
python3 qualified/executed-inputs/tools/test_t_file.py --output /absolute/fresh/simulation --mutations
python3 review-tools/tools/audit_control_clock_fit.py timing --qualified qualified --output /absolute/review.json
python3 review-tools/tools/test_processor_capture_fit.py timing --qualified qualified --component t_file --output /absolute/fresh/reader-controls
```

Next: T/R parity and operand/register-file timing; remaining Ready/MIR/Hold
producers, startup/freeze and board/peripheral timing; native integration and
original-firmware diagnostics/boot. Cedar rollback, repeated fresh boots, cold
disk persistence and two-hour stress remain required. Main/daemon are unchanged.
