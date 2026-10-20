# Serial69: real native memory pause fits; timing rejects it

The complete nineteen-board design fits at 41,468 / 41,910 ALMs (442 free),
53,121 registers, 329 RAM blocks, 2,498,471 block-memory bits, 35 DSPs and 3 PLLs.
All 660 prepared inputs are unchanged at terminal; builder2566540 and every
project child are absent. No assembly/RBF/SOF/deployment occurred.

Setup fails at -0.435 ns (slow100C) and -0.204 ns (slow-40C). Hold passes all
four conditions, minimum +0.076 ns. SDRAM IO passes; actual clocks and inherited
framework exceptions match Native59 (three input/fifty output exceptions,
zero illegal/unconstrained clocks). This is not full framework IO coverage.
All 304 crossing reports are retained; fourteen groups fail setup.

The worst path is MSA2Odd storage15 write_pending through aggregate readiness
to falling-edge network_step (10 ns). Other reported setup failures include
network_step distribution to MemX RAM write enable and img_size/pack readiness
to network_step. Fanout64, a global-signal assignment and seed2 previously failed;
this result does not justify relaxing periods or hiding paths.

The memory pause is the qualified eight-MSA late freeze on the actual arbiter/
SDRAM path. Other service ACKs remain virtual and complete state access is
unfinished. Source inverse review checks 659 parents/660 actual inputs and 2,761
original pins with ten damaged controls. Fresh actual-QSF XML retains all
2,094,301 required bits, including 120 task-written SDRAM bits, with fifteen
controls and no sequential exclusions. The board-only schema remains
af0b647cb43a3a62; top emu/UIO, external raw RAM and ARM state remain separate work.

The capacity reader explicitly supports this new source/classification scope,
with eight damaged controls. Earlier Serial65/68 reader regressions still pass.
The rejected actual timing uses six reader controls against the known-passing
Native59 baseline; three crossing controls use a passing actual fast report.

All 2,207 raw archive members (220,647,123 bytes; 16,065,112 stored) read back
losslessly. Extracted frozen tools replay inverse source, fresh XML, capacity,
timing rejection and crossing reviews. Restaging reproduces all 660 input hashes
at the original destination without working HDL. Immutable parent receipt
dependencies remain in the referenced committed report directories.

No complete machine snapshot, restored Cedar desktop, faster clocks or new
hardware speed measurement is claimed. Native59 remains the qualified core.
