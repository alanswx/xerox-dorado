# MC10135 state adapter — simulation and coverage

The original dual J-K cell, independent oracle, and state adapter agree across
92,195 host edges and all four independent PRECAP modes. All128 saved state
words and first-edge restore pass, with20,000 subsequent randomized cycles.
Seven intentionally broken RTL variants fail. Three altered original-source
controls and eight altered XML/schema controls reject. Fresh XML accounts
for all seven mutable bits: qa/qb, four J/K setup bits, shared clock history.

Matched negative-edge launch/positive-edge capture50MHz fits now pass all four
conditions,min+0.164ns, no cuts/unconstrained paths. Both variants use identical
cell RTL/SDC,10ns into cell/20ns output paths and2ns max0ns min virtual I/O.
Resources68->73ALMs/38->74registers/zeroRAM include shell/admin/pruning. Five
altered-fit evidence controls reject. Earlier positive-edge shell failed
virtual-input hold at-0.465ns; multi-corner option alone did not repair it.
Both failed fits and executed scripts are preserved separately; no waiver.

This is primitive qualification; no IFU integration, global barrier, physical
snapshot or speed claim. Production cells/running FPGA remain unchanged.

Final executed sources and raw logs/XML are byte-preserved as gzip files.
Source hashes in simulation/results.json match the current sources.
