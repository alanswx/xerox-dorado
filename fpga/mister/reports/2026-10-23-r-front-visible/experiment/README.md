# Existing read-latch boundary improves timing; 15 ns still rejected

The original read and visible-R latches have complementary windows. In both
processor boards, the wire lists and single-driver checks establish:

```
Clock1'Bb = ppClk'a | pClock1En   (g05/h05 read-data latch gates)
PreSHCP'B = ~pClock1En           (g06/g07 visible-R latch gates)
```

The MC10173 is LOW-transparent. When visible R is open, the read-data,
write-data and bypass-select latches are closed. Replacing two visible-R
next-state mux expressions with their existing stored read-latch values is
therefore binary-equivalent for all data and controls. No architectural
state, capture edge, RAM copy or pipeline cycle is added. The original
MC10182 stack-adder cells remain; this is separate from the rejected adder
experiment.

Evidence classes remain explicit:

- Source-bound symbolic proof: correct complementary windows UNSAT for any
  differing byte; two overlapping-window controls SAT. Both boards' clock
  wiring, part identities, fanout drivers and unwired LOW inputs are checked.
- Two new complete graph comparisons, Icarus and Verilator: 13515 events,
  328 write-active events, 5608 read-active events and 797 old-parity epochs
  per run. All 792 Qs, 9216 RAM bits, 68 clocks and functional ports compare,
  including real LOW-root NoDispatch, CP, old-MIR and write-window checks.
- Twelve previous profile/engine runs and two negative clock cases are
  inherited through the binary combinational proof. They were not rerun.
  This does not prove analog skew or full-machine next-state equivalence.

Only compiled `dorado_local_r_operand_step.sv` changes. The top, 23 other RTL
files, combinational cone, constraints and ordered project sources are
byte-identical to the previous MIR-mask fit. All parity/fault endpoints remain.

| Condition | Setup / evaluator setup | Hold | Minimum pulse width |
| --- | ---: | ---: | ---: |
| Slow 1100mV 100C | -0.134 ns | +0.387 ns | +6.468 ns |
| Slow 1100mV -40C | -0.382 ns | +0.360 ns | +6.460 ns |
| Fast 1100mV 100C | +4.210 / +6.706 ns | +0.172 ns | +6.181 ns |
| Fast 1100mV -40C | +4.625 / +7.980 ns | +0.155 ns | +6.117 ns |

Worst setup improves from -1.996 to -0.382 ns, a 1.614 ns margin gain in
this controlled source comparison. Timing remains **REJECTED**. Resources:
3345 ALMs, 3020 registers, exactly four uninitialized 256x9 MLAB banks with
asynchronous reads. No M10K blocks, DSPs, PLLs, exceptions or changed budget.

Worst remaining path, 16 levels / 15.066 ns at slow -40C:

```
launch[607] = before_controls[289] = CP bus bit 7
 -> prior CP-clock detection / CP Q -> normal0 / WIM / MIR selection
 -> low-address preview -> actual rm_h read -> rm_flags_n[0] = result[229]
```

At slow 100C, controller Q6 reaches stored R read-latch bit 563 through
MIR selection, the original stack-address carry and the actual stk_h read:
17 levels / 14.821 ns. Next investigate the remaining CP/MIR/address/read
and flag paths, retaining LOW-window maintenance and old/new capture epochs.
The current shortfall cannot be waived merely because it is small.

All four stages returned zero in
`/home/alans/mister/Dorado_RVisible_1_20261023`; all five process IDs
1077474/1077486/1079859/1091677/1093399 are absent. Terminal source readback
permits only the Quartus version annotation. Twenty malformed/rehashed timing
controls and six damaged equivalence receipts reject; the previous stack
reader's 20 controls reproduce. The source audit's initial j02 Ba/Bb typo
failed before SMT and is preserved; v3 corrects only the audit.

This lossless receipt stores 4284 members in 421 blobs, 2,442,029 compressed
bytes. Verify with:

```
python3 tools/archive_r_front_ready_followup.py --verify \
  fpga/mister/reports/2026-10-23-r-front-visible/experiment
```

No native instantiation, assembly or MiSTer load occurred. Hardware 5c65 and
its measured speed are unchanged. Remaining real producers, original 60 ns
native timing/timers, SDRAM crossings/IO, Cedar comparisons, persistence,
fresh boots and sustained stress remain open.
