# Complete R sign/odd component

PASS: 596 original state bits, 170 packages, 1,748 original pins and 33 clocks.
Both simulators agree for 23,672 events in each of five arrival profiles; seven
deliberately broken implementations reject. Every new l06 bit takes both values;
493 observations change the new latch state. Each architectural active-low flag
includes all three original complemented outputs, including l06's two halves.

The isolated 15 ns registered fixture passes four operating conditions:
setup +4.242 ns, hold +0.135 ns, minimum pulse width +6.547 ns; targeted evaluator
setup minimum +4.469 ns. Resources: 1,321 ALMs / 2,199 registers / no block RAM.
All five recorded processes exited before exact input collection. Eight damaged
or rehashed evidence controls reject, including a separately negative evaluator
path. The prior 592-bit component still passes the updated reader.

851 source/report members are losslessly archived and read back. Extracting the
archive and rerunning the independent positive timing review gives the same
accepted result. No second full simulation replay after extraction is claimed.
The first fixture compile failed because its driver included the new module
twice; that driver and log are retained in `history/`. Fixing its dependency
path did not change the candidate RTL.

This is a coupled component experiment, not native clock integration or a 60 ns
operating result. Functional RM/STK ports and bypass/IFU/Ready/MIR/Hold producers
remain boundaries. Its state is an evaluator interface, not an FPGA snapshot.
No native hardware or Main/server change came from this component.

Commands from the repository root:

```
python3 tools/test_r_flags.py --output /tmp/r-flags-check --mutations
python3 tools/fit_r_flags.py --prepare /tmp/r-flags-fit
# On misterubuntu only: python3 fit_r_flags.py --run .
python3 tools/audit_control_clock_fit.py FIT --qualified QUALIFIED --output review.json
python3 tools/test_control_clock_fit_reader.py FIT --qualified QUALIFIED --output CONTROLS
```

`qualified/executed-inputs/` freezes the complete simulation input tree;
`timing/` freezes the fit inputs, raw reports and terminal process/readback proof.
`review-tools/` carries the reader, collector, controls and archive utility.
