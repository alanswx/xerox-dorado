# Current operand forwarding and complete-core integration

The physical forwarding export and both operand-capture inputs agree with the
original reference. The complete 15 ns component fit still fails. A private
57.142857 MHz/SYSPER=4 complete-core timing experiment is now running; no new
FPGA image has been assembled or loaded, and no speed improvement is qualified.

`forwarding/summary.json` contains the reviewed results. The content-addressed
archive contains 5,417 members in 818 gzip blobs (3,436,822 compressed bytes),
verified by lossless readback. It preserves tested RTL, original references,
all profile comparisons, rejected controls, timing reports, native staging and
structural warning reports. Review code is `tools/review_r_front_forward_view.py`.

## Physical source and capture checks

The existing BEFORE cone now exports current `RbBypass`, through the two
adapters, instead of exporting the forecast through next state. The independent
forecast assertion remains. Additional assertions compare current forwarding
both before and after owner commit. There is no extra state, cone, RAM, guest
cycle, or removed functional observation.

Both the preceding current-SRAM matrix and this forwarding matrix are terminal
passes: six established profiles in Icarus and Verilator, 12 cases each. Each
forwarding case compares all 844 physical Qs, 9,216 RAM bits and 100 clocks over
11,300 events, 238 writes, 4,368 reads and 392 epochs. Every clock has a positive
edge count. The inverted-current control fails at check 0 in both simulators.

Two more checks compare the current export against both actual operand
adapters' RbBypass inputs, before and after commit, with byte-identical positive
RTL. Both simulators pass the complete comparison. Inverting the ProcH input
fails at check 0 in both. The original state/clock comparisons still include
the captured l07 Qs; this is not merely an export-to-export check.

Original Sil establishes ProcL l10's MC1660 driving C32/C33, the l08/l09 input
connections on both processor boards, l07 B's data/clock, and `RbBypassDly`
selecting g06/g07. The read-latch and l07 clocks come from complementary
SE10212 outputs. Preserve the first source-audit failure: it incorrectly
expected i05 to be SE10210; both boards actually use SE10212. These topology
checks do not establish physical setup/hold or future native phase timing.

## Timing remains a failure

The unchanged registered fixture retains all 1,400 result fields and the 15 ns
SDC. All four operating conditions were checked, with exact source readback
apart from Quartus's version annotation. The new fit uses 4,028 ALMs, 3,324
registers, 9,216 MLAB bits and no M10K bits, with no unconstrained paths.

Minimum setup is **-1.372 ns**, hold **+0.134 ns**, minimum pulse width
**+6.113 ns**. This regresses from the preceding current-port fit's -1.122 ns.
The worst path ends at `unsettled`; another failure ends at
`shared_state_mismatch`. Neither output has a synthesizable consumer in the
frozen native backplane; declarations and evaluator connections were checked.

A separate read-only TimeQuest query preserves that complete failing result
and examines all keeper destinations. It partitions 8,679 keepers into 8,675
retained endpoints and exactly four diagnostic keepers: the register and port
for result bits 94 and 355. All 2,322 RM/STK memory keepers remain included.
Even this query **fails setup by 1.167 ns**, at address-forecast result bit 310.
Hold remains +0.134 ns. No source, SDC or timing exception changed; this is
not a timing waiver or a claim that functional timing passes. The first local
review parser expected the wrong TimeQuest success banner; the corrected
parser reads the same completed reports without rerunning or altering the fit.

## Complete native experiment

The new staging tool separates the 80-bit closed-latch encoding from physical
Q. Existing board ports receive physical Q; the central owner alone stores
closed values. All sixteen original visible-R latch owners remain. Native
pin history, advance gating, storage barrier, clocks and SYSPER remain unchanged.
See HM section 2.3/Table 4 for the ordinary cadence target; this experiment
does not implement or qualify that 60 ns target.

An input projection derives masks from every literal reference in the two
current SRAM address/select functions. The functions consume only their
original pointer/address Qs and pins 172..176; no open operand-Q bits feed
these addresses. Both simulators pass the complete original-reference
comparison after that projection. The projection adds no state or capture.

Three complete-core structural elaborations pass with warnings visible:

- v1 integrates physical Q and the tested forwarding component.
- v2 adds the checked SRAM input projection.
- v3 adds inverse-checked Verilator split comments only. They do **not**
  remove the new packed-view dependency warning, so v3 is not selected.

All three retain the pre-existing disk ECC combinational warning and media
buffer multiple-clock warning, plus the new operand/SRAM packed-view warning.
The projection therefore proves functional equivalence, not dependency closure.

The frozen **v2** project is being measured with the existing sequential native
builder, including four operating conditions, SDRAM and crossing reports:

```
/home/alans/mister/r-front-control844-physical-windows-native-v2-20261023
builder PID 3276353; completed map PID 3276356; live fit PID 3290995
tool session 12095
```

Mapping completed with zero errors. Fitting remains in progress; mapping
success does not settle warning or timing qualification. Revalidate these
handles and `run-state.json` before acting. Never restart a
live build on a polling timeout. The builder has no assembly stage. Native
consumer arrival/phase correctness, actual firmware boot, physical memory
timing and speed remain open even if the fit passes.

The original queued storage simulation ended normally at its unchanged 5B-host
bound (`HOST_LIMIT`), without reaching the Cedar113 window; this is not a boot
pass. Its control run and two cold-prefix experiments remain separate. Do not
extend or restart them merely because an observation or original bound ended.
Hardware remains qualified 5c65, with the last pinned 4.025473M StartCycle
issues/s measurement unchanged. The full A–F goal remains active.
