# Joint ContA/ContB/MemC integration: incomplete arrival contract

**The required matrix remains failed. This integration is not promoted to
maintained RTL, a native fit, or hardware.** Generated candidate
`b79903a970ad7e4bd4ec79020f3d43f542dc9a71b412b21a2f4f39365528323e`
combines156 original Qs,56 clocks and94 selected flags. The prior maintained
798-Q processor and46-Q memory/ContB components are unchanged.

## What is connected

The original110-Q ContA graph now connects to40 ContB and six MemC Qs through
the original registered enable copies, MemSH, StartCycle'a, BNTGtCT'a, gated
UseDMD, ASEL/BSEL, StopMIRClk, shared wired-OR rMIRa and common CP bus. The
reference uses the original BaseBd distribution and actual package/pin gates.
Twenty-four inherited functions are byte-identical apart from CB name prefixes;
34 original pin connections and the distinct physical register mappings audit.

MemC l02 must capture old MemSH: its clock comes directly from CLK.mc', while
the new CA MemSH value passes through CA l01 and k03 clock-to-Q. UseDMD is
l17.Q0 AND SetRun, rather than just the stored l17 bit. These relationships
remain explicit in the candidate and wrong-epoch/source controls.

## Bounded comparisons

Six complete comparisons pass: both engines for balanced, fast-gate and the
Control-A component's established later-FF profile. Each compares5,678 events,
all156 Qs,56 clock counts and94 flags. Every clock fires at least eight times;
all engine counts agree. The cases exercise independent registered copies,
actual FF=UseDMD and its SetRun gate, all three maintenance selector codes,
shared reset/CP transactions, parity-stop capture and a LOW NoDispatch capture
with real memory-produced Hold and a pending higher task.

Seven wrong crossings/epochs reject in both engines: wrong CLKEnable copy,
wrong StartCycle copy, new rather than old MemSH, wrong ASEL.0 copy, ungated
UseDMD, missing local CB reset contribution and an unshared CP bus.

## Required failure: RunRefresh release

Both engines fail the required `later_control` stress profile at event403:
gate delay1750ps, ordinary FF400ps, phase FF1250ps. The original l03.Q0/Q1
(global14/15) become0 while the candidate retains1. Actual MemClkEnable'a
therefore rises and clocks MemC l02's B half (channel51); the candidate misses
that edge. All other physical Qs agree at that first failing observation.

The old state is `300000426f4000a2a82a0000000080f6030dc46`. It contains
RunRefresh=1, WantRunRfsh=0 and a halted processor. A minimal replay changes
only the root clock; it reproduces the failure in both engines. Holding only
WantRunRfsh asserted removes the mismatch. Changing only phase-FF delay from
1250ps to2000ps also removes it. The full original failed matrix is preserved.

The original source route explains the ordering sensitivity:

- CA l06.B samples WantRunRfsh and drives RunRefresh directly onto both
  asynchronous Set pins of l03.
- l06's common clock comes from CA l01's free-running fanout.
- l03's clock comes from the parallel l01 output through an additional f13
  clock-distribution gate.
- In the tested snapshot, release reaches l03 after the phase-FF delay, while
  its capture clock arrives after f13's gate delay. Releasing Set first permits
  D=0 to capture; retaining Set through that clock holds the outputs at1.

This is a limit in the inherited controller capture function, not a newly
invented cross-board alias. The next implementation must establish the proper
release/capture relationship; the failing profile has not been dropped or
reclassified as a passing gate.

The 1978 Motorola MECL book, printed pages3-123/3-126 (PDF pages170/173),
tables MC10210 propagation at1.0–2.5ns and MC10231 clock propagation at
1.5–3.3ns at25C. An additional causal probe using1.75ns gates,2ns ordinary FFs
and1.5ns phase FFs still exposes the mismatch. Those clock-part values fit
the Motorola table intervals; this does not characterize Signetics parts,
all other board devices, Set recovery, pulse widths or route skew. Source
hashes and limitations are preserved in the source-notes member of the receipt.
The Hardware Manual's memory initialization Clocks paragraph (p60) specifies
RunRefresh's function but does not settle this internal capture order.

## Open raw-parity handoff

Both engines also reject a simultaneous raw parity-fault clear/source rise at
event4070, with26 extra observed MIR clock edges: six ContA and20 ContB.
Instruction data is stable in this test. The ordinary passing cases restore
the root HIGH before clearing that external fault. Actual IM/parity production
must be connected before claiming unrestricted arrivals. These illustrative
delays do not prove a physical board pulse of adequate width.

## Testbench fixes and receipt

Earlier fixture failures exposed missing setup time between fixture data and
load, rather than an invalid independent-register state. Explicit setup time
and a neutral settling load correct the fixture while retaining the requested
independent copies. The LOW NoDispatch probe also needed a real captured Hold
and a pending higher task; source NoDispatch alone does not close MIR. All
failed predecessors remain preserved.

The independent read-only review reports `INCOMPLETE_JOINT_CONTROL_HOLD`:
six bounded passes, two required profile failures,14 wrong variants, two raw
parity gaps and five causal probes. There are58 terminal compile/test actions
in that reviewed set. Additional earlier pilots/diagnostics are archived too.
`summary.json` states the incomplete result; `manifest.json.gz` maps7,851 exact
source/log members to460 gzip blobs (1,954,783 compressed bytes). Lossless
readback verifies every blob and retains the original failed aggregation.

Reproduce the gate in a fresh output directory (it is expected to report the
unresolved required arrival failures):

```sh
python3 tools/qualify_control_hold.py --output build/rtl-executive/control-hold-new-review
```

After the RunRefresh capture issue, remaining work includes actual MCR DisHold,
reference/Md/miscellaneous hold-data, IM/parity/DMD and B-bus producers, full
processor/capture-phase integration, original-ROM/native60ns operation, physical
66.7MHz SDRAM, timers and all Cedar/rollback/persistence/input/fresh-boot/stress
acceptance. The full goal stays active. There is no hardware speed gain here.

The native simulations were not restarted. Their second observer ended its
own four-hour bound; a new read-only observer follows the same processes.
