# Complete-MSA shared FF clock experiment: rejected for area

Matched enabled components retain all 458 fields / 2,690 physical bits, all
original normal pins/equations, eight helper bodies and the original raw memory
policy. The dedicated ALTCLKCTRL candidate uses **2,222 ALMs** versus **2,213**
for the existing host-clock/enable implementation. Registers after fit are
4,598/4,684; this is not an area improvement. Candidate is not adopted.

Both use the same 50 MHz registered shell, 991 virtual pins, 2 ns max/0 ns min
IO budgets, device and area policy. All four device conditions pass setup,
hold and minimum pulse width with minimum +0.157 ns, zero unconstrained clock/IO
paths and no recovery/removal paths in this synchronous fixture. Actual fitter
reports show CLKCTRL_G6 serving 866 FF loads. Controllers/frontends stay on the
host clock. No period relaxation, added cuts, retiming, assembly or deployment.

Gate enable is advance_r OR the scan request, supplied BEFORE the falling edge.
The separately latched actual_advance cannot feed a gate sampling that same edge.
The source/policy review checks this independently and rejects five damaged
phase/constraint/timing cases, with damaged metadata rehashed where relevant.
Six source controls check inverse byte identity and unchanged physical schemas.

Simulation-v3 passes 2,507,166 host edges / 32,840,341 checks / 40,002 guest
advances, 55,632 raw requests, 8,672 writes, 904 state operations and 2,421,007
scan edges. All original physical fields, normal outputs and raw traffic agree.
Full inversion/restoration, first resumed operation and 20,000 further guest
advances pass. Other-board scan clocks leave a locally held board unchanged.
Four actual bad RTL variants reject, including an unsafe AND clock. Separate
falling-edge and transparent-low model units each run 5,000 source cycles with
40,000 high-half request changes and three damaged gate controls. Late low-half
changes distinguish the two models. These are contract models, NOT vendor-netlist
equivalence or a qualified native gate/barrier.

Fresh XML accounts for 2,690 bits per side, 83 board-private bits, one REQUIRED
clock-control phase bit and 33 fixture enable/monitor bits: 5,497 sequential bits,
none excluded. The three additional writer edge relationships are checked.
Seventeen damaged inventory/phase/exact-alias cases reject.

History is retained: v1/v2 use a low-transparent latch contract; v3 supplies raw
intent before the falling edge and uses the sampled guest flag. The early unit
version lacks the late-low check. The exploratory XML differs from authoritative
classification-v1. The first Quartus map failed from omitted combinational
sources; the corrected isolated matched run includes all five and is terminal.
Reports and historical executed sources are preserved without modification.

All 692 raw members pass readback. The extracted generator reproduces board and
eight helpers byte for byte; extracted original-DRAM rebuild, source controls,
XML/alias controls, both clock units and matched-fit review pass. tools contains
the local Python dependency closure and original cell bytes. Generation/source
review also use the already committed immutable snapshot-msa-ff, snapshot-msa-board
and snapshot-msa-transport receipt archives; replay records this dependency.
Objects, databases and executables are excluded. Sources/reports/logs are lossless
gzip; JSON over 100 KiB is gzip. No hardware or measured speed change.
