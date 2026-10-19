# Enabled18-bit snapshot RAM: RTL and isolated fit PASS

The actual ContB audit proves eight18-bit half-banks, including parity. This
prototype preserves an original half-bank's RAM/read behavior using the host
clock and an explicit guest_advance equal to its original gated-clock enable.
Ordinary backend holds suppress both guest reads and writes. Frozen scans
borrow the RAM ports while a saved raw read output remains visible until the
first actual resumed guest advance. The system-wide hold is not implemented.

Against18 unchanged F10415A cells and an independent array,57,417 host edges,
20,225 backend-held edges and5,125 state operations pass. Coverage includes
all RAM/read-pipeline bits, pre-CE export, old-before-write collisions,
destructive continuation, repeated pipeline writes, recapture before the first
resumed guest edge, and30,000 random running steps. Five damaged RTL variants
fail for their intended reasons. No production source or FPGA is changed.

Quartus17.0.2 on5CSEBA6U23I7 infers one1024x18 simple-dual-port memory with
OLD_DATA mixed-port behavior. Both variants retain18,432 RAM bits/twoM10Ks.
With matched opposite-edge launch/capture registers,50MHz,2ns/0ns virtual
I/O budgets and10ns internal budgets, all12 setup/hold/pulse checks per
variant pass across four operating conditions; snapshot minimum+0.255ns.
Recovery/removal have no asynchronous paths. Illegal/unconstrained clocks
and I/O paths are zero. Six damaged-source/report/constraint controls reject.

Measured with the register shell:53->102 ALMs and48->138 registers. Counts
include the fixture; baseline admin inputs/constant outputs can be pruned,
so the delta49ALMs/90registers is NOT a pure bank overhead or a whole-core
projection. Actual ContB integration and routed core timing remain necessary.

Rejected evidence is preserved in rejected-experiments.tar.gz. Two dual-clock,
two-write-port templates pass RTL tests but Quartus reports multiple memory
drivers. The first enabled-bank fixture maps correctly but its directly driven
virtual-input baseline fails hold; ALL_PATHS hold optimization does not fix
that fixture. The final fixture uses actual launch/capture registers in BOTH
variants, retaining I/O constraints and imposing half-cycle internal paths.
No negative slack is waived and no timing exception is added.

The target's common-clock mixed-port old-data mode is documented in the
[Cyclone V memory handbook](https://docs.altera.com/r/docs/683375/current/cyclone-v-device-handbook-volume-1-device-interfaces-and-integration/embedded-memory-features?contentId=W70RQm9AY4r7x0G9SRTukA).
The fitted parameters and original-cell oracle establish this prototype's
specific behavior. Full board state, global disk/network/input barriers,
external SDRAM, atomic bundles and physical Cedar restore remain unfinished.

Commands: python3 fpga/mister/snapshot/test_f10415_enable.py --mutations;
on misterubuntu run fit_f10415_enable.py --output NEW_DIRECTORY; audit the
collected directory with audit_f10415_enable.py DIRECTORY --negative-controls.
