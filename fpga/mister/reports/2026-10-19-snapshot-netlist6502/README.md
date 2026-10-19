# Complete netlist6502 state: matched routed qualification

**PASS: simulation/classification and corrected matched50MHz fits.** The
simulation and fresh XML evidence are in the companion
`2026-10-19-snapshot-netlist6502-simulation` report.

All four timing conditions pass with **+0.151ns minimum snapshot slack**,
no illegal/unconstrained clocks or I/O and no additional timing exceptions.
Eight damaged-evidence controls reject. Source/provenance/QSF/SDC and the
independently tested 96-word schema are checked. The original vendor's
CRLF bytes, combinational netlist and relaxation process remain unchanged.

The synthesized enabled CPU adapter retains **1,746 registers: all1,725node
bits plus18response and3handshake bits**. Its ordinary original counterpart
has767registers after constant/alias pruning. Both have zero block RAM.
These are synthesis entity counts, distinct from fitted totals. Matched
shell Fitter totals are443ALMs/912registers baseline and1,649ALMs/1,990registers
snapshot. Counts include staging/capture/admin logic and physical duplication;
they do not budget a complete BaseBoard or globally enabled core snapshot.

The initial fit is deliberately retained as **REJECTED**: scalar frozen,
done,error ports were incorrectly targeted with vector wildcard virtual-pin
assignments, causing critical169085 for three unassigned physical pins.
The corrected fixture uses scalar virtual-pin declarations, with identical
CPU adapter, data paths, clock and timing budget. No warning whitelist or
constraint relaxation was used to pass. The fresh v2builder is terminal;
its independent read-only auditor passes all conditions and state counts.

Both original and corrected project sources, provenance, raw reports and
failure/review logs are archived. Every compiled source hash except the
Quartus-managed QSF revision line was read back and verified; the QSF's
complete normalized assignments are checked by the auditor. Raw source,
logs and reports are losslessly gzip compressed.

Next: other BaseBoard primitives/RIOT, complete board classification and
restore tests, then machine-wide barriers/DDR staging/ARM and matching disk
state. No FPGA CPU-state restore, global machine snapshot or speed gain is
claimed by this isolated result.
