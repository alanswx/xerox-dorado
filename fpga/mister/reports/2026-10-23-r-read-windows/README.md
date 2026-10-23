# Closed R-input storage and live latch views

The new integration passes both complete balanced component comparisons.
Its isolated 15 ns fit **still fails setup by 3.557 ns**, improved from
4.184 ns. No FPGA image was assembled or loaded, and no hardware speed
increase is claimed. The original 60 ns CPU cadence remains a delivery goal.

[Reviewed results](windows/summary.json) and the content-addressed receipt
in `windows/manifest.json.gz` preserve sources, independent references,
testbenches, commands, failures and Quartus readback. Receipt verification
read back all 11,270 members / 713 unique blobs losslessly (2,864,839 compressed
bytes). `tools/review_r_read_windows.py` recreates the review from local runs.

## What changed

The original dRm and dR inputs are LOW-transparent MC10173 latches. Their Q
follows D throughout the open window and retains the final D at closure.
Storing each open update in a host-clocked register creates an unnecessary
path through the newly resolved controls and SRAM read to that register.

The experiment keeps the same 32 storage bits as the last closed values,
exposes open Q combinationally, and captures the actual pre-close data when
the gate closes. Each existing RM/STK bank has one current asynchronous read
address and an independent synchronous write address. There is no second
read port, memory copy, architectural reset or added guest cycle.

The complete original comparison still checks all 844 physical Qs, 9,216 RAM
bits, 100 clock channels and functional ports. The physical-Q comparison now
runs after owner commit: an open latch's physical Q must not be confused with
its closed storage encoding. Both simulators pass **11,300 events, 238 writes,
4,368 reads and 392 port epochs**. Omitting closing capture makes both reject
event 15 on actual dR bits, confirming those bits remain checked.

The separate 48-Q ownership prototype compares twelve independent original
MC10173 models over 6,145 comparisons, including 1,024 same-edge read-close /
visible-open handoffs, late updates, selector changes and held edges. Its
visible-R owner is not installed in native RTL. The native sixteen visible-R
bits remain solely owned by the original latch packages.

## Other completed checks

- The task-clock mask passes all 14 baseline/expanded cases in both engines
  over the six established profiles. Every expanded clock channel has edges.
- Actual LOW-root NoDispatch reset and original MemC Hold 0/1 pass 266 events
  in both engines over four control profiles. Freezing the reset view makes
  both reject event 261: original clock 12 rises and the prediction misses it.
- The 32-bit helper passes 2,304 independent-latch comparisons per engine.
  Ten wrong-latch unit controls reject their intended mismatches.
- A source audit binds CPAddr/CPStrb to the original BaseBoard i62 RIOT,
  ownership muxes and TTL-to-ECL translators. Firmware writes the address,
  then toggles the strobe using INC/DEC. This supplies no universal remote-CP
  setup/hold guarantee and justifies no timing exception.

Initial failures are preserved: an assertion width error, initial/task NBA
scheduling disagreement, and two Quartus parser failures. The corrected
fixture uses real clocked NBAs. The Quartus-only separate-genvar declaration
is inverse-checked against the tested helper and independently run in both
simulators. The final full balanced current-source comparisons also pass.

## Physical result and next work

| Experiment | Worst setup slack | Worst data delay |
| --- | ---: | ---: |
| Coherent control capture | −8.512 ns | 23.170 ns |
| Task-clock masking | −4.184 ns | 18.863 ns |
| Current read port / closed input storage | **−3.557 ns** | **17.941 ns** |

The final fit uses 3,999 ALMs, 3,363 total shell registers and 9,216 MLAB bits
(no M10K blocks). Minimum hold is +0.145 ns; pulse width is +6.110 ns. All four
conditions were reviewed, with unchanged 15 ns constraints and zero
unconstrained paths. Exact input readback allows only Quartus's version
annotation; the builder and all four stage PIDs had exited before collection.

The new worst path is launch 835 (state 29) to result 339 (`faults[0]`):
control/MIR capture → task clock → joint Switchb Q41 → the child's task Q72
→ T input/visible latch → parity fault, 21 logic levels. Next inspect this
serial controller/task computation and the real capture epochs; retain every
functional observation and the timing budget. A new register or timing waiver
would not establish the required original cadence.

The broader read-window matrix continues at
`build/rtl-executive/r-front-read-windows-matrix-v2-20261023`, driver 63204
(tool session 67889). At handoff, all six Verilator profiles and three Icarus
profiles passed; verify the current process/results before acting. Its helper
and two driver tools remain frozen. The first matrix attempt stopped because
the sandbox denied a read-only process-liveness probe; the simulations were
left running and reused by the authorized second driver.

Native integration must distinguish current live D/gates from historical
`r_front_before` and upcoming capture inputs, and export live Q rather than
the closed encoding. Its new packed-view Verilator dependency warning also
needs a source-correct partition before native acceptance. Remaining gates
include the original early RBase overlap, actual native producers and ROM
boot, 60 ns phases/timers, physical 66.7 MHz SDRAM, storage/disk/network work
and the full A–F acceptance plan. Hardware 5c65/Native59, Main/daemon and packs
remain unchanged; the last pinned hardware result remains 4.025473 million
StartCycle issues/s (24.15% of nominal, not retired instructions).
