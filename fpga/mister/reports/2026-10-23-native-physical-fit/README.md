# Full native physical-operand fit: rejected

The complete core fits the device, but fails setup at the existing
57.142857 MHz/SYSPER=4 clock. Minimum setup is **-4.630 ns**; this is a real
machine path, not an unused consistency export. No image was assembled or
loaded and there is no qualified speed improvement. Hardware remains 5c65.

`native/summary.json` is the reviewed result. The content-addressed archive
preserves 2,728 members in 996 gzip blobs (9,163,146 compressed bytes), verified
by lossless readback. It includes the 949-member raw native collection, all
compiled source/configuration, original-reference tests, rejected controls,
full timing reports and private v4/v5 staging. `collection.json.gz` preserves
the raw collection's member checksums. Review tool:
`tools/review_r_front_physical_native.py`.

## Complete-core result

All nine sequential build/report stages and the separate read-only central
audit completed successfully. Collection verified every recorded process had
exited and every compiled input matched its manifest. No source or SDC was
changed after launch. The flow has no assembly stage.

| Operating condition | Setup | Hold | Minimum pulse width |
| --- | ---: | ---: | ---: |
| Slow 100 C | -4.630 ns | +0.139 ns | +1.122 ns |
| Slow -40 C | -4.408 ns | +0.168 ns | +1.122 ns |
| Fast 100 C | +3.047 ns | +0.131 ns | +1.122 ns |
| Fast -40 C | +3.974 ns | +0.117 ns | +1.122 ns |

Resource use is 35,371/41,910 ALMs (84%), 54,657 registers, 2,506,566 block
memory bits, 332 RAM blocks and 35 DSP blocks. The separately queried actual
central paths also fail: slow-100 internal -3.608 ns, incoming -4.630 ns and
outgoing -3.607 ns. Nothing is excluded to turn this into a pass.

The audit covers 84,126 keeper endpoints, 779 central registers (688 primary
owner bits and 91 fitter duplicates), 61 history registers and all sixteen
original visible-R owners. Physical SRAM keeper counts are RM-H 576, STK-H
584, RM-L 576 and STK-L 585. The lower physical-register count is synthesis
retention, not a change to the 844-Q source/state comparison.

There are no illegal or unconstrained clocks. The native host shell does have
unconstrained external ports: 3 input ports/13 paths and 50 output ports/154
paths in each condition. Inputs are HDMI_I2C_SDA, IO_SDA and SDCD_SPDIF;
outputs include HDMI/audio pins. Do not quote the isolated fixture's zero-UCP
result as a complete-core result or treat all-device timing as qualified.

All four RM/STK banks are inferred 256x9 LUTRAM with unregistered data outputs.
RM read addresses are UNREGISTERED; STK read addresses are OUTCLOCK. Preserve
this difference for review: Quartus has incorporated address registers into
the STK inference. The source is an asynchronous array read, but this report
does not prove either harmless register absorption or an extra guest cycle.
Audit the folded address enables/data correspondence before promotion.

## Actual critical path

At slow 100 C the 17.500 ns path takes 21.433 ns of data delay across 24 logic
levels, with -0.537 ns clock skew. It starts at MemC c15.qa, original net
`Aad.4b`, traverses MemC d12's memory/compare logic and the internal wired-OR
boundary `MemC15.sil+2` (component pin 386), then the control resolver/MIR,
operand predecode, pointer next state and l17 stack-address sum. It ends at
`r_front_upper_q[30]`, global state bit 618: ProcL g08, an MC10176 stack-address
register. A companion worst path ends at STK-H's incorporated read-address
register. Original Sil and the physical inventory establish these identities.

The next change therefore targets actual address capture data/epochs. It does
not relax the clock, gate a fault, omit an observer, or change Main/daemon.

## Bounded experiments and next fit

The control-input projection uses the stored owner directly for control bits
that are provably unchanged by the 80-bit open-latch projection. Both the
initial projection and the complete 156-bit control projection pass all 844
physical Qs, 9,216 RAM bits, 100 clocks and functional outputs in both simulators
over 11,300 events/238 writes/4,368 reads/392 epochs. Native v4 elaborates, but
the packed-view warning remains, with a shorter reported dependency path.
This does not close source/phase/dependency qualification.

The targeted address experiment changes only the six stack-address data pins
166..171: use the existing pre_view during LOW root as well as HIGH; all other
consumers retain their resolved LOW-window inputs. The complete comparison
passes in BOTH engines. A zero-data control fails check 2169 in BOTH, on an
actual address mismatch. The independent oracle and all rejection checks stay
unchanged. Original producer/capture setup/hold still requires review.

Native v5 includes that six-pin change and the checked input projections. Its
only RTL differences from v2 are the control-owner aliases and six-pin address
view; all SDC files are byte-identical. Structural native elaboration passes
with the existing media-buffer multiple-clock and disk ECC warnings plus the
remaining operand/SRAM packed-view warning. No warning is suppressed.

Two jobs are now running separately:

```
Mac: build/rtl-executive/r-front-address-epoch-matrix-v1-20261023
     driver PID 88027 / tool 55945; 7/12 passes at receipt
     freeze probe_r_front_address_epoch.py, test_r_front_address_matrix.py,
            test_r_front_control_inputs.py, test_r_front_memory_inputs.py

Linux: /home/alans/mister/r-front-control844-physical-windows-native-v5-20261023
       builder PID 3412626 / completed map 3412629 / live fit 3427287 / tool 93495
       read-only follower PID 3413068 / tool 36299
```

Revalidate these handles and state before acting. Observe the same jobs; do
not restart them on a polling timeout. The full v2 failed project remains
available. Native v5 is an experiment with broader profiles and full timing
still pending, not a selected faster core. Next: finish those gates, inspect
the measured path change and STK register mapping, then native ROM/Cedar/input,
original 60 ns phases/timers, faster SDRAM and full A-F acceptance. Full goal
ACTIVE; the last pinned 4.025473M StartCycle issues/s is unchanged.
