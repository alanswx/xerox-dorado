# Serial68Seed2: legal capacity, rejected setup and hold

Only Serial65's placement seed changes 1 ->2. Actual fitter settings confirm
2 applied, default1. All 659 prepared inputs remain unchanged; 2,761 original
pins/parameters and schema af0b647cb43a3a62 are preserved. Builder2532081,
fitter2534171 and every project process are absent. No assembly or hardware.

The nineteen-board core fits 41,395 /41,910 ALMs (515 free), 52,780 registers,
329 RAM blocks, 2,498,471 memory bits, 35 DSPs and 3 PLLs. REJECTED timing:
setup -0.287 ns slow100C /-0.245 ns slow-40C; hold fails at all four conditions,
worst -1.758 ns. SDRAM input/output checks pass. Clocks and inherited three
input/fifty output framework exceptions match Native59, zero illegal or
unconstrained clocks. This is not complete physical framework IO coverage.

All 304 adapter crossing reports are checked; ten MSA output setup groups fail.
Worst setup includes u_media.cyl -> network_step; worst hold is the virtual
CAPACITY_SERVICES_HELD input -> UIO capture. No period, IO budget or exception
is relaxed. Six source/five capacity-and-actual-seed/six known-passing-baseline
timing/three crossing-reader controls reject. All 1,670 raw members read back;
extracted readers and controls replay with prior immutable tool dependencies.
Serial65 is still the better candidate. Real services, full state capture,
full-core timing and FPGA qualification remain required.
