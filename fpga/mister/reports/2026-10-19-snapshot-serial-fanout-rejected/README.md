# Serial66Fanout: rejected timing experiment

The complete nineteen-board core fits at 41,360 / 41,910 ALMs (550 free),
52,926 registers, 329 RAM blocks and 2,498,471 memory bits. All 659 prepared
inputs remain unchanged; all 2,761 original pin/parameter connections and the
board-only schema af0b647cb43a3a62 are preserved. Builder 2489421, fitter
2491688 and all project processes are absent. No assembly or hardware changes.

The target MAX_FANOUT=64 policy is applied, inserting four cells and duplicating
network_step. Actual timing gets worse than Serial65: setup -1.067 ns at slow
100C and -1.029 ns at slow -40C. All reported hold checks pass (minimum +0.117 ns),
as does SDRAM IO. Actual clocks and inherited framework IO exceptions match
Native59; zero illegal/unconstrained clocks, three input/fifty output exceptions.
This is not complete physical framework IO coverage.

All 304 nineteen-board crossing reports are checked. Seventeen setup groups
fail: eight MSA output groups at both slow conditions and MemX input at 100C.
Worst paths now run from the MSA frontend into duplicated network_step registers.
No clock period, timing exception, RTL or schema changes were made. REJECTED.

All 1,665 archive members read back losslessly. Extracted source/capacity/timing/
crossing readers and their six/four/six/three negative controls replay, using
prior immutable readiness tools for dependencies. The known passing Native59
baseline supplies timing-reader controls. No real pause, complete snapshot or
speed improvement is claimed.
