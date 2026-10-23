# Flag-output factorization is equivalent; timing experiment rejected

The original R sign/odd outputs combine three complemented MC1668 outputs.
During an open read window the inactive sources are set to one, leaving the
selected RAM, Md or Pd bit. A factored expression selects that bit directly;
when the window closes it retains the original combination of stored Qs.
All physical latch updates, clock events, RAM ports and epochs remain intact.
The proven visible-R stored-latch rewrite remains present; original stack
arithmetic is retained.

The source-bound symbolic proof finds no binary counterexample (UNSAT),
and admits counterexamples (SAT) for a stale RAM bit and swapped Md/Pd source.
SMT inputs are hash-bound in the receipt. Two complete new original-graph
comparisons pass in Icarus and Verilator: 13515 events, 328 write-active,
5608 read-active and 797 old-parity epochs per run. All 792 Qs, 9216 RAM bits,
68 clocks and functional ports compare, including LOW-root NoDispatch and
late CP. Two earlier visible-R comparisons and twelve original arrival runs
remain inherited evidence; none is represented as a new delay-profile run.
This proves binary combinational equivalence, not analog skew or native boot.

Only `dorado_local_r_flags_step.sv` changes relative to the previous visible-R
fit. The top, other 23 RTL files, original combinational cone, ordered project
sources and all constraints are byte-identical.

| Condition | Setup / evaluator setup | Hold | Minimum pulse width |
| --- | ---: | ---: | ---: |
| Slow 1100mV 100C | -0.820 ns | +0.385 ns | +6.462 ns |
| Slow 1100mV -40C | -0.908 ns | +0.361 ns | +6.489 ns |
| Fast 1100mV 100C | +3.817 / +6.461 ns | +0.200 ns | +6.177 ns |
| Fast 1100mV -40C | +4.340 / +7.765 ns | +0.173 ns | +6.115 ns |

3369 ALMs, 3028 registers and four uninitialized 256x9 MLAB banks with
asynchronous reads. The physical architectural state is unchanged; Quartus
register duplication/optimization can change the fitted register count.
All functional parity/fault and file-read endpoints remain registered.
No timing exceptions or relaxed budgets were introduced.

Worst setup worsens from -0.382 to -0.908 ns. **Reject the flag rewrite as a
speed candidate; retain the preceding visible-R version as the better timing
reference.** A simpler Boolean expression did not improve this mapped design.
The worst -40C path is 16 levels / 15.291 ns:

```
launch[712] = state_q[75] (task SwitchUp boundary)
 -> repeat_a / MIR capture -> new FF/front selection
 -> address preview -> actual rm_h asynchronous read
 -> file_read_data[6] = result[143]
```

At 100C controller Q7 reaches rm_flags_n[1] in 17 levels / 15.511 ns.
Clock skew also differs between the placements. The path evidence continues
to identify actual MIR/address/read dependencies; it does not establish a
hardware speed or MHz gain.

The isolated build at `/home/alans/mister/Dorado_RFlags_1_20261023` is terminal:
all four stages returned zero and all five PIDs
1142953/1142962/1145693/1159867/1161536 are absent. Input readback accepts only
Quartus's version annotation. Twenty malformed/rehashed timing controls and
six damaged proof receipts reject; the preceding visible-R reader's twenty
controls reproduce. All failed and passing functional attempts are retained.

The lossless receipt has 4360 members, 424 blobs and 2,297,743 compressed
bytes. Verify it with:

```
python3 tools/archive_r_front_ready_followup.py --verify \
  fpga/mister/reports/2026-10-23-r-front-flags/experiment
```

Next close the actual ContA l17 -> d17 producer and its distinct clock/data
dependencies, then continue the remaining ContB and MemC producers and
capture-stage integration. Preserve late LOW controls and original epochs.
Native original-ROM startup, 60 ns clocks/timers, physical SDRAM crossings/IO,
Cedar, persistence, fresh boots and sustained stress remain open. No assembly,
MiSTer load, pack change or hardware speed qualification occurred here.
